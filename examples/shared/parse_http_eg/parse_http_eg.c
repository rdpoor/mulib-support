/**
 * MIT License
 *
 * Copyright (c) 2020 R. D. Poor <rdpoor@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file parse_http_eg.c
 *
 * This example shows a few of mulib's in-place string and array manipulation
 * functions.  To make it interesting, it parses key/value pairs from an HTTP
 * header into individual key and value strings and prints them.  It then sorts
 * the collection by the keys and prints the results.
 *
 * What is significant about this demo is that string data is never moved: only
 * references pointing to sub-strings within the overall string are manipulated.
 * In addition to being efficient, it makes it easy to write string manipulation
 * code without dynamic allocation (i.e. malloc() and free()).
 */

// =============================================================================
// Includes

#include "parse_http_eg.h"
#include "mulib/core/mu_array.h"
#include "mulib/core/mu_strbuf.h"
#include "mulib/core/mu_str.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// =============================================================================
// Local types and definitions

// A struct to hold an HTTP header key / value pair.
typedef struct {
  mu_str_t key;
  mu_str_t value;
} kv_pair_t;

#define MAX_CSTR_LENGTH 100
#define MAX_KV_PAIRS 20

// =============================================================================
// Local storage

static const char *s_request_header =
    "GET / HTTP/1.1\r\n"
    "Host: example.org\r\n"
    "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:88.0) Gecko/20100101 Firefox/88.0\r\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
    "Accept-Language: en-US,en;q=0.5\r\n"
    "Accept-Encoding: gzip, deflate\r\n"
    "DNT: 1\r\n"
    "Connection: keep-alive\r\n"
    "Upgrade-Insecure-Requests: 1\r\n"
    "\r\n";

static const char *s_response_header =
    "HTTP/1.1 200 OK\r\n"
    "Content-Encoding: gzip\r\n"
    "Accept-Ranges: bytes\r\n"
    "Age: 48784\r\n"
    "Cache-Control: max-age=604800\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n"
    "Date: Sun, 23 May 2021 14:57:44 GMT\r\n"
    "Etag: \"3147526947\"\r\n"
    "Expires: Sun, 30 May 2021 14:57:44 GMT\r\n"
    "Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT\r\n"
    "Server: ECS (oxr/830C)\r\n"
    "Vary: Accept-Encoding\r\n"
    "X-Cache: HIT\r\n"
    "Content-Length: 648\r\n"
    "\r\n";

// temp buffer for printing c-style strings
static char s_cstr_buf[MAX_CSTR_LENGTH];

// provide storage for the key-value pairs
static kv_pair_t s_kv_pairs[MAX_KV_PAIRS];

// =============================================================================
// Local (forward) declarations

/**
 * @brief Extract and print the contents of an HTTP header.
 */
static void process_http_header(const char *msg, const char *http_hdr);

/**
 * @brief Extract the first line of an HTTP header.
 *
 * @param reader A mu_str object that contains the HTTP header.  Its internal
 *        state is modified by reference.
 * @param first_line A mu_str object to receive the first line.  Its internal
 *        state is modified by reference.
 * @return true if the header line is well formed, false otherwise.
 */
static bool extract_first_line(mu_str_t *reader, mu_str_t *first_line);

/**
 * @brief Print the contents of the first line of the HTTP header.
 */
static void print_first_line(mu_str_t *first_line);

/**
 * @brief Parse key/value pairs from an HTTP header
 *
 * @param reader A mu_str object that contains the HTTP header.  Its internal
 *        state is modified by reference.
 * @param kv_pairs An array of kv_pair objects.  Each kv_pair will be filled in
 *         with key / value pairs.
 * @param max_count The length of the kv_pairs array.
 * @return The number of kv pairs actually read.
 */
static int extract_kvs(mu_str_t *reader, kv_pair_t *kv_pairs, int max_count);

/**
 * @brief Parse a key/value pair from an HTTP header
 *
 * @param reader A mu_str object that contains the HTTP header.  Its internal
 *        state is modified by reference.
 * @param kv_pair A kv_pair object.  It will bewill be filled in with a key and
 *        value from the HTTP header.
 * @return true if reader contained a valid key value pair.
 */
static bool extract_kv(mu_str_t *reader, kv_pair_t *kv_pair);

/**
 * @brief Skip over consecutive \r and \n characters.
 *
 * @param reader A mu_str object that contains the HTTP header.  Its internal
 *        state is modified by reference.
 */
static void skip_eol(mu_str_t *reader);

/**
 * @brief Skip over consecutive \t and <space> characters.
 *
 * @param reader A mu_str object that contains the HTTP header.  Its internal
 *        state is modified by reference.
 */
static void skip_whitespace(mu_str_t *reader);

/**
 * @brief Print a key / value pair.
 */
static void print_kv_pair(kv_pair_t *pair);

/**
 * @brief Sort the array of key-value pairs alphabetically by key.
 */
static void sort_kv_pairs(kv_pair_t *pairs, int count);

/**
 * @brief Sorting predicate for kv pairs.
 *
 * Return a negative value if the key for e1 is less than that of e2, a positive
 * value if the key for e1 is greater than that of e2, and 0 if they are equal.
 */
static int kv_sort_fn(void *e1, void *e2);

// =============================================================================
// Public code

void parse_http_eg_init(void) {
  process_http_header("HTTP Request", s_request_header);
  process_http_header("HTTP Response", s_response_header);
}

void parse_http_eg_step(void) {
  // nothing to do past initialization
}

// =============================================================================
// Local (static) code

static void process_http_header(const char *msg, const char *http_hdr) {
  mu_strbuf_t header;
  mu_str_t reader;
  mu_str_t first_line;

  printf("========== %s:\n", msg);

  // Initialize the mu_strbuf and the reader mu_str objects.
  mu_strbuf_init_from_cstr(&header, http_hdr);
  mu_str_init_for_read(&reader, &header);

  // Extract and print the first line of the http header.
  extract_first_line(&reader, &first_line);
  print_first_line(&first_line);

  // Extract and store each HTTP key / value header pair.
  int found = extract_kvs(&reader, s_kv_pairs, MAX_KV_PAIRS);
  printf("Found %d header line%s:\n", found, found == 1 ? "" : "s");

  // Print the HTTP key / value pairs.
  printf("===== Before sorting:\n");
  for (int i=0; i<found; i++) {
    print_kv_pair(&s_kv_pairs[i]);
  }

  // Sort and print the HTTP key / value pairs.
  printf("===== After sorting:\n");
  sort_kv_pairs(s_kv_pairs, found);
  for (int i=0; i<found; i++) {
    print_kv_pair(&s_kv_pairs[i]);
  }
}

static bool extract_first_line(mu_str_t *reader, mu_str_t *first_line) {
  int index = mu_str_index(reader, '\r');
  if (index < 0) {
    // figure out a better error handler
    return false;
  }
  // Capture everything up to (but not including) the \r in first_line and
  // advance reader
  mu_str_slice(first_line, reader, 0, index);
  mu_str_read_increment(reader, index);
  return true;
}

static void print_first_line(mu_str_t *first_line) {
  int desired = mu_str_read_available(first_line);
  int copied = mu_str_to_cstr(first_line, s_cstr_buf, MAX_CSTR_LENGTH);
  printf("%s%s\n", s_cstr_buf, desired>copied ? "..." : "");
}

static int extract_kvs(mu_str_t *reader, kv_pair_t *kv_pairs, int max_count) {
  int i;

  for (i=0; i<max_count; i++) {
    if (!extract_kv(reader, &s_kv_pairs[i])) {
      // Didn't find a kv pair - quit now.
      break;
    }
  }
  // return the number of kv pairs found
  return i;
}

static bool extract_kv(mu_str_t *reader, kv_pair_t *kv_pair) {
  int index;

  skip_eol(reader);                     // skip \r\n chars
  index = mu_str_index(reader, ':');    // find : separator
  if (index < 0) {
    return false;                       // didn't find ':'
  }
  mu_str_slice(&kv_pair->key, reader, 0, index);
  mu_str_read_increment(reader, index+1); // +1 to skip : itself

  skip_whitespace(reader);
  index = mu_str_index(reader, '\r');   // find '\r' termination
  if (index < 0) {
    return false;                       // probably malformed
  }
  mu_str_slice(&kv_pair->value, reader, 0, index);
  mu_str_read_increment(reader, index);

  return true;
}

static void skip_eol(mu_str_t *reader) {
  while (true) {
    char ch = *mu_str_read_ref(reader);
    if ((ch == '\r') || (ch == '\n')) {
      mu_str_read_increment(reader, 1);
    } else {
      break;
    }
  }
}

static void skip_whitespace(mu_str_t *reader) {
  while (true) {
    char ch = *mu_str_read_ref(reader);
    if ((ch == ' ') || (ch == '\t')) {
      mu_str_read_increment(reader, 1);
    } else {
      break;
    }
  }
}

static void print_kv_pair(kv_pair_t *pair) {
  int desired, copied;

  desired = mu_str_read_available(&pair->key);
  copied = mu_str_to_cstr(&pair->key, s_cstr_buf, MAX_CSTR_LENGTH);
  printf("%s%s => ", s_cstr_buf, desired>copied ? "..." : "");

  desired = mu_str_read_available(&pair->value);
  copied = mu_str_to_cstr(&pair->value, s_cstr_buf, MAX_CSTR_LENGTH);
  printf("%s%s\n", s_cstr_buf, desired>copied ? "..." : "");
}

static void sort_kv_pairs(kv_pair_t *pairs, int count) {
  mu_array_sort(pairs, count, kv_sort_fn, sizeof(kv_pair_t));
}

static int kv_sort_fn(void *e1, void *e2) {
  kv_pair_t *kv1 = (kv_pair_t *)e1;
  kv_pair_t *kv2 = (kv_pair_t *)e2;
  mu_str_t *key1 = &kv1->key;
  mu_str_t *key2 = &kv2->key;
  size_t len1 = mu_str_read_available(key1);
  size_t len2 = mu_str_read_available(key2);
  int cmp = strncmp((const char *)mu_str_read_ref(key1),
                    (const char *)mu_str_read_ref(key2),
                    len1 < len2 ? len1 : len2);
  return cmp;
}

#if 0

/*
cc -Wall -g -o parse_http_app -I../../../ parse_http_eg.c ../../../mulib/core/mu_array.c ../../../mulib/core/mu_strbuf.c ../../../mulib/core/mu_str.c
rm -rf parse_http_app parse_http_eg.dSYM
 */

int main(void) {
  parse_http_eg_init();
  return 0;
}

#endif

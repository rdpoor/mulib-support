/**
MIT License

Copyright (c) 2019 R. Dunbar Poor <rdpoor@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// =============================================================================
// includes

#include "../src/jemi.h"
#include "test_utilities.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// =============================================================================
// private types and definitions

#define JSON_BUF_LENGTH 400

// =============================================================================
// private declarations

// define our own memory management routines
static void memmgmt_stats();
static void memmgmt_init();
static void *memmgmt_alloc(size_t nbytes);
static void memmgmt_free(void *obj);

static int verify_json(jemi_element_t *e,
                       const char *expected,
                       int start,
                       int end);

// static void visit_test(jemi_element_t *element, int depth);

// =============================================================================
// local storage

char json_buf[JSON_BUF_LENGTH];

// =============================================================================
// public code

void jemi_test() {

  jemi_element_t *e1;

  // print state of memory manager before tests
  memmgmt_init();
  memmgmt_stats();
  jemi_init(memmgmt_alloc, memmgmt_free);

  e1 = jemi_alloc_array();
  UTEST_ASSERT(e1 != NULL);
  UTEST_ASSERT(e1->type == JEMI_ARRAY);
  verify_json(e1, "[]", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_dict();
  UTEST_ASSERT(e1 != NULL);
  UTEST_ASSERT(e1->type == JEMI_DICT);
  verify_json(e1, "{}", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_string("dorado");
  UTEST_ASSERT(e1 != NULL);
  UTEST_ASSERT(e1->type == JEMI_STRING);
  verify_json(e1, "\"dorado\"", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_null();
  UTEST_ASSERT(e1 != NULL);
  UTEST_ASSERT(e1->type == JEMI_NULL);
  verify_json(e1, "\"null\"", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_true();
  UTEST_ASSERT(e1 != NULL);
  UTEST_ASSERT(e1->type == JEMI_TRUE);
  verify_json(e1, "\"true\"", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_false();
  UTEST_ASSERT(e1 != NULL);
  UTEST_ASSERT(e1->type == JEMI_FALSE);
  verify_json(e1, "\"false\"", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_float(2.5);
  UTEST_ASSERT(e1 != NULL);
  UTEST_ASSERT(e1->type == JEMI_FLOAT);
  verify_json(e1, "2.500000", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_uint32(1);
  UTEST_ASSERT(e1 != NULL);
  UTEST_ASSERT(e1->type == JEMI_UINT32);
  verify_json(e1, "1", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_int32(-1);
  UTEST_ASSERT(e1 != NULL);
  UTEST_ASSERT(e1->type == JEMI_INT32);
  verify_json(e1, "-1", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_double(0.25);
  UTEST_ASSERT(e1 != NULL);
  UTEST_ASSERT(e1->type == JEMI_DOUBLE);
  verify_json(e1, "0.250000", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_uint64(8589934592);  // 1<<33
  UTEST_ASSERT(e1 != NULL);
  UTEST_ASSERT(e1->type == JEMI_UINT64);
  verify_json(e1, "8589934592", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_int64(-8589934592);
  UTEST_ASSERT(e1 != NULL);
  UTEST_ASSERT(e1->type == JEMI_INT64);
  verify_json(e1, "-8589934592", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_array();
  jemi_array_push(e1, jemi_alloc_string("red"), NULL);
  jemi_array_push(e1, jemi_alloc_string("blu"), NULL);
  verify_json(e1, "[\"blu\",\"red\"]", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_array();
  jemi_array_append(e1, jemi_alloc_string("red"), NULL);
  jemi_array_append(e1, jemi_alloc_string("blu"), NULL);
  verify_json(e1, "[\"red\",\"blu\"]", 0, JSON_BUF_LENGTH);
  jemi_free(e1);

  e1 = jemi_alloc_dict();
  jemi_dict_add(e1, "red", jemi_alloc_uint32(1), NULL);
  verify_json(e1, "{\"red\":1}", 0, JSON_BUF_LENGTH);
  jemi_dict_add(e1, "blu", jemi_alloc_uint32(2), NULL);
  verify_json(e1, "{\"red\":1,\"blu\":2}", 0, JSON_BUF_LENGTH);
  jemi_dict_add(e1, "red", jemi_alloc_uint32(3), NULL);
  verify_json(e1, "{\"red\":3,\"blu\":2}", 0, JSON_BUF_LENGTH);
  UTEST_ASSERT(((jemi_container_t *)e1)->children->type == JEMI_MEMBER);
  jemi_free(e1);

  e1 = jemi_dict_add(jemi_alloc_dict(),
    "macAddress", jemi_alloc_string("F8:F0:05:E5:EA:B6"),
    "timestamp", jemi_alloc_uint64(1564591532122),
    "events", jemi_alloc_array(),
    "samples", jemi_array_append(jemi_alloc_array(),
      jemi_dict_add(jemi_alloc_dict(),
        "index", jemi_alloc_uint32(0),
        "time", jemi_alloc_uint64(1564591532123),
        NULL),
      jemi_dict_add(jemi_alloc_dict(),
        "index", jemi_alloc_uint32(1),
        "time", jemi_alloc_uint64(1564591532124),
        NULL),
      NULL),
    "metrics", jemi_array_append(jemi_alloc_array(),
      jemi_dict_add(jemi_alloc_dict(),
        "timestamp", jemi_alloc_uint64(1564591532125),
        NULL),
      jemi_dict_add(jemi_alloc_dict(),
        "timestamp", jemi_alloc_uint64(1564591532126),
        NULL),
      NULL),
    NULL);
  verify_json(e1,
              "{\"macAddress\":\"F8:F0:05:E5:EA:B6\",\"timestamp\":1564591532122,\"events\":[],\"samples\":[{\"index\":0,\"time\":1564591532123},{\"index\":1,\"time\":1564591532124}],\"metrics\":[{\"timestamp\":1564591532125},{\"timestamp\":1564591532126}]}",
              0,
              JSON_BUF_LENGTH);

  // see how much we've allocated
  memmgmt_stats();

  jemi_free(e1);

  // should equal same as when the test started
  memmgmt_stats();
}

// =============================================================================
// private code

// #ifdef UNIX
static void memmgmt_stats() {
  // in malloc we trust...
}

static void memmgmt_init() {
  // no action required
}

static void *memmgmt_alloc(size_t nbytes) {
  return calloc(1, nbytes);
}

static void memmgmt_free(void *obj) {
  free(obj);
}
// #endif

static int verify_json(jemi_element_t *e,
                       const char *expected,
                       int start,
                       int end) {
  memset(json_buf, 0, JSON_BUF_LENGTH);
  int s = jemi_render(json_buf, start, end, e);
  if (strcmp(json_buf, expected) != 0) {
    printf("\r\n=====");
    printf("\r\nexpected: |%s|", expected);
    printf("\r\n     got: |%s|", json_buf);
  }
  UTEST_ASSERT(s == strlen(expected));
  UTEST_ASSERT(strcmp(json_buf, expected) == 0);
  return s;
}

// static void visit_test(jemi_element_t *element, int depth) {
//   int depths[] = {0, 1, 2, 2, 1, 1, 1};
//   jemi_type_t types[] = {JEMI_TYPE_ARRAY,
//                          JEMI_TYPE_DICT,
//                          JEMI_TYPE_MEMBER,
//                          JEMI_TYPE_MEMBER,
//                          JEMI_TYPE_UINT32,
//                          JEMI_TYPE_UINT32,
//                          JEMI_TYPE_UINT32};
//   UTEST_ASSERT(element->type == types[s_visit_count]);
//   UTEST_ASSERT(depth == depths[s_visit_count]);
//   s_visit_count++;
// }

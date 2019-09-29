/**
 * MIT License
 *
 * Copyright (c) 2019 R. Dunbar Poor <rdpoor@gmail.com>
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

#ifndef MULIB_JEMI_H_
#define MULIB_JEMI_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

// =============================================================================
// types and definitions

#ifndef JEMI_MAX_STRING_LENGTH
#define JEMI_MAX_STRING_LENGTH 100
#endif

typedef enum {
  JEMI_ARRAY,    // [ element0, element1, ... ]
  JEMI_DICT,     // { key0:element0, key1:element1, ...}
  JEMI_MEMBER,   // key:element pair
  JEMI_STRING,
  JEMI_NULL,     // "null"
  JEMI_TRUE,     // "true"
  JEMI_FALSE,    // "false"
  JEMI_FLOAT,
  JEMI_UINT32,
  JEMI_INT32,
  JEMI_DOUBLE,
  JEMI_UINT64,
  JEMI_INT64,
} jemi_type_t;

// common to all types
typedef struct jemi_element {
  struct jemi_element *next;
  jemi_type_t type;
} jemi_element_t;

// contains arrays and dicts
typedef struct {
  jemi_element_t header;
  jemi_element_t *children;
} jemi_container_t;

// member (i.e. key:value pair).
typedef struct {
  jemi_element_t header;
  const char *key;       // key must be const
  jemi_element_t *value;
} jemi_member_t;

// strings
typedef struct {
  jemi_element_t header;
  const char *value;
} jemi_string_t;

// true, false, null
typedef struct {
  jemi_element_t header;
} jemi_literal_t;

// any 32 bit value
typedef struct {
  jemi_element_t header;
  union {
    float float_value;
    uint32_t uint32_value;
    int32_t int32_value;
  };
} jemi_value32_t;

typedef struct {
  jemi_element_t header;
  union {
    double double_value;
    uint64_t uint64_value;
    int64_t int64_value;
  };
} jemi_value64_t;


/**
 * @brief: prototype for alloc function.  Function should return a pointer to
 * zeroe'd memory, large enough to hold nbytes of data.  If allocation fails,
 * the function should return NULL.
 */
typedef void *(*jemi_alloc_fn)(size_t nbytes);

/**
 * @brief: prototype for free fn.  Function should return object to free memory.
 * obj may be NULL, in which case it is ignored.
 */
typedef void (*jemi_free_fn)(void *obj);

// =============================================================================
// declarations

void jemi_init(jemi_alloc_fn, jemi_free_fn);

jemi_element_t *jemi_alloc_array();
jemi_element_t *jemi_alloc_dict();
jemi_element_t *jemi_alloc_string(char *string);
jemi_element_t *jemi_alloc_null();
jemi_element_t *jemi_alloc_true();
jemi_element_t *jemi_alloc_false();
jemi_element_t *jemi_alloc_float(float value);
jemi_element_t *jemi_alloc_uint32(uint32_t value);
jemi_element_t *jemi_alloc_int32(int32_t value);
jemi_element_t *jemi_alloc_double(double value);
jemi_element_t *jemi_alloc_uint64(uint64_t value);
jemi_element_t *jemi_alloc_int64(int64_t value);

jemi_element_t *jemi_array_push(jemi_element_t *array, ...);
jemi_element_t *jemi_array_append(jemi_element_t *array, ...);

// add (or upate) pairs of const char *key, jemi_element_t *value
jemi_element_t *jemi_dict_add(jemi_element_t *dict, ...);

void jemi_free(jemi_element_t *element);
jemi_type_t jemi_type(jemi_element_t *element);
int jemi_render(char *buf, int s, int e, jemi_element_t *element);

#ifdef __cplusplus
}
#endif

#endif /* MULIB_JEMI_H_ */

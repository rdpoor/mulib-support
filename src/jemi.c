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

// =============================================================================
// includes

#include "jemi.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>


// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static jemi_element_t *jemi_alloc(size_t size, jemi_type_t type);
static char *jemi_strdup(char *string);

static jemi_element_t *array_vpush(jemi_container_t *c, va_list ap);
static jemi_element_t *array_push_one(jemi_container_t *c, jemi_element_t *e);
static jemi_element_t *array_vappend(jemi_container_t *c, va_list ap);
static jemi_element_t *array_append_one(jemi_container_t *c, jemi_element_t *e);
static jemi_element_t *dict_vadd(jemi_container_t *c, va_list ap);
static jemi_element_t *dict_add_one(jemi_container_t *dict,
                                    const char *key,
                                    jemi_element_t *value);

static int jemi_render_aux(char *buf, int i, int s, int e, jemi_element_t *element);
static int jemi_render_string(char *buf, int i, int s, int e, const char *string);
static int jemi_render_container(char *buf, int i, int s, int e, jemi_element_t *container);
static int jemi_render_printf(char *buf, int i, int s, int e, const char *fmt, ...);

// =============================================================================
// local storage

jemi_alloc_fn s_alloc_fn;
jemi_free_fn s_free_fn;

// =============================================================================
// public code

void jemi_init(jemi_alloc_fn alloc_fn, jemi_free_fn free_fn) {
  s_alloc_fn = alloc_fn;
  s_free_fn = free_fn;
}

jemi_element_t *jemi_alloc_array() {
  return jemi_alloc(sizeof(jemi_container_t), JEMI_ARRAY);
}

jemi_element_t *jemi_alloc_dict() {
  return jemi_alloc(sizeof(jemi_container_t), JEMI_DICT);
}

jemi_element_t *jemi_alloc_string(char *string) {
  jemi_element_t *element = jemi_alloc(sizeof(jemi_string_t), JEMI_STRING);
  if (element) {
    jemi_string_t *s = (jemi_string_t *)element;
    s->value = jemi_strdup(string);
  }
  return element;
}

jemi_element_t *jemi_alloc_null() {
  return jemi_alloc(sizeof(jemi_literal_t), JEMI_NULL);
}

jemi_element_t *jemi_alloc_true() {
  return jemi_alloc(sizeof(jemi_literal_t), JEMI_TRUE);
}

jemi_element_t *jemi_alloc_false() {
  return jemi_alloc(sizeof(jemi_literal_t), JEMI_FALSE);
}

jemi_element_t *jemi_alloc_float(float value) {
  jemi_element_t *element = jemi_alloc(sizeof(jemi_value32_t), JEMI_FLOAT);
  if (element) {
    ((jemi_value32_t *)element)->float_value = value;
  }
  return element;
}

jemi_element_t *jemi_alloc_uint32(uint32_t value) {
  jemi_element_t *element = jemi_alloc(sizeof(jemi_value32_t), JEMI_UINT32);
  if (element) {
    ((jemi_value32_t *)element)->uint32_value = value;
  }
  return element;
}

jemi_element_t *jemi_alloc_int32(int32_t value) {
  jemi_element_t *element = jemi_alloc(sizeof(jemi_value32_t), JEMI_INT32);
  if (element) {
    ((jemi_value32_t *)element)->int32_value = value;
  }
  return element;
}

jemi_element_t *jemi_alloc_double(double value) {
  jemi_element_t *element = jemi_alloc(sizeof(jemi_value64_t), JEMI_DOUBLE);
  if (element) {
    ((jemi_value64_t *)element)->double_value = value;
  }
  return element;
}

jemi_element_t *jemi_alloc_uint64(uint64_t value) {
  jemi_element_t *element = jemi_alloc(sizeof(jemi_value64_t), JEMI_UINT64);
  if (element) {
    ((jemi_value64_t *)element)->uint64_value = value;
  }
  return element;
}

jemi_element_t *jemi_alloc_int64(int64_t value) {
  jemi_element_t *element = jemi_alloc(sizeof(jemi_value64_t), JEMI_INT64);
  if (element) {
    ((jemi_value64_t *)element)->int64_value = value;
  }
  return element;
}

jemi_element_t *jemi_array_push(jemi_element_t *array, ...) {
  va_list ap;

  va_start(ap, array);
  array_vpush((jemi_container_t *)array, ap);
  va_end(ap);
  return array;
}

jemi_element_t *jemi_array_append(jemi_element_t *array, ...) {
  va_list ap;

  va_start(ap, array);
  array_vappend((jemi_container_t *)array, ap);
  va_end(ap);
  return array;
}

// add (or upate) pairs of const char *key, jemi_element_t *value
jemi_element_t *jemi_dict_add(jemi_element_t *dict, ...) {
  va_list ap;

  va_start(ap, dict);
  dict_vadd((jemi_container_t *)dict, ap);
  va_end(ap);
  return dict;
}

void jemi_free(jemi_element_t *element) {
  if (element == NULL) return;

  switch(element->type) {
    case JEMI_ARRAY:
    case JEMI_DICT:
      jemi_free(((jemi_container_t *)element)->children);
      break;
    case JEMI_MEMBER:
      jemi_free(((jemi_member_t *)element)->value);
      break;
    case JEMI_STRING:
      free((void *)(((jemi_string_t *)element)->value));
      break;
    case JEMI_NULL:
    case JEMI_TRUE:
    case JEMI_FALSE:
    case JEMI_FLOAT:
    case JEMI_DOUBLE:
    case JEMI_INT32:
    case JEMI_INT64:
    case JEMI_UINT32:
    case JEMI_UINT64:
      break;
  }
  jemi_element_t *next = element->next;
  // free the element itself
  s_free_fn(element);
  // if the element is part of a list, free siblings
  if (next) {
    jemi_free(next);
  }
}

jemi_type_t jemi_type(jemi_element_t *element) {
  // TODO: provide JEMU_UNKNOWN_TYPE and handle null element
  return element->type;
}

int jemi_render(char *buf, int s, int e, jemi_element_t *element) {
  return jemi_render_aux(buf, 0, s, e, element);
}

// =============================================================================
// private code

static jemi_element_t *jemi_alloc(size_t size, jemi_type_t type) {
  jemi_element_t *element = (jemi_element_t *)s_alloc_fn(size);
  if (element) {
    element->type = type;
  }
  return element;
}

static char *jemi_strdup(char *string) {
  size_t len = strlen(string);
  char *dst = malloc(len+1);  // include room for null terminator
  if (dst) {
    strcpy(dst, string);
  }
  return dst;
}

static jemi_element_t *array_vpush(jemi_container_t *c, va_list ap) {
  jemi_element_t *e;
  while ((e = va_arg(ap, jemi_element_t *)) != NULL) {
    array_push_one(c, e);
  }
  return (jemi_element_t *)c;
}

static jemi_element_t *array_push_one(jemi_container_t *c, jemi_element_t *e) {
  if (c) {
    e->next = c->children;
    c->children = e;
  }
  return (jemi_element_t *)c;
}

static jemi_element_t *array_vappend(jemi_container_t *c, va_list ap) {
  jemi_element_t *e;
  while ((e = va_arg(ap, jemi_element_t *)) != NULL) {
    // potential optimization here, since container_append_element() walks to
    // the end of the list each time.
    array_append_one(c, e);
  }
  return (jemi_element_t *)c;
}

static jemi_element_t *array_append_one(jemi_container_t *c, jemi_element_t *e) {
  if (c) {
    jemi_element_t *prev = NULL;
    jemi_element_t *curr = c->children;
    // walk to end of list
    while (curr != NULL) {
      prev = curr;
      curr = curr->next;
    }
    if (prev) {
      // push onto end of list
      prev->next = e;
    } else {
      // list was empty: e is new head.
      c->children = e;
    }
  }
  return (jemi_element_t *)c;
}

static jemi_element_t *dict_vadd(jemi_container_t *c, va_list ap) {
  const char *key;
  jemi_element_t *e;

  while ((key = va_arg(ap, const char *)) != NULL) {
    e = va_arg(ap, jemi_element_t *);
    // assert(e != NULL);
    if (dict_add_one(c, key, e) == NULL) return NULL;
  }
  return (jemi_element_t *)c;
}

// if key is present in the dict, replace its value.  Otherwise add a new
// key:value pair.
static jemi_element_t *dict_add_one(jemi_container_t *c,
                                    const char *key,
                                    jemi_element_t *value) {
  if (!c) return NULL;

  jemi_element_t *prev = NULL;
  jemi_element_t *curr = c->children;
  while (curr) {
    // assert(e->type == JEMI_MEMBER);
    if (strcmp(key, ((jemi_member_t *)curr)->key) == 0) break;
    prev = curr;
    curr = curr->next;
  }

  if (curr) {
    // curr non-null: key matches curr->key.  Release the previous value.
    // (We will set curr->key below).
    jemi_free(((jemi_member_t *)curr)->value);  // free old value

  } else {
    // key was not in the list.  create a new member...
    curr = jemi_alloc(sizeof(jemi_member_t), JEMI_MEMBER);
    if (!curr) return NULL; // alloc failed
    ((jemi_member_t *)curr)->key = key;  // assume key is const, no copy needed
    if (prev) {
      // append at end of container list
      prev->next = curr;
    } else {
      // adding first member to container
      c->children = curr;
    }
  }
  // finally, assign value to member
  ((jemi_member_t *)curr)->value = value;

  return (jemi_element_t *)c;
}

static int jemi_render_aux(char *buf, int i, int s, int e, jemi_element_t *element) {
  if (element == NULL) return i;

  switch(element->type) {
    case JEMI_ARRAY:
      i = jemi_render_string(buf, i, s, e, "[");
      i = jemi_render_container(buf, i, s, e, element);
      i = jemi_render_string(buf, i, s, e, "]");
      break;

    case JEMI_DICT:
      i = jemi_render_string(buf, i, s, e, "{");
      i = jemi_render_container(buf, i, s, e, element);
      i = jemi_render_string(buf, i, s, e, "}");
      break;

    case JEMI_MEMBER:
      i = jemi_render_printf(buf, i, s, e, "\"%s\":",
                             ((jemi_member_t *)element)->key);
      i = jemi_render_aux(buf, i, s, e, ((jemi_member_t *)element)->value);
      break;

    case JEMI_STRING:
      i = jemi_render_printf(buf, i, s, e, "\"%s\"", ((jemi_string_t *)element)->value);
      break;

    case JEMI_NULL:
      i = jemi_render_string(buf, i, s, e, "\"null\"");
      break;

    case JEMI_TRUE:
      i = jemi_render_string(buf, i, s, e, "\"true\"");
      break;

    case JEMI_FALSE:
      i = jemi_render_string(buf, i, s, e, "\"false\"");
      break;

    case JEMI_FLOAT:
      i = jemi_render_printf(buf, i, s, e, "%f",
                             ((jemi_value32_t *)element)->float_value);
      break;

    case JEMI_UINT32:
      i = jemi_render_printf(buf, i, s, e, "%u",
                             ((jemi_value32_t *)element)->uint32_value);
      break;

    case JEMI_INT32:
      i = jemi_render_printf(buf, i, s, e, "%d",
                             ((jemi_value32_t *)element)->int32_value);
      break;

    case JEMI_DOUBLE:
      i = jemi_render_printf(buf, i, s, e, "%f",
                             ((jemi_value64_t *)element)->double_value);
      break;

    case JEMI_UINT64:
      i = jemi_render_printf(buf, i, s, e, "%llu",
                             ((jemi_value64_t *)element)->uint64_value);
      break;

    case JEMI_INT64:
      i = jemi_render_printf(buf, i, s, e, "%lld",
                             ((jemi_value64_t *)element)->int64_value);
      break;

  }
  return i;
}

/**
 * Copy characters from string to buf[i-s], stopping at a null character or when
 * i == e, whichever comes first.  Returns updated i.
 */
static int jemi_render_string(char *buf, int i, int s, int e, const char *string) {
  while ((*string != 0) && (i < e)) {
    char ch = *string++;
    if (i >= s) buf[i-s] = ch;
    i += 1;
  }
  return i;
}

static int jemi_render_container(char *buf,
                                 int i,
                                 int s,
                                 int e,
                                 jemi_element_t *container) {
  bool is_first_element = true;
  jemi_element_t *element = ((jemi_container_t *)container)->children;
  while (element) {
    // print a comma separator before each element except the first
    if (is_first_element) {
      is_first_element = false;
    } else {
      i = jemi_render_string(buf, i, s, e, ",");
    }
    i = jemi_render_aux(buf, i, s, e, element);
    element = element->next;
  }
  return i;
}

static int jemi_render_printf(char *buf,
                              int i,
                              int s,
                              int e,
                              const char *fmt,
                              ...) {
  va_list ap;
  int n;
  char src[100];  // assume that's enough...

  memset(src, 0, 100);
  va_start(ap, fmt);
  n = vsnprintf(src, 100, fmt, ap);
  va_end(ap);

  if (n < 0) return 0;
  return jemi_render_string(buf, i, s, e, src);
}

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

 #include "mu_test_utils.h"
 #include "mu_buf.h"
 #include <string.h>
 #include <stdlib.h>
 #include <stdint.h>

 #include <stdio.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

#define ELEMENT_COUNT 10

typedef struct {
  uint8_t id;
  const char *name;
} inventory_t;

// =============================================================================
// local storage

static inventory_t s_inventory[] = {
  {.id = 1, .name="cat"},
  {.id = 2, .name="emu"},
  {.id = 3, .name="eel"},
};

// =============================================================================
// public code

void mu_buf_test() {
  mu_buf_t bi;
  mu_buf_t *b = &bi;
  const char *s1 = "the quick brown fox jumps over the lazy dog.";
  char s2[ELEMENT_COUNT];
  uint8_t d8;
  uint16_t d16;
  uint32_t d32;
  inventory_t dinv;

  ASSERT(mu_buf_init(b, NULL, true, sizeof(char), sizeof(s1) / sizeof(char)) == MU_BUF_ERR_ILLEGAL_ARG);
  ASSERT(mu_buf_init(b, (char *)s1, true, MU_BUF_MAX_ELEMENT_SIZE, sizeof(s1) / sizeof(char)) == MU_BUF_ERR_ILLEGAL_ARG);
  ASSERT(mu_buf_init(b, (char *)s1, true, sizeof(char), MU_BUF_MAX_CAPACITY) == MU_BUF_ERR_ILLEGAL_ARG);

  ASSERT(mu_buf_init(b, (char *)s1, true, sizeof(char), sizeof(s1) / sizeof(char)) == MU_BUF_ERR_NONE);
  ASSERT(mu_buf_elements(b) == s1);
  ASSERT(mu_buf_is_read_only(b) == true);
  ASSERT(mu_buf_element_size(b) == sizeof(char));
  ASSERT(mu_buf_capacity(b) == sizeof(s1) / sizeof(char));

  ASSERT(mu_buf_init(b, s2, false, sizeof(char), sizeof(s2) / sizeof(char)) == MU_BUF_ERR_NONE);
  ASSERT(mu_buf_elements(b) == s2);
  ASSERT(mu_buf_is_read_only(b) == false);
  ASSERT(mu_buf_element_size(b) == sizeof(char));
  ASSERT(mu_buf_capacity(b) == sizeof(s2) / sizeof(char));

  ASSERT(mu_buf_from_cstr(b, NULL) == MU_BUF_ERR_ILLEGAL_ARG);
  ASSERT(mu_buf_from_cstr(b, s1) == MU_BUF_ERR_NONE);
  ASSERT(mu_buf_elements(b) == s1);
  ASSERT(mu_buf_is_read_only(b) == true);
  ASSERT(mu_buf_element_size(b) == sizeof(char));
  ASSERT(mu_buf_capacity(b) == strlen(s1));

  uint8_t s8[] = {1, 2, 3};
  mu_buf_init(b, s8, true, sizeof(uint8_t), sizeof(s8) / sizeof(uint8_t));
  ASSERT(mu_buf_get(b, 3, &d8) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_get(b, 2, &d8) == MU_BUF_ERR_NONE);
  ASSERT(d8 == 3);

  d8 = 4;
  ASSERT(mu_buf_set(b, 3, &d8) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_set(b, 2, &d8) == MU_BUF_ERR_NONE);
  ASSERT(s8[2] == 4);

  uint16_t s16[] = {11, 22, 33};
  mu_buf_init(b, s16, true, sizeof(uint16_t), sizeof(s16) / sizeof(uint16_t));
  ASSERT(mu_buf_get(b, 3, &d16) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_get(b, 2, &d16) == MU_BUF_ERR_NONE);
  ASSERT(d16 == 33);

  d16 = 44;
  ASSERT(mu_buf_set(b, 3, &d16) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_set(b, 2, &d16) == MU_BUF_ERR_NONE);
  ASSERT(s16[2] == 44);

  uint32_t s32[] = {111, 222, 333};
  mu_buf_init(b, s32, true, sizeof(uint32_t), sizeof(s32) / sizeof(uint32_t));
  ASSERT(mu_buf_get(b, 3, &d32) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_get(b, 2, &d32) == MU_BUF_ERR_NONE);
  ASSERT(d32 == 333);

  d32 = 444;
  ASSERT(mu_buf_set(b, 3, &d32) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_set(b, 2, &d32) == MU_BUF_ERR_NONE);
  ASSERT(s32[2] == 444);

  ASSERT(mu_buf_init(b, s_inventory, true, sizeof(inventory_t), sizeof(s_inventory)/sizeof(inventory_t)) == MU_BUF_ERR_NONE);
  ASSERT(mu_buf_get(b, 3, &dinv) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_get(b, 2, &dinv) == MU_BUF_ERR_NONE);
  ASSERT(dinv.id == s_inventory[2].id);
  ASSERT(strcmp(dinv.name, s_inventory[2].name) == 0);

  dinv.id = 11;
  ASSERT(mu_buf_set(b, 3, &dinv) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_set(b, 2, &dinv) == MU_BUF_ERR_NONE);
  ASSERT(s_inventory[2].id == 11);
  ASSERT(strcmp(dinv.name, s_inventory[2].name) == 0);  // unchanged
}

// =============================================================================
// private code

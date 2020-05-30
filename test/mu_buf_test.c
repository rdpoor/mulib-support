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

#define ELEMENT_COUNT 10

typedef struct {
  uint8_t id;
  const char *name;
} inventory_t;

// =============================================================================
// private declarations

static int sort_str_a(void *a, void *b);
static int sort_inventory_a(void *a, void *b);
static int sort_inventory_b(void *a, void *b);

// static void print_inventories(void);

// =============================================================================
// local storage

static inventory_t s_inventory[] = {
  {.id = 1, .name="cat"},
  {.id = 2, .name="emu"},
  {.id = 3, .name="auk"},
};

// =============================================================================
// public code

void mu_buf_test() {
  mu_buf_t bi;
  mu_buf_t *b = &bi;
  const char *s1 = "the quick brown fox jumps over the lazy dog.";
  char s2[ELEMENT_COUNT];

  ASSERT(mu_buf_init(b, NULL, true, sizeof(char), sizeof(s1) / sizeof(char)) == MU_BUF_ERR_ILLEGAL_ARG);
  ASSERT(mu_buf_init(b, (char *)s1, true, MU_BUF_MAX_ELEMENT_SIZE, sizeof(s1) / sizeof(char)) == MU_BUF_ERR_ILLEGAL_ARG);
  ASSERT(mu_buf_init(b, (char *)s1, true, sizeof(char), MU_BUF_MAX_CAPACITY) == MU_BUF_ERR_ILLEGAL_ARG);

  ASSERT(mu_buf_init(b, (char *)s1, true, sizeof(char), sizeof(s1) / sizeof(char)) == MU_BUF_ERR_NONE);
  ASSERT(mu_buf_elements(b) == s1);
  ASSERT(mu_buf_is_read_only(b) == true);
  ASSERT(mu_buf_element_size(b) == sizeof(char));
  ASSERT(mu_buf_capacity(b) == sizeof(s1) / sizeof(char));

  char c1;
  ASSERT(mu_buf_get(b, 6, &c1) == MU_BUF_ERR_NONE);
  ASSERT(c1 == 'i');
  c1 = 'a';
  ASSERT(mu_buf_put(b, 6, &c1) == MU_BUF_ERR_READ_ONLY);

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

  uint8_t d8;
  uint8_t s8[] = {1, 2, 3};
  uint8_t *p8;
  mu_buf_init(b, s8, false, sizeof(uint8_t), sizeof(s8) / sizeof(uint8_t));

  ASSERT(mu_buf_ref(b, 3, (void **)&p8) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(p8 == NULL);
  ASSERT(mu_buf_ref(b, 2, (void **)&p8) == MU_BUF_ERR_NONE);
  ASSERT(p8 == &s8[2]);

  ASSERT(mu_buf_get(b, 3, &d8) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_get(b, 2, &d8) == MU_BUF_ERR_NONE);
  ASSERT(d8 == 3);

  d8 = 4;
  ASSERT(mu_buf_put(b, 3, &d8) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_put(b, 2, &d8) == MU_BUF_ERR_NONE);
  ASSERT(s8[2] == 4);

  uint16_t d16;
  uint16_t s16[] = {11, 22, 33};
  uint16_t *p16;
  mu_buf_init(b, s16, false, sizeof(uint16_t), sizeof(s16) / sizeof(uint16_t));

  ASSERT(mu_buf_ref(b, 3, (void **)&p16) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(p16 == NULL);
  ASSERT(mu_buf_ref(b, 2, (void **)&p16) == MU_BUF_ERR_NONE);
  ASSERT(p16 == &s16[2]);

  ASSERT(mu_buf_get(b, 3, &d16) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_get(b, 2, &d16) == MU_BUF_ERR_NONE);
  ASSERT(d16 == 33);

  d16 = 44;
  ASSERT(mu_buf_put(b, 3, &d16) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_put(b, 2, &d16) == MU_BUF_ERR_NONE);
  ASSERT(s16[2] == 44);

  uint32_t d32;
  uint32_t s32[] = {111, 222, 333};
  uint32_t *p32;
  mu_buf_init(b, s32, false, sizeof(uint32_t), sizeof(s32) / sizeof(uint32_t));

  ASSERT(mu_buf_ref(b, 3, (void **)&p32) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(p32 == NULL);
  ASSERT(mu_buf_ref(b, 2, (void **)&p32) == MU_BUF_ERR_NONE);
  ASSERT(p32 == &s32[2]);

  ASSERT(mu_buf_get(b, 3, &d32) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_get(b, 2, &d32) == MU_BUF_ERR_NONE);
  ASSERT(d32 == 333);

  d32 = 444;
  ASSERT(mu_buf_put(b, 3, &d32) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_put(b, 2, &d32) == MU_BUF_ERR_NONE);
  ASSERT(s32[2] == 444);

  inventory_t dinv;
  inventory_t *pinv;
  ASSERT(mu_buf_init(b, s_inventory, false, sizeof(inventory_t), sizeof(s_inventory)/sizeof(inventory_t)) == MU_BUF_ERR_NONE);

  ASSERT(mu_buf_ref(b, 3, (void **)&pinv) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(pinv == NULL);
  ASSERT(mu_buf_ref(b, 2, (void **)&pinv) == MU_BUF_ERR_NONE);
  ASSERT(pinv == &s_inventory[2]);

  ASSERT(mu_buf_get(b, 3, &dinv) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_get(b, 2, &dinv) == MU_BUF_ERR_NONE);
  ASSERT(dinv.id == s_inventory[2].id);
  ASSERT(strcmp(dinv.name, s_inventory[2].name) == 0);

  dinv.id = 11;
  ASSERT(mu_buf_put(b, 3, &dinv) == MU_BUF_ERR_INDEX_BOUNDS);
  ASSERT(mu_buf_put(b, 2, &dinv) == MU_BUF_ERR_NONE);
  ASSERT(s_inventory[2].id == 11);
  ASSERT(strcmp(dinv.name, s_inventory[2].name) == 0);  // unchanged

  // mu_sort
  // char *s3 = "decaf";  // creates readonly string?
  char s3[6];
  strcpy(s3, "decaf");
  ASSERT(mu_buf_init(b, (char *)s3, false, sizeof(char), strlen(s3)) == MU_BUF_ERR_NONE);
  ASSERT(mu_buf_sort(b, NULL) == MU_BUF_ERR_ILLEGAL_ARG);
  ASSERT(mu_buf_sort(b, sort_str_a) == MU_BUF_ERR_NONE);
  ASSERT(strcmp(s3, "acdef") == 0);

  // printf("\r\ninventory before:");
  // print_inventories();

  ASSERT(mu_buf_init(b, s_inventory, false, sizeof(inventory_t), sizeof(s_inventory)/sizeof(inventory_t)) == MU_BUF_ERR_NONE);
  ASSERT(mu_buf_sort(b, sort_inventory_a) == MU_BUF_ERR_NONE);
  ASSERT(strcmp(s_inventory[0].name, "auk") == 0);
  ASSERT(strcmp(s_inventory[1].name, "cat") == 0);
  ASSERT(strcmp(s_inventory[2].name, "emu") == 0);

  // printf("\r\ninventory after sort_inventory_a:");
  // print_inventories();

  ASSERT(mu_buf_sort(b, sort_inventory_b) == MU_BUF_ERR_NONE);
  ASSERT(s_inventory[0].id == 11);
  ASSERT(s_inventory[1].id == 2);
  ASSERT(s_inventory[2].id == 1);

  // printf("\r\ninventory after sort_inventory_b:");
  // print_inventories();
}

// =============================================================================
// private code

static int sort_str_a(void *a, void *b) {
  char *ca = (char *)a;
  char *cb = (char *)b;
  return *ca - *cb;
}

static int sort_inventory_a(void *a, void *b) {
  // one way to do casting
  inventory_t *ia = (inventory_t *)a;
  inventory_t *ib = (inventory_t *)b;
  return strcmp(ia->name, ib->name);
}

static int sort_inventory_b(void *a, void *b) {
  // another way to do casting
  return ((inventory_t *)b)->id - ((inventory_t *)a)->id;
}

// static void print_inventories() {
//   for (int i=0; i<3; i++) {
//     printf("\r\n  inventory@%p: index=%d, name=%s", &s_inventory[i], s_inventory[i].id, s_inventory[i].name);
//   }
// }

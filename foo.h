# 1 "src/mu_items.h"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 362 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "src/mu_items.h" 2
# 32 "src/mu_items.h"
# 1 "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0/include/stdbool.h" 1 3 4
# 33 "src/mu_items.h" 2
# 1 "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0/include/stddef.h" 1 3 4
# 51 "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0/include/stddef.h" 3 4
typedef long int ptrdiff_t;
# 62 "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 90 "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0/include/stddef.h" 3 4
typedef int wchar_t;
# 118 "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0/include/stddef.h" 3 4
# 1 "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0/include/__stddef_max_align_t.h" 1 3 4
# 32 "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0/include/__stddef_max_align_t.h" 3 4
typedef long double max_align_t;
# 119 "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0/include/stddef.h" 2 3 4
# 34 "src/mu_items.h" 2

typedef enum {
  MU_COLLECTION_ERR_NONE,
  MU_COLLECTION_ERR_EMPTY,
  MU_COLLECTION_ERR_FULL,
  MU_COLLECTION_ERR_BOUNDS,
} mu_collection_err_t;
# 87 "src/mu_items.h"
typedef struct { size_t capacity; size_t count; foo_t *data; } mu_vect_foo_t; typedef struct { mu_vect_foo_t *c; size_t index; } mu_vref_foo_t; typedef _Bool (mu_vect_foo_traverse_fn)(mu_vref_foo_t *r, void *target); mu_vect_foo_t *mu_vect_foo_init(mu_vect_foo_t *c, foo_t *items, size_t n_items); mu_vect_foo_t *mu_vect_foo_reset(mu_vect_foo_t *c); _Bool mu_vect_foo_is_empty(mu_vect_foo_t *c); _Bool mu_vect_foo_is_full(mu_vect_foo_t *c); mu_collection_err_t mu_vect_foo_first(mu_vect_foo_t *c, foo_t *item); mu_collection_err_t mu_vect_foo_last(mu_vect_foo_t *c, foo_t *item); mu_collection_err_t mu_vect_foo_pop(mu_vect_foo_t *c, foo_t *item); mu_collection_err_t mu_vect_foo_push(mu_vect_foo_t *c, foo_t item); mu_collection_err_t mu_vect_foo_append(mu_vect_foo_t *c, foo_t item); mu_collection_err_t mu_vect_foo_remove(mu_vect_foo_t *c, foo_t *item); mu_collection_err_t mu_vect_foo_deref(mu_vref_foo_t *r, foo_t *item); mu_collection_err_t mu_vref_foo_first(mu_vect_foo_t *c, mu_vref_foo_t *r); mu_collection_err_t mu_vref_foo_last(mu_vect_foo_t *c, mu_vref_foo_t *r); mu_collection_err_t mu_vref_foo_next(mu_vref_foo_t *r); mu_collection_err_t mu_vref_foo_prev(mu_vref_foo_t *r); mu_collection_err_t mu_vref_foo_push(mu_vref_foo_t *r, foo_t i); mu_collection_err_t mu_vref_foo_pop(mu_vref_foo_t *r, foo_t *i); mu_collection_err_t mu_vref_foo_append(mu_vref_foo_t *r, foo_t i); mu_collection_err_t mu_vref_foo_remove(mu_vref_foo_t *r, foo_t *i); size_t mu_vect_foo_capacity(mu_vect_foo_t *c); size_t mu_vect_foo_count(mu_vect_foo_t *c); size_t mu_vect_foo_from_array(mu_vect_foo_t *c, foo_t items, size_t n_items); size_t mu_vect_foo_to_array(mu_vect_foo_t *c, foo_t items, size_t n_items); void mu_vect_foo_traverse(mu_vect_foo_t *c, mu_vect_foo_traverse_fn fn, foo_t target);
typedef struct { size_t capacity; size_t count; uint8_t *data; } mu_vect_u8_t; typedef struct { mu_vect_u8_t *c; size_t index; } mu_vref_u8_t; typedef _Bool (mu_vect_u8_traverse_fn)(mu_vref_u8_t *r, void *target); mu_vect_u8_t *mu_vect_u8_init(mu_vect_u8_t *c, uint8_t *items, size_t n_items); mu_vect_u8_t *mu_vect_u8_reset(mu_vect_u8_t *c); _Bool mu_vect_u8_is_empty(mu_vect_u8_t *c); _Bool mu_vect_u8_is_full(mu_vect_u8_t *c); mu_collection_err_t mu_vect_u8_first(mu_vect_u8_t *c, uint8_t *item); mu_collection_err_t mu_vect_u8_last(mu_vect_u8_t *c, uint8_t *item); mu_collection_err_t mu_vect_u8_pop(mu_vect_u8_t *c, uint8_t *item); mu_collection_err_t mu_vect_u8_push(mu_vect_u8_t *c, uint8_t item); mu_collection_err_t mu_vect_u8_append(mu_vect_u8_t *c, uint8_t item); mu_collection_err_t mu_vect_u8_remove(mu_vect_u8_t *c, uint8_t *item); mu_collection_err_t mu_vect_u8_deref(mu_vref_u8_t *r, uint8_t *item); mu_collection_err_t mu_vref_u8_first(mu_vect_u8_t *c, mu_vref_u8_t *r); mu_collection_err_t mu_vref_u8_last(mu_vect_u8_t *c, mu_vref_u8_t *r); mu_collection_err_t mu_vref_u8_next(mu_vref_u8_t *r); mu_collection_err_t mu_vref_u8_prev(mu_vref_u8_t *r); mu_collection_err_t mu_vref_u8_push(mu_vref_u8_t *r, uint8_t i); mu_collection_err_t mu_vref_u8_pop(mu_vref_u8_t *r, uint8_t *i); mu_collection_err_t mu_vref_u8_append(mu_vref_u8_t *r, uint8_t i); mu_collection_err_t mu_vref_u8_remove(mu_vref_u8_t *r, uint8_t *i); size_t mu_vect_u8_capacity(mu_vect_u8_t *c); size_t mu_vect_u8_count(mu_vect_u8_t *c); size_t mu_vect_u8_from_array(mu_vect_u8_t *c, uint8_t items, size_t n_items); size_t mu_vect_u8_to_array(mu_vect_u8_t *c, uint8_t items, size_t n_items); void mu_vect_u8_traverse(mu_vect_u8_t *c, mu_vect_u8_traverse_fn fn, uint8_t target);
typedef struct { size_t capacity; size_t count; ctype *data; } mu_vect_t; typedef struct { mu_vect_t *c; size_t index; } mu_vref_t; typedef _Bool (mu_vect_traverse_fn)(mu_vref_t *r, void *target); mu_vect_t *mu_vect_init(mu_vect_t *c, ctype *items, size_t n_items); mu_vect_t *mu_vect_reset(mu_vect_t *c); _Bool mu_vect_is_empty(mu_vect_t *c); _Bool mu_vect_is_full(mu_vect_t *c); mu_collection_err_t mu_vect_first(mu_vect_t *c, ctype *item); mu_collection_err_t mu_vect_last(mu_vect_t *c, ctype *item); mu_collection_err_t mu_vect_pop(mu_vect_t *c, ctype *item); mu_collection_err_t mu_vect_push(mu_vect_t *c, ctype item); mu_collection_err_t mu_vect_append(mu_vect_t *c, ctype item); mu_collection_err_t mu_vect_remove(mu_vect_t *c, ctype *item); mu_collection_err_t mu_vect_deref(mu_vref_t *r, ctype *item); mu_collection_err_t mu_vref_first(mu_vect_t *c, mu_vref_t *r); mu_collection_err_t mu_vref_last(mu_vect_t *c, mu_vref_t *r); mu_collection_err_t mu_vref_next(mu_vref_t *r); mu_collection_err_t mu_vref_prev(mu_vref_t *r); mu_collection_err_t mu_vref_push(mu_vref_t *r, ctype i); mu_collection_err_t mu_vref_pop(mu_vref_t *r, ctype *i); mu_collection_err_t mu_vref_append(mu_vref_t *r, ctype i); mu_collection_err_t mu_vref_remove(mu_vref_t *r, ctype *i); size_t mu_vect_capacity(mu_vect_t *c); size_t mu_vect_count(mu_vect_t *c); size_t mu_vect_from_array(mu_vect_t *c, ctype items, size_t n_items); size_t mu_vect_to_array(mu_vect_t *c, ctype items, size_t n_items); void mu_vect_traverse(mu_vect_t *c, mu_vect_traverse_fn fn, ctype target);

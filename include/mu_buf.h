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

/**
 * @file Manipulate arrays of byte-sized data.
 */

#ifndef MU_BUF_H_
#define MU_BUF_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// =============================================================================
// types and definitions

// the underlying data type.  May be modified for uint8_t, char, etc.
typedef uint8_t mu_buf_item_t;

// mu_buf_t accesses a subset of the bytes in a buffer, which can be either
// mutable or immutable.  The only difference is in the accessor functions.
typedef struct _mu_buf {
  union {
    const mu_buf_item_t const *ro_items; // read-only data
    mu_buf_item_t const *rw_items;       // modifiable / mutable data
  };
  size_t capacity; // number of elements in items
  int start;       // start of slice (inclusive)
  int end;         // end of slice (exclusive)
} mu_buf_t;

// =============================================================================
// declarations

/**
 * \brief Initialize a read-only mu_buf from an array of items.
 *
 * Upon return, mu_buf->start is set to zero and mu_buf->end is set to capacity.
 *
 * \param buf Pointer to a mu_buf structure to be initialized.
 * \param items Pointer to an array of items.
 * \param capacity The number of items in the array.
 * \return Pointer to the mu_buf structure.
 */
mu_buf_t *mu_robuf_init(mu_buf_t *buf,
                        const mu_buf_item_t const *items,
                        size_t capacity);

/**
 * \brief Initialize a read-only mu_buf from a null-terminated C style string.
 *
 * Upon return, mu_buf->start is set to zero and mu_buf->end is set to capacity.
 *
 * \param buf Pointer to a mu_buf structure to be initialized.
 * \param items Pointer to a null-terminated string.
 * \return Pointer to the mu_buf structure.
 */
mu_buf_t *mu_robuf_init_from_cstr(mu_buf_t *buf, const char *cstr);

/**
 * \brief Reset a read-only mu_buf to span the entire array of items.
 *
 * This sets mu_buf->start to zero and mu_buf->end to capacity.
 *
 * \param buf Pointer to a mu_buf structure.
 * \return Pointer to the mu_buf structure.
 */
mu_buf_t *mu_robuf_reset(mu_buf_t *buf);

/**
 * \brief Initialize a read/write mu_buf with a backing store.
 *
 * Upon return, mu_buf->start and mu_buf->end are set to zero.
 *
 * \param buf Pointer to a mu_buf structure to be initialized.
 * \param items Pointer to a backing store.
 * \param capacity The number of items in the backing store.
 * \return Pointer to the mu_buf structure.
 */
mu_buf_t *mu_rwbuf_init(mu_buf_t *buf,
                       const mu_buf_item_t const *items,
                       size_t capacity);

/**
 * \brief Reset mu_buf to span the entire array of items.
 *
 * This sets mu_buf->start and mu_buf->end to zero.
 *
 * \param buf Pointer to a mu_buf structure.
 * \return Pointer to the mu_buf structure.
 */
mu_buf_t *mu_rwbuf_reset(mu_buf_t *buf);

/**
 * \brief Return a pointer to the underlying read-only array of items.
 *
 * \param buf Pointer to a mu_buf structure.
 * \return Pointer to the underlying array of items.
 */
const mu_buf_item_t const *mu_robuf_items(mu_buf_t *buf);

/**
 * \brief Return a pointer to the underlying read-write array of items.
 *
 * \param buf Pointer to a mu_buf structure.
 * \return Pointer to the underlying array of items.
 */
mu_buf_item_t const *mu_rwbuf_items(mu_buf_t *buf);

/**
 * \brief Return the capacity of a mu_buf
 *
 * \param buf Pointer to a mu_buf structure.
 * \return The capacity of the mu_buf.
 */
size_t mu_buf_capacity(mu_buf_t *buf);

/**
 * \brief Return the starting index of a mu_buf
 *
 * The starting index is 0-based and can range from 0 to capacity-1.
 *
 * \param buf Pointer to a mu_buf structure.
 * \return The starting index of the mu_buf.
 */
int mu_buf_start(mu_buf_t *buf);

/**
 * \brief Return the ending index of a mu_buf
 *
 * The ending index indicates the first element past the substring.
 *
 * \param buf Pointer to a mu_buf structure.
 * \return The ending index of the mu_buf.
 */
int mu_buf_end(mu_buf_t *buf);

/**
 * \brief Return the length of the array slice of a mu_buf.
 *
 * \param buf Pointer to a mu_buf structure.
 * \return The length of the slice.
 */
size_t mu_buf_length(mu_buf_t *buf);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MU_BUF_H_ */

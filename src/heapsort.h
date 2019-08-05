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

#ifndef MULIB_HEAPSORT_H_
#define MULIB_HEAPSORT_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

// =============================================================================
// types and definitions

typedef int (*heap_cmp_fn)(void *items, int i1, int i2);
typedef void (*heap_swap_fn)(void *items, int i1, int i2);

// =============================================================================
// declarations

/**
 * @brief Efficient in-place sort routine.
 *
 * @param[in] cmp   User-supplied comparison function.   Call signature is
 *                  `cmp(void *items, int i1, int i2)`, where items is the
 *                  list of items to sort, i1 and i2 are indeces into the
 *                  array.  The function should return a negative, zero or
 *                  positive value if items[i1] is less than, equal to or
 *                  greather than items[i2].
 * @param[in] swap  User-supplied swap function.  Call signature is
 *                  `swap(void *items, int i1, int i2)`.  The function should
 *                  swap the elements referred to by items[i1] and items[i2].
 * @param[in] items The array of items to sort.
 * @param[in] count The nmber of items in the array.
 *
 * On return, the items are sorted according to the user-supplied cmp function.
 */
void heapsort(heap_cmp_fn cmp, heap_swap_fn swap, void *items, int count);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MULIB_HEAPSORT_H_

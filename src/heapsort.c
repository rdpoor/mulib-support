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

#include "heapsort.h"
#include <stddef.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static void heapify(heap_cmp_fn cmp, heap_swap_fn swap, void *items, int count);

static void sift_down(heap_cmp_fn cmp,
                      heap_swap_fn swap,
                      void *items,
                      int start,
                      int end);

// =============================================================================
// local storage

// =============================================================================
// public code

// https://rosettacode.org/wiki/Sorting_algorithms/Heapsort#C

void heapsort(heap_cmp_fn cmp, heap_swap_fn swap, void *items, int count) {

  heapify(cmp, swap, items, count);

  size_t end = count - 1;

  while (end > 0) {
    // swap the root(maximum value) of the heap with last element of the heap
    swap(items, end, 0);
    // decrement the size of the heap so that the previous max value will stay
    // in its proper place)
    end -= 1;
    // put the heap back in max-heap order
    sift_down(cmp, swap, items, 0, end);
  }

}
// =============================================================================
// private code

static void heapify(heap_cmp_fn cmp, heap_swap_fn swp, void *items, int count) {
  int start = (count - 2) / 2;  // index of last parent node

  while (start >= 0) {
    sift_down(cmp, swp, items, start, count-1);
    start -= 1;
  }
}

static void sift_down(heap_cmp_fn cmp,
                      heap_swap_fn swap,
                      void *items,
                      int start,
                      int end) {
  int root = start;
  while (root * 2 + 1 <= end) {          // while root has at least one child...
    int child = root * 2 + 1;            // left child
    if ((child + 1 <= end) && cmp(items, child, child+1) < 0) {
      // child has a sibling and its value is less than the sibling's...
      child += 1;                        // then act on right child instead
    }
    if (cmp(items, root, child) < 0) {   // out of max-heap order
      swap(items, root, child);
      root = child;                      // continue sifting down the child
    } else {
      return;
    }
  }
}

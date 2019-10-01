# Collection notes

It should be possible to implement collections using either arrays or linked
lists, which means both implementations could have similar APIs.

## implementations

For vector-like objects, `front` is at index == 0, `back` is at count-1.  For
list-like objects, `front` is the tail of the list, `back` is the head.  This
terminology means that `push` and `pop` operations operate on the back of the
collection.

### mu_vect

Fixed size array.  Efficient for push and pop, inefficient for push_front and
pop_front.  Efficient for ref_next and ref_prev.

### mu_list

Extensible linked list.  Efficient for push and pop, inefficient for push_front
and pop_front.  Efficient for ref_next, wildly inefficient for ref_prev.

### mu_dvect

Fixed size ring buffer.  Efficient for push, pop, push_front and pop_front.
Efficient for ref_next and ref_prev.

### mu_dlist

Extensible double-ended linked list.  Efficient for push and pop, push_front
and pop_front.  Efficient for ref_next, wildly inefficient for ref_prev.

|        |push_back|pop_back|push_front|pop_front|ref_back|ref_front|ref_next|ref_prev|
|--------|---------|--------|----------|---------|--------|---------|--------|--------|
|mu_list |O(1)     |O(1)    |O(N)      | O(N)    |O(1)    |O(N)     | O(1)   |O(N)    |
|mu_dlist|O(1)     |O(1)    |O(1)      | O(1)    |O(1)    |O(1)     | O(1)   |O(N)    |
|mu_vect |O(1)     |O(1)    |O(N)      | O(N)    |O(1)    |O(1)     | O(1)   |O(1)    |
|mu_dvect|O(1)     |O(1)    |O(1)      | O(1)    |O(1)    |O(1)     | O(1)   |O(1)    |

## Dequeue Functions

### push(c, item)

Alias for push_back()

### pop(c, &item)

Alias for pop_back()

### push_back(c, item)

Push item onto the collection.  For lists, push onto the head of the list.  For
vectors, pushes onto the end, i.e. at the highest index.

### push_front(c, item)

### pop_back(c, &item)

Pop an item from the collection.  For lists, pop from the head of the list.  For
vectors, pop from the end, i.e. at the highest index.

### pop_front(c, &item)

### back(c, &item)

### front(c, &item)

## Cursor functions

By abstracting out the idea of a "cursor" or "reference" to a collection, we can
continue to have similar functions for both arrays and linked lists.  A "ref"
is a reference into a specific collection (array or linked list):

### ref_back(c, &ref)

Get a reference to the back of the collection.

### ref_front(c, &ref)

Get a reference to the front of the collection.

### ref_of(c, item, &ref)

Get a reference to an item in the collection.

### ref(ref, &item)

Dereference an item in the collection

### insert_before(ref, item)

Insert item before the reference.

### insert_after(ref, item)

Insert item after the reference.

### remove_at(ref, &item)

Remove the referred-to item from the collection.

### traverse(c, mu_msg)

Visit each item in c, calling mu_msg with a reference to each element.

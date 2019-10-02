# Collection notes

It should be possible to implement collections using either arrays or linked
lists, which means both implementations could have similar APIs.

## terminology

Collections are ordered, with a "first" and a "last" element.  In this system,
"push" means insert before first and "append" means insert after last.  Inverse
functions are "pop" and "remove" respectively.

This means that push and pop are efficient for list-like collections but
inefficient for array-like collections.  Correspondingly, append and remove are
efficient for array-like collections and inefficient for list-like collections.

Note, however, the double ended collections dvect and dlist are efficient at
all four operations: push, pop, append, delete.

## Function Names

Each function has a name of the form:

    `mu_<T>_<verb><I>`

where \<T\> is the underling collection type:

- vect
- list
- dvect
- dlist

\<verb\> is an action, such as:

- push
- pop
- count
- first
- last
- ... etc

and \<I\> is the underlying item type:

- \<blank\> indicates a pointer-sized items `(void *)`
- `_u8` and `_i8` for signed and unsigned 8 bit integers
- `_u16` and `_i16` for signed and unsigned 16 bit integers
- `_u32` and `_i32` for unsigned and signed 32 bit integers
- `_i` for native integers
- `_f` for floats
- `_d` for doubles

## implementations

### mu_vect

Fixed size array.  Efficient for append and remove, inefficient for push and
pop.  Efficient for ref_next and ref_prev.

### mu_list

Extensible linked list.  Efficient for push and pop, inefficient for append
and remove.  Efficient for ref_next, inefficient for ref_prev.

### mu_dvect

Fixed size ring buffer.  Efficient for push, pop, append and remove.
Efficient for ref_next and ref_prev.

### mu_dlist

Extensible double-ended linked list.  Efficient for push and pop, append
and remove.  Efficient for ref_next, wildly inefficient for ref_prev.

|        |push|pop |append|remove|ref_first|ref_last|ref_next|ref_prev|
|--------|----|----|------|------|---------|--------|--------|--------|
|mu_list |O(1)|O(1)|O(N)  |O(N)  |O(1)     |O(N)    | O(1)   |O(N)    |
|mu_dlist|O(1)|O(1)|O(1)  |O(1)  |O(1)     |O(1)    | O(1)   |O(N)    |
|mu_vect |O(1)|O(1)|O(N)  |O(N)  |O(1)     |O(1)    | O(1)   |O(1)    |
|mu_dvect|O(1)|O(1)|O(1)  |O(1)  |O(1)     |O(1)    | O(1)   |O(1)    |

## Adding and removing items

### push(c, item)

Add item before first item in the collection.

### pop(c, &item)

Remove first item from the collection.

### append(c, item)

Add item after the list item in the collection.

### remove(c, &item)

Remove the last item from the collection.

### first(c, &item)

Access the first item in the collection.

### last(c, &item)

Access the last item in the collection.

### reset(c)

Remove all items from the collection.

### to_array(c, \*items, count)

Copy up to count items from collection into the items array.  Returns
the number of items copied.

### from_array(c, \*items, count)

Clears the collection, then copies items from the items array into the
collection, stopping after count items have been copied or the collection
is full, whichever comes first.  Returns the number of items copied.

## Cursor functions

A "ref" is a reference to an item in a collection.  It allows you to access
items in the collection without regard as to whether the underlying storage
is an array, list, or some other data structure.

### ref_first(c, &ref)

Get a reference to the first of the collection.

### ref_last(c, &ref)

Get a reference to the last of the collection.

### ref_of(c, item, &ref)

Get a reference to an item in the collection.

### ref_next(sref, &ref)

Get a reference to the next item in the collection

### ref_prev(sref, &ref)

Get a reference to the previous item in the collection.

### deref(ref, &item)

Dereference an item in the collection

### insert_before(ref, item)

Insert item before the reference.

### insert_after(ref, item)

Insert item after the reference.

### remove_at(ref, &item)

Remove the referred-to item from the collection.

### traverse(c, mu_msg)

Visit each item in c, calling mu_msg with a reference to each element.

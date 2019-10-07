# mu_collection family

A mu collection provides ordered storage for homogeneous objects, such as
pointers, integers or characters.  Mu collection provides several storage
strategies, including single- and double-ended linked lists, arrays and ring
buffers.

A consistent set of interface functions makes it easy to switch among storage
strategies, depending on your requirements.

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

## References

A "ref" is a reference to an item in a collection.  A ref provides a common
interface to items in the collection, independent of the underlying storage
strategy.

### ref_first(c, &ref)

Get a reference to the first of the collection.

### ref_last(c, &ref)

Get a reference to the last of the collection.

### ref_next(&ref)

Get a reference to the next item in the collection

### ref_prev(&ref)

Get a reference to the previous item in the collection.

### deref(ref, &item)

Dereference an item in the collection

### ref_push(ref, item)

Insert item before the reference.

### ref_append(ref, item)

Insert item after the reference.

### ref_pop(ref, &item)

Remove the item referred to by ref and move ref to next.

### ref_remove(ref, &item)

Remove the item referred to by ref and move ref to prev.

### traverse(c, fn)

Visit each item in c, calling fn with a reference to each element.  Stop
when mu_task returns non-null value and returns that.

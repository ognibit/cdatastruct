# C99 Data Structure

A simple project that implement common data structure in C99.

## Implementations

The goal is to provide a small implementation of the most common data structure
that can be integrated easily in every project.
The main idea is to provide only the logic, essentially index management for
arrays, in order to allow the developer to build the complete data structure
following the project constraints (custom memory management, error handling).
Where indexes do not make sense or too restrictive, pointers are used.
It is coherent with the assumption that a C programmer knows what is doing.
There are no under the hood memory allocation or abort calls.

The current version is in development stage and compiled only on Linux.
There is no overflow/underflow management for now.

The code is released as header only just for simplicity, but is straightforward
to convert it into classical header/code pair.

In the `tests` directory there are example of usage.

### Range

`range.h`: mainly for safely indexing arrays.

There is no `array` (or `vector`) data structure because it is trivial.
However, the indexing should be verified to be in a certain range.
And to provide more flexibility, the range can start from a different number
than zero, even negative. 
The access to the actual array is done just converting the index with the
range. A callback procedure can be set to manage violations.
The range can be passed as normally is done with the array size, as additional
argument to a function. Or it is trivial to pack it with the actual array
pointer.

```
struct ArrayInt {
    Range range;
    int *values;
};
```

## Stack

`stack.h`: provides the `StackIndex` for building a stack on an array.

It does not provide the actual array manipulation because it depends on the
array type. Moreover, the error management is application specific.
Therefore, the user must implement the actual stack for every type needed, but
it is just as trivial as using the `StackIndex` along with the support array.

## Queue

`queue.h`: provides the `QueueIndex` for building a circular queue on an array.

It does not provide the actual array manipulation because it depends on the
array type. Moreover, the error management is application specific.
Therefore, the user must implement the actual queue for every type needed, but
it is just as trivial as using the `QueueIndex` along with the support array.

## Object Pool

`objpool.h`: a fixed size allocator for instances of same type (dimension).

The pool must be initialized with a memory arena where to store the objects.
The user is in charge for the scope of the arena.
The pool manage to provide the objects (`acquire`) that must be re-inserted in
the pool (`release`) to be available again.
The pool is generic and the object pointer type must be cast.
The objects in the pool are not touched, therefore they can be initialized and
released. When acquired again the data are still there. There is no guarantee
in the order of acquire works, therefore the object must be all fungible if
used as real pool, like a connection pool.
The code provide safety asserts that can be turned off setting `NDEGUG=1` as
usual. In development stage, they could help to spot the release of wrong
pointers.

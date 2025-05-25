# DS - Data Structure

A simple project that implement common data structure in C99.

## Implementations

### Range

`range.h`: mainly for safely indexing arrays.

There is no `array` (or `vector`) data structure because it is trivial.
However, the indexing should be verified to be in a certain range.
And to provide more flexibility, the range can start from a different number
than zero, even negative. 
The access to the actual array is done just converting the index with the
range. A callback procedure can be set to manage violations.
The range can be passed as normaly is done with the array size, as additional
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

#ifndef _DS_STACK_H
#define _DS_STACK_H

/* Stack (Index) Data Structure (for array indexing)
 * Namespace: stack
 *
 * Basic operation for implementing a array based stack.
 */

#include <stddef.h>
#include <stdbool.h>

/* Do not use top directly, use the methods */
struct StackIndex {
    size_t size; /* size included as index */
    size_t top; /* 0: underflow, valid between [1,size] */
};

typedef struct StackIndex StackIndex;

int stack_init(StackIndex *s, const size_t size)
{
    if (s == NULL){
        return -1;
    }
    s->size = size;
    s->top = 0;

    return 0;
}

bool stack_isempty(const StackIndex *s)
{
    if (s == NULL){
        return false;
    }
    return s->top == 0;
}

bool stack_isfull(const StackIndex *s)
{
    if (s == NULL){
        return false;
    }
    return s->top >= s->size;
}

/* return the index for setting the value in the support array.
 * -1 in case of overflow
 */
long stack_push(StackIndex *s)
{
    if (s == NULL){
        return -1;
    }

    if (stack_isfull(s)){
        return -1;
    }

    s->top++;
    return (long)s->top-1;
}

/* return the index for getting the value in the support array.
 * -1 in case of underflow
 */
long stack_pop(StackIndex *s)
{
    if (s == NULL){
        return -1;
    }

    if (stack_isempty(s)){
        return -1;
    }

    long i = (long)s->top - 1;
    s->top--;
    return i;
}
#endif

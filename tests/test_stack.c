#include "stack.h"
#include "asserts.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* a proposed implementation */
typedef struct {
    StackIndex index;
    int *values;
} StackInt;

int stacki_init(StackInt *s, size_t size)
{
    int *v = calloc(size, sizeof(int));
    if (!v){
        return -1;
    }
    if (stack_init(&s->index, size) < 0){
        return -2;
    }
    s->values = v;
    return 0;
}

bool stacki_push(StackInt *s, int x)
{
    long i = stack_push(&s->index);
    if (i == -1){
        return false;
    }
    s->values[i] = x;
    return true;
}

bool stacki_pop(StackInt *s, int *x)
{
    long i = stack_pop(&s->index);
    if (i == -1){
        return false;
    }
    *x = s->values[i];
    return true;
}

int main()
{
    StackInt stack;
    stacki_init(&stack, 3);
    assert_true(stack_isempty(&stack.index), "init empty");
    assert_false(stack_isfull(&stack.index), "init full");

    bool p;

    /* TEST PUSH */
    puts("PUSH");
    p = stacki_push(&stack, 1);
    assert_true(p, "");
    assert_false(stack_isfull(&stack.index), "");
    assert_false(stack_isempty(&stack.index), "");

    p = stacki_push(&stack, 2);
    assert_true(p, "");
    assert_false(stack_isfull(&stack.index), "");
    assert_false(stack_isempty(&stack.index), "");

    p = stacki_push(&stack, 3);
    assert_true(p, "");
    assert_true(stack_isfull(&stack.index), "");
    assert_false(stack_isempty(&stack.index), "");

    /* stack full */
    p = stacki_push(&stack, 3);
    assert_false(p, "");
    assert_true(stack_isfull(&stack.index), "");
    assert_false(stack_isempty(&stack.index), "");

    /* TEST POP */
    puts("POP");
    int x;
    p = stacki_pop(&stack, &x);
    assert_true(p, "");
    assert_true(x == 3, "");
    assert_false(stack_isfull(&stack.index), "");
    assert_false(stack_isempty(&stack.index), "");

    p = stacki_pop(&stack, &x);
    assert_true(p, "");
    assert_true(x == 2, "");
    assert_false(stack_isfull(&stack.index), "");
    assert_false(stack_isempty(&stack.index), "");

    p = stacki_pop(&stack, &x);
    assert_true(p, "");
    assert_true(x == 1, "");
    assert_false(stack_isfull(&stack.index), "");
    assert_true(stack_isempty(&stack.index), "");

    /* stack emtpy */
    p = stacki_pop(&stack, &x);
    assert_false(p, "");
    assert_true(x == 1, "untouch x");
    assert_false(stack_isfull(&stack.index), "");
    assert_true(stack_isempty(&stack.index), "");

    puts("OK");
    return 0;
}

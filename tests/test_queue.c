#include "queue.h"
#include "asserts.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* a proposed implementation */
typedef struct {
    QueueIndex index;
    int *values;
} QueueInt;

int queuei_init(QueueInt *s, size_t size)
{
    int *v = calloc(size, sizeof(int));
    if (!v){
        return -1;
    }
    if (queue_init(&s->index, size) < 0){
        return -2;
    }
    s->values = v;
    return 0;
}

bool queuei_enqueue(QueueInt *s, int x)
{
    long i = queue_enqueue(&s->index);
    if (i == -1){
        return false;
    }
    s->values[i] = x;
    return true;
}

bool queuei_dequeue(QueueInt *s, int *x)
{
    long i = queue_dequeue(&s->index);
    if (i == -1){
        return false;
    }
    *x = s->values[i];
    return true;
}

int main()
{
    QueueInt queue;
    queuei_init(&queue, 3);
    assert_true(queue_isempty(&queue.index), "init empty");
    assert_false(queue_isfull(&queue.index), "init full");
    assert_true(queue_length(&queue.index) == 0, "init length");
    assert_true(queue_size(&queue.index) == 3, "init size");

    bool p;

    /* TEST ENQUEUE */
    puts("ENQUEUE");
    p = queuei_enqueue(&queue, 1);
    assert_true(p, "");
    assert_false(queue_isfull(&queue.index), "isfull");
    assert_false(queue_isempty(&queue.index), "isempty");
    assert_true(queue_length(&queue.index) == 1, "length");

    p = queuei_enqueue(&queue, 2);
    assert_true(p, "");
    assert_false(queue_isfull(&queue.index), "isfull");
    assert_false(queue_isempty(&queue.index), "isempty");
    assert_true(queue_length(&queue.index) == 2, "length");

    p = queuei_enqueue(&queue, 3);
    assert_true(p, "");
    assert_true(queue_isfull(&queue.index), "isfull");
    assert_false(queue_isempty(&queue.index), "isempty");
    assert_true(queue_length(&queue.index) == 3, "length");

    p = queuei_enqueue(&queue, 4);
    assert_false(p, "should be full");
    assert_true(queue_isfull(&queue.index), "isfull");
    assert_false(queue_isempty(&queue.index), "isempty");
    assert_true(queue_length(&queue.index) == 3, "length");

    /* TEST DEQUEUE */
    puts("DEQUEUE");
    int x;
    p = queuei_dequeue(&queue, &x);
    assert_true(p, "");
    assert_true(x == 1, "");
    assert_false(queue_isfull(&queue.index), "");
    assert_false(queue_isempty(&queue.index), "");

    p = queuei_dequeue(&queue, &x);
    assert_true(p, "");
    assert_true(x == 2, "");
    assert_false(queue_isfull(&queue.index), "");
    assert_false(queue_isempty(&queue.index), "");

    p = queuei_dequeue(&queue, &x);
    assert_true(p, "");
    assert_true(x == 3, "");
    assert_false(queue_isfull(&queue.index), "");
    assert_true(queue_isempty(&queue.index), "");

    p = queuei_dequeue(&queue, &x);
    assert_false(p, "should be empty");
    assert_true(x == 3, "unchanged");
    assert_false(queue_isfull(&queue.index), "");
    assert_true(queue_isempty(&queue.index), "");

    for (int i=0; i < 100; i++){
        p = queuei_enqueue(&queue, i);
        assert_true(p, "enqueue in loop");

        p = queuei_dequeue(&queue, &x);
        assert_true(p, "");
        assert_true(x == i, "dequeue in loop");
    }

    puts("OK");
    return 0;
}

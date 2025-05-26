#ifndef _DS_QUEUE_H
#define _DS_QUEUE_H

/* Circular Queue (Index) Data Structure (for array indexing)
 * Namespace: queue
 *
 * Basic operation for implementing a array based queue.
 *
 * In this version the possible overflow of indexes are not managed.
 */

#include <stddef.h>
#include <stdbool.h>

struct QueueIndex {
    size_t size; /* capacity */
    size_t head; /* start of the data */
    size_t len;  /* how many data */
};

typedef struct QueueIndex QueueIndex;

int queue_init(QueueIndex *q, const size_t size)
{
    if (q == NULL){
        return -1;
    }
    q->size = size;
    q->head = 0;
    q->len = 0;

    return 0;
}

bool queue_isempty(const QueueIndex *q)
{
    if (q == NULL){
        return false;
    }
    return q->len == 0;
}

bool queue_isfull(const QueueIndex *q)
{
    if (q == NULL){
        return false;
    }
    return q->len == q->size;
}

size_t queue_length(const QueueIndex *q)
{
    if (q == NULL){
        return false;
    }
    return q->len;
}

size_t queue_size(const QueueIndex *q)
{
    if (q == NULL){
        return false;
    }
    return q->size;
}

/* return the index for setting the value in the support array.
 * -1 in case of overflow
 */
long queue_enqueue(QueueIndex *q)
{
    if (q == NULL){
        return -1;
    }

    if (queue_isfull(q)){
        return -1;
    }

    /* circular */
    long i = (long)((q->head + q->len) % q->size);
    q->len++;
    return i;
}

/* return the index for getting the value in the support array.
 * -1 in case of underflow
 */
long queue_dequeue(QueueIndex *q)
{
    if (q == NULL){
        return -1;
    }

    if (queue_isempty(q)){
        return -1;
    }

    long i = (long)q->head;
    q->head = (q->head + 1) % q->size;
    q->len--;
    return i;
}
#endif

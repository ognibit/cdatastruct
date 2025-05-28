#ifndef _DS_OBJPOOL_H
#define _DS_OBJPOOL_H

/* ObjPool Data Structure
 * Namespace: objpool
 *
 * Fixed size object pool.
 * This implementation is more an allocator that an object pool
 * because the object in the pool are not initialized.
 * For example, a connection pool should acquire all the object (connections)
 * from the pool, initialize them and release all.
 * The code does not clean the content of the released object.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

struct ObjPoolBlock {
    size_t next;   /* index in the pool of the next free block */
    uint8_t obj[]; /* Flexible Array Member */
};

typedef struct ObjPoolBlock ObjPoolBlock;

#define OBJPOOL_SIZEOF(cnt, objsize) (((size_t)cnt) * (((size_t)objsize) + sizeof(ObjPoolBlock)))

struct ObjPool {
    size_t size;     /* max number of blocks */
    size_t objsize;  /* bytes for the object in the block */
    size_t blksize;  /* bytes for one entire block */
    size_t len;      /* number of blocks allocated */
    size_t head;     /* index of the first free block */
    uint8_t *blocks; /* the raw memmory in bytes */
};

typedef struct ObjPool ObjPool;

/* Initialize the object pool on the arena memory.
 * The size of the arena should be defined with OBJPOOL_SIZEOF.
 * It set cnt objects of dimension objsize in the arena.
 * Return true if all the arguments are not zero or NULL.
 */
bool objpool_init(ObjPool *pool, void *arena, size_t cnt, size_t objsize)
{
    if (pool == NULL){
        return false;
    }
    if (arena == NULL){
        return false;
    }
    if (cnt == 0){
        return false;
    }
    if (objsize == 0){
        return false;
    }

    pool->size = cnt;
    pool->objsize = objsize;
    pool->blksize = sizeof(ObjPoolBlock) + objsize;
    pool->len = 0;
    pool->head = 0;
    pool->blocks = (uint8_t*)arena;

    /* init free list over all the arena */
    for (size_t i=0; i < pool->size - 1; i++){
        ObjPoolBlock *b = (ObjPoolBlock*)&pool->blocks[i * pool->blksize];
        b->next = i+1;
    }

    return true;
}

/* Get an instance among the available in the pool.
 * Return the pointer to the object.
 * Return NULL if the poll is NULL or no more instances available.
 */
void * objpool_acquire(ObjPool *pool)
{
    if (pool == NULL){
        return NULL;
    }
    if (pool->len >= pool->size){ /* defensive */
        return NULL;
    }

    /* get head of the free list */
    size_t head = pool->head;
    size_t index = head * pool->blksize;
    ObjPoolBlock *o = (ObjPoolBlock*)&pool->blocks[index];
    pool->head = o->next;
    pool->len++;

    o->next = head; /* save for release */

    assert(pool->len <= pool->size);

    return (void *)o->obj;
}

/* Release a previously acquired object from the pool.
 * If obj is NULL nothing happen.
 */
void objpool_release(ObjPool *pool, void *obj)
{
    if (pool == NULL){
        return;
    }
    if (obj == NULL){
        return;
    }
    if (pool->len == 0){
        return;
    }

    uint8_t *p = (uint8_t *)obj;
    /* check the pointer in the blocks memory area */
    assert(p >= pool->blocks);
    assert(p < pool->blocks + ((pool->size +1) * pool->blksize));

    /* get the pointer to the structure going back in the memory */
    ObjPoolBlock *b = (ObjPoolBlock *)(p - sizeof(ObjPoolBlock));
    /* acquire() set next to the block index in blocks */
    size_t blkidx = b->next;
    assert(blkidx < pool->size);

    /* append the block to the free list */
    b->next = pool->head;
    pool->head = blkidx;

    pool->len--;
}

#endif

#ifndef _DS_SLIST_H
#define _DS_SLIST_H

/* Single Linked List on memory arena.
 * Namespace: slist
 *
 * The actual values must be stored outside the list and
 * guaranteed to be in the same scope of the list.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

/* the corresponding value for NULL in size_t indexes is SIZE_MAX */
#define SLIST_NIL SIZE_MAX
#define SLIST_SIZEOF(n) ( sizeof(SList) + (sizeof(SListItem) * (size_t)n) )

typedef struct SList SList;
typedef struct SListItem SListItem;
typedef struct SListIter SListIter;

struct SListItem {
    size_t next;  /* index of the next element in SList.items */
    void *value;  /* pointer to the user object to store */
};

struct SList {
    size_t size;  /* capacity */
    size_t len;   /* number of stored items */
    size_t head;  /* index of the list head item */
    size_t free;  /* index of the free list head item */
    SListItem *items; /* array of 'size' items */
};

/* SList iterator.
 * It refers to items from the first to the SLIST_NIL
 */
struct SListIter {
    size_t prev; /* index to list->items of the previous item */
    size_t curr; /* index to list->items of the current item */
    SList *list; /* the reference to the list under iteration */
};

/* Construct a list of indicated capacity into the memory arena.
 * The arena must be at least SLIST_SIZEOF(capacity) long
 * otherwise the behavior is undefined.
 * No aditional memory is allocated.
 * Time complexity: O(1)
 * Returns the pointer to the list in the arena or NULL in case of errors
 */
SList * slist_init(void *arena, size_t capacity)
{
    if ((arena == NULL) || (capacity == 0) || (capacity == SLIST_NIL)){
        return NULL;
    }

    /* point to the end of the SList struct */
    uint8_t *mem = (uint8_t*)arena;
    mem = &mem[sizeof(SList)];

    /* set list values */
    SList *list = (SList*)arena;
    list->size = capacity;
    list->len = 0;
    list->head = SLIST_NIL;
    list->items = (SListItem*)mem;

    /* initialize the free list head */
    list->free = 0;
    list->items[list->free].next = (list->size > 1)?1:SLIST_NIL;
    /* the free list is not completely initialized to avoid an O(n) operation */

    return list;
} /* slist_init */

/* Number of items in the list.
 * Time complexity: O(1)
 */
size_t slist_len(const SList *list)
{
    if (list == NULL){
        return 0;
    }
    return list->len;
} /* slist_len */

/* same as
 * slist_len(list) == 0
 */
bool slist_isempty(SList *list)
{
    if (list == NULL){
        return true;
    }

    return list->len == 0;
} /* slist_isempty */

/* Return true if the list has reached its maximum capacity */
bool slist_isfull(SList *list)
{
    if (list == NULL){
        return true;
    }

    return list->len == list->size;
} /* slist_isfull */

/* Return true if the iterator has reached the end of the list */
bool slist_exhausted(SListIter it)
{
    return it.curr == SLIST_NIL;
} /* slist_exhausted */

/* Get the value pointed by the iterator.
 * Return NULL if iterator is exhausted.
 */
void * slist_value(SListIter it)
{
    if (it.curr == SLIST_NIL){
        return NULL;
    }

    assert(it.list != NULL);
    assert(it.curr < it.list->size);

    return it.list->items[it.curr].value;
} /* slist_value */

/* Start an iterator and returns the first value.
 * Return NULL if no items present in the list.
 */
void * slist_iter(SListIter *it, SList *list)
{
    if (list == NULL || it == NULL){
        return NULL;
    }

    it->prev = SLIST_NIL;
    it->curr = list->head;
    it->list = list;

    return slist_value(*it);
} /* slist_iter */

/* Move the iterator to the next element, if possible.
 * Return if the operation succeeded.
 */
bool slist_next(SListIter *it)
{
    if (it == NULL){
        return false;
    }
    if (it->curr == SLIST_NIL){
        return false;
    }

    it->prev = it->curr;
    it->curr = it->list->items[it->curr].next;

    assert(it->curr < it->list->size || it->curr == SLIST_NIL);

    return true;
} /* slist_next */

/* Internal use.
 * Provide the next free item.
 * return the index or SLIST_NIL
 */
size_t _slist_alloc(SList *list)
{
    assert(list != NULL);

    if (list->free == SLIST_NIL){
        return SLIST_NIL;
    }

    assert(list->free < list->size);

    /* alloc the head of free list */
    size_t ind = list->free;
    /* move the freelist head to the next free item */
    list->free = list->items[list->free].next;
    assert(list->free < list->size || list->free == SLIST_NIL);

    list->len++;
    /* If the list is compact, all the items are allocated in [0, len) and
     * the free head then points to items[len].
     * If the list is fragmented, the list has been filled by slist_delete()
     * so free cannot refers to items[len] because it will be the last element
     * of the free list.
     */
    if (list->free == list->len){
        /* freelist runout items */
        list->items[list->free].next = (list->len+1 < list->size)?list->len+1:SLIST_NIL;
    }

    assert(ind < list->size);
    return ind;
} /* _slist_alloc */

/* Internal use.
 * Prepend the item to the free list
 */
void _slist_dealloc(SList *list, size_t item)
{
    assert(list != NULL);
    assert(item < list->size);
    assert(list->len > 0);

    list->items[item].next = list->free;
    list->free = item;
    list->len--;

    assert(list->len < list->size);
    assert(list->free < list->size);
} /* _slist_dealloc */

/* insert value before it, if there is space left.
 * value can be NULL.
 * Return true if value inserted.
 */
bool slist_insert(SListIter *it, void *value)
{
    size_t f = _slist_alloc(it->list);
    if (f == SLIST_NIL){
        return false;
    }
    /* store the value in the previous free head item */
    it->list->items[f].value = value;

    /* prepend to curr */
    it->list->items[f].next = it->curr;

    if (it->prev == SLIST_NIL){
        it->list->head = f; /* new head */
    } else {
        assert(it->prev < it->list->size);
        it->list->items[it->prev].next = f;
    }

    /* the new item is now preceding the current (untouched) */
    it->prev = f;

    return true;
} /* slist_insert */

/* Remove the item refered by it.
 * Return true if the operation succeed.
 */
bool slist_delete(SListIter *it)
{
    if (it == NULL){
        return false;
    }
    if (it->curr == SLIST_NIL){
        return false;
    }

    assert(it->list != NULL);

    size_t item = it->curr;
    if (it->curr == it->list->head){
        assert(it->prev == SLIST_NIL);

        /* update head and iterator to the next element */
        it->curr = it->list->items[it->curr].next;
        it->prev = SLIST_NIL;
        it->list->head = it->curr;
    } else {
        assert(it->prev != SLIST_NIL);

        /* connect previous with the next because current will be removed */
        it->list->items[it->prev].next = it->list->items[it->curr].next;
        it->curr = it->list->items[it->prev].next;
    }

    _slist_dealloc(it->list, item);

    return true;
} /* slist_delete */

/* as stacks, prepend value to the head.
 * Return true if succeed.
 */
bool slist_push(SList *list, void *value)
{
    SListIter it;
    slist_iter(&it, list);
    return slist_insert(&it, value);
} /* slist_push */

/* as stacks, delete the head
 * return the value of the delete head (or NULL if empty)
 */
void * slist_pop(SList *list)
{
    SListIter it;
    slist_iter(&it, list);
    void *v = slist_value(it);
    slist_delete(&it);
    return v;
} /* slist_pop */

#endif

/* Test Single Linked List */

#include "slist.h"
#include "asserts.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

const size_t N = 5;

static
SList * setup()
{
    uint8_t *arena = (uint8_t*)malloc(SLIST_SIZEOF(N));
    SList *list = slist_init(arena, N);
    return list;
}

static
void teardown(SList *list)
{
    free(list);
}

static
void test_init()
{
    puts("slist/test_init");
    /* prepare the arena for N pointers */
    uint8_t *arena = (uint8_t*)malloc(SLIST_SIZEOF(N));
    SList *list;

    /* INIT */
    list = slist_init(NULL, N);
    assert_true(list == NULL, "arena null");

    list = slist_init(arena, 0);
    assert_true(list == NULL, "size zero");

    list = slist_init(arena, N);
    assert_true(list != NULL, "slist init");
    assert_true(slist_isempty(list), "slist init empty");
    assert_false(slist_isfull(list), "slist init full");

    free(arena);
}

static
void test_iterator()
{
    puts("slist/test_iterator");
    SList *list = setup();

    /* ITERATOR */
    int values[N];

    /* prepend/push to the list */
    for (size_t i=0; i < N; i++){
        values[i] = (int)i;
        bool rc = slist_push(list, &values[i]);
        assert_true(rc, "insert fail");
    }
    assert_true(slist_len(list) == N, "len");

    /* expect to see the values in reversed order */
    int i = N-1;
    SListIter it;
    int *v = slist_iter(&it, list);
    assert_false(slist_exhausted(it), "iter init not exhausted");

    while (!slist_exhausted(it)){
        assert_true(i >= 0, "next overflow");
        assert_true(*v == i, "invalid item");
        i--;
        slist_next(&it);
        v = slist_value(it);
    }
    /* no more items */
    assert_true(slist_exhausted(it), "iter loop exhausted");

    teardown(list);
}

static
void test_delete()
{
    puts("slist/test_delete");
    SList *list = setup();

    int values[N];
    SListIter it;

    /* prepend/push to the list */
    for (size_t i=0; i < N; i++){
        values[i] = (int)i;
        bool rc = slist_push(list, &values[i]);
        assert_true(rc, "insert fail");
    }
    assert_true(slist_len(list) == N, "len");

    /* delete */
    slist_iter(&it, list);
    int i = 0;
    while (!slist_exhausted(it)){
        assert_true(i < (int)N, "next overflow");
        bool del = slist_delete(&it);
        assert_true(del, "delete fails");
        i++;
        /* next is not necessary, delete update the iterator */
    }
    /* no more items */
    assert_true(slist_exhausted(it), "iter loop exhausted");
    assert_true(slist_isempty(list), "iter loop emtpy list");

    teardown(list);
}

static
void test_append()
{
    puts("slist/test_append");
    SList *list = setup();
    SListIter it;
    int v[N];

    slist_iter(&it, list);
    bool add = false;
    add = slist_insert(&it, &v[2]);
    assert_true(add, "insert failed");

    add = slist_insert(&it, &v[0]);
    assert_true(add, "insert failed");

    add = slist_insert(&it, &v[3]);
    assert_true(add, "insert failed");

    slist_iter(&it, list);
    assert_true(slist_value(it) == &v[2], "wrong value");
    assert_true(slist_next(&it), "next failed");

    assert_true(slist_value(it) == &v[0], "wrong value");
    assert_true(slist_next(&it), "next failed");

    assert_true(slist_value(it) == &v[3], "wrong value");
    assert_true(slist_next(&it), "next failed");
    assert_true(slist_exhausted(it), "exhausted failed");
    assert_false(slist_next(&it), "next failed");

    /* POP */
    assert_true(slist_pop(list) == &v[2], "pop failed");
    assert_true(slist_pop(list) == &v[0], "pop failed");
    assert_true(slist_pop(list) == &v[3], "pop failed");
    assert_true(slist_pop(list) == NULL, "pop failed");
    assert_true(slist_isempty(list), "pop emtpy");

    teardown(list);
}

static
void test_mix()
{
    puts("slist/test_mix");
    SList *list = setup();
    SListIter it;
    int v[N];


    /* append */
    slist_iter(&it, list);
    slist_insert(&it, &v[2]);
    slist_insert(&it, &v[0]);
    slist_insert(&it, &v[3]);
    slist_insert(&it, &v[1]);
    slist_insert(&it, &v[4]);

    /* [2,0,3,1,4] -> [2,3,1,4] */
    slist_iter(&it, list);
    slist_next(&it);
    slist_delete(&it); /* delete v[0] */

    /* [2,3,1,4] -> [2,3,4] */
    slist_iter(&it, list);
    slist_next(&it);
    slist_next(&it);
    slist_delete(&it); /* delete v[1] */

    slist_iter(&it, list);
    assert_true(slist_value(it) == &v[2], "wrong delete");
    slist_next(&it);
    assert_true(slist_value(it) == &v[3], "wrong delete");
    slist_next(&it);
    assert_true(slist_value(it) == &v[4], "wrong delete");
    slist_next(&it);
    assert_true(slist_exhausted(it), "wrong delete");

    /* [2,3,4] -> [2,0,3,1,4] */
    slist_iter(&it, list);
    while (!slist_exhausted(it)){
        if (slist_value(it) == &v[3]){
            slist_insert(&it, &v[0]);
        }
        if (slist_value(it) == &v[4]){
            slist_insert(&it, &v[1]);
        }

        slist_next(&it);
    }

    slist_iter(&it, list);
    assert_true(slist_value(it) == &v[2], "wrong insert");
    slist_next(&it);
    assert_true(slist_value(it) == &v[0], "wrong insert");
    slist_next(&it);
    assert_true(slist_value(it) == &v[3], "wrong insert");
    slist_next(&it);
    assert_true(slist_value(it) == &v[1], "wrong insert");
    slist_next(&it);
    assert_true(slist_value(it) == &v[4], "wrong insert");
    slist_next(&it);
    assert_true(slist_exhausted(it), "wrong insert");

    teardown(list);
}

void test_random()
{
    puts("slist/test_random");
    const int M = 1024;
    int *values = (int*)calloc(M, sizeof(M));
    uint8_t *arena = (uint8_t*)malloc(SLIST_SIZEOF(M));
    SList *list = slist_init(arena, M);

    int n = rand() % M;
    for (int i=0; i < n; i++){
        bool rc = slist_push(list, &values[i]);
        assert_true(rc, "push");
    }
    int m = rand() % n;
    for (int i=0; i < m; i++){
        void *p = slist_pop(list);
        assert_true(p != NULL, "pop");
    }

    /* scramble */
    for (int i=0; i < 10 * M; i++){
        SListIter it;
        /* insert between existing items */
        slist_iter(&it, list);
        int o = rand() % (slist_len(list));
        for (int j=0; j < o; j++){
            slist_next(&it);
        }
        bool rc = slist_insert(&it, &values[0]);
        assert_true(rc, "insert");

        /* delete a random item */
        slist_iter(&it, list);
        int p = rand() % (slist_len(list));
        for (int j=0; j < p; j++){
            slist_next(&it);
        }
        rc = slist_delete(&it);
        assert_true(rc, "delete");
    }

    free(arena);
}


int main()
{
    test_init();
    test_iterator();
    test_delete();
    test_append();
    test_mix();
    test_random();

    puts("OK");

    return 0;
}

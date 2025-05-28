#include "objpool.h"
#include "asserts.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int a;
    char s[3];
} MyStruct;

#define MYSTRUCT_MAX 5

int main()
{
    /* allocate enough bytes for MYSTRUCT_MAX MyStruct instances */
    void *arena = malloc(OBJPOOL_SIZEOF(MYSTRUCT_MAX, sizeof(MyStruct)));
    ObjPool pool;
    bool rc;

    rc = objpool_init(NULL, arena, MYSTRUCT_MAX, sizeof(MyStruct));
    assert_false(rc, "init not pool");

    rc = objpool_init(&pool, arena, 0, sizeof(MyStruct));
    assert_false(rc, "init not capacity");

    rc = objpool_init(&pool, NULL, MYSTRUCT_MAX, sizeof(MyStruct));
    assert_false(rc, "init not arena");

    rc = objpool_init(&pool, arena, MYSTRUCT_MAX, 0);
    assert_false(rc, "init not size");

    rc = objpool_init(&pool, arena, MYSTRUCT_MAX, sizeof(MyStruct));
    assert_true(rc, "init");

    MyStruct *tmp[MYSTRUCT_MAX];
    for (int i=0; i < MYSTRUCT_MAX; i++){
        tmp[i] = objpool_acquire(&pool);
        assert_false(tmp[i] == NULL, "acquire loop");
        /* initialization */
        tmp[i]->a = i;
        tmp[i]->s[0] = 'a' + i;
        tmp[i]->s[1] = '\0';
    }

    /* no more objects in the pool */
    MyStruct *s = objpool_acquire(&pool);
    assert_true(s == NULL, "acquire NULL");

    /* no crash in case of null */
    objpool_release(NULL, s);
    objpool_release(&pool, s);

    /* release all */
    for (int i=0; i < MYSTRUCT_MAX; i++){
        printf("Release [%i] {a=%i, s=%s}\n", i, tmp[i]->a, tmp[i]->s);
        objpool_release(&pool, tmp[i]);
    }

    /* acquire again */
    for (int i=0; i < MYSTRUCT_MAX; i++){
        /* previsous initialized object are acquired in random order */
        tmp[i] = objpool_acquire(&pool);
        printf("Acquire [%i] {a=%i, s=%s}\n", i, tmp[i]->a, tmp[i]->s);
        assert_false(tmp[i] == NULL, "acquire again");
        assert_true(tmp[i]->a >= 0, "acquire again checks");
        assert_true(tmp[i]->a < MYSTRUCT_MAX, "acquire again checks");
        assert_true(tmp[i]->s[0] >= 'a', "acquire again checks");
        assert_true(tmp[i]->s[0] < 'a' + MYSTRUCT_MAX, "acquire again checks");
    }
    /* release all in reverse order */
    for (int i=MYSTRUCT_MAX -1 ; i >= 0; i--){
        objpool_release(&pool, tmp[i]);
    }

    free(arena);

    puts("OK");
    return 0;
}

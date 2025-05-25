#include "range.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

RangeType test_range_i = 0;

void assert_true(bool exp, const char *msg)
{
    if (!exp){
        puts(msg);
        abort();
    }
}

RangeType test_range_callback(Range r, RangeType i)
{
    assert_true(strcmp(r.name,"Test") == 0, "wrong name in callback");
    assert_true(r.start == -5, "wrong start in callback");
    assert_true(r.end == 5, "wrong end in callback");
    test_range_i = i;

    /* in real scenaio here that can be an abort */

    /* Since it does not abort, return a sage index */
    return r.start;
}

int main()
{
    int std[RANGE_SIZE(-5, 5)];
    Range r = range_init("Test", -5, 5, test_range_callback);

    /* from range index to canonical index */
    assert_true(range_at(r, -5) == 0, "-5 -> 0");
    assert_true(range_at(r, -4) == 1, "-4 -> 1");
    assert_true(range_at(r, -3) == 2, "-3 -> 2");
    assert_true(range_at(r, -2) == 3, "-2 -> 3");
    assert_true(range_at(r, -1) == 4, "-1 -> 4");
    assert_true(range_at(r,  0) == 5,  "0 -> 5");
    assert_true(range_at(r,  1) == 6,  "1 -> 6");
    assert_true(range_at(r,  2) == 7,  "2 -> 7");
    assert_true(range_at(r,  3) == 8,  "3 -> 8");
    assert_true(range_at(r,  4) == 9,  "4 -> 9");
    assert_true(range_at(r,  5) == 10, "5 -> 10");

    /* from canonical index to range index */
    assert_true(range_of(r, 0) == -5, "0 -> -5");
    assert_true(range_of(r, 1) == -4, "1 -> -4");
    assert_true(range_of(r, 2) == -3, "2 -> -3");
    assert_true(range_of(r, 3) == -2, "3 -> -2");
    assert_true(range_of(r, 4) == -1, "4 -> -1");
    assert_true(range_of(r, 5) ==  0, "5 ->  0");
    assert_true(range_of(r, 6) ==  1, "6 ->  1");
    assert_true(range_of(r, 7) ==  2, "7 ->  2");
    assert_true(range_of(r, 8) ==  3, "8 ->  3");
    assert_true(range_of(r, 9) ==  4, "9 ->  4");
    assert_true(range_of(r, 10) ==  5,"10 -> 5");

    assert_true(range_size(r) == 11, "size");

    /* plain way, using canonical zero based index */
    for (size_t i=0; i < range_size(r); i++){
        std[i] = (int)i;
    }
    assert_true(std[range_at(r, r.end)] == (int)range_size(r) - 1, "plain way check");

    /* index way, using the range index if that value is needed */
    for (RangeType i = r.start; range_in(r, i); i++){
        std[range_at(r, i)] = (int)i;
    }
    assert_true(std[range_at(r, r.end)] == (int)r.end, "index way check");

    /* out of bound, callback */
    /* callback return start -> 0 */
    assert_true(range_at(r, -6) == 0, "at -6");
    assert_true(test_range_i == -6, "callback not called");

    assert_true(range_at(r, 6) == 0, "at 6");
    assert_true(test_range_i == 6, "callback not called");

    puts("OK");

    return 0;
}

#ifndef _DS_RANGE_H
#define _DS_RANGE_H

/* Range Data Structure (for array indexing)
 * Namespace: range
 *
 * Simple interval [min, max] included.
 *
 *  int std[RANGE_SIZE(-90, 90)];
 *  Range r = range_init("Degrees", -90, 90, NULL);
 *
 *  std[range_at(-90)] = 0; // first element
 *  std[range_at(-90)] = 0; // last element
 *
 */

#include <stddef.h>
#include <stdbool.h>

#define RANGE_SIZE(start, end) ((size_t)((end) - (start) + 1))

typedef int RangeType;
typedef struct Range Range;

typedef RangeType (*RangeCallback)(Range r, RangeType i);

/* extreme included */
struct Range {
    RangeType start;
    RangeType end;
    RangeCallback clbk;
    const char *name;
};

/* Create the range from start to end (both included).
 * It stores a callback pointer that will be called in case of index out of
 * bound in the auxiliary methods.
 * The callback is also called by this function if end <= start.
 * The callback can abort the program or return a safe index.
 *
 * name: an identifier for the user, not used by the methods.
 * start, end: can be negative
 * clbk: the callback function. It can be NULL.
 */
Range range_init(const char *name, RangeType start, RangeType end, RangeCallback clbk)
{
    Range r = {.start = start, .end = end, .clbk = clbk, .name = name};

    if (clbk != NULL && end <= start) {
        clbk(r, 0);
    }

    return r;
}

/* Check if the range index is in the boundaries after a change (i++) */
bool range_in(Range r, RangeType i)
{
    return (r.start <= i) && (i <= r.end);
}

/* Convert the range index i into the canonical index zero based */
size_t range_at(Range r, RangeType i)
{
    if (r.clbk != NULL && !range_in(r, i)){
        i = r.clbk(r, i);
    }
    return i - r.start;
}

/* Convert the canonical index i into the range index */
RangeType range_of(Range r, size_t i)
{
    RangeType j = r.start + i;
    if (r.clbk != NULL && !range_in(r, j)){
        j = r.clbk(r, j);
    }
    return j;
}

/* Number of element in the range (or in the array indexed by the range) */
size_t range_size(Range r)
{
    return RANGE_SIZE(r.start, r.end);
}


#endif

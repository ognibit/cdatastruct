#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void assert_true(bool exp, const char *msg)
{
    if (!exp){
        puts(msg);
        abort();
    }
}

void assert_false(bool exp, const char *msg)
{
    if (exp){
        puts(msg);
        abort();
    }
}

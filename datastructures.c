//#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "alearenaloc.h"

def_dynarr(i64);
def_statarr(i64, 10);

int main() {
    i64x10 s = {.data = {1,2,3,4,5,6,7,8,9,10}, .len=10};
    i64s d;

    printf("%d %d\n", isstaticarr(s), isstaticarr(d));
    printf("%lld %lld\n", countof(s.data), countof(d.data));

    foridx(idx, s) {
        printf("%lld ", s.data[idx]);
    }

    return 0;
}

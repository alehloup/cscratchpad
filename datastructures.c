//#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "alearenaloc.h"

def_dynarr(i64);
def_statarr(i64, 10);

int main() {
    i64x10 s;
    i64s d;

    printf("%d %d\n", isstaticarr(s), isstaticarr(d));
    printf("%lld %lld\n", countof(s.data), countof(d.data));

    s.len = 5;
    
    foridx(idx, s) {
        s.data[idx] = idx;
    }

    foridx(idx, s) {
        printf("%lld ", s.data[idx]);
    }

    return 0;
}

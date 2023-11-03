//#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

def_statarr(i64, 10);
def_statarr(i64, 164);
def_dynarr(i64);

void test(arena scratch) {
    i64x10 s = {.data = {1,2,3,4,5,6,7,8,9,10}, .len=10};
    i64s d = {};

    // printf("%d %d\n", isstaticarr(s), isstaticarr(d));
    // printf("%lld %lld\n", countof(s.data), countof(d.data));

    // foridx(idx, s) {
    //     printf("%lld ", s.data[idx]);
    // }
    // printf("\n");

    foridx(idx, d) {
        printf("%lld ", d.data[idx]);
    }
    printf("\n");

    fori(164) {
        *push(d, scratch) = i;
        printf("%lld ", d.data[i]);
    }

    // foridx(idx, d) {
    //     printf("%lld ", d.data[idx]);
    // }
    // printf("\n");
}

int main() {

    arena scratch = newarena(2 << 14);
    test(scratch);

    return 0;
}

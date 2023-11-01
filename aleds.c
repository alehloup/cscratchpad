//#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "alearena.h"

//dynarr have their typename as type##s, i.e: i64s
#define def_dynarr(typ)          \
    typedef struct typ##s typ##s;\
    struct typ##s {              \
        isize len; isize cap;    \
        isize start; b32 invalid;\
        typ *arr;                \
    }                            \
//end of def_dynarr  

//statarr have their typename as type##x##count, i.e: i64x10
#define def_statarr(typ, count)                 \
    typedef struct typ##x##count typ##x##count; \
    struct typ##x##count {                      \
        isize len; isize cap;                   \
        isize start; b32 invalid;               \
        typ arr[(count)];                       \
    }                                           \
//end of def_statarr

#define foridx(var, array) forrange(var, array.start, array.start + array.len, 1)

def_dynarr(i64);
def_statarr(i64, 10);

int main() {
    i64x10 s;
    i64s d;

    printf("%d %d\n", isstaticarr(s.arr), isstaticarr(d.arr));
    printf("%lld %lld\n", countof(s.arr), countof(d.arr));

    s.len = 5;
    
    foridx(idx, s) {
        s.arr[idx] = idx;
    }

    foridx(idx, s) {
        printf("%lld ", s.arr[idx]);
    }

    return 0;
}

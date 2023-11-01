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
        typ *data;                \
    }                            \
//end of def_dynarr  

//statarr have their typename as type##x##count, i.e: i64x10
#define def_statarr(typ, count)                 \
    typedef struct typ##x##count typ##x##count; \
    struct typ##x##count {                      \
        isize len; isize cap;                   \
        isize start; b32 invalid;               \
        typ data[(count)];                       \
    }                                           \
//end of def_statarr

#define foridx(var, array) forrange(var, array.start, array.start + array.len, 1)
#define isstaticarr(a)  (countof(a.data) > 8) //u8 dynarr count == ptr_size / 8 == 8

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

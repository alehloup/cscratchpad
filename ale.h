#pragma once

#include <string.h> //memset memcpy
#include <stdlib.h> //malloc
#include <stdio.h> //printf

//better assert
#define assert(c)  while (!(c)) __builtin_unreachable()

//useful constants
#define _1GB   1073741824
#define _512MB  536870912
#define _256MB  268435456
#define _128MB  134217728
#define _64MB    67108864
#define _32MB    33554432
#define _16MB    16777216
#define  _8MB     8388608
#define _19_ones 1111111111111111111

//better type names
typedef long long i64; typedef long i32; typedef short i16; typedef signed char i8;
typedef unsigned long long u64; typedef unsigned long u32; typedef unsigned short u16; typedef unsigned char u8;
typedef long long isize; typedef unsigned long long usize;
typedef float f32; typedef double f64;
typedef i32 b32; //boolean

//Pythonesque
#define True 1l
#define False 0l
#define and &&
#define or ||
#define not !

//Not null pointer parameter
#define REF [static 1] /* NOT NULL pointer parameter*/

//convenient way to define a function pointer
#define def_funcp(ret, name, ...) typedef ret (*name)(__VA_ARGS__)

//better names for size operations
#define sizeof(x)       ((isize)sizeof(x))
#define alignof(x)      ((isize)_Alignof(x))
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define cstrlen(s) __extension__ ({          \
    isize cOunt_ = countof(s) - 1;           \
    cOunt_ != 7 ? cOunt_ : (isize)strlen(s); \
}) // count returns pointer size for dyn cstr so... hacky fix

//for shortcuts
#define loop(var, times) for(isize var = 0; var < times; ++var)
#define fori(times) loop(i, times)
#define forj(times) loop(j, times)
#define for_k(times) loop(k, times)
#define forrange(var, from, to, inc)          \
    for(isize var = (from), var##_TO__ = (to);\
        var != var##_TO__; var+=inc)          \
//end of forrange

#define printn printf("\n")

/*
    STRINGS
*/
typedef struct s8{ isize len; u8 *data; }s8;
#define s8(s) (s8){ cstrlen(s), (u8 *)s }
#define s8equal(s1, s2) (s1.len != s2.len ? False \
    : (b32) not memcmp(s1.data, s2.data, (u64)s1.len))
#define s8substr(s, from, count) \
    ((s8){ .data = (s.data)+from, .len = count })
#define s8print(s) printf("%.*s", (int)s.len, s.data)

//TRICK scope that "opens" at start, and "closes" at end 
//(careful, if returns mid scope |end| will never run)
int MACRO_scoped__;
#define scoped(start, end) MACRO_scoped__ = 1;         \
    for(start; MACRO_scoped__; (--MACRO_scoped__), end)\
//end of scoped

//Fast % when the number is a power of 2
#define MODPWR2(number, modval) ((number) & (modval - 1))

//One liner Pseudo Random generator
static _Thread_local u64 MACRO_rnd64_seed__ = 0;
#define RNDSEED(x) ((MACRO_rnd64_seed__) = (u64)(x) >> 1)
#define RND64() ((MACRO_rnd64_seed__) = ((MACRO_rnd64_seed__) * 0x3FFFFBFFFFF + 0x7A5662DCDF) >> 1)
#define RNDN(n) (RND64() % (n))

/*
    ARENA
*/

typedef struct arena{ u8 *beg; u8 *end; }arena;

__attribute((malloc, alloc_size(2, 4), alloc_align(3)))
void *alloc(arena a REF, isize size, isize align, isize count) {
    isize total = size * count;
    isize pad = MODPWR2(- (isize)a->beg, align); //mod -x gives n for next align

    if (total > (a->end - a->beg - pad)) {
        printf("OUT OF MEMORY, Ask:%lld Avail: %lld\n", total, a->end - a->beg - pad);
        assert(total < (a->end - a->beg - pad)); //ARENA OUT OF MEMORY
        return 0; //ARENA OUT OF MEMORY
    }

    u8 *p = a->beg + pad;
    a->beg += pad + total;
    
    return memset(p, 0, total);
}

#define newx(a, t) (t *)alloc(a, sizeof(t), alignof(t), 1)
#define newxs(a, t, n) (t *)alloc(a, sizeof(t), alignof(t), n)

arena newarena(isize cap) {
    arena a = {0};
    a.beg = (u8 *)malloc(cap);
    a.end = a.beg ? a.beg + cap : 0;
    return a;
}

/*
    ARRAY
*/

#define ARR_HEADER_DATA(type)     \
        isize len; isize cap;     \
        isize start; b32 invalid; \
        type *data                \

#define foridx(var, array) forrange(var, array.start, array.start + array.len, 1)
#define isstaticarr(a)  (countof(a.data) > 8) //u8 dynarr count == ptr_size / 8 == 8
#define print_arr(label, _ArR__, format) __extension__ ({              \
    printf("\n=== %s ===\n", label);                                   \
    printf("start:%lld len:%lld cap:%lld invalid:%ld\n",               \
        (_ArR__).start, (_ArR__).len, (_ArR__).cap, (_ArR__).invalid); \
    foridx(_pridx__, (_ArR__)) {                                       \
        printf("|");                                                   \
        printf(format, (_ArR__).data[_pridx__]);                       \
        printf("| ");                                                  \
    }                                                                  \
    printf("\n----------------\n");                                    \
    (_ArR__).invalid;                                                  \
})

void grow(void *slice /*arr struct*/, isize size, isize align, arena *a) {
    struct{ARR_HEADER_DATA(u8);} replica = {0};
    memcpy(&replica, slice, sizeof(replica));

    replica.cap *= 2; 

    if (!replica.data) { //empty array
        replica.cap = 64;
        replica.data = alloc(a, size, align, replica.cap);
    } else if (a->beg == (replica.data + replica.cap * size)) { //extend array
        alloc(a, size, 1, replica.cap >> 1);
    } else { // rellocate array
        u8 *data = alloc(a, size, align, replica.cap);
        memcpy(data, replica.data + replica.start, size*replica.len);
        replica.data = data;
    }

    memcpy(slice, &replica, sizeof(replica));
}

#define push(dynarr, arena) __extension__ ({  \
    typeof(dynarr) dynarr_ = (dynarr);        \
    typeof(arena) arena_ = (arena);           \
                                              \
    if (dynarr_->len >= dynarr_->cap) {       \
        grow(dynarr_, sizeof(*dynarr_->data), \
            alignof(*dynarr_->data), arena_); \
    }                                         \
    dynarr_->data + dynarr_->len++;           \
})

/*
    MATRIX alloc (just to remember the VLA matrix ALLOC syntax)
    i64 (*p_matrix_vla)[m][n] =  malloc(sizeof(*p_matrix_vla));

    To be used as:  (*p_matrix_vla)[i][j]

    As function return type its a pointer to the type: i64 *
*/ 
#define NEWMATRIX_VAR(type, var, m, n, arena_) \
    type (*var)[m][n] = alloc(arena_, sizeof(type [m][n]), alignof(type [m][n]), 1);

//to be used as:  (*var)[i][j], has nice type safety
#define matat(mat, i, j) (*mat)[i][j]

/*
    HASH'n'TABLE
*/

/* 
    mask-step-index (MSI) Hash Table from Null Program 
    Capacity needs to be a power of 2, an example for 2^15:
        capmask = 32767 (2^15 - 1)
        shiftstep = 49 (that is 64 - 15)
*/ 
i32 ht_lookup(u64 hash_ /* 1st hash, "access" a "list of elements" for it */, 
              i32 index_ /* 2nd hash, iterates over the "list of elements"*/, 
              i32 shiftstep /* Fixed, 64 - Exp */, 
              i32 capmask /* Fixed, 2^Exp - 1*/) 
{
    u32 step = (u32)((hash_ >> shiftstep) | 1); //odd makes cycle impossible
    return (index_ + step) | capmask; 
}

u64 hash_s8(s8 str) {
    u64 h = 0x7A5662DCDF;
    fori(str.len) { 
        h ^= str.data[i] & 255; h *= _19_ones;
    }
    return h ^ h>>32;
}
u64 hash_i64(i64 x_) {
    u64 x = (u64)x_;
    x ^= x >> 30; x *= 0xbf58476d1ce4e5b9; 
    x ^= x >> 27; x *= 0x94d049bb133111eb; 
    return x ^ x>>31;
}
u64 hash_i32(i32 x_)
{
    u32 x = (u32)x_;
    x ^= x >> 16; x *= 0x7feb352d; 
    x ^= x >> 15; x *= 0x846ca68b; 
    return x ^ x>>16;
}

//deprecated, will need to be fixed:
#define ht_foridx_print(var, label, _HaSH_tAble__, print_statement) __extension__ ({  \
    typeof((_HaSH_tAble__).data) _data__ = (_HaSH_tAble__).data;                      \
    typeof(_data__[0].key) _zero_key__ = {0};                                         \
    typeof(_data__[0]) var = {0};                                                     \
    printf("\n=== %s ===\n", label);                                                  \
    printf("start:%lld len:%lld cap:%lld invalid:%ld\n",                              \
        (_HaSH_tAble__).start, (_HaSH_tAble__).len,                                   \
        (_HaSH_tAble__).cap, (_HaSH_tAble__).invalid);                                \
    forrange(_pridx__, 0, (_HaSH_tAble__).cap, 1) {                                   \
        var = _data__[_pridx__];                                                      \
        if (not memcmp(&(var.key), &_zero_key__, sizeof(_zero_key__)))                \
            continue;                                                                 \
        printf("|");                                                                  \
        print_statement;                                                              \
        printf("| ");                                                                 \
    }                                                                                 \
    printf("\n----------------\n");                                                   \
    (_HaSH_tAble__).invalid;                                                          \
})



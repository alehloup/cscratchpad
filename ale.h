#pragma once

#include <string.h> //memset memcpy
#include <stdlib.h> //malloc
#include <stdio.h> //printf

//better assert
#define assert(c)  while (!(c)) __builtin_unreachable()

/*
    CONSTANTS
*/
#define _1GB   1073741824
#define _512MB  536870912
#define _256MB  268435456
#define _128MB  134217728
#define _64MB    67108864
#define _32MB    33554432
#define _16MB    16777216
#define  _8MB     8388608
#define _19_ones 1111111111111111111

/* 
    PRIMITIVES
*/
typedef long long i64; typedef long i32; typedef short i16; typedef signed char i8;
typedef unsigned long long u64; typedef unsigned long u32; typedef unsigned short u16; typedef unsigned char u8;
typedef long long isize; typedef unsigned long long usize;
typedef float f32; typedef double f64;
typedef i32 b32; //boolean

/*
    COMPOSITES
*/
#define lendata(type)    i32 len; type *data // slice
#define caplendata(type) i32 cap; lendata(type) // growable array
#define keyval(keyt, valt) keyt key; valt val // entry fields
#define tkeyval(keyt, valt) struct{ keyval(keyt, valt); } //struct entry

#define datatypeof(table) typeof(((table)->data))
#define keytypeof(table) typeof(((table)->data[0].key))
#define valtypeof(table) typeof(((table)->data[0].val))

/*
    VLA MATRIX 
    i64 (*vla_matrix_pointer)[m][n] =  malloc(sizeof(*vla_matrix_pointer))
*/ 
#define NEWMATRIX_VAR(type, var, m, n, arena_) \
    type (*var)[m][n] = alloc(arena_, sizeof(type [m][n]), alignof(type [m][n]), 1);
#define matat(mat, i, j) (*mat)[i][j]

/*
    LAZY PYTHONESQUE
*/
#define True 1l
#define False 0l
#define and &&
#define or ||
#define not !
#define printn printf("\n")
#define print(...) printf(__VA_ARGS__); printf("\n")
 
#define REF [static 1] /* NOT NULL pointer parameter*/
#define def_funcp(ret, name, ...) typedef ret (*name)(__VA_ARGS__) //define a function pointer
#define threadlocal static _Thread_local//thread variable

//TRICK scope that "opens" at start, and "closes" at end 
threadlocal b32 MACRO_scoped__;
#define scoped(start, end) MACRO_scoped__ = 1;         \
    for(start; MACRO_scoped__; (--MACRO_scoped__), end)

/* 
    MEMORYops
*/
#define sizeof(x)       ((isize)sizeof(x))
#define alignof(x)      ((isize)_Alignof(x))
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define cstrlen(s) __extension__ ({             \
    isize cOunt_ = countof(s) - 1;              \
    cOunt_ != 7 ? (i32)cOunt_ : (i32)strlen(s); \
}) // count returns pointer size for dyn cstr so... hacky fix
#define memequal(x1, x2) (sizeof(x1) != sizeof(x2) ? False : not memcmp(&x1, &x2, sizeof(x1)))

/*
    FORs
*/
#define loop(var, times) for(isize var = 0; var < times; ++var)
#define fori(times) loop(i, times)
#define forj(times) loop(j, times)
#define for_k(times) loop(k, times)
#define forrange(var, from, to, inc)          \
    for(isize var = (from), var##_TO__ = (to);\
        var != var##_TO__; var+=inc)

/*
    STRINGS
*/
typedef struct s8{ i32 len; u8 *data; }s8;
#define s(cstr) ((s8){ cstrlen(cstr), (u8 *)cstr })

s8 s8substr(s8 s, i32 from, i32 count) {
    return (s8){ .data = (s.data)+from, .len = count };
}
b32 s8equal(s8 s1, s8 s2) {
    return s1.len != s2.len ? False : \
        (s1.len == 0 ? True : not memcmp(s1.data, s2.data, s1.len));
}
void s8print(s8 s) {
    printf("%.*s", (int)s.len, s.data);
} 
#define s8printn(s8str) s8print(s8str); printn  

/*
    MATH
*/

//Fast % when the number is a power of 2
#define MODPWR2(number, modval) ((number) & (modval - 1))

// Returns first power 2 that size+1 fits (it starts at 2^9 == 512)
i32 fit_pwr2_exp(i64 size) {
    i32 exp=9; i64 val=512; ++size;
    while (val < size) {
        ++exp; val*=2;
    }
    return exp;
}

/*
    RANDOM
*/
threadlocal u64 MACRO_rnd64_seed__ = _19_ones;
#define RNDSEED(x) ((MACRO_rnd64_seed__) = (u64)(x) >> 1)
#define RND64() (MACRO_rnd64_seed__ = hash_i64(MACRO_rnd64_seed__))
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
    GROWABLE ARRAY
*/

void grow(void *slice /*slice struct*/, isize size, isize align, arena *a) {
    struct{caplendata(u8);} replica = {0};
    memcpy(&replica, slice, sizeof(replica)); //type prunning

    if (!replica.data) { 
        replica.data = alloc(a, size, align, replica.cap = 64); // empty: default to 64
    } else if (a->beg == (replica.data + replica.cap * size)) { 
        alloc(a, size, 1, replica.cap); // neighbour mem avail: extend array
        replica.cap *= 2;
    } else {
        u8 *data = alloc(a, size, align, replica.cap *= 2); // reloc to 2*current cap
        memcpy(data, replica.data, size*replica.len);
        replica.data = data;
    }

    memcpy(slice, &replica, sizeof(replica)); //type prunning
}

#define push(dynarr, arena) __extension__ ({  \
    typeof(dynarr) dynarr_ = (dynarr);        \
    typeof(arena) arena_ = (arena);           \
                                              \
    if (dynarr_->len >= dynarr_->cap) {       \
        grow(dynarr_, sizeof(*dynarr_->data), alignof(*dynarr_->data), arena_); \
    }                                         \
    dynarr_->data + dynarr_->len++;           \
})

/*
    HASH'n'TABLE
*/

u64 hash_s8(s8 str) {
    u64 h = 0x7A5662DCDF;
    fori(str.len) { 
        h ^= str.data[i] & 255; h *= _19_ones;
    }
    return h ^ h>>32;
}
u64 hash_cstr(char *str) {
    return hash_s8(s(str));
}
u64 hash_i64(i64 int64) {
    u64 x = (u64)int64;
    x ^= x >> 30; x *= 0xbf58476d1ce4e5b9; 
    x ^= x >> 27; x *= 0x94d049bb133111eb; 
    return x ^ x>>31;
}
u64 hash_i32(i32 int32)
{
    u32 x = (u32)int32;
    x ^= x >> 16; x *= 0x7feb352d; 
    x ^= x >> 15; x *= 0x846ca68b; 
    return x ^ x>>16;
}

#define hash_it(X) _Generic((X), char *: hash_cstr, s8: hash_s8, i32: hash_i32, default: hash_i64)(X)

threadlocal i32 msi_expo = 16; // default MSI exp: 2^16 = capacity of 65536 - 1 elements
threadlocal i32 msi_mask = 65535; // capacity - 1
threadlocal i32 msi_step = 48; // 64 - exp
//Mask-Step-Index (MSI) Hash Table
i32 msi_lookup(u64 hash, // 1st hash acts as base location
              i32 index // 2nd "hash" steps over the "list of elements" from base-location
            )
{
    u32 step = (u32)(hash >> msi_step) | 1;

    return (index + step) & msi_mask;
}

void msi_set_maxn_elements(isize size) {
    msi_expo = fit_pwr2_exp(size);
    msi_mask = (1 << msi_expo) - 1;
    msi_step = 64 - msi_expo;
}

#define msi_newdata(arena, table, expected_maxn) __extension__ ({\
    msi_set_maxn_elements(expected_maxn); \
    newxs(&a, typeof(*((table)->data)), msi_mask + 1); \
})
#define newmsi(arena, varname, expected_maxn) {.data = msi_newdata(arena, &varname, expected_maxn)};

#define msi_upsert(table, key_, val_) __extension__ ({    \
    datatypeof(table) data = (table)->data;               \
    keytypeof(table) searchk = (keytypeof(table))key_;    \
    valtypeof(table)  setval = (valtypeof(table))val_;    \
    typeof(searchk) zero_key = {0};                       \
    typeof(setval)  zero_val = {0};                       \
                                                          \
    u64 hash = hash_it(searchk);                          \
    i32 index = (i32)hash;                                \
    index = msi_lookup(hash, index);                      \
    for(;                                                 \
        not memequal(data[index].key, searchk);           \
        index = msi_lookup(hash, index))                  \
    {                                                     \
        if(memequal(data[index].key, zero_key)){          \
            if(not memequal(setval, zero_val)) {          \
                data[index].key = searchk;                \
            }                                             \
            break;                                        \
        }                                                 \
    }                                                     \
    if(not memequal(setval, zero_val)) {                  \
        data[index].val = setval;                         \
    }                                                     \
    index;                                                \
})
#define msi_only_get {0}
#define msi_get_idx(table, key_) __extension__ ({msi_upsert(&table, key_, msi_only_get);})
#define msi_get(table, key_) __extension__({(table).data[msi_get_idx(table, key_)].val;})


#pragma once

#include <string.h> // memset memcpy memcmp
#include <stdlib.h> // system (malloc is not used)
#include <stdio.h> // printf sprintf
#include <stdarg.h> //variadic functions

typedef long long int64; 
typedef unsigned char u8; typedef unsigned int uint; typedef unsigned long long uint64;

const int True = 1; const int False = 0;
#define MegaBytes 1048576 //constexpr uint64 MegaBytes = 1048576;

#ifndef NDEBUG
#define assert(_cond_, ...) __extension__ ({         \
    while (!(_cond_)) {                              \
        printf("\n!! [%s:%d] ", __FILE__, __LINE__); \
        printf(__VA_ARGS__);                         \
        printf(" !!\n");                             \
        __builtin_unreachable();                     \
    }                                                \
})
#endif
#ifdef NDEBUG
#define assert(_cond_, ...) __extension__ ({})
#endif 

/*
    COMPOSITES
*/
#define    lendata(type)          int len; type *data // slice
#define caplendata(type) int cap; lendata(type) // growable array

#define keyval(keyt, valt) keyt key; valt val // entry 
#define tkeyval(keyt, valt) struct{ keyval(keyt, valt); } // struct entry
#define msi_ht_data(keytype_, valtype_) \
    int stepshift; int capmask; lendata(tkeyval(keytype_, valtype_)) // msi hash table

#define datatypeof(_table_) typeof(((_table_)->data)) // get array type
#define keytypeof(_table_) typeof(((_table_)->data[0].key)) // get ht key type
#define valtypeof(_table_) typeof(((_table_)->data[0].val)) // get ht val type

#define NEWMATRIX_VAR(type, var, m, n, arena_) \
    type (*var)[m][n] = alloc(arena_, sizeof(type [m][n]), alignof(type [m][n]), 1);
#define matat(mat, i, j) (*mat)[i][j]

#define printn printf("\n")
#define print(...) printf(__VA_ARGS__); printf("\n")
 
#define REF [static 1] /* NOT NULL pointer parameter*/
#define def_funcp(ret, name, ...) typedef ret (*name)(__VA_ARGS__) //define a function pointer
#define threadlocal static _Thread_local//thread variable

//TRICK scope that "opens" at start, and "closes" at end 
threadlocal int MACRO_scoped__;
#define scoped(start, end) MACRO_scoped__ = 1;         \
    for(start; MACRO_scoped__; (--MACRO_scoped__), end)

/* 
    MEMORYops
*/
#define   sizeof(x)      ((int64)sizeof(x))
#define  alignof(x)      ((int64)_Alignof(x))
#define  countof(a)      (sizeof(a) / sizeof(*(a)))
#define memequal(x1, x2) (sizeof(x1) != sizeof(x2) ? False : ! memcmp(&x1, &x2, sizeof(x1)))

/*
    STRINGS
*/
typedef struct s8{ int len; u8 *data; }s8;

#define s(cstr) ((s8){ \
    countof(cstr) == 8? (int)strlen(cstr) : (int) (countof(cstr) - 1), (u8 *)cstr \
})

int s8equal(s8 s1, s8 s2) {
    return s1.len != s2.len ? False : \
        (s1.len == 0 ? True : ! memcmp(s1.data, s2.data, s1.len));
}
void print_s8(s8 s) {
    printf("%.*s", (int)s.len, s.data);
}

/*
    SHELL
*/
int shellrun(char buffer [static 256], ...) {
    memset(buffer, '\0', 256);

    va_list args; va_start(args, buffer);
    char *format = va_arg(args, char *);

    vsprintf(buffer, format, args);
    return system(buffer);
}

/*
    MATH
*/
//Fast % when the number is a power of 2
#define MODPWR2(number, modval) ((number) & (modval - 1))

// Returns first power 2 that size+1 fits (it starts at 2^9 == 512)
int fit_pwr2_exp(int size) {
    int exp=9; int val=512; ++size;
    while (val < size) {
        ++exp; val*=2;
    }
    return exp;
}

/*
    RANDOM
*/
uint64 hash_int64(int64 integer64); // defined more down low
threadlocal uint64 MACRO_rnd64_seed__ = 1111111111111111111;
void SET_RND_SEED(uint64 x) { (MACRO_rnd64_seed__) = (uint64)(x) >> 1; }
uint64 RND64() { return MACRO_rnd64_seed__ = hash_int64(MACRO_rnd64_seed__) >> 1; }
int RNDN(int n) { return (int) RND64() % n; }

/*
    ARENA
*/
typedef struct arena{ u8 *beg; u8 *end; }arena;

__attribute((malloc, alloc_size(2, 4), alloc_align(3)))
void *alloc(arena a REF, int64 size, int64 align, int64 count) {
    int64 total = size * count;
    int64 pad = MODPWR2(- (int64)a->beg, align); //mod -x gives n for next align

    assert(total < (a->end - a->beg - pad), \
        "ARENA OUT OF MEMORY Ask:%lld Avail: %lld\n", total, a->end - a->beg - pad);

    u8 *p = a->beg + pad;
    a->beg += pad + total;
    
    return memset(p, 0, total);
}

#define newx(a, t) (t *)alloc(a, sizeof(t), alignof(t), 1)
#define newxs(a, t, n) (t *)alloc(a, sizeof(t), alignof(t), n)

arena newarena(int64 cap, u8 buffer[static cap]) {
    arena a = {0};
    a.beg = buffer;
    a.end = a.beg ? a.beg + cap : 0;
    return a;
}

/*
    GROWABLE ARRAY
*/
void grow(void *slice /*slice struct*/, int64 size, int64 align, arena *a) {
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

#define push(dynarr, arena, _value_) __extension__ ({  \
    typeof(dynarr) dynarr_ = (dynarr);                 \
    typeof(arena) arena_ = (arena);                    \
                                                       \
    if (dynarr_->len >= dynarr_->cap) {                \
        grow(dynarr_, sizeof(*dynarr_->data),          \
            alignof(*dynarr_->data), arena_);          \
    }                                                  \
    dynarr_->data[dynarr_->len++] = _value_;           \
})

#define pop(array) __extension__ ({                    \
    assert((array)->len > 0, "POP ON EMPTY ARRAY");    \
    (array)->data[--(array)->len];                     \
})

/*
    HASH
*/
uint64 hash_s8(s8 str) {
    uint64 h = 0x7A5662DCDF;
    for(int i = 0; i < str.len; ++i) { 
        h ^= str.data[i] & 255; h *= 1111111111111111111;
    }
    return h ^ h>>32;
}
uint64 hash_cstr(char *str) {
    return hash_s8(s(str));
}
uint64 hash_int64(int64 integer64) {
    uint64 x = (uint64)integer64;
    x ^= x >> 30; x *= 0xbf58476d1ce4e5b9; 
    //x ^= x >> 27; x *= 0x94d049bb133111eb; 
    return x ^ x>>31;
}
uint64 hash_int32(int integer32)
{
    uint x = (uint)integer32;
    x ^= x >> 16; x *= 0x7feb352d; 
    //x ^= x >> 15; x *= 0x846ca68b; 
    return x ^ x>>16;
}

#define hash_it(X) _Generic((X), \
     char *: hash_cstr, s8: hash_s8, int: hash_int32, default: hash_int64)(X)

/*
    HASH TABLE : Mask-Step-Index (MSI) 
*/
int msi_lookup(uint64 hash, // 1st hash acts as base location
              int index, // 2nd "hash" steps over the "list of elements" from base-location
              int capmask, // trims number to < ht_capacity
              int stepshift // use |exp| bits for step 
            )
{
    uint step = (uint)(hash >> stepshift) | 1;

    return (index + step) & capmask;
}

#define newmsi(arena_, varname, expected_maxn) __extension__ ({          \
    int msi_expo = fit_pwr2_exp(expected_maxn);                          \
    int msi_mask = (1 << msi_expo) - 1;                                 \
    int msi_step = 64 - msi_expo;                                        \
    typeof(varname) temp_ht_ = {                                        \
        .capmask = msi_mask, .stepshift = msi_step,                     \
        .data = newxs(arena_, typeof(*((varname).data)), msi_mask + 1)  \
    };                                                                  \
    temp_ht_;                                                           \
});

#define msi_idx(table, key_, msi_insert_if_not_found) __extension__ ({ \
    datatypeof(table) data = (table)->data;                   \
    keytypeof(table) searchk = (keytypeof(table))key_;        \
    typeof(searchk) zero_key = {0};                           \
                                                              \
    uint64 hash = hash_it(searchk);                              \
    int index = (int)hash;                                    \
    index = msi_lookup(hash, index,                           \
        (table)->capmask, (table)->stepshift);                \
    for(;                                                     \
         ! memequal(data[index].key, searchk);               \
        index = msi_lookup(hash, index,                       \
            (table)->capmask, (table)->stepshift))            \
    {                                                         \
        if(memequal(data[index].key, zero_key)){              \
            if(msi_insert_if_not_found) {                     \
                data[index].key = searchk;                    \
            }                                                 \
            break;                                            \
        }                                                     \
    }                                                         \
    index;                                                    \
})

#define msi_get(table, key) __extension__({        \
    (table)->data[msi_idx(table, key, False)].val; \
})
#define msi_set(table, key, val_) __extension__({              \
    assert((table)->len < (table)->capmask, "MSI HT IS FULL"); \
    int msi_index_ = msi_idx(table, key, True);                \
    valtypeof(table) msi_current_val                           \
        = (valtypeof(table)) (table)->data[msi_index_].val;    \
    (table)->data[msi_index_].val = val_;                      \
})

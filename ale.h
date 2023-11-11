#pragma once

#include <stdarg.h>   // standard variadic
#include <stdint.h>   // standard ints
#include <string.h>   // memset memcpy memcmp
#include <stdio.h>    // printf sprintf

#define assert(_cond_, ...)                          \
    if (!(_cond_)) {                                 \
        printf("\n!! [%s:%d] ", __FILE__, __LINE__); \
        printf(__VA_ARGS__);                         \
        printf(" !!\n");                             \
        __builtin_unreachable();                     \
    }                                                \

//TRICK scope that "opens" at start, and "closes" at end 
static char MACRO_scoped__;
#define scoped(start, end) MACRO_scoped__ = 1; for(start; MACRO_scoped__; (--MACRO_scoped__), end)

#ifndef false
#define false 0
#define true 1
#endif

#ifdef __cplusplus
#define _at_least_(_size_) /* static _size_ */
#define  alignof(x) ((int64_t)alignof(x))
#define Zero {}
#define cpound(type) /* (type) */
#define threadlocal thread_local
#endif

#ifndef __cplusplus
#define _at_least_(_size_) static _size_
#define  alignof(x) ((int64_t)_Alignof(x))
#define Zero {0}
#define cpound(type) (type)
#define threadlocal _Thread_local
#endif

/* 
    MEMORYops
*/
#define MegaBytes 1048576 //constexpr uint64_t MegaBytes = 1048576;
#define   sizeof(x)      ((int64_t)sizeof(x))
#define  countof(a)      (sizeof(a) / sizeof(*(a)))
#define memequal(x1, x2) (sizeof(x1) != sizeof(x2) ? 0 : ! memcmp(&x1, &x2, sizeof(x1)))

/*
    STRINGS
*/
typedef char * cstring;
typedef const char * const staticstring;
#define cstrlen(str) (countof(str) == 8? (int32_t)strlen(str) : (int32_t) (countof(str) - 1))
typedef struct strslice{ int32_t len; cstring data; }strslice;
static inline strslice cstr_to_slice(int32_t len, staticstring data) {
    strslice temp = {len, (cstring) data};
    return temp;
} 
#define s8(cstr) cstr_to_slice(cstrlen(cstr), cstr)

/*
    SHELL
*/
int32_t __cdecl system(const char *_Command);
static int32_t shellrun(char buffer [_at_least_(512)], ...) {
    memset(buffer, '\0', 512);

    va_list args; va_start(args, buffer);
    char *format = va_arg(args, char *);

    vsprintf(buffer, format, args);
    return system(buffer);
}

/*
    MATH
*/
//Fast mod when the number is a power of 2
static inline int64_t MODPWR2(int64_t number, int64_t modval) {
    return (number) & (modval - 1);
}

// Returns first power 2 that size+1 fits (it starts at 2^9 == 512)
static inline int32_t fit_pwr2_exp(int32_t size) {
    int32_t exp=9; int32_t val=512; ++size;
    while (val < size) {
        ++exp; val*=2;
    }
    return exp;
}

// RANDOM
static inline int32_t RND(uint64_t seed[_at_least_(1)]) {
    *seed = *seed * 0x9b60933458e17d7dLL + 0xd737232eeccdf7edLL;
    int32_t shift = 29 - (uint32_t)(*seed >> 61);
    
    return ((int32_t) (*seed >> shift)) & 2147483647;
}

/*
    ARENA
*/
typedef struct arena{ char *beg; char *end; }arena;
static inline arena newarena(int64_t cap, void * buffer) {
    arena a = Zero;
    a.beg = (char *)buffer;
    a.end = a.beg ? a.beg + cap : 0;
    return a;
}

__attribute((malloc, alloc_size(2, 4), alloc_align(3)))
static void *alloc(arena a[_at_least_(1)], int64_t size, int64_t align, int64_t count) {
    int64_t total = size * count;
    int64_t pad = MODPWR2(- (int64_t)a->beg, align); //mod -x gives n for next align

    assert(total < (a->end - a->beg - pad), \
        "ARENA OUT OF MEMORY Ask:%lld Avail: %lld\n", total, a->end - a->beg - pad);

    char *p = a->beg + pad;
    a->beg += pad + total;
    
    return memset(p, 0, total);
}

#define newx(a, t) (t *)alloc(a, sizeof(t), alignof(t), 1)
#define newxs(a, t, n) (t *)alloc(a, sizeof(t), alignof(t), n)
#define newmat(a, t, m, n) (t (*)[m][n]) alloc(a, sizeof(t [m][n]), alignof(t [m][n]), 1)

/*
    GROWABLE ARRAY
*/
static void grow(
    void *slice /*slice struct*/, 
    int64_t size, int64_t align, arena *a
) {
    struct{int32_t cap; int32_t len; char *data;} replica = Zero;
    memcpy(&replica, slice, sizeof(replica)); //type prunning

    if (!replica.data) { 
        replica.data = (char *) alloc(a, size, align, replica.cap = 64); // empty: default to 64
    } else if (a->beg == (replica.data + replica.cap * size)) { 
        alloc(a, size, 1, replica.cap); // neighbour mem avail: extend array
        replica.cap *= 2;
    } else {
        char *data = (char *) alloc(a, size, align, replica.cap *= 2); // reloc to 2*current cap
        memcpy(data, replica.data, size*replica.len);
        replica.data = data;
    }

    memcpy(slice, &replica, sizeof(replica)); //type prunning
}

/*
    PUSH TO GROWABLE ARRAY
*/
static void push_i64(void *dynarr, arena a[_at_least_(1)], int64_t int64) {
    struct{int32_t cap; int32_t len; int64_t *data;} replica = Zero;
    memcpy(&replica, dynarr, sizeof(replica)); //type prunning

    if (replica.len >= replica.cap) {
        int64_t oldcap = replica.cap;
        grow(&replica, sizeof(int64_t), alignof(int64_t), a);
        assert(replica.cap > oldcap, "GROW FAILED");
    }

    replica.data[replica.len++] = int64;
    
    memcpy(dynarr, &replica, sizeof(replica)); //type prunning
}
static inline void push_double(void *dynarr, arena a[_at_least_(1)], double float64) {
    int64_t replica;
    memcpy(&replica, &float64, sizeof(replica)); //type prunning

    push_i64(dynarr, a, replica);
}
static inline void push_cstr(void *dynarr, arena a[_at_least_(1)], const cstring cstr) {
    push_i64(dynarr, a, (int64_t) cstr);
}
static inline void push_ptr(void *dynarr, arena a[_at_least_(1)], void *ptr) {
    push_i64(dynarr, a, (int64_t) ptr);
}

/*
    POP OF GROWABLE ARRAY
*/
static int64_t pop_i64(void *dynarr) {
    struct{int32_t cap; int32_t len; int64_t *data;} replica = Zero;
    memcpy(&replica, dynarr, sizeof(replica)); //type prunning

    assert(replica.len > 0, "POP ON EMPTY ARRAY");
    int64_t val = replica.data[--replica.len];

    memcpy(dynarr, &replica, sizeof(replica)); //type prunning
    return val;
}
static inline double pop_double(void *dynarr) {
    int64_t replica = pop_i64(dynarr);
    double val = 0;
    memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val; 
}
static inline cstring pop_cstr(void *dynarr) {
     return (cstring) pop_i64(dynarr);
}
static inline void * pop_ptr(void *dynarr) {
     return (void *) pop_i64(dynarr);
}

/*
    HASH
*/
static int64_t hash_cstr(cstring str) {
    uint64_t h = 0x7A5662DCDF;
    while(*str) { 
        h ^= (*(str++)) & 255; h *= 1111111111111111111;
    }

    return (h ^ h>>32) >> 1;
}
static int64_t hash_i64(int64_t integer64) {
    uint64_t x = (uint64_t)integer64;
    x ^= x >> 30; x *= 0xbf58476d1ce4e5b9; 
    
    return (x ^ x>>31) >> 1;
}

#define hash_it(X) _Generic((X), \
     char *: hash_cstr, default: hash_i64)(X)

/*
    HASH TABLE : Mask-Step-Index (MSI) 
*/
static inline int32_t 
    msi_lookup(
        uint64_t hash, // 1st hash acts as base location
        int32_t index, // 2nd "hash" steps over the "list of elements" from base-location
        int32_t capmask, // trims number to < ht_capacity
        int32_t stepshift // use |exp| bits for step 
    )
{
    uint32_t step = (uint32_t)(hash >> stepshift) | 1;
    return (index + step) & capmask;
}

#define newmsi(arena_, varname, expected_maxn) __extension__ ({         \
    int32_t msi_expo = fit_pwr2_exp(expected_maxn);                     \
    int32_t msi_mask = (1 << msi_expo) - 1;                             \
    int32_t msi_step = 64 - msi_expo;                                   \
    typeof(varname) temp_ht_ = {                                        \
        msi_step, msi_mask, 0,                                          \
        newxs(arena_, typeof(*((varname).data)), msi_mask + 1)          \
    };                                                                  \
    temp_ht_;                                                           \
});

#define msi_idx(table, key_, msi_insert_if_not_found) __extension__ ({ \
    typeof(((table)->data)) data = (table)->data;                      \
    typeof(data[0].key) searchk = (typeof(data[0].key))key_;           \
    typeof(searchk) zero_key = Zero;                                    \
                                                                       \
    uint64_t hash = (uint64_t) hash_it(searchk);                       \
    int32_t index = (int32_t)hash;                                     \
    index = msi_lookup(hash, index,                                    \
        (table)->capmask, (table)->stepshift);                         \
    for(;                                                              \
         ! memequal(data[index].key, searchk);                         \
        index = msi_lookup(hash, index,                                \
            (table)->capmask, (table)->stepshift))                     \
    {                                                                  \
        if(memequal(data[index].key, zero_key)){                       \
            if(msi_insert_if_not_found) {                              \
                data[index].key = searchk;                             \
            }                                                          \
            break;                                                     \
        }                                                              \
    }                                                                  \
    index;                                                             \
})

#define msi_get(table, key_) __extension__({                            \
    (table)->data[msi_idx(table, key_, 0)].val;                         \
})
#define msi_set(table, key_, val_) __extension__({                     \
    assert((table)->len < (table)->capmask, "MSI HT IS FULL");         \
    int32_t msi_index_ = msi_idx(table, key_, 1);                      \
    typeof(((table)->data)) data = (table)->data;                      \
    typeof(data[0].val) msi_current_val                                \
        = (typeof(data[0].val)) data[msi_index_].val;                  \
    data[msi_index_].val = val_;                                       \
})

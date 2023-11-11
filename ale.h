#pragma once

#include <stdarg.h>    // standard variadic
#include <stdint.h>    // standard ints

#include <string.h> // Memory Optimized functions
#define ale_memset memset
#define ale_memcpy memcpy
#define ale_memcmp memcmp
#define ale_strlen strlen

#include <stdio.h>
#define ale_printf printf
#define ale_vsprintf vsprintf

#include <stdlib.h>
#define ale_system system

#define ale_assert(_cond_, ...)                          \
    if (!(_cond_)) {                                     \
        ale_printf("\n!! [%s:%d] ", __FILE__, __LINE__); \
        ale_printf(__VA_ARGS__);                         \
        ale_printf(" !!\n");                             \
        __builtin_unreachable();                         \
    }                                                    \

#ifdef __cplusplus
#define _at_least_(_size_) /* static _size_ */
#define  alignof(x) ((int64_t)alignof(x))
#define Zero {}
#define cpound(type) /* (type) */
#define threadlocal static thread_local
#endif

#ifndef __cplusplus
#define _at_least_(_size_) static _size_
#define  alignof(x) ((int64_t)_Alignof(x))
#define Zero {0}
#define cpound(type) (type)
#define threadlocal static _Thread_local
#endif

//TRICK scope that "opens" at start, and "closes" at end 
threadlocal char MACRO_scoped__;
#define scoped(start, end) MACRO_scoped__ = 1; for(start; MACRO_scoped__; (--MACRO_scoped__), end)

/* 
    MEMORYops
*/
#define MegaBytes 1048576 //constexpr uint64_t MegaBytes = 1048576;
#define   sizeof(x)      ((int64_t)sizeof(x))
#define  countof(a)      (sizeof(a) / sizeof(*(a)))
#define memequal(x1, x2) (sizeof(x1) != sizeof(x2) ? 0 : ! ale_memcmp(&x1, &x2, sizeof(x1)))

/*
    STRINGS
*/
typedef char * cstring;
typedef const char * const staticstring;
typedef struct slicestring{ int32_t len; cstring data; }slicestring;

static inline slicestring cstr_to_slice(int32_t len, staticstring data) {
    slicestring temp = {len, (cstring) data};
    return temp;
} 

// uses sizeof in static c-strings
#define cstrlen(str) (countof(str) == 8? (int32_t)ale_strlen(str) : (int32_t) (countof(str) - 1))
#define s8(cstr) cstr_to_slice(cstrlen(cstr), cstr)

/*
    SHELL
*/
static int32_t shellrun(char buffer [_at_least_(512)], staticstring format, ...) {
    ale_memset(buffer, '\0', 512);

    va_list args; va_start(args, format);

    ale_vsprintf(buffer, format, args);
    return ale_system(buffer);
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

    ale_assert(total < (a->end - a->beg - pad), \
        "ARENA OUT OF MEMORY Ask:%lld Avail: %lld\n", total, a->end - a->beg - pad);

    char *p = a->beg + pad;
    a->beg += pad + total;
    
    return ale_memset(p, 0, total);
}

#define newx(a, t) (t *)alloc(a, sizeof(t), alignof(t), 1)
#define newxs(a, t, n) (t *)alloc(a, sizeof(t), alignof(t), n)
#define newmat(a, t, m, n) (t (*)[m][n]) alloc(a, sizeof(t [m][n]), alignof(t [m][n]), 1)

/*
    GROWABLE ARRAY
*/
static void grow(void *slice /*slice struct*/,  arena a[_at_least_(1)]) {
    struct dyna{int32_t cap; int32_t len; int64_t *data;};
    struct dyna *dynarray = (struct dyna *) slice;

    if (!dynarray->data) {
        // DEFAULT 64
        dynarray->data = (int64_t *) \
            alloc(a, sizeof(int64_t), alignof(int64_t), dynarray->cap = 64); 
    } else if (a->beg == ((char *) &(dynarray->data[dynarray->cap]))) { 
        // EXTEND
        int64_t *data = (int64_t *)  \
            alloc(a, sizeof(int64_t), 1, dynarray->cap);
        dynarray->cap *= 2;
    } else {
        // RELOC
        int64_t *data = (int64_t *)  \
            alloc(a, sizeof(int64_t), alignof(int64_t), dynarray->cap *= 2);
        ale_memcpy(data, dynarray->data, sizeof(int64_t)*dynarray->len);
        dynarray->data = data;
    }
}

/*
    PUSH TO GROWABLE ARRAY
*/
static void push_i64(void *dynarr, arena a[_at_least_(1)], int64_t int64) {
    struct dyna{int32_t cap; int32_t len; int64_t *data;};
    struct dyna *dynarray = (struct dyna *) dynarr;

    if (dynarray->len >= dynarray->cap) {
        grow(dynarray, a);
    }

    dynarray->data[dynarray->len++] = int64;
}
static inline void push_double(void *dynarr, arena a[_at_least_(1)], double float64) {
    int64_t replica;
    ale_memcpy(&replica, &float64, sizeof(replica)); //type prunning

    push_i64(dynarr, a, replica);
}
static inline void push_cstr(void *dynarr, arena a[_at_least_(1)], staticstring cstr) {
    push_i64(dynarr, a, (int64_t) cstr);
}
static inline void push_ptr(void *dynarr, arena a[_at_least_(1)], void *ptr) {
    push_i64(dynarr, a, (int64_t) ptr);
}

/*
    POP OF GROWABLE ARRAY
*/
static int64_t pop_i64(void *dynarr) {
    struct dyna{int32_t cap; int32_t len; int64_t *data;};
    struct dyna *dynarray = (struct dyna *) dynarr;

    ale_assert(dynarray->len > 0, "POP ON EMPTY ARRAY");

    return dynarray->data[--dynarray->len];
}
static inline double pop_double(void *dynarr) {
    int64_t replica = pop_i64(dynarr);
    double val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

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

static void * newmsi(arena a[_at_least_(1)], int32_t expected_maxn) {
    const int32_t msi_expo = fit_pwr2_exp(expected_maxn);
    ale_assert(msi_expo <= 24, "%d IS TOO BIG FOR MSI, MAX IS 2^24 - 1", expected_maxn);

    struct msi_entry{int64_t key; int64_t val;};
    struct msi_ht{
        int32_t stepshift;int32_t capmask; int32_t len;
        struct msi_entry *data;
    };
    struct msi_ht *ht = (struct msi_ht*) \
        alloc(a, sizeof(struct msi_ht), alignof(struct msi_ht), 1);
    
    ht->stepshift = 64 - msi_expo;
    ht->capmask = (1 << msi_expo) - 1;
    ht->len = 0;
    ht->data = (struct msi_entry *) \
        alloc(a, sizeof(struct msi_entry), alignof(struct msi_entry), ht->capmask + 1);

    return ht;
}

// Finds the index of |key| in the msi |table|, insert key if |insert_if_not_found| is true
static int32_t msi_idx_i64(
    void *table /* msi_ht */, 
    int64_t key, int32_t insert_if_not_found
) {
    struct msi_entry{int64_t key; int64_t val;};
    struct msi_ht{
        int32_t stepshift;int32_t capmask; int32_t len;
        struct msi_entry *data;
    };
    struct msi_ht *ht = (struct msi_ht*) table;

    uint64_t hash = (uint64_t) hash_i64(key);
    int32_t index = (int32_t)hash;
    
    for(
        index = msi_lookup(hash, index, ht->capmask, ht->stepshift);
        ht->data[index].key != key; 
        msi_lookup(hash, index, ht->capmask, ht->stepshift)
    ) {
        if (ht->data[index].key == 0) {
            if (insert_if_not_found) {
                ht->data[index].key = key;
            }
            break; // found empty slot
        }
    }


    return index; // index of entry found OR entry empty
}



#define msi_get(table, key_) __extension__({                           \
    (table)->data[msi_idx(table, key_, 0)].val;                        \
})
#define msi_set(table, key_, val_) __extension__({                     \
    ale_assert((table)->len < (table)->capmask, "MSI HT IS FULL");         \
    int32_t msi_index_ = msi_idx(table, key_, 1);                      \
    typeof(((table)->data)) data = (table)->data;                      \
    typeof(data[0].val) msi_current_val                                \
        = (typeof(data[0].val)) data[msi_index_].val;                  \
    data[msi_index_].val = val_;                                       \
})

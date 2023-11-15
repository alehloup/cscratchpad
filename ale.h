#pragma once

#include <stdarg.h>    // variadic
#include <stdint.h>    // ints

#include <string.h> // memory
#define ale_memset memset
#define ale_memcpy memcpy
#define ale_strlen strlen
#define ale_strcmp strcmp

#include <stdio.h> // output
#define ale_printf printf
#define ale_vsprintf vsprintf

#include <stdlib.h> // shell
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
#define cpound(type) /* (type) */
#define threadlocal static thread_local
#define buffer_param(_param_name, _len_size) void * _param_name /* static _len_size */
#endif

#ifndef __cplusplus
#define _at_least_(_size_) static _size_
#define  alignof(x) ((int64_t)_Alignof(x))
#define cpound(type) (type)
#define threadlocal static _Thread_local
#define buffer_param(_param_name, _len_size) char _param_name[static _len_size] 
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

/*
    STRINGS
*/
typedef const char * cstring;
typedef const char * const staticstring;
typedef struct slicestring{ int32_t len; cstring data; }slicestring;

static slicestring cstr_to_slice(int32_t len, cstring data) {
    slicestring temp = {len, (cstring) data};
    return temp;
} 

// uses sizeof in static c-strings
#define cstrlen(str) (countof(str) == 8? (int32_t)ale_strlen(str) : (int32_t) (countof(str) - 1))
#define s8(cstr) cstr_to_slice(cstrlen(cstr), cstr)

/*
    Generic 64 Union Type
*/
typedef union _ale_generic64{
    int64_t i;
    double d;
    cstring s;
    void *p;
}_ale_generic64;

/*
    SHELL
*/
static int32_t shellrun(char buffer [_at_least_(512)], cstring format, ...) {
    ale_memset(buffer, '\0', 512);

    va_list args; va_start(args, format);

    ale_vsprintf(buffer, format, args);
    return ale_system(buffer);
}

/*
    MATH
*/
//Fast mod when the number is a power of 2
static int64_t MODPWR2(int64_t number, int64_t modval) {
    return (number) & (modval - 1);
}

// Returns first power 2 that size+1 fits (it starts at 2^9 == 512)
static int32_t fit_pwr2_exp(int32_t size) {
    int32_t exp=2; int32_t val=4; ++size;
    while (val < size) {
        ++exp; val*=2;
    }
    return exp;
}

// RANDOM
static int32_t RND(uint64_t seed[_at_least_(1)]) {
    *seed = *seed * 0x9b60933458e17d7dLL + 0xd737232eeccdf7edLL;
    int32_t shift = 29 - (uint32_t)(*seed >> 61);
    
    return ((int32_t) (*seed >> shift)) & 2147483647;
}

/*
    ARENA
*/
typedef struct arena{ char *beg; char *end; }arena;
static arena newarena(int64_t buffer_len, buffer_param(buffer, buffer_len)) {
    arena a = {};
    a.beg = (char *)buffer;
    a.end = a.beg ? a.beg + buffer_len : 0;
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

#define newx(  a, t)       (t *)         alloc(a, sizeof(t),        alignof(t),        1)
#define newxs( a, t, n)    (t *)         alloc(a, sizeof(t),        alignof(t),        n)
#define newmat(a, t, m, n) (t (*)[m][n]) alloc(a, sizeof(t [m][n]), alignof(t [m][n]), 1)

/*
    GROWABLE ARRAY
*/
typedef int64_t dynael_layout;
typedef struct dyna_layout{int32_t cap; int32_t len; int64_t *data;} dyna_layout;

static void grow(void *dynamic_array,  arena a[_at_least_(1)]) { 
    ale_assert(dynamic_array, "DYNAMIC ARRAY IS NULL");

    static const int32_t DYNA_FIRST_SIZE = 64;

    dyna_layout *dynarray = (dyna_layout *)dynamic_array;

    if (!dynarray->data) {
        int64_t *DYNA_START = dynarray->data = (dynael_layout *)
            alloc(a, sizeof(dynael_layout), alignof(dynael_layout), dynarray->cap = DYNA_FIRST_SIZE); 
    } else if (a->beg == ((char *) &(dynarray->data[dynarray->cap]))) { 
        // EXTEND
        int64_t *DYNA_EXTEND = (dynael_layout *)
            alloc(a, sizeof(dynael_layout), 1, dynarray->cap);
        dynarray->cap *= 2;
    } else {
        // RELOC
        int64_t *DYNA_RELOC = (dynael_layout *)
            alloc(a, sizeof(dynael_layout), alignof(dynael_layout), dynarray->cap *= 2);
        ale_memcpy(DYNA_RELOC, dynarray->data, sizeof(dynael_layout)*dynarray->len);
        dynarray->data = DYNA_RELOC;
    }
}

/*
    PUSH TO GROWABLE ARRAY
*/
static void push_i64(void *dynamic_array, arena a[_at_least_(1)], int64_t int64) {
    dyna_layout *dynarray = (dyna_layout *)dynamic_array;

    if (dynarray->len >= dynarray->cap) {
        grow(dynarray, a);
    }

    dynarray->data[dynarray->len++] = int64;
}
static void push_double(void *dynarr, arena a[_at_least_(1)], double float64) {
    int64_t replica;
    ale_memcpy(&replica, &float64, sizeof(replica)); //type prunning

    push_i64(dynarr, a, replica);
}
static void push_cstr(void *dynarr, arena a[_at_least_(1)], cstring cstr) {
    push_i64(dynarr, a, (int64_t) cstr);
}
static void push_ptr(void *dynarr, arena a[_at_least_(1)], void *ptr) {
    push_i64(dynarr, a, (int64_t) ptr);
}

/*
    POP OF GROWABLE ARRAY
*/
static int64_t pop_i64(void *dynamic_array) {
    dyna_layout *dynarray = (dyna_layout *)dynamic_array;

    ale_assert(dynarray->len > 0, "POP ON EMPTY ARRAY");

    return dynarray->data[--dynarray->len];
}
static double pop_double(void *dynarr) {
    int64_t replica = pop_i64(dynarr);
    double val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val; 
}
static cstring pop_cstr(void *dynarr) {
     return (cstring) pop_i64(dynarr);
}
static void * pop_ptr(void *dynarr) {
     return (void *) pop_i64(dynarr);
}

/*
    HASH
*/
static int64_t hash_cstr(cstring str) {
    cstring sp = (cstring) str;

    uint64_t h = 0x7A5662DCDF;
    while(*sp) { 
        h ^= (*(sp++)) & 255; h *= 1111111111111111111;
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
typedef struct msi_entry_layout{int64_t key; int64_t val;}msi_entry_layout;
typedef struct msi_ht_layout{
    int32_t shift;int32_t mask; int32_t len;
    msi_entry_layout *data;
}msi_ht_layout;

static int32_t 
    msi_lookup(
        uint64_t hash, // 1st hash acts as base location
        int32_t index, // 2nd "hash" steps over the "list of elements" from base-location
        int32_t mask, // trims number to < ht_capacity
        int32_t shift // use |exp| bits for step 
    )
{
    uint32_t step = (uint32_t)(hash >> shift) | 1;
    return (index + step) & mask;
}

static void * newmsi(arena a[_at_least_(1)], int32_t expected_maxn) {
    const int32_t msi_expo = fit_pwr2_exp(expected_maxn + 64);
    ale_assert(msi_expo <= 24, "%d IS TOO BIG FOR MSI, MAX IS 2^24 - 1", expected_maxn);

    msi_ht_layout *ht = (msi_ht_layout*)
        alloc(a, sizeof(msi_ht_layout), alignof(msi_ht_layout), 1);
    
    ht->shift = 64 - msi_expo;
    ht->mask = (1 << msi_expo) - 1;
    ht->len = 0;
    ht->data = (msi_entry_layout *) \
        alloc(a, sizeof(msi_entry_layout), alignof(msi_entry_layout), ht->mask + 1);

    return ht;
}

// Finds the index of |keyi64| in the msi |table|, creates key if |create_if_not_found| is true
static int32_t msi_i64(
    void *table /* msi_ht */, 
    int64_t keyi64, int32_t create_if_not_found
) {
    ale_assert(table, "MSI_i64 TABLE IS NULL");

    msi_ht_layout *ht = (msi_ht_layout*) table;
    typeof(ht->data) data = ht->data;

    uint64_t hash = (uint64_t) hash_i64(keyi64);
    int32_t index = (int32_t)hash;
    int32_t mask = ht->mask;
    int32_t shift = ht->shift;
    
    for(
        index = msi_lookup(hash, index, mask, shift);
        data[index].key != 0 && data[index].key != keyi64; 
        index = msi_lookup(hash, index, mask, shift)
    ) {
        /* empty body */
    }

    if (data[index].key == 0 && create_if_not_found) {
        ale_assert(ht->len < mask - 2, "MSI HT IS FULL");
        data[index].key = keyi64;
        ++ht->len;
    }

    return index; // index of entry found OR entry empty
}

// Returns the index of |ikey| in the msi |table|
static int32_t msi_get_by_ikey(void *table, int64_t ikey) {
    return msi_i64(table, ikey, 0);
}
// Creates key if not found, then returns the index of |ikey| in the msi |table|
static int32_t msi_set_ikey(void *table, int64_t ikey) {
    return msi_i64(table, ikey, 1);
}

// Finds the index of |keycstr| in the msi |table|, creates key if |create_if_not_found| is true
static int32_t msi_cstr(
    void *table /* msi_ht */, 
    cstring keycstr, int32_t create_if_not_found
) {
    ale_assert(table, "MSI_CSTR TABLE IS NULL");

    msi_ht_layout *ht = (msi_ht_layout*) table;
    typeof(ht->data) data = ht->data;

    uint64_t hash = (uint64_t) hash_cstr(keycstr);
    int32_t index = (int32_t)hash;
    int32_t mask = ht->mask;
    int32_t shift = ht->shift;
    
    for(
        index = msi_lookup(hash, index, mask, shift);
        data[index].key != 0 && ale_strcmp((cstring) data[index].key, keycstr);
        index = msi_lookup(hash, index, mask, shift)
    ) {
        /* empty body */
    }

    if (data[index].key == 0 && create_if_not_found) {
        ale_assert(ht->len < mask - 2, "MSI HT IS FULL");
        data[index].key = (int64_t) keycstr;
        ++ht->len;
    }

    return index; // index of entry found OR entry empty
}
// Returns the index of |ikey| in the msi |table|
static int32_t msi_get_by_skey(void *table, cstring skey) {
    return msi_cstr(table, skey, 0);
}
// Creates key if not found, then returns the index of |ikey| in the msi |table|
static int32_t msi_set_skey(void *table, cstring skey) {
    return msi_cstr(table, skey, 1);
}

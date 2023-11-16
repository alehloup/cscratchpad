#pragma once

#include <stdarg.h>    // variadic
#include <stdint.h>    // ints
#include <stdbool.h>   // booleans
#include <stdalign.h>  // alignof

#include <string.h> // memory
#define ale_memset memset
#define ale_memcpy memcpy
#define ale_strlen strlen
#define ale_strcmp strcmp

#include <stdio.h> // output
#define ale_printf printf
// #undef  ale_printf // uncomment for removing prints
// #define ale_printf(format, ...) ; // uncomment for removing prints
#define ale_vsprintf vsprintf

#include <stdlib.h> // shell
#define ale_system system

#define _Mega_Bytes 1048576 //constexpr uint64_t _Mega_Bytes = 1048576;

#define ale_assert(_cond_, ...)                          \
    if (!(_cond_)) {                                     \
        ale_printf("\n!! [%s:%d] ", __FILE__, __LINE__); \
        ale_printf(__VA_ARGS__);                         \
        ale_printf(" !!\n");                             \
        __builtin_unreachable();                         \
    }                                                    \

//TRICK scope that "opens" at start, and "closes" at end 
static thread_local char MACRO_scoped__;
#define scoped(start, end) MACRO_scoped__ = 1; for(start; MACRO_scoped__; (--MACRO_scoped__), end)

/*
    STRINGS
*/
typedef const char * cstring;
typedef const char * const staticstring;
typedef struct slicestring{ int32_t len; cstring data; }slicestring;
typedef int64_t cstrtoi; // to convert a cstring, a pointer, to an int64
typedef cstring itocstr; // to convert a int64 to a cstring (a pointer)

static slicestring s8(int32_t len, cstring data) {
    slicestring temp = {len, (cstring) data};
    return temp;
} 

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
static int32_t shellrun(char buffer [512], cstring format, ...) {
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
static int32_t RND(uint64_t seed[1]) {
    *seed = *seed * 0x9b60933458e17d7dLL + 0xd737232eeccdf7edLL;
    int32_t shift = 29 - (uint32_t)(*seed >> 61);
    
    return ((int32_t) (*seed >> shift)) & 2147483647;
}

/*
    ARENA
*/
typedef struct arena{ uint8_t *beg; uint8_t *end; }arena;
static arena newarena(int64_t buffer_len, uint8_t buffer[]) {
    arena a = {};
    a.beg = (uint8_t *)buffer;
    a.end = a.beg ? a.beg + buffer_len : 0;
    return a;
}

__attribute((malloc, alloc_size(2, 4), alloc_align(3)))
static void *alloc(arena a[1], int64_t size, int64_t align, int64_t count) {
    int64_t total = size * count;
    int64_t pad = MODPWR2(- (int64_t)a->beg, align); //mod -x gives n for next align

    ale_assert(total < (a->end - a->beg - pad), \
        "ARENA OUT OF MEMORY Ask:%lld Avail: %lld\n", total, a->end - a->beg - pad);

    uint8_t *p = a->beg + pad;
    a->beg += pad + total;
    
    return ale_memset(p, 0, total);
}

/*
    GROWABLE ARRAY
*/
typedef int64_t vector64_element;
typedef struct vector64{int32_t cap; int32_t len; int64_t *data;}vector64;

static void grow(vector64 dynarray[1],  arena a[1]) { 
    static const int32_t DYNA_FIRST_SIZE = 64;

    if (!dynarray->data) {
        int64_t *DYNA_START = dynarray->data = (vector64_element *)
            alloc(a, sizeof(vector64_element), alignof(vector64_element), dynarray->cap = DYNA_FIRST_SIZE); 
    } else if (a->beg == ((uint8_t *) &(dynarray->data[dynarray->cap]))) { 
        // EXTEND
        int64_t *DYNA_EXTEND = (vector64_element *)
            alloc(a, sizeof(vector64_element), 1, dynarray->cap);
        dynarray->cap *= 2;
    } else {
        // RELOC
        int64_t *DYNA_RELOC = (vector64_element *)
            alloc(a, sizeof(vector64_element), alignof(vector64_element), dynarray->cap *= 2);
        ale_memcpy(DYNA_RELOC, dynarray->data, sizeof(vector64_element)*dynarray->len);
        dynarray->data = DYNA_RELOC;
    }
}

/*
    PUSH TO GROWABLE ARRAY
*/
static void push_i64(vector64 dynarray[1], arena a[1], int64_t int64) {
    if (dynarray->len >= dynarray->cap) {
        grow(dynarray, a);
    }

    dynarray->data[dynarray->len++] = int64;
}
static void push_double(vector64 dynarr[1], arena a[1], double float64) {
    int64_t replica;
    ale_memcpy(&replica, &float64, sizeof(replica)); //type prunning

    push_i64(dynarr, a, replica);
}
static void push_cstr(vector64 dynarr[1], arena a[1], cstring cstr) {
    push_i64(dynarr, a, (int64_t) cstr);
}
static void push_ptr(vector64 dynarr[1], arena a[1], void *ptr) {
    push_i64(dynarr, a, (int64_t) ptr);
}

typedef int32_t vector32_element;
typedef struct vector32{int32_t cap; int32_t len; int32_t *data;}vector32;

static void grow32(vector32 dynarray[1],  arena a[1]) { 
    static const int32_t DYNA_FIRST_SIZE = 64;

    if (!dynarray->data) {
        int32_t *DYNA_START = dynarray->data = (vector32_element *)
            alloc(a, sizeof(vector32_element), alignof(vector32_element), dynarray->cap = DYNA_FIRST_SIZE); 
    } else if (a->beg == ((uint8_t *) &(dynarray->data[dynarray->cap]))) { 
        // EXTEND
        int32_t *DYNA_EXTEND = (vector32_element *)
            alloc(a, sizeof(vector32_element), 1, dynarray->cap);
        dynarray->cap *= 2;
    } else {
        // RELOC
        int32_t *DYNA_RELOC = (vector32_element *)
            alloc(a, sizeof(vector32_element), alignof(vector32_element), dynarray->cap *= 2);
        ale_memcpy(DYNA_RELOC, dynarray->data, sizeof(vector32_element)*dynarray->len);
        dynarray->data = DYNA_RELOC;
    }
}

/*
    PUSH TO grow32ABLE ARRAY
*/
static void push_i32(vector32 dynarray[1], arena a[1], int32_t int32) {
    if (dynarray->len >= dynarray->cap) {
        grow32(dynarray, a);
    }

    dynarray->data[dynarray->len++] = int32;
}
static void push_float(vector32 dynarr[1], arena a[1], float float32) {
    int32_t replica;
    ale_memcpy(&replica, &float32, sizeof(replica)); //type prunning

    push_i32(dynarr, a, replica);
}

/*
    POP OF GROWABLE ARRAY
*/
static int64_t pop_i64(vector64 dynarray[1]) {
    ale_assert(dynarray->len > 0, "POP ON 64bit EMPTY ARRAY");

    return dynarray->data[--dynarray->len];
}
static double pop_double(vector64 dynarr[1]) {
    int64_t replica = pop_i64(dynarr);
    double val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val; 
}
static cstring pop_cstr(vector64 dynarr[1]) {
     return (cstring) pop_i64(dynarr);
}
static void * pop_ptr(vector64 dynarr[1]) {
     return (void *) pop_i64(dynarr);
}

static int32_t pop_i32(vector32 dynarray[1]) {
    ale_assert(dynarray->len > 0, "POP ON 32bit EMPTY ARRAY");

    return dynarray->data[--dynarray->len];
}
static float pop_float(vector32 dynarr[1]) {
    int32_t replica = pop_i32(dynarr);
    float val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val; 
}

/*
    DATA AS GROWABLE ARRAY
*/
static int64_t * vector_data_as_i64(vector64 *dynamic_array) {
    return (int64_t *) dynamic_array->data;
}
static double * vector_data_as_double(vector64 *dynamic_array) {
    return (double *) dynamic_array->data;
}
static cstring * vector_data_as_cstring(vector64 *dynamic_array) {
    return (cstring *) dynamic_array->data;
}
static void * * vector_data_as_ptr(vector64 *dynamic_array) {
    return (void * *) dynamic_array->data;
}
static int32_t * vector_data_as_i32(vector32 *dynamic_array) {
    return (int32_t *) dynamic_array->data;
}
static float * vector_data_as_float(vector32 *dynamic_array) {
    return (float *) dynamic_array->data;
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

typedef struct msiht64_entry{int64_t key; int64_t val;}msiht64_entry;
typedef struct msiht64{
    int32_t shift;int32_t mask; int32_t len;
    msiht64_entry *data;
}msiht64;

static msiht64 * new_64msi(arena a[1], int32_t expected_maxn) {
    const int32_t msi_expo = fit_pwr2_exp(expected_maxn + 64);
    ale_assert(msi_expo <= 24, "%d IS TOO BIG FOR MSI, MAX IS 2^24 - 1", expected_maxn);

    msiht64 *ht = (msiht64*)
        alloc(a, sizeof(msiht64), alignof(msiht64), 1);
    
    ht->shift = 64 - msi_expo;
    ht->mask = (1 << msi_expo) - 1;
    ht->len = 0;
    ht->data = (msiht64_entry *)
        alloc(a, sizeof(msiht64_entry), alignof(msiht64_entry), ht->mask + 1);

    return ht;
}

// Finds the index of |keyi64| in the msi |table|, creates key if |create_if_not_found| is true
static int32_t msi_i64(
    msiht64 *ht /* msi_ht */, 
    int64_t keyi64, int32_t create_if_not_found
) {
    ale_assert(ht, "MSI_i64 TABLE IS NULL");

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
static int32_t msi_get_by_i64key(msiht64 *table, int64_t ikey) {
    return msi_i64(table, ikey, 0);
}
// Creates key if not found, then returns the index of |ikey| in the msi |table|
static int32_t msi_set_i64key(msiht64 *table, int64_t ikey) {
    return msi_i64(table, ikey, 1);
}

// Finds the index of |keycstr| in the msi |table|, creates key if |create_if_not_found| is true
static int32_t msi_cstr(
    msiht64 *ht /* msi_ht */, 
    cstring keycstr, int32_t create_if_not_found
) {
    ale_assert(ht, "MSI_CSTR TABLE IS NULL");

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
static int32_t msi_get_by_skey(msiht64 *table, cstring skey) {
    return msi_cstr(table, skey, 0);
}
// Creates key if not found, then returns the index of |ikey| in the msi |table|
static int32_t msi_set_skey(msiht64 *table, cstring skey) {
    return msi_cstr(table, skey, 1);
}

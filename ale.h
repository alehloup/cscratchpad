#pragma once

/*
    Choose which core functions to use
*/
#include <string.h>      // memory
#define ale_memset memset
#define ale_memcpy memcpy
#define ale_strlen strlen
#define ale_strcmp strcmp

#include <stdio.h>       // output
#define ale_printf printf
// #undef  ale_printf // uncomment for removing prints
// #define ale_printf(format, ...) ; // uncomment for removing prints
#define ale_vsprintf vsprintf

#include <stdlib.h>      // shell
#define ale_system system

/*
    STANDARD TYPES
*/
#include <stdalign.h>    // alignof
#include <stdarg.h>      // variadic
#include <stdbool.h>     // booleans
#include <stdint.h>      // ints
//       <stdfloat.h>    // consistency with stdint
typedef float float32_t;
typedef double float64_t;
//       <stdstring.h>   // better string typenames
typedef const char * cstring;
typedef const char * const staticstring;
typedef struct slicestring{ int32_t len; cstring data; }slicestring;
typedef int64_t cstrtoi; // to convert a cstring, a pointer, to an int64
typedef cstring itocstr; // to convert a int64 to a cstring (a pointer)
//       <stdptr.h>      // pointer typename
typedef void * pointer;  // generic pointer
//       <stdconst.h>    // constants 
#define _Mega_Bytes 1048576 // malloc(52*_Mega_Bytes)
//       <stdunion.h>    // generic unions
typedef union union64_t{
    int64_t i;
    float64_t d;
    cstring s;
    pointer p;
}union64_t;
typedef union union32_t{
    int32_t i;
    float32_t d;
}union32_t;

/*
    Auto + _Thread_local + Better Assert + Scope/Defer
*/
// auto
#ifdef __cplusplus
    #define auto auto
    #define _Thread_local thread_local
#endif 
#ifndef __cplusplus
    #define auto __auto_type
    #define thread_local _Thread_local
#endif
#define ale_assert(_cond_, ...)                          \
    if (!(_cond_)) {                                     \
        ale_printf("\n!! [%s:%d] ", __FILE__, __LINE__); \
        ale_printf(__VA_ARGS__);                         \
        ale_printf(" !!\n");                             \
        __builtin_unreachable();                         \
    }
static _Thread_local char MACRO_scoped__;
//TRICK scope that "opens" at start, and "closes" at end 
#define scoped(start, end) MACRO_scoped__ = 1; for(start; MACRO_scoped__; (--MACRO_scoped__), end)

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
static pointer alloc(arena a[1], int64_t size, int64_t align, int64_t count) {
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
static void grow64(vector64 dynarray[1],  arena a[1]) { 
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
    PUSH TO GROWABLE ARRAY
*/
static void push_i64(vector64 dynarray[1], arena a[1], int64_t int64) {
    if (dynarray->len >= dynarray->cap) {
        grow64(dynarray, a);
    }

    dynarray->data[dynarray->len++] = int64;
}
static void push_f64(vector64 dynarr[1], arena a[1], float64_t float64) {
    int64_t replica;
    ale_memcpy(&replica, &float64, sizeof(replica)); //type prunning

    push_i64(dynarr, a, replica);
}
static void push_cstr(vector64 dynarr[1], arena a[1], cstring cstr) {
    push_i64(dynarr, a, (int64_t) cstr);
}
static void push_ptr(vector64 dynarr[1], arena a[1], pointer ptr) {
    push_i64(dynarr, a, (int64_t) ptr);
}

static void push_i32(vector32 dynarray[1], arena a[1], int32_t int32) {
    if (dynarray->len >= dynarray->cap) {
        grow32(dynarray, a);
    }

    dynarray->data[dynarray->len++] = int32;
}
static void push_f32(vector32 dynarr[1], arena a[1], float32_t float32) {
    int32_t replica;
    ale_memcpy(&replica, &float32, sizeof(replica)); //type prunning

    push_i32(dynarr, a, replica);
}

/*
    POP GROWABLE ARRAY
*/
static int64_t pop_i64(vector64 dynarray[1]) {
    ale_assert(dynarray->len > 0, "POP ON 64bit EMPTY ARRAY");

    return dynarray->data[--dynarray->len];
}
static float64_t pop_f64(vector64 dynarr[1]) {
    int64_t replica = pop_i64(dynarr);
    float64_t val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val; 
}
static cstring pop_cstr(vector64 dynarr[1]) {
     return (cstring) pop_i64(dynarr);
}
static pointer pop_ptr(vector64 dynarr[1]) {
     return (pointer) pop_i64(dynarr);
}

static int32_t pop_i32(vector32 dynarray[1]) {
    ale_assert(dynarray->len > 0, "POP ON 32bit EMPTY ARRAY");

    return dynarray->data[--dynarray->len];
}
static float32_t pop_f32(vector32 dynarr[1]) {
    int32_t replica = pop_i32(dynarr);
    float32_t val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val; 
}

/*
    GROWABLE ARRAY DATA AS
*/
static int64_t * vector_data_as_i64(vector64 *dynamic_array) {
    return (int64_t *) dynamic_array->data;
}
static float64_t * vector_data_as_f64(vector64 *dynamic_array) {
    return (float64_t *) dynamic_array->data;
}
static cstring * vector_data_as_cstring(vector64 *dynamic_array) {
    return (cstring *) dynamic_array->data;
}
static pointer * vector_data_as_ptr(vector64 *dynamic_array) {
    return (pointer *) dynamic_array->data;
}
static int32_t * vector_data_as_i32(vector32 *dynamic_array) {
    return (int32_t *) dynamic_array->data;
}
static float32_t * vector_data_as_f32(vector32 *dynamic_array) {
    return (float32_t *) dynamic_array->data;
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

    return (h ^ (h >> 32)) >> 1;
}
static int64_t hash_i64(int64_t integer64) {
    uint64_t x = (uint64_t)integer64;
    x *= 0x94d049bb133111eb; 
    x ^= (x >> 30); 
    x *= 0xbf58476d1ce4e5b9; 
    
    return (x ^ (x >> 31)) >> 1;
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

typedef struct entry_i64_i64{int64_t key; int64_t val;}entry_i64_i64;
typedef struct msiht64{
    int32_t shift;int32_t mask; int32_t len;
    entry_i64_i64 *data;
}msiht64;
static msiht64 * newmsi64(arena a[1], int32_t expected_maxn) {
    const int32_t msi_expo = fit_pwr2_exp(expected_maxn + 64);
    ale_assert(msi_expo <= 24, "%d IS TOO BIG FOR MSI, MAX IS 2^24 - 1", expected_maxn);

    msiht64 *ht = (msiht64*)
        alloc(a, sizeof(msiht64), alignof(msiht64), 1);
    
    ht->shift = 64 - msi_expo;
    ht->mask = (1 << msi_expo) - 1;
    ht->len = 0;
    ht->data = (entry_i64_i64 *)
        alloc(a, sizeof(entry_i64_i64), alignof(entry_i64_i64), ht->mask + 1);

    return ht;
}

typedef struct entry_i32_i32{int32_t key; int32_t val;}entry_i32_i32;
typedef struct msiht32{
    int32_t shift;int32_t mask; int32_t len;
    entry_i32_i32 *data;
}msiht32;
static msiht32 * newmsi32(arena a[1], int32_t expected_maxn) {
    const int32_t msi_expo = fit_pwr2_exp(expected_maxn + 64);
    ale_assert(msi_expo <= 24, "%d IS TOO BIG FOR MSI, MAX IS 2^24 - 1", expected_maxn);

    msiht32 *ht = (msiht32*)
        alloc(a, sizeof(msiht32), alignof(msiht32), 1);
    
    ht->shift = 64 - msi_expo;
    ht->mask = (1 << msi_expo) - 1;
    ht->len = 0;
    ht->data = (entry_i32_i32 *)
        alloc(a, sizeof(entry_i32_i32), alignof(entry_i32_i32), ht->mask + 1);

    return ht;
}

/*
    MSI Hash Table with Integer64 as Key
*/
// Finds the index of |keyi64| in the msi |table|, creates key if |create_if_not_found| is true
static int32_t msiki(
    msiht64 *ht /* msi_ht */, 
    int64_t keyi64, int32_t create_if_not_found
) {
    ale_assert(ht, "MSI KI TABLE IS NULL");

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
        ale_assert(ht->len < mask - 2, "MSI KI HT IS FULL");
        data[index].key = keyi64;
        ++ht->len;
    }

    return index; // index of entry found OR entry empty
}

// Returns the index of |ikey| in the msi |table|
static int32_t msiki_get_idx(msiht64 *table, int64_t ikey) {
    return msiki(table, ikey, 0);
}

static int64_t msiki_get_i64(msiht64 *table, int64_t ikey) {
    return table->data[msiki(table, ikey, 0)].val;
}
static float64_t msiki_get_f64(msiht64 *table, int64_t ikey) {
    int64_t replica = table->data[msiki(table, ikey, 0)].val;
    float64_t val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val;
}
static cstring msiki_get_cstr(msiht64 *table, int64_t ikey) {
    return (itocstr) table->data[msiki(table, ikey, 0)].val;
}
static pointer msiki_get_ptr(msiht64 *table, int64_t ikey) {
    return (pointer) table->data[msiki(table, ikey, 0)].val;
}

// Creates key if not found, then returns the index of |ikey| in the msi |table|
static int32_t msiki_set_idx(msiht64 *table, int64_t ikey) {
    return msiki(table, ikey, 1);
}

static int64_t msiki_set_i64(msiht64 *table, int64_t ikey, int64_t ival) {
    table->data[msiki(table, ikey, 1)].val = ival;
    return ival;
}
static float64_t msiki_set_f64(msiht64 *table, int64_t ikey, float64_t dval) {
    int64_t replica;
    ale_memcpy(&replica, &dval, sizeof(replica)); //type prunning

    table->data[msiki(table, ikey, 1)].val = replica;
    return dval;
}
static cstring msiki_set_cstr(msiht64 *table, int64_t ikey, cstring sval) {
    table->data[msiki(table, ikey, 1)].val = (cstrtoi) sval;
    return sval;
}
static pointer msiki_set_ptr(msiht64 *table, int64_t ikey, pointer pval) {
    table->data[msiki(table, ikey, 1)].val = (int64_t) pval;
    return pval;
}

static entry_i64_i64 * msiki_data_as_int64(msiht64 *table) {
    auto data = table->data;
    return (entry_i64_i64 *) data;
}
typedef struct entry_i64_f64{int64_t key; float64_t val;}entry_i64_f64;
static entry_i64_f64 * msiki_data_as_f64(msiht64 *table) {
    auto data = table->data;
    return (entry_i64_f64 *) data;
}
typedef struct entry_i64_cstr{int64_t key; cstring val;}entry_i64_cstr;
static entry_i64_cstr * msiki_data_as_cstr(msiht64 *table) {
    auto data = table->data;
    return (entry_i64_cstr *) data;
}
typedef struct entry_i64_ptr{int64_t key; pointer val;}entry_i64_ptr;
static entry_i64_ptr * msiki_data_as_ptr(msiht64 *table) {
    auto data = table->data;
    return (entry_i64_ptr *) data;
}

/*
    MSI Hash Table with Cstring as Key
*/
// Finds the index of |keycstr| in the msi |table|, creates key if |create_if_not_found| is true
static int32_t msiks(
    msiht64 *ht /* msi_ht */, 
    cstring keycstr, int32_t create_if_not_found
) {
    ale_assert(ht, "MSI KS TABLE IS NULL");

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
        ale_assert(ht->len < mask - 2, "MSI KS HT IS FULL");
        data[index].key = (int64_t) keycstr;
        ++ht->len;
    }

    return index; // index of entry found OR entry empty
}
// Returns the index of |ikey| in the msi |table|
static int32_t msiks_get_idx(msiht64 *table, cstring skey) {
    return msiks(table, skey, 0);
}

static int64_t msiks_get_i64(msiht64 *table, cstring skey) {
    return (int64_t) table->data[msiks(table, skey, 0)].val;
}
static float64_t msiks_get_f64(msiht64 *table, cstring skey) {
    int64_t replica = table->data[msiks(table, skey, 0)].val;
    float64_t val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val;
}
static cstring msiks_get_cstr(msiht64 *table, cstring skey) {
    return (itocstr) table->data[msiks(table, skey, 0)].val;
}
static pointer msiks_get_ptr(msiht64 *table, cstring skey) {
    return (pointer) table->data[msiks(table, skey, 0)].val;
}

// Creates key if not found, then returns the index of |skey| in the msi |table|
static int32_t msiks_set_idx(msiht64 *table, cstring skey) {
    return msiks(table, skey, 1);
}

static int64_t msiks_set_i64(msiht64 *table, cstring skey, int64_t ival) {
    table->data[msiks(table, skey, 1)].val = ival;
    return ival;
}
static float64_t msiks_set_f64(msiht64 *table, cstring skey, float64_t dval) {
    int64_t replica;
    ale_memcpy(&replica, &dval, sizeof(replica)); //type prunning

    table->data[msiks(table, skey, 1)].val = replica;
    return dval;
}
static cstring msiks_set_cstr(msiht64 *table, cstring skey, cstring sval) {
    table->data[msiks(table, skey, 1)].val = (cstrtoi) sval;
    return sval;
}
static pointer msiks_set_ptr(msiht64 *table, cstring skey, pointer pval) {
    table->data[msiks(table, skey, 1)].val = (int64_t) pval;
    return pval;
}

typedef struct entry_cstr_i64{cstring key; int64_t val;}entry_cstr_i64;
static entry_cstr_i64 * msiks_data_as_int64(msiht64 *table) {
    auto data = table->data;
    return (entry_cstr_i64 *) data;
}
typedef struct entry_cstr_f64{cstring key; float64_t val;}entry_cstr_f64;
static entry_cstr_f64 * msiks_data_as_f64(msiht64 *table) {
    auto data = table->data;
    return (entry_cstr_f64 *) data;
}
typedef struct entry_cstr_cstr{cstring key; cstring val;}entry_cstr_cstr;
static entry_cstr_cstr * msiks_data_as_cstr(msiht64 *table) {
    auto data = table->data;
    return (entry_cstr_cstr *) data;
}
typedef struct entry_cstr_ptr{cstring key; pointer val;}entry_cstr_ptr;
static entry_cstr_ptr * msiks_data_as_ptr(msiht64 *table) {
    auto data = table->data;
    return (entry_cstr_ptr *) data;
}

/*
    MSI Hash Table with Integer32 as Key
*/
static int32_t msiki32(
    msiht32 *ht /* msi_ht */, 
    int32_t keyi32, int32_t create_if_not_found
) {
    ale_assert(ht, "MSI KI32 TABLE IS NULL");

    typeof(ht->data) data = ht->data;

    uint64_t hash = (uint64_t) hash_i64(keyi32);
    int32_t index = (int32_t)hash;
    int32_t mask = ht->mask;
    int32_t shift = ht->shift;
    
    for(
        index = msi_lookup(hash, index, mask, shift);
        data[index].key != 0 && data[index].key != keyi32; 
        index = msi_lookup(hash, index, mask, shift)
    ) {
        /* empty body */
    }

    if (data[index].key == 0 && create_if_not_found) {
        ale_assert(ht->len < mask - 2, "MSI KI32 HT IS FULL");
        data[index].key = keyi32;
        ++ht->len;
    }

    return index; // index of entry found OR entry empty
}

// Returns the index of |ikey| in the msi |table|
static int32_t msiki32_get_idx(msiht32 *table, int32_t ikey) {
    return msiki32(table, ikey, 0);
}

static int32_t msiki_get_i32(msiht32 *table, int32_t ikey) {
    return table->data[msiki32(table, ikey, 0)].val;
}
static float32_t msiki_get_f32(msiht32 *table, int32_t ikey) {
    int32_t replica = table->data[msiki32(table, ikey, 0)].val;
    float32_t val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val;
}

// Creates key if not found, then returns the index of |ikey| in the msi |table|
static int32_t msiki32_set_idx(msiht32 *table, int32_t ikey) {
    return msiki32(table, ikey, 1);
}

static int32_t msiki_set_i32(msiht32 *table, int32_t ikey, int32_t ival) {
    table->data[msiki32(table, ikey, 1)].val = ival;
    return ival;
}
static float32_t msiki_set_f32(msiht32 *table, int32_t ikey, float32_t dval) {
    int32_t replica;
    ale_memcpy(&replica, &dval, sizeof(replica)); //type prunning

    table->data[msiki32(table, ikey, 1)].val = replica;
    return dval;
}

static entry_i32_i32 * msiki_data_as_int32(msiht32 *table) {
    auto data = table->data;
    return (entry_i32_i32 *) data;
}
typedef struct entry_i32_f32{int32_t key; float32_t val;}entry_i32_f32;
static entry_i32_f32 * msiki_data_as_f32(msiht32 *table) {
    auto data = table->data;
    return (entry_i32_f32 *) data;
}

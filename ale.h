#pragma once

// ================ CONFIGURE WHAT TO USE ================
#define ale_attr(...) __attribute((__VA_ARGS__)) static // Use attributes
//#define ale_attr(...) static //do NOT use attributes
// Core functions to use
#include <string.h>
#define ale_memset memset
#define ale_memcpy memcpy
#define ale_strlen strlen
#define ale_strcmp strcmp
#include <stdio.h>
#define ale_printf printf
// #undef  ale_printf // uncomment for removing prints
// #define ale_printf(format, ...) ; // uncomment for removing prints
#define ale_vsprintf vsprintf
#define ale_fopen fopen
#define ale_fseek fseek
#define ale_ftell ftell
#define ale_fread fread
#define ale_fclose fclose
#include <stdlib.h>
#define ale_system system
// ========================================================

#define _Mega_Bytes 1048576 // malloc(52*_Mega_Bytes)
// STANDARD TYPES
#include <stdalign.h>    // alignof
#include <stdarg.h>      // variadic
#include <stdint.h>      // ints
// Float
typedef float float32_t;
typedef double float64_t;
// Bool
typedef int32_t b32_t;     // int to be used as boolean
static const b32_t True = 1;
static const b32_t False = 0;
// String
typedef const char * cstring;
typedef const char * const staticstring;
typedef char * modstring;
typedef struct s8{ int32_t len; modstring data; }s8;
// Assert
#define ale_assert(_cond_, ...)                          \
    if (!(_cond_)) {                                     \
        ale_printf("\n!! [%s:%d] ", __FILE__, __LINE__); \
        ale_printf(__VA_ARGS__);                         \
        ale_printf(" !!\n");                             \
        __builtin_unreachable();                         \
    }

// Simple attributes
#define $math ale_attr(const, warn_unused_result)
#define $fun ale_attr(nonnull, warn_unused_result)
#define $proc ale_attr(nonnull) void
#define $get ale_attr(warn_unused_result)
#define $set ale_attr(nonnull)
#define $pure ale_attr(pure, warn_unused_result)
// Parametric attributes
#define $format(paramidx_format, paramidx_varargs) \
    ale_attr(format (printf, paramidx_format, paramidx_varargs))
#define $read_buffer(paramidx_buffer, paramidx_bufferlen) \
    ale_attr(nonnull, warn_unused_result, access(read_only, paramidx_buffer, paramidx_bufferlen))
#define $malloc(paramidx_elementsize, paramidx_elementcount, paramidx_elementalign) \
    ale_attr(malloc, alloc_size(paramidx_elementsize, paramidx_elementcount), alloc_align(paramidx_elementalign), nonnull, warn_unused_result, no_sanitize ("leak"))

// SHELL
$format(/*format*/2, /*varargs*/3) 
int32_t shellrun(char buffer [512], cstring format, ...) {
    ale_memset(buffer, '\0', 512);

    va_list args; va_start(args, format);

    ale_vsprintf(buffer, format, args);
    return ale_system(buffer);
}

$math int64_t MODPWR2(int64_t number, int64_t modval) {
    return (number) & (modval - 1);
}

// Returns first power 2 that size+1 fits (it starts at 2^9 == 512)
$math int32_t fit_pwr2_exp(int32_t size) {
    int32_t exp=2; int32_t val=4; ++size;
    while (val < size) {
        ++exp; val*=2;
    }
    return exp;
}

// RANDOM
$fun int32_t rnd(uint64_t seed[1]) {
    *seed = *seed * 0x9b60933458e17d7dLL + 0xd737232eeccdf7edLL;
    int32_t shift = 29 - (uint32_t)(*seed >> 61);
    
    return ((int32_t) (*seed >> shift)) & 2147483647;
}

// ARENA
typedef struct arena_t{ uint8_t *beg; uint8_t *end; }arena_t;

$read_buffer(/*buffer*/2, /*bufferlen*/1) 
arena_t newarena(int64_t buffer_len, uint8_t buffer[]) {
    arena_t arena = {};
    arena.beg = (uint8_t *)buffer;
    arena.end = arena.beg ? arena.beg + buffer_len : 0;
    return arena;
}

// Arena Allocator that always zeroes the memory
$malloc(/*size*/2, /*count*/4, /*align*/3)
void * alloc(arena_t arena[1], int64_t size, int64_t align, int64_t count) {
    int64_t total = size * count;
    int64_t pad = MODPWR2(- (int64_t)arena->beg, align); //mod -x gives n for next align

    ale_assert(total < (arena->end - arena->beg - pad),
        "ARENA OUT OF MEMORY Ask:%lld Avail: %lld\n", (long long int)(total), (long long int)(arena->end - arena->beg - pad));

    uint8_t *p = arena->beg + pad;
    arena->beg += pad + total;
    
    return ale_memset(p, 0, total);
}
$fun void * alloc1(arena_t arena[1], int64_t size, int64_t align) {
    return alloc(arena, size, align, 1);
}

// VECTOR (dynamic array)
typedef int64_t vector64_t_element;
typedef struct vector64_t{int32_t cap; int32_t len; int64_t *data;}vector64_t;

$proc grow64(vector64_t dynamic_array[1],  arena_t arena[1]) { 
    static const int32_t DYNA_FIRST_SIZE = 64;

    if (!dynamic_array->data) {
        int64_t *DYNA_START = dynamic_array->data = (vector64_t_element *)
            alloc(arena, sizeof(vector64_t_element), alignof(vector64_t_element), dynamic_array->cap = DYNA_FIRST_SIZE); 
    } else if (arena->beg == ((uint8_t *) &(dynamic_array->data[dynamic_array->cap]))) { 
        // EXTEND
        int64_t *DYNA_EXTEND = (vector64_t_element *)
            alloc(arena, sizeof(vector64_t_element), 1, dynamic_array->cap);
        dynamic_array->cap *= 2;
    } else {
        // RELOC
        int64_t *DYNA_RELOC = (vector64_t_element *)
            alloc(arena, sizeof(vector64_t_element), alignof(vector64_t_element), dynamic_array->cap *= 2);
        ale_memcpy(DYNA_RELOC, dynamic_array->data, sizeof(vector64_t_element)*dynamic_array->len);
        dynamic_array->data = DYNA_RELOC;
    }
}

typedef int32_t vector32_t_element;
typedef struct vector32_t{int32_t cap; int32_t len; int32_t *data;}vector32_t;

$proc grow32(vector32_t dynamic_array[1], arena_t arena[1]) { 
    static const int32_t DYNA_FIRST_SIZE = 64;

    if (!dynamic_array->data) {
        int32_t *DYNA_START = dynamic_array->data = (vector32_t_element *)
            alloc(arena, sizeof(vector32_t_element), alignof(vector32_t_element), dynamic_array->cap = DYNA_FIRST_SIZE); 
    } else if (arena->beg == ((uint8_t *) &(dynamic_array->data[dynamic_array->cap]))) { 
        // EXTEND
        int32_t *DYNA_EXTEND = (vector32_t_element *)
            alloc(arena, sizeof(vector32_t_element), 1, dynamic_array->cap);
        dynamic_array->cap *= 2;
    } else {
        // RELOC
        int32_t *DYNA_RELOC = (vector32_t_element *)
            alloc(arena, sizeof(vector32_t_element), alignof(vector32_t_element), dynamic_array->cap *= 2);
        ale_memcpy(DYNA_RELOC, dynamic_array->data, sizeof(vector32_t_element)*dynamic_array->len);
        dynamic_array->data = DYNA_RELOC;
    }
}

// Vector Push
$proc push_i64(vector64_t dynamic_array[1], arena_t arena[1], int64_t int64) {
    if (dynamic_array->len >= dynamic_array->cap) {
        grow64(dynamic_array, arena);
    }

    dynamic_array->data[dynamic_array->len++] = int64;
}
$proc push_f64(vector64_t dynamic_array[1], arena_t arena[1], float64_t float64) {
    int64_t replica;
    ale_memcpy(&replica, &float64, sizeof(replica)); //type prunning

    push_i64(dynamic_array, arena, replica);
}
$proc push_cstr(vector64_t dynamic_array[1], arena_t arena[1], cstring cstr) {
    push_i64(dynamic_array, arena, (int64_t) cstr);
}
$proc push_ptr(vector64_t dynamic_array[1], arena_t arena[1], void * ptr) {
    push_i64(dynamic_array, arena, (int64_t) ptr);
}
$proc push_s8ptr(vector64_t dynamic_array[1], arena_t arena[1], s8 * stringslice_ptr) {
    push_i64(dynamic_array, arena, (int64_t) stringslice_ptr);
}
$proc push_i32(vector32_t dynamic_array[1], arena_t arena[1], int32_t int32) {
    if (dynamic_array->len >= dynamic_array->cap) {
        grow32(dynamic_array, arena);
    }

    dynamic_array->data[dynamic_array->len++] = int32;
}
$proc push_f32(vector32_t dynamic_array[1], arena_t arena[1], float32_t float32) {
    int32_t replica;
    ale_memcpy(&replica, &float32, sizeof(replica)); //type prunning

    push_i32(dynamic_array, arena, replica);
}

// Vector Pop
$fun int64_t pop_i64(vector64_t dynamic_array[1]) {
    ale_assert(dynamic_array->len > 0, "POP ON 64bit EMPTY ARRAY");

    return dynamic_array->data[--dynamic_array->len];
}
$fun float64_t pop_f64(vector64_t dynamic_array[1]) {
    int64_t replica = pop_i64(dynamic_array);
    float64_t val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val; 
}
$fun cstring pop_cstr(vector64_t dynamic_array[1]) {
     return (cstring) pop_i64(dynamic_array);
}
$fun void * pop_ptr(vector64_t dynamic_array[1]) {
     return (void *) pop_i64(dynamic_array);
}
$fun s8 * pop_s8ptr(vector64_t dynamic_array[1]) {
     return (s8 *) pop_i64(dynamic_array);
}
$fun int32_t pop_i32(vector32_t dynamic_array[1]) {
    ale_assert(dynamic_array->len > 0, "POP ON 32bit EMPTY ARRAY");

    return dynamic_array->data[--dynamic_array->len];
}
$fun float32_t pop_f32(vector32_t dynamic_array[1]) {
    int32_t replica = pop_i32(dynamic_array);
    float32_t val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val; 
}

// Vector as C Array
$fun int64_t * vector_data_as_i64(vector64_t dynamic_array[1]) {
    return (int64_t *) dynamic_array->data;
}
$fun float64_t * vector_data_as_f64(vector64_t dynamic_array[1]) {
    return (float64_t *) dynamic_array->data;
}
$fun cstring * vector_data_as_cstring(vector64_t dynamic_array[1]) {
    return (cstring *) dynamic_array->data;
}
$fun void * * vector_data_as_ptr(vector64_t dynamic_array[1]) {
    return (void * *) dynamic_array->data;
}
$fun s8 * vector_data_as_s8ptr(vector64_t dynamic_array[1]) {
    return (s8 *) dynamic_array->data;
}
$fun int32_t * vector_data_as_i32(vector32_t dynamic_array[1]) {
    return (int32_t *) dynamic_array->data;
}
$fun float32_t * vector_data_as_f32(vector32_t dynamic_array[1]) {
    return (float32_t *) dynamic_array->data;
}

// Vector remove by last swap
$proc vector64_remove_by_lastswap(vector64_t dynamic_array[1], int32_t index) {
    dynamic_array->data[index] = dynamic_array->data[--dynamic_array->len];
}
$proc vector32_remove_by_lastswap(vector32_t dynamic_array[1], int32_t index) {
    dynamic_array->data[index] = dynamic_array->data[--dynamic_array->len];
}

// Hash
$pure int64_t hash_cstr(cstring str) {
    cstring sp = (cstring) str;

    uint64_t h = 0x7A5662DCDF;
    while(*sp) { 
        h ^= (*(sp++)) & 255; h *= 1111111111111111111;
    }

    return (h ^ (h >> 32)) >> 1;
}
$math int64_t hash_i64(int64_t integer64) {
    uint64_t x = (uint64_t)integer64;
    x *= 0x94d049bb133111eb; 
    x ^= (x >> 30); 
    x *= 0xbf58476d1ce4e5b9; 
    
    return (x ^ (x >> 31)) >> 1;
}

// HASH TABLE : Mask-Step-Index (MSI) 

$math int32_t msi_lookup(
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

$fun msiht64 newmsi64(arena_t arena[1], int32_t expected_maxn) {
    const int32_t msi_expo = fit_pwr2_exp(expected_maxn + 64);
    ale_assert(msi_expo <= 24, "%d IS TOO BIG FOR MSI, MAX IS 2^24 - 1", expected_maxn);

    msiht64 *ht = (msiht64*)
        alloc(arena, sizeof(msiht64), alignof(msiht64), 1);
    
    ht->shift = 64 - msi_expo;
    ht->mask = (1 << msi_expo) - 1;
    ht->len = 0;
    ht->data = (entry_i64_i64 *)
        alloc(arena, sizeof(entry_i64_i64), alignof(entry_i64_i64), ht->mask + 1);

    return *ht;
}

typedef struct entry_i32_i32{int32_t key; int32_t val;}entry_i32_i32;
typedef struct msiht32{
    int32_t shift;int32_t mask; int32_t len;
    entry_i32_i32 *data;
}msiht32;

$fun msiht32 newmsi32(arena_t arena[1], int32_t expected_maxn) {
    const int32_t msi_expo = fit_pwr2_exp(expected_maxn + 64);
    ale_assert(msi_expo <= 24, "%d IS TOO BIG FOR MSI, MAX IS 2^24 - 1", expected_maxn);

    msiht32 *ht = (msiht32*)
        alloc(arena, sizeof(msiht32), alignof(msiht32), 1);
    
    ht->shift = 64 - msi_expo;
    ht->mask = (1 << msi_expo) - 1;
    ht->len = 0;
    ht->data = (entry_i32_i32 *)
        alloc(arena, sizeof(entry_i32_i32), alignof(entry_i32_i32), ht->mask + 1);

    return *ht;
}

// MSI Hash Table with Integer64 as Key
// Finds the index of |keyi64| in the msi |table|, creates key if |create_if_not_found| is true
$fun int32_t msiki(
    msiht64 ht[1] /* msi_ht */, 
    int64_t keyi64, int32_t create_if_not_found
) {
    entry_i64_i64 *data = ht->data;

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
$get int32_t msiki_get_idx(msiht64 table, int64_t ikey) {
    return msiki(&table, ikey, 0);
}
$get int64_t msiki_get_i64(msiht64 table, int64_t ikey) {
    return table.data[msiki(&table, ikey, 0)].val;
}
$get float64_t msiki_get_f64(msiht64 table, int64_t ikey) {
    int64_t replica = table.data[msiki(&table, ikey, 0)].val;
    float64_t val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val;
}
$get cstring msiki_get_cstr(msiht64 table, int64_t ikey) {
    return (cstring) table.data[msiki(&table, ikey, 0)].val;
}
$get void * msiki_get_ptr(msiht64 table, int64_t ikey) {
    return (void *) table.data[msiki(&table, ikey, 0)].val;
}

// Creates key if not found, then returns the index of |ikey| in the msi |table|
$set int32_t msiki_set_idx(msiht64 table[1], int64_t ikey) {
    return msiki(table, ikey, 1);
}
$set int64_t msiki_set_i64(msiht64 table[1], int64_t ikey, int64_t ival) {
    table->data[msiki(table, ikey, 1)].val = ival;
    return ival;
}
$set float64_t msiki_set_f64(msiht64 table[1], int64_t ikey, float64_t dval) {
    int64_t replica;
    ale_memcpy(&replica, &dval, sizeof(replica)); //type prunning

    table->data[msiki(table, ikey, 1)].val = replica;
    return dval;
}
$set cstring msiki_set_cstr(msiht64 table[1], int64_t ikey, cstring sval) {
    table->data[msiki(table, ikey, 1)].val = (int64_t) sval;
    return sval;
}
$fun void * msiki_set_ptr(msiht64 table[1], int64_t ikey, void * pval) {
    table->data[msiki(table, ikey, 1)].val = (int64_t) pval;
    return pval;
}
$fun entry_i64_i64 * msiki_data_as_int64(msiht64 table[1]) {
    return (entry_i64_i64 *) table->data;
}
typedef struct entry_i64_f64{int64_t key; float64_t val;}entry_i64_f64;
$fun entry_i64_f64 * msiki_data_as_f64(msiht64 table[1]) {
    return (entry_i64_f64 *) table->data;
}
typedef struct entry_i64_cstr{int64_t key; cstring val;}entry_i64_cstr;
$fun entry_i64_cstr * msiki_data_as_cstr(msiht64 table[1]) {
    return (entry_i64_cstr *) table->data;
}
typedef struct entry_i64_ptr{int64_t key; void * val;}entry_i64_ptr;
$fun entry_i64_ptr * msiki_data_as_ptr(msiht64 table[1]) {
    return (entry_i64_ptr *) table->data;
}

// MSI Hash Table with Cstring as Key
// Finds the index of |keycstr| in the msi |table|, creates key if |create_if_not_found| is true
$fun int32_t msiks(
    msiht64 ht[1] /* msi_ht */, 
    cstring keycstr, int32_t create_if_not_found
) {
    entry_i64_i64 *data = ht->data;

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
$get int32_t msiks_get_idx(msiht64 table, cstring skey) {
    return msiks(&table, skey, 0);
}
$get int64_t msiks_get_i64(msiht64 table, cstring skey) {
    return (int64_t) table.data[msiks(&table, skey, 0)].val;
}
$get float64_t msiks_get_f64(msiht64 table, cstring skey) {
    int64_t replica = table.data[msiks(&table, skey, 0)].val;
    float64_t val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val;
}
$get cstring msiks_get_cstr(msiht64 table, cstring skey) {
    return (cstring) table.data[msiks(&table, skey, 0)].val;
}
$get void * msiks_get_ptr(msiht64 table, cstring skey) {
    return (void *) table.data[msiks(&table, skey, 0)].val;
}

// Creates key if not found, then returns the index of |skey| in the msi |table|
$set int32_t msiks_set_idx(msiht64 table[1], cstring skey) {
    return msiks(table, skey, 1);
}
$set int64_t msiks_set_i64(msiht64 table[1], cstring skey, int64_t ival) {
    table->data[msiks(table, skey, 1)].val = ival;
    return ival;
}
$set float64_t msiks_set_f64(msiht64 table[1], cstring skey, float64_t dval) {
    int64_t replica;
    ale_memcpy(&replica, &dval, sizeof(replica)); //type prunning

    table->data[msiks(table, skey, 1)].val = replica;
    return dval;
}
$set cstring msiks_set_cstr(msiht64 table[1], cstring skey, cstring sval) {
    table->data[msiks(table, skey, 1)].val = (int64_t) sval;
    return sval;
}
$fun void * msiks_set_ptr(msiht64 table[1], cstring skey, void * pval) {
    table->data[msiks(table, skey, 1)].val = (int64_t) pval;
    return pval;
}

typedef struct entry_cstr_i64{cstring key; int64_t val;}entry_cstr_i64;
$fun entry_cstr_i64 * msiks_data_as_int64(msiht64 table[1]) {
    return (entry_cstr_i64 *) table->data;
}
typedef struct entry_cstr_f64{cstring key; float64_t val;}entry_cstr_f64;
$fun entry_cstr_f64 * msiks_data_as_f64(msiht64 table[1]) {
    return (entry_cstr_f64 *) table->data;
}
typedef struct entry_cstr_cstr{cstring key; cstring val;}entry_cstr_cstr;
$fun entry_cstr_cstr * msiks_data_as_cstr(msiht64 table[1]) {
    return (entry_cstr_cstr *) table->data;
}
typedef struct entry_cstr_ptr{cstring key; void * val;}entry_cstr_ptr;
$fun entry_cstr_ptr * msiks_data_as_ptr(msiht64 table[1]) {
    return (entry_cstr_ptr *) table->data;
}

// MSI Hash Table with Integer32 as Key
$fun int32_t msiki32(
    msiht32 ht[1] /* msi_ht */, 
    int32_t keyi32, int32_t create_if_not_found
) {
    entry_i32_i32 *data = ht->data;

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
$get int32_t msiki32_get_idx(msiht32 table, int32_t ikey) {
    return msiki32(&table, ikey, 0);
}
$get int32_t msiki_get_i32(msiht32 table, int32_t ikey) {
    return table.data[msiki32(&table, ikey, 0)].val;
}
$get float32_t msiki_get_f32(msiht32 table, int32_t ikey) {
    int32_t replica = table.data[msiki32(&table, ikey, 0)].val;
    float32_t val = 0;
    ale_memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val;
}

// Creates key if not found, then returns the index of |ikey| in the msi |table|
$set int32_t msiki32_set_idx(msiht32 table[1], int32_t ikey) {
    return msiki32(table, ikey, 1);
}
$set int32_t msiki_set_i32(msiht32 table[1], int32_t ikey, int32_t ival) {
    table->data[msiki32(table, ikey, 1)].val = ival;
    return ival;
}
$set float32_t msiki_set_f32(msiht32 table[1], int32_t ikey, float32_t dval) {
    int32_t replica;
    ale_memcpy(&replica, &dval, sizeof(replica)); //type prunning

    table->data[msiki32(table, ikey, 1)].val = replica;
    return dval;
}
$fun entry_i32_i32 * msiki_data_as_int32(msiht32 table[1]) {
    return (entry_i32_i32 *) table->data;
}
typedef struct entry_i32_f32{int32_t key; float32_t val;}entry_i32_f32;
$fun entry_i32_f32 * msiki_data_as_f32(msiht32 table[1]) {
    return (entry_i32_f32 *) table->data;
}

// FILES
$fun size_t ale_fread_noex(void * __restrict__ __dst, size_t __sz, size_t __n, FILE * __restrict__ __f) {
    #ifdef __cplusplus
        try { return ale_fread(__dst, __sz, __n, __f); } catch(...) {return 0;}
    #endif 
              return ale_fread(__dst, __sz, __n, __f);
}

$fun modstring file_to_buffer(arena_t arena[1], cstring filename) {
    modstring contents = 0;

    {FILE *f = 
    ale_fopen(filename, "rb");
    
        if (!f) {
            return contents;
        }
    
        ale_fseek(f, 0, SEEK_END);
        int32_t fsize = ale_ftell(f);
        ale_fseek(f, 0, SEEK_SET);

        contents = (modstring) alloc(arena, sizeof(char), alignof(char), fsize+1);
        if (!contents) {
            ale_fclose(f);
            return contents;
        }
        int32_t bytesread = (int32_t) ale_fread_noex(contents, sizeof(char), fsize, f);
        if (bytesread != fsize) {
            ale_printf("could not read fsize:%d bytes, bytesread:%d ", fsize, bytesread);
        }
        contents[fsize] = 0;

    ale_fclose(f);
    }
    
    return contents;
}

// Alters a text by converting \n to \0 and pushing each line as a cstring in the returned vector
$fun vector64_t slice_into_lines(arena_t arena[1], modstring text_to_alter) {
    vector64_t lines = {.cap=0, .len=0, .data=0};
    
    for (modstring cursor = text_to_alter, current = text_to_alter; *cursor != '\0'; ++cursor) {
        if (*cursor == '\r') {
            *cursor = '\0';
        }
        else if (*cursor == '\n') {
            *cursor = '\0';
            
            push_cstr(&lines,  arena, current);
            current = cursor+1;
        }
    }

    return lines;
}

$pure int void_compare_cstrings(const void *a, const void *b) {
    return ale_strcmp(*(cstring *)a, *(cstring *)b);
}
$get b32_t is_empty_cstring(cstring str) {
    for (cstring cursor = str; *cursor != '\0'; ++cursor) {
        if (*cursor != ' ') {
            return False;
        }
    }
    return True;
}

// Alters a text by converting \n to \0 and pushing each nonempty line as a cstring in the returned vector
$fun vector64_t slice_into_nonempty_lines(arena_t arena[1], modstring text_to_alter) {
    vector64_t lines = {.cap=0, .len=0, .data=0};
    
    for (modstring cursor = text_to_alter, current = text_to_alter; *cursor != '\0'; ++cursor) {
        if (*cursor == '\r') {
            *cursor = '\0';
        }
        else if (*cursor == '\n') {
            *cursor = '\0';
            
            if (!is_empty_cstring(current)) {
                push_cstr(&lines,  arena, current);
            }
            current = cursor+1;
        }
    }

    return lines;
}

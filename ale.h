#pragma once

#include <assert.h>  // standard assert
#include <stdarg.h>  // allows variadic functions
#include <stdint.h>  // explicit size ints
#include <string.h>  // memory operations

/*
    ==================== TYPES ====================
*/

#define _Mega_Bytes 1048576 // malloc(52*_Mega_Bytes)

// Float
typedef float float32_t;
typedef double float64_t;

// Bool
typedef int32_t b32_t;     // int to be used as boolean
static const b32_t True = 1;
static const b32_t False = 0;

// String
typedef const char * cstr_t; // const string
typedef const char * const sstr_t; // static string
typedef char * mstr_t; // modifiable string

typedef struct s8_struct{ int32_t len; mstr_t data; }s8_struct; // slice struct
typedef s8_struct * s8str_t; // slice string

#ifdef _MSC_VER
    #define gcc_attr(...) static //do NOT use attributes in MSVC
#endif
#ifndef _MSC_VER
    #define gcc_attr(...) __attribute((__VA_ARGS__)) static // Use attributes in GCC and Clang
#endif
// Simple attributes
#define $math gcc_attr(const, warn_unused_result)
#define $pure gcc_attr(pure, nonnull, warn_unused_result)
#define $fun gcc_attr(nonnull, warn_unused_result)
#define $proc gcc_attr(nonnull) void
// Parametric attributes
#define $format(paramidx_bufferlen, paramidx_buffer, paramidx_format, paramidx_varargs) \
    gcc_attr(access(read_only, paramidx_buffer, paramidx_bufferlen), format(printf, paramidx_format, paramidx_varargs))
#define $read_buffer(paramidx_bufferlen, paramidx_buffer) \
    gcc_attr(nonnull, warn_unused_result, access(read_only, paramidx_buffer, paramidx_bufferlen))
#define $malloc(paramidx_elementsize, paramidx_elementcount) \
    gcc_attr(malloc, alloc_size(paramidx_elementsize, paramidx_elementcount), nonnull, warn_unused_result, no_sanitize ("leak"))

/*
    ==================== MATH ====================
*/

// Random
$fun int32_t rnd(uint64_t seed[1]) {
    *seed = *seed * 0x9b60933458e17d7dLL + 0xd737232eeccdf7edLL;
    int32_t shift = 29 - (uint32_t)(*seed >> 61);
    
    return ((int32_t) (*seed >> shift)) & 2147483647;
}

// Hash
$pure int64_t hash_str(cstr_t str) {
    cstr_t sp = (cstr_t) str;

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

// Mask-Step-Index (MSI) lookup
$math int32_t ht_lookup(
    uint64_t hash, // 1st hash acts as base location
    int32_t index, // 2nd "hash" steps over the "list of elements" from base-location
    int32_t mask, // trims number to < ht_capacity
    int32_t shift // use |exp| bits for step 
)
{
    uint32_t step = (uint32_t)(hash >> shift) | 1;
    return (index + step) & mask;
}

// bitmask for optimized Mod for power 2 numbers
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

/*
    ==================== ARENA ALLOCATION ====================
*/

// ARENA
typedef struct arena_t{ uint8_t *beg; uint8_t *end; }arena_t;

$read_buffer(/*bufferlen*/1, /*buffer*/2) 
arena_t newarena(int64_t buffer_len, uint8_t buffer[]) {
    arena_t arena = {0, 0};
    arena.beg = (uint8_t *)buffer;
    arena.end = arena.beg ? arena.beg + buffer_len : 0;
    return arena;
}

// Arena Allocator that always zeroes the memory, always 8 aligned
$malloc(/*size*/2, /*count*/3)
void * alloc(arena_t arena[1], int64_t size, int64_t count) {
    int64_t total = size * count;
    int64_t pad = MODPWR2(- (int64_t)arena->beg, 8); //mod -x gives n for next align

    assert(total < (arena->end - arena->beg - pad) && "ARENA OUT OF MEMORY");

    uint8_t *p = arena->beg + pad;
    arena->beg += pad + total;
    
    return memset(p, 0, total);
}

/*
    ==================== DATA STRUCTURES ====================
*/

// VECTOR (dynamic array)
typedef struct vector64_t{int32_t cap; int32_t len; int64_t *data;}vector64_t;

$proc vec64_grow(vector64_t dynamic_array[1],  arena_t arena[1]) { 
    static const int32_t DYNA_FIRST_SIZE = 64;

    if (!dynamic_array->data) {
        /*       DYNA_START */ dynamic_array->data = (int64_t *)
            alloc(arena, 8LL, dynamic_array->cap = DYNA_FIRST_SIZE); 
    } else if (arena->beg == ((uint8_t *) &(dynamic_array->data[dynamic_array->cap]))) { 
        int64_t *DYNA_EXTEND = (int64_t *)
            alloc(arena, 8LL, dynamic_array->cap);
        assert((DYNA_EXTEND == &(dynamic_array->data[dynamic_array->cap])) && "extend misaligned");
        dynamic_array->cap *= 2;
    } else {
        int64_t *DYNA_RELOC = (int64_t *)
            alloc(arena, 8LL, dynamic_array->cap *= 2);
        memcpy(DYNA_RELOC, dynamic_array->data, 8LL*dynamic_array->len);
        dynamic_array->data = DYNA_RELOC;
    }
}

typedef struct vector32_t{int32_t cap; int32_t len; int32_t *data;}vector32_t;

$proc vec32_grow(vector32_t dynamic_array[1], arena_t arena[1]) { 
    static const int32_t DYNA_FIRST_SIZE = 64;

    if (!dynamic_array->data) {
        /*       DYNA_START */ dynamic_array->data = (int32_t *)
            alloc(arena, 4LL, dynamic_array->cap = DYNA_FIRST_SIZE); 
    } else if (arena->beg == ((uint8_t *) &(dynamic_array->data[dynamic_array->cap]))) { 
        int32_t *DYNA_EXTEND = (int32_t *)
            alloc(arena, 4LL, dynamic_array->cap);
        assert((DYNA_EXTEND == &(dynamic_array->data[dynamic_array->cap])) && "extend misaligned");
        dynamic_array->cap *= 2;
    } else {
        int32_t *DYNA_RELOC = (int32_t *)
            alloc(arena, 4LL, dynamic_array->cap *= 2);
        memcpy(DYNA_RELOC, dynamic_array->data, 4LL*dynamic_array->len);
        dynamic_array->data = DYNA_RELOC;
    }
}

// Vector Push
$proc vec_push_i64(vector64_t dynamic_array[1], arena_t arena[1], int64_t int64) {
    if (dynamic_array->len >= dynamic_array->cap) {
        vec64_grow(dynamic_array, arena);
    }

    dynamic_array->data[dynamic_array->len++] = int64;
}
$proc vec_push_f64(vector64_t dynamic_array[1], arena_t arena[1], float64_t float64) {
    int64_t replica;
    memcpy(&replica, &float64, sizeof(replica)); //type prunning

    vec_push_i64(dynamic_array, arena, replica);
}
$proc vec_push_str(vector64_t dynamic_array[1], arena_t arena[1], cstr_t cstr) {
    vec_push_i64(dynamic_array, arena, (int64_t) cstr);
}
$proc vec_push_ptr(vector64_t dynamic_array[1], arena_t arena[1], void * ptr) {
    vec_push_i64(dynamic_array, arena, (int64_t) ptr);
}
$proc vec_push_s8str(vector64_t dynamic_array[1], arena_t arena[1], s8str_t s8str) {
    vec_push_i64(dynamic_array, arena, (int64_t) s8str);
}
$proc vec_push_i32(vector32_t dynamic_array[1], arena_t arena[1], int32_t int32) {
    if (dynamic_array->len >= dynamic_array->cap) {
        vec32_grow(dynamic_array, arena);
    }

    dynamic_array->data[dynamic_array->len++] = int32;
}
$proc vec_push_f32(vector32_t dynamic_array[1], arena_t arena[1], float32_t float32) {
    int32_t replica;
    memcpy(&replica, &float32, sizeof(replica)); //type prunning

    vec_push_i32(dynamic_array, arena, replica);
}

// Vector Pop
$fun int64_t vec_pop_i64(vector64_t dynamic_array[1]) {
    assert(dynamic_array->len > 0 && "POP ON 64bit EMPTY ARRAY");

    return dynamic_array->data[--dynamic_array->len];
}
$fun float64_t vec_pop_f64(vector64_t dynamic_array[1]) {
    int64_t replica = vec_pop_i64(dynamic_array);
    float64_t val = 0;
    memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val; 
}
$fun cstr_t vec_pop_str(vector64_t dynamic_array[1]) {
     return (cstr_t) vec_pop_i64(dynamic_array);
}
$fun void * vec_pop_ptr(vector64_t dynamic_array[1]) {
     return (void *) vec_pop_i64(dynamic_array);
}
$fun s8str_t vec_pop_s8str(vector64_t dynamic_array[1]) {
     return (s8str_t) vec_pop_i64(dynamic_array);
}
$fun int32_t vec_pop_i32(vector32_t dynamic_array[1]) {
    assert(dynamic_array->len > 0 && "POP ON 32bit EMPTY ARRAY");

    return dynamic_array->data[--dynamic_array->len];
}
$fun float32_t vec_pop_f32(vector32_t dynamic_array[1]) {
    int32_t replica = vec_pop_i32(dynamic_array);
    float32_t val = 0;
    memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val; 
}

// Vector as C Array
$fun int64_t * vec_data_as_i64(vector64_t dynamic_array[1]) {
    return (int64_t *) dynamic_array->data;
}
$fun float64_t * vec_data_as_f64(vector64_t dynamic_array[1]) {
    assert(sizeof(float64_t) == 8 && "ale.h assumes that a double == 64bits");
    return (float64_t *) dynamic_array->data;
}
$fun cstr_t * vec_data_as_cstr(vector64_t dynamic_array[1]) {
    assert(sizeof(cstr_t) == 8 && "ale.h assumes that a pointer == 64bits");
    return (cstr_t *) dynamic_array->data;
}
$fun int32_t * vec_data_as_i32(vector32_t dynamic_array[1]) {
    return (int32_t *) dynamic_array->data;
}
$fun float32_t * vec_data_as_f32(vector32_t dynamic_array[1]) {
    assert(sizeof(float32_t) == 4 && "ale.h assumes that a double == 32bits");
    return (float32_t *) dynamic_array->data;
}

// Vector remove by last swap
$proc vec64_rem(vector64_t dynamic_array[1], int32_t index) {
    dynamic_array->data[index] = dynamic_array->data[--dynamic_array->len];
}
$proc vec32_rem(vector32_t dynamic_array[1], int32_t index) {
    dynamic_array->data[index] = dynamic_array->data[--dynamic_array->len];
}

typedef struct entry64_t{int64_t key; int64_t val;}entry64_t;
typedef struct ht64_t{
    int32_t shift;int32_t mask; int32_t len;
    entry64_t *data;
}ht64_t;

$fun ht64_t new_ht64(arena_t arena[1], int32_t expected_maxn) {
    const int32_t ht_expo = fit_pwr2_exp(expected_maxn + 64);
    assert(ht_expo <= 24 && "IS TOO BIG FOR MSI, MAX IS 2^24 - 1");

    ht64_t *ht = (ht64_t*)
        alloc(arena, sizeof(ht64_t), 1);
    
    ht->shift = 64 - ht_expo;
    ht->mask = (1 << ht_expo) - 1;
    ht->len = 0;
    ht->data = (entry64_t *)
        alloc(arena, sizeof(entry64_t), ht->mask + 1);

    return *ht;
}

typedef struct entry32_t{int32_t key; int32_t val;}entry32_t;
typedef struct ht32_t{
    int32_t shift;int32_t mask; int32_t len;
    entry32_t *data;
}ht32_t;

$fun ht32_t new_ht32(arena_t arena[1], int32_t expected_maxn) {
    const int32_t ht_expo = fit_pwr2_exp(expected_maxn + 64);
    assert(ht_expo <= 24 && "IS TOO BIG FOR MSI, MAX IS 2^24 - 1");

    ht32_t *ht = (ht32_t*)
        alloc(arena, sizeof(ht32_t), 1);
    
    ht->shift = 64 - ht_expo;
    ht->mask = (1 << ht_expo) - 1;
    ht->len = 0;
    ht->data = (entry32_t *)
        alloc(arena, sizeof(entry32_t), ht->mask + 1);

    return *ht;
}

// MSI Hash Table with Integer64 as Key
// Finds the index of |keyi64| in the msi |table|, creates key if |create_if_not_found| is true
$fun int32_t htnum(
    ht64_t ht[1] /* ht_ht */, 
    int64_t keyi64, int32_t create_if_not_found
) {
    entry64_t *data = ht->data;

    uint64_t hash = (uint64_t) hash_i64(keyi64);
    int32_t index = (int32_t)hash;
    int32_t mask = ht->mask;
    int32_t shift = ht->shift;
    
    for(
        index = ht_lookup(hash, index, mask, shift);
        data[index].key != 0 && data[index].key != keyi64; 
        index = ht_lookup(hash, index, mask, shift)
    ) {
        /* empty body */
    }

    if (data[index].key == 0 && create_if_not_found) {
        assert(ht->len < mask - 2 && "MSI KI HT IS FULL");
        data[index].key = keyi64;
        ++ht->len;
    }

    return index; // index of entry found OR entry empty
}

// Returns the index of |ikey| in the msi |table|
$fun int32_t htnum_get_idx(ht64_t table, int64_t ikey) {
    return htnum(&table, ikey, 0);
}
$fun int64_t htnum_get_i64(ht64_t table, int64_t ikey) {
    return table.data[htnum(&table, ikey, 0)].val;
}
$fun float64_t htnum_get_f64(ht64_t table, int64_t ikey) {
    int64_t replica = table.data[htnum(&table, ikey, 0)].val;
    float64_t val = 0;
    memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val;
}
$fun cstr_t htnum_get_str(ht64_t table, int64_t ikey) {
    return (cstr_t) table.data[htnum(&table, ikey, 0)].val;
}
$fun void * htnum_get_ptr(ht64_t table, int64_t ikey) {
    return (void *) table.data[htnum(&table, ikey, 0)].val;
}

// Creates key if not found, then returns the index of |ikey| in the msi |table|
$proc htnum_set_idx(ht64_t table[1], int64_t ikey) {
    htnum(table, ikey, 1);
}
$proc htnum_set_i64(ht64_t table[1], int64_t ikey, int64_t ival) {
    table->data[htnum(table, ikey, 1)].val = ival;
}
$proc htnum_set_f64(ht64_t table[1], int64_t ikey, float64_t dval) {
    int64_t replica;
    memcpy(&replica, &dval, sizeof(replica)); //type prunning

    table->data[htnum(table, ikey, 1)].val = replica;
}
$proc htnum_set_str(ht64_t table[1], int64_t ikey, cstr_t sval) {
    table->data[htnum(table, ikey, 1)].val = (int64_t) sval;
}
$proc htnum_set_ptr(ht64_t table[1], int64_t ikey, void * pval) {
    table->data[htnum(table, ikey, 1)].val = (int64_t) pval;
}
$fun entry64_t * htnum_data_as_int64(ht64_t table[1]) {
    return (entry64_t *) table->data;
}
typedef struct entry_i64_f64{int64_t key; float64_t val;}entry_i64_f64;
$fun entry_i64_f64 * htnum_data_as_f64(ht64_t table[1]) {
    return (entry_i64_f64 *) table->data;
}
typedef struct entry_i64_str{int64_t key; cstr_t val;}entry_i64_str;
$fun entry_i64_str * htnum_data_as_str(ht64_t table[1]) {
    return (entry_i64_str *) table->data;
}
typedef struct entry_i64_ptr{int64_t key; void * val;}entry_i64_ptr;
$fun entry_i64_ptr * htnum_data_as_ptr(ht64_t table[1]) {
    return (entry_i64_ptr *) table->data;
}

// MSI Hash Table with Cstring as Key
// Finds the index of |keycstr| in the msi |table|, creates key if |create_if_not_found| is true
$fun int32_t htstr(
    ht64_t ht[1] /* ht_ht */, 
    cstr_t keycstr, int32_t create_if_not_found
) {
    entry64_t *data = ht->data;

    uint64_t hash = (uint64_t) hash_str(keycstr);
    int32_t index = (int32_t)hash;
    int32_t mask = ht->mask;
    int32_t shift = ht->shift;
    
    for(
        index = ht_lookup(hash, index, mask, shift);
        data[index].key != 0 && strcmp((cstr_t) data[index].key, keycstr);
        index = ht_lookup(hash, index, mask, shift)
    ) {
        /* empty body */
    }

    if (data[index].key == 0 && create_if_not_found) {
        assert(ht->len < mask - 2 && "MSI KS HT IS FULL");
        data[index].key = (int64_t) keycstr;
        ++ht->len;
    }

    return index; // index of entry found OR entry empty
}
// Returns the index of |ikey| in the msi |table|
$fun int32_t htstr_get_idx(ht64_t table, cstr_t skey) {
    return htstr(&table, skey, 0);
}
$fun int64_t htstr_get_i64(ht64_t table, cstr_t skey) {
    return (int64_t) table.data[htstr(&table, skey, 0)].val;
}
$fun float64_t htstr_get_f64(ht64_t table, cstr_t skey) {
    int64_t replica = table.data[htstr(&table, skey, 0)].val;
    float64_t val = 0;
    memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val;
}
$fun cstr_t htstr_get_str(ht64_t table, cstr_t skey) {
    return (cstr_t) table.data[htstr(&table, skey, 0)].val;
}
$fun void * htstr_get_ptr(ht64_t table, cstr_t skey) {
    return (void *) table.data[htstr(&table, skey, 0)].val;
}

// Creates key if not found, then returns the index of |skey| in the msi |table|
$proc htstr_set_idx(ht64_t table[1], cstr_t skey) {
    htstr(table, skey, 1);
}
$proc htstr_set_i64(ht64_t table[1], cstr_t skey, int64_t ival) {
    table->data[htstr(table, skey, 1)].val = ival;
}
$proc htstr_set_f64(ht64_t table[1], cstr_t skey, float64_t dval) {
    int64_t replica;
    memcpy(&replica, &dval, sizeof(replica)); //type prunning

    table->data[htstr(table, skey, 1)].val = replica;
}
$proc htstr_set_str(ht64_t table[1], cstr_t skey, cstr_t sval) {
    table->data[htstr(table, skey, 1)].val = (int64_t) sval;
}
$proc htstr_set_ptr(ht64_t table[1], cstr_t skey, void * pval) {
    table->data[htstr(table, skey, 1)].val = (int64_t) pval;
}

typedef struct entry_str_i64{cstr_t key; int64_t val;}entry_str_i64;
$fun entry_str_i64 * htstr_data_as_int64(ht64_t table[1]) {
    return (entry_str_i64 *) table->data;
}
typedef struct entry_str_f64{cstr_t key; float64_t val;}entry_str_f64;
$fun entry_str_f64 * htstr_data_as_f64(ht64_t table[1]) {
    return (entry_str_f64 *) table->data;
}
typedef struct entry_str_str{cstr_t key; cstr_t val;}entry_str_str;
$fun entry_str_str * htstr_data_as_str(ht64_t table[1]) {
    return (entry_str_str *) table->data;
}
typedef struct entry_str_ptr{cstr_t key; void * val;}entry_str_ptr;
$fun entry_str_ptr * htstr_data_as_ptr(ht64_t table[1]) {
    return (entry_str_ptr *) table->data;
}

// MSI Hash Table with Integer32 as Key
$fun int32_t htint(
    ht32_t ht[1] /* ht_ht */, 
    int32_t keyi32, int32_t create_if_not_found
) {
    entry32_t *data = ht->data;

    uint64_t hash = (uint64_t) hash_i64(keyi32);
    int32_t index = (int32_t)hash;
    int32_t mask = ht->mask;
    int32_t shift = ht->shift;
    
    for(
        index = ht_lookup(hash, index, mask, shift);
        data[index].key != 0 && data[index].key != keyi32; 
        index = ht_lookup(hash, index, mask, shift)
    ) {
        /* empty body */
    }

    if (data[index].key == 0 && create_if_not_found) {
        assert(ht->len < mask - 2 && "MSI KI32 HT IS FULL");
        data[index].key = keyi32;
        ++ht->len;
    }

    return index; // index of entry found OR entry empty
}

// Returns the index of |ikey| in the msi |table|
$fun int32_t htint_get_idx(ht32_t table, int32_t ikey) {
    return htint(&table, ikey, 0);
}
$fun int32_t htint_get_i32(ht32_t table, int32_t ikey) {
    return table.data[htint(&table, ikey, 0)].val;
}
$fun float32_t htint_get_f32(ht32_t table, int32_t ikey) {
    int32_t replica = table.data[htint(&table, ikey, 0)].val;
    float32_t val = 0;
    memcpy(&val, &replica, sizeof(replica)); //type prunning

    return val;
}

// Creates key if not found, then returns the index of |ikey| in the msi |table|
$proc htint_set_idx(ht32_t table[1], int32_t ikey) {
    htint(table, ikey, 1);
}
$proc htint_set_i32(ht32_t table[1], int32_t ikey, int32_t ival) {
    table->data[htint(table, ikey, 1)].val = ival;
}
$proc htint_set_f32(ht32_t table[1], int32_t ikey, float32_t dval) {
    int32_t replica;
    memcpy(&replica, &dval, sizeof(replica)); //type prunning

    table->data[htint(table, ikey, 1)].val = replica;
}
$fun entry32_t * htint_data_as_int32(ht32_t table[1]) {
    return (entry32_t *) table->data;
}
typedef struct entry_i32_f32{int32_t key; float32_t val;}entry_i32_f32;
$fun entry_i32_f32 * htint_data_as_f32(ht32_t table[1]) {
    return (entry_i32_f32 *) table->data;
}

/*
    ==================== TEXT / STRINGS ====================
*/

// Alters a text by converting \n to \0 and pushing each line as a cstr_t in the returned vector
$fun vector64_t slice_into_lines(arena_t arena[1], mstr_t text_to_alter) {
    vector64_t lines = {0, 0, 0};
    
    for (mstr_t cursor = text_to_alter, current = text_to_alter; *cursor != '\0'; ++cursor) {
        if (*cursor == '\r') {
            *cursor = '\0';
        }
        else if (*cursor == '\n') {
            *cursor = '\0';
            
            vec_push_str(&lines,  arena, current);
            current = cursor+1;
        }
    }

    return lines;
}

static int void_compare_strings(const void *a, const void *b) {
    return strcmp(*(cstr_t *)a, *(cstr_t *)b);
}
$fun b32_t is_empty_string(cstr_t str) {
    for (cstr_t cursor = str; *cursor != '\0'; ++cursor) {
        if (*cursor != ' ') {
            return False;
        }
    }
    return True;
}

// Alters a text by converting \n to \0 and pushing each nonempty line as a cstr_t in the returned vector
$fun vector64_t slice_into_nonempty_lines(arena_t arena[1], mstr_t text_to_alter) {
    vector64_t lines = {0, 0, 0};
    
    for (mstr_t cursor = text_to_alter, current = text_to_alter; *cursor != '\0'; ++cursor) {
        if (*cursor == '\r') {
            *cursor = '\0';
        }
        else if (*cursor == '\n') {
            *cursor = '\0';
            
            if (!is_empty_string(current)) {
                vec_push_str(&lines,  arena, current);
            }
            current = cursor+1;
        }
    }

    return lines;
}

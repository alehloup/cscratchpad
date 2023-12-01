#pragma once

/*
    ==================== COMPILER SPECIFIC ====================
*/ 
    #ifdef stdout
        // Print Assert if stdio.h was included
        #define diagnostic_ printf("ASSERT FAILED %s:%s:%d", __FILE__, __func__, __LINE__)
    #endif
    #ifndef stdout
        static int assert_trapped_ = 0;
        #define diagnostic_ assert_trapped_ = 1
    #endif
    #ifdef _MSC_VER
        // MSVC
        #define assert(c) if(!(c)) (diagnostic_, __debugbreak())
        #define gcc_attr(...) //do NOT use attributes in MSVC
    #endif
    #ifndef _MSC_VER
        // GCC Clang
        #define assert(c) if(!(c)) (diagnostic_, __builtin_trap())
        #define gcc_attr(...) __attribute((unused, __VA_ARGS__)) // Use attributes in GCC and Clang
    #endif

    // Simple attributes
    #define _math gcc_attr(const, warn_unused_result) static
    #define _pure gcc_attr(pure, nonnull, warn_unused_result) static
    #define _nonnull gcc_attr(nonnull) static
    #define _fun gcc_attr(nonnull, warn_unused_result) static
    #define _proc gcc_attr(nonnull) static void
    // Standard functions attributes
    #define _main \
        gcc_attr(nonnull, warn_unused_result, access(read_only, 2, 1)) i32
    #define _malloc(paramidx_elementsize, paramidx_elementcount) \
        gcc_attr(malloc, assume_aligned(8), alloc_size(paramidx_elementsize, paramidx_elementcount), \
            nonnull, warn_unused_result) static
    #define _format(paramidx_bufferlen, paramidx_buffer, paramidx_format, paramidx_varargs) \
        gcc_attr(access(read_only, paramidx_buffer, paramidx_bufferlen), \
            format(printf, paramidx_format, paramidx_varargs)) static
    // Buffer attributes
    #define _fun_rbuffer(paramidx_bufferlen, paramidx_buffer) \
        gcc_attr(nonnull, warn_unused_result, \
            access(read_only, paramidx_buffer, paramidx_bufferlen)) static
    #define _proc_rbuffer(paramidx_bufferlen, paramidx_buffer) \
        gcc_attr(nonnull, \
            access(read_only, paramidx_buffer, paramidx_bufferlen)) static void 
    #define _fun_wbuffer(paramidx_bufferlen, paramidx_buffer) \
        gcc_attr(nonnull, warn_unused_result, \
            access(write_only, paramidx_buffer, paramidx_bufferlen)) static
    #define _proc_wbuffer(paramidx_bufferlen, paramidx_buffer) \
        gcc_attr(nonnull, \
            access(write_only, paramidx_buffer, paramidx_bufferlen)) static void 
//  ^^^^^^^^^^^^^^^^^^^^ COMPILER SPECIFIC ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== TYPES ====================
*/
#define MBs_ 1048576 // malloc(52*MBs_)

// Bool
typedef int b32; // Boolean
static const b32 True = 1;
static const b32 False = 0;

// Int
typedef unsigned char u8;
typedef unsigned long long u64;
typedef signed char i8;
typedef int i32;
typedef long long i64;

// Float
typedef float f32;
typedef double f64;

// String
typedef const char * const cstr; // const string const pointer
typedef char * mstr; // modifiable string
typedef const char * pstr; // const string

typedef struct s8_struct{ i32 len; mstr data; }s8_struct; // slice struct
typedef s8_struct * s8str; // slice string
//  ^^^^^^^^^^^^^^^^^^^^ TYPES ^^^^^^^^^^^^^^^^^^^^

/*
    STRINGS
*/
_pure i64 cstrlen(cstr cstring) {
    i64 len;
    for (len = 0; cstring[len]; ++len) {
        /* Empty Body */
    }
    return len;
}

_pure i32 cstrcmp(cstr cstr1, cstr cstr2) {
    i64 i = 0;
    for (i = 0; cstr1[i] && cstr2[i] && cstr1[i] == cstr2[i]; ++i) {
        /* Empty Body */
    }
    return cstr1[i] - cstr2[i];
}

_pure b32 startswith(cstr string, cstr prefix) {
    i64 i = 0;
    if (!prefix[0]) {
        return True;
    }
    if (!string[0]) {
        return False;
    }

    for (i = 0; string[i] && prefix[i] && string[i] == prefix[i]; ++i) {
        /* Empty Body */
    }

    if (!string[i] && prefix[i]) { 
        return False;
    } else if (!prefix[i]) {
        return True;
    } else {
        return False;
    }
}

_pure i32 void_compare_strings(const void *a, const void *b) {
    return cstrcmp(*(cstr *)a, *(cstr *)b);
}

_pure b32 is_empty_string(cstr string) {
    for (i64 i = 0; string[i]; ++i) {
        if (string[i] != ' ') {
            return False;
        }
    }
    return True;
}

_math b32 is_digit(char character) {
    return character >= '0' && character <= '9';
}

// Returns the digit value, if as character or in english lowercase name, or -1 if its not a digit
_fun i8 named_digit(cstr string) {
    if (string[0] && is_digit(string[0])) {
        return string[0] - '0';
    }

    cstr named[] = {"zero", "one", "two", "three", "four", 
                    "five", "six", "seven", "eight", "nine"};
    for (i8 i = 0; i < 10; ++i) {
        if (startswith(string, named[i])) {
            return i;
        }
    }

    return -1;
}

_pure i64 cstr_to_num(cstr str) {
    i64 num = 0, power = 1;
    for (i64 i = cstrlen(str) - 1; i >= 0; --i) {
        char character = str[i];

        if (character == '-') {
            return -num;
        }
        
        if (is_digit(character)) {
            num += (character - '0') * power;
            power *= 10;
        } else {
            return num;
        }
    }
    return num;
}
//  ^^^^^^^^^^^^^^^^^^^^ STRINGS ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== RANDOM ====================
*/
_pure i32 rnd(u64 seed[1]) {
    i32 shift = 29;
    *seed = *seed * 0x9b60933458e17d7dULL + 0xd737232eeccdf7edULL;
    shift -= (i32)(*seed >> 61);
    
    return ((i32) (*seed >> shift)) & 2147483647;
}
//  ^^^^^^^^^^^^^^^^^^^^ RANDOM ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== HASH ====================
*/
_pure i64 hash_str(cstr str) {
    u64 h = 0x7A5662DCDF;
    for(i64 i = 0; str[i]; ++i) { 
        h ^= str[i] & 255; h *= 1111111111111111111;
    }

    return (i64) ((h ^ (h >> 32)) >> 1);
}
_math i64 hash_i64(i64 integer64) {
    u64 x = (u64)integer64;
    x *= 0x94d049bb133111eb; 
    x ^= (x >> 30); 
    x *= 0xbf58476d1ce4e5b9; 
    
    return (i64) ((x ^ (x >> 31)) >> 1);
}
//  ^^^^^^^^^^^^^^^^^^^^ HASH ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== TYPE PRUNNING ====================
*/
typedef union i64_f64_union{i64 as_int; f64 as_float;}i64_f64_union;
_math f64 reinterpret_i64_as_f64(i64 int64) {
    i64_f64_union u = {int64};
    return u.as_float;
}
_math i64 reinterpret_f64_as_i64(f64 float64) {
    i64_f64_union u = {0};
    u.as_float = float64;
    return u.as_int;
}
typedef union i32_f32_union{i32 as_int; f32 as_float;}i32_f32_union;
_math f32 reinterpret_i32_as_f32(i32 int32) {
    i32_f32_union u = {int32};
    return u.as_float;
}
_math i32 reinterpret_f32_as_i32(f32 float32) {
    i32_f32_union u = {0};
    u.as_float = float32;
    return u.as_int;
}
//  ^^^^^^^^^^^^^^^^^^^^ TYPE PRUNNING ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== MATH ====================
*/
// bitmask for optimized Mod for power 2 numbers
_math i64 MODPWR2(i64 number, i64 modval) {
    return (number) & (modval - 1);
}

// Returns first power 2 that size+1 fits (it starts at 2^9 == 512)
_math i32 fit_pwr2_exp(i32 size) {
    i32 exp=2; i32 val=4; ++size;
    while (val < size) {
        ++exp; val*=2;
    }
    return exp;
}
//  ^^^^^^^^^^^^^^^^^^^^ MATH ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== ARENA ALLOCATION ====================
*/
typedef struct arena_t{ u8 *beg; u8 *end; }arena_t;

_fun_rbuffer(/*bufferlen*/1, /*buffer*/2) 
arena_t newarena(i64 buffer_len, u8 buffer[]) {
    arena_t arena = {0, 0};
    arena.beg = (u8 *)buffer;
    arena.end = arena.beg ? arena.beg + buffer_len : 0;
    return arena;
}

_fun_wbuffer(/*bufferlen*/2, /*buffer*/1) 
u8 * zeromem(u8 * __restrict dst, i64 count) {
    for (i64 i = 0; i < count; ++i) {
        dst[i] = 0;
    }
    return dst;
}

// Arena Allocator that always zeroes the memory, always 8 aligned
_malloc(/*size*/2, /*count*/3)
void * alloc(arena_t arena[1], i64 size, i64 count) {
    i64 total = size * count;
    i64 pad = MODPWR2(- (i64)arena->beg, 8); //mod -x gives n for next align

    u8 *p = arena->beg + pad;

    assert(total < (arena->end - arena->beg - pad) && "ARENA OUT OF MEMORY");
    arena->beg += pad + total;
    
    return (void *) zeromem(p, total);
}

_proc_wbuffer(/*bufferlen*/ 3, /*buffer*/ 1) 
copymem64(i64 * __restrict dst64, const i64 * __restrict src64, i64 count) {
    for (i64 i = 0; i < count; ++i) {
        dst64[i] = src64[i];
    }
}
_proc_wbuffer(/*bufferlen*/ 3, /*buffer*/ 1) 
copymem32(i32 * __restrict dst32, const i32 * __restrict src32, i64 count) {
    for (i64 i = 0; i < count; ++i) {
        dst32[i] = src32[i];
    }
}
//  ^^^^^^^^^^^^^^^^^^^^ ARENA ALLOCATION ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== VECTOR ====================
*/
typedef struct vector64_t{i32 cap; i32 len; i64 *data;}vector64_t;

_proc vec64_grow(vector64_t vector[1],  arena_t arena[1]) { 
    static const i32 VEC_FIRST_SIZE = 64;

    if        ("VEC START" && !vector->data) {
        vector->data = (i64 *) alloc(arena, 8LL, vector->cap = VEC_FIRST_SIZE); 
    } else if ("VEC EXTEND" && arena->beg == ((u8 *) &(vector->data[vector->cap]))) { 
        i64 *VEC_EXTEND = (i64 *) alloc(arena, 8LL, vector->cap);
        assert((VEC_EXTEND == &(vector->data[vector->cap])) && "extend misaligned");
        vector->cap *= 2;
    } else if ((i64)"VEC RELOC") {
        i64 *VEC_RELOC = (i64 *) alloc(arena, 8LL, vector->cap *= 2);
        copymem64(VEC_RELOC, vector->data, vector->len);
        vector->data = VEC_RELOC;
    }
}

typedef struct vector32_t{i32 cap; i32 len; i32 *data;}vector32_t;

_proc vec32_grow(vector32_t vector[1], arena_t arena[1]) { 
    static const i32 VEC_FIRST_SIZE = 64;

    if        ("VEC START" && !vector->data) { 
        vector->data = (i32 *) alloc(arena, 4LL, vector->cap = VEC_FIRST_SIZE); 
    } else if ("VEC EXTEND" && arena->beg == ((u8 *) &(vector->data[vector->cap]))) { 
        i32 *VEC_EXTEND = (i32 *) alloc(arena, 4LL, vector->cap);
        assert((VEC_EXTEND == &(vector->data[vector->cap])) && "extend misaligned");
        vector->cap *= 2;
    } else if ((i64)"VEC RELOC") {
        i32 *VEC_RELOC = (i32 *) alloc(arena, 4LL, vector->cap *= 2);
        copymem32(VEC_RELOC, vector->data, vector->len);
        vector->data = VEC_RELOC;
    }
}

// Vector Push
_proc vec_push_i64(vector64_t vector[1], arena_t arena[1], i64 int64) {
    if (vector->len >= vector->cap) {
        vec64_grow(vector, arena);
    }

    vector->data[vector->len++] = int64;
}
_proc vec_push_f64(vector64_t vector[1], arena_t arena[1], f64 float64) {
    vec_push_i64(vector, arena, reinterpret_f64_as_i64(float64));
}
_proc vec_push_str(vector64_t vector[1], arena_t arena[1], cstr cstring) {
    vec_push_i64(vector, arena, (i64) cstring);
}
_proc vec_push_ptr(vector64_t vector[1], arena_t arena[1], void * ptr) {
    vec_push_i64(vector, arena, (i64) ptr);
}
_proc vec_push_s8str(vector64_t vector[1], arena_t arena[1], s8str s8string) {
    vec_push_i64(vector, arena, (i64) s8string);
}
_proc vec_push_i32(vector32_t vector[1], arena_t arena[1], i32 int32) {
    if (vector->len >= vector->cap) {
        vec32_grow(vector, arena);
    }

    vector->data[vector->len++] = int32;
}
_proc vec_push_f32(vector32_t vector[1], arena_t arena[1], f32 float32) {
    vec_push_i32(vector, arena, reinterpret_f32_as_i32(float32));
}

// Vector Pop
_fun i64 vec_pop_i64(vector64_t vector[1]) {
    assert(vector->len > 0 && "POP ON 64bit EMPTY ARRAY");

    return vector->data[--vector->len];
}
_fun f64 vec_pop_f64(vector64_t vector[1]) {
    return reinterpret_i64_as_f64(vec_pop_i64(vector));
}
_fun pstr vec_pop_str(vector64_t vector[1]) {
    i64 cstr_as_i64 = vec_pop_i64(vector);
    return (pstr) cstr_as_i64;
}
_fun void * vec_pop_ptr(vector64_t vector[1]) {
    i64 pointer_as_i64 = vec_pop_i64(vector);
    return (void *) pointer_as_i64;
}
_fun s8str vec_pop_s8str(vector64_t vector[1]) {
    i64 s8str_as_i64 = vec_pop_i64(vector);
    return (s8str) s8str_as_i64;
}
_fun i32 vec_pop_i32(vector32_t vector[1]) {
    assert(vector->len > 0 && "POP ON 32bit EMPTY ARRAY");

    return vector->data[--vector->len];
}
_fun f32 vec_pop_f32(vector32_t vector[1]) {
    return reinterpret_i32_as_f32(vec_pop_i32(vector));
}

// Vector as C Array
_fun i64 * vec_data_as_i64(vector64_t vector[1]) {
    return (i64 *) vector->data;
}
_fun f64 * vec_data_as_f64(vector64_t vector[1]) {
    assert(sizeof(f64) == 8 && "ale.h vec_data_as_f64 assumes that a double == 64bits");
    return (f64 *) vector->data;
}
_fun pstr * vec_data_as_cstr(vector64_t vector[1]) {
    assert(sizeof(cstr) == 8 && "ale.h vec_data_as_cstr assumes that a pointer == 64bits");
    return (pstr *) vector->data;
}
_fun i32 * vec_data_as_i32(vector32_t vector[1]) {
    return (i32 *) vector->data;
}
_fun f32 * vec_data_as_f32(vector32_t vector[1]) {
    assert(sizeof(f32) == 4 && "ale.h vec_data_as_f32 assumes that a float == 32bits");
    return (f32 *) vector->data;
}

// Vector remove by last swap
_proc vec64_rem(vector64_t vector[1], i32 index) {
    vector->data[index] = vector->data[--vector->len];
}
_proc vec32_rem(vector32_t vector[1], i32 index) {
    vector->data[index] = vector->data[--vector->len];
}
//  ^^^^^^^^^^^^^^^^^^^^ VECTOR ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== HASH TABLE ====================
*/
typedef struct entry64_t{i64 key; i64 val;}entry64_t;
typedef struct ht64_t{
    i32 shift;i32 mask; i32 len;
    entry64_t *data;
}ht64_t;

_fun ht64_t new_ht64(arena_t arena[1], i32 expected_maxn) {
    ht64_t *ht = (ht64_t*)
        alloc(arena, sizeof(ht64_t), 1);

    const i32 ht_expo = fit_pwr2_exp(expected_maxn + 64);
    assert(ht_expo <= 24 && "IS TOO BIG FOR MSI, MAX IS 2^24 - 1");
    
    ht->shift = 64 - ht_expo;
    ht->mask = (1 << ht_expo) - 1;
    ht->len = 0;
    ht->data = (entry64_t *)
        alloc(arena, sizeof(entry64_t), ht->mask + 1);

    return *ht;
}

typedef struct entry32_t{i32 key; i32 val;}entry32_t;
typedef struct ht32_t{
    i32 shift;i32 mask; i32 len;
    entry32_t *data;
}ht32_t;

_fun ht32_t new_ht32(arena_t arena[1], i32 expected_maxn) {
    ht32_t *ht = (ht32_t*)
        alloc(arena, sizeof(ht32_t), 1);

    const i32 ht_expo = fit_pwr2_exp(expected_maxn + 64);
    assert(ht_expo <= 24 && "IS TOO BIG FOR MSI, MAX IS 2^24 - 1");
    
    ht->shift = 64 - ht_expo;
    ht->mask = (1 << ht_expo) - 1;
    ht->len = 0;
    ht->data = (entry32_t *)
        alloc(arena, sizeof(entry32_t), ht->mask + 1);

    return *ht;
}

// Mask-Step-Index (MSI) lookup
_math i32 ht_lookup(
    u64 hash, // 1st hash acts as base location
    i32 index, // 2nd "hash" steps over the "list of elements" from base-location
    i32 mask, // trims number to < ht_capacity
    i32 shift // use |exp| bits for step 
)
{
    i32 step = (i32)(hash >> shift) | 1;
    return (i32) ((index + step) & mask);
}

// MSI Hash Table with Integer64 as Key
// Finds the index of |keyi64| in the msi |table|, creates key if |create_if_not_found| is true
_nonnull i32 htnum(
    ht64_t ht[1] /* ht_ht */, 
    i64 keyi64, i32 create_if_not_found
) {
    entry64_t *data = ht->data;

    u64 hash = (u64) hash_i64(keyi64);
    i32 index = (i32)hash;
    i32 mask = ht->mask;
    i32 shift = ht->shift;
    
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
_fun i32 htnum_get_idx(ht64_t table, i64 ikey) {
    return htnum(&table, ikey, 0);
}
_fun i64 htnum_get_i64(ht64_t table, i64 ikey) {
    return table.data[htnum(&table, ikey, 0)].val;
}
_fun f64 htnum_get_f64(ht64_t table, i64 ikey) {
    return reinterpret_i64_as_f64(table.data[htnum(&table, ikey, 0)].val);
}
_fun pstr htnum_get_str(ht64_t table, i64 ikey) {
    return (pstr) table.data[htnum(&table, ikey, 0)].val;
}
_fun void * htnum_get_ptr(ht64_t table, i64 ikey) {
    return (void *) table.data[htnum(&table, ikey, 0)].val;
}

// Creates key if not found, then returns the index of |ikey| in the msi |table|
_proc htnum_set_idx(ht64_t table[1], i64 ikey) {
    htnum(table, ikey, 1);
}
_proc htnum_set_i64(ht64_t table[1], i64 ikey, i64 ival) {
    table->data[htnum(table, ikey, 1)].val = ival;
}
_proc htnum_set_f64(ht64_t table[1], i64 ikey, f64 dval) {
    table->data[htnum(table, ikey, 1)].val = reinterpret_f64_as_i64(dval);
}
_proc htnum_set_str(ht64_t table[1], i64 ikey, cstr sval) {
    table->data[htnum(table, ikey, 1)].val = (i64) sval;
}
_proc htnum_set_ptr(ht64_t table[1], i64 ikey, void * pval) {
    table->data[htnum(table, ikey, 1)].val = (i64) pval;
}
_fun entry64_t * htnum_data_as_int64(ht64_t table[1]) {
    return (entry64_t *) table->data;
}
typedef struct entry_i64_f64{i64 key; f64 val;}entry_i64_f64;
_fun entry_i64_f64 * htnum_data_as_f64(ht64_t table[1]) {
    return (entry_i64_f64 *) table->data;
}
typedef struct entry_i64_str{i64 key; cstr val;}entry_i64_str;
_fun entry_i64_str * htnum_data_as_str(ht64_t table[1]) {
    return (entry_i64_str *) table->data;
}
typedef struct entry_i64_ptr{i64 key; void * val;}entry_i64_ptr;
_fun entry_i64_ptr * htnum_data_as_ptr(ht64_t table[1]) {
    return (entry_i64_ptr *) table->data;
}

// MSI Hash Table with Cstring as Key
// Finds the index of |keycstr| in the msi |table|, creates key if |create_if_not_found| is true
_nonnull i32 htstr(
    ht64_t ht[1] /* ht_ht */, 
    cstr keycstr, i32 create_if_not_found
) {
    entry64_t *data = ht->data;

    u64 hash = (u64) hash_str(keycstr);
    i32 index = (i32)hash;
    i32 mask = ht->mask;
    i32 shift = ht->shift;
    
    for(
        index = ht_lookup(hash, index, mask, shift);
        data[index].key != 0 && cstrcmp((pstr) data[index].key, keycstr);
        index = ht_lookup(hash, index, mask, shift)
    ) {
        /* empty body */
    }

    if (data[index].key == 0 && create_if_not_found) {
        assert(ht->len < mask - 2 && "MSI KS HT IS FULL");
        data[index].key = (i64) keycstr;
        ++ht->len;
    }

    return index; // index of entry found OR entry empty
}
// Returns the index of |ikey| in the msi |table|
_fun i32 htstr_get_idx(ht64_t table, cstr skey) {
    return htstr(&table, skey, 0);
}
_fun i64 htstr_get_i64(ht64_t table, cstr skey) {
    return (i64) table.data[htstr(&table, skey, 0)].val;
}
_fun f64 htstr_get_f64(ht64_t table, cstr skey) {
    return reinterpret_i64_as_f64(table.data[htstr(&table, skey, 0)].val);
}
_fun pstr htstr_get_str(ht64_t table, cstr skey) {
    return (pstr) table.data[htstr(&table, skey, 0)].val;
}
_fun void * htstr_get_ptr(ht64_t table, cstr skey) {
    return (void *) table.data[htstr(&table, skey, 0)].val;
}

// Creates key if not found, then returns the index of |skey| in the msi |table|
_proc htstr_set_idx(ht64_t table[1], cstr skey) {
    htstr(table, skey, 1);
}
_proc htstr_set_i64(ht64_t table[1], cstr skey, i64 ival) {
    table->data[htstr(table, skey, 1)].val = ival;
}
_proc htstr_set_f64(ht64_t table[1], cstr skey, f64 dval) {
    table->data[htstr(table, skey, 1)].val = reinterpret_f64_as_i64(dval);
}
_proc htstr_set_str(ht64_t table[1], cstr skey, cstr sval) {
    table->data[htstr(table, skey, 1)].val = (i64) sval;
}
_proc htstr_set_ptr(ht64_t table[1], cstr skey, void * pval) {
    table->data[htstr(table, skey, 1)].val = (i64) pval;
}

typedef struct entry_str_i64{cstr key; i64 val;}entry_str_i64;
_fun entry_str_i64 * htstr_data_as_int64(ht64_t table[1]) {
    return (entry_str_i64 *) table->data;
}
typedef struct entry_str_f64{cstr key; f64 val;}entry_str_f64;
_fun entry_str_f64 * htstr_data_as_f64(ht64_t table[1]) {
    return (entry_str_f64 *) table->data;
}
typedef struct entry_str_str{cstr key; cstr val;}entry_str_str;
_fun entry_str_str * htstr_data_as_str(ht64_t table[1]) {
    return (entry_str_str *) table->data;
}
typedef struct entry_str_ptr{cstr key; void * val;}entry_str_ptr;
_fun entry_str_ptr * htstr_data_as_ptr(ht64_t table[1]) {
    return (entry_str_ptr *) table->data;
}

// MSI Hash Table with Integer32 as Key
_nonnull i32 htint(
    ht32_t ht[1] /* ht_ht */, 
    i32 keyi32, i32 create_if_not_found
) {
    entry32_t *data = ht->data;

    u64 hash = (u64) hash_i64(keyi32);
    i32 index = (i32)hash;
    i32 mask = ht->mask;
    i32 shift = ht->shift;
    
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
_fun i32 htint_get_idx(ht32_t table, i32 ikey) {
    return htint(&table, ikey, 0);
}
_fun i32 htint_get_i32(ht32_t table, i32 ikey) {
    return table.data[htint(&table, ikey, 0)].val;
}
_fun f32 htint_get_f32(ht32_t table, i32 ikey) {
    return reinterpret_i32_as_f32(table.data[htint(&table, ikey, 0)].val);
}

// Creates key if not found, then returns the index of |ikey| in the msi |table|
_proc htint_set_idx(ht32_t table[1], i32 ikey) {
    htint(table, ikey, 1);
}
_proc htint_set_i32(ht32_t table[1], i32 ikey, i32 ival) {
    table->data[htint(table, ikey, 1)].val = ival;
}
_proc htint_set_f32(ht32_t table[1], i32 ikey, f32 float32) {
    table->data[htint(table, ikey, 1)].val = reinterpret_f32_as_i32(float32);
}
_fun entry32_t * htint_data_as_int32(ht32_t table[1]) {
    return (entry32_t *) table->data;
}
typedef struct entry_i32_f32{i32 key; f32 val;}entry_i32_f32;
_fun entry_i32_f32 * htint_data_as_f32(ht32_t table[1]) {
    return (entry_i32_f32 *) table->data;
}
//  ^^^^^^^^^^^^^^^^^^^^ HASH TABLE ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== TEXT ====================
*/
// Alters a text by converting \n to \0 and pushing each line as a cstr in the returned vector
_fun vector64_t slice_into_lines(arena_t arena[1], char text_to_alter[1]) {
    vector64_t lines = {0, 0, 0};
    
    i64 last_line = 0;
    for (i64 i = 0, current = 0; text_to_alter[i]; ++i) {
        if (text_to_alter[i] == '\r') {
            text_to_alter[i] = '\0';
        }
        else if (text_to_alter[i] == '\n') {
            text_to_alter[i] = '\0';
            
            vec_push_str(&lines,  arena, &text_to_alter[current]);
            last_line = current;
            current = i+1;
        }
    }
    if (!is_empty_string(&text_to_alter[last_line])) {
        vec_push_str(&lines,  arena, "");
    }

    return lines;
}

// Alters a text by converting \n to \0 and pushing each nonempty line as a cstr in the returned vector
_fun vector64_t slice_into_nonempty_lines(arena_t arena[1], char text_to_alter[1]) {
    vector64_t lines = {0, 0, 0};
    
    for (i64 i = 0, current = 0; text_to_alter[i]; ++i) {
        if (text_to_alter[i] == '\r') {
            text_to_alter[i] = '\0';
        }
        else if (text_to_alter[i] == '\n') {
            text_to_alter[i] = '\0';
            
            if (!is_empty_string(&text_to_alter[current])) {
                vec_push_str(&lines,  arena, &text_to_alter[current]);
            }
            current = i+1;
        }
    }

    return lines;
}
//  ^^^^^^^^^^^^^^^^^^^^ TEXT ^^^^^^^^^^^^^^^^^^^^

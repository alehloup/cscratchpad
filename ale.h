#pragma once

/*
    ==================== ASSERT ====================
*/ 
#ifdef stdout
    // Print Assert if stdio.h was included
    #define diagnostic_ printf("ASSERT FAILED %s:%s:%d", __FILE__, __func__, __LINE__)
#endif
#ifndef stdout
    static int assert_trapped_ = 0;
    #define diagnostic_ assert_trapped_ = 1
#endif
#define assert(c) if(!(c)) (diagnostic_, __builtin_trap())
//  ^^^^^^^^^^^^^^^^^^^^ ASSERT ^^^^^^^^^^^^^^^^^^^^
    
/*
    ==================== ATTRIBUTES ====================
*/
#define gcc_attr(...) __attribute((__VA_ARGS__))
// Simple attributes
#define _math gcc_attr(const, warn_unused_result) static
#define _math_hot gcc_attr(const, warn_unused_result, hot) static
#define _pure gcc_attr(pure, nonnull, warn_unused_result) static
#define _pure_hot gcc_attr(pure, nonnull, warn_unused_result, hot) static
#define _nonnull gcc_attr(nonnull) static
#define _nonnull_hot gcc_attr(nonnull, hot) static
#define _fun gcc_attr(nonnull, warn_unused_result) static
#define _fun_hot gcc_attr(nonnull, hot, warn_unused_result) static
#define _proc gcc_attr(nonnull) static void
#define _proc_hot gcc_attr(nonnull, hot) static void
//  ^^^^^^^^^^^^^^^^^^^^ ATTRIBUTTES ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== TYPES ====================
*/
#define MBs_ 1048576 // malloc(52*MBs_)

// Bool
typedef int b32; // Boolean
typedef const b32 cb32;
static cb32 True = 1;
static cb32 False = 0;

// Int
typedef unsigned char u8;
typedef const u8 cu8;
typedef unsigned int u32;
typedef const u32 cu32;
typedef unsigned long long u64;
typedef const u64 cu64;
typedef signed char i8;
typedef const i8 ci8;
typedef int i32;
typedef const i32 ci32;
typedef long long i64;
typedef const i64 ci64;

// Float
typedef float f32;
typedef const f32 cf32;
typedef double f64;
typedef const f64 cf64;

// String
typedef const char cchar;
typedef cchar * const ccstr; // const string const pointer
typedef cchar * cstr; // const string
typedef char * mstr; // modifiable string

// Arena
typedef struct Arena{ u8 *beg; u8 *end; }Arena;

// Data Structures Header (stb strategy)
typedef struct ds_header{Arena *arena; i32 elsize; i32 cap; i32 len;}ds_header;
_math_hot ds_header * hd_(void * ds) {
    return ((ds_header *) ds) - 1;
}
//  ^^^^^^^^^^^^^^^^^^^^ TYPES ^^^^^^^^^^^^^^^^^^^^

/*
    STRINGS
*/
_pure_hot i64 cstrlen(ccstr cstring) {
    i64 len;
    for (len = 0; cstring[len]; ++len) {
        /* Empty Body */
    }
    return len;
}

_pure_hot i32 cstrcmp(ccstr cstr1, ccstr cstr2) {
    i64 i = 0;
    for (i = 0; cstr1[i] && cstr2[i] && cstr1[i] == cstr2[i]; ++i) {
        /* Empty Body */
    }
    return cstr1[i] - cstr2[i];
}

_pure_hot b32 startswith(ccstr string, ccstr prefix) {
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

_pure_hot i32 void_compare_strings(const void *a, const void *b) {
    return cstrcmp(*(ccstr *)a, *(ccstr *)b);
}

_pure_hot b32 is_empty_string(ccstr string) {
    for (i64 i = 0; string[i]; ++i) {
        if (string[i] != ' ') {
            return False;
        }
    }
    return True;
}

_math_hot b32 is_digit(char character) {
    return character >= '0' && character <= '9';
}

_pure_hot i64 digitlen(ccstr cstring) {
    i64 len;
    for (len = 0; is_digit(cstring[len]); ++len) {
        /* Empty Body */
    }
    return len;
}

// Returns the digit value, if as character or in english lowercase name, or -1 if its not a digit
_fun i8 named_digit(ccstr string) {
    ccstr named[] = {"zero", "one", "two", "three", "four", 
                    "five", "six", "seven", "eight", "nine"};
                    
    if (string[0] && is_digit(string[0])) {
        return string[0] - '0';
    }

    for (i8 i = 0; i < 10; ++i) {
        if (startswith(string, named[i])) {
            return i;
        }
    }

    return -1;
}

typedef struct i64num_i64len{i64 num; i64 len;}i64num_i64len;
_pure_hot i64num_i64len cstr_to_num(ccstr str) {
    i64 power = 1;
    i64num_i64len ret = {0, digitlen(str)};

    for (i64 i = ret.len - 1; i >= 0; --i) {
        char character = str[i];

        if (character == '-') {
            ret.num = -ret.num;
            return ret;
        }
        
        if (is_digit(character)) {
            ret.num += (character - '0') * power;
            power *= 10;
        } else {
            return ret;
        }
    }
    return ret;
}
//  ^^^^^^^^^^^^^^^^^^^^ STRINGS ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== RANDOM ====================
*/
_pure_hot i32 rnd(u64 seed[1]) {
    i32 shift = 29;
    *seed = *seed * 0x9b60933458e17d7dULL + 0xd737232eeccdf7edULL;
    shift -= (i32)(*seed >> 61);
    
    return ((i32) (*seed >> shift)) & 2147483647;
}
//  ^^^^^^^^^^^^^^^^^^^^ RANDOM ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== HASH ====================
*/
_pure_hot i64 hash_str(ccstr str) {
    u64 h = 0x7A5662DCDF;
    for(i64 i = 0; str[i]; ++i) { 
        h ^= str[i] & 255; h *= 1111111111111111111;
    }

    return (i64) ((h ^ (h >> 32)) >> 1);
}
_math_hot i64 hash_i64(i64 integer64) {
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
typedef union Union_i64_f64{i64 as_i64; f64 as_f64;}Union_i64_f64;
_math_hot f64 reinterpret_i64_as_f64(i64 int64) {
    Union_i64_f64 u = {int64};
    return u.as_f64;
}
_math_hot i64 reinterpret_f64_as_i64(f64 float64) {
    Union_i64_f64 u = {0};
    u.as_f64 = float64;
    return u.as_i64;
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
_math u8 fit_pwr2_exp(i32 size) {
    u8 exp=6; i32 val=64; ++size;

    assert(size <= 16777216 && "Max size allowed: 16777215");

    while (val < size) {
        ++exp; val*=2;
    }
    return exp;
}
//  ^^^^^^^^^^^^^^^^^^^^ MATH ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== ARENA ALLOCATION ====================
*/
_fun Arena newarena(i64 buffer_len, u8 buffer[]) {
    Arena arena = {0, 0};
    arena.beg = (u8 *)buffer;
    arena.end = arena.beg ? arena.beg + buffer_len : 0;
    return arena;
}

_fun_hot u8 * zeromem(u8 * __restrict dst, i64 count) {
    for (i64 i = 0; i < count; ++i) {
        dst[i] = 0;
    }
    return dst;
}
_proc_hot copymem(u8 * __restrict dst, cu8 * __restrict src, i64 count) {
    for (i64 i = 0; i < count; ++i) {
        dst[i] = src[i];
    }
}

gcc_attr(malloc, assume_aligned(8), alloc_size(2, 3), nonnull, warn_unused_result, hot) static
// Arena Allocator always zeroes the memory, always 8 aligned
void * alloc(Arena arena[1], i64 size, i64 count) {
    i64 total = size * count;
    i64 pad = MODPWR2(- (i64)arena->beg, 8); //mod -x gives n for next align

    u8 *p = arena->beg + pad;

    assert(total < (arena->end - arena->beg - pad) && "ARENA OUT OF MEMORY");
    arena->beg += pad + total;
    
    return (void *) zeromem(p, total);
}
//  ^^^^^^^^^^^^^^^^^^^^ ARENA ALLOCATION ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== VECTOR ====================
*/
_fun_hot void * new_vec(Arena arena[1], i32 elsize) {
    ds_header * vec = (ds_header *)alloc(arena, (i64)sizeof(ds_header) + (64*elsize), 1);
    vec->elsize = elsize;
    vec->cap = 64;
    vec->len = 0;
    vec->arena = arena;
    return vec + 1;
}
#define VNEW(arena, type) (type *) new_vec(arena, (i32)sizeof(type))

_fun_hot u8 * grow_vec(u8 * arr) { 
    ds_header * dh = hd_(arr);
    Arena *arena = dh->arena;
    cu8 *capend = &arr[dh->cap * dh->elsize];

    if ("VEC EXTEND" && arena->beg == capend) {
        u8 *VEC_EXTEND = (u8 *)alloc(arena, dh->elsize, dh->cap);
        
        assert((u64)VEC_EXTEND == (u64)capend && "extend misaligned");
    } else if ((u64)"VEC RELOC") {
        ds_header *VEC_RELOC = (ds_header *)
            alloc(arena, (dh->elsize*dh->cap * 2) + (i64)sizeof(ds_header), 1);
        u8 *newarr = (u8 *)(VEC_RELOC + 1);

        copymem(newarr, arr, dh->cap * dh->elsize);
        arr = newarr;
        copymem((u8 *)VEC_RELOC, (u8 *)dh, (i64)sizeof(ds_header));
        dh = VEC_RELOC;
    }
    dh->cap <<= 1;

    return arr;
}

_fun_hot void * push_vec(void * ptr_to_array) {
    u8 * *arr = (u8 * *)ptr_to_array;
    ds_header * dh = hd_(*arr);
    
    if (dh->len >= dh->cap) {
       *arr = grow_vec(*arr);
    }

    return *arr + (dh->elsize * dh->len++);
}
#define VAPPEND(arr) *(typeof(arr))push_vec(&arr)
#define VPOP(arr) arr[--hd_(arr)->len]
//  ^^^^^^^^^^^^^^^^^^^^ VECTOR ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== HASH TABLE ====================
*/
typedef struct Entry64{i64 key; i64 val;}Entry64;
typedef struct Ht64{
    i32 cap; i32 len;
    Entry64 *data;
    u8 shift;
}Ht64;

_fun Ht64 new_Ht64(Arena arena[1], i32 expected_maxn) {
    Ht64 *ht = (Ht64*)
        alloc(arena, sizeof(Ht64), 1);

    const u8 ht_expo = fit_pwr2_exp(expected_maxn + 64);
    assert(ht_expo <= 24 && "IS TOO BIG FOR MSI, MAX IS 2^24 - 65");
    
    ht->shift = 64u - ht_expo;
    ht->cap = (1 << ht_expo) - 1;
    ht->len = 0;
    ht->data = (Entry64 *)
        alloc(arena, sizeof(Entry64), ht->cap + 2);

    return *ht;
}

// Mask-Step-Index (MSI) lookup
_math_hot i32 ht_lookup(
    u64 hash, // 1st hash acts as base location
    i32 index, // 2nd "hash" steps over the "list of elements" from base-location
    u32 mask, // trims number to < ht_capacity
    u8  shift // use |exp| bits for step 
)
{
    u32 step = (u32)(hash >> shift) | 1;
    return (i32) (((u32)index + step) & mask);
}

// Finds the index of |keyi64| in the msi |table|, creates key if |create_if_not_found| is true
_nonnull_hot i32 htloop(
    Ht64 ht[1] /* ht_ht */, 
    i64 keyi64, ccstr keystr, 
    i32 create_if_not_found
) {
    Entry64 *data = ht->data;

    u64 hash = (u64) hash_i64(keyi64);
    i32 index = (i32)hash;
    u32 mask = (u32) ht->cap;
    u8 shift = ht->shift; 
    b32 intkey = keyi64 != 0;

    if (!keyi64) {
        keyi64 = (i64) keystr;
    }

    for(
        index = ht_lookup(hash, index, mask, shift);
        data[index].key != 0 
            && data[index].key != keyi64
            && (intkey || cstrcmp((cstr) data[index].key, keystr));
        index = ht_lookup(hash, index, mask, shift)
    ) {
        /* empty body */
    }

    if (data[index].key == 0 && create_if_not_found) {
        assert(ht->len < (i32)(mask - 1) && "MSI HT IS FULL");
        data[index].key = keyi64;
        ++ht->len;
    }

    return index; // index of entry found OR entry empty
}

// Returns the index of |ikey| in the msi |table|
_fun i32 hti_get_idx(Ht64 table, i64 ikey) {
    return htloop(&table, ikey, "", 0);
}
_fun i64 hti_get_i64(Ht64 table, i64 ikey) {
    return table.data[htloop(&table, ikey, "", 0)].val;
}
_fun f64 hti_get_f64(Ht64 table, i64 ikey) {
    return reinterpret_i64_as_f64(table.data[htloop(&table, ikey, "", 0)].val);
}
_fun cstr hti_get_str(Ht64 table, i64 ikey) {
    return (cstr) table.data[htloop(&table, ikey, "", 0)].val;
}
_fun void * hti_get_ptr(Ht64 table, i64 ikey) {
    return (void *) table.data[htloop(&table, ikey, "", 0)].val;
}

// Creates key if not found, then returns the index of |ikey| in the msi |table|
_proc hti_set_idx(Ht64 table[1], i64 ikey) {
    htloop(table, ikey, "", 1);
}
_proc hti_set_i64(Ht64 table[1], i64 ikey, i64 ival) {
    table->data[htloop(table, ikey, "", 1)].val = ival;
}
_proc hti_set_f64(Ht64 table[1], i64 ikey, f64 dval) {
    table->data[htloop(table, ikey, "", 1)].val = reinterpret_f64_as_i64(dval);
}
_proc hti_set_str(Ht64 table[1], i64 ikey, ccstr sval) {
    table->data[htloop(table, ikey, "", 1)].val = (i64) sval;
}
_proc hti_set_ptr(Ht64 table[1], i64 ikey, void * pval) {
    table->data[htloop(table, ikey, "", 1)].val = (i64) pval;
}
_fun Entry64 * hti_data_as_int64(Ht64 table[1]) {
    return (Entry64 *) table->data;
}
typedef struct Entry_i64_f64{i64 key; f64 val;}Entry_i64_f64;
_fun Entry_i64_f64 * hti_data_as_f64(Ht64 table[1]) {
    return (Entry_i64_f64 *) table->data;
}
typedef struct Entry_i64_str{i64 key; ccstr val;}Entry_i64_str;
_fun Entry_i64_str * hti_data_as_str(Ht64 table[1]) {
    return (Entry_i64_str *) table->data;
}
typedef struct Entry_i64_ptr{i64 key; void * val;}Entry_i64_ptr;
_fun Entry_i64_ptr * hti_data_as_ptr(Ht64 table[1]) {
    return (Entry_i64_ptr *) table->data;
}

// Returns the index of |ikey| in the msi |table|
_fun i32 hts_get_idx(Ht64 table, ccstr skey) {
    return htloop(&table, 0, skey, 0);
}
_fun i64 hts_get_i64(Ht64 table, ccstr skey) {
    return (i64) table.data[htloop(&table, 0, skey, 0)].val;
}
_fun f64 hts_get_f64(Ht64 table, ccstr skey) {
    return reinterpret_i64_as_f64(table.data[htloop(&table, 0, skey, 0)].val);
}
_fun cstr hts_get_str(Ht64 table, ccstr skey) {
    return (cstr) table.data[htloop(&table, 0, skey, 0)].val;
}
_fun void * hts_get_ptr(Ht64 table, ccstr skey) {
    return (void *) table.data[htloop(&table, 0, skey, 0)].val;
}

// Creates key if not found, then returns the index of |skey| in the msi |table|
_proc hts_set_idx(Ht64 table[1], ccstr skey) {
    htloop(table, 0, skey, 1);
}
_proc hts_set_i64(Ht64 table[1], ccstr skey, i64 ival) {
    table->data[htloop(table, 0, skey, 1)].val = ival;
}
_proc hts_set_f64(Ht64 table[1], ccstr skey, f64 dval) {
    table->data[htloop(table, 0, skey, 1)].val = reinterpret_f64_as_i64(dval);
}
_proc hts_set_str(Ht64 table[1], ccstr skey, ccstr sval) {
    table->data[htloop(table, 0, skey, 1)].val = (i64) sval;
}
_proc hts_set_ptr(Ht64 table[1], ccstr skey, void * pval) {
    table->data[htloop(table, 0, skey, 1)].val = (i64) pval;
}

typedef struct Entry_str_i64{ccstr key; i64 val;}Entry_str_i64;
_fun Entry_str_i64 * hts_data_as_int64(Ht64 table[1]) {
    return (Entry_str_i64 *) table->data;
}
typedef struct Entry_str_f64{ccstr key; f64 val;}Entry_str_f64;
_fun Entry_str_f64 * hts_data_as_f64(Ht64 table[1]) {
    return (Entry_str_f64 *) table->data;
}
typedef struct Entry_str_str{ccstr key; ccstr val;}Entry_str_str;
_fun Entry_str_str * hts_data_as_str(Ht64 table[1]) {
    return (Entry_str_str *) table->data;
}
typedef struct Entry_str_ptr{ccstr key; void * val;}Entry_str_ptr;
_fun Entry_str_ptr * hts_data_as_ptr(Ht64 table[1]) {
    return (Entry_str_ptr *) table->data;
}
//  ^^^^^^^^^^^^^^^^^^^^ HASH TABLE ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== TEXT ====================
*/
// Alters a text by converting \n to \0 and pushing each line as a ccstr in the returned vector
_fun_hot mstr * into_lines(Arena arena[1], mstr text_to_alter) {
    mstr *lines = VNEW(arena, mstr);
    
    for (i64 i = 0, current = 0; text_to_alter[i]; ++i) {
        if (text_to_alter[i] == '\r') {
            text_to_alter[i] = '\0';
        }
        else if (text_to_alter[i] == '\n') {
            text_to_alter[i] = '\0';
            
            VAPPEND(lines) = &text_to_alter[current];
            current = i+1;
        }
    }

    return lines;
}

_fun_hot mstr * split(Arena arena[1], mstr text_to_alter, char splitter) {
    mstr *words = VNEW(arena, mstr);
    i64 i = 0, current = 0;

    for (i = 0; text_to_alter[i]; ++i) {
        if (text_to_alter[i] == splitter) {
            text_to_alter[i] = '\0';
            
            if (!is_empty_string(&text_to_alter[current])) {
                VAPPEND(words) = &text_to_alter[current];
            }
            current = i+1;
        }
    }

    if (current != i && !is_empty_string(&text_to_alter[current])) {
        VAPPEND(words) = &text_to_alter[current];
    }

    return words;
}
//  ^^^^^^^^^^^^^^^^^^^^ TEXT ^^^^^^^^^^^^^^^^^^^^

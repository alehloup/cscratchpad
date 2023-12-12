#pragma once

/*
    ==================== ASSERT ====================
*/ 
#ifdef stdout
    // Print Assert if stdio.h was included
    #define diagnostic_ printf("\n\n  |ASSERT FAILED %s:%s:%d|  \n\n", __FILE__, __func__, __LINE__)
#endif
#ifndef stdout
    static int assert_trapped_ = 0;
    #define diagnostic_ assert_trapped_ = 1
#endif

#if defined(_MSC_VER)
    #define assert(c) if(!(c)) (diagnostic_, __debugbreak())
#else
    #define assert(c) if(!(c)) (diagnostic_, __builtin_trap())
#endif
//  ^^^^^^^^^^^^^^^^^^^^ ASSERT ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== ATTRIBUTES ====================
*/
#if defined(__GNUC__) || defined(__clang__)
    #define gcc_attr(...) __attribute((nonnull, __VA_ARGS__)) inline static
#else
    #define gcc_attr(...) inline static
#endif
// 2 types of function attributes: either returns value (fun) or not (proc)
#define _fun_attr(...) gcc_attr(warn_unused_result, __VA_ARGS__)
#define _proc_attr(...) gcc_attr(__VA_ARGS__) void
// function attributes
#define _math _fun_attr(const) 
#define _math_hot _fun_attr(const, hot)
#define _pure _fun_attr(pure)
#define _pure_hot _fun_attr(pure, hot)
#define _fun _fun_attr()
#define _fun_hot _fun_attr(hot)
#define _proc _proc_attr()
#define _proc_hot _proc_attr(hot)
// for explicit discarding returns
#define discard_ (void) 
//  ^^^^^^^^^^^^^^^^^^^^ ATTRIBUTTES ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== TYPES ====================
*/
#define MBs_ 1048576 // malloc(52*MBs_)

// Bool
typedef int b32; // Boolean
typedef const b32 cb32;
#define True 1
#define False 0

// Int
typedef unsigned char u8;
typedef const u8 cu8;
typedef const u8* const ccu8;
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
typedef const char cchar; // const char
typedef cchar * const ccstr; // const string const pointer
typedef cchar * cstr; // const string
typedef char * mstr; // modifiable string

// Void
typedef void * voidp;
typedef void * const voidpc;
typedef const void * cvoidp;
typedef const void * const ccvoidp;

// Arena
typedef struct Arena{ u8 *beg; u8 *end; }Arena;

// Data Structures Header (stb strategy)
typedef struct ds_header{
    Arena *arena; 
    i32 cap; i32 len;
    u8 elsize; u8 ptrcheck; u8 is_str; u8 invalid;
}ds_header;
_pure_hot ds_header * hd_(voidpc ds) {
    return ((ds_header *) ds) - 1;
}
_pure_hot i32 hd_len_(voidpc ds) {
    return hd_(ds)->len;
}
_pure_hot b32 hd_checkptr(voidpc ds) {
    return (u8)((u64)ds) == hd_(ds)->ptrcheck;
}
//  ^^^^^^^^^^^^^^^^^^^^ TYPES ^^^^^^^^^^^^^^^^^^^^

/*
    MEMORY
*/
#define isizeof(x_element_) ((i64)sizeof(x_element_))
// COMPILE TIME Count Static Sized Array elements:
#define countof(x_array_) (isizeof(x_array_) / isizeof(*x_array_))

_fun_hot u8 * zeromem(u8 * const __restrict dst, ci64 count) {
    for (i64 i = 0; i < count; ++i) {
        dst[i] = 0;
    }
    return dst;
}
_proc_hot copymem(u8 * const __restrict dst, ccu8 __restrict src, ci64 count) {
    for (i64 i = 0; i < count; ++i) {
        dst[i] = src[i];
    }
}

_pure_hot i32 cmpmem(ccvoidp mem1_, ccvoidp mem2_, ci64 count) {
    ccu8 mem1 = (cu8 *)mem1_, mem2 = (cu8 *)mem2_;
    i64 i = 0; 
    ci64 last = count - 1;
    for (; i < last && mem1[i] == mem2[i]; ++i) {
        /* Empty Body */
    }
    
    return mem1[i] - mem2[i];
}

_pure_hot i32 is_not_zero(ccvoidp mem1_, ci64 size) {
    ccu8 mem1 = (cu8 *)mem1_;
    i64 i = 0; 
    for (; i < size; ++i) {
        if (mem1[i]) {
            return True;
        }
    }
    
    return False;
}

_pure_hot i32 is_zero(ccvoidp mem1_, ci64 size) {
    ccu8 mem1 = (cu8 *)mem1_;
    i64 i = 0; 
    for (; i < size && !mem1[i]; ++i) {
        /* Empty Body */
    }
    
    return i == size;
}
//  ^^^^^^^^^^^^^^^^^^^^ MEMORY ^^^^^^^^^^^^^^^^^^^^

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
//Only works for COMPILE TIME STRINGS:
#define compile_time_cstrlen(compile_time_string_) countof(compile_time_string_) - 1

_pure_hot i32 cstrcmp(ccstr cstr1, ccstr cstr2) {
    i64 i = 0;

    for (i = 0; cstr1[i] && cstr2[i] && cstr1[i] == cstr2[i]; ++i) {
        /* Empty Body */
    }
    return cstr1[i] - cstr2[i];
}

_pure_hot u8 is_str_typename(ccstr type_) {
    static ccstr str_types[] = {
        "cstr", "ccstr", "mstr", "cchar *", "cchar*",
        "char *", "char*", "const char*", "const char *",
        "const char *const", "const char * const"
    };

    for (int i = 0; i < countof(str_types); ++i) {
        if (!cstrcmp(str_types[i], type_)) {
            return True;
        }
    }
    return False;
}
#define IS_STR_TYPENAME(type_s_) is_str_typename(#type_s_)

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

_pure_hot i32 void_compare_strings(cvoidp a, cvoidp b) {
    return cstrcmp(*(ccstr*)a, *(ccstr*)b);
}

_pure_hot b32 is_empty_string(ccstr string) {
    for (i64 i = 0; string[i]; ++i) {
        if (string[i] != ' ') {
            return False;
        }
    }
    return True;
}

_math_hot b32 is_digit(cchar character) {
    return character >= '0' && character <= '9';
}

_pure_hot i32 digitlen(ccstr cstring) {
    i32 len;
    for (len = 0; is_digit(cstring[len]); ++len) {
        /* Empty Body */
    }
    

    return len;
}

typedef struct i64num_i32len{i64 num; i32 len;}i64num_i32len;
_pure_hot i64num_i32len cstr_to_num(ccstr str) {
    i64 power = 1;
    i64num_i32len ret = {0, digitlen(str)};

    for (i32 i = ret.len - 1; i >= 0; --i) {
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
    ==================== MATH ====================
*/
// bitmask for optimized Mod for power 2 numbers
_math_hot i64 mod_pwr2(ci64 number, ci64 modval) {
    return (number) & (modval - 1);
}

// Returns first power 2 that size fits
_math_hot u8 fit_pwr2_exp(ci32 size) {
    u8 exp=8; 
    i32 val= 1<<exp;

    assert(size <= 16777216 && "Max size allowed: 16777215");

    while (val < size) {
        ++exp; val<<= 1;
    }
    return exp;
}

_math_hot i64 greatest_common_divisor(i64 m, i64 n) {
    i64 tmp;
    while(m) { tmp = m; m = n % m; n = tmp; }       
    return n;
}
_math_hot i64 least_common_multiple(i64 m, i64 n) {
     return m / greatest_common_divisor(m, n) * n;
}

// Next Power of 2 for numbers upto 2*31 (2'147'483'648)
#define NEXT_POWER2(n_) \
    ((((n_)-1) | (((n_)-1) >> 1) | (((n_)-1) >> 2) | (((n_)-1) >> 4) | (((n_)-1) >> 8) | (((n_)-1) >> 16))+1)

//  ^^^^^^^^^^^^^^^^^^^^ MATH ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== RANDOM ====================
*/
_pure_hot i32 rnd(u64 seed[1]) {
    i32 shift = 29;
    *seed = *seed * 0x9b60933458e17d7dULL + 0xd737232eeccdf7edULL;
    shift -= (i32)(*seed >> 61);
    
    return (i32)((*seed >> shift) & 2147483647);
}
//  ^^^^^^^^^^^^^^^^^^^^ RANDOM ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== HASH ====================
*/
_pure_hot u64 hash_str(ccstr str) {
    u64 h = 0x7A5662DCDF;
    for(i64 i = 0; str[i]; ++i) { 
        h ^= str[i] & 255; h *= 1111111111111111111;
    }

    return (h ^ (h >> 32)) >> 1;
}
_pure_hot u64 hash_bytes(ccvoidp bytes_, ci64 count) {
    ccu8 bytes = (cu8 *)bytes_;
    u64 h = 0x7A5662DCDF;
    for(i64 i = 0; i < count; ++i) { 
        h ^= bytes[i] & 255; h *= 1111111111111111111;
    }

    return (h ^ (h >> 32)) >> 1;
}
//  ^^^^^^^^^^^^^^^^^^^^ HASH ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== TYPE PRUNNING ====================
*/
typedef union Union_i64_f64{i64 as_i64; f64 as_f64;}Union_i64_f64;
_math_hot f64 reinterpret_i64_as_f64(ci64 int64) {
    Union_i64_f64 u = {int64};
    return u.as_f64;
}
_math_hot i64 reinterpret_f64_as_i64(cf64 float64) {
    Union_i64_f64 u = {0};
    u.as_f64 = float64;
    return u.as_i64;
}
//  ^^^^^^^^^^^^^^^^^^^^ TYPE PRUNNING ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== ARENA ALLOCATION ====================
*/
_fun Arena new_arena(ci64 buffer_len, u8 buffer[]) {
    Arena arena = {0, 0};
    arena.beg = (u8 *)buffer;
    arena.end = arena.beg ? arena.beg + buffer_len : 0;
    return arena;
}

gcc_attr(malloc, assume_aligned(8), alloc_size(2, 3), nonnull, warn_unused_result, hot)
// Arena Allocator always zeroes the memory, always 8 aligned
voidp alloc(Arena arena[1], ci64 size, ci64 count) {
    i64 total = size * count;
    i64 pad = mod_pwr2(- (i64)arena->beg, 8); //mod -x gives n for next align

    u8 *p = arena->beg + pad;

    assert(total < (arena->end - arena->beg - pad) && "ARENA OUT OF MEMORY");
    arena->beg += pad + total;
    
    return (voidp) zeromem(p, total);
}
#define ALLOC(arena, type) (type *) alloc(arena, isizeof(type), 1)
#define ALLOCN(arena_, type_, count_) (type_ *) alloc(arena_, isizeof(type_), count_)
//  ^^^^^^^^^^^^^^^^^^^^ ARENA ALLOCATION ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== VECTOR ====================
*/
// Creates an Ale Vector, guarantees power-2 Capacity
_fun_hot voidp new_vec(Arena arena[1], cu8 elsize, ci32 initial_cap, cu8 is_str) {
    ci32 cap_to_use = NEXT_POWER2(initial_cap ? initial_cap : 256);
    ci64 alloc_size = isizeof(ds_header) + (cap_to_use * elsize);
    ds_header * vec = (ds_header *)alloc(arena, alloc_size, 1);
    
    vec->elsize = elsize;
    vec->cap = cap_to_use;
    vec->len = 0;
    vec->arena = arena;
    vec->is_str = is_str;
    vec->ptrcheck = (u8)((u64)(vec+1));
    return vec + 1;
}

//NEW VECTOR (prefixes the array with ds_header, see new_vec and hd_)
#define NEW_VEC(arena, varname_, type) \
    type *varname_ = new_vec(arena, (u8)sizeof(type), 0, IS_STR_TYPENAME(type))
#define NEW_VEC_WITH_CAP(arena, varname_, type, capacity_) \
    type *varname_ = new_vec(arena, (u8)sizeof(type), capacity_, IS_STR_TYPENAME(type))

_fun_hot u8 * grow_vec(u8 * arr) { 
    ds_header * dh = hd_(arr);
    Arena *arena = dh->arena;
    i64 cap_x_elsize = dh->cap * dh->elsize;
    ccu8 capend = &arr[cap_x_elsize];

    assert(hd_checkptr(arr) && "Vec was relloced, arr stale!");

    if (arena->beg == capend) {
        /* VEC EXTEND */
        u8 *VEC_EXTEND = (u8 *)alloc(arena, dh->elsize, dh->cap);
        assert((u64)VEC_EXTEND == (u64)capend && "extend misaligned");
        dh->cap <<= 1;
        return arr;
    } else {
        /* VEC RELOC */
        ds_header *VEC_RELOC = (ds_header *)
            alloc(arena, isizeof(ds_header) + (cap_x_elsize * 2), 1);
        u8 *newarr = (u8 *)(VEC_RELOC + 1);
        copymem((u8 *)VEC_RELOC, (u8 *)dh, isizeof(ds_header) + cap_x_elsize);
        
        VEC_RELOC->cap <<= 1;
        VEC_RELOC->ptrcheck = (u8)((u64)newarr);
        dh->ptrcheck = VEC_RELOC->ptrcheck;

        return newarr;
    }
}

// Increases len by 1 and returns last index 
_fun_hot i32 vec_inc(voidp array_by_reference) {
    u8 * *arr = (u8 * *)array_by_reference;
    ds_header * dh = hd_(*arr);
    
    if (dh->len >= dh->cap) {
       *arr = grow_vec(*arr);
    }

    return (dh->len++);
}
#define vec_inc_ref(arr) (&arr[vec_inc(&arr)])
#define vec_append(arr, value) (arr[vec_inc(&arr)] = value)
#define vec_pop(arr) arr[--hd_(arr)->len]
//  ^^^^^^^^^^^^^^^^^^^^ VECTOR ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== HASH TABLE ====================
*/
// MSI Set
#define NEW_SET(arena, varname_, type, capacity) \
    NEW_VEC_WITH_CAP(arena, varname_, type, capacity)

// MSI Hash Table
#define NEW_HTABLE(arena, prefix_name_, keytype_, valtype_, capacity) \
    NEW_VEC_WITH_CAP(arena, prefix_name_##_keys, keytype_, capacity); \
    NEW_VEC_WITH_CAP(arena, prefix_name_##_vals, valtype_, capacity)

// Mask-Step-Index (MSI) lookup
_math_hot i32 ht_lookup(
    cu64 hash, // 1st hash acts as base location
    ci32 index, // 2nd "hash" steps over the "list of elements" from base-location
    cu32 mask, // trims number to < ht_capacity
    cu8  shift // use |exp| bits for step 
)
{
    u32 step = (u32)(hash >> shift) | 1;
    return (i32) (((u32)index + step) & mask);
}

_fun_hot i32 htloop(
    voidpc keys_, cu64 key_, b32 create_if_not_found
) {
    ds_header *hd = hd_(keys_);
    cu8 elsize = hd->elsize;
    cu32 mask = (u32) (hd->cap - 1);
    cu8 shift = 64 - (fit_pwr2_exp(hd->cap));

    cu8 * const numeric_key = (cu8*)(&key_);
    u8 * const u8keys = (u8 *) keys_;
    
    ccstr string_key = hd->is_str ? (cstr) key_ : 0;
    mstr * const strkeys = (mstr *) keys_;

    cu64 hash = string_key ? hash_str(string_key) : hash_bytes(numeric_key, elsize);
    i32 index = (i32)hash;

    assert(hd_checkptr(keys_) && "This is not a ale.h vector!");
    assert(is_not_zero(numeric_key, elsize) && "Hash Table does NOT support ZERO Key");

    for(
        index = ht_lookup(hash, index, mask, shift);
        
        is_not_zero(&u8keys[index*elsize], elsize)
        && cmpmem(&u8keys[index*elsize], (cu64*)(&key_), elsize)
        && (!string_key || cstrcmp(strkeys[index], string_key));
        
        index = ht_lookup(hash, index, mask, shift)
    ) {
        /* empty body */
    }

    if (is_zero(&u8keys[index*elsize], elsize) && create_if_not_found) {
        assert(hd->len < hd->cap - 2 && "MSI HT IS FULL");
        copymem(&u8keys[index*elsize], numeric_key, elsize);
        ++hd->len;
    }

    return index; // index of entry found OR entry empty
}
#define hset_idx(keys_vec_, search_key_) htloop(keys_vec_, (u64)search_key_, 0)
#define hset_get(keys_vec_, search_key_) keys_vec_[htloop(keys_vec_, (u64)search_key_, 0)]
#define hset_set(keys_vec_, search_key_) keys_vec_[htloop(keys_vec_, (u64)search_key_, 1)]

#define htable_idx(name, search_key_) htloop(name##_keys, (u64)search_key_, 0)
#define htable_get(name, search_key_) name##_vals[htloop(name##_keys, (u64)search_key_, 0)]
#define htable_set(name, search_key_, value_) (name##_vals[htloop(name##_keys, (u64)search_key_, 1)] = value_)
//  ^^^^^^^^^^^^^^^^^^^^ HASH TABLE ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== TEXT ====================
*/
// Alters a text by converting \n to \0 and pushing each line into lines, return number of lines
_fun_hot i32 into_lines(mstr text_to_alter, i32 lines_cap, mstr lines[64]) {
    i32 lines_len = 0;
    
    for (i64 i = 0, current = 0; text_to_alter[i]; ++i) {
        if (text_to_alter[i] == '\r') {
            text_to_alter[i] = '\0';
        }
        else if (text_to_alter[i] == '\n') {
            text_to_alter[i] = '\0';
            
            assert(lines_len < lines_cap && "lines not big enough to store all lines");
            lines[lines_len++] = &text_to_alter[current];
            current = i+1;
        }
    }

    return lines_len;
}

_fun_hot i32 split(mstr text_to_alter, cchar splitter, i32 words_cap, mstr words[64]) {
    i32 i = 0, current = 0, words_len = 0;

    for (i = 0; text_to_alter[i]; ++i) {
        if (text_to_alter[i] == splitter) {
            text_to_alter[i] = '\0';
            
            if (!is_empty_string(&text_to_alter[current])) {
                assert(words_len < words_cap && "words not big enough to store all words");
                words[words_len++] = &text_to_alter[current];
            }
            current = i+1;
        }
    }

    if (current != i && !is_empty_string(&text_to_alter[current])) {
        assert(words_len < words_cap && "words not big enough to store all words");
        words[words_len++] = &text_to_alter[current];
    }

    return words_len;
}
//  ^^^^^^^^^^^^^^^^^^^^ TEXT ^^^^^^^^^^^^^^^^^^^^

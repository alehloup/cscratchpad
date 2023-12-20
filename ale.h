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
    #define _gcc_attr(...) __attribute((__VA_ARGS__)) inline static
#else
    #define _gcc_attr(...) inline static
#endif
// 2 types of function attributes: either returns value (fun) or not (proc)
#define _fun_attr(...) _gcc_attr(nonnull, warn_unused_result, __VA_ARGS__)
#define _proc_attr(...) _gcc_attr(nonnull, __VA_ARGS__) void
// math = no pointers, same inputs always produce same output
#define _math _fun_attr(const) 
#define _math_hot _fun_attr(const, hot)
// pure = do not uses global variables, uses only its input
#define _pure _fun_attr(pure)
#define _pure_hot _fun_attr(pure, hot)
// "normal" function / procedure
#define _fun _fun_attr()
#define _fun_hot _fun_attr(hot)
#define _proc _proc_attr()
#define _proc_hot _proc_attr(hot)
// always inline
#define _fun_inlined _fun_attr(always_inline)
#define _proc_inlined _proc_attr(always_inline)
//  ^^^^^^^^^^^^^^^^^^^^ ATTRIBUTTES ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== TYPES ====================
*/
//
#define MBs_ 1048576

// Int
typedef unsigned int u32;
typedef unsigned long long u64;
typedef int i32;
typedef const i32 ci32;
typedef long long i64;
typedef const i64 ci64;

// Visual Int
typedef int b32; // Boolean
typedef const b32 cb32;
typedef u64 hash64;
typedef i32 cmp32;
typedef i32 idx32;
typedef i64 idx64;
typedef i32 len32;
typedef i64 len64;
typedef ci32 cap32;
typedef ci64 cap64;

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
typedef char bufferchar; // just for visual:  bufferchar buffer[64];

// Void
typedef const void * cvoidp;
//  ^^^^^^^^^^^^^^^^^^^^ TYPES ^^^^^^^^^^^^^^^^^^^^

/*
    Keyword Alternatives
*/
//
#define True 1
#define False 0
#ifndef __cplusplus
    #define and &&
    #define or ||
    #define not !
#endif
//  ^^^^^^^^^^^^^^^^^^^^ Keyword Alternatives ^^^^^^^^^^^^^^^^^^^^

/*
    MEMORY
*/
//
#define isizeof(x_element_) ((i64)sizeof(x_element_))
#define arraysizeof(static_array_) (isizeof(static_array_) / isizeof(*static_array_))

//  ^^^^^^^^^^^^^^^^^^^^ MEMORY ^^^^^^^^^^^^^^^^^^^^

/*
    ARRAYS
*/
//
#define array_insert_in_pos(array_len_ref_, array_, element_, pos_) \
    for (idx32 ivec_insert_ = (*array_len_ref_); ivec_insert_ > pos_; --ivec_insert_) \
        array_[ivec_insert_] = array_[ivec_insert_-1]; \
    array_[pos_] = element_; \
    ++(*array_len_ref_)

//  ^^^^^^^^^^^^^^^^^^^^ ARRAYS ^^^^^^^^^^^^^^^^^^^^


/*
    STRINGS
*/
//
_pure_hot len64 cstrlen64(ccstr cstring) {
    len64 cstring_len;
    for (cstring_len = 0; cstring[cstring_len]; ++cstring_len) {
        /* Empty Body */
    }
    return cstring_len;
}
_fun_inlined len32 cstrlen32(ccstr cstring) {
    return (len32) cstrlen64(cstring);
}
//Only works for COMPILE TIME STRINGS:
#define compile_time_cstrlen(compile_time_string_) arraysizeof(compile_time_string_) - 1

_pure_hot cmp32 cstrcmp(ccstr cstr1, ccstr cstr2) {
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

_fun_inlined cmp32 void_compare_strings(cvoidp a, cvoidp b) {
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

_pure_hot len32 digitlen(ccstr cstring) {
    len32 cstring_len;
    for (cstring_len = 0; is_digit(cstring[cstring_len]); ++cstring_len) {
        /* Empty Body */
    }
    

    return cstring_len;
}

_pure_hot idx32 char_pos_in_str(cchar letter, ccstr cstring) {
    for (idx32 letter_pos = 0; cstring[letter_pos]; ++letter_pos) {
        if(cstring[letter_pos] == letter) {
            return letter_pos;
        }
    }

    return -1;
}

_fun_inlined b32 char_in_(cchar letter, ccstr cstring) {
    return (char_pos_in_str(letter, cstring) + 1);
}

// returns the index after the last element
_fun_hot idx32 cstrcpy(mstr dst, ccstr src, len64 dst_len) {
    idx32 i = 0;
    for (i = 0; i < dst_len and src[i]; ++i) {
        dst[i] = src[i];
    }
    dst[i++] = 0;
    return i;
}
//  ^^^^^^^^^^^^^^^^^^^^ STRINGS ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== MATH ====================
*/
//
#define abs_(number_) ((number_) > -1 ? (number_) : -(number_))
#define min_(number1_, number2_) ((number1_) < (number2_) ? (number1_) : (number2_))
#define max_(number1_, number2_) ((number1_) > (number2_) ? (number1_) : (number2_))

// bitmask for optimized Mod for power 2 numbers
_math_hot i64 mod_pwr2(ci64 number, ci64 modval) {
    return (number) & (modval - 1);
}

_math_hot i64 greatest_common_divisor(i64 m, i64 n) {
    i64 tmp;
    while(m) { tmp = m; m = n % m; n = tmp; }       
    return n;
}
_fun_inlined i64 least_common_multiple(i64 m, i64 n) {
     return m / greatest_common_divisor(m, n) * n;
}

// Next Power of 2 for numbers upto 2*31 (2'147'483'648)
#define Next_power2(n_) \
    ((((n_)-1) | (((n_)-1) >> 1) | (((n_)-1) >> 2) | (((n_)-1) >> 4) | (((n_)-1) >> 8) | (((n_)-1) >> 16))+1)
//  ^^^^^^^^^^^^^^^^^^^^ MATH ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== RANDOM ====================
*/
//
#define Rnd_positive_mask 2147483647
#define Rnd_mult_n 0x9b60933458e17d7dULL
#define Rnd_sum_n 0xd737232eeccdf7edULL

_pure_hot i32 rnd(u64 seed[1]) {
    i32 shift = 29;
    *seed = *seed * Rnd_mult_n + Rnd_sum_n;
    shift -= (i32)(*seed >> 61);
    
    return (i32)((*seed >> shift) & Rnd_positive_mask);
}
//  ^^^^^^^^^^^^^^^^^^^^ RANDOM ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== HASH ====================
*/
//
#define Hash_shift_mix(cur_hash_)  (cur_hash_ ^ (cur_hash_ >> 31))

#define Hash_start_n 0x7A5662DCDF
#define Hash_mul_n 1111111111111111111 // 11 ones

_pure_hot hash64 hash_str(ccstr str) {
    hash64 h = Hash_start_n;
    
    for(i64 i = 0; str[i]; ++i) { 
        h ^= str[i] & 255; h *= Hash_mul_n;
    }
    h = Hash_shift_mix(h);

    return h >> 1;
}

#define Hash_mul_n1 0x94d049bb133111eb
#define Hash_mul_n2 0xbf58476d1ce4e5b9

_math_hot hash64 hash_int(i64 integer64) {
    hash64 x = (u64)integer64;
    
    x *= Hash_mul_n1; 
    x = Hash_shift_mix(x);
    x *= Hash_mul_n2; 
    x = Hash_shift_mix(x);
    
    return x >> 1;
}
//  ^^^^^^^^^^^^^^^^^^^^ HASH ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== HASH TABLE ====================
*/

#define HT_CAP    4096 //   2 ^ 12
#define Ht_mask   (HT_CAP - 1)
#define Ht_shift  52   //  64 - 12

// Mask-Step-Index (MSI) lookup
_math_hot idx32 ht_lookup(
    hash64 hash, // 1st hash acts as base location
    ci32 index // 2nd "hash" steps over the "list of elements" from base-location
)
{
    u32 step = (u32)(hash >> Ht_shift) | 1;
    idx32 idx = (i32) (((u32)index + step) & Ht_mask);
    return idx ? idx : 1;
}

_gcc_attr(always_inline, warn_unused_result) idx32 str_in_ht_(ccstr search_key, cstr keys[HT_CAP], len32 *keys_len) {
    hash64 h = hash_str(search_key);
    idx32 i = ht_lookup(h, (i32)h);
    b32 found = False;

    while (keys[i] and cstrcmp(search_key, keys[i])) {
        i = ht_lookup(h, i);
    }

    found = keys[i] ? True : False;
    if (keys_len) {
        keys[i] = search_key;
        (*keys_len) += found;
    }

    return found ? i : - i;
}

_gcc_attr(always_inline, warn_unused_result) idx32 i32_in_ht_(i32 search_key, i32 keys[HT_CAP], len32 *keys_len) {
    hash64 h = hash_int(search_key);
    idx32 i = ht_lookup(h, (i32)h);
    b32 found = False;

    while (keys[i] and search_key != keys[i]) {
        i = ht_lookup(h, i);
    }

    found = keys[i] ? True : False;
    if (keys_len) {
        keys[i] = search_key;
        (*keys_len) += found;
    }

    return found ? i : - i;
}

_gcc_attr(always_inline, warn_unused_result) idx32 i64_in_ht_(i64 search_key, i64 keys[HT_CAP], len32 *keys_len) {
    hash64 h = hash_int(search_key);
    idx32 i = ht_lookup(h, (i32)h);
    b32 found = False;

    while (keys[i] and search_key != keys[i]) {
        i = ht_lookup(h, i);
    }

    found = keys[i] ? True : False;
    if (keys_len) {
        keys[i] = search_key;
        (*keys_len) += found;
    }

    return found ? i : - i;
}
//  ^^^^^^^^^^^^^^^^^^^^ HASH TABLE ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== TEXT ====================
*/
//
// Alters a text by converting \n to \0 and pushing each line into lines, return number of lines
_fun_hot len32 into_lines_(mstr text_to_alter, cap32 lines_cap, mstr lines[2], b32 include_empty_lines) {
    len32 lines_len = 0;
    
    idx64 current = 0;
    for (idx64 i = 0; text_to_alter[i]; ++i) {
        if (text_to_alter[i] == '\r') {
            text_to_alter[i] = '\0';
        }
        else if (text_to_alter[i] == '\n') {
            text_to_alter[i] = '\0';
            
            if (include_empty_lines || !is_empty_string(&text_to_alter[current])) {
                assert(lines_len < lines_cap && "lines not big enough to store all lines");
                lines[lines_len++] = &text_to_alter[current];
            }
            current = i+1;
        }
    }
    if (include_empty_lines || !is_empty_string(&text_to_alter[current])) {
        assert(lines_len < lines_cap && "lines not big enough to store all lines");
        lines[lines_len++] = &text_to_alter[current];
    }

    return lines_len;
}
_fun_inlined len32 into_lines(mstr text_to_alter, cap32 lines_cap, mstr lines[2]) {
    return into_lines_(text_to_alter, lines_cap, lines, False);
}
_fun_inlined len32 into_lines_including_empty(mstr text_to_alter, cap32 lines_cap, mstr lines[2]) {
    return into_lines_(text_to_alter, lines_cap, lines, True);
}

_fun_hot len32 split(mstr text_to_alter, cchar splitter, cap32 words_cap, mstr words[2]) {
    idx32 i = 0, current = 0;
    len32 words_len = 0;

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

/*
    ==================== TIME BENCHMARK ====================
*/
//
#ifdef CLOCKS_PER_SEC 
// time.h

_fun_inlined f64 seconds_since(clock_t start)
{
    return (f64)(clock() - start) / CLOCKS_PER_SEC;
}

// time.h
#endif  

#if defined CLOCKS_PER_SEC && defined stdout 
// time.h && stdio.h

_proc_inlined print_clock(clock_t start) {
    printf("\n\nExecuted in %f seconds \n", seconds_since(start));
}

// time.h && stdio.h
#endif 
//  ^^^^^^^^^^^^^^^^^^^^ TIME BENCHMARK ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== SHELL ====================
*/
//
#if defined RAND_MAX && defined stdout
// stdlib.h && stdio.h

#include <stdarg.h>

_gcc_attr(format(printf, 1, 2), nonnull)
i32 shellrun(ccstr format, ...) {
    va_list args;

    cap32 buffer_cap = 512; 
    bufferchar buffer [512] = {0};

    va_start(args, format);

    vsprintf_s(buffer, (u64) buffer_cap, format, args);
    printf("\n");
    vprintf(format, args);
    printf("\n");
    return system(buffer);
}

// stdlib.h && stdio.h
#endif 
//  ^^^^^^^^^^^^^^^^^^^^ SHELL ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== FILES ====================
*/
//
#ifdef stdout
// stdio.h

_fun_inlined i64 fread_noex(mstr dst, i64 sz, i64 count, FILE * f) {
    #ifdef __cplusplus
        try { return (i64) fread(dst, (u64) sz, (u64) count, f); } catch(...) {return 0;}
    #endif 
              return (i64) fread(dst, (u64) sz, (u64) count, f);
}
_fun_inlined i64 fwrite_noex(ccstr Str, i64 Size, i64 Count, FILE * File) {
    #ifdef __cplusplus
        try { return (i64) fwrite(Str, (u64) Size, (u64) Count, File); } catch(...) {return 0;}
    #endif 
              return (i64) fwrite(Str, (u64) Size, (u64) Count, File);
}

_fun len64 file_to_cstring(ccstr filename, cap64 charbuffer_cap, bufferchar charbuffer[2]) {
    i64 fsize = 0;

        FILE *f = 0; i32 err = 
    fopen_s(&f, filename, "rb");
    
        assert(!err && "Could not open file for reading");
    
        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        assert(charbuffer_cap >= fsize+2 && "charbuffer is not enough for file size");

        {
            i64 bytesread = fread_noex(charbuffer, 1LL, fsize, f);
            assert(bytesread == fsize && "could not read fsize#bytes"); 
            
            charbuffer[fsize] = charbuffer[fsize-1] != '\n' ? '\n' : '\0';
            charbuffer[fsize+1] = '\0';
        }

    fclose(f);

    return fsize;
}

_fun_inlined len32 file_to_lines(ccstr filename, cap32 lines_cap, mstr lines[2], cap64 charbuffer_cap, bufferchar charbuffer[2]) {
    len64 charbuffer_len = file_to_cstring(filename, charbuffer_cap, charbuffer);
    (void) charbuffer_len;
    return into_lines(charbuffer, lines_cap, lines);
}

_proc_hot cstring_to_file(ccstr buffer, ccstr filename) {
        FILE *f = 0; i32 err = 
    fopen_s(&f, filename, "wb");
        assert(!err && "Could not open file for writting");
        {
            len64 buffer_len = cstrlen64(buffer);
            i64 bytes_written = fwrite_noex(buffer, 1, buffer_len, f);
            assert(bytes_written == buffer_len && "could not write buffer_len#bytes");
        }
    fclose(f);
}

_proc_hot lines_to_file(len32 lines_len, mstr lines[2], ccstr filename) {
        FILE *f = 0; i32 err = 
    fopen_s(&f, filename, "wb");
        assert(!err && "Could not open file for writting");
        {
            i64 bytes_written = 0;
            len64 line_len = 0;

            for (i32 i = 0; i < lines_len; ++i) {
                ccstr line = lines[i]; 

                line_len = cstrlen64(line);
                bytes_written = fwrite_noex(line, 1, line_len, f);
                bytes_written += fwrite_noex("\n", 1, 1, f);
                assert(bytes_written == line_len + 1 && "could not write line_len#bytes");
            }
        }
    fclose(f);
}

// stdio.h
#endif 
//  ^^^^^^^^^^^^^^^^^^^^ FILES ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== PRINT ====================
*/
//
#ifdef stdout
// stdio.h

#define array_printf(format_str_, vec_to_print_len, vec_to_print_) \
    for (idx32 ivec_ = 0; ivec_ < vec_to_print_len; ++ivec_) \
        printf(format_str_, vec_to_print_[ivec_]); \
    printf("\n")

#define matrix_printf(format_str_, number_of_lines_, number_of_columns_, matrix_to_print_) \
    printf("Matrix %d x %d :\n", number_of_lines_, number_of_columns_); \
    for (idx32 imatrix_line_ = 0; imatrix_line_ < number_of_lines_; ++imatrix_line_) { \
        for (idx32 imatrix_column_ = 0; imatrix_column_ < number_of_columns_; ++ imatrix_column_) \
            printf(format_str_, matrix_to_print_[imatrix_line_][imatrix_column_]); \
        printf("\n"); \
    } \
    printf("\n")

// stdio.h
#endif 
//  ^^^^^^^^^^^^^^^^^^^^ PRINT ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== STDLIB ====================
*/
// 
#ifdef RAND_MAX
// stdlib.h

_proc_inlined sort_cstrings(len64 cstrings_len, mstr cstrings[1]) {
    qsort(
        cstrings, (u64) cstrings_len,
        sizeof(mstr), void_compare_strings
    );
} 

_proc_inlined sort_cstrings_custom(len64 cstrings_len, mstr cstrings[1], cmp32 (*compare_fun)(cvoidp a, cvoidp b)) {
    qsort(
        cstrings, (u64)  cstrings_len,
        sizeof(mstr), compare_fun
    );
}

// stdlib.h
#endif 
//  ^^^^^^^^^^^^^^^^^^^^ STDLIB ^^^^^^^^^^^^^^^^^^^^

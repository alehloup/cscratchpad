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
typedef u8 bufferbytes; // just for visual:  bufferbytes buffer[64];
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

// Visual Int
typedef u64 hash64;
typedef i32 idx32;
typedef i32 len32;
typedef i64 len64;
typedef i32 cap32;
typedef i64 cap64;

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
typedef void * voidp;
typedef void * const voidpc;
typedef const void * cvoidp;
typedef const void * const ccvoidp;
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

_pure_hot b32 is_not_zero(ccvoidp mem1_, ci64 size) {
    ccu8 mem1 = (cu8 *)mem1_;
    i64 i = 0; 
    for (; i < size; ++i) {
        if (mem1[i]) {
            return True;
        }
    }
    
    return False;
}

_pure_hot b32 is_zero(ccvoidp mem1_, ci64 size) {
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
_pure_hot len64 cstrlen(ccstr cstring) {
    len64 cstring_len;
    for (cstring_len = 0; cstring[cstring_len]; ++cstring_len) {
        /* Empty Body */
    }
    return cstring_len;
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

_pure_hot len32 digitlen(ccstr cstring) {
    len32 cstring_len;
    for (cstring_len = 0; is_digit(cstring[cstring_len]); ++cstring_len) {
        /* Empty Body */
    }
    

    return cstring_len;
}

typedef struct i64num_i32len{i64 num; len32 num_len;}i64num_i32len;
_pure_hot i64num_i32len cstr_to_num(ccstr str) {
    i64 power = 1;
    i64num_i32len ret = {0, digitlen(str)};

    for (i32 i = ret.num_len - 1; i >= 0; --i) {
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

_pure_hot idx32 letter_pos_in_cstring(cchar letter, ccstr cstring) {
    i32 letter_pos = 0;
    for (; cstring[letter_pos]; ++letter_pos) {
        if(cstring[letter_pos] == letter) {
            return letter_pos;
        }
    }
    return -1;
}
//  ^^^^^^^^^^^^^^^^^^^^ STRINGS ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== MATH ====================
*/
// bitmask for optimized Mod for power 2 numbers
_math_hot i64 mod_pwr2(ci64 number, ci64 modval) {
    return (number) & (modval - 1);
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
_pure_hot hash64 hash_str(ccstr str) {
    u64 h = 0x7A5662DCDF;
    for(i64 i = 0; str[i]; ++i) { 
        h ^= str[i] & 255; h *= 1111111111111111111;
    }

    return (h ^ (h >> 32)) >> 1;
}
_pure_hot hash64 hash_bytes(ccvoidp bytes_, ci64 count) {
    ccu8 bytes = (cu8 *)bytes_;
    u64 h = 0x7A5662DCDF;
    for(i64 i = 0; i < count; ++i) { 
        h ^= bytes[i] & 255; h *= 1111111111111111111;
    }

    return (h ^ (h >> 32)) >> 1;
}
//  ^^^^^^^^^^^^^^^^^^^^ HASH ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== HASH TABLE ====================
*/

#define MSI_HT_CAP_    4096 //   2 ^ 12
#define MSI_HT_SHIFT_  52   //  64 - 12
#define MSI_HT_MASK_   4095 //  Cap - 1

// Mask-Step-Index (MSI) lookup
_math_hot i32 ht_lookup(
    cu64 hash, // 1st hash acts as base location
    ci32 index // 2nd "hash" steps over the "list of elements" from base-location
)
{
    u32 step = (u32)(hash >> MSI_HT_SHIFT_) | 1;
    return (i32) (((u32)index + step) & MSI_HT_MASK_);
}
//  ^^^^^^^^^^^^^^^^^^^^ HASH TABLE ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== TEXT ====================
*/

// Alters a text by converting \n to \0 and pushing each line into lines, return number of lines
_fun_hot len32 into_lines(mstr text_to_alter, cap32 lines_cap, mstr lines[64]) {
    len32 lines_len = 0;
    
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

_fun_hot len32 split(mstr text_to_alter, cchar splitter, cap32 words_cap, mstr words[64]) {
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
#ifdef CLOCKS_PER_SEC

_fun f64 seconds_since(clock_t start)
{
    return (f64)(clock() - start) / CLOCKS_PER_SEC;
}
#endif 
#if defined CLOCKS_PER_SEC && defined stdout
_proc print_clock(clock_t start) {
    printf("\n\nExecuted in %f seconds \n", seconds_since(start));
}

#endif
//  ^^^^^^^^^^^^^^^^^^^^ TIME BENCHMARK ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== SHELL ====================
*/
#if defined RAND_MAX && defined stdout && defined va_start

gcc_attr(format(printf, 3, 4), nonnull)
i32 shellrun(cap32 buffer_cap, bufferchar buffer [512], ccstr format, ...) {
    va_list args;
    u8 *buf = zeromem((u8 *) buffer, 512);
    assert(buf == (u8 *)buffer && "zeromem returned different address!");

    va_start(args, format);

    vsprintf_s(buffer, (u64) buffer_cap, format, args);
    return system(buffer);
}

#endif
//  ^^^^^^^^^^^^^^^^^^^^ SHELL ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== FILES ====================
*/
#ifdef stdout

_fun i64 fread_noex(mstr dst, i64 sz, i64 count, FILE * f) {
    #ifdef __cplusplus
        try { return (i64) fread(dst, (u64) sz, (u64) count, f); } catch(...) {return 0;}
    #endif 
              return (i64) fread(dst, (u64) sz, (u64) count, f);
}
_fun i64 fwrite_noex(ccstr Str, i64 Size, i64 Count, FILE * File) {
    #ifdef __cplusplus
        try { return (i64) fwrite(Str, (u64) Size, (u64) Count, File); } catch(...) {return 0;}
    #endif 
              return (i64) fwrite(Str, (u64) Size, (u64) Count, File);
}

_fun len64 file_to_cstring(ccstr filename, cap64 charbuffer_cap, bufferchar charbuffer[64]) {
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

_fun len32 file_to_lines(ccstr filename, cap32 lines_cap, mstr lines[64], cap64 charbuffer_cap, bufferchar charbuffer[64]) {
    len64 charbuffer_len = file_to_cstring(filename, charbuffer_cap, charbuffer);
    discard_ charbuffer_len;
    return into_lines(charbuffer, lines_cap, lines);
}

_proc_hot cstring_to_file(ccstr buffer, ccstr filename) {
        FILE *f = 0; i32 err = 
    fopen_s(&f, filename, "wb");
        assert(!err && "Could not open file for writting");
        {
            len64 buffer_len = cstrlen(buffer);
            i64 bytes_written = fwrite_noex(buffer, 1, buffer_len, f);
            assert(bytes_written == buffer_len && "could not write buffer_len#bytes");
        }
    fclose(f);
}

_proc_hot lines_to_file(len32 lines_len, mstr lines[64], ccstr filename) {
        FILE *f = 0; i32 err = 
    fopen_s(&f, filename, "wb");
        assert(!err && "Could not open file for writting");
        {
            i64 bytes_written = 0;
            len64 line_len = 0;

            for (i32 i = 0; i < lines_len; ++i) {
                ccstr line = lines[i]; 

                line_len = cstrlen(line);
                bytes_written = fwrite_noex(line, 1, line_len, f);
                bytes_written += fwrite_noex("\n", 1, 1, f);
                assert(bytes_written == line_len + 1 && "could not write line_len#bytes");
            }
        }
    fclose(f);
}

#define print_array(vec_to_print_, vec_to_print_len, format_str_) \
    for(int ivec_ = 0; ivec_ < vec_to_print_len; ++ivec_) \
        printf(format_str_, vec_to_print_[ivec_]); \
    printf("\n")

#endif // stdout
//  ^^^^^^^^^^^^^^^^^^^^ FILES ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== STDLIB ====================
*/ 
#ifdef RAND_MAX

_proc sort_cstrings(len64 cstrings_len, mstr cstrings[1]) {
    qsort(
        cstrings, (u64) cstrings_len,
        sizeof(mstr), void_compare_strings
    );
} 

_proc sort_cstrings_custom(len64 cstrings_len, mstr cstrings[1], int (*compare_fun)(cvoidp a, cvoidp b)) {
    qsort(
        cstrings, (u64)  cstrings_len,
        sizeof(mstr), compare_fun
    );
}

#endif // RAND_MAX
//  ^^^^^^^^^^^^^^^^^^^^ STDLIB ^^^^^^^^^^^^^^^^^^^^

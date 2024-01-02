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

#define _fun  _gcc_attr(nonnull, warn_unused_result)
#define _math _gcc_attr(nonnull, warn_unused_result, const) // math = no pointers, same inputs always produce same output
#define _pure _gcc_attr(nonnull, warn_unused_result, pure) // pure = do not uses global variables, uses only its input
#define _fun_inlined  _gcc_attr(nonnull, warn_unused_result, always_inline)
#define _proc _gcc_attr(nonnull) void
#define _proc_inlined _gcc_attr(nonnull, always_inline) void
//  ^^^^^^^^^^^^^^^^^^^^ ATTRIBUTTES ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== TYPES ====================
*/
//
#define MBs_ 1048576

// Int
#define Long long long

#ifdef __SIZEOF_INT128__
__extension__ typedef unsigned __int128 UBig;
__extension__ typedef __int128 Big;
#endif

// String
typedef const char cchar; // const char
typedef cchar * const ccstr; // const string const pointer
typedef cchar * cstr; // const string
typedef char * mstr; // modifiable string
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
#define isizeof(x_element_) ((Long)sizeof(x_element_))
#define arraysizeof(static_array_) (isizeof(static_array_) / isizeof(*static_array_))

//  ^^^^^^^^^^^^^^^^^^^^ MEMORY ^^^^^^^^^^^^^^^^^^^^

/*
    ARRAYS
*/
//
#define array_insert_in_pos(array_len_ref_, array_, element_, pos_) \
    for (int ivec_insert_ = (*array_len_ref_); ivec_insert_ > pos_; --ivec_insert_) \
        array_[ivec_insert_] = array_[ivec_insert_-1]; \
    array_[pos_] = element_; \
    ++(*array_len_ref_)

//  ^^^^^^^^^^^^^^^^^^^^ ARRAYS ^^^^^^^^^^^^^^^^^^^^


/*
    STRINGS
*/
//

_pure Long cstrlen(ccstr cstring) {
    Long cstring_len;
    for (cstring_len = 0; cstring[cstring_len]; ++cstring_len) {
        /* Empty Body */
    }
    return cstring_len;
}
//Only works for COMPILE TIME STRINGS:
#define compile_time_cstrlen(compile_time_string_) arraysizeof(compile_time_string_) - 1

_pure int cstrcmp(ccstr cstr1, ccstr cstr2) {
    Long i = 0;

    for (i = 0; cstr1[i] && cstr2[i] && cstr1[i] == cstr2[i]; ++i) {
        /* Empty Body */
    }
    return cstr1[i] - cstr2[i];
}

_pure int startswith(ccstr string, ccstr prefix) {
    Long i = 0;
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

_fun_inlined int void_compare_strings(const void * a, const void * b) {
    return cstrcmp(*(ccstr*)a, *(ccstr*)b);
}

_pure int is_empty_string(ccstr string) {
    for (Long i = 0; string[i]; ++i) {
        if (string[i] != ' ') {
            return False;
        }
    }
    return True;
}

_math int is_digit(cchar character) {
    return character >= '0' && character <= '9';
}

_pure int digitlen(ccstr cstring) {
    int cstring_len;
    for (cstring_len = 0; is_digit(cstring[cstring_len]); ++cstring_len) {
        /* Empty Body */
    }
    

    return cstring_len;
}

_pure int char_pos_in_str(cchar letter, ccstr cstring) {
    for (int letter_pos = 0; cstring[letter_pos]; ++letter_pos) {
        if(cstring[letter_pos] == letter) {
            return letter_pos;
        }
    }

    return -1;
}
_fun_inlined int char_in_(cchar letter, ccstr cstring) {
    return (char_pos_in_str(letter, cstring) + 1);
}

_pure int char_pos_in_substr(cchar letter, ccstr cstring, int start, int count) {
    for (int letter_pos = start, i = 0; cstring[letter_pos] and i < count; ++letter_pos, ++i) {
        if(cstring[letter_pos] == letter) {
            return letter_pos;
        }
    }

    return -1;
}
_fun_inlined int char_in_substr_(cchar letter, ccstr cstring, int start, int count) {
    return (char_pos_in_substr(letter, cstring, start, count) + 1);
}

// returns the index after the last element
_fun int cstrcpy(mstr dst, ccstr src, Long dst_len) {
    int i = 0;
    for (i = 0; i < dst_len and src[i]; ++i) {
        dst[i] = src[i];
    }
    dst[i++] = 0;
    return i;
}

_fun cstr save_str_to_worldbuffer(cstr string, char worldbuffer[], Long *worldbuffer_len_ref, const Long worldbuffer_cap) {
    Long begin = (*worldbuffer_len_ref);
    
    cstr new_str = &worldbuffer[begin];

    for (int istr = 0; string[istr] and begin < worldbuffer_cap; ++istr, ++begin) {
        worldbuffer[begin] = string[istr];
    }
    worldbuffer[begin++] = 0;

    (*worldbuffer_len_ref) = begin;
    return new_str;
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
_math Long mod_pwr2(Long number, Long modval) {
    return (number) & (modval - 1);
}

_math Long greatest_common_divisor(Long m, Long n) {
    Long tmp;
    while(m) { tmp = m; m = n % m; n = tmp; }       
    return n;
}
_fun_inlined Long least_common_multiple(Long m, Long n) {
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

_pure int rnd(unsigned Long seed[1]) {
    int shift = 29;
    *seed = *seed * Rnd_mult_n + Rnd_sum_n;
    shift -= (int)(*seed >> 61);
    
    return (int)((*seed >> shift) & Rnd_positive_mask);
}
//  ^^^^^^^^^^^^^^^^^^^^ RANDOM ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== HASH ====================
*/
//
#define Hash_shift_mix(cur_hash_)  (cur_hash_ ^ (cur_hash_ >> 31))

#define Hash_start_n 0x7A5662DCDF
#define Hash_mul_n 1111111111111111111 // 11 ones

_pure unsigned Long hash_str(ccstr str) {
    unsigned Long h = Hash_start_n;
    
    for(Long i = 0; str[i]; ++i) { 
        h ^= str[i] & 255; h *= Hash_mul_n;
    }
    h = Hash_shift_mix(h);

    return h >> 1;
}

#define Hash_mul_n1 0x94d049bb133111eb
#define Hash_mul_n2 0xbf58476d1ce4e5b9

_math unsigned Long hash_int(Long integer64) {
    unsigned Long x = (unsigned Long)integer64;
    
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

#define Ht_exp 18
#define HT_CAP    262144 //   2 ^ Ht_exp
#define Ht_mask   (HT_CAP - 1)
#define Ht_shift  (64 - Ht_exp)

// Mask-Step-Index (MSI) lookup. Returns the next index. 
_math int ht_lookup(
    unsigned Long hash, // 1st hash acts as base location
    int index // 2nd "hash" steps over the "list of elements" from base-location
)
{
    unsigned int step = (unsigned int)(hash >> Ht_shift) | 1;
    int idx = (int) (((unsigned int)index + step) & Ht_mask);
    return idx;
}

// Returns +idx if the key was in keys, -idx if was not. If keys_len_ref is passed (is not null) it will insert the key.  
_gcc_attr(always_inline, warn_unused_result) int str_in_ht_(ccstr key, cstr keys[HT_CAP], int *keys_len_ref) {
    unsigned Long h = hash_str(key);
    int i = ht_lookup(h, (int)h);
    int found = False;

    while (i == 0 or (keys[i] and cstrcmp(key, keys[i]))) {
        i = ht_lookup(h, i);
    }

    found = keys[i] ? True : False;
    if (keys_len_ref) {
        keys[i] = key;
        (*keys_len_ref) += not found;
    }

    return found ? i : - i;
}

// Returns +idx if the key was in keys, -idx if was not. If keys_len_ref is passed (is not null) it will insert the key.  
_gcc_attr(always_inline, warn_unused_result) int int_in_ht_(int key, int keys[HT_CAP], int *keys_len_ref) {
    unsigned Long h = hash_int(key);
    int i = ht_lookup(h, (int)h);
    int found = False;

    while (i == 0 or (keys[i] and key != keys[i])) {
        i = ht_lookup(h, i);
    }

    found = keys[i] ? True : False;
    if (keys_len_ref) {
        keys[i] = key;
        (*keys_len_ref) += not found;
    }

    return found ? i : - i;
}

// Returns +idx if the key was in keys, -idx if was not. If keys_len_ref is passed (is not null) it will insert the key.  
_gcc_attr(always_inline, warn_unused_result) int long_in_ht_(Long key, Long keys[HT_CAP], int *keys_len_ref) {
    unsigned Long h = hash_int(key);
    int i = ht_lookup(h, (int)h);
    int found = False;

    while (i == 0 or (keys[i] and key != keys[i])) {
        i = ht_lookup(h, i);
    }

    found = keys[i] ? True : False;
    if (keys_len_ref) {
        keys[i] = key;
        (*keys_len_ref) += not found;
    }

    return found ? i : - i;
}

#ifdef __SIZEOF_INT128__
// Returns +idx if the key was in keys, -idx if was not. If keys_len_ref is passed (is not null) it will insert the key.  
_gcc_attr(always_inline, warn_unused_result) int big_in_ht_(Big key, Big keys[HT_CAP], int *keys_len_ref) {
    unsigned Long h = hash_int((Long)key);
    int i = ht_lookup(h, (int)h);
    int found = False;

    while (i == 0 or (keys[i] and key != keys[i])) {
        i = ht_lookup(h, i);
    }

    found = keys[i] ? True : False;
    if (keys_len_ref) {
        keys[i] = key;
        (*keys_len_ref) += found;
    }

    return found ? i : - i;
}
#endif
//  ^^^^^^^^^^^^^^^^^^^^ HASH TABLE ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== TEXT ====================
*/
//
// Alters a text by converting \n to \0 and pushing each line into lines, return number of lines
_fun int into_lines_(mstr text_to_alter, const int lines_cap, mstr lines[2], int include_empty_lines) {
    int lines_len = 0;
    
    Long current = 0;
    for (Long i = 0; text_to_alter[i]; ++i) {
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
_fun_inlined int into_lines(mstr text_to_alter, const int lines_cap, mstr lines[2]) {
    return into_lines_(text_to_alter, lines_cap, lines, False);
}
_fun_inlined int into_lines_including_empty(mstr text_to_alter, const int lines_cap, mstr lines[2]) {
    return into_lines_(text_to_alter, lines_cap, lines, True);
}

_fun int split(mstr text_to_alter, cchar splitter, const int words_cap, mstr words[2]) {
    int i = 0, current = 0;
    int words_len = 0;

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

_fun_inlined double seconds_since(clock_t start)
{
    return (double)(clock() - start) / CLOCKS_PER_SEC;
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

#define shellrun_buffer_cap 512

_gcc_attr(format(printf, 1, 2), nonnull)
int shellrun(ccstr format, ...) {
    va_list args;
 
    char buffer [shellrun_buffer_cap] = {0};

    va_start(args, format);

    vsprintf(buffer, format, args);
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

_fun_inlined Long fread_noex(mstr dst, Long sz, Long count, FILE * f) {
    #ifdef __cplusplus
        try { return (Long) fread(dst, (unsigned Long) sz, (unsigned Long) count, f); } catch(...) {return 0;}
    #endif 
              return (Long) fread(dst, (unsigned Long) sz, (unsigned Long) count, f);
}
_fun_inlined Long fwrite_noex(ccstr Str, Long Size, Long Count, FILE * File) {
    #ifdef __cplusplus
        try { return (Long) fwrite(Str, (unsigned Long) Size, (unsigned Long) Count, File); } catch(...) {return 0;}
    #endif 
              return (Long) fwrite(Str, (unsigned Long) Size, (unsigned Long) Count, File);
}

_fun Long file_to_cstring(ccstr filename, const Long charbuffer_cap, char charbuffer[2]) {
    Long fsize = 0;

        FILE *f =  
    fopen(filename, "rb");
    
        assert(f && "Could not open file for reading");
    
        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        assert(charbuffer_cap >= fsize+2 && "charbuffer is not enough for file size");

        {
            Long bytesread = fread_noex(charbuffer, 1LL, fsize, f);
            assert(bytesread == fsize && "could not read fsize#bytes"); 
            
            charbuffer[fsize] = charbuffer[fsize-1] != '\n' ? '\n' : '\0';
            charbuffer[fsize+1] = '\0';
        }

    fclose(f);

    return fsize;
}

_fun_inlined int file_to_lines(ccstr filename, const int lines_cap, mstr lines[2], const Long charbuffer_cap, char charbuffer[2]) {
    Long charbuffer_len = file_to_cstring(filename, charbuffer_cap, charbuffer);
    (void) charbuffer_len;
    return into_lines(charbuffer, lines_cap, lines);
}

_proc cstring_to_file(ccstr buffer, ccstr filename) {
        FILE *f =  
    fopen(filename, "wb");
        assert(f && "Could not open file for writting");
        {
            Long buffer_len = cstrlen(buffer);
            Long bytes_written = fwrite_noex(buffer, 1, buffer_len, f);
            assert(bytes_written == buffer_len && "could not write buffer_len#bytes");
        }
    fclose(f);
}

_proc lines_to_file(int lines_len, mstr lines[2], ccstr filename) {
        FILE *f =  
    fopen(filename, "wb");
        assert(f && "Could not open file for writting");
        {
            Long bytes_written = 0;
            Long line_len = 0;

            for (int i = 0; i < lines_len; ++i) {
                ccstr line = lines[i]; 

                line_len = cstrlen(line);
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

#define println(format, ...) printf(format, __VA_ARGS__); printf("\n")

#define array_printf(format_str_, vec_to_print_len, vec_to_print_) \
    for (int ivec_ = 0; ivec_ < vec_to_print_len; ++ivec_) \
        printf(format_str_, vec_to_print_[ivec_]); \
    printf("\n")

#define matrix_printf(format_str_, number_of_lines_, number_of_columns_, matrix_to_print_) \
    printf("Matrix %d x %d :\n", number_of_lines_, number_of_columns_); \
    for (int imatrix_line_ = 0; imatrix_line_ < number_of_lines_; ++imatrix_line_) { \
        for (int imatrix_column_ = 0; imatrix_column_ < number_of_columns_; ++ imatrix_column_) \
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

_proc_inlined sort_cstrings(Long cstrings_len, mstr cstrings[1]) {
    qsort(
        cstrings, (unsigned Long) cstrings_len,
        sizeof(mstr), void_compare_strings
    );
} 

_proc_inlined sort_cstrings_custom(Long cstrings_len, mstr cstrings[1], 
                                   int (*compare_fun)(const void * a, const void * b)) 
{
    qsort(
        cstrings, (unsigned Long)  cstrings_len,
        sizeof(mstr), compare_fun
    );
}

// stdlib.h
#endif 
//  ^^^^^^^^^^^^^^^^^^^^ STDLIB ^^^^^^^^^^^^^^^^^^^^

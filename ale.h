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
#define Char char
#define Bool int
#define Int int
#define Long long long
#ifdef __SIZEOF_INT128__
__extension__ typedef __int128 Big;
#endif
#define Float float
#define Double double

// String
typedef const char * const ccstr; // const string const pointer
typedef const char * cstr; // const string
typedef char * mstr; // modifiable string
//  ^^^^^^^^^^^^^^^^^^^^ TYPES ^^^^^^^^^^^^^^^^^^^^

/*
    Keyword Alternatives
*/
//
#define True (Bool)1
#define False (Bool)0
#ifndef __cplusplus
    #define and &&
    #define or ||
    #define not !

    //stupid C++ does not accept (type) before brace struct
    #define _create(Type) (Type)
#endif
#ifdef __cplusplus
    //stupid C++ does not accept (type) before brace struct
    #define _create(Type)
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
#define _structarray(element_type) { const Long cap; Long len; element_type *data; }
#define _typedef_structarray(type_name, element_type) typedef struct type_name _structarray(element_type) type_name;
#define _array_init(base_static_array, starting_len) \
    { /*cap:*/ arraysizeof(base_static_array) - 1, /*len:*/ starting_len, /*data:*/ base_static_array }
//remove 1 from CAP to make the array "zero" terminated

#define _append(mut_array, new_element) \
    assert((mut_array)->len < (mut_array)->cap && "Array Overflow"); \
    (mut_array)->data[(mut_array)->len++] = new_element;

#define _delidx(mut_array, idx_to_del) \
    assert((mut_array)->len > 0 && "Array Underflow"); \
    (mut_array)->data[idx_to_del] = (mut_array)->data[--(mut_array)->len]

_typedef_structarray(Array_char,   Char);
_typedef_structarray(Array_int,    Int);
_typedef_structarray(Array_long,   Long);
_typedef_structarray(Array_float,  Float);
_typedef_structarray(Array_double, Double);

//  ^^^^^^^^^^^^^^^^^^^^ ARRAYS ^^^^^^^^^^^^^^^^^^^^


/*
    STRINGS
*/
//

typedef Array_char Buffer;
typedef struct String { const long long len; char const * const data; } String;

typedef struct Array_buffer { const long long cap; long long len; Buffer *data; } Array_buffer;
typedef struct Array_string { const long long cap; long long len; String *data; } Array_string;

_pure Long cstrlen(ccstr cstring) {
    Long cstring_len;
    for (cstring_len = 0; cstring[cstring_len] != 0; ++cstring_len) {
        /* Empty Body */
    }
    return cstring_len;
}

_fun_inlined Bool is_empty_cstr(ccstr string) {
    Long i;
    for (i = 0; string[i] > 32; ++i) {
        /* empty body */
    }
    return (Bool) string[i] == 0;
}

#define String(string) _create(String) {cstrlen(string), string}

_pure Int string_compare(const String str1, const String str2) {
    Long i = 0;

    ccstr ccstr1 = str1.data, ccstr2 = str2.data;
    Long len1 = str1.len, len2 = str2.len;

    for (i = 0; i < len1 and i < len2 and ccstr1[i] == ccstr2[i]; ++i) {
        /* Empty Body */
    }
    return (Bool)(ccstr1[i] - ccstr2[i]);
}

_pure Bool string_startswith(const String string, const String prefix) {
    Long i = 0;

    ccstr ccstring = string.data, ccprefix = prefix.data;
    Long lenstring = string.len, lenprefix = prefix.len;

    if (!ccprefix[0]) {
        return True;
    }
    if (!ccstring[0] or lenprefix > lenstring) {
        return False;
    }

    for (i = 0; i < lenprefix and i < lenstring and ccstring[i] == ccprefix[i]; ++i) {
        /* Empty Body */
    }

    if (i == lenprefix) {
        return True;
    } else {
        return False;
    }
}

_fun_inlined Int string_voidcompare(const void * a, const void * b) {
    return stringcmp(*(String*)a, *(String*)b);
}

_fun_inlined Bool string_isempty(const String string) {
    return (Bool) string.len == 0;
}

_math Bool isdigit(const Char character) {
    return character >= '0' && character <= '9';
}

_pure Long string_digitlen(const String string) {
    Long i = 0;
    Long lenstring = string.len;
    ccstr ccstring = string.data;

    for (; i < lenstring and is_digit(ccstring[i]); ++i) {
        /* Empty Body */
    }

    return i;
}

_pure Long char_pos_in_string(const Char letter, const String string) {
    Long lenstring = string.len;
    ccstr ccstring = string.data;

    for (int letter_pos = 0; letter_pos < lenstring; ++letter_pos) {
        if(ccstring[letter_pos] == letter) {
            return letter_pos;
        }
    }

    return -1;
}
_fun_inlined Bool char_in_(const Char letter, const String string) {
    return (Bool) (char_pos_in_string(letter, string) != -1);
}

_pure Long char_pos_in_sub(const Char letter, const String string, const Int start, const Int count) {
    Long lenstring = string.len;
    ccstr ccstring = string.data;

    for (int letter_pos = start, i = 0; i < lenstring and i < count; ++letter_pos, ++i) {
        if(ccstring[letter_pos] == letter) {
            return letter_pos;
        }
    }

    return -1;
}
_fun_inlined Bool char_in_sub_(const Char letter, const String string, int start, int count) {
    return (Bool) (char_pos_in_substr(letter, string, start, count) != -1);
}

_proc string_copy(Buffer *dst, const String src) {
    Long i = 0;
    Long capdst = dst->cap, lensrc = src.len;
    ccstr ccsrc = src.data;
    mstr databuffer = dst->data;

    assert(capdst <= lensrc and "stringcpy: Buffer would overflow!");

    for (i = 0; i < lensrc; ++i) {
        databuffer[i] = ccsrc[i];
    }

    dst->len = lensrc;
}

_proc buffer_append(Buffer *dst, const String src) {
    Long begin = dst->len, lensrc = src.len, capdst = dst->cap;
    ccstr ccsrc = src.data;
    mstr databuffer = dst->data;

    assert(capdst <= lensrc and "append_string_to_buffer: Buffer would overflow!");

    for (Long i = 0; i < lensrc; ++i, ++begin) {
        databuffer[begin] = ccsrc[i];
    }
    dst->len += lensrc;
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

// Next Power of 2 for numbers upto 2*31 (2 147 483 648)
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

// Returns +idx if the key was in keys, -idx if was not. If keys_len_ref is passed it will insert the key, if its null then no insert.  
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
            
            if (include_empty_lines || !is_empty_cstr(&text_to_alter[current])) {
                assert(lines_len < lines_cap && "lines not big enough to store all lines");
                lines[lines_len++] = &text_to_alter[current];
            }
            current = i+1;
        }
    }
    if (include_empty_lines || !is_empty_cstr(&text_to_alter[current])) {
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

_fun int split(mstr text_to_alter, char splitter, const int words_cap, mstr words[2]) {
    int i = 0, current = 0;
    int words_len = 0;

    for (i = 0; text_to_alter[i]; ++i) {
        if (text_to_alter[i] == splitter) {
            text_to_alter[i] = '\0';
            
            if (!is_empty_cstr(&text_to_alter[current])) {
                assert(words_len < words_cap && "words not big enough to store all words");
                words[words_len++] = &text_to_alter[current];
            }
            current = i+1;
        }
    }

    if (current != i && !is_empty_cstr(&text_to_alter[current])) {
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
            Long bytesread = (Long) fread(charbuffer, 1LL, (unsigned Long)fsize, f);
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

_fun_inlined int file_to_lines_including_empty(ccstr filename, const int lines_cap, mstr lines[2], const Long charbuffer_cap, char charbuffer[2]) {
    Long charbuffer_len = file_to_cstring(filename, charbuffer_cap, charbuffer);
    (void) charbuffer_len;
    return into_lines_including_empty(charbuffer, lines_cap, lines);
}

_proc cstring_to_file(ccstr buffer, ccstr filename) {
        FILE *f =  
    fopen(filename, "wb");

        assert(f && "Could not open file for writting");
        {
            Long buffer_len = cstrlen(buffer);
            Long bytes_written = (Long) fwrite(buffer, 1, (unsigned Long)buffer_len, f);
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
                bytes_written = (Long) fwrite(line, 1, (unsigned Long)line_len, f);
                bytes_written += (Long) fwrite("\n", 1, 1, f);
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

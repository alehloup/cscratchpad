#pragma once


/*
    ==================== ASSERT ====================
*/ 
//
#ifdef stdout
    // Print Assert if stdio.h was included
    #define diagnostic_ printf("\n\n  |ASSERT FAILED %s:%s:%d|  \n\n", __FILE__, __func__, __LINE__)
#endif // stdout
#ifndef stdout
    static int assert_trapped_ = 0;
    #define diagnostic_ assert_trapped_ = 1
#endif //not stdout

#if defined(_MSC_VER)
    #define assert(c) if(!(c)) (diagnostic_, __debugbreak())
#else
    #define assert(c) if(!(c)) (diagnostic_, __builtin_trap())
#endif // _MSC_VER
//  ^^^^^^^^^^^^^^^^^^^^ ASSERT ^^^^^^^^^^^^^^^^^^^^


/*
    ==================== ATTRIBUTES ====================
*/
//
#if defined(__GNUC__) || defined(__clang__)
    #define _gcc_attr(...) __attribute((__VA_ARGS__)) inline static
#else
    #define _gcc_attr(...) inline static
#endif // defined(__GNUC__) || defined(__clang__)

#define _fun  _gcc_attr(nonnull, warn_unused_result)
#define _proc _gcc_attr(nonnull) void
//  ^^^^^^^^^^^^^^^^^^^^ ATTRIBUTTES ^^^^^^^^^^^^^^^^^^^^


/*
    ==================== TYPES ====================
*/
//
#define Byte char
#define Char char
#define Bool int
#define Int int
#define Long long long
#ifdef __SIZEOF_INT128__
__extension__ typedef __int128 Big;
#endif // __SIZEOF_INT128__
#define Float float
#define Double double


typedef const char * const Ccstr; // const Cstr const pointer
typedef const char * Cstr; // const Cstr
typedef char * Mstr; // modifiable Cstr
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

    #define _struct(Type) (Type)
    #define ZERO_INIT {0}
#endif // not __cplusplus
#ifdef __cplusplus

    #define _struct(Type) //C++ does not accept compund literal
    #define ZERO_INIT {} //C++ uses empty braces
#endif // __cplusplus

#define ZERO_VAL(Type) _struct(Type) ZERO_INIT
//  ^^^^^^^^^^^^^^^^^^^^ Keyword Alternatives ^^^^^^^^^^^^^^^^^^^^


/*
    MEMORY
*/
//
#define isizeof(x_element_) ((Long)sizeof(x_element_))
#define arraysizeof(static_array_) (isizeof(static_array_) / isizeof(*static_array_))

_fun Int memocmp(void *any_ele1, void *any_ele2, Long sz) {
    Long i = 0;
    char *bytes1 = (char*) any_ele1, *bytes2 = (char*) any_ele2;

    if (sz == 0) {
        return 0;
    }
    --sz;

    for (i = 0; i < sz and bytes1[i] == bytes2[i]; ++i) {
        /* Empty Body */
    }

    return (Int)(bytes1[i] - bytes2[i]);
}

_proc memozero(void *any_element, Long sz) {
    char *bytes = (char*) any_element;

    for (Long i = 0; i < sz; ++i) {
        bytes[i] = 0;
    }
}
//  ^^^^^^^^^^^^^^^^^^^^ MEMORY ^^^^^^^^^^^^^^^^^^^^


/*
    ARRAYS
*/
//
#define _structarray(element_type) { const Long cap; Long len; element_type *elements; }

#define _typedef_structarray(type_name, element_type) typedef struct type_name _structarray(element_type) type_name
_typedef_structarray(Chars,   Char);
_typedef_structarray(Ints,    Int);
_typedef_structarray(Longs,   Long);
_typedef_structarray(Floats,  Float);
_typedef_structarray(Doubles, Double);

#define A(base_static_array) \
    { /*cap:*/ arraysizeof(base_static_array) - 1, /*len:*/ 0, /*elements:*/ base_static_array }
//  remove 1 capacity to make the array "zero" terminated (if zero alocated)

#define _new_array(varname, typename, capacity) \
    typename varname##_base[(capacity)] = ZERO_INIT; \
    typename##s varname = A(varname##_base)
//  creates a local struct array, there must be a base Array of type typename##s (like Char -> Chars)

#define _append(mut_array, new_element) \
    assert((mut_array)->len < (mut_array)->cap && "Array Overflow"); \
    (mut_array)->elements[(mut_array)->len++] = (new_element)

#define _delidx(mut_array, idx_to_del) \
    assert((mut_array)->len > 0 && "Array Underflow"); \
    (mut_array)->elements[idx_to_del] = (mut_array)->elements[--(mut_array)->len]

#define _isempty(container_with_len) ((Bool) container_with_len.len == 0)

#ifdef RAND_MAX
// stdlib.h
//
#define _sort(ptr_array_of_things, sort_function) \
     qsort(ptr_array_of_things, ptr_array_of_things->len, sizeof(ptr_array_of_things->elements[0]), sort_function)

// stdlib.h
#endif // RAND_MAX
//  ^^^^^^^^^^^^^^^^^^^^ ARRAYS ^^^^^^^^^^^^^^^^^^^^


/*
    STRINGS
*/
//
typedef Chars Bytes;
typedef Chars Buffer;
typedef struct String { Long len; Cstr text; } String;

typedef struct Buffers { const Long cap; Long len; Buffer *elements; } Buffers;
typedef struct Strings { const Long cap; Long len; String *elements; } Strings;

_fun Long Cstrlen(Ccstr Cstring) {
    Long Cstring_len;
    for (Cstring_len = 0; Cstring[Cstring_len] != 0; ++Cstring_len) {
        /* Empty Body */
    }
    return Cstring_len;
}

#define S(string) _struct(String) {Cstrlen(string), string}

#ifdef stdout
// stdio.h

_proc string_print(String str) {
    printf("%.*s", (Int) str.len, str.text);
}

#define printn printf("\n")

// stdio.h
#endif //stdout

_fun Int string_compare(const String str1, const String str2) {
    Long i = 0;

    Ccstr ccstr1 = str1.text, ccstr2 = str2.text;
    Long len1 = str1.len, len2 = str2.len;

    for (i = 0; i < len1 and i < len2 and ccstr1[i] == ccstr2[i]; ++i) {
        /* Empty Body */
    }

    return (Int)(ccstr1[i] - ccstr2[i]);
}
_fun Int string_voidcompare(const void * a, const void * b) {
    return string_compare(*(const String*)a, *(const String*)b);
}

_fun Bool startswith(const String string, const String prefix) {
    Long i = 0;

    Ccstr Ccstring = string.text, ccprefix = prefix.text;
    Long lenstring = string.len, lenprefix = prefix.len;

    if (_isempty(prefix)) {
        return True;
    }
    if (_isempty(string) or lenprefix > lenstring) {
        return False;
    }

    for (i = 0; i < lenprefix and i < lenstring and Ccstring[i] == ccprefix[i]; ++i) {
        /* Empty Body */
    }

    if (i == lenprefix) {
        return True;
    } else {
        return False;
    }
}

_fun String trim(String str) {
    Long i = 0;

    for (/* i = 0 */; i < str.len and str.text[i] <= ' '; ++i) {
        --str.len;
    }

    str.text = &str.text[i];

    for (i = str.len - 1; i >= 0 and str.text[i] <= ' '; --i) {
        --str.len;
    }

    return str;
}

_fun Bool is_digit(const Char character) {
    return character >= '0' && character <= '9';
}

_fun Long char_pos(const Char letter, const String string) {
    Long lenstring = string.len;
    Ccstr Ccstring = string.text;

    for (int letter_pos = 0; letter_pos < lenstring; ++letter_pos) {
        if(Ccstring[letter_pos] == letter) {
            return letter_pos;
        }
    }

    return -1;
}
_fun Bool char_in_(const Char letter, const String string) {
    return (Bool) (char_pos(letter, string) != -1);
}

_fun Long char_pos_in_sub(const Char letter, const String string, const Int start, const Int count) {
    Long lenstring = string.len;
    Ccstr Ccstring = string.text;

    for (int letter_pos = start, i = 0; i < lenstring and i < count; ++letter_pos, ++i) {
        if(Ccstring[letter_pos] == letter) {
            return letter_pos;
        }
    }

    return -1;
}
_fun Bool char_in_sub_(const Char letter, const String string, int start, int count) {
    return (Bool) (char_pos_in_sub(letter, string, start, count) != -1);
}

// Returns the lines in a String
_proc to_lines_base(Strings *dest_lines, String src_text, Bool include_empty_lines) {
    Bool not_empty = False;
    Long current = 0;
    
    Ccstr text = src_text.text;
    for (Long i = 0; i < src_text.len; ++i) {
        not_empty = not_empty or text[i] > 32;

        if (text[i] == '\n') {
            
            if (include_empty_lines || not_empty) {
                String line = {(i - current), (&text[current])};
                _append(dest_lines, line);
            }

            not_empty = False;
            current = i+1;
        }
    }
    if (include_empty_lines || not_empty) {
        String line = {(src_text.len - current), (&text[current])};
        _append(dest_lines, line);
    }
}

// Returns the non-empty lines in a String
_proc to_lines(Strings *dest_lines, String src_text) {
    to_lines_base(dest_lines, src_text, False);
}

// Returns the lines in a String, including empty ones
_proc to_lines_including_empty(Strings *dest_lines, String src_text) {
    to_lines_base(dest_lines, src_text, True);
}

// Returns the words of a line, the line is divided by the splitter
_proc split(Strings *dest_words, String src_text, char splitter) {
    Bool not_empty = False;
    int i = 0, current = 0;
    
    Ccstr text = src_text.text;

    for (i = 0; i < src_text.len; ++i) {
        not_empty = not_empty or text[i] > 32;

        if (text[i] == splitter) {
            if (not_empty) {
                String word = {(i - current), (&text[current])};
                _append(dest_words, word);
            }

            not_empty = False;
            current = i+1;
        }
    }

    if (not_empty) {
        String word = {(src_text.len - current), (&text[current])};
        _append(dest_words, word);
    }
}

_proc buffer_set(Buffer *dst, const String src) {
    Long i = 0;
    Long capdst = dst->cap, lensrc = src.len, lendst = dst->len;
    Ccstr ccsrc = src.text;
    Mstr databuffer = dst->elements;

    assert(lensrc <= capdst and "stringcpy: Buffer would overflow!");

    for (Long x = 0; x < lendst; ++x) {
        databuffer[x] = 0;
    }

    for (i = 0; i < lensrc; ++i) {
        databuffer[i] = ccsrc[i];
    }
    databuffer[i] = 0;

    dst->len = lensrc;
}

_proc buffer_append(Buffer *dst, const String src) {
    Long begin = dst->len, lensrc = src.len, capdst = dst->cap;
    Ccstr ccsrc = src.text;
    Mstr databuffer = dst->elements;

    assert(lensrc <= capdst and "append_string_to_buffer: Buffer would overflow!");

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
_fun Long mod_pwr2(Long number, Long modval) {
    return (number) & (modval - 1);
}

_fun Long greatest_common_divisor(Long m, Long n) {
    Long tmp;
    while(m) { tmp = m; m = n % m; n = tmp; }       
    return n;
}
_fun Long least_common_multiple(Long m, Long n) {
     return m / greatest_common_divisor(m, n) * n;
}

// Next Power of 2 for numbers upto 2*31 (2_147_483_648)
#define Next_power2(n_) \
    ((((n_)-1) | (((n_)-1) >> 1) | (((n_)-1) >> 2) | (((n_)-1) >> 4) | (((n_)-1) >> 8) | (((n_)-1) >> 16))+1)

#define Rnd_positive_mask 2147483647
#define Rnd_mult_n 0x9b60933458e17d7dULL
#define Rnd_sum_n 0xd737232eeccdf7edULL

_fun int rnd(unsigned Long seed[1]) {
    int shift = 29;
    *seed = *seed * Rnd_mult_n + Rnd_sum_n;
    shift -= (int)(*seed >> 61);
    
    return (int)((*seed >> shift) & Rnd_positive_mask);
}

#define Hash_shift_mix(cur_hash_)  (cur_hash_ ^ (cur_hash_ >> 31))

#define Hash_start_n 0x7A5662DCDF
#define Hash_mul_n 1111111111111111111 // 11 ones

_fun unsigned Long string_hash(String text) {
    Ccstr str = text.text;
    Long str_len = text.len;

    unsigned Long h = Hash_start_n;
    
    for(Long i = 0; i < str_len; ++i) { 
        h ^= str[i] & 255; h *= Hash_mul_n;
    }
    h = Hash_shift_mix(h);

    return h >> 1;
}

#define Hash_mul_n1 0x94d049bb133111eb
#define Hash_mul_n2 0xbf58476d1ce4e5b9

_fun unsigned Long long_hash(Long integer64) {
    unsigned Long x = (unsigned Long)integer64;
    
    x *= Hash_mul_n1; 
    x = Hash_shift_mix(x);
    x *= Hash_mul_n2; 
    x = Hash_shift_mix(x);
    
    return x >> 1;
}

// Mask-Step-Index (MSI) lookup. Returns the next index. 
_fun Int ht_lookup(
    unsigned Long hash, // 1st hash acts as base location
    Int index, // 2nd "hash" steps over the "list of elements" from base-location
    Int exp // power-2 exp used as the Hash Table capacity
) {
    unsigned Int step = (unsigned Int)(hash >> (64 - exp)) | 1;
    Int idx = (Int) (((unsigned Int)index + step) & ((unsigned Int) ((1 << exp) - 1)));
    return idx;
}
//  ^^^^^^^^^^^^^^^^^^^^ MATH ^^^^^^^^^^^^^^^^^^^^


/*
    ==================== HASH TABLE ====================
*/
//
#define _structentry(key_type, val_type) { key_type key; val_type value; }

#define _typedef_structentry(type_name, key_type, val_type) typedef struct type_name _structentry(key_type, val_type) type_name
_typedef_structentry(Long_Long,   Long, Long);
_typedef_structentry(String_Long, String, Long);

//A Hash Table is just an Array of entries that map Type to a Long, defining them for Long and String:
_typedef_structarray(Long_Longs,     Long_Long);
_typedef_structarray(String_Longs,   String_Long);

_fun Int ht_get_exp(Long_Longs haystack) {
    Long true_cap = haystack.cap + 1;
    switch (true_cap) {
        case (1 << 10): return 10; case (1 << 11): return 11; case (1 << 12): return 12;
        case (1 << 13): return 13; case (1 << 14): return 14; case (1 << 15): return 15;
        case (1 << 16): return 16; case (1 << 17): return 17; case (1 << 18): return 18;
        case (1 << 19): return 19; case (1 << 20): return 20; case (1 << 21): return 21;
        default: assert(true_cap == 52 and "Not a power-2 in range 10 <= exp <= 21"); return 0;
    }
}

_fun Int ht_long_long_pos_of(Long search_key, Long_Longs haystack) {
    Long_Long *elements = haystack.elements;

    Int exp = ht_get_exp(haystack);    
    unsigned Long h = long_hash(search_key);
    Int pos = ht_lookup(h, (Int) h, exp);

    while (elements[pos].key != 0 and (pos == 0 or search_key != elements[pos].key)) {
        pos = ht_lookup(h, pos, exp);
    }

    return pos;
}
//  ^^^^^^^^^^^^^^^^^^^^ HASH TABLE ^^^^^^^^^^^^^^^^^^^^


/*
    ==================== FILES ====================
*/
//
#ifdef stdout
// stdio.h

_proc file_to_buffer(String filename, Buffer *dest_buffer) {
    Long fsize = 0;

        FILE *f =  
    fopen(filename.text, "rb");
    
        assert(f && "Could not open file for reading");
    
        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        assert(dest_buffer->cap >= fsize+2 && "buffer is not enough for file size");

        {
            Long bytesread = (Long) fread(dest_buffer->elements, 1LL, (unsigned Long)fsize, f);
            assert(bytesread == fsize && "could not read fsize#bytes"); 
            
            dest_buffer->elements[fsize] = '\0';
            dest_buffer->len = fsize;
        }

    fclose(f);
}

_fun String file_to_string(String filename, Buffer *dest_buffer) {
    String src_text;
    
    file_to_buffer(filename, dest_buffer);
    src_text.len = dest_buffer->len; 
    src_text.text = dest_buffer->elements;
    
    return src_text;
}

_proc file_to_lines(String filename, Strings *dest_lines, Buffer *dest_buffer) {
    to_lines(dest_lines, file_to_string(filename, dest_buffer));
}

_proc file_to_lines_including_empty(String filename, Strings *dest_lines, Buffer *dest_buffer) {
    to_lines_including_empty(dest_lines, file_to_string(filename, dest_buffer));
}

_proc string_to_file(String filename, String src_text) {
        FILE *f =  
    fopen(filename.text, "wb");

        assert(f && "Could not open file for writting");
        {
            Long bytes_written = (Long) fwrite(src_text.text, 1, (unsigned Long)src_text.len, f);
            assert(bytes_written == src_text.len && "could not write buffer_len#bytes");
        }

    fclose(f);
}

_proc lines_to_file(String filename, Strings lines) {
        FILE *f =  
    fopen(filename.text, "wb");

        assert(f && "Could not open file for writting");
        {
            for (int i = 0; i < lines.len; ++i) {
                String line = lines.elements[i];
                Long bytes_written = (Long) fwrite(line.text, 1, (unsigned Long)line.len, f);
                bytes_written += (Long) fwrite("\n", 1, 1, f);
                assert(bytes_written == line.len + 1 && "could not write line_len#bytes");
            }
        }

    fclose(f);
}

// stdio.h
#endif
//  ^^^^^^^^^^^^^^^^^^^^ FILES ^^^^^^^^^^^^^^^^^^^^


/*
    ==================== TIME BENCHMARK ====================
*/
//
#ifdef CLOCKS_PER_SEC 
// time.h

_fun double seconds_since(clock_t start)
{
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

// time.h
#endif  

#if defined CLOCKS_PER_SEC && defined stdout 
// time.h && stdio.h

_proc print_clock(clock_t start) {
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
Int shellrun(Ccstr format, ...) {
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

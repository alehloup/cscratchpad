#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#endif

#define assert_(c) if(!(c)) printf("\n\n  |ASSERT FAILED %s:%s:%d %s|\n\n", __FILE__, __func__, __LINE__, #c)

#if defined(__GNUC__) || defined(__clang__)
    #define _gcc_attr(...) __attribute((__VA_ARGS__)) inline static
#else
    #define _gcc_attr(...) inline static
#endif

#define _fun  _gcc_attr(nonnull, warn_unused_result)
#define _proc _gcc_attr(nonnull) void

#ifdef __SIZEOF_INT128__
__extension__ typedef __int128 int128_t;
__extension__ typedef unsigned __int128 uint128_t;
#endif // __SIZEOF_INT128__

#ifndef __cplusplus 
    #define and &&
    #define or ||
    #define not !

    #define _struct(Type) (Type)
    #define ZERO_INIT {0}

    #ifdef __SIZEOF_INT128__
        #define _static_n(N) static N
    #endif
    #ifndef __SIZEOF_INT128__
        #define _static_n(N) /*static N*/
    #endif
#endif
#ifdef __cplusplus

    #define _struct(Type)
    #define ZERO_INIT {}
    #define _static_n(N) N
#endif // __cplusplus

/*
    ARRAYS
*/
#define isizeof(x_element_) ((int64_t)sizeof(x_element_))
#define arraysizeof(static_array_) (isizeof(static_array_) / isizeof(*static_array_))

// _new_cap_array_len(arrayname, typename, capacity)
#define _new_cap_array_len(arrayname, typename, capacity) \
    typename arrayname[(capacity)+1] = ZERO_INIT; \
    const int64_t arrayname##_cap = arraysizeof(arrayname##_base) - 1; \
    int64_t  arrayname##_len_val_ = 0; \
    int64_t* arrayname##_len = &arrayname##_len_val_

#define array_param_(arrayname, typename) \
    const int64_t arrayname##_cap, typename arrayname[_static_n(arrayname##_cap)], int64_t* arrayname##_len

#define array_arg_(arrayname) arrayname##_cap, arrayname, arrayname##_len

#define _append(array, new_element) \
    assert_((*array##_len) < array##_cap && "Array Overflow"); \
    array[(*array##_len)++] = (new_element)

#define _delidx(array, idx_to_del) \
    assert_((*array##_len) > 0 && "Array Underflow"); \
    array[idx_to_del] = array[--(*array##_len)]

#define _isempty(container_with_len) ((bool)((*array##_len) == 0))

/*
    STRING SLICES
*/
// struct strslice_t { int64_t len; const char* text; }
struct strslice_t { int64_t len; const char* text; }; // struct strslice_t { int64_t len; const char* text; }
#define _to_strslice(string) _struct(strslice_t) {strlen(string), string}

_proc strslice_print(struct strslice_t str) {
    printf("%.*s", (int) str.len, str.text);
}
#define printn printf("\n")

_fun int32_t strslice_cmp(const struct strslice_t a_text_slice, const struct strslice_t b_text_slice) {
    const char* const a_text = a_text_slice.text;
    const int64_t a_text_len = a_text_slice.len;

    const char* const b_text = b_text_slice.text;
    const int64_t b_text_len = b_text_slice.len;

    if (a_text_len != b_text_len) {
        return (int32_t) (a_text_len - b_text_len);
    }

    for (int64_t i = 0; i < a_text_len; ++i) {
        if (a_text[i] != b_text[i]) {
            return (int32_t) (a_text[i] - b_text[i]);
        }
    }

    return 0;
}
_fun bool startswith(const struct strslice_t prefix_slice, const struct strslice_t text_slice) {
    if (prefix_slice.len > text_slice.len) {
        return false;
    }
    struct strslice_t only_begin = {prefix_slice.len, text_slice.text};

    return strslice_cmp(prefix_slice, only_begin) == 0;
}

_fun struct strslice_t trimmed(const struct strslice_t text_slice) {
    const char* const text = text_slice.text;
    int64_t text_len = text_slice.len;

    int64_t start = 0;
    for (start = 0; start < text_len and text[start] <= ' '; ++start) {
        --text_len;
    }

    for (int64_t last = text_len - 1; last >= 0 and text[last] <= ' '; --last) {
        --text_len;
    }

    struct strslice_t trimmed_slice = {text_len, &text[start]};
    return trimmed_slice;
}

_fun bool is_digit(const char character) {
    return character >= '0' && character <= '9';
}

_fun int64_t char_pos(const char letter, const struct strslice_t text_slice) {
    const char* const text = text_slice.text;
    const int64_t text_len = text_slice.len;

    for (int64_t letter_pos = 0; letter_pos < text_len; ++letter_pos) {
        if(text[letter_pos] == letter) {
            return letter_pos;
        }
    }

    return -1;
}

_proc split(const struct strslice_t text_slice, const char splitter, array_param_(parts, struct strslice_t)) {
    const char* const text = text_slice.text;
    const int64_t text_len = text_slice.len;

    int64_t current = 0;
    for (int64_t i = 0; i < text_len; ++i) {
        if (text[i] == splitter) {            
            struct strslice_t part = {(i - current), (&text[current])};
            _append(parts, part);

            current = i+1;
        }
    }
    if (current < text_len) {
        struct strslice_t part = {(text_len - current), (&text[current])};
        _append(parts, part);
    }

    struct strslice_t part = ZERO_INIT;
    parts[(*parts_len)] = part;
}
_proc to_lines(const struct strslice_t text_slice, array_param_(lines, struct strslice_t)) {
    split(text_slice, '\n', lines_cap, lines, lines_len);
}

_proc buffer_append(array_param_(dst_buffer, char), const struct strslice_t src_chars_slice) {
    const char* const src_chars = src_chars_slice.text;
    const int64_t src_chars_len = src_chars_slice.len;

    assert_(src_chars_len <= dst_buffer_cap and "buffer_append: Buffer would overflow!");

    for (int64_t i = 0; i < src_chars_len; ++i) {
        dst_buffer[(*dst_buffer_len)++] = src_chars[i]; 
    }
    dst_buffer[(*dst_buffer_len)] = 0;
}
_proc buffer_set(array_param_(dst_buffer, char), const struct strslice_t src_chars_slice) {
    *dst_buffer_len = 0;
    buffer_append(dst_buffer_cap, dst_buffer, dst_buffer_len, src_chars_slice);
}

/*
    ==================== MATH ====================
*/
// bitmask for optimized Mod for power 2 numbers
_fun int64_t mod_pwr2(int64_t number, int64_t modval) {
    return (number) & (modval - 1);
}

_fun int64_t greatest_common_divisor(int64_t m, int64_t n) {
    int64_t tmp;
    while(m) { tmp = m; m = n % m; n = tmp; }       
    return n;
}
_fun int64_t least_common_multiple(int64_t m, int64_t n) {
     return m / greatest_common_divisor(m, n) * n;
}

#define Rnd_positive_mask 2147483647
#define Rnd_mult_n 0x9b60933458e17d7dULL
#define Rnd_sum_n 0xd737232eeccdf7edULL

_fun int32_t rnd(uint64_t seed[_static_n(1)]) {
    int32_t shift = 29;
    *seed = *seed * Rnd_mult_n + Rnd_sum_n;
    shift -= (int)(*seed >> 61);
    
    return (int)((*seed >> shift) & Rnd_positive_mask);
}

#define Hash_shift_mix(cur_hash_)  (cur_hash_ ^ (cur_hash_ >> 31))

#define Hash_start_n 0x7A5662DCDF
#define Hash_mul_n 1111111111111111111 // 11 ones

_fun uint64_t cstring_hash(const char* const cstring) {
    uint64_t h = Hash_start_n;
    
    for(int64_t i = 0; cstring[i]; ++i) { 
        h ^= cstring[i] & 255; h *= Hash_mul_n;
    }
    h = Hash_shift_mix(h);

    return h >> 1;
}

_fun uint64_t strslice_hash(const struct strslice_t chars_slice) {
    const char* const chars = chars_slice.text;
    int64_t chars_len = chars_slice.len;

    uint64_t h = Hash_start_n;
    
    for(int64_t i = 0; i < chars_len; ++i) { 
        h ^= chars[i] & 255; h *= Hash_mul_n;
    }
    h = Hash_shift_mix(h);

    return h >> 1;
}

#define Hash_mul_n1 0x94d049bb133111eb
#define Hash_mul_n2 0xbf58476d1ce4e5b9

_fun uint64_t long_hash(int64_t integer64) {
    uint64_t x = (uint64_t)integer64;
    
    x *= Hash_mul_n1; 
    x = Hash_shift_mix(x);
    x *= Hash_mul_n2; 
    x = Hash_shift_mix(x);
    
    return x >> 1;
}

/*
    ==================== HASH TABLE ====================
*/
//
_fun int32_t array_cap_to_exp(int64_t cap) {
    switch (cap) {
        case (1 << 10): return 10; case (1 << 11): return 11; case (1 << 12): return 12;
        case (1 << 13): return 13; case (1 << 14): return 14; case (1 << 15): return 15;
        case (1 << 16): return 16; case (1 << 17): return 17; case (1 << 18): return 18;
        case (1 << 19): return 19; case (1 << 20): return 20; case (1 << 21): return 21;
        default: assert_( cap == (10 & 21) and "Not a power-2 in range 10 <= exp <= 21"); return 0;
    }
}

// Mask-Step-Index (MSI) lookup. Returns the next index. 
_fun int32_t ht_lookup(
    uint64_t hash, // 1st hash acts as base location
    int32_t index, // 2nd "hash" steps over the "list of elements" from base-location
    int32_t exp // power-2 exp used as the Hash Table capacity
) {
    uint32_t step = (uint32_t)(hash >> (64 - exp)) | 1;
    int32_t idx = (int) (((uint32_t)index + step) & ((uint32_t) ((1 << exp) - 1)));
    return idx;
}

_fun int32_t i64_msi_lookup(const int64_t search_key, array_param_(haystack_keys, int64_t)) {
    (void) haystack_keys_len;
    const int32_t exp = array_cap_to_exp(haystack_keys_cap);    
    uint64_t h = long_hash(search_key);
    int32_t pos = ht_lookup(h, (int) h, exp);

    assert_(search_key != 0 && "Zero int64_t Key not supported");

    while (haystack_keys[pos] != 0 and search_key != haystack_keys[pos]) {
        pos = ht_lookup(h, pos, exp);
    }

    return pos;
}

_fun int32_t i64_msi_upsert(const int64_t search_key, array_param_(haystack_keys, int64_t)) {
    assert_((*haystack_keys_len) < haystack_keys_cap && "msi keys overflow, can't insert");
    
    int32_t pos = i64_msi_lookup(search_key, haystack_keys_cap, haystack_keys, haystack_keys_len);
    (*haystack_keys_len) += (haystack_keys[pos] == 0) ? 1 : 0;
    haystack_keys[pos] = search_key;
    return pos;
}

_fun int32_t strslice_msi_lookup(const struct strslice_t search_key, array_param_(haystack_keys, struct strslice_t)) {
    (void) haystack_keys_len;

    int32_t exp = array_cap_to_exp(haystack_keys_cap);
    uint64_t h = strslice_hash(search_key);
    int32_t pos = ht_lookup(h, (int) h, exp);

    assert_(search_key.len != 0 && "Empty String Key not supported");

    while (haystack_keys[pos].len != 0 and strslice_cmp(search_key, haystack_keys[pos]) != 0) {
        pos = ht_lookup(h, pos, exp);
    }

    return pos;
}

_fun int32_t strings_msi_upsert(const struct strslice_t search_key, array_param_(haystack_keys, struct strslice_t)) {
    assert_((*haystack_keys_len) < haystack_keys_cap && "msi keys overflow, can't insert");
    int32_t pos = strslice_msi_lookup(search_key, haystack_keys_cap, haystack_keys, haystack_keys_len);
    (*haystack_keys_len) += (haystack_keys[pos].len == 0) ? 1 : 0;
    haystack_keys[pos] = search_key;
    return pos;
}

/*
    ==================== FILES ====================
*/
//
_fun int64_t file_size(FILE* f) {
    fseek(f, 0, SEEK_END);
    int64_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    return fsize;
}

_proc file_to_buffer(const char *const filename, array_param_(dst_buffer, char)) {
        FILE *f =  
    fopen(filename, "rb");
    {
        assert_(f && "Could not open file for reading");
    
        int64_t fsize = file_size(f);

        assert_(dst_buffer_cap >= fsize+2 && "buffer is not enough for file size");
        
        int64_t bytesread = (int64_t) fread(dst_buffer, 1LL, (uint64_t)fsize, f);
        assert_(bytesread == fsize && "could not read fsize#bytes"); 
        
        dst_buffer[fsize] = '\0';
        *dst_buffer_len = fsize;
    }
    fclose(f);
}

_proc file_to_lines(const char *const filename, array_param_(buffer, char), array_param_(lines, struct strslice_t)) {
    file_to_buffer(filename, buffer_cap, buffer, buffer_len);
    struct strslice_t chars_slice = {(*buffer_len), buffer};
    to_lines(chars_slice, lines_cap, lines, lines_len);
}

_proc buffer_to_file(array_param_(buffer, char), const char *const filename) {
        FILE *f =  
    fopen(filename, "wb");
    {
        assert_(f && "Could not open file for writting");
     
        int64_t bytes_written = (int64_t) fwrite(buffer, 1, (uint64_t)(*buffer_len), f);
        assert_(bytes_written == (*buffer_len) && "could not write buffer_len#bytes");
    }
    fclose(f);
}

_proc lines_to_file(array_param_(lines, struct strslice_t), const char *const filename) {
        FILE *f =  
    fopen(filename, "wb");
    {
        assert_(f && "Could not open file for writting");
        
            for (int64_t i = 0; i < (*lines_len); ++i) {
                int64_t bytes_written = (int64_t) fwrite(lines[i].text, 1, (uint64_t)lines[i].len, f);
                bytes_written += (int64_t) fwrite("\n", 1, 1, f);
                assert_(bytes_written == lines[i].len + 1 && "could not write line_len#bytes");
            }
    }
    fclose(f);
}


/*
    ==================== SYSTEM APIs ====================
*/

// struct mmap_file_t { void *file; void *map; const char *const filename; const int64_t filesize; char *contents; }
struct mmap_file_t { void *file; void *map; const char *const filename; const int64_t filesize; char *contents; };

#if defined(_WINDOWS_)
_fun struct mmap_file_t mmap_open(const char *const filename) {
    HANDLE hFile = CreateFile(filename,
        GENERIC_READ,                          // dwDesiredAccess
        0,                                     // dwShareMode
        NULL,                                  // lpSecurityAttributes
        OPEN_EXISTING,                         // dwCreationDisposition
        FILE_ATTRIBUTE_NORMAL,                 // dwFlagsAndAttributes
        0);                                    // hTemplateFile

    assert_(hFile != INVALID_HANDLE_VALUE);
    
    LARGE_INTEGER liFileSize = ZERO_INIT;
    assert_(GetFileSizeEx(hFile, &liFileSize) && "File Size exception");
    
    assert_(liFileSize.QuadPart && "File Empty!");

    HANDLE hMap = CreateFileMapping(hFile, 0, PAGE_READWRITE, 0, 0, 0);

    assert_(hMap && "Failed to create file mapping");

    void* lpBasePtr = MapViewOfFile(hMap, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
    assert_(lpBasePtr && "MapView failed");

    struct mmap_file_t mmap_info = {hFile, hMap, filename, (int64_t)liFileSize.QuadPart, (char*)lpBasePtr};
    
    return mmap_info;    
}

_proc mmap_close(struct mmap_file_t mmap_info) {
    UnmapViewOfFile((void*)mmap_info.contents);
    CloseHandle(mmap_info.map);
    CloseHandle(mmap_info.file);
}

typedef HANDLE thread_t;
typedef long unsigned int return_code_t;

#define _thread_fun _fun return_code_t

_fun thread_t go(return_code_t (*routine)(void* arg), const int8_t *const thread_id) {
    thread_t thread = CreateThread(0, 0, routine, (void*)thread_id, 0, 0);
    assert_(thread != 0 && "Error creating thread");

    return thread;
}

_proc go_threads(return_code_t (*routine)(void* arg), int8_t times, thread_t threads[_static_n(times)]) {
    static const int8_t thread_ids[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 
        17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    };

    for (int8_t i = 0; i < times; ++i) {
        threads[i] = go(routine, &thread_ids[i]);
    }
}

_proc join_threads(array_param_(threads, thread_t)) {
    WaitForMultipleObjects((long unsigned int) (*threads_len), threads, TRUE, INFINITE);
}
#endif //_WINDOWS_

#define shellrun_buffer_cap 512
_gcc_attr(format(printf, 1, 2), nonnull)
int32_t shellrun(const char* const format, ...) {
    va_list args;
 
    char buffer [shellrun_buffer_cap] = {0};

    va_start(args, format);

    vsprintf(buffer, format, args);
    printf("\n");
    vprintf(format, args);
    printf("\n");
    return system(buffer);
}

// Creates the clock variable |start|, starting it
#define _START_WATCH clock_t start = clock()
// Prints the current clock time, using the variable |start|
#define _STOP_WATCH printf("\n\nExecuted in %f seconds \n", (double)(clock() - start) / CLOCKS_PER_SEC)

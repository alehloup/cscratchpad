#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#endif

// #if defined(POSIX?)
//     #include <linux.h>
// #endif

#define assert_(c) if(!(c)) printf("\n\n  |ASSERT FAILED %s:%s:%d %s|\n\n", __FILE__, __func__, __LINE__, #c)

#if defined(__GNUC__) || defined(__clang__)
    #define fun_   __attribute((nonnull, warn_unused_result)) inline static
    #define proc_  __attribute((nonnull)) inline static void
#else
    #define fun_   inline static
    #define proc_  inline static void
#endif

#if defined(_WINDOWS_)
    #define routine_ inline static long unsigned int
    #define THREAD_ HANDLE
#else 
    #define routine_ inline static void * 
    #define THREAD_ HANDLE
#endif 

#define arraysizeof(static_array_) ((int64_t)(sizeof(static_array_) / sizeof(*static_array_)))

#define APPEND_(array, new_element) \
    assert_((*array##_len) < array##_cap); \
    array[(*array##_len)++] = (new_element)

#define DELIDX_(array, idx_to_del) \
    assert_((*array##_len) > 0); \
    array[idx_to_del] = array[--(*array##_len)]

struct mmap_file_t { void *file; void *map; const char *const filename; const int64_t filesize; char *contents; };

struct sslice_t { int64_t len; const char *text; };

fun_ struct sslice_t cstring_to_sslice(const char *const cstring) {
    struct sslice_t sslice = {(int64_t) strlen(cstring), cstring};
    return sslice;
}

fun_ int32_t sslice_cmp(const struct sslice_t a_text_slice, const struct sslice_t b_text_slice) {
    if (a_text_slice.len != b_text_slice.len) {
        return (int32_t) (a_text_slice.len - b_text_slice.len);
    }

    for (int64_t i = 0; i < a_text_slice.len; ++i) {
        if (a_text_slice.text[i] != b_text_slice.text[i]) {
            return (int32_t) (a_text_slice.text[i] - b_text_slice.text[i]);
        }
    }
    return 0;
}
fun_ bool startswith(const struct sslice_t prefix_slice, const struct sslice_t text_slice) {
    struct sslice_t only_begin = {prefix_slice.len, text_slice.text};

    return sslice_cmp(prefix_slice, only_begin) == 0;
}

fun_ struct sslice_t trimmed(const struct sslice_t text_slice) {
    const char *const text = text_slice.text;
    int64_t text_len = text_slice.len;

    int64_t start = 0;
    for (start = 0; start < text_len && text[start] <= ' '; ++start) {
        --text_len;
    }

    for (int64_t last = text_len - 1; last >= 0 && text[last] <= ' '; --last) {
        --text_len;
    }

    struct sslice_t trimmed_slice = {text_len, &text[start]};
    return trimmed_slice;
}

fun_ int64_t char_pos(const char letter, const struct sslice_t text_slice) {
    const char *const text = text_slice.text;
    const int64_t text_len = text_slice.len;

    for (int64_t letter_pos = 0; letter_pos < text_len; ++letter_pos) {
        if(text[letter_pos] == letter) {
            return letter_pos;
        }
    }

    return -1;
}

proc_ split(const struct sslice_t text_slice, const char splitter, int64_t parts_cap, struct sslice_t parts[], int64_t *parts_len) {
    const char *const text = text_slice.text;
    const int64_t text_len = text_slice.len;

    int64_t current = 0;
    for (int64_t i = 0; i < text_len; ++i) {
        if (text[i] == splitter) {            
            struct sslice_t part = {(i - current), (&text[current])};
            APPEND_(parts, part);

            current = i+1;
        }
    }
    if (current < text_len) {
        struct sslice_t part = {(text_len - current), (&text[current])};
        APPEND_(parts, part);
    }
}
proc_ to_lines(const struct sslice_t text_slice, int64_t lines_cap, struct sslice_t lines[], int64_t *lines_len) {
    split(text_slice, '\n', lines_cap, lines, lines_len);
}

proc_ buffer_append(int64_t dst_buffer_cap, char dst_buffer[], int64_t *dst_buffer_len, const struct sslice_t src_chars_slice) {
    const char *const src_chars = src_chars_slice.text;
    const int64_t src_chars_len = src_chars_slice.len;

    assert_(src_chars_len <= dst_buffer_cap);

    for (int64_t i = 0; i < src_chars_len; ++i) {
        dst_buffer[(*dst_buffer_len)++] = src_chars[i]; 
    }
    dst_buffer[(*dst_buffer_len)] = 0;
}
proc_ buffer_set(int64_t dst_buffer_cap, char dst_buffer[], int64_t *dst_buffer_len, const struct sslice_t src_chars_slice) {
    *dst_buffer_len = 0;
    buffer_append(dst_buffer_cap, dst_buffer, dst_buffer_len, src_chars_slice);
}


// bitmask for optimized Mod for power 2 numbers
fun_ int64_t mod_pwr2(int64_t number, int64_t modval) {
    return (number) & (modval - 1);
}

fun_ int64_t greatest_common_divisor(int64_t m, int64_t n) {
    int64_t tmp;
    while(m) { tmp = m; m = n % m; n = tmp; }       
    return n;
}
fun_ int64_t least_common_multiple(int64_t m, int64_t n) {
     return m / greatest_common_divisor(m, n) * n;
}

fun_ int32_t rnd(uint64_t seed[1]) {
    *seed = *seed * 0x9b60933458e17d7dULL + 0xd737232eeccdf7edULL;
    int32_t shift = 29 - (int32_t)(*seed >> 61);
    
    return (int32_t)((*seed >> shift) & 2147483647);
}

fun_ uint64_t cstring_hash(const char *const cstring) {
    uint64_t h = 0x7A5662DCDF;
    
    for(int64_t i = 0; cstring[i]; ++i) { 
        h ^= cstring[i] & 255; h *= 1111111111111111111;
    }
    return (h ^ (h >> 31)) >> 1;
}

fun_ uint64_t sslice_hash(const struct sslice_t chars_slice) {
    const char *const chars = chars_slice.text;
    int64_t chars_len = chars_slice.len;

    uint64_t h = 0x7A5662DCDF;
    
    for(int64_t i = 0; i < chars_len; ++i) { 
        h ^= chars[i] & 255; h *= 1111111111111111111;
    }
    return (h ^ (h >> 31)) >> 1;
}

fun_ uint64_t long_hash(int64_t integer64) {
    uint64_t x = (uint64_t)integer64;
    
    x *= 0x94d049bb133111eb; 
    x = (x ^ (x >> 31));
    x *= 0xbf58476d1ce4e5b9; 
    
    return (x ^ (x >> 31)) >> 1;
}


fun_ int32_t array_cap_to_exp(int64_t cap) {
    switch (cap) {
        case (1 << 10): return 10; case (1 << 11): return 11; case (1 << 12): return 12;
        case (1 << 13): return 13; case (1 << 14): return 14; case (1 << 15): return 15;
        case (1 << 16): return 16; case (1 << 17): return 17; case (1 << 18): return 18;
        case (1 << 19): return 19; case (1 << 20): return 20; case (1 << 21): return 21;
        default: assert_( cap == (10 & 21) ); return 0;
    }
}

// Mask-Step-Index (MSI) lookup. Returns the next index. 
fun_ int32_t ht_lookup(
    uint64_t hash, // 1st hash acts as base location
    int32_t index, // 2nd "hash" steps over the "list of elements" from base-location
    int32_t exp // power-2 exp used as the Hash Table capacity
) {
    uint32_t step = (uint32_t)(hash >> (64 - exp)) | 1;
    int32_t idx = (int) (((uint32_t)index + step) & ((uint32_t) ((1 << exp) - 1)));
    return idx;
}

fun_ int32_t i64_msi_lookup(const int64_t search_key, int64_t haystack_keys_cap, int64_t haystack_keys[]) {
    const int32_t exp = array_cap_to_exp(haystack_keys_cap);    
    uint64_t h = long_hash(search_key);
    int32_t pos = ht_lookup(h, (int) h, exp);

    assert_(search_key != 0);

    while (haystack_keys[pos] != 0 && search_key != haystack_keys[pos]) {
        pos = ht_lookup(h, pos, exp);
    }

    return pos;
}

fun_ int32_t i64_msi_upsert(
        const int64_t search_key, 
        int64_t haystack_keys_cap, int64_t haystack_keys[], int64_t *haystack_keys_len) 
{
    assert_((*haystack_keys_len) < haystack_keys_cap);
    
    int32_t pos = i64_msi_lookup(search_key, haystack_keys_cap, haystack_keys);
    (*haystack_keys_len) += (haystack_keys[pos] == 0) ? 1 : 0;
    haystack_keys[pos] = search_key;
    return pos;
}

fun_ int32_t sslice_msi_lookup(
        const struct sslice_t search_key, 
        int64_t haystack_keys_cap, struct sslice_t haystack_keys[]) 
{
    int32_t exp = array_cap_to_exp(haystack_keys_cap);
    uint64_t h = sslice_hash(search_key);
    int32_t pos = ht_lookup(h, (int) h, exp);

    assert_(search_key.len != 0);

    while (haystack_keys[pos].len != 0 && sslice_cmp(search_key, haystack_keys[pos]) != 0) {
        pos = ht_lookup(h, pos, exp);
    }

    return pos;
}

fun_ int32_t sslice_msi_upsert(
        const struct sslice_t search_key, 
        int64_t haystack_keys_cap, struct sslice_t haystack_keys[], int64_t *haystack_keys_len) 
{
    assert_((*haystack_keys_len) < haystack_keys_cap);
    int32_t pos = sslice_msi_lookup(search_key, haystack_keys_cap, haystack_keys);
    (*haystack_keys_len) += (haystack_keys[pos].len == 0) ? 1 : 0;
    haystack_keys[pos] = search_key;
    return pos;
}

fun_ int64_t file_size(FILE *f) {
    fseek(f, 0, SEEK_END);
    int64_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    return fsize;
}

proc_ file_to_buffer(
    const char *const filename, 
    int64_t dst_buffer_cap, char dst_buffer[], int64_t *dst_buffer_len) 
{
    FILE *f = fopen(filename, "rb");
    {
        assert_(f);
    
        int64_t fsize = file_size(f);

        assert_(dst_buffer_cap >= fsize+2);
        
        int64_t bytesread = (int64_t) fread(dst_buffer, 1LL, (uint64_t)fsize, f);
        assert_(bytesread == fsize); 
        
        dst_buffer[fsize] = '\0';
        *dst_buffer_len = fsize;
    }
    fclose(f);
}

proc_ file_to_lines(
    const char *const filename, 
    int64_t buffer_cap, char buffer[], int64_t *buffer_len, 
    int64_t lines_cap, struct sslice_t lines[], int64_t *lines_len) 
{
    file_to_buffer(filename, buffer_cap, buffer, buffer_len);
    struct sslice_t chars_slice = {(*buffer_len), buffer};
    to_lines(chars_slice, lines_cap, lines, lines_len);
}

proc_ buffer_to_file(int64_t buffer_cap, char buffer[], int64_t *buffer_len, const char *const filename) {  
    FILE *f = fopen(filename, "wb");
    {
        assert_(f);
        assert_(*buffer_len < buffer_cap);
     
        int64_t bytes_written = (int64_t) fwrite(buffer, 1, (uint64_t)(*buffer_len), f);
        assert_(bytes_written == (*buffer_len));
    }
    fclose(f);
}

proc_ lines_to_file(int64_t lines_cap, struct sslice_t lines[], int64_t *lines_len, const char *const filename) {
    FILE *f = fopen(filename, "wb");
    {
        assert_(f);
        assert_(*lines_len < lines_cap);
        
        for (int64_t i = 0; i < (*lines_len); ++i) {
            int64_t bytes_written = (int64_t) fwrite(lines[i].text, 1, (uint64_t)lines[i].len, f);
            bytes_written += (int64_t) fwrite("\n", 1, 1, f);
            assert_(bytes_written == lines[i].len + 1);
        }
    }
    fclose(f);
}

proc_ sslice_to_file(struct sslice_t text_slice, const char *const filename) {
    FILE *f = fopen(filename, "wb");
    {
        assert_(f);
        
        int64_t bytes_written = (int64_t) fwrite(text_slice.text, 1, (uint64_t)text_slice.len, f);
        bytes_written += (int64_t) fwrite("\n", 1, 1, f);
        assert_(bytes_written == text_slice.len + 1);
    }
    fclose(f);
}


#if defined(_WINDOWS_)
fun_ struct mmap_file_t mmap_open(const char *const filename) {
    HANDLE hFile = CreateFile(filename,
        GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    assert_(hFile != INVALID_HANDLE_VALUE);
    
    LARGE_INTEGER liFileSize;
    assert_(GetFileSizeEx(hFile, &liFileSize));
    
    assert_(liFileSize.QuadPart);

    HANDLE hMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);

    assert_(hMap);

    void *lpBasePtr = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
    assert_(lpBasePtr);

    struct mmap_file_t mmap_info = {hFile, hMap, filename, (int64_t)liFileSize.QuadPart, (char*)lpBasePtr};
    
    return mmap_info;
}

proc_ mmap_close(struct mmap_file_t mmap_info) {
    UnmapViewOfFile((void*)mmap_info.contents);
    CloseHandle(mmap_info.map);
    CloseHandle(mmap_info.file);
}

fun_ THREAD_ go(long unsigned int (*routine)(void *thread_idx)) {
    static uintptr_t incrementing_idx = 0;

    THREAD_ thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)routine, (void *)(incrementing_idx++), 0, 0);
    assert_(thread != 0);

    return thread;
}
proc_ go_threads(
    long unsigned int (*routine)(void *thread_idx), int32_t number_of_threads_to_spawn, 
    int64_t threads_cap, THREAD_ threads[], int64_t *threads_len)
{
        assert_(*threads_len < threads_cap && number_of_threads_to_spawn < threads_cap);
        assert_(number_of_threads_to_spawn < 16000);

        for (int32_t i = 0; i < number_of_threads_to_spawn; ++i) {
            threads[i] = go(routine);
        }
        *threads_len = number_of_threads_to_spawn;
}

proc_ join_threads(int64_t threads_cap, THREAD_ threads[], int64_t *threads_len) {
    assert_(*threads_len < threads_cap);
    
    WaitForMultipleObjects((long unsigned int) (*threads_len), threads, TRUE, INFINITE);
}
#endif //_WINDOWS_

fun_ int32_t compile_c(const char *const flags, const char *const c_file) {
    const size_t flags_len = strlen(flags);
    const size_t c_file_len = strlen(c_file);
    assert_(flags_len < 1024 && c_file_len < 512);

    char buffer [2048] = {0}; size_t idx = 0; 
    
    memcpy(buffer, flags, flags_len); idx += flags_len;

    memcpy(&buffer[idx], " ./", 3); idx += 3;
    memcpy(&buffer[idx], c_file, c_file_len); idx += c_file_len;

    memcpy(&buffer[idx], " -o ", 4); idx += 4;

    memcpy(&buffer[idx], "./", 2); idx += 2;
    memcpy(&buffer[idx], c_file, c_file_len - 2); idx += (c_file_len - 2);
    memcpy(&buffer[idx], ".exe\0", 5); idx += 5;

    printf("\n%.*s\n", 2000, buffer);
    return system(buffer);
}

static clock_t BENCHCLOCK_ = 0;
proc_ start_benchclock(void) {
    BENCHCLOCK_ = clock(); 
} 
proc_ stop_benchclock(void) {
    printf("\n\nExecuted in %f seconds \n", (double)(clock() - BENCHCLOCK_) / CLOCKS_PER_SEC);
}

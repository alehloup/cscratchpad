#pragma once

#include <memory.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#endif

// #if defined(POSIX?)
//     #include <linux.h>
// #endif

#define assert_(c) if(!(c)) \
    (fprintf(stderr, "\n\n  ASSERT FAILED %s:%s:%d | %s |\n\n", __FILE__, __func__, __LINE__, #c), exit(52))

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

#if defined(__cplusplus)
    #define STRUCT_(struct_name, ...) /*(struct struct_name)*/ { __VA_ARGS__ }
#else
    #define STRUCT_(struct_name, ...) (struct struct_name) { __VA_ARGS__ }
#endif 

#define ARRCAP_(static_array_) ((int64_t)(sizeof(static_array_) / sizeof(*static_array_)))

#define APPEND_(array, new_element) \
    assert_((*array##_len) < array##_cap); \
    array[(*array##_len)++] = new_element

#define DELIDX_(array, idx_to_del) \
    assert_((*array##_len) > 0); \
    array[idx_to_del] = array[--(*array##_len)]

struct mmap_file_t { void *file; void *map; const char *const filename; const int64_t filesize; char *contents; };

struct sslice_t { int64_t len; const char *text; };

fun_ struct sslice_t cstring_to_sslice(const char *const cstring) {
    return STRUCT_(sslice_t, (int64_t)strlen(cstring), cstring);
}

fun_ int32_t sslice_cmp(const struct sslice_t a_text_slice, const struct sslice_t b_text_slice) {
    if (a_text_slice.len == 0 || b_text_slice.len == 0 || a_text_slice.len != b_text_slice.len)
        return (int32_t) (a_text_slice.len - b_text_slice.len);
    return memcmp(a_text_slice.text, b_text_slice.text, (size_t)a_text_slice.len);
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

    return STRUCT_(sslice_t, text_len, &text[start]);
}

fun_ int64_t char_pos(const char letter, const char *const cstring) {
    const char *ptr = strchr(cstring, letter);
    if (ptr != NULL) {
        return (int64_t)(ptr - cstring);
    } else {
        return -1;
    }
}

fun_ int64_t char_pos_slice(const char letter, const struct sslice_t text_slice) {
    if (text_slice.len < 1) {
        return -1;
    }

    const char *const ptr = (const char *) memchr((void const *)text_slice.text, letter, (size_t)text_slice.len);
    if (ptr != NULL) {
        return (int64_t)(ptr - text_slice.text);
    } else {
        return -1;
    }
}

proc_ split(const struct sslice_t text_slice, const char splitter, 
    const int64_t parts_cap, struct sslice_t parts[], int64_t *parts_len) 
{
    struct sslice_t cur = text_slice;
    int64_t pos = 0;
    
    for (pos = char_pos_slice(splitter, cur); pos != -1; pos = char_pos_slice(splitter, cur)) {
        APPEND_(parts, STRUCT_(sslice_t, pos, &cur.text[pos]));
        ++pos;
        cur.len -= pos; 
        cur.text = &cur.text[pos];
    }

    if (cur.len > 0) {
        APPEND_(parts, STRUCT_(sslice_t, pos, cur.text));
    }
}

proc_ to_lines(const struct sslice_t text_slice, int64_t lines_cap, struct sslice_t lines[], int64_t *lines_len) {
    split(text_slice, '\n', lines_cap, lines, lines_len);
}

proc_ buffer_appendslice(const int64_t dst_buffer_cap, char dst_buffer[], int64_t *dst_buffer_len, 
    const struct sslice_t src_chars_slice) 
{
    assert_(src_chars_slice.len <= dst_buffer_cap);

    memcpy(dst_buffer, src_chars_slice.text, (size_t)src_chars_slice.len);

    *dst_buffer_len += src_chars_slice.len;
    dst_buffer[(*dst_buffer_len)] = 0;
}
proc_ buffer_appendcstr(const int64_t dst_buffer_cap, char dst_buffer[], int64_t *dst_buffer_len, 
const char *const cstr) 
{
    const int64_t cstr_len = (int64_t)strlen(cstr);

    assert_(cstr_len <= dst_buffer_cap);

    memcpy(&dst_buffer[*dst_buffer_len], cstr, (size_t)cstr_len);

    *dst_buffer_len += cstr_len;
    dst_buffer[(*dst_buffer_len)] = 0;
}
proc_ buffer_appendcstrs(const int64_t dst_buffer_cap, char dst_buffer[], int64_t *dst_buffer_len, 
    const char *const cstrs[], const int64_t cstrs_len) 
{ 
    for (int64_t i = 0; i < cstrs_len; ++i) {
        buffer_appendcstr(dst_buffer_cap, dst_buffer, dst_buffer_len, cstrs[i]);
    }
}


// bitmask for optimized Mod for power 2 numbers
fun_ int64_t mod_pwr2(int64_t number, int64_t modval) {
    return (number) & (modval - 1);
}

fun_ int64_t greatest_common_divisor(int64_t m, int64_t n) {
    int64_t tmp;
    while (m) { tmp = m; m = n % m; n = tmp; }       
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
    
    to_lines(STRUCT_(sslice_t, *buffer_len, buffer), lines_cap, lines, lines_len);
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

fun_ int32_t compile_c(const char *const flags, const char *const c_file_c) {
    char buffer[2048] = {0}; 
    int64_t buffer_len = 0; 

    char c_file[256] = {0};
    const int32_t c_file_len = (int32_t)strlen(c_file_c) - 2;
    assert_(c_file_len < 255);

    memcpy(c_file, c_file_c, (size_t)c_file_len); // remove .c

    const char *const parts[] = {
        flags, " ./", c_file, ".c -o ./", c_file, ".exe"
    };
    buffer_appendcstrs(ARRCAP_(buffer), buffer, &buffer_len, parts, ARRCAP_(parts));

    printf("\n%.*s\n", (int32_t)buffer_len, buffer);
    return system(buffer);
}

static clock_t BENCHCLOCK_ = 0;
proc_ start_benchclock(void) {
    BENCHCLOCK_ = clock(); 
} 
proc_ stop_benchclock(void) {
    printf("\n\nExecuted in %f seconds \n", (double)(clock() - BENCHCLOCK_) / CLOCKS_PER_SEC);
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

    return STRUCT_(mmap_file_t, hFile, hMap, filename, (int64_t)liFileSize.QuadPart, (char*)lpBasePtr);
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

proc_ join_threads(THREAD_ threads[], const int64_t threads_len) {
    assert_(threads_len < 16000);
    
    WaitForMultipleObjects((long unsigned int) threads_len, threads, TRUE, INFINITE);
}
#endif //_WINDOWS_

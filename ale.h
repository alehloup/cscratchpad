#pragma once

#pragma region Includes
#include <memory.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#else
    #include <pthread.h>
    #include <sys/mman.h>
#endif
#pragma endregion Includes

#pragma region Defines
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
    #define ROURET_T long unsigned int
    #define THREAD_T HANDLE
#else 
    #define ROURET_T void *
    #define THREAD_T pthread_t
#endif
#define routine_ inline static ROURET_T
#define THREAD_STACK_SIZE_ 64 * 1024
#pragma endregion Defines

#pragma region Macros
#if defined(__cplusplus)
    #define STRUCT_(struct_name, ...) /*(struct struct_name)*/ { __VA_ARGS__ }
#else
    #define STRUCT_(struct_name, ...) (struct struct_name) { __VA_ARGS__ }

    #define not !
    #define and &&
    #define or ||
#endif 

#define ARRCAP_(static_array_) ((int64_t)(sizeof(static_array_) / sizeof(*static_array_)))

#define APPEND_(array, new_element) \
    assert_((*array##_len) < array##_cap); \
    array[(*array##_len)++] = new_element

#define DELIDX_(array, idx_to_del) \
    assert_((*array##_len) > 0); \
    array[idx_to_del] = array[--(*array##_len)]
#pragma endregion Macros

#define NEWCMPF_(fun_name, type, ...) \
    fun_ int32_t fun_name(const void *avoid, const void *bvoid) { \
        const type a = *((const type *)avoid); \
        const type b = *((const type *)bvoid); \
        __VA_ARGS__; \
    }

#pragma region Structs
// void *file; void *map; const char *const filename; const int64_t filesize; char *contents;
struct mmap_file_t { void *file; void *map; const char *const filename; const int64_t filesize; char *contents; };

//int64_t len; const char *text;
struct sslice_t { int64_t len; const char *text; };
#pragma endregion Structs

#pragma region Strings
fun_ struct sslice_t to_sslice(const char *const cstring) { return STRUCT_(sslice_t, (int64_t)strlen(cstring), cstring); }

proc_ sslice_print(const struct sslice_t slice) { printf("%.*s\n", (int32_t)slice.len, slice.text); }
proc_ sslice_printend(const struct sslice_t slice, const char *const end) { printf("%.*s%s", (int32_t)slice.len, slice.text, end); }

fun_ int32_t sslice_cmp(const struct sslice_t a_text_slice, const struct sslice_t b_text_slice) {
    int64_t min_len = a_text_slice.len <= b_text_slice.len ? a_text_slice.len : b_text_slice.len;
    for (int32_t i = 0; i < min_len; ++i) {
        if (a_text_slice.text[i] != b_text_slice.text[i]) {
            return a_text_slice.text[i] - b_text_slice.text[i];
        }
    }
    return (int32_t)(a_text_slice.len - b_text_slice.len);
}
fun_ bool startswith(const struct sslice_t prefix, const struct sslice_t text) {
    return sslice_cmp(prefix, STRUCT_(sslice_t, .len=prefix.len, .text=text.text)) == 0;
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
    if (ptr != 0) {
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
    if (ptr != 0) {
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
        APPEND_(parts, STRUCT_(sslice_t, .len=pos, .text=cur.text));
        ++pos;
        cur.len -= pos; 
        cur.text = &cur.text[pos];
    }

    if (cur.len > 0) {
        APPEND_(parts, STRUCT_(sslice_t, .len=pos, .text=cur.text));
    }
}

proc_ to_lines(const struct sslice_t text_slice, const int64_t lines_cap, struct sslice_t lines[], int64_t *lines_len) {
    split(text_slice, '\n', lines_cap, lines, lines_len);
}

proc_ buffer_to_lines(
    char buffer[], const int64_t buffer_len, 
    const int64_t lines_cap, struct sslice_t lines[], int64_t *lines_len) 
{   
    to_lines(STRUCT_(sslice_t, .len=buffer_len, .text=buffer), lines_cap, lines, lines_len);
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
#pragma endregion Strings

#pragma region Math
fun_ int64_t power2_number_mod(int64_t power2_number, int64_t modval) { return (power2_number) & (modval - 1); }

fun_ int64_t greatest_common_divisor(int64_t m, int64_t n) {
    int64_t tmp = 0;
    while (m) { tmp = m; m = n % m; n = tmp; }       
    return n;
}
fun_ int64_t least_common_multiple(int64_t m, int64_t n) { return m / greatest_common_divisor(m, n) * n; }

fun_ float sqrtapproximate_newton(float n) {
    float x = 1;
    while (fabsf(x * x - n) > 1e-8f)
        x = (x + n / x) / 2;
    return x;
}
fun_ float inversesqrtapproximate_newton(float number) {
    union FloatIntUnion { float f; uint32_t i; };

    float y = 0;

    union FloatIntUnion u; u.f = number;
    u.i = 0x5f3759df - (u.i >> 1);
    
    y = u.f;
    y = y * (1.5f - ((number * 0.5f) * y * y));

    return y;
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


fun_ int32_t array_cap_to_exp(const int64_t cap) {
    switch (cap) {
        case(1 << 10):case(1 << 10)+1:return 10; case(1 << 11):case (1 << 11)+1:return 11; 
        case(1 << 12):case(1 << 12)+1:return 12; case(1 << 13):case(1 << 13)+1:return 13; 
        case(1 << 14):case(1 << 14)+1:return 14; case (1 << 15):case (1 << 15)+1:return 15;
        case(1 << 16):case(1 << 16)+1:return 16; case(1 << 17):case(1 << 17)+1:return 17; 
        case(1 << 18):case(1 << 18)+1:return 18; case(1 << 19):case(1 << 19)+1:return 19; 
        case(1 << 20):case(1 << 20)+1:return 20; case(1 << 21):case(1 << 21)+1:return 21;
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
#pragma endregion Math

#pragma region Hashtable
fun_ int32_t i64_msi_lookup(const int64_t search_key, const int64_t haystack_keys_cap, int64_t haystack_keys[]) {
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
        const int64_t haystack_keys_cap, int64_t haystack_keys[], int64_t *haystack_keys_len) 
{
    assert_((*haystack_keys_len) < haystack_keys_cap);
    
    int32_t pos = i64_msi_lookup(search_key, haystack_keys_cap, haystack_keys);
    (*haystack_keys_len) += (haystack_keys[pos] == 0) ? 1 : 0;
    haystack_keys[pos] = search_key;
    return pos;
}

proc_ i64_msi_print(const int64_t haystack_keys_cap, int64_t haystack_keys[], const int64_t haystack_keys_len) {
    printf("#%" PRId64 "/%" PRId64 "\n", haystack_keys_len, haystack_keys_cap);
    for (int64_t i = 0; i < haystack_keys_cap; ++i) {
        if (not haystack_keys[0]) continue;
        printf("%" PRId64 ", ", haystack_keys[i]);
    }
    printf("\n");
}

fun_ int32_t sslice_msi_lookup(
        const struct sslice_t search_key, 
        const int64_t haystack_keys_cap, struct sslice_t haystack_keys[]) 
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
        const int64_t haystack_keys_cap, struct sslice_t haystack_keys[], int64_t *haystack_keys_len) 
{
    assert_((*haystack_keys_len) < haystack_keys_cap);
    int32_t pos = sslice_msi_lookup(search_key, haystack_keys_cap, haystack_keys);
    (*haystack_keys_len) += (haystack_keys[pos].len == 0) ? 1 : 0;
    haystack_keys[pos] = search_key;
    return pos;
}

proc_ sslice_msi_print(const int64_t haystack_keys_cap, struct sslice_t haystack_keys[], const int64_t haystack_keys_len) {
    printf("#%" PRId64 "/%" PRId64 "\n", haystack_keys_len, haystack_keys_cap);
    for (int64_t i = 0; i < haystack_keys_cap; ++i) {
        if (haystack_keys[i].len == 0) 
            continue;
        sslice_printend(haystack_keys[i], ", ");
    }
    printf("\n");
}
#pragma endregion Hashtable

#pragma region Files
fun_ int64_t file_size(FILE *f) {
    fseek(f, 0, SEEK_END);
    int64_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    return fsize;
}

proc_ file_to_buffer(
    const char *const filename, 
    const int64_t dst_buffer_cap, char dst_buffer[], int64_t *dst_buffer_len) 
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
    const int64_t buffer_cap, char buffer[], int64_t *buffer_len, 
    const int64_t lines_cap, struct sslice_t lines[], int64_t *lines_len) 
{
    file_to_buffer(filename, buffer_cap, buffer, buffer_len);
    
    to_lines(STRUCT_(sslice_t, .len=*buffer_len, .text=buffer), lines_cap, lines, lines_len);
}

proc_ buffer_to_file(const int64_t buffer_cap, char buffer[], int64_t *buffer_len, const char *const filename) {  
    FILE *f = fopen(filename, "wb");
    {
        assert_(f);
        assert_(*buffer_len < buffer_cap);
     
        int64_t bytes_written = (int64_t) fwrite(buffer, 1, (uint64_t)(*buffer_len), f);
        assert_(bytes_written == (*buffer_len));
    }
    fclose(f);
}

proc_ lines_to_file(const int64_t lines_cap, struct sslice_t lines[], int64_t *lines_len, const char *const filename) {
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
#pragma endregion Files

#pragma region Tools
fun_ int32_t compile_run_c(const char *const flags, const char *const c_file_c) {
    char buffer[2048] = {0}; 
    int64_t buffer_len = 0; 

    char c_file[256] = {0};
    const int32_t c_file_len = (int32_t)strlen(c_file_c) - 2;
    assert_(c_file_len < 255 && c_file_len > 0);

    memcpy(c_file, c_file_c, (size_t)c_file_len); // remove .c

    const char *const parts[] = {
        flags, // pass the compiler and flags
        c_file, ".c -o ", c_file, ".exe && ",  // compile .c to .exe
        c_file, ".exe" // execute
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
#pragma endregion Tools

#pragma region Mmap
#if defined(_WINDOWS_) // if _WINDOWS_ else Unix
fun_ int64_t handle_to_filesize(HANDLE hFile) {
    DWORD dwFileSizeHigh = 0; 
    DWORD dwFileSizeLow = GetFileSize(hFile, &dwFileSizeHigh);

    assert_(dwFileSizeLow != INVALID_FILE_SIZE || GetLastError() == NO_ERROR);

    return ((int64_t)dwFileSizeHigh << 32) | dwFileSizeLow;
}
fun_ struct mmap_file_t mmap_open(const char *const filename) {
    HANDLE hFile = CreateFile(filename,
        GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    assert_(hFile != INVALID_HANDLE_VALUE);
    
    int64_t fileSize = handle_to_filesize(hFile);
    assert_(fileSize > 0);

    HANDLE hMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
    assert_(hMap);

    void *lpBasePtr = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
    assert_(lpBasePtr);

    return STRUCT_(mmap_file_t, .file=hFile, .map=hMap, .filename=filename, .filesize=fileSize, .contents=(char*)lpBasePtr);
}
proc_ mmap_close(struct mmap_file_t mmap_info) {
    UnmapViewOfFile((void*)mmap_info.contents);
    CloseHandle(mmap_info.map);
    CloseHandle(mmap_info.file);
}
#else // Unix
fun_ struct mmap_file_t mmap_open(const char *const filename) {
    FILE *hFile = fopen(filename, "r");
    assert_(hFile != 0);
    
    int64_t fileSize = file_size(hFile);
    assert_(fileSize > 0);

    char *mapped = mmap(0, fileSize, PROT_READ, MAP_SHARED, fileno(hFile), 0);
    assert_(mapped);

    return STRUCT_(mmap_file_t, .file=hFile, .map=mapped, .filename=filename, .filesize=fileSize, .contents=mapped);
}
proc_ mmap_close(struct mmap_file_t mmap_info) {
    munmap(mmap_info.map, mmap_info.filesize);
}
#endif // endif _WINDOWS_ else Unix
#pragma endregion Mmap

#pragma region Threads
#if defined(_WINDOWS_) // if _WINDOWS_ else Unix
fun_ THREAD_T go(ROURET_T (*routine)(void *thread_idx), uintptr_t thread_id) {
    THREAD_T thread = CreateThread(0, THREAD_STACK_SIZE_, (LPTHREAD_START_ROUTINE)routine, (void *)(thread_id), 0, 0);
    assert_(thread != 0);

    return thread;
}
proc_ join_thread(THREAD_T thread) {
    WaitForSingleObject(thread, INFINITE);
}
#else // Unix
fun_ THREAD_T go(ROURET_T (*routine)(void *thread_idx), uintptr_t thread_id) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE_);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    THREAD_T thread;
    int32_t err = pthread_create(&thread, &attr, routine, (void*)(thread_id));
    assert_(err == 0);

    return thread;
}
proc_ join_thread(THREAD_T thread) {
    pthread_join(thread, 0);
}
#endif // endif _WINDOWS_ else Unix

proc_ go_threads(
    ROURET_T (*routine)(void *thread_idx), int32_t number_of_threads_to_spawn, 
    const int64_t threads_cap, THREAD_T threads[], int64_t *threads_len)
{
        assert_(*threads_len <= threads_cap && number_of_threads_to_spawn <= threads_cap);
        assert_(number_of_threads_to_spawn < 16000);

        int32_t i = 0;
        for (i = 0; i < number_of_threads_to_spawn; ++i) {
            threads[i] = go(routine, (uintptr_t)i);
        }
        *threads_len = i;
}
proc_ join_threads(THREAD_T threads[], const int64_t threads_len) {
    assert_(threads_len <= 16384);
    
    for (int32_t i = 0; i < threads_len; ++i) {
        join_thread(threads[i]);
    }
}
#pragma endregion Threads

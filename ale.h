#pragma once

#pragma region Includes
#include <assert.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <locale.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
    #include <io.h>
#else // assume Unix:
    #include <unistd.h>
    #include <pthread.h>
    #include <sys/mman.h>
#endif
#pragma endregion Includes

#pragma region Defines
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
    #define ZERO_INIT_ {}
#else
    #define STRUCT_(struct_name, ...) (struct struct_name) { __VA_ARGS__ }
    #define ZERO_INIT_ {0}

    #define not !
    #define and &&
    #define or ||
#endif 

#define CAP_(static_array_) (\
    assert(sizeof(static_array_) != sizeof(&static_array_)), \
    (int64_t)(sizeof(static_array_) / sizeof(*(static_array_))) \
)

#define APPEND_(array, new_element) \
    assert((*array##_len) < array##_cap); \
    array[(*array##_len)++] = new_element

#define DELIDX_(array, idx_to_del) \
    assert((*array##_len) > 0); \
    array[idx_to_del] = array[--(*array##_len)]

#define NEWCMPF_(fun_name, type, ...) \
    fun_ int32_t fun_name(const void *avoid, const void *bvoid) { \
        const type a = *((const type *)avoid); \
        const type b = *((const type *)bvoid); \
        __VA_ARGS__; \
    }
#pragma endregion Macros

#pragma region Structs
// void *file; void *map; const char *const filename; const int64_t filesize; char *contents;
struct mmap_t { void *file; void *map; const char *const filename; const int64_t filesize; char *contents; };

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

proc_ set_locale_to_utf8(void) {
    setlocale(LC_CTYPE, "en_US.UTF-8");
}
// You need to have set a locale, default locale is C locale. Best to call set_locale_to_utf8
fun_ int32_t sslice_cmp_locale(const struct sslice_t a_text_slice, const struct sslice_t b_text_slice) {
    char a[512], b[512];
    assert(a_text_slice.len < 512 && b_text_slice.len < 512);

    memcpy(a, a_text_slice.text, (size_t)a_text_slice.len);
    memcpy(b, b_text_slice.text, (size_t)b_text_slice.len);
    a[a_text_slice.len] = 0;
    b[b_text_slice.len] = 0;

    return strcoll(a, b);
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

fun_ struct sslice_t subss(struct sslice_t text_slice, const int64_t start, const int64_t end) {
    int64_t true_start = start < 0 ? text_slice.len + start : start;
    int64_t true_end = end < 0 ? text_slice.len + end : end;

    true_end = true_end > text_slice.len ? text_slice.len : true_end;

    int64_t len = true_end - true_start;
    len = len < 0 ? 0 : len;

    if (len == 0) {
        true_start = 0;
    }

    return STRUCT_(sslice_t, len, &text_slice.text[true_start]);
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
    assert(src_chars_slice.len <= dst_buffer_cap);

    memcpy(&dst_buffer[*dst_buffer_len], src_chars_slice.text, (size_t)src_chars_slice.len);

    *dst_buffer_len += src_chars_slice.len;
    dst_buffer[(*dst_buffer_len)] = 0;
}
proc_ buffer_appendcstr(const int64_t dst_buffer_cap, char dst_buffer[], int64_t *dst_buffer_len, 
const char *const cstr) 
{
    const int64_t cstr_len = (int64_t)strlen(cstr);

    assert(cstr_len <= dst_buffer_cap);

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

fun_ int64_t rnd(uint64_t seed[1]) {
    *seed = *seed * 0x9b60933458e17d7dULL + 0xd737232eeccdf7edULL;
    int32_t shift = 29 - (int32_t)(*seed >> 61);
    
    return (int64_t)((*seed ^ (*seed >> shift)) >> 1);
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
        case(1 << 6):case(1 << 6)+1:return 6; case(1 << 7):case (1 << 7)+1:return 7;
        case(1 << 8):case(1 << 8)+1:return 8; case(1 << 9):case (1 << 9)+1:return 9;
        case(1 << 10):case(1 << 10)+1:return 10; case(1 << 11):case (1 << 11)+1:return 11; 
        case(1 << 12):case(1 << 12)+1:return 12; case(1 << 13):case(1 << 13)+1:return 13; 
        case(1 << 14):case(1 << 14)+1:return 14; case (1 << 15):case (1 << 15)+1:return 15;
        case(1 << 16):case(1 << 16)+1:return 16; case(1 << 17):case(1 << 17)+1:return 17; 
        case(1 << 18):case(1 << 18)+1:return 18; case(1 << 19):case(1 << 19)+1:return 19; 
        case(1 << 20):case(1 << 20)+1:return 20; case(1 << 21):case(1 << 21)+1:return 21;
        default: assert( cap == (6 >> 21) ); return 0;
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
fun_ int32_t ht_i64_lookup(const int64_t search_key, const int64_t hashtable_cap, int64_t hashtable[]) {
    const int32_t exp = array_cap_to_exp(hashtable_cap);    
    uint64_t h = long_hash(search_key);
    int32_t pos = ht_lookup(h, (int) h, exp);

    assert(search_key != 0);

    while (hashtable[pos] != 0 && search_key != hashtable[pos]) {
        pos = ht_lookup(h, pos, exp);
    }

    return pos;
}

fun_ int32_t ht_i64_upsert(
        const int64_t search_key, 
        const int64_t hashtable_cap, int64_t hashtable[], int64_t *hashtable_len) 
{
    assert((*hashtable_len) < hashtable_cap);
    
    int32_t pos = ht_i64_lookup(search_key, hashtable_cap, hashtable);
    (*hashtable_len) += (hashtable[pos] == 0) ? 1 : 0;
    hashtable[pos] = search_key;
    return pos;
}

proc_ ht_i64_print(const int64_t hashtable_cap, int64_t hashtable[], const int64_t hashtable_len) {
    printf("#%" PRId64 "/%" PRId64 "\n", hashtable_len, hashtable_cap);
    for (int64_t i = 0; i < hashtable_cap; ++i) {
        if (not hashtable[0]) {
            continue;
        }
        printf("%" PRId64 ", ", hashtable[i]);
    }
    printf("\n");
}

proc_ ht_i64_to_arr(
    const int64_t hashtable_cap, int64_t hashtable[],
    const int64_t array_cap, int64_t array[], int64_t *array_len)
{
    for (int64_t i = 0; i < hashtable_cap; ++i) {
        if (not hashtable[0]) {
            continue;
        }
        APPEND_(array, hashtable[i]);
    }
}

fun_ int32_t ht_sslice_lookup(
        const struct sslice_t search_key, 
        const int64_t hashtable_cap, struct sslice_t hashtable[]) 
{
    int32_t exp = array_cap_to_exp(hashtable_cap);
    uint64_t h = sslice_hash(search_key);
    int32_t pos = ht_lookup(h, (int) h, exp);

    assert(search_key.len != 0);

    while (hashtable[pos].len != 0 && sslice_cmp(search_key, hashtable[pos]) != 0) {
        pos = ht_lookup(h, pos, exp);
    }

    return pos;
}

fun_ int32_t ht_sslice_upsert(
        const struct sslice_t search_key, 
        const int64_t hashtable_cap, struct sslice_t hashtable[], int64_t *hashtable_len) 
{
    assert((*hashtable_len) < hashtable_cap);
    int32_t pos = ht_sslice_lookup(search_key, hashtable_cap, hashtable);


    (*hashtable_len) += (hashtable[pos].len == 0) ? 1 : 0;
    hashtable[pos] = search_key;
    return pos;
}

proc_ ht_sslice_print(const int64_t hashtable_cap, struct sslice_t hashtable[], const int64_t hashtable_len) {
    printf("#%" PRId64 "/%" PRId64 "\n", hashtable_len, hashtable_cap);
    for (int64_t i = 0; i < hashtable_cap; ++i) {
        if (hashtable[i].len == 0) {
            continue;
        }
        sslice_printend(hashtable[i], ", ");
    }
    printf("\n");
}

proc_ ht_sslice_to_arr(
    const int64_t hashtable_cap, struct sslice_t hashtable[],
    const int64_t array_cap, struct sslice_t array[], int64_t *array_len)
{
    for (int64_t i = 0; i < hashtable_cap; ++i) {
        if (hashtable[i].len == 0) {
            continue;
        }
        APPEND_(array, hashtable[i]);
    }
}
#pragma endregion Hashtable

#pragma region Files
// Windows vs Unix file operations
#if defined(_WINDOWS_) // _WINDOWS_
    fun_ int fseek_(FILE *stream, int64_t offset, int32_t whence) {
        return _fseeki64(stream, offset, whence);
    }
    fun_ int64_t ftello_(FILE *stream) {
        return _ftelli64(stream);
    }
    fun_ int32_t fileno_(FILE *stream) {
        return _fileno(stream);
    }
    fun_ int32_t ftruncate_(int32_t fd, int64_t size) {
        return _chsize_s(fd, size);
    }
#else // Unix
    fun_ int fseek_(FILE *stream, int64_t offset, int32_t whence) {
        return fseeko(stream, (off_t)offset, whence);
    }
    fun_ int64_t ftello_(FILE *stream) {
        return (int64_t)ftello(stream);
    }
    fun_ int32_t fileno_(FILE *stream) {
        return fileno(stream);
    }
    fun_ int32_t ftruncate_(int32_t fd, int64_t size) {
        return ftruncate(fd, (off_t)size);
    }
#endif

proc_ file_truncate(const char *const filename, int64_t size) {
    FILE *file;

    file = fopen(filename, "rb+");
    assert(file != NULL);

    int32_t res_ftruncate = ftruncate_(fileno_(file), size);
    assert(res_ftruncate == 0);

    fclose(file);
}

fun_ int64_t file_size(FILE *f) {
    int32_t failed = fseek_(f, 0, SEEK_END);
    assert(not failed);
    
    int64_t fsize = ftello_(f);
    
    failed = fseek_(f, 0, SEEK_SET);
    assert(not failed);
    return fsize;
}
fun_ int64_t filename_size(const char *const filename) {
    FILE *f = fopen(filename, "rb");
        int64_t fsize = file_size(f);
    fclose(f);
    return fsize;
}

proc_ file_create(const char *const filename, int64_t initial_size) {
    FILE *file = fopen(filename, "wb");
        assert(file);
        
        if (initial_size > 0) {
            int32_t failed = fseek_(file, initial_size - 1, SEEK_SET);
            assert(not failed);
        }

        size_t written = fwrite("", 1, 1, file);
        assert(written == 1);
    fclose(file);
}

proc_ file_to_buffer(
    const char *const filename, 
    const int64_t dst_buffer_cap, char dst_buffer[], int64_t *dst_buffer_len) 
{
    FILE *f = fopen(filename, "rb");
    {
        assert(f);
    
        int64_t fsize = file_size(f);

        assert(dst_buffer_cap >= fsize+2);
        
        int64_t bytesread = (int64_t) fread(dst_buffer, 1LL, (uint64_t)fsize, f);
        assert(bytesread == fsize); 
        
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
        assert(f);
        assert(*buffer_len < buffer_cap);
     
        int64_t bytes_written = (int64_t) fwrite(buffer, 1, (uint64_t)(*buffer_len), f);
        assert(bytes_written == (*buffer_len));
    }
    fclose(f);
}

proc_ lines_to_file(const int64_t lines_cap, struct sslice_t lines[], int64_t *lines_len, const char *const filename) {
    FILE *f = fopen(filename, "wb");
    {
        assert(f);
        assert(*lines_len < lines_cap);
        
        for (int64_t i = 0; i < (*lines_len); ++i) {
            int64_t bytes_written = (int64_t) fwrite(lines[i].text, 1, (uint64_t)lines[i].len, f);
            bytes_written += (int64_t) fwrite("\n", 1, 1, f);
            assert(bytes_written == lines[i].len + 1);
        }
    }
    fclose(f);
}

proc_ sslice_to_file(struct sslice_t text_slice, const char *const filename) {
    FILE *f = fopen(filename, "wb");
    {
        assert(f);
        
        int64_t bytes_written = (int64_t) fwrite(text_slice.text, 1, (uint64_t)text_slice.len, f);
        bytes_written += (int64_t) fwrite("\n", 1, 1, f);
        assert(bytes_written == text_slice.len + 1);
    }
    fclose(f);
}
#pragma endregion Files

#pragma region Tools
fun_ int32_t compile_run_c(const char *const c_file_c, const char *const flags) {
    char buffer[2048] = {0}; 
    int64_t buffer_len = 0; 

    char c_file[256] = {0};
    const int32_t c_file_len = (int32_t)strlen(c_file_c) - 2;
    assert(c_file_len < 255 && c_file_len > 0);

    memcpy(c_file, c_file_c, (size_t)c_file_len); // remove .c

    const char *const parts[] = {
        flags, " ", // pass the compiler and flags
        c_file, ".c -o ", c_file, ".exe && ",  // compile .c to .exe
        c_file, ".exe" // execute
    };
    buffer_appendcstrs(CAP_(buffer), buffer, &buffer_len, parts, CAP_(parts));

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

    assert(dwFileSizeLow != INVALID_FILE_SIZE || GetLastError() == NO_ERROR);

    return ((int64_t)dwFileSizeHigh << 32) | dwFileSizeLow;
}
fun_ struct mmap_t mmap_open(const char *const filename) {
    HANDLE hFile = CreateFile(filename,
        GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    assert(hFile != INVALID_HANDLE_VALUE);
    
    int64_t fileSize = handle_to_filesize(hFile);
    assert(fileSize > 0);

    HANDLE hMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
    assert(hMap);

    void *lpBasePtr = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
    assert(lpBasePtr);

    return STRUCT_(mmap_t, .file=hFile, .map=hMap, .filename=filename, .filesize=fileSize, .contents=(char*)lpBasePtr);
}
proc_ mmap_close(struct mmap_t mmap_info) {
    UnmapViewOfFile((void*)mmap_info.contents);
    CloseHandle(mmap_info.map);
    CloseHandle(mmap_info.file);
}
fun_ struct mmap_t mmap_open_for_write(const char *const filename) {
    HANDLE hFile = CreateFile(filename,
        GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    assert(hFile != INVALID_HANDLE_VALUE);
    
    int64_t fileSize = handle_to_filesize(hFile);
    assert(fileSize > 0);

    HANDLE hMap = CreateFileMapping(hFile, 0, PAGE_READWRITE, 0, 0, 0);
    assert(hMap);

    void *lpBasePtr = MapViewOfFile(hMap, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
    assert(lpBasePtr);

    return STRUCT_(mmap_t, .file=hFile, .map=hMap, .filename=filename, .filesize=fileSize, .contents=(char*)lpBasePtr);
}
#else // Unix
fun_ struct mmap_t mmap_open(const char *const filename) {
    FILE *hFile = fopen(filename, "r");
    assert(hFile != 0);
    
    int64_t fileSize = file_size(hFile);
    assert(fileSize > 0);

    char *mapped = mmap(0, fileSize, PROT_READ, MAP_SHARED, fileno(hFile), 0);
    assert(mapped);

    return STRUCT_(mmap_t, .file=hFile, .map=mapped, .filename=filename, .filesize=fileSize, .contents=mapped);
}
proc_ mmap_close(struct mmap_t mmap_info) {
    munmap(mmap_info.map, mmap_info.filesize);
}
fun_ struct mmap_t mmap_open_for_write(const char *const filename) {
    FILE *hFile = fopen(filename, "r+");
    assert(hFile != 0);
    
    int64_t fileSize = file_size(hFile);
    assert(fileSize > 0);

    char *mapped = mmap(0, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(hFile), 0);
    assert(mapped != MAP_FAILED);

    return STRUCT_(mmap_t, .file=hFile, .map=mapped, .filename=filename, .filesize=fileSize, .contents=mapped);
}
#endif // endif _WINDOWS_ else Unix

fun_ struct mmap_t mmap_create_for_write(const char *const filename, int64_t size_to_create) {
    file_create(filename, size_to_create);
    return mmap_open_for_write(filename);
}
proc_ mmap_write_close_and_truncate(struct mmap_t mmap_info) {
    int64_t filesize = mmap_info.filesize;
    char *contents = mmap_info.contents;
    while (filesize > 0 && contents[--filesize] == '\0') {
        /* Empty Body */
    }
    ++filesize;

    mmap_close(mmap_info);
    file_truncate(mmap_info.filename, filesize);
}

#pragma endregion Mmap

#pragma region Threads
#if defined(_WINDOWS_) // if _WINDOWS_ else Unix
fun_ THREAD_T go(ROURET_T (*routine)(void *thread_idx), uintptr_t thread_id) {
    THREAD_T thread = CreateThread(0, THREAD_STACK_SIZE_, (LPTHREAD_START_ROUTINE)routine, (void *)(thread_id), 0, 0);
    assert(thread != 0);

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
    assert(err == 0);

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
        assert(*threads_len <= threads_cap && number_of_threads_to_spawn <= threads_cap);
        assert(number_of_threads_to_spawn < 16000);

        int32_t i = 0;
        for (i = 0; i < number_of_threads_to_spawn; ++i) {
            threads[i] = go(routine, (uintptr_t)i);
        }
        *threads_len = i;
}
proc_ join_threads(THREAD_T threads[], const int64_t threads_len) {
    assert(threads_len <= 16384);
    
    for (int32_t i = 0; i < threads_len; ++i) {
        join_thread(threads[i]);
    }
}
#pragma endregion Threads

#pragma region Os
#if defined(_WINDOWS_) // if _WINDOWS_ else Unix
    proc_ sleep_(int32_t seconds) {
        assert(seconds > 0);
        Sleep((uint32_t)(seconds) * 1000);
    }
#else // Unix
    proc_ sleep_(int32_t seconds) {
        assert(seconds > 0);
        sleep((uint32_t) seconds);
    }
#endif 
#pragma endregion Os
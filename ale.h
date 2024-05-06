#pragma once

#pragma region Includes
#include <assert.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
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

#define SZ_NOT_FOUND_ (size_t)-1 // constant for using as in-band error

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
#define CAP_(static_array_) (\
    assert(sizeof(static_array_) != sizeof(&static_array_)), \
    sizeof(static_array_) / sizeof(*(static_array_)) \
)

#define APPEND_(array, new_element) \
    assert((*array##_len) < array##_cap); \
    array[(*array##_len)++] = new_element

#define DELIDX_(array, idx_to_del) \
    assert((*array##_len) > 0); \
    array[idx_to_del] = array[--(*array##_len)]

#define NEWCMPF_(fun_name, type, ...) \
    fun_ int fun_name(const void *avoid, const void *bvoid) { \
        const type a = *((const type *)avoid); \
        const type b = *((const type *)bvoid); \
        __VA_ARGS__; \
    }
#pragma endregion Macros

#pragma region Structs
// void *file; void *map; const char *const filename; const size_t filesize; char *contents;
struct mmap_t { void *file; void *map; const char *const filename; const size_t filesize; char *contents; };

// size_t len; const char *text;
struct sslice_t { size_t len; const char *text; };
#pragma endregion Structs

#pragma region Strings
fun_ struct sslice_t to_sslice(const char *const cstring) { return STRUCT_(sslice_t, strlen(cstring), cstring); }

proc_ sslice_print(const struct sslice_t slice) { printf("%.*s\n", (int)slice.len, slice.text); }
proc_ sslice_printend(const struct sslice_t slice, const char *const end) { printf("%.*s%s", (int)slice.len, slice.text, end); }

fun_ int sslice_cmp(const struct sslice_t a_text_slice, const struct sslice_t b_text_slice) {
    size_t min_len = a_text_slice.len <= b_text_slice.len ? a_text_slice.len : b_text_slice.len;
    for (size_t i = 0; i < min_len; ++i) {
        if (a_text_slice.text[i] != b_text_slice.text[i]) {
            return a_text_slice.text[i] - b_text_slice.text[i];
        }
    }
    return a_text_slice.len == b_text_slice.len ? 0 : (a_text_slice.len < b_text_slice.len ? - 1 : 1);
}

proc_ set_locale_to_utf8(void) {
    setlocale(LC_CTYPE, "en_US.UTF-8");
}
// You need to have set a locale, default locale is C locale. Best to call set_locale_to_utf8
fun_ int sslice_cmp_locale(const struct sslice_t a_text_slice, const struct sslice_t b_text_slice) {
    char a[512], b[512];
    assert(a_text_slice.len < 512 && b_text_slice.len < 512);

    memcpy(a, a_text_slice.text, a_text_slice.len);
    memcpy(b, b_text_slice.text, b_text_slice.len);
    a[a_text_slice.len] = 0;
    b[b_text_slice.len] = 0;

    return strcoll(a, b);
}

fun_ int startswith(const struct sslice_t prefix, const struct sslice_t text) {
    return sslice_cmp(prefix, STRUCT_(sslice_t, .len=prefix.len, .text=text.text)) == 0;
}

fun_ struct sslice_t trimmed(const struct sslice_t text_slice) {
    const char *const text = text_slice.text;
    size_t text_len = text_slice.len;

    size_t start = 0;
    for (start = 0; start < text_len && text[start] <= ' '; ++start) {
        --text_len;
    }

    if (text_len > 0) {
        for (size_t last = text_len - 1; last < text_len && text_len > 0 && text[last] <= ' '; --last) {
            --text_len;
        }
    }

    return STRUCT_(sslice_t, text_len, &text[start]);
}

fun_ size_t char_pos(const char letter, const char *const cstring) {
    const char *ptr = strchr(cstring, letter);
    if (ptr >= cstring) {
        return (size_t)(ptr - cstring);
    } else {
        return SZ_NOT_FOUND_;
    }
}

fun_ size_t char_pos_slice(const char letter, const struct sslice_t text_slice) {
    const char *const ptr = 
        text_slice.len == 0 ? 0 
        : (const char *) memchr((void const *)text_slice.text, letter, text_slice.len);
    if (ptr >= text_slice.text) {
        return (size_t)(ptr - text_slice.text);
    } else {
        return SZ_NOT_FOUND_;
    }
}

fun_ struct sslice_t subss(struct sslice_t text_slice, const int start, const int end) {
    size_t pos_start = start < 0 ? text_slice.len - (size_t)abs(start) : (size_t)start;
    size_t pos_end = end < 0 ? text_slice.len - (size_t)abs(end) : (size_t)end;

    size_t true_end = pos_end > text_slice.len ? text_slice.len : pos_end;
    size_t true_start = pos_start != 0 && pos_start > true_end ? true_end-1 : pos_start;

    size_t len = true_start < true_end ? true_end - true_start : 0;

    return STRUCT_(sslice_t, len, &text_slice.text[true_start]);
}

proc_ split(const struct sslice_t text_slice, const char splitter, 
    const size_t parts_cap, struct sslice_t parts[], size_t *parts_len) 
{
    struct sslice_t cur = text_slice;
    size_t pos = 0;
    
    for (pos = char_pos_slice(splitter, cur); pos != SZ_NOT_FOUND_; pos = char_pos_slice(splitter, cur)) {
        APPEND_(parts, STRUCT_(sslice_t, .len=pos, .text=cur.text));
        ++pos;
        cur.len -= pos; 
        cur.text = &cur.text[pos];
    }

    if (cur.len > 0) {
        APPEND_(parts, STRUCT_(sslice_t, .len=cur.len, .text=cur.text));
    }
}

proc_ to_lines(const struct sslice_t text_slice, const size_t lines_cap, struct sslice_t lines[], size_t *lines_len) {
    split(text_slice, '\n', lines_cap, lines, lines_len);
}

fun_ struct sslice_t next_line(struct sslice_t *text) {
    struct sslice_t line = *text;

    size_t slashnpos = char_pos_slice('\n', *text);

    if (slashnpos == SZ_NOT_FOUND_) {
        text->len = 0;
        text->text = "";

        return line;
    } 

    line.len = slashnpos;
    
    text->text = &text->text[(slashnpos+1)];
    text->len -= (slashnpos+1);

    return line;
}

proc_ buffer_to_lines(
    char buffer[], const size_t buffer_len, 
    const size_t lines_cap, struct sslice_t lines[], size_t *lines_len) 
{   
    to_lines(STRUCT_(sslice_t, .len=buffer_len, .text=buffer), lines_cap, lines, lines_len);
}

proc_ buffer_appendslice(const size_t dst_buffer_cap, char dst_buffer[], size_t *dst_buffer_len, 
    const struct sslice_t src_chars_slice) 
{
    assert(src_chars_slice.len <= dst_buffer_cap);

    memcpy(&dst_buffer[*dst_buffer_len], src_chars_slice.text, src_chars_slice.len);

    *dst_buffer_len += src_chars_slice.len;
    dst_buffer[(*dst_buffer_len)] = 0;
}
proc_ buffer_appendcstr(const size_t dst_buffer_cap, char dst_buffer[], size_t *dst_buffer_len, 
const char *const cstr) 
{
    const size_t cstr_len = strlen(cstr);

    assert(cstr_len <= dst_buffer_cap);

    memcpy(&dst_buffer[*dst_buffer_len], cstr, cstr_len);

    *dst_buffer_len += cstr_len;
    dst_buffer[(*dst_buffer_len)] = 0;
}
proc_ buffer_appendcstrs(const size_t dst_buffer_cap, char dst_buffer[], size_t *dst_buffer_len, 
    const char *const cstrs[], const size_t cstrs_len) 
{ 
    for (size_t i = 0; i < cstrs_len; ++i) {
        buffer_appendcstr(dst_buffer_cap, dst_buffer, dst_buffer_len, cstrs[i]);
    }
}
#pragma endregion Strings

#pragma region Math
fun_ size_t power2_number_mod(size_t power2_number, size_t modval) { return (power2_number) & (modval - 1); }

fun_ size_t greatest_common_divisor(size_t m, size_t n) {
    size_t tmp = 0;
    while (m) { tmp = m; m = n % m; n = tmp; }       
    return n;
}
fun_ size_t least_common_multiple(size_t m, size_t n) { return m / greatest_common_divisor(m, n) * n; }

/* COMMENTED OUT SINCE NOT ALL PLATFORMS HAVE FLOATS*/
// fun_ float sqrtapproximate_newton(float n) {
//     float x = 1;
//     while (fabsf(x * x - n) > 1e-8f)
//         x = (x + n / x) / 2;
//     return x;
// }
// fun_ float inversesqrtapproximate_newton(float number) {
//     union FloatIntUnion { float f; int i; };

//     float y = 0;

//     union FloatIntUnion u; u.f = number;
//     u.i = 0x5f3759df - (u.i >> 1);
    
//     y = u.f;
//     y = y * (1.5f - ((number * 0.5f) * y * y));

//     return y;
// }
#pragma endregion Math

#pragma region Random
fun_ size_t rnd(size_t seed[1]) {
    size_t sd = *seed = *seed * 0x9b60933458e17d7dULL + 0xd737232eeccdf7edULL;

    unsigned int shift = (sizeof(size_t)*4 - 3) - (sd >> (sizeof(size_t)*8 - 3));
    
    return (size_t)((sd ^ (sd >> shift)) >> 1u);
}
#pragma endregion Random

#pragma region Hashfuns
fun_ size_t sslice_hash(const struct sslice_t chars_slice) {
    const char *const chars = chars_slice.text;
    size_t chars_len = chars_slice.len;

    size_t h = 0x7A5662DCDF;
    
    for(size_t i = 0; i < chars_len; ++i) { 
        h ^= chars[i] & 255; h *= 1111111111111111111;
    }
    return (h ^ (h >> 31)) >> 1;
}

fun_ size_t number_hash(size_t number) {
    size_t x = number;
    
    x *= 0x94d049bb133111eb; 
    x = (x ^ (x >> 31));
    x *= 0xbf58476d1ce4e5b9; 
    
    return (x ^ (x >> 31)) >> 1;
}
#pragma endregion Hashfuns

#pragma region Hashtable
fun_ unsigned int array_cap_to_exp(const size_t cap) {
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
fun_ unsigned int ht_lookup(
    size_t hash, // 1st hash acts as base location
    unsigned int index, // 2nd "hash" steps over the "list of elements" from base-location
    unsigned int exp // power-2 exp used as the Hash Table capacity
) {
    unsigned int step = (unsigned int)(hash >> (sizeof(size_t)*8 - exp)) | 1;
    return (((unsigned int)index + step) & ((unsigned int) ((1 << exp) - 1)));
}

fun_ unsigned int ht_number_lookup(const size_t search_key, const size_t hashtable_cap, size_t hashtable[]) {
    const unsigned int exp = array_cap_to_exp(hashtable_cap);
    size_t h = number_hash(search_key);
    unsigned int pos = ht_lookup(h, (unsigned int) h, exp);

    assert(search_key != 0);

    while (hashtable[pos] != 0 && search_key != hashtable[pos]) {
        pos = ht_lookup(h, pos, exp);
    }

    return pos;
}

fun_ unsigned int ht_number_upsert(
        const size_t search_key, 
        const size_t hashtable_cap, size_t hashtable[], size_t *hashtable_len) 
{   
    unsigned int pos = (
        assert((*hashtable_len) < hashtable_cap),
        ht_number_lookup(search_key, hashtable_cap, hashtable)
    );
    (*hashtable_len) += (hashtable[pos] == 0) ? 1 : 0;
    hashtable[pos] = search_key;
    return pos;
}

proc_ ht_number_print(const size_t hashtable_cap, size_t hashtable[], const size_t hashtable_len) {
    printf("#%zu %zu\n", hashtable_len, hashtable_cap);
    for (size_t i = 0; i < hashtable_cap; ++i) {
        if (not hashtable[0]) {
            continue;
        }
        printf("%zu, ", hashtable[i]);
    }
    printf("\n");
}

proc_ ht_number_to_arr(
    const size_t hashtable_cap, size_t hashtable[],
    const size_t array_cap, size_t array[], size_t *array_len)
{
    for (size_t i = 0; i < hashtable_cap; ++i) {
        if (not hashtable[0]) {
            continue;
        }
        APPEND_(array, hashtable[i]);
    }
}

fun_ unsigned int ht_sslice_lookup(
        const struct sslice_t search_key, 
        const size_t hashtable_cap, struct sslice_t hashtable[]) 
{
    unsigned int exp = array_cap_to_exp(hashtable_cap);
    size_t h = sslice_hash(search_key);
    unsigned int pos = ht_lookup(h, (unsigned int) h, exp);

    assert(search_key.len != 0);

    while (hashtable[pos].len != 0 && sslice_cmp(search_key, hashtable[pos]) != 0) {
        pos = ht_lookup(h, pos, exp);
    }

    return pos;
}

fun_ unsigned int ht_sslice_upsert(
        const struct sslice_t search_key, 
        const size_t hashtable_cap, struct sslice_t hashtable[], size_t *hashtable_len) 
{
    unsigned int pos = (
        assert((*hashtable_len) < hashtable_cap),
        ht_sslice_lookup(search_key, hashtable_cap, hashtable)
    );


    (*hashtable_len) += (hashtable[pos].len == 0) ? 1 : 0;
    hashtable[pos] = search_key;
    return pos;
}

proc_ ht_sslice_print(const size_t hashtable_cap, struct sslice_t hashtable[], const size_t hashtable_len) {
    printf("#%zu %zu\n", hashtable_len, hashtable_cap);
    for (size_t i = 0; i < hashtable_cap; ++i) {
        if (hashtable[i].len == 0) {
            continue;
        }
        sslice_printend(hashtable[i], ", ");
    }
    printf("\n");
}

proc_ ht_sslice_to_arr(
    const size_t hashtable_cap, struct sslice_t hashtable[],
    const size_t array_cap, struct sslice_t array[], size_t *array_len)
{
    for (size_t i = 0; i < hashtable_cap; ++i) {
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
    fun_ int fileno_(FILE *stream) {
        return _fileno(stream);
    }
    fun_ int fseek_(FILE *stream, size_t offset, int whence) {
        return _fseeki64(stream, (long long)offset, whence);
    }
    fun_ size_t ftello_(FILE *stream) {
        return (size_t)_ftelli64(stream);
    }
    fun_ int ftruncate_(int fd, size_t size) {
        return _chsize_s(fd, (long long)size);
    }
#else // Unix
    fun_ int fileno_(FILE *stream) {
        return fileno(stream);
    }
    fun_ int fseek_(FILE *stream, size_t offset, int whence) {
        return fseeko(stream, (off_t)offset, whence);
    }
    fun_ size_t ftello_(FILE *stream) {
        return (size_t)ftello(stream);
    }
    fun_ int ftruncate_(int fd, size_t size) {
        return ftruncate(fd, (off_t)size);
    }
#endif

proc_ file_truncate(const char *const filename, size_t size) {
    FILE *file = fopen(filename, "rb+");
        int res_ftruncate = (assert(file != NULL), ftruncate_(fileno_(file), size));
        assert(res_ftruncate == 0);
    fclose(file);
}

fun_ size_t file_size(FILE *f) {
    int success = fseek_(f, 0, SEEK_END) == 0;
    
    size_t fsize = (assert(success), ftello_(f));
    
    success = fseek_(f, 0, SEEK_SET) == 0;
    assert(success);

    return fsize;
}
fun_ size_t filename_size(const char *const filename) {
    FILE *f = fopen(filename, "rb");
        size_t fsize = file_size(f);
    fclose(f);
    return fsize;
}

proc_ file_create(const char *const filename, size_t initial_size) {
    FILE *file = fopen(filename, "wb");
        int success = (
            assert(file && initial_size > 0),
            fseek_(file, initial_size - 1, SEEK_SET) == 0
        );

        size_t written = (
            assert(success),
            fwrite("", 1, 1, file)
        );
        assert(written == 1);
    fclose(file);
}

proc_ file_to_buffer(
    const char *const filename, 
    const size_t dst_buffer_cap, char dst_buffer[], size_t *dst_buffer_len) 
{
    FILE *f = fopen(filename, "rb");
        size_t fsize = (assert(f), file_size(f));
        
        size_t bytesread = (
            assert(dst_buffer_cap >= fsize+2),
            fread(dst_buffer, 1LL, fsize, f)
        );
        assert(bytesread == fsize); 
        
        dst_buffer[fsize] = '\0';
        *dst_buffer_len = fsize;
    fclose(f);
}

proc_ file_to_lines(
    const char *const filename, 
    const size_t buffer_cap, char buffer[], size_t *buffer_len, 
    const size_t lines_cap, struct sslice_t lines[], size_t *lines_len) 
{
    file_to_buffer(filename, buffer_cap, buffer, buffer_len);
    
    to_lines(STRUCT_(sslice_t, .len=*buffer_len, .text=buffer), lines_cap, lines, lines_len);
}

proc_ buffer_to_file(const size_t buffer_cap, char buffer[], size_t *buffer_len, const char *const filename) {  
    FILE *f = fopen(filename, "wb");
        size_t bytes_written = (
            assert(f && *buffer_len < buffer_cap),
            fwrite(buffer, 1, *buffer_len, f)
        );
        assert(bytes_written == *buffer_len);
    fclose(f);
}

proc_ lines_to_file(const size_t lines_cap, struct sslice_t lines[], size_t *lines_len, const char *const filename) {
    FILE *f = fopen(filename, "wb");
        assert(f);
        assert(*lines_len < lines_cap);
        
        for (size_t i = 0; i < (*lines_len); ++i) {
            size_t bytes_written = fwrite(lines[i].text, 1, lines[i].len, f);
            bytes_written += fwrite("\n", 1, 1, f);
            assert(bytes_written == lines[i].len + 1);
        }
    fclose(f);
}

proc_ sslice_to_file(struct sslice_t text_slice, const char *const filename) {
    FILE *f = fopen(filename, "wb");
        size_t bytes_written = (
            assert(f),
            fwrite(text_slice.text, 1, text_slice.len, f)
        );
        bytes_written += fwrite("\n", 1, 1, f);
        assert(bytes_written == text_slice.len + 1);
    fclose(f);
}
#pragma endregion Files

#pragma region Mmap
#if defined(_WINDOWS_) // if _WINDOWS_ else Unix
fun_ size_t handle_to_filesize(HANDLE hFile) {
    DWORD dwFileSizeHigh = 0; 
    DWORD dwFileSizeLow = GetFileSize(hFile, &dwFileSizeHigh);

    assert(dwFileSizeLow != INVALID_FILE_SIZE || GetLastError() == NO_ERROR);

    return ((size_t)dwFileSizeHigh << 32) | (size_t)dwFileSizeLow;
}
fun_ struct mmap_t mmap_open(const char *const filename) {
    HANDLE hFile = CreateFile(filename,
        GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    
    size_t fileSize = (assert(hFile != INVALID_HANDLE_VALUE), handle_to_filesize(hFile));

    HANDLE hMap = (assert(fileSize > 0), CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0));

    void *lpBasePtr = (assert(hMap), MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0));
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
    
    size_t fileSize = (
        assert(hFile != INVALID_HANDLE_VALUE),
        handle_to_filesize(hFile)
    );

    HANDLE hMap = (assert(fileSize > 0), CreateFileMapping(hFile, 0, PAGE_READWRITE, 0, 0, 0));

    void *lpBasePtr = (assert(hMap), MapViewOfFile(hMap, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0));
    assert(lpBasePtr);

    return STRUCT_(mmap_t, .file=hFile, .map=hMap, .filename=filename, .filesize=fileSize, .contents=(char*)lpBasePtr);
}
#else // Unix
fun_ struct mmap_t mmap_open(const char *const filename) {
    FILE *hFile = fopen(filename, "r");
    
    size_t fileSize = (assert(hFile != 0), file_size(hFile));

    char *mapped = (assert(fileSize > 0), mmap(0, fileSize, PROT_READ, MAP_SHARED, fileno(hFile), 0));
    assert(mapped);

    return STRUCT_(mmap_t, .file=hFile, .map=mapped, .filename=filename, .filesize=fileSize, .contents=mapped);
}
proc_ mmap_close(struct mmap_t mmap_info) {
    munmap(mmap_info.map, mmap_info.filesize);
}
fun_ struct mmap_t mmap_open_for_write(const char *const filename) {
    FILE *hFile = fopen(filename, "r+");
    
    size_t fileSize = (assert(hFile != 0), file_size(hFile));;

    char *mapped = (assert(fileSize > 0), mmap(0, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(hFile), 0));
    assert(mapped != MAP_FAILED);

    return STRUCT_(mmap_t, .file=hFile, .map=mapped, .filename=filename, .filesize=fileSize, .contents=mapped);
}
#endif // endif _WINDOWS_ else Unix

fun_ struct mmap_t mmap_create_for_write(const char *const filename, size_t size_to_create) {
    file_create(filename, size_to_create);
    return mmap_open_for_write(filename);
}
proc_ mmap_write_close_and_truncate(struct mmap_t mmap_info) {
    size_t filesize = mmap_info.filesize;
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
fun_ THREAD_T go(ROURET_T (*routine)(void *thread_idx), size_t thread_id) {
    THREAD_T thread = CreateThread(0, THREAD_STACK_SIZE_, (LPTHREAD_START_ROUTINE)routine, (void *)(thread_id), 0, 0);
    assert(thread != 0);

    return thread;
}
proc_ join_thread(THREAD_T thread) {
    WaitForSingleObject(thread, INFINITE);
}
#else // Unix
fun_ THREAD_T go(ROURET_T (*routine)(void *thread_idx), size_t thread_id) {
    int success = 0;
    THREAD_T thread = ZERO_INIT_;

    pthread_attr_t attr = ZERO_INIT_;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE_);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    success = pthread_create(&thread, &attr, routine, (void*)(thread_id)) == 0;
    assert(success);

    return thread;
}
proc_ join_thread(THREAD_T thread) {
    pthread_join(thread, 0);
}
#endif // endif _WINDOWS_ else Unix

proc_ go_threads(
    ROURET_T (*routine)(void *thread_idx), unsigned int number_of_threads_to_spawn, 
    const size_t threads_cap, THREAD_T threads[], size_t *threads_len)
{
    size_t total_after_spawn = *threads_len + number_of_threads_to_spawn;    
    assert(total_after_spawn <= threads_cap && total_after_spawn < 8192);

    for (size_t i = *threads_len; i < total_after_spawn; ++i) {
        threads[i] = go(routine, (size_t)i);
    }
    *threads_len += number_of_threads_to_spawn;
}
proc_ join_threads(THREAD_T threads[], const size_t threads_len) {
    assert(threads_len <= 8192);
    
    for (size_t i = 0; i < threads_len; ++i) {
        join_thread(threads[i]);
    }
}
#pragma endregion Threads

#pragma region System
#if defined(_WINDOWS_) // if _WINDOWS_ else Unix
    proc_ sleep_(unsigned int seconds) {
        assert(seconds < 60 * 60 * 24 + 1);
        Sleep(seconds * 1000);
    }
#else // Unix
    proc_ sleep_(unsigned int seconds) {
        assert(seconds < 60 * 60 * 24 + 1);
        sleep(seconds);
    }
#endif 

fun_ int compile_run_c(const char *const c_file_c, const char *const flags) {
    char buffer[2048] = {0}; 
    size_t buffer_len = 0; 

    char c_file[256] = {0};
    const size_t c_file_len = strlen(c_file_c) - 2;

    void *ptr = (
        assert(c_file_len < 255),
        memcpy(c_file, c_file_c, c_file_len)
    ); // remove .c

    const char *const parts[] = {
        flags, " ", // pass the compiler and flags
        c_file, ".c -o ", c_file, ".exe && ",  // compile .c to .exe
        c_file, ".exe" // execute
    };
    buffer_appendcstrs(CAP_(buffer), buffer, &buffer_len, parts, CAP_(parts));

    (void) ptr;

    printf("\n%.*s\n", (int)buffer_len, buffer);
    return system(buffer);
}
#pragma endregion System

#pragma region Benchmark
static clock_t BENCHCLOCK_ = 0;
proc_ start_benchclock(void) {
    BENCHCLOCK_ = clock(); 
} 
proc_ stop_benchclock(void) {
    printf("\n\nExecuted in %f seconds \n", (double)(clock() - BENCHCLOCK_) / CLOCKS_PER_SEC);
}

#define BENCH_MAIN_ int main(void) {start_benchclock(); run(); stop_benchclock(); return 0;}
#pragma endregion Benchmark

#pragma once
#ifdef __cplusplus
extern "C" { /* Cancels Name Mangling when compiled as C++ */
#endif


/* configure */
#define THREAD_STACK_SIZE_ 64 * 1024
#define MAX_NTHREADS 8192


#pragma region Includes
/* */
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#if defined(_WIN32) || defined(_WIN64)
    #if defined(_MSC_VER)
        #pragma warning(disable: 28301) 
        #pragma warning(disable: 28251)
    #endif
    #include <intrin.h>
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #include <io.h>
#else /* assume Unix: */
    #include <unistd.h>
    #include <pthread.h>
    #include <sys/stat.h>
    #include <sys/mman.h>
#endif

/* really useful macro for array size at compile time */
#define arrsizeof(static_array_) (sizeof(static_array_) / sizeof(*(static_array_)))

/* constant for using as in-band error in size_t returns */
#define SZ_NOT_FOUND_ (size_t)-1 

#if (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901) && !defined(__cplusplus)
#define _C89_COMPAT_
#define inline
#define __func__ "c89"
#endif
/* */
#pragma endregion Includes


#pragma region Strings
/* */
/* struct lenstr_t { size_t len; const char *str; }; */
struct lenstr_t { size_t len; const char *str; };

static inline struct lenstr_t to_lenstr(const char *const cstring) {  
    struct lenstr_t lenstr = {0, 0};
    lenstr.len = strlen(cstring);
    lenstr.str = cstring;

    return lenstr;
}

static inline void lenstr_print(const struct lenstr_t lenstr) { printf("%.*s\n", (int)lenstr.len, lenstr.str); }
static inline void lenstr_printend(const struct lenstr_t lenstr, const char *const end) { printf("%.*s%s", (int)lenstr.len, lenstr.str, end); }

static inline int lenstr_cmp(const struct lenstr_t a_lenstr, const struct lenstr_t b_lenstr) {
    size_t min_len = a_lenstr.len <= b_lenstr.len ? a_lenstr.len : b_lenstr.len;
    size_t i;
    for (i = 0; i < min_len; ++i) {
        if (a_lenstr.str[i] != b_lenstr.str[i]) {
            return a_lenstr.str[i] - b_lenstr.str[i];
        }
    }
    return a_lenstr.len == b_lenstr.len ? 0 : (a_lenstr.len < b_lenstr.len ? - 1 : 1);
}

static inline void set_locale_to_utf8(void) {
    setlocale(LC_CTYPE, "en_US.UTF-8");
}
/* You need to have set a locale, default locale is C locale. Best to call set_locale_to_utf8 */
static inline int lenstr_cmp_locale(const struct lenstr_t a_lenstr, const struct lenstr_t b_lenstr) {
    char a[512], b[512];
    assert(a_lenstr.len < 512 && b_lenstr.len < 512);

    memcpy(a, a_lenstr.str, a_lenstr.len);
    memcpy(b, b_lenstr.str, b_lenstr.len);
    a[a_lenstr.len] = 0;
    b[b_lenstr.len] = 0;

    return strcoll(a, b);
}

static inline int startswith(const struct lenstr_t prefix, const struct lenstr_t text) {
    struct lenstr_t text_start = {0, 0}; 
    text_start.len = prefix.len; 
    text_start.str = text.str;
    return lenstr_cmp(prefix, text_start) == 0;
}

static inline size_t char_pos(const char letter, const char *const cstring) {
    const char *ptr = strchr(cstring, letter);
    if (ptr >= cstring) {
        return (size_t)(ptr - cstring);
    } else {
        return SZ_NOT_FOUND_;
    }
}

static inline size_t char_pos_lenstr(const char letter, const struct lenstr_t lenstr) {
    const char *const ptr = 
        lenstr.len == 0 ? 0 
        : (const char *) memchr((void const *)lenstr.str, letter, lenstr.len);
    if (ptr >= lenstr.str) {
        return (size_t)(ptr - lenstr.str);
    } else {
        return SZ_NOT_FOUND_;
    }
}

static inline struct lenstr_t subss(struct lenstr_t lenstr, const int start, const int end) {
    size_t pos_start = start < 0 ? lenstr.len - (size_t)abs(start) : (size_t)start;
    size_t pos_end = end < 0 ? lenstr.len - (size_t)abs(end) : (size_t)end;

    size_t true_end = pos_end > lenstr.len ? lenstr.len : pos_end;
    size_t true_start = pos_start != 0 && pos_start > true_end ? true_end-1 : pos_start;

    size_t len = true_start < true_end ? true_end - true_start : 0;

    struct lenstr_t sub = {0, 0};
    sub.len = len;
    sub.str = &lenstr.str[true_start];
    return sub;
}

static inline void split(const struct lenstr_t lenstr, const char splitter, 
    const size_t parts_cap, struct lenstr_t parts[], size_t *parts_len) 
{
    struct lenstr_t cur = lenstr;
    size_t pos = 0;
    
    for (pos = char_pos_lenstr(splitter, cur); pos != SZ_NOT_FOUND_; pos = char_pos_lenstr(splitter, cur)) {
        struct lenstr_t part = {0, 0};
        part.len = pos;
        part.str = cur.str;

        assert(*parts_len < parts_cap);
        parts[(*parts_len)++] = part;
        ++pos;
        cur.len -= pos; 
        cur.str = &cur.str[pos];
    }

    if (cur.len > 0) {
        struct lenstr_t part = {0, 0};
        part.len = cur.len;
        part.str = cur.str;
        assert(*parts_len < parts_cap);
        parts[(*parts_len)++] = part;
    }
}

static inline void to_lines(const struct lenstr_t lenstr, const size_t lines_cap, struct lenstr_t lines[], size_t *lines_len) {
    split(lenstr, '\n', lines_cap, lines, lines_len);
}


static inline void buffer_to_lines(
    const char buffer[], const size_t buffer_len, 
    const size_t lines_cap, struct lenstr_t lines[], size_t *lines_len) 
{   
    struct lenstr_t text = {0, 0};
    text.len = buffer_len;
    text.str = buffer;
    to_lines(text, lines_cap, lines, lines_len);
}

static inline void buffer_append_lenstr(const size_t dst_buffer_cap, char dst_buffer[], size_t *dst_buffer_len, 
    const struct lenstr_t src_chars_lenstr) 
{
    assert((*dst_buffer_len) + src_chars_lenstr.len < dst_buffer_cap);

    memcpy(&dst_buffer[*dst_buffer_len], src_chars_lenstr.str, src_chars_lenstr.len);

    *dst_buffer_len += src_chars_lenstr.len;
    dst_buffer[(*dst_buffer_len)] = 0;
}
static inline void buffer_append_cstr(const size_t dst_buffer_cap, char dst_buffer[], size_t *dst_buffer_len, 
const char *const cstr) 
{
    const size_t cstr_len = strlen(cstr);

    assert((*dst_buffer_len) + cstr_len < dst_buffer_cap);

    memcpy(&dst_buffer[*dst_buffer_len], cstr, cstr_len);

    *dst_buffer_len += cstr_len;
    dst_buffer[(*dst_buffer_len)] = 0;
}
static inline void buffer_append_cstrs(const size_t dst_buffer_cap, char dst_buffer[], size_t *dst_buffer_len, 
    const char *const cstrs[], const size_t cstrs_len) 
{ 
    size_t i;
    for (i = 0; i < cstrs_len; ++i) {
        buffer_append_cstr(dst_buffer_cap, dst_buffer, dst_buffer_len, cstrs[i]);
    }
}

static inline size_t size_without_nullbytes(const char *const buffer, size_t current_size) {
    while (current_size > 0 && buffer[--current_size] == '\0') {
        /* Empty Body */
    }
    
    return ++current_size;
}
/* */
#pragma endregion Strings


#pragma region Math
/* */
static inline size_t power2_number_mod(size_t power2_number, size_t modval) { return (power2_number) & (modval - 1); }

static inline size_t greatest_common_divisor(size_t m, size_t n) {
    size_t tmp = 0;
    while (m) { tmp = m; m = n % m; n = tmp; }       
    return n;
}
static inline size_t least_common_multiple(size_t m, size_t n) { return m / greatest_common_divisor(m, n) * n; }
/* */
#pragma endregion Math


#pragma region Bits
/* */
static inline unsigned char highbit(unsigned int uint_) {
    #if defined(__GNUC__) || defined(__clang__)
        return (unsigned char)((char)(sizeof(int) * 8 - 1) - (char)__builtin_clz(uint_));
    #elif defined(_MSC_VER)
        unsigned long index;
        _BitScanReverse(&index, uint_);
        return (unsigned char) index;
    #else 
        unsigned int capacity = 1 << 15;
        unsigned char exp = 15;

        while (capacity > uint_) {
            capacity = capacity >> 1;
            --exp;
        }

        return exp;
    #endif     
}
/* */
#pragma endregion Bits


#pragma region Hashfuns
/* */
static inline size_t lenstr_hash(const struct lenstr_t chars_lenstr) {
    const char *const chars = chars_lenstr.str;
    size_t chars_len = chars_lenstr.len;

    size_t h = ((size_t)2147482697);
    
    size_t i;
    for(i = 0; i < chars_len; ++i) { 
        h ^= chars[i] & 255; h *= ((size_t)2147482951);
    }
    return (h ^ (h >> 31)) >> 1;
}

static inline size_t number_hash(size_t number) {
    size_t x = number;
    
    x *= ((size_t)2147480369); 
    x = (x ^ (x >> 31));
    x *= ((size_t)2147476769); 
    
    return (x ^ (x >> 31)) >> 1;
}
/* */
#pragma endregion Hashfuns


#pragma region Hashtable
/* */
/* Mask-Step-Index (MSI) lookup. Returns the next index. */
static inline unsigned int ht_lookup(
    size_t hash, /* 1st hash acts as base location */
    unsigned int index, /* 2nd "hash" steps over the "list of elements" from base-location */
    unsigned char exp /* power-2 exp used as the Hash Table capacity */
) {
    unsigned int step = (unsigned int)(hash >> (sizeof(size_t)*8 - exp)) | 1;
    return (((unsigned int)index + step) & ((unsigned int) ((1 << exp) - 1)));
}

static inline unsigned int ht_number_lookup(const size_t search_key, const unsigned int hashtable_cap, size_t hashtable[]) {
    const unsigned char exp = highbit((unsigned int)hashtable_cap);
    size_t h = number_hash(search_key);
    unsigned int pos = ht_lookup(h, (unsigned int)h, exp);

    assert(search_key != 0);

    while (hashtable[pos] != 0 && search_key != hashtable[pos]) {
        pos = ht_lookup(h, pos, exp);
    }

    return pos;
}

static inline unsigned int ht_number_upsert(
        const size_t search_key, 
        const unsigned int hashtable_cap, size_t hashtable[], unsigned int *hashtable_len) 
{   
    unsigned int pos = (
        (void)assert((*hashtable_len) < hashtable_cap),
        ht_number_lookup(search_key, hashtable_cap, hashtable)
    );
    (*hashtable_len) += (hashtable[pos] == 0) ? 1 : 0;
    hashtable[pos] = search_key;
    return pos;
}

static inline void ht_number_to_arr(
    const unsigned int hashtable_cap, size_t hashtable[],
    const unsigned int array_cap, size_t array[], unsigned int *array_len)
{
    unsigned int i;
    for (i = 0; i < hashtable_cap; ++i) {
        if (!hashtable[0]) {
            continue;
        }
        assert(*array_len < array_cap);
        array[(*array_len)++] = hashtable[i];
    }
}

static inline unsigned int ht_lenstr_lookup(
        const struct lenstr_t search_key, 
        const unsigned int hashtable_cap, struct lenstr_t hashtable[]) 
{
    unsigned char exp = highbit((unsigned int)hashtable_cap);
    size_t h = lenstr_hash(search_key);
    unsigned int pos = ht_lookup(h, (unsigned int)h, exp);

    assert(search_key.len != 0);

    while (hashtable[pos].len != 0 && lenstr_cmp(search_key, hashtable[pos]) != 0) {
        pos = ht_lookup(h, pos, exp);
    }

    return pos;
}

static inline unsigned int ht_lenstr_upsert(
        const struct lenstr_t search_key, 
        const unsigned int hashtable_cap, struct lenstr_t hashtable[], unsigned int *hashtable_len) 
{
    unsigned int pos = (
        (void)assert((*hashtable_len) < hashtable_cap),
        ht_lenstr_lookup(search_key, hashtable_cap, hashtable)
    );


    (*hashtable_len) += (hashtable[pos].len == 0) ? 1 : 0;
    hashtable[pos] = search_key;
    return pos;
}

static inline void ht_lenstr_print(const unsigned int hashtable_cap, struct lenstr_t hashtable[], const unsigned int hashtable_len) {
    unsigned int i;
    printf("#%u %u\n", hashtable_len, hashtable_cap);
    for (i = 0; i < hashtable_cap; ++i) {
        if (hashtable[i].len == 0) {
            continue;
        }
        lenstr_printend(hashtable[i], ", ");
    }
    printf("\n");
}

static inline void ht_lenstr_to_arr(
    const unsigned int hashtable_cap, struct lenstr_t hashtable[],
    const unsigned int array_cap, struct lenstr_t array[], unsigned int *array_len)
{
    unsigned int i;
    for (i = 0; i < hashtable_cap; ++i) {
        if (hashtable[i].len == 0) {
            continue;
        }
        assert(*array_len < array_cap);
        array[(*array_len)++] = hashtable[i];
    }
}
/* */
#pragma endregion Hashtable


#pragma region Files
/* */
/* Windows vs Unix file operations */
#if defined(_WINDOWS_) /* _WINDOWS_ */
    static inline int fileno_(FILE *stream) {
        return _fileno(stream);
    }
    static inline size_t filelen_(FILE *stream) {
        return (size_t)_filelengthi64(fileno_(stream));
    }
    static inline int fseek_(FILE *stream, size_t offset, int whence) {
        #ifdef _C89_COMPAT_
            return _fseeki64(stream, (long int)offset, whence);
        #else 
            return _fseeki64(stream, (long long int)offset, whence);
        #endif 
    }
    static inline void ftruncate_(FILE *stream, size_t size) {
        #ifdef _C89_COMPAT_
            int success = _chsize_s(fileno_(stream), (long int)size) == 0;
        #else
            int success = _chsize_s(fileno_(stream), (long long int)size) == 0;
        #endif
        assert(success);
    }

    #if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
        static inline FILE * fopen_(const char *pathname, const char *mode) {
            FILE *f = 0;
            errno_t err = fopen_s(&f, pathname, mode);
            
            assert(err == 0);
            return f;
        }
    #else 
        static inline FILE * fopen_(const char *pathname, const char *mode) {
            return fopen(pathname, mode);
        }
    #endif
#else /* Unix */
    static inline int fileno_(FILE *stream) {
        return fileno(stream);
    }
    static inline int fseek_(FILE *stream, size_t offset, int whence) {
        return fseeko(stream, (off_t)offset, whence);
    }
    static inline size_t filelen_(FILE *stream) {
        struct stat file_stat;
        int fstat_success = fstat(fileno(stream), &file_stat) != -1;
        
        assert(fstat_success);
        return (size_t)file_stat.st_size;
    }
    static inline void ftruncate_(FILE *stream, size_t size) {
        int success = ftruncate(fileno_(stream), (off_t)size) == 0;
        assert(success);
    }

    static inline FILE * fopen_(const char *pathname, const char *mode) {
        return fopen(pathname, mode);
    }
#endif

static inline void file_create(const char *const filename, size_t initial_size) {
    FILE *file = fopen_(filename, "wb");
        int fopen_success = (
            (void)assert(file && initial_size > 0),
            fseek_(file, initial_size - 1, SEEK_SET) == 0
        );

        size_t written = (
            (void)assert(fopen_success),
            fwrite("", 1, 1, file)
        );
        assert(written == 1);
    fclose(file);
}

static inline void file_to_buffer(
    const char *const filename, 
    const size_t dst_buffer_cap, char dst_buffer[], size_t *dst_buffer_len) 
{
    FILE *f = fopen_(filename, "rb");
        size_t fsize = ((void)assert(f), filelen_(f));
        
        size_t bytesread = (
            (void)assert(dst_buffer_cap >= fsize+2),
            fread(dst_buffer, 1, fsize, f)
        );
        assert(bytesread == fsize); 
        
        dst_buffer[fsize] = '\0';
        *dst_buffer_len = fsize;
    fclose(f);
}

static inline void file_to_lines(
    const char *const filename, 
    const size_t buffer_cap, char buffer[], size_t *buffer_len, 
    const size_t lines_cap, struct lenstr_t lines[], size_t *lines_len) 
{
    file_to_buffer(filename, buffer_cap, buffer, buffer_len);
    
    {
        struct lenstr_t text = {0, 0};
        text.len = *buffer_len;
        text.str = buffer;
        to_lines(text, lines_cap, lines, lines_len);
    }
}

static inline void buffer_to_file(const size_t buffer_cap, char buffer[], size_t *buffer_len, const char *const filename) {  
    FILE *f = fopen_(filename, "wb");
        size_t bytes_written = (
            (void)assert(f && *buffer_len < buffer_cap),
            fwrite(buffer, 1, *buffer_len, f)
        );
        assert(bytes_written == *buffer_len);
    fclose(f);
}

static inline void lines_to_file(const size_t lines_cap, struct lenstr_t lines[], size_t *lines_len, const char *const filename) {
    FILE *f = fopen_(filename, "wb");
        size_t i;
        assert(f);
        assert(*lines_len < lines_cap);
        
        for (i = 0; i < (*lines_len); ++i) {
            size_t bytes_written = fwrite(lines[i].str, 1, lines[i].len, f);
            bytes_written += fwrite("\n", 1, 1, f);
            assert(bytes_written == lines[i].len + 1);
        }
    fclose(f);
}

static inline void lenstr_to_file(struct lenstr_t lenstr, const char *const filename) {
    FILE *f = fopen_(filename, "wb");
        size_t bytes_written = (
            (void)assert(f),
            fwrite(lenstr.str, 1, lenstr.len, f)
        );
        bytes_written += fwrite("\n", 1, 1, f);
        assert(bytes_written == lenstr.len + 1);
    fclose(f);
}
/* */
#pragma endregion Files


#pragma region Mmap
/* */
#if defined(_WINDOWS_) /* if _WINDOWS_ else Unix */
static inline char * mmap_open(const char *const filename, const char *const mode, size_t *out_buffer_len) {
    void * mmap_handle;
    char * out_mmap_buffer;

    int readit = mode[0] == 'r' && mode[1] != '+';

    FILE *file = fopen_(filename, mode);
    { /* File opened */
        size_t fileSize = filelen_(file);
        assert(fileSize > 0);

        mmap_handle = CreateFileMapping(
            (void *)(size_t)_get_osfhandle(fileno_(file)),
            0, 
            readit ? PAGE_READONLY : PAGE_READWRITE, 
            0, 0, 0
        );
        { /* FileMapping handle opened */
            assert(mmap_handle);

            out_mmap_buffer = (char *)MapViewOfFile(
                mmap_handle, 
                FILE_MAP_READ | (readit ? 0 : FILE_MAP_WRITE),
                0, 0, 0
            );
            assert(out_mmap_buffer);
        } /* Closing FileMapping handle: */
        CloseHandle(mmap_handle);

        *out_buffer_len = fileSize;
    } /* Closing file: */
    fclose(file);

    return out_mmap_buffer;
}
static inline void mmap_close(char *mmap_buffer, size_t mmap_buffer_size) {
    assert(mmap_buffer_size);
    UnmapViewOfFile((void *)mmap_buffer);
}
#else /* Unix */
static inline char * mmap_open(const char *const filename, const char *const mode, size_t *out_buffer_len) {
    char * out_mmap_buffer;

    int readit = mode[0] == 'r' && mode[1] != '+';
    
    FILE *file = fopen_(filename, mode);
    { /* File opened */
        size_t fileSize = filelen_(file);
        assert(fileSize);
        *out_buffer_len = fileSize;

        out_mmap_buffer = (char *)mmap(
            0, 
            fileSize, 
            PROT_READ | (readit ? 0 : PROT_WRITE), 
            MAP_SHARED, 
            fileno_(file),
            0
        );
        assert(out_mmap_buffer);
    } /* Closing file: */
    fclose(file);

    return out_mmap_buffer;
}
static inline void mmap_close(char *mmap_buffer, size_t mmap_buffer_size) {
    munmap(mmap_buffer, mmap_buffer_size);
}
#endif /* endif _WINDOWS_ else Unix */
/* */
#pragma endregion Mmap


#pragma region Threads
/* */
#if defined(_WINDOWS_)
    /* is _WINDOWS_ */
#define THREAD_T HANDLE

static inline THREAD_T go(void * (*routine)(void *thread_idx), size_t thread_id) {
    THREAD_T thread = CreateThread(0, THREAD_STACK_SIZE_, (LPTHREAD_START_ROUTINE)(size_t)routine, (void *)(thread_id), 0, 0);
    assert(thread != 0);

    return thread;
}
static inline void join_thread(THREAD_T thread) {
    WaitForSingleObject(thread, INFINITE);
}
    /* */
#else 
    /* is Unix */
#define THREAD_T pthread_t

static inline THREAD_T go(void * (*routine)(void *thread_idx), size_t thread_id) {
    int create_thread_success = 0;
    THREAD_T thread;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE_);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    create_thread_success = pthread_create(&thread, &attr, routine, (void *)(thread_id)) == 0;
    assert(create_thread_success);

    return thread;
}
static inline void join_thread(THREAD_T thread) {
    pthread_join(thread, 0);
}
#endif /* endif _WINDOWS_ else Unix */

static inline void go_threads(
    void * (*routine)(void *thread_idx), unsigned int number_of_threads_to_spawn, 
    const unsigned int threads_cap, THREAD_T threads[], unsigned int *threads_len)
{
    unsigned int i;
    unsigned int total_after_spawn = *threads_len + number_of_threads_to_spawn;
    assert(*threads_len < MAX_NTHREADS);
    assert(total_after_spawn < MAX_NTHREADS);
    assert(total_after_spawn <= threads_cap);


    for (i = *threads_len; i < total_after_spawn; ++i) {
        threads[i] = go(routine, (size_t)i);
    }
    *threads_len += number_of_threads_to_spawn;
}
static inline void join_threads(THREAD_T threads[], const unsigned int threads_len) {
    unsigned int i;
    assert(threads_len <= MAX_NTHREADS);
    
    for (i = 0; i < threads_len; ++i) {
        join_thread(threads[i]);
    }
}
/* */
#pragma endregion Threads


#pragma region System
/* */
#if defined(_WINDOWS_) /* if _WINDOWS_ else Unix */
    static inline void sleep_(unsigned int seconds) {
        assert(seconds < 60 * 60 * 24 + 1);
        Sleep(seconds * 1000);
    }
#else /* Unix */
    static inline void sleep_(unsigned int seconds) {
        assert(seconds < 60 * 60 * 24 + 1);
        sleep(seconds);
    }
#endif 

static inline int compile_c(const char *const c_file_c, const char *const flags) {
    char buffer[2048] = {0}; 
    size_t buffer_len = 0; 

    char c_file[256] = {0};
    const size_t dot_pos = char_pos('.', c_file_c);
    const size_t c_file_len = dot_pos == SZ_NOT_FOUND_ ? strlen(c_file_c) : dot_pos;

    void *ptr = (
        (void)assert(c_file_len < 255),
        memcpy(c_file, c_file_c, c_file_len)
    ); /* remove .c */

    const char * parts[] = {
        0/* 0 flags*/, 
        " ", 0 /*2 c_file*/, ".c -o ", 0/*4 c_file*/, ".exe ",  /* compile .c to .exe */
        "&& echo _ Compiled ", 0 /*7 c_file*/, ".exe! \n", /* print that it was compiled */
    };
    parts[0] = flags; parts[2] = c_file; parts[4] = c_file; parts[7] = c_file;

    buffer_append_cstrs(arrsizeof(buffer), buffer, &buffer_len, parts, arrsizeof(parts));

    (void) ptr;

    printf("\n%.*s\n", (int)buffer_len, buffer);
    return system(buffer);
}

static inline int compile_run_c(const char *const c_file_c, const char *const flags) {
    char buffer[2048] = {0}; 
    size_t buffer_len = 0; 

    char c_file[256] = {0};
    const size_t dot_pos = char_pos('.', c_file_c);
    const size_t c_file_len = dot_pos == SZ_NOT_FOUND_ ? strlen(c_file_c) : dot_pos;

    void *ptr = (
        (void)assert(c_file_len < 255),
        memcpy(c_file, c_file_c, c_file_len)
    ); /* remove .c */

    const char * parts[] = {
        0 /*0 flags*/, /* pass the compiler and flags */
        " ", 0 /*2 c_file*/, ".c -o ", 0 /*4 c_file*/, ".exe ",  /* compile .c to .exe */
        "&& echo _ Running ", 0 /*7 c_file*/, ".exe... ", /* print that execution will begin */
        "&& \"./", 0 /*10 c_file*/, ".exe", "\" " /* execute */
    };
    parts[0] = flags; parts[2] = c_file; parts[4] = c_file; parts[7] = c_file; parts[10] = c_file;

    buffer_append_cstrs(arrsizeof(buffer), buffer, &buffer_len, parts, arrsizeof(parts));

    (void) ptr;

    printf("\n%.*s\n", (int)buffer_len, buffer);
    return system(buffer);
}
/* */
#pragma endregion System


#pragma region Benchmark
/* */
static clock_t BENCHCLOCK_ = 0;
static inline void start_benchclock(void) {
    BENCHCLOCK_ = clock(); 
} 
static inline void stop_benchclock(void) {
    clock_t end_time = clock();
    unsigned int total_time = (unsigned int)(end_time - BENCHCLOCK_);
    unsigned int seconds = (unsigned int) (total_time / CLOCKS_PER_SEC);
    unsigned int milliseconds = (unsigned int) ((total_time % CLOCKS_PER_SEC) * 1000 / CLOCKS_PER_SEC);
    printf("\n\nExecuted in %u seconds and %03u milliseconds \n", seconds, milliseconds);
}

#define BENCH_MAIN_ int main(void) {start_benchclock(); run(); stop_benchclock(); return 0;}
/* */
#pragma endregion Benchmark


#ifdef __cplusplus
} /* closes extern C, Cancels Name Mangling when compiled as C++ */
#endif

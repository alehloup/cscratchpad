/*
      █████╗ ██╗     ███████╗
     ██╔══██╗██║     ██╔════╝
     ███████║██║     █████╗  
     ██╔══██║██║     ██╔══╝  
     ██║  ██║███████╗███████╗
     ╚═╝  ╚═╝╚══════╝╚══════╝

            A L E . h
        · Brutalist C ·
*/
// Heavely inspired by:
//  Chris Nullprogram "Examples of quick hash tables and dynamic arrays in C" 
//      https://nullprogram.com/blog/2025/01/19/
//  Aartaka Pretty C and "C Until its no Longer C" 
//      https://aartaka.me/c-not-c


#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* OS INCLUDES */
#ifdef _WIN32
    #define OSWIN_
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #include <io.h>
    typedef ptrdiff_t ssize_t;
#else // assume Posix
    #define OSLIN_
    #include <unistd.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
#endif

#define not	!
#define and	&&
#define or	||


/* CONSTANTS */

#define KB (1024LL)
#define MB (1024LL * 1024LL)
#define GB (1024LL * 1024LL * 1024LL)

#ifdef OSWIN_
    #define TMP_FOLDER "%TMP%/"
#else
    #define TMP_FOLDER "/tmp/"
#endif


/* MACROS */

// Discard anything
#define _ (void)
#define ssizeof(x) ( (ssize_t)sizeof(x) )
#define countof(x)  ( ssizeof(x) / ssizeof(x[0]) )

#ifndef typeof
    #define typeof __typeof__
#endif
#if defined(_MSC_VER) && !defined(__clang__)
    #ifndef alignof
        #define alignof _Alignof
    #endif
#else
    #ifndef alignof
        #define alignof __alignof__
    #endif
#endif
#ifndef threadlocal
    #define threadlocal _Thread_local
#endif
#if defined(__clang__) || defined(__GNUC__)
    #define atleast static
#else
    #define atleast /* static */
#endif

#define salignof(x) ( (ssize_t)alignof(x) )

#define macrocat_(a, b) a##b
#define macrocat(a, b) macrocat_(a, b)

#if !defined(min) && !defined(max)
    #define min(a,b) ( (a) < (b) ? (a) : (b) )
    #define max(a,b) ( (a) > (b) ? (a) : (b) )
#endif

#define foreach(var, array, length) \
    for (typeof((array)[0]) *var = (array), *var##_end_ = (array) + (length); var < var##_end_; ++var)
#define forspan(var, start, end) \
    for (typeof((start)[0]) *var = (start), *var##_end_ = (end); var < var##_end_; ++var)

// Check the macro with later in this file, which uses the _Generic drop!
#define with_close(close, var, ...) \
    for (typeof(__VA_ARGS__) var = __VA_ARGS__; var; close(var), var = NULL)


/* MEMORY */

extern size_t strlen(const char *);
static inline ssize_t cstrlen(const char *s) {
    return !s ? 0 : (ssize_t)strlen(s);
}

extern int strcmp(const char *s1, const char *s2);
static inline int cstrcmp(const char *s1, const char *s2) {
    if (!s1 or !s2) return !(s1 == NULL and s2 == NULL);

    return strcmp(s1, s2);
}

extern void * memchr(const void *s, int c, size_t n);
static inline void * cmemchr(const void *s, char c, ssize_t n) {
    assert(n >= 0 and "cmemchr: n can't be negative!");
    return !n ? NULL : memchr(s, c & 255, (size_t)n);
}

extern int memcmp(const void *s1, const void *s2, size_t n);
static inline int cmemcmp(const void *s1, const void *s2, ssize_t n) {
    assert(n >= 0 and "cmemcmp: n can't be negative!");
    return !n ? 0 : memcmp(s1, s2, (size_t)n);
}

extern void * memcpy(void *dest, const void *src, size_t n);
static inline void * cmemcpy(void *dest, const void *src, ssize_t n) {
    assert(n >= 0 and "cmemcpy: n can't be negative!");
    return n ? memcpy(dest, src, (size_t)n) : dest;
}

extern void * memset(void *s, int c, size_t n);
static inline void * cmemset(void *s, int c, ssize_t n) {
    assert(n >= 0 and "cmemset: n can't be negative!");
    return n ? memset(s, c, (size_t)n) : s;
}


/* ARENA */

typedef struct arena { char *beg; char *end; } arena;
static inline void * alloc(arena *a, ptrdiff_t count, ptrdiff_t size, ptrdiff_t align) {
    assert(count >= 0 and size >= 0 and "count and size can't be negative!");
    if (!count or !size) return a->beg; // alloc 0 elements or n elements of size 0 is a no-op

    // Nullprogram trick, quoted from his blog:
    // we can compute modulo by subtracting one and masking with AND
    // however, we want the number of bytes to advance to the next alignment, which is the inverse
    ptrdiff_t pad = (ptrdiff_t)( -(uintptr_t)a->beg & (uintptr_t)(align-1) );

    // Explicit Integer Overflow check in the assert
    assert( count < (a->end - a->beg - pad)/size and "arena space left is not enough" );
    ssize_t total_size = count*size;
    
    void *r = a->beg + pad;
    a->beg += pad + total_size;

    return cmemset(r, 0, total_size);
}
#define new(parena, n, t) ((t *)(alloc(parena, n, (ptrdiff_t) sizeof(t), (ptrdiff_t) alignof(t))))
#define arr2arena(arr) ( (arena){(char *) arr, (char *) (arr + countof(arr))} )


/* STRING */

typedef struct str { char *data; ptrdiff_t len; } str;
#define S(s) ( (str){(char *) s, max(ssizeof(s) - 1, 0)} )

#define cstr2str(cstring) ((str){ (char *)cstring, cstrlen(cstring) })

static inline str scopy(arena *a, str original) {
    str copied = original;
    copied.data = new(a, copied.len, char);
    cmemcpy(copied.data, original.data, copied.len);
    return copied;
}

// Converts a str to a null-terminated char* for use with legacy C APIs
// BEWARE: char * is only valid while the arena keeps its data!
static inline char * str2cstr(arena *a, str string) {
    if ((string.data + string.len) != a->beg) {
        string = scopy(a, string);
    }
    *(a->beg++) = '\0';

    return string.data;
}

static inline int sequal(str a, str b) {
    return (a.len != b.len) ? 0 : !cmemcmp(a.data, b.data, a.len);
}

static inline int scomp(str a, str b) {
    int mincomp = cmemcmp(a.data, b.data, min(a.len, b.len));

    return !mincomp ? (int)(a.len - b.len) : mincomp;
}

// cat always starts by putting str head at the top of the arena
static inline str cat(arena *a, str head, str tail) {
    // use copy as realoc if head is not at the top of arena
    if (!head.data or (head.data + head.len) != a->beg) {
        head = scopy(a, head);
    }
    
    // since head is always at the top of arena, copy extends it
    head.len += scopy(a, tail).len;
    
    return head;
}
static inline str sjoin(arena *a, str *arr, ssize_t len, char separator) {
    str res = {0};
    char *psep = &separator;
    if (!a or !arr) return res;

    if (!separator) separator = ' ';

    foreach(s, arr, len) {
        res = cat(a, res, *s);
        psep = new(a, 1, char);
        *psep = separator;
    }
    *psep = '\0'; //makes res.data compatible to null terminated c strings

    return res;
}

static inline str span(char *beg, char *end) {
    return (str){beg, beg ? max(end - beg, 0) : 0};
}

typedef struct {str head; str tail; int ok; int padding;} head_tail_ok;
// cut s at c, returns {str head; str tail; int ok;}
static inline head_tail_ok cut(str s, char c) {
    head_tail_ok r = {0};
    if (!s.len) return r;

    char *end = s.data + s.len;

    char *it = cmemchr(s.data, c, s.len);
    if (!it) it = end;

    r.ok = it < end;
    r.head = span(s.data, it);
    r.tail = span(it + r.ok, end);

    return r;
}
#define forsep(var, string, sep) \
    head_tail_ok var##_res = cut( (string) , sep); \
    for (str var = var##_res.head; \
         var##_res.head.len or var##_res.tail.len; \
         var##_res = cut(var##_res.tail, sep), var = var##_res.head)

#define forlines(var, string) forsep(var, string, '\n')

// splits string into arr, returns number of elements splited into
static inline ptrdiff_t split(str text, char sep, str arr[atleast 1], ssize_t cap) {
    assert(cap > 0 and "cap must be atleast 1");

    ssize_t len = 0;
    forsep(part, text, sep) {
        if (len >= cap) break;

        arr[len++] = part;
    }

    return len;
}

static inline int starts(str s, str prefix) {
    return (s.len >= prefix.len) \
        and sequal(span(s.data, s.data + prefix.len), prefix);
}
static inline int ends(str s, str suffix) {
    return (s.len >= suffix.len) \
        and sequal(span(s.data + s.len - suffix.len, s.data + s.len), suffix);
}

static inline str trimleft(str s) {
    for (; s.len and (unsigned char)*s.data <= ' '; ++s.data, --s.len);
    return s;
}
static inline str trimright(str s) {
    for (; s.len and (unsigned char)s.data[s.len-1] <= ' '; --s.len);
    return s;
}
static inline str trim(str s) {
    return trimleft(trimright(s));
}

static inline str sadvanced(str s, ssize_t i) {
    if (i > 0) {
        s.data += i;
        s.len -= i;
    }
    return s;
}

static inline int parseint(str s) {
    unsigned int r = 0;
    int sign = 1;

    foreach(pc, s.data, s.len) {
        switch(*pc) {
            case '+': case '.': case ',': case '_': break;
            case '-': sign *= -1; break;
            default: r = 10u*r + (unsigned)(*pc - '0');
        }
    }

    return sign * (int)r;
}

static inline float parsefloat(str s) {
    float r = 0.0f;
    float sign = 1.0f;
    float exp = 0.0f;

    foreach(pc, s.data, s.len) {
        switch(*pc) {
            // exp notation unsupported, 
            // most of times is close to zero, so it returns 0
            case 'e': case 'E': return 0.0f; break;

            case '+': case ',': case '_': break;
            case '-': sign *= -1; break;
            case '.': exp = 1;  break;
            default: r = 10.0f*r + (*pc - '0'); 
                     exp *= 0.1f;
        }
    }

    return sign * r * (exp == 0.0f ? 1.0f : exp);
}


/* HASH GENERIC */

static inline size_t hash_str(str s, size_t seed) {
    size_t h = seed;
    for(int i = 0; i < s.len; ++i) {
        h ^= s.data[i] & 255;
        h *= 1111111111111111111; // intentional uint overflow to spread bits
    }
    return h;
}
static inline size_t hash_cstr(const char * cs, size_t seed) {
    return hash_str(cstr2str(cs), seed);
}

#define hash64(a, seed) \
    _Generic((a), \
            str: hash_str, const char *: hash_cstr, \
            char *: hash_cstr \
        )(a, seed)


/* EQUAL GENERIC */

static inline int char_equal(char a, char b) { return a == b; }
static inline int int_equal(int a, int b) { return a == b; }
static inline int ssize_equal(ssize_t a, ssize_t b) { return a == b; }
static inline int size_equal(size_t a, size_t b) { return a == b; }
static inline int float_equal(float a, float b)
{
    const float epsilon = 1e-5f;
	const float min_abs = 1e-8f;

	float abs_a = a < 0? -a : a;
	float abs_b = b < 0? -b : b;
	float diff  = a - b;
    diff = diff < 0? -diff: diff;

	if (abs_a < min_abs && abs_b < min_abs) {
		// Both numbers are extremely close to 0
		return diff < min_abs;
	}

	// Use relative comparison
	return diff <= epsilon * max(abs_a, abs_b);
}
static inline int double_equal(double a, double b)
{
    const double epsilon = 1e-9;
	const double min_abs = 1e-12;

	double abs_a = a < 0? -a : a;
	double abs_b = b < 0? -b : b;
	double diff  = a - b;
    diff = diff < 0? -diff: diff;

	if (abs_a < min_abs && abs_b < min_abs) {
		// Both numbers are extremely close to 0
		return diff < min_abs;
	}

	// Use relative comparison
	return diff <= epsilon * max(abs_a, abs_b);
}
static inline int str_equal(str a, str b) {
    return sequal(a, b);
}
static inline int cstr_equal(const char * a, const char * b) {
    return !cstrcmp(a, b);
}

#define equal(a, b) \
    _Generic((a), \
        char: char_equal, int: int_equal, \
        ssize_t: ssize_equal, size_t: size_equal, \
        float: float_equal, double: double_equal, \
        str: str_equal, const char *: cstr_equal,  \
        char *: cstr_equal \
    )(a, b)


/* HASH TRIE */

/* 
    [REQUIRED] to define htrie_valtype before including this header
    
    hashtrie_ds.h implements:
    |htrie_prefix|lookup for ht|htrie_prefix|struct, |htrie_keytype|key -> |htrie_valtype|val
*/

#define htrie_valtype str
#include "htrie_ds.h"

#define htrie_valtype int
#include "htrie_ds.h"


/* PRINT GENERIC */

static inline void print_char(char c) { printf("%c", c); }
static inline void print_int(int i) { printf("%d", i); }
static inline void print_ssize(ssize_t s) { printf("%zd", s); }
static inline void print_size(size_t z) { printf("%zu", z); }
static inline void print_float(float f) { printf("%.5f", (double)f); }
static inline void print_double(double d) { printf("%.9f", d); }
static inline void print_str(str s) { printf("%.*s", (int)s.len, s.data); }
static inline void print_cstr(const char * cs) { printf("%s", cs); }

#define print(x) \
    _Generic((x), \
        char: print_char, int: print_int, \
        ssize_t: print_ssize, size_t: print_size, \
        float: print_float, double: print_double, \
        str: print_str, const char *: print_cstr, \
        char *: print_cstr \
    )(x)
#define println(x) print(x); printf("\n")

#define printarr(arr, n) print("{ "); foreach(arr_el, arr, n) { print(*arr_el); print(" "); } println("}")


/* SCAN GENERIC */

static inline void scan_char(char *c) { (void)scanf(" %c", c); }
static inline void scan_int(int *i) { (void)scanf(" %d", i); }
static inline void scan_ssize(ssize_t *s) { (void)scanf(" %zd", s); }
static inline void scan_size(size_t *z) { (void)scanf(" %zu", z); }
static inline void scan_float(float *f) { (void)scanf(" %f", f); }
static inline void scan_double(double *d) { (void)scanf(" %lf", d); }

static inline str scanword(arena *a) {
    char buffer[256];
    (void)scanf(" %255s", buffer);

    ssize_t len = cstrlen(buffer);
    char *data = new(a, len + 1, char);
    cmemcpy(data, buffer, len);
    data[len] = 0;

    return (str){ data, len };
}
static inline str scanline(arena *a) {
    char buffer[1024];
    (void)scanf(" %1023[^\n]", buffer);

    ssize_t len = cstrlen(buffer);
    char *data = new(a, len + 1, char);
    cmemcpy(data, buffer, len);
    data[len] = 0;

    return (str){ data, len };
}

#define scan(x) \
    _Generic((x), \
        char: scan_char, int: scan_int, \
        ssize_t: scan_ssize, size_t: scan_size, \
        float: scan_float, double: scan_double \
    )(&x)


/* MINI-RAII */

typedef int descriptor_t;

static inline int empty_descriptor(descriptor_t descriptor) {
    return descriptor < 0; 
}
static inline int empty_str(str string) {
    return !string.len or !string.data;
}
static inline int empty_cstr(const char * cstring) {
    return !cstring or !cstring[0];
}
static inline int empty_pointer(void *pointer) {
    return pointer == NULL;
}
#define empty(x) \
    _Generic((x), \
        descriptor_t: empty_descriptor, \
        str: empty_str, \
        const char *: empty_cstr, char *: empty_cstr, \
        default: empty_pointer \
    )(x)

typedef str MMAP;
static inline void mclose(MMAP s) {
    if (empty(s)) return;

    #ifdef OSWIN_
        UnmapViewOfFile(s.data);
    #else
        munmap(s.data, (size_t)s.len);
    #endif
}

static inline void drop_file(FILE *file) {
    if (file) _ fclose(file);
}
static inline void drop_mmap(MMAP s) {
    mclose(s);
}
#define drop(x) \
    _Generic((x), \
        FILE*: drop_file, \
        MMAP: drop_mmap \
    )(x)

#define with(var, ...) \
    for (typeof(__VA_ARGS__) var = __VA_ARGS__; !empty(var); drop(var), var = (typeof(var)){0})


/* FILES */

static inline const char * ensure_binary_mode(const char * cstring) {
    ssize_t len = cstrlen(cstring);
    assert(len > 0 and len < 4);

    if (cmemchr(cstring, 'b', len)) {
        return cstring;
    }

    // C allows returning string literals
    switch (cstring[0]) {
        case 'r': return (cstring[1] == '+' ? "rb+" : "rb");
        case 'w': return (cstring[1] == '+' ? "wb+" : "wb");
        case 'a': return (cstring[1] == '+' ? "ab+" : "ab");
        default: return "rb";
    }
}

static inline str file2str(arena * a, str filename) {
    str r = {a->beg, 0};

    with(file, fopen(filename.data, "rb")) {
        r.len = (ssize_t)fread(r.data, 1, (size_t)(a->end - a->beg), file);

        if (r.len < 0 or r.data + r.len + 1 >= a->end) {
            r.len = 0;
        } else {
            a->beg += r.len + 1;
            r.data[r.len] = '\0';
        }
    }

    return r;
}
static inline int str2file(str content, str filename) {
    size_t written = 0;

    with(file, fopen(filename.data, "wb")) {
        written = fwrite(content.data, 1, (size_t)content.len, file);
    }

    return written != (size_t)content.len;
}

static inline ssize_t filelen(FILE *file) {

    #ifdef OSWIN_
        ssize_t len = (ssize_t)_filelengthi64(_fileno(file));
    #else // assume POSIX
        struct stat file_stat;
        int fstat_success = fstat(fileno(file), &file_stat) != -1; 
        assert(fstat_success);
        ssize_t len = (ssize_t)file_stat.st_size;
    #endif 

    assert(len >= 0 and "filelen must not be negative!");
    return len;
}

static inline MMAP mopen(const char *filename, const char *mode) {
    int readit = mode[0] == 'r' and mode[1] != '+';
    MMAP r = {0};

    with(file, fopen(filename, ensure_binary_mode(mode))) {
        ssize_t len = filelen(file);
        assert(len > 0 and "filelen can't be zero");

        #ifdef OSWIN_
            void *mhandle = CreateFileMapping(
                (void *)(size_t)_get_osfhandle(_fileno(file)), 0, 
                readit ? PAGE_READONLY : PAGE_READWRITE, 0, 0, 0
            );
                if (mhandle) {
                    r.data = (char *)MapViewOfFile(
                        mhandle, 
                        FILE_MAP_READ | (readit ? 0 : FILE_MAP_WRITE),
                        0, 0, 0
                    );
                    CloseHandle(mhandle);
                }
        #else // assume POSIX
            r.data = (char *)mmap(
                0, (size_t)len,
                PROT_READ | (readit ? 0 : PROT_WRITE), 
                MAP_SHARED, fileno(file), 0
            );
        #endif

        if (r.data) {
            r.len = len;
        }
    }

    return r;
}


/* MACRO ALGOS */

#define decl_cmpfn(fnname, type, ... ) \
    static inline int fnname(const void *a_, const void *b_) { const type *a = a_; const type *b = b_;  return (__VA_ARGS__); }

#define sort(arr, n, cmpfn) qsort(arr, n, sizeof(arr[0]), cmpfn)


/* OS FUNCTIONS */

static inline int scmd(arena *a, str command) {
    println(command); 
    printf("\n");

    int ret = system(str2cstr(a, command));
    printf("\n");

    return ret;
}

static inline void print_stopwatch(clock_t stopwatch) {
    clock_t end = clock();
    if (end < stopwatch) {
        printf("Clock error, end < start");
        return;
    }
    printf("  Executed in %.3f seconds\n", (double)(clock() - stopwatch) / (double)CLOCKS_PER_SEC);
}

static inline void sleepsecs(unsigned int seconds) {
    assert(seconds < 60 * 60 * 24 + 1);

    #if defined(OSWIN_)
        Sleep(seconds * 1000);
    #else // Unix
        sleep(seconds);
    #endif
}

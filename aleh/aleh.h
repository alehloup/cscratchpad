// Heavely inspired by:
//  Chris Nullprogram "Examples of quick hash tables and dynamic arrays in C" 
//      https://nullprogram.com/blog/2025/01/19/
//  Aartaka Pretty C and "C Until its no Longer C" 
//      https://aartaka.me/c-not-c


#pragma once

// yes, I want the source Fortified > : )
#ifndef _FORTIFY_SOURCE
    #define _FORTIFY_SOURCE 3
#endif

// necessary to fix file and offsets to 64 
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE

// necessary when using msvc-clang, msvc yaps too much
#ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS
#endif

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


#ifndef typeof
    #define typeof __typeof__
#endif
#ifndef alignof
    #define alignof __alignof__
#endif
#ifndef threadlocal
    #define threadlocal _Thread_local
#endif
#if defined(__clang__) || defined(__GNUC__)
    #define atleast static
#else
    #define atleast /* static */
#endif


#if defined(_WIN32) && !defined(ssize_t)
  typedef ptrdiff_t ssize_t;
#endif
#define countof(x)  ((ssize_t)(sizeof(x) / sizeof(x[0])))


#if !defined(_ISO646_H) && !defined(and)
    #define not	!
    #define and	&&
    #define or	||
#endif
#define nil NULL
#define None NULL
#define True true
#define False false

#define KB (1024LL)
#define MB (1024LL * 1024LL)
#define GB (1024LL * 1024LL * 1024LL)


extern size_t strlen(const char *);
static inline ssize_t cstrlen(const char *s) {
    return !s ? 0 : (ssize_t)strlen(s);
}

extern int memcmp(const void *s1, const void *s2, size_t n);
static inline int cmemcmp(const void *s1, const void *s2, ssize_t n) {
    assert(n >= 0 and "cmemcmp: N can't be negative!");
    return !n ? 1 : memcmp(s1, s2, (size_t)n);
}

extern void * memcpy(void *dest, const void *src, size_t n);
static inline void * cmemcpy(void *dest, const void *src, ssize_t n) {
    assert(n >= 0 and "cmemcpy: N can't be negative!");
    return n ? memcpy(dest, src, (size_t)n) : dest;
}

extern void * memset(void *s, int c, size_t n);
static inline void * cmemset(void *s, int c, ssize_t n) {
    assert(n >= 0 and "cmemset: N can't be negative!");
    return n ? memset(s, c, (size_t)n) : s;
}


#define macro_abs(x) ({ typeof(x) _x = (x); _x < 0 ? -_x : _x; })
#if !defined(min) && !defined(max)
    #define min(a,b) ({ typeof(a) _a=(a); typeof(b) _b=(b); _a<_b?_a:_b; })
    #define max(a,b) ({ typeof(a) _a=(a); typeof(b) _b=(b); _a>_b?_a:_b; })
#endif
#define between(lo, n, hi) \
    ({ typeof(n) _n = (n); typeof(lo) _lo = (lo); typeof(hi) _hi = (hi); _n >= _lo && _n <= _hi; })
#define limit(lo, n, hi) \
    ({ typeof(n) _n = (n); typeof(lo) _lo = (lo); typeof(hi) _hi = (hi); _n < _lo ? _lo : _n > _hi ? _hi : _n; })
#define clamp(lo, n, hi)  limit(lo, n, hi)


#define foreach(var, array, length) \
    for (typeof((array)[0]) *var = (array), *var##_end_ = (array) + (length); var < var##_end_; ++var)
#define fortimes(var, times) \
    for (typeof(times) var##_max_ = (times), var = 0; var < var##_max_; ++var)
#define forasc(var, from, to) \
    for (typeof(to) var##_to_ = (to), var = (from); var < var##_to_; ++var)
#define fordesc(var, from, to) \
    for (typeof(to) var##_to_ = (to), var = (from); var > var##_to_; --var)
#define forascby(var, from, to, by) \
    for (typeof(to) var = (from), var##_to_ = (to); var < var##_to_; var += (by))
#define fordescby(var, from, to, by) \
    for (typeof(from) var = (from), var##_to_ = (to); var > var##_to_; var -= (by))
#define for_i(times) forasc(i, 0, times)
#define for_j(times) forasc(j, 0, times)
#define for_k(times) forasc(k, 0, times)


#define with(close, var, ...) for (typeof(__VA_ARGS__) var = __VA_ARGS__; var; close(var), var = NULL)
#define withfile(var, filename, mode) with(fclose, var, fopen(filename, mode))
#define defer(...) for (bool defer_flag_##__LINE__ = 1; defer_flag_##__LINE__; defer_flag_##__LINE__ = 0, (__VA_ARGS__))


typedef struct arena { char *beg; char *end; } arena;
static inline void* alloc(arena *a, ptrdiff_t count, ptrdiff_t size, ptrdiff_t align) {
    // Nullprogram trick, quoted from his blog:
    // we can compute modulo by subtracting one and masking with AND
    // however, we want the number of bytes to advance to the next alignment, which is the inverse
    ptrdiff_t pad = (ptrdiff_t)( -(uintptr_t)a->beg & (uintptr_t)(align-1) );
    
    assert( count < (a->end - a->beg - pad)/size and "arena space left is not enough" );
    ssize_t total_size = count*size;
    
    void *r = a->beg + pad;
    a->beg += pad + total_size;

    // zero at-most 64KB to avoid mega memsets...    
    return cmemset(r, 0, min(total_size, 64*KB));
}
#define new(a, n, t) ((t *)(alloc(a, n, (ptrdiff_t)sizeof(t), alignof(t))))
#define arenaarr(arr) ({ (arena){arr, arr + countof(arr)}; })


typedef struct str { char *data; ptrdiff_t len; } str;
#define S(s) ({(str){(char *)s, countof(s)-1};})

static inline int sequal(str a, str b) {
    return (a.len != b.len) ? 0 : !cmemcmp(a.data, b.data, a.len);
}

static inline str copy(arena *a, str original) {
    str copied = original;
    copied.data = new(a, copied.len, char);
    cmemcpy(copied.data, original.data, copied.len);
    return copied;
}

static inline str cat(arena *a, str head, str tail) {
    // use copy as realoc if head is not at the top of arena
    if (!head.data or (head.data + head.len) != a->beg) {
        head = copy(a, head);
    }
    
    // since head is always at the top of arena, copy extends it
    head.len += copy(a, tail).len;
    
    return head;
}

static inline str span(char *beg, char *end) {
    return (str){beg, beg ? end - beg : 0};
}

typedef struct {str head; str tail; int ok; int padding;} head_tail_ok;
// cut s at c, returns {str head; str tail; int ok;}
static inline head_tail_ok cut(str s, char c) {
    head_tail_ok r = {0};
    if (!s.len) return r;

    char *it = s.data;
    char *end = s.data + s.len;
    for (; it < end and *it != c; ++it) {}

    r.ok = it < end;
    r.head = span(s.data, it);
    r.tail = span(it + r.ok, end);
    return r;
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

static inline str substring(str s, ssize_t i) {
    if (i > 0) {
        s.data += i;
        s.len -= i;
    }
    return s;
}

static inline int starts(str s, str prefix) {
    return (s.len >= prefix.len) \
        and sequal(span(s.data, s.data + prefix.len), prefix);
}
static inline int ends(str s, str suffix) {
    return (s.len >= suffix.len) \
        and sequal(substring(s, s.len - suffix.len), suffix);
}

static inline str scanword(arena *a) {
    static char buffer[256];
    (void)scanf(" %255s", buffer);

    ssize_t len = cstrlen(buffer);
    char *data = new(a, len + 1, char);
    cmemcpy(data, buffer, len);
    data[len] = 0;

    return (str){ data, len };
}
static inline str scanline(arena *a) {
    static char buffer[1024];
    (void)scanf(" %1023[^\n]", buffer);

    ssize_t len = cstrlen(buffer);
    char *data = new(a, len + 1, char);
    cmemcpy(data, buffer, len);
    data[len] = 0;

    return (str){ data, len };
}


static inline int char_equal(char a, char b) { return a == b; }
static inline int int_equal(int a, int b) { return a == b; }
static inline int ssize_equal(ssize_t a, ssize_t b) { return a == b; }
static inline int size_equal(size_t a, size_t b) { return a == b; }
static inline int float_equal(float a, float b)
{
	float abs_a = macro_abs(a);
	float abs_b = macro_abs(b);
	float diff  = macro_abs(a - b);

	static const float epsilon = 1e-5f;
	static const float min_abs = 1e-8f;
	if (abs_a < min_abs && abs_b < min_abs) {
		// Both numbers are extremely close to 0
		return diff < min_abs;
	}

	// Use relative comparison
	return diff <= epsilon * max(abs_a, abs_b);
}
static inline int double_equal(double a, double b)
{
	double abs_a = macro_abs(a);
    double abs_b = macro_abs(b);
	double diff  = macro_abs(a - b);

	static const double epsilon = 1e-9;
	static const double min_abs = 1e-12;

	if (abs_a < min_abs && abs_b < min_abs) {
		// Both numbers are extremely close to 0
		return diff < min_abs;
	}

	// Use relative comparison
	return diff <= epsilon * max(abs_a, abs_b);
}

// don't use char* as strings, so no equal for it!
#define equal(a, b) \
    _Generic((a), \
        char: char_equal, int: int_equal, \
        ssize_t: ssize_equal, size_t: size_equal, \
        float: float_equal, double: double_equal, \
        str: sequal \
    )(a, b)

static inline void print_char(char c) { printf("%c", c); }
static inline void print_int(int i) { printf("%d", i); }
static inline void print_ssize(ssize_t s) { printf("%zd", s); }
static inline void print_size(size_t z) { printf("%zu", z); }
static inline void print_float(float f) { printf("%.5f", (double)f); }
static inline void print_double(double d) { printf("%.9f", d); }
static inline void print_str(str s) { printf("%.*s", (int)s.len, s.data); }

#define print(a) \
    _Generic((a), \
        char: print_char, int: print_int, \
        ssize_t: print_ssize, size_t: print_size, \
        float: print_float, double: print_double, \
        str: print_str \
    )(a)
#define println(a) print(a); printf("\n")

static inline void scan_char(char *c) { (void)scanf(" %c", c); }
static inline void scan_int(int *i) { (void)scanf(" %d", i); }
static inline void scan_ssize(ssize_t *s) { (void)scanf(" %zd", s); }
static inline void scan_size(size_t *z) { (void)scanf(" %zu", z); }
static inline void scan_float(float *f) { (void)scanf(" %f", f); }
static inline void scan_double(double *d) { (void)scanf(" %lf", d); }

#define scan(a) \
    _Generic((a), \
        char: scan_char, int: scan_int, \
        ssize_t: scan_ssize, size_t: scan_size, \
        float: scan_float, double: scan_double \
    )(&a)

static inline size_t hash_str(str s, size_t seed) {
    size_t h = seed;
    for_i(s.len) {
        h ^= s.data[i] & 255;
        h *= 11111111111;
    }
    return h;
}

static inline str filestr(arena * a, str filename) {
    FILE* file = fopen(filename.data, "rb");
        if (!file) return (str){0};

        ssize_t cap = a->end - a->beg;
        str r = {a->beg, cap};
        r.len = (ssize_t)fread(r.data, 1, (size_t)r.len, file);
        assert(r.len >= 0 and r.data + r.len + 1 < a->end and "couldn't read file");
        r.data[r.len] = 0;

    fclose(file);

    return r;
}

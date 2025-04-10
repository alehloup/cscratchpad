/* Heavely inspired by Aartaka Pretty C and "C Until its no Longer C"*/
// https://aartaka.me/c-not-c


#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

extern size_t strlen(const char *);
extern int memcmp(const void *s1, const void *s2, size_t n);
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *memset(void *s, int c, size_t n);


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


#define countof(x)  ((ssize_t)(sizeof(x) / sizeof(x[0])))
#define lengthof(x) (((x) == NULL) ? 0 : (ssize_t)(strlen(x)))


#if !defined(_ISO646_H) && !defined(and)
    #define not	!
    #define and	&&
    #define or	||
#endif
#define nil NULL
#define None NULL
#define True true
#define False false


#if !defined(min) && !defined(max)
    #define min(a,b) ({ typeof(a) _a=(a); typeof(b) _b=(b); _a<_b?_a:_b; })
    #define max(a,b) ({ typeof(a) _a=(a); typeof(b) _b=(b); _a>_b?_a:_b; })
#endif
#define between(lo, n, hi) \
    ({ typeof(n) _n = (n); typeof(lo) _lo = (lo); typeof(hi) _hi = (hi); _n >= _lo && _n <= _hi; })
#define limit(lo, n, hi) \
    ({ typeof(n) _n = (n); typeof(lo) _lo = (lo); typeof(hi) _hi = (hi); _n < _lo ? _lo : _n > _hi ? _hi : _n; })
#define clamp(lo, n, hi)  limit(lo, n, hi)


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
#define foreach(var, array, length) \
    for (typeof((array)[0]) *var = (array), *var##_end_ = (array) + (length); var < var##_end_; ++var)
#define for_i(times) forasc(i, 0, times)
#define for_j(times) forasc(j, 0, times)
#define for_k(times) forasc(k, 0, times)


#define with(close, var, ...) for (typeof(__VA_ARGS__) var = __VA_ARGS__; var; close(var), var = NULL)
#define withfile(var, filename, mode) with(fclose, var, fopen(filename, mode))
#define defer(...) for (bool defer_flag_##__LINE__ = 1; defer_flag_##__LINE__; defer_flag_##__LINE__ = 0, (__VA_ARGS__))



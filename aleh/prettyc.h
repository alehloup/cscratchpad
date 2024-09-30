#pragma once

/* Heavely inspired by Aartaka Pretty C and "C Until its no Longer C"*/
// https://aartaka.me/c-not-c

#ifdef __cplusplus
extern "C" { /* Cancels Name Mangling when compiled as C++ */
#endif

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#if !defined(__cplusplus) && !defined(_ISO646_H)
    #ifndef and
    #define and	&&
    #define and_eq	&=
    #define bitand	&
    #define bitor	|
    #define compl	~
    #define not	!
    #define not_eq	!=
    #define or	||
    #define or_eq	|=
    #define xor	^
    #define xor_eq	^=
    #endif
#endif
#define eq ==
#define is ==
#define isnt !=
#define bitnot ~
#define bitxor ^

typedef char*  str;
typedef char   byte;
typedef char*  bytes;
typedef void*  any;
typedef int64_t i64;

#define nil NULL
#define None NULL
#define True true
#define False false

#if !defined(min) && !defined(max)
    #define min(x, ...)        ((x) < (__VA_ARGS__) ? (x) : (__VA_ARGS__))
    #define max(x, ...)        ((x) > (__VA_ARGS__) ? (x) : (__VA_ARGS__))
#endif
#define divisible(n, ...)  ((__VA_ARGS__ == 0) ? 0 : ((n) % (__VA_ARGS__) == 0))
#define ensure(x, ...)     ((x) ? (x) : (__VA_ARGS__))
#define limit(lo, n, hi)   ((n) < (lo) ? (lo) : (n) > (hi) ? (hi) : (n))
#define clamp(lo, n, hi)   limit(lo, n, hi)
#define between(lo, n, hi) ((n) <= (hi) && (n) >= (lo))

#define even               0 == 1 &
#define odd                1 == 1 &
#define positive           0 <
#define negative           0 >
#define zero               0 ==
#define empty              NULL ==
#define null               NULL ==

#define fortimes(var, times) for (int var = 0; var < times; ++var)
#define forasc(var, from, to) for (int var = from; var < to; ++var)
#define fordesc(var, from, to) for (int var = from; var > to; --var)
#define forascby(var, from, to, by) for (int var = from; var < to; var+=by)
#define fordescby(var, from, to, by) for (int var = from; var > to; var-=by)
#define foreach(var, type, length, array) for (type *var = array, *prettyc_end##__LINE__ = &array[length]; var < prettyc_end##__LINE__; ++var)

#define withfile(var, filename, mode) for (FILE* var = fopen(filename, mode), *prettyc_flag_##__LINE__ = (FILE*)1; prettyc_flag_##__LINE__; fclose(var), prettyc_flag_##__LINE__ = NULL)
#define defer(...) for (bool prettyc_flag_##__LINE__ = 1; prettyc_flag_##__LINE__; prettyc_flag_##__LINE__ = 0, (__VA_ARGS__))

#define equal_array(array_a, array_b) (sizeof(array_a) == sizeof(array_b) ? !memcmp(array_a, array_b, sizeof(array_a)) : 0)
#define equal_pstruct(pstruct_a, pstruct_b) (sizeof(*pstruct_a) == sizeof(*pstruct_b) ? !memcmp(pstruct_a, pstruct_b, sizeof(*pstruct_a)) : 0)

#ifdef __cplusplus
} /* closes extern C, Cancels Name Mangling when compiled as C++ */
#endif

#ifdef __cplusplus
    #define ZEROINIT {}
    #define threadlocal thread_local
    #define atleast /* static */

    #include <type_traits>
    //len
    static constexpr size_t len(char* ptr) {
        return ptr == nullptr ? 0 : strlen(ptr);
    }
    template <typename T, size_t N> constexpr size_t len(T (&)[N]) {
        return N;
    }

    //equal
    template<typename T> bool equal(T a, T b) {
        return a == b;
    }
    template<> bool equal<float>(float a, float b) {
        return fabs(a - b) < FLT_EPSILON;
    }
    template<> bool equal<double>(double a, double b) {
        return fabs(a - b) < DBL_EPSILON;
    }
    template<> bool equal<long double>(long double a, long double b) {
        return fabs(a - b) < LDBL_EPSILON;
    }
    template<> bool equal<const char*>(const char* a, const char* b) {
        return strcmp(a, b) == 0;
    }
    template<> bool equal<char*>(char* a, char* b) {
        return strcmp(a, b) == 0;
    }
    template<typename T> bool equal(T* a, T* b) {
        return a == b;
    }
#else // Pure C
    #define ZEROINIT {0}
    #define threadlocal _Thread_local

    #if defined(__clang__) || defined(__GNUC__)
        #define atleast static
    #else
        #define atleast /* static */
    #endif 
    
    //len
    #define len(...) \
        ((__VA_ARGS__) == NULL) ? 0                  \
        : _Generic((__VA_ARGS__),                    \
        char const *: (strlen((const char*)(__VA_ARGS__))),    \
        char *: (strlen((char*)(__VA_ARGS__))),          \
        default: (sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__[0])))
    
    //equal
    static inline int prettyc_char_equal(char a, char b) { return a == b; }
    static inline int prettyc_short_equal(short a, short b) { return a == b; }
    static inline int prettyc_int_equal(int a, int b) { return a == b; }
    static inline int prettyc_longlong_equal(long long a, long long b) { return a == b; }
    static inline int prettyc_sizet_equal(size_t a, size_t b) { return a == b; }
    static inline int prettyc_float_equal(float a, float b) { return fabs((double)a - (double)b) < (double)FLT_EPSILON; }
    static inline int prettyc_double_equal(double a, double b) { return fabs(a - b) < DBL_EPSILON; }
    static inline int prettyc_literal_equal(const char *a, const char *b) { return !strcmp(a, b); }
    static inline int prettyc_string_equal(char *a, char *b) { return !strcmp(a, b); }
    static inline int prettyc_anything_equal(void *a, void *b) { return a == b; }
    //
    #define equal(a, ...)                           \
        _Generic((__VA_ARGS__),                     \
            char: prettyc_char_equal,               \
            short: prettyc_short_equal,             \
            int: prettyc_int_equal,                 \
            long long: prettyc_longlong_equal,      \
            size_t: prettyc_sizet_equal,            \
            float: prettyc_float_equal,             \
            double: prettyc_double_equal,           \
            char *: prettyc_string_equal,           \
            char const *: prettyc_literal_equal,    \
            default: prettyc_anything_equal)        \
        (a, __VA_ARGS__)
    #define like(a, ...) equal(a, __VA_ARGS__)

#endif

#pragma once
#ifdef __cplusplus
extern "C" { /* Cancels Name Mangling when compiled as C++ */
#endif


#if defined(_WIN32) || defined(_WIN64)
    #if defined(_MSC_VER)
        #pragma warning(disable: 28301) 
        #pragma warning(disable: 28251)
    #endif
    #include <intrin.h>
#endif
#include <math.h>
#include <stdlib.h>


static inline size_t power2_number_mod(size_t power2_number, size_t modval) { return (power2_number) & (modval - 1); }

static inline size_t greatest_common_divisor(size_t m, size_t n) {
    size_t tmp = 0;
    while (m) { tmp = m; m = n % m; n = tmp; }       
    return n;
}
static inline size_t least_common_multiple(size_t m, size_t n) { return m / greatest_common_divisor(m, n) * n; }

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


#ifdef __cplusplus
} /* closes extern C, Cancels Name Mangling when compiled as C++ */
#endif

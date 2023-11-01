#pragma once

//better assert
#define assert(c)  while (!(c)) __builtin_unreachable()

//better type names
typedef long long i64; typedef long i32; typedef short i16; typedef signed char i8;
typedef unsigned long long u64; typedef unsigned long u32; typedef unsigned short u16; typedef unsigned char u8;
typedef long long isize; typedef unsigned long long usize;
typedef float f32; typedef double f64;
typedef i32 b32; //boolean

//better names for size operations
#define sizeof(x)       ((isize)sizeof(x))
#define alignof(x)      ((isize)_Alignof(x))
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define cstrlengthof(s) (countof(s) - 1)

//shortcut for typedef struct that allows recursion
#define tstruct(name) typedef struct name name; struct name

//for shortcuts
#define loop(var, times) for(isize var = 0; var < times; ++var)
#define fori(times) loop(i, times)
#define forj(times) loop(j, times)
#define for_k(times) loop(k, times)
#define forrange(var, from, to, inc) for(isize var = from; var != to; var+=inc)

//better static strings -- cstrlength |does not work| in dynamic strings
tstruct(s8){ isize len; u8 *data; };
#define s8(s) (s8){ cstrlengthof(s), (u8 *)s }

//TRICK scope that "opens" at start, and "closes" at end (careful, if returns mid scope |end| will never run)
int MACRO_scoped__;
#define scoped(start, end) MACRO_scoped__ = 1;for(start; MACRO_scoped__; (--MACRO_scoped__), end)

//Fast % when the number is a power of 2
#define MODPWR2(number, modval) ((number) & (modval - 1))

//One liner Pseudo Random generator
static u64 MACRO_rnd64_seed__;
#define RNDSEED(x) ((MACRO_rnd64_seed__) = (u64)(x) >> 1)
#define RND64() ((MACRO_rnd64_seed__) = ((MACRO_rnd64_seed__) * 0x3FFFFBFFFFF + 0x7A5662DCDF) >> 1)
#define RNDN(n) (RND64() % (n))

//Implement memset to zero as a macro
static isize MACRO_zeromem_len__;
static u8 *MACRO_zeromem_ptr__;
#define ZEROMEM(dest, len)         \
    MACRO_zeromem_ptr__ = (dest);  \
    MACRO_zeromem_len__ = (len);   \
    while(MACRO_zeromem_len__-->0) \
        *MACRO_zeromem_ptr__++ = 0;\
//end of memset macro

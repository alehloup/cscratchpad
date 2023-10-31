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

//better static strings -- cstrlength |does not work| in dynamic strings
tstruct(s8){ isize len; u8 *data; };
#define s8(s) (s8){ cstrlengthof(s), (u8 *)s }

//TRICK scope that "opens" at start, and "closes" at end (careful, if returns mid scope |end| will never run)
int MACRO_scoped__;
#define scoped(start, end) MACRO_scoped__ = 1;for (start; MACRO_scoped__; (--MACRO_scoped__), end)

//Fast % when the number is a power of 2
#define MODPWR2(number) ((number) & (align - 1))

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

//for shortcuts
#define loop(var, times) for(isize var = 0; var < times; ++var)
#define fori(times) loop(i, times)
#define forj(times) loop(j, times)
#define for_k(times) loop(k, times)
#define forrange(var, from, to, inc)          \
    for(isize var = (from), var##_TO__ = (to);\
        var != var##_TO__; var+=inc)          \
//end of forrange

//better static strings -- cstrlength |does not work| in dynamic strings
typedef struct s8{ isize len; u8 *data; }s8;
#define s8(s) (s8){ cstrlengthof(s), (u8 *)s }

//TRICK scope that "opens" at start, and "closes" at end (careful, if returns mid scope |end| will never run)
int MACRO_scoped__;
#define scoped(start, end) MACRO_scoped__ = 1;         \
    for(start; MACRO_scoped__; (--MACRO_scoped__), end)\
//end of scoped

//Fast % when the number is a power of 2
#define MODPWR2(number, modval) ((number) & (modval - 1))

//One liner Pseudo Random generator
static __thread u64 MACRO_rnd64_seed__;
#define RNDSEED(x) ((MACRO_rnd64_seed__) = (u64)(x) >> 1)
#define RND64() ((MACRO_rnd64_seed__) = ((MACRO_rnd64_seed__) * 0x3FFFFBFFFFF + 0x7A5662DCDF) >> 1)
#define RNDN(n) (RND64() % (n))

/*
    ARENA defs and operations that do not do memory allocation
*/

typedef struct arena{ u8 *beg; u8 *end; }arena;

//Implement mem set to zero as a macro
static __thread isize MACRO_zeromem_len__;
static __thread u8 *MACRO_zeromem_ptr__;
#define ZEROMEM(dest, len)         \
    MACRO_zeromem_ptr__ = (dest);  \
    MACRO_zeromem_len__ = (len);   \
    while(MACRO_zeromem_len__-->0) \
        *MACRO_zeromem_ptr__++ = 0;\
//end of memset macro

__attribute((malloc, alloc_size(2, 4), alloc_align(3)))
void *alloc(arena *a, isize size, isize align, isize count) {
    isize total = size * count;
    isize pad = MODPWR2(- (isize)a->beg, align); //mod -x gives n for next align
    
    if (count > (a->end - a->beg - pad)/size) {
        assert(count <= (a->end - a->beg - pad)/size); //ARENA OUT OF MEMORY
        return 0; //ARENA OUT OF MEMORY
    }

    u8 *p = a->beg + pad;
    a->beg += pad + total;
    
    return ZEROMEM(p, total);
}

#define newx(a, t) (t *)alloc(a, sizeof(t), alignof(t), 1)
#define newxs(a, t, n) (t *)alloc(a, sizeof(t), alignof(t), n)

/*
    ARRAY defs and operations
*/

//dynarr have their typename as type##s, i.e: i64s
#define def_dynarr(typ)          \
    typedef struct typ##s{       \
        isize len; isize cap;    \
        isize start; b32 invalid;\
        typ *data;               \
    }typ##s                      \
//end of def_dynarr  

//statarr have their typename as type##x##count, i.e: i64x10
#define def_statarr(typ, count)                 \
    typedef struct typ##x##count{               \
        isize len; isize cap;                   \
        isize start; b32 invalid;               \
        typ data[(count)];                      \
    }typ##x##count                              \
//end of def_statarr

#define foridx(var, array) forrange(var, array.start, array.start + array.len, 1)
#define isstaticarr(a)  (countof(a.data) > 8) //u8 dynarr count == ptr_size / 8 == 8

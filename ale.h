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


#pragma once

// Clang -wEverything bans C Arrays and Pointers, which is insane. disabling that:
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif

typedef unsigned long long ulong;

/* STRUCTS */

// char *data; ulong len;
typedef struct str { char *data; ulong len; } str;

// str head; str tail; int ok;
typedef struct head_tail_ok { str head; str tail; int ok; int padding; } head_tail_ok;

enum { MSIEXP = 11 }; // configure hash table size, 2^11 = 2048
// const char * keys[2<<MSIEXP]; int idxs[2<<MSIEXP];
typedef struct htindex { const char * keys[2<<MSIEXP]; int idxs[2<<MSIEXP]; } htindex;


/* MACROS */

#if !defined(min) && !defined(max)
    #define min(a,b) ( (a) < (b) ? (a) : (b) )
    #define max(a,b) ( (a) > (b) ? (a) : (b) )
#endif

#define countof(x)  ( sizeof(x) / sizeof(x[0]) )


/* MEMORY */

static inline ulong cstrlen(const char *s) {
    const char *p = s;
    while (*p) ++p;
    return (ulong)(p - s);
}

static inline int cstrcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        ++s1;
        ++s2;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}

static inline void *cmemchar(const void *s, int c, ulong n) {
    const unsigned char *p = (const unsigned char *)s;
    unsigned char uc = (unsigned char)c;

    for (ulong i = 0; i < n; ++i) {
        if (p[i] == uc)
            return (void *)(p + i);
    }
    return 0;
}

static inline int cmemcmp(const void *s1, const void *s2, ulong n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    for (ulong i = 0; i < n; ++i) {
        if (p1[i] != p2[i])
            return (int)p1[i] - (int)p2[i];
    }
    return 0;
}

static inline void *cmemcpy(void *dst, const void *src, ulong n) {
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;

    for (ulong i = 0; i < n; ++i)
        d[i] = s[i];

    return dst;
}

static inline void *cmemset(void *s, int c, ulong n) {
    unsigned char *p = (unsigned char *)s;
    unsigned char uc = (unsigned char)c;

    for (ulong i = 0; i < n; ++i)
        p[i] = uc;

    return s;
}


/* STRING */

// creates a str { data: s, len: cstrlen(s) }
static inline S(const char * s) {
    str s = { (char *)(ulong) s, cstrlen(s) };
    return s;
}

// creates a str { data: beg, len: (end - beg) }
static inline str span(const char *beg, const char *end) {
    str s = { (char *)(ulong) beg, beg && end > beg ? end - beg : 0 };
    return s;
}

// appends src from len onwards into dst, does not add \0 to allow repeatedly appends
static inline void cstrappend(char * dst, ulong *len, const ulong cap, const char * src)
{
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;

    ulong i = *len;
    for (; i < cap && s[i]; ++i)
        d[i] = s[i];
    *len = i;
}

// glues the strings in srcs into dst, adding the string separator between them
static inline ulong cstrjoin(char * dst, const ulong dst_cap, const char * srcs[2], ulong srcs_len, const char * separator)
{
    ulong dst_len = 0;

    if (srcs_len == 0) 
        srcs_len = 1;

    --srcs_len; // to take out the last iteration

    for(int i = 0; i < srcs_len; ++i) {
        cstrappend(dst, srcs[i], &dst_len, dst_cap);
        cstrappend(dst, separator, &dst_len, dst_cap);
    }
    
    cstrappend(dst, srcs[srcs_len], &dst_len, dst_cap);
    // no separator after last iteration

    if (dst_len < dst_cap) {
        dst[dst_len++] = '\0';
    } else {
        dst[dst_cap - 1] = '\0';
    }

    return dst_len;
}

// cut s at c, returns {str head; str tail; int ok;}
static inline head_tail_ok cut(str s, char c)
{
    head_tail_ok r = {0};
    char *end = 0, *it = 0;

    if (!s.len) return r;

    end = s.data + s.len;

    it = cmemchr(s.data, c, s.len);
    if (!it) it = end;

    r.ok = it < end;
    r.head = span(s.data, it);
    r.tail = span(it + r.ok, end);

    return r;
}

#define forsep(var, string, sep) \
    head_tail_ok var##_res = cut( (string) , sep); \
    for (str var = var##_res.head; \
        var##_res.head.len || var##_res.tail.len; \
        var##_res = cut(var##_res.tail, sep), var = var##_res.head)

#define forlines(var, string) forsep(var, string, '\n')

// splits string into arr, returns number of elements splited into
static inline ulong split(str text, char sep, str arr[1], ulong cap)
{
    ulong len = 0;

    forsep(part, text, sep) {
        if (len >= cap) break;

        arr[len++] = part;
    }

    return len;
}


static inline int parseint(str s)
{
    unsigned int r = 0;
    int sign = 1;

    for(char *pc = s.data; pc < s.data + s.len; ++pc) {
        switch(*pc) {
            case '+': case '.': case ',': case '_': break;
            case '-': sign *= -1; break;
            default: r = 10u*r + (unsigned)(*pc - '0');
        }
    }

    return sign * (int)r;
}

static inline float parsefloat(str s)
{
    float r = 0.0f;
    float sign = 1.0f;
    float exp = 0.0f;

    for(char *pc = s.data; pc < s.data + s.len; ++pc) {
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


/* RANDOM */

static unsigned int random32_max_uint = ((1U << (sizeof(unsigned int) * 8)) - 1);

static ulong random32_state = 1111111111111111111;

static inline unsigned int random32()
{
    unsigned int xorshifted = 0, rot = 0;
    ulong oldstate = random32_state;
    random32_state = oldstate * 6364136223846793005ULL + 1111111111111111111ULL;
    
    xorshifted = (unsigned int)(((oldstate >> 18u) ^ oldstate) >> 27u);
    rot = (unsigned int)(oldstate >> 59u);
    
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static inline unsigned int rando(unsigned int from_including, unsigned int to_including)
{
    unsigned int range = to_including - from_including + 1;

    // Exclude last % range cycle to eliminate mod bias
    unsigned int limit = random32_max_uint - (random32_max_uint % range);
    unsigned int r;

    if (to_including < from_including) {
        unsigned int temp = to_including;
        to_including = from_including;
        from_including = temp;
    }
        return from_including;

    do {
        r = random32();
    } while (r >= limit);  // Rejects if its in the last % range cycle

    return (r % range) + from_including;
}


/* HASH */

static inline ulong hash_cstr(const char * s, ulong seed)
{
    ulong h = seed;
    for(int i = 0; s[i]; ++i) {
        h ^= s[i] & 255;
        h *= 1111111111111111111; // intentional uint overflow to spread bits
    }

    h += !h;
    return h;
}

static inline ulong hash_num(ulong x)
{
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9U;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebU;
    x ^= x >> 31;

    x += !x;
    return x;
}


/* HASHTABLE INDEX */


static inline int lookup(const char *key, htindex *table, int create_if_not_found)
{
    ulong hash = hash_cstr(key, (ulong) table);
    
    unsigned int mask = (1<<MSIEXP) - 1;
    unsigned int step = (unsigned int) ( (hash >> ( (sizeof(ulong)*8) - MSIEXP ) ) | 1 );
    unsigned int i = (unsigned int)hash;

    for(i = (i + step) & mask; /* forever */ ; i = (i + step) & mask) {
        const char *k = table->keys[i];
        if (i == 0) continue;

        if (!k) {
            if (create_if_not_found) {
                table->keys[i] = key;
                return table->idxs[i] = ++table->idxs[0];
            } else {
                return 0;
            }
        } else if (!cstrcmp(key, k)) {
            return table->idxs[i];
        }
    }
}

// return htindex table len, always stored at table->idxs[0]
static inline int htablelen(htindex *table) {
    return table->idxs[0];
}

// gets idx or return 0 if not found
static inline int msiget(const char *key, htindex *table) {
    return lookup(key, table, 0);
}

// create key if not exists, always return idx
static inline msiins(const char *key, htindex *table) {
    return lookup(key, table, 1);
}


/* PRINT */

#define println(...) printf(__VA_ARGS__); printf("\n")
#define printarr(format_specifier, arr, n) printf("{ "); for(int i = 0; i < n; ++i) { printf(format_specifier " ", arr[i]); } println("}")

#define print_stopwatch(stopwatch) \
    printf("  Executed in %.3f seconds\n", (double)(clock() - stopwatch) / (double)CLOCKS_PER_SEC)


/* LIBC SORT */

#define decl_cmpfn(fnname, type, ... ) \
    static inline int fnname(const void *a_, const void *b_) { const type *a = a_; const type *b = b_;  return (__VA_ARGS__); }

#define sort(arr, n, cmpfn) qsort(arr, n, sizeof(arr[0]), cmpfn)

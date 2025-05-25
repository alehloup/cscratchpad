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

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* OS INCLUDES & COMPAT */
#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <Windows.h>
    #include <io.h>

    typedef HANDLE THREAD;
    #define threadfun_ret DWORD WINAPI
    typedef DWORD (WINAPI *threadfun_)(void *);
#else // assume Posix
    #include <unistd.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <pthread.h>

    typedef pthread_t THREAD;
    #define threadfun_ret void *
    typedef void * (*threadfun_)(void *);
#endif

// Clang -wEverything bans C Arrays and Pointers, which is insane. disabling that:
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif


/* STRUCTS */

// char *data; size_t len;
typedef struct str { char *data; size_t len; } str;

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

static inline size_t cstrlen(const char *s) {
    const char *p = s;
    while (*p) ++p;
    return (size_t)(p - s);
}

static inline int cstrcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        ++s1;
        ++s2;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}

static inline void *cmemchar(const void *s, int c, size_t n) {
    const unsigned char *p = (const unsigned char *)s;
    unsigned char uc = (unsigned char)c;

    for (size_t i = 0; i < n; ++i) {
        if (p[i] == uc)
            return (void *)(p + i);
    }
    return NULL;
}

static inline int cmemcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    for (size_t i = 0; i < n; ++i) {
        if (p1[i] != p2[i])
            return (int)p1[i] - (int)p2[i];
    }
    return 0;
}

static inline void *cmemcpy(void *dst, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;

    for (size_t i = 0; i < n; ++i)
        d[i] = s[i];

    return dst;
}

static inline void *cmemset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    unsigned char uc = (unsigned char)c;

    for (size_t i = 0; i < n; ++i)
        p[i] = uc;

    return s;
}


/* STRING */

// creates a str { data: s, len: cstrlen(s) }
static inline S(const char * s) {
    str s = { (char *)(uintptr_t) s, cstrlen(s) };
    return s;
}

// creates a str { data: beg, len: (end - beg) }
static inline str span(const char *beg, const char *end) {
    str s = { (char *)(uintptr_t) beg, beg && end > beg ? end - beg : 0 };
    return s;
}

// appends src from len onwards into dst, does not add \0 to allow repeatedly appends
static inline void cstrappend(char * dst, size_t *len, const size_t cap, const char * src)
{
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;

    size_t i = *len;
    for (; i < cap && s[i]; ++i)
        d[i] = s[i];
    *len = i;
}

// glues the strings in srcs into dst, adding the string separator between them
static inline size_t cstrjoin(char * dst, const size_t dst_cap, const char * srcs[2], size_t srcs_len, const char * separator)
{
    size_t dst_len = 0;
    srcs_len = srcs_len > 0 ? srcs_len - 1 : srcs_len;  

    for(int i = 0; i < srcs_len; ++i) {
        cstrappend(dst, srcs[i], &dst_len, dst_cap);
        cstrappend(dst, separator, &dst_len, dst_cap);
    }
    cstrappend(dst, srcs[i], &dst_len, dst_cap);

    assert(dst_len < dst_cap);
    dst[dst_len++] = '\0';

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
static inline size_t split(str text, char sep, str arr[1], size_t cap)
{
    size_t len = 0;

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

static inline unsigned int random32(size_t *state)
{
    unsigned int xorshifted = 0, rot = 0;
    size_t oldstate = *state;
    *state = oldstate * 6364136223846793005ULL + 1111111111111111111ULL;
    
    xorshifted = (unsigned int)(((oldstate >> 18u) ^ oldstate) >> 27u);
    rot = (unsigned int)(oldstate >> 59u);
    
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}
static inline unsigned int rando(size_t *state, unsigned int from_including, unsigned int to_including)
{
    unsigned int range = to_including - from_including + 1;

    // Exclude last % range cycle to eliminate mod bias
    unsigned int limit = UINT32_MAX - (UINT32_MAX % range);
    unsigned int r;

    assert(from_including <= to_including && "invalid range for random!");
    if (to_including < from_including) return from_including;

    do {
        r = random32(state);
    } while (r >= limit);  // Rejects if its in the last % range cycle

    return (r % range) + from_including;
}


/* HASH */

static inline size_t hash_cstr(const char * s, size_t seed)
{
    size_t h = seed;
    for(int i = 0; s[i]; ++i) {
        h ^= s[i] & 255;
        h *= 1111111111111111111; // intentional uint overflow to spread bits
    }

    h += !h;
    return h;
}
static inline size_t hash_num(size_t x)
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
    size_t hash = hash_cstr(key, (uintptr_t) table);
    
    unsigned int mask = (1<<MSIEXP) - 1;
    unsigned int step = (unsigned int) ( (hash >> ( (sizeof(size_t)*8) - MSIEXP ) ) | 1 );
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

static inline void print_stopwatch(clock_t stopwatch)
{
    clock_t end = clock();
    if (end < stopwatch) {
        printf("Clock error, end < start");
        return;
    }
    printf("  Executed in %.3f seconds\n", (double)(clock() - stopwatch) / (double)CLOCKS_PER_SEC);
}

/* LIBC SORT */

#define decl_cmpfn(fnname, type, ... ) \
    static inline int fnname(const void *a_, const void *b_) { const type *a = a_; const type *b = b_;  return (__VA_ARGS__); }

#define sort(arr, n, cmpfn) qsort(arr, n, sizeof(arr[0]), cmpfn)


/* FILES */

static inline const char * ensure_binary_mode(const char * cstring)
{
    size_t len = cstrlen(cstring);
    assert(len > 0 && len < 4);

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

static inline str file2str(arena * a, const char *filename)
{
    str r = {a->beg, 0};

    for(FILE *file = fopen(filename, "rb"); file; fclose(file), file = NULL) {
        r.len = (size_t)fread(r.data, 1, (size_t)(a->end - a->beg), file);

        if (r.len < 0 || r.data + r.len + 1 >= a->end) {
            r.len = 0;
        } else {
            a->beg += r.len + 1;
            r.data[r.len] = '\0';
        }
    }

    return r;
}
static inline int str2file(str content, const char *filename)
{
    size_t written = 0;

    for(FILE* file = fopen(filename, "wb"); file; fclose(file), file = NULL) {
        written = fwrite(content.data, 1, (size_t)content.len, file);
    }

    return written != (size_t)content.len;
}

static inline size_t filelen(FILE *file)
{
    #ifdef _WIN32
        size_t len = (size_t)_filelengthi64(_fileno(file));
    #else // assume POSIX
        struct stat file_stat;
        int fstat_success = fstat(fileno(file), &file_stat) != -1;
        size_t len = ((void)assert(fstat_success), (size_t)file_stat.st_size);

        (void) fstat_success;
    #endif 

    assert(len >= 0 && "filelen must not be negative!");
    return len;
}

static inline str mopen(const char *filename, const char *mode)
{
    int readit = mode[0] == 'r' && mode[1] != '+';
    str r = {0};

    for(FILE *file = fopen(filename, ensure_binary_mode(mode)); file; fclose(file), file = NULL) {
        size_t len = filelen(file);

        #ifdef _WIN32
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
            r.data = (char *)str(
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

static inline void mclose(str s)
{
    if (!s.len || !s.data) return;

    #ifdef _WIN32
        UnmapViewOfFile(s.data);
    #else
        munmap(s.data, (size_t)s.len);
    #endif
}


/* THREADS */

static inline THREAD go(threadfun_ threadfun, void * threadarg, size_t thread_stack_size) 
{
    THREAD thread;
    size_t thread_stack_size_adjusted = (size_t)max(thread_stack_size, 16*KB);
    int error = 0;

    #ifdef _WIN32
        thread = CreateThread(0, thread_stack_size_adjusted, threadfun, threadarg, 0, 0);
        assert(thread && "fatal error: could not launch thread");
    #else // assume POSIX
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setstacksize(&attr, thread_stack_size_adjusted);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        error = pthread_create(&thread, &attr, threadfun, threadarg);
        assert(!error && "fatal error: could not launch thread");
        pthread_attr_destroy(&attr);
    #endif

    (void) error;

    return thread;
}

static inline void join_thread(THREAD thread)
{
    #ifdef _WIN32
        WaitForSingleObject(thread, INFINITE);
    #else // assume POSIX
        pthread_join(thread, 0);
    #endif
}


/* OS FUNCTIONS */

static inline int scmd(arena *a, str command)
{
    const char *cstr_command = str2cstr(a, command);
    int ret = 0;

    print_str(command); printf("\n");

    ret = system(cstr_command); printf("\n");

    return ret;
}

static inline void sleepsecs(unsigned int seconds)
{
    assert(seconds < 60 * 60 * 24 + 1);

    #if defined(_WIN32)
        Sleep(seconds * 1000);
    #else // Unix
        sleep(seconds);
    #endif
}

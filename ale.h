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
//  Chris Nullprogram Essays, like "Examples of quick hash tables and dynamic arrays in C" 
//      https://nullprogram.com/blog/2025/01/19/
//  Aartaka Essays, like "C Until its no Longer C"
//      https://aartaka.me/c-not-c


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
    #include <windows.h>
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


/* STRUCTS */

// char *beg; char *end;
typedef struct arena { char *beg; char *end; } arena;

// char *data; ptrdiff_t len;
typedef struct str { char *data; ptrdiff_t len; } str;

// str head; str tail; int ok;
typedef struct head_tail_ok { str head; str tail; int ok; int padding; } head_tail_ok;

enum { MSIEXP = 11 }; // configure hash table size, 2^11 = 2048
// const char * keys[2<<MSIEXP]; int idxs[2<<MSIEXP];
typedef struct htindex { const char * keys[2<<MSIEXP]; int idxs[2<<MSIEXP]; } htindex;

// for a bit more clarity, since mopen saves the mapped buffer to a str struct
typedef str MMAP;


/* CONSTANTS */

#define KB (1024LL)
#define MB (1024LL * 1024LL)
#define GB (1024LL * 1024LL * 1024LL)


/* MACROS */

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

#if !defined(min) && !defined(max)
    #define min(a,b) ( (a) < (b) ? (a) : (b) )
    #define max(a,b) ( (a) > (b) ? (a) : (b) )
#endif

#define countof(x)  ( (ptrdiff_t)sizeof(x) / (ptrdiff_t)sizeof(x[0]) )


/* MEMORY */

extern size_t strlen(const char *);
static inline ptrdiff_t cstrlen(const char *s) { return !s ? 0 : (ptrdiff_t)strlen(s); }

extern int strcmp(const char *s1, const char *s2);
static inline int cstrcmp(const char *s1, const char *s2)
{
    if (!s1 || !s2) 
        return !(s1 == NULL && s2 == NULL);

    return strcmp(s1, s2);
}

extern void * memchr(const void *s, int c, size_t n);
static inline void * cmemchr(const void *s, char c, ptrdiff_t n)
{
    assert(n >= 0 && "cmemchr: n can't be negative!");
    return !n ? NULL : memchr(s, c & 255, (size_t)n);
}

extern int memcmp(const void *s1, const void *s2, size_t n);
static inline int cmemcmp(const void *s1, const void *s2, ptrdiff_t n)
{
    assert(n >= 0 && "cmemcmp: n can't be negative!");
    return !n ? 0 : memcmp(s1, s2, (size_t)n);
}

extern void * memcpy(void *dest, const void *src, size_t n);
static inline void * cmemcpy(void *dest, const void *src, ptrdiff_t n)
{
    assert(n >= 0 && "cmemcpy: n can't be negative!");
    return n ? memcpy(dest, src, (size_t)n) : dest;
}

extern void * memset(void *s, int c, size_t n);
static inline void * cmemset(void *s, int c, ptrdiff_t n)
{
    assert(n >= 0 && "cmemset: n can't be negative!");
    return n ? memset(s, c, (size_t)n) : s;
}


/* ARENA */

static inline void * alloc(arena *a, ptrdiff_t count, ptrdiff_t size, ptrdiff_t align)
{
    assert(count >= 0 &&  size >= 0 && "count and size can't be negative!");
    if (!count || !size) return a->beg; // alloc 0 elements or n elements of size 0 is a no-op

    // Nullprogram trick, quoted from his blog:
    // we can compute modulo by subtracting one and masking with AND
    // however, we want the number of bytes to advance to the next alignment, which is the inverse
    ptrdiff_t pad = (ptrdiff_t)( -(uintptr_t)a->beg & (uintptr_t)(align-1) );

    // Explicit Integer Overflow check in the assert
    assert( count < (a->end - a->beg - pad)/size && "arena space left is not enough" );
    ptrdiff_t total_size = count*size;
    
    void *r = a->beg + pad;
    a->beg += pad + total_size;

    return cmemset(r, 0, total_size);
}
#define new(parena, n, t) ((t *)(alloc(parena, n, (ptrdiff_t) sizeof(t), (ptrdiff_t) alignof(t))))
#define arr2arena(arr) ( (arena){(char *) arr, (char *) (arr + countof(arr))} )


/* STRING */

#define S(s) ( (str){ (char *) s, max(((ptrdiff_t)sizeof(s)) - 1, 0) } )

#define cstr2str(cstring) ((str){ (char *)cstring, cstrlen(cstring) })

static inline str scopy(arena *a, str original)
{
    str copied = original;
    copied.data = new(a, copied.len, char);
    cmemcpy(copied.data, original.data, copied.len);
    return copied;
}

// Converts a str to a null-terminated char* for use with legacy C APIs
// BEWARE: char * is only valid while the arena keeps its data!
static inline char * str2cstr(arena *a, str string)
{
    if ((string.data + string.len) != a->beg)
        string = scopy(a, string);

    *(a->beg++) = '\0';

    return string.data;
}

static inline int sequal(str a, str b) { return (a.len != b.len) ? 0 : !cmemcmp(a.data, b.data, a.len); }

static inline int scomp(str a, str b)
{
    int mincomp = cmemcmp(a.data, b.data, min(a.len, b.len));
    return !mincomp ? (int)(a.len - b.len) : mincomp;
}

// cat always starts by putting str head at the top of the arena
static inline str cat(arena *a, str head, str tail)
{
    // use copy as realoc if head is not at the top of arena
    if (!head.data || (head.data + head.len) != a->beg) {
        head = scopy(a, head);
    }
    
    // since head is always at the top of arena, copy extends it
    head.len += scopy(a, tail).len;
    
    return head;
}
static inline str sjoin(arena *a, str *arr, ptrdiff_t len, char separator)
{
    str res = {0};
    char *psep = &separator;
    if (!a || !arr) return res;

    if (!separator) separator = ' ';

    for (str *s = arr; s < arr + len; ++s) {
        res = cat(a, res, *s);
        psep = new(a, 1, char);
        *psep = separator;
    }
    *psep = '\0'; //makes res.data compatible to null terminated c strings

    return res;
}

static inline str span(char *beg, char *end) { return (str){ beg, beg ? max(end - beg, 0) : 0 }; }

// cut s at c, returns {str head; str tail; int ok;}
static inline head_tail_ok cut(str s, char c)
{
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
         var##_res.head.len || var##_res.tail.len; \
         var##_res = cut(var##_res.tail, sep), var = var##_res.head)

#define forlines(var, string) forsep(var, string, '\n')

// splits string into arr, returns number of elements splited into
static inline ptrdiff_t split(str text, char sep, str arr[1], ptrdiff_t cap)
{
    assert(cap > 0 && "cap must be atleast 1");

    ptrdiff_t len = 0;
    forsep(part, text, sep) {
        if (len >= cap) break;

        arr[len++] = part;
    }

    return len;
}

static inline int starts(str s, str prefix)
{
    return (s.len >= prefix.len) \
       &&  sequal(span(s.data, s.data + prefix.len), prefix);
}
static inline int ends(str s, str suffix)
{
    return (s.len >= suffix.len) \
       &&  sequal(span(s.data + s.len - suffix.len, s.data + s.len), suffix);
}

static inline str trimleft(str s)
{
    for (; s.len && (unsigned char)*s.data <= ' '; ++s.data, --s.len);
    return s;
}
static inline str trimright(str s)
{
    for (; s.len && (unsigned char)s.data[s.len-1] <= ' '; --s.len);
    return s;
}
static inline str trim(str s) { return trimleft(trimright(s)); }

static inline str sadvanced(str s, ptrdiff_t i)
{
    if (i > 0) {
        s.data += i;
        s.len -= i;
    }
    return s;
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
    size_t oldstate = *state;
    *state = oldstate * 6364136223846793005ULL + 1111111111111111111ULL;
    unsigned int xorshifted = (unsigned int)(((oldstate >> 18u) ^ oldstate) >> 27u);
    unsigned int rot = (unsigned int)(oldstate >> 59u);
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}
static inline unsigned int rando(size_t *state, unsigned int from_including, unsigned int to_including)
{
    assert(from_including <= to_including && "invalid range for random!");
    if (to_including < from_including) return from_including;

    unsigned int range = to_including - from_including + 1;

    // Exclude last % range cycle to eliminate mod bias
    unsigned int limit = UINT32_MAX - (UINT32_MAX % range);
    unsigned int r;
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
        if (i == 0) continue;

        const char *k = table->keys[i];

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
static inline int htablelen(htindex *table) {
    return table->idxs[0];
}


/* PRINT GENERIC */

static inline void print_char(char c) { printf("%c", c); }
static inline void print_int(int i) { printf("%d", i); }
static inline void print_uint(unsigned int i) { printf("%u", i); }
static inline void print_ssize(ptrdiff_t s) { printf("%zd", s); }
static inline void print_size(size_t z) { printf("%zu", z); }
static inline void print_float(float f) { printf("%.5f", (double)f); }
static inline void print_double(double d) { printf("%.9f", d); }
static inline void print_str(str s) { printf("%.*s", (int)s.len, s.data); }
static inline void print_cstr(const char * cs) { printf("%s", cs); }

#define print(x) \
    _Generic((x), \
        char: print_char, \
        int: print_int, unsigned int: print_uint, \
        ptrdiff_t: print_ssize, size_t: print_size, \
        float: print_float, double: print_double, \
        str: print_str, const char *: print_cstr, \
        char *: print_cstr \
    )(x)

#define printend(x, end) print(x); print((char)end)
#define printsp(x) printend(x, ' ')
#define printcomma(x) printend(x, ', ')
#define println(x) printend(x, '\n')

#define printarr(arr, n) print("{ "); for(int i = 0; i < n; ++i) { printsp(arr[i]); } println("}")

static inline void print_stopwatch(clock_t stopwatch)
{
    clock_t end = clock();
    if (end < stopwatch) {
        printf("Clock error, end < start");
        return;
    }
    printf("  Executed in %.3f seconds\n", (double)(clock() - stopwatch) / (double)CLOCKS_PER_SEC);
}

/* SCAN GENERIC */

static inline void scan_char(char *c) { (void)scanf(" %c", c); }
static inline void scan_int(int *i) { (void)scanf(" %d", i); }
static inline void scan_ssize(ptrdiff_t *s) { (void)scanf(" %zd", s); }
static inline void scan_size(size_t *z) { (void)scanf(" %zu", z); }
static inline void scan_float(float *f) { (void)scanf(" %f", f); }
static inline void scan_double(double *d) { (void)scanf(" %lf", d); }

static inline str scanword(arena *a)
{
    char buffer[256];
    (void)scanf(" %255s", buffer);

    ptrdiff_t len = cstrlen(buffer);
    char *data = new(a, len + 1, char);
    cmemcpy(data, buffer, len);
    data[len] = 0;

    return (str){ data, len };
}
static inline str scanline(arena *a)
{
    char buffer[1024];
    (void)scanf(" %1023[^\n]", buffer);

    ptrdiff_t len = cstrlen(buffer);
    char *data = new(a, len + 1, char);
    cmemcpy(data, buffer, len);
    data[len] = 0;

    return (str){ data, len };
}

#define scan(x) \
    _Generic((x), \
        char: scan_char, int: scan_int, \
        ptrdiff_t: scan_ssize, size_t: scan_size, \
        float: scan_float, double: scan_double \
    )(&x)


/* MACRO ALGOS */

#define decl_cmpfn(fnname, type, ... ) \
    static inline int fnname(const void *a_, const void *b_) { const type *a = a_; const type *b = b_;  return (__VA_ARGS__); }

#define sort(arr, n, cmpfn) qsort(arr, n, sizeof(arr[0]), cmpfn)


/* FILES */

static inline const char * ensure_binary_mode(const char * cstring)
{
    ptrdiff_t len = cstrlen(cstring);
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
        r.len = (ptrdiff_t)fread(r.data, 1, (size_t)(a->end - a->beg), file);

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

static inline ptrdiff_t filelen(FILE *file)
{
    #ifdef _WIN32
        ptrdiff_t len = (ptrdiff_t)_filelengthi64(_fileno(file));
    #else // assume POSIX
        struct stat file_stat;
        int fstat_success = fstat(fileno(file), &file_stat) != -1; 
        assert(fstat_success);
        ptrdiff_t len = (ptrdiff_t)file_stat.st_size;
    #endif 

    assert(len >= 0 && "filelen must not be negative!");
    return len;
}

static inline MMAP mopen(const char *filename, const char *mode)
{
    int readit = mode[0] == 'r' && mode[1] != '+';
    MMAP r = {0};

    for(FILE *file = fopen(filename, ensure_binary_mode(mode)); file; fclose(file), file = NULL) {
        ptrdiff_t len = filelen(file);
        assert(len > 0 && "filelen can't be zero");

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

static inline void mclose(MMAP s)
{
    if (!s.len || !s.data) return;

    #ifdef _WIN32
        UnmapViewOfFile(s.data);
    #else
        munmap(s.data, (size_t)s.len);
    #endif
}


/* THREADS */

static inline THREAD go(threadfun_ threadfun, void * threadarg, ptrdiff_t thread_stack_size) 
{
    THREAD thread;
    thread_stack_size = max(thread_stack_size, 16*KB);

    #ifdef _WIN32
        thread = CreateThread(0, (size_t)thread_stack_size, threadfun, threadarg, 0, 0);
        assert(thread && "fatal error: could not launch thread");
    #else // assume POSIX
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setstacksize(&attr, (size_t)thread_stack_size);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        int error = pthread_create(&thread, &attr, threadfun, threadarg);
        assert(!error && "fatal error: could not launch thread");
        pthread_attr_destroy(&attr);
    #endif

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
    println(command); 
    printf("\n");

    int ret = system(str2cstr(a, command));
    printf("\n");

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

#pragma once
#ifdef __cplusplus
extern "C" { /* Cancels Name Mangling when compiled as C++ */
#endif


#include <assert.h>
#include <stdio.h>
#if defined(_WIN32) || defined(_WIN64)
    #if defined(_MSC_VER)
        #pragma warning(disable: 28301) 
        #pragma warning(disable: 28251)
    #endif
    #include <intrin.h>
#endif


#if !defined(LENSTR_T_DEFINED)
    #define LENSTR_T_DEFINED
    /* struct lenstr_t { size_t len; const char *str; }; */
    struct lenstr_t { size_t len; const char *str; };
#endif

static inline int aleh_msi_lenstr_cmp(const struct lenstr_t a_lenstr, const struct lenstr_t b_lenstr) {
    size_t min_len = a_lenstr.len <= b_lenstr.len ? a_lenstr.len : b_lenstr.len;
    size_t i;
    for (i = 0; i < min_len; ++i) {
        if (a_lenstr.str[i] != b_lenstr.str[i]) {
            return a_lenstr.str[i] - b_lenstr.str[i];
        }
    }
    return a_lenstr.len == b_lenstr.len ? 0 : (a_lenstr.len < b_lenstr.len ? - 1 : 1);
}

static inline void aleh_msi_lenstr_printend(const struct lenstr_t lenstr, const char *const end) { printf("%.*s%s", (int)lenstr.len, lenstr.str, end); }

static inline unsigned char aleh_msi_highbit(unsigned int uint_) {
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



static inline size_t aleh_msi_lenstr_hash(const struct lenstr_t chars_lenstr) {
    const char *const chars = chars_lenstr.str;
    size_t chars_len = chars_lenstr.len;

    size_t h = ((size_t)2147482697);
    
    size_t i;
    for(i = 0; i < chars_len; ++i) { 
        h ^= chars[i] & 255; h *= ((size_t)2147482951);
    }
    return (h ^ (h >> 31)) >> 1;
}

static inline size_t aleh_msi_number_hash(size_t number) {
    size_t x = number;
    
    x *= ((size_t)2147480369); 
    x = (x ^ (x >> 31));
    x *= ((size_t)2147476769); 
    
    return (x ^ (x >> 31)) >> 1;
}




/* Mask-Step-Index (MSI) lookup. Returns the next index. */
static inline unsigned int ht_msi_lookup(
    size_t hash, /* 1st hash acts as base location */
    unsigned int index, /* 2nd "hash" steps over the "list of elements" from base-location */
    unsigned char exp /* power-2 exp used as the Hash Table capacity */
) {
    unsigned int step = (unsigned int)(hash >> (sizeof(size_t)*8 - exp)) | 1;
    return (((unsigned int)index + step) & ((unsigned int) ((1 << exp) - 1)));
}

static inline unsigned int ht_msi_number_lookup(const size_t search_key, const unsigned int hashtable_cap, size_t hashtable[]) {
    const unsigned char exp = aleh_msi_highbit((unsigned int)hashtable_cap);
    size_t h = aleh_msi_number_hash(search_key);
    unsigned int pos = ht_msi_lookup(h, (unsigned int)h, exp);

    assert(search_key != 0);

    while (hashtable[pos] != 0 && search_key != hashtable[pos]) {
        pos = ht_msi_lookup(h, pos, exp);
    }

    return pos;
}

static inline unsigned int ht_msi_number_upsert(
        const size_t search_key, 
        const unsigned int hashtable_cap, size_t hashtable[], unsigned int *hashtable_len) 
{   
    unsigned int pos = (
        (void)assert((*hashtable_len) < hashtable_cap),
        ht_msi_number_lookup(search_key, hashtable_cap, hashtable)
    );
    (*hashtable_len) += (hashtable[pos] == 0) ? 1 : 0;
    hashtable[pos] = search_key;
    return pos;
}

static inline void ht_msi_number_to_arr(
    const unsigned int hashtable_cap, size_t hashtable[],
    const unsigned int array_cap, size_t array[], unsigned int *array_len)
{
    unsigned int i;
    for (i = 0; i < hashtable_cap; ++i) {
        if (!hashtable[0]) {
            continue;
        }
        assert(*array_len < array_cap);
        array[(*array_len)++] = hashtable[i];
    }
}

static inline unsigned int ht_msi_lenstr_lookup(
        const struct lenstr_t search_key, 
        const unsigned int hashtable_cap, struct lenstr_t hashtable[]) 
{
    unsigned char exp = aleh_msi_highbit((unsigned int)hashtable_cap);
    size_t h = aleh_msi_lenstr_hash(search_key);
    unsigned int pos = ht_msi_lookup(h, (unsigned int)h, exp);

    assert(search_key.len != 0);

    while (hashtable[pos].len != 0 && aleh_msi_lenstr_cmp(search_key, hashtable[pos]) != 0) {
        pos = ht_msi_lookup(h, pos, exp);
    }

    return pos;
}

static inline unsigned int ht_msi_lenstr_upsert(
        const struct lenstr_t search_key, 
        const unsigned int hashtable_cap, struct lenstr_t hashtable[], unsigned int *hashtable_len) 
{
    unsigned int pos = (
        (void)assert((*hashtable_len) < hashtable_cap),
        ht_msi_lenstr_lookup(search_key, hashtable_cap, hashtable)
    );


    (*hashtable_len) += (hashtable[pos].len == 0) ? 1 : 0;
    hashtable[pos] = search_key;
    return pos;
}

static inline void ht_msi_lenstr_print(const unsigned int hashtable_cap, struct lenstr_t hashtable[], const unsigned int hashtable_len) {
    unsigned int i;
    printf("#%u %u\n", hashtable_len, hashtable_cap);
    for (i = 0; i < hashtable_cap; ++i) {
        if (hashtable[i].len == 0) {
            continue;
        }
        aleh_msi_lenstr_printend(hashtable[i], ", ");
    }
    printf("\n");
}

static inline void ht_msi_lenstr_to_arr(
    const unsigned int hashtable_cap, struct lenstr_t hashtable[],
    const unsigned int array_cap, struct lenstr_t array[], unsigned int *array_len)
{
    unsigned int i;
    for (i = 0; i < hashtable_cap; ++i) {
        if (hashtable[i].len == 0) {
            continue;
        }
        assert(*array_len < array_cap);
        array[(*array_len)++] = hashtable[i];
    }
}


#ifdef __cplusplus
} /* closes extern C, Cancels Name Mangling when compiled as C++ */
#endif
#pragma once
#ifdef __cplusplus
extern "C" { /* Cancels Name Mangling when compiled as C++ */
#endif


#include "./math.h"
#include "./hash.h"


#if !defined(LENSTR_T_DEFINED)
    #define LENSTR_T_DEFINED
    /* struct lenstr_t { size_t len; const char *str; }; */
    struct lenstr_t { size_t len; const char *str; };

    static inline int lenstr_cmp(const struct lenstr_t a_lenstr, const struct lenstr_t b_lenstr) {
        size_t min_len = a_lenstr.len <= b_lenstr.len ? a_lenstr.len : b_lenstr.len;
        size_t i;
        for (i = 0; i < min_len; ++i) {
            if (a_lenstr.str[i] != b_lenstr.str[i]) {
                return a_lenstr.str[i] - b_lenstr.str[i];
            }
        }
        return a_lenstr.len == b_lenstr.len ? 0 : (a_lenstr.len < b_lenstr.len ? - 1 : 1);
    }
#endif


/* Mask-Step-Index (MSI) lookup. Returns the next index. */
static inline unsigned int ht_lookup(
    size_t hash, /* 1st hash acts as base location */
    unsigned int index, /* 2nd "hash" steps over the "list of elements" from base-location */
    unsigned char exp /* power-2 exp used as the Hash Table capacity */
) {
    unsigned int step = (unsigned int)(hash >> (sizeof(size_t)*8 - exp)) | 1;
    return (((unsigned int)index + step) & ((unsigned int) ((1 << exp) - 1)));
}

static inline unsigned int ht_number_lookup(const size_t search_key, const unsigned int hashtable_cap, size_t hashtable[]) {
    const unsigned char exp = highbit((unsigned int)hashtable_cap);
    size_t h = number_hash(search_key);
    unsigned int pos = ht_lookup(h, (unsigned int)h, exp);

    assert(search_key != 0);

    while (hashtable[pos] != 0 && search_key != hashtable[pos]) {
        pos = ht_lookup(h, pos, exp);
    }

    return pos;
}

static inline unsigned int ht_number_upsert(
        const size_t search_key, 
        const unsigned int hashtable_cap, size_t hashtable[], unsigned int *hashtable_len) 
{   
    unsigned int pos = (
        (void)assert((*hashtable_len) < hashtable_cap),
        ht_number_lookup(search_key, hashtable_cap, hashtable)
    );
    (*hashtable_len) += (hashtable[pos] == 0) ? 1 : 0;
    hashtable[pos] = search_key;
    return pos;
}

static inline void ht_number_to_arr(
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

static inline unsigned int ht_lenstr_lookup(
        const struct lenstr_t search_key, 
        const unsigned int hashtable_cap, struct lenstr_t hashtable[]) 
{
    unsigned char exp = highbit((unsigned int)hashtable_cap);
    size_t h = lenstr_hash(search_key);
    unsigned int pos = ht_lookup(h, (unsigned int)h, exp);

    assert(search_key.len != 0);

    while (hashtable[pos].len != 0 && lenstr_cmp(search_key, hashtable[pos]) != 0) {
        pos = ht_lookup(h, pos, exp);
    }

    return pos;
}

static inline unsigned int ht_lenstr_upsert(
        const struct lenstr_t search_key, 
        const unsigned int hashtable_cap, struct lenstr_t hashtable[], unsigned int *hashtable_len) 
{
    unsigned int pos = (
        (void)assert((*hashtable_len) < hashtable_cap),
        ht_lenstr_lookup(search_key, hashtable_cap, hashtable)
    );


    (*hashtable_len) += (hashtable[pos].len == 0) ? 1 : 0;
    hashtable[pos] = search_key;
    return pos;
}

static inline void ht_lenstr_print(const unsigned int hashtable_cap, struct lenstr_t hashtable[], const unsigned int hashtable_len) {
    unsigned int i;
    printf("#%u %u\n", hashtable_len, hashtable_cap);
    for (i = 0; i < hashtable_cap; ++i) {
        if (hashtable[i].len == 0) {
            continue;
        }
        lenstr_printend(hashtable[i], ", ");
    }
    printf("\n");
}

static inline void ht_lenstr_to_arr(
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
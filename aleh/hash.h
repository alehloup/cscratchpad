#pragma once
#ifdef __cplusplus
extern "C" { /* Cancels Name Mangling when compiled as C++ */
#endif


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


static inline size_t lenstr_hash(const struct lenstr_t chars_lenstr) {
    const char *const chars = chars_lenstr.str;
    size_t chars_len = chars_lenstr.len;

    size_t h = ((size_t)2147482697);
    
    size_t i;
    for(i = 0; i < chars_len; ++i) { 
        h ^= chars[i] & 255; h *= ((size_t)2147482951);
    }
    return (h ^ (h >> 31)) >> 1;
}

static inline size_t number_hash(size_t number) {
    size_t x = number;
    
    x *= ((size_t)2147480369); 
    x = (x ^ (x >> 31));
    x *= ((size_t)2147476769); 
    
    return (x ^ (x >> 31)) >> 1;
}


#ifdef __cplusplus
} /* closes extern C, Cancels Name Mangling when compiled as C++ */
#endif
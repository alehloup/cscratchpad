#pragma once
#ifdef __cplusplus
extern "C" { /* Cancels Name Mangling when compiled as C++ */
#endif


#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <math.h>


#if !defined(LENSTR_T_DEFINED)
    #define LENSTR_T_DEFINED
    /* struct lenstr_t { size_t len; const char *str; }; */
    struct lenstr_t { size_t len; const char *str; };
#endif

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

static inline struct lenstr_t to_lenstr(const char *const cstring) {  
    struct lenstr_t lenstr = {0, 0};
    lenstr.len = strlen(cstring);
    lenstr.str = cstring;

    return lenstr;
}

static inline void lenstr_print(const struct lenstr_t lenstr) { printf("%.*s\n", (int)lenstr.len, lenstr.str); }
static inline void lenstr_printend(const struct lenstr_t lenstr, const char *const end) { printf("%.*s%s", (int)lenstr.len, lenstr.str, end); }

static inline void set_locale_to_utf8(void) {
    setlocale(LC_CTYPE, "en_US.UTF-8");
}
/* You need to have set a locale, default locale is C locale. Best to call set_locale_to_utf8 */
static inline int lenstr_cmp_locale(const struct lenstr_t a_lenstr, const struct lenstr_t b_lenstr) {
    char a[512], b[512];
    assert(a_lenstr.len < 512 && b_lenstr.len < 512);

    memcpy(a, a_lenstr.str, a_lenstr.len);
    memcpy(b, b_lenstr.str, b_lenstr.len);
    a[a_lenstr.len] = 0;
    b[b_lenstr.len] = 0;

    return strcoll(a, b);
}

static inline int startswith(const struct lenstr_t prefix, const struct lenstr_t text) {
    struct lenstr_t text_start = {0, 0}; 
    text_start.len = prefix.len; 
    text_start.str = text.str;
    return lenstr_cmp(prefix, text_start) == 0;
}

static inline size_t char_pos(const char letter, const char *const cstring) {
    const char *ptr = strchr(cstring, letter);
    if (ptr >= cstring) {
        return (size_t)(ptr - cstring);
    } else {
        return (size_t)-1;
    }
}

static inline size_t char_pos_lenstr(const char letter, const struct lenstr_t lenstr) {
    const char *const ptr = 
        lenstr.len == 0 ? 0 
        : (const char *) memchr((void const *)lenstr.str, letter, lenstr.len);
    if (ptr >= lenstr.str) {
        return (size_t)(ptr - lenstr.str);
    } else {
        return (size_t)-1;
    }
}

static inline struct lenstr_t subss(struct lenstr_t lenstr, const int start, const int end) {
    size_t pos_start = start < 0 ? lenstr.len - (size_t)abs(start) : (size_t)start;
    size_t pos_end = end < 0 ? lenstr.len - (size_t)abs(end) : (size_t)end;

    size_t true_end = pos_end > lenstr.len ? lenstr.len : pos_end;
    size_t true_start = pos_start != 0 && pos_start > true_end ? true_end-1 : pos_start;

    size_t len = true_start < true_end ? true_end - true_start : 0;

    struct lenstr_t sub = {0, 0};
    sub.len = len;
    sub.str = &lenstr.str[true_start];
    return sub;
}

static inline void split(const struct lenstr_t lenstr, const char splitter, 
    const size_t parts_cap, struct lenstr_t parts[], size_t *parts_len) 
{
    struct lenstr_t cur = lenstr;
    size_t pos = 0;
    
    for (pos = char_pos_lenstr(splitter, cur); pos != (size_t)-1; pos = char_pos_lenstr(splitter, cur)) {
        struct lenstr_t part = {0, 0};
        part.len = pos;
        part.str = cur.str;

        assert(*parts_len < parts_cap);
        parts[(*parts_len)++] = part;
        ++pos;
        cur.len -= pos; 
        cur.str = &cur.str[pos];
    }

    if (cur.len > 0) {
        struct lenstr_t part = {0, 0};
        part.len = cur.len;
        part.str = cur.str;
        assert(*parts_len < parts_cap);
        parts[(*parts_len)++] = part;
    }
}

static inline void to_lines(const struct lenstr_t lenstr, const size_t lines_cap, struct lenstr_t lines[], size_t *lines_len) {
    split(lenstr, '\n', lines_cap, lines, lines_len);
}


static inline void buffer_to_lines(
    const char buffer[], const size_t buffer_len, 
    const size_t lines_cap, struct lenstr_t lines[], size_t *lines_len) 
{   
    struct lenstr_t text = {0, 0};
    text.len = buffer_len;
    text.str = buffer;
    to_lines(text, lines_cap, lines, lines_len);
}

static inline void buffer_append_lenstr(const size_t dst_buffer_cap, char dst_buffer[], size_t *dst_buffer_len, 
    const struct lenstr_t src_chars_lenstr) 
{
    assert((*dst_buffer_len) + src_chars_lenstr.len < dst_buffer_cap);

    memcpy(&dst_buffer[*dst_buffer_len], src_chars_lenstr.str, src_chars_lenstr.len);

    *dst_buffer_len += src_chars_lenstr.len;
    dst_buffer[(*dst_buffer_len)] = 0;
}
static inline void buffer_append_cstr(const size_t dst_buffer_cap, char dst_buffer[], size_t *dst_buffer_len, 
const char *const cstr) 
{
    const size_t cstr_len = strlen(cstr);

    assert((*dst_buffer_len) + cstr_len < dst_buffer_cap);

    memcpy(&dst_buffer[*dst_buffer_len], cstr, cstr_len);

    *dst_buffer_len += cstr_len;
    dst_buffer[(*dst_buffer_len)] = 0;
}
static inline void buffer_append_cstrs(const size_t dst_buffer_cap, char dst_buffer[], size_t *dst_buffer_len, 
    const char *const cstrs[], const size_t cstrs_len) 
{ 
    size_t i;
    for (i = 0; i < cstrs_len; ++i) {
        buffer_append_cstr(dst_buffer_cap, dst_buffer, dst_buffer_len, cstrs[i]);
    }
}

static inline size_t size_without_nullbytes(const char *const buffer, size_t current_size) {
    while (current_size > 0 && buffer[--current_size] == '\0') {
        /* Empty Body */
    }
    
    return ++current_size;
}


#ifdef __cplusplus
} /* closes extern C, Cancels Name Mangling when compiled as C++ */
#endif

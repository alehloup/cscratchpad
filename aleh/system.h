#pragma once
#ifdef __cplusplus
extern "C" { /* Cancels Name Mangling when compiled as C++ */
#endif


#if defined(_WIN32) || defined(_WIN64)
    #if !defined(WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <Windows.h>
    #include <io.h>
#else /* assume Unix: */
    #include <unistd.h>
#endif
#include <assert.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


static inline size_t aleh_system_char_pos(const char letter, const char *const cstring) {
    const char *ptr = strchr(cstring, letter);
    if (ptr >= cstring) {
        return (size_t)(ptr - cstring);
    } else {
        return (size_t)-1;
    }
}

static inline void aleh_system_buffer_append_cstr(const size_t dst_buffer_cap, char dst_buffer[], size_t *dst_buffer_len, 
const char *const cstr) 
{
    const size_t cstr_len = strlen(cstr);

    assert((*dst_buffer_len) + cstr_len < dst_buffer_cap);

    memcpy(&dst_buffer[*dst_buffer_len], cstr, cstr_len);

    *dst_buffer_len += cstr_len;
    dst_buffer[(*dst_buffer_len)] = 0;
}
static inline void aleh_system_buffer_append_cstrs(const size_t dst_buffer_cap, char dst_buffer[], size_t *dst_buffer_len, 
    const char *const cstrs[], const size_t cstrs_len) 
{ 
    size_t i;
    for (i = 0; i < cstrs_len; ++i) {
        aleh_system_buffer_append_cstr(dst_buffer_cap, dst_buffer, dst_buffer_len, cstrs[i]);
    }
}

static clock_t BENCHCLOCK_ = 0;
static inline void start_benchclock(void) {
    BENCHCLOCK_ = clock(); 
} 
static inline void stop_benchclock(void) {
    printf("\n\nExecuted in %.3f seconds \n", (double)(clock() - BENCHCLOCK_) / (double)CLOCKS_PER_SEC);
}

static inline int compile_c(const char *const c_file_c, const char *const flags) {
    char buffer[1024] = {0}; 
    size_t buffer_len = 0; 

    char c_file[256] = {0};
    const size_t dot_pos = aleh_system_char_pos('.', c_file_c);
    const size_t c_file_len = dot_pos == (size_t)-1 ? strlen(c_file_c) : dot_pos;

    void *ptr = (
        (void)assert(c_file_len < 255),
        memcpy(c_file, c_file_c, c_file_len)
    ); /* remove .c */

    const char * parts[6] = {
        flags, 
        " ", c_file, ".c -o ", c_file, ".exe "  /* compile .c to .exe */
    };

    aleh_system_buffer_append_cstrs(1024, buffer, &buffer_len, parts, 6);

    (void) ptr;
    printf("\n%.*s\n", (int)buffer_len, buffer);

    return system(buffer);
}

static inline int compile_run_c(const char *const c_file_c, const char *const flags) {
    char buffer[256] = {0};
    size_t buffer_len = 0;

    char c_file[256] = {0};
    const size_t dot_pos = aleh_system_char_pos('.', c_file_c);
    const size_t c_file_len = dot_pos == (size_t)-1 ? strlen(c_file_c) : dot_pos;

    void *ptr = (
        (void)assert(c_file_len < 255),
        memcpy(c_file, c_file_c, c_file_len)
    ); /* remove .c */

    const char * parts[4] = {
        "true && \"./", c_file, ".exe", "\" " /* execute */
    };

    int success = compile_c(c_file_c, flags);

    aleh_system_buffer_append_cstrs(256, buffer, &buffer_len, parts, 4);

    (void) ptr;
    printf("\n%.*s\n", (int)buffer_len, buffer);

    start_benchclock();
    success = system(buffer);
    stop_benchclock();

    return success;
}


#ifdef __cplusplus
} /* closes extern C, Cancels Name Mangling when compiled as C++ */
#endif

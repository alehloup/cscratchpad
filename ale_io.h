#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ale.h"

/*
    ==================== TIME BENCHMARK ====================
*/
_fun f64 seconds_since(clock_t start)
{
    return (f64)(clock() - start) / CLOCKS_PER_SEC;
}

_proc print_clock(clock_t start) {
    printf("\n\nExecuted in %f seconds \n", seconds_since(start));
}
//  ^^^^^^^^^^^^^^^^^^^^ TIME BENCHMARK ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== SHELL ====================
*/
gcc_attr(format(printf, 3, 4), nonnull)
i32 shellrun(i32 bufferlen, char buffer [512], ccstr format, ...) {
    va_list args;
    u8 *buf = zeromem((u8 *) buffer, 512);
    assert(buf == (u8 *)buffer && "zeromem returned different address!");

    va_start(args, format);

    vsprintf_s(buffer, (u64) bufferlen, format, args);
    return system(buffer);
}
//  ^^^^^^^^^^^^^^^^^^^^ SHELL ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== FILES ====================
*/
_fun i64 fread_noex(mstr dst, i64 sz, i64 count, FILE * f) {
    #ifdef __cplusplus
        try { return (i64) fread(dst, (u64) sz, (u64) count, f); } catch(...) {return 0;}
    #endif 
              return (i64) fread(dst, (u64) sz, (u64) count, f);
}
_fun i64 fwrite_noex(ccstr Str, i64 Size, i64 Count, FILE * File) {
    #ifdef __cplusplus
        try { return (i64) fwrite(Str, (u64) Size, (u64) Count, File); } catch(...) {return 0;}
    #endif 
              return (i64) fwrite(Str, (u64) Size, (u64) Count, File);
}

_fun i64 file_to_cstring(ccstr filename, i64 charbuffer_cap, char charbuffer[64]) {
    i64 fsize = 0;

        FILE *f = 0; i32 err = 
    fopen_s(&f, filename, "rb");
    
        assert(!err && "Could not open file for reading");
    
        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        assert(charbuffer_cap >= fsize+2 && "charbuffer is not enough for file size");

        {
            i64 bytesread = fread_noex(charbuffer, 1LL, fsize, f);
            assert(bytesread == fsize && "could not read fsize#bytes"); 
            
            charbuffer[fsize] = charbuffer[fsize-1] != '\n' ? '\n' : '\0';
            charbuffer[fsize+1] = '\0';
        }

    fclose(f);

    return fsize;
}

_fun i32 file_to_lines(ccstr filename, i32 lines_cap, mstr lines[64], i64 charbuffer_cap, char charbuffer[64]) {
    i64 charbuffer_len = file_to_cstring(filename, charbuffer_cap, charbuffer);
    discard_ charbuffer_len;
    return into_lines(charbuffer, lines_cap, lines);
}

_proc_hot cstring_to_file(ccstr buffer, ccstr filename) {
        FILE *f = 0; i32 err = 
    fopen_s(&f, filename, "wb");
        assert(!err && "Could not open file for writting");
        {
            i64 fsize = cstrlen(buffer);
            i64 bytes_written = fwrite_noex(buffer, 1, fsize, f);
            assert(bytes_written == fsize && "could not write fsize#bytes");
        }
    fclose(f);
}

_proc_hot lines_to_file(i32 lines_len, mstr lines[64], ccstr filename) {
        FILE *f = 0; i32 err = 
    fopen_s(&f, filename, "wb");
        assert(!err && "Could not open file for writting");
        {
            i64 bytes_written = 0;
            i64 fsize = 0;

            for (i32 i = 0; i < lines_len; ++i) {
                ccstr line = lines[i]; 

                fsize = cstrlen(line);
                bytes_written = fwrite_noex(line, 1, fsize, f);
                bytes_written += fwrite_noex("\n", 1, 1, f);
                assert(bytes_written == fsize + 1 && "could not write fsize#bytes");
            }
        }
    fclose(f);
}
//  ^^^^^^^^^^^^^^^^^^^^ FILES ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== STDLIB ====================
*/

_proc sort_cstrings(i64 cstrings_len, mstr cstrings[1]) {
    qsort(
        cstrings, (u64) cstrings_len,
        sizeof(mstr), void_compare_strings
    );
} 

_proc sort_cstrings_custom(i64 cstrings_len, mstr cstrings[1], int (*compare_fun)(cvoidp a, cvoidp b)) {
    qsort(
        cstrings, (u64)  cstrings_len,
        sizeof(mstr), compare_fun
    );
} 
//  ^^^^^^^^^^^^^^^^^^^^ STDLIB ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== PRINT ====================
*/

#define print_vec(vec_to_print_, vec_to_print_len_, format_str_) \
    for(int ivec_ = 0; ivec_ < vec_to_print_len_; ++ivec_) \
        printf(format_str_, vec_to_print_[ivec_]); \
    printf("\n")

//  ^^^^^^^^^^^^^^^^^^^^ PRINT ^^^^^^^^^^^^^^^^^^^^

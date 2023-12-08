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
    printf("Executed in %f seconds \n", seconds_since(start));
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

_fun mstr file_to_buffer(Arena arena[1], ccstr filename) {
    mstr contents = 0;
    i64 fsize = 0;

    {
        FILE *f = 0; i32 err = 
    fopen_s(&f, filename, "rb");
    
        assert(!err && "Could not open file for reading");
    
        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        contents = (mstr) alloc(arena, 1LL, fsize+2);
        assert(contents && "Could not allocate buffer");

        {
            i64 bytesread = fread_noex(contents, 1LL, fsize, f);
            assert(bytesread == fsize && "could not read fsize#bytes"); 
            
            contents[fsize] = contents[fsize-1] != '\n' ? '\n' : '\0';
            contents[fsize+1] = '\0';
        }

    fclose(f);
    }
    
    return contents;
}

_fun mstr * file_to_lines(Arena arena[1], ccstr filename) {
    mstr buffer = file_to_buffer(arena, filename);
    return into_lines(arena, buffer);
}

_proc_hot buffer_to_file(ccstr buffer, ccstr filename) {
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

_proc_hot lines_to_file(mstr lines[1], ccstr filename) {
        FILE *f = 0; i32 err = 
    fopen_s(&f, filename, "wb");
        assert(!err && "Could not open file for writting");
        {
            i64 bytes_written = 0;
            i64 fsize = 0;

            for (i32 i = 0; i < hd_(lines)->len; ++i) {
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

_proc vec_sort_cstr(mstr cstrings[1]) {
    qsort(
        cstrings, (u64)  hd_(cstrings)->len,
        sizeof(i64), void_compare_strings
    );
} 
//  ^^^^^^^^^^^^^^^^^^^^ STDLIB ^^^^^^^^^^^^^^^^^^^^

/*
    ==================== PRINT ====================
*/

#define print_vec(vec_to_print_, format_str_) \
    for(int ivec_ = 0; ivec_ < hd_len_(vec_to_print_); ++ivec_) \
        printf(format_str_, vec_to_print_[ivec_]); \
    printf("\n")

//  ^^^^^^^^^^^^^^^^^^^^ PRINT ^^^^^^^^^^^^^^^^^^^^

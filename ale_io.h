#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

/*
    ==================== INPUT/OUTPUT IO ====================
*/

// SHELL
$format(/*bufferlen*/1, /*buffer*/2, /*format*/3, /*varargs*/4) 
i32 shellrun(i32 bufferlen, char buffer [512], cstr format, ...) {
    memset(buffer, '\0', 512);

    va_list args; va_start(args, format);

    vsprintf_s(buffer, (u64) bufferlen, format, args);
    return system(buffer);
}

// FILES
$fun i64 fread_noex(void * __dst, i64 __sz, i64 __n, FILE * __f) {
    #ifdef __cplusplus
        try { return (i64) fread(__dst, (u64) __sz, (u64) __n, __f); } catch(...) {return 0;}
    #endif 
              return (i64) fread(__dst, (u64) __sz, (u64) __n, __f);
}
$fun i64 fwrite_noex(cstr _Str, i64 _Size, i64 _Count, FILE * _File) {
    #ifdef __cplusplus
        try { return (i64) fwrite(_Str, (u64) _Size, (u64) _Count, _File); } catch(...) {return 0;}
    #endif 
              return (i64) fwrite(_Str, (u64) _Size, (u64) _Count, _File);
}

$fun mstr file_to_buffer(arena_t arena[1], cstr filename) {
    mstr contents = 0;

    {FILE *f = 0; i32 err = 
    fopen_s(&f, filename, "rb");
    
        _assert_(!err && "Could not open file for reading");
    
        fseek(f, 0, SEEK_END);
        i32 fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        contents = (mstr) alloc(arena, 1LL, fsize+2);
        _assert_(contents && "Could not allocate buffer");

        i64 bytesread = fread_noex(contents, 1LL, fsize, f);
        _assert_(bytesread == fsize && "could not read fsize#bytes"); 
        
        contents[fsize] = contents[fsize-1] != '\n' ? '\n' : '\0';
        contents[fsize+1] = '\0';

    fclose(f);
    }
    
    return contents;
}

$fun vector64_t file_to_lines(arena_t arena[1], cstr filename) {
    mstr buffer = file_to_buffer(arena, filename);
    return slice_into_lines(arena, buffer);
}

$fun vector64_t file_to_nonempty_lines(arena_t arena[1], cstr filename) {
    mstr buffer = file_to_buffer(arena, filename);
    return slice_into_nonempty_lines(arena, buffer);
}

$nonnull b32_t buffer_to_file(cstr buffer, cstr filename) {
    {FILE *f = 0; i32 err = 
    fopen_s(&f, filename, "wb");

        _assert_(!err && "Could not open file for writting");

        i64 fsize = cstrlen(buffer);
        i64 bytes_written = fwrite_noex(buffer, 1, fsize, f);
        _assert_(bytes_written == fsize && "could not write fsize#bytes");

    fclose(f);
    }
    return True;
}

$nonnull b32_t lines_to_file(vector64_t lines, cstr filename) {
    {FILE *f = 0; i32 err = 
    fopen_s(&f, filename, "wb");

        _assert_(!err && "Could not open file for writting");

        cstr *data = vec_data_as_cstr(&lines);
        i64 bytes_written = 0;
        cstr line = "";
        i64 fsize = 0;

        for (i32 i = 0; i < lines.len; ++i) {
            line = data[i]; 

            fsize = cstrlen(line);
            bytes_written = fwrite_noex(line, 1, fsize, f);
            bytes_written += fwrite_noex("\n", 1, 1, f);
            _assert_(bytes_written == fsize + 1 && "could not write fsize#bytes");
        }

    fclose(f);
    }
    return True;
}

$proc vec_sort_cstr(vector64_t cstrings) {
    qsort(
        vec_data_as_cstr(&cstrings), (u64) cstrings.len, 
        sizeof(i64), void_compare_strings
    );
} 

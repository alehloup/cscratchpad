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
int32_t shellrun(int32_t bufferlen, char buffer [512], cstr_t format, ...) {
    memset(buffer, '\0', 512);

    va_list args; va_start(args, format);

    vsprintf_s(buffer, bufferlen, format, args);
    return system(buffer);
}

// FILES
$fun size_t fread_noex(void * __dst, size_t __sz, size_t __n, FILE * __f) {
    #ifdef __cplusplus
        try { return fread(__dst, __sz, __n, __f); } catch(...) {return 0;}
    #endif 
              return fread(__dst, __sz, __n, __f);
}

$fun mstr_t file_to_buffer(arena_t arena[1], cstr_t filename) {
    mstr_t contents = 0;

    {FILE *f = 0; int32_t err = 
    fopen_s(&f, filename, "rb");
    
        assert(!err && "Could not open file for reading");
    
        fseek(f, 0, SEEK_END);
        int32_t fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        contents = (mstr_t) alloc(arena, 1LL, fsize+2);
        assert(contents && "Could not allocate buffer");

        int64_t bytesread = fread_noex(contents, 1LL, fsize, f);
        assert(bytesread == fsize && "could not read fsize#bytes"); 
        
        contents[fsize] = contents[fsize-1] != '\n' ? '\n' : '\0';
        contents[fsize+1] = '\0';

    fclose(f);
    }
    
    return contents;
}

$fun vector64_t file_to_lines(arena_t arena[1], cstr_t filename) {
    mstr_t buffer = file_to_buffer(arena, filename);
    return slice_into_lines(arena, buffer);
}

$fun vector64_t file_to_nonempty_lines(arena_t arena[1], cstr_t filename) {
    mstr_t buffer = file_to_buffer(arena, filename);
    return slice_into_nonempty_lines(arena, buffer);
}

$nonnull b32_t buffer_to_file(cstr_t buffer, cstr_t filename) {
    {FILE *f = 0; int32_t err = 
    fopen_s(&f, filename, "wb");

        assert(!err && "Could not open file for writting");

        int64_t fsize = strlen(buffer);
        int64_t bytes_written = fwrite(buffer, 1, fsize, f);
        assert(bytes_written == fsize && "could not write fsize#bytes");

    fclose(f);
    }
    return True;
}

$nonnull b32_t lines_to_file(vector64_t lines, cstr_t filename) {
    {FILE *f = 0; int32_t err = 
    fopen_s(&f, filename, "wb");

        assert(!err && "Could not open file for writting");

        cstr_t *data = vec_data_as_cstr(&lines);
        int64_t bytes_written = 0;
        cstr_t line = "";
        int64_t fsize = 0;

        for (int i = 0; i < lines.len; ++i) {
            line = data[i]; 

            fsize = strlen(line);
            bytes_written = fwrite(line, 1, fsize, f);
            bytes_written += fwrite("\n", 1, 1, f);
            assert(bytes_written == fsize + 1 && "could not write fsize#bytes");
        }

    fclose(f);
    }
    return True;
}

$proc vec_sort_cstr(vector64_t cstrings) {
    qsort(
        vec_data_as_cstr(&cstrings), (uint64_t) cstrings.len, 
        sizeof(int64_t), void_compare_strings
    );
} 

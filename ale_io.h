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
    
        if (err) {
            return contents;
        }
    
        fseek(f, 0, SEEK_END);
        int32_t fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        contents = (mstr_t) alloc(arena, 1LL, fsize+1);
        if (!contents) {
            fclose(f);
            return contents;
        }
        int32_t bytesread = (int32_t) fread_noex(contents, 1LL, fsize, f);
        assert(bytesread == fsize && "could not read fsize"); 
        
        contents[fsize] = 0;

    fclose(f);
    }
    
    return contents;
}

#pragma once
#ifdef __cplusplus
extern "C" { /* Cancels Name Mangling when compiled as C++ */
#endif


#include "./file.h"


#if defined(_WINDOWS_) /* if _WINDOWS_ else Unix */
static inline char * mmap_open(const char *const filename, const char *const mode, size_t *out_buffer_len) {
    void * mmap_handle;
    char * out_mmap_buffer;

    int readit = mode[0] == 'r' && mode[1] != '+';

    FILE *file = fopen_(filename, mode);
    { /* File opened */
        size_t fileSize = filelen_(file);
        assert(fileSize > 0);

        mmap_handle = CreateFileMapping(
            (void *)(size_t)_get_osfhandle(fileno_(file)),
            0, 
            readit ? PAGE_READONLY : PAGE_READWRITE, 
            0, 0, 0
        );
        { /* FileMapping handle opened */
            assert(mmap_handle);

            out_mmap_buffer = (char *)MapViewOfFile(
                mmap_handle, 
                FILE_MAP_READ | (readit ? 0 : FILE_MAP_WRITE),
                0, 0, 0
            );
            assert(out_mmap_buffer);
        } /* Closing FileMapping handle: */
        CloseHandle(mmap_handle);

        *out_buffer_len = fileSize;
    } /* Closing file: */
    fclose(file);

    return out_mmap_buffer;
}
static inline void mmap_close(char *mmap_buffer, size_t mmap_buffer_size) {
    assert(mmap_buffer_size);
    UnmapViewOfFile((void *)mmap_buffer);
}
#else /* Unix */
static inline char * mmap_open(const char *const filename, const char *const mode, size_t *out_buffer_len) {
    char * out_mmap_buffer;

    int readit = mode[0] == 'r' && mode[1] != '+';
    
    FILE *file = fopen_(filename, mode);
    { /* File opened */
        size_t fileSize = filelen_(file);
        assert(fileSize);
        *out_buffer_len = fileSize;

        out_mmap_buffer = (char *)mmap(
            0, 
            fileSize, 
            PROT_READ | (readit ? 0 : PROT_WRITE), 
            MAP_SHARED, 
            fileno_(file),
            0
        );
        assert(out_mmap_buffer);
    } /* Closing file: */
    fclose(file);

    return out_mmap_buffer;
}
static inline void mmap_close(char *mmap_buffer, size_t mmap_buffer_size) {
    munmap(mmap_buffer, mmap_buffer_size);
}
#endif /* endif _WINDOWS_ else Unix */


#ifdef __cplusplus
} /* closes extern C, Cancels Name Mangling when compiled as C++ */
#endif

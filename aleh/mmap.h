#pragma once
#ifdef __cplusplus
extern "C" { /* Cancels Name Mangling when compiled as C++ */
#endif

#include <assert.h>
#include <stdio.h>


#if defined(_WIN32) || defined(_WIN64)
    #if !defined(WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
        #include <Windows.h>
    #endif
    #include <io.h>

    static inline int aleh_mmap_fileno_(FILE *stream) {
        return _fileno(stream);
    }

    static inline size_t aleh_mmap_filelen_(FILE *stream) {
        return (size_t)_filelengthi64(_fileno(stream));
    }
#else /* Unix */
    #include <unistd.h>
    #include <sys/mman.h>
    #include <sys/stat.h>

    static inline int aleh_mmap_fileno_(FILE *stream) {
        return fileno(stream);
    }

    static inline size_t aleh_mmap_filelen_(FILE *stream) {
        struct stat file_stat;
        int fstat_success = fstat(fileno(stream), &file_stat) != -1;
        
        assert(fstat_success);
        return (size_t)file_stat.st_size;
    }
#endif


#if defined(_WINDOWS_) /* if _WINDOWS_ else Unix */
static inline char * mmap_open(const char *const filename, const char *const mode, size_t *out_buffer_len) {
    void * mmap_handle;
    char * out_mmap_buffer;

    int readit = mode[0] == 'r' && mode[1] != '+';

    FILE *file = fopen(filename, mode);
    { /* File opened */
        size_t fileSize = aleh_mmap_filelen_(file);
        assert(fileSize > 0);

        mmap_handle = CreateFileMapping(
            (void *)(size_t)_get_osfhandle(aleh_mmap_fileno_(file)),
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
    
    FILE *file = fopen(filename, mode);
    { /* File opened */
        size_t fileSize = aleh_mmap_filelen_(file);
        assert(fileSize);
        *out_buffer_len = fileSize;

        out_mmap_buffer = (char *)mmap(
            0, 
            fileSize, 
            PROT_READ | (readit ? 0 : PROT_WRITE), 
            MAP_SHARED, 
            aleh_mmap_fileno_(file),
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

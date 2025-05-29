#pragma once

#include <stdio.h>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <Windows.h>
    #include <io.h>
#else // assume Posix
    #include <unistd.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
#endif

typedef struct mmap_buffer { char *data; size_t len; } mmap_buffer;

static inline size_t filelen(FILE *file)
{
    #ifdef _WIN32
        size_t len = (size_t)_filelengthi64(_fileno(file));
    #else // assume POSIX
        struct stat file_stat;
        int fstat_success = fstat(fileno(file), &file_stat) != -1;
        size_t len = (size_t)file_stat.st_size;

        (void) fstat_success;
    #endif 

    return len;
}

static inline mmap_buffer mopen(const char *filename, const char *mode)
{
    int readit = mode[0] == 'r' && mode[1] != '+';
    mmap_buffer r = {0};

    for(FILE *file = fopen(filename, mode); file; fclose(file), file = 0) {
        size_t len = filelen(file);

        #ifdef _WIN32
            void *mhandle = CreateFileMapping(
                (void *)(size_t)_get_osfhandle(_fileno(file)), 0, 
                readit ? PAGE_READONLY : PAGE_READWRITE, 0, 0, 0
            );
                if (mhandle) {
                    r.data = (char *)MapViewOfFile(
                        mhandle, 
                        FILE_MAP_READ | (readit ? 0 : FILE_MAP_WRITE),
                        0, 0, 0
                    );
                    CloseHandle(mhandle);
                }
        #else // assume POSIX
            r.data = (char *)str(
                0, (size_t)len,
                PROT_READ | (readit ? 0 : PROT_WRITE), 
                MAP_SHARED, fileno(file), 0
            );
        #endif

        if (r.data) {
            r.len = len;
        }
    }

    return r;
}

static inline void mclose(mmap_buffer s)
{
    if (!s.len || !s.data) return;

    #ifdef _WIN32
        UnmapViewOfFile(s.data);
    #else
        munmap(s.data, (size_t)s.len);
    #endif
}

#pragma once
#ifdef __cplusplus
extern "C" { /* Cancels Name Mangling when compiled as C++ */
#endif


#if defined(_WIN32) || defined(_WIN64)
    #if !defined(WIN32_LEAN_AND_MEAN)
        #if defined(_MSC_VER)
            #pragma warning(disable: 28301) 
            #pragma warning(disable: 28251)
        #endif
        #define WIN32_LEAN_AND_MEAN
        #include <Windows.h>
        #include <io.h>
    #endif
#else /* assume Unix: */
    #include <unistd.h>
    #include <sys/stat.h>
#endif
#include <assert.h>
#include <stdio.h>


#if defined(_WINDOWS_) /* _WINDOWS_ */
    static inline int fileno_(FILE *stream) {
        return _fileno(stream);
    }
    static inline size_t filelen_(FILE *stream) {
        return (size_t)_filelengthi64(fileno_(stream));
    }
    static inline int fseek_(FILE *stream, size_t offset, int whence) {
        return _fseeki64(stream, (long long int)offset, whence);
    }
    static inline void ftruncate_(FILE *stream, size_t size) {
        int success = _chsize_s(fileno_(stream), (long long int)size) == 0;
        assert(success);
    }

    #if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
        static inline FILE * fopen_(const char *pathname, const char *mode) {
            FILE *f = 0;
            errno_t err = fopen_s(&f, pathname, mode);
            
            assert(err == 0);
            return f;
        }
    #else 
        static inline FILE * fopen_(const char *pathname, const char *mode) {
            return fopen(pathname, mode);
        }
    #endif
#else /* Unix */
    static inline int fileno_(FILE *stream) {
        return fileno(stream);
    }
    static inline int fseek_(FILE *stream, size_t offset, int whence) {
        return fseeko(stream, (off_t)offset, whence);
    }
    static inline size_t filelen_(FILE *stream) {
        struct stat file_stat;
        int fstat_success = fstat(fileno(stream), &file_stat) != -1;
        
        assert(fstat_success);
        return (size_t)file_stat.st_size;
    }
    static inline void ftruncate_(FILE *stream, size_t size) {
        int success = ftruncate(fileno_(stream), (off_t)size) == 0;
        assert(success);
    }

    static inline FILE * fopen_(const char *pathname, const char *mode) {
        return fopen(pathname, mode);
    }
#endif

static inline void file_create(const char *const filename, size_t initial_size) {
    FILE *file = fopen_(filename, "wb");
        int fopen_success = (
            (void)assert(file && initial_size > 0),
            fseek_(file, initial_size - 1, SEEK_SET) == 0
        );

        size_t written = (
            (void)assert(fopen_success),
            fwrite("", 1, 1, file)
        );
        assert(written == 1);
    fclose(file);
}


#ifdef __cplusplus
} /* closes extern C, Cancels Name Mangling when compiled as C++ */
#endif
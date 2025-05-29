#pragma once

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <Windows.h>
    #include <io.h>

    typedef HANDLE THREAD;
    #define threadfun_ret DWORD WINAPI
    typedef DWORD (WINAPI *threadfun_)(void *);
#else // assume Posix
    #include <unistd.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <pthread.h>

    typedef pthread_t THREAD;
    #define threadfun_ret void *
    typedef void * (*threadfun_)(void *);
#endif

static inline THREAD go(threadfun_ threadfun, void * threadarg, size_t thread_stack_size) 
{
    THREAD thread = {0};
    size_t thread_stack_size_adjusted = (size_t) thread_stack_size > 16*1024 ? thread_stack_size : 16*1024;
    int error = 0;

    #ifdef _WIN32
        thread = CreateThread(0, thread_stack_size_adjusted, threadfun, threadarg, 0, 0);
    #else // assume POSIX
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setstacksize(&attr, thread_stack_size_adjusted);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        error = pthread_create(&thread, &attr, threadfun, threadarg);
        pthread_attr_destroy(&attr);
    #endif

    (void) error;

    return thread;
}

static inline void join_thread(THREAD thread)
{
    #ifdef _WIN32
        WaitForSingleObject(thread, INFINITE);
    #else // assume POSIX
        pthread_join(thread, 0);
    #endif
}

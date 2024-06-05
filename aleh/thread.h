#pragma once
#ifdef __cplusplus
extern "C" { /* Cancels Name Mangling when compiled as C++ */
#endif


/* configure */
#define THREAD_STACK_SIZE_ 64 * 1024
#define MAX_NTHREADS 8192

#include <assert.h>
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
    #include <pthread.h>
    #include <sys/stat.h>
#endif


#if defined(_WINDOWS_)
    /* is _WINDOWS_ */
#define THREAD_T HANDLE

static inline THREAD_T go(void * (*routine)(void *thread_idx), size_t thread_id) {
    THREAD_T thread = CreateThread(0, THREAD_STACK_SIZE_, (LPTHREAD_START_ROUTINE)(size_t)routine, (void *)(thread_id), 0, 0);
    assert(thread != 0);

    return thread;
}
static inline void join_thread(THREAD_T thread) {
    WaitForSingleObject(thread, INFINITE);
}
    /* */
#else 
    /* is Unix */
#define THREAD_T pthread_t

static inline THREAD_T go(void * (*routine)(void *thread_idx), size_t thread_id) {
    int create_thread_success = 0;
    THREAD_T thread;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE_);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    create_thread_success = pthread_create(&thread, &attr, routine, (void *)(thread_id)) == 0;
    assert(create_thread_success);

    return thread;
}
static inline void join_thread(THREAD_T thread) {
    pthread_join(thread, 0);
}
#endif /* endif _WINDOWS_ else Unix */

static inline void go_threads(
    void * (*routine)(void *thread_idx), unsigned int number_of_threads_to_spawn, THREAD_T threads[])
{
    unsigned int i;
    assert(number_of_threads_to_spawn < MAX_NTHREADS);

    for (i = 0; i < number_of_threads_to_spawn; ++i) {
        threads[i] = go(routine, (size_t)i);
    }
}
static inline void join_threads(THREAD_T threads[], const unsigned int threads_len) {
    unsigned int i;
    assert(threads_len <= MAX_NTHREADS);
    
    for (i = 0; i < threads_len; ++i) {
        join_thread(threads[i]);
    }
}


#ifdef __cplusplus
} /* closes extern C, Cancels Name Mangling when compiled as C++ */
#endif

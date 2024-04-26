#include "ale.h"

#define NUM_THREADS_TO_CREATE 15999
static int64_t results[NUM_THREADS_TO_CREATE];

routine_ sum(void* thread_idx) {
    int32_t threadIdx = (int32_t)(uintptr_t)(thread_idx);
    results[threadIdx] = threadIdx * 10l;
    return 0;
}

int32_t main() {
    THREAD_T threads[NUM_THREADS_TO_CREATE];
    int64_t threads_len = 0;
    
    go_threads(sum, NUM_THREADS_TO_CREATE, ARRCAP_(threads), threads, &threads_len);
    join_threads(threads, threads_len);

    int64_t totalSum = 0;
    for (int32_t i = 0; i < NUM_THREADS_TO_CREATE; ++i) {
        totalSum += results[i];
    }

    int64_t correctSum = 0;
    for (int32_t i = 0; i < NUM_THREADS_TO_CREATE; ++i) {
        correctSum += (i*10);
    }
    

    printf("\nTotal sum: %" PRId64 " vs %" PRId64 " \n ", totalSum, correctSum);

    return 0;
}

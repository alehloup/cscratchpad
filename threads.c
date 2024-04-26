#include "ale.h"

#define NUM_THREADS_TO_CREATE 8
static int32_t results[NUM_THREADS_TO_CREATE];

routine_ sum(void* thread_idx) {
    int32_t threadIdx = (int32_t)(uintptr_t)(thread_idx);
    results[threadIdx] = threadIdx * 10;
    return 0;
}

int32_t main() {
    THREAD_T threads[16];
    int64_t threads_len = 0;
    
    go_threads(sum, NUM_THREADS_TO_CREATE, ARRCAP_(threads), threads, &threads_len);
    join_threads(threads, threads_len);

    int32_t totalSum = 0;
    for (int32_t i = 0; i < NUM_THREADS_TO_CREATE; ++i) {
        printf("results[%d]: %d, ", i, results[i]);
        totalSum += results[i];
    }

    printf("Total sum: %d vs %d \n ", totalSum, 10 + 20 + 30 + 40 + 50 + 60 + 70);

    return 0;
}

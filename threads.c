#include "ale.h"

#define NUM_THREADS_TO_CREATE 8001
static size_t results[NUM_THREADS_TO_CREATE];

static inline void * sum(void *thread_idx) {
    size_t threadIdx = (size_t)(thread_idx);
    results[threadIdx] = threadIdx * 10l;
    return 0;
}

int main() {
    THREAD_T threads[NUM_THREADS_TO_CREATE];
    size_t threads_len = 0, totalSum = 0, correctSum = 0;
    
    go_threads(sum, NUM_THREADS_TO_CREATE, arrsizeof(threads), threads, &threads_len);
    join_threads(threads, threads_len);

    for (unsigned int i = 0; i < NUM_THREADS_TO_CREATE; ++i) {
        totalSum += results[i];
    }

    for (unsigned int i = 0; i < NUM_THREADS_TO_CREATE; ++i) {
        correctSum += (i*10);
    }
    
    printf("\nTotal sum: %zu vs %zu \n ", totalSum, correctSum);

    return 0;
}

#include <stdio.h>
#include "./aleh/thread.h"

#define NUM_THREADS_TO_CREATE 8001
static unsigned int results[NUM_THREADS_TO_CREATE];

static inline void * sum(void *thread_idx) {
    unsigned int threadIdx = (unsigned int)(size_t)(thread_idx);
    results[threadIdx] = threadIdx * 10;
    return 0;
}

int main() {
    THREAD_T threads[NUM_THREADS_TO_CREATE];
    unsigned int totalSum = 0, correctSum = 0;
    
    go_threads(sum, NUM_THREADS_TO_CREATE, threads);
    join_threads(threads, NUM_THREADS_TO_CREATE);

    {
        unsigned int i;
        for (i = 0; i < NUM_THREADS_TO_CREATE; ++i) {
            totalSum += results[i];
        }
    }

    {
        unsigned int i;
        for (i = 0; i < NUM_THREADS_TO_CREATE; ++i) {
            correctSum += (i*10);
        }
    }
    
    printf("\nTotal sum: %u %s %u \n ", totalSum, totalSum == correctSum ? "==" : "!=" ,correctSum);

    return 0;
}

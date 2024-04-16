#include <stdio.h>
#include <Windows.h>
#include "ale.h"

#define NUM_THREADS 8
int32_t results[NUM_THREADS];

_thread_fun sum(void* args) {
    int32_t threadIdx = *(int32_t *)args;
    results[threadIdx] = threadIdx * 10;
    return 0;
}

int32_t main() {
    
    go_threads(threads, sum, NUM_THREADS);
    join_threads(threads);

    int32_t totalSum = 0;
    for (int32_t i = 0; i < NUM_THREADS; ++i) {
        totalSum += results[i];
    }

    printf("Total sum: %d vs %d \n ", totalSum, 10 + 20+ 30 + 40 + 50 + 60 + 70);

    return 0;
}

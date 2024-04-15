#include <stdio.h>
#include <Windows.h>
#include "ale.h"

#define NUM_THREADS 8
int results[NUM_THREADS];

_thread_fun sum(Any* args) {
    int threadIdx = *(int *)args;
    results[threadIdx] = threadIdx * 10;
    return 0;
}

int main() {
    
    go_threads(threads, sum, 8);
    join_threads(threads);

    int totalSum = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        totalSum += results[i];
    }

    printf("Total sum: %d vs %d \n ", totalSum, 10 + 20+ 30 + 40 + 50 + 60 + 70);

    return 0;
}

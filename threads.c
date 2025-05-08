#include "ale.h"

enum { NUM_THREADS_TO_CREATE = 16000 };
static ssize_t results[NUM_THREADS_TO_CREATE];

threadfun(sum) {
    ssize_t threadIdx = (ssize_t)(uintptr_t)(threadarg);
    results[threadIdx] = threadIdx * 10;
    return 0;
}

int main() {
    THREAD threads[NUM_THREADS_TO_CREATE] = {0};
    ssize_t totalSum = 0, correctSum = 0;

    for(ssize_t i = 0; i < NUM_THREADS_TO_CREATE; ++i) {
        threads[i] = go(sum, (void *)(uintptr_t)i, 0);
    }
    
    for(int i = 0; i < NUM_THREADS_TO_CREATE; ++i) {
        join_thread(threads[i]);
    }

    for (int i = 0; i < NUM_THREADS_TO_CREATE; ++i) {
        totalSum += results[i];
    }

    for (int i = 0; i < NUM_THREADS_TO_CREATE; ++i) {
        correctSum += (i*10);
    }
    
    printsp("\nTotal sum:"); printsp(totalSum);
    printsp(totalSum == correctSum ? "==" : "!=");
    printsp("Correct sum:"); printsp(correctSum);

    return 0;
}

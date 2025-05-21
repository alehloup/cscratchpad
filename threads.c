#include "ale.h"

enum { NUM_THREADS_TO_CREATE = 16000 };
static ptrdiff_t results[NUM_THREADS_TO_CREATE];

static inline threadfun_ret sum(void * threadarg) 
{
    ptrdiff_t threadIdx = (ptrdiff_t)(uintptr_t)(threadarg);
    results[threadIdx] = threadIdx * 10;
    return 0;
}

int main() 
{
    THREAD threads[NUM_THREADS_TO_CREATE] = {0};
    ptrdiff_t totalSum = 0, correctSum = 0;

    for(ptrdiff_t i = 0; i < NUM_THREADS_TO_CREATE; ++i) {
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
    
    printf("\nTotal sum: "); print_ssize(totalSum);
    printf(totalSum == correctSum ? "== " : "!= ");
    printf("Correct sum: "); print_ssize(correctSum);

    return 0;
}

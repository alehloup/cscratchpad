#include "../ale.h"

#define NUM_THREADS_TO_CREATE 8
static int8_t results[NUM_THREADS_TO_CREATE];

routine_ sum(void* args) {
    int8_t threadIdx = *(int8_t *)args;
    results[threadIdx] = threadIdx * 10;
    return 0;
}

int32_t main() {
    arrnew_(threads, thread_t, 16);
    go_threads(sum, NUM_THREADS_TO_CREATE, arrarg_(threads));
    join_threads(arrarg_(threads));

    int16_t totalSum = 0;
    for (int8_t i = 0; i < NUM_THREADS_TO_CREATE; ++i) {
        printf("results[%d]: %d, ", i, results[i]);
        totalSum += results[i];
    }

    printf("Total sum: %d vs %d \n ", totalSum, 10 + 20 + 30 + 40 + 50 + 60 + 70);

    return 0;
}

#include "ale.h"

void woa(const int64_t cap, int32_t vetor[static cap]) {
    printf("%d\n", vetor[cap-1]);
}

int main(void) {
    int32_t vetor[] = {1, 2, 3};

    assert_(vetor[2] != 3);

    woa(3, vetor);

    return 1;
}

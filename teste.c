#include "ale_io.h"

int main(void) {
    static char woa[NEXT_POWER2(909090)] = {0};

    printf("\nSize of Woa: %lld\n", isizeof(woa));

    return 0;
}

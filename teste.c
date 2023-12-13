#include "ale_io.h"

#define keys_cap 1024

int main(void) {
    static i32 keys[keys_cap] = {0};
    static i32 teste[POWER2_EXP(256)];

    printf("mask: %d\n", HTMASK(keys));
    printf("shift: %d\n", HTSHIFT(keys));
    printf("Keys 0: %d\n", keys[0]);

    int n = 256;
    for (int i = 8; i <= 25; ++i, n*=2) {
        printf("%d %d %d\n", n, i, POWER2_EXP(n));
    }

    printf("Size: %lld\n", countof(teste));

    return 0;
}

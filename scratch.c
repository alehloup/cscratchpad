#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

void print_mem(void * ptr, int32_t len) {
    uint8_t * mem = (uint8_t *) ptr;

    printf("\n");
    printf("[%lld] ", (int64_t) ptr);
    for (int i = 0; i < len; ++i) {
        if (i > 0 && i % 8 == 0) {
            printf(" ;; ");
        }
        printf("|%d| ", mem[i]);
    }
    printf("\n");
}

$main main(int argslen, cstr_t args[]) {
    assert(argslen > 0);

    cstr_t x = args[0];
    printf("%s", x);
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

void print_mem(void * ptr, i32 len) {
    u8 * mem = (u8 *) ptr;

    printf("\n");
    printf("[%lld] ", (i64) ptr);
    for (i32 i = 0; i < len; ++i) {
        if (i > 0 && i % 8 == 0) {
            printf(" ;; ");
        }
        printf("|%d| ", mem[i]);
    }
    printf("\n");
}

$main main(i32 argslen, cstr args[]) {
    _assert_(argslen > 0);

    cstr x = args[0];
    printf("%s", x);
    
    return 0;
}

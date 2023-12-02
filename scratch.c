#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

_proc print_mem(void * ptr, i32 len) {
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

_main main(i32 argslen, cstr args[]) {
    cstr x = argslen > 0 ? args[0] : "";

    assert(argslen > 0);

    printf("%s\n", x);

    #ifdef _MSC_VER
        printf("MSVC!");
    #endif 
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

_math i8 ht_get_exp_from_cap(i32 int32) {
    u32 high = (u32) int32; 
    i8 exp = 6;
    high >> exp;
    
    while(high >>= 1) {
        ++exp;
    }
    return exp;
}

_main main(i32 argslen, cstr args[]) {
    cstr x = argslen > 0 ? args[0] : "";

    assert(argslen > 0);

    printf("%s\n", x);


    
    return 0;
}

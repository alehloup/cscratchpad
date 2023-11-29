#include <stdio.h>
#include "ale.h"

i32 main() {
    printf("%lld\n", cstr_to_num("-10"));

    printf("Alex%lld Sarah%lld Z%lld", 
    cstrlen("Alex"), cstrlen("Sarah"), cstrlen(""));

    return 0;
}

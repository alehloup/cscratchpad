#include <stdio.h>
#include "ale.h"

static char MACRO_scoped;
//TRICK scope that "opens" at start, and "closes" at end 
#define scoped(start, end) MACRO_scoped = 1; for(start; MACRO_scoped; (--MACRO_scoped), end)

i32 main(void) {

    scoped(long long x = (printf("inicio\n"), 5), printf("fim\n")) {
        printf("Meio! 1\n");
        printf("Meio! %lld\n", x);
        printf("Meio! 3\n");
    }

    scoped(printf("inicio\n"), printf("fim\n")) {
        printf("Meio!\n");
    }

    scoped(cstr arquivo = "Fechando\n", printf("%s", arquivo)) {
        printf("Meio do arquivo\n");
    }

    return 0;
}

#include <stdio.h>
#include "ale.h"

static char MACRO_scoped__;
//TRICK scope that "opens" at start, and "closes" at end 
#define scoped(start, end) MACRO_scoped__ = 1; for(start; MACRO_scoped__; (--MACRO_scoped__), end)

int32_t main() {

    scoped(long long x = (printf("inicio\n"), 5), printf("fim\n")) {
        printf("Meio! 1\n");
        printf("Meio! %lld\n", x);
        printf("Meio! 3\n");
    }

    scoped(printf("inicio\n"), printf("fim\n")) {
        printf("Meio!\n");
    }

    scoped(cstr_t arquivo = "Fechando\n", printf("%s", arquivo)) {
        printf("Meio do arquivo\n");
    }

    return 0;
}

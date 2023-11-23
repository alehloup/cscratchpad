#include <stdio.h>
#include "ale.h"

static char MACRO_scoped__;
//TRICK scope that "opens" at start, and "closes" at end 
#define scoped(start, end) MACRO_scoped__ = 1; for(start; MACRO_scoped__; (--MACRO_scoped__), end)

int32_t main() {

    scoped(long long x = (ale_printf("inicio\n"), 5), ale_printf("fim\n")) {
        ale_printf("Meio! 1\n");
        ale_printf("Meio! %lld\n", x);
        ale_printf("Meio! 3\n");
    }

    scoped(ale_printf("inicio\n"), ale_printf("fim\n")) {
        ale_printf("Meio!\n");
    }

    scoped(cstring arquivo = "Fechando\n", ale_printf("%s", arquivo)) {
        ale_printf("Meio do arquivo\n");
    }

    return 0;
}

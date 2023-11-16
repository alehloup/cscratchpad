#include <stdio.h>
#include "ale.h"

int32_t main(int32_t argc, const cstring *argv) {

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

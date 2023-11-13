#include <stdio.h>
#include "ale.h"

int32_t main(int32_t argc, const cstring *argv) {

    scoped(long long x = (printf("inicio\n"), 5), printf("fim\n")) {
        printf("Meio! 1\n");
        printf("Meio! %lld\n", x);
        printf("Meio! 3\n");
    }

    scoped(printf("inicio\n"), printf("fim\n")) {
        printf("Meio!\n");
    }

    scoped(cstring arquivo = "Fechando\n", printf("%s", arquivo)) {
        printf("Meio do arquivo\n");
    }

    return 0;
}

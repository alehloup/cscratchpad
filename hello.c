#include "ale.h"

int32_t xisdigit(char c)
{
    return c>='0' && c<='9';
}

int32_t main(int32_t argc, const cstring *argv) {
    cstring arg = (cstring) argv[argc-1];

    printf("%s %d\n", arg, cstrlen(arg));

    printf("%s %d\n", "Alessandro", cstrlen("Alessandro"));
    printf("%s %d\n", s8("AlexSlice").data, s8("AlexSlice").len);

    scoped(long long x = (printf("inicio\n"), 5), printf("fim\n")) {
        printf("Meio! 1\n");
        printf("Meio! %lld\n", x);
        printf("Meio! 3\n");
    }

    scoped(printf("inicio\n"), printf("fim\n")) {
        printf("Meio!\n");
    }

    scoped(staticstring arquivo = "Fechando\n", printf("%s", arquivo)) {
        printf("Meio do arquivo\n");
    }   

    printf("cão maçã: %d\n", !strcmp("cão", "maçã"));
    printf("cão cão: %d\n", !strcmp("cão", "cão"));
    printf("maçã cão: %d\n", !strcmp("maçã", "cão"));
    printf("maçã maçã: %d\n", !strcmp("maçã", "maçã"));

    return 0;
}

#include "ale.h"

int xisdigit(char c)
{
    return c>='0' && c<='9';
}

int main(int argc, char **argv) {
    char* arg = argv[argc-1];
    s8 string = s(arg);
    //printf("%lld %s\n", string.len, string.data);
    //printf("%d\n", xisdigit(string.data[0]));

    print_s8(string); printn;

    scoped(long long x = (printf("inicio\n"), 5), printf("fim\n")) {
        printf("Meio! 1\n");
        printf("Meio! %lld\n", x);
        printf("Meio! 3\n");
    }

    scoped(printf("inicio\n"), printf("fim\n")) {
        printf("Meio!\n");
    }

    scoped(s8 arquivo = s("Fechando\n"), printf("%s", arquivo.data)) {
        printf("Meio do arquivo\n");
    }   

    printf("cão maçã: %d\n", s8equal(s("cão"), s("maçã")));
    printf("cão cão: %d\n", s8equal(s("cão"), s("cão")));
    printf("maçã cão: %d\n", s8equal(s("maçã"), s("cão")));
    printf("maçã maçã: %d\n", s8equal(s("maçã"), s("maçã")));

    return 0;
}

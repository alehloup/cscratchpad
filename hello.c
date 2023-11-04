#include <stdio.h>
#include <string.h>
#include "ale.h"

int xisdigit(char c)
{
    return c>='0' && c<='9';
}

int main(int argc, char **argv) {
    char* arg = argv[argc-1];
    s8 string = s8(arg);
    printf("%llu %s\n", string.len, string.data);
    printf("%d\n", xisdigit(string.data[0]));

    scoped(i64 x = (printf("inicio\n"), 5), printf("fim\n")) {
        printf("Meio! 1\n");
        printf("Meio! %lld\n", x);
        printf("Meio! 3\n");
    }

    scoped(printf("inicio\n"), printf("fim\n")) {
        printf("Meio!\n");
    }

    scoped(s8 arquivo = s8("Fechando\n"), printf("%s", arquivo.data)) {
        printf("Meio do arquivo\n");
    }   


    return 0;
}

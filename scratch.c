#include <stdio.h>
#include "ale.h"

static unsigned char table[536870913] = {};

int32_t main(int32_t argc, cstring *argv) {

    unsigned char *S = table; //calloc(536870913, sizeof(char));
    S[536870910] = 5;
    S[536870909] = 2;

    printf("d: %d", S[536870910]);

    int i = 0;
    for (; !S[i]; ++i) {
        /* empty */
    }
    printf(" d: %d", S[536870909]);

    //free(S);

    return 0;
}

#include <stdio.h>
#include "ale.h"

int32_t main(int32_t argc, cstring *argv) {

    unsigned char *S = calloc(268435457, sizeof(char));
    S[268435456] = 5;
    S[268435455] = 2;

    printf("d: %d", S[268435456]);

    int i = 0;
    for (; !S[i]; ++i) {
        /* empty */
    }
    printf(" d: %d", S[268435455]);

    free(S);

    return 0;
}

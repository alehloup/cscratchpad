#include <stdio.h>
#include "ale.h"

int32_t main(int32_t argc, cstring *argv) {

    static char s1[64] = Zero;
    static char s2[64] = Zero;
    scanf("%s %s", s1, s2);
    printf("Aha: %s %s", s1, s2);

    return 0;
}

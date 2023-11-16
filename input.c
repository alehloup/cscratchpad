#include <stdio.h>
#include "ale.h"

int32_t main(int32_t argc, cstring *argv) {

    static char s1[64] = {};
    static char s2[64] = {};
    scanf("%s %s", s1, s2);
    ale_printf("Aha: %s %s", s1, s2);

    return 0;
}

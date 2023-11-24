#include <stdio.h>
#include "ale.h"

int32_t main() {

    static char s1[64] = {0};
    static char s2[64] = {0};
    scanf("%s %s", s1, s2);
    ale_printf("Aha: %s %s", s1, s2);

    return 0;
}

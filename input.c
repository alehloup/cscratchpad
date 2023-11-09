#include "ale.h"

int main(int argc, char **argv) {

    static char s1[64] = {0};
    static char s2[64] = {0};
    scanf("%s %s", s1, s2);
    printf("Aha: %s %s", s1, s1);

    return 0;
}

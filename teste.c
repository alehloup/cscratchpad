#include "aleh/aleh.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(void) {
    char const * const s11 = "hello";          // 5 (literal, const char *)
    int vec[] = {1, 2, 3, 4};          // 4 (array de int)

    const char *str1 = "Hello, world!";
    char arr[] = {1, 2, 3, 4};
    char const *str2 = "Dynamic string";

    // Testando o countof e lengthof
    printf("countof(arr): %zd\n", countof(arr));   // Espera 4 (array de tamanho fixo)
    printf("lengthof(str1): %zd\n", lengthof(str1)); // Espera 13 (string literal)
    printf("lengthof(str2): %zd\n", lengthof(str2)); // Espera 14 (string dinâmica)

    printf("vec %zd\n", countof(vec));
    printf("não é literal... %zd\n", lengthof(s11));
    printf("É literal %zd\n", lengthof("É literal"));

    char *s = 0;
    printf("lengthof(s): %zd\n", lengthof(s)); // nulo, 0

    return 0;
}


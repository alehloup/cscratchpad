#include <stdio.h>
#include "ale.h"

_fun i8 named_digit(cstr string) {
    if (string[0] && is_digit(string[0])) {
        return string[0] - '0';
    }

    cstr named[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    for (i8 i = 0; i < 10; ++i) {
        if (startswith(string, named[i])) {
            return i;
        }
    }

    return -1;
}

i32 main(void) {
    printf("%lld\n", cstr_to_num("-10"));

    printf("Alex%lld Sarah%lld Z%lld\n", 
    cstrlen("Alex"), cstrlen("Sarah"), cstrlen(""));

    printf("%s Starts with %s : %d\n", "Sarah Sakamoto", "Sarah", 
        startswith("Sarah Sakamoto", "Sarah"));

    printf("%s Starts with %s : %d\n", "Sarah Sakamoto", "Sara ", 
        startswith("Sarah Sakamoto", "Sara "));

    printf("%s Starts with %s : %d\n", &("Sarah Sakamoto com outras palavras"[6]), "Sakamoto", 
        startswith(&("Sarah Sakamoto com outras palavras"[6]), "Sakamoto"));

    {
        cstr examples[] = {"5dois", "three6433", "nine3453", "woa", "zero5"};
        for (i8 i = 0; i < (i8) (sizeof(examples)/sizeof(cstr)); ++i) {
            printf("%s : %d\n", examples[i], named_digit(examples[i]));
        }
    }

    return 0;
}

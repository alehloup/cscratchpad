#include <stdio.h>
#include "ale.h"

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

    {
        u8 buffer[MBs_ / 8] = {0};
        arena_t arena = newarena(MBs_ / 8, buffer);
        vector64_t words = {0, 0, 0};
        char teste[16] = {'A', 'l', 'e', ' ', 'M', 'a', 't', ' ', '\0'};

        words = slice_by_splitter(&arena, teste, ' ');
        for (int i = 0; i < words.len; ++i) {
            printf("word: %s\n", (cstr) words.data[i]);
        }
    }

    return 0;
}

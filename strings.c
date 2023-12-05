#include <stdio.h>
#include "ale.h"

i32 main(void) {
    printf("%lld\n", cstr_to_num("-10").num);

    printf("Alex%lld Sarah%lld Z%lld\n", 
    cstrlen("Alex"), cstrlen("Sarah"), cstrlen(""));

    printf("%s Starts with %s : %d\n", "Sarah Sakamoto", "Sarah", 
        startswith("Sarah Sakamoto", "Sarah"));

    printf("%s Starts with %s : %d\n", "Sarah Sakamoto", "Sara ", 
        startswith("Sarah Sakamoto", "Sara "));

    printf("%s Starts with %s : %d\n", &("Sarah Sakamoto com outras palavras"[6]), "Sakamoto", 
        startswith(&("Sarah Sakamoto com outras palavras"[6]), "Sakamoto"));

    {
        ccstr examples[] = {"5dois", "three6433", "nine3453", "woa", "zero5"};
        for (i8 i = 0; i < (i8) (sizeof(examples)/sizeof(cstr)); ++i) {
            printf("%s : %d\n", examples[i], named_digit(examples[i]));
        }
    }

    {
        u8 buffer[MBs_ / 8] = {0};
        Arena arena = newarena(MBs_ / 8, buffer);
        mstr *words = VNEW(&arena, mstr);
        char teste[16] = {'A', 'l', 'e', ' ', 'M', 'a', 't', ' ', '\0'};

        words = mutslice_by_splitter(&arena, teste, ' ');
        for (int i = 0; i < hd_(words)->len; ++i) {
            printf("word: %s\n", words[i]);
        }
    }

    {
        ccstr ccstrs[4] = {"123 456 789", "52#89;", "69 . 3", " 91"};
        for (int i = 0; i < 4; ++i) {
            i64num_i64len num_len = cstr_to_num(ccstrs[i]);
            printf("\"%s\" |%lld| #%lld\n", ccstrs[i], num_len.num, num_len.len);
        }
    }

    return 0;
}

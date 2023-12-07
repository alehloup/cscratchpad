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
        u8 buffer[MBs_ / 8] = {0};
        Arena arena = new_arena(MBs_ / 8, buffer);
        mstr *words = NEW_VEC(&arena, mstr);
        char teste[16] = {'A', 'l', 'e', ' ', 'M', 'a', 't', ' ', '\0'};

        words = split(&arena, teste, ' ');
        for (int i = 0; i < hd_(words)->len; ++i) {
            printf("word: %s\n", words[i]);
        }
    }

    {
        ccstr ccstrs[4] = {"123 456 789", "52#89;", "69 . 3", " 91"};
        for (int i = 0; i < 4; ++i) {
            i64num_i32len num_len = cstr_to_num(ccstrs[i]);
            printf("\"%s\" |%lld| #%lld\n", ccstrs[i], num_len.num, num_len.len);
        }
    }

    {
        i32 x = 52, y = 69, alex = 2019912801;
        union {struct {char a;char b;char c;char d;} s; i32 i;}  z = {{'a', 'l', 'e', 'x'}}; 
        printf("%d %d : %d\n", x, y, cmpmem(&x, &y, isizeof(x)));
        printf("%d %d : %d\n", x, x, cmpmem(&x, &x, isizeof(x)));
        printf("%d %d : %d\n", y, y, cmpmem(&y, &y, isizeof(y)));
        printf("%d %c%c%c%c : %d\n", x, z.s.a,z.s.b,z.s.c,z.s.d, cmpmem(&x, &z, isizeof(x)));
        printf("%d %d : %d\n", alex, z.i, cmpmem(&alex, &z, isizeof(alex)));
        printf("%d %d : %d\n", alex, y, cmpmem(&alex, &y, isizeof(y)));
    }

    return 0;
}

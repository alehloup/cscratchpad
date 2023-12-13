#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../ale.h"

i32 main(i32 argc, ccstr*argv) {
    char buffer[512];
    ccstr _1_ = argc > 1 ? argv[1] : "";

    if (argc < 1) {
        printf("No args...");
    }

    printf(
        (
            " msvc"
            " && clang-cl" 
            " -Ofast -march=native -flto -fuse-ld=lld -ftrivial-auto-var-init=zero"
            " -Weverything -Werror -fsanitize-trap -fsanitize=undefined -Wno-unsafe-buffer-usage -Wno-unused-function"
            " %s -o \"./%s.exe\"" 
            " && \"./%s.exe\""
            " && rm \"./%s.exe\""
            " \n"
        ), 
        _1_, _1_, _1_, _1_
    );

    printf("\n");
    
    shellrun(isizeof(buffer), buffer, 
        (
            " msvc"
            " && clang-cl" 
            " -Ofast -march=native -flto -fuse-ld=lld -ftrivial-auto-var-init=zero"
            " -Weverything -Werror -fsanitize-trap -fsanitize=undefined -Wno-unsafe-buffer-usage -Wno-unused-function"
            " %s -o \"./%s.exe\"" 
            " && \"./%s.exe\""
            " && rm \"./%s.exe\""
        ),
        _1_, _1_, _1_, _1_
    );

    return 0;
}

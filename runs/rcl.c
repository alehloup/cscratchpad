#include <stdio.h>
#include <stdlib.h>
#include "../ale.h"

i32 main(i32 argc, ccstr*argv) {
    ccstr _1_ = argc > 1 ? argv[1] : "";
    
    shellrun(
        " msvc"
        " && clang-cl" 
        " -Ofast -march=native -flto -fuse-ld=lld -ftrivial-auto-var-init=zero"
        " -Weverything -Werror -fsanitize-trap -fsanitize=undefined -Wno-unsafe-buffer-usage -Wno-unused-function"
        " %s -o \"./%s.exe\"" 
        " && \"./%s.exe\""
        " && rm \"./%s.exe\""
        "\n"
        ,
        _1_, _1_, _1_, _1_
    );

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "../ale.h"

int32_t main(int32_t argc, Mstr *argv) {
    const char* const _1_c = argc > 1 ? argv[1] : "\0";
    char _1_[512] = {0};
    for (int32_t i = 0; _1_c[i] and _1_c[i] != '.'; ++i) {
        _1_[i] = _1_c[i];
    }
    
    shellrun(
        "msvc"
        " && clang" 
        " -std=gnu2x -Ofast -march=native -flto -fuse-ld=lld -ftrivial-auto-var-init=zero"
        " -Weverything -Werror -fsanitize-trap -fsanitize=undefined" 
        " -Wno-pre-c2x-compat -Wno-unsafe-buffer-usage -Wno-unused-function -Wno-declaration-after-statement"
        " -D_CRT_SECURE_NO_WARNINGS"
        " %s -o \"./%s.exe\"" 
        " && \"./%s.exe\""
        " && rm \"./%s.exe\""
        "\n"
        ,
        _1_c, _1_, _1_, _1_
    );

    return 0;
}

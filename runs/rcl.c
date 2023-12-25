#include <stdio.h>
#include <stdlib.h>
#include "../ale.h"

int main(int argc, mstr *argv) {
    ccstr _1_c = argc > 1 ? argv[1] : "\0";
    char _1_[512] = {0};
    for (int i = 0; _1_c[i] and _1_c[i] != '.'; ++i) {
        _1_[i] = _1_c[i];
    }
    
    shellrun(
        " clang" 
        " -Ofast -march=native -flto -fuse-ld=lld -ftrivial-auto-var-init=zero"
        " -Weverything -Werror -fsanitize-trap -fsanitize=undefined -Wno-unsafe-buffer-usage -Wno-unused-function"
        " %s -o \"./%s.exe\"" 
        " && \"./%s.exe\""
        " && rm \"./%s.exe\""
        "\n"
        ,
        _1_c, _1_, _1_, _1_
    );

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "../ale.h"

int main(int argc, ccstr*argv) {
    ccstr _1_c = argc > 1 ? argv[1] : "\0";
    char _1_[512] = {0};
    for (int i = 0; _1_c[i] and _1_c[i] != '.'; ++i) {
        _1_[i] = _1_c[i];
    }
    
    shellrun(
        " g++" 
        " -Ofast -march=native -static-pie -flto"
        " -D_FORTIFY_SOURCE=3 -g3 -fcf-protection=full -fstack-protector-strong -ftrivial-auto-var-init=zero"
        " -Wall -Wextra -Wpedantic -Werror -fsanitize-trap -fsanitize=undefined -fanalyzer"
        " -Wcast-align=strict -Wnull-dereference -Wwrite-strings -Wformat-signedness -Wconversion"
        " -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wswitch-default -Wswitch-enum"
        " -fno-exceptions"
        " %s -o \"./%s.exe\"" 
        " && \"./%s.exe\""
        " && rm \"./%s.exe\""
        "\n"
        ,
        _1_c, _1_, _1_, _1_
    );

    return 0;
}

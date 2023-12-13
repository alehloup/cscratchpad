#include <stdio.h>
#include <stdlib.h>
#include "../ale.h"

i32 main(i32 argc, ccstr*argv) {
    ccstr _1_ = argc > 1 ? argv[1] : "";
    
    shellrun(
        " gcc"
        " -Ofast -march=native -static-pie -flto"
        " -D_FORTIFY_SOURCE=3 -g3 -fcf-protection=full -fstack-protector-strong -ftrivial-auto-var-init=zero"
        " -Wall -Wextra -Wpedantic -Werror -fsanitize-trap -fsanitize=undefined -fanalyzer"
        " -Wcast-align=strict -Wnull-dereference -Wwrite-strings -Wformat-signedness -Wconversion"
        " -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wswitch-default -Wswitch-enum"
        " %s -o \"./%s.exe\"" 
        " && \"./%s.exe\""
        " && rm \"./%s.exe\""
        "\n"
        ,
        _1_, _1_, _1_, _1_
    );

    return 0;
}

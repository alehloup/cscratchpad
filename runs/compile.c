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
        " -Wstack-usage=32768 -Wframe-larger-than=16384"
        " %s -o \"./%s.exe\"" 
        "\n"
        ,
        _1_, _1_
    );

    return 0;
}

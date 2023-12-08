#include "../ale_io.h"

i32 main(i32 argc, ccstr*argv) {
    char buffer[512];
    ccstr _1_ = argc > 1 ? argv[1] : "";

    if (argc < 1) {
        printf("No args...");
    }

    printf(
        (
            " gcc"
            " -Ofast -march=native -static-pie -flto"
            " -D_FORTIFY_SOURCE=3 -g3 -fcf-protection=full -fstack-protector-strong -ftrivial-auto-var-init=zero"
            " -Wall -Wextra -Wpedantic -Werror -fsanitize-trap -fsanitize=undefined -fanalyzer"
            " -Wcast-align=strict -Wnull-dereference -Wwrite-strings -Wformat-signedness -Wconversion"
            " -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wswitch-default -Wswitch-enum"
            " %s -o \"./%s.exe\"" 
            " && \"./%s.exe\""
            " && rm \"./%s.exe\""
        ), 
        _1_, _1_, _1_, _1_
    );

    printf("\n");
    
    shellrun(isizeof(buffer), buffer, 
        (
            " gcc"
            " -Ofast -march=native -static-pie -flto"
            " -D_FORTIFY_SOURCE=3 -g3 -fcf-protection=full -fstack-protector-strong -ftrivial-auto-var-init=zero"
            " -Wall -Wextra -Wpedantic -Werror -fsanitize-trap -fsanitize=undefined -fanalyzer"
            " -Wcast-align=strict -Wnull-dereference -Wwrite-strings -Wformat-signedness -Wconversion"
            " -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wswitch-default -Wswitch-enum"
            " %s -o \"./%s.exe\"" 
            " && \"./%s.exe\""
            " && rm \"./%s.exe\""
        ),
        _1_, _1_, _1_, _1_
    );

    return 0;
}

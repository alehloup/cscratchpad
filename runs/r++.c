#include <stdio.h>
#include <stdlib.h>
#include "../ale.h"

int32_t main(int32_t argc, const char *const *argv) {
    int32_t success =
        compile_run_c(
             " g++" 
        " -std=gnu++23 -Ofast -march=native -static-pie -flto"
        " -D_FORTIFY_SOURCE=3 -g3 -fcf-protection=full -fstack-protector-strong -ftrivial-auto-var-init=zero"
        " -Wall -Wextra -Wpedantic -Werror -fsanitize-trap -fsanitize=undefined -fanalyzer"
        " -Wcast-align=strict -Wnull-dereference -Wwrite-strings -Wformat-signedness -Wconversion"
        " -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wswitch-default -Wswitch-enum"
        " -fno-exceptions",
        
            argc > 1 ? argv[1] : "\0"
        );

    return success;
}

#include <stdio.h>
#include <stdlib.h>
#include "../ale.h"

int32_t main(int32_t argc, const char *const *argv) {
    if (argc < 1) return 0;

    int32_t success =
        compile_run_c(
              "msvc"
        " && clang" 
        " -std=gnu2x -Ofast -march=native -flto -fuse-ld=lld -ftrivial-auto-var-init=zero"
        " -Weverything -Werror -fsanitize-trap -fsanitize=undefined" 
        " -Wno-pre-c2x-compat -Wno-unsafe-buffer-usage -Wno-unused-function -Wno-declaration-after-statement"
        " -D_CRT_SECURE_NO_WARNINGS",
        
            argv[1]
        );

    return success;
}

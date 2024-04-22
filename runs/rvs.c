#include <stdio.h>
#include <stdlib.h>
#include "../ale.h"

int32_t main(int32_t argc, const char *const *argv) {
    if (argc < 1) return 0;

    int32_t success =
        compile_run_c(
            "msvc && cl /std:clatest /W4 /Ox -D_CRT_SECURE_NO_WARNINGS",
        
            argv[1]
        );

    return success;
}

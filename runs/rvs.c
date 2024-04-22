#include <stdio.h>
#include <stdlib.h>
#include "../ale.h"

int32_t main(int32_t argc, const char *const *argv) {
    int32_t success =
        compile_run_c(
            "msvc && cl /std:clatest /W4 /Ox -D_CRT_SECURE_NO_WARNINGS",
        
            argc > 1 ? argv[1] : "\0"
        );

    return success;
}

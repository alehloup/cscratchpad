#include "ale.h"

int32_t main(int32_t argc, const char *const *argv) {
    const char * compiler = "";
    const char * filename_c = "";
    
    switch (argc) {
        case 0:case 1: 
            printf("no args passed\n"); 
        return 1;

        case 2: 
            compiler = "gcc";
            filename_c=argv[1]; 
        break;

        default: 
            compiler=argv[1]; 
            filename_c=argv[2];
        break;
    }

    const char * flags = "";
    switch (compiler[0]) {

        default:
            flags = 
                "gcc"
                " -std=gnu2x -Ofast -march=native -static-pie -flto"
                " -D_FORTIFY_SOURCE=3 -g3 -fcf-protection=full -fstack-protector-strong -ftrivial-auto-var-init=zero"
                " -Wall -Wextra -Wpedantic -Werror -fsanitize-trap -fsanitize=undefined -fanalyzer"
                " -Wcast-align=strict -Wnull-dereference -Wwrite-strings -Wformat-signedness -Wconversion"
                " -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wswitch-default -Wswitch-enum";
        break;

        case 'p':case 'P':
            flags = 
                "g++" 
                " -std=gnu++23 -Ofast -march=native -static-pie -flto"
                " -D_FORTIFY_SOURCE=3 -g3 -fcf-protection=full -fstack-protector-strong -ftrivial-auto-var-init=zero"
                " -Wall -Wextra -Wpedantic -Werror -fsanitize-trap -fsanitize=undefined -fanalyzer"
                " -Wcast-align=strict -Wnull-dereference -Wwrite-strings -Wformat-signedness -Wconversion"
                " -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wswitch-default -Wswitch-enum"
                " -fno-exceptions";
        break;

        case 'c':case 'C':
            flags =
                "clang" 
                " -std=gnu2x -Ofast -march=native -flto -fuse-ld=lld -ftrivial-auto-var-init=zero"
                " -Weverything -Werror -fsanitize-trap -fsanitize=undefined" 
                " -Wno-pre-c2x-compat -Wno-unsafe-buffer-usage -Wno-unused-function -Wno-declaration-after-statement"
                " -D_CRT_SECURE_NO_WARNINGS";
        break;

        case 'm':case 'M':
            flags =
                "cl /std:clatest /MT /W4 /Ox /analyze /GS /sdl -D_CRT_SECURE_NO_WARNINGS";
        break;

        case 't':case 'T':
            flags =
                "tcc -Wall -Werror";
        break;
    } 
    
    return compile_run_c(filename_c, flags);
}

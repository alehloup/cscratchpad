#include "ale.h"

#if defined(_WIN32) || defined(_WIN64)
    #define FORTIFY " -D_FORTIFY_SOURCE=3"
    #define CLANG_ACCEPT_C_ARRAY_PLS " -Wno-unsafe-buffer-usage"
    #define GCC_ACCEPT_PRAGMA_REGION_PLS ""
#else 
    // GCC errors with "fortify is redefined" so... no fortify
    #define FORTIFY "" 
    #define CLANG_ACCEPT_C_ARRAY_PLS ""
    #define GCC_ACCEPT_PRAGMA_REGION_PLS " -Wno-unknown-pragmas"
#endif

int main(int argc, const char *const *argv) {
    const char * compiler = "";
    const char * filename_c = "";
    const char * flags = "";
    
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

    switch (compiler[0]) {

        default:
            flags = 
                "gcc"
                " -std=gnu2x -Ofast -march=native -static-pie -flto -g3"
                FORTIFY " -fcf-protection=full -fstack-protector-strong"
                " -Wall -Wextra -Wpedantic -Wuninitialized -Werror" 
                " -fsanitize-undefined-trap-on-error -fsanitize=undefined -fanalyzer"
                " -Wcast-align=strict -Wnull-dereference -Wwrite-strings -Wformat-signedness -Wconversion"
                " -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wswitch-default -Wswitch-enum"
                GCC_ACCEPT_PRAGMA_REGION_PLS
            ;
        break;

        case 'p':case 'P':
            flags = 
                "g++" 
                " -std=gnu++23 -Ofast -march=native -static-pie -flto -g3"
                FORTIFY " -fcf-protection=full -fstack-protector-strong"
                " -Wall -Wextra -Wpedantic -Wuninitialized -Werror" 
                " -fsanitize-undefined-trap-on-error -fsanitize=undefined -fanalyzer"
                " -Wcast-align=strict -Wnull-dereference -Wwrite-strings -Wformat-signedness -Wconversion"
                " -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wswitch-default -Wswitch-enum"
                " -fno-exceptions"
                GCC_ACCEPT_PRAGMA_REGION_PLS
            ;
        break;

        case 'c':case 'C':
            flags =
                "clang" 
                " -std=gnu2x -Ofast -march=native -Weverything -Werror" 
                " -fsanitize-undefined-trap-on-error -fsanitize=undefined"
                CLANG_ACCEPT_C_ARRAY_PLS
            ;
        break;

        case 'm':case 'M':
            flags =
                "cl /std:clatest /MT /W4 /Ox /analyze /GS /sdl"
            ;
        break;

        case 't':case 'T':
            flags =
                "tcc -Wall -Werror"
            ;
        break;
    } 
    
    return compile_run_c(filename_c, flags);
}

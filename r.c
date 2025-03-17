#include "aleh/system.h"

/* For enabling Shadow Stacks: */
// https://x86.lol/generic/2024/09/23/user-shadow-stacks.html
// 
// You opt in to shadow stacks using a glibc tunable (an Env Var). When everything works, you’ll see that stack smashing is prevented
// $ GLIBC_TUNABLES=glibc.cpu.hwcaps=SHSTK ./test

// Linux 6.6 or later with CONFIG_X86_USER_SHADOW_STACK=y
// GCC 8 or clang 7 or later && glibc 2.39 or later
// Intel Tiger Lake or later || AMD Zen 3 or later
// GLIBC_TUNABLES=glibc.cpu.hwcaps=SHSTK env var
// -fcf-protection=return (or full)

/* A small bat for activating MSVC environment (For MSVC and MSVC Clang): */
// chcp 65001
// call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64

#if defined(_WIN32) || defined(_WIN64)
    #define FORTIFY " -D_FORTIFY_SOURCE=3"
    #define CLANG_ACCEPT_C_ARRAY_PLS " -Wno-unsafe-buffer-usage"
    #define GCC_ACCEPT_PRAGMA_REGION_PLS ""
#else 
    /* GCC errors with "fortify is redefined" so... no fortify */
    #define FORTIFY "" 
    #define CLANG_ACCEPT_C_ARRAY_PLS ""
    #define GCC_ACCEPT_PRAGMA_REGION_PLS " -Wno-unknown-pragmas"
#endif

#define delete_files "rm *.exe *.out *.tmp *.obj *.nativecodeanalysis.xml *.ilk *.pdb"

static const char *const flags_gcc  = 
    "  gcc"
    " -std=gnu2x -Ofast -march=native -static-pie -flto -g3 -fno-omit-frame-pointer"
    FORTIFY " -fcf-protection=full -fstack-protector-strong"
    " -Wall -Wextra -Wpedantic -Wuninitialized -Werror"
    " -fanalyzer"
    " -fsanitize-undefined-trap-on-error" 
    " -fsanitize=undefined -fsanitize=bounds-strict"
    " -Wcast-align=strict -Wnull-dereference -Wwrite-strings -Wformat-signedness -Wconversion"
    " -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wswitch-default -Wswitch-enum"
    " -Wshadow -Wdouble-promotion -Wvla"
    GCC_ACCEPT_PRAGMA_REGION_PLS
;

static const char *const flags_gpp = 
    "  g++" 
    " -std=gnu++23 -Ofast -march=native -static-pie -flto -g3 -fno-omit-frame-pointer"
    FORTIFY " -fcf-protection=full -fstack-protector-strong"
    " -Wall -Wextra -Wpedantic -Wuninitialized -Werror" 
    " -fanalyzer"
    " -fsanitize-undefined-trap-on-error" 
    " -fsanitize=undefined -fsanitize=bounds-strict"
    " -Wcast-align=strict -Wnull-dereference -Wwrite-strings -Wformat-signedness -Wconversion"
    " -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wswitch-default -Wswitch-enum"
    " -Wshadow -Wdouble-promotion -Wvla"
    " -fno-exceptions"
    GCC_ACCEPT_PRAGMA_REGION_PLS
;

static const char *const flags_tinyc = 
    "  tcc -Wall -Werror"
;

static const char *const flags_msvc =
    "  cl /std:clatest /MT /W4 /Ox /analyze /GS /sdl /D_CRT_SECURE_NO_WARNINGS"
;

static const char *const flags_clang = 
    "  clang" 
    " -std=gnu2x -Ofast -march=native -g3 -fno-omit-frame-pointer"
    " -Weverything -Werror" 
    " -fsanitize-undefined-trap-on-error" 
    " -fsanitize=undefined -fsanitize=bounds"
    " -Wshadow -Wdouble-promotion"
    " -D_CRT_SECURE_NO_WARNINGS -Wno-pre-c2x-compat"
    CLANG_ACCEPT_C_ARRAY_PLS
;

static void delete_artifacts(void) {
    int discard = printf("\n===== Delete Artifacts =====\n");
    discard = printf(delete_files); printf("\n");
    discard = system(delete_files);
    (void) discard;
}

int main(int argc, const char *const *argv) {
    const char * compiler = "";
    const char * filename_c = "";
    const char * flags = "";
    int printed = 0, success = 1;

    printf("\n");
    
    switch (argc) {
        case 0:case 1: 
            printf("no args passed\n"); 
        return 1;

        case 2: 
            compiler = "gcc";
            filename_c=argv[1]; 

            if (filename_c[0] == 'd' && filename_c[1] == '\0') {
                delete_artifacts();
                return 0;
            }
        break;

        default: 
            compiler=argv[1]; 
            filename_c=argv[2];
        break;
    }

    switch (compiler[0]) {

        default:
            flags = flags_gcc;
        break;

        case 'p':case 'P':
            flags = flags_gpp;
        break;

        case 't':case 'T':
            flags = flags_tinyc;
        break;

        case 'm':case 'M':
            flags = flags_msvc;
        break;

        case 'c':case 'C':
            flags = flags_clang;
        break;

        case 'a':case 'A':
            flags = "all";
        break;

        case 'd':case 'D':
            return system(delete_files);
        /* break; */
    } 

    switch (flags[0]) {
        default: 
            success = compile_run_c(filename_c, flags);

            if (success == 0) {
                // printf("\n\nDone, SUCCESS\n\n");
            } else {
                printf("\n\nDone, ERROR!\n\n");
            }
        
            return success
        ;

        case 'a':
            printed = printf("\n===== GCC =====");
            success = compile_run_c(filename_c, flags_gcc);
            printed = printf("gcc=%d %s\n", success, success == 0? "success" : "error");
            printed = printf("\n===== G++ =====");
            success = compile_run_c(filename_c, flags_gpp);
            printed = printf("g++=%d %s\n", success, success == 0? "success" : "error");
            printed = printf("\n===== TCC =====");
            success = compile_run_c(filename_c, flags_tinyc);
            printed = printf("tcc=%d %s\n", success, success == 0? "success" : "error");
            printed = printf("\n===== MSVC =====");
            success = compile_run_c(filename_c, flags_msvc);
            printed = printf("msvc=%d %s\n", success, success == 0? "success" : "error");
            printed = printf("\n===== Clang =====");
            success = compile_run_c(filename_c, flags_clang);
            printed = printf("clang=%d %s\n", success, success == 0? "success" : "error");

            delete_artifacts();

            (void) printed;

            if (success == 0) {
                // printf("\n\nDone, SUCCESS\n\n");
            } else {
                printf("\n\nDone, ERROR!\n\n");
            }
        
            return success
        ;
    }
}

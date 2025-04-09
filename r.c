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

// ===== OPTIMIZATION & BASE FLAGS =====
#define FLAGS_OPTIMIZE \
    " -std=gnu2x -O3 -march=native -g3 -fno-omit-frame-pointer"

// ===== MEMORY & SAFETY FLAGS =====
#define FLAGS_MEMORY \
    " -fstack-protector-strong -fcf-protection=full" \
    " -Wvla -fstrict-flex-arrays=3" \
    " -ftrivial-auto-var-init=zero" \
    " -fno-delete-null-pointer-checks -fno-strict-aliasing -fwrapv" \
    " -Wnull-dereference -Wwrite-strings" \
    " -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3"

// ===== WARNING FLAGS =====
#define FLAGS_WARNING \
    " -Wall -Wextra -Wpedantic -Wuninitialized -Werror -Werror=implicit" \
    " -Werror=incompatible-pointer-types -Wconversion -Werror=int-conversion" \
    " -Wshadow -Wdouble-promotion -Wformat=2 -Wswitch-default -Wswitch-enum"

// ===== SANITIZERS (Common to GCC and Clang) =====
#define FLAGS_SANITIZE_COMMON \
    " -fsanitize-undefined-trap-on-error -fsanitize=undefined"

// ===== COMMON FLAGS FOR GCC/Clang =====
#define FLAGS_COMMON \
    FLAGS_OPTIMIZE \
    FLAGS_MEMORY \
    FLAGS_WARNING \
    FLAGS_SANITIZE_COMMON

// ===== LINKER FLAGS =====
#ifdef _WIN32
    #define FLAGS_LINKER ""
    #define FLAGS_NO_MSVC_CRT_WARNINGS " -D_CRT_SECURE_NO_WARNINGS"
#else
    #define FLAGS_LINKER \
        " -flto" \
        " -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now" \
        " -Wl,--as-needed -Wl,--no-copy-dt-needed-entries"
    #define FLAGS_NO_MSVC_CRT_WARNINGS ""
#endif


static const char *const flags_gcc =
    " gcc" \
    "  -fno-strict-overflow -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wbidi-chars=any" \
    FLAGS_COMMON " -Wcast-align=strict -fsanitize=bounds-strict -fanalyzer" \
    FLAGS_LINKER;

static const char *const flags_clang =
    " clang" \
    " -Weverything -Wno-pre-c2x-compat -Wno-unsafe-buffer-usage" FLAGS_NO_MSVC_CRT_WARNINGS \
    FLAGS_COMMON " -Wcast-align -fsanitize=bounds" \
    FLAGS_LINKER;

static const char *const flags_tinyc = 
    " tcc -std=c11 -Wall -Werror";



#define delete_files "rm *.exe *.out *.tmp *.obj *.nativecodeanalysis.xml *.ilk *.pdb 2>/dev/null"

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

        case 'c':case 'C':
            flags = flags_clang;
        break;

        case 't':case 'T':
            flags = flags_tinyc;
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

            printed = printf("\n===== Clang =====");
            success = compile_run_c(filename_c, flags_clang);
            printed = printf("clang=%d %s\n", success, success == 0? "success" : "error");

            printed = printf("\n===== TCC =====");
            success = compile_run_c(filename_c, flags_tinyc);
            printed = printf("tcc=%d %s\n", success, success == 0? "success" : "error");

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

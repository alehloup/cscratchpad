#include "aleh/system.h"

// For Shadow Stacks: CONFIG_X86_USER_SHADOW_STACK=y and GLIBC_TUNABLES=glibc.cpu.hwcaps=SHSTK

// ===== OPTIMIZATION & BASE FLAGS =====
#define FLAGS_OPTIMIZE \
    " -std=gnu2x -O3 -march=native -g3 -fno-omit-frame-pointer"

// ===== MEMORY & SAFETY FLAGS =====
#define FLAGS_MEMORY \
    " -fstack-protector-strong -fcf-protection=full" \
    " -Wvla -fstrict-flex-arrays=3" \
    " -ftrivial-auto-var-init=zero" \
    " -Wcast-align -fsanitize=bounds" \
    " -fno-delete-null-pointer-checks -fno-strict-aliasing -fwrapv" \
    " -Wnull-dereference -Wwrite-strings"

// ===== WARNING FLAGS =====
#define FLAGS_WARNING \
    " -Wall -Wextra -Wuninitialized -Werror -Werror=implicit" \
    " -Werror=incompatible-pointer-types -Wconversion -Werror=int-conversion" \
    " -Wshadow -fno-common -Wdouble-promotion -Wformat=2 -Wswitch-default -Wswitch-enum" \
    " -Wno-cast-qual -Wno-declaration-after-statement"

// ===== SANITIZERS (Common to GCC and Clang) =====
#define FLAGS_SANITIZE_COMMON \
    " -fsanitize-undefined-trap-on-error -fsanitize=undefined"

// ===== LINKER FLAGS =====
#ifdef _WIN32
    #define FLAGS_LIN ""
#else
    #define FLAGS_LIN \
        " -flto -fPIE -pie -fstack-clash-protection" \
        " -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now" \
        " -Wl,--as-needed -Wl,--no-copy-dt-needed-entries"
#endif

// ===== COMMON FLAGS FOR GCC/Clang =====
#define FLAGS \
    FLAGS_OPTIMIZE \
    FLAGS_MEMORY \
    FLAGS_WARNING \
    FLAGS_SANITIZE_COMMON \
    FLAGS_LIN

static const char *const flags_gcc = " gcc -fanalyzer " FLAGS;
static const char *const flags_clang = " clang " FLAGS;
static const char *const flags_tinyc = " tcc -std=c11 -Wall -Werror";


static void delete_artifacts(void) {
    (void) printf("\n===== Delete Artifacts =====\n");
    (void) printf("rm *.exe *.out *.tmp *.obj *.nativecodeanalysis.xml *.ilk *.pdb 2>/dev/null"); 
    printf("\n");
    (void) system("rm *.exe *.out *.tmp *.obj *.nativecodeanalysis.xml *.ilk *.pdb 2>/dev/null");
}

int main(int argc, const char *const *argv) {
    const char * compiler = "";
    const char * filename_c = "";
    const char * flags = "";
    int success = 1;

    #ifdef _WIN32
        system("chcp 65001");
    #endif 

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
        default:           flags = flags_gcc;   break;
        case 'c':case 'C': flags = flags_clang; break;
        case 't':case 'T': flags = flags_tinyc; break;
        case 'a':case 'A': flags = "all";       break; //used in next switch
        case 'd':case 'D': delete_artifacts();//break
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
            (void)printf("\n===== GCC =====");
            success = compile_run_c(filename_c, flags_gcc);
            (void)printf("gcc=%d %s\n", success, success == 0? "success" : "error");

            (void)printf("\n===== Clang =====");
            success = compile_run_c(filename_c, flags_clang);
            (void)printf("clang=%d %s\n", success, success == 0? "success" : "error");

            (void)printf("\n===== TCC =====");
            success = compile_run_c(filename_c, flags_tinyc);
            (void)printf("tcc=%d %s\n", success, success == 0? "success" : "error");

            delete_artifacts();

            if (success == 0) {
                // printf("\n\nDone, SUCCESS\n\n");
            } else {
                printf("\n\nDone, ERROR!\n\n");
            }
            return success
        ;
    }
}

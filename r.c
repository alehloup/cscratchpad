#include "aleh/aleh.h"
#define _ (void)

// For Shadow Stacks: CONFIG_X86_USER_SHADOW_STACK=y and GLIBC_TUNABLES=glibc.cpu.hwcaps=SHSTK

#ifdef _WIN32
    #define FLAGS_LIN ""
#else
    #define FLAGS_LIN "@LINFLAGS "
#endif

static const char *const flags_gcc = " gcc -fanalyzer @FLAGS " FLAGS_LIN;
static const char *const flags_clang = " clang @FLAGS " FLAGS_LIN;
static const char *const flags_tinyc = " tcc -std=c11 -Wall -Werror ";


static inline int compile_run_c(const char *const c_file_c, const char *const flags) {
    static char buffer[4096] = {0};
    arena a = arenaarr(buffer);

    head_tail_ok cuted = cut(S(c_file_c), '.');

    str parts[] = {S(flags), cuted.head, S(".c -o " TMP_FOLDER), cuted.head, S(".exe")};
    str command = catarr(&a, parts, countof(parts));
    command.data[command.len] = '\0';
    println(command);
    printf("\n");
    int err = system(command.data);
    printf("\n");

    if (err) return err;

    a = arenaarr(buffer);
    str parts2[] = {S(TMP_FOLDER), cuted.head, S(".exe")};
    str command2 = catarr(&a, parts2, countof(parts2));
    command2.data[command2.len] = '\0';
    println(command2);
    printf("\n");
    err = system(command2.data);
    printf("\n");

    return err;
}


int main(int argc, const char *const *argv) {
    const char * compiler = "";
    const char * filename_c = "";
    const char * flags = "";
    int error = 1;

    #ifdef _WIN32
        system("chcp 65001");
    #endif 

    printf("\n");
    
    switch (argc) {
        case 0:case 1: printf("no args passed\n"); return 1;
        case 2:  compiler = "gcc"; filename_c=argv[1]; break;
        default: compiler=argv[1]; filename_c=argv[2]; break;
    }

    switch (compiler[0]) {
        default:           flags = flags_gcc;   break;
        case 'c':case 'C': flags = flags_clang; break;
        case 't':case 'T': flags = flags_tinyc; break;
        case 'a':case 'A': flags = "all"; //used in next switch
    } 

    switch (flags[0]) {
        default: 
            error = compile_run_c(filename_c, flags);
            if (error) printf("\n\nDone, ERROR!\n\n");
            return error;
        ;
        case 'a':
            _ printf("\n===== GCC =====");
            error = compile_run_c(filename_c, flags_gcc);
            _ printf("gcc=%d %s\n",   error, error? "error" : "success");

            _ printf("\n===== Clang =====");
            error = compile_run_c(filename_c, flags_clang);
            _ printf("clang=%d %s\n", error, error? "error" : "success");

            _ printf("\n===== TCC =====");
            error = compile_run_c(filename_c, flags_tinyc);
            _ printf("tcc=%d %s\n",   error, error? "error" : "success");

            if (error) printf("\n\nDone, ERROR!\n\n");
            return error;
        ;
    }
}

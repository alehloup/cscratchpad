// For Shadow Stacks: CONFIG_X86_USER_SHADOW_STACK=y and GLIBC_TUNABLES=glibc.cpu.hwcaps=SHSTK

#include "aleh/aleh.h"

#ifdef OSWIN_
    #define FLAGS_LIN ""
    #define TIME_CMD ""
#else
    #define FLAGS_LIN "@LINFLAGS "
    #define TIME_CMD "/usr/bin/time -f \"\\n  Elapsed: %E\\n  User: %U | Sys: %S\\n  MaxMem: %M KB | CPU: %P\" "
#endif

static const char *const flags_gcc = " gcc -fanalyzer @FLAGS " FLAGS_LIN;
static const char *const flags_clang = " clang @FLAGS " FLAGS_LIN;
static const char *const flags_tinyc = " tcc -std=c11 -Wall -Werror ";


static inline interror compile_run_c(const char *const c_file_c, const char *const flags) {
    static char buffer[4096] = {0};
    static arena a = {0};

    interror err = 0;
    str scommand = {0};
    clock_t stopwatch = {0};

    head_tail_ok cuted = cut(S(c_file_c), '.');
    str compileit[] = {S(flags), cuted.head, S(".c -o " TMP_FOLDER), cuted.head, S(".exe")};
    str runit[] = {S(TIME_CMD " " TMP_FOLDER), cuted.head, S(".exe")};

    a = arenaarr(buffer); //clears arena
    err = scmd(&a, sjoin(&a, compileit, countof(compileit), ' '));
    if (err) return err;

    a = arenaarr(buffer); //clears arena
    scommand = sjoin(&a, runit, countof(runit), ' ');
    
    stopwatch = clock();
        err = scmd(&a, scommand);
        #ifdef OSWIN_
            print_stopwatch(stopwatch);
        #else
            _ stopwatch; // In Linux clock does not work since system forks 
        #endif
    
    return err;
}


int main(int argc, const char *const *argv) {
    const char * compiler = "";
    const char * filename_c = "";
    const char * flags = "";
    int error = 1;

    #ifdef OSWIN_
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

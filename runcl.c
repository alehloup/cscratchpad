#include "ale_io.h"

i32 main(i32 argc, ccstr*argv) {
    char buffer[512];
    ccstr _1_ = argc > 1 ? argv[1] : "";

    if (argc < 1) {
        printf("No args...");
    }

    printf(
        (
            "msvc"
            " && clang-cl @Flagscl %s -o \"./%s.exe\"" 
            " && \"./%s.exe\""
            " && rm \"./%s.exe\""
        ), 
        _1_, _1_, _1_, _1_
    );

    printf("\n");
    
    shellrun(sizeof(buffer), buffer, 
        (
            "msvc"
            " && clang-cl @Flagscl %s -o \"./%s.exe\"" 
            " && \"./%s.exe\""
            " && rm \"./%s.exe\""
        ),
        _1_, _1_, _1_, _1_
    );

    return 0;
}
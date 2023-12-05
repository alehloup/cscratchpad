#include "ale_io.h"

i32 main(i32 argc, ccstr*argv) {
    char buffer[512];
    ccstr_1_ = argc > 1 ? argv[1] : "";

    if (argc < 1) {
        printf("No args...");
    }

    printf(
        (
            "g++ @Flags %s -o \"./%s.exe\"" 
            " && \"./%s.exe\""
        ), 
        _1_, _1_, _1_
    );

    printf("\n");
    
    shellrun(sizeof(buffer), buffer, 
        (
            "g++ @Flags %s -o \"./%s.exe\"" 
            " && \"./%s.exe\""
        ),
        _1_, _1_, _1_
    );

    return 0;
}

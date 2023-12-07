#include "../ale_io.h"

i32 main(i32 argc, ccstr*argv) {
    char buffer[512];
    ccstr _1_ = argc > 1 ? argv[1] : "";

    if (argc < 1) {
        printf("No args...");
    }

    printf(
        (
            "msvc"
            " && cl /W4 /Ox /Fo:%s.obj /Fe:%s.exe %s" 
            " && \"./%s.exe\""
            " && rm \"./%s.exe\""
            " && rm \"./%s.obj\""
        ), 
        _1_, _1_, _1_, _1_, _1_, _1_
    );

    printf("\n");
    
    shellrun(isizeof(buffer), buffer, 
        (
            "msvc"
            " && cl /W4 /Ox /Fo:%s.obj /Fe:%s.exe %s" 
            " && \"./%s.exe\""
            " && rm \"./%s.exe\""
            " && rm \"./%s.obj\""
        ), 
        _1_, _1_, _1_, _1_, _1_, _1_
    );

    return 0;
}

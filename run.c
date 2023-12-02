#include "ale_io.h"

i32 main(i32 argc, cstr *argv) {
    char buffer[512];
    cstr _1_ = argc > 1 ? argv[1] : "";

    if (argc < 1) {
        printf("No args...");
    }

    printf(    
        (
        "gcc @Flags %s -o \"./builds/%s.exe\"" 
        " && " 
        "\"./builds/%s.exe\""
            " | "
            "tee \"./outputs/%sout.txt\""
        ), 
    _1_, _1_, _1_, _1_);

    printf("\n");
    
    shellrun(sizeof(buffer), buffer, 
        (
        "gcc @Flags %s -o \"./builds/%s.exe\"" 
        " && " 
        "\"./builds/%s.exe\""
            " | "
            "tee \"./outputs/%sout.txt\""
        ),
    _1_, _1_, _1_, _1_
    );

    return 0;
}

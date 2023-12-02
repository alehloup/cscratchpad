#include "ale_io.h"

i32 main(i32 argc, cstr *argv) {
    char buffer[512];
    cstr _1_ = argc > 1 ? argv[1] : "", _2_ = argc > 2 ? argv[2] : "";

    if (argc < 1) {
        printf("No args...");
    }

    printf(
        (
        "g++ @Flags %s -o \"./builds/%s++.exe\"" 
        " && " 
        "\"./builds/%s++.exe\" < %s"
            " | "
            "tee \"./outputs/%s++out.txt\""
        ), 
    _1_, _1_, _1_, _2_, _1_);
    printf("\n");

    shellrun(sizeof(buffer), buffer,
        (
        "g++ @Flags %s -o \"./builds/%s++.exe\"" 
        " && " 
        "\"./builds/%s++.exe\" < %s"
            " | "
            "tee \"./outputs/%s++out.txt\""
        ),
    _1_, _1_, _1_, _2_, _1_);

    return 0;
}

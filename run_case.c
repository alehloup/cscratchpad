#include "ale_io.h"

i32 main(i32 argc, cstr *argv) {
    if (argc < 1) {
        printf("No args...");
    }

    char buffer[512];
    cstr __1 = argv[1], __2 = argv[2];

    printf(
        (
        "gcc @Flags %s -o \"./builds/%s.exe\"" 
        " && " 
        "\"./builds/%s.exe\" < %s"
            " | "
            "tee \"./outputs/%sout.txt\""
        ), 
    __1, __1, __1, __2, __1);
    printf("\n");

    shellrun(sizeof(buffer), buffer, 
        (
        "gcc @Flags %s -o \"./builds/%s.exe\"" 
        " && " 
        "\"./builds/%s.exe\" < %s"
            " | "
            "tee \"./outputs/%sout.txt\""
        ), 
    __1, __1, __1, __2, __1);

    return 0;
}

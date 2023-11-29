#include "ale_io.h"

i32 main(i32 argc, cstr *argv) {
    if (argc < 1) {
        printf("No args...");
    }

    char buffer[512];
    cstr _$1 = argv[1], _$2 = argv[2];

    printf(
        (
        "g++ @Flags %s -o \"./builds/%s++.exe\"" 
        " && " 
        "\"./builds/%s++.exe\" < %s"
            " | "
            "tee \"./outputs/%s++out.txt\""
        ), 
    _$1, _$1, _$1, _$2, _$1);
    printf("\n");

    shellrun(sizeof(buffer), buffer,
        (
        "g++ @Flags %s -o \"./builds/%s++.exe\"" 
        " && " 
        "\"./builds/%s++.exe\" < %s"
            " | "
            "tee \"./outputs/%s++out.txt\""
        ),
    _$1, _$1, _$1, _$2, _$1);

    return 0;
}

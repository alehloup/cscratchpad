#include "ale_io.h"

int32_t main(int32_t argc, cstr_t *argv) {
    if (argc < 1) {
        printf("No args...");
    }

    char buffer[512];
    cstr_t _$1 = argv[1], _$2 = argv[2];

    printf(
        (
        "gcc @Flags %s -o \"./builds/%s.exe\"" 
        " && " 
        "\"./builds/%s.exe\" < %s"
            " | "
            "tee \"./outputs/%sout.txt\""
        ), 
    _$1, _$1, _$1, _$2, _$1);
    printf("\n");

    shellrun(sizeof(buffer), buffer, 
        (
        "gcc @Flags %s -o \"./builds/%s.exe\"" 
        " && " 
        "\"./builds/%s.exe\" < %s"
            " | "
            "tee \"./outputs/%sout.txt\""
        ), 
    _$1, _$1, _$1, _$2, _$1);

    return 0;
}

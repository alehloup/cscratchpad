#include "ale_io.h"

int32_t main(int32_t argc, cstr_t *argv) {
    if (argc < 1) {
        printf("No args...");
    }

    char buffer[512];
    cstr_t _$1 = argv[1];

    sstr_t format = (
        "gcc @Flags %s -o \"./builds/%s.exe\"" 
        " && " 
        "\"./builds/%s.exe\""
            " | "
            "tee \"./outputs/%sout.txt\""
    );

    printf(format, _$1, _$1, _$1, _$1);
    printf("\n");
    
    shellrun(sizeof(buffer), buffer, format, _$1, _$1, _$1, _$1
    );

    return 0;
}

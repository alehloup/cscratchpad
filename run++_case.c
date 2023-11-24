#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

int32_t main(int32_t argc, cstr_t *argv) {
    if (argc < 1) {
        ale_printf("No args...");
    }

    char buffer[512];
    cstr_t _$1 = argv[1], _$2 = argv[2];

    sstr_t format = (
        "g++ @Flagscpp %s -o \"./builds/%s++.exe\"" 
        " && " 
        "\"./builds/%s++.exe\" < %s"
            " | "
            "tee \"./outputs/%s++out.txt\""
    );

    ale_printf(format, _$1, _$1, _$1, _$2, _$1);
    ale_printf("\n");

    shellrun(buffer, format, _$1, _$1, _$1, _$2, _$1);

    return 0;
}

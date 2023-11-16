#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

int32_t main(int32_t argc, cstring *argv) {
    char buffer[512];
    cstring _$1 = argv[1];

    staticstring format = (
        "g++ @Flagscpp %s -o \"./builds/%s++.exe\"" 
        " && " 
        "\"./builds/%s++.exe\""
            " | "
            "tee \"./outputs/%s++out.txt\""
    );

    ale_printf(format, _$1, _$1, _$1, _$1);
    ale_printf("\n");
    
    shellrun(buffer, format, _$1, _$1, _$1, _$1);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

int32_t main(int32_t argc, char **argv) {
    char buffer[512];
    char *_$1 = argv[1], *_$2 = argv[2];

    staticstring format = (
        "gcc @Flags %s -o \"./builds/%s.exe\"" 
        " && " 
        "\"./builds/%s.exe\" < %s"
            " | "
            "tee \"./outputs/%sout.txt\""
    );

    printf(format, _$1, _$1, _$1, _$2, _$1);
    printf("\n");

    shellrun(buffer, format, _$1, _$1, _$1, _$2, _$1);

    return 0;
}

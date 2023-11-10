#include "ale.h"

int32_t main(int32_t argc, char **argv) {
    char buffer[512];
    char *_$1 = argv[1];
    
    shellrun(
        buffer,

        "gcc @Flags %s -o \"./builds/%s.exe\"" 
        " && " 
        "\"./builds/%s.exe\""
            " | "
            "tee \"./outputs/%sout.txt\""
            
        , _$1, _$1, _$1, _$1
    );

    return 0;
}

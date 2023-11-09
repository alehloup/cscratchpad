#include "ale.h"

int main(int argc, char **argv) {
    char buffer[256];
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

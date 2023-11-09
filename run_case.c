#include "ale.h"

int main(int argc, char **argv) {
    char buffer[512];
    char *_$1 = argv[1], *_$2 = argv[2];

    shellrun(
        buffer,
        
        "gcc @Flags %s -o \"./builds/%s.exe\"" 
        " && " 
        "\"./builds/%s.exe\" < %s"
            " | "
            "tee \"./outputs/%sout.txt\""
            
        , _$1, _$1, _$1, _$2, _$1
    );

    return 0;
}

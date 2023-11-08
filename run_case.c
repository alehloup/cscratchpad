#include "ale.h"

int main(MAINARGS) {
    winit;

    shellrun(
        "gcc @Flags %s -o \"./builds/%s.exe\"" 
        " && " 
        "\"./builds/%s.exe\" < %s"
            " | "
            "tee \"./outputs/%sout.txt\""
            
        , _$1, _$1, _$1, _$2, _$1
    );

    return 0;
}

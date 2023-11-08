#include "ale.h"

int main(MAINARGS) {
    winit;

    char buffer[256];
    shellrun(
        buffer,
        
        "gcc @Flags %32s -o \"./builds/%32s.exe\"" 
        " && " 
        "\"./builds/%32s.exe\" < %32s"
            " | "
            "tee \"./outputs/%32sout.txt\""
            
        , _$1, _$1, _$1, _$2, _$1
    );

    return 0;
}

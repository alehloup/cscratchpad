#include "ale.h"

int main(int argc, char **argv) {
    char buffer[256];
    char *_$1 = argv[1];
    
    shellrun(
        buffer,

        "gcc @Flags %32s -o \"./builds/%32s.exe\"" 
        " && " 
        "\"./builds/%32s.exe\""
            " | "
            "tee \"./outputs/%32sout.txt\""
            
        , _$1, _$1, _$1, _$1
    );

    return 0;
}

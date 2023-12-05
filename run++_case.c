#include "ale_io.h"

i32 main(i32 argc, ccstr*argv) {
    char buffer[512];
    ccstr_1_ = argc > 1 ? argv[1] : "", _2_ = argc > 2 ? argv[2] : "";

    if (argc < 1) {
        printf("No args...");
    }

    printf(
        (
        "g++ @Flags %s -o \"./%s.exe\"" 
        " && " 
        "\"./%s.exe\" < %s"
        ), 
    _1_, _1_, _1_, _2_);
    printf("\n");

    shellrun(sizeof(buffer), buffer,
        (
        "g++ @Flags %s -o \"./%s.exe\"" 
        " && " 
        "\"./%s.exe\" < %s"
        ),
    _1_, _1_, _1_, _2_);

    return 0;
}

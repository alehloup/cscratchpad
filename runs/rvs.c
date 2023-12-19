#include <stdio.h>
#include <stdlib.h>
#include "../ale.h"

i32 main(i32 argc, ccstr*argv) {
    ccstr _1_c = argc > 1 ? argv[1] : "\0";
    bufferchar _1_[512] = {0};
    for (idx32 i = 0; _1_c[i] and _1_c[i] != '.'; ++i) {
        _1_[i] = _1_c[i];
    }

    shellrun(
        "msvc"
        " && cl /W4 /Ox /Fo:%s.obj /Fe:%s.exe %s" 
        " && \"./%s.exe\""
        " && rm \"./%s.exe\""
        " && rm \"./%s.obj\""
        "\n"
        , 
        _1_c, _1_, _1_, _1_, _1_, _1_
    );

    return 0;
}

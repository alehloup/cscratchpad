#include <stdio.h>
#include <stdlib.h>
#include "../ale.h"

i32 main(i32 argc, ccstr*argv) {
    ccstr _1_ = argc > 1 ? argv[1] : "";

    shellrun(
        "msvc"
        " && cl /W4 /Ox /Fo:%s.obj /Fe:%s.exe %s" 
        " && \"./%s.exe\""
        " && rm \"./%s.exe\""
        " && rm \"./%s.obj\""
        , 
        _1_, _1_, _1_, _1_, _1_, _1_
    );

    return 0;
}

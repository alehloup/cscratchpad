#include <stdio.h>
#include <stdlib.h>
#include "../ale.h"

int main(int argc, Ccstr*argv) {
    Ccstr _1_c = argc > 1 ? argv[1] : "\0";
    char _1_[512] = {0};
    for (int i = 0; _1_c[i] and _1_c[i] != '.'; ++i) {
        _1_[i] = _1_c[i];
    }

    shellrun(
        "msvc"
        " && cl /W4 /Ox /Fo:%s.obj /Fe:%s.exe -D_CRT_SECURE_NO_WARNINGS %s" 
        " && \"./%s.exe\""
        " && rm \"./%s.exe\""
        " && rm \"./%s.obj\""
        "\n"
        , 
        _1_, _1_, _1_c, _1_, _1_, _1_
    );

    return 0;
}

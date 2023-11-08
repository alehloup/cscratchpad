#include "ale.h"

int main(MAINARGS) {
    shellrun("gcc @Flags %s -o %s.exe && %s.exe", _$1, _$1, _$1);

    return 0;
}

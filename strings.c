#include <stdio.h>
#include "ale.h"

int main(void) {
    String string = String("Alessandro");
    char bdata[256] = {0};
    Buffer buff = _array_init(bdata, 0);

    buffer_append(&buff, string);
    buffer_append(&buff, S(" Luiz"));
    buffer_append(&buff, S(" Stamatto"));

    buffer_set(&buff, S("Woa"));

    printf("%s |%lld| \n", buff.data, buff.len);

    return 0;
}
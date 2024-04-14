#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ale.h"

_proc run(void) {
    Long sum = 0;
    _new_array(buffer, static Byte, 1024 * 1024);

    FILE *file = fopen("C:/Users/Aleh/1brc_java/measurements1b.txt", "r");
    {
        while ((buffer.len = (Long) fread(buffer.elements, 1, (ULong) buffer.cap, file)) > 0) {
            sum += buffer.elements[0];
        }
    }
    fclose(file);

    //printf("%s\n", buffer.elements);
    printf("%lld\n", sum);
}

int main(void) {
    START_WATCH;
    run();
    STOP_WATCH;

    return 0;
}

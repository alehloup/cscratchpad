#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

#define print(...)  //printf(__VA_ARGS__)

//AoC 11
_proc aoc(len32 lines_len, mstr lines[64]) {
    len32 columns_len = (len32) cstrlen(lines[0]);
    printf("%d x %d\n", lines_len, columns_len);
    for (idx32 iline = 0; iline < lines_len; ++iline) {
        printf("%s\n", lines[iline]);
    }
} 


i32 main(void) {
    static char charbuffer[Next_power2(64000)] = {0};
    static mstr lines[256] = {0};
    i64 charbuffer_cap = 64000;
    i32 lines_cap = 256;

    len32 lines_len = file_to_lines("./txts/aoc.txt", \
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

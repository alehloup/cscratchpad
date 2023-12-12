#include <math.h>
#include "ale_io.h"

#define print(...)  //printf(__VA_ARGS__)
#define printn(...) //print(__VA_ARGS__); printf("\n")

_fun i32 per_line(mstr line) {
    printf("%s \n", line);
    return (i32) cstrlen(line);
}

//AoC 10
_proc aoc(ci32 lines_len, mstr lines[64]) {
    i64 sum = 0;
    for (int iline = 0; iline < lines_len; ++iline) {
        mstr line = lines[iline];
        if (is_empty_string(line)) {
            continue;
        }
        sum += per_line(line);
    }
    printf("Sum: %lld \n", sum);
} 


i32 main(void) {
    static char charbuffer[NEXT_POWER2(64000)] = {0};
    static mstr lines[256] = {0};
    i64 charbuffer_cap = 64000;
    i32 lines_cap = 256;

    i32 lines_len = file_to_lines("./txts/aoc.txt", \
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

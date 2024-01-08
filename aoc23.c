#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

#define print(...)  printf(__VA_ARGS__)

//AoC 13
_proc aoc(int lines_len, mstr lines[64]) {
    (void) lines;
    (void) lines_len;
} 


int main(void) {
    static char charbuffer[Next_power2(64000)] = {0};
    static mstr lines[256] = {0};
    Long charbuffer_cap = 64000;
    int lines_cap = 256;

    int lines_len = file_to_lines("./txts/aoc.txt",
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

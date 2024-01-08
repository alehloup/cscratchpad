#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

_fun int per_matrix(int lines_len, mstr lines) {
    static Long cols_hash[64] = {0};
    static Long lins_hash[64] = {0};
    for (int i = 0; i < 64; ++i) {
        cols_hash[i] = 0;
    }
    for (int i = 0; i < 64; ++i) {
        lins_hash[i] = 1;
    }

    for (int i = 0; i < lines_len and not is_empty_string(lines[i]); ++i) {

    }

    
}

//AoC 13
_proc aoc(int lines_len, mstr lines[]) {
    int i = 0;
    while (i < (lines_len - 2))
        int res = per_matrix(lines_len, lines);
        printf("%d\n", res);
    }
} 

#define lines_cap 2048
#define charbuffer_cap (Next_power2(64000))
int main(void) {
    static char charbuffer[charbuffer_cap] = {0};
    static mstr lines[lines_cap] = {0};

    int lines_len = file_to_lines_including_empty("./txts/small.txt",
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

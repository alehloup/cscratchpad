#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

static const b32 PRINT_ALL_ = True;
#define print(...) if (PRINT_ALL_) printf(__VA_ARGS__)

_fun i32 solve_line(mstr line) {
    mstr words[4] = {0};
    len32 words_len = split(line, ' ', 4, words);
    assert(words_len == 2 && "Error parsing");
    print("%s | %s \n", words[0], words[1]);
    return cstrlen32(line);
}

//AoC 12
_proc aoc(len32 lines_len, mstr lines[2]) {
    i64 sum = 0;
    
    for (idx32 iline = 0; iline < lines_len; ++iline) {
        mstr line = lines[iline];
        i32 res = solve_line(line);
        print("%s : %d\n", line, res);
        sum += res;
    }

    printf("\nSum: %lld\n", sum);
} 


i32 main(void) {
    static char charbuffer[Next_power2(64000)] = {0};
    static mstr lines[256] = {0};
    i64 charbuffer_cap = 64000;
    i32 lines_cap = 256;

    len32 lines_len = file_to_lines("./txts/aoc.txt",
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

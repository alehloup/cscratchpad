#include <math.h>
#include "ale_io.h"

#define print(...)  //printf(__VA_ARGS__)
#define printn(...) //print(__VA_ARGS__); printf("\n")

_fun i32 fuse_lincol(i32 iline, i32 icol) {
    return 1000*iline + icol;
}

//AoC 10
_proc aoc(ci32 lines_len, ccstr lines[64]) {
    static ccstr TOPS = "|LJ";
    static ccstr RIGHTS = "-FL";
    static ccstr BOTTOMS = "|LF";
    static ccstr LEFTS = "-J7";
    i32 s_iline = 0, s_icol = 0;

    printf("TOPS: %s\nRIGHTS: %s\nBOTTOMS:%s\nLEFTS:%s\n", TOPS, RIGHTS, BOTTOMS, LEFTS);

    for (int iline = 0; iline < lines_len; ++iline) {
        ccstr line = lines[iline];
        i32 s_pos = -1;
        if (is_empty_string(line)) {
            continue;
        }

        s_pos = letter_pos_in_cstring('S', line);
        if (s_pos > -1) {
            s_iline = iline; s_icol = s_pos;
            break;
        }        
    }

    printf("S_pos: %d %d\n", s_iline, s_icol);
    
} 


i32 main(void) {
    static char charbuffer[NEXT_POWER2(64000)] = {0};
    static mstr lines[256] = {0};
    i64 charbuffer_cap = 64000;
    i32 lines_cap = 256;

    i32 lines_len = file_to_lines("./txts/aoc.txt", \
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, (ccstr * )lines);
    print_clock(start);

    return 0;
}

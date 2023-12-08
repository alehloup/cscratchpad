#include "ale_io.h"

static u8 mem[128*MBs_] = {0};
static Arena A = {0, 0};

//AoC 5
_proc aoc(ci32 lineslen, mstr lines[1]) {
    mstr seeds = split(&A, lines[0], ':')[1];
    printf("%s\n", seeds);

    discard_ lineslen;
} 


i32 main(void) {
    A = new_arena(128*MBs_, mem);

    mstr *lines = file_to_lines(&A, "./txts/aoc.txt");
    clock_t start = clock();
    aoc(hd_len_(lines), lines);
    print_clock(start);

    return 0;
}

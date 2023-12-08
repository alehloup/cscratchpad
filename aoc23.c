#include "ale_io.h"

static u8 mem[128*MBs_] = {0};
static Arena arena = {0, 0};

//AoC 5
_proc aoc(ci32 lineslen, mstr lines[1]) {
    discard_ lineslen;
    discard_ lines;
} 


i32 main(void) {
    arena = new_arena(128*MBs_, mem);

    mstr *lines = file_to_lines(&arena, "./txts/aoc.txt");
    clock_t start = clock();
    aoc(hd_len_(lines), lines);
    print_clock(start);

    return 0;
}

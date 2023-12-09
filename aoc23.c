#include <math.h>
#include "ale_io.h"

//#define print(...) 

static u8 mem[128*MBs_] = {0};
static Arena A = {0, 0};

//AoC 7
_proc aoc(ci64 lineslen, mstr lines[1]) {
   
    discard_ lines;
    discard_ lineslen;
} 


i32 main(void) {
    A = new_arena(128*MBs_, mem);

    {
        mstr *lines = file_to_lines(&A, "./txts/aoc.txt");
        clock_t start = clock();
        aoc(hd_len_(lines), lines);
        print_clock(start);
    }

    return 0;
}

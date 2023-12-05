#include "ale_io.h"

_proc aoc3(ci32 lineslen, mstr lines[1]) {
    for (int i = 1; i < lineslen-1; ++i) {
        ccstr line = lines[i], top = lines[i-1], bot = lines[i+1];
        ccstr region[] = {top, line, bot};

        for (int c = 0; lines[i][c]; ++c) {
            cchar letter = lines[i][c];
            if (letter == '*') {
                ci32 lower = c - 1, upper = c + 1;
                assert(lower < upper);
            
                printf("\n-----------------\n");
                for (int r = 0; r < countof(region); r++) {
                    printf("%s \n", region[r]);
                }
                printf("-----------------\n");

            }
        }

    }
}

i32 main(void) {
    static u8 mem[128*MBs_] = {0};
    Arena arena = new_arena(128*MBs_, mem);

    mstr *lines = file_to_lines(&arena, "./txts/aoc.txt");
    aoc3(hd_len_(lines), lines);

    return 0;
}

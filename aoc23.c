#include "ale_io.h"

i32 main(void) {
    static u8 mem[128*MBs_] = {0};
    Arena arena = newarena(128*MBs_, mem);

    mstr *lines = file_to_nonempty_lines(&arena, "aoc.txt");

    printf("%d lines", hd_(lines)->len);

    return 0;
}

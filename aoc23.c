#include "ale_io.h"

_proc aoc3(ci32 lineslen, mstr lines[1]) {
    i64 sum = 0;
    for (int i = 1; i < lineslen-1; ++i) {
        ccstr line = lines[i], top = lines[i-1], bot = lines[i+1];
        ccstr regions[] = {top, line, bot};

        for (int iline_letter = 0; lines[i][iline_letter]; ++iline_letter) {
            cchar line_letter = lines[i][iline_letter];
            if (line_letter == '*') {
                ci32 asterisk_start = iline_letter - 1, asterisk_end = iline_letter + 1;
                i32 nregion = 0;
                i64 multregion = 1;
                for (int iregion = 0; iregion < countof(regions); iregion++) {
                    ccstr region = regions[iregion];
                    for (int iregion_letter = 0; region[iregion_letter] && nregion < 3; ++iregion_letter) {
                        if (is_digit(region[iregion_letter])) {
                            i64num_i32len parsed = cstr_to_num(&region[iregion_letter]);
                            ci32 iregion_start = iregion_letter; 
                            ci32 iregion_end = iregion_start + parsed.len - 1;

                            if ((iregion_start >= asterisk_start && iregion_start <= asterisk_end)
                                || (iregion_end >= asterisk_start && iregion_end <= asterisk_end)) 
                            {
                                ++nregion; multregion *= parsed.num;
                            }
                            iregion_letter += parsed.len;
                        }
                    }
                }
                if (nregion == 2) {
                    sum += multregion;
                }
            }
        }
    }
    printf("\n Final Sum: %lld \n", sum);
}

i32 main(void) {
    static u8 mem[128*MBs_] = {0};
    Arena arena = new_arena(128*MBs_, mem);

    mstr *lines = file_to_lines(&arena, "./txts/aoc.txt");
    clock_t start = clock();
    aoc3(hd_len_(lines), lines);
    print_clock(start);

    return 0;
}

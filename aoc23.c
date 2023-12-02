#include "ale_io.h"

_proc_rbuffer(1, 2)
aoc3(i32 lineslen, mstr lines[]) {
    static u8 buff[MBs_ / 8] = {0};
    Arena arena = newarena(MBs_ / 8, buff);

    i8 reds = 12, greens = 13, blues = 14;
    printf("Reds: %d, Greens: %d, Blues: %d\n", reds, greens, blues);

    for (int iline = 0; iline < lineslen; ++iline) {
        Vec64 game = mutslice_by_splitter(&arena,  lines[iline], ':');
        Vec64 grabs = mutslice_by_splitter(&arena,  (mstr) game.data[1], ';');

        for (int igrab = 0; igrab < grabs.len; ++igrab) {
            Vec64 grab = mutslice_by_splitter(&arena,  (mstr) grabs.data[igrab], ',');

        }
        

        printf("| \n");
    }

}

_proc_rbuffer(1, 2)
aoc1(i32 lineslen, cstr lines[]) {
    i64 sum = 0;
    for (int iline = 0; iline < lineslen; ++iline) {
        cstr line = lines[iline];
        i8 first = -1, last = -1;

        if (is_empty_string(line)) {
            continue;
        }
        
        for (int i = 0; line[i]; ++i) {
            i8 digit = named_digit(&line[i]);
            if (digit > -1) {
                if (first == -1) {
                    first = digit;
                } 
                last = digit;
            }
        }
        printf("%s: %d%d\n", line, first, last);
        sum += first*10 + last;
    }    
    printf("Sum: %lld\n", sum);
}

i32 main(void) {
    static u8 mem[128*MBs_] = {0};
    Arena arena = newarena(128*MBs_, mem);

    Vec64 lines = file_to_nonempty_lines(&arena, "aoc.txt");
    mstr *data = vec_data_as_mstr(&lines);

    aoc3(lines.len, data);

    return 0;
}

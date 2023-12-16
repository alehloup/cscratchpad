#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

#define print(...)  //printf(__VA_ARGS__)

_proc transform_matrix(len32 *lines_len_ref, len32 *columns_len_ref, mstr lines[64]) {
    b32 col_to_has_galaxy[512] = {0};

    for (idx32 iline = 0; iline < (*lines_len_ref); ++iline) {
        idx32 pos = char_pos_in_str('#', lines[iline]);        
        if (pos == -1) {
            for (idx32 icol = 0; icol < (*columns_len_ref); ++icol) 
                lines[iline][icol] = 'v';
        } else {
            col_to_has_galaxy[pos] = True;
        }
    }

    for (idx32 icol = 0; icol < (*columns_len_ref); ++icol) {
        if (not col_to_has_galaxy[icol]) {
            for (idx32 iline = 0; iline < (*lines_len_ref); ++iline) {
                lines[iline][icol] = lines[iline][icol] == 'v' ? '*' : '>';
            }
        }
    }
}

//AoC 11
_proc aoc(len32 lines_len, mstr lines[64]) {
    len32 columns_len = cstrlen32(lines[0]);
    transform_matrix(&lines_len, &columns_len, lines);
    
    printf("%d x %d\n", lines_len, columns_len);
    matrix_print("%c", lines_len, columns_len, lines);
} 


i32 main(void) {
    static char charbuffer[Next_power2(64000)] = {0};
    static mstr lines[256] = {0};
    i64 charbuffer_cap = 64000;
    i32 lines_cap = 256;

    len32 lines_len = file_to_lines("./txts/aoc.txt", \
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

#define print(...)  //printf(__VA_ARGS__)

_fun_inlined i32 fuse_lincol(i32 iline, i32 icol) {
    return 1000*iline + icol;
}

_fun_inlined i32 lin_(i32 fused) {
    return fused / 1000;
}
_fun_inlined i32 col_(i32 fused) {
    return fused % 1000;
}

//AoC 10
_proc aoc(ci32 lines_len, ccstr lines[64]) {
    static ccstr TOPS = "|LJ";
    static ccstr RIGHTS = "-FL";
    static ccstr BOTTOMS = "|F7";
    static ccstr LEFTS = "-J7";
    static i32 steps[(NEXT_POWER2(140*140))] = {0};
    len32 steps_len = 0;
    i32 pos_line = 0, pos_col = 0, fused = 0, start_fused = 0;
    char last_move_dir = '.';

    for (idx32 iline = 0; iline < lines_len; ++iline) {
        ccstr line = lines[iline];
        idx32 s_pos = -1;
        if (is_empty_string(line)) {
            continue;
        }

        s_pos = letter_pos_in_cstring('S', line);
        if (s_pos > -1) {
            pos_line = iline; pos_col = s_pos;
            break;
        }        
    }

    fused = fuse_lincol(pos_line, pos_col);
    steps[steps_len++] = fused;
    start_fused = fused;

    print("Starting Position: %d %d\n", pos_line, pos_col);   

    if (pos_line > 0 and char_in_(lines[pos_line-1][pos_col], BOTTOMS)) {
        --pos_line; 
        last_move_dir = 't';
    } else if (char_in_(lines[pos_line][pos_col+1], LEFTS)) {
        ++pos_col;
        last_move_dir = 'r';
    } else if (pos_line < (lines_len-1) and char_in_(lines[pos_line+1][pos_col], TOPS)) {
        ++pos_line;
        last_move_dir = 'b';
    } else if (pos_col > 0 and char_in_(lines[pos_line][pos_col-1], RIGHTS)) {
        --pos_col;
        last_move_dir = 'l';
    } else {
        print("ERROR: could not move from start");
    }

    print("Move: %c Step: %d Pipe: %c\n", last_move_dir, steps_len, lines[pos_line][pos_col]);
    fused = fuse_lincol(pos_line, pos_col);
    steps[steps_len++] = fused;
    
    
    while(fused != start_fused) {
        cchar cur_pipe = lines[pos_line][pos_col];
        
               if (/*GO TOP*/ last_move_dir != 'b' and char_in_(cur_pipe, TOPS)) {
            --pos_line; 
            last_move_dir = 't';
        } else if (/*GO RIGHT*/ last_move_dir != 'l' and char_in_(cur_pipe, RIGHTS)) {
            ++pos_col;
            last_move_dir = 'r';
        } else if (/*GO BOTTOM*/ last_move_dir != 't' and char_in_(cur_pipe, BOTTOMS)) {
            ++pos_line;
            last_move_dir = 'b';
        } else if (/*GO LEFT*/ last_move_dir != 'r' and char_in_(cur_pipe, LEFTS)) {
            --pos_col;
            last_move_dir = 'l';
        } else {
            print("ERROR: pipe unrecognized %c %d %d\n", cur_pipe, pos_line, pos_col);
            print("Move: %c Step: %d Pipe: %c\n", last_move_dir, steps_len, lines[pos_line][pos_col]);
            break;
        }

        print("Move: %c Step: %d Pipe: %c\n", last_move_dir, steps_len, lines[pos_line][pos_col]);
        fused = fuse_lincol(pos_line, pos_col);
        steps[steps_len++] = fused;
    };

    printf("Reached S in [%d] steps, fused: %d(%d) \n", (steps_len - 1), steps_len/2, steps[steps_len/2]);
    
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

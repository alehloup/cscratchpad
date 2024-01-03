#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

#define print(...)  printf(__VA_ARGS__)

#define t -77 //'^'
#define b -77 //'v'
#define r -16 // '>'//-81 //»
#define l -16 //'<'//-82 //«

#define I -2 // ■
#define O -78 // ▓

#define pipe -80 // ░

_fun int count_I(int lines_len, mstr lines[64]) {
    int count = 0;
    for (int iline = 0; iline < lines_len; ++iline) {
        for (int icol = 0; lines[iline][icol]; ++icol) {
            count += lines[iline][icol] == I ? 1 : 0;
        }
    }    

    return count;
}

_proc transform_pipes(int lines_len, mstr lines[64]) {
    for (int iline = 0; iline < lines_len; ++iline) {
        for (int icol = 0; lines[iline][icol]; ++icol) {
            switch(lines[iline][icol]) {
                case '|': case 'L': case 'J': case '-': case 'F': case '7':
                    lines[iline][icol] = pipe; break;
                default:
                    lines[iline][icol] = lines[iline][icol]; break;
            }
        }
    }
}

_math int is_path(cchar tile) {
    return tile == t || tile == r || tile == b || tile == l || tile == 'S';
}

_fun int out_of_bounds(int line, int col, int lines_len, mstr lines[64]) {
    return (line < 0 or col < 0 or line >= lines_len or !lines[line][col]);
}

_proc fill(char char_to_fill, int line, int col, int lines_len, mstr lines[64]) {
    if (
        out_of_bounds(line, col, lines_len, lines)
        || ('A' <= lines[line][col] and lines[line][col] <= 'z')
        || is_path(lines[line][col])
        || lines[line][col] == O
        || lines[line][col] == I
    ) {
        return;
    }

    lines[line][col] = char_to_fill;
    fill(char_to_fill, line-1, col, lines_len, lines);
    fill(char_to_fill, line, col+1, lines_len, lines);
    fill(char_to_fill, line+1, col, lines_len, lines);
    fill(char_to_fill, line, col-1, lines_len, lines);
}

_proc fill_outs_matrix(int lines_len, mstr lines[64]) {
    int icol = 0;
    for (icol = 0; lines[0][icol]; ++icol) {
        fill(O, 0, icol, lines_len, lines);
    }
    for (icol = 0; lines[lines_len-1][icol]; ++icol) {
        fill(O, lines_len-1, icol, lines_len, lines);
    }
    for (int iline = 0; iline < lines_len; ++iline) {
        fill(O, iline, 0, lines_len, lines);
    }
    for (int iline = 0; iline < lines_len; ++iline) {
        fill(O, iline, icol-1, lines_len, lines);
    }
}

_proc fill_ins_matrix(int lines_len, mstr lines[64]) {
    for (int iline = 0; iline < lines_len; ++iline) {
        for (int icol = 0; lines[iline][icol]; ++icol) {
            fill(I, iline, icol, lines_len, lines);
        }
    }
}

_proc print_matrix(int lines_len, mstr lines[64]) {
    printf("\nMATRIX:\n");
    for (int i = 0; i < lines_len; ++i) {
        printf("%s\n", lines[i]);
    }
    printf("\n");
}

//AoC 10
_proc aoc(int lines_len, mstr lines[64]) {
    static ccstr TOPS = "|LJ";
    static ccstr RIGHTS = "-FL";
    static ccstr BOTTOMS = "|F7";
    static ccstr LEFTS = "-J7";
    
    int pos_line = 0, pos_col = 0;
    int stepn = 0;
    char last_move_dir = '.';

    for (int iline = 0; iline < lines_len; ++iline) {
        ccstr line = lines[iline];
        int s_pos = -1;
        if (is_empty_string(line)) {
            continue;
        }

        s_pos = char_pos_in_str('S', line);
        if (s_pos > -1) {
            pos_line = iline; pos_col = s_pos;
            break;
        }        
    }

    print("Starting Position: %d %d\n", pos_line, pos_col);   

    if (pos_line > 0 and char_in_(lines[pos_line-1][pos_col], BOTTOMS)) {
        lines[pos_line][pos_col] = t;
        --pos_line; 
        last_move_dir = 't';
    } else if (char_in_(lines[pos_line][pos_col+1], LEFTS)) {
        lines[pos_line][pos_col] = r;
        ++pos_col;
        last_move_dir = 'r';
    } else if (pos_line < (lines_len-1) and char_in_(lines[pos_line+1][pos_col], TOPS)) {
        lines[pos_line][pos_col] = b;
        ++pos_line;
        last_move_dir = 'b';
    } else if (pos_col > 0 and char_in_(lines[pos_line][pos_col-1], RIGHTS)) {
        lines[pos_line][pos_col] = l;
        --pos_col;
        last_move_dir = 'l';
    } else {
        print("ERROR: could not move from start");
    }

    ++stepn;
    print("Move: %c Step: %d Pipe: %c\n", last_move_dir, stepn, lines[pos_line][pos_col]);
    
    while(not is_path(lines[pos_line][pos_col])) {
        cchar cur_pipe = lines[pos_line][pos_col];
        
        if        (/*GO TOP*/ last_move_dir != 'b' and char_in_(cur_pipe, TOPS)) {
            lines[pos_line][pos_col] = t;
            --pos_line; 
            last_move_dir = 't';
        } else if (/*GO RIGHT*/ last_move_dir != 'l' and char_in_(cur_pipe, RIGHTS)) {
            lines[pos_line][pos_col] = r;
            ++pos_col;
            last_move_dir = 'r';
        } else if (/*GO BOTTOM*/ last_move_dir != 't' and char_in_(cur_pipe, BOTTOMS)) {
            lines[pos_line][pos_col] = b;
            ++pos_line;
            last_move_dir = 'b';
        } else if (/*GO LEFT*/ last_move_dir != 'r' and char_in_(cur_pipe, LEFTS)) {
            lines[pos_line][pos_col] = l;
            --pos_col;
            last_move_dir = 'l';
        } else {
            print("ERROR: pipe unrecognized %c %d %d\n", cur_pipe, pos_line, pos_col);
            print("Move: %c Step: %d Pipe: %c\n", last_move_dir, stepn, cur_pipe);
            break;
        }

        ++stepn;
        print("Move: %c Step: %d Pipe: %c\n", last_move_dir, stepn, lines[pos_line][pos_col]);
    }

    printf("Reached S in [%d] steps (farthest: %d) \n", (stepn - 1), stepn/2);

    transform_pipes(lines_len, lines);

    fill_outs_matrix(lines_len, lines);
    fill_ins_matrix(lines_len, lines);

    print_matrix(lines_len, lines);

    printf("# tiles: %d\n", count_I(lines_len, lines));

    // for (int i = 128; i < 256; ++i) {
    //     printf(" %c : %d (%d) \n", (char)i, i, (char)i);
    // }    
} 


int main(void) {
    static char charbuffer[Next_power2(64000)] = {0};
    static mstr lines[256] = {0};
    Long charbuffer_cap = 64000;
    int lines_cap = 256;

    int lines_len = file_to_lines("./txts/aoc.txt", \
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

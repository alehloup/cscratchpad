#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

#define print(...)  //printf(__VA_ARGS__)

#define v -66
#define I -78

_fun i32 count_I(len32 lines_len, mstr lines[64]) {
    i32 count = 0;
    for (idx32 iline = 0; iline < lines_len; ++iline) {
        for (idx32 icol = 0; lines[iline][icol]; ++icol) {
            count += lines[iline][icol] == I ? 1 : 0;
        }
    }    

    return count;
}

char char_to_fill = '`';

// returns 1 if X or I, 0 if pipe, -99999 if out of bounds
_fun i32 fill(idx32 line, idx32 col, len32 lines_len, mstr lines[64]) {
    i32 is_enclosed = 0;
    cchar cur = (line < 0 or col < 0 or line >= lines_len or !lines[line][col])
        ? 'N' : lines[line][col];
    cb32 isletter = (cur >= '`' and cur <= 'z');
    
    switch(cur) {
        case 'N': return -99999; // out of bounds
        
        case 'X': case 'S': case '*': case '_': // inside enclosement
        case '^': case v: case '>': case '<': 
            lines[line][col] =  cur; return 1; 

        case 'I': case ' ': case '?': return -1; // do not consider because its being investigated

        case '|': case 'L': case 'J': case '-': case 'F': case '7':
            lines[line][col] = ' ';
            return -99999; // pipe

        case '.':
            lines[line][col] = '?';
                is_enclosed = 
                    fill(line - 1, col, lines_len, lines)
                    + fill(line, col + 1, lines_len, lines)
                    + fill(line + 1, col, lines_len, lines)
                    + fill(line, col - 1, lines_len, lines)

                    + fill(line - 1, col - 1, lines_len, lines)
                    + fill(line - 1, col + 1, lines_len, lines)
                    + fill(line + 1, col - 1, lines_len, lines)
                    + fill(line + 1, col + 1, lines_len, lines)
                ;
                lines[line][col] = is_enclosed > 0 ? I : char_to_fill;
                return cur == '#' || isletter ? -1 : -99999;

        default: // letter
            return -1;
            
            
    }
}

_proc print_matrix(len32 lines_len, mstr lines[64]) {
    printf("\nMATRIX:\n");
    for (idx32 i = 0; i < lines_len; ++i) {
        printf("%s\n", lines[i]);
    }
    printf("\n");
}

//AoC 10
_proc aoc(ci32 lines_len, mstr lines[64]) {
    static ccstr TOPS = "|LJ";
    static ccstr RIGHTS = "-FL";
    static ccstr BOTTOMS = "|F7";
    static ccstr LEFTS = "-J7";
    
    idx32 pos_line = 0, pos_col = 0;
    i32 stepn = 0;
    char last_move_dir = '.';

    for (idx32 iline = 0; iline < lines_len; ++iline) {
        ccstr line = lines[iline];
        idx32 s_pos = -1;
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

    ++stepn;
    print("Move: %c Step: %d Pipe: %c\n", last_move_dir, stepn, lines[pos_line][pos_col]);
    
    while(lines[pos_line][pos_col] != 'S') {
        cchar cur_pipe = lines[pos_line][pos_col];
        
        if        (/*GO TOP*/ last_move_dir != 'b' and char_in_(cur_pipe, TOPS)) {
            lines[pos_line][pos_col] = '^';
            --pos_line; 
            last_move_dir = 't';
        } else if (/*GO RIGHT*/ last_move_dir != 'l' and char_in_(cur_pipe, RIGHTS)) {
            lines[pos_line][pos_col] = '>';
            ++pos_col;
            last_move_dir = 'r';
        } else if (/*GO BOTTOM*/ last_move_dir != 't' and char_in_(cur_pipe, BOTTOMS)) {
            lines[pos_line][pos_col] = v;
            ++pos_line;
            last_move_dir = 'b';
        } else if (/*GO LEFT*/ last_move_dir != 'r' and char_in_(cur_pipe, LEFTS)) {
            lines[pos_line][pos_col] = '<';
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

    for (idx32 iline = 0; iline < lines_len; ++iline) {
        char_to_fill = (char)(96 + (iline % 26));
        for (idx32 icol = 0; lines[iline][icol]; ++icol) {
            stepn = fill(iline, icol, lines_len, lines);
        }
    }

    print_matrix(lines_len, lines);

    printf("# tiles: %d\n", count_I(lines_len, lines));

    for (idx32 i = 128; i < 256; ++i) {
        printf(" %c : %d (%d) \n", (char)i, i, (char)i);
    }    
} 


i32 main(void) {
    static char charbuffer[Next_power2(64000)] = {0};
    static mstr lines[256] = {0};
    i64 charbuffer_cap = 64000;
    i32 lines_cap = 256;

    i32 lines_len = file_to_lines("./txts/aoc.txt", \
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

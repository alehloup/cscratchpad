#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

#define print(...)  printf(__VA_ARGS__)

#define t '^'
#define b 'v'
#define r '>'
#define l '<'

//F7LJ
#define cornerF '{'
#define corner7 '}'
#define cornerL '['
#define cornerJ ']'

#define I '*'
#define O '\''

#define empty ' '
#define visited ','
#define pipe 'x'

typedef char (*padded_matrix_ptr)[300][300];
static int padded_cols_len = 0;
static int padded_lines_len = 0;

_fun int count_I(padded_matrix_ptr padded_ptr) {
    int count = 0;
    for (int iline = 0; iline < padded_lines_len; ++iline) {
        for (int icol = 0; (*padded_ptr)[iline][icol]; ++icol) {
            count += (*padded_ptr)[iline][icol] == I ? 1 : 0;
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

_math int is_path(char tile) {
    return tile == 'S' 
        || tile == t || tile == r || tile == b || tile == l 
        || tile == cornerF || tile == corner7 
        || tile == cornerL || tile == cornerJ;
}

_fun int out_of_bounds(int line, int col) {
    return (line < 0 or col < 0 or line >= (padded_lines_len - 1) or col >= (padded_cols_len - 1));
}

_proc fill(char char_to_fill, int line, int col, padded_matrix_ptr padded) {
    if (
        out_of_bounds(line, col)
        || is_path((*padded)[line][col])
        || (*padded)[line][col] == O
        || (*padded)[line][col] == I
        || (*padded)[line][col] == visited
    ) {
        return;
    }

    (*padded)[line][col] = (*padded)[line][col] == empty ? visited : char_to_fill;

    fill(char_to_fill, line-1, col, padded);
    fill(char_to_fill, line, col+1, padded);
    fill(char_to_fill, line+1, col, padded);
    fill(char_to_fill, line, col-1, padded);
}

_fun int erase(char to_erase) {
    return to_erase == empty or to_erase == visited or to_erase == pipe or to_erase == O;
}

_proc fill_matrix(padded_matrix_ptr padded) {
    fill(O, 0, 0, padded);
    for (int iline = 0; iline < padded_lines_len; ++iline) {
        for (int icol = 0; (*padded)[iline][icol]; ++icol) {
            fill(I, iline, icol, padded);
        }
    }

    for (int iline = 0; iline < padded_lines_len; ++iline) {
        for (int icol = 0; (*padded)[iline][icol]; ++icol) {
            (*padded)[iline][icol] = erase((*padded)[iline][icol]) ? ' ' : (*padded)[iline][icol];
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

_fun padded_matrix_ptr padded_matrix(int lines_len, mstr lines[64]) {
    static char padded[300][300] = {0};
    int cols_len = (int) cstrlen(lines[0]);
    int new_cols_len = (cols_len * 2) + 2;
    int new_lines_len = (lines_len * 2) + 2;
    
    for (int i = 0; i < new_lines_len; ++i) {
        for (int j = 0; j < new_cols_len; ++j) {
            padded[i][j] = empty;
        }
    }

    for (int i = 0, ni = 1; i < lines_len; ++i, ni+=2) {
        for (int j = 0, nj = 1; j < cols_len; ++j, nj+=2) {
            char tile = lines[i][j];
            
            padded[ni][nj] = tile;
            if (tile == t or tile == b) {
                padded[ni+1][nj] = tile;
            } else if (tile == r or tile == l) {
                padded[ni][nj+1] = tile;
            } else if (tile == cornerF) {
                padded[ni+1][nj] = lines[i+1][j];
                padded[ni][nj+1] = lines[i][j+1];
            } else if (tile == corner7) {
                padded[ni+1][nj] = lines[i+1][j];
            } else if (tile == cornerL) {
                padded[ni][nj+1] = lines[i][j+1];
            }

        }
    }

    padded_cols_len = (int) cstrlen(padded[0]);
    for (padded_lines_len = 0; padded[padded_lines_len][0]; ++padded_lines_len);

    return &padded;
}

_proc print_padded_matrix(padded_matrix_ptr matrix) {
    printf("\nMATRIX %dx%d :\n", padded_lines_len, padded_cols_len);
    for (int i = 0; (*matrix)[i][0]; ++i) {
        printf("%s\n", (*matrix)[i]);
    }
    printf("\n");
}

//AoC 10
_proc aoc(int lines_len, mstr lines[64]) {
    static ccstr TOPS = "|LJ";
    static ccstr RIGHTS = "-FL";
    static ccstr BOTTOMS = "|F7";
    static ccstr LEFTS = "-J7";

    padded_matrix_ptr padded = 0;
    
    int pos_line = 0, pos_col = 0;
    int stepn = 0;
    char last_move_dir = '.';
    char cur_pipe = '.';

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
        char past_move_dir = last_move_dir;
        int cur_line = pos_line, cur_col = pos_col;
        cur_pipe = lines[pos_line][pos_col];
        
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

             if (last_move_dir == 'r' and past_move_dir == 't') lines[cur_line][cur_col] = cornerF;
        else if (last_move_dir == 'r' and past_move_dir == 'b') lines[cur_line][cur_col] = cornerL;
        else if (last_move_dir == 'l' and past_move_dir == 't') lines[cur_line][cur_col] = corner7;
        else if (last_move_dir == 'l' and past_move_dir == 'b') lines[cur_line][cur_col] = cornerJ;
        else if (last_move_dir == 't' and past_move_dir == 'l') lines[cur_line][cur_col] = cornerL;
        else if (last_move_dir == 't' and past_move_dir == 'r') lines[cur_line][cur_col] = cornerJ;
        else if (last_move_dir == 'b' and past_move_dir == 'l') lines[cur_line][cur_col] = cornerF;
        else if (last_move_dir == 'b' and past_move_dir == 'r') lines[cur_line][cur_col] = corner7;

        ++stepn;
        //print("Move: %c Step: %d Pipe: %c\n", last_move_dir, stepn, lines[pos_line][pos_col]);
    }

    cur_pipe = lines[pos_line][pos_col];
         if (cur_pipe == r and last_move_dir == 't') lines[pos_line][pos_col] = cornerF;
    else if (cur_pipe == r and last_move_dir == 'b') lines[pos_line][pos_col] = cornerL;
    else if (cur_pipe == l and last_move_dir == 't') lines[pos_line][pos_col] = corner7;
    else if (cur_pipe == l and last_move_dir == 'b') lines[pos_line][pos_col] = cornerJ;
    else if (cur_pipe == t and last_move_dir == 'l') lines[pos_line][pos_col] = cornerL;
    else if (cur_pipe == t and last_move_dir == 'r') lines[pos_line][pos_col] = cornerJ;
    else if (cur_pipe == b and last_move_dir == 'l') lines[pos_line][pos_col] = cornerF;
    else if (cur_pipe == b and last_move_dir == 'r') lines[pos_line][pos_col] = corner7;

    printf("Reached S in [%d] steps (farthest: %d) \n", (stepn - 1), stepn/2);

    transform_pipes(lines_len, lines);
    //print_matrix(lines_len, lines);

    padded = padded_matrix(lines_len, lines);

    fill_matrix(padded);

    //print_padded_matrix(padded);

    printf("# tiles: %d\n", count_I(padded));
} 


int main(void) {
    static char charbuffer[Next_power2(64000)] = {0};
    static mstr lines[256] = {0};
    Long charbuffer_cap = 64000;
    int lines_cap = 256;

    int lines_len = file_to_lines("./txts/big.txt",
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

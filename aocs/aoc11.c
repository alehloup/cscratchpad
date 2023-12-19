#include <math.h>
#include <stdio.h>
#include <time.h>
#include "../ale.h"

static len32 M = 0, N = 0;
static char matrix[512][512] = {0};

typedef struct coord{idx32 line; idx32 column;}coord;
_proc coord_printf(coord c) {
    printf("(%d,%d) ", c.line, c.column);
}
_fun i64 distance(coord coord1, coord coord2) {
    i32 lower_line    = min_(coord1.line, coord2.line);
    i32 higher_line   = max_(coord1.line, coord2.line);
    i32 lower_column  = min_(coord1.column, coord2.column);
    i32 higher_column = max_(coord1.column, coord2.column);
    i64 dist = 0;

    for (idx32 icolumn = lower_column; icolumn < higher_column ; ++icolumn) {
        cchar tile = matrix[lower_line][icolumn];
        dist += tile == ',' or tile == '-' ? 999999 : 1;
    }
    for (idx32 iline = lower_line; iline < higher_line ; ++iline) {
        cchar tile = matrix[iline][lower_column];
        dist += tile == ',' or tile == '-' ? 999999 : 1;
    }

    return dist;
}

static coord galaxies[512] = {{0,0}};
static len32 galaxies_len = 0;

_proc initialize_matrix(mstr lines[2]) {
    len32 newM = M, newN = N;
    b32 column_has_galaxy[256] = {0};

    for (idx32 iline = 0, newiline = 0; iline < M; ++iline, ++newiline) {
        b32 line_has_galaxy = False;
        for (idx32 icol = 0; icol < N; ++icol) {
            matrix[newiline][icol] = lines[iline][icol];
            if (lines[iline][icol] == '#') {
                line_has_galaxy = True;
                column_has_galaxy[icol] = True;
            }   
        }
        if (not line_has_galaxy) {
            for (idx32 icol = 0; icol < N; ++icol) {
                matrix[newiline+1][icol] = ',';
            }
            ++newM;
            ++newiline;
        }
    }
    M = newM;
        
    for (idx32 icolumn = 0, adjust = 0; icolumn < newN; ++icolumn) {
        if (not column_has_galaxy[icolumn-adjust]) {
            for (idx32 iline = 0; iline < M; ++iline) {
                for (idx32 icol = newN; icol > icolumn; --icol) {
                    matrix[iline][icol] = matrix[iline][icol-1];
                }
                matrix[iline][icolumn] = '-';
            }
            ++newN;
            ++icolumn;
            ++adjust;
        }
    }
    N = newN;
    (void) newN;
    matrix_printf("%c ", M, N, matrix);
}

static void get_galaxies(void) {
    for (idx32 iline = 0; iline < M; ++iline) {
        for (idx32 icolumn = 0; icolumn < N; ++icolumn) {
            if (matrix[iline][icolumn] == '#') {
                coord galaxy = {iline, icolumn};
                galaxies[galaxies_len++] = galaxy;
            }
        }
    }
}

//AoC 11
_proc aoc(len32 lines_len, mstr lines[2]) {
    i64 sum_dists = 0;
    M = lines_len;
    N = cstrlen32(lines[0]);
    
    initialize_matrix(lines);
    get_galaxies();

    for (idx32 i = 0; i < galaxies_len; ++i) {
        for (idx32 j = i+1; j < galaxies_len; ++j) {
            i64 dist = distance(galaxies[i], galaxies[j]);
            printf("Coord1: "); coord_printf(galaxies[i]); 
            printf("Coord2: "); coord_printf(galaxies[j]);
            printf(" distance: %lld\n", dist);
            sum_dists += dist;
        }
    }
    printf("Sum dists: %lld\n", sum_dists);
} 


i32 main(void) {
    static char charbuffer[Next_power2(64000)] = {0};
    static mstr lines[256] = {0};
    i64 charbuffer_cap = 64000;
    i32 lines_cap = 256;

    len32 lines_len = file_to_lines("./inputs/aoc11.txt",
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

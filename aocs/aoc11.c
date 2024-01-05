#include <math.h>
#include <stdio.h>
#include <time.h>
#include "../ale.h"

static int M = 0, N = 0;
static char matrix[512][512] = {0};

typedef struct coord{int line; int column;}coord;
_proc coord_printf(coord c) {
    printf("(%d,%d) ", c.line, c.column);
}
_fun Long distance(coord coord1, coord coord2) {
    int lower_line    = min_(coord1.line, coord2.line);
    int higher_line   = max_(coord1.line, coord2.line);
    int lower_column  = min_(coord1.column, coord2.column);
    int higher_column = max_(coord1.column, coord2.column);
    Long dist = 0;

    for (int icolumn = lower_column; icolumn < higher_column ; ++icolumn) {
        char tile = matrix[lower_line][icolumn];
        dist += tile == ',' or tile == '-' ? 999999 : 1;
    }
    for (int iline = lower_line; iline < higher_line ; ++iline) {
        char tile = matrix[iline][lower_column];
        dist += tile == ',' or tile == '-' ? 999999 : 1;
    }

    return dist;
}

static coord galaxies[512] = {{0,0}};
static int galaxies_len = 0;

_proc initialize_matrix(mstr lines[2]) {
    int newM = M, newN = N;
    int column_has_galaxy[256] = {0};

    for (int iline = 0, newiline = 0; iline < M; ++iline, ++newiline) {
        int line_has_galaxy = False;
        for (int icol = 0; icol < N; ++icol) {
            matrix[newiline][icol] = lines[iline][icol];
            if (lines[iline][icol] == '#') {
                line_has_galaxy = True;
                column_has_galaxy[icol] = True;
            }   
        }
        if (not line_has_galaxy) {
            for (int icol = 0; icol < N; ++icol) {
                matrix[newiline+1][icol] = ',';
            }
            ++newM;
            ++newiline;
        }
    }
    M = newM;
        
    for (int icolumn = 0, adjust = 0; icolumn < newN; ++icolumn) {
        if (not column_has_galaxy[icolumn-adjust]) {
            for (int iline = 0; iline < M; ++iline) {
                for (int icol = newN; icol > icolumn; --icol) {
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
    for (int iline = 0; iline < M; ++iline) {
        for (int icolumn = 0; icolumn < N; ++icolumn) {
            if (matrix[iline][icolumn] == '#') {
                coord galaxy = {iline, icolumn};
                galaxies[galaxies_len++] = galaxy;
            }
        }
    }
}

//AoC 11
_proc aoc(int lines_len, mstr lines[2]) {
    Long sum_dists = 0;
    M = lines_len;
    N = cstrint(lines[0]);
    
    initialize_matrix(lines);
    get_galaxies();

    for (int i = 0; i < galaxies_len; ++i) {
        for (int j = i+1; j < galaxies_len; ++j) {
            Long dist = distance(galaxies[i], galaxies[j]);
            printf("Coord1: "); coord_printf(galaxies[i]); 
            printf("Coord2: "); coord_printf(galaxies[j]);
            printf(" distance: %lld\n", dist);
            sum_dists += dist;
        }
    }
    printf("Sum dists: %lld\n", sum_dists);
} 


int main(void) {
    static char charbuffer[Next_power2(64000)] = {0};
    static mstr lines[256] = {0};
    Long charbuffer_cap = 64000;
    int lines_cap = 256;

    int lines_len = file_to_lines("./inputs/aoc11.txt",
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

_fun int per_matrix(int lines_len, Mstr lines[]) {
    static Long lins_hash[64] = {0};
    static Long cols_hash[64] = {0};
    int cols_len = (int)Cstrlen(lines[0]);

    for (int i = 0; i < 64; ++i) lins_hash[i] = 1;
    for (int i = 0; i < 64; ++i) cols_hash[i] = 1;

    for (int j = 0; j < cols_len; ++j) {
        for (int i = 0; i < lines_len and not is_empty_cstr(lines[i]); ++i) 
            cols_hash[j] = (cols_hash[j] << 1) + (lines[i][j] == '#' ? 1 : 0);
        printf("%lld ", cols_hash[j]);
    }
    printf("\n");

    for (int i = 1; i < cols_len; ++i) {
        
    }

    for (int i = 0; i < lines_len and not is_empty_cstr(lines[i]); ++i) {
        for (int j = 0; j < cols_len; ++j) 
            lins_hash[i] = (lins_hash[i] << 1) + (lines[i][j] == '#' ? 1 : 0);
        printf("%lld ", lins_hash[i]);
    }
    printf("\n");
    
    return (int)Cstrlen(lines[0]);
}

//AoC 13
_proc aoc(int lines_len, Mstr lines[]) {
    while (lines_len > 2) {
        int res = per_matrix(lines_len, lines);
        printf(" (%d) \n\n", res);
        
        while (lines_len > 2 and not is_empty_cstr(lines[0])) {
            lines = &lines[1];
            --lines_len;
        }
        lines = &lines[1];
        --lines_len;
    }
} 

#define lines_cap 2048
#define charbuffer_cap (Next_power2(64000))
int main(void) {
    static char charbuffer[charbuffer_cap] = {0};
    static Mstr lines[lines_cap] = {0};

    int lines_len = file_to_lines_including_empty("./txts/small.txt",
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

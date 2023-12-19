#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

_fun i32 npossibilities(len32 groups_len, i32 groups[], len32 line_len, mstr line) {
    i32 npossibili = 0;
    i32 cur_group = groups_len > 0 ? groups[0] : 0;
    
    i32 end = line_len - cur_group;
    
    printf("%s|%d [%d]|%d\n", line, line_len, cur_group, groups_len);

    if (groups_len < 1) {
        return 1;
    }
    
    if (line_len < cur_group) {
        return 0;
    }

    if (line_len == cur_group) {
        for (idx32 i = 0; i < line_len; ++i) {
            if (line[i] == '.') {
                return 0;
            }
        }
        return 1;
    }

    for (idx32 i = 0; i < end; ++i) {
        b32 fit_group = True;
        i32 iplusg = i + cur_group;
        
        for (idx32 j = i; j < iplusg; ++j) {
            cchar letter = line[j];
            if (letter == '.') {
                fit_group = False;
                break;
            }
        }
        if (not fit_group or line[iplusg] == '#') {
            continue;
        }

        npossibili += npossibilities(
            groups_len - 1, &groups[1], 
             line_len - (iplusg + 1), &line[iplusg +1 ]
        );
    }

    return npossibili;
}

_fun i32 solve_line(mstr line) {
    mstr words[4] = {0};
    len32 words_len = split(line, ' ', 4, words);
    mstr groups_str[16] = {0};
    i32  groups[16] = {0};
    len32 groups_len = split(words[1], ',', 16, groups_str);

    printf("%s ", words[0]);

    for (idx32 i = 0; i < groups_len; ++i) {
        sscanf_s(groups_str[i], "%d ", &groups[i]);
        printf("%d ", groups[i]);
    }
    printf("\n");
    
    assert(words_len == 2 && "Error parsing");
    
    return npossibilities(groups_len, groups, cstrlen32(line), line);
}

//AoC 12
_proc aoc(len32 lines_len, mstr lines[2]) {
    i64 sum = 0;
    
    for (idx32 iline = 0; iline < lines_len; ++iline) {
        mstr line = lines[iline];
        i32 res = solve_line(line);
        printf(" = %d\n", res);
        sum += res;
    }

    printf("\nSum: %lld\n", sum);
} 


i32 main(void) {
    static char charbuffer[Next_power2(64000)] = {0};
    static mstr lines[256] = {0};
    i64 charbuffer_cap = 64000;
    i32 lines_cap = 256;

    len32 lines_len = file_to_lines("./txts/aoc.txt",
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

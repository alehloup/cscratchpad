#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

#define X_TIMES 1

_fun i64 npossibilities(i8 groups[], len32 line_len, mstr line) {
    i64 npossibili = 0, past_broken = False;
    i32 cur_group = groups[0] ? groups[0] : 0, end = line_len - cur_group + 1;

    if (not groups[0]) // Base: no more groups
        return not char_in_substr_('#', line, 0, line_len);

    if (line_len == cur_group) // Base: Groups len == Line len
        return not groups[1] and not char_in_substr_('.', line, 0, cur_group);

    if (line_len < cur_group) // Base: end of line
        return 0;  
   
    for (idx32 i = 0; i < end; ++i) { // Do a Recursion for each index
        ci32 iplusg = i + cur_group;

        if (past_broken) return npossibili; // past was unaccounted
        past_broken = line[i] == '#';

        if (line[iplusg] == '#') continue; // Cant fit group since it will colide

        if (char_in_substr_('.', line, i, iplusg)) continue; // Group interrupted

        npossibili += npossibilities(
            &groups[1], line_len - (iplusg + 1), &line[iplusg + 1]
        );
    }
    return npossibili;
}

_fun mstr adjust_springs(mstr line) {
    static bufferchar adjusted_line[128] = {0};
    
    idx32 i = 0;
    for (int times = 0; times < X_TIMES; ++times) {
        for (idx32 iline = 0; line[iline]; ++i, ++iline) {
            adjusted_line[i] = line[iline];
        }
    }
    adjusted_line[i] = 0;

    --i;
    while (adjusted_line[i] == '.') {
        adjusted_line[i--] = 0;
    }

    i = 0;
    while (adjusted_line[i] == '.') {
        ++i;
    }

    return &adjusted_line[i];
}

#define groups_cap 32
len32 max_group = 0, max_groups = 0, max_springs = 0;
_fun i64 solve_line(mstr line) {
    mstr words[4] = {0};
    len32 words_len = split(line, ' ', 4, words);
    mstr groups_str[groups_cap] = {0};
    i8 groups[groups_cap] = {0};
    len32 groups_len = split(words[1], ',', groups_cap, groups_str);

    mstr springs = adjust_springs(line);
    len32 springs_len = cstrlen32(springs);

    idx32 i = groups_len;
    for (idx32 times = 0; times < (X_TIMES - 1); ++times) {
        for (idx32 igroups_str = 0; igroups_str < groups_len; ++i, ++igroups_str) {
            groups_str[i] = groups_str[igroups_str];
        }
    }
    groups_len = i;

    max_groups = max_(max_groups, groups_len);
    max_springs = max_(max_springs, springs_len);

    //printf("{%s} ", springs);

    for (idx32 i = 0; i < groups_len; ++i) {
        sscanf_s(groups_str[i], "%d ", &groups[i]);
        //printf("%d ", groups[i]);
        max_group = max_(max_group, groups[i]);
    }
    
    assert(words_len == 2 && "Error parsing");
    
    return npossibilities(groups, springs_len, springs);
}

//AoC 12
_proc aoc(len32 lines_len, mstr lines[2]) {
    i64 sum = 0;
    
    for (idx32 iline = 0; iline < lines_len; ++iline) {
        mstr line = lines[iline];
        i64 res = solve_line(line);
        //printf(" = %lld\n", res);
        sum += res;
    }

    printf("\nSum: %lld\n", sum);
    printf("MSprings: %d, MGroups: %d, MGroup: %d\n", max_springs, max_groups, max_group);
} 


i32 main(void) {
    static char charbuffer[Next_power2(64000)] = {0};
    static mstr lines[1024] = {0};
    i64 charbuffer_cap = 64000;
    i32 lines_cap = 1024;

    len32 lines_len = file_to_lines("./txts/aoc.txt",
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

#define X_TIMES 1

#define groups_cap 32

static Long (*memo)[512][32] = 0;
_fun Long find_nemo(int line_len, int igroup) {
    if ((*memo)[line_len+1][igroup]) {
        return (*memo)[line_len+1][igroup];
    } else {
        return 0;
    }
}
_fun Long nemo_it(int line_len, int igroup, Long possibilities) {
    (*memo)[line_len+1][igroup] = possibilities + 1;
    return possibilities;
}

_fun Long npossibilities(int groups_len, int groups[], int igroup,  int line_len, mstr line) {
    Long npossibili = 0;
    //Long nemo = 0;
    int end_of_groups = igroup==groups_len, end_of_line = line_len==0;
    int cur_group = groups[igroup], end = line_len - cur_group + 1, past_broken = False;

    if (end_of_groups) return not char_in_substr_('#', line, 0, line_len);
    if (end_of_line or line_len < cur_group) return 0; // Base: end of line
    if (line_len == cur_group) // Base: Groups len == Line len
        return (igroup+1)==groups_len and not char_in_substr_('.', line, 0, cur_group);
        
    //nemo = find_nemo(line_len, igroup); if (nemo) return nemo - 1;
   
    for (int i = 0; i < end; ++i) { // Do a Recursion for each index
        int iplusg = i + cur_group;
        int nextline_len = (line_len - (iplusg + 1));

        if (nextline_len + 1 < 0) continue; 

        if (past_broken) {return npossibili;} past_broken = line[i] == '#'; // past was unaccounted

        if (line[iplusg] == '#') continue; // Cant fit group since it will colide

        if (char_in_substr_('.', line, i, cur_group)) continue; // Group interrupted
        
        //nemo = find_nemo(nextline_len, igroup + 1);
        //if (nemo) { npossibili += (nemo - 1); continue;}
        
        npossibili += npossibilities(groups_len, groups, igroup+1, nextline_len, &line[iplusg+1]);
    }
    return npossibili;
}

_fun mstr adjust_springs(mstr line) {
    static char adjusted_line[128] = {0};
    
    int i = 0;
    for (int times = 0; times < X_TIMES; ++times) {
        for (int iline = 0; line[iline]; ++i, ++iline) {
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

static int max_group = 0, max_groups = 0, max_springs = 0;
_fun Long solve_line(mstr line) {
    Long lmemo [512][32] = {0};
    mstr words[4] = {0};
    int words_len = split(line, ' ', 4, words);
    mstr groups_str[groups_cap] = {0};
    int groups[groups_cap] = {0};
    int groups_len = split(words[1], ',', groups_cap, groups_str);

    mstr springs = adjust_springs(line);
    int springs_len = cstrint(springs);

    int i = groups_len;
    for (int times = 0; times < (X_TIMES - 1); ++times) {
        for (int igroups_str = 0; igroups_str < groups_len; ++i, ++igroups_str) {
            groups_str[i] = groups_str[igroups_str];
        }
    }
    groups_len = i;

    max_groups = max_(max_groups, groups_len);
    max_springs = max_(max_springs, springs_len);


    for (i = 0; i < groups_len; ++i) {
        sscanf(groups_str[i], "%d ", &groups[i]);
        //printf("%d ", groups[i]);
        max_group = max_(max_group, groups[i]);
    }
    
    assert(words_len == 2 && "Error parsing");

    (void) memo;
    memo = &lmemo;

    return npossibilities(groups_len, groups, 0, springs_len, springs);
}

//AoC 12
_proc aoc(int lines_len, mstr lines[2]) {
    Long sum = 0;
    
    for (int iline = 0; iline < lines_len; ++iline) {
        mstr line = lines[iline];
        Long res = solve_line(line);
        printf("%d = %lld\n", iline, res);
        sum += res;
    }

    printf("\nSum: %lld\n", sum);
    printf("MSprings: %d, MGroups: %d, MGroup: %d\n", max_springs, max_groups, max_group);
} 


int main(void) {
    static char charbuffer[Next_power2(64000)] = {0};
    static mstr lines[1024] = {0};
    Long charbuffer_cap = 64000;
    int lines_cap = 1024;

    int lines_len = file_to_lines("./txts/aoc.txt",
        lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    aoc(lines_len, lines);
    print_clock(start);

    return 0;
}

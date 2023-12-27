#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

#define groups_cap 32
static int X_TIMES = 1;

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
    Long npossibili = 0, nemo = 0;
    int end_of_groups = igroup==groups_len, end_of_line = line_len==0;
    int cur_group = groups[igroup], end = line_len - cur_group + 1, past_broken = False;

    if (end_of_groups) return not char_in_substr_('#', line, 0, line_len);// Base: end of groups
    if (end_of_line or line_len <= cur_group) return 0; // Base: end of line
   
    for (int i = 0; i < end; ++i) { // Do a Recursion for each index
        int iplusg = i + cur_group;
        int nextline_len = (line_len - (iplusg + 1));
        Long possib=0;

        if (nextline_len + 1 < 0) return npossibili; 

        if (past_broken) {return npossibili;} past_broken = line[i] == '#'; // past was unaccounted

        if (line[iplusg] == '#') continue; // Cant fit group since it will colide

        if (char_in_substr_('.', line, i, cur_group)) continue; // Group interrupted
        
        nemo = find_nemo(nextline_len, igroup + 1);
        if (nemo) { npossibili += (nemo - 1); continue;}

        possib = npossibilities(groups_len, groups, igroup+1, nextline_len, &line[iplusg+1]);
        npossibili += nemo_it(nextline_len, igroup+1, possib);
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
    adjusted_line[i++] = '.';
    adjusted_line[i++] = 0;

    return adjusted_line;
}

_fun Long solve_line(mstr line) {
    Long lmemo [512][32] = {0};
    mstr words[4] = {0};
    int words_len = split(line, ' ', 4, words);
    mstr groups_str[groups_cap] = {0};
    int groups[groups_cap] = {0};
    int groups_len = split(words[1], ',', groups_cap, groups_str);

    mstr springs = adjust_springs(line);
    int springs_len = (int) cstrlen(springs);

    int i = groups_len;
    for (int times = 0; times < (X_TIMES - 1); ++times) {
        for (int igroups_str = 0; igroups_str < groups_len; ++i, ++igroups_str) {
            groups_str[i] = groups_str[igroups_str];
        }
    }
    groups_len = i;

	printf("%s ", springs);

    for (i = 0; i < groups_len; ++i) {
        sscanf(groups_str[i], "%d ", &groups[i]);
        printf("%d ", groups[i]);
    }
    
    assert(words_len == 2 && "Error parsing");
    
    memo = &lmemo;

    return npossibilities(groups_len, groups, 0, springs_len, springs);
}

//AoC 12
_proc aoc(int lines_len, mstr lines[2]) {
    Long sum = 0;
    
    for (int iline = 0; iline < lines_len; ++iline) {
        mstr line = lines[iline];
        Long res = solve_line(line);
        printf(" = %lld\n", res);
        sum += res;
    }

    printf("\nSum: %lld\n", sum);
} 


_proc solve(ccstr filename, int times) {
    static char charbuffer[Next_power2(64000)] = {0};
    static mstr lines[1024] = {0};
    Long charbuffer_cap = 64000;
    int lines_cap = 1024;

    int lines_len = file_to_lines(filename, lines_cap, lines, charbuffer_cap, charbuffer);

    clock_t start = clock();
    lines[lines_len] = 0;
    X_TIMES = times;
    aoc(lines_len, lines);
    print_clock(start);
}

int main(void) {
	solve("./txts/big.txt", 1);
	solve("./txts/big.txt", 5);
	solve("./txts/small.txt", 5);

	return 0;
}

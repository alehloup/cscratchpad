#include <math.h>
#include <stdio.h>
#include <time.h>
#include "ale.h"

_proc print_encoded(i32 x) {
    static bufferchar str[32] = {0};
    static len32 len = 0;

    len = 0;

    while (x > 1) {
        str[len++] = x % 2 ? '#' : '.';
        x = x >> 1;
    }

    while (len) {
        printf("%c", str[--len]);
    }    
}

_fun i32 encode_it(ccstr s) {
    i32 encoded = 1;

    for (idx32 i = 0; s[i]; ++i) {
        encoded = encoded << 1;
        encoded += s[i] == '#' ? 1 : 0;
    }

    return encoded;
}

_proc print_possibilities(i32 set[HT_CAP]) {
    for (idx32 i = 0; i < HT_CAP; ++i) {
        if (set[i]) {
            printf("{");
            print_encoded(i);
            printf("}, ");
        }            
    }
    printf("\n");
}

_fun i32 npossibilities(len32 groups_len, i32 groups[], len32 line_len, mstr line, i32 cur_encoded) {
    static i32 set[HT_CAP] = {0};
    static len32 set_len = 0;

    i32 npossibili = 0;
    i32 cur_group = groups_len > 0 ? groups[0] : 0;
    i32 end = line_len - cur_group + 1;
    b32 past_broken = False;
    b32 print_possib = False;

    if (cur_encoded == 1) {
        for (idx32 i = 0; i < HT_CAP; ++i) {
            set[i] = 0;
        }
        print_possib = False; //True;
    }

    if (groups_len == 0) {
        for (idx32 i = 0; i < line_len; ++i) {
            if (line[i] == '#') {
                return 0;
            }
            cur_encoded = cur_encoded << 1;
        }

        return i32_in_ht_(cur_encoded, set, &set_len) > 0 ? 0 : 1; 
    }
    
    if (line_len < cur_group) {
        return 0;
    }

    if (line_len == cur_group) {
        if (groups_len > 1) {
            return 0;
        }

        for (idx32 i = 0; i < cur_group; ++i) {
            cur_encoded = cur_encoded << 1;
            if (line[i] == '.') {
                return 0;
            }
            cur_encoded += 1;
        }

        return i32_in_ht_(cur_encoded, set, &set_len) > 0 ? 0 : 1;
    }

    for (idx32 i = 0; i < end; ++i) {
        b32 fit_group = True;
        i32 iplusg = i + cur_group;

        i32 advance_encoded = cur_encoded;

        if (past_broken) {
            break;
        }

        cur_encoded = cur_encoded << 1;
        cur_encoded += (past_broken = line[i] == '#');


        for (idx32 j = i; j < iplusg; ++j) {
            cchar letter = line[j];
            advance_encoded = advance_encoded << 1;

            if (letter == '.') {
                fit_group = False;
            } else {
                advance_encoded += 1;
            }
        }
        if (not fit_group or line[iplusg] == '#') {
            continue;
        } else {
            if (iplusg < line_len) {
                advance_encoded = advance_encoded << 1;
            }

            npossibili += npossibilities(
                groups_len - 1, &groups[1], 
                line_len - (iplusg + 1), &line[iplusg + 1 ],
                advance_encoded
            );
        }
    }

    if (print_possib) {
        print_possibilities(set);
    }

    return npossibili;
}

_fun mstr adjust_springs(mstr line) {
    static bufferchar adjusted_line[128] = {0};
    
    idx32 i = 0;
    for (i = 0; line[i]; ++i) {
        adjusted_line[i] = line[i];
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

len32 max_group = 0, max_groups = 0, max_springs = 0;
_fun i32 solve_line(mstr line) {
    mstr words[4] = {0};
    len32 words_len = split(line, ' ', 4, words);
    mstr groups_str[16] = {0};
    i32  groups[16] = {0};
    len32 groups_len = split(words[1], ',', 16, groups_str);

    mstr springs = adjust_springs(line);
    len32 springs_len = cstrlen32(springs);

    max_groups = max_(max_groups, groups_len);
    max_springs = max_(max_springs, springs_len);

    //printf("{%s} ", springs);

    for (idx32 i = 0; i < groups_len; ++i) {
        sscanf_s(groups_str[i], "%d ", &groups[i]);
        //printf("%d ", groups[i]);
        max_group = max_(max_group, groups[i]);
    }
    
    assert(words_len == 2 && "Error parsing");
    
    return npossibilities(
        groups_len, groups, 
        springs_len, springs, 
        1
    );
}

//AoC 12
_proc aoc(len32 lines_len, mstr lines[2]) {
    i64 sum = 0;
    
    for (idx32 iline = 0; iline < lines_len; ++iline) {
        mstr line = lines[iline];
        i32 res = solve_line(line);
        //printf(" = %d\n", res);
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

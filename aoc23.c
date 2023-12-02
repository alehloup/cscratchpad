#include "ale_io.h"

_proc_rbuffer(1, 2)
aoc3(i32 lineslen, cstr lines[]) {
    i8 reds = 12, greens = 13, blues = 14;
    printf("Reds: %d, Greens: %d, Blues: %d\n", reds, greens, blues);

    for (int iline = 0; iline < lineslen; ++iline) {
        cstr line = lines[iline];
     
        printf("%d: %s\n", iline+1, line);
    }

}

_proc_rbuffer(1, 2)
aoc1(i32 lineslen, cstr lines[]) {
    i64 sum = 0;
    for (int iline = 0; iline < lineslen; ++iline) {
        cstr line = lines[iline];
        i8 first = -1, last = -1;

        if (is_empty_string(line)) {
            continue;
        }
        
        for (int i = 0; line[i]; ++i) {
            i8 digit = named_digit(&line[i]);
            if (digit > -1) {
                if (first == -1) {
                    first = digit;
                } 
                last = digit;
            }
        }
        printf("%s: %d%d\n", line, first, last);
        sum += first*10 + last;
    }    
    printf("Sum: %lld\n", sum);
}

i32 main(void) {
    static u8 mem[128*MBs_] = {0};
    arena_t arena = newarena(128*MBs_, mem);

    vector64_t lines = file_to_nonempty_lines(&arena, "aoc.txt");
    cstr *data = vec_data_as_cstr(&lines);

    aoc3(lines.len, data);

    return 0;
}

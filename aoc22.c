#include "ale_io.h"

_proc_rbuffer(1, 2) 
aoc2(i32 lineslen, cstr lines[]) {

    // eu - oponente:
    //     pedra (1) - papel (2) = -1  (perdi)
    //     pedra (1) - tesoura(3) = -2 (ganhei)

	// papel(2) - pedra(1) = 1 (ganhei)
	// papel(2) - tesoura(3) = -1 (perdi)

	// tesoura(3) - pedra(1) = 2 (perdi)
	// tesoura(3) - papel(2) = 1 (ganhei)

    // Ganhei: 1, -2
    // Perdi: -1, 2
    // Empate: 0

    cstr hands[4] = {"", "ROCK", "PAPER", "SCISSORS"};
    i32 hand_score[3] = {/*rock*/ 1, /*paper*/2, /*scissors*/3};
    static const i32 won=6, draw=3, lost=0;
    i32 res_score[5] = {won, lost, draw, won, lost};
    i32 score = 0;

    for (i32 i = 0; i < lineslen; ++i) {
        if (!is_empty_string(lines[i])) {
            i32 op = hand_score[lines[i][0] - 'A'], me = hand_score[lines[i][2] - 'X'];
            i32 res = res_score[me - op + 2];
            score += (me + res);
            printf("%d eu:%s X op:%s | %d + %d (%d)\n", i+1, hands[me], hands[op], me, res, me+res);
        }
    }
    printf("Max Score: %d\n", score);
}

_proc_rbuffer(1, 2)
aoc1(i32 lineslen, cstr lines[]) {
    i64 best3[3] = {0, 0, 0};
    i64 cur = 0, elf = 0;
    for (i32 i = 0; i < lineslen; ++i) {
        cstr line = lines[i];
        if (is_empty_string(line)) {
            if (cur > best3[0]) {
                best3[2] = best3[1];
                best3[1] = best3[0];
                best3[0] = cur;
            } else if (cur > best3[1]) {
                best3[2] = best3[1];
                best3[1] = cur;
            } else if (cur > best3[2]) {
                best3[2] = cur;
            }
            printf("\n %lldelf: %lld \n", ++elf, cur);
            cur = 0;
            continue;
        }
        cur += cstr_to_num(line);
    }
    printf("%lld + %lld + %lld = %lld\n", 
        best3[0], best3[1], best3[2], best3[0] + best3[1] + best3[2]);
}

i32 main() {
    static u8 mem[128*MBs_] = {0};
    arena_t arena = newarena(128*MBs_, mem);

    vector64_t lines = file_to_lines(&arena, "aoc.txt");
    cstr *data = vec_data_as_cstr(&lines);

    aoc2(lines.len, data);

    return 0;
}

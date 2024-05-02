#include "ale.h"

struct len_count_t { int32_t len; int64_t count; };

NEWCMPF_(ss_cmp, struct sslice_t, return sslice_cmp_locale(a, b))
NEWCMPF_(lens_cmp, struct len_count_t, return (int32_t)(b.count - a.count))

static struct sslice_t cities[1<<15] = ZERO_INIT_;

fun_ uint64_t hashss(struct sslice_t text_slice) {
    struct sslice_t one2nine = subss(text_slice, 1, 9);

    uint64_t h = 0, sum = 0;
    for (int64_t i = 0; i < one2nine.len; ++i) {
        sum += (uint64_t) one2nine.text[i];
        h = sum;
        sum = sum << 8;
    }

    h = (h % 13087) % 6570;

    return h;
}

proc_ run(void) {
    struct mmap_t map = mmap_open("./measurements10k.txt");
    
    struct sslice_t lines[32000];
    int64_t lines_len = 0;
    buffer_to_lines(map.contents, map.filesize, CAP_(lines), lines, &lines_len);

    int64_t lens_count[128] = ZERO_INIT_;
    int64_t cities_len = 0;

    for (int64_t i = 0; i < lines_len; ++i) {
        struct sslice_t ss = {char_pos_slice(';', lines[i]), lines[i].text };
        int32_t pos = ht_sslice_upsert(ss, CAP_(cities), cities, &cities_len);
        (void) pos;

        ++lens_count[lines[i].len];
    }

    struct sslice_t cities_arr[1024];
    int64_t cities_arr_len = 0;

    ht_sslice_to_arr(
        CAP_(cities), cities,
        CAP_(cities_arr), cities_arr, &cities_arr_len
    );

    qsort(cities_arr, (size_t)cities_arr_len, sizeof(cities_arr[0]), ss_cmp);

    for (int i = 0; i < cities_arr_len; ++i) {
        // printf("[");
        // sslice_printend(cities_arr[i], " : ");
        // printf("%llu]\n", hashss(cities_arr[i]));
        sslice_printend(cities_arr[i], "\n");
    }

    struct len_count_t counts[64] = ZERO_INIT_;
    int64_t counts_len = 0;

    for (int i = 0; i < CAP_(lens_count); ++i) {
        if (lens_count[i]) {
            counts[counts_len++] = STRUCT_(len_count_t, i, lens_count[i]);
        }
    }

    qsort(counts, (size_t)counts_len, sizeof(counts[0]), lens_cmp);

    for (int i = 0; i < counts_len; ++i) {
        printf("%d: %lld, ", counts[i].len, counts[i].count);
    }
    printf("\n");

}

int main(void) {
    set_locale_to_utf8();
    start_benchclock();
    run();
    stop_benchclock();
    return 0;
}

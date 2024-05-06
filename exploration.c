#include "ale.h"

struct len_count_t { size_t len; size_t count; };

NEWCMPF_(ss_cmp, struct sslice_t, return sslice_cmp_locale(a, b))
NEWCMPF_(lens_cmp, struct len_count_t, return (int)(b.count - a.count))

static struct sslice_t cities[1<<15] = ZERO_INIT_;

// fun_ size_t hashss(struct sslice_t text_slice) {
//     struct sslice_t one2nine = subss(text_slice, 1, 9);

//     size_t h = 0, sum = 0;
//     for (size_t i = 0; i < one2nine.len; ++i) {
//         sum += (size_t) one2nine.text[i];
//         h = sum;
//         sum = sum << 8;
//     }

//     h = (h % 13087) % 6570;

//     return h;
// }

proc_ run(void) {
    struct mmap_t map = mmap_open("./measurements10k.txt");
    
    struct sslice_t lines[32000];
    size_t lines_len = 0, cities_len = 0, lens_count[128] = ZERO_INIT_;
    
    buffer_to_lines(map.contents, map.filesize, CAP_(lines), lines, &lines_len);

    // Process measurements%SZ.txt putting each city name in the hash table, and each line len in lens_count
    for (size_t i = 0; i < lines_len; ++i) {
        struct sslice_t ss = {char_pos_slice(';', lines[i]), lines[i].text };
        unsigned int pos = ht_sslice_upsert(ss, CAP_(cities), cities, &cities_len);
        (void) pos;

        ++lens_count[lines[i].len];
    }

    { // Print Cities Names sorted 
        struct sslice_t cities_arr[1024];
        size_t cities_arr_len = 0;

        ht_sslice_to_arr(
            CAP_(cities), cities,
            CAP_(cities_arr), cities_arr, &cities_arr_len
        );

        qsort(cities_arr, cities_arr_len, sizeof(cities_arr[0]), ss_cmp);

        for (size_t i = 0; i < cities_arr_len; ++i) {
            sslice_printend(cities_arr[i], "\n");
        }
    }

    { // Show Counts of len of cities names
        struct len_count_t counts[64] = ZERO_INIT_;
        size_t counts_len = 0;

        for (size_t i = 0; i < CAP_(lens_count); ++i) {
            if (lens_count[i]) {
                counts[counts_len++] = STRUCT_(len_count_t, i, lens_count[i]);
            }
        }

        qsort(counts, counts_len, sizeof(counts[0]), lens_cmp);

        for (size_t i = 0; i < counts_len; ++i) {
            printf("%zu: %zu, ", counts[i].len, counts[i].count);
        }
        printf("\n");
    }
}

int main(void) {
    set_locale_to_utf8();
    start_benchclock();
    run();
    stop_benchclock();
    return 0;
}

#include "./aleh/string.h"
#include "./aleh/htmsi.h"
#include "./aleh/mmap.h"
#include "./aleh/system.h"

struct lencount_t { unsigned int len; unsigned int count; };

static inline int ss_cmp(const void *a_void, const void *b_void) {
    const struct lenstr_t *a = (const struct lenstr_t *)a_void, *b = (const struct lenstr_t *)b_void;
    return lenstr_cmp_locale(*a, *b);
}

static inline int lens_cmp(const void *a_void, const void *b_void) {
    const struct lencount_t *a = (const struct lencount_t *)a_void, *b = (const struct lencount_t *)b_void;
    return (int)(b->count - a->count); /* reverse */
}

#define cities_cap 1<<15
static struct lenstr_t cities[cities_cap] = {{0,0}};

#define lines_cap 32000
static struct lenstr_t lines[lines_cap];

#define cities_arr_cap 1024
static struct lenstr_t cities_arr[1024];

#define lens_count_cap 128

static inline void run(void) {
    size_t filesize = 0, lines_len = 0;
    unsigned int cities_len = 0, lens_count[lens_count_cap] = {0};
    
    char *contents = mmap_open("./measurements10k.txt", "r", &filesize);
    
    buffer_to_lines(contents, filesize, lines_cap, lines, &lines_len);

    { /* Process measurements%SZ.txt putting each city name in the hash table, and each line len in lens_count */
        size_t i;
        for (i = 0; i < lines_len; ++i) {
            unsigned int pos = 0;
            struct lenstr_t ss = {0, 0};

            ss.len = char_pos_lenstr(';', lines[i]);
            ss.str = lines[i].str;

            pos = ht_lenstr_upsert(ss, cities_cap, cities, &cities_len);
            (void) pos;

            ++lens_count[lines[i].len];
        }
    }

    { /* Print Cities Names sorted */
        unsigned int cities_arr_len = 0;
        unsigned int i;

        ht_lenstr_to_arr(
            cities_cap, cities,
            cities_arr_cap, cities_arr, &cities_arr_len
        );

        qsort(cities_arr, (size_t)cities_arr_len, sizeof(cities_arr[0]), ss_cmp);

        for (i = 0; i < cities_arr_len; ++i) {
            lenstr_printend(cities_arr[i], "\n");
        }
    }

    { /* Show Counts of len of cities names */
        struct lencount_t counts[64] = {{0, 0}};
        unsigned int counts_len = 0, i = 0;

        for (i = 0; i < lens_count_cap; ++i) {
            if (lens_count[i]) {
                struct lencount_t cnts = {0, 0};
                cnts.count = i;
                cnts.len = lens_count[i];
                counts[counts_len++] = cnts;
            }
        }

        qsort(counts, (size_t)counts_len, sizeof(counts[0]), lens_cmp);

        for (i = 0; i < counts_len; ++i) {
            printf("%u: %u, ", counts[i].len, counts[i].count);
        }
        printf("\n");
    }

    mmap_close(contents, filesize);
}

int main(void) {
    set_locale_to_utf8();
    start_benchclock();
    run();
    stop_benchclock();
    return 0;
}

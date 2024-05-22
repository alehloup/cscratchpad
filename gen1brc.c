#include "ale.h"

/* #define N_LINES 1000000000 */
/* #define N_LINES_STR "1B" */
#define N_LINES 10000
#define N_LINES_STR "10k"

#define FILENAME "measurements" N_LINES_STR ".txt"

#define nmax_cidades 512

static size_t seed = 2147483269;

static inline void run(void) {
    size_t i;
    struct lenstr_t cidades[nmax_cidades] = {{0, 0}};
    size_t idx = 0, cidades_len = 0, mmap_cidades_len = 0, mmap_measurements_len = 0;

    char *mmap_cidades = 0, *mmap_mmeasurements = 0;

    file_create(FILENAME, (size_t)16 * N_LINES);
    
    mmap_cidades = mmap_open("cidades.txt", "r", &mmap_cidades_len);
    mmap_mmeasurements = mmap_open(FILENAME, "r+", &mmap_measurements_len);
    { /* mmaps opened */
        buffer_to_lines(
        mmap_cidades, mmap_cidades_len, 
        nmax_cidades, cidades, &cidades_len
        );

        for (i = 0; i < N_LINES; ++i) {
            int printit = i % 100 == 0;
            size_t start_idx = idx;
            size_t range = 1001;

            struct lenstr_t cidade = cidades[((size_t)rand()) % cidades_len];
            size_t measurement = ((size_t)rand()) % range;
            
            buffer_append_lenstr(mmap_measurements_len, mmap_mmeasurements, &idx, cidade);
            buffer_append_cstr(mmap_measurements_len, mmap_mmeasurements, &idx, ";");

            if (cidade.len % 2 == 0) {
                range = 552;
            }

            if (((size_t)rand()) % 100 == 1) {
                size_t measurement2 = ((size_t)rand()) % (range - 152);
                
                mmap_mmeasurements[idx++] = '-';
                measurement = measurement < measurement2 ? measurement : measurement2;
            } else {
                size_t measurement2 = ((size_t)rand()) % range;
                
                measurement = measurement > measurement2 ? measurement : measurement2;
            }

            { /* size_t to format DD.D */
                size_t tenths = measurement / 100;
                size_t unities = measurement;
                if (tenths > 0) {
                    mmap_mmeasurements[idx++] = '0' + (char)tenths;
                    unities = unities % 100;
                }
                mmap_mmeasurements[idx++] = '0' + (char)(unities / 10);
                mmap_mmeasurements[idx++] = '.';
                mmap_mmeasurements[idx++] = '0' + (char)(unities % 10);

                mmap_mmeasurements[idx++] = '\n';
            }

            if (printit) {
                printf("%.*s", (int)(idx - start_idx), &mmap_mmeasurements[start_idx]);
            }
        }
        printf("size: %zu\n", idx);
    } /* closing mmaps */
    mmap_close(mmap_cidades, mmap_cidades_len);
    mmap_close(mmap_mmeasurements, mmap_measurements_len);

    {
        FILE *file = fopen_(FILENAME, "r+");
            printf("size to truncate: %zu\n", idx);
            ftruncate_(file, idx);
        fclose(file);
    }
}

int main(void) {
    srand((unsigned int)seed);
    run();
    printf("Done \n");
}

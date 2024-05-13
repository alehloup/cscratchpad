#include "ale.h"

// #define N_LINES 1000000000ll
// #define N_LINES_STR "1B"
#define N_LINES 10000ll
#define N_LINES_STR "10k"

fun_ struct mmap_t read_cidades(const size_t cap, struct sslice_t cidades[], size_t *len) {
    struct mmap_t m = mmap_open("cidades.txt");
    buffer_to_lines(m.contents, m.filesize, cap, cidades, len);
    return m;
} 

proc_ run(void) {
    struct sslice_t cidades[512] = {{.len=0, .text=0}};
    size_t cidades_len = 0;
    
    struct mmap_t mcidades = read_cidades(arrsizeof(cidades), cidades, &cidades_len);
    
    struct mmap_t mmeasurements = mmap_create_for_write("measurements" N_LINES_STR ".txt", 16ll * N_LINES);
    char *buffer = mmeasurements.contents;
    size_t idx = 0;

    size_t seed = 41635984;
    for (size_t i = 0; i < N_LINES; ++i) {
        int printit = i % 100 == 0;
        size_t start_idx = idx;
        size_t range = 1001;

        struct sslice_t cidade = cidades[rnd(&seed) % cidades_len];
        size_t measurement = rnd(&seed) % range;
        
        buffer_appendslice(mmeasurements.filesize, buffer, &idx, cidade);
        buffer_appendcstr(mmeasurements.filesize, buffer, &idx, ";");

        if (cidade.len % 2 == 0) {
            range = 552;
        }

        if (rnd(&seed) % 100 == 1) {
            size_t measurement2 = rnd(&seed) % (range - 152);
            
            buffer[idx++] = '-';
            measurement = measurement < measurement2 ? measurement : measurement2;
        } else {
            size_t measurement2 = rnd(&seed) % range;
            
            measurement = measurement > measurement2 ? measurement : measurement2;
        }

        { // size_t to format DD.D
            size_t tenths = measurement / 100;
            size_t unities = measurement;
            if (tenths > 0) {
                buffer[idx++] = '0' + (char)tenths;
                unities = unities % 100;
            }
            buffer[idx++] = '0' + (char)(unities / 10);
            buffer[idx++] = '.';
            buffer[idx++] = '0' + (char)(unities % 10);

            buffer[idx++] = '\n';
        }

        if (printit) {
            printf("%.*s", (int)(idx - start_idx), &buffer[start_idx]);
        }
    }
    printf("size: %zu\n", idx);

    mmap_close(mcidades);
    mmap_write_close_and_truncate(mmeasurements);
}

int main(void) {
    run();
    printf("Done \n");
}

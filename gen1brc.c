#include "ale.h"

#define N_LINES 1000000000ll
#define N_LINES_STR "1B"

proc_ read_cidades(const int64_t cap, struct sslice_t cidades[], int64_t *len) {
    struct mmap_t m = mmap_open("cidades.txt");
    buffer_to_lines(m.contents, m.filesize, cap, cidades, len);
} 

proc_ run(void) {
    struct sslice_t cidades[512] = ZERO_INIT_; 
    int64_t cidades_len = 0;
    read_cidades(CAP_(cidades), cidades, &cidades_len);
    
    struct mmap_t m = mmap_create_for_write("measurements" N_LINES_STR ".txt", 16ll * N_LINES);
    char *buffer = m.contents;
    int64_t idx = 0;

    uint64_t seed = 41635984;
    for (int i = 0; i < N_LINES; ++i) {
        int printit = i % 100000 == 0;

        struct sslice_t cidade = cidades[rnd(&seed) % cidades_len];
        buffer_appendslice(m.filesize, buffer, &idx, cidade);
        buffer_appendcstr(m.filesize, buffer, &idx, ";");

        if (printit) {
            printf("[%d] ", i);
            sslice_printend(cidade, ";");
        }

        int64_t start_idx = idx;

        int64_t range = 1001;
        if (cidade.len % 2 == 0) {
            range = 552;
        }

        int64_t measurement = rnd(&seed) % range;

        if (rnd(&seed) % 100 == 1) {
            buffer[idx++] = '-';

            int64_t measurement2 = rnd(&seed) % (range - 152);
            measurement = measurement < measurement2 ? measurement : measurement2;
        } else {
            int64_t measurement2 = rnd(&seed) % range;
            measurement = measurement > measurement2 ? measurement : measurement2;
        }

        int64_t tenths = measurement / 100;
        int64_t unities = measurement;
        if (tenths > 0) {
            buffer[idx++] = '0' + (char)tenths;
            unities = unities % 100;
        }
        buffer[idx++] = '0' + (char)(unities / 10);
        buffer[idx++] = '.';
        buffer[idx++] = '0' + (char)(unities % 10);

        buffer[idx++] = '\n';

        if (printit) {
            printf("%.*s", (int32_t)(idx - start_idx), &buffer[start_idx]);
        }
    }
    printf("size: \n%lld\n", idx);

    mmap_write_close_and_truncate(m);
}

int main(void) {
    run();
    printf("Done \n");
}

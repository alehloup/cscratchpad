#include <stdio.h>
#include <stdlib.h>

static void file_to_buffer(const char *filename, char buffer[500]) {
    FILE *f = fopen(filename, "rb");

    if (!f) {
        return;
    }

    if (f) {
        size_t res = fread(buffer, sizeof(char), 27, f); // FANALYZER: FILE LEAK HERE!
    }

    fclose(f);
}

int main(void) {
    char buffer[500] = {};
    file_to_buffer("exemplo.txt", buffer);

    printf("%s", buffer);

    return 0;
}

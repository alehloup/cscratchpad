#include "ale.h"

int main(void) {

    const char *values[MSIEXP] = {0};
    htindex table = {0};

    values[msiins("Alessandro", &table)] = "Stamatto";
    values[msiins("Sarah", &table)] = "Sakamoto";

    printsp(values[msiget("Alessandro", &table)]);
    printsp(values[msiget("Sarah", &table)]);

    return 0;
}

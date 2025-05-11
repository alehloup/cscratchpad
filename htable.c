#include "ale.h"

int main(void) {

    const char *values[MSIEXP] = {0};
    htindex table = {0};

    values[lookup("Alessandro", &table, 1)] = "Stamatto";
    values[lookup("Sarah", &table, 1)] = "Sakamoto";

    printsp(values[lookup("Alessandro", &table, 0)]);
    printsp(values[lookup("Sarah", &table, 0)]);

    return 0;
}

#include "ale.h"

int main(void) {

    const char *values[MSIEXP] = {0};
    htindex table = {0};

    values[msiins("Alessandro", &table)] = "Stamatto";
    values[msiins("Sarah", &table)] = "Sakamoto";

    print_cstr(values[msiget("Alessandro", &table)]);
    print_cstr(values[msiget("Sarah", &table)]);

    return 0;
}

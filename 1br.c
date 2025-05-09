#include "ale.h"

int main(void)
{
    with(contents, mopen("measurements10k.txt", "rb")) {
        println(contents.len);
    }

    return 0;
}

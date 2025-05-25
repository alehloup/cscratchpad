#include "ale.h"

int main(void)
{
    for(str contents = mopen("measurements10k.txt", "rb"); contents.len; mclose(contents), contents.len = 0) {
        println(contents.len);
    }

    return 0;
}

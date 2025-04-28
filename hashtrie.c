#include "aleh/aleh.h"

int main(void) {
    char arr[KB];
    arena a = arr2arena(arr);

    hashtrie_int *ints = 0;
    
    *lookup(&ints, S("Alessandro"), &a) = 52;

    *lookup(&ints, S("Sarah"), &a) = 69;

    int * resint = lookup(&ints, S("Alessandro"), 0);
    if (resint) {
        println(*resint);
    }

    resint = lookup(&ints, S("Sarah"), 0);
    if (resint) {
        println(*resint);
    }

    *lookup(&ints, S("Karol"), &a) = 69;

    foreach(x, (hashtrie_int *)arr, (a.beg - arr)/ssizeof(hashtrie_int)) {
        print(x->key); printf(" "); println(x->value);
    }

    ssize_t base = (ssize_t)a.beg;
    
    hashtrie_str *strings = 0;

    *lookup(&strings, S("Alessandro"), &a) = S("Stamatto");
    *lookup(&strings, S("Sarah"), &a) = S("Sakamoto");

    foreach(x, (hashtrie_str *)base, ((ptrdiff_t)a.beg - base)/ssizeof(hashtrie_str)) {
        print(x->key); printf(" "); println(x->value);
    }

    return 0;
}

#include "aleh/aleh.h"

int main(void) {
    char arr[KB];
    arena a = arr2arena(arr);

    hashtrie_int *ints = 0;
    
    *htset(&ints, S("Alessandro"), &a) = 52;

    *htset(&ints, S("Sarah"), &a) = 69;

    hashtrie_int * resint = htget(&ints, S("Alessandro"));
    if (resint) {
        println(resint->value);
    }

    resint = htget(&ints, S("Sarah"));
    if (resint) {
        println(resint->value);
    }

    *htset(&ints, S("Karol"), &a) = 69;

    foreach(x, (hashtrie_int *)arr, (a.beg - arr)/ssizeof(hashtrie_int)) {
        print(x->key); printf(" "); println(x->value);
    }

    ssize_t base = (ssize_t)a.beg;
    
    hashtrie_str *strings = 0;

    *htset(&strings, S("Alessandro"), &a) = S("Stamatto");
    *htset(&strings, S("Sarah"), &a) = S("Sakamoto");

    foreach(x, (hashtrie_str *)base, ((ptrdiff_t)a.beg - base)/ssizeof(hashtrie_str)) {
        print(x->key); printf(" "); println(x->value);
    }

    return 0;
}

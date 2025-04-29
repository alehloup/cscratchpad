/* 
    hashtrie_ds.h implements:
    |htrie_funname|lookup-function for |hashTrie|struct |htrie_keytype|key -> |htrie_valtype|val

    Define:
        htrie_name: the name of the HashTrie struct
        htrie_keytype: the type of the KEY
        htrie_valtype: the type of the VALUE
        htrie_funname: the name of the lookup function

    Copy:
        #define htrie_name htstr
        #define htrie_keytype str
        #define htrie_valtype str
        #define htrie_funname lookup
*/

/* HASHTRIE */

#ifndef htrie_name
    #define htrie_name htstr
#endif
#ifndef htrie_keytype
    #define htrie_keytype str
#endif
#ifndef htrie_valtype
    #define htrie_valtype str
#endif
#ifndef htrie_funname
    #define htrie_funname lookup
#endif

typedef struct htrie_name htrie_name;
struct htrie_name {
    htrie_name *child[4];
    htrie_keytype key;
    htrie_valtype value;
};

// If null is passed as arena does lookup and might return null, otherwise returns value address
static inline htrie_valtype * htrie_funname(htrie_name **node, htrie_keytype key, arena *a) {
    size_t seed = node ? (size_t)*node : 0;
    for (size_t h = hash64(key, seed); *node; h <<= 2) {
        if (equal(key, (*node)->key)) {
            return &(*node)->value;
        }

        node = &(*node)->child[h >> 62];
    }
    if (!a) return NULL;

    *node =  new(a, 1, htrie_name);
    (*node)->key = key;

    return &(*node)->value;
}

#undef htrie_name
#undef htrie_keytype
#undef htrie_valtype
#undef htrie_funname

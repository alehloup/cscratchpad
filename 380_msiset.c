#include "ale.h"

static uint8_t memory[4*_Mega_Bytes] = {};
static arena_t perm = {};

typedef struct {
    ht32_t table;
    vector32_t cache;
} RandomizedSet;


void print_cache(RandomizedSet* obj) {
    printf("|Cache . .");
    for(int i = 0; i < obj->cache.len; ++i) {
        printf(" %d:%d ", i, obj->cache.data[i]);
    }
    printf("| ");
}

RandomizedSet * randomizedSetCreate() {
    perm = newarena(sizeof(memory), memory);
    RandomizedSet *S = (RandomizedSet *) alloc1(&perm, sizeof(RandomizedSet), alignof(RandomizedSet));
    S->table = new_ht32(&perm, 200000);
    vec_push_i32(&S->cache, &perm, 0);
    return S;
}

bool randomizedSetInsert(RandomizedSet* obj, int val) {
    bool ret = 0;
    if (val == 0) {
        ret = !obj->cache.data[0];
        obj->cache.data[0] = 1;
        return ret;
    }

    int32_t msiidx = htint_get_idx(obj->table, val);
    entry32_t entry = obj->table.data[msiidx];
    if (entry.val) {
        return 0;
    }

    vec_push_i32(&obj->cache, &perm, val);

    obj->table.data[msiidx].key = val;
    obj->table.data[msiidx].val = obj->cache.len - 1;
    ++obj->table.len;

    return 1;
}

bool randomizedSetRemove(RandomizedSet* obj, int val) {
    if (val == 0) {
        if (obj->cache.data[0]) {
            obj->cache.data[0] = 0;
            return 1;
        }
        return 0;
    }

    int32_t msiidx = htint_get_idx(obj->table, val);
    
    if (obj->table.data[msiidx].val) {
        int32_t idxval = obj->table.data[msiidx].val;
        obj->table.data[msiidx].val = 0;

        idxval = idxval < obj->cache.len ? idxval : obj->cache.len - 1;
        while (obj->cache.data[idxval] != val) 
        {
            --idxval;
        }

        

        obj->cache.data[idxval] = obj->cache.data[--obj->cache.len];
        --obj->table.len;
        return 1;
    }

    return 0;
}

int randomizedSetGetRandom(RandomizedSet* obj) {
    static uint64_t seed = 11111111111l;
    int32_t range = obj->cache.len - 1;
    if (obj->cache.data[0]) {
        ++range;
    }



    int32_t choosen = (rnd(&seed) % range) + 1;
    if (choosen == obj->cache.len) {
        return 0;
    }
    return obj->cache.data[choosen];
}

void randomizedSetFree(RandomizedSet* obj) {
    if (!obj) {
        ale_printf("Obj is null!");
    }
    perm.beg = 0;
    perm.end = 0;
}

void printbool(int b) {
    if (b) {
        ale_printf("true, ");
    }
    else {
        ale_printf("false, ");
    };
}

void test() {
    RandomizedSet* obj = randomizedSetCreate();
    ale_printf("[[], ");

    printbool(randomizedSetInsert(obj, 1));

    printbool(randomizedSetRemove(obj, 2));

    printbool(randomizedSetInsert(obj, 2));

    ale_printf("%d, ", randomizedSetGetRandom(obj));

    printbool(randomizedSetRemove(obj, 1));

    printbool(randomizedSetInsert(obj, 2));
    
    ale_printf("]");
    randomizedSetFree(obj);
    
}

int32_t main() {
    test();
    return 0;
}

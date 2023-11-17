#define ale_assert(_cond_, ...)                      \
    if (!(_cond_)) {                                 \
        ale_printf("\n!! [%s:%d] ", __FILE__, __LINE__); \
        ale_printf(__VA_ARGS__);                         \
        ale_printf(" !!\n");                             \
        __builtin_unreachable();                     \
    }                                                \

#include <stdio.h>
#define ale_printf printf

// #undef  ale_printf
// #define ale_printf(format, ...) ;

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint8_t table[536870913] = {};
static int32_t shuffle[200001] = {};

typedef struct {
    uint8_t *table;
    int32_t *shuffle;
    int32_t shuffle_len;
} RandomizedSet;

static RandomizedSet S = {};


RandomizedSet * randomizedSetCreate() {
    S.table = table;
    S.shuffle = shuffle;
    S.shuffle_len = 0;

    memset(table, 0, sizeof(table));
    memset(shuffle, 0, sizeof(shuffle));

    return &S;
}

typedef struct refbit{uint8_t *ref; uint8_t mod_bit; bool was_set;}refbit;
refbit randomizedSetGet(RandomizedSet* obj, int val) {
    uint32_t index = (uint32_t)val;
    int32_t block_index = (int32_t) (index / 8);
    uint8_t mod_bit = 1 << (index % 8);

    uint8_t *block = &(obj->table[block_index]);

    refbit ret = { block, mod_bit, (bool) (*block & mod_bit)};
    return ret;
}

bool randomizedSetInsert(RandomizedSet* obj, int val) {
    refbit x = randomizedSetGet(obj, val);

    ale_printf("\niantes v:%d b:%d m:%d w:%d \n", val, *(x.ref), x.mod_bit, x.was_set);
    *(x.ref) = (*(x.ref) | x.mod_bit);
    ale_printf("idepois v:%d b:%d m:%d w:%d \n", val, *(x.ref), x.mod_bit, x.was_set);

    if (!x.was_set) {
        obj->shuffle[obj->shuffle_len++] = val;
    }

    return !x.was_set;
}

bool randomizedSetRemove(RandomizedSet* obj, int val) {
    refbit x = randomizedSetGet(obj, val);
    
    ale_printf("\nrantes v:%d b:%d m:%d w:%d \n", val, *(x.ref), x.mod_bit, x.was_set);
    *(x.ref) = (*(x.ref) & (uint8_t)(~x.mod_bit));
    ale_printf("rdepois v:%d b:%d m:%d w:%d \n", val, *(x.ref), x.mod_bit, x.was_set);
    
    return x.was_set;
}

static uint64_t seed = 0x9b60933458e17d6dLL;
static int32_t RAND() {
    seed = seed * 0x9b60933458e17d7dLL + 0xd737232eeccdf7edLL;
    int32_t shift = 29 - (uint32_t)(seed >> 61);
    
    return ((int32_t) (seed >> shift)) & 2147483647;
}

int randomizedSetGetRandom(RandomizedSet* obj) {
    int32_t val = 0, idx = 0;

    while(obj->shuffle_len) {
        idx = RAND() % obj->shuffle_len;

        val = obj->shuffle[idx];
        if (randomizedSetGet(obj, val).was_set) {
            return val;
        } else {
            obj->shuffle[idx] = obj->shuffle[--obj->shuffle_len];
        }
    }
    return -1;
}

void randomizedSetFree(RandomizedSet* obj) {
    // free(obj->table);
    // free(obj->shuffle);
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

    printbool(randomizedSetInsert(obj, 0));

    printbool(randomizedSetInsert(obj, 1));

    printbool(randomizedSetRemove(obj, 0));

    printbool(randomizedSetInsert(obj, 2));

    printbool(randomizedSetRemove(obj, 1));

    ale_printf("%d", randomizedSetGetRandom(obj));
    
    ale_printf("]");
    randomizedSetFree(obj);
    
}

int32_t main(int32_t argc, const char * *argv) {
    test();
    return 0;
}

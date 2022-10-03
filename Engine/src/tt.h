#include <cstdint>
#include <memory>

#ifndef TICTACTOE_TT_H
#define TICTACTOE_TT_H

#define PV_NODE  0
#define ALL_NODE 1
#define CUT_NODE 2

// TT size in entries
#define TT_MASK 0xFFFFF

struct Entry {
    uint64_t key;
    uint8_t age;
    uint8_t type;
    uint16_t depth;
    int16_t score;
    int16_t move;

    void set(uint64_t k, uint8_t a, uint8_t t, uint16_t d, int16_t s, int16_t m) {
        this->key   = k;
        this->age   = a;
        this->type  = t;
        this->depth = d;
        this->score = s;
        this->move  = m;
    }
};

class TT {
    int8_t age;
    std::unique_ptr<Entry[]> entries;

    void init();

    public:
    TT() {init();};

    Entry get(uint64_t key);

    void put(uint64_t k, uint8_t t, uint16_t d, int16_t s, int16_t m);

    void incrementAge();

    void prefetch(uint64_t key);
};

#endif
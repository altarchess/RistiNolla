#include "board.h"
#include "tt.h"

#include <cstdint>

struct SearchData {
    TT tt = TT();
    long long time_limit;
    long long start_time;
    int16_t max_depth;
    bool force_quit;

    long long nodes;

    int16_t best_move;
};

int16_t searchRoot(Board* board, SearchData* sd);
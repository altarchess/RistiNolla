#include "board.h"
#include "tt.h"

#include <cstdint>

struct SearchData {
    TT tt = TT();
    long long time_limit;
    int16_t max_depth;
    bool force_quit;

    long long nodes;

    int16_t best_move;

    bool timeLeft();
};

int16_t searchRoot(Board* board, SearchData* sd);
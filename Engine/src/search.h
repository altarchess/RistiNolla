#include "board.h"

struct SearchData {
    long long time_limit;
    long long start_time;
    int max_depth;
    bool force_quit;

    long long nodes;

    int best_move;
};

int searchRoot(Board* board, SearchData* sd);
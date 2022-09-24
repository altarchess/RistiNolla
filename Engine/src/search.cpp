#include "search.h"
#include "board.h"

#include <iostream>

int NegaMax(Board* b, SearchData* sd, int alpha, int beta, int depth, bool root) {
    sd->nodes++;
    if (depth == 0)
        return b->evaluate();
    
    if (b->evaluate() > MIN_MATE_SCORE || b->evaluate() < -MIN_MATE_SCORE) 
        return b->evaluate();

    int best_score = alpha;

    b->generate();
    int m = b->next();

    while (m != -1) {
        b->makeMove(m, 1 + b->getActivePlayer());
        int score = -NegaMax(b, sd, -beta, -alpha, depth - 1, false);
        b->undoMove();
        if (score > best_score) {
            best_score = score;
            if (root)
                sd->best_move = m;
            if (score > alpha)
                alpha = score;
            if (score > beta)
                return score;
        }
        m = b->next();
    }
    return best_score;
}

int searchRoot(Board* b, SearchData*  sd) {
    sd->nodes = 0;
    for (int depth = 1; depth <= sd->max_depth; depth++) {
        int score = NegaMax(b, sd, -MAX_MATE_SCORE, MAX_MATE_SCORE, depth, true);
        std::cout << "depth " << depth << " score " << score << " nodes " << sd->nodes << std::endl;
    }
    return sd->best_move;
}
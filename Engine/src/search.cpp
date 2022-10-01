#include "search.h"
#include "board.h"

#include <iostream>

/***
 *  Basic NegaMax Search 
 *  See https://www.chessprogramming.org/Negamax
 ***/

int NegaMax(Board* b, SearchData* sd, int alpha, int beta, int depth, bool root) {
    sd->nodes++;
    // When we hit depth 0, evaluate the leaves.
    if (depth == 0)
        return b->evaluate();
    
    // if We hit a matescore in the current node, we can exit early.
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
            // Alpha-Beta cutoff.
            if (score > beta)
                return score;
        }
        m = b->next();
    }
    return best_score;
}

/***
 *  Iterative deepening loop
 *  See https://www.chessprogramming.org/Iterative_Deepening
 ***/

int searchRoot(Board* b, SearchData*  sd) {
    sd->nodes = 0;
    for (int depth = 1; depth <= sd->max_depth; depth++) {
        int score = NegaMax(b, sd, -MAX_MATE_SCORE, MAX_MATE_SCORE, depth, true);
        std::cout << "depth " << depth << " score " << score << " nodes " << sd->nodes << std::endl;
    }
    return sd->best_move;
}
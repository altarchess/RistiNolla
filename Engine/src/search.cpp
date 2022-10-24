#include "search.h"
#include "board.h"

#include <iostream>
#include <cstdint>
#include <chrono>

bool SearchData::timeLeft() {
    auto time  = std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::steady_clock::now().time_since_epoch()).count(); 
    return time < time_limit;
};

/***
 *  Basic NegaMax Search 
 *  See https://www.chessprogramming.org/Negamax
 ***/

int16_t negaMax(Board* b, SearchData* sd, int alpha, int beta, int16_t depth, bool root) {
    sd->nodes++;
    // When we hit depth 0, evaluate the leaves.
    if (depth == 0)
        return b->evaluate();

    // Check time manager
    if ((sd->nodes % 16) == 13 && !sd->timeLeft()) {
        sd->force_quit = true;
        return beta;
    } 

    // if We hit a matescore in the current node, we can exit early.
    if (b->evaluate() > MIN_MATE_SCORE || b->evaluate() < -MIN_MATE_SCORE) {
        b->print();
        return b->evaluate();
    }

    Entry en = sd->tt.get(b->hash);
    if (en.key == b->hash) {
        if (en.depth >= depth) {
            if (en.score >= beta ? en.type != ALL_NODE 
              : en.type != CUT_NODE && en.score <= alpha)
                return en.score;
        }
    }

    int16_t best_score   = -MAX_MATE_SCORE;
    bool raise_alpha     = false;

    b->generate();
    int16_t m = b->next();

    while (m != -1 && !sd->force_quit) {
        b->makeMove(m, 1 + b->getActivePlayer(), &sd->tt);
        int score = -negaMax(b, sd, -beta, -alpha, depth - 1, false);
        b->undoMove();
        if (score > best_score) {
            best_score = score;
            if (root && !sd->force_quit)
                sd->best_move = m;
            if (score > alpha) {
                alpha = score;
                raise_alpha = true;
            }
            // Alpha-Beta cutoff.
            if (score >= beta && !sd->force_quit) {
                sd->tt.put(b->hash, CUT_NODE, depth, score, m);
                return score;
            }
        }
        m = b->next();
    }
    // If alpha was raised, store as PV_NODE, otherwise as ALL_NODE
    if (!sd->force_quit)
        sd->tt.put(b->hash, 1 - raise_alpha, depth, best_score, 0);
    return best_score;
}

/***
 *  Iterative deepening loop
 *  See https://www.chessprogramming.org/Iterative_Deepening
 ***/

int16_t searchRoot(Board* b, SearchData*  sd) {
    sd->nodes = 0;
    for (int16_t depth = 1; depth <= sd->max_depth; depth++) {
        int16_t score = negaMax(b, sd, -MAX_MATE_SCORE, MAX_MATE_SCORE, depth, true);
        if (sd->force_quit)
            break;
        std::cout << "depth " << depth << " score " << score << " nodes " << sd->nodes << std::endl;
    }
    return sd->best_move;
}
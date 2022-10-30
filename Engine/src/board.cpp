#include "board.h"

#include <iostream>
#include <cstring>

// Random numbers for each piecetype on each square
uint64_t rand_hash[2 * MAX_BOARD_SIZE];

uint64_t seed;

/***
 * Random number gen
 ***/

uint64_t rand64() {
    seed ^= seed << 13;
    seed ^= seed >> 7;
    seed ^= seed << 17;
    return seed;
}

/***
 * Generate ranom numbers for both piecetypes for all squares
 ***/

void initKeys() {
    seed = 3812734618273475183;
    for (int i = 0; i < 2 * MAX_BOARD_SIZE; i++) {
        rand_hash[i] = rand64();
    }
}

/***
 * Swap index of 2 moves in the MoveList struct.
 ***/
void MoveList::swap(int i_1, int i_2) {
    int move    = moves[i_1];
    int score   = scores[i_1];
    moves[i_1]  = moves[i_2];
    moves[i_2]  = move;
    scores[i_1] = scores[i_2];
    scores[i_2] = score;
}

/***
 *  Add move to movegen active square list
 ***/

void Board::addMoveGenMove(int square) {
    for (int i = 0; i < x_size * y_size; i++) {
        if (move_gen_list[i] == -1 || move_gen_list[i] == square) {
            move_gen_list[i] = square;
            max_active_slots = std::max(max_active_slots, i);
            return;
        } 
    }
}

/***
 *  Add square to movegen square table
 ***/

void Board::addMoveGenSquare(int square, int ply) {
    if (!move_gen_squares[square] && !squares[square]) {
        move_gen_squares[square] = ply;
        addMoveGenMove(square);
    }
}

/***
 *  Remove square from movegen square table
 ***/

void Board::removeMoveGenSquare(int square, int ply) {
    if (move_gen_squares[square] == ply)
        move_gen_squares[square] = 0;
}


/***
 *  Generate all the moves for movelist at current ply
 ***/

void Board::generate(int16_t hash_move, int16_t killer_move) {
    MoveList* mv = &search_move_lists[internal_ply];
    mv->size     = 0;
    mv->searched = 0;
    for (int i = 0; i <= max_active_slots; i++) {
        if (move_gen_list[i] != -1 && move_gen_list[i] != hash_move && move_gen_list[i] != killer_move) {
            // If the square was activated by a child node of sibling or by a sibling, disregard it in move generation.
            if (move_gen_squares[move_gen_list[i]] >= internal_ply) {
                move_gen_list[i] = -1;
            } else if (move_gen_squares[move_gen_list[i]] != 0) {
                mv->scores[mv->size] = history[move_gen_list[i]][active_player];
                mv->moves[mv->size++] = move_gen_list[i];
            }
        }
    }
    // Give the killer move a score above all history scores but below hash move.
    if (killer_move != -1 && squares[killer_move] == 0) {
        mv->scores[mv->size] = 1000;
        mv->moves[mv->size++] = killer_move;
    }
    // Give the hash move a score above killer move and all history scores.
    if (hash_move != -1 && hash_move != killer_move && squares[hash_move] == 0) {
        mv->scores[mv->size] = 1001;
        mv->moves[mv->size++] = hash_move;
    }
}


/***
 *  Get next move in movelist
 ***/

int Board::next() {
    MoveList* mv = &search_move_lists[internal_ply];
    if (!mv->size)
        return -1;
    int best_index = 0;
    for (int i = 1; i < mv->size; i++) {
        if (mv->scores[i] > mv->scores[best_index]) {
            best_index = i;
        }
    }
    mv->swap(mv->size - 1, best_index);
    mv->searched_moves[mv->searched++] =  mv->moves[mv->size - 1];
    return mv->moves[--mv->size];
}

/***
 *  Translate char to piece type for makemove
 ***/

void Board::makeMove(int square, char c) {
    switch (c) {
        case 'e':
            break;
        case 'X':
            makeMove(square, 1);
            break;
        case '0':
            makeMove(square, 2);
            break;
    }
};

/***
 *  Calculates how many in a row we have in specific dir & check for open endedness of row.
 ***/

inARowData Board::pointsInDir(int x, int y, int xd, int yd, int type) {
    x += xd;
    y += yd;
    inARowData ret;
    while (y >= 0 && y <= y_size - 1 && x >= 0 && x <= x_size - 1) { 
        if (squares[y * x_size + x] == type) {
            ret.points++;
        } else if (squares[y * x_size + x] == 0) {
            ret.openended = 1;
            return ret;
        } else {
            return ret;
        }

        x += xd;
        y += yd;
    }
    return ret;
}

/***
 *  Calcualtes evaluation change from previous ply
 *  Unfortunately this function needs to be complex.
 ***/

void Board::evalChange(int square, int type) {
    int y = square / x_size;
    int x = square - (y * x_size);

    //Used to incremet the in a row count for the placed piece (previously existing in a row + newly placed)
    inARowData bonus_point;
    bonus_point.points++;

    inARowData H_in_row;
    inARowData V_in_row;
    inARowData D_in_row1;
    inARowData D_in_row2;

    // Update eval patterns of side who made move
    H_in_row  = pointsInDir(x, y, 1, 0, type)  + pointsInDir(x, y, -1, 0, type)  + bonus_point;
    V_in_row  = pointsInDir(x, y, 0, 1, type)  + pointsInDir(x, y, 0, -1, type)  + bonus_point;
    D_in_row1 = pointsInDir(x, y, 1, 1, type)  + pointsInDir(x, y, -1, -1, type) + bonus_point;
    D_in_row2 = pointsInDir(x, y, 1, -1, type) + pointsInDir(x, y, -1, 1, type)  + bonus_point;

    // Reset eval params to previous ply
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            eval_pattern[internal_ply + 1][type - 1][i][2 + j]       = eval_pattern[internal_ply][type - 1][i][2 + j];
            eval_pattern[internal_ply + 1][1 - (type - 1)][i][2 + j] = eval_pattern[internal_ply][1 - (type - 1)][i][2 + j];
        }
    }

    eval_pattern[internal_ply + 1][type - 1][H_in_row.openended][H_in_row.points]       = eval_pattern[internal_ply][type - 1][H_in_row.openended][H_in_row.points]       + 1;
    eval_pattern[internal_ply + 1][type - 1][V_in_row.openended][V_in_row.points]       = eval_pattern[internal_ply][type - 1][V_in_row.openended][V_in_row.points]       + 1;
    eval_pattern[internal_ply + 1][type - 1][D_in_row1.openended][D_in_row1.points]     = eval_pattern[internal_ply][type - 1][D_in_row1.openended][D_in_row1.points]     + 1;
    eval_pattern[internal_ply + 1][type - 1][D_in_row2.openended][D_in_row2.points]     = eval_pattern[internal_ply][type - 1][D_in_row2.openended][D_in_row2.points]     + 1;
    eval_pattern[internal_ply + 1][type - 1][H_in_row.openended][H_in_row.points - 1]   = eval_pattern[internal_ply][type - 1][H_in_row.openended][H_in_row.points - 1]   - 1;
    eval_pattern[internal_ply + 1][type - 1][V_in_row.openended][V_in_row.points - 1]   = eval_pattern[internal_ply][type - 1][V_in_row.openended][V_in_row.points - 1]   - 1;
    eval_pattern[internal_ply + 1][type - 1][D_in_row1.openended][D_in_row1.points - 1] = eval_pattern[internal_ply][type - 1][D_in_row1.openended][D_in_row1.points - 1] - 1;
    eval_pattern[internal_ply + 1][type - 1][D_in_row2.openended][D_in_row2.points - 1] = eval_pattern[internal_ply][type - 1][D_in_row2.openended][D_in_row2.points - 1] - 1;

    // Reduce openendedness of potential not side to move eval patterns
    H_in_row  = pointsInDir(x, y, 1, 0, 3 - type);
    eval_pattern[internal_ply + 1][1 - (type - 1)][H_in_row.openended + 1][H_in_row.points] = eval_pattern[internal_ply][1 - (type - 1)][H_in_row.openended + 1][H_in_row.points] - 1;
    eval_pattern[internal_ply + 1][1 - (type - 1)][H_in_row.openended][H_in_row.points] = eval_pattern[internal_ply][1 - (type - 1)][H_in_row.openended][H_in_row.points] + 1;
    H_in_row  = pointsInDir(x, y, -1, 0, 3 - type);
    eval_pattern[internal_ply + 1][1 - (type - 1)][H_in_row.openended + 1][H_in_row.points] = eval_pattern[internal_ply][1 - (type - 1)][H_in_row.openended + 1][H_in_row.points] - 1;
    eval_pattern[internal_ply + 1][1 - (type - 1)][H_in_row.openended][H_in_row.points] = eval_pattern[internal_ply][1 - (type - 1)][H_in_row.openended][H_in_row.points] + 1;
    V_in_row  = pointsInDir(x, y, 0, 1, 3 - type); 
    eval_pattern[internal_ply + 1][1 - (type - 1)][V_in_row.openended + 1][V_in_row.points] = eval_pattern[internal_ply][1 - (type - 1)][V_in_row.openended + 1][V_in_row.points] - 1;
    eval_pattern[internal_ply + 1][1 - (type - 1)][V_in_row.openended][V_in_row.points] = eval_pattern[internal_ply][1 - (type - 1)][V_in_row.openended][V_in_row.points] + 1;
    V_in_row  = pointsInDir(x, y, 0, -1, 3 - type);
    eval_pattern[internal_ply + 1][1 - (type - 1)][V_in_row.openended + 1][V_in_row.points] = eval_pattern[internal_ply][1 - (type - 1)][V_in_row.openended + 1][V_in_row.points] - 1;
    eval_pattern[internal_ply + 1][1 - (type - 1)][V_in_row.openended][V_in_row.points] = eval_pattern[internal_ply][1 - (type - 1)][V_in_row.openended][V_in_row.points] + 1;
    D_in_row1 = pointsInDir(x, y, 1, 1, 3 - type);  
    eval_pattern[internal_ply + 1][1 - (type - 1)][D_in_row1.openended + 1][D_in_row1.points] = eval_pattern[internal_ply][1 - (type - 1)][D_in_row1.openended + 1][D_in_row1.points] - 1;
    eval_pattern[internal_ply + 1][1 - (type - 1)][D_in_row1.openended][D_in_row1.points] = eval_pattern[internal_ply][1 - (type - 1)][D_in_row1.openended][D_in_row1.points] + 1;
    D_in_row1 = pointsInDir(x, y, -1, -1, 3 - type);
    eval_pattern[internal_ply + 1][1 - (type - 1)][D_in_row1.openended + 1][D_in_row1.points] = eval_pattern[internal_ply][1 - (type - 1)][D_in_row1.openended + 1][D_in_row1.points] - 1;
    eval_pattern[internal_ply + 1][1 - (type - 1)][D_in_row1.openended][D_in_row1.points] = eval_pattern[internal_ply][1 - (type - 1)][D_in_row1.openended][D_in_row1.points] + 1;
    D_in_row2 = pointsInDir(x, y, 1, -1, 3 - type);
    eval_pattern[internal_ply + 1][1 - (type - 1)][D_in_row2.openended + 1][D_in_row2.points] = eval_pattern[internal_ply][1 - (type - 1)][D_in_row2.openended + 1][D_in_row2.points] - 1;
    eval_pattern[internal_ply + 1][1 - (type - 1)][D_in_row2.openended][D_in_row2.points] = eval_pattern[internal_ply][1 - (type - 1)][D_in_row2.openended][D_in_row2.points] + 1;
    D_in_row2 = pointsInDir(x, y, -1, 1, 3 - type);
    eval_pattern[internal_ply + 1][1 - (type - 1)][D_in_row2.openended + 1][D_in_row2.points] = eval_pattern[internal_ply][1 - (type - 1)][D_in_row2.openended + 1][D_in_row2.points] - 1;
    eval_pattern[internal_ply + 1][1 - (type - 1)][D_in_row2.openended][D_in_row2.points] = eval_pattern[internal_ply][1 - (type - 1)][D_in_row2.openended][D_in_row2.points] + 1;
}

/***
 *  MakeMove function. Keeps track of active movegen squares etc.
 ***/

void Board::makeMove(int square, int type, TT* tt_pointer) {
    moves[internal_ply]      = MoveData(square, move_gen_squares[square]);
    squares[square]          = type;
    move_gen_squares[square] = 0;
    
    // Update hash key
    hash ^= rand_hash[(type - 1) * MAX_BOARD_SIZE + square];
    if (tt_pointer != nullptr) 
        tt_pointer->prefetch(hash);

    // Activate nearby squares for movegen.
    int y = square / x_size;
    int x = square - (y * x_size);
    
    // Add moves to movegen
    bool W = x > 0;
    bool E = x < x_size - 1;
    bool S = y > 0;
    bool N = y < y_size - 1;

    if (N) addMoveGenSquare(square + d_N, internal_ply);
    if (N && E) addMoveGenSquare(square + d_NE, internal_ply);
    if (E) addMoveGenSquare(square + d_E, internal_ply);
    if (S && E) addMoveGenSquare(square + d_SE, internal_ply);
    if (S) addMoveGenSquare(square + d_S, internal_ply);
    if (S && W) addMoveGenSquare(square + d_SW, internal_ply);
    if (W) addMoveGenSquare(square + d_W, internal_ply);
    if (N && W) addMoveGenSquare(square + d_NW, internal_ply);

    // Update evaluation terms
    evalChange(square, type);

    active_player = 1 - active_player;

    internal_ply++;
};

/***
 *  Reverse makemove
 ***/

void Board::undoMove() {
    internal_ply--;

    int square               = moves[internal_ply].move;
    int type                 = squares[square];
    squares[square]          = 0;
    move_gen_squares[square] = moves[internal_ply].gen_ply;

    // Update hash key
    hash ^= rand_hash[(type - 1) * MAX_BOARD_SIZE + square];

    // Remove moves from movegen
    int y = square / x_size;
    int x = square - (y * x_size);
    
    bool W = x > 0;
    bool E = x < x_size - 1;
    bool S = y > 0;
    bool N = y < y_size - 1;

    if (N) removeMoveGenSquare(square + d_N, internal_ply);
    if (N && E) removeMoveGenSquare(square + d_NE, internal_ply);
    if (E) removeMoveGenSquare(square + d_E, internal_ply);
    if (S && E) removeMoveGenSquare(square + d_SE, internal_ply);
    if (S) removeMoveGenSquare(square + d_S, internal_ply);
    if (S && W) removeMoveGenSquare(square + d_SW, internal_ply);
    if (W) removeMoveGenSquare(square + d_W, internal_ply);
    if (N && W) removeMoveGenSquare(square + d_NW, internal_ply);

    active_player = 1 - active_player;
};

/***
 *  Change active player
 ***/

void Board::setActivePlayer(bool turn) {
    active_player = turn;
}

/***
 *  get active player
 ***/

bool Board::getActivePlayer() {
    return active_player;
}

/***
 *  Set board size
 ***/

void Board::resize(int x, int y) {
    max_active_slots = 0;
    memset(&squares, 0, sizeof(int) * MAX_BOARD_SIZE);
    memset(&move_gen_squares, 0, sizeof(int) * MAX_BOARD_SIZE);
    memset(&move_gen_list, -1, sizeof(int) * MAX_BOARD_SIZE);

    x_size = x;
    y_size = y;

    internal_ply = 1;

    d_N  = x_size;
    d_NE = x_size + 1;
    d_E  = 1;
    d_SE = -x_size + 1;
    d_S  = -x_size;
    d_SW = -x_size - 1;
    d_W  = -1;
    d_NW = x_size -1;
}

/***
 *  Print current board state
 ***/

void Board::print() {
    for (int i = 0; i < y_size; i++) {
        for (int j = 0; j < x_size; j++) {
            switch (squares[i * x_size + j]) {
                case 0:
                    std::cout << "e";
                    break;
                case 1:
                    std::cout << "X";
                    break;
                case 2:
                    std::cout << "0";
            }
        }
        std::cout << std::endl;
    }

    std::cout << "HASH KEY: " << hash << std::endl;
}

/***
 *  print current movegen active squares. Used for debugging purposes
 ***/

void Board::printt() {
    for (int i = 0; i < y_size; i++) {
        for (int j = 0; j < x_size; j++) {
            std::cout << move_gen_squares[i * x_size + j];
        }
        std::cout << std::endl;
    }
    for (int i = 0; i <= max_active_slots; i++) {
        std::cout << move_gen_list[i];
        std::cout << std::endl;
    }
}

/***
 * Position evaluation and mate detection.
 ***/

int Board::evaluate() {
    // Eval always from pov of side to move player. active_player automatically makes sure eval is from correct pov
    
    // Mate detection.
    int eval = 5000 * (eval_pattern[internal_ply][active_player][0][5] + eval_pattern[internal_ply][active_player][1][5] + eval_pattern[internal_ply][active_player][2][5]
                     - eval_pattern[internal_ply][1 - active_player][0][5] - eval_pattern[internal_ply][1 - active_player][1][5] - eval_pattern[internal_ply][1 - active_player][2][5]);

    if (eval)
        return eval;

    // Give openended 4s very high score
    eval += 25 * (10 * eval_pattern[internal_ply][active_player][1][4] +  10 * eval_pattern[internal_ply][active_player][2][4]
                 - 1 * eval_pattern[internal_ply][1 - active_player][2][4]);

    // Give openended 3s bonus score
    eval += 5 * (4 * eval_pattern[internal_ply][active_player][2][3]
               - eval_pattern[internal_ply][1 - active_player][2][3]);

    // Give openended 2s bonus score
    eval += 1 * (eval_pattern[internal_ply][active_player][2][2] - eval_pattern[internal_ply][1 - active_player][2][2]);

    return eval;
};

/***
 * Increase history score of a given move depending on search depth.
 ***/

void Board::addHistory(int16_t move, int16_t depth) {
    int16_t bonus = std::min(depth, (int16_t)BONUS_CAP);
    history[move][active_player] += bonus * bonus - history[move][active_player] * (bonus * bonus) / MAX_HISTORY_SCORE;
};

/***
 * Decrease history score of a given move depending on search depth.
 ***/

void Board::decHistory(int16_t move, int16_t depth) {
    int16_t bonus = std::min(depth, (int16_t)BONUS_CAP);
    history[move][active_player] += -bonus * bonus - history[move][active_player] * (bonus * bonus) / MAX_HISTORY_SCORE;
};

/***
 * Decrease histories of all but last searched move depending on search depth.
 ***/

void Board::decHistories(int16_t depth) {
    MoveList* mv = &search_move_lists[internal_ply];
    for (int i = 0; i < mv->searched - 1; i++) {
        decHistory(mv->searched_moves[i], depth);
    }
}
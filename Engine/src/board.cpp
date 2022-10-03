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

void Board::generate() {
    MoveList* mv = &search_move_lists[internal_ply];
    mv->size     = 0;
    for (int i = 0; i <= max_active_slots; i++) {
        if (move_gen_list[i] != -1) {
            if (move_gen_squares[move_gen_list[i]] >= internal_ply) {
                move_gen_list[i] = -1;
            } else if (move_gen_squares[move_gen_list[i]] != 0) {
                mv->moves[mv->size++] = move_gen_list[i];
            }
        }
    }
}


/***
 *  Get next move in movelist
 ***/

int Board::next() {
    MoveList* mv = &search_move_lists[internal_ply];
    if (mv->size) {
        return mv->moves[--mv->size];
    }
    return -1;
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
 *  Calculates how many in a row we have in specific dir
 ***/

int Board::pointsInDir(int x, int y, int xd, int yd, int type) {
    int points = 0;

    x += xd;
    y += yd;
    while (y >= 0 && y <= y_size - 1 && x >= 0 && x <= x_size) { 
        if (squares[y * x_size + x] == type)
            points++;
        else return points;

        x += xd;
        y += yd;
    }
    return points;
}

/***
 *  Calcualtes evaluation change from previous ply
 ***/

int Board::evalChange(int square, int type) {
    int y = square / x_size;
    int x = square - (y * x_size);

    int H_in_row;
    int V_in_row;
    int D_in_row1;
    int D_in_row2;

    H_in_row  = pointsInDir(x, y, 1, 0, type) + 1 + pointsInDir(x, y, -1, 0, type);
    V_in_row  = pointsInDir(x, y, 0, 1, type) + 1 + pointsInDir(x, y, 0, -1, type);
    D_in_row1 = pointsInDir(x, y, 1, 1, type) + 1 + pointsInDir(x, y, -1, -1, type);
    D_in_row2 = pointsInDir(x, y, 1, -1, type) + 1 + pointsInDir(x, y, -1, 1, type);

    if (std::max(H_in_row, V_in_row) > 4 || std::max(D_in_row1, D_in_row2) > 4)
        return MAX_MATE_SCORE;
    else 
        return 0;
}

/***
 *  MakeMove function. Keeps track of active movegen squares etc.
 ***/

void Board::makeMove(int square, int type) {
    moves[internal_ply]      = MoveData(square, move_gen_squares[square]);
    squares[square]          = type;
    move_gen_squares[square] = 0;
    
    // Update hash key
    hash ^= rand_hash[(type - 1) * MAX_BOARD_SIZE + square];

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

    active_player = 1 - active_player;

    // update evaluation - Note: currently only spots mate
    eval_pattern[internal_ply + 1] = -eval_pattern[internal_ply] - evalChange(square, type);

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
 *  print current movegen active squares.
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
int Board::evaluate() {
    return eval_pattern[internal_ply];
};
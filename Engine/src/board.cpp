#include "board.h"

#include <iostream>
#include <cstring>


void Board::addMoveGenMove(int square) {
    for (int i = 0; i < x_size * y_size; i++) {
        if (move_gen_list[i] == -1) {
            move_gen_list[i] = square;
            max_active_slots = std::max(max_active_slots, i);
            return;
        } 
    }
}

void Board::addMoveGenSquare(int square, int ply) {
    if (!move_gen_squares[square] && !squares[square]) {
        move_gen_squares[square] = ply;
        addMoveGenMove(square);
    }
}

void Board::removeMoveGenSquare(int square, int ply) {
    if (move_gen_squares[square] == ply)
        move_gen_squares[square] = 0;
}

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

int Board::next() {
    MoveList* mv = &search_move_lists[internal_ply];
    if (mv->size) {
        return mv->moves[--mv->size];
    }
    return -1;
}

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

// Make move by square & type
void Board::makeMove(int square, int type) {

    moves[internal_ply]      = MoveData(square, move_gen_squares[square]);
    squares[square]          = type;
    move_gen_squares[square] = 0;
    // activate nearby squares for movegen.

    int y = square / x_size;
    int x = square - (y * x_size);
    
    bool W = x > 0;
    bool E = x < x_size - 1;
    bool S = y > 0;
    bool N = y < y_size - 1;

    // note add moves to movegen list

    if (N) addMoveGenSquare(square + d_N, internal_ply);
    if (N && E) addMoveGenSquare(square + d_NE, internal_ply);
    if (E) addMoveGenSquare(square + d_E, internal_ply);
    if (S && E) addMoveGenSquare(square + d_SE, internal_ply);
    if (S) addMoveGenSquare(square + d_S, internal_ply);
    if (S && W) addMoveGenSquare(square + d_SW, internal_ply);
    if (W) addMoveGenSquare(square + d_W, internal_ply);
    if (N && W) addMoveGenSquare(square + d_NW, internal_ply);

    active_player = 1 - active_player;

    internal_ply++;
};

// Undo move last move
void Board::undoMove() {

    internal_ply--;

    int square = moves[internal_ply].move;
    squares[square] = 0;
    move_gen_squares[square] = moves[internal_ply].gen_ply;

    int y = square / x_size;
    int x = square - (y * x_size);
    
    bool W = x > 0;
    bool E = x < x_size - 1;
    bool S = y > 0;
    bool N = y < y_size - 1;

    // note add moves to movegen list

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

void Board::setActivePlayer(bool turn) {
    active_player = turn;
}

bool Board::getActivePlayer() {
    return active_player;
}

// Set size of the board.
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

// Print's board state to console
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
}

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
    return 0;
};
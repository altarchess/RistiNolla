#include "board.h"

#include <iostream>

void Board::makeMove(int square, char c) {
    switch (c) {
        case 'e':
            makeMove(square, 0);
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
    squares[square] = type;
    // activate nearby squares for movegen.
    int board_ply = moves.size();
    
    // To-Do continue from here.
};

// Undo move last move
void Board::undoMove() {

};

// Set size of the board.
void Board::resize(int x, int y) {
    x_size = x;
    y_size = y;
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
#include <vector>

#define MAX_AXIS_SIZE 40
#define MAX_BOARD_SIZE MAX_AXIS_SIZE * MAX_AXIS_SIZE
#define MAX_MATE_SCORE 5000
#define MIN_MATE_SCORE 4900
#define MAX_INTERNAL_PLY 32

#ifndef TICTACTOE_BOARD_H
#define TICTACTOE_BOARD_H

struct MoveList {
    int moves[MAX_BOARD_SIZE] = {};
    int size;
};

struct MoveData {
    int move;
    int gen_ply;

    MoveData(int m = 0, int p = 0) {
        move    = m;
        gen_ply = p;
    };
};

class Board {
    // Size of the tic tac toe grid;
    int x_size = 0;
    int y_size = 0;

    int internal_ply = 1;

    bool active_player;

    // we allow a maximum board size of 40 * 40
    int squares[MAX_BOARD_SIZE]                             = {};
    // Arrays used in move generation
    int move_gen_squares[MAX_BOARD_SIZE]                    = {};
    int move_gen_list[MAX_BOARD_SIZE]                       = {};
    int max_active_slots                                    = {};
    MoveList search_move_lists[MAX_INTERNAL_PLY]            = {};
    MoveData moves[MAX_BOARD_SIZE];
    int eval_pattern[MAX_INTERNAL_PLY]                      = {};
    
    int d_N, d_NE, d_E, d_SE, d_S, d_SW, d_W, d_NW;

    void addMoveGenMove(int square);
    void addMoveGenSquare(int square, int ply);
    void removeMoveGenSquare(int square, int ply);
    int pointsInDir(int x, int y, int xd, int yd, int type);
    int evalChange(int square, int type);

    public:
    Board() {};
    
    void generate();
    int  next();
    void makeMove(int square, int type);
    void makeMove(int square, char c);
    void undoMove();

    void setActivePlayer(bool turn);
    bool getActivePlayer();

    void resize(int x, int y);

    int evaluate();

    void print();
    void printt();
};

#endif
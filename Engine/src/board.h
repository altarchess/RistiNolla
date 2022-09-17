#include <vector>

class Board {
    // Size of the tic tac toe grid;
    int x_size = 0;
    int y_size = 0;

    // we allow a maximum board size of 25 * 25
    int squares[625]              = {};
    // Arrays used in move generation
    int move_gen_squares[625]     = {};
    int move_gen_squares_ply[625] = {};
    int move_gen_list[625]        = {};

    std::vector<int> moves;
    
    public:
    Board() {};
    
    void makeMove(int square, int type);
    void makeMove(int square, char c);

    void undoMove();

    void resize(int x, int y);

    int evaluate();

    void print();
};
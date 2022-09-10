Programming language for project: C++, likely some python.

Human language for project: English.


# Design document

The RistiNolla program will contain 2 parts. A minimax alpha-beta tic tac toe engine (a tic tac toe playing program), as well as a simple GUI for displaying the game state. The board/grid size should be optional in the GUI, but the engine might have to be limited to less than 21 * 21 (possibly even less), so as to allow for a lookup table for evaluating rows/columns/diagonals. 

# Algorithm design

The tic tac toe engine will use minimax & alpha-beta pruning for searching the game tree, and a simple evaluation function for esimating the winning probabilities of the current board state. Many enhancemennts to alpha-beta, such as a transposition table will be needed to make the tic tac toe engine reasonably strong.

## Communication protocol

* all communication is done via standard input and output

* the engine must always be able to process input from stdin, even while thinking

### GUI to Engine
* position <xsize> <ysize> <position> - The size parameters are used the specify the board size. the position part is just a long string with e for empty, x for x, and 0 for 0, starting from the bottom, left to right.
* godepth <x> - Search to depth x, then return bestmove.
* gotime <x> - Search for x ms, then return bestmove.
* stop - stop the ongoing search, and return bestmove.

### Engine to GUI

* bestmove <x.y> - for example bestmove 0.0 would mean a cross or a zero in the bottom left corner.
* eval <eval> - Estimated winning chances, 100 for an expected win, 50 for a draw, and 0 for a loss. 

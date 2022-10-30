#include "../src/board.h"
#include "../src/util.h"
#include <string>

/*
 * Sets the board state accoring to position command.
 */

void setPos(Board* b, std::string position_command) {
    std::vector<std::string> args = split(position_command);
    int x_size = stoi(args.at(1));
    int y_size = stoi(args.at(2));
    int turn   = stoi(args.at(3));
    std::string pos = args.at(4);
    b->resize(x_size, y_size);
    for (unsigned long long i = 0; i < pos.size(); i++) {
        b->makeMove(i, pos[i]);
    }
    b->setActivePlayer(turn);
}
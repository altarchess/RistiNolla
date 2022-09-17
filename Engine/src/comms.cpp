#include "board.h"
#include "util.h"
#include "comms.h"

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>

Board board;

// main communication loop
void loop(int argc, char *argv[]) {
    std::atexit(quit);
    std::string line;
    
    while (std::getline(std::cin, line)) {
        processCommand(line);
    }
}

void processCommand(std::string str) {
    std::vector<std::string> args = split(str);
    if (args.size() < 1)
        return;
    if (args.at(0) == "quit") {
        quit();
        exit(0);
    } else if (args.at(0) == "position") {
        if (args.size() < 4) {
            INVALID_POSITION
            return;
        }
        int x_size = stoi(args.at(1));
        int y_size = stoi(args.at(2));

        if (std::clamp(x_size, 5, 25) == x_size && std::clamp(y_size, 5, 25) == y_size) {
            std::clamp(x_size, 5, 25);
            std::clamp(y_size, 5, 25);
        } else {
            INVALID_POSITION
            return;
        }

        std::string pos = args.at(3);
        board.resize(x_size, y_size);
        for (int i = 0; i < pos.size(); i++) {
            board.makeMove(i, pos[i]);
        }
    } else if (args.at(0) == "print") {
        board.print();
    }
}

void quit() {

}
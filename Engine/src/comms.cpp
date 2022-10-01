#include "board.h"
#include "util.h"
#include "comms.h"
#include "search.h"

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>

Board board;

SearchData sd;

/***
 *  Main communication loop
 ***/

void loop() {
    std::atexit(quit);
    std::string line;
    
    while (std::getline(std::cin, line)) {
        processCommand(line);
    }
}

/***
 *  Process commands from the GUI/User
 ***/

void processCommand(std::string str) {
    std::vector<std::string> args = split(str);
    if (args.size() < 1)
        return;
    if (args.at(0) == "quit") {
        quit();
        exit(0);
    } else if (args.at(0) == "position") {
        if (args.size() < 5) {
            INVALID_POSITION
            return;
        }
        int x_size = stoi(args.at(1));
        int y_size = stoi(args.at(2));
        int turn   = stoi(args.at(3));

        if (std::clamp(x_size, 5, MAX_AXIS_SIZE) == x_size && std::clamp(y_size, 5, MAX_AXIS_SIZE) == y_size) {
            std::clamp(x_size, 5, MAX_AXIS_SIZE);
            std::clamp(y_size, 5, MAX_AXIS_SIZE);
        } else {
            INVALID_POSITION
            return;
        }

        std::string pos = args.at(4);
        board.resize(x_size, y_size);
        for (int i = 0; i < pos.size(); i++) {
            board.makeMove(i, pos[i]);
        }

        board.setActivePlayer(turn);

    } else if (args.at(0) == "print") {
        board.print();
        //board.printt();
    }
    else if (args.at(0) == "godepth") {
        if (args.size() < 2) {
            INVALID_ARGUMENT
            return;
        }
        sd.max_depth  = std::clamp(stoi(args.at(1)), 1, MAX_INTERNAL_PLY - 1);
        sd.force_quit = false;
        sd.time_limit = 0xFFFFFFFF;
        sd.start_time = 0;

        int best_move = searchRoot(&board, &sd);
        std::cout << "bestmove " << best_move << std::endl; 
    }
}

void quit() {}
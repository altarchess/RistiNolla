#include "board.h"
#include "util.h"
#include "comms.h"
#include "search.h"

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

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
        for (unsigned long long i = 0; i < pos.size(); i++) {
            board.makeMove(i, pos[i]);
        }

        board.setActivePlayer(turn);

        // increment tt age for replacement scheme
        sd.tt.incrementAge();

    } else if (args.at(0) == "print") {
        board.print();
    }
    else if (args.at(0) == "godepth") {
        if (args.size() < 2) {
            INVALID_ARGUMENT
            return;
        }
        sd.max_depth  = std::clamp(stoi(args.at(1)), 1, MAX_INTERNAL_PLY - 1);
        sd.force_quit = false;
        sd.time_limit = 0xFFFFFFFF;

        int best_move = searchRoot(&board, &sd);
        std::cout << "bestmove " << best_move << std::endl; 
    }
    else if (args.at(0) == "gotime") {
        if (args.size() < 2) {
            INVALID_ARGUMENT
            return;
        }
        sd.max_depth  = 100;
        sd.force_quit = false;
        auto time  = std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::steady_clock::now().time_since_epoch()).count(); 
        sd.time_limit = time + std::max(stoi(args.at(1)), 1);

        int best_move = searchRoot(&board, &sd);
        std::cout << "bestmove " << best_move << std::endl; 
    }
}

void quit() {}
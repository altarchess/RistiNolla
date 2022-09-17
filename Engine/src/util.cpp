#include <string>
#include <vector>
#include <sstream>
#include <iostream>

// Splits strings at spaces
std::vector<std::string> split(std::string str) {
    std::vector<std::string> args;
    std::string arg;
    std::stringstream ss(str);
    while (getline(ss, arg, ' ')) {
        args.push_back(arg);
    }
    return args;
};
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include "../src/board.h"

Board b;

TEST_CASE("makemove activation of movegen squares", "[makemove]") {
    b.resize(5,5);
    b.setActivePlayer(0);
    b.makeMove(7, 1);
    b.generate(-1, -1);
    int count = 0;
    while (b.next() != -1)count++; 
    REQUIRE(count == 8);
};
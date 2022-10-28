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
    b.makeMove(8, 1);
    b.generate(-1, -1);
    count = 0;
    while (b.next() != -1)count++; 
    REQUIRE(count == 10);
};

TEST_CASE("makemove updates eval patterns correctly", "[makemove]") {
    b.resize(5,5);
    b.setActivePlayer(0);
    b.makeMove(0, 1);
    b.makeMove(1, 1);
    b.makeMove(2, 1);
    b.makeMove(3, 1); 
    b.makeMove(4, 1); 
    REQUIRE(abs(b.evaluate()) == 5000);
    b.resize(5,5);
    b.setActivePlayer(0);
    b.makeMove(0, 2);
    b.makeMove(1, 2);
    b.makeMove(2, 2);
    b.makeMove(3, 2); 
    b.makeMove(4, 2); 
    REQUIRE(abs(b.evaluate()) == 5000);
};
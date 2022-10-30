#include "catch.hpp"
#include "../src/board.h"

Board board;

/***
 * This test case tests wether or not the makemove function activates the correct amount of squares to be active in movegen.
 ***/

TEST_CASE("makemove activation of movegen squares", "[makemove]") {
    board.resize(5,5);
    board.setActivePlayer(0);
    board.makeMove(7, 1);
    board.generate(-1, -1);
    int count = 0;
    while (board.next() != -1)count++; 
    REQUIRE(count == 8);
    board.makeMove(8, 1);
    board.generate(-1, -1);
    count = 0;
    while (board.next() != -1)count++; 
    REQUIRE(count == 10);
};

/***
 * This test case makes sure evaluation patterns are updated correctly both horizontally & diagonally.
 ***/

TEST_CASE("makemove updates eval patterns correctly", "[makemove]") {
    board.resize(5,5);
    board.setActivePlayer(0);
    board.makeMove(0, 1);
    board.makeMove(1, 1);
    board.makeMove(2, 1);
    board.makeMove(3, 1);
    REQUIRE(abs(board.evaluate()) < 5000); 
    board.makeMove(4, 1); 
    REQUIRE(abs(board.evaluate()) == 5000);
    board.resize(5,5);
    board.setActivePlayer(0);
    board.makeMove(0, 2);
    board.makeMove(6, 2);
    board.makeMove(12, 2);
    board.makeMove(18, 2);
    REQUIRE(abs(board.evaluate()) <= 5000); 
    board.makeMove(24, 2); 
    REQUIRE(abs(board.evaluate()) == 5000);
};

/***
 * Just a simple test making sure side to move is updated properly on makemove
 ***/

TEST_CASE("makemove updates side to move correctly", "[makemove]") {
    board.resize(5,5);
    board.setActivePlayer(0);
    board.makeMove(0, 1);
    REQUIRE(board.getActivePlayer() == 1);
    board.makeMove(0, 2);
    REQUIRE(board.getActivePlayer() == 0);
};

/***
 * Make sure hash key is the same before and after making and unmaking the same moves.
 * Also check that we arrive at the same key when getting to the same position from different move orders
 ***/

TEST_CASE("makemove and unmakemove updates hash key correctly", "[makemove]") {
    initKeys();
    board.resize(5,5);
    board.setActivePlayer(0);
    uint64_t initial_key = board.hash;
    board.makeMove(0, 1);
    board.makeMove(1, 2);
    uint64_t depth2_key  = board.hash;
    board.undoMove();
    board.undoMove();
    REQUIRE(initial_key == board.hash);
    board.makeMove(1, 2);
    board.makeMove(0, 1);
    REQUIRE(depth2_key == board.hash);
    REQUIRE(initial_key != board.hash);
};
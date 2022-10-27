#include "catch.h"
#include "../src/board.h"

SCENARIO("b.makeMove activates correct movegen squares and generate generates the correct squares") {
    GIVEN("An empty board of size 10x10") {
        Board b;
        b.resize(5,5);

        WHEN("When a move is made on square 10") {
            b.makeMove(7, 1);

            THEN("Count of squares to generate moves for equals 8") {
                b.generate(-1, -1);
                int count = 0;
                while (b.next() != -1)count++; 
                REQUIRE(count == 8);
            }
        }
    }
}
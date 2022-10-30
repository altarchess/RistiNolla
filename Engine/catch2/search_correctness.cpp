#include "catch.hpp"
#include "test_util.h"
#include "../src/board.h"
#include "../src/search.h"

#include <string>


Board search_board;
SearchData search_data;

/***
 * Correctness testing of search is a bit of a curios topic, it's only sensible as long as no selective search heuristics are used.
 * This first test tests if the search finds mates at the correct depth in a few different positions. Only testing for mates as otherwise
 * changes to eval would break testing. 5000 is used as an internal mate score in search.
 ***/

TEST_CASE("test correctness of search scores", "[search]") {
    initKeys();
    // Position from /Engine/positions.txt, side to move looses at depth 2.
    setPos(&search_board, "position 5 5 0 e00000000eXX0X0XX0Xeeeeee");
    search_data.time_limit = 0xFFFFFFFF;
    REQUIRE(abs(negaMax(&search_board, &search_data, -5000, 5000, 1, 0)) < 5000); // Should not see the mate at depth 1.
    REQUIRE(abs(negaMax(&search_board, &search_data, -5000, 5000, 2, 0)) == 5000); // Should see mate at depth 2.
    // Position from /Engine/positions.txt, side to move wins at depth 1.
    setPos(&search_board, "position 5 5 1 eeeee0000eXX0X0XX0Xeeeeee");
    REQUIRE(abs(negaMax(&search_board, &search_data, -5000, 5000, 1, 0)) == 5000); // Should see the mate at depth 1.
    // Position from /Engine/positions.txt, should not see mate at all
    setPos(&search_board, "position 7 7 1 eeeeeeeeeeeeeeeeeeeeeeeeXeeeeeeeeeeeeeeeeeeeeeeee");
    REQUIRE(abs(negaMax(&search_board, &search_data, -5000, 5000, 5, 0)) < 5000); // No mate in sight
};

/***
 * This 2nd test case tests for correct moves in positions where there is a mate.
 ***/ 

TEST_CASE("test correctness of bestmoves", "[search]") {
    initKeys();
    // Position from /Engine/positions.txt, but with flippped side to move so its mate in 1.
    setPos(&search_board, "position 5 5 1 e00000000eXX0X0XX0Xeeeeee");
    search_data.time_limit = 0xFFFFFFFF;
    search_data.max_depth  = 2;
    int16_t move           = searchRoot(&search_board, &search_data);
    REQUIRE((move == 0 || move == 9));
    // Position from /Engine/positions.txt, side to move wins at depth 1.
    setPos(&search_board, "position 5 5 1 eeeee0000eXX0X0XX0Xeeeeee");
    search_data.time_limit = 0xFFFFFFFF;
    search_data.max_depth  = 2;
    move                   = searchRoot(&search_board, &search_data);
    REQUIRE(move == 9); 
};


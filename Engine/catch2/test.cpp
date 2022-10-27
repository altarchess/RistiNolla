#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include "../src/board.h"

TEST_CASE("simple test case","[simple]"){
	REQUIRE(1 == 1);
};

TEST_CASE("makemove", "[makemove]") {
    std::cout << "WAFASFASF"<<  std::endl;
    Board b;
    initKeys();
    b.resize(5,5);
    b.makeMove(7, 1);
    b.generate(-1, -1);
    int count = 0;
    while (b.next() != -1)count++; 
    REQUIRE(count == 8);
};
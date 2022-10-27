CC      = g++
SRC     = *.cpp ../src/board.h ../src/board.cpp
EXE     = test-runner

WFLAGS   = -std=c++17 -Wall -Wextra -Wshadow
CFLAGS   = -O3 $(WFLAGS) -DNDEBUG -flto -march=native
COVERAGE = --coverage

release:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE)

coverage:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(COVERAGE)
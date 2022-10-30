CC      = g++
SRC     = *.cpp ../src/board.h ../src/board.cpp ../src/search.h ../src/search.cpp ../src/tt.cpp ../src/util.cpp
EXE     = test-runner

WFLAGS   = -std=c++17 -Wall
CFLAGS   = -O3 $(WFLAGS) -flto
COVERAGE = --coverage

release:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE)

coverage:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(COVERAGE)
CC      = g++
SRC     = *.cpp
EXE     = ../RistiNolla

WFLAGS = -std=c++17 -Wall -Wextra -Wshadow
CFLAGS = -O3 $(WFLAGS) -DNDEBUG -flto -march=native
EFLAGS = -D ENGINE_MAIN

release:
	$(CC) $(CFLAGS) $(EFLAGS) $(SRC) -o $(EXE)
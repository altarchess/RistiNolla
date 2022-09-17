CC      = g++
SRC     = *.cpp
EXE     = ../RistiNolla

WFLAGS = -std=c++17 -Wall -Wextra -Wshadow
CFLAGS = -O3 $(WFLAGS) -DNDEBUG -flto -march=native

release:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE)
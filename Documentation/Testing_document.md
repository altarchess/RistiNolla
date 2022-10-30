## Correctness testing
Unit testing using catch2 is used to test stuff is working as it' suposed to. Only the engine has been tested, testing the GUI was not required, so I decided not to. If this was a more professional project, testing the GUI would be very important as well, as it is used for performance testing, and glitches in the GUI could severly harm progress in game play strength.

### Search
The negaMax function (returns score) is tested by checking if it finds mate in positions that are either mate or not. The searchRoot function (returns best move), is tested by giving it positions where mate can be found within the given depth, and checking if it finds the correct move.

### Board
Correct updating of evaluation params & active move gen squares on makemove etc. are tested simply by giving it some positions, making moves and checking if evaluation / generated movecount matches. There are also simple tests for some simple things like correct updating of side to move etc. The Board.print() and .printt() functions that are used to print out debugging information are excluded from the coverage code coverage report.


### Test coverage measured with gcov:

```bash
File '../src/search.cpp'
Lines executed:93.48% of 46

File '../src/board.cpp'
Lines executed:94.44% of 216

File '../src/tt.cpp'
Lines executed:80.00% of 15
```

### Running the tests
Navigate to /Engine/catch2/

to run the tests, you first need to build with make
```bash
make
```
then, simply run the binary, on windows it would be
```bash
test-runner.exe
```

For code measuring code coverage, build with
```bash
make coverage
```
once again run the binary
```bash
test-runner.exe
```
Now you can check code coverage with
```bash
gcov board.cpp
gcov search.cpp
```

## Self play testing
The only proper way to test if something improves game playing strength (Aka perforamance) is to play a large quantity of games between two versions of the program. Organized self play document [here](https://github.com/altarchess/RistiNolla/blob/main/Documentation/Selfplay.md).

## Other testing
A few people have played against the program using the GUI, so that counts as manual testing I guess :P

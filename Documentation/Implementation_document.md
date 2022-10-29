# Implementation document

## Project structure
The project contains 2 completely separate programs, the engine and the gui. The engine and the gui commincate via standard input & output. 

![Class diagram](https://user-images.githubusercontent.com/57199282/198801451-b54b03c1-6e2d-4622-ba0e-188da1f86ba0.png)

## Complexity
Using, d = depth, b = branching factor

Time complexity of minmax is O(b^d), Alpha-beta pruning, move ordering, and other search improvements only help reduce the branching factor.

Space complexity of minimax is O(d). In practice (when using a transposition table) for the algorithm to work efficiently, space complexity scales with the amount of unique nodes, so in that sense space complexity ends up being more or less the same as time complexity.

## Some interesting implementation details
Originally I had thought of making a lookup table for evaluating rows, columns, and diagonals for efficient positional evaluation. While that seemed to be a good idea at first, it would have limited the board size. In the end, I decided to go for an evaluation that updates itself each time a move is made/unmade. This way, the full evaluation does not have to be recalculated each move, only the difference to the previous move has to be calculated. In theory, this should allow fast evaluation, no matter how large the board is. It was clear from the start considering every legal move in a position just wouldn't be feasible with a branching factor as high as is in M,n,k. Thus I decided to only generate moves for neighbouring squares of already placed pieces. Just as with evaluation, the list of active squares is also updates move by move. As mentioned later, that's something that should be improved upon if development was to continue.

### Improvements over standard minimax
* [NegaMax](https://www.chessprogramming.org/Negamax) is used just for the sake of simplicy, does not make the program stronger. 
* [Alpha-Beta](https://www.chessprogramming.org/Alpha-Beta)
* [Transposition table](https://www.chessprogramming.org/Transposition_Table)
* [Killer moves](https://www.chessprogramming.org/Killer_Move)
* [History](https://www.chessprogramming.org/History_Heuristic)
* [Iterative deepening](https://www.chessprogramming.org/Iterative_Deepening)

## Possible improvements
Simple speedups should definetly be possible, infact I'd be surprised if nps (nodes per second), couldn't be doubled. Even a doubling in search speed wouldn't dramatically increase the strength of the program though. I suspect improvements in search and evaluation could yield much more elo. For many search heuristics used in chess/other games, an accurate evaluation function is required. Because of that, I think developing a good evaluation function would be critical to furhter progress. The simplest sulution I am aware of would be to texel tune handwritten evaluation terms, but even that is clearly outside the scope of this course. There are certinaly many M,n,k game specific ideas that could be tried. In order to make the program truly strong, more that just the neighbours of pieces should be active in movegen. My thinking is that squares that are relevant for the building straights could be activated while updating the evaluation. It might be necessary to look at all neighbours neighbours of pieces, which might actually not be increase the branching factor much if we asume a good line only contains a few of those.


It's very unfortunate the course ended now, I feel the program could have grown way way stronger given a little more time. I wish there was a way to continue working on this, this is something I love doing.

## Sources
[Wikipedia article on M,n,k game](https://en.wikipedia.org/wiki/M,n,k-game)

[Chessprogramming Wiki, especially search related articles](https://www.chessprogramming.org/Search)

Other than that, inspiration & ideas have been taken from discussions with the teacher of the course.

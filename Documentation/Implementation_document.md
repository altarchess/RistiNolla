# Implementation document

## Project structure
The project contains 2 completely separate programs, the engine and the gui. The engine and the gui commincate via standard input & output. 

![Class diagram](https://user-images.githubusercontent.com/57199282/198801451-b54b03c1-6e2d-4622-ba0e-188da1f86ba0.png)


## Complexity
Using, d = depth, b = branching factor

Time complexity of minmax is O(b^d), Alpha-beta pruning, move ordering, and other search improvements only help reduce the branching factor.

Space complexity of minimax is O(d). In practice (when using a transposition table) for the algorithm to work efficiently, space complexity scales with the amount of unique nodes, so in that sense space complexity ends up being more or less the same as time complexity.

## Possible improvements
Simple speedups should definetly be possible, infact I'd be surprised if nps (nodes per second), couldn't be doubled. Even a doubling in search speed wouldn't dramatically increase the strength of the program though. I suspect improvements in search and evaluation could yield much more elo. For many search heuristics used in chess/other games, an accurate evaluation function is required. Because of that, I think developing a good evaluation function would be critical to furhter progress. The simplest sulution I am aware of would be to texel tune handwritten evaluation terms, but even that is clearly outside the scope of this course. There are certinaly many M,n,k game specific ideas that could be tried. In order to make the program truly strong, more that just the neighbours of pieces should be active in movegen. My thinking is that squares that are relevant for the building straights could be activated while updating the evaluation. It might be necessary to look at all neighbours neighbours of pieces, which might actually not be increase the branching factor much if we asume a good line only contains a few of those.

## Sources
[Wikipedia article on M,n,k game](https://en.wikipedia.org/wiki/M,n,k-game)

[Chessprogramming Wiki, especially search related articles](https://www.chessprogramming.org/Search)

Other than that, inspiration & ideas have been taken from discussions with the teacher of the course.

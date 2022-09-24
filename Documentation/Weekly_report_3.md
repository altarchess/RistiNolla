# Week 3
[Hour reporting](https://github.com/altarchess/RistiNolla/blob/main/Documentation/Hour_reporting.md)

## What happened during week 3?
* Finished movegen the way I wanted, relatively fast & only generating moves for relevant squares (close to another X or 0). With a simple negamax alpha-beta search & detecting mates & calculating how many u have in a row, it's doing around 10mnps on my laptop when given an empty board. I think movege speed will drop off a bit when the board gets filled, however I don't know if it will affect overall speed that much.
* Simple NegaMax alpha-beta search.
* Also updating the basic in a row counting for updating of eval terms
* Started looking at testing with doctest for cpp. 
* Started tracking code quality with [CodeFactor.io](https://www.codefactor.io/repository/github/altarchess/ristinolla) 

## Next week?
Testing, Documentation & the GUI are top priorities. Algorithmic improvements should only be made once I can do self play tests. This will also require the creation of a set of starting positions used in testing.

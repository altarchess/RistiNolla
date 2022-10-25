# Self play
## Conditions
Testing conditions for self play games are 15x15 board, 100ms per move, randomized startpos option in gui

## Tests
### RistiNolla 0.3 hashmove2 - RistiNolla 0.2 killermove
`220 - 40` 
patch #3 now uses  hashmove for move ordering also when move is from pv / all_nod

### RistiNolla 0.2 killermove - RistiNolla 0.1 hashmove
`97 - 12` 
patch #2 introduced killer heuristic for move ordering

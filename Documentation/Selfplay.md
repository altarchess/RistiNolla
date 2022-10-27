# Self play
Self play is used to test potential playing strength improvements in the engine.

## Conditions
Testing conditions for self play games are 15x15 board, 100ms per move, "random" startpos option in GUI.

## Tests
Note: Only the tests of comitted improvements since proper randomized testing was implemented in the GUI.
### RistiNolla 0.5 history2 - RistiNolla 0.4 history
`759 - 595`
patch #5 fix history bug

### RistiNolla 0.4 history - Ristinolla 0.3 hashmove2
`1822.5 - 1622.5`
patch #4 improve move ordering + fix unbalanced opening positions in testing

### RistiNolla 0.3 hashmove2 - RistiNolla 0.2 killermove
`220 - 40` 
patch #3 now uses  hashmove for move ordering also when move is from pv / all_nod

### RistiNolla 0.2 killermove - RistiNolla 0.1 hashmove
`97 - 12` 
patch #2 introduced killer heuristic for move ordering

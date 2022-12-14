# Self play
Self play is used to test potential playing strength improvements in the engine.

## Conditions
GUI config used in self play testing (For further instructions on how to do self play testing, see [User guide](https://github.com/altarchess/RistiNolla/blob/main/Documentation/User_guide.md)).
```bash
[Board]
size_x = 15
size_y = 15
grid_size = 40
startpos = RANDOM
rounds = 1000

[AISettings]
max_time = 100

[Players]  
player_x = command1
player_0 = command2
```


## Tests
Note: Only the tests of comitted improvements since proper randomized testing was implemented in the GUI.

### RistiNolla 0.6 matefromtt - RistiNolla 0.5 history2
`481.5 - 320.5`
patch #6 use mate information from tt regardless of depth

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

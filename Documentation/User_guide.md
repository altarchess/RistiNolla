# User Guide
*Note: This is up to date as of Week 4 deadline.

## Tic Tac Toe Engine

### Building

navigate to /Engine/src/ & just simply 

```bash 
make
```

It's rather difficult to use the program from the command line, the use of a GUI is necessary, which brings us to...

## The Simple GUI

### Installing & Running

Install dependencies with:

```bash
poetry install
```

Run with

```bash
poetry run python GUI/gui/main.py
```

To play the tic tac toe engine, you need to specify the path of the engine etc in the config..

### Config
Edit the config in /GUI/gui/config.cfg

Let's take a look at the config
```bash
[Board]
size_x = 20 
size_y = 20
grid_size = 40
```
size_x refers to the horizontal size of the board/grid and size_y refers to the vertical. Grid_size refers to the size of a cell in pixels.

```bash
[AISettings]
max_depth = 4
```
max_depth refers to the maximum depth the engine should search to.

```bash
[Players]  
player_x = HUMAN
player_0 = C:\Users\Example\RistiNolla\Engine\RistiNolla.exe
```
player_x refers to the player playing with the X's, player_0 to the player playing with the 0's. HUMAN just means the human player should input the moves (left click with mouse on the square). If you want the AI to play either colour instead, you should specify the starting command of the AI program. 

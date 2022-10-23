import sys
import subprocess
import pygame  
import config
import board
import time

# Read config file
settings = config.Config()
# Initialize board
main_board = board.Board(settings.size_x, settings.size_y, settings)

# Initialize pygame screen etc
pygame.init()
pygame.display.set_caption('')
scr = pygame.display.set_mode((settings.grid_size * settings.size_x, settings.grid_size * settings.size_y))

# Main game loop
done = False  
while not done:  

    # Get HUMAN/Engine setting for side to move
    opponent_setting = settings.player_x
    if main_board.turn == 1:
        opponent_setting = settings.player_0
                
    # Unless side to move is HUMAN, we need to get the move from the engine specified in config.
    if opponent_setting != "HUMAN":
        sp = subprocess.Popen(opponent_setting, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
        print("position " + str(settings.size_x) + " " + str(settings.size_y) + " " + str(main_board.turn) + " " + main_board.pos_string() + "\n")
        sp.stdin.write(("position " + str(settings.size_x) + " " + str(settings.size_y) + " " + str(main_board.turn) + " " + main_board.pos_string() + "\n").encode())
        sp.stdin.flush()
        print("gotime " + str(settings.engine_time) + "\n")
        sp.stdin.write(("gotime " + str(settings.engine_time) + "\n").encode())
        sp.stdin.flush()
                    
        # Wait till program dies or reports bestmove.
        while (True):
            if sp.poll() != None:
                print("Failure to print bestmove!")
                break
            line = sp.stdout.readline().decode()
            print(line)
            cmd = line.split(" ")
            if cmd[0] == "bestmove":
                main_board.make_move(int(cmd[1]), board.PieceType(main_board.turn + 1))
                sp.kill()
                break

    for event in pygame.event.get():  
        if event.type == pygame.QUIT:  
            done = True
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                grid_x = main_board.get_internal_cord(pygame.mouse.get_pos()[0])
                grid_y = main_board.get_internal_cord(pygame.mouse.get_pos()[1])
                print(main_board.make_move(grid_x + settings.size_x * grid_y, board.PieceType(main_board.turn + 1)))

    # Render board state
    main_board.render(scr)
    pygame.display.flip()

from enum import Enum
from random import randint, random
import pygame
import config

# Enum for piece types (empty, X or 0 in this game)
class PieceType(Enum):
    square_empty = 0
    square_X = 1
    square_0 = 2

# Enum for game state
class GameState(Enum):
    ONGOING = 0
    WIN_X = 1
    WIN_0 = 2
    DRAW = 3

class Board:
    def __init__(self, size_x, size_y, conf: config.Config):
        self.pieces = [PieceType.square_empty] * (size_x * size_y)
        self.config = conf
        self.turn = 0

    # Try top make move for given square. If square is empty, make move & return True, otherwise return False
    def make_move(self, square: int, piece_type: PieceType):

        if (self.config.rounds < 2):
            print("Made move on square " + str(square))

        if self.pieces[square] == PieceType.square_empty:
            self.pieces[square] = piece_type
            self.turn = 1 - self.turn
            return True
        else:
            return False

    def get_internal_cord(self, clicked_cord: int):
        return clicked_cord // self.config.grid_size

    def get_render_cord(self, internal_cord):
        return internal_cord * self.config.grid_size

    def render(self, screen):
        # Draw grid
        for i in range(self.config.size_x + 1):
            pygame.draw.line(screen, (255, 255, 255), (i * self.config.grid_size, 0), (i * self.config.grid_size, self.config.size_y * self.config.grid_size))

        for i in range(self.config.size_y + 1):
            pygame.draw.line(screen, (255, 255, 255), (0, i * self.config.grid_size), (self.config.size_x * self.config.grid_size, i * self.config.grid_size))

        font = pygame.font.SysFont(None, self.config.grid_size // 3 * 2)
        for i in range(self.config.size_x):
            for j in range(self.config.size_y):
                piece = font.render(" ", True, (255, 255, 255))
                if self.pieces[i + j * self.config.size_x] == PieceType.square_X:
                    piece = font.render('X', True, (255, 255, 255))
                elif self.pieces[i + j * self.config.size_x] == PieceType.square_0:
                    piece = font.render('0', True, (255, 255, 255))
                screen.blit(piece, (self.get_render_cord(i) + self.config.grid_size // 4, self.get_render_cord(j) + self.config.grid_size // 4))

    def traverse(self, x, y, xd, yd):
        cscore = 0
        while x >= 0 and x < self.config.size_x and y >= 0 and y < self.config.size_y:
            if self.pieces[x + self.config.size_x * y] == PieceType.square_empty:
                cscore = 0
            if self.pieces[x + self.config.size_x * y] == PieceType.square_X:
                cscore = max(1, cscore + 1)
            if self.pieces[x + self.config.size_x * y] == PieceType.square_0:
                cscore = min(-1, cscore - 1)
            if cscore == 5:
                return GameState.WIN_X
            if cscore == -5:
                return GameState.WIN_0
            x += xd
            y += yd
        return GameState.ONGOING

    def game_state(self):
        # Check for horizontal 5 in a rows
        for i in range(self.config.size_x):
            if self.traverse(i, 0, 0, 1) != GameState.ONGOING:
                return self.traverse(i, 0, 0, 1)
        # Check for vertical 5 in a rows
        for i in range(self.config.size_y):
            if self.traverse(0, i, 1, 0) != GameState.ONGOING:
                return self.traverse(0, i, 1, 0)
            
        # Check diagonals
        for i in range(self.config.size_x):
            if self.traverse(i, 0, 1, 1) != GameState.ONGOING:
                return self.traverse(i, 0, 1, 1)
            if self.traverse(i, 0, -1, 1) != GameState.ONGOING:
                return self.traverse(i, 0, -1, 1)
        for i in range(self.config.size_y):
            if self.traverse(0, i, 1, 1) != GameState.ONGOING:
                return self.traverse(0, i, 1, 1)
            if self.traverse(self.config.size_x - 1, i, -1, 1) != GameState.ONGOING:
                return self.traverse(self.config.size_x - 1, i, -1, 1)

        # Check for possible drawn game
        for i in range(self.config.size_x * self.config.size_y):
            if self.pieces[i] == PieceType.square_empty:
                return GameState.ONGOING

        # If neither side has won and the board is filled declare draw
        return GameState.DRAW

    def new_game(self):
        if (self.config.startpos == "random"):
            self.pieces = [PieceType.square_empty] * (self.config.size_x * self.config.size_y)
            for i in range(self.config.size_x + (self.config.size_x % 2)):
                self.pieces[randint(0, self.config.size_x * self.config.size_y - 1)] = PieceType(1 + (i % 2))
            self.turn = self.config.size_x % 2;
        else:
            self.pieces = [PieceType.square_empty] * (self.config.size_x * self.config.size_y)
            self.turn = 0

    def pos_string(self):
        s = ""
        for i in range(self.config.size_x):
            for j in range(self.config.size_y):
                if self.pieces[j + i * self.config.size_x] == PieceType.square_empty:
                    s += "e"
                elif self.pieces[j + i * self.config.size_x] == PieceType.square_X:
                    s += "X"
                elif self.pieces[j + i * self.config.size_x] == PieceType.square_0:
                    s += "0"
        return s
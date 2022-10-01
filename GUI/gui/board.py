from enum import Enum
import pygame
import config

# Enum for piece types (empty, X or 0 in this game)
class PieceType(Enum):
    square_empty = 0
    square_X = 1
    square_0 = 2

class Board:
    def __init__(self, size_x, size_y, conf: config.Config):
        self.pieces = [PieceType.square_empty] * (size_x * size_y)
        self.config = conf
        self.turn = 0

    # Try top make move for given square. If square is empty, make move & return True, otherwise return False
    def make_move(self, square: int, piece_type: PieceType):

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

    def pos_string(self):
        s = ""
        for i in range(self.config.size_x):
            for j in range(self.config.size_y):
                if self.pieces[j + i * self.config.size_x] == PieceType.square_empty:
                    s += "e";
                elif self.pieces[j + i * self.config.size_x] == PieceType.square_X:
                    s += "X";
                elif self.pieces[j + i * self.config.size_x] == PieceType.square_0:
                    s += "0";
        return s
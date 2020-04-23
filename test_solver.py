import unittest
from solver import *


class TestBoard(unittest.TestCase):

    def test_initialize_board(self):
        rows = 6
        cols = 6
        board = initialize_board(rows, cols)
        self.assertEqual(board.shape, (rows, cols, 4))
        self.assertEqual(board[0][0][0], -1)

    def test_initialize_pieces(self):
        pieces = initialize_pieces()
        self.assertEqual(pieces.shape[0], 4)
        self.assertEqual(tuple(pieces[0]), (1, 0, 0, 3))
        self.assertEqual(tuple(pieces[1]), (17, 0, 0, 1))
        self.assertEqual(tuple(pieces[2]), (15, 0, 0, 17))
        self.assertEqual(tuple(pieces[3]), (3, 0, 0, 15))

    def test_pieces_to_orientations(self):
        pieces = pieces_to_orientations(initialize_pieces())
        self.assertEqual(pieces.shape[0], 4 * 4)

        self.assertEqual(tuple(pieces[0]), (1, 0, 0, 3))
        self.assertEqual(tuple(pieces[1]), (0, 3, 0, 1))
        self.assertEqual(tuple(pieces[15]), (15, 0, 3, 0))

    def test_rotate_piece(self):
        piece = (1, 2, 3, 4)
        self.assertEqual(rotate_piece(piece, 1), (3, 4, 2, 1))
        self.assertEqual(rotate_piece(piece, 3), (4, 3, 1, 2))
        self.assertEqual(rotate_piece(piece, 3)[0], 4)

class TestAction(unittest.TestCase):

    def test_place_piece_on_grid(self):
        grid = initialize_board(6, 6)
        piece = (GRAY, 2, 3, 4)
        success, grid, next_position = place_piece_on_grid(grid, piece, (0, 1))

        self.assertEqual(success, True)
        self.assertEqual(grid[0][1][0], GRAY)
        self.assertEqual(grid[0][1][1], 2)
        self.assertEqual(grid[0][1][2], 3)
        self.assertEqual(grid[0][1][3], 4)
        self.assertEqual(next_position, (0, 2))

    def test_get_next_position(self):
        grid = initialize_board(6, 6)
        piece = (1, 2, 3, 4)
        next_position = get_next_position(grid, (1, 1))
        self.assertEqual(
            get_next_position(grid, (1, 1)),
            (1, 2)
        )

    def test_is_move_legal_borders(self):
        grid = initialize_board(6, 6)
        piece = (GRAY, 2, GRAY, 3)
        self.assertEqual(is_move_legal(grid, piece, (0, 0)), True)

        piece = (2, 2, 2, 3)
        self.assertEqual(is_move_legal(grid, piece, (0, 0)), False)

        piece = (GRAY, 2, 2, 3)
        self.assertEqual(is_move_legal(grid, piece, (0, 0)), False)

        piece = (3, 2, GRAY, 3)
        self.assertEqual(is_move_legal(grid, piece, (0, 0)), False)

        piece = (3, 2, GRAY, 3)
        self.assertEqual(is_move_legal(grid, piece, (1, 0)), True)
        
        piece = (3, GRAY, 3, 3)
        self.assertEqual(is_move_legal(grid, piece, (1, 0)), False)


        piece = (3, GRAY, 3, 3)
        self.assertEqual(is_move_legal(grid, piece, (5, 0)), False)

        piece = (3, GRAY, 3, GRAY)
        self.assertEqual(is_move_legal(grid, piece, (5, 5)), True)

        piece = (3, 2, 3, GRAY)
        self.assertEqual(is_move_legal(grid, piece, (5, 5)), False)

        piece = (3, GRAY, 3, 2)
        self.assertEqual(is_move_legal(grid, piece, (5, 5)), False)

    def test_border_tile_cannot_be_in_center(self):
        grid = initialize_board(6, 6)
        piece = (GRAY, 2, 3, 3)
        self.assertEqual(is_move_legal(grid, piece, (1, 1)), False)
        
        piece = (2, GRAY, 3, 3)
        self.assertEqual(is_move_legal(grid, piece, (1, 1)), False)

        piece = (2, 4, GRAY, 3)
        self.assertEqual(is_move_legal(grid, piece, (1, 1)), False)

        piece = (2, 4, 2, GRAY)
        self.assertEqual(is_move_legal(grid, piece, (1, 1)), False)

    def test_border_tiles_neighbours(self):
        grid = initialize_board(6, 6)
        grid[0][0][NORTH] = GRAY
        grid[0][0][SOUTH] = 2
        grid[0][0][WEST] = GRAY
        grid[0][0][EAST] = 1

        piece = (GRAY, 2, 1, 3)
        self.assertEqual(is_move_legal(grid, piece, (0, 1)), True)

        piece = (GRAY, 2, 2, 3)
        self.assertEqual(is_move_legal(grid, piece, (0, 1)), False)

        piece = (2, 2, GRAY, 3)
        self.assertEqual(is_move_legal(grid, piece, (1, 0)), True)

        piece = (3, 2, GRAY, 3)
        self.assertEqual(is_move_legal(grid, piece, (1, 0)), False)

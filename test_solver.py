import unittest
from solver import *


class Testgrid(unittest.TestCase):

    def test_initialize_grid(self):
        rows = 6
        cols = 6
        grid = initialize_grid(rows, cols)
        self.assertEqual(len(grid), rows)
        self.assertEqual(len(grid[0]), cols)
        self.assertEqual(len(grid[0][0]), 4)
        self.assertEqual(grid[0][0][0], -1)

    def test_initialize_pieces(self):
        pieces, _ = initialize_pieces(puzzles_format=VANSTONE_FORMAT, filename='bvanston_puzzles/Data6_2_6/RandPuzzle06x06_100.txt')
        self.assertEqual(tuple(pieces[0]), (0, 1, 2, 0))
        self.assertEqual(tuple(pieces[1]), (0, 1, 1, 0))
        self.assertEqual(tuple(pieces[2]), (0, 2, 1, 0))
        self.assertEqual(tuple(pieces[3]), (0, 2, 2, 0))

    def test_pieces_to_orientations(self):
        pieces, _ = initialize_pieces(puzzles_format=VANSTONE_FORMAT, filename='bvanston_puzzles/Data6_2_6/RandPuzzle06x06_100.txt')
        pieces =  pieces_to_orientations(pieces)
        self.assertEqual(len(pieces), 6 * 6 * 4)

        self.assertEqual(tuple(pieces[0]), (0, 1, 2, 0))
        self.assertEqual(tuple(pieces[1]), (0, 0, 1, 2))

    def test_rotate_piece(self):
        piece = (1, 2, 3, 4)
        self.assertEqual(rotate_piece(piece, 0), (1, 2, 3, 4))
        self.assertEqual(rotate_piece(piece, 1), (4, 1, 2, 3))
        self.assertEqual(rotate_piece(piece, 2), (3, 4, 1, 2))
        self.assertEqual(rotate_piece(piece, 3), (2, 3, 4, 1))

class TestAction(unittest.TestCase):

    def test_place_piece_on_grid(self):
        grid = initialize_grid(6, 6)
        rows = 6
        cols = 6
        piece = (GRAY, 2, 3, 4)

        success, grid, next_position = place_piece_on_grid(grid, piece, (0, 1))

        self.assertEqual(success, True)
        self.assertEqual(grid[0][1][0], GRAY)
        self.assertEqual(grid[0][1][1], 2)
        self.assertEqual(grid[0][1][2], 3)
        self.assertEqual(grid[0][1][3], 4)
        self.assertEqual(next_position, (0, 2))

    def test_get_next_position(self):
        grid = initialize_grid(6, 6)
        piece = (1, 2, 3, 4)
        next_position = get_next_position(grid, (1, 1))
        self.assertEqual(
            get_next_position(grid, (1, 1)),
            (1, 2)
        )

    def test_get_next_position_circular(self):
        grid = initialize_grid(3, 3)
        piece = (1, 2, 3, 4)
        next_position = get_next_position(grid, (1, 1), is_circular=True)
        self.assertEqual(
            get_next_position(grid, (0, 0), is_circular=True),
            (0, 1)
        )

        self.assertEqual(
            get_next_position(grid, (0, 1), is_circular=True),
            (0, 2)
        )

        self.assertEqual(
            get_next_position(grid, (0, 2), is_circular=True),
            (1, 2)
        )

        self.assertEqual(
            get_next_position(grid, (1, 2), is_circular=True),
            (2, 2)
        )

        self.assertEqual(
            get_next_position(grid, (2, 2), is_circular=True),
            (2, 1)
        )

        self.assertEqual(
            get_next_position(grid, (2, 1), is_circular=True),
            (2, 0)
        )

        self.assertEqual(
            get_next_position(grid, (2, 0), is_circular=True),
            (1, 0)
        )

        self.assertEqual(
            get_next_position(grid, (1, 0), is_circular=True),
            (1, 1)
        )
    def test_get_next_position_circular_4_4(self):
        grid = initialize_grid(4, 4)
        piece = (1, 2, 3, 4)
        next_position = get_next_position(grid, (1, 1), is_circular=True)
        self.assertEqual(
            get_next_position(grid, (1, 0), is_circular=True),
            (1, 1)
        )
        self.assertEqual(
            get_next_position(grid, (1, 1), is_circular=True),
            (1, 2)
        )
        self.assertEqual(
            get_next_position(grid, (1, 2), is_circular=True),
            (2, 1)
        )
        self.assertEqual(
            get_next_position(grid, (2, 1), is_circular=True),
            (2, 2)
        )

    def test_is_move_legal_borders(self):
        grid = initialize_grid(6, 6)
        piece = (GRAY, 3, 2, GRAY)
        rows = 6
        self.assertEqual(is_move_legal(grid, piece, (0, 0)), True)

        piece = (2, 3, 2, 2)
        self.assertEqual(is_move_legal(grid, piece, (0, 0)), False)

        piece = (GRAY, 2, 2, 3)
        self.assertEqual(is_move_legal(grid, piece, (0, 0)), False)

        piece = (3, 3, 2, GRAY)
        self.assertEqual(is_move_legal(grid, piece, (0, 0)), False)

        piece = (3, 3, 2, GRAY)
        self.assertEqual(is_move_legal(grid, piece, (1, 0)), True)
        
        piece = (3, 3, GRAY, 3)
        self.assertEqual(is_move_legal(grid, piece, (1, 0)), False)

        piece = (GRAY, 2, GRAY, GRAY)
        self.assertEqual(is_move_legal(grid, piece, (1, 0)), False)

        piece = (3, 3, GRAY, 3)
        self.assertEqual(is_move_legal(grid, piece, (5, 0)), False)

        piece = (3, GRAY, GRAY, 3)
        self.assertEqual(is_move_legal(grid, piece, (5, 5)), True)

        piece = (3, GRAY, 2, 3)
        self.assertEqual(is_move_legal(grid, piece, (5, 5)), False)

        piece = (3, 2, GRAY, 3)
        self.assertEqual(is_move_legal(grid, piece, (5, 5)), False)

    def test_border_tile_cannot_be_in_center(self):
        grid = initialize_grid(6, 6)
        piece = (GRAY, 3, 2, 3)
        self.assertEqual(is_move_legal(grid, piece, (1, 1)), False)
        
        piece = (2, 3, GRAY, 3)
        self.assertEqual(is_move_legal(grid, piece, (1, 1)), False)

        piece = (2, 3, 4, GRAY)
        self.assertEqual(is_move_legal(grid, piece, (1, 1)), False)

        piece = (2, GRAY, 4, 2)
        self.assertEqual(is_move_legal(grid, piece, (1, 1)), False)

    def test_border_tiles_neighbours(self):
        grid = initialize_grid(6, 6)
        grid[0][0][NORTH] = GRAY
        grid[0][0][SOUTH] = 2
        grid[0][0][WEST] = GRAY
        grid[0][0][EAST] = 1

        piece = (GRAY, 3,  2, 1)
        self.assertEqual(is_move_legal(grid, piece, (0, 1)), True)

        piece = (GRAY, 3, 2, 2)
        self.assertEqual(is_move_legal(grid, piece, (0, 1)), False)

        piece = (2, 3, 2, GRAY)
        self.assertEqual(is_move_legal(grid, piece, (1, 0)), True)

        piece = (3, 3, 2, GRAY)
        self.assertEqual(is_move_legal(grid, piece, (1, 0)), False)

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

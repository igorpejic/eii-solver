import numpy as np


SIDES = 4  # tetravex

EMPTY = -1  # indicating free cell in solution

NORTH = 0
SOUTH = 1
WEST = 2
EAST = 3

piece_type = np.dtype([('NORTH', np.int8), ('SOUTH', np.int8), ('WEST', np.int8), ('EAST', np.int8)])

def initialize_board(rows, cols):
    return np.full((rows, cols, SIDES), EMPTY, dtype=np.int8)

def initialize_pieces():
    with open("4pieces.txt", 'r') as f:
        pieces = np.array([], dtype=piece_type)
        for line in f:
            piece = line.strip().split(" ")
            pieces = np.append(pieces, np.array((piece[NORTH], piece[SOUTH], piece[WEST], piece[EAST]), dtype=piece_type))
    return pieces

def pieces_to_orientations(pieces):
    '''
    given an array of pieces, return an array with all the pieces possible orientations
    '''
    ret_pieces = np.full(pieces.shape[0] * SIDES, EMPTY, dtype=piece_type)
    for i, piece in enumerate(pieces):
        ret_pieces[0 + i * 4][0] = piece[0]
        ret_pieces[0 + i * 4][1] = piece[1]
        ret_pieces[0 + i * 4][2] = piece[2]
        ret_pieces[0 + i * 4][3] = piece[3]

        ret_pieces[1 + i * 4][0] = piece[2]
        ret_pieces[1 + i * 4][1] = piece[3]
        ret_pieces[1 + i * 4][2] = piece[1]
        ret_pieces[1 + i * 4][3] = piece[0]

        ret_pieces[2 + i * 4][0] = piece[1]
        ret_pieces[2 + i * 4][1] = piece[0]
        ret_pieces[2 + i * 4][2] = piece[3]
        ret_pieces[2 + i * 4][3] = piece[2]

        ret_pieces[3 + i * 4][0] = piece[3]
        ret_pieces[3 + i * 4][1] = piece[2]
        ret_pieces[3 + i * 4][2] = piece[0]
        ret_pieces[3 + i * 4][3] = piece[1]
    return ret_pieces

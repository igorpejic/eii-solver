import numpy as np


SIDES = 4  # tetravex

EMPTY = -1  # indicating free cell in solution
GRAY = 0  # indicating border

NORTH = 0
SOUTH = 1
WEST = 2
EAST = 3

piece_type = np.dtype([('NORTH', np.int8), ('SOUTH', np.int8), ('WEST', np.int8), ('EAST', np.int8)])

def initialize_grid(rows, cols):
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

def rotate_piece(piece, orientation):
    '''
    orientation is integer; rotation clockwise
    '''

    if orientation == 0:
        ret_piece = (piece[0], piece[1], piece[2], piece[3])
    elif orientation == 1:
        ret_piece = (piece[2], piece[3], piece[1], piece[0])
    elif orientation == 2:
        ret_piece = (piece[1], piece[0], piece[3], piece[2])
    elif orientation == 3:
        ret_piece = (piece[3], piece[2], piece[0], piece[1])
    return ret_piece

def place_piece_on_grid(grid, piece, position):
    '''
    place position on some position.
    position is determined by strategy.
    '''
    rows, cols = grid.shape[:2]

    grid = np.copy(grid)

    success = is_move_legal(grid, piece, position)
    if not success:
        return False, None, None

    grid[position[0]][position[1]][0] = piece[0]
    grid[position[0]][position[1]][1] = piece[1]
    grid[position[0]][position[1]][2] = piece[2]
    grid[position[0]][position[1]][3] = piece[3]
    next_position = get_next_position(grid, position)
    return success, grid, next_position


def is_move_legal(grid, piece, position):

    rows, cols = grid.shape[:2]

    print(position)
    row, col = position

    if (
        # tiles at border with non-matching borders
        (position[0] == 0 and piece[NORTH] != GRAY) or
        (position[0] == rows - 1 and piece[SOUTH] != GRAY) or
        (position[1] == 0 and piece[WEST] != GRAY) or
        (position[1] == cols - 1 and piece[EAST] != GRAY) or 

        # border tiles in center
        (position[0] != 0 and piece[NORTH] == GRAY) or
        (position[0] != rows -1 and piece[SOUTH] == GRAY) or
        (position[1] != 0 and piece[WEST] == GRAY) or
        (position[1] != cols -1 and piece[EAST] == GRAY)
    ):
        return False
    elif (
        (row > 0 and piece[NORTH] != grid[row - 1][col][SOUTH] and grid[row - 1][col][SOUTH] != EMPTY) or 
        (row < rows -1 and piece[SOUTH] != grid[row + 1][col][NORTH] and grid[row + 1][col][NORTH] != EMPTY) or
        (col > 0 and piece[WEST] != grid[row][col - 1][EAST] and grid[row][col - 1][EAST] != EMPTY) or
        (col < cols - 1 and piece[EAST] != grid[row][col + 1][WEST] and grid[row][col + 1][WEST] != EMPTY)
    ):
        return False
    return True
 


def get_next_position(grid, prev_position):
    rows, cols, _ = grid.shape

    next_position = (
        ((prev_position[0] * cols) + prev_position[1] + 1) // cols, 
        ((prev_position[0] * cols) + prev_position[1] + 1) % cols 
    )
    return next_position

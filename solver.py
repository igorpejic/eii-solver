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

PYTHON_FORMAT = 'python_format' 
JAVA_FORMAT = 'java_format' 
NATLO_FORMAT = 'natlo_format' 

def initialize_pieces(n_pieces=4, puzzles_format=PYTHON_FORMAT, filename=None):
    with open(filename, 'r') as f:
        pieces = np.array([], dtype=piece_type)
        for line in f:
            if puzzles_format == JAVA_FORMAT:
                puzzle_subpiece = [int(x) for x in line.strip().split(' ') if x]
                for i in range(len(puzzle_subpiece) // SIDES):
                    pieces = np.append(pieces, np.array(
                        (puzzle_subpiece[i * SIDES + NORTH],
                         puzzle_subpiece[i * SIDES + 2],
                         puzzle_subpiece[i * SIDES + 3],
                         puzzle_subpiece[i * SIDES + 1]),
                        dtype=piece_type))
            elif puzzles_format == NATLO_FORMAT:
                piece = line.strip().split(", ")
                pieces = np.append(pieces, np.array((piece[WEST], piece[NORTH], piece[EAST], piece[SOUTH]), dtype=piece_type))
            elif puzzles_format == PYTHON_FORMAT:
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

def place_piece_on_grid(grid, piece, position, is_circular=False):
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
    next_position = get_next_position(grid, position, is_circular=is_circular)
    return success, grid, next_position

def get_valid_next_moves(grid, pieces, position):

    '''
    return valid next moves as a tuple indicating (piece index, orientation)
    '''
    possible_moves = []
    for i, piece in enumerate(pieces):
        for orientation in range(SIDES):
            _piece = rotate_piece(piece, orientation)
            if is_move_legal(grid, _piece, position):
                possible_moves.append((i, orientation))
    return possible_moves


def is_move_legal(grid, piece, position):

    rows, cols = grid.shape[:2]

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
 


def get_next_position(grid, prev_position, is_circular=True):
    rows, cols, _ = grid.shape

    if is_circular:
        '''
        first fill the border
        '''
        if prev_position[0] == 0 and prev_position[1] == cols - 1: # right top corner
            next_position = (prev_position[0] + 1, prev_position[1])
        elif prev_position[0] == rows - 1 and prev_position[1] == cols - 1: # right bottom corner
            next_position = (prev_position[0], prev_position[1] - 1)
        elif prev_position[0] == rows - 1 and prev_position[1] == 0: # left bottom corner
            next_position = (prev_position[0] - 1, prev_position[1])
        elif prev_position[0] == 0 and prev_position[1] == 0: # left top corner
            next_position = (prev_position[0], prev_position[1] + 1)
        elif prev_position[0] == 1 and prev_position[1] == 0: # frame has been filled
            next_position = (prev_position[0], prev_position[1] + 1)
        elif prev_position[0] == 0:  # top row
            next_position = (prev_position[0], prev_position[1] + 1)
        elif prev_position[0] == rows - 1:  # bottom row
            next_position = (prev_position[0], prev_position[1] - 1)
        elif prev_position[1] == 0:  # left side
            next_position = (prev_position[0] - 1, prev_position[1])
        elif prev_position[1] == cols - 1:  # right side
            next_position = (prev_position[0] + 1, prev_position[1])
        else:
            # row by row avoiding frame
            if prev_position[0] == rows -1 and prev_position[1] == cols -1:
                next_position = None
            elif prev_position[1] == cols - 2:
                next_position = (prev_position[0] + 1, 1)
            else: # not close to any border, just do the normal
                next_position = (
                    ((prev_position[0] * cols) + prev_position[1] + 1) // cols, 
                    ((prev_position[0] * cols) + prev_position[1] + 1) % cols 
                )
    else:
        # row by row
        next_position = (
            ((prev_position[0] * cols) + prev_position[1] + 1) // cols, 
            ((prev_position[0] * cols) + prev_position[1] + 1) % cols 
        )
    return next_position

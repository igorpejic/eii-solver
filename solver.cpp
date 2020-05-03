#include <cstdlib>
#include <tuple>
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define GRAY 0
#define EMPTY -1 

int *rotate_piece(int *piece, int orientation) {
    int *rotated_piece = new int[4];
    if (orientation == 0) {
        rotated_piece[0] = piece[0];
        rotated_piece[1] = piece[1];
        rotated_piece[2] = piece[2];
        rotated_piece[3] = piece[3];
    } else if (orientation == 1) {
        rotated_piece[0] = piece[3];
        rotated_piece[1] = piece[0];
        rotated_piece[2] = piece[1];
        rotated_piece[3] = piece[2];
    } else if (orientation == 2) {
        rotated_piece[0] = piece[2];
        rotated_piece[1] = piece[3];
        rotated_piece[2] = piece[0];
        rotated_piece[3] = piece[1];
    } else if (orientation == 3) {
        rotated_piece[0] = piece[1];
        rotated_piece[1] = piece[2];
        rotated_piece[2] = piece[3];
        rotated_piece[3] = piece[0];
    }
    return rotated_piece; 
}

int *get_next_position(int cols, int *prev_position) {
    int *next_position = new int[2];
    next_position[0] = ((prev_position[0] * cols) + prev_position[1] + 1) / cols;
    next_position[1] = ((prev_position[0] * cols) + prev_position[1] + 1) % cols;
    return next_position;
}

int *initialize_grid(int rows, int cols) {
    int *grid = (int*)malloc(rows*cols*4*sizeof(int));
    for (int i=0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            for(int k = 0; k < 4; k++) {
                grid[i * cols * 4 + j * 4 + k] = -1;
            }
        }
    }
    return grid;
}

int **initialize_pieces(const char *filename) {
    cout << filename;
    std::ifstream infile(filename);
    int i = 0;
    int **pieces;
    int top, right, down, left;
    int n_pieces;
    int pieces_index = 0;

    for ( std::string line; getline( infile, line ); ) {
        if (i == 0) {
            sscanf(line.c_str(), "%i", &n_pieces);
            n_pieces *= n_pieces;
            pieces = (int**)malloc(n_pieces * sizeof(int*));
            for (int j = 0; j < n_pieces; j++) {
                pieces[j] = (int*)malloc(sizeof(int) * 4);
            }
        } else if (i> 2) {
            sscanf(line.c_str(), "%d %d %d %d", &top, &right, &down, &left);
            pieces[pieces_index][0] = top;
            pieces[pieces_index][1] = right;
            pieces[pieces_index][2] = down;
            pieces[pieces_index][3] = left;
            pieces_index++;
        }
        i++;
    }
    return pieces;
}

bool is_move_legal(int *grid, int *piece, int *position, int rows, int cols) {
    int row = position[0];
    int col = position[1];

    if (
        // tiles at border with non-matching borders
        (position[0] == 0 && piece[NORTH] != GRAY) ||
        (position[0] == rows - 1 && piece[SOUTH] != GRAY) ||
        (position[1] == 0 && piece[WEST] != GRAY) ||
        (position[1] == cols - 1 && piece[EAST] != GRAY) || 

        // border tiles in center
        (position[0] != 0 && piece[NORTH] == GRAY) ||
        (position[0] != rows -1 && piece[SOUTH] == GRAY) ||
        (position[1] != 0 && piece[WEST] == GRAY) ||
        (position[1] != cols -1 && piece[EAST] == GRAY)
    ) {
        return false;
    } else if (
        (row > 0 && piece[NORTH] != grid[(row - 1) * cols * 4 + col * 4 + SOUTH] && grid[(row - 1) * cols * 4 + col * 4 + SOUTH] != EMPTY) || 
        (row < rows -1 && piece[SOUTH] != grid[(row + 1) * cols * 4 + col * 4 + NORTH] && grid[(row + 1) * cols * 4 + col * 4 + NORTH] != EMPTY) ||
        (col > 0 && piece[WEST] != grid[row * cols * 4 + (col - 1) * 4 + EAST] && grid[row * cols * 4 + (col - 1) * 4 + EAST] != EMPTY) ||
        (col < cols - 1 && piece[EAST] != grid[row * cols * 4 + (col + 1) * 4 + WEST] && grid[row * cols * 4 + (col + 1) * 4 + WEST] != EMPTY)
    ) {
        return false;
    }

    return true;
}

std::tuple<bool, int*, int*> place_piece_on_grid(int *grid, int *piece, int *position, int rows, int cols) {
    if (!is_move_legal(grid, piece, position, rows, cols)) {
        return std::make_tuple(false, grid, position);
    }
    int *new_grid = (int*)malloc(sizeof(int) * rows * cols * 4);
    memcpy(new_grid, grid, sizeof(int) * rows * cols * 4);

    new_grid[position[0] * cols * 4 + position[1] * 4 + 0] = piece[0];
    new_grid[position[0] * cols * 4 + position[1] * 4 + 1] = piece[1];
    new_grid[position[0] * cols * 4 + position[1] * 4 + 2] = piece[2];
    new_grid[position[0] * cols * 4 + position[1] * 4 + 3] = piece[3];
    std::cout << new_grid[0];
    int *next_position = get_next_position(cols, position);

    return std::make_tuple(true, new_grid, next_position);
}

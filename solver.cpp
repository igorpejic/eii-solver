#include <cstdlib>
#include <utility>      // std::pair, std::make_pair
#include <tuple>
#include <cstring>
#include <fstream>
#include <vector>
#include <iostream>
#include <array>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define GRAY 0
#define EMPTY -1 
using namespace std;

std::array<int, 4> rotate_piece(std::array<int, 4> piece, int orientation) {
    std::array<int, 4> rotated_piece;
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

std::array<int, 2> get_next_position(int cols, std::array<int, 2> prev_position) {
    std::array <int, 2> next_position = {0, 0};
    next_position[0] = ((prev_position[0] * cols) + prev_position[1] + 1) / cols;
    next_position[1] = ((prev_position[0] * cols) + prev_position[1] + 1) % cols;
    return next_position;
}

std::vector<int> initialize_grid(int rows, int cols) {
    std::vector<int> grid;
    for (int i=0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            for(int k = 0; k < 4; k++) {
                grid.push_back(EMPTY);
            }
        }
    }
    return grid;
}

bool print_pieces(std::vector<std::array<int, 4>> pieces) {
    for (int i = 0; i < pieces.size(); i++) {
        if (i % 3 == 0) {
            std::cout << std::endl;
        }
        for (int j = 0; j < 4; j++) {
            std::cout << pieces[i][j] << " ";
        }
        std::cout << "      ";
    }
    std::cout << std::endl;
}

std::vector<std::array<int, 4>> initialize_pieces(const char *filename) {
    std::cout << filename << std::endl;
    std::ifstream infile(filename);
    int i = 0;
    std::vector<std::array<int, 4>> pieces;
    int top, right, down, left;
    int n_pieces = -1;
    int pieces_index = 0;

    for ( std::string line; getline( infile, line ); ) {
        if (i == 0) {
            sscanf(line.c_str(), "%i", &n_pieces);
            n_pieces *= n_pieces;
        } else if(n_pieces != -1 && i - 2 > n_pieces) {
            break;
        } else if (i> 2) {
            sscanf(line.c_str(), "%d %d %d %d", &top, &right, &down, &left);
            std::array<int, 4> piece = {top, right, down, left};
            pieces.push_back(piece);
            pieces_index++;
        }
        i++;
    }
    std::cout << "Pieces length: " << pieces.size() << std::endl;
    print_pieces(pieces);
    return pieces;
}



bool is_move_legal(std::vector<int> grid, std::array<int, 4> piece, std::array<int, 2> position, int rows, int cols) {
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

std::tuple<bool, std::vector<int>, std::array<int, 2>> place_piece_on_grid(std::vector<int> grid, std::array<int, 4> piece, std::array<int, 2> position, int rows, int cols) {
    if (!is_move_legal(grid, piece, position, rows, cols)) {
        return std::make_tuple(false, grid, position);
    }
    vector<int> new_grid;
    new_grid = grid;

    new_grid[position[0] * cols * 4 + position[1] * 4 + 0] = piece[0];
    new_grid[position[0] * cols * 4 + position[1] * 4 + 1] = piece[1];
    new_grid[position[0] * cols * 4 + position[1] * 4 + 2] = piece[2];
    new_grid[position[0] * cols * 4 + position[1] * 4 + 3] = piece[3];
    std::array<int, 2> next_position = get_next_position(cols, position);

    return std::make_tuple(true, new_grid, next_position);
}

std::vector<std::pair<int, int>> get_valid_next_moves(std::vector<int> grid, std::vector<std::array<int, 4>> pieces, std::array<int, 2> position, int rows, int cols) {
    std::vector<std::pair<int, int>> possible_moves;
    for (int i = 0; i < pieces.size(); i++) {
        for (int orientation = 0; orientation < 4; orientation++) {
            if(is_move_legal(grid, rotate_piece(pieces[i], orientation), position, rows, cols)) {
                possible_moves.push_back(std::make_pair(i, orientation));
            }
        }
    }
    return possible_moves;
}

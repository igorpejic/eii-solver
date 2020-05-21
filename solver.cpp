#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <utility>      // std::pair, std::make_pair
#include <tuple>
#include <cstring>
#include <fstream>
#include <vector>
#include <iostream>
#include <array>
#include "solver.hpp"

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

Piece rotate_piece_b(Piece piece, int orientation) {
    Piece rotated_piece;
    if (orientation == 0) {
        rotated_piece.top = piece.top;
        rotated_piece.right = piece.right;
        rotated_piece.bottom = piece.bottom;
        rotated_piece.left = piece.left;
    } else if (orientation == 1) {
        rotated_piece.top = piece.left;
        rotated_piece.right = piece.top;
        rotated_piece.bottom = piece.right;
        rotated_piece.left = piece.bottom;
    } else if (orientation == 2) {
        rotated_piece.top = piece.bottom;
        rotated_piece.right = piece.left;
        rotated_piece.bottom = piece.top;
        rotated_piece.left = piece.right;
    } else if (orientation == 3) {
        rotated_piece.top = piece.right;
        rotated_piece.right = piece.bottom;
        rotated_piece.bottom = piece.left;
        rotated_piece.left = piece.top;
    }
    return rotated_piece; 
}

std::array<int, 2> get_next_position(int cols, std::array<int, 2> prev_position) {
    std::array <int, 2> next_position = {0, 0};
    next_position[0] = ((prev_position[0] * cols) + prev_position[1] + 1) / cols;
    next_position[1] = ((prev_position[0] * cols) + prev_position[1] + 1) % cols;
    return next_position;
}

Position get_next_position_b(int cols, Position prev_position) {
    Position next_position = {0, 0};
    next_position.i = ((prev_position.i * cols) + prev_position.j + 1) / cols;
    next_position.j = ((prev_position.i * cols) + prev_position.j + 1) % cols;
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

void print_pieces_b(pieces pieces) {
    int rows = sqrt(pieces.size());
    for (int i = 0; i < pieces.size(); i++) {
        if (i % rows == 0) {
            std::cout << std::endl;
        }
        std::cout << pieces[i].top << " ";
        std::cout << pieces[i].right << " ";
        std::cout << pieces[i].bottom << " ";
        std::cout << pieces[i].left << " ";
        std::cout << "      ";
    }
    std::cout << std::endl;
}

void print_pieces_louis_format(board &board, pieces &pieces) {
    int rows = sqrt(pieces.size());
    for (int i = 0; i < board.size(); i++) {
        if (i % rows == 0 && i !=0) {
            std::cout << std::endl;
        }
        Piece board_piece = board[i];
        bool match_found = false;
        for (int j = 0; j < pieces.size(); j++) {
            if (match_found) {
                break;
            }
            for (int orientation = 0; orientation < 4; orientation++) {
                Piece rotated_piece = rotate_piece_b(pieces[j], orientation);
                if (rotated_piece == board_piece) {
                    match_found = true;
                    std::cout << std::setw(5) << j << "/" << orientation;
                    //std::cout << "     ";
                }
            }
        }
    }
    std::cout << std::endl;
}

std::vector<std::array<int, 4>> initialize_pieces(const char *filename) {
    std::cout << filename << std::endl;
    std::ifstream infile(filename);
    int i = 0;
    std::vector<std::array<int, 4>> pieces;
    int top, right, bottom, left;
    int n_pieces = -1;
    int pieces_index = 0;

    for ( std::string line; getline( infile, line ); ) {
        if (i == 0) {
            sscanf(line.c_str(), "%i", &n_pieces);
            n_pieces *= n_pieces;
        } else if(n_pieces != -1 && i - 2 > n_pieces) {
            break;
        } else if (i> 2) {
            sscanf(line.c_str(), "%d %d %d %d", &top, &right, &bottom, &left);
            std::array<int, 4> piece = {top, right, bottom, left};
            pieces.push_back(piece);
            pieces_index++;
        }
        i++;
    }
    std::cout << "Pieces length: " << pieces.size() << std::endl;
    print_pieces(pieces);
    return pieces;
}


pieces initialize_pieces_backtracker(const char *filename) {
    std::cout << filename << std::endl;
    std::ifstream infile(filename);
    int i = 0;
    pieces pieces;
    int top, right, bottom, left;
    int n_pieces = -1;
    int pieces_index = 0;

    for ( std::string line; getline( infile, line ); ) {
        if (i == 0) {
            sscanf(line.c_str(), "%i", &n_pieces);
            n_pieces *= n_pieces;
        } else if(n_pieces != -1 && i - 2 > n_pieces) {
            break;
        } else if (i> 2) {
            sscanf(line.c_str(), "%d %d %d %d", &top, &right, &bottom, &left);
            Piece piece = {top, right, bottom, left};
            pieces.push_back(piece);
            pieces_index++;
        }
        i++;
    }
    std::cout << "Pieces length: " << pieces.size() << std::endl;
    print_pieces_b(pieces);
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

bool is_move_legal_b(board &board, Piece piece, Position &position, int rows, int cols) {
    int row = position.i;
    int col = position.j;
    //cout << piece.top << piece.right << piece.bottom << piece.left << std::endl;
    //cout << row << col << std::endl;
    //cout << board[(row +1) * cols + col].top << std::endl;

    if (
        // tiles at border with non-matching borders
        (position.i == 0 && piece.top != GRAY) ||
        (position.i == rows - 1 && piece.bottom != GRAY) ||
        (position.j == 0 && piece.left != GRAY) ||
        (position.j == cols - 1 && piece.right != GRAY) || 

        // border tiles in center
        (position.i != 0 && piece.top == GRAY) ||
        (position.i != rows -1 && piece.bottom == GRAY) ||
        (position.j != 0 && piece.left == GRAY) ||
        (position.j != cols -1 && piece.right == GRAY)
    ) {
        return false;
    } else if (
        (row > 0 && piece.top != board[(row - 1) * cols + col].bottom && board[(row - 1) * cols + col].bottom != EMPTY) || 
        (row < rows -1 && piece.bottom != board[(row + 1) * cols + col].top && board[(row + 1) * cols + col].top != EMPTY) ||
        (col > 0 && piece.left != board[row * cols + (col - 1)].right && board[row * cols + (col - 1)].right != EMPTY) ||
        (col < cols - 1 && piece.right != board[row * cols + (col + 1)].left && board[row * cols + (col + 1)].left != EMPTY)
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

std::vector<PiecePlacement> get_valid_next_moves_b(board &board, placed_pieces &placed_pieces, pieces &pieces, Position &position, Piece** rotated_pieces,  int rows, int cols) {
    std::vector<PiecePlacement> possible_moves;
    for (int i = 0; i < pieces.size(); i++) {
        for (int orientation = 0; orientation < 4; orientation++) {
            if(!placed_pieces[i] && is_move_legal_b(board, rotated_pieces[i][orientation], position, rows, cols)) {
                PiecePlacement placement;
                placement.index = i;
                placement.rotation = orientation;
                possible_moves.push_back(placement);
            }
        }
    }
    return possible_moves;
}


board initialize_board_b(int rows, int cols) {
    board board;
    for (int i=0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            Piece piece;
            piece.top = EMPTY;
            piece.right = EMPTY;
            piece.bottom = EMPTY;
            piece.left = EMPTY;
            board.push_back(piece);
        }
    }
    return board;
}

void print_board_b(board board, int rows, int cols) {
    for (int line = 0; line < rows; line++) {
        int matrix_row = line;
        for (int col = 0; col < cols; col++) {
            std::cout<< "\\" << std::setw(2) << board[matrix_row * cols  + col ].top << " /" << "|";
        }
        std::cout<< std::endl;

        for (int col = 0; col < cols; col++) {
            std::cout<< std::setw(2) << board[matrix_row * cols  + col].left  << "X" << std::setw(2) << board[matrix_row * cols  + col].right << "|";
        }
        std::cout<< std::endl;

        for (int col = 0; col < cols; col++) {
            std::cout<< "/" << std::setw(2) << board[matrix_row * cols  + col].bottom << " \\" << "|";
        }
        std::cout<< std::endl;
        for (int col = 0; col < cols; col++) {
            std::cout<< "------";
        }
        std::cout<< std::endl;
    }
    std::cout<< "\n" << std::endl;
}


bool operator==(Piece& one, Piece& other) {
    return (
            one.top == other.top &&
            one.right == other.right &&
            one.bottom == other.bottom &&
            one.left == other.left) ;
}

Piece **get_rotated_pieces_b(pieces pieces) {
    int rows = pieces.size();
    Piece **rotated_pieces = (Piece**)malloc(sizeof(Piece*) * rows +  sizeof(Piece) * rows * 4);

    Piece *ptr;

    ptr = (Piece*)(rotated_pieces + rows);

    for(int i = 0; i < rows; i++) {
        rotated_pieces[i] = (ptr + 4 * i);
    }

    for (int i = 0; i < pieces.size(); i++) {
        for (int j = 0; j < 4; j++) {
            rotated_pieces[i][j] = rotate_piece_b(pieces[i], j);
        }
    }
    return rotated_pieces;
}

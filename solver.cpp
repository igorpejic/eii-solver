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
#include <cinttypes>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define GRAY 0
#define EMPTY 5

#define BOTTOM 2
#define RIGHT 1

#define NEIGHBOUR_TOP 1
#define NEIGHBOUR_LEFT 3


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

void print_pieces(std::vector<std::array<int, 4>> pieces) {
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

void print_board_editor_b(board board, Piece ** rotated_pieces) {
    int rows = sqrt(board.size());
    for (int i = 0; i < board.size(); i++) {
        if (i % rows == 0) {
            std::cout << std::endl;
        }
        Piece piece;
        if (board[i].orientation == EMPTY) {
            piece.top = 0; 
            piece.right = 0; 
            piece.bottom = 0; 
            piece.left = 0; 
        } else{
            piece = rotated_pieces[board[i].index][board[i].orientation];
        }
        std::cout << (unsigned)piece.top << " ";
        std::cout << (unsigned)piece.right << " ";
        std::cout << (unsigned)piece.bottom << " ";
        std::cout << (unsigned)piece.left << " ";
        std::cout << "      ";
    }
    std::cout << std::endl;
}

void print_board_louis_format(board &board) {
    int rows = sqrt(board.size());
    for (int i = 0; i < board.size(); i++) {
        if (i % rows == 0 && i !=0) {
            std::cout << std::endl;
        }
        std::cout << std::setw(5) << board[i].index << "/" << +board[i].orientation;
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
    uint_fast8_t top, right, bottom, left;
    int n_pieces = -1;

    if(strcmp(filename, "256pieces.txt") == 0) {
        std::cout << "initializing E-II" << std::endl;
        for ( std::string line; getline( infile, line ); ) {
            n_pieces = 256;
            sscanf(line.c_str(), "%" SCNu8 " %" SCNu8 " %" SCNu8 " %" SCNu8, &top, &bottom, &left, &right);
            Piece piece = {top, right, bottom, left};
            pieces.push_back(piece);
        }
    } else {
        for ( std::string line; getline( infile, line ); ) {
            if (i == 0) {
                sscanf(line.c_str(), "%i", &n_pieces);
                n_pieces *= n_pieces;
            } else if(n_pieces != -1 && i - 2 > n_pieces) {
                break;
            } else if (i> 2) {
                sscanf(line.c_str(), "%" SCNu8 " %" SCNu8 " %" SCNu8 " %" SCNu8, &top, &right, &bottom, &left);
                Piece piece = {top, right, bottom, left};
                pieces.push_back(piece);
            }
            i++;
        }
    }
    std::cout << "Pieces length: " << pieces.size() << std::endl;
    //print_pieces_b(pieces);
    return pieces;
}

bool operator==(const PiecePlacement& one, const PiecePlacement& other) {
    return (one.index == other.index && one.orientation == other.orientation);
}

bool operator==(const Position& one, const Position& other) {
    return (one.i == other.i && one.j == other.j);
}

// use when pieceplacement in neighbours_map is unordered_set
//int pieceHasher(const PiecePlacement &p){
//    return ((uint64_t)p.index)<<32 | (uint64_t)p.orientation;
//}

bool operator==(Piece& one, Piece& other) {
    return (
            one.top == other.top &&
            one.right == other.right &&
            one.bottom == other.bottom &&
            one.left == other.left) ;
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
    std::vector<int> new_grid;
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

std::vector<PiecePlacement> get_valid_next_moves_b(board &board, placed_pieces &placed_pieces, neighbours_map_t &neighbours_map, Position &position, Piece** rotated_pieces) {
    std::vector<PiecePlacement> possible_moves;

    //reserve memory; some speedup might be observed
    possible_moves.reserve(50);

    int row = position.i;
    int col = position.j;

    if (position.i == 0 && position.j == 0) {
        for (int i = 0; i < PUZZLE_SIZE * PUZZLE_SIZE; i++) {
            for (int orientation = 0; orientation < 4; orientation++) {
                Piece piece = rotated_pieces[i][orientation];
                //std::cout << "piece:" << piece.top << piece.right << piece.bottom << piece.left << std::endl;
                if(!placed_pieces[i] && piece.top == GRAY && piece.left == GRAY) {
                    PiecePlacement placement;
                    placement.index = i;
                    placement.orientation = orientation;
                    possible_moves.push_back(placement);
                }
            }
        }
    } else if(position.i == 0) {
        PiecePlacement piece_on_left = board[row * PUZZLE_SIZE + (col - 1)];
        std::vector<PiecePlacement> &possible_piece_placements_left = neighbours_map[get_piece_hash(
                piece_on_left.index, piece_on_left.orientation, RIGHT)];
        for (const auto& elem: possible_piece_placements_left) {
            if (placed_pieces[elem.index] || rotated_pieces[elem.index][elem.orientation].top != GRAY) {
                continue;
            }
            possible_moves.push_back(elem);
        }
    } else if(position.j == 0) {
        PiecePlacement piece_on_top = board[(row - 1) * PUZZLE_SIZE + col];
        std::vector<PiecePlacement> &possible_piece_placements_top = neighbours_map[get_piece_hash(
                piece_on_top.index, piece_on_top.orientation, BOTTOM)];
        for (const auto& elem: possible_piece_placements_top) {
            if (placed_pieces[elem.index] || rotated_pieces[elem.index][elem.orientation].left != GRAY) {
                continue;
            }
            possible_moves.push_back(elem);
        }
    } else {
        PiecePlacement piece_on_top = board[(row - 1) * PUZZLE_SIZE + col];
        PiecePlacement piece_on_left = board[row * PUZZLE_SIZE + (col - 1)];

        std::vector<PiecePlacement> *possible_piece_placements;

        uint_fast8_t neighbour_with_less_choices;

        if (neighbours_map[get_piece_hash(piece_on_top.index, piece_on_top.orientation, BOTTOM)].size() < neighbours_map[get_piece_hash(piece_on_left.index, piece_on_left.orientation, RIGHT)].size()) {
            possible_piece_placements = &neighbours_map[get_piece_hash(piece_on_top.index, piece_on_top.orientation, BOTTOM)];
            neighbour_with_less_choices =  NEIGHBOUR_TOP;
        } else {
            possible_piece_placements = &neighbours_map[get_piece_hash(piece_on_left.index, piece_on_left.orientation, RIGHT)];
            neighbour_with_less_choices =  NEIGHBOUR_LEFT;
        }

        //std::cout << neighbours_map[get_piece_hash(piece_on_left.index, piece_on_left.orientation, RIGHT)].size() << "_";
        //std::cout << neighbours_map[get_piece_hash(piece_on_top.index, piece_on_top.orientation, BOTTOM)].size() << "rr ";
        //std::cout << possible_piece_placementssize() << "dddddddd" << std::endl;
        //std::cout << row << ":" << col << std::endl;
        //std::cout << possible_piece_placements_top.size() << std::endl;

        //iterate over smaller set for performance
        for (const auto& elem: *possible_piece_placements) {
            if(placed_pieces[elem.index]) {
                continue;
            }

            if (position.i == (PUZZLE_SIZE - 1)  && rotated_pieces[elem.index][elem.orientation].bottom != GRAY) {
                continue;
            }
            if (position.j == (PUZZLE_SIZE - 1)  && rotated_pieces[elem.index][elem.orientation].right != GRAY) {
                continue;
            }
            if (neighbour_with_less_choices == NEIGHBOUR_TOP) {
                if(rotated_pieces[elem.index][elem.orientation].left == rotated_pieces[piece_on_left.index][piece_on_left.orientation].right) {
                    possible_moves.push_back(elem);
                }
            } else if (neighbour_with_less_choices == NEIGHBOUR_LEFT) {
                if(rotated_pieces[elem.index][elem.orientation].top == rotated_pieces[piece_on_top.index][piece_on_top.orientation].bottom) {
                    possible_moves.push_back(elem);
                }
            }
        }
        //std::cout << "POSSIBLE MOVES" << possible_moves.size() << std::endl;
    }
    //std::cout << possible_moves.size() << std::endl;

    return possible_moves;
}


board initialize_board_b(int rows, int cols) {
    board board;
    for (int i=0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            PiecePlacement piece;
            piece.index = EMPTY;
            piece.orientation = EMPTY;
            board.push_back(piece);
            //std::cout << (unsigned)piece.orientation << std::endl;
        }
    }
    return board;
}

void print_board_b(board board, Piece** rotated_pieces, int rows, int cols) {
    for (int line = 0; line < rows; line++) {
        int matrix_row = line;

        for (int col = 0; col < cols; col++) {
            int top = -1;
            if (board[matrix_row * cols + col].orientation != EMPTY) {
                Piece piece = rotated_pieces[board[matrix_row * cols + col].index][board[matrix_row * cols + col].orientation];
                top = piece.top;
            }
            std::cout<< "\\" << std::setw(2) << top << " /" << "|";
        }
        std::cout<< std::endl;

        for (int col = 0; col < cols; col++) {
            int right = -1;
            int left = -1;
            if (board[matrix_row * cols + col].orientation != EMPTY) {
                Piece piece = rotated_pieces[board[matrix_row * cols + col].index][board[matrix_row * cols + col].orientation];
                right = piece.right;
                left = piece.left;
            }
            std::cout<< std::setw(2) << left  << "X" << std::setw(2) << right << "|";
        }
        std::cout<< std::endl;

        for (int col = 0; col < cols; col++) {
            int bottom = -1;
            if (board[matrix_row * cols + col].orientation != EMPTY) {
                Piece piece = rotated_pieces[board[matrix_row * cols + col].index][board[matrix_row * cols + col].orientation];
                bottom = piece.bottom;
            }
            std::cout<< "/" << std::setw(2) << bottom << " \\" << "|";
        }
        std::cout<< std::endl;
        for (int col = 0; col < cols; col++) {
            std::cout<< "------";
        }
        std::cout<< std::endl;
    }
    std::cout<< "\n" << std::endl;
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

neighbours_map_t get_possible_neighbours_map(pieces pieces, Piece** rotated_pieces) {
    /* key is (piece_index, orientation, side), all integers
     * value is vector with: PiecePlacement (piece_index, orientation)
     * */

    neighbours_map_t possible_neighbours_map;

    //std::cout << "Pieces size" << pieces.size() << std::endl;
    for (int i = 0; i < pieces.size(); i++) {
        for (int orientation = 0; orientation < 4; orientation++) {
            Piece piece = rotated_pieces[i][orientation];
            //std:: cout << "--------------------" << std::endl;
            //std:: cout << i << ":" << orientation << " " << piece.top << piece.right << piece.bottom << piece.left << std::endl;

            int piece_hash_bottom = get_piece_hash(i, orientation, BOTTOM);
            int piece_hash_right = get_piece_hash(i, orientation, RIGHT);

            std::vector<PiecePlacement> possible_right_neighbours;
            std::vector<PiecePlacement> possible_bottom_neighbours;

            /* find all the pieces which can be placed next to this piece */
            for (int k = 0; k < pieces.size(); k++) {
                if (i == k) {
                    /* skip itself */
                    continue;
                }
                for (int neighbour_orientation=0; neighbour_orientation < 4; neighbour_orientation++) {
                    Piece neighbour_piece = rotated_pieces[k][neighbour_orientation];
                    PiecePlacement neighbour;
                    neighbour.index = k;
                    neighbour.orientation = neighbour_orientation;
                    if (neighbour_piece.top == piece.bottom && piece.bottom != GRAY) {
                        possible_bottom_neighbours.push_back(neighbour);
                    }
                    if (neighbour_piece.left == piece.right && piece.right != GRAY) {
                        possible_right_neighbours.push_back(neighbour);
                    }
                }
            }

            // bottom
            possible_neighbours_map[piece_hash_bottom] = possible_bottom_neighbours;
            // right
            possible_neighbours_map[piece_hash_right] = possible_right_neighbours;
            //std::cout << piece_hash_bottom << " " << possible_neighbours_map[piece_hash_bottom].size() << std::endl;
            //for (const auto& elem: possible_neighbours_map[piece_hash_right]) {
            //    Piece piece = rotated_pieces[elem.index][elem.orientation];
            //    std::cout << piece.top << piece.right << piece.bottom << piece.left << std::endl;
            //}

        }
        //std::cout << std::endl;
    }
    return possible_neighbours_map;
}

int get_piece_hash(int piece_index, uint_fast8_t orientation, uint_fast8_t side) {
    return piece_index * 4 * 4 + orientation * 4 + side;
}

void print_final_solution(board board, Piece **rotated_pieces) {
    print_board_b(board, rotated_pieces, PUZZLE_SIZE, PUZZLE_SIZE);
    print_board_editor_b(board, rotated_pieces);
    print_board_louis_format(board);
}

uint_fast8_t get_num_correct_edges(board board, Position position, Piece **rotated_pieces) {
    uint_fast8_t num_correct_edges = 0;
    Piece *current_piece = &rotated_pieces[board[position.i * PUZZLE_SIZE + position.j].index][board[position.i * PUZZLE_SIZE + position.j].orientation];
    if (position.i != 0) {
        if (rotated_pieces[board[(position.i - 1) * PUZZLE_SIZE + position.j].index][board[(position.i - 1) * PUZZLE_SIZE + position.j].orientation].bottom == current_piece->top) {
            num_correct_edges += 1;
        }
    }
    if (position.j != 0) {
        if (rotated_pieces[board[(position.i) * PUZZLE_SIZE + position.j - 1].index][board[(position.i) * PUZZLE_SIZE + position.j - 1].orientation].right == current_piece->left) {
            num_correct_edges += 1;
        }
    }
    if (position.j != PUZZLE_SIZE - 1) {
        if (rotated_pieces[board[(position.i) * PUZZLE_SIZE + position.j + 1].index][board[(position.i) * PUZZLE_SIZE + position.j + 1].orientation].left == current_piece->right) {
            num_correct_edges += 1;
        }
    }
    if (position.i != PUZZLE_SIZE - 1) {
        if (rotated_pieces[board[(position.i + 1) * PUZZLE_SIZE + position.j].index][board[(position.i + 1) * PUZZLE_SIZE + position.j].orientation].top == current_piece->bottom) {
            num_correct_edges += 1;
        }
    }
    return num_correct_edges;
}

int swap_tiles(board &board, Position position_1, Position position_2, int n_correct_edges, Piece ** rotated_pieces) {
    /* return updated count of correct edges */

    int new_n_correct_edges = n_correct_edges;

    uint_fast8_t n_lost_correct_edges  = 0;

    //TODO: maybe can be optimized by storing this value somewhere
    n_lost_correct_edges += get_num_correct_edges(board, position_1, rotated_pieces);
    n_lost_correct_edges += get_num_correct_edges(board, position_2, rotated_pieces);

    std::swap(board[position_1.i * PUZZLE_SIZE + position_1.j], board[position_2.i * PUZZLE_SIZE + position_2.j]);

    new_n_correct_edges -= n_lost_correct_edges;
    new_n_correct_edges += get_num_correct_edges(board, position_1, rotated_pieces);
    new_n_correct_edges += get_num_correct_edges(board, position_2, rotated_pieces);

    return new_n_correct_edges;
}

std::pair<Position, Position> get_corner_pair(corner_positions corner_positions)  {
    //TODO: dont initialize every time
    std::vector<uint_fast8_t> corner_indexes{0, 1, 2, 3};

    std::shuffle(corner_indexes);
    return std::make_pair(corner_positions[corner_indexes[0]], corner_position[corner_indexes[1]]);
}

corner_positions initialize_corner_positions() {
    corner_positions tmp_corner_positions;
    Position tmp_position = {0, 0};
    tmp_corner_positions.push_back (tmp_position);
    tmp_position = {0, PUZZLE_SIZE - 1};
    tmp_corner_positions.push_back (tmp_position);
    tmp_position = {PUZZLE_SIZE - 1 , 0};
    tmp_corner_positions.push_back (tmp_position);
    tmp_position = {PUZZLE_SIZE - 1, PUZZLE_SIZE - 1};
    tmp_corner_positions.push_back (tmp_position);
    return tmp_corner_positions;
}

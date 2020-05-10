#include <vector>
#include <array>
#include <utility>      // std::pair, std::make_pair
#include <tuple>

bool print_pieces(std::vector<std::array<int, 4>> pieces);
std::array<int, 4> rotate_piece(std::array<int, 4> piece, int orientation);
std::array<int, 2> get_next_position(int cols, std::array<int, 2> prev_position);
std::vector<int> initialize_grid(int rows, int cols);
std::vector<std::array<int, 4>> initialize_pieces(const char *filename);
bool is_move_legal(int *grid, std::array<int, 4> piece, std::array<int, 2> position, int rows, int cols);
std::tuple<bool, std::vector<int>, std::array<int, 2>> place_piece_on_grid(std::vector<int> grid, std::array<int, 4> piece, std::array<int, 2> position, int rows, int cols);

std::vector<std::pair<int, int>> get_valid_next_moves(std::vector<int> grid, std::vector<std::array<int, 4>> pieces, std::array<int, 2> position, int rows, int cols);


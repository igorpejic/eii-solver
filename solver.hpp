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



/* backtracker */


typedef struct PiecePlacement {
    int index;
    int rotation;
} PiecePlacement;

typedef struct Piece {
    int top;
    int right;
    int bottom;
    int left;
} Piece;

bool operator==(Piece& one, Piece& other);

typedef struct Position {
    int i;
    int j;
} Position;

typedef std::vector<Piece> pieces;
typedef std::vector<Piece> board;
pieces initialize_pieces_backtracker(const char *filename);
board initialize_board_b(int rows, int cols);
Piece rotate_piece_b(Piece piece, int orientation);
typedef std::vector<int> placed_pieces;

bool is_move_legal_b(board &board, Piece piece, Position &position, int rows, int cols);
Position get_next_position_b(int cols, Position prev_position);
std::vector<PiecePlacement> get_valid_next_moves_b(board board, placed_pieces &placed_pieces, pieces pieces, Position position,  int rows, int cols);

void print_pieces_b(board board);
void print_board_b(board board, int rows, int cols);

void print_pieces_louis_format(board &board, pieces &pieces);

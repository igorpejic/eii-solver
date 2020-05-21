#include <vector>
#include <array>
#include <utility>      // std::pair, std::make_pair
#include <tuple>
#include <unordered_set>
#include <unordered_map>

void print_pieces(std::vector<std::array<int, 4>> pieces);
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
    int orientation;
} PiecePlacement;

typedef struct Piece {
    int top;
    int right;
    int bottom;
    int left;
} Piece;

bool operator==(Piece& one, Piece& other);
bool operator==(PiecePlacement& one, PiecePlacement& other);

typedef struct Position {
    int i;
    int j;
} Position;

int pieceHasher(const PiecePlacement &p);

typedef std::vector<Piece> pieces;
typedef std::vector<PiecePlacement> board;
typedef std::unordered_map<int, std::unordered_set<PiecePlacement>> neighbours_map_t;


pieces initialize_pieces_backtracker(const char *filename);
board initialize_board_b(int rows, int cols);
Piece rotate_piece_b(Piece piece, int orientation);
typedef std::vector<int> placed_pieces;

Position get_next_position_b(int cols, Position prev_position);

std::vector<PiecePlacement> get_valid_next_moves_b(board &board, placed_pieces &placed_pieces, pieces &pieces, neighbours_map_t &neighbours_map_t,  Position &position, Piece** rotated_pieces,  int rows, int cols);

void print_board_editor_b(board board, Piece ** rotated_pieces);
void print_board_b(board board, Piece** rotated_pieces, int rows, int cols);

void print_board_louis_format(board &board);

Piece **get_rotated_pieces_b(pieces pieces);

neighbours_map_t get_possible_neighbours_map(pieces pieces, Piece** rotated_pieces);

int get_piece_hash(int piece_index, int orientation, int side);

namespace std {
    template <> struct hash<PiecePlacement>
    {
        std::size_t operator()(PiecePlacement const& p) const
        {
            return ((uint64_t)p.index)<<32 | (uint64_t)p.orientation;
        }
    };
}

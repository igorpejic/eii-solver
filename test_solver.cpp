#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "solver.hpp"
using namespace std;

TEST_CASE("Rotate piece", "[factorial]") {
    std::array<int, 4> piece = {1, 2, 3, 4}; 
    REQUIRE(rotate_piece(piece, 1)[0] == 4);
    REQUIRE(rotate_piece(piece, 1)[1] == 1);
    REQUIRE(rotate_piece(piece, 1)[2] == 2);
    REQUIRE(rotate_piece(piece, 1)[3] == 3);

    REQUIRE(rotate_piece(piece, 2)[0] == 3);
    REQUIRE(rotate_piece(piece, 2)[1] == 4);
    REQUIRE(rotate_piece(piece, 2)[2] == 1);
    REQUIRE(rotate_piece(piece, 2)[3] == 2);

    REQUIRE(rotate_piece(piece, 3)[0] == 2);
    REQUIRE(rotate_piece(piece, 3)[1] == 3);
    REQUIRE(rotate_piece(piece, 3)[2] == 4);
    REQUIRE(rotate_piece(piece, 3)[3] == 1);
}

TEST_CASE("Get next position", "[factorial]") {
    int cols = 6; 
    std::array<int, 4> piece = {1, 2, 3, 4}; 
    std::array<int, 2> position = {1, 1};

    std::array<int, 2> next_position = get_next_position(cols, position);
    REQUIRE(next_position[0] == 1);
    REQUIRE(next_position[1] == 2);

    position = {1, 2};
    next_position = get_next_position(cols, position);
    REQUIRE(next_position[0] == 1);
    REQUIRE(next_position[1] == 3);

    position = {1, 3};
    next_position = get_next_position(cols, position);
    REQUIRE(next_position[0] == 1);
    REQUIRE(next_position[1] == 4);

    position = {1, 4};
    next_position = get_next_position(cols, position);
    REQUIRE(next_position[0] == 1);
    REQUIRE(next_position[1] == 5);

    position = {1, 5};
    next_position = get_next_position(cols, position);
    REQUIRE(next_position[0] == 2);
    REQUIRE(next_position[1] == 0);

    position = {3, 5};
    next_position = get_next_position(cols, position);
    REQUIRE(next_position[0] == 4);
    REQUIRE(next_position[1] == 0);
}

TEST_CASE("Initialize grid", "[factorial]") {
    int cols = 6; 
    int rows = 6; 

    std::vector<int> grid = initialize_grid(rows, cols);
    REQUIRE(grid[0 * cols * 4 + 0 * 4 + 0] == EMPTY);
    REQUIRE(grid[5 * cols * 4 + 5 * 4 + 3] == EMPTY);
    REQUIRE(grid[5 * cols * 4 + 5 * 4 + 3] == EMPTY);
}

TEST_CASE("Is move legal", "[factorial]") {
    int cols = 6; 
    int rows = 6; 

    std::vector<int> grid = initialize_grid(rows, cols);
    std::array<int, 4> piece = {1, 2, 3, 4}; 
    std::array<int, 2> position = {0, 0};
    REQUIRE(is_move_legal(grid, piece, position, 6, 6) == false);
    std::array<int, 4> piece_2 = {0, 1, 2, 0}; 
    REQUIRE(is_move_legal(grid, piece_2, position, 6, 6) == true);
    std::array<int, 4> piece_3 = {0, 2, 0, 0}; 
    REQUIRE(is_move_legal(grid, piece_3, position, 6, 6) == false);
}

TEST_CASE("Is move legal 2", "[factorial]") {
    int cols = 6; 
    int rows = 6; 

    std::vector<int> grid = initialize_grid(rows, cols);
    std::array<int, 4> piece = {0, 4, 1, 0}; 
    std::array<int, 2> position = {0, 0};

    std::vector<int> new_grid;
    std::array<int, 2> new_position;
    bool success;

    std::tie(success, new_grid, new_position) = place_piece_on_grid(grid, piece, position, rows, cols);
    REQUIRE(success == true);
    REQUIRE(new_grid[0] == 0);
    REQUIRE(new_grid[1] == 4);
    REQUIRE(new_grid[2] == 1);
    REQUIRE(new_grid[3] == 0);
    REQUIRE(new_position[0] == 0);
    REQUIRE(new_position[1] == 1);

    piece = {0, 2, 1, 3}; 
    REQUIRE(is_move_legal(new_grid, piece, new_position, 6, 6) == false);
    REQUIRE(is_move_legal(new_grid, rotate_piece(piece, 1), new_position, 6, 6) == false);
    REQUIRE(is_move_legal(new_grid, rotate_piece(piece, 2), new_position, 6, 6) == false);
    REQUIRE(is_move_legal(new_grid, rotate_piece(piece, 3), new_position, 6, 6) == false);

    std::tie(success, new_grid, new_position) = place_piece_on_grid(new_grid, piece, new_position, rows, cols);
    REQUIRE(success == false);

    std::vector<std::array<int, 4>> pieces;
    pieces.push_back(piece);
    auto _result = get_valid_next_moves(new_grid, pieces, position, 6, 6);
    REQUIRE(_result.size() == 0);
}

TEST_CASE("Place piece on grid", "[factorial]") {
    int cols = 6; 
    int rows = 6; 
    std::array<int, 2> position = {0, 0};
    std::array<int, 4> piece = {0, 2, 3, 0}; 
    std::vector<int> grid = initialize_grid(rows, cols);

    std::vector<int> new_grid;
    std::array<int, 2> new_position;
    bool success;

    std::tie(success, new_grid, new_position) = place_piece_on_grid(grid, piece, position, rows, cols);
    REQUIRE(success == true);
    REQUIRE(new_grid[0 * cols * 4 + cols * 0 + 0] == 0);
    REQUIRE(new_grid[0 * cols * 4 + cols * 0 + 1] == 2);
    REQUIRE(new_grid[0 * cols * 4 + cols * 0 + 2] == 3);
    REQUIRE(new_grid[0 * cols * 4 + cols * 0 + 3] == 0);
    REQUIRE(grid[0 * cols * 4 + cols * 0 + 3] == EMPTY);
}

TEST_CASE("Get valid next moves", "[factorial]") {
    int cols = 6; 
    int rows = 6; 
    std::array<int, 2> position = {0, 0};
    std::array<int, 4> piece = {1, 2, 0, 0}; 
    std::vector<std::array<int, 4>> pieces;
    pieces.push_back(piece);
    std::vector<int> grid = initialize_grid(rows, cols);
    std::vector<std::pair<int, int>> result;

    result = get_valid_next_moves(grid, pieces, position, 6, 6);
    REQUIRE(result.size() == 1);
    REQUIRE(result[0].first == 0);
    REQUIRE(result[0].second == 1);
}

TEST_CASE("Find best position per hole tile", "[factorial]") {
    int cols = 4; 
    int rows = 4; 
    std::vector<Piece> pieces_b = initialize_pieces_backtracker(
            "test_fixtures/RandPuzzle4x4_01.txt");

    Piece **rotated_pieces = get_rotated_pieces_b(pieces_b);
    board  board;
    // 0/0   11/0    4/0    3/1
    // 8/3   12/2   15/2   10/1
    // 5/3   13/2   14/2    9/1
    // 2/3    7/2    6/2    1/2
    //
    //
    
    
    find_best_position_per_hole_tile (positions list_of_positions, board board, Piece ** rotated_pieces, int count_correct_edges);
    REQUIRE(pieces_b.size() == 16);
}

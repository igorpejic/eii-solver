#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "solver.hpp"

TEST_CASE("Rotate piece", "[factorial]") {
    int piece[4] = {1, 2, 3, 4}; 
    REQUIRE(rotate_piece(piece, 1)[0] == 4);
    REQUIRE(rotate_piece(piece, 2)[0] == 3);
}

TEST_CASE("Get next position", "[factorial]") {
    int cols = 4; 
    int piece[4] = {1, 2, 3, 4}; 
    int position[2] = {1, 1};

    int *next_position = get_next_position(cols, position);
    REQUIRE(next_position[0] == 1);
    REQUIRE(next_position[1] == 2);
}

TEST_CASE("Initialize grid", "[factorial]") {
    int cols = 6; 
    int rows = 6; 

    int *grid = initialize_grid(rows, cols);
    REQUIRE(grid[0 * cols * 4 + 0 * 4 + 0] == -1);
    REQUIRE(grid[5 * cols * 4 + 5 * 4 + 3] == -1);
    REQUIRE(grid[5 * cols * 4 + 5 * 4 + 3] == -1);
}

TEST_CASE("Is move legal", "[factorial]") {
    int cols = 6; 
    int rows = 6; 

    int *grid = initialize_grid(rows, cols);
    int piece[4] = {1, 2, 3, 4};
    int position[2] = {0, 0};
    REQUIRE(is_move_legal(grid, piece, position, 6, 6) == false);
    int piece_2[4] = {0, 1, 2, 0};
    REQUIRE(is_move_legal(grid, piece_2, position, 6, 6) == true);
}

TEST_CASE("Place piece on grid", "[factorial]") {
    int cols = 6; 
    int rows = 6; 
    int position[2] = {0, 0};
    int piece[4] = {0, 2, 3, 0};
    int *grid = initialize_grid(rows, cols);

    int *new_grid; 
    int *new_position;
    bool success;

    std::tie(success, new_grid, new_position) = place_piece_on_grid(grid, piece, position, rows, cols);
    REQUIRE(success == true);
    REQUIRE(new_grid[0 * cols * 4 + cols * 0 + 0] == 0);
    REQUIRE(new_grid[0 * cols * 4 + cols * 0 + 1] == 2);
    REQUIRE(new_grid[0 * cols * 4 + cols * 0 + 2] == 3);
    REQUIRE(new_grid[0 * cols * 4 + cols * 0 + 3] == 0);
    REQUIRE(grid[0 * cols * 4 + cols * 0 + 3] == -1);
}

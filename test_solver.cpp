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

    int ***grid = initialize_grid(rows, cols);
    REQUIRE(grid[0][0][0] == -1);
    REQUIRE(grid[5][5][3] == -1);
    REQUIRE(grid[5][5][3] == -1);
}

TEST_CASE("Is move legal", "[factorial]") {
    int cols = 6; 
    int rows = 6; 

    int ***grid = initialize_grid(rows, cols);
    int piece[4] = {1, 2, 3, 4};
    REQUIRE(is_move_legal(grid, cols, rows == -1);
    REQUIRE(grid[5][5][3] == -1);
    REQUIRE(grid[5][5][3] == -1);
}

#include "solver.hpp"

#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <map>
#include <tuple>
#include <cmath>
#include "mcts.hpp"
#include "backtracker.hpp"
#define MAX_DEPTH 100
#define AVG_DEPTH 101
#define SIMULATION_REPEATS 5


using namespace std;

void print_solution(std::vector<std::pair<std::array<int, 4>, std::array<int, 2>>> solution_pieces_order, int rows);

void print_solution(std::vector<std::pair<std::array<int, 4>, std::array<int, 2>>> solution_pieces_order, int rows) {
    for (int i = 0; i < rows * rows; i++) {
        if (i % rows == 0) {
            std::cout << std::endl;
        }
        std::cout << solution_pieces_order[i].first[0] << " "
            << solution_pieces_order[i].first[1] << " "
            << solution_pieces_order[i].first[2] << " "
            << solution_pieces_order[i].first[3] << "   ";
    }
    std::cout << std::endl;
}

int main (int argc, const char* argv[]) {
    std::vector<Piece> pieces_b = initialize_pieces_backtracker(argv[1]);
    int rows = sqrt(pieces_b.size());
    Position position = {0, 0};

    // holding rotated pieces to speed up computation; read only
    Piece **rotated_pieces = get_rotated_pieces_b(pieces_b);

    placed_pieces placed_pieces(pieces_b.size(), 0);  
    board board = initialize_board_b(rows, rows);

    neighbours_map_t neighbours_map = get_possible_neighbours_map(pieces_b, rotated_pieces);

    int tiles_placed = 0;
    bool solution_found = false;
    backtrack(pieces_b, placed_pieces, rotated_pieces, neighbours_map, board, position, rows, &tiles_placed, &solution_found);

    std::cout << "tiles_placed: " << tiles_placed << std::endl;

    return 0;
}

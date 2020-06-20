#include "solver.hpp"

#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <map>
#include <random>
#include <tuple>
#include <cmath>
#include <chrono>
#include "mcts.hpp"
#include "backtracker.hpp"
#include "ucb.hpp"
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

    //placed_pieces placed_pieces(pieces_b.size(), false);  
    placed_pieces placed_pieces;  

    int max_pieces_placed = 0;

    board board = initialize_board_b(rows, rows);

    neighbours_map_t neighbours_map = get_possible_neighbours_map(pieces_b, rotated_pieces);

    int tiles_placed = 0;
    bool solution_found = false;
    auto start = std::chrono::high_resolution_clock::now();

    std::random_device rd;
    auto rng = std::default_random_engine{rd()};
    bool use_backtracker = true;
    if (argc > 2) {
        if(std::string(argv[2]) == "mcts") {
            use_backtracker = false;
            std::cout << "Running with MCTS" << std::endl;
        } else {
            std::cout << "Running using backtracker." << std::endl;
        }
    }
    if (use_backtracker) {

        positions corner_positions = initialize_corner_positions();
        positions edge_positions = initialize_edge_positions();
        positions inner_positions = initialize_inner_positions();

        position_indexes corner_indexes = get_range(corner_positions.size());
        position_indexes edge_indexes = get_range(edge_positions.size());
        position_indexes inner_indexes = get_range(inner_positions.size());

        backtrack(placed_pieces, rotated_pieces, neighbours_map, board, position, corner_positions, edge_positions, inner_positions, corner_indexes, edge_indexes, inner_indexes, &tiles_placed, &max_pieces_placed, rng, &solution_found);
    } else {
        int _tiles_placed = play_game(placed_pieces, rotated_pieces, neighbours_map, board, position, &tiles_placed, &max_pieces_placed, rng, &solution_found);
        tiles_placed = _tiles_placed;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "duration: " << duration / 1000.0 << "s" << std::endl;
    std::cout << "placements/s: " << tiles_placed / (duration / 1000.0) << std::endl;
    std::cout << "tiles_placed: " << tiles_placed << std::endl;

    //std::ofstream outfile;
    //outfile.open("results_backtrack.csv", std::ofstream::app);
    //std::time_t timestamp = std::time(nullptr);
    //outfile << std::put_time(std::localtime(&timestamp), "%y-%m-%d %OH:%OM:%OS") << ";";
    //
    //outfile << argv[1] << ";";
    //outfile << rows << ";";
    //outfile << tiles_placed << ";";
    //outfile << duration << ";";
    //outfile << std::endl;

    return 0;
}

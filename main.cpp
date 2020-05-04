#include "solver.hpp"

#include <iostream>
#include <tuple>
#include <cmath>
#include "mcts.hpp"
#define MAX_DEPTH 100
#define AVG_DEPTH 101

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
    // insert code here...
    std::cout << "Hello, World!\n";
    std::vector<std::array<int, 4>> pieces = initialize_pieces(argv[1]);
    int rows = sqrt(pieces.size());
    CustomMCTS mcts(initialize_pieces(argv[1]), rows, rows, MAX_DEPTH);
    tuple<state_t, int, bool> result;
    result = mcts.predict(10000);
    cout << "In total placed tiles:" << mcts.m_pieces_placed << std::endl;
    cout << "Pieces placed:" << mcts.m_solution_pieces_order.size() << std::endl;
    if (mcts.m_solution_pieces_order.size() == rows * rows) {
        print_solution(mcts.m_solution_pieces_order, rows);
    }
    return 0;
}

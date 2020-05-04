#include "solver.hpp"

#include <iostream>
#include <tuple>
#include <cmath>
#include "mcts.hpp"
#define MAX_DEPTH 100
#define AVG_DEPTH 101

using namespace std;

int main (int argc, const char* argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    std::vector<std::array<int, 4>> pieces = initialize_pieces(argv[1]);
    int rows = sqrt(pieces.size());
    CustomMCTS mcts(initialize_pieces(argv[1]), rows, rows, MAX_DEPTH);
    tuple<state_t, int, bool> result;
    result = mcts.predict(5000);
    cout << "In total placed tiles:" << mcts.m_pieces_placed << std::endl;
    cout << "Pieces placed:" << mcts.m_solution_pieces_order.size() << std::endl;
    return 0;
}

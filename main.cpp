#include "solver.hpp"

#include <iostream>
#include <tuple>
#include <cmath>
#include "mcts.hpp"
using namespace std;

int main (int argc, const char* argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    std::vector<std::array<int, 4>> pieces = initialize_pieces(argv[1]);
    int rows = sqrt(pieces.size());
    CustomMCTS mcts(initialize_pieces(argv[1]), rows, rows);
    tuple<state_t, int, bool> result;
    result = mcts.predict(1000);
    return 0;
}

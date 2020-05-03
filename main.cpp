#include "solver.hpp"

#include <iostream>
#include "mcts.hpp"
using namespace std;

int main (int argc, const char* argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    CustomMCTS(initialize_pieces(argv[1]));
    return 0;
}

#include <vector>
#include <array>
#include "mcts.hpp"

CustomMCTS::CustomMCTS(std::vector<std::array<int, 4>> pieces) {
    initial_pieces = pieces;
}

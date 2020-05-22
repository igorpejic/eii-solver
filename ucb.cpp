#include <vector>
#include <cstring>
#include <tuple>
#include <utility>      // std::pair, std::make_pair
#include <array>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <random>
#include "mcts.hpp"
#include "solver.hpp"
#include "ucb.hpp"

void play_game(pieces &pieces, placed_pieces _placed_pieces, Piece** rotated_pieces, neighbours_map_t &neighbours_map, board board, Position position, int * const tiles_placed, int *max_pieces_placed, std::default_random_engine rng, bool *solution_found) { 
    Node node(board, _placed_pieces);

    MCTS mcts(1);

    while(true) {
        break;

        if (node.m_is_terminal) {
            // terminal node
            std::cout << "terminal node" << std::endl;
        }
        node = mcts.choose(node); 
    }
}

MCTS::MCTS(int dummy) {
    m_dummy = 1;

}

Node::Node(board &board, placed_pieces _placed_pieces) {
    m_board = board;
    m_placed_pieces = _placed_pieces;
    m_is_terminal = false;
}

Node MCTS::choose(Node node) {
    return node;
}

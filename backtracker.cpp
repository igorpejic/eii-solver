#include "solver.hpp"
#include <ctime>
#include <algorithm>

#include <random>
#include <iostream>
using namespace std;

void swapper(placed_pieces _placed_pieces, Piece** rotated_pieces, neighbours_map_t &neighbours_map, board board, positions &corner_positions, positions &edge_positions, positions &inner_positions, position_indexes &corner_indexes, position_indexes &edge_indexes, position_indexes &inner_indexes,  int * const tiles_placed, int *max_pieces_placed, std::default_random_engine rng, bool *solution_found) { 
    pair_positions tmp_positions = get_pair_pieces(inner_indexes, inner_positions, rng);

    int correct_positions = count_correct_edges(board, rotated_pieces);
    cout << "correct_positions before starting to swap:" << correct_positions << std::endl;
    std::cout << (int)std::get<0>(tmp_positions).i << "-" << (int)std::get<0>(tmp_positions).j << std::endl;
    std::cout << (int)std::get<1>(tmp_positions).i << "-" << (int)std::get<1>(tmp_positions).j << std::endl;
     print_board_b(board, rotated_pieces, PUZZLE_SIZE, PUZZLE_SIZE);

    int new_correct_positions = swap_tiles(board, std::get<0>(tmp_positions), std::get<1>(tmp_positions), correct_positions, rotated_pieces);

    std::cout << "Correct positions after swap:" << new_correct_positions << std::endl;
    print_board_b(board, rotated_pieces, PUZZLE_SIZE, PUZZLE_SIZE);
}

void backtrack(placed_pieces _placed_pieces, Piece** rotated_pieces, neighbours_map_t &neighbours_map, board board, Position position, positions &corner_positions, positions &edge_positions, positions &inner_positions, position_indexes &corner_indexes, position_indexes &edge_indexes, position_indexes &inner_indexes,  int * const tiles_placed, int *max_pieces_placed, std::default_random_engine rng, bool *solution_found) { 
    if (*solution_found) {
        // exit all backtracks
        return;
    }
    std::vector<PiecePlacement> next_moves = get_valid_next_moves_b(board, _placed_pieces, neighbours_map, position, rotated_pieces);


    if (_placed_pieces.count() > *max_pieces_placed) {
        *max_pieces_placed = _placed_pieces.count();

        pair_positions tmp_positions = get_pair_pieces(inner_indexes, inner_positions, rng);
        //std::cout << *max_pieces_placed << std::endl;
        //print_board_b(board, rotated_pieces, PUZZLE_SIZE, PUZZLE_SIZE);
        //print_board_editor_b(board, rotated_pieces);
    }

    if (position.i == PUZZLE_SIZE && position.j == 0) {
        print_board_b(board, rotated_pieces, PUZZLE_SIZE, PUZZLE_SIZE);
        for (int j = 0; j < _placed_pieces.size(); j++) {
            cout << _placed_pieces[j];
        }
        cout << std::endl;
        std::cout << "solution found" << std::endl;
        *solution_found = true;

        if (*solution_found) {
            std::cout << "entering swapper" << std::endl;
            swapper(_placed_pieces, rotated_pieces, neighbours_map, board,  corner_positions, edge_positions, inner_positions, corner_indexes, edge_indexes, inner_indexes, tiles_placed, max_pieces_placed, rng, solution_found);
        }
        print_board_editor_b(board, rotated_pieces);
        print_board_louis_format(board);
        return;
    }

    //shuffle the vector of possible moves before returning it such that some randomness is introduced in the search process
    //this is equivalent to drawing a lottery ticket,
    //although the chances of winning are lower with E-II
    //std::shuffle(std::begin(next_moves), std::end(next_moves), rng);
    for (int i = 0; i < next_moves.size(); i++) {
        PiecePlacement piece_placement;
        piece_placement.index = next_moves[i].index;
        piece_placement.orientation = next_moves[i].orientation;
        board[position.i * PUZZLE_SIZE + position.j] = piece_placement;
        placed_pieces new_placed_pieces;
        new_placed_pieces = _placed_pieces;
        new_placed_pieces[next_moves[i].index] = true;
        (*tiles_placed)++;
        Position next_position = get_next_position_b(PUZZLE_SIZE, position);

        backtrack(new_placed_pieces, rotated_pieces, neighbours_map, board, next_position, corner_positions, edge_positions, inner_positions, corner_indexes, edge_indexes, inner_indexes, tiles_placed, max_pieces_placed, rng, solution_found);
    }
    //print_board_b(board, rotated_pieces, PUZZLE_SIZE, PUZZLE_SIZE);
}

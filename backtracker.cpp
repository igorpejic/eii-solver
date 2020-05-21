#include "solver.hpp"
#include <ctime>
#include <chrono>

#include <iostream>
using namespace std;

void backtrack(pieces &pieces, placed_pieces _placed_pieces, Piece** rotated_pieces, board board, Position position, int rows, int *tiles_placed, bool *solution_found) { 
    if (*solution_found) {
        // exit all backtracks
        return;
    }

    auto start = std::chrono::steady_clock::now();

    std::vector<PiecePlacement> next_moves = get_valid_next_moves_b(board, _placed_pieces, pieces, position, rotated_pieces, rows, rows);
    if (position.i == rows && position.j == 0) {
        print_board_b(board, rows, rows);
        for (int j = 0; j < _placed_pieces.size(); j++) {
            cout << _placed_pieces[j];
        }
        cout << std::endl;
        print_board_b(board, rows, rows);
        std::cout << "solution found" << std::endl;
        *solution_found = true;
        print_pieces_b(board);
        print_pieces_louis_format(board, pieces);
        return;
    }
    for (int i = 0; i < next_moves.size(); i++) {
        Piece piece = rotated_pieces[next_moves[i].index][next_moves[i].rotation];
        board[position.i * rows + position.j] = piece;
        placed_pieces new_placed_pieces;
        new_placed_pieces = _placed_pieces;
        new_placed_pieces[next_moves[i].index] = 1;
        (*tiles_placed)++;
        Position next_position = get_next_position_b(rows, position);

        backtrack(pieces, new_placed_pieces, rotated_pieces, board, next_position, rows, tiles_placed, solution_found);
    }
}

#include <vector>
#include <tuple>
#include <array>
#include <iostream>
#include "mcts.hpp"
#include "solver.hpp"
#define ALL_PIECES_USED -2


CustomMCTS::CustomMCTS(std::vector<std::array<int, 4>> pieces, int rows, int cols) {
    m_pieces_placed = 0;
    m_rows = rows;
    m_cols = cols;
    m_initial_pieces = pieces;
    m_initial_grid = initialize_grid(rows, cols);
    state_t * m_state;
}

std::tuple<state_t, int, bool>CustomMCTS::predict(int N=1000) {
    state_t *initial_state = &m_state;
    state_t *state = &m_state;
    auto available_pieces = m_initial_pieces;
    state->m_pieces = available_pieces;
    state->m_grid = m_initial_grid;
    state_t *prev_state = state;
    bool solution_found = false;

    int depth = 0;
    int next_position[2] = {0, 0};

    while (state->m_pieces.size()) {
        int piece_placed = false;
        std::vector<state_t> states;
        std::cout << depth << '\n';
        for(int j = 0; j < state->m_pieces.size() * 4; j++) {
            int rotation = j % 4;
            int piece_index = j / 4;
            std::array<int, 4> piece = rotate_piece(state->m_pieces[piece_index], rotation);
            bool success;
            int *new_grid;
            int *potential_next_position;
            std::tie(success, new_grid, potential_next_position) = place_piece_on_grid(
                    state->m_grid, piece, next_position, m_rows, m_cols);
            if (success) {
                std::cout << "success";
            }
            m_pieces_placed += 1;

            if (!success) {
                //cannot place piece. this branch will not be considered
                continue;
            } else {
                piece_placed = true;
                auto new_pieces = state->m_pieces;
                new_pieces.erase(new_pieces.begin() + piece_index);
                state_t new_state;
                new_state.m_grid = new_grid;
                new_state.m_pieces = new_pieces;
            }
        }
    }
}

std::tuple<bool, float, std::vector<std::array<int, 6>>> CustomMCTS::perform_simulations(state_t state, int* next_position, int N) {  
    int *depths = (int*) malloc(sizeof(int) * N);
    float _sum = 0;
    double _max = 0;
    int max_depth = 0;
    for (int i = 0; i < N; i++) {
        //
        bool all_pieces_placed = false;
        int depth = 0;
        if (all_pieces_placed) {

        } else {
            depths[i] = depth;
        }
        if (m_strategy == MAX_DEPTH && depth > _max) {
            _max = depth;
        } else if(m_strategy == AVG_DEPTH) {
            _sum += depth;
        }
    }
    if (m_strategy == AVG_DEPTH) {
        float = _sum / N;
    }
    return std::make_tuple(false, _max, NULL);
}

std::tuple<int, std::vector<std::array<int, 6>>> CustomMCTS::perform_simulation(state_t state, int *next_position) {
    std::vector<std::array<int, 6>> solution_pieces_order;
    int depth = 0;
    if (state->m_pieces.size() == 0) {
        std::cout << "perform simulation called with empty spaces \n";
        return std::make_tuple(ALL_PIECES_USED, solution_pieces_order);
    }
    int *grid = state->m_grid;

    while(true) {
        if (state->m_pieces.size() == 0) {
            std::cout << "solution found in perform_simulation";
            return std::make_tuple(ALL_PIECES_USED, solution_pieces_order);
        }
    }
}

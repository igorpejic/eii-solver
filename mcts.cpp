#include <vector>
#include <tuple>
#include <utility>      // std::pair, std::make_pair
#include <array>
#include <iostream>
#include <iomanip>
#include "mcts.hpp"
#include "solver.hpp"
#define ALL_PIECES_USED -2
#define NO_VALUE -3

#define MAX_DEPTH 100
#define AVG_DEPTH 101


CustomMCTS::CustomMCTS(std::vector<std::array<int, 4>> pieces, int rows, int cols, int strategy) {
    m_pieces_placed = 0;
    m_rows = rows;
    m_cols = cols;
    m_initial_pieces = pieces;
    m_initial_grid = initialize_grid(rows, cols);
    m_strategy = strategy;
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
    state_t new_state;

    int depth = 0;
    std::array<int, 2> next_position = {0, 0};

    std::vector<std::pair<std::array<int, 4>, std::array<int, 2>>> solution_pieces_order;
    srand(1);

    while (state->m_pieces.size()) {
        int piece_placed = false;
        std::vector<state_t> states;
        //print_grid(state->m_grid, m_rows, m_cols);
        //print_pieces(state->m_pieces);
        for(int j = 0; j < state->m_pieces.size() * 4; j++) {
            int rotation = j % 4;
            int piece_index = j / 4;
            std::array<int, 4> piece = rotate_piece(state->m_pieces[piece_index], rotation);
            bool success;
            int *new_grid;
            std::array<int, 2> potential_next_position;
            std::tie(success, new_grid, potential_next_position) = place_piece_on_grid(
                    state->m_grid, piece, next_position, m_rows, m_cols);
            m_pieces_placed += 1;

            if (!success) {
                //cannot place piece. this branch will not be considered
                continue;
            } else {
                //std::cout << "success" << piece[0] << piece[1] << piece[2] << piece[3] << std::endl;
                //std::cout << "piece" << state->m_pieces[piece_index][0] << state->m_pieces[piece_index][1] << state->m_pieces[piece_index][2] << state->m_pieces[piece_index][3] << std::endl;
                //print_grid(state->m_grid, m_rows, m_cols);
                
                piece_placed = true;
                auto new_pieces = state->m_pieces;
                new_pieces.erase(new_pieces.begin() + piece_index);
                new_state.m_grid = new_grid;
                new_state.m_pieces = new_pieces;
                new_state.m_piece_placed[0] = NO_VALUE;
                float simulation_depth;
                tie(solution_found, simulation_depth, solution_pieces_order) = perform_simulations(
                        new_state, potential_next_position, N);
                if (solution_found) {
                    std::vector<std::pair<std::array<int, 4>, std::array<int, 2>>> vector_to_insert;
                    if (state->m_piece_placed[0] != NO_VALUE) {
                        vector_to_insert.push_back(std::make_pair(state->m_piece_placed, next_position));
                        m_solution_pieces_order.insert(m_solution_pieces_order.end(), vector_to_insert.begin(), vector_to_insert.end());
                        m_solution_pieces_order.insert(m_solution_pieces_order.end(), solution_pieces_order.begin(), solution_pieces_order.end());
                    } else {
                        vector_to_insert.push_back(std::make_pair(piece, next_position));
                        m_solution_pieces_order.insert(m_solution_pieces_order.end(), vector_to_insert.begin(), vector_to_insert.end());
                        m_solution_pieces_order.insert(m_solution_pieces_order.end(), solution_pieces_order.begin(), solution_pieces_order.end());
                    }
                    std::cout << "Solution found in simulation!" << std::endl;
                    solution_found = true;
                    return std::make_tuple(*initial_state, depth, solution_found);
                } else {
                    new_state.m_score = simulation_depth;
                    new_state.m_next_position = potential_next_position;
                    new_state.m_previous_piece = piece;
                    state->m_piece_placed = piece;
                    states.push_back(new_state);
                }
            }
        }
        if (!piece_placed) {
            std::cout << "No piece placed. Exiting" << std::endl;
            return std::make_tuple(*initial_state, depth, solution_found);
        }

        depth += 1;
        int best_action = get_max_index(states);
        state_t *prev_state = state;
        new_state = states[best_action];

        std::vector<std::pair<std::array<int, 4>, std::array<int, 2>>> vector_to_insert;
        vector_to_insert.push_back(std::make_pair(new_state.m_previous_piece, next_position));
        m_solution_pieces_order.insert(m_solution_pieces_order.end(), vector_to_insert.begin(), vector_to_insert.end());

        new_state = states[best_action];
        next_position = new_state.m_next_position;
        //std::cout << "Best action" << best_action <<std::endl;
        //std::cout << "Next position (" << next_position[0] << " "  << next_position[1] << ")" << std::endl;
        state = &new_state;
        print_pieces_solution_order();
    }
    std::cout << "Pieces size: " << state->m_pieces.size() << std::endl;
}

std::tuple<bool, float, std::vector<std::pair<std::array<int, 4>, std::array<int, 2>> >> CustomMCTS::perform_simulations(state_t state, std::array<int, 2> next_position, int N) {  
    float _sum = 0;
    float _max = 0;
    int max_depth = 0;
    std::vector<std::pair<std::array<int, 4>, std::array<int, 2> >> solution_pieces_order;
    for (int i = 0; i < N; i++) {
        int depth = 0;
        tie(depth, solution_pieces_order) = perform_simulation(state, next_position);
        if (depth == ALL_PIECES_USED) {
            return std::make_tuple(true, depth, solution_pieces_order);
        } 
        if (m_strategy == MAX_DEPTH && depth > _max) {
            _max = depth;
        } else if(m_strategy == AVG_DEPTH) {
            _sum += depth;
        }
    }
    if (m_strategy == AVG_DEPTH) {
        _max = _sum / N;
    }
    return std::make_tuple(false, _max, solution_pieces_order);
}

std::tuple<int, std::vector<std::pair<std::array<int, 4>, std::array<int, 2>> >> CustomMCTS::perform_simulation(state_t state, std::array<int, 2> next_position) {
    std::vector<std::pair< std::array<int, 4>, std::array<int, 2> > > solution_pieces_order;
    int depth = 0;
    if (state.m_pieces.size() == 0) {
        std::cout << "perform simulation called with empty spaces \n";
        return std::make_tuple(ALL_PIECES_USED, solution_pieces_order);
    }
    int *grid = state.m_grid;

    std::vector<std::array<int, 4>> new_pieces;
    while(true) {
        if (state.m_pieces.size() == 0) {
            std::cout << "solution found in perform_simulation";
            return std::make_tuple(ALL_PIECES_USED, solution_pieces_order);
        }
        std::vector<std::pair<int, int>> valid_moves = get_valid_next_moves(
                grid, state.m_pieces, next_position, m_rows, m_cols);
        if (valid_moves.size() == 0) {
            return make_tuple(depth, solution_pieces_order);
        }
        int next_random_piece_index = std::rand() % valid_moves.size();
        std::pair<int, int> next_piece = valid_moves[next_random_piece_index];

        bool success;
        std::array<int, 2> _next_position;

        std::array<int, 4> piece_to_place;

        piece_to_place = rotate_piece(state.m_pieces[next_piece.first], next_piece.second);

        std::tie(success, grid, _next_position) = place_piece_on_grid(
                grid, piece_to_place,
                next_position, m_rows, m_cols);

        m_pieces_placed += 1;

        solution_pieces_order.push_back(std::make_pair(piece_to_place, next_position));
        next_position = _next_position;

        new_pieces = state.m_pieces;
        new_pieces.erase(new_pieces.begin() + next_random_piece_index);

        state_t new_state;
        new_state.m_grid = grid;
        new_state.m_pieces = new_pieces;
        state = new_state;
        depth += 1 ;
    }
    return std::make_tuple(depth, solution_pieces_order);
}

int get_max_index(std::vector<state_t> states) {
    int max_score = 0;
    int max_index = 0;
    for(int i = 0; i < states.size(); i++) {
        if(states[i].m_score > max_score) {
            max_score = states[i].m_score;
            max_index = i;
        }
    }
    return max_index;
}

void CustomMCTS::print_pieces_solution_order() {
    for (int i =0; i < m_solution_pieces_order.size(); i++) {
        std::cout << "{(" << m_solution_pieces_order[i].first[0] << "," <<  m_solution_pieces_order[i].first[1] << "," << m_solution_pieces_order[i].first[2] << "," << m_solution_pieces_order[i].first[3] << "," << ") (" <<  m_solution_pieces_order[i].second[0] << " " << m_solution_pieces_order[i].second[1] << ")}  " ;
    }
    std::cout << std::endl;
}

void print_grid(int *grid, int rows, int cols) {
    for (int line = 0; line < rows; line++) {
        int matrix_row = line;
        for (int col = 0; col < cols; col++) {
            std::cout<< "\\" << std::setw(2) << grid[matrix_row * cols * 4 + col * 4] << " /" << "|";
        }
        std::cout<< std::endl;

        for (int col = 0; col < cols; col++) {
            std::cout<< std::setw(2) << grid[matrix_row * cols * 4 + col * 4 + 3]  << "X" << std::setw(2) << grid[matrix_row * cols * 4 + col * 4 + 1] << "|";
        }
        std::cout<< std::endl;

        for (int col = 0; col < cols; col++) {
            std::cout<< "/" << std::setw(2) << grid[matrix_row * cols * 4 + col * 4 + 2] << " \\" << "|";
        }
        std::cout<< std::endl;
        for (int col = 0; col < cols; col++) {
            std::cout<< "------";
        }
        std::cout<< std::endl;
    }
    std::cout<< "\n" << std::endl;
}

void print_pieces(std::vector<std::array<int, 4>> m_pieces) {
    for (int i=0; i < m_pieces.size(); i++) {
        std::cout << "(" 
            << m_pieces[i][0] << ","
            << m_pieces[i][1] << ","
            << m_pieces[i][2] << ","
            << m_pieces[i][3] << ","
            << ")" << " ";
    }
    std::cout << std::endl;
}

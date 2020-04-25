import math
import json
import numpy as np
import random
from collections import OrderedDict
from state import State
from solver import rotate_piece, place_piece_on_grid, get_valid_next_moves

ALL_PIECES_USED = 'A'

def get_max_index(_list):
    max_index = 0
    max_value = -math.inf
    for i, el in enumerate(_list):
        if not el:
            continue
        if el.score > max_value:
            max_value = el.score
            max_index = i
    return max_index

class CustomMCTS():
    def __init__(self, pieces, grid, strategy='max_depth', is_circular=False):

        self.initial_pieces, self.initial_grid = pieces, grid
        self.state = State(self.initial_grid, self.initial_pieces)
        self.strategy=strategy
        self.n_pieces_placed = 0
        self.solution_pieces_order = []
        self.is_circular = is_circular

    def predict(self, temp=1, N=1000):
        initial_state = self.state
        state = self.state
        available_pieces = state.pieces
        prev_state = state
        solution_found = False

        depth = 0
        val = 1
        next_position = (0, 0)
        while len(state.pieces):
            piece_placed = False
            states = []
            best_score = 0
            for j in range(0, len(state.pieces) * 4):
                rotation = j % 4
                piece_index = j // 4
                piece = rotate_piece(state.pieces[piece_index], rotation)
                success, new_grid, potential_next_position = place_piece_on_grid(
                    state.grid, piece, next_position, is_circular=self.is_circular)
                self.n_pieces_placed += 1
                if not success:
                    # cannot place piece. this branch will not be considered
                    continue
                else:
                    piece_placed = True
                    new_pieces = np.delete(state.pieces, piece_index)
                    new_state = State(grid=new_grid, pieces=new_pieces, parent=state)
                    state.children.append(new_state)
                    simulation_result, solution_pieces_order = self.perform_simulations(new_state, potential_next_position, N=N)
                    if simulation_result == ALL_PIECES_USED:
                        print('Solution found in simulation!')
                        solution_found = True
                        if state.piece_placed:
                            self.solution_pieces_order.extend([[state.piece_placed, next_position]] + solution_pieces_order)
                        else:
                            self.solution_pieces_order.extend([[piece, next_position]] + solution_pieces_order)
                        return initial_state, depth, solution_found
                    new_state.score = simulation_result
                    new_state.next_position = potential_next_position
                    state.solution_pieces_order.append([piece, next_position])
                    state.piece_placed = piece
                    states.append(new_state)
            if not piece_placed:
                # no piece was placed, it's a dead end; end game
                print(state.pieces)
                return initial_state, depth, solution_found

            depth += 1
            best_action = get_max_index(states) 
            prev_state = state
            new_state = states[best_action]
            if prev_state.piece_placed:
                self.solution_pieces_order.append([prev_state.piece_placed, next_position])
            print(self.solution_pieces_order)
            next_position = new_state.next_position

            state = new_state

        print('Solution found!')
        solution_found = True
        return initial_state, depth, solution_found


    def perform_simulations(self, state, position, N=3000):
        '''
        Given a state perform N simulations.
        One simulation consists of either filling container or having no more pieces to place.

        Returns average depth
        '''
        depths = []
        for n in range(N):
            depth, simulation_root_state, solution_pieces_order = self.perform_simulation(state.copy(), position)
            if depth == ALL_PIECES_USED:
                state.children = simulation_root_state.children
                return ALL_PIECES_USED, solution_pieces_order
            else:
                depths.append(depth)
        if self.strategy=='max_depth':
            _max = np.max(np.array(depths))
        elif self.strategy == 'avg_depth':
            _max = np.average(np.array(depths))
        return _max, None


    def perform_simulation(self, state, next_position):
        '''
        Performs the simulation until legal moves are available.
        If simulation ends by finding a solution, a root state starting from this simulation is returned
        '''

        solution_pieces_order = []
        depth = 0
        simulation_root_state = state  # in case simulation ends in solution; these states are the solution
        if len(state.pieces) == 0:
            print('perform_simulation called with empty pieces')
            return ALL_PIECES_USED, simulation_root_state, solution_pieces_order
        grid = state.grid
        while True:
            val = 1
            if len(state.pieces) == 0:
                print('solution found in simulation')
                return ALL_PIECES_USED, simulation_root_state, solution_pieces_order

            valid_moves = get_valid_next_moves(state.grid, state.pieces, next_position)
            if not valid_moves:
                return depth, simulation_root_state, solution_pieces_order

            next_random_piece_index = random.randint(0, len(valid_moves) -1)
            # print(len(valid_moves), state.pieces, next_random_piece_index)
            # print(next_random_piece_index)

            next_piece = valid_moves[next_random_piece_index]

            piece_to_place = rotate_piece(state.pieces[next_piece[0]], next_piece[1])
            success, grid, _next_position = place_piece_on_grid(
                grid, rotate_piece(state.pieces[next_piece[0]], next_piece[1]),
                next_position, is_circular=self.is_circular
            )
            
            self.n_pieces_placed += 1
            solution_pieces_order.append([piece_to_place, next_position])
            next_position = _next_position

            new_pieces = np.copy(state.pieces)
            new_pieces = np.delete(state.pieces, next_random_piece_index)
            new_state = State(grid=grid, pieces=new_pieces, parent=state)

            new_state.score = -1  #  because no choice is performed for sequent actions
            state.children.append(new_state)
            state = new_state
            depth += 1
        return depth, simulation_root_state, solution_pieces_order

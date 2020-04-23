import math
import json
import numpy as np
import random
from collections import OrderedDict
from state import State

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
    def __init__(self, pieces, grid, strategy='max_depth'):

        self.initial_pieces, self.initial_grid = pieces, grid
        self.state = State(self.initial_grid, self.initial_pieces)
        self.solution_checker = SolutionChecker(len(pieces), get_rows(grid), get_cols(grid))
        self.strategy=strategy
        self.n_pieces_placed = 0
        self.solution_pieces_order = []

    def predict(self, temp=1, N=3000):
        initial_state = self.state
        state = self.state
        available_pieces = state.pieces
        prev_state = state
        solution_found = False

        depth = 0
        val = 1
        next_position = (0, 0)
        while len(state.pieces):
            tile_placed = False
            states = []
            best_score = 0
            for i, piece in enumerate(pieces):
                for rotation in range(4):
                    rotated_piece = rotate_piece(piece, rotation)
                    success, new_grid, next_position = place_piece_on_grid(
                        grid, rotated_piece, next_position)

                    self.n_pieces_placed += 1
                    if not success:
                        # cannot place piece. this branch will not be considered
                        continue
                    else:
                        tile_placed = True
                        new_pieces = np.delete(state.pieces, i)
                        new_state = State(grid=new_grid, pieces=new_pieces, parent=state)
                        state.children.append(new_state)
                        simulation_result, solution_pieces_order = self.perform_simulations(new_state, N=N)
                        if simulation_result == ALL_pieces_USED:
                            print('solution found in simulation!')
                            print(tile)
                            solution_found = True
                            if state.tile_placed:
                                self.solution_pieces_order.extend([state.tile_placed] + [tile] + solution_pieces_order)
                            else:
                                self.solution_pieces_order.extend([tile] + solution_pieces_order)
                            return initial_state, depth, solution_found
                        new_state.score = simulation_result
                        if new_state.score > best_score:
                            best_tile = tile
                            best_score = new_state.score
                        new_state.tile_placed = tile
                        state.solution_pieces_order.append(tile)
                        states.append(new_state)
            if not tile_placed:
                # no tile was placed, it's a dead end; end game
                return initial_state, depth, solution_found

            # PERFORMANCE:
            # for visualization this can be changed
            # all pieces will be 1 inside the frame for performance reasons
            # val += 1

            depth += 1
            best_action = get_max_index(states) 
            prev_state = state
            new_state = states[best_action]
            print(best_tile, prev_state.tile_placed)
            if prev_state.tile_placed:
                self.solution_pieces_order.append(prev_state.tile_placed)


            state = new_state

        print('Solution found!')
        solution_found = True
        return initial_state, depth, solution_found


    def perform_simulations(self, state, N=3000):
        '''
        Given a state perform N simulations.
        One simulation consists of either filling container or having no more pieces to place.

        Returns average depth
        '''
        depths = []
        for n in range(N):
            depth, simulation_root_state, solution_pieces_order = self.perform_simulation(state.copy())
            if depth == ALL_pieces_USED:
                state.children = simulation_root_state.children
                return ALL_pieces_USED, solution_pieces_order
            else:
                depths.append(depth)
        if self.strategy=='max_depth':
            _max = np.max(np.array(depths))
        elif self.strategy == 'avg_depth':
            _max = np.average(np.array(depths))
        return _max, None


    def perform_simulation(self, state):
        '''
        Performs the simulation until legal moves are available.
        If simulation ends by finding a solution, a root state starting from this simulation is returned
        '''

        solution_pieces_order = []
        depth = 0
        simulation_root_state = state  # in case simulation ends in solution; these states are the solution
        if len(state.pieces) == 0:
            print('perform_simulation called with empty pieces')
            return ALL_pieces_USED, simulation_root_state, solution_pieces_order
        while True:
            val = 1
            if len(state.pieces) == 0:
                print('solution found in simulation')
                return ALL_pieces_USED, simulation_root_state, solution_pieces_order
            valid_moves = SolutionChecker.get_valid_next_moves(state, state.pieces )
            if not valid_moves:
                return depth, simulation_root_state, solution_pieces_order

            next_random_tile_index = random.randint(0, len(valid_moves) -1)
            success, new_grid = SolutionChecker.get_next_turn(
                state, valid_moves[next_random_tile_index], val, destroy_state=True)
            self.n_pieces_placed += 1
            solution_pieces_order.append(valid_moves[next_random_tile_index])

            if success == ALL_pieces_USED:
                print('grid is full')
                # no LFB on grid; probably means grid is full
                solution_pieces_order.append(valid_moves[next_random_tile_index])
                return ALL_pieces_USED, simulation_root_state, solution_pieces_order
            elif success == NO_NEXT_POSITION_pieces_UNUSED:
                print('no next position with unused pieces')
                return depth, simulation_root_state, solution_pieces_order
            elif success == TILE_CANNOT_BE_PLACED:
                # cannot place the tile. return depth reached
                return depth, simulation_root_state, solution_pieces_order
            else:
                new_pieces = SolutionChecker.eliminate_pair_pieces(state.pieces, valid_moves[next_random_tile_index])
                new_state = State(grid=new_grid, pieces=new_pieces, parent=state)

                new_state.score = -1  #  because no choice is performed for sequent actions
                state.children.append(new_state)
                state = new_state
            depth += 1
        return depth, simulation_root_state, solution_pieces_order

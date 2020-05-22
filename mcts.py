import math
import json
import numpy as np
import random
import copy
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

    def predict(self, N=1000):
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
                    new_pieces = copy.deepcopy(state.pieces)
                    del new_pieces[piece_index]
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
                    new_state.previous_tile = piece
                    states.append(new_state)
            if not piece_placed:
                # no piece was placed, it's a dead end; end game
                print(state.pieces)
                return initial_state, depth, solution_found

            depth += 1
            best_action = get_max_index(states) 
            prev_state = state
            new_state = states[best_action]
            self.solution_pieces_order.append([new_state.previous_tile, next_position])
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
        print(_max)
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

            new_pieces = copy.deepcopy(state.pieces)
            del new_pieces[next_random_piece_index]
            new_state = State(grid=grid, pieces=new_pieces, parent=state)

            new_state.score = -1  #  because no choice is performed for sequent actions
            state.children.append(new_state)
            state = new_state
            depth += 1
        return depth, simulation_root_state, solution_pieces_order


"""
A minimal implementation of Monte Carlo tree search (MCTS) in Python 3
Luke Harold Miles, July 2019, Public Domain Dedication
See also https://en.wikipedia.org/wiki/Monte_Carlo_tree_search
https://gist.github.com/qpwo/c538c6f73727e254fdc7fab81024f6e1
"""
from abc import ABC, abstractmethod
from collections import defaultdict
import math

def play_game(grid, pieces, is_circular=False):
    tree = MCTS()
    next_position = (0, 0)
    node = Node(grid, pieces, next_position, False, len(pieces), is_circular=is_circular)

    n_rollouts = 100

    n_restarts = 0
    while True:
        if node.is_terminal and len(node.pieces) == 0:
            break
        elif node.is_terminal:
            n_restarts += 1
            # reinitialize initial node
            print('reinitialize search')
            node = Node(grid, pieces, next_position, False, len(pieces), is_circular=is_circular)

        node = tree.choose(node)
        # You can train as you go, or only at the beginning.
        # Here, we train as we go, doing fifty rollouts each turn.
        for _ in range(n_rollouts):
            solution_found = tree.do_rollout(node)
            if solution_found == True:
                return return_solution_node(tree.solution_node)
                print('solution found in simulation')
                return tree.solution_node

        print(min([len(x.pieces) for x in tree.children]))

    print('solution found in search process')
    return return_solution_node(tree.solution_node)

def return_solution_node(node):
    print(f'solution node {node}')
    print(f'solution {node.grid}')
    return node


class MCTS:
    "Monte Carlo tree searcher. First rollout the tree then choose a move."

    def __init__(self, exploration_weight=2.01):
        self.Q = defaultdict(int)  # total reward of each node
        self.N = defaultdict(int)  # total visit count for each node
        self.children = dict()  # children of each node
        self.exploration_weight = exploration_weight

    def choose(self, node):
        "Choose the best successor of node. (Choose a move in the game)"
        if node.is_terminal:
            raise RuntimeError(f"choose called on terminal node {node}")

        if node not in self.children:
            return node.find_random_child()

        def score(n):
            if self.N[n] == 0:
                return float("-inf")  # avoid unseen moves
            return self.Q[n] / self.N[n]  # average reward

        return max(self.children[node], key=score)

    def do_rollout(self, node):
        "Make the tree one layer better. (Train for one iteration.)"
        path = self._select(node)
        leaf = path[-1]
        self._expand(leaf)
        reward = self._simulate(leaf)
        if reward == True:  # solution found
            return True
        self._backpropagate(path, reward)
        return None

    def _select(self, node):
        "Find an unexplored descendent of `node`"
        path = []
        while True:
            path.append(node)
            if node not in self.children or not self.children[node]:
                # node is either unexplored or terminal
                return path
            unexplored = self.children[node] - self.children.keys()
            if unexplored:
                n = unexplored.pop()
                path.append(n)
                return path
            node = self._uct_select(node)  # descend a layer deeper

    def _expand(self, node):
        "Update the `children` dict with the children of `node`"
        if node in self.children:
            return  # already expanded
        self.children[node] = node.find_children()

    def _simulate(self, node):
        "Returns the reward for a random simulation (to completion) of `node`"
        while True:
            if node.is_solution:
                self.solution_node = node
                return True 
            if node.is_terminal:
                reward = node.reward()
                return reward
            node = node.find_random_child()

    def _backpropagate(self, path, reward):
        "Send the reward back up to the ancestors of the leaf"
        for node in reversed(path):
            self.N[node] += 1
            self.Q[node] += reward

    def _uct_select(self, node):
        "Select a child of node, balancing exploration & exploitation"

        # All children of node should already be expanded:
        assert all(n in self.children for n in self.children[node])

        log_N_vertex = math.log(self.N[node])

        def uct(n):
            "Upper confidence bound for trees"
            return self.Q[n] / self.N[n] + self.exploration_weight * math.sqrt(
                log_N_vertex / self.N[n]
            )

        return max(self.children[node], key=uct)


class Node():
    """
    A representation of a single board state.
    MCTS works by constructing a tree of these Nodes.
    Could be e.g. a chess or checkers board state.
    """
    def __init__(self, grid, pieces, next_position, is_terminal, total_n_pieces, is_circular=False, is_solution=False):
        self.grid = grid
        self.pieces = pieces
        self.next_position = next_position
        self.is_terminal = is_terminal
        self.total_n_pieces = total_n_pieces

        self.hash = hash(str(self.grid))
        self.is_circular = is_circular
        self.is_solution = is_solution

    def find_children(self):
        "All possible successors of this grid state"

        valid_moves = get_valid_next_moves(self.grid, self.pieces, self.next_position)
        if not valid_moves:
            return set()

        nodes = set()
        for next_piece in valid_moves:
            node = self.make_move(next_piece)
            nodes.add(node)
        return nodes

    def find_random_child(self):
        "Random successor of this grid state (for more efficient simulation)"
        if self.is_terminal:
            return None

        valid_moves = get_valid_next_moves(self.grid, self.pieces, self.next_position)
        next_piece = random.choice(valid_moves)
        ret = self.make_move(next_piece)
        return ret

    def make_move(self, next_piece):
        piece_to_place = rotate_piece(self.pieces[next_piece[0]], next_piece[1])
        success, grid, next_position = place_piece_on_grid(
            self.grid, piece_to_place, self.next_position, is_circular=self.is_circular
        )
        new_pieces = copy.deepcopy(self.pieces)
        del new_pieces[next_piece[0]]
        is_terminal = False
        new_possible_moves = get_valid_next_moves(grid, new_pieces, next_position)
        if not new_possible_moves:
            is_terminal = True

        if not len(new_pieces):
            is_solution = True
        else:
            is_solution = False

        node = Node(grid, new_pieces, next_position, is_terminal,
                    self.total_n_pieces, self.is_circular,
                    is_solution=is_solution)
        return node

    def reward(self):
        """Assumes `self` is terminal node.
        Reward is number_of_pieces_placed / total_number_of_pieces
        """
        ret =  1 - len(self.pieces) / self.total_n_pieces
        return ret

    def __hash__(self):
        "Nodes must be hashable"
        if not self.hash:
            self.hash = hash(self.grid.tostring())
            return self.hash
        return self.hash

    def __eq__(node1, node2):
        "Nodes must be comparable"
        return node1.hash == node2.hash

    def __repr__(self, short=True):
        if short:
            return f'{self.hash}, ({len(self.pieces)}), {self.is_terminal}, {self.is_solution}'

        output_list = [list(x) for x in self.pieces]
        if len(output_list) > 6:
            output_list = str(output_list[:6]) +  '(...)'

        output_grid = ''
        if len(self.pieces) <= 4:
            output_grid = self.grid
        return f'Remaining pieces: {len(self.pieces)}, pieces: {self.pieces}. {output_grid}'

    def to_pretty_string(self):
        return self.__repr__()

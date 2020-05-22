
from mcts import CustomMCTS, MCTS, play_game

import argparse
from solver import initialize_pieces, initialize_grid, JAVA_FORMAT, NATLO_FORMAT, PYTHON_FORMAT, BENOIST_FORMAT, VANSTONE_FORMAT, pieces_to_editor_format
import math


def main():
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('--sum', dest='accumulate', action='store_const',
                        const=sum, default=max,
                        help='sum the integers (default: find the max)')
    args = parser.parse_args()
    #puzzles_format = JAVA_FORMAT
    #filename = 'editor_puzzles/5x5x4.txt'

    # puzzles_format = BENOIST_FORMAT
    # filename = 'bvanston_puzzles/E2BenchmarkWithoutHints/E_7_1.b6i6.nohint.txt'

    puzzles_format = VANSTONE_FORMAT
    filename = 'puzzles_set/RandPuzzle4x4_01.txt'
    pieces, grid_size = initialize_pieces(
        puzzles_format=puzzles_format, 
        filename=filename
    )   
    n_pieces = len(pieces)
    print(f"pieces length: {n_pieces}")

    if not grid_size:
        grid_size = int(math.sqrt(n_pieces))

    grid = initialize_grid(grid_size, grid_size)
    print(pieces_to_editor_format(pieces))

    # mcts = CustomMCTS(pieces, grid, strategy='avg_depth', is_circular=False)
    # state, depth, solution_found = mcts.predict(N=1000)
    # print(f'\n{solution_found}. Final solution pieces placed: {len(mcts.solution_pieces_order)} Depth: {depth}. Total search pieces placed: {mcts.n_pieces_placed}')
    # print(mcts.solution_pieces_order)

    # print(grid.shape, len(pieces))

    node = play_game(grid, pieces, is_circular=False)
    print(pieces_to_editor_format(node.grid))

if __name__ == '__main__':
    main()

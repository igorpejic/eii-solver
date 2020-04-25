from mcts import CustomMCTS

import argparse
from solver import initialize_pieces, initialize_grid, JAVA_FORMAT, NATLO_FORMAT, PYTHON_FORMAT
import math

def main():
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('--sum', dest='accumulate', action='store_const',
                        const=sum, default=max,
                        help='sum the integers (default: find the max)')
    args = parser.parse_args()
    puzzles_format = JAVA_FORMAT
    filename = 'editor_puzzles/3x3x4.txt'
    pieces = initialize_pieces(
        puzzles_format=puzzles_format, 
        filename=filename
    )   
    n_pieces = len(pieces)

    grid = initialize_grid(int(math.sqrt(n_pieces)), int(math.sqrt(n_pieces)))

    mcts = CustomMCTS(pieces, grid, strategy='max_depth', is_circular=True)
    state, depth, solution_found = mcts.predict(N=5000)
    print(f'\n{solution_found}. Pieces placed: {len(mcts.solution_pieces_order)} Depth: {depth}.')
    print(mcts.solution_pieces_order)

if __name__ == '__main__':
    main()

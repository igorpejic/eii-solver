from mcts import CustomMCTS

import argparse
from solver import initialize_pieces, initialize_grid

def main():
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('--sum', dest='accumulate', action='store_const',
                        const=sum, default=max,
                        help='sum the integers (default: find the max)')
    args = parser.parse_args()
    pieces = initialize_pieces()   
    grid = initialize_grid(2, 2)

    CustomMCTS(pieces, grid).predict()

if __name__ == '__main__':
    main()

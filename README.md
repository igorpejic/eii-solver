# Monte Carlo tree search for Eternity II

## How to compile

### For mcts:

g++ -g solver.cpp main.cpp mcts.cpp -DEMPTY=-1 -DPUZZLE_SIZE=5 && ./a.out e_ii_like_puzzles/RandPuzzle5x5_03.txt

### For UCB mcts:

g++ -g solver.cpp main2.cpp mcts.cpp ucb.cpp backtracker.cpp -DEMPTY=5 -DPUZZLE_SIZE=5 && time ./a.out e_ii_like_puzzles/RandPuzzle5x5_03.txt mcts

### For backtracker (best results)

g++ -g solver.cpp main2.cpp mcts.cpp ucb.cpp backtracker.cpp -DEMPTY=5 -DPUZZLE_SIZE=5 && time ./a.out e_ii_like_puzzles/RandPuzzle5x5_03.txt mcts

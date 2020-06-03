#!/bin/bash

for i in 0 1 2 3 4
do
    for filename in bvanston_puzzles/E2BenchmarkWithoutHints/E_7_*nohint.txt; do
        echo $filename
        g++  -O2  solver.cpp main2.cpp mcts.cpp ucb.cpp  backtracker.cpp -DPUZZLE_SIZE=7  &&  ./a.out $filename
    done

    for filename in bvanston_puzzles/E2BenchmarkWithoutHints/E_8_*nohint.txt; do
        echo $filename
        g++  -O2  solver.cpp main2.cpp mcts.cpp ucb.cpp  backtracker.cpp -DPUZZLE_SIZE=8  &&  ./a.out $filename
    done

    for filename in bvanston_puzzles/E2BenchmarkWithoutHints/E_9_*nohint.txt; do
        echo $filename
        g++  -O2  solver.cpp main2.cpp mcts.cpp ucb.cpp  backtracker.cpp -DPUZZLE_SIZE=9  &&  ./a.out $filename
    done

    for filename in bvanston_puzzles/E2BenchmarkWithoutHints/E_10_*nohint.txt; do
        echo $filename
        g++  -O2  solver.cpp main2.cpp mcts.cpp ucb.cpp  backtracker.cpp -DPUZZLE_SIZE=10  &&  ./a.out $filename
    done
done

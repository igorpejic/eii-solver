#!/bin/bash

for filename in puzzles_set/RandPuzzle*; do
    echo $filename
    ./a.out $filename
done

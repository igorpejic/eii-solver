#!/bin/bash

for filename in puzzles_set2/RandPuzzle*; do
    echo $filename
    ./a.out $filename
done

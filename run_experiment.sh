#!/bin/bash

for filename in generate_puzzles2/RandPuzzle*; do
    echo $filename
    ./a.out $filename
done

E2 Benchmark - Samples Eternity-like puzzles of various sizes
Thierry Benoist

    * E2BenchmarkWithoutHints.zip is the set of instances with no hint at all (only one corner is fixed so as to break solution symetries). 
    * E2BenchmarkWithCornerHints.zip is the same set of instances where all four corners are fixed on all instances. 

File format is the following.

sizeOfTheBoard
numberOfColors 
numberOfHints
row column tileIndex rotation    // for each hint (numberOfHints lines)
topColor rightColor bottomColor leftColor // for each tile (sizeOfBoard*sizeOfBoard lines)


Colors are numbered from 0 (grey) to numberOfColors
tiles are numbered from 1 to sizeOfBoard*sizeOfBoard
rows are numbered from 1 to sizeBoard starting from the top
columns are numbered from 1 to sizeBoard starting from the left
rotation is an integer in [0,3]




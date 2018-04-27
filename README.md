# EightPuzzle Solver
This is an Eight Puzzle Solver, which uses three different algorithms to solve
and will give differing runtimes and space complexities based on each choice.

## Installation and Running
To install, download a zip of the files.
Open a terminal in the directory and run the following commands:
```
make
```
which will create the executable: EightPuzzle.
To run the executable, run the following command:
```
./EightPuzzle
```
## Input
Input will be an eight puzzle, with a 0 representing the blank space. 
The program will ask for the 9 values though standard input.
after that, the program will ask which method you wish to solve it.
The method will determine the runtime and space complexity, where uniforn cost
search is the slowest and most expensive and where A* with Manhattan Distance
Heuristic is the fastest and least expensive. 
##Output
The program's output will be the set of states to reach the completed state.
Each state will have an outputted g(n) and h(n), where g(n) refers to the depth
of the tree, or how many moves into the puzzle it is in. h(n) refers to the 
heuristic value at the given state. Uniform cost search assumes a h(n) of 0,
the misplaced tile heuristic uses an h(n) value of how many tiles are not in a 
the solved state, and the Manhattan Distance heuristc uses a manhattan distance
of each tile to get the h(n).
It will also output the size of tableOfContents, which is a dynamic programming
table for preventing mass duplicates of the same puzzle states from being
created. So, the size will tell how many unique states were created. 
It will also output the highest number of nodes in queue at once as well as 
the depth of the goal node, which is how many moves are necessary. 
If the puzzle is unsolvable, the program will state that it is unsolvable
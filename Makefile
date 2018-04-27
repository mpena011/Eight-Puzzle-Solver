FLAGS = -g -pedantic -ansi

all:
	g++ main.cpp $(FLAGS) -o EightPuzzle

clean: 
	rm -rf EightPuzzle

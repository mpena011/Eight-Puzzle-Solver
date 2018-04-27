#ifndef _PUZZLE_H_
#define _PUZZLE_H_

#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;


//Create a puzzle class which contains a vector of unsigned ints to store
//the puzzle, which will also have additional functions
//Puzzle will be in the form of 
//	a b c
//	d e f
//	g h i
//with the top left corner, a, being the first element,
//c, the top right element, being the third element,
//d, the middle left element, being the fourth element
//continuing this pattern until i being the last element
class Puzzle {
public:
	vector<unsigned> slider;

	Puzzle() {
		slider.resize(9);
		for(unsigned i = 0; i < 9; ++i) {
			slider.at(i) = i + 1;
		}
		slider.at(8) = 0;
	}

	//Function fills the given position with the given value
	void fillslot(unsigned position, unsigned value) {
		slider.at(position) = value;
	}

	int fillPuzzle();

	//Simple display function that displays in the form above
	//Will also replace 0 with * for the blank space
	void display() {
		for(unsigned i = 0; i < 9; ++i) {
			if(slider.at(i) == 0) {
				cout << "* ";
			} else {
				cout << slider.at(i) << " ";
			}
			if(i == 2 || i == 5 || i == 8) {
				cout << endl;
			}
		}
	}

	//Checks if the puzzle is a final state or not
	bool isFinalState() {
		for(unsigned i = 0; i < 8; ++i) {
			if (slider.at(i) != i+1) {
				return false;
			}
		}
		if (slider.at(8) != 0) {
			return false;
		} else {
			return true;
		}
	}

	//Assignment operator overload
	Puzzle operator=(Puzzle other) {
		this->slider = other.slider;
		return *this;
	}

	//Swap two values based on their given positions
	Puzzle swap(unsigned first, unsigned second){
		//cout << "Swapping " << first << " and " << second << endl;
		Puzzle newPuzzle = *this;
		unsigned temp = newPuzzle.slider.at(first);
		newPuzzle.slider.at(first) = newPuzzle.slider.at(second);
		newPuzzle.slider.at(second) = temp;
		//cout << "Successfully swapped" << endl;
		return newPuzzle;
	}


	//Checks if the puzzle is actually solvable
	bool isSolvable() {
		for(unsigned i = 0; i < 9; ++i) {
			int count = 0;
			for(unsigned j = 0; j < 9; ++j) {
				if (slider.at(j) == i) {
					++count;
				}
			}
			if(count != 1) {
				return false;
			}
		}
		return true;
	}

	//Checks if the puzzle is actually unsolvable
	//Unsolvable puzzles are puzzles with two complete rows, with the last row
	//having two values swapped
	//ex.
	//1 2 3
	//4 5 6
	//8 7 0
	//As no moves are able to fix this
	bool CheckForUnsolveness(){
		bool row1 = false;
		bool row2 = false;
		bool row3 = false;
		if (slider.at(0) == 1 && slider.at(1) == 2 && slider.at(2) == 3) {
			row1 = true;
		}
		if (slider.at(3) == 4 && slider.at(4) == 5 && slider.at(5) == 6) {
			row2 = true;
		}
		if (slider.at(6) == 7 && slider.at(7) == 8 && slider.at(8) == 0) {
			row3 = true;
		}
		if (row1 && row2 && !row3) {
			if (slider.at(6) != 7 && slider.at(7) != 8 && slider.at(8) == 0) {
				return true;
			} else if (slider.at(6) != 7 && slider.at(7) == 8 && slider.at(8) != 0) {
				return true;
			}
		} else if (row1 && !row2 && row3) {
			if (slider.at(3) == 4 && slider.at(4) != 5 && slider.at(5) != 6) {
				return true;
			} else if (slider.at(3) != 4 && slider.at(4) != 5 && slider.at(5) == 6) {
				return true;
			} //else if (slider.at(3) != 4 && slider.at(4) == 5 && slider.at(5) != 6) {
			//	return true;
			//}
		} else if (!row1 && row2 && row3) {
			if (slider.at(0) == 1 && slider.at(1) != 2 && slider.at(2) != 3) {
				return true;
			} else if (slider.at(0) != 1 && slider.at(1) != 2 && slider.at(2) == 3) {
				return true;
			} //else if (slider.at(0) != 1 && slider.at(1) == 2 && slider.at(2) != 3) {
			//	return true;
			//}
		} 
		return false;
	}

	//Finds where the blank space is in the puzzle and returns its location
	int blankSpaceLocation() {
		for(unsigned i = 0; i < 9; ++i) {
			if (slider.at(i) == 0) {
				return i;
			}
		}
		return 8;
	}

	//Counts the number of misplaced tiles for the second heuristic
	int misplacedTiles() {
		int count = 0;
		for(unsigned i = 0; i < 8; ++i) {
			if (slider.at(i) != i+1) {
				++count;
			}
		}
		return count;
	}

	//Counts the manhattan distance for the third heuristic
	int ManhattanDistance() {
		int count = 0;
		for(unsigned i = 0; i < 8; ++i) {
			int position;
			int destination = i;
			for(unsigned j = 0; j < 9; ++j) {
				if(i+1 == slider.at(j)) {
					position = j;
				}
			}
			while(position != destination) {
				if (position/3 != destination/3) {
					if (position/3 > destination/3) {
						position -= 3;
						++count;
					} else {
						position += 3;
						++count;
					}
				} else {
					if (position > destination) {
						--position;
						++count;
					} else {
						++position;
						++count;
					}
				}

			}
		}
		return count;
	}

};
	//Code to fill the puzzle when creating a puzzle
	int Puzzle::fillPuzzle() {
		string string1;
		int a = -1;
		cout << "Enter your puzzle values, using 0 to represent the blank space," << endl;
		cout << "pressing enter after every value entered. " << endl;
		cout << "Do not use any spaces or tabs and only enter numeric values 0-8" << endl;
		cout << "Enter from the top left value, moving right, then moving to the next row" << endl;
		cout << "Enter your first value:  ";
		cin >> string1;
		a = atoi(string1.c_str());
		if (a < 0 || a > 8) {
			return -1;
		}
		fillslot(0,a);
		cout << "Enter your second value:  ";
		cin >> string1;
		a = atoi(string1.c_str());
		if (a < 0 || a > 8) {
			return -1;
		}
		fillslot(1,a);
		cout << "Enter your third value:  ";
		cin >> string1;
		a = atoi(string1.c_str());
		if (a < 0 || a > 8) {
			return -1;
		}
		fillslot(2,a);
		cout << "Enter your fourth value:  ";
		cin >> string1;
		a = atoi(string1.c_str());
		if (a < 0 || a > 8) {
			return -1;
		}
		fillslot(3,a);
		cout << "Enter your fifth value:  ";
		cin >> string1;
		a = atoi(string1.c_str());
		if (a < 0 || a > 8) {
			return -1;
		}
		fillslot(4,a);
		cout << "Enter your sixth value:  ";
		cin >> string1;
		a = atoi(string1.c_str());
		if (a < 0 || a > 8) {
			return -1;
		}
		fillslot(5,a);
		cout << "Enter your seventh value:  ";
		cin >> string1;
		a = atoi(string1.c_str());
		if (a < 0 || a > 8) {
			return -1;
		}
		fillslot(6,a);
		cout << "Enter your eighth value:  ";
		cin >> string1;
		a = atoi(string1.c_str());
		if (a < 0 || a > 8) {
			return -1;
		}
		fillslot(7,a);
		cout << "Enter your ninth value:  ";
		cin >> string1;
		a = atoi(string1.c_str());
		if (a < 0 || a > 8) {
			return -1;
		}
		fillslot(8,a);
		cout << "Your puzzle is: " << endl;
		this->display();
		return 0;
	}

#endif
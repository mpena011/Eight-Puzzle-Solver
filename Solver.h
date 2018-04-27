#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "Puzzle.h"
#include <utility>
#include <map>

using namespace std;

int heuristicMethod;

//We will use a tree to solve this problem
//The tree will consist of multiple nodes, which store an instance of a puzzle
//and contain four pointers, each named after which direction where the blank
//space will move. If the pointer is null, it means the blank space cannot 
//move in that direction. Height refers to how far the node is from the root
//where the root will be 0
//prev is a pointer to the parent node, null in root
struct Node {
	Puzzle nodePuzzleData;
	Node* prev;
	Node* left;
	Node* up;
	Node* right;
	Node* down;
	int height;
	int heurisitcValue;
	int cost;
	Node(Puzzle puzzle, int height) {
		nodePuzzleData = puzzle;
		this->height = height;
		if (heuristicMethod == 1) {
			heurisitcValue = 0;
		} else if (heuristicMethod == 2) {
			heurisitcValue = nodePuzzleData.misplacedTiles();
		} else {
			heurisitcValue = nodePuzzleData.ManhattanDistance();
		}
		cost = height + heurisitcValue;
	}
	void setPrev(Node* node) {
		prev = node;
	}
	//Simplifying functions that simply call their Puzzle counterpart
	void display() {
		nodePuzzleData.display();
	}
	bool finalNodeState() {
		return nodePuzzleData.isFinalState();
	}
	int blankSpaceLocation() {
		return nodePuzzleData.blankSpaceLocation();
	}
	Puzzle swap(int first, int second){
		return nodePuzzleData.swap(first,second);
	}
	bool unsolvable() {
		return nodePuzzleData.CheckForUnsolveness();
	}
};

//Here the tree class starts with a root node, which will be our starting point
//This is also the initial state given by the user
//totalHeight is the total height of the tree, while height limit is the limit
//of the tree's height, as we need the tree to stop eventually 
class TreeSolver {
public:
	Node* root;
	int totalHeight;
	multimap<int,Node*> pqueue;
	int numberOfNodes;
	int queueNum;
	vector<vector<unsigned> >tableOfContents;
	//Gets the user input for heuristic picking
	TreeSolver(Puzzle puzzle) {
		heuristicMethod = 0;
		numberOfNodes = 0;
		cout << "Please choose an algorithm: " << endl;
		cout << "1. Uniform Cost Search" << endl;
		cout << "2. A* with Misplaced Tile Heuristic" << endl;
		cout << "3. A* with Manhattan Distance Heuristic" << endl;
		cin >> heuristicMethod;
		while (heuristicMethod < 1 && heuristicMethod > 3) {
			cout << "Error: Please choose a method: 1, 2, or 3" << endl;
			cout << "Please choose an algorithm: " << endl;
			cout << "1. Uniform Cost Search" << endl;
			cout << "2. A* with Misplaced Tile Heuristic" << endl;
			cout << "3. A* with Manhattan Distance Heuristic" << endl;
			cin >> heuristicMethod;
		}
		root = new Node(puzzle, 0);
		pqueue.insert(pair<int,Node*>(root->cost,root));
		++numberOfNodes;
		queueNum = 1;
		totalHeight = 1;
	}

	//Expands the given node into up to four possible other puzzles
	void Expand(Node* node) {
		//This will create a lookup table to see if a node has already been created
		//as this will prevent duplicate nodes from being created to save on time 
		//and greatly on space
		for(unsigned i = 0; i < tableOfContents.size(); ++i) {
			if (tableOfContents.at(i) == node->nodePuzzleData.slider) {
				return;
			}
		}
		tableOfContents.push_back(node->nodePuzzleData.slider);
		int zero = node->blankSpaceLocation();
		//Using the blank space location, we can determine where we can move the 
		//blank space to give us new puzzle nodes which get put into the priority
		//queue for later
		if (node->height+1 > totalHeight) {
			totalHeight = node->height+1;
		}
		if (zero%3 != 0) {
			Node* left = new Node(node->swap(zero,zero-1),node->height+1);
			left->setPrev(node);
			pqueue.insert(pair<int,Node*>(left->cost,left));
			++numberOfNodes;
		}
		if (zero%3 != 2) {
			//cout << "Making right" << endl;
			Node* right = new Node(node->swap(zero, zero+1), node->height+1);
			right->setPrev(node);
			pqueue.insert(pair<int,Node*>(right->cost,right));
			++numberOfNodes;
		}
		if (zero-3 > 0) {
			//cout << "Making up" << endl;
			Node* up = new Node(node->swap(zero, zero-3), node->height+1);
			up->setPrev(node);
			pqueue.insert(pair<int,Node*>(up->cost,up));
			++numberOfNodes;
		}
		if (zero+3 < 9) {
			//cout << "Making down" << endl;
			Node* down = new Node(node->swap(zero,zero+3), node->height+1);
			down->setPrev(node);
			pqueue.insert(pair<int,Node*>(down->cost,down));
			++numberOfNodes;
		}
	}

	//A simple trace algorithm which will trace the node to its solution
	//Only called if it is solvable
	void Trace(Node* node) {
		Node* pointer = node;
		multimap<int,Node*> tracer;
		while(pointer != 0) {
			tracer.insert(pair<int,Node*>(pointer->height, pointer));
			pointer = pointer->prev;
		}
		multimap<int,Node*>::iterator it = tracer.begin();
		cout << "The steps to solve this puzzle using this method is as follows: " << endl;
		cout << "Starting with: " << endl;
		it->second->display();
		++it;
		for(it; it != tracer.end(); ++it) {
			cout << "Expand to: " << endl;
			it->second->display();
			cout << "which has a g(n) of " << it->second->height;
			cout << " and an h(n) of " << it->second->heurisitcValue << endl << endl;
		}
		cout << "Which is the goal state, thus completing the algorithm" << endl;
		cout << "Size of tableOfContents " << tableOfContents.size() << endl;
	}

	//Actual solving algorithm
	//Iterate through the priority queue, checking if the node is the final state node
	//Otherwise itll expand that node and use the next in the queue
	//Also checks is a puzzle is solvable 
	void Solve() {
		multimap<int,Node*>::iterator it = pqueue.begin();
		while(!pqueue.empty() && !it->second->finalNodeState()) {
			Expand(it->second);
			if (totalHeight > 31 || it->second->height > 31 || it->second->unsolvable()) {
				cout << "Solution could not be found: Puzzle is unsolvable" << endl;
				cout << "Puzzle stopped at: " << endl;
				it->second->display();
				cout << "An unsolvable state" << endl;
				return;
			}
			multimap<int,Node*>::iterator deleteThis = it;
			++it;
			pqueue.erase(deleteThis);
			if (pqueue.size() > queueNum) {
				queueNum = pqueue.size();
			}
		}
		//Once it finds a solution does it output this and the rest of requested things
		Node* solution = it->second;
		Trace(solution);
		cout << "The tree expanded a total number of " << numberOfNodes << " nodes" << endl;
		cout << "The highest number of nodes in queue at any one time was " << queueNum << endl;
		cout << "The depth of the goal node was " << solution->height << endl;

	}
};

#endif
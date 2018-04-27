#include "Puzzle.h"
#include "Solver.h"
#include <ctime>

using namespace std;

int main() {
    Puzzle temp;
    while (temp.fillPuzzle() == -1) {
        cout << endl;
        cout << "ERROR: Improper value entered, only enter digits 0-8" << endl;
        cout << "Please Try Again" << endl << endl;
    }
    if (!temp.isSolvable()) {
        cout << "This puzzle is unsolvable due to values entered" << endl;
    }
    TreeSolver solvepls(temp);

    solvepls.Solve();
    /*double duration;
    clock_t start = clock();//
    duration = (clock() - start) /(double)CLOCKS_PER_SEC;

    cout << endl << "Time taken: " << duration << endl;*/

    return 0;
}
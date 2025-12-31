#include <iostream>
#include <ctime>
#include "sudokusolver.h"

using namespace std;


int main(int argc, const char* argv[]) {
	if (argc < 2) {
		cout << "Sudoku solver can be used in two ways." << endl;
		cout << "1. Command line input as a sequential string and output in console." << endl;
		cout << "   Example:" << endl;
		cout << "   Sudoku.exe 005300000800050020070010500400005300010073006003200080060500009004000030000009700" << endl;
		cout << "2. File input/output:" << endl;
		cout << "   Example:" << endl;
		cout << "   Sudoku.exe in00.txt out00.txt" << endl;
		return 0;
	}

	Sudoku::SudokuSolver ssolver;
	if (argc == 2)
	{
		string input(argv[1]);
		ssolver.setPuzzle(input);
		int starttime = clock();
		int numnodes = 0;
		int res = ssolver.solve(numnodes);
		int endtime = clock();
		string ostr;
		ssolver.getSolvedPuzzle(ostr);
		cout << ostr << endl;
		cout << "time taken: " << (endtime - starttime) / double(CLOCKS_PER_SEC) * 1000 << "millisecs" << endl;
		cout << "num nodes explored: " << numnodes << endl;
	}
	else {
		string ifname(argv[1]);
		ssolver.readPuzzleFile(ifname);
		int starttime = clock();
		int numnodes = 0;
		int res = ssolver.solve(numnodes);
		int endtime = clock();
		string ofname(argv[2]);
		if (res == 0)
			ssolver.writePuzzleFile(ofname);
		cout << res << endl;
		cout << "time taken: " << (endtime - starttime) / double(CLOCKS_PER_SEC) * 1000 << "millisecs" << endl;
		cout << "num nodes explored: " << numnodes << endl;
	}
	return 0;
}

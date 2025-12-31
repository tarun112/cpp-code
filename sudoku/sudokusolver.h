#pragma once
#include <string>

#include "sudokugrid.h"

using namespace std;

namespace Sudoku {
class SudokuSolver {
public:
	int readPuzzleFile(string& fname);
	int writePuzzleFile(string& fname);

	int setPuzzle(string& instr);
	int getSolvedPuzzle(string& ostr);

	int solve(int& numnodes);
	int solveRules();

private:
	int solve(int ri, int ci, int& numnodes);

private:
	SudokuGrid m_sgrid;
};
}
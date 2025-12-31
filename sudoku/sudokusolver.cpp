#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_set>

#include "sudokusolver.h"
#include "sudokufileio.h"

using namespace std;

namespace Sudoku {
	const int ZEROINT = static_cast<int>('0');

int SudokuSolver::setPuzzle(string& instr) {
	int val = -1;
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			val = static_cast<int>(instr[i * 9 + j]) - ZEROINT;
			if (val < 0 || val > 9)
				val = 0;
			m_sgrid.put(i, j, val);
		}
	}
	return 0;
}

int SudokuSolver::getSolvedPuzzle(string& ostr) {
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			int val = m_sgrid.get(i, j);
			char chr = static_cast<char>(val + ZEROINT);
			ostr.push_back(chr);
		}
	}
	return 0;
}

int SudokuSolver::readPuzzleFile(string& fname) {
	Sudoku::SudokuFileIO sfread;
	sfread.readFile(fname, m_sgrid);
	return 0;
}

int SudokuSolver::writePuzzleFile(string& fname) {
	SudokuFileIO sfread;
	sfread.writeFile(fname, m_sgrid);
	return 0;
}

int SudokuSolver::solve(int& numnodes) {
	int res = solveRules();
	res = solve(0, 0, numnodes);
	return res;
}

int SudokuSolver::solveRules() {
	bool cflag = true;
	while (cflag) {
		cflag = false;
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				if (!m_sgrid.isEmpty(i, j))
					continue;
				int lastval = 0;
				int count = 0;
				for (int k = 1; k <= 9; ++k) {
					if (m_sgrid.isValid(i, j, k)) {
						lastval = k;
						++count;
					}
				}
				if (count == 1) {
					m_sgrid.put(i, j, lastval);
					cflag = true;
				}
			}
		}
	}
	return 0;
}

int SudokuSolver::solve(int ri, int ci, int& numnodes) {
	if (ci == 9) {
		ci = 0;
		++ri;
	}

	if (ri == 9)
		return 0;
	
	++numnodes;

	int ii = ri;
	int jj = ci;
	while (!m_sgrid.isEmpty(ii, jj)) {
		++jj;
		if (jj == 9) {
			jj = 0;
			++ii;
		}
		if (ii == 9)
			return 0;
	}

	for (int k = 1; k <= 9; ++k) {
		if (m_sgrid.isValid(ii, jj, k)) {
			m_sgrid.put(ii, jj, k);
			int ret = solve(ii, jj + 1, numnodes);
			if (ret == 0)
				return 0;
			m_sgrid.makeEmpty(ii, jj);
		}
	}
	return -1;
}
}
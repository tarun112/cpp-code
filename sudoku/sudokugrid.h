#pragma once
#include <utility>

using namespace std;

namespace Sudoku {

class SudokuGrid {
public:
	SudokuGrid() {
		for (int i = 0; i < 9; ++i)
			for (int j = 0;j < 9; ++j)
				m_grid[i][j] = EMPTYGRID;
		//m_rowset.resize(9);
		//m_colset.resize(9);
		//m_cellset.resize(9);
	}

	inline int put(int row, int col, int val) {
		m_grid[row][col] = val;
		//m_rowset[r].insert(val);
		//m_colset[c].insert(val);
		//m_cellset[getCell(r, c)].insert(val);
		return 0;
	}

	inline int get(int row, int col) const {
		return m_grid[row][col];
	}

	//int SudokuGrid::copyGrid(vector < vector<int>>& cgrid) const {
	//	cgrid.resize(9);
	//	for (int i = 0; i < 9; ++i) {
	//		cgrid[i].resize(9);
	//		for (int j = 0; j < 9; ++j)
	//			cgrid[i][j] = m_grid[i][j];
	//	}
	//	return 0;
	//}

	inline bool isEmpty(int row, int col) const {
		return m_grid[row][col] == EMPTYGRID;
	}

	inline int makeEmpty(int row, int col) {
		//int val = m_grid[row][col];
		m_grid[row][col] = EMPTYGRID;
		//if (val != EMPTYGRID) {
		//	m_rowset[row].erase(val);
		//	m_colset[col].erase(val);
		//	m_cellset[getCell(row,col)].erase(val);
		//}
		return 0;
	}

	//bool isValid(int row, int col, int val) const ;
	bool isValid(int row, int col, int val) const;
private:
	const int EMPTYGRID = 0;

	inline int getCell(int row, int col) const {
		int cell = 3 * (row / 3) + (col / 3);
		return cell;
	}

	inline pair<int, int> getCellIndices0(int row, int col) const {
		int cell = getCell(row, col);
		int cr0 = 3 * (cell / 3);
		int cc0 = 3 * (cell % 3);
		return make_pair(cr0, cc0);
	}

private:
	int m_grid[9][9];
	//vector<unordered_set<int>> m_rowset;
	//vector<unordered_set<int>> m_colset;
	//vector<unordered_set<int>> m_cellset;
};
}
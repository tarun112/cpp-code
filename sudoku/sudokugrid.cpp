
#include "sudokugrid.h"

using namespace std;

namespace Sudoku{


//bool isValid(int row, int col, int val) const {
//	if (m_rowset[row].find(val) != m_rowset[row].end())
//		return false;
//	if (m_colset[col].find(val) != m_colset[col].end())
//		return false;
//	if (m_cellset[getCell(row, col)].find(val) != m_cellset[getCell(row,col)].end())
//		return false;
//	return true;
//}

bool SudokuGrid::isValid(int row, int col, int val) const {
	for (int i = 0; i < 9; ++i) {
		if (m_grid[i][col] == val)
			return false;
		if (m_grid[row][i] == val)
			return false;
	}
	pair<int, int> ci0 = getCellIndices0(row, col);
	int cr0 = ci0.first;
	int cc0 = ci0.second;
	for (int i = cr0; i < cr0 + 3; ++i)
		for (int j = cc0; j < cc0 + 3; ++j)
			if (m_grid[i][j] == val)
				return false;

	return true;
}

}
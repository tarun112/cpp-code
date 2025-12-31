#pragma once

#include <string>
#include "sudokugrid.h"

using namespace std;

namespace Sudoku {
class SudokuFileIO {
public:
	int readFile(string& filename, SudokuGrid& sgrid);
	int writeFile(string fname, const SudokuGrid& csgrid);
};
}
#include <fstream>
#include <iostream>
#include <sstream>
#include "sudokufileio.h"

using namespace std;

namespace Sudoku{
	const int ZEROINT = static_cast<int>('0');
int SudokuFileIO::readFile(string& filename, SudokuGrid& sgrid)
{
	ifstream ifile;
	ifile.open(filename);
	if (!ifile.is_open())
		return -1;

	string str;
	int row = 0;
	int val = 0;
	while (getline(ifile, str)) {
		if (str.empty())
			continue;

		stringstream ss(str);
//		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				ss >> val;
//				val = static_cast<int>(str[i * 9 + j]) - ZEROINT;
//				sgrid.put(i, j, val);
				if (val < 0 || val > 9)
					val = 0;
				sgrid.put(row, j, val);
			}
//		}
		++row;
	}
	ifile.close();
	return 0;
}

int SudokuFileIO::writeFile(string fname, const SudokuGrid& csgrid) {
	ofstream ofile;
	ofile.open(fname);
	if (!ofile.is_open())
		return -1;
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			ofile << csgrid.get(i, j) << " ";
//			ofile << csgrid.get(i, j);
		}
		ofile << endl;
	}
	ofile.close();
	return 0;
}
}
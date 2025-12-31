Sudoku Solver

Sudoku.exe 
built using MS Visual Studio 2015 on Windows 10

Usage:
0. Sudoku.exe
   Prints the Usage
1. Sudoku.exe inputstring
   For example: sudoku.exe 005300000800050020070010500400005300010073006003200080060500009004000030000009700
   Solved puzzle is output in console
2. Sudoku.exe inputfilename outputfilename
   Where inputfilename contains puzzle in grid form and solved puzzle is writen to 
   outputfilename. 
   For example: File sudokuinpot.txt
   Contents:
   0 0 5 3 0 0 0 0 0
   8 0 0 0 5 0 0 2 0 
   0 7 0 0 1 0 5 0 0
   4 0 0 0 0 5 3 0 0 
   0 1 0 0 7 3 0 0 6
   0 0 3 2 0 0 0 8 0
   0 6 0 5 0 0 0 0 9
   0 0 4 0 0 0 0 3 0 
   0 0 0 0 0 9 7 0 0

Source:
-main.cpp
-sudokufileio.h,.cpp
-sudokugrid.h,.cpp
-sudokusolver.h,.cpp


Solver Algorithm
1. Rule checking(constraint propagation): repeatedly checking for gridcells 
    with only one possible value according to constraints.
2. Rest of the puzzle is solved using DFS Tree search with 
    backtracking and constraint checking.
 



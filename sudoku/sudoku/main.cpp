#include "./model/YGSudokuJudge.h"
#include <iostream>

UINT8 GMATRIX[6][7] =
{
{1, 0, 0, 1, 0, 0, 1},
{1, 0, 0, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 0, 1},
{1, 0, 0, 1, 0, 0, 1},
{0, 1, 1, 0, 0, 1, 1},
{0, 1, 1, 0, 1, 1, 0}
};

// 唯一解的数独
UINT8 GSUDOKU[9][9] =
{
{ 8,0,0,0,0,0,0,0,0 },
{ 0,0,3,6,0,0,0,0,0 },
{ 0,7,0,0,9,0,2,0,0 },
{ 0,5,0,0,0,7,0,0,0 },
{ 0,0,0,0,4,5,7,0,0 },
{ 0,0,0,1,0,0,0,3,0 },
{ 0,0,1,0,0,0,0,6,8 },
{ 0,0,8,5,0,0,0,1,0 },
{ 0,9,0,0,0,0,4,0,0 }
};

// 有多个解的数独
//UINT8 GSUDOKU[9][9] =
//{
//{ 1,7,5,8,3,9,4,2,6 },
//{ 0,3,0,2,7,4,9,1,5 },
//{ 4,2,9,6,5,1,3,7,8 },
//{ 0,1,0,3,9,5,7,4,2 },
//{ 5,4,7,1,6,2,8,3,9 },
//{ 2,9,3,4,8,7,6,5,1 },
//{ 7,5,4,9,2,6,1,8,3 },
//{ 9,8,1,5,4,3,2,6,7 },
//{ 3,6,2,7,1,8,5,9,4 }
//};

int main()
{
	// 验证DLX
	Matrix mMatrix(6, std::vector<bool>(7));
	for (UINT32 row = 0; row < 6; ++row)
	{
		for (UINT32 col = 0; col < 7; ++col)
		{
			mMatrix[row][col] = GMATRIX[row][col];
		}
	}

	YGDancingLinks YGDLX(6, 7, mMatrix);
	YGDLX.startDance();

	// 验证Sudoku
	Sudoku sudoku;
	for (UINT8 i = 0; i < 9; ++i)
	{
		for (UINT8 j = 0; j < 9; ++j)
		{
			sudoku._ppLayout[i][j] = GSUDOKU[i][j];
		}
	}
	sudoku._uRow = 9;
	sudoku._uCol = 9;
	YGSudokuJudge ygSudokuJudge(sudoku);
	ygSudokuJudge.printAnswer();

	int a;
	std::cin >> a;

	return 0;
}
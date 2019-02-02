#include "./model/YGSudokuProducer.h"
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

// 可能有多个解的数独
//UINT8 GSUDOKU[9][9] =
//{
//{ 0,0,0,0,0,0,0,0,0 },
//{ 0,0,7,0,0,2,0,0,5 },
//{ 4,6,2,0,0,0,0,0,0 },
//{ 8,0,9,5,0,0,2,3,4 },
//{ 0,1,0,0,8,0,0,0,0 },
//{ 2,0,0,3,0,0,0,0,0 },
//{ 0,0,0,0,0,6,0,1,2 },
//{ 6,0,0,0,0,0,7,0,0 },
//{ 0,2,5,4,7,0,0,0,0 }
//};

int main()
{
	// 验证DLX
	/*Matrix mMatrix(6, std::vector<bool>(7));
	for (UINT32 row = 0; row < 6; ++row)
	{
		for (UINT32 col = 0; col < 7; ++col)
		{
			mMatrix[row][col] = GMATRIX[row][col];
		}
	}

	YGDancingLinks YGDLX(6, 7, mMatrix);
	YGDLX.startDance();*/

	// 验证Sudoku计算者
	printf("--------验证Sudoku计算者--------:\n");
	Sudoku sudoku;
	for (UINT8 i = 0; i < 9; ++i)
	{
		for (UINT8 j = 0; j < 9; ++j)
		{
			sudoku._ppLayout[i][j] = GSUDOKU[i][j] - 1;
		}
	}
	sudoku._uRow = 9;
	sudoku._uCol = 9;
	YGSudokuJudge ygSudokuJudge(sudoku);
	ygSudokuJudge.printAnswer();

	// 验证Sudoku生产者
	printf("--------验证Sudoku生产者--------:\n");
	YGSudokuProducer ygSudokuProducer;
	Sudoku sdQuestion, sdAnswer;
	ygSudokuProducer.getQuestion(sdQuestion);
	ygSudokuProducer.getAnswer(sdAnswer);
	printf("Question:\n");
	sdQuestion.print();
	printf("Answer:\n");
	sdAnswer.print();

	int a;
	std::cin >> a;

	return 0;
}
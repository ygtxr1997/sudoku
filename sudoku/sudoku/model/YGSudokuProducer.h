#pragma once

#include "YGSudokuJudge.h"

// 生成随机的数独终盘并尝试挖洞
// 使用优化过的枚举算法，参见：http://www.afjarvis.staff.shef.ac.uk/sudoku/
class YGSudokuProducer
{
public:
	YGSudokuProducer();
	~YGSudokuProducer();

private:
	Sudoku _sdFinal;	// 数独终盘
	Sudoku _sdStart;	// 数独初盘
};
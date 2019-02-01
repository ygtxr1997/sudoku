#include "../DLX/YGDancingLinks.h"

// 判断一个数独盘面的解是否唯一
class YGSudokuJudge
{
public:
	YGSudokuJudge(Sudoku& sdLayout);
	~YGSudokuJudge();

	void printAnswer() const;
private:
	void convertSudoku2Matrix();
	void fillGridByPossibleWay(UINT8 uRow, UINT8 uCol, UINT8 uValue);
private:
	Sudoku _sdLayout;					// 初始数独盘面
	std::set<std::vector<bool>> _set;	// 用集合缓存初始盘面
	Matrix _mtMatrix;					// 转化后的01矩阵
	PYGDancingLinks _ygDlx;				// DLX

private:
	UINT8 _kRow;
	UINT8 _kCol;
	UINT8 _kGridRow;
	UINT8 _kGridCol;
	UINT8 _kGridNum;
	UINT8 _kRuleNum;
};


#include "YGSudokuJudge.h"

YGSudokuJudge::YGSudokuJudge(Sudoku& sdLayout)
	: _sdLayout(sdLayout),
	_mtMatrix(),
	_ygDlx(nullptr)
{
	_kRow = _sdLayout._uRow;
	_kCol = _sdLayout._uCol;
	_kGridRow = (UINT8)sqrt(_kRow);
	_kGridCol = (UINT8)sqrt(_kCol);
	_kGridNum = _kGridRow * _kGridCol;
	_kRuleNum = 4;

	convertSudoku2Matrix();
	_ygDlx = new YGDancingLinks(_mtMatrix.size(), _mtMatrix[0].size(), _mtMatrix);
	_ygDlx->startDance();
	printf("Dance time : %u", _ygDlx->getDanceTime());
}

YGSudokuJudge::~YGSudokuJudge()
{
	if (!_ygDlx)
	{
		delete _ygDlx;
		_ygDlx = nullptr;
	}
}

// 打印数独解
void YGSudokuJudge::printAnswer() const
{

}

// 数独问题转化为精确覆盖问题
void YGSudokuJudge::convertSudoku2Matrix()
{
	// 根据初始盘面生成缓存集合
	for (UINT8 row = 0; row < _kRow; ++row)
	{
		for (UINT8 col = 0; col < _kRow; ++col)
		{
			// -1代表未填，0~8代表1~9
			UINT8 value = _sdLayout._ppLayout[row][col] - 1;

			if (value != (UINT8)-1)
			{
				// 1.若初始盘面该位置已被填写，则该[row,col]只有一行
				fillGridByPossibleWay(row, col, value);
			}
			else
			{
				// 2.若初始盘面该位置为空，那么该[row,col]可能填入的数字有kCol(9)种情况
				for (UINT8 uPossible = 0; uPossible < _kCol; ++uPossible)
				{
					fillGridByPossibleWay(row, col, uPossible);
				}
			}
		}
	}

	// 根据缓存集合生成01矩阵
	_mtMatrix.resize(_set.size());
	for (UINT32 i = 0; i < _mtMatrix.size(); ++i)
	{
		_mtMatrix[i].resize(_kRow * _kCol * _kRuleNum);
	}

	auto ite = _set.begin();
	for (UINT16 row = 0; row < _mtMatrix.size() && ite != _set.end(); ++row, ++ite)
	{
		for (UINT16 col = 0; col < _mtMatrix[0].size(); ++col)
		{
			_mtMatrix[row][col] = (*ite)[col];
		}
	}
}

void YGSudokuJudge::fillGridByPossibleWay(UINT8 uRow, UINT8 uCol, UINT8 uValue)
{
	// 规则1.共81个格子每个都有数字
	UINT16 rule1 = uRow * _kCol + uCol;

	// 规则2.共9行都恰好包括1~9
	UINT16 rule2 = _kRow * _kCol + uRow * _kCol + uValue;

	// 规则3.共9列都恰好包括1~9
	UINT16 rule3 = _kRow * _kCol * 2 + uCol * _kRow + uValue;

	// 规则4.共9宫都恰好包括1~9
	UINT16 uGrid = (uRow / _kGridRow) * _kGridCol + uCol / _kGridCol;
	UINT16 rule4 = _kRow * _kCol * 3 + uGrid * _kGridNum + uValue;

	// 转化为01矩阵，行代表填写方法，列代表满足的条件
	UINT16 uConvertRow = uValue * _kRow * _kCol + uRow * _kCol + uCol;
	std::vector<bool> vecRow(_kRow * _kCol * _kRuleNum);
	vecRow[rule1] = true;
	vecRow[rule2] = true;
	vecRow[rule3] = true;
	vecRow[rule4] = true;
	_set.insert(vecRow);
}
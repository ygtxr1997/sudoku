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

// ��ӡ������
void YGSudokuJudge::printAnswer() const
{

}

// ��������ת��Ϊ��ȷ��������
void YGSudokuJudge::convertSudoku2Matrix()
{
	// ���ݳ�ʼ�������ɻ��漯��
	for (UINT8 row = 0; row < _kRow; ++row)
	{
		for (UINT8 col = 0; col < _kRow; ++col)
		{
			// -1����δ�0~8����1~9
			UINT8 value = _sdLayout._ppLayout[row][col] - 1;

			if (value != (UINT8)-1)
			{
				// 1.����ʼ�����λ���ѱ���д�����[row,col]ֻ��һ��
				fillGridByPossibleWay(row, col, value);
			}
			else
			{
				// 2.����ʼ�����λ��Ϊ�գ���ô��[row,col]���������������kCol(9)�����
				for (UINT8 uPossible = 0; uPossible < _kCol; ++uPossible)
				{
					fillGridByPossibleWay(row, col, uPossible);
				}
			}
		}
	}

	// ���ݻ��漯������01����
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
	// ����1.��81������ÿ����������
	UINT16 rule1 = uRow * _kCol + uCol;

	// ����2.��9�ж�ǡ�ð���1~9
	UINT16 rule2 = _kRow * _kCol + uRow * _kCol + uValue;

	// ����3.��9�ж�ǡ�ð���1~9
	UINT16 rule3 = _kRow * _kCol * 2 + uCol * _kRow + uValue;

	// ����4.��9����ǡ�ð���1~9
	UINT16 uGrid = (uRow / _kGridRow) * _kGridCol + uCol / _kGridCol;
	UINT16 rule4 = _kRow * _kCol * 3 + uGrid * _kGridNum + uValue;

	// ת��Ϊ01�����д�����д�������д������������
	UINT16 uConvertRow = uValue * _kRow * _kCol + uRow * _kCol + uCol;
	std::vector<bool> vecRow(_kRow * _kCol * _kRuleNum);
	vecRow[rule1] = true;
	vecRow[rule2] = true;
	vecRow[rule3] = true;
	vecRow[rule4] = true;
	_set.insert(vecRow);
}
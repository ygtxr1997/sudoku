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

	clock_t cStart, cEnd;
	cStart = clock();
	convertSudoku2Matrix();
	cEnd = clock();
	printf("ת����ʱ:%u\n", cEnd - cStart);
	cStart = clock();
	_ygDlx = new YGDancingLinks(_mtMatrix.size(), _mtMatrix[0].size(), _mtMatrix);
	cEnd = clock();
	printf("�����ʱ:%u\n", cEnd - cStart);
	cStart = clock();
	_ygDlx->startDance();
	cEnd = clock();
	printf("�赸��ʱ:%u\n", cEnd - cStart);
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
	Sudoku sdResult;
	Vec vAnswer;
	_ygDlx->getAns(vAnswer);

	for (UINT32 row = 0; row < vAnswer.size() && row < _mtMatrix.size(); ++row)
	{
		// ��ȡÿ�ж�Ӧ�Ĺ������
		UINT16 rules[4];
		UINT8 curRule = 0;
		for (UINT32 col = 0; col < _mtMatrix[0].size(); ++col)
		{
			if (_mtMatrix[vAnswer[row]][col] && curRule < 4)
			{
				rules[curRule++] = col;
				col = curRule * _kRow * _kCol - 1;
			}
		}

		// �ɹ����������������������r����c��ֵv
		UINT8 uSudokuR = rules[0] / _kCol;
		UINT8 uSudokuC = rules[0] % _kCol;
		UINT8 uSudokuV = (rules[1] - _kRow * _kCol) - (uSudokuR * _kCol);

		if (uSudokuR < _kRow && uSudokuC < _kCol && uSudokuV < _kRow)
		{
			sdResult._ppLayout[uSudokuR][uSudokuC] = uSudokuV;
		}
	}

	// print
	for (UINT8 i = 0; i < sdResult._uRow; ++i)
	{
		printf("\n");
		if (!(i % 3))
		{
			for (UINT8 k = 0; k < sdResult._uCol; ++k)
				printf("---");
			printf("\n");
		}
		for (UINT8 j = 0; j < sdResult._uCol; ++j)
		{
			if (!(j % 3)) printf("| ");
			printf("%u ", sdResult._ppLayout[i][j] + 1);
		}
	}
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
	_mtMatrix.push_back(vecRow);
}
#include "../DLX/YGDancingLinks.h"

// �ж�һ����������Ľ��Ƿ�Ψһ
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
	Sudoku _sdLayout;					// ��ʼ��������
	std::set<std::vector<bool>> _set;	// �ü��ϻ����ʼ����
	Matrix _mtMatrix;					// ת�����01����
	PYGDancingLinks _ygDlx;				// DLX

private:
	UINT8 _kRow;
	UINT8 _kCol;
	UINT8 _kGridRow;
	UINT8 _kGridCol;
	UINT8 _kGridNum;
	UINT8 _kRuleNum;
};


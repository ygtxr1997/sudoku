#pragma once

#include "YGSudokuJudge.h"

// ����������������̲������ڶ�
// ʹ���Ż�����ö���㷨���μ���http://www.afjarvis.staff.shef.ac.uk/sudoku/
class YGSudokuProducer
{
public:
	YGSudokuProducer();
	~YGSudokuProducer();

private:
	Sudoku _sdFinal;	// ��������
	Sudoku _sdStart;	// ��������
};
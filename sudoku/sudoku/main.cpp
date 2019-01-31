#include "./DLX/YGDancingLinks.h"
#include <iostream>

UINT8 GMATRIX[6][7] =
{
{1, 0, 0, 1, 0, 0, 1},
{1, 0, 0, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 0, 1},
{0, 0, 1, 0, 1, 1, 0},
{0, 1, 1, 0, 0, 1, 1},
{0, 1, 0, 0, 0, 0, 1}
};

int main()
{
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
	Vec answer;
	YGDLX.getAns(answer);

	printf("Answer is : ");
	for (UINT32 i = 0; i < answer.size(); ++i)
	{
		printf("%u ", answer[i]);
	}

	int a;
	std::cin >> a;

	return 0;
}
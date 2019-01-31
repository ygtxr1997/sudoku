#include "YGDancingLinks.h"



YGDancingLinks::YGDancingLinks(UINT32 uRowNum, UINT32 uColNum, Matrix& pMatrix)
	:_pHead(new YGDancingItem(true)),
	_uRowNum(uRowNum),
	_uColNum(uColNum),
	_pMatrix(pMatrix)
{
	initSignedItems();
	initGeneralItems();
}


YGDancingLinks::~YGDancingLinks()
{
}

// 获取第uRow行、第uCol列的元素
PYGDancingItem YGDancingLinks::getItem(UINT32 uRow, UINT32 uCol) const
{
	if (uRow > _uRowNum || uCol > _uColNum)
	{
		TRACE_CMH_2("ERROR: [%d]\n", 1);
		return nullptr;
	}

	PYGDancingItem pSignedItem = getSignedItem(uCol);
	if (pSignedItem == nullptr)
	{
		TRACE_CMH_2("ERROR: [%d]\n", 1);
		return nullptr;
	}

	PYGDancingItem pCurItem = pSignedItem;
	while (pCurItem->_pDown != pSignedItem)
	{
		pCurItem = pCurItem->_pDown;
		if (pCurItem->_uRow == uRow)
		{
			// 找到了指定元素
			return pCurItem;
		}
	}

	return nullptr;
}

// 获取第uCol列的标识元素
PYGDancingItem YGDancingLinks::getSignedItem(UINT32 uCol) const
{
	PYGDancingItem pTargetSignedItem = _pHead;
	for (UINT32 index = 0; index <= uCol; ++index)
	{
		pTargetSignedItem = pTargetSignedItem->_pRight;
		if (pTargetSignedItem->_uCol == uCol)
		{
			return pTargetSignedItem;
		}
	}
	return nullptr;
}

// 获取列中1的个数最少的标识元素
PYGDancingItem YGDancingLinks::getSignedItemMinTrue(UINT32& uMinNum)
{
	if (_pHead == _pHead->_pRight)
	{
		TRACE_CMH_2("ERROR: [%d]\n", 1);
		return nullptr;
	}

	// 记录矩阵每行值为1的个数
	Vec vColTrueNum(_uColNum, -1);
	PYGDancingItem pSignedItem = _pHead->_pRight;

	// 横向遍历第0行的标识元素
	while (pSignedItem != _pHead)
	{
		PYGDancingItem pCurItem = pSignedItem->_pDown;
		vColTrueNum[pCurItem->_uCol] = 0;
		// 纵向遍历
		while (pCurItem != pSignedItem)
		{
			vColTrueNum[pCurItem->_uCol]++;
			pCurItem = pCurItem->_pDown;
		}
		pSignedItem = pSignedItem->_pRight;
	}

	uMinNum = -1;
	UINT32 minIndex = 0;
	for (UINT32 colIndex = 0; colIndex < vColTrueNum.size(); ++colIndex)
	{
		if (vColTrueNum[colIndex] < uMinNum && vColTrueNum[colIndex] != -1)
		{
			uMinNum = vColTrueNum[colIndex];
			minIndex = colIndex;
		}
	}

	return getSignedItem(minIndex);
}

PYGDancingItem YGDancingLinks::getHead() const
{
	return _pHead;
}

void YGDancingLinks::getAns(Vec& vVec) const
{
	vVec = _vAns;
	return;
}

UINT32 YGDancingLinks::getRowNum() const
{
	return _uRowNum;
}

UINT32 YGDancingLinks::getColNum() const
{
	return _uColNum;
}

// 开始运行
INT32 YGDancingLinks::startDance()
{
	if (_pHead->_pLeft == _pHead)
	{
		return 1;
	}

	// 获得标识元素
	UINT32 uMinNum;
	PYGDancingItem pSignedItem = getSignedItemMinTrue(uMinNum);
	if (!pSignedItem)
	{
		TRACE_CMH_2("ERROR: [%d]\n", 1);
		return -1;
	}
	if (!uMinNum)
	{
		return 0;
	}

	// 遍历标识元素S所在列的元素C
	PYGDancingItem pCurColItem = pSignedItem->_pDown;
	while (pCurColItem->_pDown != pSignedItem->_pDown)
	{
		_vAns.push_back(pCurColItem->_uRow);
		//debug
		for (UINT32 i = 0; i < _vAns.size(); ++i)
		{
			printf("-%d", _vAns[i]);
		}
		printf("\n");

		std::set<PYGDancingItem> wipeSet;
		std::vector<PYGDancingItem> signedVec;

		// 抹去元素
		// 1.由该元素C获取所有的标识元素Sn
		PYGDancingItem tmpRowItem = pCurColItem;
		do
		{
			signedVec.push_back(tmpRowItem->_pCol);
			wipeSet.insert(tmpRowItem->_pCol);
			tmpRowItem = tmpRowItem->_pRight;
		} while (tmpRowItem != pCurColItem);

		// 2.遍历Sn，对于每个Si，得到其所有普通元素Cn
		for (UINT32 index = 0; index < signedVec.size(); ++index)
		{
			PYGDancingItem vecSignedItem = signedVec[index];
			PYGDancingItem tmpGeneralItem = vecSignedItem->_pDown;

			// 3.遍历Cn，对于每个Ci，抹去其对应行L
			do
			{
				PYGDancingItem wipeGeneralItem = tmpGeneralItem;

				// 4.抹去一行L，对于行L中的每个Ii，Ii入栈
				do
				{
					wipeSet.insert(wipeGeneralItem);
					wipeGeneralItem = wipeGeneralItem->_pRight;
				} while (wipeGeneralItem != tmpGeneralItem);
				tmpGeneralItem = tmpGeneralItem->_pDown;
			} while (tmpGeneralItem != vecSignedItem);
		}

		for (auto ite = wipeSet.rbegin(); ite != wipeSet.rend(); ++ite)
		{
			wipeItem(*ite);
		}

		// 递归
		if (startDance())
		{
			return 1;
		}

		// 恢复元素
		// 1.逐个出栈，恢复Ii
		for (auto ite = wipeSet.begin(); ite != wipeSet.end(); ++ite)
		{
			PYGDancingItem recoverItem = *ite;
			joinItem(recoverItem);
		}

		_vAns.pop_back();
		pCurColItem = pCurColItem->_pDown;
	}
	return 0;
}

// 暂时抹去该元素的连接关系
INT32 YGDancingLinks::wipeItem(PYGDancingItem pItem)
{
	if (!pItem)
	{
		TRACE_CMH_2("ERROR: [%d]\n", 1);
		return -1;
	}

	PYGDancingItem pLeft = pItem->_pLeft;
	PYGDancingItem pRight = pItem->_pRight;
	PYGDancingItem pUp = pItem->_pUp;
	PYGDancingItem pDown = pItem->_pDown;

	connectItemByRow(pLeft, pRight);
	connectItemByCol(pUp, pDown);
	printf("wipe [%u,%u]\n", pItem->_uRow, pItem->_uCol);
	return 0;
}

// 元素重新加入至连接关系
INT32 YGDancingLinks::joinItem(PYGDancingItem pItem)
{
	if (!pItem)
	{
		TRACE_CMH_2("ERROR: [%d]\n", 1);
		return -1;
	}

	PYGDancingItem pLeft = pItem->_pLeft;
	PYGDancingItem pRight = pItem->_pRight;
	PYGDancingItem pUp = pItem->_pUp;
	PYGDancingItem pDown = pItem->_pDown;

	connectItemByRow(pLeft, pItem);
	connectItemByRow(pItem, pRight);
	connectItemByCol(pUp, pItem);
	connectItemByCol(pItem, pDown);
	printf("join [%u,%u]\n", pItem->_uRow, pItem->_uCol);
	return 0;
}

// 初始化第0行的标识元素，不包括_pHead
INT32 YGDancingLinks::initSignedItems()
{
	PYGDancingItem pCurItem = _pHead;
	for (UINT32 uCol = 0; uCol < _uColNum; ++uCol)
	{
		PYGDancingItem pSignItem = new YGDancingItem(true);
		pSignItem->_uCol = uCol;
		pSignItem->_uRow = -1;
		connectItemByRow(pCurItem, pSignItem);
		pCurItem = pSignItem;
	}
	connectItemByRow(pCurItem, _pHead);
	return 0;
}

// 初始化由矩阵生成的非标识普通元素
INT32 YGDancingLinks::initGeneralItems()
{
	if (_uRowNum > _pMatrix.size())
	{
		TRACE_CMH_2("ERROR: [%d]\n", 1);
		return -1;
	}

	if (_uColNum > _pMatrix[0].size())
	{
		TRACE_CMH_2("ERROR: [%d]\n", 1);
		return -1;
	}

	for (UINT32 row = 0; row < _uRowNum; ++row)
	{
		PYGDancingItem pCurRowLastItem = nullptr;
		PYGDancingItem pCurRowFirstItem = nullptr;
		for (UINT32 col = 0; col < _uColNum; ++col)
		{
			// 矩阵为1代表有元素
			if (!_pMatrix[row][col])
			{
				continue;
			}

			PYGDancingItem pGeneralItem = new YGDancingItem(false);
			pGeneralItem->_uRow = row;
			pGeneralItem->_uCol = col;

			PYGDancingItem pCurColSignedItem = getSignedItem(col);
			PYGDancingItem pCurColLastItem = pCurColSignedItem;
			if (!pCurColLastItem)
			{
				TRACE_CMH_2("ERROR: [%d]\n", 1);
				return -1;
			}
			pGeneralItem->_pCol = pCurColSignedItem;
			while (pCurColLastItem->_pDown != pCurColSignedItem)
				pCurColLastItem = pCurColLastItem->_pDown;

			connectItemByCol(pCurColLastItem, pGeneralItem);
			connectItemByCol(pGeneralItem, pCurColSignedItem);

			if (pCurRowLastItem)
			{
				connectItemByRow(pCurRowLastItem, pGeneralItem);
			}
			else
			{
				pCurRowFirstItem = pGeneralItem;
			}
			pCurRowLastItem = pGeneralItem;
		}
		connectItemByRow(pCurRowLastItem, pCurRowFirstItem);
	}

	return 0;
}

INT32 YGDancingLinks::connectItemByRow(PYGDancingItem pLeftItem, PYGDancingItem pRightItem)
{
	if (pLeftItem == nullptr || pRightItem == nullptr)
	{
		TRACE_CMH_2("ERROR: [%d]\n", 1);
		return -1;
	}

	pLeftItem->_pRight = pRightItem;
	pRightItem->_pLeft = pLeftItem;
	return 0;
}

INT32 YGDancingLinks::connectItemByCol(PYGDancingItem pUpItem, PYGDancingItem pDownItem)
{
	if (pUpItem == nullptr || pDownItem == nullptr)
	{
		TRACE_CMH_2("ERROR: [%d]\n", 1);
		return -1;
	}

	pUpItem->_pDown = pDownItem;
	pDownItem->_pUp = pUpItem;
	return 0;
}
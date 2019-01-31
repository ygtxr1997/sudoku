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

// ��ȡ��uRow�С���uCol�е�Ԫ��
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
			// �ҵ���ָ��Ԫ��
			return pCurItem;
		}
	}

	return nullptr;
}

// ��ȡ��uCol�еı�ʶԪ��
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

// ��ȡ����1�ĸ������ٵı�ʶԪ��
PYGDancingItem YGDancingLinks::getSignedItemMinTrue(UINT32& uMinNum)
{
	if (_pHead == _pHead->_pRight)
	{
		TRACE_CMH_2("ERROR: [%d]\n", 1);
		return nullptr;
	}

	// ��¼����ÿ��ֵΪ1�ĸ���
	Vec vColTrueNum(_uColNum, -1);
	PYGDancingItem pSignedItem = _pHead->_pRight;

	// ���������0�еı�ʶԪ��
	while (pSignedItem != _pHead)
	{
		PYGDancingItem pCurItem = pSignedItem->_pDown;
		vColTrueNum[pCurItem->_uCol] = 0;
		// �������
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

// ��ʼ����
INT32 YGDancingLinks::startDance()
{
	if (_pHead->_pLeft == _pHead)
	{
		return 1;
	}

	// ��ñ�ʶԪ��
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

	// ������ʶԪ��S�����е�Ԫ��C
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

		// ĨȥԪ��
		// 1.�ɸ�Ԫ��C��ȡ���еı�ʶԪ��Sn
		PYGDancingItem tmpRowItem = pCurColItem;
		do
		{
			signedVec.push_back(tmpRowItem->_pCol);
			wipeSet.insert(tmpRowItem->_pCol);
			tmpRowItem = tmpRowItem->_pRight;
		} while (tmpRowItem != pCurColItem);

		// 2.����Sn������ÿ��Si���õ���������ͨԪ��Cn
		for (UINT32 index = 0; index < signedVec.size(); ++index)
		{
			PYGDancingItem vecSignedItem = signedVec[index];
			PYGDancingItem tmpGeneralItem = vecSignedItem->_pDown;

			// 3.����Cn������ÿ��Ci��Ĩȥ���Ӧ��L
			do
			{
				PYGDancingItem wipeGeneralItem = tmpGeneralItem;

				// 4.Ĩȥһ��L��������L�е�ÿ��Ii��Ii��ջ
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

		// �ݹ�
		if (startDance())
		{
			return 1;
		}

		// �ָ�Ԫ��
		// 1.�����ջ���ָ�Ii
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

// ��ʱĨȥ��Ԫ�ص����ӹ�ϵ
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

// Ԫ�����¼��������ӹ�ϵ
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

// ��ʼ����0�еı�ʶԪ�أ�������_pHead
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

// ��ʼ���ɾ������ɵķǱ�ʶ��ͨԪ��
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
			// ����Ϊ1������Ԫ��
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
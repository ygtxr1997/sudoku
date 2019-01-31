#pragma once

#include "../define.h"

// �赸���Ĵ洢Ԫ�ض���
typedef struct YGDancingItem YGDancingItem, *PYGDancingItem;
struct YGDancingItem
{
	YGDancingItem(bool bSigned)
		: _pLeft(this),
		_pRight(this),
		_pUp(this),
		_pDown(this),
		_pCol(nullptr),
		_uRow(0),
		_uCol(0),
		_bSigned(bSigned)
	{
	}
	~YGDancingItem() {};

	PYGDancingItem		_pLeft;
	PYGDancingItem		_pRight;
	PYGDancingItem		_pUp;
	PYGDancingItem		_pDown;
	PYGDancingItem		_pCol;
	UINT32				_uRow;
	UINT32				_uCol;
	bool				_bSigned;
};

// �赸����
class YGDancingLinks
{
public:
	YGDancingLinks(UINT32 uRowNum, UINT32 uColNum, Matrix& pMatrix);
	~YGDancingLinks();
public:
	PYGDancingItem getItem(UINT32 uRow, UINT32 uCol) const;
	PYGDancingItem getSignedItem(UINT32 uCol) const;
	PYGDancingItem getSignedItemMinTrue(UINT32& uMinNum);
	PYGDancingItem getHead() const;
	void getAns(Vec& vVec) const;
	UINT32 getRowNum() const;
	UINT32 getColNum() const;
public:
	INT32 startDance();
	INT32 wipeItem(PYGDancingItem pItem);
	INT32 joinItem(PYGDancingItem pItem);
private:
	INT32 initSignedItems();
	INT32 initGeneralItems();
	inline INT32 connectItemByRow(PYGDancingItem pLeftItem, PYGDancingItem pRightItem);
	inline INT32 connectItemByCol(PYGDancingItem pUpItem, PYGDancingItem pDownItem);
private:
	// DLX���ݽṹ��صı���
	PYGDancingItem	_pHead;
	Vec				_vAns;

	// ԭʼ������صı���
	UINT32			_uRowNum;
	UINT32			_uColNum;
	Matrix			_pMatrix;
};


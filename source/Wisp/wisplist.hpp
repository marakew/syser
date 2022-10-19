
#ifndef _WISPLIST_HPP_
#define _WISPLIST_HPP_

#include "../Code/define.h"
#include "../Code/tstrmem.hpp"
#include "../Code/alt/altlist.hpp"
#include "wispwnd.hpp"
#include "wisptipwnd.hpp"
#include "wispedit.hpp"

//uStatus
#define LIST_EXPAND		0x01
#define LIST_CHECKED		0x02
#define LIST_SEMICHECKED	0x04
#define LIST_SELECT		0x08
#define LIST_COLOR		0x10

#define COL_EXPAND	0x1001
#define COL_CHECK	0x1002
#define COL_LAYOUT	0x1003
#define COL_EMPTY	0x1004

#define WHI_EDIT 1
#define WHI_FIXED 2

struct CListStringItem : public TList<CListStringItem>
{
	struct CONTENT
	{
		TWideString String;
		CWispDIB *pDIB; //???
		unsigned long long Number;
		ULONGLONG v1;
		ULONGLONG v2;
		unsigned long Color;
		CWispDIB *Icon; //???
	};

	TList<CONTENT> Contents;
	CListStringItem *pParentItem;
	unsigned int uStatus;
	unsigned long Color;
	int Checked;
	int SemiChecked;

	int GetItemLevel();
	int GetChildItemExpandCount();
	bool ItemHasChildren(CListStringItem *Item);
};

struct CWispList : public CWispWnd
{
	struct stWispHeaderItem
	{
		TWideString strHeading;
		CWispDIB *pDIB;
		int nRealWidth;
		int nInitWidth;
		unsigned int nFormat;
		WISP_RECT RCDIB;
		WISP_RECT RCRECT;
		WISP_RECT RCDRAG;
		//int;
		//int;
	};

	CWispTipWnd m_TipWnd;

	int m_nCtrlCol;
	int m_CharMiddleWidth;
	int m_nColumnWidth;
	int m_nMaxLayoutHeight;
	int m_nLayoutHeight;

	int m_nEditCol;
	CListStringItem *m_pEditItem;
	CWispEdit m_EditWnd;

	stWispHeaderItem *m_pDragColumn;
	TListIter<CListStringItem> m_RootIter;
	TListIter<CListStringItem> m_FocusedIter;
	int m_nLButtonCol;
	WISP_POINT m_PrevMousePos;

	CListStringItem *m_pTipItem;
	int m_nTipCol;

	int m_nSpaceWidth;
	int m_nCtrlBoxWidth;
	int m_nLineHeight;
	WISP_RECT m_LayoutRect;
	WISP_RECT m_ColumnRect;
	TList<stWispHeaderItem> m_Columns;
	CListStringItem m_RootItem;
	int m_nVSBPage; //m_nItemCount//m_nLinesPerPage
	int m_nNonTextWidth;

	unsigned long m_BKColor;
	unsigned long m_ItemColor;
	unsigned long _____________m_Color_field_CD4;
	unsigned long m_ColorFocus;
	unsigned long m_ColorUnFocus;
	unsigned long _____________m_Color_field_CE0;
	unsigned long m_SpaceColor;

	bool m_bEnableEdit;
	bool m_bEditDateUpdate;

	bool m_bTipShow;
	int m_nTipPeriod;

	CWispList();
	virtual ~CWispList();

	virtual bool Destroy() override;

	bool Recalc(bool bMin);
	virtual void RecalcLayout() override;
	virtual bool InitWnd() override;
	virtual void Hide() override;
	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnUpdate(WISP_MSG *pMsg);

	bool OnSize(WISP_MSG *pMsg);
	bool OnScrollEvent(WISP_MSG *pMsg);

	bool OnTimer(WISP_MSG *pMsg);
	bool OnLostFocus(WISP_MSG *pMsg);
	bool OnDragBegin(WISP_MSG *pMsg);
	bool OnDragMove(WISP_MSG *pMsg);
	bool OnDragEnd(WISP_MSG *pMsg);
	bool OnUpdateClient(WISP_MSG *pMsg);
	bool OnMouseLeave(WISP_MSG *pMsg);
	bool OnCmdEdit(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnMouseMove(WISP_MSG *pMsg);
	bool OnCreating(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);

	//0x80000203
	virtual void OnLButton(CListStringItem *Item, int nCol); //0x38
	//0x80000204
	virtual void OnRButton(CListStringItem *Item, int nCol); //+3C

	//0x80000122
	virtual void OnRButtonLayout(int nCol); //+40
	//0x80000121
	virtual void OnLButtonLayout(int nCol); //+44

//DBLCLICK

	virtual void OnMouseDblClick(CListStringItem *Item, int nCol); //+48
	bool OnMouseDblClick(WISP_MSG *pMsg);

//TIP
	bool HideTip();
	bool ShowTip(CListStringItem *Item, int nCol, const WCHAR *Text);
	bool CheckShowTip(CListStringItem *Item, int nCol);
	virtual bool GetTipText(CListStringItem *Item, int nCol, int *pPosX, int *pPosY, WCHAR *pOutText); //+4C
	virtual bool OnHideTip(CListStringItem *Item, int nCol, const WCHAR *Text); //+50

//EDIT
	void EditChange(bool bEnable);
	virtual bool Edit(CListStringItem *Item, int nCol, TWideString& String); //Content? 0x80000111 //+54
	void EditItem(CListStringItem *Item, int nCol);
	virtual bool OnEditing(CListStringItem *Item, int nCol, TWideString& String); //80000113,80000112 //+58
	virtual void OnEdit(CListStringItem *Item, int nCol, TWideString& String); //0x8000010E //+5C
	void EditUpdate();


	virtual bool OnExpanding(CListStringItem *Item); //+60
	virtual void OnExpand(CListStringItem *Item); //+64
	bool Expand(CListStringItem *Item);

	virtual bool OnCollapsing(CListStringItem *Item); //+68
	virtual void OnCollapse(CListStringItem *Item); //+6C
	bool Collapse(CListStringItem *Item);

	bool Toggle(CListStringItem *Item);

	virtual bool OnSelecting(CListStringItem *Item); //+70
	virtual void OnSelect(CListStringItem *Item); //+74
	bool Select(CListStringItem *Item);

	virtual bool OnUnselecting(CListStringItem *Item); //+78
	virtual void OnUnselect(CListStringItem *Item); //+7C
	bool Unselect(CListStringItem *Item);


	virtual bool OnChecking(CListStringItem *Item); //0x80
	virtual void OnChecked(CListStringItem *Item); //0x84
	bool Checked(CListStringItem *Item, bool bWalk);

	virtual bool OnUnchecking(CListStringItem *Item); //0x88
	virtual void OnUnchecked(CListStringItem *Item); //0x8C
	bool Unchecked(CListStringItem *Item, bool bWalk);

	bool ToggleChecked(CListStringItem *Item, char bWalk);

	virtual unsigned int GetContent(CListStringItem *Item, int nCol, WCHAR *pOutStr, unsigned int OutLen, const WCHAR *pStr); //+90

	virtual void DrawContent(int nCol, CListStringItem::CONTENT *Content, WISP_RECT *pRect); //+94
	virtual void DrawHeader(stWispHeaderItem *Item, WISP_RECT *pRect); //+98

	void DrawItem(int nCol, TListIter<CListStringItem> & It);

	bool PrevOpen(TListIter<CListStringItem> & It);
	bool NextOpen(TListIter<CListStringItem> & It);
	int ScrollPage(int Delta);

	int Point2Iter(WISP_POINT *pPoint, TListIter<CListStringItem> & It);
	CListStringItem *Point2Index(WISP_POINT *pPoint, int *pIndex);

	CListStringItem *FindItem(CListStringItem *Item, unsigned int uStatus);
	int GetItemCount(CListStringItem *Item, bool Sub);
	CListStringItem *GetItem(int Pos, CListStringItem *Item);
	CListStringItem *GetRootItem();
	bool Next(TListIter<CListStringItem>& It);
	bool Prev(TListIter<CListStringItem>& It);
	bool NextSibling(TListIter<CListStringItem>& It);
	CListStringItem *FullFindItem(CListStringItem *Item, unsigned int uStatus);
	//GetNextSiblingItem
	CListStringItem *GetNext(CListStringItem *Item, unsigned int uStatus);
	//GetPrevSiblingItem
	CListStringItem *GetPrev(CListStringItem *Item, unsigned int uStatus);
	int GetPosition(CListStringItem *Item);
	CListStringItem *GetItemParent(CListStringItem *Item);
	int GetItemLevel(CListStringItem *Item);
	int GetParentLevel(CListStringItem *Item);

	stWispHeaderItem *InsertColumn(const WCHAR *lpszHeading, int nWidth, int nFormat, CWispDIB *pDIB, int nCol);
	CListStringItem::CONTENT *GetItemContent(CListStringItem *Item, int nCol);
	
	void WalkChecked(CListStringItem *Item);

	void UpdateItemContentColor(CListStringItem *Item);
	void ClearItemContent(CListStringItem *Item);
	int UpdateItemColor(CListStringItem *Item);
	int ClearItem(CListStringItem *Item);
	void Scroll(CListStringItem *Item, int Count);
	bool CheckItemPage(CListStringItem *Item);
	int GetItemIndex(CListStringItem *Item);
	bool GetItemRect(CListStringItem *Item, int nCol, WISP_RECT *pRect);
	int GetItemText(CListStringItem *Item, int nCol, WCHAR *Des, int Len);
	unsigned long long GetItemValue(CListStringItem *Item, int nCol);

	bool SetItemText(CListStringItem *Item, int nCol, const WCHAR *lpszString);
	bool SetItemTextColor(CListStringItem *Item, int nCol, const unsigned long *lpColorString);
	bool SetItemValue(CListStringItem *Item, int nCol, unsigned long long Value);
	bool SetItemDIB(CListStringItem *Item, int nCol, CWispDIB *pDIB);
	bool SetItemColor(CListStringItem *Item, int nCol, unsigned long Color);
	bool SetItemText(CListStringItem *Item, int nCol, const char *Text);
	bool SetItemFormat(CListStringItem *Item, int nCol, unsigned long long Value, const WCHAR *Format);
	
	bool DeleteItem(CListStringItem *Item);
	bool DeleteAllChildrenItems(CListStringItem *Item);

	CListStringItem *InsertItem(const WCHAR *pString, CListStringItem *pParentItem, unsigned int uStatus, CWispDIB *pDIB, int Pos);

	CListStringItem *InsertItemStr(const char *pString, CListStringItem *pParentItem, unsigned int uStatus, CWispDIB *pDIB, int Pos);
	CListStringItem *InsertItemStr(const WCHAR *pString, CListStringItem *pParentItem, unsigned int uStatus, CWispDIB *pDIB, int Pos);
	CListStringItem *InsertItemValue(unsigned long long Value, CListStringItem *pParentItem, const WCHAR *Format, unsigned int uStatus, CWispDIB *pDIB, int Pos);

	int Adjust(CListStringItem *Item, int Pos);
};

#endif

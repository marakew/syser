
#ifndef _WISPTABWND_HPP_
#define _WISPTABWND_HPP_

#include "../Code/define.h"
#include "../Code/alt/altlist.hpp"
#include "wispwnd.hpp"
#include "wispdib.hpp"

	struct CWispButton;

struct WISP_TAB_WND_ITEM
{
	CWispWnd *pWnd;
	WCHAR Name[32];
	WISP_RECT TabRC;
	CWispDIB *pDIB;
	CWispButton *pButton;
};

struct CWispTabWnd : public CWispWnd
{
	CWispTabWnd();
	virtual ~CWispTabWnd();

	TList<WISP_TAB_WND_ITEM> m_WndList;
	CWispWnd *m_pActiveWnd;
	int m_ActiveWndIndex;
	int m_nHoverIndex;
	int m_TabHeight;
	int m_TabSpaceHeight;
	int m_TextSpaceWidth;
	WISP_RECT m_Rect;
	int m_FixedTabWidth;
	int m_FullWidth;
	WISP_SIZE m_ButtonSize;
	CWispDIBList *m_MiniCloseDIBList;
	unsigned long m_TextColor;

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnEvent(WISP_MSG *pMsg);
	bool OnEventButtonClose(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnUpdateClient(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnSize(WISP_MSG *pMsg);
	bool OnRecalcLayout(WISP_MSG *pMsg);
	bool OnGetFocus(WISP_MSG *pMsg);
	bool OnMouseMove(WISP_MSG *pMsg);
	bool OnMouseLeave(WISP_MSG *pMsg);
	virtual bool InitWnd() override;
	void DrawTabTop();
	void DrawTabButtom();
	void DrawTabLeft();
	void DrawTabRight();

	void ResizeTabTop();
	void ResizeTabButtom();
	void ResizeTabLeft();
	void ResizeTabRight();

	unsigned int InsertWnd(const WCHAR *Name, CWispWnd *pWnd, CWispDIB *pDIB);
	unsigned int InsertWndAfter(int Index, const WCHAR *Name, CWispWnd *pWnd, CWispDIB *pDIB);
	unsigned int InsertWndBefore(int Index, const WCHAR *Name, CWispWnd *pWnd, CWispDIB *pDIB);
	unsigned int RemoveWnd(CWispWnd *pWnd, bool bDestroy);
	unsigned int RemoveWnd(int Index, bool bDestroy);
	CWispWnd *GetActiveWnd();
	unsigned int GetActiveWndIndex();
	CWispWnd *SetActiveWnd(unsigned int Index);
	bool SetTabText(unsigned int Index, const WCHAR *pStr);
	void ChangeActiveWnd(CWispWnd *pActiveWindow);
	void ActiveNextPage();
	void ActivePrevPage();
	unsigned int GetWndCount();

	CWispWnd *GetWnd(int Index);
	CWispButton *GetButton(int Index);

	//+0x38
	virtual void InsertTab(WISP_TAB_WND_ITEM *pTabItem, const WCHAR *pString, CWispWnd *pWnd, CWispDIB *pDIB);
	//+0x3C
	virtual void RemoveTab(WISP_TAB_WND_ITEM *pTabItem, bool bDestroy);

	//+0x40
	virtual bool OnRemoveTab(CWispWnd *pWnd) { return true; }
	//+0x44
	virtual void OnRemoved() { }
	//+0x48
	virtual void OnActiveWnd(CWispWnd *pWnd) { }
	
	void RemoveAllTab(bool bDestory);
};


#endif
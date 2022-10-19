
#ifndef _WISPTOOLBAR_HPP_
#define _WISPTOOLBAR_HPP_

#include "../Code/define.h"
#include "../Code/alt/altlist.hpp"
#include "wispwnd.hpp"
#include "wispdib.hpp"

	struct CWispButton;
	struct CWispEdit;
	struct CWispStaticDIB;
	struct CWispStaticStr;

#define WISP_TOOLBAR_SEPARATOR 0x10000
#define WISP_TOOLBAR_ALIGN 0x20000
#define WISP_TOOLBAR_BTN 0
#define WISP_TOOLBAR_EDIT 1
#define WISP_TOOLBAR_DIB 2
#define WISP_TOOLBAR_STR 4


struct WISP_TOOLBAR_RES_ITEM
{
	unsigned int CmdID;
	unsigned int DIBIndex;
	unsigned int Type;
	const WCHAR *pString;
	unsigned int Width;
	unsigned int Style;
};

struct WISP_TOOLBAR_ITEM
{
	union {
		CWispBaseWnd *pItem;
		CWispButton *pButton;
		CWispEdit *pEdit;
		CWispStaticDIB *pStaticDIB;
		CWispStaticStr *pStaticStr;
	};
	CWispDIB *pDIB;
	unsigned int Type;
};

struct CWispToolbar : public CWispWnd
{
	CWispToolbar();
	virtual ~CWispToolbar();

	TList<WISP_TOOLBAR_ITEM> m_BTList;
	CWispDIBList *m_pDIBList;
	int m_Margin;
	int m_SeparatorWidth;

	virtual void SetOwner(CWispBaseWnd *pOwnerWnd) override;
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnUpdateClient(WISP_MSG *pMsg);
	bool OnRecalcLayout(WISP_MSG *pMsg);
	bool OnResizeChild(WISP_MSG *pMsg);
	bool CreateEx(CWispWnd *pParentWnd, int y, int cy, unsigned int CmdID, unsigned int Style);
	CWispBaseWnd *GetItem(unsigned int CmdID);
	bool LoadToolbar(CWispDIBList *pDIBList, WISP_TOOLBAR_RES_ITEM *pResItem);
	bool InsertButton(int Pos, unsigned int Type, unsigned int CmdID, unsigned int Style, CWispDIB *pDIB, const WCHAR *HelpString);
	bool InsertEdit(int Pos, unsigned int Type, int Width, unsigned int CmdID, unsigned int Style, const WCHAR *HelpString);
	bool InsertStaticDIB(int Pos, unsigned int Type, unsigned int CmdID, CWispDIB *pDIB);
	bool InsertStaticStr(int Pos, unsigned int Type, unsigned int CmdID, const WCHAR *HelpString);
	bool InsertSeparator(int Pos, unsigned int Type);
	bool RemoveItem(int Pos);
	bool Enable(unsigned int nID, bool bEnable);
};

#endif
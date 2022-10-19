
#ifndef _WISPBUTTON_HPP_
#define _WISPBUTTON_HPP_

#include "../Code/define.h"
#include "wispbasewnd.hpp"
#include "wispdrawobj.hpp"

struct CWispWnd; //???

struct CWispButton : public CWispBaseWnd
{
	CWispButton();
	virtual ~CWispButton();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	virtual void RecalcLayout() override;

	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnMouseMove(WISP_MSG *pMsg);
	bool OnMouseLeave(WISP_MSG *pMsg);
	bool OnMove(WISP_MSG *pMsg);
	bool OnUpdate(WISP_MSG *pMsg);

	virtual bool InitWnd() override;

	virtual void OnButtonEnter(); // {} Press
	virtual void OnButtonLeave(); // {} Release

	bool m_bTextRect;
	WISP_RECT m_TextRect;

	bool m_bPressed;
	bool m_bMouseInBTRect;

	CWispDIB *m_DIBList[WISP_BT_DIB_COUNT];

	bool CreateEx(CWispDIBList *pDIBList, int x, int y, CWispWnd *pParentWnd, unsigned int CmdID, unsigned int Style);
	bool CreateEx(CWispDIB *pDIB, int x, int y, CWispWnd *pParentWnd, unsigned int CmdID, unsigned int Style);
	bool SetDIB(CWispDIB *pDIB, WISP_BT_DIB DIB);
	bool SetDIBList(CWispDIBList *pDIBList);
};

#endif
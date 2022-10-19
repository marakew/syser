
#ifndef _WISPSTATIC_HPP_
#define _WISPSTATIC_HPP_

#include "../Code/define.h"
#include "wispbasewnd.hpp"
#include "wispdib.hpp"

struct CWispStaticStr : public CWispBaseWnd
{
	CWispStaticStr();
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnUpdate(WISP_MSG *pMsg);
	virtual ~CWispStaticStr();
};

struct CWispStaticDIB : public CWispBaseWnd
{
	CWispDIB *m_pDIB;

	CWispStaticDIB();
	virtual ~CWispStaticDIB();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool CreateEx(CWispDIB *pDIB, int x, int y, CWispBaseWnd *pParentWnd, unsigned int CmdID);
	bool OnUpdate(WISP_MSG *pMsg);
};

struct CWispStaticGroup : public CWispBaseWnd
{
	CWispStaticGroup();
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnUpdate(WISP_MSG *pMsg);
	bool OnGetFocus(WISP_MSG *pMsg);
	virtual ~CWispStaticGroup();
};

struct CWispStaticURL : public CWispBaseWnd
{
	bool m_MouseEnter;

	CWispStaticURL();
	virtual ~CWispStaticURL();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnUpdate(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnMouseEnter(WISP_MSG *pMsg);
	bool OnMouseLeave(WISP_MSG *pMsg);
};


#endif


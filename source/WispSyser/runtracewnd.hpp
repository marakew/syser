
#ifndef _RUNTRACEWND_HPP_
#define _RUNTRACEWND_HPP_

#include "../Wisp/wisplist.hpp"

struct CRunTraceWnd : public CWispList
{
	unsigned long m_Color[16];

	CRunTraceWnd();
	virtual ~CRunTraceWnd();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnUpdate(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnClose(WISP_MSG *pMsg);
	bool OnMouseDblClick(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	//bool OnCommand(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	void UpdateContext();

	void Create();
};

#endif
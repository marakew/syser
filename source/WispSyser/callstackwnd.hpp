
#ifndef _CALLSTACKWND_HPP_
#define _CALLSTACKWND_HPP_

#include "../Wisp/wisplist.hpp"

struct CCallStackWnd : public CWispList
{
	CListStringItem *m_pItem;

	CCallStackWnd();
	virtual ~CCallStackWnd();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreate(WISP_MSG *pMsg);
	bool OnClose(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	virtual void OnMouseDblClick(CListStringItem *Item, int nCol) override;

	void SetLineItem(CListStringItem *Item);
	void UpdateContext();

	void Create();
};

#endif

#ifndef _PROCESSLIST_HPP_
#define _PROCESSLIST_HPP_

#include "../Wisp/wisplist.hpp"

struct CProcessList : public CWispList
{
	CListStringItem *m_pSystemList;

	CProcessList();
	virtual ~CProcessList();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnUpdate(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnClose(WISP_MSG *pMsg);

	virtual void OnLButton(CListStringItem *Item, int nCol) override;

	bool UpdateContext();
	void Create();
};

#endif
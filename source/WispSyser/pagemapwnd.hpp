
#ifndef _PAGEMAPWND_HPP_
#define _PAGEMAPWND_HPP_

#include "../Wisp/wispsplitwnd.hpp"
#include "../Wisp/wisplist.hpp"

struct CPageMapWnd: public CWispSplitWnd
{
	bool _field_260;

	CWispList m_PageDirectList;
	CWispList m_PageTableList;
	unsigned long *m_PageDirectBaseAddress;
	unsigned long *m_PageTableBaseAddress;

	CPageMapWnd();
	virtual ~CPageMapWnd();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);

	bool InsertPageTableList(WISP_MSG *pMsg);

	void Create();
	void InitContext();
};

#endif

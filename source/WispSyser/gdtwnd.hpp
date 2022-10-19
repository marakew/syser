
#ifndef _GDTWND_HPP_
#define _GDTWND_HPP_

#include "../Wisp/wisplist.hpp"

struct CGDTWnd : public CWispList
{
	void *m_pGDTAddress;
	unsigned long m_dwGDTSize;
	unsigned long m_dwBeginIndex;

	CGDTWnd();
	virtual ~CGDTWnd();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
        bool OnCreate(WISP_MSG *pMsg);
        bool OnClose(WISP_MSG *pMsg);

	bool InsertGDT();
	void InitContext();
	void Create();
};

#endif

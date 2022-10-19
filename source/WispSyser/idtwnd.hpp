
#ifndef _IDTWND_HPP_
#define _IDTWND_HPP_

#include "../Wisp/wisplist.hpp"

struct CIDTWnd : public CWispList
{
	void *m_pIDTAddress;
	unsigned long m_dwIDTSize;
	unsigned long m_dwBeginIndex;

	CIDTWnd();
	virtual ~CIDTWnd();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
        bool OnCreate(WISP_MSG *pMsg);
        bool OnClose(WISP_MSG *pMsg);

	bool InsertIDT();
	void InitContext();
	void Create();
};

#endif

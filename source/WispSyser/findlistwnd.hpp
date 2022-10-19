
#ifndef _FINDLISTWND_HPP_
#define _FINDLISTWND_HPP_

#include "../Wisp/wispwnd.hpp"
#include "findstringform.hpp"

struct CStatusWnd : public CWispWnd
{
	TWideString m_StatusStr;

	CStatusWnd();
	virtual ~CStatusWnd();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreate(WISP_MSG *pMsg);
	bool OnUpdateClient(WISP_MSG *pMsg);
};

struct CFindListWnd : public CWispWnd
{
	CStatusWnd m_StatusWnd;
	CStringReferenceList m_StringReferenceList;
	unsigned long m_ShowFormat;

	CFindListWnd();
	virtual ~CFindListWnd();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreate(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdStrFind(WISP_MSG *pMsg);
	bool OnSize(WISP_MSG *pMsg);
};

#endif

#ifndef _STRINPUTWND_HPP_
#define _STRINPUTWND_HPP_

#include "../Wisp/wispform.hpp"

struct CStrInputWnd : public CWispForm
{
	TWideString *m_pResult;
	const WCHAR *m_szCaption;
	const WCHAR *m_szDefault;

	CStrInputWnd();
	virtual ~CStrInputWnd();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreateForm(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdOK(WISP_MSG *pMsg);
	
	static bool Input(TWideString *Result, const WCHAR *szDefault, const WCHAR *szCaption);
};

#endif
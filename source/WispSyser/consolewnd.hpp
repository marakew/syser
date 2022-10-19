
#ifndef _CONSOLEWND_HPP_
#define _CONSOLEWND_HPP_

#include "../Wisp/wispconsolewnd.hpp"

struct CConsoleWnd : public CWispConsoleWnd
{
	unsigned long m_StartCmd;

	CConsoleWnd();
	virtual ~CConsoleWnd();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreate(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);

	virtual void OnBeginCmd(const WCHAR *szCmd) override;// {}
	//virtual void OnCmd(const WCHAR *szCmd) override;// {}
	virtual void OnEndCmd(const WCHAR *szCmd) override;// {}

	bool Run(const char *szFile, const char *szParam, unsigned long *pResult);
};

#endif
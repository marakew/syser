
#ifndef _TERMINALWND_HPP_
#define _TERMINALWND_HPP_

#include "../Wisp/wispconsolewnd.hpp"

struct CTerminalWnd : public CWispTerminalWnd
{
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnKeyEvent(WISP_MSG *pMsg);
};

#endif

#ifndef _WISPSOFTKEYBOARD_HPP_
#define _WISPSOFTKEYBOARD_HPP_

#include "../Code/define.h"
#include "wispwnd.hpp"
#include "wispbutton.hpp"
#include "wispstatic.hpp"

struct WISP_SKB_KEY
{
	const WCHAR *szKeyName;
	unsigned int VKCmdID;
	WISP_RECT BTRect;
};

struct CWispSoftKeyboard : public CWispWnd
{
	CWispSoftKeyboard();
	virtual ~CWispSoftKeyboard();

	static WISP_SKB_KEY m_SoftKeyTable[];
	CWispBaseWnd **m_BTAry;
	CWispStaticDIB m_StaticDIB;

	bool CreateEx(int x, int y, CWispWnd *pParentWnd, unsigned int Style, unsigned int ShowMode, unsigned int CmdID);
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnGetFocus(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	void Create();
};

#endif
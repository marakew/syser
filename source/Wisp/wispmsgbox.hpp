
#ifndef _WISPMSGBOX_HPP_
#define _WISPMSGBOX_HPP_

#include "../Code/define.h"
#include "../Code/alt/altlist.hpp"
#include "wispwnd.hpp"
#include "wispdib.hpp"

struct CWispMsgBox : public CWispWnd
{
	CWispMsgBox();
	virtual ~CWispMsgBox();
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnUpdateClient(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool Create(CWispBaseWnd *pParentWnd);
	static int StaticShowMsgBox(const WCHAR *pText, const WCHAR *pCaption, unsigned int uType, CWispBaseWnd *pParentWnd);
	int ShowMsgBox(const WCHAR *pText, const WCHAR *pCaption, unsigned int uType);
	void AddButton(const WCHAR *pText, int nID);
	void AddDefaultButton(unsigned int uType);
	void AddDefaultIcon(unsigned int uType);
	void RemoveAllButtons();

	struct BTNPAIR
	{
		TWideString pBtnText;
		int nID;
	};

	TList<BTNPAIR> m_lButtons;
	TWideString m_strText;
	int m_nBtnWidth;
	CWispDIB m_dibIcon;
	int m_nReturnStatus;
};

#endif

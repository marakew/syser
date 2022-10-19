
#ifndef _WISPMULTITABVIEW_HPP_
#define _WISPMULTITABVIEW_HPP_

#include "../Code/define.h"
#include "wisptabwnd.hpp"
#include "wisptoolbar.hpp"

//enum
//{
//  WISP_MULTI_TAB_CMD_FULL_SCREEN = 0x40000000,
//  WISP_MULTI_TAB_CMD_NEW_VIEW = 0x40000001,
//  WISP_MULTI_TAB_CMD_REMOVE_VIEW = 0x40000002,
//};

struct CWispMultiTabView : public CWispTabWnd
{
	CWispMultiTabView();
	virtual ~CWispMultiTabView();
	int m_nWithToolbar;
	CWispToolbar m_Toolbar;

//	CWispButton m_FullScrBT;
//	CWispButton m_NewViewBT;
//	CWispButton m_RemoveViewBT;
//	CWispDIBList m_FullScrDIB;
//	CWispDIBList m_NewViewDIB;
//	CWispDIBList m_RemoveViewDIB;
//	CWispDIBList m_NormalViewDIB;
//	WISP_SIZE m_ViewBTSize;

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreate(WISP_MSG *pMsg);
	bool OnSize(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);

	//bool OnCommand(WISP_MSG *pMsg);
	//bool OnCmdFullScr(WISP_MSG *pMsg);
	//bool OnCmdNewView(WISP_MSG *pMsg);
	//bool OnCmdRemoveView(WISP_MSG *pMsg);
};
#endif

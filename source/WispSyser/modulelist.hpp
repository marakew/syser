
#ifndef _MODULELIST_HPP_
#define _MODULELIST_HPP_

#include "../Wisp/wisplist.hpp"
#include "../Wisp/wispmenu.hpp"

struct CDbgModule;
struct CModuleListWnd : public CWispList
{
	bool m_bClose;
	CWispMenu m_PopupMenu;
	CDbgModule *m_pDbgMod;

	CModuleListWnd();
	virtual ~CModuleListWnd();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnUpdate(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnClose(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	bool OnEventOpenPEExplorer(WISP_MSG *pMsg);
	bool OnEventGotoBase(WISP_MSG *pMsg);
	bool OnEventAnalyze(WISP_MSG *pMsg);

	virtual void OnLButton(CListStringItem *Item, int nCol) override;

	void Create(bool bClose);
	void UpdateContext();
};

#endif

#ifndef _SDSMODULELIST_HPP_
#define _SDSMODULELIST_HPP_

#include "../Wisp/wisplist.hpp"
#include "../Wisp/wispmenu.hpp"
#include "../EXEAnalyzer/sdsmodule.hpp"

struct CSDSModuleList : public CWispList
{
	CWispDIB *m_pDibC;
	CWispDIB *m_pDibCPP;
	CWispDIB *m_pDibH;
	CWispDIB *m_pDibMod;
	CListStringItem *m_pItem;
	CSDSModule *m_pSDSMod;
	CWispMenu m_PopupMenu;

	bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	bool OnEventActive(WISP_MSG *pMsg);
	bool OnEventUnload(WISP_MSG *pMsg);

	void ExpandModule(CListStringItem *Item);
	void ShowPopupMenu();
	bool OnKeyEvent(WISP_MSG *pMsg);

	virtual void OnMouseDblClick(CListStringItem *Item, int nCol) override;

	void AppendSrcFiles(CListStringItem *ParentItem, CSDSModule *pSDSMod);
	void AppendSDSModule(CSDSModule *pSDSMod);
	bool RemoveSDSModule(CSDSModule *pSDSMod);
	void ExpandSDSModule(CSDSModule *pSDSMod);
};

#endif
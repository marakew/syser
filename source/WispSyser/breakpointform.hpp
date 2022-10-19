
#ifndef _BREAKPOINTFORM_HPP_
#define _BREAKPOINTFORM_HPP_

#include "../Wisp/wispform.hpp"
#include "../Wisp/wisplist.hpp"
#include "../Wisp/wispedit.hpp"
#include "../Wisp/wispbutton.hpp"
#include "../Wisp/wisptabwnd.hpp"
#include "../EXEAnalyzer/debugger.hpp"

struct CBreakPointForm : public CWispForm
{
	CWispList *m_pCodeBP;
	CWispList *m_pDataBP;
	CWispList *m_pCurList;
	CWispTabWnd *m_pTabWnd;
	CWispButton *m_pEdit;
	CWispButton *m_pRemove;
	CWispButton *m_pRemoveAll;
	CWispButton *m_pEnableAll;
	CWispButton *m_pDisableAll;

	CBreakPointForm();
	virtual ~CBreakPointForm();
	
	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreateForm(WISP_MSG *pMsg);

	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdTabSelect(WISP_MSG *pMsg);
	bool OnCmdListSelect(WISP_MSG *pMsg);

	bool OnEvent(WISP_MSG *pMsg);
	bool OnEventEdit(WISP_MSG *pMsg);
	bool OnEventNew(WISP_MSG *pMsg);
	bool OnEventRemove(WISP_MSG *pMsg);
	bool OnEventRemoveAll(WISP_MSG *pMsg);
	bool OnEventEnableAll(WISP_MSG *pMsg);
	bool OnEventDisableAll(WISP_MSG *pMsg);


	void CreateShow();
	int FillCode();
	int ShowCode();
	int FillData();
	int ShowData();
	void UpdateButton();

	void InitContext(CWispList *pList);
	void DrawBP(CWispList *pList, CListStringItem *Item, BREAK_POINT *BreakPoint);
	void DrawModuleBP(CWispList *pList, CListStringItem *Item, BREAK_POINT::MODULE *BreakPoint);
	void Create();
};

#endif

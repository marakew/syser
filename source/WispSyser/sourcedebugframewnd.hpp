
#ifndef _SOURCEDEBUGFRAMEWND_HPP_
#define _SOURCEDEBUGFRAMEWND_HPP_

#include "../Wisp/wispwnd.hpp"
#include "../Wisp/wispstatic.hpp"
#include "../Wisp/wispmenu.hpp"
#include "../Wisp/wispedit.hpp"
#include "../Wisp/wisptoolbar.hpp"
#include "../Wisp/wisptabwnd.hpp"
#include "../Wisp/wispsplitwnd.hpp"
//#include "..\Wisp/wispconsolewnd.hpp"
#include "../Wisp/wispdib.hpp"

#include "sdsmodulelist.hpp"
#include "terminalwnd.hpp"
#include "multisourcecodeview.hpp"
#include "typeviewerwnd.hpp"
#include "../EXEAnalyzer/sdsmodule.hpp"
#include "sourcecodewnd.hpp"

#define SRC_W_TAB 0
#define SRC_T_TAB 1

struct CSourceDebugFrameWnd : public CWispSplitWnd
{
	CSDSModuleList m_SDSModuleList;
	CWispTabWnd m_SDSModuleListTabWnd;
	CWispTabWnd m_ContextTabWnd; //TAB
	CTerminalWnd m_TerminalWnd;
	CMultiSourceCodeView m_MultiSourceCodeView;
	CTypeViewerWnd m_TypeViewerWnd;
	CWispToolbar m_Toolbar;
	CWispMenu m_MainMenu;
	CWispSplitWnd m_ViewSplitWnd;
	CWispSplitWnd m_MainSplitWnd;
	CWispDIBList m_DIBList;

	CSourceDebugFrameWnd();
	virtual ~CSourceDebugFrameWnd();
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	void AdjustSplit();
	bool OnSize(WISP_MSG *pMsg);
	bool OnGetFocus(WISP_MSG *pMsg);

	bool OnEventDbgStep(WISP_MSG *pMsg);
	bool OnEventDbgProceed(WISP_MSG *pMsg);
	bool OnEventDbgReturn(WISP_MSG *pMsg);
	bool OnEventDbgContinue(WISP_MSG *pMsg);
	bool OnEventBreakPointForm(WISP_MSG *pMsg);
	bool OnEventSDSModuleList(WISP_MSG *pMsg);
	bool OnEventWatchView(WISP_MSG *pMsg);
	bool OnEventTerminal(WISP_MSG *pMsg);

	bool OnEventCallStack(WISP_MSG *pMsg);

	bool OnEventRegView(WISP_MSG *pMsg);
	bool OnEventDataViewForm(WISP_MSG *pMsg);

	bool OnEventToggleBM(WISP_MSG *pMsg);
	bool OnEventNextBM(WISP_MSG *pMsg);
	bool OnEventPrevBM(WISP_MSG *pMsg);
	bool OnEventClearBM(WISP_MSG *pMsg);

	bool OnEventMenuDebug(WISP_MSG *pMsg);
	bool OnEventMenuMonitor(WISP_MSG *pMsg);
	bool OnEventMenuView(WISP_MSG *pMsg);
	bool OnEventGotoLine(WISP_MSG *pMsg);

	void UpdateMenu();
	void UpdateContext(bool bUpdateSrc);

	CSourceCodeWnd *OpenSrcView(CSDSModule *pSDSMod, unsigned long FileID, unsigned long LineNumber);
	void RemoveSDSModule(CSDSModule *pSDSMod);
};

#endif
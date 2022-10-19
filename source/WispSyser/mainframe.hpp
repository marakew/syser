
#ifndef _MAINFRAME_HPP_
#define _MAINFRAME_HPP_

#include "../Code/define.h"
#include "../Wisp/wispwnd.hpp"
#include "../Wisp/wispstatic.hpp"
#include "../Wisp/wisptabwnd.hpp"
#include "../Wisp/wispsoftkeyboard.hpp"
#include "../Wisp/wispcalcwnd.hpp"
#include "../Wisp/wispprogressform.hpp"

#include "systemexplorer.hpp"
#include "consolewnd.hpp"
#include "genereglist.hpp"
#include "dataviewform.hpp"
#include "debuggerselectform.hpp"
#include "pluginlistform.hpp"
#include "breakpointform.hpp"
#include "processlist.hpp"
#include "peexplorerform.hpp"
#include "sourcedebugframewnd.hpp"
#include "runtracewnd.hpp"
#include "callstackwnd.hpp"
#include "dataoperatordlg.hpp"
#include "keymappingpage.hpp"
#include "modulelist.hpp"
#include "crossreferenceform.hpp"
#include "commentlist.hpp"
#include "pagemapwnd.hpp"
#include "reghelpwnd.hpp"
#include "idtwnd.hpp"
#include "gdtwnd.hpp"
#include "findstringform.hpp"
#include "findlistwnd.hpp"

#define CV_TAB 0
#define TERM_TAB 1
#define SRC_TAB 2

struct CMainFrame : public CWispWnd
{
	//Register View
	CWispTabWnd m_CPUTabWnd;
	CGeneRegList *m_pGeneRegList;
	CDataViewForm m_DataViewForm;
	CSystemExplorer m_SystemExplorer;
	CConsoleWnd m_ConsoleWnd;
	CSourceDebugFrameWnd m_SourceDebugFrameWnd;
	CWispTabWnd m_MainTabWnd; //TAB
	CWispProgressForm m_ProgressForm;
	CDebuggerSelectForm m_DebuggerSelectForm;
	CPluginListForm m_PluginListForm;
	CKeyMappingPage m_KeyMappingPage;
	CWispSoftKeyboard m_SoftKeyboard;
	CBreakPointForm m_BreakPointForm;
	CPageMapWnd m_PageMapWnd;
	CX86RegHelpTabWnd m_X86RegHelpTabWnd;
	CWispCalcWnd m_CalcWnd;
	//CObjectDirectoryWnd m_ObjectDirectoryWnd;
	CDataOperatorDlg m_DataOperatorDlg;
	CCrossReferenceForm m_CrossReferenceForm;
	CCommentList m_CommentList;
	CCallStackWnd m_CallStackWnd;
	CRunTraceWnd m_RunTraceWnd;
	CModuleListWnd m_ModuleListWnd;
	//CHwndListWnd m_HwndListWnd;
	//CHwndPropertyForm m_HwndPropertyForm;
	CProcessList m_ProcessList;
	CIDTWnd m_IDTWnd;
	CGDTWnd m_GDTWnd;
	CWispStaticStr m_StaticText;
	CStringReferenceList m_StringReferenceList;
	CFindListWnd m_FindListWnd;
	CPEExplorerForm m_PEExplorerForm;
	//CIBMAsciiWnd m_IBMAsciiWnd;
	//CIBMAsciiWnd m_EBCDICWnd;
	//CIBMAsciiWnd m_ANSIAsciiWnd;

	CMainFrame();
	virtual ~CMainFrame();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnUpdate(WISP_MSG *pMsg);
	bool OnClose(WISP_MSG *pMsg);
	bool OnSize(WISP_MSG *pMsg);
	bool OnBeginMove(WISP_MSG *pMsg);
	bool OnEndMove(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	bool Move();
	bool OnKeyEvent(WISP_MSG *pMsg);

	bool OnDebuggerPlunge(WISP_MSG *pMsg); //30
	bool OnDebuggerClose(WISP_MSG *pMsg); //31
	void FocusActive();

	bool OnEventDbgStep(WISP_MSG *pMsg);
	bool OnEventDbgProceed(WISP_MSG *pMsg);
	bool OnEventStepBranch(WISP_MSG *pMsg);
	bool OnEventDbgReturn(WISP_MSG *pMsg);
	bool OnEventDbgContinue(WISP_MSG *pMsg);
	bool OnEventBreakPointForm(WISP_MSG *pMsg);
	bool OnEventCallStack(WISP_MSG *pMsg);
	bool OnEventRegView(WISP_MSG *pMsg);
	bool OnEventDataViewForm(WISP_MSG *pMsg);

	void CreateProgress(const WCHAR *Text, unsigned long MaxPos);
	void UpdateProgress(unsigned long Pos);
	void UpdateCaption(const WCHAR *Text);
	void DestroyProgress();
};

#endif
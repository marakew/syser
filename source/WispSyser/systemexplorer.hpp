
#ifndef _SYSTEMEXPLORER_HPP_
#define _SYSTEMEXPLORER_HPP_

#include "../Code/define.h"
#include "../Code/alt/altmap.hpp"

#include "../Wisp/wispwnd.hpp"
#include "../Wisp/wispstatic.hpp"
#include "../Wisp/wispmenu.hpp"
#include "../Wisp/wispedit.hpp"
#include "../Wisp/wisptoolbar.hpp"
#include "../Wisp/wisptabwnd.hpp"
#include "../Wisp/wispsplitwnd.hpp"

#include "watchlist.hpp"
#include "stackwatchlist.hpp"
#include "multicodeview.hpp"
#include "multidataview.hpp"
#include "genereglist.hpp"
#include "fpuregisterlist.hpp"
#include "sseregisterlist.hpp"
#include "terminalwnd.hpp"
#include "syseroptionform.hpp"
#include "codeview.hpp"
#include "runtrace.hpp"
#ifndef _X64_
#include "sehchainwnd.hpp"
#endif
#include "searchdialog.hpp"
#include "typeviewerwnd.hpp"

//TODO
int GetActiveCPU();

struct CMainMenu : public CWispMenu
{
};

	struct StringRef
	{
		ULONG_PTR Address;
		unsigned long Len;
		StringRef(ULONG_PTR Address_, unsigned long Len_):
			Address(Address_), Len(Len_)
		{}
	};

#define CV_REG_GENE_TAB 0
#define CV_REG_FPU_TAB 1
#define CV_REG_SSE_TAB 2

#define CV_S_TAB 0
#define CV_W_TAB 1

struct CSystemExplorer : public CWispWnd
{
	CSyserOptionForm m_SyserOptionForm;
	CSearchDialog m_SearchDialog;
	CMainMenu m_MainMenu;
	CWispStaticStr m_VersionStr;
	CWispStaticStr m_InfoStr;
	CWispEdit *m_pInstrInfo;
	CWispToolbar m_Toolbar;
	CWatchList m_WatchList;
	CStackWatchList m_StackWatchList;
	CWispTabWnd m_RegTabWnd; //TAB
	CWispTabWnd m_ContextTabWnd; //TAB
	int m_SplitRight;
	CWispSplitWnd m_ViewSplitWnd;
	CWispSplitWnd m_MainSplitWnd;
	CWispSplitWnd m_MoniteSplitWnd;

	CMultiCodeView m_MultiCodeView;
	CMultiDataView m_MultiDataView;

	int m_nCPUNumbers;
	CFpuRegisterList *m_pFpuRegisterList;
	CSSERegisterList *m_pSSERegisterList;
	CGeneRegList *m_pGeneRegList;
	CWispTabWnd *m_pRegsTabWnd;

	CTerminalWnd m_TerminalWnd;
	CCodeView *m_pCodeView;

	CRunTrace m_RunTrace;
	//X86_CPU_REG m_CPUReg;
	unsigned int m_CIPTrace;
	unsigned int m_CIPRegWndTrace;

	//CThreadListWnd m_ThreadListWnd;
#ifndef _X64_
	CSEHChainWnd m_SEHChainWnd;
#endif
	map<ULONG_PTR,StringRef> m_FoundStringRefMap;
	//CAnalyzer m_Analyzer;
	CWispList m_FunctionListWnd;

	CTypeViewerWnd *m_pTypeViewerWnd;	//+++

	CSystemExplorer();
	virtual ~CSystemExplorer();

	virtual void Hide() override;
	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool CreateRegsTabs(WISP_MSG *pMsg);

	bool OnUpdate(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);

	bool OnCmdMainSplit(WISP_MSG *pMsg);
	bool OnCmdViewSplit(WISP_MSG *pMsg);
	bool OnCmdMoniteSplit(WISP_MSG *pMsg);
	bool OnCmdGotoFunction(WISP_MSG *pMsg);

	bool OnCommand(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	void AdjustSplit();
	bool OnSize(WISP_MSG *pMsg);
	bool OnGetFocus(WISP_MSG *pMsg);

	bool OnEventOpen(WISP_MSG *pMsg);
	bool OnEventClose(WISP_MSG *pMsg);

	bool OnEventSaveComment(WISP_MSG *pMsg);
	bool OnEventLoadComment(WISP_MSG *pMsg);
	bool OnEventLoadSymbol(WISP_MSG *pMsg);
	bool OnEventNextPos(WISP_MSG *pMsg);
	bool OnEventPrevPos(WISP_MSG *pMsg);
	bool OnEventSavePos(WISP_MSG *pMsg);
	bool OnEventResetPos(WISP_MSG *pMsg);
	bool OnEventDbgReset(WISP_MSG *pMsg);
	bool OnEventDbgStep(WISP_MSG *pMsg);
	bool OnEventDbgProceed(WISP_MSG *pMsg);
	bool OnEventStepBranch(WISP_MSG *pMsg);
	bool OnEventDbgReturn(WISP_MSG *pMsg);
	bool OnEventDbgContinue(WISP_MSG *pMsg);
	bool OnEventBreakPointForm(WISP_MSG *pMsg);

	bool OnEventDebuggerSelectForm(WISP_MSG *pMsg);
	bool OnEventOptionForm(WISP_MSG *pMsg);
	bool OnEventPluginListForm(WISP_MSG *pMsg);

	bool OnEventTabGeneReg(WISP_MSG *pMsg);
	bool OnEventTabFPUReg(WISP_MSG *pMsg);
	bool OnEventTabSSEReg(WISP_MSG *pMsg);

	bool OnEventWatch(WISP_MSG *pMsg);
	bool OnEventStack(WISP_MSG *pMsg);
	bool OnEventIDT(WISP_MSG *pMsg);
	bool OnEventGDT(WISP_MSG *pMsg);
	bool OnEventLDT(WISP_MSG *pMsg);
	bool OnEventProcessList(WISP_MSG *pMsg);
	bool OnEventThreadList(WISP_MSG *pMsg);
	bool OnEventModuleList(WISP_MSG *pMsg);
	bool OnEventFunctionList(WISP_MSG *pMsg);
	bool OnEventHwndList(WISP_MSG *pMsg);
	bool OnEventRunTrace(WISP_MSG *pMsg);
	bool OnEventSeachDialog(WISP_MSG *pMsg);
	bool OnEventCommentList(WISP_MSG *pMsg);
	bool OnEventPageMap(WISP_MSG *pMsg);
	bool OnEventRegHelp(WISP_MSG *pMsg);

	bool OnEventAboutForm(WISP_MSG *pMsg);

	bool OnEventCheckNew(WISP_MSG *pMsg);
	bool OnEventObjDirWnd(WISP_MSG *pMsg);

	bool OnEventCalc(WISP_MSG *pMsg);

	bool OnEventCallStack(WISP_MSG *pMsg);

	bool OnEventRegView(WISP_MSG *pMsg);
	bool OnEventDataViewForm(WISP_MSG *pMsg);
	bool OnEventSoftKB(WISP_MSG *pMsg);
	bool OnEventKeyMap(WISP_MSG *pMsg);
	bool OnEventPEExplorerForm(WISP_MSG *pMsg);
	bool OnEventIBM(WISP_MSG *pMsg);
	bool OnEventEBCDI(WISP_MSG *pMsg);
	bool OnEventANSI(WISP_MSG *pMsg);

	bool OnEventToggleBM(WISP_MSG *pMsg);
	bool OnEventNextBM(WISP_MSG *pMsg);
	bool OnEventPrevBM(WISP_MSG *pMsg);
	bool OnEventClearBM(WISP_MSG *pMsg);

	bool OnEventSEH(WISP_MSG *pMsg);
	bool OnEventFile(WISP_MSG *pMsg);
	bool OnEventDebug(WISP_MSG *pMsg);
	bool OnEventMonitor(WISP_MSG *pMsg);
	bool OnEventView(WISP_MSG *pMsg);
	bool OnEventInformation(WISP_MSG *pMsg);
	bool OnEventOption(WISP_MSG *pMsg);
	bool OnEventTools(WISP_MSG *pMsg);
	bool OnEventHelp(WISP_MSG *pMsg);

	bool OnEventDataShow(WISP_MSG *pMsg);
	bool OnEventCodeShow(WISP_MSG *pMsg);

	bool OnEventStrRef(WISP_MSG *pMsg);

	bool OnEventNextCVTrace(WISP_MSG *pMsg);
	bool OnEventPrevCVTrace(WISP_MSG *pMsg);

	bool OnEventBPEnableAll(WISP_MSG *pMsg);
	bool OnEventBPRemoveAll(WISP_MSG *pMsg);
	bool OnEventBPDisableAll(WISP_MSG *pMsg);

	bool OnEventLoadHistory(WISP_MSG *pMsg);

	bool OnEventTypeViewerWnd(WISP_MSG *pMsg);

	void ResetContext();
	void UpdateMenu();
	void UpdateContext();
	void UpdateRegView();
	void ViewCodeAddress(ULONG_PTR Address);
	void ResetView();
	void SelectCodeAddress(ULONG_PTR Address);
	void CodeGotoAddr(ULONG_PTR Address, bool bHistory);
	void DataGotoAddr(ULONG_PTR Address, unsigned long Size);

	CTracePoint *PrevCIP();
	CTracePoint *NextCIP();
	bool TracePoint(unsigned int TracePoint);
};

#endif
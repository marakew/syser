
#include "systemexplorer.hpp"
#include "resource.hpp"
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "aboutform.hpp"
#include "syserconfig.hpp"
#include "../Code/sysdep.hpp"

//#define LSPLIT

//TODO
	int dwCPUNumbers = 3;

	int GetActiveCPU()
	{
		//TODO
		return 0;
	}


//
	CSystemExplorer::CSystemExplorer()
	{
		m_SplitRight = 0;//gSyserConfig.iSplitRight;
		m_pCodeView = 0;
		m_nCPUNumbers = 1;
		m_pTypeViewerWnd = nullptr;
	}
	CSystemExplorer::~CSystemExplorer()
	{
	}

	void CSystemExplorer::Hide()
	{
		CCodeView *pWnd = (CCodeView *)m_MultiCodeView.GetActiveWnd();
		if (pWnd && pWnd->m_TipWnd.IsWindowShow())
			pWnd->HideTip();
	}

	WISP_MSG_MAP_BEGIN(CSystemExplorer)
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP(WISP_WM_GET_FOCUS, OnGetFocus)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispWnd)

	WISP_CMD_MAP_BEGIN(CSystemExplorer)
		WISP_CMD_MAP(CMD_ID_SYSTEM_MAIN_SPLIT, OnCmdMainSplit)
		WISP_CMD_MAP(CMD_ID_SYSTEM_VIEW_SPLIT, OnCmdViewSplit)
		WISP_CMD_MAP(CMD_ID_SYSTEM_MONIT_SPLIT, OnCmdMoniteSplit)
		WISP_CMD_MAP(0x45, OnCmdGotoFunction)
	WISP_CMD_MAP_END

	WISP_EVENT_MAP_BEGIN(CSystemExplorer)

		WISP_EVENT_MAP(EVENT_ID_OPEN, OnEventOpen)
		WISP_EVENT_MAP(EVENT_ID_CLOSE, OnEventClose)

		WISP_EVENT_MAP(EVENT_ID_SAVECMT, OnEventSaveComment)
		WISP_EVENT_MAP(EVENT_ID_LOADCMT, OnEventLoadComment)

		WISP_EVENT_MAP(EVENT_ID_LOADSYM, OnEventLoadSymbol)

		WISP_EVENT_MAP(EVENT_ID_NEXTPOS, OnEventNextPos)
		WISP_EVENT_MAP(EVENT_ID_PREVPOS, OnEventPrevPos)
		WISP_EVENT_MAP(EVENT_ID_SAVEPOS, OnEventSavePos)
		WISP_EVENT_MAP(EVENT_ID_CLRPOS, OnEventResetPos)
		WISP_EVENT_MAP(EVENT_ID_DBG_RESET, OnEventDbgReset)
		WISP_EVENT_MAP(EVENT_ID_DBG_STEP, OnEventDbgStep)
		WISP_EVENT_MAP(EVENT_ID_DBG_PROCEED, OnEventDbgProceed)
		WISP_EVENT_MAP(EVENT_ID_DBG_STEPB, OnEventStepBranch)
		WISP_EVENT_MAP(EVENT_ID_DBG_RETURN, OnEventDbgReturn)
		WISP_EVENT_MAP(EVENT_ID_GO, OnEventDbgContinue)
		WISP_EVENT_MAP(EVENT_ID_BREAK_POINT, OnEventBreakPointForm)
		WISP_EVENT_MAP(EVENT_ID_DBG_LIST, OnEventDebuggerSelectForm)
		WISP_EVENT_MAP(EVENT_ID_CONFIG_OPTION, OnEventOptionForm)
		WISP_EVENT_MAP(EVENT_ID_PLUGINS, OnEventPluginListForm)
		WISP_EVENT_MAP(EVENT_ID_GENE_REG, OnEventTabGeneReg)
		WISP_EVENT_MAP(EVENT_ID_FPU_REG, OnEventTabFPUReg)
		WISP_EVENT_MAP(EVENT_ID_SSE_REG, OnEventTabSSEReg)

		WISP_EVENT_MAP(EVENT_ID_WATCH_LIST, OnEventWatch)
		WISP_EVENT_MAP(EVENT_ID_STACK_WATCH_LIST, OnEventStack)
		WISP_EVENT_MAP(EVENT_ID_IDT, OnEventIDT)
		WISP_EVENT_MAP(EVENT_ID_GDT, OnEventGDT)
		WISP_EVENT_MAP(EVENT_ID_LDT, OnEventLDT)
		WISP_EVENT_MAP(EVENT_ID_PROCESS, OnEventProcessList)
		WISP_EVENT_MAP(EVENT_ID_THREADS, OnEventThreadList)
		WISP_EVENT_MAP(EVENT_ID_MODULES, OnEventModuleList)
		WISP_EVENT_MAP(EVENT_ID_FUNCTIONS, OnEventFunctionList)
		WISP_EVENT_MAP(EVENT_ID_HWND, OnEventHwndList)
		WISP_EVENT_MAP(EVENT_ID_RUNTRACE, OnEventRunTrace)
		WISP_EVENT_MAP(EVENT_ID_MEMSEARCH, OnEventSeachDialog)
		WISP_EVENT_MAP(EVENT_ID_CMTLIST, OnEventCommentList)
		WISP_EVENT_MAP(EVENT_ID_PAGEMAP, OnEventPageMap)
		WISP_EVENT_MAP(EVENT_ID_REG_HELP, OnEventRegHelp)
		WISP_EVENT_MAP(EVENT_ID_ABOUT, OnEventAboutForm)
		WISP_EVENT_MAP(EVENT_ID_CHECK_NEW_VER, OnEventCheckNew)
		WISP_EVENT_MAP(EVENT_ID_OBJDIR, OnEventObjDirWnd)

		WISP_EVENT_MAP(EVENT_ID_CALC, OnEventCalc)
		WISP_EVENT_MAP(EVENT_ID_CALLSTACK, OnEventCallStack)
		WISP_EVENT_MAP(EVENT_ID_REGVIEW, OnEventRegView)
		WISP_EVENT_MAP(EVENT_ID_DATAVIEW, OnEventDataViewForm)
		WISP_EVENT_MAP(EVENT_ID_SOFTKB, OnEventSoftKB)
		WISP_EVENT_MAP(EVENT_ID_HOTKEY, OnEventKeyMap)
		WISP_EVENT_MAP(EVENT_ID_PE_EXPLORER, OnEventPEExplorerForm)
		WISP_EVENT_MAP(EVENT_ID_IBM, OnEventIBM)
		WISP_EVENT_MAP(EVENT_ID_EBCDIC, OnEventEBCDI)
		WISP_EVENT_MAP(EVENT_ID_ANSI, OnEventANSI)

		WISP_EVENT_MAP(EVENT_ID_TOGGLE_BM, OnEventToggleBM)
		WISP_EVENT_MAP(EVENT_ID_NEXT_BM, OnEventNextBM)
		WISP_EVENT_MAP(EVENT_ID_PREV_BM, OnEventPrevBM)
		WISP_EVENT_MAP(EVENT_ID_CLEAR_BM, OnEventClearBM)

		WISP_EVENT_MAP(EVENT_ID_SEH, OnEventSEH)
		WISP_EVENT_MAP(EVENT_ID_MENU_FILE, OnEventFile)
		WISP_EVENT_MAP(EVENT_ID_MENU_DEBUG, OnEventDebug)
		WISP_EVENT_MAP(EVENT_ID_MENU_MONITOR, OnEventMonitor)
		WISP_EVENT_MAP(EVENT_ID_MENU_VIEW, OnEventView)
		WISP_EVENT_MAP(EVENT_ID_MENU_INFO, OnEventInformation)
		WISP_EVENT_MAP(EVENT_ID_MENU_OPTION, OnEventOption)
		WISP_EVENT_MAP(EVENT_ID_MENU_TOOLS, OnEventTools)
		WISP_EVENT_MAP(EVENT_ID_MENU_HELP, OnEventHelp)

		WISP_EVENT_MAP(EVENT_ID_DATA_FOCUS, OnEventDataShow)
		WISP_EVENT_MAP(EVENT_ID_CODE_FOCUS, OnEventCodeShow)

		WISP_EVENT_MAP(EVENT_ID_STRREF, OnEventStrRef)

		WISP_EVENT_MAP(EVENT_ID_CV_NEXT_CIP, OnEventNextCVTrace)
		WISP_EVENT_MAP(EVENT_ID_CV_PREV_CIP, OnEventPrevCVTrace)

		WISP_EVENT_MAP(EVENT_ID_BP_ENABLE_ALL, OnEventBPEnableAll)
		WISP_EVENT_MAP(EVENT_ID_BP_REMOVE_ALL, OnEventBPRemoveAll)
		WISP_EVENT_MAP(EVENT_ID_BP_DISABLE_ALL, OnEventBPDisableAll)

		WISP_EVENT_MAP_DEFAULT(OnEventLoadHistory)

	WISP_EVENT_MAP_END

	bool CSystemExplorer::CreateRegsTabs(WISP_MSG *pMsg)
	{
		m_nCPUNumbers = dwCPUNumbers;

		m_pRegsTabWnd = new CWispTabWnd[m_nCPUNumbers];
		if (!m_pRegsTabWnd) return false;

		m_pFpuRegisterList = new CFpuRegisterList[m_nCPUNumbers];
		m_pSSERegisterList = new CSSERegisterList[m_nCPUNumbers];
		m_pGeneRegList = new CGeneRegList[m_nCPUNumbers];

		WISP_RECT Rect;
		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 170;
		Rect.cy = 310;

		if (!m_pFpuRegisterList || !m_pSSERegisterList || !m_pGeneRegList ||
			!m_RegTabWnd.CreateWnd(WSTR(""), Rect, &m_MoniteSplitWnd, 0, 0x10000, WISP_SH_MINSIZE))
		{
			SafeDeleteAry(m_pRegsTabWnd);
			SafeDeleteAry(m_pFpuRegisterList);
			SafeDeleteAry(m_pSSERegisterList);
			SafeDeleteAry(m_pGeneRegList);
			return false;
		}

		int nActiveCPU = GetActiveCPU();

		for (int nCPU = 0; nCPU < m_nCPUNumbers; ++nCPU)
		{
			WISP_RECT RectTab;
			RectTab.x = 0;
			RectTab.y = 0;
			RectTab.cx = 170;
			RectTab.cy = 310;

			if (!m_pRegsTabWnd[nCPU].CreateWnd(WSTR("Reg"), RectTab, &m_RegTabWnd, 0, 0x20000, WISP_SH_MINSIZE))
				return false;

                        WISP_RECT RectGene;
			RectGene.x = 0;
			RectGene.y = 0;
			RectGene.cx = 0;
			RectGene.cy = 0;
			if (!m_pGeneRegList[nCPU].CreateWnd(0, RectGene, &m_pRegsTabWnd[nCPU], CMD_ID_GENE_REG, 0x1030000, WISP_SH_MINSIZE))
				return false;
			m_pGeneRegList[nCPU].SetOwner(this);
			m_pGeneRegList[nCPU].m_nCPU = nCPU;

                        WISP_RECT RectFpu;
			RectFpu.x = 0;
			RectFpu.y = 0;
			RectFpu.cx = 0;
			RectFpu.cy = 0;
			if (!m_pFpuRegisterList[nCPU].CreateWnd(0, RectFpu, &m_pRegsTabWnd[nCPU], CMD_ID_FPU_REG, 0x1030000, WISP_SH_MINSIZE))
				return false;
			m_pFpuRegisterList[nCPU].SetOwner(this);
			m_pFpuRegisterList[nCPU].m_nCPU = nCPU;

                        WISP_RECT RectSSE;
			RectSSE.x = 0;
			RectSSE.y = 0;
			RectSSE.cx = 0;
			RectSSE.cy = 0;
			if (!m_pSSERegisterList[nCPU].CreateWnd(0, RectSSE, &m_pRegsTabWnd[nCPU], CMD_ID_SSE_REG, 0x1030000, WISP_SH_MINSIZE))
				return false;
			m_pSSERegisterList[nCPU].SetOwner(this);
			m_pSSERegisterList[nCPU].m_nCPU = nCPU;
			
			m_pRegsTabWnd[nCPU].InsertWnd(WSTR("REG ALT+R"), &m_pGeneRegList[nCPU], gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x34));
			m_pRegsTabWnd[nCPU].InsertWnd(WSTR("FPU ALT+F"), &m_pFpuRegisterList[nCPU], gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x3A));
			m_pRegsTabWnd[nCPU].InsertWnd(WSTR("MMX ALT+X"), &m_pSSERegisterList[nCPU], gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x30));

			WCHAR szCPUNum[20];
			if (nCPU == nActiveCPU)
				TSPrintf(szCPUNum, WSTR("*CPU %d "), nCPU);
			else	TSPrintf(szCPUNum, WSTR(" CPU %d "), nCPU);

			m_RegTabWnd.InsertWnd(szCPUNum, &m_pRegsTabWnd[nCPU], 0);

			m_pGeneRegList[nCPU].UpdateContext();
			m_pFpuRegisterList[nCPU].UpdateContext();
			m_pSSERegisterList[nCPU].UpdateContext();
		}

		return true;
	}

	bool CSystemExplorer::OnUpdate(WISP_MSG *pMsg)
	{
		if (m_SplitRight != gSyserConfig.iSplitRight)
		{
			m_SplitRight = gSyserConfig.iSplitRight;
			m_MainSplitWnd.m_WndList.Reverse();
			AdjustSplit();
		}
		return true;
	}

	bool CSystemExplorer::OnCreate(WISP_MSG *pMsg)
	{
		m_MainMenu.LoadMenuGroup(this, WISP_ID_MAIN_MENU, SystemExplorerMainMenu, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF));
		m_MainMenu.SetOwner(this);

		m_InfoStr.Create(WSTR(""), //680,
				m_MainMenu.m_FullMenuWidth + 16 + 150,
				m_MainMenu.m_ClientRect.cy - 16, 138, 16, &m_MainMenu, 0, 0, 1);

		m_VersionStr.Create(0, //530,
				m_MainMenu.m_FullMenuWidth + 16,
				m_MainMenu.m_ClientRect.cy - 16, 138, 16, &m_MainMenu, 0, 0, 1);
		m_VersionStr.m_WindowDC.m_TextColor = 0xFF0000;

		WCHAR szVersion[256];
		TStrCpy(szVersion, "Ver: ");
		//TStrCat(szVersion, "1.99.1900.1220");
		TStrCat(szVersion, "Develop");
		m_VersionStr.SetWindowText(szVersion);

		m_Toolbar.CreateEx(this, m_MainMenu.m_WindowRect.cy, -1, WISP_ID_MAIN_TOOLBAR, 0);
		m_Toolbar.LoadToolbar(gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF), SystemExplorerToolbar);
		m_Toolbar.SetOwner(this);

		m_pInstrInfo = (CWispEdit*)m_Toolbar.GetItem(EVENT_ID_INSTRINFO);
		if (!m_pInstrInfo)
			return false;

		WISP_RECT Rect;
		
		Rect.x = 0;
		Rect.y = m_MainMenu.m_WindowRect.cy + m_Toolbar.m_WindowRect.cy;
		Rect.cx = m_ClientRect.cx;
		Rect.cy = m_ClientRect.cy - Rect.y;
		if (!m_MainSplitWnd.CreateWnd(0, Rect, this, CMD_ID_SYSTEM_MAIN_SPLIT, 0x20000, WISP_SH_MINSIZE))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_ViewSplitWnd.CreateWnd(0, Rect, &m_MainSplitWnd, CMD_ID_SYSTEM_VIEW_SPLIT, 0x10000, WISP_SH_MINSIZE))
			return false;

		m_ViewSplitWnd.SetOwner(this);

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_MoniteSplitWnd.CreateWnd(0, Rect, &m_MainSplitWnd, CMD_ID_SYSTEM_MONIT_SPLIT, 0x10000, WISP_SH_MINSIZE))
			return false;

		m_MoniteSplitWnd.SetOwner(this);
		m_MainSplitWnd.InsertWnd(&m_MoniteSplitWnd, 0, 250, -1);
		m_MainSplitWnd.InsertWnd(&m_ViewSplitWnd, 0, 200, -1);
		m_MainSplitWnd.AdjustWndPos();

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_MultiDataView.CreateWnd(0, Rect, &m_ViewSplitWnd, CMD_ID_MULTIDATA, 0xE10000, WISP_SH_MINSIZE))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_MultiCodeView.CreateWnd(0, Rect, &m_ViewSplitWnd, CMD_ID_MULTICODE, 0xE10000, WISP_SH_MINSIZE))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_TerminalWnd.CreateWnd(0, Rect, &m_ViewSplitWnd, CMD_ID_TERMWND, 0, WISP_SH_MINSIZE))
			return false;

		m_TerminalWnd.AttachConsole(&gpSyser->m_MainFrame.m_ConsoleWnd);
		m_TerminalWnd.SetHistoryLines(350);

		m_ViewSplitWnd.InsertWnd(&m_MultiDataView, 0, 90, -1);
		m_ViewSplitWnd.InsertWnd(&m_MultiCodeView, 0, ((CWispWnd*)gpCurWisp->m_pRootWnd)->m_ClientRect.cy/2, -1);
		m_ViewSplitWnd.InsertWnd(&m_TerminalWnd, 0, 30, -1);
		m_ViewSplitWnd.AdjustWndPos();

		if (!CreateRegsTabs(pMsg))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 170;
		Rect.cy = 200;
		if (!m_ContextTabWnd.CreateWnd(WSTR("Context"), Rect, &m_MoniteSplitWnd, 0, 0x20000, WISP_SH_MINSIZE))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_WatchList.CreateWnd(0, Rect, &m_ContextTabWnd, 0, 0x1030000, WISP_SH_MINSIZE))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_StackWatchList.CreateWnd(0, Rect, &m_ContextTabWnd, 0, 0x1010000, WISP_SH_MINSIZE))
			return false;

		m_ContextTabWnd.InsertWnd(WSTR("Stack ALT+S"), &m_StackWatchList, gpCurWisp->m_DIBLib.LoadDIB("\\SyserIcon\\Stack.bmp", 0, 0xFF00FF));
		m_ContextTabWnd.InsertWnd(WSTR("Watch ALT+W"), &m_WatchList, gpCurWisp->m_DIBLib.LoadDIB("\\SyserIcon\\Watch.bmp", 0, 0xFF00FF));

		m_MoniteSplitWnd.InsertWnd(&m_RegTabWnd, 0, m_RegTabWnd.m_WindowRect.cy, -1);
		m_MoniteSplitWnd.InsertWnd(&m_ContextTabWnd, 0, m_ContextTabWnd.m_WindowRect.cy, -1);
		m_MoniteSplitWnd.AdjustWndPos();

		RegisterKeyEvent(EVENT_ID_GENE_REG, 'R', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_FPU_REG, 'F', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_SSE_REG, 'X', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_WATCH_LIST, 'W', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_STACK_WATCH_LIST, 'S', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_CODE_FOCUS, 'p', 0, 1);
		RegisterKeyEvent(EVENT_ID_DATA_FOCUS, 'p', WISP_SK_SHIFT, 1);
		RegisterKeyEvent(EVENT_ID_MENU_FILE, 'F', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_MENU_DEBUG, 'D', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_MENU_MONITOR, 'M', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_MENU_VIEW, 'V', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_MENU_INFO, 'I', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_MENU_OPTION, 'O', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_MENU_TOOLS, 'T', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_MENU_HELP, 'H', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_CV_NEXT_CIP, 'k', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_CV_PREV_CIP, 'm', WISP_SK_ALT, 1);

		gpSyser->RegisterHotKeyEvent(EVENT_ID_TOGGLE_BM, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_NEXT_BM, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_PREV_BM, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_CLEAR_BM, this);
 
		return true;
	}
	bool CSystemExplorer::OnDestroy(WISP_MSG *pMsg)
	{
		m_MainMenu.Destroy();
		return true;
	}

	bool CSystemExplorer::OnCmdMainSplit(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == WISP_ID_LBUTTON_DBLCLICK && pMsg->Command.Param1 == 0)
		{
			if (((WISP_SPLIT_WND*)pMsg->Command.Param2)->SizeXorY >= 125)
				m_MainSplitWnd.SetSize(0, 0);
			else	m_MainSplitWnd.SetSize(0, 250);
			m_MainSplitWnd.AdjustWndPos();
		}
		return true;
	}
	bool CSystemExplorer::OnCmdViewSplit(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == WISP_ID_LBUTTON_DBLCLICK && pMsg->Command.Param1 == 0)
		{
			int Size = (m_ClientRect.cy*2)/10;
			if (Size <= ((WISP_SPLIT_WND*)pMsg->Command.Param2)->SizeXorY)
			{
				m_ViewSplitWnd.SetSize(0, Size);
				m_ViewSplitWnd.AdjustWndPos();
			}
		}
		return true;
	}
	bool CSystemExplorer::OnCmdMoniteSplit(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == WISP_ID_RBUTTON_DBLCLICK && pMsg->Command.Param1 == 0)
		{
			if (((WISP_SPLIT_WND*)pMsg->Command.Param2)->SizeXorY > 300)
				m_MoniteSplitWnd.SetSize(0, 300);
			else	m_MoniteSplitWnd.SetSize(0, m_MoniteSplitWnd.m_ClientRect.cy - 5);
			m_MoniteSplitWnd.AdjustWndPos();
		} else
		if (pMsg->Command.CmdMsg == WISP_ID_LBUTTON_DBLCLICK && pMsg->Command.Param1 == 0)
		{
			if (((WISP_SPLIT_WND*)pMsg->Command.Param2)->SizeXorY >= 100)
				m_MoniteSplitWnd.SetSize(0, 0);
			else	m_MoniteSplitWnd.SetSize(0, 300);
			m_MoniteSplitWnd.AdjustWndPos();
		}
		return true;
	}

	bool CSystemExplorer::OnCmdGotoFunction(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == WISP_ID_LBUTTON_DBLCLICK)
		{
			ULONG_PTR Addr = m_FunctionListWnd.GetItemValue((CListStringItem *)pMsg->Command.Param2, CV_ADDR);
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.SetActiveAddress(Addr, true);
		}
		return true;
	}
//
	void CSystemExplorer::AdjustSplit()
	{
		if (!gSyserConfig.iSplitRight)
		{
			m_MainSplitWnd.SetSize(0, (m_ClientRect.cx*2) / 10);
			m_MainSplitWnd.SetSize(1, (m_ClientRect.cx*8) / 10);
		} else
		{
			m_MainSplitWnd.SetSize(0, (m_ClientRect.cx*8) / 10);
			m_MainSplitWnd.SetSize(1, (m_ClientRect.cx*2) / 10);
		}
		m_MainSplitWnd.AdjustWndPos();

		m_ViewSplitWnd.SetSize(0, (m_ClientRect.cy*3) / 20);
		m_ViewSplitWnd.SetSize(1, (m_ClientRect.cy*12) / 20);
		m_ViewSplitWnd.SetSize(2, (m_ClientRect.cy*5) / 20);
		m_ViewSplitWnd.AdjustWndPos();
	}
	bool CSystemExplorer::OnSize(WISP_MSG *pMsg)
	{
		m_Toolbar.Resize(pMsg->Sizing.Size.cx, m_Toolbar.m_WindowRect.cy, true);
		m_MainSplitWnd.Resize(m_ClientRect.cx, m_ClientRect.cy - m_MainMenu.m_WindowRect.cy - m_Toolbar.m_WindowRect.cy, true);
		AdjustSplit();
		return true;
	}
	bool CSystemExplorer::OnGetFocus(WISP_MSG *pMsg)
	{
		m_TerminalWnd.Focus();
		return false;
	}

	bool CSystemExplorer::OnEventOpen(WISP_MSG *pMsg)
	{
		if (!gpSyser->m_pDebugger)
			return true;

		char szFileName[MAX_FN_LEN];
	#if 0
		if (gpSyser->m_szDbgFileNameFilter.m_nLength)
		{
			OPENFILENAMEA ofn;
			memset(&ofn, 0, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hInstance = gpSyser->m_hInstance;
			ofn.hwndOwner = gpSyser->m_hWnd;

			char szFilter[MAX_FN_LEN];
			UnicodeToAnsi(gpSyser->m_szDbgFileNameFilter.operator const WCHAR*(), szFilter, MAX_FN_LEN);

			int Len = TStrLen(szFilter); //TStrLen(szFilter, MAX_FN_LEN);
			MAX_LIMIT(Len, MAX_FN_LEN-1);

			for (int i = Len; i >= 0; --i) //TStrReplace(szFilter, Len, '|', '\0');
			{
				if (szFilter[i] == '|')
					szFilter[i] = 0;
			}

			ofn.lpstrFilter = szFilter;
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_FN_LEN;
			szFileName[0] = 0;
			gpSyser->LockTimer();
			if (!::GetOpenFileNameA(&ofn))
			{
				gpSyser->UnlockTimer();
				return true;
			}
			gpSyser->UnlockTimer();

			TStrCpy(szFileName, ofn.lpstrFile);
		} else
		{
			szFileName[0] = 0;
		}
	#else
		if (!gpSyser->OpenFileDialog(szFileName))
			return true;
	#endif
		if (!gpSyser->m_SyserUI.m_SyserCodeDoc.Open(szFileName))
			gpSyser->m_MainFrame.m_SystemExplorer.m_TerminalWnd.Printf(WSTR("Fail to open file !\n"));
		return true;
	}

	bool CSystemExplorer::OnEventClose(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("close"), 0);
		return true;
	}

	bool CSystemExplorer::OnEventSaveComment(WISP_MSG *pMsg)
	{
		gpSyser->m_SyserUI.SaveComment();
		return true;
	}
	bool CSystemExplorer::OnEventLoadComment(WISP_MSG *pMsg)
	{
		gpSyser->m_SyserUI.LoadComment();
		return true;
	}
	bool CSystemExplorer::OnEventLoadSymbol(WISP_MSG *pMsg)
	{
		char szSDSDir[MAX_FN_LEN]; 
		char szSDSFullFileName[MAX_FN_LEN];

		OPENFILENAMEA ofn;
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn); //0x4C;
		ofn.hInstance = gpSyser->m_hInstance;
		ofn.hwndOwner = gpSyser->m_hWnd;
		ofn.lpstrFilter = "Symbol Files";
		ofn.nMaxFile = MAX_FN_LEN;
		ofn.lpstrFile = szSDSFullFileName;
		GetModulePath(szSDSDir, true);
		TStrCat(szSDSDir, "Symbols");
		szSDSFullFileName[0] = 0;
		ofn.lpstrInitialDir = szSDSDir;
		gpSyser->LockTimer();
		if (!::GetOpenFileNameA(&ofn))
		{
			gpSyser->UnlockTimer();
		} else
		{
			gpSyser->UnlockTimer();
			gpSyser->m_SyserUI.LoadSDSModule(szSDSFullFileName);
		}
		return true;
	}
	bool CSystemExplorer::OnEventNextPos(WISP_MSG *pMsg)
	{
		m_pCodeView->OnEventNextPos(pMsg);
		return true;
	}
	bool CSystemExplorer::OnEventPrevPos(WISP_MSG *pMsg)
	{
		m_pCodeView->OnEventPrevPos(pMsg);
		return true;
	}
	bool CSystemExplorer::OnEventSavePos(WISP_MSG *pMsg)
	{
		m_pCodeView->OnEventSavePos(pMsg);
		return true;
	}
	bool CSystemExplorer::OnEventResetPos(WISP_MSG *pMsg)
	{
		m_pCodeView->OnEventResetPos(pMsg);
		return true;
	}
	bool CSystemExplorer::OnEventDbgReset(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("reset"), 0);
		return true;
	}
	bool CSystemExplorer::OnEventDbgStep(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("t"), 0);
		return true;
	}
	bool CSystemExplorer::OnEventDbgProceed(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("p"), 0);
		return true;
	}
	bool CSystemExplorer::OnEventStepBranch(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("tb"), 0);
		return true;
	}
	bool CSystemExplorer::OnEventDbgReturn(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("p ret"), 0);
		return true;
	}
	bool CSystemExplorer::OnEventDbgContinue(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("x"), 0);
		return true;
	}
	bool CSystemExplorer::OnEventBreakPointForm(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_BreakPointForm.Create();
		return pMsg->Command.CmdMsg != 0x80002000;
	}

	bool CSystemExplorer::OnEventDebuggerSelectForm(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_DebuggerSelectForm.Create();
		return true;
	}

	bool CSystemExplorer::OnEventOptionForm(WISP_MSG *pMsg)
	{
		if (m_SyserOptionForm.IsWindow())
			m_SyserOptionForm.Show(WISP_SH_MINSIZE);
		else	m_SyserOptionForm.Create(0, WISP_SH_MINSIZE);
		return true;
	}

	bool CSystemExplorer::OnEventPluginListForm(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_PluginListForm.Create();
		return true;
	}

	bool CSystemExplorer::OnEventTabGeneReg(WISP_MSG *pMsg)
	{
		CWispTabWnd *pWnd = (CWispTabWnd *)m_RegTabWnd.GetActiveWnd();
		if (pWnd) pWnd->SetActiveWnd(CV_REG_GENE_TAB); //CV_REG_GENE_TAB(0)
		return false;
	}
	bool CSystemExplorer::OnEventTabFPUReg(WISP_MSG *pMsg)
	{
		CWispTabWnd *pWnd = (CWispTabWnd *)m_RegTabWnd.GetActiveWnd();
		if (pWnd) pWnd->SetActiveWnd(CV_REG_FPU_TAB); //CV_REG_FPU_TAB(1)
		return false;
	}
	bool CSystemExplorer::OnEventTabSSEReg(WISP_MSG *pMsg)
	{
		CWispTabWnd *pWnd = (CWispTabWnd *)m_RegTabWnd.GetActiveWnd();
		if (pWnd) pWnd->SetActiveWnd(CV_REG_SSE_TAB); //CV_REG_SSE_TAB(2)
		return false;
	}

	bool CSystemExplorer::OnEventWatch(WISP_MSG *pMsg)
	{
		m_ContextTabWnd.SetActiveWnd(CV_W_TAB); //CV_W_TAB(1)
		return false;
	}
	bool CSystemExplorer::OnEventStack(WISP_MSG *pMsg)
	{
		m_ContextTabWnd.SetActiveWnd(CV_S_TAB); //CV_S_TAB(0)
		return false;
	}
	bool CSystemExplorer::OnEventIDT(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_IDTWnd.Create();
		return true;
	}
	bool CSystemExplorer::OnEventGDT(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_GDTWnd.Create();
		return true;
	}
	bool CSystemExplorer::OnEventLDT(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_GDTWnd.Create();
		return true;
	}
	bool CSystemExplorer::OnEventProcessList(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ProcessList.Create();
		return true;
	}
	bool CSystemExplorer::OnEventThreadList(WISP_MSG *pMsg)
	{
		//m_ThreadListWnd.Create(); TODO
		return true;
	}
	bool CSystemExplorer::OnEventModuleList(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ModuleListWnd.Create(false);
		return true;
	}
	bool CSystemExplorer::OnEventFunctionList(WISP_MSG *pMsg)
	{
		if (m_FunctionListWnd.IsWindow())
		{
			m_FunctionListWnd.Show(WISP_SH_MINSIZE);
		} else
		{
			m_FunctionListWnd.CreateWnd(WSTR("Functions"), 0,0,500,350, 0, 0x45, 0x100BB, WISP_SH_MINSIZE);
			m_FunctionListWnd.SetOwner(this);
			m_FunctionListWnd.m_ItemColor = gSyserColors.Color[1];
			m_FunctionListWnd.m_Style |= 0x1000000;
			m_FunctionListWnd.SetBGColor(gSyserColors.Color[0]);
			m_FunctionListWnd.InsertColumn(WSTR("Function name"), 240, 0, 0, -1);
			m_FunctionListWnd.InsertColumn(WSTR("Start"), 80, 0, 0, -1);
			m_FunctionListWnd.InsertColumn(WSTR("Length"), 80, 0, 0, -1);
			m_FunctionListWnd.InsertColumn(WSTR("BP based frame"), 80, 0, 0, -1);
		}
		m_FunctionListWnd.Center(0);
		return true;
	}
	bool CSystemExplorer::OnEventHwndList(WISP_MSG *pMsg)
	{
		//TODO gpSyser->m_MainFrame.m_HwndListWnd.Create();
		return true;
	}
	bool CSystemExplorer::OnEventRunTrace(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_RunTraceWnd.Create();
		return true;
	}
	bool CSystemExplorer::OnEventSeachDialog(WISP_MSG *pMsg)
	{
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
			m_SearchDialog.Create(nullptr, 1);
		return true;
	}
	bool CSystemExplorer::OnEventCommentList(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_CommentList.Create();
		return true;
	}
	bool CSystemExplorer::OnEventPageMap(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_PageMapWnd.Create();
		return true;
	}
	bool CSystemExplorer::OnEventRegHelp(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_X86RegHelpTabWnd.Create();
		return true;
	}

	bool CSystemExplorer::OnEventAboutForm(WISP_MSG *pMsg)
	{
		CAboutForm AboutForm;
		AboutForm.Create(0, WISP_SH_MODAL_BLOCK);
		return true;
	}

	bool CSystemExplorer::OnEventCheckNew(WISP_MSG *pMsg)
	{
		//TODO
		return true;
	}
	bool CSystemExplorer::OnEventObjDirWnd(WISP_MSG *pMsg)
	{
		//TODO
		//m_ObjectDirectoryWnd.Create();
		return true;
	}

	bool CSystemExplorer::OnEventCalc(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_CalcWnd.Create();
		return true;
	}

	bool CSystemExplorer::OnEventCallStack(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_CallStackWnd.Create();
		return true;
	}

	bool CSystemExplorer::OnEventRegView(WISP_MSG *pMsg)
	{
		if (!gpSyser->m_MainFrame.m_pGeneRegList)
			gpSyser->m_MainFrame.m_pGeneRegList = new CGeneRegList[dwCPUNumbers];

		int nActiveCPU = GetActiveCPU();

		if (gpSyser->m_MainFrame.m_CPUTabWnd.IsWindow())
		{
			gpSyser->m_MainFrame.m_CPUTabWnd.Destroy();
			for (int i = 0; i < dwCPUNumbers; ++i)
			{
				gpSyser->m_MainFrame.m_pGeneRegList[i].Destroy();
			}
			return true;
		}

		WISP_RECT Rect;
		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 320;
		Rect.cy = 210;
		
		gpSyser->m_MainFrame.m_CPUTabWnd.CreateWnd(WSTR("Register View"), Rect, 0, 0, 0x200BB, WISP_SH_MINSIZE);

		for (int nCPU = 0; nCPU < dwCPUNumbers; ++nCPU)
		{
			Rect.x = 0;
			Rect.y = 0;
			Rect.cx = 0;
			Rect.cy = 0;
			if (!gpSyser->m_MainFrame.m_pGeneRegList[nCPU].CreateWnd(0, Rect, &gpSyser->m_MainFrame.m_CPUTabWnd, CMD_ID_GENE_REG, 0x1030000, WISP_SH_MINSIZE))
				return false;
			gpSyser->m_MainFrame.m_pGeneRegList[nCPU].m_nCPU = nCPU;

			WCHAR szCPUNum[20];
			if (nCPU == nActiveCPU)
				TSPrintf(szCPUNum, WSTR("*CPU %d "), nCPU);
			else	TSPrintf(szCPUNum, WSTR(" CPU %d "), nCPU);

			gpSyser->m_MainFrame.m_CPUTabWnd.InsertWnd(szCPUNum, &gpSyser->m_MainFrame.m_pGeneRegList[nCPU], 0);
			gpSyser->m_MainFrame.m_pGeneRegList[nCPU].UpdateContext();
		}
		return true;
	}
	bool CSystemExplorer::OnEventDataViewForm(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_DataViewForm.Create();
		return true;
	}
	bool CSystemExplorer::OnEventSoftKB(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_SoftKeyboard.Create();
		return true;
	}
	bool CSystemExplorer::OnEventKeyMap(WISP_MSG *pMsg)
	{
		//TODO
		return true;
	}
	bool CSystemExplorer::OnEventPEExplorerForm(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_PEExplorerForm.Create();
		return true;
	}
	bool CSystemExplorer::OnEventIBM(WISP_MSG *pMsg)
	{
		//TODO
		return true;
	}
	bool CSystemExplorer::OnEventEBCDI(WISP_MSG *pMsg)
	{
		//TODO
		return true;
	}
	bool CSystemExplorer::OnEventANSI(WISP_MSG *pMsg)
	{
		//TODO
		return true;
	}

	bool CSystemExplorer::OnEventToggleBM(WISP_MSG *pMsg)
	{
		m_MultiCodeView.ToggleBM();
		return true;
	}
	bool CSystemExplorer::OnEventNextBM(WISP_MSG *pMsg)
	{
		m_MultiCodeView.NextBM();
		return true;
	}
	bool CSystemExplorer::OnEventPrevBM(WISP_MSG *pMsg)
	{
		m_MultiCodeView.PrevBM();
		return true;
	}
	bool CSystemExplorer::OnEventClearBM(WISP_MSG *pMsg)
	{
		m_MultiCodeView.ClearBM();
		return true;
	}

	bool CSystemExplorer::OnEventSEH(WISP_MSG *pMsg)
	{
#ifndef _X64_
		m_SEHChainWnd.Create();
#endif
		return true;
	}
	bool CSystemExplorer::OnEventFile(WISP_MSG *pMsg)
	{
		m_MainMenu.Focus();
		CWispMenu::MenuItem *Item = m_MainMenu.GetItem(WSTR("File"));
		if (Item)
		{
			m_MainMenu.UpdateItem(Item);
			WISP_MSG Msg;
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_RETURN;
			m_MainMenu.OnKeyEvent(&Msg);
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_DOWN;
			m_MainMenu.OnKeyEvent(&Msg);
		}
		return true;
	}
	bool CSystemExplorer::OnEventDebug(WISP_MSG *pMsg)
	{
		m_MainMenu.Focus();
		CWispMenu::MenuItem *Item = m_MainMenu.GetItem(WSTR("Debug"));
		if (Item)
		{
			m_MainMenu.UpdateItem(Item);
			WISP_MSG Msg;
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_RETURN;
			m_MainMenu.OnKeyEvent(&Msg);
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_DOWN;
			m_MainMenu.OnKeyEvent(&Msg);
		}
		return true;
	}
	bool CSystemExplorer::OnEventMonitor(WISP_MSG *pMsg)
	{
		m_MainMenu.Focus();
		CWispMenu::MenuItem *Item = m_MainMenu.GetItem(WSTR("Monitor"));
		if (Item)
		{
			m_MainMenu.UpdateItem(Item);
			WISP_MSG Msg;
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_RETURN;
			m_MainMenu.OnKeyEvent(&Msg);
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_DOWN;
			m_MainMenu.OnKeyEvent(&Msg);
		}
		return true;
	}
	bool CSystemExplorer::OnEventView(WISP_MSG *pMsg)
	{
		m_MainMenu.Focus();
		CWispMenu::MenuItem *Item = m_MainMenu.GetItem(WSTR("View"));
		if (Item)
		{
			m_MainMenu.UpdateItem(Item);
			WISP_MSG Msg;
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_RETURN;
			m_MainMenu.OnKeyEvent(&Msg);
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_DOWN;
			m_MainMenu.OnKeyEvent(&Msg);
		}
		return true;
	}
	bool CSystemExplorer::OnEventInformation(WISP_MSG *pMsg)
	{
		m_MainMenu.Focus();
		CWispMenu::MenuItem *Item = m_MainMenu.GetItem(WSTR("Information"));
		if (Item)
		{
			m_MainMenu.UpdateItem(Item);
			WISP_MSG Msg;
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_RETURN;
			m_MainMenu.OnKeyEvent(&Msg);
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_DOWN;
			m_MainMenu.OnKeyEvent(&Msg);
		}
		return true;
	}
	bool CSystemExplorer::OnEventOption(WISP_MSG *pMsg)
	{
		m_MainMenu.Focus();
		CWispMenu::MenuItem *Item = m_MainMenu.GetItem(WSTR("Option"));
		if (Item)
		{
			m_MainMenu.UpdateItem(Item);
			WISP_MSG Msg;
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_RETURN;
			m_MainMenu.OnKeyEvent(&Msg);
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_DOWN;
			m_MainMenu.OnKeyEvent(&Msg);
		}
		return true;
	}
	bool CSystemExplorer::OnEventTools(WISP_MSG *pMsg)
	{
		m_MainMenu.Focus();
		CWispMenu::MenuItem *Item = m_MainMenu.GetItem(WSTR("Tools"));
		if (Item)
		{
			m_MainMenu.UpdateItem(Item);
			WISP_MSG Msg;
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_RETURN;
			m_MainMenu.OnKeyEvent(&Msg);
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_DOWN;
			m_MainMenu.OnKeyEvent(&Msg);
		}
		return true;
	}
	bool CSystemExplorer::OnEventHelp(WISP_MSG *pMsg)
	{
		m_MainMenu.Focus();
		CWispMenu::MenuItem *Item = m_MainMenu.GetItem(WSTR("Help"));
		if (Item)
		{
			m_MainMenu.UpdateItem(Item);
			WISP_MSG Msg;
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_RETURN;
			m_MainMenu.OnKeyEvent(&Msg);
			Msg.KeyEvent.bKeyDown = true;
			Msg.KeyEvent.KeyType = VK_DOWN;
			m_MainMenu.OnKeyEvent(&Msg);
		}
		return true;
	}

	bool CSystemExplorer::OnEventDataShow(WISP_MSG *pMsg)
	{
		CWispBaseWnd *pWnd = gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveWnd();
		if (pWnd) pWnd->Focus();
		return true;
	}
	bool CSystemExplorer::OnEventCodeShow(WISP_MSG *pMsg)
	{
		CWispBaseWnd *pWnd = gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
		if (pWnd) pWnd->Focus();
		return true;
	}

	bool CSystemExplorer::OnEventStrRef(WISP_MSG *pMsg)
	{
		if (gpSyser->m_MainFrame.m_FindListWnd.IsWindow())
			gpSyser->m_MainFrame.m_FindListWnd.Destroy();
		gpSyser->m_MainFrame.m_FindListWnd.CreateWnd(WSTR("String Reference"), 0,0,564,344, 0, EVENT_ID_STRFIND, 0x300BB, WISP_SH_MINSIZE);
		return true;
	}

	bool CSystemExplorer::OnEventNextCVTrace(WISP_MSG *pMsg)
	{
		CTracePoint *pTrace = gpSyser->m_MainFrame.m_SystemExplorer.NextCIP();
		if (!pTrace) return false;
		CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
		if (pWnd)
		{
			pWnd->GotoAddr(pTrace->Address, false);
			pWnd->SelectAddr(pTrace->Address + pTrace->Offset);
		}
		return true;
	}
	bool CSystemExplorer::OnEventPrevCVTrace(WISP_MSG *pMsg)
	{
		CTracePoint *pTrace = gpSyser->m_MainFrame.m_SystemExplorer.PrevCIP();
		if (!pTrace) return false;
		CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
		if (pWnd)
		{
			pWnd->GotoAddr(pTrace->Address, false);
			pWnd->SelectAddr(pTrace->Address + pTrace->Offset);
		}
		return true;
	}

	bool CSystemExplorer::OnEventBPEnableAll(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("be *"), 0);
		return true;
	}
	bool CSystemExplorer::OnEventBPRemoveAll(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("bc *"), 0);
		return true;
	}
	bool CSystemExplorer::OnEventBPDisableAll(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("bd *"), 0);
		return true;
	}

	bool CSystemExplorer::OnEventLoadHistory(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdID >= EVENT_ID_FILE1 && pMsg->Command.CmdID <= EVENT_ID_FILE8)
		{
			TListIter<RECENT_HISTORY_ITEM> It = gpSyser->m_HistoryList[pMsg->Command.CmdID-EVENT_ID_FILE1];
			if (It != gpSyser->m_HistoryList.End())
			{
				gpSyser->m_SyserUI.m_SyserCodeDoc.Close();
				gpSyser->m_SyserUI.m_SyserCodeDoc.Open(It->FileName.operator const char*());
			}
		}
		return true;
	}

	bool CSystemExplorer::OnEventTypeViewerWnd(WISP_MSG *pMsg)
	{
		m_pTypeViewerWnd = new CTypeViewerWnd;
		WISP_RECT Rect;
		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		m_pTypeViewerWnd->CreateWnd(0, Rect, &m_ContextTabWnd, 0, 0x30006, 1); //SHOW_SH_MINSIZE
		m_pTypeViewerWnd->m_AdvStyle |= 1; //????
		CWispDIB *pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0xE2);
		int Index = m_ContextTabWnd.InsertWnd(WSTR("Struct Viewer"), m_pTypeViewerWnd, pDIB);
		m_ContextTabWnd.SetActiveWnd(Index);
		return true;
	}

	void CSystemExplorer::ResetContext()
	{
		for (int i = 0; i < dwCPUNumbers; ++i)
		{
			m_pGeneRegList[i].ResetContext();
			m_pFpuRegisterList[i].ResetContext();
			m_pSSERegisterList[i].ResetContext();
		}
		m_StackWatchList.ResetContext();
		m_WatchList.ResetContext();
		m_MultiCodeView.ResetContext();
		m_MultiDataView.ResetContext();
		if (m_pTypeViewerWnd)
			m_pTypeViewerWnd->ResetContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_TypeViewerWnd.ResetContext();
		m_pInstrInfo->SetWindowText(WSTR(""));
		m_InfoStr.SetWindowText(WSTR(""));
	}

	void CSystemExplorer::UpdateMenu()
	{
		m_Toolbar.Enable(EVENT_ID_OPEN, !gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_OPEN, !gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_CLOSE, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_CLOSE, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_DBG_RESET, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_DBG_RESET, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_GO, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_GO, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_DBG_RETURN, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_DBG_RETURN, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_DBG_STEP, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_DBG_STEP, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_DBG_PROCEED, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_DBG_PROCEED, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_DBG_STEPB, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_DBG_STEPB, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_SAVECMT, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_SAVECMT, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_LOADCMT, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_LOADCMT, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_FILE0, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_FILE0, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_SAVEPOS, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_SAVEPOS, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_PREVPOS, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && m_pCodeView->m_HistoryIter != m_pCodeView->m_History.Begin());
		m_MainMenu.Enable(EVENT_ID_PREVPOS, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && m_pCodeView->m_HistoryIter != m_pCodeView->m_History.Begin());

		m_Toolbar.Enable(EVENT_ID_NEXTPOS, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && m_pCodeView->m_HistoryIter != m_pCodeView->m_History.End()-1);
		m_MainMenu.Enable(EVENT_ID_NEXTPOS, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && m_pCodeView->m_HistoryIter != m_pCodeView->m_History.End()-1);

		m_Toolbar.Enable(EVENT_ID_CLRPOS, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && m_pCodeView->m_History.Size()>1);
		m_MainMenu.Enable(EVENT_ID_CLRPOS, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && m_pCodeView->m_History.Size()>1);

		m_Toolbar.Enable(EVENT_ID_TOGGLE_BM, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_TOGGLE_BM, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		//Bookmark
		m_Toolbar.Enable(EVENT_ID_NEXT_BM, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && m_MultiCodeView.m_Bookmark.size()>0);
		m_MainMenu.Enable(EVENT_ID_NEXT_BM, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && m_MultiCodeView.m_Bookmark.size()>0);

		m_Toolbar.Enable(EVENT_ID_PREV_BM, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && m_MultiCodeView.m_Bookmark.size()>0);
		m_MainMenu.Enable(EVENT_ID_PREV_BM, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && m_MultiCodeView.m_Bookmark.size()>0);

		m_Toolbar.Enable(EVENT_ID_CLEAR_BM, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && m_MultiCodeView.m_Bookmark.size()>0);
		m_MainMenu.Enable(EVENT_ID_CLEAR_BM, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && m_MultiCodeView.m_Bookmark.size()>0);

		m_Toolbar.Update((WISP_RECT*)0);
	}

	void CSystemExplorer::UpdateContext()
	{
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		{
			int nActiveCPU = GetActiveCPU();

			CWispBaseWnd *pFocusWnd = m_pWispBase->m_pFocusWnd;

			m_RegTabWnd.SetActiveWnd(nActiveCPU);

			WCHAR Buffer[128];
			TSPrintf(Buffer, WSTR("PID:%x TID:%x "), gpSyser->m_PID, gpSyser->m_TID);
			m_InfoStr.SetWindowText(Buffer);

			for (int nCPU = 0; nCPU < dwCPUNumbers; ++nCPU)
			{
				m_pGeneRegList[nCPU].UpdateContext();
				m_pFpuRegisterList[nCPU].UpdateContext();
				m_pSSERegisterList[nCPU].UpdateContext();

				if (dwCPUNumbers < 2 || m_pGeneRegList[nCPU].m_nCPU != nActiveCPU)
					TSPrintf(Buffer, WSTR(" CPU %d "), nCPU);
				else	TSPrintf(Buffer, WSTR("*CPU %d "), nCPU);

				m_RegTabWnd.SetTabText(nCPU, Buffer);

				//RegView
				if (gpSyser->m_MainFrame.m_pGeneRegList && gpSyser->m_MainFrame.m_pGeneRegList[nCPU].IsWindow())
				{
					gpSyser->m_MainFrame.m_pGeneRegList[nCPU].UpdateContext();
					if (nCPU == nActiveCPU)
						gpSyser->m_MainFrame.m_CPUTabWnd.SetActiveWnd(nActiveCPU);
					gpSyser->m_MainFrame.m_CPUTabWnd.SetTabText(nCPU, Buffer);
				}
			}
	
			m_WatchList.UpdateContext();
			m_StackWatchList.UpdateContext(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP, false);
			m_MultiCodeView.UpdateContext();
			m_MultiDataView.UpdateContext();
			if (m_pTypeViewerWnd)
				m_pTypeViewerWnd->UpdateContext();
			gpSyser->m_SyserUI.m_SyserCodeDoc.GetInstrInfo(Buffer);
			m_pInstrInfo->SetWindowText(Buffer);

			gpSyser->m_MainFrame.m_DataViewForm.UpdateContext();
			gpSyser->m_MainFrame.m_RunTraceWnd.UpdateContext();
			pFocusWnd->Focus();
			Update((WISP_RECT*)0);
		}
	}

	void CSystemExplorer::UpdateRegView()
	{
		CCodeView *pWnd = (CCodeView *)m_MultiCodeView.GetActiveWnd();
		if (pWnd)
		{
			int nWndIndex = m_MultiCodeView.GetActiveWndIndex();
			unsigned long Index = m_RunTrace.UpdateContext(pWnd->m_CurAddr, *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP, nWndIndex);
			m_CIPRegWndTrace = Index;
			m_CIPTrace = Index;

			for (int nCPU = 0; nCPU < m_nCPUNumbers; ++nCPU)
			{
				m_pGeneRegList[nCPU].EnableWindow(true);
			}
		}
	}

	void CSystemExplorer::ViewCodeAddress(ULONG_PTR Address)
	{
		if (gpSyser->m_pDebugger->m_CodeMode != gpSyser->m_SyserUI.m_SyserCodeDoc.m_Mode)
			gpSyser->m_SyserUI.SetCodeMode(gpSyser->m_pDebugger->m_CodeMode);

		CCodeView *pWnd = (CCodeView *)m_MultiCodeView.GetActiveWnd();
		if (pWnd)
		{
			ULONG_PTR Start;
			ULONG_PTR End;
			pWnd->GetCodeRange(&Start, &End);
			WCHAR Buffer[64];
			TSPrintf(Buffer, WSTR("" F0ADDR "X"), Address);
			m_MultiCodeView.SetTabText(m_MultiCodeView.GetActiveWndIndex(), Buffer);
			if (Address < Start || Address >= End)
			{
				pWnd->GotoAddr(Address, false);
			} else
			if (!gpSyser->m_SyserUI.m_SyserCodeDoc.IsDasm(Start, End - Start))
			{
				pWnd->GotoAddr(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP, false);
			}
		}
	}

	void CSystemExplorer::ResetView()
	{
		if (gpSyser->m_bActive)
		{
			m_MultiCodeView.ResetContext();
			m_MultiDataView.ResetContext();
		}
	}

	void CSystemExplorer::SelectCodeAddress(ULONG_PTR Address)
	{
		m_pCodeView->SelectAddress(Address);
	}

	void CSystemExplorer::CodeGotoAddr(ULONG_PTR Address, bool bHistory)
	{
		m_pCodeView->GotoAddr(Address, bHistory);
	}

	void CSystemExplorer::DataGotoAddr(ULONG_PTR Address, unsigned long Size)
	{
		CDataView *pWnd = (CDataView *)m_MultiDataView.GetActiveWnd();
		if (pWnd)
		{
			if (Size>0)
			{
				pWnd->m_bSelect = true;
				pWnd->m_SelStart = Address;
				pWnd->m_SelEnd = Address + Size - 1;
			} else
			{
				pWnd->m_bSelect = false;
			}
			pWnd->ViewAddress(Address, true);
		}
	}

	CTracePoint *CSystemExplorer::PrevCIP()
	{
		if (m_CIPTrace == 1)
			return nullptr;
		--m_CIPTrace;

		for (int nCPU = 0; nCPU < m_nCPUNumbers; ++nCPU)
		{
			if (!((m_pGeneRegList[nCPU].m_Style >> 9)&1))
				m_pGeneRegList[nCPU].EnableWindow(false);
		}

		if (m_CIPTrace == m_CIPRegWndTrace)
		{
			for (int nCPU = 0; nCPU < m_nCPUNumbers; ++nCPU)
			{
				m_pGeneRegList[nCPU].EnableWindow(true);
			}
		}

		CTracePoint *pTrace = m_RunTrace.GetTracePoint(m_CIPTrace-1);
		if (pTrace)
		{
			ULONG_PTR Addr = pTrace->Address + pTrace->Offset;
			unsigned long CPUType;
			X86_CPU_REG CpuREG;
			m_RunTrace.ReadContext(m_CIPTrace - 1, &CpuREG, &CPUType);
			CWispTabWnd *pWnd = (CWispTabWnd *)m_RegTabWnd.GetActiveWnd();
			if (pWnd)
			{
				CGeneRegList *pRegWnd = (CGeneRegList *)pWnd->GetWnd(0);
				pRegWnd->UpdateContextTrace(CPUType, &CpuREG, Addr);
				pRegWnd->Update((WISP_RECT*)0);
			}
		}
		return pTrace;
	}

	CTracePoint *CSystemExplorer::NextCIP()
	{
		if (m_CIPTrace == m_CIPRegWndTrace)
			return nullptr;
		++m_CIPTrace;
		if (m_CIPTrace == m_CIPRegWndTrace)
		{
			for (int nCPU = 0; nCPU < m_nCPUNumbers; ++nCPU)
			{
				m_pGeneRegList[nCPU].EnableWindow(true);
			}
		} else
		{
			for (int nCPU = 0; nCPU < m_nCPUNumbers; ++nCPU)
			{
				if (!((m_pGeneRegList[nCPU].m_Style >> 9)&1))
					m_pGeneRegList[nCPU].EnableWindow(false);
			}
		}

		CTracePoint *pTrace = m_RunTrace.GetTracePoint(m_CIPTrace-1);
		if (pTrace)
		{
			ULONG_PTR Addr = pTrace->Address + pTrace->Offset;
			unsigned long CPUType;
			X86_CPU_REG CpuREG;
			m_RunTrace.ReadContext(m_CIPTrace - 1, &CpuREG, &CPUType);
			CWispTabWnd *pWnd = (CWispTabWnd *)m_RegTabWnd.GetActiveWnd();
			if (pWnd)
			{
				CGeneRegList *pRegWnd = (CGeneRegList *)pWnd->GetWnd(0);
				pRegWnd->UpdateContextTrace(CPUType, &CpuREG, Addr);
				pRegWnd->Update((WISP_RECT*)0);
			}
		}
		return pTrace;
	}

	//from RunTraceWnd
	bool CSystemExplorer::TracePoint(unsigned int TracePoint)
	{
		m_CIPTrace = TracePoint + 2;
		CTracePoint *pTrace = PrevCIP();
		if (!pTrace) return false;
		CCodeView *pWnd = (CCodeView *)m_MultiCodeView.GetActiveWnd();
		if (!pWnd) return false;
		pWnd->GotoAddr(pTrace->Address, false);
		pWnd->SelectAddr(pTrace->Address + pTrace->Offset);
		return true;
	}

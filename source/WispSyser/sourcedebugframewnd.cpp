
#include "sourcedebugframewnd.hpp"
#include "resource.hpp"
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"

	CSourceDebugFrameWnd::CSourceDebugFrameWnd()
	{
	}
	CSourceDebugFrameWnd::~CSourceDebugFrameWnd()
	{
	}

	WISP_MSG_MAP_BEGIN(CSourceDebugFrameWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP(WISP_WM_GET_FOCUS, OnGetFocus)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispSplitWnd)

	bool CSourceDebugFrameWnd::OnCreate(WISP_MSG *pMsg)
	{
		m_DIBList.Load("\\SyserIcon\\SourceCodeIcon.bmp", 16, 16, 80);

		m_MainMenu.LoadMenuGroup(this, WISP_ID_MAIN_MENU, SourceDebugMainMenu, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF));
		m_MainMenu.SetOwner(this);

		m_Toolbar.CreateEx(this, m_MainMenu.m_WindowRect.cy, -1, WISP_ID_MAIN_TOOLBAR, 0);
		m_Toolbar.LoadToolbar(gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF), SourceDebugToolbar);
		m_Toolbar.SetOwner(this);

		WISP_RECT Rect;
		Rect.x = 0;
		Rect.y = m_MainMenu.m_WindowRect.cy + m_Toolbar.m_WindowRect.cy;
		Rect.cx = m_ClientRect.cx;
		Rect.cy = m_ClientRect.cy - Rect.y;
		if (!m_MainSplitWnd.CreateWnd(0, Rect, this, CMD_ID_SOURCE_MAIN_SPLIT, 0x10000, WISP_SH_MINSIZE))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_ViewSplitWnd.CreateWnd(0, Rect, &m_MainSplitWnd, CMD_ID_SOURCE_VIEW_PLIT, 0x20000, WISP_SH_MINSIZE))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_SDSModuleListTabWnd.CreateWnd(0, Rect, &m_ViewSplitWnd, 0, 0x20000, WISP_SH_MINSIZE))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_MultiSourceCodeView.CreateWnd(0, Rect, &m_ViewSplitWnd, CMD_ID_SOURCE, 0x200000, WISP_SH_MINSIZE))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_SDSModuleList.CreateWnd(0, Rect, &m_SDSModuleListTabWnd, CMD_ID_SDS, 0x1030000, WISP_SH_MINSIZE))
			return false;

		m_SDSModuleListTabWnd.InsertWnd(WSTR("SDS Modules ALT+S"), &m_SDSModuleList, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0xD5));

		m_ViewSplitWnd.InsertWnd(&m_MultiSourceCodeView, 0, (m_ClientRect.cx/3)*2, -1);
		m_ViewSplitWnd.InsertWnd(&m_SDSModuleListTabWnd, 0, (m_ClientRect.cx/3), -1);
		m_ViewSplitWnd.AdjustWndPos();

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_ContextTabWnd.CreateWnd(0, Rect, &m_MainSplitWnd, 0, 0x10000, WISP_SH_MINSIZE))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_TypeViewerWnd.CreateWnd(0, Rect, &m_ContextTabWnd, CMD_ID_WATCH, 0x1030000, WISP_SH_MINSIZE))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_TerminalWnd.CreateWnd(0, Rect, &m_ContextTabWnd, CMD_ID_TERMWND, 0, WISP_SH_MINSIZE))
			return false;

		m_TerminalWnd.AttachConsole(&gpSyser->m_MainFrame.m_ConsoleWnd);

		m_ContextTabWnd.InsertWnd(WSTR("Watch ALT+W"), &m_TypeViewerWnd, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x35));
		m_ContextTabWnd.InsertWnd(WSTR("Console F1"), &m_TerminalWnd, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x3F));

		m_MainSplitWnd.InsertWnd(&m_ViewSplitWnd, 0, m_ClientRect.cy - 200, -1);
		m_MainSplitWnd.InsertWnd(&m_ContextTabWnd, 0, 200, -1);
		m_MainSplitWnd.AdjustWndPos();

//		gpSyser->RegisterHotKeyEvent(EVENT_ID_GO, this);
//		gpSyser->RegisterHotKeyEvent(EVENT_ID_DBG_STEP, this);
//		gpSyser->RegisterHotKeyEvent(EVENT_ID_DBG_PROCEED, this);
//		gpSyser->RegisterHotKeyEvent(EVENT_ID_DBG_STEPB, this); //???
//		gpSyser->RegisterHotKeyEvent(EVENT_ID_DBG_RETURN, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_TOGGLE_BM, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_NEXT_BM, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_PREV_BM, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_CLEAR_BM, this);

		RegisterKeyEvent(EVENT_ID_SDS_FOCUS, 'S', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_WATCH_LIST, 'W', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_SRC_CONSOLE, 'p', 0, 1);
		RegisterKeyEvent(EVENT_ID_SOURCECODEWND_GOTOLINE, 'G', WISP_SK_CTRL, 1);
		RegisterKeyEvent(EVENT_ID_MENU_DEBUG, 'D', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_MENU_MONITOR, 'M', WISP_SK_ALT, 1);
		RegisterKeyEvent(EVENT_ID_MENU_VIEW, 'V', WISP_SK_ALT, 1);
		return true;
	}

	bool CSourceDebugFrameWnd::OnDestroy(WISP_MSG *pMsg)
	{
		m_DIBList.Destroy();
		return true;
	}

	WISP_CMD_MAP_BEGIN(CSourceDebugFrameWnd)
//		WISP_CMD_MAP(..., ...)
	WISP_CMD_MAP_END

	WISP_EVENT_MAP_BEGIN(CSourceDebugFrameWnd)
		WISP_EVENT_MAP(EVENT_ID_DBG_STEP, OnEventDbgStep)
		WISP_EVENT_MAP(EVENT_ID_DBG_PROCEED, OnEventDbgProceed)

		WISP_EVENT_MAP(EVENT_ID_DBG_RETURN, OnEventDbgReturn)
		WISP_EVENT_MAP(EVENT_ID_GO, OnEventDbgContinue)
		WISP_EVENT_MAP(EVENT_ID_BREAK_POINT, OnEventBreakPointForm)
		WISP_EVENT_MAP(EVENT_ID_SDS_FOCUS, OnEventSDSModuleList)
		WISP_EVENT_MAP(EVENT_ID_WATCH_LIST, OnEventWatchView)
		WISP_EVENT_MAP(EVENT_ID_SRC_CONSOLE, OnEventTerminal)
		WISP_EVENT_MAP(EVENT_ID_CALLSTACK, OnEventCallStack)
		WISP_EVENT_MAP(EVENT_ID_REGVIEW, OnEventRegView)
		WISP_EVENT_MAP(EVENT_ID_DATAVIEW, OnEventDataViewForm)

		WISP_EVENT_MAP(EVENT_ID_TOGGLE_BM, OnEventToggleBM)
		WISP_EVENT_MAP(EVENT_ID_NEXT_BM, OnEventNextBM)
		WISP_EVENT_MAP(EVENT_ID_PREV_BM, OnEventPrevBM)
		WISP_EVENT_MAP(EVENT_ID_CLEAR_BM, OnEventClearBM)
		
		WISP_EVENT_MAP(EVENT_ID_MENU_DEBUG, OnEventMenuDebug)
		WISP_EVENT_MAP(EVENT_ID_MENU_MONITOR, OnEventMenuMonitor)
		WISP_EVENT_MAP(EVENT_ID_MENU_VIEW, OnEventMenuView)
		WISP_EVENT_MAP(EVENT_ID_SOURCECODEWND_GOTOLINE, OnEventGotoLine)
//		WISP_EVENT_MAP(..., ...)
	WISP_EVENT_MAP_END

	void CSourceDebugFrameWnd::AdjustSplit()
	{
		m_MainSplitWnd.SetSize(0, (m_ClientRect.cy*7) / 10);
		m_MainSplitWnd.AdjustWndPos();

		m_ViewSplitWnd.SetSize(0, (m_ClientRect.cx*7) / 10);
		m_ViewSplitWnd.AdjustWndPos();
	}

	bool CSourceDebugFrameWnd::OnSize(WISP_MSG *pMsg)
	{
		m_Toolbar.Resize(pMsg->Sizing.Size.cx, m_Toolbar.m_WindowRect.cy, true);
		m_MainSplitWnd.Resize(m_ClientRect.cx, m_ClientRect.cy - m_MainMenu.m_WindowRect.cy - m_Toolbar.m_WindowRect.cy, true);
		AdjustSplit();
		return true;
	}

	bool CSourceDebugFrameWnd::OnEventDbgStep(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("st"), 0);
		return false;
	}
	bool CSourceDebugFrameWnd::OnEventDbgProceed(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("sp"), 0);
		return false;
	}
	bool CSourceDebugFrameWnd::OnEventDbgReturn(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("p ret"), 0);
		return false;
	}
	bool CSourceDebugFrameWnd::OnEventDbgContinue(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("x"), 0);
		return false;
	}
	bool CSourceDebugFrameWnd::OnEventBreakPointForm(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_BreakPointForm.Create();
		return true;
	}
	bool CSourceDebugFrameWnd::OnEventSDSModuleList(WISP_MSG *pMsg)
	{
		m_SDSModuleList.Focus();
		return true;
	}
	bool CSourceDebugFrameWnd::OnEventWatchView(WISP_MSG *pMsg)
	{
		m_ContextTabWnd.SetActiveWnd(SRC_W_TAB); //SRC_W_TAB(0)
		m_TypeViewerWnd.Focus();
		return true;
	}
	bool CSourceDebugFrameWnd::OnEventTerminal(WISP_MSG *pMsg)
	{
		m_ContextTabWnd.SetActiveWnd(SRC_T_TAB); //SRC_T_TAB(1)
		m_TerminalWnd.Focus();
		return true;
	}

	bool CSourceDebugFrameWnd::OnEventCallStack(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_CallStackWnd.Create();
		return true;
	}

	bool CSourceDebugFrameWnd::OnEventRegView(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_SystemExplorer.OnEventRegView(pMsg);
		return true;
	}
	bool CSourceDebugFrameWnd::OnEventDataViewForm(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_SystemExplorer.OnEventDataViewForm(pMsg);
		return true;
	}

	bool CSourceDebugFrameWnd::OnEventToggleBM(WISP_MSG *pMsg)
	{
		CSourceCodeWnd *pWnd = (CSourceCodeWnd*)m_MultiSourceCodeView.GetActiveWnd();
		if (pWnd) pWnd->ToggleBM();
		return true;
	}

	bool CSourceDebugFrameWnd::OnEventNextBM(WISP_MSG *pMsg)
	{
		CSourceCodeWnd *pWnd = (CSourceCodeWnd*)m_MultiSourceCodeView.GetActiveWnd();
		if (pWnd) pWnd->NextBM();
		return true;
	}

	bool CSourceDebugFrameWnd::OnEventPrevBM(WISP_MSG *pMsg)
	{
		CSourceCodeWnd *pWnd = (CSourceCodeWnd*)m_MultiSourceCodeView.GetActiveWnd();
		if (pWnd) pWnd->PrevBM();
		return true;
	}

	bool CSourceDebugFrameWnd::OnEventClearBM(WISP_MSG *pMsg)
	{
		CSourceCodeWnd *pWnd = (CSourceCodeWnd*)m_MultiSourceCodeView.GetActiveWnd();
		if (pWnd) pWnd->ClearBM();
		return true;
	}

	bool CSourceDebugFrameWnd::OnEventMenuDebug(WISP_MSG *pMsg)
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
	bool CSourceDebugFrameWnd::OnEventMenuMonitor(WISP_MSG *pMsg)
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
	bool CSourceDebugFrameWnd::OnEventMenuView(WISP_MSG *pMsg)
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
	bool CSourceDebugFrameWnd::OnEventGotoLine(WISP_MSG *pMsg)
	{
		CSourceCodeWnd *pWnd = (CSourceCodeWnd*)m_MultiSourceCodeView.GetActiveWnd();
		if (pWnd) pWnd->OnEventGotoLine(pMsg);
		return true;
	}

	bool CSourceDebugFrameWnd::OnGetFocus(WISP_MSG *pMsg)
	{
		CWispBaseWnd *pWnd = m_MultiSourceCodeView.GetActiveWnd();
		if (pWnd)
		{
			pWnd->Focus();
			return false;
		}
		return true;
	}

	void CSourceDebugFrameWnd::UpdateMenu()
	{
		CSourceCodeWnd *pWnd = (CSourceCodeWnd *)m_MultiSourceCodeView.GetActiveWnd();

		m_Toolbar.Enable(EVENT_ID_SOURCECODEWND_GOTOLINE, pWnd != 0);
		m_MainMenu.Enable(EVENT_ID_SOURCECODEWND_GOTOLINE, pWnd != 0);

		m_Toolbar.Enable(EVENT_ID_TOGGLE_BM, pWnd != 0);
		m_MainMenu.Enable(EVENT_ID_TOGGLE_BM, pWnd != 0);

		m_Toolbar.Enable(EVENT_ID_NEXT_BM, pWnd && pWnd->m_Bookmark.size() > 0);
		m_MainMenu.Enable(EVENT_ID_NEXT_BM, pWnd && pWnd->m_Bookmark.size() > 0);

		m_Toolbar.Enable(EVENT_ID_PREV_BM, pWnd && pWnd->m_Bookmark.size() > 0);
		m_MainMenu.Enable(EVENT_ID_PREV_BM, pWnd && pWnd->m_Bookmark.size() > 0);

		m_Toolbar.Enable(EVENT_ID_CLEAR_BM, pWnd && pWnd->m_Bookmark.size() > 0);
		m_MainMenu.Enable(EVENT_ID_CLEAR_BM, pWnd && pWnd->m_Bookmark.size() > 0);

		m_Toolbar.Enable(EVENT_ID_DBG_STEP, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_DBG_STEP, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_DBG_PROCEED, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_DBG_PROCEED, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_DBG_RETURN, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_DBG_RETURN, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Enable(EVENT_ID_GO, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);
		m_MainMenu.Enable(EVENT_ID_GO, gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen);

		m_Toolbar.Update((WISP_RECT*)0);
	}

	void CSourceDebugFrameWnd::UpdateContext(bool bUpdateSrc)
	{
		if (bUpdateSrc)
		{
			CSDSModule *pSDSMod = gpSyser->m_SyserUI.FindSDSModule(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP);
			if (pSDSMod)	
			{
				TList<FilesLN*> ListFilesLN;
				m_SDSModuleList.ExpandSDSModule(pSDSMod);
				if (pSDSMod->GetFilesLN(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP - pSDSMod->m_pDbgModule->m_ImageBase, &ListFilesLN))
				{
					FilesLN *fln = *ListFilesLN[0];
					OpenSrcView(pSDSMod, fln->FileID, fln->lineNumber);
				}
			}
		}
		m_MultiSourceCodeView.UpdateContext();
		m_TypeViewerWnd.UpdateContext();
	}

	CSourceCodeWnd *CSourceDebugFrameWnd::OpenSrcView(CSDSModule *pSDSMod, unsigned long FileID, unsigned long LineNumber)
	{
		SCWKEY Key;
		Key.FileID = FileID;
		Key.pSDSMod = pSDSMod;
		map<SCWKEY,CSourceCodeWnd *>::IT it = m_MultiSourceCodeView.m_SourceCodeWndMap.find(Key);
		if (it)
		{
			if (it->second)
				m_MultiSourceCodeView.ChangeActiveWnd(it->second);
			it->second->ShowLineNum(LineNumber);
			it->second->UpdateContextSrc();
		} else
		{
			SrcFileNameID *SrcFile = pSDSMod->GetSrcFileNameID(FileID);
			if (!SrcFile)	
				return nullptr;
			if (!SrcFile->FileSize)
				return nullptr;
			map<SCWKEY,CSourceCodeWnd *>::IT newit = m_MultiSourceCodeView.m_SourceCodeWndMap.insert(_Tmap_pair<SCWKEY,CSourceCodeWnd *>(Key,(CSourceCodeWnd *)0));
			if (!newit)
				return nullptr;
			newit->second = new CSourceCodeWnd;
			newit->second->m_pSDSMod = pSDSMod;
			newit->second->m_CurrentFileID = FileID;
			newit->second->CreateWnd(0, 0,0,0,0, &m_MultiSourceCodeView, FileID + 1, 0x41030000, WISP_SH_MINSIZE);
			newit->second->m_AdvStyle |= 1;
			WCHAR FileName[MAX_FN_LEN];
			TStrCpy(FileName, TGetFileName(SrcFile->FileName));
			m_MultiSourceCodeView.InsertWnd(FileName, newit->second, 0);
			newit->second->OpenSrcFile(SrcFile->Text, SrcFile->FileSize, SrcFile);
			m_MultiSourceCodeView.ChangeActiveWnd(newit->second);
			newit->second->ShowLineNum(LineNumber);
			it = newit;
		}

		if (it->second->UpdateContextPics())
		{
			if (gSyserConfig.iFollowMode == 0)
				gpSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(SRC_TAB); //SRC_TAB(2)
		}
		Update((WISP_RECT*)0);
		return it->second;
	}

	void CSourceDebugFrameWnd::RemoveSDSModule(CSDSModule *pSDSMod)
	{
		if (pSDSMod)
			m_TypeViewerWnd.RemoveSDSModule(pSDSMod);
	}


#include "mainframe.hpp"
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "resource.hpp"

	CMainFrame::CMainFrame()
	{
		m_pGeneRegList = 0;
	}
	CMainFrame::~CMainFrame()
	{
	}

	WISP_MSG_MAP_BEGIN(CMainFrame)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_CLOSE, OnClose)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP(WISP_WM_BEGIN_MOVE, OnBeginMove)
		WISP_MSG_MAP(WISP_WM_END_MOVE, OnEndMove)
		WISP_MSG_MAP(WISP_WM_EVENT, OnEvent)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(30, OnDebuggerPlunge)
		WISP_MSG_MAP(31, OnDebuggerClose)
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
	WISP_MSG_MAP_END(CWispWnd)

	bool CMainFrame::OnCreate(WISP_MSG *pMsg)
	{
		AttachTitleDIB(gpCurWisp->m_DIBLib.LoadDIB("\\SyserIcon\\Syser.ico", 0, 0xFF00FF));

		WISP_RECT Rect;

		Rect.cx = m_ClientRect.cx;
		Rect.cy = m_ClientRect.cy;
		Rect.x = 0;
		Rect.y = 0;

		if (!m_MainTabWnd.CreateWnd(0, Rect, this, 0, 0x20000, WISP_SH_MINSIZE))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_SystemExplorer.CreateWnd(0, Rect, &m_MainTabWnd, CMD_ID_SYSEXPLORER, 0x100, WISP_SH_MINSIZE))
			return false;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		if (!m_ConsoleWnd.CreateWnd(0, Rect, &m_MainTabWnd, CMD_ID_CONSOLE, 0, WISP_SH_MINSIZE))
			return false;

		Rect = m_MainTabWnd.m_ClientRect;
		if (!m_SourceDebugFrameWnd.CreateWnd(0, Rect, &m_MainTabWnd, 0, 0x20100, WISP_SH_MINSIZE))
			return false;

		m_ConsoleWnd.Printf(WSTR("Wisp Syser Console\n"));


		m_MainTabWnd.InsertWnd(WSTR("System Explorer Ctrl+1"), &m_SystemExplorer, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x3E));
		m_MainTabWnd.InsertWnd(WSTR("Command Console Ctrl+2"), &m_ConsoleWnd, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x3F));
		m_MainTabWnd.InsertWnd(WSTR("Source Explorer Ctrl+3"), &m_SourceDebugFrameWnd, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0xF6));

		m_StaticText.Create(0, m_ClientRect.cx - 400, m_ClientRect.cy - 16, 400, 16, &m_MainTabWnd, 0, 0, 1);

		HookKeyEvent();

		gpSyser->RegisterHotKeyEvent(EVENT_ID_REGVIEW, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_DATAVIEW, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_CALLSTACK, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_BREAK_POINT, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_GO, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_DBG_RETURN, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_DBG_STEP, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_DBG_PROCEED, this);
		gpSyser->RegisterHotKeyEvent(EVENT_ID_DBG_STEPB, this);
		return true;
	}
	bool CMainFrame::OnDestroy(WISP_MSG *pMsg)
	{
		DetachTitleDIB();
		return true;
	}
	bool CMainFrame::OnUpdate(WISP_MSG *pMsg)
	{
#if 0
#if 0
		//for test only
		WISP_RECT rc;

		rc.x = 0;
		rc.y = 0;
		rc.cx = m_WindowRect.cx;//120;
		rc.cy = m_WindowRect.cy; //200;
		//m_WindowDC.DrawFullRect(&rc, 0x64A0BA);
		m_WindowDC.DrawVLine(10,10,100, 0x64A0BA);
#endif
		WISP_RECT rc;
#if 0
#if 1
//		m_WindowDC.DrawRect(10, 10, 100, 100, 0x324510);
//		m_WindowDC.DrawLine(100,100, 400,400, 0x63FFA0);
//		m_WindowDC.DrawLine(400,100, 100,400, 0x63FFA0);

//		m_WindowDC.DrawVLine(400, 100,400, 0xFFA063);
//		m_WindowDC.DrawHLine(400, 100,400, 0x63FFA0);

		rc.x = 40;
		rc.y = 100;
		rc.cx = 300;
		rc.cy = 400;
		//m_WindowDC.DrawDashRect(&rc, 0x64A0BA);
//		m_pWispBase->m_pWispDrawObj->DrawButtonClient(&m_WindowDC, &rc, 0);

#if 1

		rc.x = 40;
		rc.y = 100;
		rc.cx = 300;
		rc.cy = 400;
//		m_WindowDC.DrawRoundRect(&rc,  0x64A0BA);

	#if 1
		rc.x = 40;
		rc.y = 100;
		rc.cx = 300;
		rc.cy = 400;
		//m_WindowDC.DrawDashRect(&rc, 0x64A0BA);
		m_pWispBase->m_pWispDrawObj->DrawButtonClient(&m_WindowDC, &rc, 1);
//		m_pWispBase->m_pWispDrawObj->DrawVertScrollSlideRect(&m_WindowDC, &rc);
//		m_pWispBase->m_pWispDrawObj->DrawCtrlModalFrame(&m_WindowDC, &rc);

		WISP_POINT pt;
		pt.x = 30;
		pt.y = 29;
//		m_pWispBase->m_pWispDrawObj->DrawCursorArrow(&m_WindowDC, &pt);
//		m_DefDrawObj.
//		m_DIBDrawObj.
//		m_pWispBase->m_pWispDrawObj->DrawCursorResizeVert(&m_WindowDC, &pt);
//		m_pWispBase->m_DefDrawObj.DrawCursorResizeVert(&m_WindowDC, &pt);
//		m_pWispBase->m_DefDrawObj.DrawCursorArrow(&m_WindowDC, &pt);
		m_pWispBase->m_DIBDrawObj.DrawCursorResizeVert(&m_WindowDC, &pt);

	#endif
	#if 1
		rc.x = 500;
		rc.y = 300;
		rc.cx = 120;
		rc.cy = 200;
		m_WindowDC.DrawFullRect(&rc, 0x64A0BA);
	#endif
	#if 1
		rc.x = 400;
		rc.y = 300;
		rc.cx = 100;
		rc.cy = 150;
		m_WindowDC.DrawXGrayChgFullRect(&rc, 0x64A0BA, 30);
	#endif

		rc.x = 600;
		rc.y = 150;
		rc.cx = 200;
		rc.cy = 150;
		m_WindowDC.DrawXGrayColorFullRect2(0x64A0BA, 0xA0FFED, &rc);
#endif
#endif
//		CWispDIB *pDIB = gpCurWisp->m_DIBLib.GetDIB("\\WispApp\\SoftKeyboard\\SoftKeyboard.ico", 0);
//		CWispDIB *pDIB = gpCurWisp->m_DIBLib.GetDIB("\\WispApp\\Calc\\Calc.bmp", 0);
//		pDIB->Write("D:\\Calc.bmp");
		CWispDIB DIB;
		DIB.Load("\\WispApp\\SoftKeyboard\\SoftKeyboard.ico", 0); //\\WispApp\\Calc\\Calc.bmp", 0);
//		DIB.Write("D:\\Calc.bmp");
		m_WindowDC.DrawDIB(10, 50, &DIB); //pDIB);
//#endif
//		m_WindowDC.DrawDIB(300, 50, &m_pWispBase->m_DefDIB[0]); //pDIB);
#endif
		m_WindowDC.SetTextColor(0x64A0BA);
		m_WindowDC.DrawString(0,0,WSTR("____TEST STRING___"), -1, 0, 0);
#endif
		return true;
	}
	bool CMainFrame::OnClose(WISP_MSG *pMsg)
	{
		return true;
	}
	bool CMainFrame::OnSize(WISP_MSG *pMsg)
	{
		int SizeX = m_ClientRect.cx - m_MainTabWnd.m_FullWidth;
		MAX_LIMIT(SizeX, 400);
		MIN_LIMIT(SizeX, 0);
		m_StaticText.MoveToClient(m_ClientRect.cx - SizeX, m_ClientRect.cy - 16, true);
		m_StaticText.Resize(SizeX, m_StaticText.m_WindowRect.cy, true);
		m_MainTabWnd.Resize(m_ClientRect.cx, m_ClientRect.cy, true);
		return true;
	}
	bool CMainFrame::OnBeginMove(WISP_MSG *pMsg)
	{
		return true;
	}
	bool CMainFrame::OnEndMove(WISP_MSG *pMsg)
	{
		return true;
	}

	WISP_EVENT_MAP_BEGIN(CMainFrame)
		WISP_EVENT_MAP(EVENT_ID_DBG_STEP, OnEventDbgStep)
		WISP_EVENT_MAP(EVENT_ID_DBG_PROCEED, OnEventDbgProceed)
		WISP_EVENT_MAP(EVENT_ID_DBG_STEPB, OnEventStepBranch)
		WISP_EVENT_MAP(EVENT_ID_DBG_RETURN, OnEventDbgReturn)
		WISP_EVENT_MAP(EVENT_ID_GO, OnEventDbgContinue)
		WISP_EVENT_MAP(EVENT_ID_BREAK_POINT, OnEventBreakPointForm)
		WISP_EVENT_MAP(EVENT_ID_CALLSTACK, OnEventCallStack)
		WISP_EVENT_MAP(EVENT_ID_REGVIEW, OnEventRegView)
		WISP_EVENT_MAP(EVENT_ID_DATAVIEW, OnEventDataViewForm)
	WISP_EVENT_MAP_END

	bool CMainFrame::OnEventDbgStep(WISP_MSG *pMsg)
	{
		switch (m_MainTabWnd.GetActiveWndIndex())
		{
		case 0:
		case 1:
			m_SystemExplorer.OnEventDbgStep(pMsg);
			break;
		case 2:
			m_SourceDebugFrameWnd.OnEventDbgStep(pMsg);
			break;
		}
		return true;
	}

	bool CMainFrame::OnEventDbgProceed(WISP_MSG *pMsg)
	{
		switch (m_MainTabWnd.GetActiveWndIndex())
		{
		case 0:
		case 1:
			m_SystemExplorer.OnEventDbgProceed(pMsg);
			break;
		case 2:
			m_SourceDebugFrameWnd.OnEventDbgProceed(pMsg);
			break;
		}
		return true;
	}

	bool CMainFrame::OnEventStepBranch(WISP_MSG *pMsg)
	{
		switch (m_MainTabWnd.GetActiveWndIndex())
		{
		case 0:
		case 1:
			m_SystemExplorer.OnEventStepBranch(pMsg);
			break;
		case 2:
			//m_SourceDebugFrameWnd.OnEventStepBranch(pMsg);
			break;
		}
		return true;
	}

	bool CMainFrame::OnEventDbgReturn(WISP_MSG *pMsg)
	{
		switch (m_MainTabWnd.GetActiveWndIndex())
		{
		case 0:
		case 1:
			m_SystemExplorer.OnEventDbgReturn(pMsg);
			break;
		case 2:
			m_SourceDebugFrameWnd.OnEventDbgReturn(pMsg);
			break;
		}
		return true;
	}

	bool CMainFrame::OnEventDbgContinue(WISP_MSG *pMsg)
	{
		switch (m_MainTabWnd.GetActiveWndIndex())
		{
		case 0:
		case 1:
			m_SystemExplorer.OnEventDbgContinue(pMsg);
			break;
		case 2:
			m_SourceDebugFrameWnd.OnEventDbgContinue(pMsg);
			break;
		}
		return true;
	}

	bool CMainFrame::OnEventBreakPointForm(WISP_MSG *pMsg)
	{
		switch (m_MainTabWnd.GetActiveWndIndex())
		{
		case 0:
		case 1:
			m_SystemExplorer.OnEventBreakPointForm(pMsg);
			break;
		case 2:
			m_SourceDebugFrameWnd.OnEventBreakPointForm(pMsg);
			break;
		}
		return true;
	}

	bool CMainFrame::OnEventCallStack(WISP_MSG *pMsg)
	{
		switch (m_MainTabWnd.GetActiveWndIndex())
		{
		case 0:
		case 1:
			m_SystemExplorer.OnEventCallStack(pMsg);
			break;
		case 2:
			m_SourceDebugFrameWnd.OnEventCallStack(pMsg);
			break;
		}
		return true;
	}

	bool CMainFrame::OnEventRegView(WISP_MSG *pMsg)
	{
		switch (m_MainTabWnd.GetActiveWndIndex())
		{
		case 0:
		case 1:
			m_SystemExplorer.OnEventRegView(pMsg);
			break;
		case 2:
			m_SourceDebugFrameWnd.OnEventRegView(pMsg);
			break;
		}
		return true;
	}

	bool CMainFrame::OnEventDataViewForm(WISP_MSG *pMsg)
	{
		switch (m_MainTabWnd.GetActiveWndIndex())
		{
		case 0:
		case 1:
			m_SystemExplorer.OnEventDataViewForm(pMsg);
			break;
		case 2:
			m_SourceDebugFrameWnd.OnEventDataViewForm(pMsg);
			break;
		}
		return true;
	}

	bool CMainFrame::Move()
	{
		int DeltaX = 0;
		int DeltaY = 0;
		if (gpCurWisp->m_KeyMap[VK_LEFT])
			DeltaX = -20;
		if (gpCurWisp->m_KeyMap[VK_RIGHT])
			DeltaX += 20;

		if (gpCurWisp->m_KeyMap[VK_UP])
			DeltaY = -20;
		if (gpCurWisp->m_KeyMap[VK_DOWN])
			DeltaY += 20;
		return gpSyser->MovingFrame(DeltaX, DeltaY);
	}

	bool CMainFrame::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (m_pWispBase->m_pModalWnd)
			return true;

		if (!pMsg->KeyEvent.bKeyDown)
			return true;

		switch (pMsg->KeyEvent.KeyType)
		{
		case 0x6000002E: //WISP_SK_CTRL|WISP_SK_ALT VK_DELETE
			gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("hboot"), 0);
			return false;
		case VK_ESCAPE:
			if (m_pWispBase->m_pFocusWnd && m_pWispBase->m_pFocusWnd->m_CtrlType != WISP_CTRL_EDIT)
				FocusActive();
			break;
		case 0x40000031: //WISP_SK_CTRL
		case 0x40000032: //WISP_SK_CTRL
		case 0x40000033: //WISP_SK_CTRL
		case 0x40000034: //WISP_SK_CTRL
		case 0x40000035: //WISP_SK_CTRL
		case 0x40000036: //WISP_SK_CTRL
		case 0x40000037: //WISP_SK_CTRL
		case 0x40000038: //WISP_SK_CTRL
		case 0x40000039: //WISP_SK_CTRL
			m_MainTabWnd.SetActiveWnd((pMsg->KeyEvent.KeyType&0xffff)-0x31);
			return false;
		default:;
		}
		return true;
	}

	bool CMainFrame::OnDebuggerPlunge(WISP_MSG *pMsg) //30
	{
		gpSyser->m_SyserUI.m_DebugInterface.Plunge(pMsg->Code);
		return true;
	}	
	bool CMainFrame::OnDebuggerClose(WISP_MSG *pMsg) //31
	{
		gpSyser->m_SyserUI.m_DebugInterface.Close(pMsg->bExit != 0);
		return true;
	}

	void CMainFrame::FocusActive()
	{
		switch (m_MainTabWnd.GetActiveWndIndex())
		{
		case 0: m_SystemExplorer.m_TerminalWnd.Focus(); break;
		case 1: m_ConsoleWnd.Focus(); break;
		case 2: {
			CWispBaseWnd *pWnd = m_SourceDebugFrameWnd.m_MultiSourceCodeView.GetActiveWnd();
			if (pWnd) pWnd->Focus();
			else	m_SourceDebugFrameWnd.Focus();
			} break;
		}
	}

	void CMainFrame::CreateProgress(const WCHAR *Text, unsigned long MaxPos)
	{
		m_ProgressForm.Create(nullptr, 1);
		m_ProgressForm.SetText(Text);
		m_ProgressForm.m_pProgress->SetRange(1, MaxPos);
		m_ProgressForm.m_pProgress->SetPos(1);
		m_ProgressForm.Update((WISP_RECT*)0);
		m_ProgressForm.Show(WISP_SH_MODAL); //WISP_SH_MODAL_BLOCK 5
	}

	void CMainFrame::UpdateProgress(unsigned long Pos)
	{
		m_ProgressForm.m_pProgress->SetPos(Pos);
		m_ProgressForm.Update((WISP_RECT*)0);
		m_pWispBase->PumpQueue();
	}

	void CMainFrame::UpdateCaption(const WCHAR *Text)
	{
		m_ProgressForm.SetText(Text);
		m_ProgressForm.Update((WISP_RECT*)0);
	}

	void CMainFrame::DestroyProgress()
	{
		m_ProgressForm.Destroy();
	}

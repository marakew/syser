
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "findlistwnd.hpp"
#include "syserconfig.hpp"
#include "resource.hpp"

	WISP_MSG_MAP_BEGIN(CStatusWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
	WISP_MSG_MAP_END(CWispWnd)

	CStatusWnd::CStatusWnd()
	{
	}
	CStatusWnd::~CStatusWnd()
	{
	}

	bool CStatusWnd::OnCreate(WISP_MSG *pMsg)
	{
		m_BorderSize = 2;
		m_StatusStr.Empty();
		return true;
	}
	bool CStatusWnd::OnUpdateClient(WISP_MSG *pMsg)
	{
		CWispWnd::OnUpdateClient(pMsg);
		m_ClientDC.DrawString(4, 2, m_StatusStr.operator const WCHAR*(), -1, 0, 0);
		return false;
	}


	WISP_MSG_MAP_BEGIN(CFindListWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP_ON_COMMAND
	WISP_MSG_MAP_END(CWispWnd)

	WISP_CMD_MAP_BEGIN(CFindListWnd)
		WISP_CMD_MAP(EVENT_ID_STRFIND, OnCmdStrFind)
	WISP_CMD_MAP_END

	CFindListWnd::CFindListWnd()
	{
	}
	CFindListWnd::~CFindListWnd()
	{
	}

	bool CFindListWnd::OnCreate(WISP_MSG *pMsg)
	{
		m_ShowFormat = 1; //???
		m_StringReferenceList.CreateWnd(WSTR("String Reference"), 0,0,556,300, this, EVENT_ID_STRFIND, 0x40030000, WISP_SH_MINSIZE);
		m_StringReferenceList.m_ShowFormat = m_ShowFormat;

		gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.clear();

		int Ext = m_ClientDC.GetTextExtent(WSTR("X"), -1, 0);
		if (m_ShowFormat)
		{
			m_StringReferenceList.InsertColumn(WSTR("Address"), (3+ADDR_SIZE)*Ext+3, 0, 0, -1); //60
			m_StringReferenceList.InsertColumn(WSTR("Type"), 32, 0, 0, -1);
			m_StringReferenceList.InsertColumn(WSTR("Disassembly"), 200, 0, 0, -1);
			m_StringReferenceList.InsertColumn(WSTR("Text String"), 280, 0, 0, -1);
		} else
		{
			m_StringReferenceList.InsertColumn(WSTR("Address"), (3+ADDR_SIZE)*Ext+3, 0, 0, -1); //80
			m_StringReferenceList.InsertColumn(WSTR("Text String/Disassembly"), 200, 0, 0, -1);
		}

		m_StringReferenceList.m_ColorFocus = gSyserColors.Color[2];
		m_StringReferenceList.m_ItemColor = gSyserColors.Color[1];
		m_StringReferenceList.SetBGColor(gSyserColors.Color[0]);

		m_StatusWnd.CreateWnd(0, 0,300,564,20, this, 0, 6, WISP_SH_MINSIZE);
		m_StatusWnd.SetBGColor(gSyserColors.Color[0]);
		m_StatusWnd.m_ClientDC.SetTextColor(gSyserColors.Color[1]);

		m_StringReferenceList.InitContext(0, 0);
		return true;
	}
	bool CFindListWnd::OnCmdStrFind(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0)
		{
			m_StatusWnd.m_StatusStr = m_StringReferenceList.m_SearchString;
			if (pMsg->Command.Param1 == 0 && m_StatusWnd.m_StatusStr.m_nLength)
				m_StatusWnd.m_StatusStr += WSTR(": not found");
			m_StatusWnd.Update((WISP_RECT*)0);
		}
		return true;
	}
	bool CFindListWnd::OnSize(WISP_MSG *pMsg)
	{
		m_StringReferenceList.Resize(m_ClientRect.cx, m_ClientRect.cy - 20, 1);
		m_StatusWnd.Resize(m_ClientRect.cx, 20, 1);

		m_StringReferenceList.MoveToWindow(m_ClientRect.x, m_ClientRect.y, 1);
		m_StatusWnd.MoveToWindow(m_ClientRect.x, m_ClientRect.y + m_ClientRect.cy - 20, 1);
		return true;
	}


#include "wispstatic.hpp"
#include "wispbase.hpp"

//CWispStaticStr
//CWispStaticDIB
//CWispStaticGroup
//CWispStaticURL

//
	CWispStaticStr::CWispStaticStr()
	{
		m_CtrlType = WISP_CTRL_STATIC_STRING;
	}
	CWispStaticStr::~CWispStaticStr()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispStaticStr)
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
	WISP_MSG_MAP_END(CWispBaseWnd)

	bool CWispStaticStr::OnUpdate(WISP_MSG *pMsg)
	{
		unsigned long OrgTextColor;

		if (m_WndText.m_nLength)
		{
			//???
			if ( (m_Style >> 9) & 1 ) //0x200
			{
				OrgTextColor = m_WindowDC.m_TextColor;
				m_WindowDC.m_OldTextColor = m_WindowDC.m_TextColor;
				m_WindowDC.m_TextColor = m_pWispBase->m_pWispDrawObj->m_crSystem[12];
			}

			m_WindowDC.DrawString(m_WndText.operator const WCHAR*(),
								&m_WindowRect, 0, 0, -1);

			//???
			if ( (m_Style >> 9) & 1 ) //0x200
			{
				m_WindowDC.m_OldTextColor = m_WindowDC.m_TextColor;
				m_WindowDC.m_TextColor = OrgTextColor;
			}
		}
		return true;
	}
//
	CWispStaticDIB::CWispStaticDIB()
	{
		m_CtrlType = WISP_CTRL_STATIC_DIB;
	}

	CWispStaticDIB::~CWispStaticDIB()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispStaticDIB)
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
	WISP_MSG_MAP_END(CWispBaseWnd)

	bool CWispStaticDIB::CreateEx(CWispDIB *pDIB, int x, int y, CWispBaseWnd *pParentWnd, unsigned int CmdID)
	{
		m_pDIB = pDIB;
		return Create(0, x,y,
			pDIB->m_PixelBuf.Width, pDIB->m_PixelBuf.Height, pParentWnd, CmdID, 0, WISP_SH_MINSIZE);

	}

	bool CWispStaticDIB::OnUpdate(WISP_MSG *pMsg)
	{
		m_WindowDC.DrawDIB(0, 0, m_pDIB);
		return true;
		
	}
//
	CWispStaticGroup::CWispStaticGroup()
	{
		m_CtrlType = WISP_CTRL_STATIC_GROUP;
	}
	CWispStaticGroup::~CWispStaticGroup()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispStaticGroup)
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_GET_FOCUS, OnGetFocus)
	WISP_MSG_MAP_END(CWispBaseWnd)

	bool CWispStaticGroup::OnUpdate(WISP_MSG *pMsg)
	{
		int nTextLen = m_WindowDC.GetTextExtent(m_WndText.operator const WCHAR*(), -1, 0);

		unsigned long color = m_pWispBase->m_pWispDrawObj->m_crSystem[35];

		WISP_RECT rc = m_WindowRect;

		rc.y += m_WindowDC.m_pFont->m_Height/2;
		rc.cy += m_WindowDC.m_pFont->m_Height/-2;

		m_WindowDC.DrawCircleLeftTop(rc.x+5, rc.y+5, 5, color);
		m_WindowDC.DrawCircleLeftBottom(rc.x+5, rc.y2()-6, 5, color);
		m_WindowDC.DrawCircleRightTop(rc.x2()-6, rc.y+5, 5, color);
		m_WindowDC.DrawCircleRightBottom(rc.x2()-6, rc.y2()-6, 5, color);
		m_WindowDC.DrawVLine(rc.x, rc.y+5, rc.y2()-6, color);
		m_WindowDC.DrawVLine(rc.x2()-1, rc.y+5, rc.y2()-6, color);
		m_WindowDC.DrawHLine( (rc.x + nTextLen) ? nTextLen+15:nTextLen+5, rc.x2()-6, rc.y, color);
		m_WindowDC.DrawHLine(rc.x+5, rc.x2()-6, rc.y2()-1, color);

		rc.x += 10;
		rc.y -= m_WindowDC.m_pFont->m_Height/2;
		rc.cx = nTextLen;
		rc.cy = m_WindowDC.m_pFont->m_Height;
		if (m_WndText.m_nLength)
			m_WindowDC.DrawString(m_WndText.operator const WCHAR*(), &rc, 0x24, 0, -1);

		return false;
	}
	bool CWispStaticGroup::OnGetFocus(WISP_MSG *pMsg)
	{
		//???
		return false;
	}

//
	CWispStaticURL::CWispStaticURL()
	{
		m_CtrlType = WISP_CTRL_STATIC_URL;
		m_MouseEnter = 0;
	}
	CWispStaticURL::~CWispStaticURL()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispStaticURL)
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_MOUSE_ENTER, OnMouseEnter)
		WISP_MSG_MAP(WISP_WM_MOUSE_LEAVE, OnMouseLeave)
	WISP_MSG_MAP_END(CWispBaseWnd)

	bool CWispStaticURL::OnUpdate(WISP_MSG *pMsg)
	{
		if (!m_WndText.m_nLength)
			return true;

		//???
		if ( (m_Style >> 9) & 1 ) //0x200
		{
			m_WindowDC.SetTextColor(m_pWispBase->m_pWispDrawObj->m_crSystem[12]);
			m_WindowDC.DrawString(m_WndText.operator const WCHAR*(), &m_WindowRect, 0, 0, -1);
			m_WindowDC.SetTextColor();
			return true;
		}

		if (m_MouseEnter)
		{
			m_WindowDC.SetTextColor(m_pWispBase->m_pWispDrawObj->m_crSystem[7]);
		}

		m_WindowDC.DrawString(m_WndText.operator const WCHAR*(), &m_WindowRect, 0, 0, -1);

		if (m_MouseEnter)
		{
			m_WindowDC.DrawHLine(0, 
				m_WindowDC.GetTextExtent(m_WndText.operator const WCHAR*(), -1, 0),
				m_WindowRect.y + m_WindowDC.GetTextHeight(0),
				m_pWispBase->m_pWispDrawObj->m_crSystem[7]);
			m_WindowDC.SetTextColor();
		}
		return true;
	}
	bool CWispStaticURL::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.KeyType != VK_LBUTTON || pMsg->KeyEvent.bKeyDown)
			return true;

		char UrlStr[MAX_PATH];
		UnicodeToAnsi(GetWindowText().operator const WCHAR*(), UrlStr, MAX_PATH);

		//ShellExecuteA(0, "open", UrlStr, 0, 0, 3); //TODO

		WISP_MSG Msg;
		Msg.hWnd = m_OwnerWnd;
		Msg.Msg = WISP_WM_COMMAND;
		Msg.Command.CmdMsg = 0x80000500; //WISP_ID_OPEN_URL
		Msg.Command.CmdID = m_CmdID;
		Msg.Command.Param1 = 0;
		Msg.Command.Param2 = 0;
		if(!m_pWispBase->SendMessage(&Msg)) //SendCommand(
			return false;
		return true;
	}
	bool CWispStaticURL::OnMouseEnter(WISP_MSG *pMsg)
	{
		m_MouseEnter = true;
		Update((WISP_RECT*)0);
		return true;
	}
	bool CWispStaticURL::OnMouseLeave(WISP_MSG *pMsg)
	{
		m_MouseEnter = false;
		Update((WISP_RECT*)0);
		return true;
	}

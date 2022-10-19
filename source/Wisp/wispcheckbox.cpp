
#include "wispcheckbox.hpp"
#include "wispbase.hpp"
#include "wispwnd.hpp"

	CWispCheckBox::CWispCheckBox()
	{
		m_TextRect.cy = 0;
		m_TextRect.cx = 0;
		m_TextRect.y = 0;
		m_TextRect.x = 0;

		m_CheckBoxRect.cy = 0;
		m_CheckBoxRect.cx = 0;
		m_CheckBoxRect.y = 0;
		m_CheckBoxRect.x = 0;

		m_bIsMultiLine = false;
		m_bMouseDown = false;
		m_bMouseIn = false;
		m_CtrlType = WISP_CTRL_CHECK_BOX;
	}
	CWispCheckBox::~CWispCheckBox()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispCheckBox)
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_MOUSE_LEAVE, OnMouseLeave)
		WISP_MSG_MAP(WISP_WM_MOUSE_MOVE, OnMouseMove)
	WISP_MSG_MAP_END(CWispBaseWnd)

	bool CWispCheckBox::OnUpdate(WISP_MSG *pMsg)
	{
		if (!(m_Style & 0x40))
			m_pWispBase->m_pWispDrawObj->DrawCtrlClient(&m_WindowDC, &m_WindowRect);

		if (!(m_Style & 0x40000000))
		{
			if ((m_Style >> 10) & 1)
				m_WindowDC.DrawSystemStandardIcon(&m_CheckBoxRect, 0x0C, 0x21A221); //12 WISP_SSI_CHECKBOX_CHECKED
			else	m_WindowDC.DrawSystemStandardIcon(&m_CheckBoxRect, 0x0B, 0x21A221); //11 WISP_SSI_CHECKBOX_UNCHECKED
		}

		DrawCaption();

		if (m_Style & 0x40000000)
		{
			if (((m_Style >> 10) & 1) || (m_bMouseDown && m_bMouseIn))
				m_WindowDC.DrawFrameRect(&m_WindowRect, 0, 1);
			else	m_WindowDC.DrawFrameRect(&m_WindowRect, 1, 1);
		}
		return false;
	}
	bool CWispCheckBox::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.KeyType == VK_LBUTTON)
		{
			if (pMsg->KeyEvent.bKeyDown)
			{
				m_bMouseDown = true;
			} else
			if (OnMouseLUp(pMsg))
			{
				WISP_MSG Msg;
				Msg.Msg = WISP_WM_COMMAND;
				Msg.hWnd = (CWispWnd *)m_OwnerWnd;
				Msg.Command.CtrlType = m_CtrlType;
				Msg.Command.CmdMsg = 0x80000300; //WISP_ID_BOX_SELECT
				Msg.Command.CmdID = m_CmdID;
				Msg.Command.Param1 = (m_Style >> 10) & 1;
				m_pWispBase->SendMessage(&Msg);
			}
			Update((WISP_RECT*)0);
		}
		return true;
	}
	bool CWispCheckBox::OnMouseLDown(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.bKeyDown)
		{
			m_bMouseDown = true;
			return true;
		}
		return false;
	}
	bool CWispCheckBox::OnMouseLUp(WISP_MSG *pMsg)
	{
		if (!m_bMouseIn || !m_bMouseDown)
			return false;
		if (!((m_Style >> 10) & 1))
			Enable(1);
		else	Enable(0);
		m_bMouseDown = false;
                return true;
	}
//	bool CWispCheckBox::OnGetFocus(WISP_MSG *pMsg) { m_bFocus = 1; return true; }
//	bool CWispCheckBox::OnLostFocus(WISP_MSG *pMsg) { m_bFocus = 0; return true; }
	bool CWispCheckBox::OnMouseLeave(WISP_MSG *pMsg)
	{
		m_bMouseIn = false;
		if (m_bMouseDown == false)
			return true;
		if (!pMsg->bMsgLBTDown)
			m_bMouseDown = false;
		return false;
	}
	bool CWispCheckBox::OnMouseMove(WISP_MSG *pMsg)
	{
		m_bMouseIn = true;
		return true;
	}
	bool CWispCheckBox::InitWnd()
	{
		if (!CWispBaseWnd::InitWnd())
			return false;
		m_Style &= ~0xBB;
		if (m_Style & 0x40000000)
			m_Style |= 6;
		m_Style |= 0x40;
		if (m_Style & 0x80000000)
			m_bIsMultiLine = true;
		if (m_Style & 0x40000000)
		{
			m_TextRect = m_WindowRect;
			m_CheckBoxRect = m_WindowRect;
			return true;
		}
		if (m_Style & 0x2000000)
		{
			m_CheckBoxRect.x = m_WindowRect.cx + m_WindowRect.x - 13;
			m_CheckBoxRect.y = m_WindowRect.y + (m_WindowRect.cy - 9) / 2;
			m_CheckBoxRect.cx = 13;
			m_CheckBoxRect.cy = 13;
			if (m_Style & 0x40000)
				m_CheckBoxRect.y = m_WindowRect.y;
			else
			if (m_Style & 0x80000)
				m_CheckBoxRect.y = m_WindowRect.y + m_WindowRect.cy - 9;

			m_TextRect.x = m_WindowRect.x;
			m_TextRect.y = m_WindowRect.y + (m_WindowRect.cy - 9) / 2;
			if (m_Style & 0x40000)
				m_TextRect.y = m_WindowRect.y;
			else
			if (m_Style & 0x80000)
				m_TextRect.y = m_WindowRect.y + m_WindowRect.cy - m_WindowDC.m_pFont->m_Height;
			m_TextRect.cx = (m_WindowRect.cx + m_WindowRect.x - 13) - m_WindowRect.x - 6;
			//
			if (m_Style & 0x80000000)
			{
				m_TextRect.cy = m_WindowRect.cy;
			} else
			{
				m_TextRect.cy = m_WindowDC.m_pFont->m_Height;
				if (m_WindowDC.m_pFont->m_Height > 13)
					m_CheckBoxRect.y += (m_WindowDC.m_pFont->m_Height - 9) / 2;
			}
		} else
		{
			m_CheckBoxRect.x = m_WindowRect.x;
			m_CheckBoxRect.y = m_WindowRect.y + (m_WindowRect.cy - 9) / 2;
			m_CheckBoxRect.cx = 13;
			m_CheckBoxRect.cy = 13;
			if (m_Style & 0x40000)
				m_CheckBoxRect.y = m_WindowRect.y;
			else
			if (m_Style & 0x80000)
				m_CheckBoxRect.y = m_WindowRect.y + m_WindowRect.cy - 9;

			m_TextRect.x = m_WindowRect.x + 19;
			m_TextRect.y = m_WindowRect.y + (m_WindowRect.cy - 9) / 2;
			if (m_Style & 0x40000)
				m_TextRect.y = m_WindowRect.y;
			else
			if (m_Style & 0x80000)
				m_TextRect.y = m_WindowRect.y + m_WindowRect.cy - m_WindowDC.m_pFont->m_Height;
			//
			if (m_Style & 0x80000000)
			{
				m_TextRect.cy = m_WindowRect.cy;
			} else
			{
				m_TextRect.cy = m_WindowDC.m_pFont->m_Height;
				if (m_WindowDC.m_pFont->m_Height > 13)
					m_CheckBoxRect.y += (m_WindowDC.m_pFont->m_Height - 9) / 2;
			}
		}
		return true;
	}
//	void CWispCheckBox::DrawFocusRect(WISP_MSG *pMsg)
//	void CWispCheckBox::DrawBorder()
//	void CWispCheckBox::DrawCheckBox(WISP_MSG *pMsg)
	void CWispCheckBox::DrawCaption()
	{
		WISP_RECT Rect = m_TextRect;
		unsigned int uFormat = 4;
		if (m_Style & 0x40000000)
		{
			uFormat = 5;
			if (m_bMouseDown && m_bMouseIn)
				++Rect.x;
		}
		if (!m_bIsMultiLine)
			uFormat |= 0x20;
		if (m_Style & 0x80000)
			uFormat |= 8;
		if ((m_Style >> 9) & 1)                          // 0x200
			m_WindowDC.SetTextColor(m_pWispBase->m_pWispDrawObj->m_crSystem[12]);
		m_WindowDC.DrawString(m_WndText.operator const WCHAR*(), &Rect, uFormat, 0, -1);
		if ((m_Style >> 9) & 1)
			m_WindowDC.SetTextColor();
	}
//	int CWispCheckBox::SetState(int newState)
//	int CWispCheckBox::GetState()
	void CWispCheckBox::Enable(bool bEnable)
	{
		if (bEnable)
			m_Style |= 0x400;
		else	m_Style &= ~0x400;
	}


#include "wispradiobox.hpp"
#include "wispbase.hpp"
#include "wispform.hpp"

	CWispRadioBox::CWispRadioBox()
	{
		m_TextRect.cy = 0;
		m_TextRect.cx = 0;
		m_TextRect.y = 0;
		m_TextRect.x = 0;

		m_RadioBoxRect.cy = 0;
		m_RadioBoxRect.cx = 0;
		m_RadioBoxRect.y = 0;
		m_RadioBoxRect.x = 0;

		m_bIsMultiLine = false;
		m_bMouseDown = false;
		m_bMouseIn = false;
		m_CtrlType = WISP_CTRL_RADIO_BOX;
	}
	CWispRadioBox::~CWispRadioBox()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispRadioBox)
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_MOUSE_LEAVE, OnMouseLeave)
		WISP_MSG_MAP(WISP_WM_MOUSE_MOVE, OnMouseMove)
	WISP_MSG_MAP_END(CWispBaseWnd)

	bool CWispRadioBox::OnUpdate(WISP_MSG *pMsg)
	{
		if (!(m_Style & 0x40))
			m_pWispBase->m_pWispDrawObj->DrawCtrlClient(&m_WindowDC, &m_WindowRect);

		if (!(m_Style & 0x40000000))
		{
			if (m_Style & 0x400) //(m_Style >> 10) & 1)
			{
				if (m_bMouseDown && m_bMouseIn)
					m_WindowDC.DrawSystemStandardIcon(&m_RadioBoxRect, 0x11, 0x21A221); //17 WISP_SSI_REDIOBOX_SELECTED_DRAK
				else	m_WindowDC.DrawSystemStandardIcon(&m_RadioBoxRect, 0x0F, 0x21A221); //15 WISP_SSI_REDIOBOX_SELECTED
			} else
			{
				if (m_bMouseDown && m_bMouseIn)
					m_WindowDC.DrawSystemStandardIcon(&m_RadioBoxRect, 0x12, 0x21A221); //18 WISP_SSI_REDIOBOX_UNSELECTED_DRAK
				else	m_WindowDC.DrawSystemStandardIcon(&m_RadioBoxRect, 0x10, 0x21A221); //16 WISP_SSI_REDIOBOX_UNSELECTED
			}
		}

		DrawCaption();
		return false;
	}
	bool CWispRadioBox::OnKeyEvent(WISP_MSG *pMsg)
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
				Msg.Command.Param1 = !!(m_Style & 0x400); //(m_Style >> 10) & 1;
				if (m_pWispBase->SendMessage(&Msg))
				{
					Msg.hWnd = (CWispWnd *)m_OwnerWnd;
					Msg.Msg = WISP_WM_EVENT;
					m_pWispBase->PostMessage(&Msg);
				}
			}
			Update((WISP_RECT*)0);
		}
		return true;
	}
	bool CWispRadioBox::OnMouseLDown(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.bKeyDown)
		{
			m_bMouseDown = true;
			return true;
		}
		return false;
	}
	bool CWispRadioBox::OnMouseLUp(WISP_MSG *pMsg)
	{
		if (!m_bMouseIn || !m_bMouseDown)
			return false;
		if (!(m_Style & 0x400)) //!((m_Style >> 10) & 1))
		{
			Enable(1);
		}
		m_bMouseDown = false;
                return true;
	}
//	bool CWispRadioBox::OnGetFocus(WISP_MSG *pMsg) { m_bFocus = 1; return true; }
//	bool CWispRadioBox::OnLostFocus(WISP_MSG *pMsg) { m_bFocus = 0; return true; }
	bool CWispRadioBox::OnMouseLeave(WISP_MSG *pMsg)
	{
		m_bMouseIn = false;
		if (m_bMouseDown == false)
			return true;
		if (!pMsg->bMsgLBTDown)
			m_bMouseDown = false;
		return false;
	}
	bool CWispRadioBox::OnMouseMove(WISP_MSG *pMsg)
	{
		m_bMouseIn = true;
		return true;
	}
	bool CWispRadioBox::InitWnd()
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
			m_RadioBoxRect = m_WindowRect;
			return true;
		}
		if (m_Style & 0x2000000)
		{
			m_RadioBoxRect.x = m_WindowRect.cx + m_WindowRect.x - 13;
			m_RadioBoxRect.y = m_WindowRect.y + (m_WindowRect.cy - 9) / 2;
			m_RadioBoxRect.cx = 13;
			m_RadioBoxRect.cy = 13;
			if (m_Style & 0x40000)
				m_RadioBoxRect.y = m_WindowRect.y;
			else
			if (m_Style & 0x80000)
				m_RadioBoxRect.y = m_WindowRect.y + m_WindowRect.cy - 9;

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
					m_RadioBoxRect.y += (m_WindowDC.m_pFont->m_Height - 9) / 2;
			}
		} else
		{
			m_RadioBoxRect.x = m_WindowRect.x;
			m_RadioBoxRect.y = m_WindowRect.y + (m_WindowRect.cy - 9) / 2;
			m_RadioBoxRect.cx = 13;
			m_RadioBoxRect.cy = 13;
			if (m_Style & 0x40000)
				m_RadioBoxRect.y = m_WindowRect.y;
			else
			if (m_Style & 0x80000)
				m_RadioBoxRect.y = m_WindowRect.y + m_WindowRect.cy - 9;

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
					m_RadioBoxRect.y += (m_WindowDC.m_pFont->m_Height - 9) / 2;
			}
		}
		return true;
	}
//	void CWispRadioBox::DrawFocusRect(WISP_MSG *pMsg)
//	void CWispRadioBox::DrawBorder()
//	void CWispRadioBox::DrawRadioBox(WISP_MSG *pMsg)
	void CWispRadioBox::DrawCaption()
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
//	int CWispRadioBox::SetState(int newState)
//	int CWispRadioBox::GetState()
	void CWispRadioBox::Enable(bool bEnable)
	{
		if (bEnable)
		{
			if (m_Style & 0x400)//(m_Style >> 10) & 1)
				return;
			m_Style |= 0x400;
			UpdateForm();
		} else
		{
			if (!(m_Style & 0x400))//!((m_Style >> 10) & 1))
				return;
			m_Style &= ~0x400;
		}
		Update((WISP_RECT*)0);
	}
	void CWispRadioBox::UpdateForm()
	{
		CWispForm *pWispForm = (CWispForm *)m_ParentWnd;
		if (pWispForm->m_CtrlType == WISP_CTRL_FORM)
		{
			unsigned int CmdID = m_CmdID;
			while (1)
			{
				//--CmdID;
				map<unsigned int, CWispBaseWnd*>::IT It = pWispForm->m_FormItemMap.find(CmdID-1);
				if (It == pWispForm->m_FormItemMap.end())
					break;
				if (It->second->m_CtrlType != WISP_CTRL_RADIO_BOX)
					break;
				if ((It->second->m_Style >> 10) & 1)
				{
					CWispRadioBox *pRadioBox = (CWispRadioBox *)It->second;
					pRadioBox->Enable(0);
				}
				CmdID = It->second->m_CmdID;
			}
			//unsigned int
			CmdID = m_CmdID;
			while (1)
			{
				//++CmdID;
				map<unsigned int, CWispBaseWnd*>::IT It = pWispForm->m_FormItemMap.find(CmdID+1);
				if (It == pWispForm->m_FormItemMap.end())
					break;
				if (It->second->m_CtrlType != WISP_CTRL_RADIO_BOX)
					break;
				if ((It->second->m_Style >> 10) & 1)
				{
					CWispRadioBox *pRadioBox = (CWispRadioBox *)It->second;
					pRadioBox->Enable(0);
				}
				CmdID = It->second->m_CmdID;
			}
		}
	}

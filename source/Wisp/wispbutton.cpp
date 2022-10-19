
#include "wispbutton.hpp"
#include "wispbase.hpp"
#include "wispwnd.hpp"

	CWispButton::CWispButton()
	{
		m_CtrlType = WISP_CTRL_BUTTON;
		m_bTextRect = 0;
	}
	CWispButton::~CWispButton()
	{
		//NONE
	}

	WISP_MSG_MAP_BEGIN(CWispButton)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_MOUSE_MOVE, OnMouseMove)
		WISP_MSG_MAP(WISP_WM_MOUSE_LEAVE, OnMouseLeave)
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_MOVE, OnMove)
	WISP_MSG_MAP_END(CWispBaseWnd)

	bool CWispButton::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.KeyType == VK_LBUTTON || pMsg->KeyEvent.KeyType == VK_RETURN)
		{
			if (pMsg->KeyEvent.bKeyDown)
			{
				if (!m_bPressed)
				{
					m_bPressed = true;
					Update((WISP_RECT*)0);

					WISP_MSG Msg;
					Msg.hWnd = m_OwnerWnd;
					Msg.hParentWnd = (CWispWnd *)this;
					Msg.Msg = WISP_WM_COMMAND;
					Msg.Command.CtrlType = WISP_CTRL_BUTTON;
					Msg.Command.CmdMsg = 0x80000001; //WISP_ID_KEYDOWN
					Msg.Command.CmdID = m_CmdID;
					m_pWispBase->SendMessage(&Msg); //SendCommand(
				}
			} else
			if (m_bPressed)
			{
				m_bPressed = false;
				Update((WISP_RECT*)0);

				WISP_MSG Msg;
				Msg.hWnd = m_OwnerWnd;
				Msg.hParentWnd = (CWispWnd *)this;
				Msg.Msg = WISP_WM_COMMAND;
				Msg.Command.CtrlType = WISP_CTRL_BUTTON;
				Msg.Command.CmdMsg = 0x80000002; //WISP_ID_KEYUP
				Msg.Command.CmdID = m_CmdID;
				if (m_pWispBase->SendMessage(&Msg)) //SendCommand(
				{
					Msg.hWnd = m_OwnerWnd;
					Msg.Msg = WISP_WM_EVENT;
					m_pWispBase->PostMessage(&Msg);
				}
			}
		}
		return true;
	}
	void CWispButton::RecalcLayout()
	{
		CWispBaseWnd::RecalcLayout();
		//???
		if ((m_Style >> 9) & 1)
		{
			if (m_bMouseInBTRect)
				m_bMouseInBTRect = false;
		} else
		{
			if (PtInRect(&m_ScrWindowRect, &m_pWispBase->m_MousePT) != m_bMouseInBTRect)
			{
				m_bMouseInBTRect = m_bMouseInBTRect == false;
				Update((WISP_RECT*)0);
			}
		}
	}
	bool CWispButton::OnMouseMove(WISP_MSG *pMsg)
	{
		if (!m_bMouseInBTRect)
		{
			OnButtonEnter();
			SendCommand(WISP_ID_ENTER, 0, 0);
			m_bMouseInBTRect = true;
			Update((WISP_RECT*)0);
		}
		return true;
	}
	bool CWispButton::OnMouseLeave(WISP_MSG *pMsg)
	{
		m_bMouseInBTRect = false;
		m_bPressed = false;
		OnButtonLeave();
		SendCommand(WISP_ID_LEAVE, 0, 0);
		Update((WISP_RECT*)0);
		return true;
	}
	bool CWispButton::OnMove(WISP_MSG *pMsg)
	{
		if (m_bMouseInBTRect)
		{
			m_bMouseInBTRect = PtInRect(&m_ScrWindowRect, &m_pWispBase->m_MousePT);
			Update((WISP_RECT*)0);
		}
		return true;
	}
	bool CWispButton::OnUpdate(WISP_MSG *pMsg)
	{
		//unsigned int State;
		unsigned int uStyle;
		unsigned long OrgTextColor;
		//WISP_RECT rcBtn;
		int x, y;

		unsigned int State = 0; //WISP_BT_DIB_NORMAL
		if (m_bMouseInBTRect)
			State = 1; //WISP_BT_DIB_HOVER
		if (m_bPressed)
			State = 2; //WISP_BT_DIB_DOWN

		WISP_RECT rcBtn = m_WindowRect;

		if ( (m_Style & 0x40) == 0 )
			m_pWispBase->m_pWispDrawObj->DrawButtonClient(
				&m_WindowDC, &m_WindowRect, State);

		if (m_DIBList[0]) //!= WISP_BT_DIB_NORMAL
		{
			if ( (m_Style >> 9) & 1 )
			{
				if (!m_DIBList[3]) //WISP_BT_DIB_DISABLE
				{
					m_WindowDC.DrawDIBGray(
						(m_WindowRect.cx - m_DIBList[0]->m_PixelBuf.Width) / 2,
						(m_WindowRect.cy - m_DIBList[0]->m_PixelBuf.Height) / 2,
						m_DIBList[0]); //WISP_BT_DIB_NORMAL
				} else
				{
					m_WindowDC.DrawDIB(
						(m_WindowRect.cx - m_DIBList[3]->m_PixelBuf.Width) / 2,
						(m_WindowRect.cy - m_DIBList[3]->m_PixelBuf.Height) / 2,
						m_DIBList[3]); //WISP_BT_DIB_DISABLE
				}
			} else
			if (m_bPressed)
			{
				if (!m_DIBList[2]) //WISP_BT_DIB_DOWN
				{
					m_WindowDC.DrawDIB(
						(m_WindowRect.cx - m_DIBList[0]->m_PixelBuf.Width) / 2+1,
						(m_WindowRect.cy - m_DIBList[0]->m_PixelBuf.Height) / 2+1,
						m_DIBList[0]); //WISP_BT_DIB_NORMAL
				} else
				{
					m_WindowDC.DrawDIB(
						(m_WindowRect.cx - m_DIBList[2]->m_PixelBuf.Width) / 2,
						(m_WindowRect.cy - m_DIBList[2]->m_PixelBuf.Height) / 2,
						m_DIBList[2]); //WISP_BT_DIB_DOWN


				}
			} else
			{
				if (m_bMouseInBTRect && m_DIBList[1]) //WISP_BT_DIB_HOVER
				{
					m_WindowDC.DrawDIB(
						(m_WindowRect.cx - m_DIBList[1]->m_PixelBuf.Width) / 2,
						(m_WindowRect.cy - m_DIBList[1]->m_PixelBuf.Height) / 2,
						m_DIBList[1]); //WISP_BT_DIB_HOVER


				} else
				{
					m_WindowDC.DrawDIB(
						(m_WindowRect.cx - m_DIBList[0]->m_PixelBuf.Width) / 2,
						(m_WindowRect.cy - m_DIBList[0]->m_PixelBuf.Height) / 2,
						m_DIBList[0]); //WISP_BT_DIB_NORMAL
				}
			}
		}

		if (m_Style & 0x1000000)
		{
			if (State == 1) //WISP_BT_DIB_HOVER
			{
				m_WindowDC.DrawFrameRect(&m_WindowRect, 1, 1);
			} else
			if (State == 2) //WISP_BT_DIB_DOWN
			{
				m_WindowDC.DrawFrameRect(&m_WindowRect, 0, 1);
			}
		} else
		if ( !(m_Style & 0x800000) )
		{
			if (!m_DIBList[State])
				m_pWispBase->m_pWispDrawObj->DrawButtonFrameRect(
					&m_WindowDC, &rcBtn, State);
			if (State == 1) //WISP_BT_DIB_HOVER
			{
				rcBtn.x += 3;
				rcBtn.y += 3;
				rcBtn.cx -= 6;
				rcBtn.cy -= 6;
			} else
			{
				rcBtn.x += 1;
				rcBtn.y += 1;
				rcBtn.cx -= 2;
				rcBtn.cy -= 2;
			}
		}

		if (m_WndText.m_nLength)
		{
			if (m_bTextRect)
			{
				rcBtn = m_TextRect;
			}

			if (State != 1) //WISP_BT_DIB_HOVER
			{
				rcBtn.x += 2;
				rcBtn.y += 2;
				rcBtn.cx -= 4;
				rcBtn.cy -= 4;
			}

			uStyle = 0x24;
			if ( (m_Style & 0x30000) != 0x10000)
			{
				uStyle = 0x25;
				if ( (m_Style & 0x30000) == 0x20000)
					uStyle = 0x26;
			}

			if ( (m_Style >> 9) & 1 )
			{
				OrgTextColor = m_WindowDC.m_TextColor;
				m_WindowDC.m_OldTextColor = m_WindowDC.m_TextColor;
				m_WindowDC.m_TextColor = m_pWispBase->m_pWispDrawObj->m_crSystem[12];
			}

			if (State == 2) //WISP_BT_DIB_DOWN
			{
				++rcBtn.x;
				++rcBtn.y;
			}

			m_WindowDC.DrawString(m_WndText.operator const WCHAR*(), &rcBtn, uStyle, 0, -1);

			if ( (m_Style >> 9) & 1 )
			{
				m_WindowDC.m_OldTextColor = m_WindowDC.m_TextColor;
				m_WindowDC.m_TextColor = OrgTextColor;
			}
		}

		return false;
	}

	bool CWispButton::InitWnd()
	{
		if (!CWispBaseWnd::InitWnd())
			return false;

		m_bPressed = false;
		m_bMouseInBTRect = PtInRect(&m_ScrWindowRect, &m_pWispBase->m_MousePT);

		m_DIBList[0] = 0;	//SetDIBList(0);
		m_DIBList[1] = 0;
		m_DIBList[2] = 0;
		m_DIBList[3] = 0;

		return true;
	}

	void CWispButton::OnButtonEnter()
	{
		//NONE
	}
	void CWispButton::OnButtonLeave()
	{
		//NONE
	}

	bool CWispButton::CreateEx(CWispDIBList *pDIBList, int x, int y, CWispWnd *pParentWnd, unsigned int CmdID, unsigned int Style)
	{
		if (!Create(0, x,y,
			pDIBList->m_DIBList[0].m_PixelBuf.Width,
			pDIBList->m_DIBList[0].m_PixelBuf.Height,
			(CWispBaseWnd*)pParentWnd, CmdID, Style, WISP_SH_MINSIZE))
			return false;
		SetDIBList(pDIBList);
		return true;
	}
	bool CWispButton::CreateEx(CWispDIB *pDIB, int x, int y, CWispWnd *pParentWnd, unsigned int CmdID, unsigned int Style)
	{
		if (!Create(0, x,y,
			pDIB->m_PixelBuf.Width,
			pDIB->m_PixelBuf.Height,
			(CWispBaseWnd*)pParentWnd, CmdID, Style, WISP_SH_MINSIZE))
			return false;
		SetDIB(pDIB, WISP_BT_DIB_NORMAL); //m_DIBList[0] = pDIB;
		return true;
	}
	bool CWispButton::SetDIB(CWispDIB *pDIB, WISP_BT_DIB DIB)
	{
		//if (!(m_Style & 0x20)) return false; //BYTE2()
		m_DIBList[DIB] = pDIB;
		return true;
	}
	bool CWispButton::SetDIBList(CWispDIBList *pDIBList)
	{
		for (int n = 0; n < 4; ++n)
			m_DIBList[n] = pDIBList->GetDIB(n);
		return true;
	}

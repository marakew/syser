
#include "wispcolorstrwnd.hpp"
#include "wispbase.hpp"

	CWispColorStrWnd::CWispColorStrWnd()
	{
		m_MaxChars = 0;
		m_MaxLines = 0;
		m_Margin = 5;
	}
	CWispColorStrWnd::~CWispColorStrWnd()
	{
		//NONE
	}

	WISP_MSG_MAP_BEGIN(CWispColorStrWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_RECALC_LAYOUT, OnRecalcLayout)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_MOUSE_MOVE, OnMouseMove)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
		WISP_MSG_MAP(WISP_WM_SCROLL_EVENT, OnScrollEvent)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispWnd)

	bool CWispColorStrWnd::OnCreate(WISP_MSG *pMsg)
	{
		m_bSelect = false;
		return true;
	}
	bool CWispColorStrWnd::OnUpdateClient(WISP_MSG *pMsg)
	{
		CWispWnd::OnUpdateClient(pMsg);
		unsigned long CSBuffer[WISP_TERMINAL_LINE_SIZE+1];
		for (int line = 0; line < m_nLinesPerPage; ++line)
		{
			int PosY = line + m_ScrPos.y;
			CSBuffer[0] = 0;
			char BKClr = GetLineStr(PosY, CSBuffer);
			if (BKClr)
			{
				WISP_RECT Rect;
				Rect.x = 0;
				Rect.y = m_Margin + line * m_ClientDC.m_pFont->m_Height;
				Rect.cx = m_ClientRect.cx;
				Rect.cy = m_ClientDC.m_pFont->m_Height;
				m_ClientDC.DrawFullRect(&Rect, m_pWispBase->m_pWispDrawObj->m_crSystem[BKClr]);
			}

			int Length = TStrLen(CSBuffer);
			if (Length > 0)
			{
				if (m_Style & 0x20000 && m_bSelect &&
					PosY >= m_pSelStartPos->y && PosY <= m_pSelEndPos->y)
				{
					int PosX = m_Margin - m_ScrPos.x * m_ClientDC.m_pFont->m_Width;
					int n = 0;
					do {
						if (IsSelectChar(m_ScrPos.x + (PosX - m_Margin) / m_ClientDC.m_pFont->m_Width, PosY))
						{
							unsigned long Char = (CSBuffer[n + m_ScrPos.x] & 0xFFFF) + 0x19010000;
							PosX += m_ClientDC.DrawColorString(PosX, m_Margin + line * m_ClientDC.m_pFont->m_Height, &Char, 1, n + m_ScrPos.x);
						} else
						{
							unsigned long Char = (CSBuffer[n + m_ScrPos.x]);
							PosX += m_ClientDC.DrawColorString(PosX, m_Margin + line * m_ClientDC.m_pFont->m_Height, &Char, 1, n + m_ScrPos.x);
						}
						++n;
					} while (n < Length);
				} else
				{
					m_ClientDC.DrawColorString(
						m_Margin - m_ScrPos.x * m_ClientDC.m_pFont->m_Width,
						m_Margin + line * m_ClientDC.m_pFont->m_Height,
						CSBuffer, Length, 0);
				}
			}
		}
		return false;
	}
	bool CWispColorStrWnd::OnScrollEvent(WISP_MSG *pMsg)
	{
		CWispWnd::OnScrollEvent(pMsg);
		if (pMsg->ScrollEvent.Type == 1)
			m_ScrPos.x = m_pScrollBar[1]->CurPos;
		else	m_ScrPos.y = m_pScrollBar[0]->CurPos;
		UpdateCaretState();
		Update(&m_ClientRect);
		return false;
	}
	bool CWispColorStrWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (!pMsg->KeyEvent.bKeyDown)
			return true;

		WISP_MSG Msg;

		switch (pMsg->KeyEvent.KeyType)
		{
		case 2: //VK_RBUTTON
		case 0x4000002D: //WISP_SK_CTRL + VK_INSERT
		case 0x40000043: //WISP_SK_CTRL + C
			Msg.hWnd = this;
			Msg.Msg = WISP_WM_EVENT;
			Msg.Command.CmdID = WISP_ID_COPY;
			m_pWispBase->PostMessage(&Msg);
			return false;
		case 0x1000002D: //WISP_SK_SHIFT + VK_INSERT
		case 0x40000056: //WISP_SK_CTRL + V
			Msg.hWnd = this;
			Msg.Msg = WISP_WM_EVENT;
			Msg.Command.CmdID = WISP_ID_PASTE;
			m_pWispBase->PostMessage(&Msg);
			return false;
		}

		if (m_Style & 0x20000)
		{
			if (pMsg->KeyEvent.KeyType == VK_LBUTTON)
			{
				m_bSelect = false;
				CalcPonitStrPos(&pMsg->MsgMouseWndPT, &m_RealSelStartPos);
				m_RealSelEndPos.x = -1;
				m_RealSelEndPos.y = -1;
				m_pSelStartPos = &m_RealSelStartPos;
				m_pSelEndPos = &m_RealSelEndPos;
				Update((WISP_RECT*)0);
			}
		}

		if (m_Style & 0x10000)
		{
			if (pMsg->KeyEvent.KeyType == VK_LBUTTON)
			{
				ResetSelect();
				m_CaretPos.x = ROUNDPREV(pMsg->MsgMouseWndPT.x, m_ClientDC.m_pFont->m_Width);
				m_CaretPos.x /= m_ClientDC.m_pFont->m_Width;
				m_CaretPos.y = ROUNDPREV(pMsg->MsgMouseWndPT.y, m_ClientDC.m_pFont->m_Height);
				m_CaretPos.y /= m_ClientDC.m_pFont->m_Height;
				SetCaretPos(m_CaretPos.x * m_ClientDC.m_pFont->m_Width, m_CaretPos.y * m_ClientDC.m_pFont->m_Height);
				m_CaretPos.y += m_ScrPos.y;
				m_CaretPos.x += m_ScrPos.x;
				RecalcLayout();
				Update((WISP_RECT*)0);
			}

			switch (pMsg->KeyEvent.KeyType)
			{
			case VK_LEFT:
				ResetSelect();
				if (m_CaretPos.x <= 0)
					break;
				if (m_CaretPos.x <= m_ScrPos.x)
					SendScrollEvent(1, 1);// HorzScrollLineUp()
				--m_CaretPos.x;
				RecalcLayout();
				Update((WISP_RECT*)0);
				break;
			case VK_UP:
				ResetSelect();
				if (m_CaretPos.y <= 0)
					break;
				if (m_CaretPos.y <= m_ScrPos.y)
					SendScrollEvent(0, 1);// VertScrollLineUp()
				--m_CaretPos.y;
				RecalcLayout();
				Update((WISP_RECT*)0);
				break;
			case VK_RIGHT:
				ResetSelect();
				if (m_CaretPos.x + 1 >= m_MaxChars)
					break;
				if (m_CaretPos.x + 1 >= m_ScrPos.x + m_nCharsPerLine)
				SendScrollEvent(1, 0);// HorzScrollLineDown()
				++m_CaretPos.x;
				RecalcLayout();
				Update((WISP_RECT*)0);
				break;
			case VK_DOWN:
				ResetSelect();
				if (m_CaretPos.y + 1 >= m_MaxLines)
					break;
				if (m_CaretPos.y + 1 >= m_ScrPos.y + m_nLinesPerPage)
					SendScrollEvent(0, 0);// VertScrollLineDown()
				++m_CaretPos.y;
				RecalcLayout();
				Update((WISP_RECT*)0);
				break;
			default: ;
			}
		}
		return true;
	}
	bool CWispColorStrWnd::OnRecalcLayout(WISP_MSG *pMsg)
	{
		m_nCharsPerLine = (m_ClientRect.cx - 2 * m_Margin) / m_ClientDC.m_pFont->m_Width;
		MIN_LIMIT(m_nCharsPerLine, 0);

		m_nLinesPerPage = (m_ClientRect.cy - m_Margin) / m_ClientDC.m_pFont->m_Height;
		MIN_LIMIT(m_nLinesPerPage, 0);

		if (m_nLinesPerPage >= m_MaxLines && m_ScrPos.y)
			m_ScrPos.y = 0;

		CWispWnd::SetScrollBarInfo(0, m_ScrPos.y, m_MaxLines, m_nLinesPerPage, 0, 1);
		CWispWnd::SetScrollBarInfo(1, m_ScrPos.x, m_MaxChars, m_nCharsPerLine, 0, 1);
		UpdateCaretState();
		return true;
	}
	bool CWispColorStrWnd::OnMouseMove(WISP_MSG *pMsg)
	{
		if (!PtInRect(&m_ClientRect, &pMsg->MsgMouseWndPT))
			return true;
		
		if (!pMsg->bMsgLBTDown)
			return true;
		
		if ( !(m_Style & 0x20000))
			return true;
		
		if (m_RealSelStartPos.x < 0 || m_RealSelStartPos.y < 0)
			return true;
		
		CalcPonitStrPos(&pMsg->MsgMouseWndPT, &m_RealSelEndPos);

		if (m_RealSelEndPos.x < 0 || m_RealSelEndPos.y < 0)
			return true;
		
		if (m_RealSelStartPos.x < 0 || m_RealSelStartPos.y < 0) //??? DUP
			return true;
		
		m_bSelect = true;
		//UpdateSelect();
		if (m_RealSelStartPos.y <= m_RealSelEndPos.y &&
			(m_RealSelStartPos.y != m_RealSelEndPos.y ||
			m_RealSelStartPos.x <= m_RealSelEndPos.x))
		{
			m_pSelStartPos = &m_RealSelStartPos;
			m_pSelEndPos = &m_RealSelEndPos;
		} else
		{
			m_pSelStartPos = &m_RealSelEndPos;
			m_pSelEndPos = &m_RealSelStartPos;
		}
		Update((WISP_RECT*)0);
		return true;
	}

	WISP_EVENT_MAP_BEGIN(CWispColorStrWnd)
		WISP_EVENT_MAP(WISP_ID_COPY, OnEventCopy)
	WISP_EVENT_MAP_END

	bool CWispColorStrWnd::OnEventCopy(WISP_MSG *pMsg)
	{
		if (!m_bSelect)
			return false;
		unsigned long CSBuffer[WISP_TERMINAL_LINE_SIZE+1]; //257
		WCHAR szBuffer[WISP_TERMINAL_LINE_SIZE]; //258
		TWideString String;
		for (int ScrPosY = m_pSelStartPos->y; ScrPosY <= m_pSelEndPos->y; ++ScrPosY)
		{
			if (ScrPosY != m_pSelStartPos->y)
				String += WSTR("\r\n");
			CSBuffer[0] = 0;
			GetLineStr(ScrPosY, CSBuffer);
			TStrCpy(szBuffer, CSBuffer);

			if (m_pSelStartPos->y == m_pSelEndPos->y)
			{
				int Len = m_pSelEndPos->x - m_pSelStartPos->x + 1;
				if (Len > 0)
				{
					szBuffer[MIN(m_pSelStartPos->x + Len, WISP_TERMINAL_LINE_SIZE)] = 0; //???
					String += &szBuffer[m_pSelStartPos->x];
				}
			} else
			if (ScrPosY >= m_pSelEndPos->y)
			{
				if (ScrPosY == m_pSelEndPos->y)
				{
					szBuffer[MIN(m_pSelEndPos->x + 1, WISP_TERMINAL_LINE_SIZE)] = 0; //???
					String += szBuffer;
				}
			} else
			if (ScrPosY == m_pSelStartPos->y)
			{
				String += &szBuffer[m_pSelStartPos->x];
			} else
			{
				String += szBuffer;
			}
		}
		m_pWispBase->SetClipboard(String.operator const WCHAR*());
		m_bSelect = false;
		Update(&m_ClientRect);
		return true;
	}

	bool CWispColorStrWnd::InitWnd()
	{
		if (!CWispWnd::InitWnd())
			return false;
		m_ScrPos.y = 0;
		m_ScrPos.x = 0;
		m_CaretPos.y = 0;
		m_CaretPos.x = 0;
		return true;
	}

//	unsigned char CWispColorStrWnd::GetLineStr(int LineIndex, unsigned long *pColorStr) = 0;

	void CWispColorStrWnd::SetMaxLines(int nLine)
	{
		m_MaxLines = nLine;
	}
	void CWispColorStrWnd::SetMaxChars(int nChar)
	{
		m_MaxChars = nChar;
	}

	void CWispColorStrWnd::CalcPonitStrPos(WISP_POINT *pPoint, WISP_POINT *pStrPT)
	{
		pStrPT->x = ROUNDPREV((pPoint->x - m_Margin), m_ClientDC.m_pFont->m_Width);
		pStrPT->x /= m_ClientDC.m_pFont->m_Width;

		if ((pPoint->x - m_Margin) < m_ClientDC.m_pFont->m_Width/2 && pStrPT->x > 0)
			--pStrPT->x;

		pStrPT->y = ROUNDPREV((pPoint->y - m_Margin), m_ClientDC.m_pFont->m_Height);
		pStrPT->y /= m_ClientDC.m_pFont->m_Height;

		pStrPT->y += m_ScrPos.y;
		pStrPT->x += m_ScrPos.x;
	}

	void CWispColorStrWnd::UpdateCaretState()
	{
		if (m_CaretPos.x >= m_ScrPos.x &&  m_CaretPos.x < m_ScrPos.x + m_nCharsPerLine &&
		    m_CaretPos.y >= m_ScrPos.y &&  m_CaretPos.y < m_ScrPos.y + m_nLinesPerPage )
		{
			CWispWnd::SetCaretPos(
				m_Margin + m_ClientDC.m_pFont->m_Width * (m_CaretPos.x - m_ScrPos.x),
				m_Margin + m_ClientDC.m_pFont->m_Height * (m_CaretPos.y - m_ScrPos.y));
			CWispWnd::ShowCaret(true);
		} else
		{
			CWispWnd::ShowCaret(false);
		}
	}

	void CWispColorStrWnd::UpdateSelect()
	{
		if (m_RealSelStartPos.y <= m_RealSelEndPos.y &&
			(m_RealSelStartPos.y != m_RealSelEndPos.y || m_RealSelStartPos.x <= m_RealSelEndPos.x))
		{
			m_pSelStartPos = &m_RealSelStartPos;
			m_pSelEndPos = &m_RealSelEndPos;
		} else
		{
			m_pSelStartPos = &m_RealSelEndPos;
			m_pSelEndPos = &m_RealSelStartPos;
		}
		Update((WISP_RECT*)0);
	}

	void CWispColorStrWnd::ResetSelect()
	{
		if (m_bSelect)
		{
			m_bSelect = false;
			m_RealSelStartPos.y = -1;
			m_RealSelStartPos.x = -1;
			m_RealSelEndPos.y = -1;
			m_RealSelEndPos.x = -1;
		}
	}

	bool CWispColorStrWnd::IsSelectChar(int ScrX, int ScrY)
	{
		if (m_pSelEndPos->x == -1)
			return false;

		if (m_pSelEndPos->y == -1)
			return false;

		return ScrY >= m_pSelStartPos->y && ScrY <= m_pSelEndPos->y &&
			(ScrY != m_pSelStartPos->y || ScrX >= m_pSelStartPos->x) &&
			(ScrY != m_pSelEndPos->y || ScrX <= m_pSelEndPos->x);
	}


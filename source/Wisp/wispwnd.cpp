
#include "../Code/define.h"
//#include "../Code/osdefine.hpp"
#include "wispwnd.hpp"
#include "wispbase.hpp"
#include "wisptipwnd.hpp"

	struct SCROLL_AREA
	{
		unsigned int PageDec;	//BTDec
		unsigned int PageInc;	//BTInc
		unsigned int Slide;	//Thumb
		unsigned int Track;
		unsigned int None;
	};

	SCROLL_AREA Scroll_NUM[WISP_SCROLL_BAR_COUNT] = {
			{   1,    2,    3,    4,    5},
			{0x11, 0x12, 0x13, 0x14, 0x15} }; //0x10 |

	struct SCROLL_WM
	{
		unsigned int PageDec;	//BTDec
		unsigned int PageInc;	//BTInc
		unsigned int SLIDE;
		unsigned int BTDec;
		unsigned int BTInc;
	};


	SCROLL_WM Scroll_WM[WISP_SCROLL_BAR_COUNT] = 

{ { WISP_ID_VPAGE_DEC, WISP_ID_VPAGE_INC, WISP_ID_VSPIN_SCROLL, WISP_ID_VSPIN_DEC, WISP_ID_VSPIN_INC },
  { WISP_ID_HPAGE_DEC, WISP_ID_HPAGE_INC, WISP_ID_HSPIN_SCROLL, WISP_ID_HSPIN_DEC, WISP_ID_HSPIN_INC } };

	bool CWispWnd::OnCmdClose(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000002) //WISP_ID_KEYUP
		{
			WISP_MSG Msg;
			Msg.hWnd = this;
			Msg.Msg = WISP_WM_CLOSE;
			return m_pWispBase->SendMessage(&Msg) == 0;
		}
		return true;
	}
	bool CWispWnd::OnCmdMax(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000002) //WISP_ID_KEYUP
		{
			if (m_State & 8) //WISP_WSTATE_MINIMIZE
			{
				return true; //???
			}
			
			if (m_State & 4) //WISP_WSTATE_MAXIMIZE
			{
				MoveToScreen(m_NormalRect.x, m_NormalRect.y, true);
				Resize(m_NormalRect.cx, m_NormalRect.cy, true);
				m_State &= ~4; //~WISP_WSTATE_MAXIMIZE
				Update((WISP_RECT*)0);
				return true;
			}

			if (m_ParentWnd)
			{
				m_NormalRect = m_ScrWindowRect;
				MoveToWindow(0, 0, true);
				Resize(
					((CWispWnd*)m_ParentWnd)->m_ClientRect.cx,
					((CWispWnd*)m_ParentWnd)->m_ClientRect.cy, true);
				m_State |= 4; //WISP_WSTATE_MAXIMIZE
				Update((WISP_RECT*)0);
			}
			
		}
		return true;
	}
	bool CWispWnd::OnCmdMin(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000002) //WISP_ID_KEYUP
		{
			if (m_State & 4) //WISP_WSTATE_MAXIMIZE
			{
				return true; //???
			}
			
			if (m_State & 8) //WISP_WSTATE_MINIMIZE
			{
				Resize(m_NormalRect.cx, m_NormalRect.cy, true);
				m_State &= ~8; //~WISP_WSTATE_MINIMIZE
				Update((WISP_RECT*)0);
				return true;
			}

			m_NormalRect = m_ScrWindowRect;
			Resize(m_MinWndSize.cx, m_MinWndSize.cy, true);
			m_State |= 8; //WISP_WSTATE_MINIMIZE
			Update((WISP_RECT*)0);
		}
		return true;
	}
	bool CWispWnd::OnCmdScrollBarDrag(WISP_MSG *pMsg)
	{
		int type = pMsg->Command.Param1;

		if (pMsg->Command.CmdMsg == 0x80000001) //WISP_ID_KEYDOWN
		{
			if (m_pScrollBar[type]->Style & 2)
				m_pScrollBar[type]->DragPos = m_pScrollBar[type]->CurPos;
			m_pScrollBar[type]->State = 2;
			m_pWispBase->BeginDrag();
		} else
		if (pMsg->Command.CmdMsg == 0x80000002) //WISP_ID_KEYUP
		{
			if (m_pScrollBar[type]->Style & 2)
			{
				WISP_MSG Msg;
				Msg.hWnd = this;
				Msg.Msg = WISP_WM_SCROLL_EVENT;
				Msg.ScrollEvent.Event = WISP_SB_POS_CHANGE;
				Msg.ScrollEvent.Type = type;
				Msg.ScrollEvent.CurPos = m_pScrollBar[type]->CurPos;
				Msg.ScrollEvent.Delta = m_pScrollBar[type]->CurPos - m_pScrollBar[type]->DragPos;
				m_pWispBase->SendMessage(&Msg);
			}
			m_pScrollBar[type]->State = 0;
			m_pWispBase->EndDrag();
		}
		return true;
	}

	CWispWnd::CWispWnd()
	{
		ClearBGColor();
		
		m_pBKDIB = 0;
		m_CtrlType = WISP_CTRL_WND;
	}

	CWispWnd::~CWispWnd()
	{
	}

	unsigned int CWispWnd::IsPointInCaptionBT(WISP_POINT & ScrMousePT)
	{
		if (!PtInRect(&m_ScrCaptionRect, ScrMousePT))
			return 0;

		WISP_POINT pt = ScrMousePT;

		//CAPTION
		if (m_Style & 1)
		{
			PointToRect(&pt, &m_ScrCaptionRect);
			if (m_Style & 0x08 && PtInRect(&m_CloseBTRect, &pt))
				return WISP_ID_CLOSE;
			if (m_Style & 0x10 && PtInRect(&m_MaxBTRect, &pt))
				return WISP_ID_MAX;
			if (m_Style & 0x20 && PtInRect(&m_MinBTRect, &pt))
				return WISP_ID_MIN;
		}
		return 0;
	}
	bool CWispWnd::OnHitTest(WISP_MSG *pMsg)
	{
		if ((m_Style & 0x80) && m_State == WISP_WSTATE_RESET) //???
		{
			WISP_POINT WndPT = pMsg->MsgMouseScrPT;
			PointToRect(&WndPT, &m_ScrWindowRect);

			int v3 = m_BorderSize + m_pWispBase->m_Metrics[WISP_SM_RESIZE_SIZE];
			if (WndPT.x < v3 && WndPT.y < v3)
			{
				pMsg->HitTest.HitArea.Type = WISP_HT_TOPLEFT; //0x50000;
				return true;
			}
			if (WndPT.x > m_WindowRect.cx - v3 && WndPT.y < v3)
			{
				pMsg->HitTest.HitArea.Type = WISP_HT_TOPRIGHT; //0x60000;
				return true;
			}
			if (WndPT.x < v3 && WndPT.y > m_WindowRect.cy - v3)
			{
				pMsg->HitTest.HitArea.Type = WISP_HT_BOTTOMLEFT; //0x90000;
				return true;
			}
			if (WndPT.x > m_WindowRect.cx - v3 && WndPT.y > m_WindowRect.cy - v3)
			{
				pMsg->HitTest.HitArea.Type = WISP_HT_BOTTOMRIGHT; //0xA0000;
				return true;
			}
			if (WndPT.x < m_BorderSize)
				pMsg->HitTest.HitArea.Type |= WISP_HT_LEFT; //0x10000;
			if (WndPT.y < m_BorderSize)
				pMsg->HitTest.HitArea.Type |= WISP_HT_TOP; //0x40000;
			if (WndPT.x > m_WindowRect.cx - m_BorderSize)
				pMsg->HitTest.HitArea.Type |= WISP_HT_RIGHT; //0x20000;
			if (WndPT.y > m_WindowRect.cy - m_BorderSize)
				pMsg->HitTest.HitArea.Type |= WISP_HT_BOTTOM; //0x80000;

			if (pMsg->HitTest.HitArea.Type)
				return true;
		}

		if ( !(m_Style & 1) || !PtInRect(&m_ScrCaptionRect, &pMsg->MsgMouseScrPT))
		{
			int n = 0;
			int result;
			while (n < 2)
			{
				if (m_pScrollBar[n] && !m_pScrollBar[n]->bHide &&
					PtInRect(&m_pScrollBar[n]->BarRect, &pMsg->MsgMouseWndPT))
				{
					pMsg->HitTest.HitArea.Type = 3;
					if (PtInRect(&m_pScrollBar[n]->SlideRect, &pMsg->MsgMouseWndPT))
					{
						pMsg->HitTest.HitArea.State = Scroll_NUM[n].Slide;
						result = 1;
					} else
					if (PtInRect(&m_pScrollBar[n]->TrackRect, &pMsg->MsgMouseWndPT))
					{
						pMsg->HitTest.HitArea.State = Scroll_NUM[n].Track;
						result = 1;
					} else
					if (PtInRect(&m_pScrollBar[n]->PageIncRect, &pMsg->MsgMouseWndPT))
					{
						pMsg->HitTest.HitArea.State = Scroll_NUM[n].PageInc;
						result = 1;
					} else
					if (PtInRect(&m_pScrollBar[n]->PageDecRect, &pMsg->MsgMouseWndPT))
					{
						pMsg->HitTest.HitArea.State = Scroll_NUM[n].PageDec;
						result = 1;
					} else
					{
						pMsg->HitTest.HitArea.State = Scroll_NUM[n].None;
						result = 1;
					}
					return result;
				}
				++n;
			};

			if (PtInRect(&m_ScrClientRect, &pMsg->MsgMouseScrPT))
			{
				pMsg->HitTest.HitArea.Type = 1;
				return true;
			}

			if ( pMsg->MsgMouseScrPT.x > m_ClientRect.x2() ||
			     pMsg->MsgMouseScrPT.y > m_ClientRect.y2() )
			{
				pMsg->HitTest.HitArea.Type = 3;
			}
			return true;
		}

		//CAPTION

		if (!pMsg->HitTest.bKeyDown)
		{
			pMsg->HitTest.HitArea.Type = 2;
			return true;
		}

		unsigned int CapBTID = IsPointInCaptionBT(pMsg->MsgMouseScrPT);
		if (CapBTID == WISP_ID_CLOSE)
		{
			pMsg->HitTest.HitArea.Type = 4;
			m_CloseBTState = 2;
		} else
		if (CapBTID == WISP_ID_MAX)
		{
			pMsg->HitTest.HitArea.Type = 5;
			m_MaxBTState = 2;
		} else
		if (CapBTID == WISP_ID_MIN)
		{
			pMsg->HitTest.HitArea.Type = 6;
			m_MinBTState = 2;
		} else
		{
			pMsg->HitTest.HitArea.Type = 2;
			return true;
		}

		Update(&m_CaptionRect);

		WISP_MSG Msg;
		Msg.hWnd = this;
		Msg.Msg = WISP_WM_COMMAND;
		Msg.Command.CtrlType = 1;
		Msg.Command.CmdMsg = 0x80000001; //WISP_ID_KEYDOWN
		Msg.Command.CmdID = CapBTID;
		m_pWispBase->SendMessage(&Msg);
		return true;
	}

	bool CWispWnd::OnSizing(WISP_MSG *pMsg)		//?????
	{
		if (m_pWispBase->m_pRootWnd == this)
		{
			m_pWispBase->m_Metrics[WISP_SM_SCREEN_CX] = pMsg->Sizing.Size.cx;
			m_pWispBase->m_Metrics[WISP_SM_SCREEN_CY] = pMsg->Sizing.Size.cy;

			if (!m_pWispBase->BeginResize(pMsg->Sizing.Size.cx, pMsg->Sizing.Size.cy))
				return false;
		}

		return pMsg->Sizing.Size.cx >= m_MinWndSize.cx && pMsg->Sizing.Size.cy >= m_MinWndSize.cy;
	}
	bool CWispWnd::OnMoving(WISP_MSG *pMsg)
	{
		if (m_pWispBase->m_pRootWnd == this)
		{
			m_pWispBase->m_MousePT.x -= pMsg->Move.DeltaX;
			m_pWispBase->m_MousePT.y -= pMsg->Move.DeltaY;
			m_pWispBase->m_PrevMousePT.x -= pMsg->Move.DeltaX;
			m_pWispBase->m_PrevMousePT.y -= pMsg->Move.DeltaY;
			m_pWispBase->MovingFrame(pMsg->Move.DeltaX, pMsg->Move.DeltaY);
			return false;
		}
		return true;
	}
	void CWispWnd::CalcVertScrollSlideWithPos()
	{
		if (m_pScrollBar[0])
		{
			m_pScrollBar[0]->PageDecRect.cy = m_pScrollBar[0]->BarSize;
			m_pScrollBar[0]->PageDecRect.cx = m_pScrollBar[0]->PageDecRect.cy;
			m_pScrollBar[0]->PageIncRect.cy = m_pScrollBar[0]->PageDecRect.cx;
			m_pScrollBar[0]->PageIncRect.cx = m_pScrollBar[0]->PageIncRect.cy;
			m_pScrollBar[0]->BarRect.x = m_ClientRect.cx + m_ClientRect.x;
			m_pScrollBar[0]->PageIncRect.x = m_pScrollBar[0]->BarRect.x;
			m_pScrollBar[0]->PageDecRect.x = m_pScrollBar[0]->PageIncRect.x;
			m_pScrollBar[0]->BarRect.y = m_ClientRect.y;
			m_pScrollBar[0]->PageIncRect.y = m_pScrollBar[0]->BarRect.y;
			m_pScrollBar[0]->BarRect.cx = m_pScrollBar[0]->BarSize;
			m_pScrollBar[0]->BarRect.cy = m_ClientRect.cy;
			m_pScrollBar[0]->PageDecRect.y = m_pScrollBar[0]->BarRect.y
                                         + m_pScrollBar[0]->BarRect.cy
                                         - m_pScrollBar[0]->PageDecRect.cy;

			m_pScrollBar[0]->TrackRect.x = m_ClientRect.cx + m_ClientRect.x;
			m_pScrollBar[0]->TrackRect.y = m_ClientRect.y + m_pScrollBar[0]->BarSize;
			m_pScrollBar[0]->TrackRect.cx = m_pScrollBar[0]->BarSize;
			m_pScrollBar[0]->TrackRect.cy = m_ClientRect.cy - 2 * m_pScrollBar[0]->BarSize;

			m_pScrollBar[0]->SlideRect.x = m_pScrollBar[0]->TrackRect.x + 1;
			m_pScrollBar[0]->SlideRect.cx = m_pScrollBar[0]->TrackRect.cx - 2;
			m_pScrollBar[0]->SlideRect.y = m_pScrollBar[0]->TrackRect.y;
			m_pScrollBar[0]->SlideRect.cy = m_pScrollBar[0]->TrackRect.cy;

			if (m_pScrollBar[0]->MaxPos == m_pScrollBar[0]->MinPos)
			{
				m_pScrollBar[0]->SlideRect.cx = 0;
				m_pScrollBar[0]->SlideRect.cy = 0;
			} else
			{
				m_pScrollBar[0]->SlideRect.cy = m_pScrollBar[0]->SlideRect.cy * m_pScrollBar[0]->AdjustPage
						/ (m_pScrollBar[0]->MaxPos - m_pScrollBar[0]->MinPos);

				MIN_LIMIT(m_pScrollBar[0]->SlideRect.cy, 8);
				MAX_LIMIT(m_pScrollBar[0]->SlideRect.cy, m_pScrollBar[0]->TrackRect.cy);

				if (m_pScrollBar[0]->CurPos == m_pScrollBar[0]->MaxPos - m_pScrollBar[0]->AdjustPage)
					m_pScrollBar[0]->SlideRect.y += m_pScrollBar[0]->TrackRect.cy - m_pScrollBar[0]->SlideRect.cy;
				else
					m_pScrollBar[0]->SlideRect.y += m_pScrollBar[0]->CurPos * m_pScrollBar[0]->TrackRect.cy
					/ (m_pScrollBar[0]->MaxPos - m_pScrollBar[0]->MinPos);

				if (m_pScrollBar[0]->SlideRect.y + m_pScrollBar[0]->SlideRect.cy > m_pScrollBar[0]->TrackRect.y + m_pScrollBar[0]->TrackRect.cy)
					m_pScrollBar[0]->SlideRect.y = m_pScrollBar[0]->TrackRect.y + m_pScrollBar[0]->TrackRect.cy - m_pScrollBar[0]->SlideRect.cy;
			}
		}

	}
	void CWispWnd::CalcHorzScrollSlideWithPos()
	{
		if (m_pScrollBar[1])
		{
			m_pScrollBar[1]->PageIncRect.cy = m_pScrollBar[1]->BarSize;
			m_pScrollBar[1]->PageIncRect.cx = m_pScrollBar[1]->PageIncRect.cy;
			m_pScrollBar[1]->PageDecRect.cy = m_pScrollBar[1]->PageIncRect.cx;
			m_pScrollBar[1]->PageDecRect.cx = m_pScrollBar[1]->PageDecRect.cy;
			m_pScrollBar[1]->BarRect.x = m_ClientRect.x;
			m_pScrollBar[1]->PageIncRect.x = m_pScrollBar[1]->BarRect.x;
			m_pScrollBar[1]->PageDecRect.x = m_pScrollBar[1]->BarRect.x
                                         + m_pScrollBar[1]->BarRect.cx
                                         - m_pScrollBar[1]->PageDecRect.cx;
			m_pScrollBar[1]->BarRect.y = m_ClientRect.cy + m_ClientRect.y;
			m_pScrollBar[1]->PageIncRect.y = m_pScrollBar[1]->BarRect.y;
			m_pScrollBar[1]->PageDecRect.y = m_pScrollBar[1]->PageIncRect.y;
			m_pScrollBar[1]->BarRect.cx = m_ClientRect.cx;
			m_pScrollBar[1]->BarRect.cy = m_pScrollBar[1]->BarSize;

			m_pScrollBar[1]->TrackRect.x = m_ClientRect.x + m_pScrollBar[1]->BarSize;
			m_pScrollBar[1]->TrackRect.y = m_ClientRect.cy + m_ClientRect.y;
			m_pScrollBar[1]->TrackRect.cx = m_ClientRect.cx - 2 * m_pScrollBar[1]->BarSize;
			m_pScrollBar[1]->TrackRect.cy = m_pScrollBar[1]->BarSize;

			m_pScrollBar[1]->SlideRect.x = m_pScrollBar[1]->TrackRect.x;
			m_pScrollBar[1]->SlideRect.cx = m_pScrollBar[1]->TrackRect.cx;
			m_pScrollBar[1]->SlideRect.y = m_pScrollBar[1]->TrackRect.y + 1;
			m_pScrollBar[1]->SlideRect.cy = m_pScrollBar[1]->TrackRect.cy - 2;

			if (m_pScrollBar[1]->MaxPos == m_pScrollBar[1]->MinPos)
			{
				m_pScrollBar[1]->SlideRect.cx = 0;
				m_pScrollBar[1]->SlideRect.cy = 0;
			} else
			{
				m_pScrollBar[1]->SlideRect.cx = m_pScrollBar[1]->SlideRect.cx * m_pScrollBar[1]->AdjustPage
					/ (m_pScrollBar[1]->MaxPos - m_pScrollBar[1]->MinPos);

				MIN_LIMIT(m_pScrollBar[1]->SlideRect.cx, 8);
				MAX_LIMIT(m_pScrollBar[1]->SlideRect.cx, m_pScrollBar[1]->TrackRect.cx);

				if (m_pScrollBar[1]->CurPos == m_pScrollBar[1]->MaxPos - m_pScrollBar[1]->AdjustPage)
					m_pScrollBar[1]->SlideRect.x += m_pScrollBar[1]->TrackRect.cx - m_pScrollBar[1]->SlideRect.cx;
				else
					m_pScrollBar[1]->SlideRect.x += m_pScrollBar[1]->CurPos * m_pScrollBar[1]->TrackRect.cx
                                            / (m_pScrollBar[1]->MaxPos - m_pScrollBar[1]->MinPos);

				if (m_pScrollBar[1]->SlideRect.x + m_pScrollBar[1]->SlideRect.cx > m_pScrollBar[1]->TrackRect.x + m_pScrollBar[1]->TrackRect.cx)
					m_pScrollBar[1]->SlideRect.x = m_pScrollBar[1]->TrackRect.x + m_pScrollBar[1]->TrackRect.cx - m_pScrollBar[1]->SlideRect.cx;
			}
		}
	}
	void CWispWnd::ProcessScrollBTSpinKey(int Type, WISP_MSG *pMsg)
	{

		if (pMsg->MouseEvent.HitArea.State == Scroll_NUM[Type].PageInc)
		{
			if (!m_pScrollBar[Type]->BTSpinDecState)
				m_pScrollBar[Type]->BTSpinDecState = 1;
		} else
		{
			if (m_pScrollBar[Type]->BTSpinDecState)
			{
				m_pScrollBar[Type]->BTSpinDecState = 0;
				if (m_pWispBase->m_RepKeyIndex == 1)	//VK_LBUTTON
					m_pWispBase->StopMouseKeyRep();
			}
		}
		Update(&m_pScrollBar[Type]->BarRect);

		if (pMsg->MouseEvent.HitArea.State == Scroll_NUM[Type].PageDec)
		{
			if (!m_pScrollBar[Type]->BTSpinIncState)
				m_pScrollBar[Type]->BTSpinIncState = 1;

		} else
		{
			if (m_pScrollBar[Type]->BTSpinIncState)
			{
				m_pScrollBar[Type]->BTSpinIncState = 0;
				if (m_pWispBase->m_RepKeyIndex == 1)	//VK_LBUTTON
					m_pWispBase->StopMouseKeyRep();
			}
		}
		Update(&m_pScrollBar[Type]->BarRect);

		if (!pMsg->bMsgLBTDown)
		{
			if (m_pWispBase->m_RepKeyIndex == 1)	//VK_LBUTTON
				m_pWispBase->StopMouseKeyRep();
		}
	}
	void CWispWnd::ProcessHorzScrollMouseMove(WISP_MSG *pMsg)
	{
		int v3;
		int v6;
		int v7;

		//??? check field_10

		v6 = pMsg->MouseEvent.DeltaX;
		v3 = pMsg->MsgMouseWndPT.x;
		if ( (v3 >= m_pScrollBar[1]->SlideRect.x + m_pScrollBar[1]->HotPos || v6 <= 0) &&
		     (v3 <= m_pScrollBar[1]->SlideRect.x + m_pScrollBar[1]->HotPos || v6 >= 0) )
		{
			WISP_MSG Msg;
			Msg.hWnd = this;
			Msg.Msg = WISP_WM_SCROLL_EVENT;
			Msg.ScrollEvent.Event = WISP_SB_POS_CHANGE;
			Msg.ScrollEvent.Type = 1;	//HORZ
			m_pScrollBar[1]->SlideRect.x += v6;

			if (m_pScrollBar[1]->SlideRect.x2() < m_pScrollBar[1]->TrackRect.x2())
			{
				if (m_pScrollBar[1]->SlideRect.x >= m_pScrollBar[1]->TrackRect.x)
				{
					v7 = (m_pScrollBar[1]->MaxPos - m_pScrollBar[1]->MinPos)
						* (m_pScrollBar[1]->SlideRect.x - m_pScrollBar[1]->TrackRect.x)
						/ m_pScrollBar[1]->TrackRect.cx;
				} else
				{
					m_pScrollBar[1]->SlideRect.x = m_pScrollBar[1]->TrackRect.x;
					v7 = m_pScrollBar[1]->MinPos;
				}
			} else
			{
				m_pScrollBar[1]->SlideRect.x = m_pScrollBar[1]->TrackRect.x + m_pScrollBar[1]->TrackRect.cx - m_pScrollBar[1]->SlideRect.cx;
				v7 = m_pScrollBar[1]->MaxPos - m_pScrollBar[1]->AdjustPage;
			}

			if (m_pScrollBar[1]->Style & 2)
			{
				m_pScrollBar[1]->CurPos = v7;
				Update(&m_pScrollBar[1]->BarRect);
			} else
			{
				Msg.ScrollEvent.Delta = v7 - m_pScrollBar[1]->CurPos;
				m_pScrollBar[1]->CurPos = v7;
				Msg.ScrollEvent.CurPos = v7;
				m_pWispBase->SendMessage(&Msg);
				Update((WISP_RECT*)0);
			}
		}
	}
	void CWispWnd::ProcessVertScrollMouseMove(WISP_MSG *pMsg)
	{
		int v2;
		int v5;
		int v7;

		v2 = pMsg->MouseEvent.DeltaY;
		if (!v2)
			return;
		
		v5 = pMsg->MsgMouseWndPT.y;
		if ( (v5 >= m_pScrollBar[0]->SlideRect.y + m_pScrollBar[0]->HotPos || v2 <= 0) &&
		     (v5 <= m_pScrollBar[0]->SlideRect.y + m_pScrollBar[0]->HotPos || v2 >= 0) )
		{
			WISP_MSG Msg;
			Msg.hWnd = this;
			Msg.Msg = WISP_WM_SCROLL_EVENT;
			Msg.ScrollEvent.Event = WISP_SB_POS_CHANGE;
			Msg.ScrollEvent.Type = 0;	//VERT
			m_pScrollBar[0]->SlideRect.y += v2;

			if (m_pScrollBar[0]->SlideRect.y2() < m_pScrollBar[0]->TrackRect.y2())
			{
				if (m_pScrollBar[0]->SlideRect.y >= m_pScrollBar[0]->TrackRect.y)
				{
					v7 = (m_pScrollBar[0]->MaxPos - m_pScrollBar[0]->MinPos)
						* (m_pScrollBar[0]->SlideRect.y - m_pScrollBar[0]->TrackRect.y)
						/ m_pScrollBar[0]->TrackRect.cy;
				} else
				{
					m_pScrollBar[0]->SlideRect.y = m_pScrollBar[0]->TrackRect.y;
					v7 = m_pScrollBar[0]->MinPos;
				}
			} else
			{
				m_pScrollBar[0]->SlideRect.y = m_pScrollBar[0]->TrackRect.y + m_pScrollBar[0]->TrackRect.cy - m_pScrollBar[0]->SlideRect.cy;
				v7 = m_pScrollBar[0]->MaxPos - m_pScrollBar[0]->AdjustPage;
			}

			if (m_pScrollBar[0]->Style & 2)
			{
				m_pScrollBar[0]->CurPos = v7;
				Update(&m_pScrollBar[0]->BarRect);
			} else
			{
				Msg.ScrollEvent.Delta = v7 - m_pScrollBar[0]->CurPos;
				m_pScrollBar[0]->CurPos = v7;
				Msg.ScrollEvent.CurPos = v7;
				m_pWispBase->SendMessage(&Msg);
				Update((WISP_RECT*)0);
			}
		}
		
	}
	void CWispWnd::ProcessCaptionBTMouseMove(WISP_POINT & pPT)
	{
		WISP_POINT pt = pPT;

		PointToRect(&pt, &m_ScrCaptionRect);

		if (m_Style & 8)
		{
			if (PtInRect(&m_CloseBTRect, &pt))
			{
				if (!m_CloseBTState)
				{
					m_CloseBTState = 1;
					Update(&m_CaptionRect);
				}
			} else
			{
				if (m_CloseBTState)
				{
					m_CloseBTState = 0;
					Update(&m_CaptionRect);
				}
			}
		}

		if (m_Style & 0x10)
		{
			if (PtInRect(&m_MaxBTRect, &pt))
			{
				if (!m_MaxBTState)
				{
					m_MaxBTState = 1;
					Update(&m_CaptionRect);
				}
			} else
			{
				if (m_MaxBTState)
				{
					m_MaxBTState = 0;
					Update(&m_CaptionRect);
				}
			}
		}

		if (m_Style & 0x20)
		{
			if (PtInRect(&m_MinBTRect, &pt))
			{
				if (!m_MinBTState)
				{
					m_MinBTState = 1;
					Update(&m_CaptionRect);
				}
			} else
			{
				if (m_MinBTState)
				{
					m_MinBTState = 0;
					Update(&m_CaptionRect);
				}
			}
		}
	}
	bool CWispWnd::OnMouseMove(WISP_MSG *pMsg)
	{
		if (m_Style & 1)
			ProcessCaptionBTMouseMove(pMsg->MsgMouseScrPT);

		if (m_pScrollBar[0] && !m_pScrollBar[0]->bHide)
			ProcessScrollBTSpinKey(0, pMsg);

		if (m_pScrollBar[1] && !m_pScrollBar[1]->bHide)
			ProcessScrollBTSpinKey(1, pMsg);

		return true;
	}
	bool CWispWnd::OnMouseLeave(WISP_MSG *pMsg)
	{
		bool ret = true;

		if (m_Style & 1)
			ProcessCaptionBTMouseMove(pMsg->MsgMouseScrPT);

		if (m_pScrollBar[0] && !m_pScrollBar[0]->bHide)
		{
			ProcessScrollBTSpinKey(0, pMsg);
			if (m_pScrollBar[0]->State == 2)
				ret = 0;
		}
		if (m_pScrollBar[1] && !m_pScrollBar[1]->bHide)
		{
			ProcessScrollBTSpinKey(1, pMsg);
			if (m_pScrollBar[1]->State == 2)
				ret = 0;
		}
		return ret;
	}
	void CWispWnd::ProcessScrollMouseKey(int Type, WISP_MSG *Msg)
	{
		if (Msg->KeyEvent.bKeyDown)
		{
			WISP_MSG Msg1;

			if (Msg->KeyEvent.HitArea.State == Scroll_NUM[Type].PageInc)
			{
				Msg1.hWnd = this;
				Msg1.Msg = WISP_WM_COMMAND;
				Msg1.Command.CtrlType = 1;
				Msg1.Command.CmdMsg = 0x80000001; //WISP_ID_KEYDOWN
				Msg1.Command.CmdID = Scroll_WM[Type].BTDec;
				Msg1.Command.Param1 = Type;
				m_pWispBase->SendMessage(&Msg1);
			}

			if (Msg->KeyEvent.HitArea.State == Scroll_NUM[Type].PageDec)
			{
				Msg1.hWnd = this;
				Msg1.Msg = WISP_WM_COMMAND;
				Msg1.Command.CtrlType = 1;
				Msg1.Command.CmdMsg = 0x80000001; //WISP_ID_KEYDOWN
				Msg1.Command.CmdID = Scroll_WM[Type].BTInc;
				Msg1.Command.Param1 = Type;
				m_pWispBase->SendMessage(&Msg1);
			}

			if (m_pScrollBar[Type] && Msg->KeyEvent.HitArea.State == Scroll_NUM[Type].Track)
			{
				Msg1.hWnd = this;
				Msg1.Msg = WISP_WM_COMMAND;
				Msg1.Command.CtrlType = 1;

				if (Type == 0)
				{
					if (Msg->MsgMouseWndPT.y < m_pScrollBar[0]->SlideRect.y)
					{
						Msg1.Command.CmdMsg = 0x80000001; //WISP_ID_KEYDOWN
						Msg1.Command.CmdID = Scroll_WM[Type].PageDec;
						Msg1.Command.Param1 = Type;
						m_pWispBase->SendMessage(&Msg1);
					} else
					{
						Msg1.Command.CmdMsg = 0x80000001; //WISP_ID_KEYDOWN
						Msg1.Command.CmdID = Scroll_WM[Type].PageInc;
						Msg1.Command.Param1 = Type;
						m_pWispBase->SendMessage(&Msg1);
					}
				} else
				if (Type == 1)
				{
					if (Msg->MsgMouseWndPT.x < m_pScrollBar[1]->SlideRect.x)
					{
						Msg1.Command.CmdMsg = 0x80000001; //WISP_ID_KEYDOWN
						Msg1.Command.CmdID = Scroll_WM[Type].PageDec;
						Msg1.Command.Param1 = Type;
						m_pWispBase->SendMessage(&Msg1);
					} else
					{
						Msg1.Command.CmdMsg = 0x80000001; //WISP_ID_KEYDOWN
						Msg1.Command.CmdID = Scroll_WM[Type].PageInc;
						Msg1.Command.Param1 = Type;
						m_pWispBase->SendMessage(&Msg1);
					}
				} else	//WTF ???
				{

				}
			}

			if (m_pScrollBar[Type]->State != 2 && Msg->KeyEvent.HitArea.State == Scroll_NUM[Type].Slide)
			{
				if (Type == 0)
					m_pScrollBar[0]->HotPos = Msg->MsgMouseWndPT.y - m_pScrollBar[0]->SlideRect.y;
				if (Type == 1)
					m_pScrollBar[1]->HotPos = Msg->MsgMouseWndPT.x - m_pScrollBar[1]->SlideRect.x;

				Msg1.hWnd = this;
				Msg1.Msg = WISP_WM_COMMAND;
				Msg1.Command.CtrlType = 1;
				Msg1.Command.CmdMsg = 0x80000001; //WISP_ID_KEYDOWN
				Msg1.Command.CmdID = Scroll_WM[Type].SLIDE;
				Msg1.Command.Param1 = Type;
				m_pWispBase->SendMessage(&Msg1);
			}
			return;
		}

		if (Msg->KeyEvent.HitArea.State == Scroll_NUM[Type].PageInc)
		{
			m_pScrollBar[Type]->BTSpinDecState = 1;
			Update(&m_pScrollBar[Type]->BarRect);
		}

		if (Msg->KeyEvent.HitArea.State == Scroll_NUM[Type].PageDec)
		{
			m_pScrollBar[Type]->BTSpinIncState = 1;
			Update(&m_pScrollBar[Type]->BarRect);
		}

		if (m_pScrollBar[Type]->State == 2)
		{
			WISP_MSG Msg2;
			Msg2.hWnd = this;
			Msg2.Msg = WISP_WM_COMMAND;
			Msg2.Command.CtrlType = 1;
			Msg2.Command.CmdMsg = 0x80000002; //WISP_ID_KEYUP
			Msg2.Command.CmdID = Scroll_WM[Type].SLIDE;
			Msg2.Command.Param1 = Type;
			m_pWispBase->SendMessage(&Msg2);
		}
	}
	void CWispWnd::ProcessCaptionBTMouseKeyUp(WISP_POINT & ScrMousePT)
	{
		bool bSendCmd = false;
		unsigned int CapBTID = IsPointInCaptionBT(ScrMousePT);
		if (CapBTID == WISP_ID_CLOSE)
		{
			bSendCmd = m_CloseBTState == 2;
			m_CloseBTState = 1;
		} else
		if (CapBTID == WISP_ID_MAX)
		{
			bSendCmd = m_MaxBTState == 2;
			m_MaxBTState = 1;
		} else
		if (CapBTID == WISP_ID_MIN)
		{
			bSendCmd = m_MinBTState == 2;
			m_MinBTState = 1;
		} else
			return;

		Update(&m_CaptionRect);
		if (bSendCmd)
		{
			WISP_MSG Msg;
			Msg.hWnd = this;
			Msg.Msg = WISP_WM_COMMAND;
			Msg.Command.CtrlType = 1;
			Msg.Command.CmdMsg = 0x80000002; //WISP_ID_KEYUP
			Msg.Command.CmdID = CapBTID;
			if (m_pWispBase->SendMessage(&Msg))
			{
				Msg.hWnd = this;
				Msg.Msg = WISP_WM_EVENT;
				m_pWispBase->SendMessage(&Msg);
			}
		}
	}
	bool CWispWnd::OnUpdate(WISP_MSG *pMsg)
	{
		if (!m_pUpdateRC)
			return true;

		WISP_MSG Msg;
		Msg.hWnd = this;

		//???
		if ( (m_Style & 0x100) == 0)
		{
			if (HasClipRect(m_pUpdateRC, &m_ClientRect))
			{
				Msg.Msg = WISP_WM_UPDATE_CLIENT;
				m_pWispBase->SendMessage(&Msg);
				if (m_Caret.bEnable)
				{
					Msg.Msg = WISP_WM_UPDATE_CARET;
					m_pWispBase->SendMessage(&Msg);
				}
			}

			if ((m_Style & 2) && m_BorderSize && HasClipRect(m_pUpdateRC, &m_WindowRect))
			{
				Msg.Msg = WISP_WM_UPDATE_BORDER;
				m_pWispBase->SendMessage(&Msg);
			}

			if (m_pScrollBar[1] && !m_pScrollBar[1]->bHide && HasClipRect(m_pUpdateRC, &m_pScrollBar[1]->BarRect))
			{
				Msg.Msg = WISP_WM_UPDATE_HORZ_SCROLL_BAR;
				m_pWispBase->SendMessage(&Msg);
			}

			if (m_pScrollBar[0] && !m_pScrollBar[0]->bHide && HasClipRect(m_pUpdateRC, &m_pScrollBar[0]->BarRect))
			{
				Msg.Msg = WISP_WM_UPDATE_VERT_SCROLL_BAR;
				m_pWispBase->SendMessage(&Msg);
			}

			if (m_pScrollBar[1] && m_pScrollBar[0])
			{
				WISP_RECT rc_v26;
				rc_v26.x = m_ClientRect.x2();
				rc_v26.y = m_ClientRect.y2();
				rc_v26.cx = m_WindowRect.cx - m_BorderSize - m_ClientRect.x2();
				rc_v26.cy = m_WindowRect.cy - m_BorderSize - m_ClientRect.y2();
				m_WindowDC.DrawFullRect(&rc_v26, m_pWispBase->m_pWispDrawObj->m_crSystem[28]);
			}

			if ((m_Style & 1) && HasClipRect(m_pUpdateRC, &m_CaptionRect))
			{
				Msg.Msg = WISP_WM_UPDATE_CAPTION;
				m_pWispBase->SendMessage(&Msg);
			}
		}

		for (CWispBaseWnd *pChildWnd = m_ChildWnd;
			pChildWnd;
			pChildWnd = pChildWnd->m_NextWnd)
		{
			if (pChildWnd->m_ShowMode != WISP_SH_HIDDEN)
			{
				if (pChildWnd->m_pUpdateRC &&
					HasClipRect(pChildWnd->m_pUpdateRC, &pChildWnd->m_WindowRect))
				{
					Msg.hWnd = pChildWnd;
					Msg.Msg = WISP_WM_UPDATE;
					m_pWispBase->SendMessage(&Msg);
					pChildWnd->m_pUpdateRC = 0;
				}
			}
		}
		m_pUpdateRC = 0;
		
		return true;
	}

	bool CWispWnd::OnUpdateClient(WISP_MSG *pMsg)
	{
		//???
		if ( !(m_Style & 0x40) )
		{
			if (m_pBKDIB)
				m_ClientDC.DrawDIBTiled(m_pBKDIB, 0);
			else
			if (m_bBGColor)
				m_WindowDC.DrawFullRect(&m_ClientRect, m_crBGColor);
			else
				m_pWispBase->m_pWispDrawObj->DrawClient(&m_WindowDC, &m_ClientRect);
		}
		return true;
	}
	bool CWispWnd::OnUpdateBorder(WISP_MSG *pMsg)
	{
		if (m_Style & 2)
		{
			m_pWispBase->m_pWispDrawObj->DrawBorder(&m_WindowDC, &m_WindowRect, m_BorderSize);
			return true;
		}
		return false;
	}
	bool CWispWnd::IsBlinkSlowTime(unsigned long BlinkTime)
	{
		//TODO
		return true;
	}
	bool CWispWnd::OnUpdateCaret(WISP_MSG *pMsg)
	{
		if (m_Caret.pDIB && IsBlinkSlowTime(m_Caret.BlinkTime) && m_pWispBase->m_pFocusWnd == this)
		{
			//m_ClientDC.DrawDIB(m_Caret.x, m_Caret.y, m_Caret.pDIB);
			m_ClientDC.DrawVLine(m_Caret.x + 0, m_Caret.y + 1,
					m_ClientDC.m_pFont->m_Height + m_Caret.y - 2, m_CaretColor);

			m_ClientDC.DrawVLine(m_Caret.x + 1, m_Caret.y + 1,
					m_ClientDC.m_pFont->m_Height + m_Caret.y - 2, m_CaretColor);
			return true;
		}
		return false;
	}
	bool CWispWnd::OnUpdateHorzScrollBar(WISP_MSG *pMsg)
	{
		m_pWispBase->m_pWispDrawObj->DrawHorzScrollRect(&m_WindowDC, &m_pScrollBar[1]->TrackRect);
		m_pWispBase->m_pWispDrawObj->DrawHorzScrollSlideRect(&m_WindowDC, &m_pScrollBar[1]->SlideRect);
		WISP_RECT rc;
		rc.x = m_ClientRect.x;
		rc.y = m_ClientRect.y + m_ClientRect.cy;
		rc.cx = m_pScrollBar[1]->BarSize;
		rc.cy = m_pScrollBar[1]->BarSize;
		m_pWispBase->m_pWispDrawObj->DrawSystemStandardIcon(&m_WindowDC, &rc, WISP_SSI_HORZ_DEC_SPIN, m_pScrollBar[1]->BTSpinDecState);
		rc.x = m_ClientRect.x + m_ClientRect.cx - m_pScrollBar[1]->BarSize;
		m_pWispBase->m_pWispDrawObj->DrawSystemStandardIcon(&m_WindowDC, &rc, WISP_SSI_HORZ_INC_SPIN, m_pScrollBar[1]->BTSpinIncState);
		return true;
	}
	bool CWispWnd::OnUpdateVertScrollBar(WISP_MSG *pMsg)
	{
		m_pWispBase->m_pWispDrawObj->DrawVertScrollRect(&m_WindowDC, &m_pScrollBar[0]->TrackRect);
		m_pWispBase->m_pWispDrawObj->DrawVertScrollSlideRect(&m_WindowDC, &m_pScrollBar[0]->SlideRect);
		WISP_RECT rc;
		rc.x = m_ClientRect.x + m_ClientRect.cx;
		rc.y = m_ClientRect.y;
		rc.cx = m_pScrollBar[0]->BarSize;
		rc.cy = m_pScrollBar[0]->BarSize;
		m_pWispBase->m_pWispDrawObj->DrawSystemStandardIcon(&m_WindowDC, &rc, WISP_SSI_VERT_DEC_SPIN, m_pScrollBar[0]->BTSpinDecState);
		rc.y = m_ClientRect.y + m_ClientRect.cy - m_pScrollBar[0]->BarSize;
		m_pWispBase->m_pWispDrawObj->DrawSystemStandardIcon(&m_WindowDC, &rc, WISP_SSI_VERT_INC_SPIN, m_pScrollBar[0]->BTSpinIncState);
		return true;
	}

	bool CWispWnd::CreateWnd(const WCHAR *Name, const CWispRect & pRC, CWispBaseWnd *pParentWnd, unsigned int CmdID, unsigned int Style, unsigned int ShowMode)
	{
		return CreateWnd(Name, pRC.x, pRC.y, pRC.cx, pRC.cy, pParentWnd, CmdID, Style, ShowMode);
	}

	bool CWispWnd::CreateWnd(const WCHAR *Name, int x, int y, int cx, int cy, CWispBaseWnd *pParentWnd, unsigned int CmdID, unsigned int Style, unsigned int ShowMode)
	{
		if (!CWispBaseWnd::Create(Name, x,y,cx,cy, pParentWnd, CmdID, Style | 0x8000, ShowMode))
			return false;

		if (!m_ClientDC.Create(this, &m_ScrClientRect))
			return false;

		if (!m_CaptionDC.Create(this, &m_ScrCaptionRect))
			return false;

		m_CaptionDC.SetTextColor(m_pWispBase->m_pWispDrawObj->m_crSystem[1]);

		if (!InitWnd())
			return false;

		RecalcLayout();
		m_State = WISP_WSTATE_RESET;

		WISP_MSG Msg;
		Msg.hWnd = this;
		Msg.Msg = WISP_WM_CREATING;
		if (!m_pWispBase->SendMessage(&Msg))
		{
			Destroy();
			return false;
		}

		Msg.Msg = WISP_WM_CREATE;
		if (!m_pWispBase->SendMessage(&Msg))
		{
			Destroy();
			return false;
		}
		Show(ShowMode);
		return true;
	}

	bool CWispWnd::Destroy()
	{
		if (!CWispBaseWnd::Destroy())
			return false;

		m_ClientDC.Destroy();
		m_CaptionDC.Destroy();

		SafeDelete(m_pScrollBar[0]);
		SafeDelete(m_pScrollBar[1]);

		if (m_AdvStyle & 1)	//WISP_ADV_AUTODEL
			delete this;

		return true;
	}

	bool CWispWnd::AttachBKDIB(CWispDIB *pDIB)
	{
		m_pBKDIB = pDIB;
		return true;
	}

	void CWispWnd::DetachBKDIB()
	{
		m_pBKDIB = 0;
	}

	bool CWispWnd::LoadBKDIB(const char *FileName)
	{
		m_pBKDIB = gpCurWisp->m_DIBLib.GetDIB(FileName, -1);
		return true;
	}

	bool CWispWnd::AttachTitleDIB(CWispDIB *pDIB)
	{
		m_pTitleDIB = pDIB;
		return true;
	}
	void CWispWnd::DetachTitleDIB()
	{
		m_pTitleDIB = 0;
	}
	bool CWispWnd::LoadTitleDIB(const char *FileName, int Index)
	{
		m_pTitleDIB = gpCurWisp->m_DIBLib.GetDIB(FileName, Index);
		return true;
	}
	bool CWispWnd::InitWnd()
	{
		if (!CWispBaseWnd::InitWnd())
			return false;

		m_MaxWndSize.cy = 0;
		m_MaxWndSize.cx = 0;
		m_bInResizeArea = 0;
		m_pScrollBar[1] = 0;
		m_pScrollBar[0] = 0;
		m_pBKDIB = 0;
		m_pTitleDIB = 0;
		m_Caret.y = 0;
		m_Caret.x = 0;
		m_Caret.bEnable = false;
		m_Caret.BlinkTime = 300;

		//4+2 TIP HELP
		if ( (m_Style & 6) == 6 )
			m_BorderSize = m_pWispBase->m_Metrics[WISP_SM_TIP_BORDER_SIZE];
		else
		if (m_Style & 2)
			m_BorderSize = m_pWispBase->m_Metrics[WISP_SM_BORDER_SIZE];
		else	m_BorderSize = 0;


		m_MinBTState = 0;
		m_MaxBTState = 0;
		m_CloseBTState = 0;
		m_CaretColor = m_pWispBase->m_pWispDrawObj->m_crSystem[1];

		return true;
	}
	bool CWispWnd::CreateCaret(unsigned int ID)
	{
		m_Caret.pDIB = m_pWispBase->GetDefDIB(ID);
		m_Caret.bEnable = false;
		return true;
	}
	bool CWispWnd::SetCaretPos(int PosX, int PosY)
	{
		m_Caret.x = PosX;
		m_Caret.y = PosY;
		return true;
	}
	bool CWispWnd::GetCaretPos(WISP_POINT *pPT)
	{
		pPT->x = m_Caret.x;
		pPT->y = m_Caret.y;
		return true;
	}
	void CWispWnd::ShowCaret(bool bShow)
	{
		if (m_Caret.bEnable != bShow)
			m_Caret.bEnable = bShow;
	}
	void CWispWnd::BlinkCaret(unsigned long BlinkTime)
	{
		m_Caret.BlinkTime = BlinkTime;
	}
	void CWispWnd::DestroyCaret() //???
	{
		m_Caret.pDIB = 0;
		m_Caret.bEnable = false;
	}
	void CWispWnd::UpdateScrollBarState(int Type)
	{
		if (m_pScrollBar[Type] && m_pScrollBar[Type]->Style & 1)
		{
			if (m_pScrollBar[Type]->MaxPos - m_pScrollBar[Type]->MinPos > m_pScrollBar[Type]->Page)
			{
				if (m_pScrollBar[Type]->bHide)
				{
					m_pScrollBar[Type]->bHide = 0;
					RecalcLayout();
				}
			} else
			if (!m_pScrollBar[Type]->bHide)
			{
				m_pScrollBar[Type]->bHide = 1;
				RecalcLayout();
			}
		}
	}
	bool CWispWnd::EnableScrollBar(int Type, bool bEnable, int Style)
	{
		if (bEnable)
		{
			if (m_pScrollBar[Type])
				return false;

			m_pScrollBar[Type] = new WISP_SCROLL_BAR;
			memset(m_pScrollBar[Type], 0, sizeof(WISP_SCROLL_BAR));
			m_pScrollBar[Type]->BarSize = m_pWispBase->GetMetrics(WISP_SM_SCROLL_BAR_SIZE);
			m_pScrollBar[Type]->bHide = 0;
			m_pScrollBar[Type]->State = 0;
			m_pScrollBar[Type]->BTSpinDecState = 0;
			m_pScrollBar[Type]->BTSpinIncState = 0;
			m_pScrollBar[Type]->Style = Style;
			m_pScrollBar[Type]->CurPos = 0;
			m_pScrollBar[Type]->MinPos = 0;
			m_pScrollBar[Type]->MaxPos = 0;
			m_pScrollBar[Type]->Page = 0;

			if (m_pWispBase->m_RepKeyIndex == 1)	//VK_LBUTTON
				m_pWispBase->StopMouseKeyRep();
		} else
		{
			if (!m_pScrollBar[Type])
				return false;
			SafeDelete(m_pScrollBar[Type]);
		}
		RecalcLayout();
		return true;
	}
	void CWispWnd::LimitScrollInfo(int Type, bool bSendEvent)
	{
		int v4 = m_pScrollBar[Type]->CurPos;

		MIN_LIMIT(m_pScrollBar[Type]->MaxPos, m_pScrollBar[Type]->MinPos);

		m_pScrollBar[Type]->AdjustPage = MIN(m_pScrollBar[Type]->MaxPos - m_pScrollBar[Type]->MinPos, m_pScrollBar[Type]->Page);

		MIN_LIMIT(m_pScrollBar[Type]->AdjustPage, 0);
		MAX_LIMIT(m_pScrollBar[Type]->CurPos, m_pScrollBar[Type]->MaxPos);
		MIN_LIMIT(m_pScrollBar[Type]->CurPos, m_pScrollBar[Type]->MinPos);
		MAX_LIMIT(m_pScrollBar[Type]->CurPos, m_pScrollBar[Type]->MaxPos - m_pScrollBar[Type]->AdjustPage);

		if (bSendEvent)
		{
			if (v4 != m_pScrollBar[Type]->CurPos)
			{
				WISP_MSG Msg;
				Msg.hWnd = this;
				Msg.Msg = WISP_WM_SCROLL_EVENT;
				Msg.ScrollEvent.Event = WISP_SB_POS_CHANGE;
				Msg.ScrollEvent.Type = Type;
				Msg.ScrollEvent.CurPos = m_pScrollBar[Type]->CurPos;
				Msg.ScrollEvent.Delta = m_pScrollBar[Type]->CurPos - v4;
				m_pWispBase->SendMessage(&Msg);
			}
		}
	}
	bool CWispWnd::BeginDrag()
	{
		if (m_pWispBase->m_pDragWnd == this)
			return false;
		m_pWispBase->m_pDragWnd = this;
		m_pWispBase->m_BeginPT = m_pWispBase->m_MousePT;
		PointToRect(&m_pWispBase->m_BeginPT, &m_ScrWindowRect);
		WISP_MSG Msg;
		Msg.hWnd = this;
		Msg.Msg = WISP_WM_DRAG_BEGIN;
		Msg.DragEvent.BeginMouseWndPT = m_pWispBase->m_BeginPT;
		m_pWispBase->SendMessage(&Msg);
		m_pWispBase->BeginDrag();
		return true;
	}
	void CWispWnd::EndDrag()
	{
		if (m_pWispBase->m_pDragWnd)
		{
			WISP_MSG Msg;
			Msg.hWnd = this;
			Msg.Msg = WISP_WM_DRAG_END;
			Msg.DragEvent.BeginMouseWndPT = m_pWispBase->m_BeginPT;
			PointToRect(&Msg.DragEvent.BeginMouseWndPT, &m_WindowRect);
			m_pWispBase->SendMessage(&Msg);
			m_pWispBase->EndDrag();
			m_pWispBase->m_pDragWnd = 0;
		}
	}
	void CWispWnd::SendScrollEvent(int Type, int Event)
	{
		if (m_pScrollBar[Type])
		{
			WISP_MSG Msg;
			Msg.hWnd = this;
			Msg.Msg = WISP_WM_SCROLL_EVENT;
			Msg.ScrollEvent.Event = Event;
			Msg.ScrollEvent.Type = Type;
			Msg.ScrollEvent.CurPos = m_pScrollBar[Type]->CurPos;
			Msg.ScrollEvent.Delta = 0;
			m_pWispBase->SendMessage(&Msg);
			Update((WISP_RECT*)0);
		}
	}
	bool CWispWnd::OnCreating(WISP_MSG *pMsg)
	{
		if (m_pWispBase->m_pRootWnd != this)
			return true;

		m_pWispBase->m_pTopMostRootWnd = new CWispWnd;
		m_pWispBase->m_pTopMostRootWnd->CreateWnd(0, m_WindowRect, this, WISP_ID_TOP_MOST_ROOT, 0x100, WISP_SH_MINSIZE);
		m_pWispBase->m_pTopMostRootWnd->m_AdvStyle |= 1; //WISP_ADV_AUTODEL

		m_pWispBase->m_pTipWnd = new CWispTipWnd;
		m_pWispBase->m_pTipWnd->CreateWnd(0, 0,0,0,0, 0, WISP_ID_HELP_TIP, 6, WISP_SH_HIDDEN);
		m_pWispBase->m_pTipWnd->m_AdvStyle |= 1; //WISP_ADV_AUTODEL
		return true;
	}
	bool CWispWnd::OnTimer(WISP_MSG *pMsg)
	{
		if (pMsg->Timer.ID == WISP_ID_CARET)
		{
			WISP_RECT Rect;
			Rect.x = m_Caret.x;
			Rect.y = m_Caret.y;
			WISP_RECT *OrgRc = m_pUpdateRC;
			Rect.cx = m_Caret.pDIB->m_PaintRect.cx;
			Rect.cy = m_Caret.pDIB->m_PaintRect.cy;
			m_pUpdateRC = &Rect;
			OnUpdateCaret(pMsg);
			Rect.y += m_ScrWindowRect.y;
			Rect.x += m_ScrWindowRect.x;
			m_pWispBase->EndDraw(&Rect);
			m_pUpdateRC = OrgRc;
		}
		return true;
	}
	bool CWispWnd::OnMouseWheel(WISP_MSG *pMsg)
	{
		if (pMsg->MouseEvent.Wheel >= 0)
			SendScrollEvent(0, WISP_SB_DEC);	//VertScrollLineUp()
		else	SendScrollEvent(0, WISP_SB_INC);	//VertScrollLineDown()

		return true;
	}
	bool CWispWnd::OnCmdPageDec(WISP_MSG *pMsg)
	{
		if (m_pScrollBar[pMsg->Command.Param1] && pMsg->Command.CmdMsg == 0x80000001) //WISP_ID_KEYDOWN
			SendScrollEvent(pMsg->Command.Param1, WISP_SB_PAGE_UP);	//PAGE UP
		return true;
	}
	bool CWispWnd::OnCmdPageInc(WISP_MSG *pMsg)
	{
		if (m_pScrollBar[pMsg->Command.Param1] && pMsg->Command.CmdMsg == 0x80000001) //WISP_ID_KEYDOWN
			SendScrollEvent(pMsg->Command.Param1, WISP_SB_PAGE_DOWN); //PAGE DOWN
		return true;
	}
	bool CWispWnd::OnCmdSpinInc(WISP_MSG *pMsg)
	{
		int type = pMsg->Command.Param1;

		if (!m_pScrollBar[type])
			return true;
		if (pMsg->Command.CmdMsg != 0x80000001) //WISP_ID_KEYDOWN
		{
			m_pScrollBar[type]->BTSpinIncState = 0;
			return true;
		}
		if (m_pScrollBar[type]->BTSpinIncState != 2)
		{
			m_pScrollBar[type]->BTSpinIncState = 2;
			m_pWispBase->StartMouseKeyRep(VK_LBUTTON);
		}
		SendScrollEvent(type, WISP_SB_INC);
		return true;
	}
	bool CWispWnd::OnCmdSpinDec(WISP_MSG *pMsg)
	{
		int type = pMsg->Command.Param1;

		if (!m_pScrollBar[type])
			return true;
		if (pMsg->Command.CmdMsg != 0x80000001) //WISP_ID_KEYDOWN
		{
			m_pScrollBar[type]->BTSpinDecState = 0;
			return true;
		}
		if (m_pScrollBar[type]->BTSpinDecState != 2)
		{
			m_pScrollBar[type]->BTSpinDecState = 2;
			m_pWispBase->StartMouseKeyRep(VK_LBUTTON);
		}
		SendScrollEvent(type, WISP_SB_DEC);
		return true;
	}
	bool CWispWnd::OnMove(WISP_MSG *pMsg)
	{
		if (m_Style & 1)
			ProcessCaptionBTMouseMove(pMsg->MsgMouseScrPT);
		return true;
	}
	bool CWispWnd::OnSize(WISP_MSG *pMsg)
	{
		if (m_pWispBase->m_pRootWnd == this)
		{
			//WISP_WSTATE_RESIZE
			if (m_State != 2 && !m_pWispBase->EndResize(m_ClientRect.cx, m_ClientRect.cy))
				return false;

			if (!m_pWispBase->m_pTopMostRootWnd->Resize(m_ClientRect.cx, m_ClientRect.cy, true))
				return false;
		}

		if (m_Style & 1)
			ProcessCaptionBTMouseMove(pMsg->MsgMouseScrPT);
		return true;

	}
	bool CWispWnd::OnHorzScrollMove(WISP_MSG *pMsg)
	{
		if (m_pScrollBar[1])
		{
			if (!m_pScrollBar[1]->bHide)
				ProcessScrollBTSpinKey(1, pMsg);
			ProcessHorzScrollMouseMove(pMsg);
		}
		return false;
	}
	bool CWispWnd::OnVertScrollMove(WISP_MSG *pMsg)
	{
		if (m_pScrollBar[0])
		{
			ProcessVertScrollMouseMove(pMsg);
		}
		return false;
	}

	void CWispWnd::CalcScrollSlideWithPos(int Type)
	{
		if (Type == 0)
			CalcVertScrollSlideWithPos();
		if (Type == 1)
			CalcHorzScrollSlideWithPos();
	}

	bool CWispWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
		switch (pMsg->KeyEvent.KeyType)
		{
		case 0x40000025:	//WISP_SK_CTRL+VK_LEFT
				if (pMsg->KeyEvent.bKeyDown && m_pScrollBar[1] && !m_pScrollBar[1]->bHide)
				{
					SendScrollEvent(1, WISP_SB_DEC);	//HorzScrollLineUp()
					return false;
				}
				break;
		case 0x40000026:	//WISP_SK_CTRL+VK_UP
				if (pMsg->KeyEvent.bKeyDown && m_pScrollBar[0] && !m_pScrollBar[0]->bHide)
				{
					SendScrollEvent(0, WISP_SB_DEC);	//VertScrollLineUp()
					return false;
				}
				break;
		case 0x40000027:	//WISP_SK_CTRL+VK_RIGHT
				if (pMsg->KeyEvent.bKeyDown && m_pScrollBar[1] && !m_pScrollBar[1]->bHide)
				{
					SendScrollEvent(1, WISP_SB_INC);	//HorzScrollLineDown()
					return false;
				}
				break;
		case 0x40000028:   	//WISP_SK_CTRL+VK_DOWN
				if (pMsg->KeyEvent.bKeyDown && m_pScrollBar[0] && !m_pScrollBar[0]->bHide)
				{
					SendScrollEvent(0, WISP_SB_INC);	//VertScrollLineDown()
					return false;
				}
				break;

		case VK_LBUTTON:
			if (m_pScrollBar[0] && !m_pScrollBar[0]->bHide)
				ProcessScrollMouseKey(0, pMsg);
			if (m_pScrollBar[1] && !m_pScrollBar[1]->bHide)
				ProcessScrollMouseKey(1, pMsg);

			if (!pMsg->KeyEvent.bKeyDown)
			{
				ProcessCaptionBTMouseKeyUp(pMsg->MsgMouseScrPT);

				if (m_pScrollBar[0] && m_pScrollBar[0]->State == 2)
					m_pScrollBar[0]->State = 0;

				if (m_pScrollBar[1] && m_pScrollBar[1]->State == 2)
					m_pScrollBar[1]->State = 0;
			}
			break;
		case VK_PRIOR:
			if (pMsg->KeyEvent.bKeyDown)
				SendScrollEvent(0, WISP_SB_PAGE_UP);
			break;
		case VK_NEXT:
			if (pMsg->KeyEvent.bKeyDown)
				SendScrollEvent(0, WISP_SB_PAGE_DOWN);
			break;
		default:
			break;
		}
		return true;
	}
	bool CWispWnd::OnUpdateCaption(WISP_MSG *pMsg)
	{
		m_pWispBase->m_pWispDrawObj->DrawCaption(&m_WindowDC, &m_CaptionRect);
		int x = 2;
		if (m_pTitleDIB)
		{
			m_CaptionDC.DrawDIB(2, (m_CaptionRect.cy - m_pTitleDIB->m_PixelBuf.Height) / 2, m_pTitleDIB);
			x = m_pTitleDIB->m_PixelBuf.Width + 2;
		}

		if (m_WndText.m_nLength)
		{
			WISP_RECT Rect;
			Rect.x = x + m_CaptionDC.m_pFont->m_Width;
			Rect.y = (m_CaptionRect.cy - m_CaptionDC.m_pFont->m_Height) / 2;
			Rect.cx = m_CaptionRect.cx - 60;
			Rect.cy = 14;
			m_CaptionDC.DrawString(m_WndText.operator const WCHAR*(), &Rect, 0x8020, 0, -1);
		}

		if (m_Style & 8)
			m_pWispBase->m_pWispDrawObj->DrawSystemStandardIcon(&m_CaptionDC, &m_CloseBTRect, 4, m_CloseBTState);

		if (m_Style & 0x10)
			m_pWispBase->m_pWispDrawObj->DrawSystemStandardIcon(&m_CaptionDC, &m_MaxBTRect,
				((m_State & 4) | 0x18) >> 2, (m_State & 8) ? 3:m_MaxBTState); //WISP_WSTATE_MAXIMIZE WISP_WSTATE_MINIMIZE

		if (m_Style & 0x20)
			m_pWispBase->m_pWispDrawObj->DrawSystemStandardIcon(&m_CaptionDC, &m_MinBTRect,
				((m_State & 8) | 0x14) >> 2, (m_State & 4) ? 3:m_MinBTState);

		return true;
	}
	void CWispWnd::RecalcMinWndSize()
	{
		m_MinWndSize.cy = 0;
		m_MinWndSize.cx = 0;

		//CAPTION TEXT

		if (m_Style & 1)
		{
			m_MinWndSize.cy = m_pWispBase->m_Metrics[WISP_SM_CAPTION_CY];
			if (m_pTitleDIB)
				m_MinWndSize.cx = m_pTitleDIB->m_PaintRect.cx + m_CaptionDC.m_pFont->m_Width;

			if (m_WndText.m_nLength)
			{
				int len = m_CaptionDC.GetTextExtent(m_WndText.operator const WCHAR*(), -1, 0) + 20;
				MAX_LIMIT(len, 120);
				m_MinWndSize.cx += len + m_CaptionDC.m_pFont->m_Width;
			}
		}

		//BORDER

		if (m_Style & 2)
		{
			m_MinWndSize.cy += 2 * m_pWispBase->m_Metrics[WISP_SM_BORDER_SIZE];
			m_MinWndSize.cx += 2 * m_pWispBase->m_Metrics[WISP_SM_BORDER_SIZE];
		}

		//CAPTION BT

		if (m_Style & 8)	
			m_MinWndSize.cx += m_pWispBase->m_Metrics[WISP_SM_CAPTION_BT_SIZE];
		if (m_Style & 0x10)
			m_MinWndSize.cx += m_pWispBase->m_Metrics[WISP_SM_CAPTION_BT_SIZE];
		if (m_Style & 0x20)
			m_MinWndSize.cx += m_pWispBase->m_Metrics[WISP_SM_CAPTION_BT_SIZE];
	}
	void CWispWnd::RecalcLayout()
	{
		CWispBaseWnd::RecalcLayout();
		RecalcMinWndSize();

		m_ClientRect.y = 0;
		m_ClientRect.x = 0;
		m_ClientRect.cx = m_ScrWindowRect.cx;
		m_ClientRect.cy = m_ScrWindowRect.cy;

		//BORDER
		if (m_Style & 2)
		{
			m_ClientRect.x = m_BorderSize;
			m_ClientRect.y = m_BorderSize;
			m_ClientRect.cx = m_ScrWindowRect.cx - 2 * m_BorderSize;
			m_ClientRect.cy = m_ScrWindowRect.cy - 2 * m_BorderSize;
		}

		m_CaptionRect.x = m_ClientRect.x;
		m_CaptionRect.y = m_ClientRect.y;
		m_CaptionRect.cx = m_ClientRect.cx;
		m_CaptionRect.cy = 0;

		//CAPTION
		if (m_Style & 1)
		{
			m_ClientRect.y += m_pWispBase->m_Metrics[WISP_SM_CAPTION_CY];
			m_ClientRect.cy -= m_pWispBase->m_Metrics[WISP_SM_CAPTION_CY];

			m_CaptionRect.cy = m_pWispBase->m_Metrics[WISP_SM_CAPTION_CY];

			if (m_Style & 8) //BT_CLOSE
			{
				m_CloseBTRect.x = m_CaptionRect.cx - 17;
				m_CloseBTRect.y = (m_CaptionRect.cy - 16) / 2;
				m_CloseBTRect.cx = 16;
				m_CloseBTRect.cy = 16;
			}

			if (m_Style & 0x10) //BT_MAX
			{
				m_MaxBTRect.x = m_CaptionRect.cx - 17*2;
				m_MaxBTRect.y = (m_CaptionRect.cy - 16) / 2;
				m_MaxBTRect.cx = 16;
				m_MaxBTRect.cy = 16;
			}

			if (m_Style & 0x20) //BT_MIN
			{
				m_MinBTRect.x = m_CaptionRect.cx - 17*3;
				m_MinBTRect.y = (m_CaptionRect.cy - 16) / 2;
				m_MinBTRect.cx = 16;
				m_MinBTRect.cy = 16;
			}
		}

		if (m_pScrollBar[0] && !m_pScrollBar[0]->bHide)
			m_ClientRect.cx -= m_pScrollBar[0]->BarSize;

		if (m_pScrollBar[1] && !m_pScrollBar[1]->bHide)
			m_ClientRect.cy -= m_pScrollBar[1]->BarSize;

		if (m_pScrollBar[0])
			UpdateScrollBarState(0);
		if (m_pScrollBar[1])
			UpdateScrollBarState(1);

		CalcVertScrollSlideWithPos();
		CalcHorzScrollSlideWithPos();

		m_ScrClientRect.x = m_ClientRect.x;
		m_ScrClientRect.y = m_ClientRect.y;
		m_ScrClientRect.cx = m_ClientRect.cx;

		m_ScrCaptionRect.x = m_CaptionRect.x;
		m_ScrCaptionRect.y = m_CaptionRect.y;
		m_ScrCaptionRect.cx = m_CaptionRect.cx;

		m_ScrClientRect.cy = m_ClientRect.cy;
		m_ScrClientRect.x += m_ScrWindowRect.x;
		m_ScrClientRect.y += m_ScrWindowRect.y;

		m_ScrCaptionRect.cy = m_CaptionRect.cy;
		m_ScrCaptionRect.x += m_ScrWindowRect.x;
		m_ScrCaptionRect.y += m_ScrWindowRect.y;

		if (m_State != WISP_WSTATE_INIT)
		{
			WISP_MSG Msg;
			Msg.hWnd = this;
			Msg.Msg = WISP_WM_RECALC_LAYOUT;
			m_pWispBase->SendMessage(&Msg);
		}
	}
	bool CWispWnd::SetWindowText(const WCHAR *Text)
	{
		if (m_Style & 0x20)
		{
			if (!CWispBaseWnd::SetWindowText(Text))
				return false;

			RecalcMinWndSize();
			return true;
		}
		return CWispBaseWnd::SetWindowText(Text);
	}
	void CWispWnd::SetScrollBarCurPos(int Type, int CurPos, bool bSendEvent)
	{
		if (m_pScrollBar[Type])
		{
			WISP_MSG Msg;
			Msg.hWnd = this;
			Msg.Msg = WISP_WM_SCROLL_EVENT;
			Msg.ScrollEvent.Event = WISP_SB_POS_CHANGE;
			Msg.ScrollEvent.Type = Type;
			int v6 = m_pScrollBar[Type]->CurPos;
			m_pScrollBar[Type]->CurPos = CurPos;
			LimitScrollInfo(Type, false);
			int Delta = m_pScrollBar[Type]->CurPos - v6;
			Msg.ScrollEvent.Delta = Delta;
			if (bSendEvent && Delta)
			{
				Msg.ScrollEvent.CurPos = m_pScrollBar[Type]->CurPos;
				m_pWispBase->SendMessage(&Msg);
			}
			CalcScrollSlideWithPos(Type);
			UpdateScrollBarState(Type);
			Update(&m_pScrollBar[Type]->BarRect);
		}

	}
	void CWispWnd::SetScrollBarRange(int Type, int MaxPos, int MinPos)
	{
		if (m_pScrollBar[Type])
		{
			m_pScrollBar[Type]->MaxPos = MaxPos;
			m_pScrollBar[Type]->MinPos = MinPos;
			LimitScrollInfo(Type, true);
			CalcScrollSlideWithPos(Type);
			UpdateScrollBarState(Type);
		}

	}
	void CWispWnd::SetScrollBarPage(int Type, int Page)
	{
		if (m_pScrollBar[Type])
		{
			m_pScrollBar[Type]->Page = Page;
			LimitScrollInfo(Type, true);
			CalcScrollSlideWithPos(Type);
			UpdateScrollBarState(Type);
		}
	}
	void CWispWnd::AdjustScrollBarMaxPos(int Type, int MaxPos)
	{
		if (m_pScrollBar[Type])
		{
			m_pScrollBar[Type]->MaxPos += MaxPos;
			LimitScrollInfo(Type, true);
			CalcScrollSlideWithPos(Type);
			UpdateScrollBarState(Type);
		}
	}

	WISP_CMD_MAP_BEGIN(CWispWnd)
		WISP_CMD_MAP(WISP_ID_VPAGE_DEC, OnCmdPageDec)
		WISP_CMD_MAP(WISP_ID_HPAGE_DEC, OnCmdPageDec)
		WISP_CMD_MAP(WISP_ID_VPAGE_INC, OnCmdPageInc)
		WISP_CMD_MAP(WISP_ID_HPAGE_INC, OnCmdPageInc)
		WISP_CMD_MAP(WISP_ID_VSPIN_SCROLL, OnCmdScrollBarDrag)
		WISP_CMD_MAP(WISP_ID_HSPIN_SCROLL, OnCmdScrollBarDrag)
		WISP_CMD_MAP(WISP_ID_VSPIN_DEC, OnCmdSpinDec)
		WISP_CMD_MAP(WISP_ID_HSPIN_DEC, OnCmdSpinDec)
		WISP_CMD_MAP(WISP_ID_VSPIN_INC, OnCmdSpinInc)
		WISP_CMD_MAP(WISP_ID_HSPIN_INC, OnCmdSpinInc)
		WISP_CMD_MAP(WISP_ID_CLOSE, OnCmdClose)
		WISP_CMD_MAP(WISP_ID_MAX, OnCmdMax)
		WISP_CMD_MAP(WISP_ID_MIN, OnCmdMin)
	WISP_CMD_MAP_END

	bool CWispWnd::OnScrollEvent(WISP_MSG *pMsg)
	{
		int type = pMsg->ScrollEvent.Type;

		int OldCurPos = m_pScrollBar[type]->CurPos;

		switch (pMsg->ScrollEvent.Event)
		{
		case WISP_SB_INC:
			++m_pScrollBar[type]->CurPos;
			LimitScrollInfo(type, false);
			pMsg->ScrollEvent.Delta = m_pScrollBar[type]->CurPos - OldCurPos;
			CalcScrollSlideWithPos(type);
			break;

		case WISP_SB_DEC:
			--m_pScrollBar[type]->CurPos;
			LimitScrollInfo(type, false);
			pMsg->ScrollEvent.Delta = m_pScrollBar[type]->CurPos - OldCurPos;
			CalcScrollSlideWithPos(type);
			break;

		case WISP_SB_PAGE_UP:
			m_pScrollBar[type]->CurPos -= m_pScrollBar[type]->AdjustPage;
			LimitScrollInfo(type, false);
			pMsg->ScrollEvent.Delta = m_pScrollBar[type]->CurPos - OldCurPos;
			CalcScrollSlideWithPos(type);
			break;

		case WISP_SB_PAGE_DOWN:
			m_pScrollBar[type]->CurPos += m_pScrollBar[type]->AdjustPage;
			LimitScrollInfo(type, false);
			pMsg->ScrollEvent.Delta = m_pScrollBar[type]->CurPos - OldCurPos;
			CalcScrollSlideWithPos(type);
			break;

		default: ;

		}
		Update((WISP_RECT*)0);
		return true;
	}
	void CWispWnd::SetScrollBarInfo(int Type, int CurPos, int Max, int Page, int Min, bool bSendEvent)
	{
		if (m_pScrollBar[Type])
		{
			m_pScrollBar[Type]->MaxPos = Max;
			m_pScrollBar[Type]->MinPos = Min;
			m_pScrollBar[Type]->Page = Page;
			SetScrollBarCurPos(Type, CurPos, bSendEvent);
		}
	}

	WISP_EVENT_MAP_BEGIN(CWispWnd)
	WISP_EVENT_MAP_END

	WISP_MSG_MAP_BEGIN(CWispWnd)
		WISP_MSG_MAP(WISP_WM_UPDATE_VERT_SCROLL_BAR, OnUpdateVertScrollBar)
		WISP_MSG_MAP(WISP_WM_UPDATE_HORZ_SCROLL_BAR, OnUpdateHorzScrollBar)
		WISP_MSG_MAP(WISP_WM_MOVE, OnMove)
		WISP_MSG_MAP(WISP_WM_MOVING, OnMoving)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP(WISP_WM_SIZING, OnSizing)
		WISP_MSG_MAP(WISP_WM_SCROLL_EVENT, OnScrollEvent)
		WISP_MSG_MAP(WISP_WM_VERT_SCROLL_MOVE, OnVertScrollMove)
		WISP_MSG_MAP(WISP_WM_HORZ_SCROLL_MOVE, OnHorzScrollMove)
		WISP_MSG_MAP(WISP_WM_TIMER_EVENT, OnTimer)
		WISP_MSG_MAP(WISP_WM_HIT_TEST, OnHitTest)
		WISP_MSG_MAP_ON_EVENT
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_UPDATE_BORDER, OnUpdateBorder)
		WISP_MSG_MAP(WISP_WM_UPDATE_CAPTION, OnUpdateCaption)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
		WISP_MSG_MAP(WISP_WM_UPDATE_CARET, OnUpdateCaret)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP(WISP_WM_MOUSE_MOVE, OnMouseMove)
		WISP_MSG_MAP(WISP_WM_CREATING, OnCreating)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_MOUSE_LEAVE, OnMouseLeave)
		WISP_MSG_MAP(WISP_WM_MOUSE_WHEEL, OnMouseWheel)
	WISP_MSG_MAP_END(CWispBaseWnd)


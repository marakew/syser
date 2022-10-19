
//#include "../Code/osdefine.hpp"
#include "../Code/define.h"
#include "../Code/sysdep.hpp"

#include "wispbase.hpp"
#include "wispbasewnd.hpp"
#include "wispwnd.hpp"
#include "wisptipwnd.hpp"
#include "wispmsgbox.hpp"

	CWispBaseWnd::CWispBaseWnd()
	{
		m_Style = 0;
		m_ShowMode = WISP_SH_NORMAL;
		m_State = WISP_WSTATE_INIT;
		m_ParentWnd = 0;
		m_CtrlType = WISP_CTRL_BASE_WND;
		m_pUpdateRC = 0;
		m_IsWindow = 0;
	}
	CWispBaseWnd::~CWispBaseWnd()
	{
		if (m_ShowMode == WISP_SH_MODAL_BLOCK)
			Destroy();
	}

	bool CWispBaseWnd::Create(const WCHAR *Name, int x, int y, int cx, int cy, CWispBaseWnd *pParentWnd, unsigned int CmdID, unsigned int Style, unsigned int ShowMode)
	{
		WISP_MSG Msg;

		m_pWispBase = gpCurWisp;

		if (!pParentWnd)
		{
			if (m_pWispBase->m_pRootWnd)
			{
				pParentWnd = m_pWispBase->m_pTopMostRootWnd;
			} else
			{
				m_pWispBase->m_pRootWnd = this;
				m_pWispBase->m_pMouseEventWnd = this;
				m_pWispBase->m_pFocusWnd = this;
				pParentWnd = m_pWispBase->m_pRootWnd;
				m_pWispBase->m_Metrics[WISP_SM_SCREEN_CX] = cx;
				m_pWispBase->m_Metrics[WISP_SM_SCREEN_CY] = cy;

				m_pWispBase->OnCreateMousePos(
					m_pWispBase->m_FrameBufferInfo.Width / 2,
					m_pWispBase->m_FrameBufferInfo.Height / 2);

				m_pWispBase->m_MouseRectUpd.x = 0;
				m_pWispBase->m_MouseRectUpd.y = 0;
				m_pWispBase->m_MouseRectUpd.cx = 0;
				m_pWispBase->m_MouseRectUpd.cy = 0;
				m_pWispBase->ChangeCursor(WISP_CT_ARROW);
				m_pWispBase->OnFrameBufferBppChg(m_pWispBase->m_FrameBufferInfo.Bpp);
			}
		}

		if (m_pWispBase->m_pRootWnd == this)
		{
			m_ScrWindowRect.x = x;
			m_ScrWindowRect.y = y;
		} else
		{
			m_ScrWindowRect.x = x + ((CWispWnd*)pParentWnd)->m_ScrClientRect.x; //???
			m_ScrWindowRect.y = y + ((CWispWnd*)pParentWnd)->m_ScrClientRect.y; //???
		}

		m_ScrWindowRect.cx = cx;
		m_WindowRect.cx = cx;

		m_ScrWindowRect.cy = cy;
		m_WindowRect.cy = cy;

		m_Style = Style;
		m_WindowRect.y = 0;
		m_WindowRect.x = 0;
		m_State = WISP_WSTATE_RESET;
		m_CmdID = CmdID;
		m_ParentWnd = 0;
		m_ChildTopWnd = 0;
		m_ChildWnd = 0;
		m_NextWnd = 0;
		m_PrevWnd = 0;
		m_PrevModalWnd = 0;

		m_pWispBase->InsertWnd(pParentWnd, this);
		m_OwnerWnd = m_ParentWnd;

		if (!m_WindowDC.Create(this, &m_ScrWindowRect))
			return false;

		if (Name)
			m_WndText = Name;

		if (m_Style & 0x8000)
		{
			m_IsWindow = 0x77777777;
			return true;
		}
		
		if (!InitWnd())
		{
			m_WndText.Empty();
			return false;
		}

		m_IsWindow = 0x77777777;
		Msg.hWnd = this;
		Msg.Msg = WISP_WM_CREATE;
		if (!m_pWispBase->SendMessage(&Msg))
		{
			Destroy();
			return false;
		}

		Show(ShowMode);
		return true;
	}
	bool CWispBaseWnd::Destroy()
	{
		if (m_IsWindow != 0x77777777)
			return false;

		WISP_MSG Msg;
		Msg.Msg = WISP_WM_DESTROY;
		Msg.hWnd = (CWispWnd *)this;
		if (!m_pWispBase->SendMessage(&Msg))
			return false;

		if ( (CWispBaseWnd *)m_pWispBase->m_pMouseEventWnd == this)
			m_pWispBase->m_pMouseEventWnd = (CWispWnd *)m_pWispBase->m_pRootWnd;

		CWispBaseWnd *pWnd = m_ChildWnd;
		while (pWnd)
		{
			CWispBaseWnd *old = pWnd;
			pWnd = pWnd->m_NextWnd;
			if (!old->Destroy())
				return false;
		}

		m_pWispBase->RemoveWndMsg(this);
		m_pWispBase->RemoveWnd(this);
		m_WindowDC.Destroy();

		if (m_pWispBase->m_pFocusWnd == this)
			m_pWispBase->m_pFocusWnd = m_pWispBase->m_pRootWnd != this ? m_pWispBase->m_pRootWnd : 0;

		if (m_pWispBase->m_pModalWnd == this)
		{
			m_pWispBase->m_pModalWnd = m_PrevModalWnd;
			m_PrevModalWnd = 0;
		}

		map<WISP_TIMER_KEY, WISP_TIMER>::IT it = m_pWispBase->m_TimerList.begin();
		for (; it != m_pWispBase->m_TimerList.end();)
		{
			if (it->first.pWnd == this)
			{
				m_pWispBase->m_TimerList.erase(it++);
			} else
			{
				++it;
			}
		}

		UnhookKeyEvent();
		UnhookMouseEvent();

		m_WndText.Empty();

		if (m_pWispBase->m_pTipWnd == this)
			m_pWispBase->m_pTipWnd = 0;

		if (m_pWispBase->m_pTipWnd &&
			m_pWispBase->m_pTipWnd->m_pTipOwnerWnd == this &&
			m_pWispBase->m_pTipWnd->m_ShowMode != WISP_SH_HIDDEN)
		{
			m_pWispBase->m_pTipWnd->Show(WISP_SH_HIDDEN);
			m_pWispBase->m_pTipWnd->m_pTipOwnerWnd = 0;
		}

		if (m_pWispBase->m_pRootWnd != this && m_pWispBase->m_pRootWnd && m_ShowMode != WISP_SH_HIDDEN)
		{
			m_pWispBase->m_pRootWnd->Update(&m_ScrWindowRect, this);
			m_pWispBase->RedrawNextTime();
		}

		m_State = WISP_WSTATE_INIT;
		m_IsWindow = 0;
		m_ShowMode = WISP_SH_NORMAL;
		if ((m_Style & 0x8000) == 0)
		{
			if (m_AdvStyle & 1) //WISP_ADV_AUTODEL
				delete this;
		}
		return true;
	}
	void CWispBaseWnd::RecalcLayout()
	{
		m_WindowRect.y = 0;
		m_WindowRect.x = 0;
		m_WindowRect.cx = m_ScrWindowRect.cx;
		m_WindowRect.cy = m_ScrWindowRect.cy;
	}
	bool CWispBaseWnd::InitWnd()
	{
		m_pTabWndItem = 0;
		m_ShowMode = WISP_SH_NORMAL;
		m_AdvStyle = 0;
		m_pUpdateRC = 0;
		m_LockUpdateRef = 0;
		return true;
	}
	void CWispBaseWnd::SetOwner(CWispBaseWnd *pOwnerWnd)
	{
		m_OwnerWnd = pOwnerWnd;
	}
	bool CWispBaseWnd::SetWindowText(const WCHAR *pString)
	{
		m_WndText = pString;
		Update((WISP_RECT*)0);
		return true;
	}
	TWideString & CWispBaseWnd::GetWindowText()
	{
		return m_WndText;
	}

	bool CWispBaseWnd::IsRect(WISP_POINT *pPT)
	{
		return PtInRect(&m_ScrWindowRect, pPT);
	}
	void CWispBaseWnd::Hide()
	{
		//EMPTY
	}
	void CWispBaseWnd::Show()
	{
		//EMPTY
	}

	WISP_MSG_MAP_BEGIN(CWispBaseWnd)
		WISP_MSG_MAP(WISP_WM_BEGIN_MOVE, OnBeginMove)
		WISP_MSG_MAP(WISP_WM_TIMER_EVENT, OnTimer)
		WISP_MSG_MAP(WISP_WM_GET_FOCUS, OnGetFocus)
		WISP_MSG_MAP(WISP_WM_LOST_FOCUS, OnLostFocus)
		WISP_MSG_MAP(WISP_WM_MOUSE_ENTER, OnMouseEnter)
		WISP_MSG_MAP(WISP_WM_MOUSE_LEAVE, OnMouseLeave)
		WISP_MSG_MAP(WISP_WM_CLOSE, OnClose)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
	WISP_MSG_MAP_END_

	bool CWispBaseWnd::Create(const WCHAR *Name, const CWispRect & pRC, CWispBaseWnd *pParentWnd, unsigned int CmdID, unsigned int Style, unsigned int ShowMode)
	{
		return Create(Name, pRC.x, pRC.y, pRC.cx, pRC.cy, pParentWnd, CmdID, Style, ShowMode);
	}

	bool CWispBaseWnd::SendCommand(unsigned int CmdMsg, int nParam1, void* pParam2)
	{
		WISP_MSG Msg;

		Msg.hWnd = m_OwnerWnd;
		Msg.Msg = WISP_WM_COMMAND;
		Msg.hParentWnd = this;
		Msg.Command.CtrlType = m_CtrlType;
		Msg.Command.CmdMsg = CmdMsg;
		Msg.Command.CmdID = m_CmdID;
		Msg.Command.Param1 = nParam1;
		Msg.Command.Param2 = pParam2;
		return m_pWispBase->SendMessage(&Msg);
	}
	bool CWispBaseWnd::SendEvent(unsigned int CmdMsg, int nParam1, void* pParam2)
	{
		WISP_MSG Msg;

		Msg.hWnd = m_OwnerWnd;
		Msg.Msg = WISP_WM_EVENT;
		Msg.hParentWnd = this;
		Msg.Command.CtrlType = m_CtrlType;
		Msg.Command.CmdMsg = CmdMsg;
		Msg.Command.CmdID = m_CmdID;
		Msg.Command.Param1 = nParam1;
		Msg.Command.Param2 = pParam2;
		return m_pWispBase->PostMessage(&Msg);

	}
	void CWispBaseWnd::Close()
	{
		WISP_MSG Msg;

		Msg.hWnd = this;
		Msg.Msg = WISP_WM_CLOSE;
		m_pWispBase->PostMessage(&Msg);
	}
	void CWispBaseWnd::Top(bool bUpdate)
	{
		CWispBaseWnd *pParentWnd = m_ParentWnd;
		if (pParentWnd && this != pParentWnd)
		{
			m_pWispBase->RemoveWnd(this);
			m_pWispBase->InsertWnd(pParentWnd, this);
			pParentWnd->Top(bUpdate);

			if (m_pWispBase->m_pTopMostRootWnd &&
				pParentWnd == m_pWispBase->m_pRootWnd &&
				this != m_pWispBase->m_pTopMostRootWnd)
				m_pWispBase->m_pTopMostRootWnd->Top(bUpdate);

			if (bUpdate)
				Update((WISP_RECT*)0);
		}
	}
	void CWispBaseWnd::Update(WISP_RECT *pRect, CWispBaseWnd *pWnd)
	{
		if (pRect)
		{
			if (m_pUpdateRC)
			{
				WISP_RECT rc2_v13 = *pRect;
				WISP_RECT rc1_v12 = *m_pUpdateRC;

				m_pUpdateRC = &m_UpdateRC;
				RectToRect(&rc1_v12, &rc2_v13, &m_UpdateRC); //Union |
			} else
			{
				m_UpdateRC = *pRect; //???
				m_pUpdateRC = &m_UpdateRC;
			}

			if (!m_pUpdateRC)
				return;

			if (!ClipRect(m_pUpdateRC, &m_WindowRect))
			{
				m_pUpdateRC = 0;
				return;
			}
		} else
		{
			m_pUpdateRC = &m_WindowRect;
		}

		for (CWispBaseWnd *pChildWnd = m_ChildWnd;
			pChildWnd;
			pChildWnd = pChildWnd->m_NextWnd)
		{
			if (pChildWnd != pWnd)
			{
				if (m_pUpdateRC)
				{
					WISP_RECT rc1_v12 = *m_pUpdateRC;

					rc1_v12.x += m_ScrWindowRect.x - pChildWnd->m_ScrWindowRect.x;
					rc1_v12.y += m_ScrWindowRect.y - pChildWnd->m_ScrWindowRect.y;

					if (pChildWnd->m_pUpdateRC)
					{
						WISP_RECT rc2_v13;
						RectToRect(&rc1_v12, pChildWnd->m_pUpdateRC, &rc2_v13); //Union |

						pChildWnd->Update(&rc2_v13, pWnd);
					} else
					{
						pChildWnd->Update(&rc1_v12, pWnd);
					}
					
				}
			}
		}
	}
	void CWispBaseWnd::Update(WISP_RECT *pRect)
	{
		if (m_LockUpdateRef > 0)
			return;
		
		Update(pRect, 0);

		if (m_pWispBase->m_pRootWnd)
		{
			if (m_pWispBase->m_pRootWnd != this)
			{
				if (m_pUpdateRC)
				{
					WISP_RECT rc1_v15 = *m_pUpdateRC;

					rc1_v15.x += m_ScrWindowRect.x;
					rc1_v15.y += m_ScrWindowRect.y;

					if (m_pWispBase->m_pRootWnd->m_pUpdateRC)
					{
						WISP_RECT rc3_v16;
						RectToRect(&rc1_v15, m_pWispBase->m_pRootWnd->m_pUpdateRC, &rc3_v16); //Union |

						m_pWispBase->m_pRootWnd->Update(&rc3_v16, this);
					} else
					{
						m_pWispBase->m_pRootWnd->Update(&rc1_v15, this);
					}
					
				}
			}
		}
		m_pWispBase->RedrawNextTime();
	}
	void CWispBaseWnd::Move(int DeltaX, int DeltaY, bool bUpdate)
	{
		if (DeltaX || DeltaY)
		{
			WISP_MSG Msg;
			Msg.hWnd = this;
			Msg.Msg = WISP_WM_MOVING;
			Msg.Move.DeltaX = DeltaX;
			Msg.Move.DeltaY = DeltaY;

			WISP_RECT rc1_v19;
			if (m_ParentWnd)
			{
				rc1_v19 = m_ScrWindowRect;
			}

			if (m_pWispBase->SendMessage(&Msg))
			{
				m_ScrWindowRect.x += DeltaX;
				m_ScrWindowRect.y += DeltaY;
				RecalcLayout();
				for (CWispBaseWnd *pWnd = m_ChildWnd;
					pWnd;
					pWnd = pWnd->m_NextWnd)
					pWnd->Move(DeltaX, DeltaY, 0);

				Msg.hWnd = this;
				Msg.Msg = WISP_WM_MOVE;
				Msg.Move.DeltaX = DeltaX;
				Msg.Move.DeltaY = DeltaY;
				m_pWispBase->SendMessage(&Msg);
				RecalcLayout();

				if (bUpdate)
				{
					if (m_ParentWnd)
					{
						WISP_RECT rc2_v17;
						rc2_v17 = m_ScrWindowRect;

						WISP_RECT rc3_v21;
						RectToRect(&rc1_v19, &rc2_v17, &rc3_v21); //Union |

						rc3_v21.x -= m_ParentWnd->m_ScrWindowRect.x;
						rc3_v21.y -= m_ParentWnd->m_ScrWindowRect.y;
						m_ParentWnd->Update(&rc3_v21);
					} else
					{
						Update((WISP_RECT*)0);
					}
				}
			}
		}
	}
	void CWispBaseWnd::MoveToWindow(int PosX, int PosY, bool bUpdate)
	{
		if (this != m_pWispBase->m_pRootWnd)
		{
			if (m_ParentWnd)
				Move(
					PosX + m_ParentWnd->m_ScrWindowRect.x - m_ScrWindowRect.x,
					PosY + m_ParentWnd->m_ScrWindowRect.y - m_ScrWindowRect.y,
					bUpdate);
			else
				Move(PosX - m_ScrWindowRect.x, PosY - m_ScrWindowRect.y, bUpdate);
		}

	}

	void CWispBaseWnd::MoveToClient(int PosX, int PosY, bool bUpdate)
	{
		if (this != m_pWispBase->m_pRootWnd)
		{
			if (m_ParentWnd)
				Move(
					PosX + ((CWispWnd*)m_ParentWnd)->m_ScrClientRect.x - m_ScrWindowRect.x,
					PosY + ((CWispWnd*)m_ParentWnd)->m_ScrClientRect.y - m_ScrWindowRect.y,
					bUpdate);
			else
				Move(PosX - m_ScrWindowRect.x, PosY - m_ScrWindowRect.y, bUpdate);
		}
	}
	void CWispBaseWnd::MoveToScreen(int PosX, int PosY, bool bUpdate)
	{
		if (this != m_pWispBase->m_pRootWnd)
			Move(PosX - m_ScrWindowRect.x, PosY - m_ScrWindowRect.y, bUpdate);
	}
	bool CWispBaseWnd::Resize(int cx, int cy, bool bUpdate)
	{
		if (cx == m_WindowRect.cx && cy == m_WindowRect.cy)
			return true;
		
		MIN_LIMIT(cx, 0);
		MIN_LIMIT(cy, 0);

		if (m_Style & 0x8000) //CWispWnd
		{
			if (((CWispWnd*)this)->m_MaxWndSize.cx && cx > ((CWispWnd*)this)->m_MaxWndSize.cx)
			{
				if (m_State & 2) //WISP_WSTATE_RESIZE
					return false;
				cx = ((CWispWnd*)this)->m_MaxWndSize.cx;
			}

			if (((CWispWnd*)this)->m_MaxWndSize.cy && cy > ((CWispWnd*)this)->m_MaxWndSize.cy)
			{
				if (m_State & 2) //WISP_WSTATE_RESIZE
					return false;
				cy = ((CWispWnd*)this)->m_MaxWndSize.cy;
			}
		}
		if (cx == -1) cx = m_WindowRect.cx;
		if (cy == -1) cy = m_WindowRect.cy;

		WISP_MSG Msg;
		Msg.hWnd = this;
		Msg.Msg = WISP_WM_SIZING; //???
		Msg.Sizing.Size.cx = cx;
		Msg.Sizing.Size.cy = cy;

		WISP_RECT rc1_v25;
		if (m_ParentWnd)
		{
			rc1_v25 = m_ScrWindowRect;
			rc1_v25.x -= m_ParentWnd->m_ScrWindowRect.x;
			rc1_v25.y -= m_ParentWnd->m_ScrWindowRect.y;
		}

		Msg.Sizing.ScrSize.cx = cx - m_ScrWindowRect.cx;
		Msg.Sizing.ScrSize.cy = cy - m_ScrWindowRect.cy;

		if (!m_pWispBase->SendMessage(&Msg))
			return false;

		m_ScrWindowRect.cx = cx;
		m_ScrWindowRect.cy = cy;
		RecalcLayout();

		Msg.hWnd = this;
		Msg.Msg = WISP_WM_SIZE;
		m_pWispBase->SendMessage(&Msg);
		RecalcLayout();
		for (CWispBaseWnd *pChildWnd = m_ChildWnd;
			pChildWnd;
			pChildWnd = pChildWnd->m_NextWnd)
		{
			Msg.hWnd = pChildWnd;
			Msg.Msg = WISP_WM_RESIZE_CHILD;
			m_pWispBase->SendMessage(&Msg);
		}

		if (bUpdate)
		{
			if (m_ParentWnd)
			{
				WISP_RECT rc2_v24 = m_ScrWindowRect;

				rc2_v24.x -= m_ParentWnd->m_ScrWindowRect.x;
				rc2_v24.y -= m_ParentWnd->m_ScrWindowRect.y;

				WISP_RECT rc3_v26;
				RectToRect(&rc1_v25, &rc2_v24, &rc3_v26); //Union |

				m_ParentWnd->Update(&rc3_v26);
			} else
			{
				Update((WISP_RECT*)0);
			}
		}

		return true;
	}
	void CWispBaseWnd::Center(CWispBaseWnd *pCenterWnd)
	{
		if (pCenterWnd)
		{
			MoveToWindow(
				pCenterWnd->m_ScrWindowRect.x + (pCenterWnd->m_WindowRect.cx - m_WindowRect.cx) / 2,
				pCenterWnd->m_ScrWindowRect.y + (pCenterWnd->m_WindowRect.cy - m_WindowRect.cy) / 2,
				true);
		} else
		{
			if (this != m_pWispBase->m_pRootWnd)
				Move(
				(m_pWispBase->m_pRootWnd->m_WindowRect.cx - m_WindowRect.cx) / 2 - m_ScrWindowRect.x,
				(m_pWispBase->m_pRootWnd->m_WindowRect.cy - m_WindowRect.cy) / 2 - m_ScrWindowRect.y,
				true);
		}
	}
	CWispBaseWnd *CWispBaseWnd::Focus()
	{
		WISP_MSG Msg;

		if (m_pWispBase->m_pFocusWnd == this)
			return m_pWispBase->m_pFocusWnd;

		Msg.hWnd = this;
		Msg.Msg = WISP_WM_GET_FOCUS;
		Msg.hOtherWnd = m_pWispBase->m_pFocusWnd;
		if (m_pWispBase->SendMessage(&Msg))
		{
			Msg.hWnd = (CWispBaseWnd*)Msg.hOtherWnd; //??????m_pWispBase->m_pFocusWnd; //??? Msg.u.field_0
			Msg.Msg = WISP_WM_LOST_FOCUS;
			Msg.hOtherWnd = this;
			if (m_pWispBase->SendMessage(&Msg))
				m_pWispBase->m_pFocusWnd = this;
		}
		Update((WISP_RECT*)0);
		m_pWispBase->CaptureMouseEvent(this);
		return m_pWispBase->m_pFocusWnd;
	}
	bool CWispBaseWnd::IsWindow()
	{
		return m_IsWindow == 0x77777777;
	}
	bool CWispBaseWnd::IsChildWndOf(CWispBaseWnd *pParentWnd)
	{
		for (CWispBaseWnd *pWnd = m_ParentWnd;
			pWnd;
			pWnd = pWnd->m_ParentWnd)
		{
			if (pWnd == pParentWnd)
				return true;
		}
		return false;
	}
	bool CWispBaseWnd::IsWindowShow()
	{
		if (!IsWindow())
			return false;
		if (m_ShowMode != WISP_SH_HIDDEN && (m_Style & 0x100) == 0 && m_WindowRect.cx && m_WindowRect.cy)
			return true;
		return false;
	}

	bool CWispBaseWnd::OnGetFocus(WISP_MSG *pMsg)
	{
		if (!SendCommand(WISP_ID_CHILD_GET_FOCUS, 0, 0))
			return false;
		Top(true);
		return true;
	}
	bool CWispBaseWnd::OnLostFocus(WISP_MSG *pMsg)
	{
		return SendCommand(WISP_ID_CHILD_LOST_FOCUS, 0, pMsg->hOtherWnd);
	}

	bool CWispBaseWnd::Show(unsigned int ShowMode)
	{
		if (m_ShowMode == ShowMode)
			return true;

		if (m_ShowMode == WISP_SH_HIDDEN && ShowMode != WISP_SH_HIDDEN)
			Show();

		m_ShowMode = ShowMode;

		if (ShowMode == WISP_SH_MODAL)
		{
			if (m_pWispBase->m_pModalWnd != this)
			{
				m_PrevModalWnd = m_pWispBase->m_pModalWnd;
				m_pWispBase->m_pModalWnd = this;
			}
			Top(true);
			Update((WISP_RECT*)0);
			return true;
		}

		if (ShowMode == WISP_SH_MODAL_BLOCK)
		{
			if (m_pWispBase->m_pModalWnd != this)
			{
				m_PrevModalWnd = m_pWispBase->m_pModalWnd;
				m_pWispBase->m_pModalWnd = this;
				Top(true);
				Update((WISP_RECT*)0);
				m_pWispBase->BlockPump(this);
				return true;
			}
		} else
		if (m_pWispBase->m_pModalWnd == this)
		{
			m_pWispBase->m_pModalWnd = m_PrevModalWnd;
			m_PrevModalWnd = 0;
		}

		if (m_ShowMode == WISP_SH_HIDDEN)
		{
			if (m_pWispBase->m_pMouseEventWnd == this)
			{
				WISP_MSG Msg;
				Msg.hWnd = this;
				Msg.Msg = WISP_WM_MOUSE_LEAVE;
				Msg.MouseEvent.HitArea.Type = 0;
				Msg.MouseEvent.HitArea.State = 0;
				Msg.MouseEvent.hWndMouseAt = this;
				Msg.MouseEvent.DeltaX = 0;
				Msg.MouseEvent.DeltaY = 0;
				m_pWispBase->SendMessage(&Msg);
			}
			Hide();
		}

		Update((WISP_RECT*)0);
		return true;
	}

	bool CWispBaseWnd::MoveWindowToWindow(const WISP_RECT & Rect, bool bUpdate)
	{
		MoveToWindow(Rect.x, Rect.y, bUpdate);
		return Resize(Rect.cx, Rect.cy, bUpdate);
	}
	bool CWispBaseWnd::MoveWindowToWindow(int x, int y, int cx, int cy, bool bUpdate)
	{
		MoveToWindow(x, y, bUpdate);
		return Resize(cx, cy, bUpdate);
	}
	bool CWispBaseWnd::MoveWindowToClient(const WISP_RECT & Rect, bool bUpdate)
	{
		MoveToClient(Rect.x, Rect.y, bUpdate);
		return Resize(Rect.cx, Rect.cy, bUpdate);
	}
	bool CWispBaseWnd::MoveWindowToClient(int x, int y, int cx, int cy, bool bUpdate)
	{
		MoveToClient(x, y, bUpdate);
		return Resize(cx, cy, bUpdate);
	}

	bool CWispBaseWnd::HookKeyEvent()
	{
		TListIter<CWispBaseWnd*> it = m_pWispBase->m_KeyHookWndList.Begin();
		for (;it != m_pWispBase->m_KeyHookWndList.End(); ++it)
		{
			if (*it == this)
				return false;
		}
		CWispBaseWnd *pWnd = this;
		m_pWispBase->m_KeyHookWndList.Append(&pWnd);
		return true;
	}

	bool CWispBaseWnd::UnhookKeyEvent()
	{
		TListIter<CWispBaseWnd*> it = m_pWispBase->m_KeyHookWndList.Begin();
		for (;it != m_pWispBase->m_KeyHookWndList.End(); ++it)
		{
			if (*it == this)
			{
				m_pWispBase->m_KeyHookWndList.Remove(it);
				return true;
			}
		}
		return false;
	}

	bool CWispBaseWnd::HookMouseEvent()
	{
		TListIter<CWispBaseWnd*> it = m_pWispBase->m_MouseHookWndList.Begin();
		for (;it != m_pWispBase->m_MouseHookWndList.End(); ++it)
		{
			if (*it == this)
				return false;
		}
		CWispBaseWnd *pWnd = this;
		m_pWispBase->m_MouseHookWndList.Append(&pWnd);
		return true;
	}

	bool CWispBaseWnd::UnhookMouseEvent()
	{
		TListIter<CWispBaseWnd*> it = m_pWispBase->m_MouseHookWndList.Begin();
		for (;it != m_pWispBase->m_MouseHookWndList.End(); ++it)
		{
			if (*it == this)
			{
				m_pWispBase->m_MouseHookWndList.Remove(it);
				return true;
			}
		}
		return false;
	}

	WISP_KEY_EVENT * CWispBaseWnd::RegisterKeyEvent(unsigned int CmdID, unsigned int KeyType, unsigned int SpecKey, int Disable)
	{
		TListIter<WISP_KEY_EVENT> p = m_pWispBase->m_KeyEventList.Append();
		p->CmdID = CmdID;
		p->KeyType = KeyType;
		p->SpecKey = SpecKey;
		p->pWnd = (CWispWnd*)this;
		p->Disable = Disable;
		return &*p;
	}

	bool CWispBaseWnd::UnregisterKeyEvent(unsigned int CmdID)
	{
		TListIter<WISP_KEY_EVENT> it = m_pWispBase->m_KeyEventList.Begin();
		for (;it != m_pWispBase->m_KeyEventList.End(); ++it)
		{
			if (it->CmdID == CmdID)
			{
				m_pWispBase->m_KeyEventList.Remove(it);
				return true;
			}
		}
		return false;
	}

	bool CWispBaseWnd::EnableHelpTip(const WCHAR *pString)
	{
		if (!pString) return false;

		m_HelpTipString = pString;
		m_AdvStyle |= 2;	//WISP_ADV_HINT
		return true;
	}

	bool CWispBaseWnd::DisableHelpTip()
	{
		m_HelpTipString.Empty();
		m_AdvStyle &= ~2;	//WISP_ADV_HINT
		return true;
	}

	void *CWispBaseWnd::GetTimer(unsigned int TimerID)
	{
		WISP_TIMER_KEY Key;
		Key.pWnd = this;
		Key.ID = TimerID;
		map<WISP_TIMER_KEY, WISP_TIMER>::IT it = m_pWispBase->m_TimerList.find(Key);
		if (it == m_pWispBase->m_TimerList.end())
			return nullptr;

		return &it->second;
	}

	WISP_TIMER *CWispBaseWnd::InsertTimer(unsigned int TimerID, int Period, unsigned int UserData, void *UserPtr)
	{
		if (GetTimer(TimerID))
			return nullptr;

		WISP_TIMER_KEY Key;
		Key.pWnd = this;
		Key.ID = TimerID;

		map<WISP_TIMER_KEY, WISP_TIMER>::IT it = m_pWispBase->m_TimerList.insert(
			_Tmap_pair<WISP_TIMER_KEY, WISP_TIMER>(Key,WISP_TIMER() ) );
		if (it == m_pWispBase->m_TimerList.end())
			return nullptr;

		it->second.Period = Period;
		it->second.UserData = UserData;
		it->second.UserPtr = UserPtr;
		it->second.Elapse = 0;

		if (!m_pWispBase->InsertTimer(it->second))
		{
			m_pWispBase->m_TimerList.erase(it);
			return nullptr;
		}
		return &it->second;
	}
	bool CWispBaseWnd::RemoveTimer(unsigned int TimerID)
	{
		WISP_TIMER_KEY Key;
		Key.pWnd = this;
		Key.ID = TimerID;

		map<WISP_TIMER_KEY, WISP_TIMER>::IT it = m_pWispBase->m_TimerList.find(Key);
		if (it == m_pWispBase->m_TimerList.end())
			return false;

		if (m_pWispBase->RemoveTimer(it->second))
			m_pWispBase->m_TimerList.erase(it);

		return false;
	}

	bool CWispBaseWnd::EnableWindow(bool bState)
	{
		bool Result = ~(m_Style >> 9) & 1;	//???

		if (bState)
		{
			m_Style &= ~0x200;
		} else
		{
			m_Style |= 0x200;

			//WISP_ADV_HINT
			if (m_AdvStyle & 2 && m_pWispBase->m_pTipWnd->m_pTipOwnerWnd == this)
			{
				RemoveTimer(WISP_ID_HELP_TIP);
				m_pWispBase->m_pTipWnd->Show(WISP_SH_HIDDEN);
				m_pWispBase->m_pTipWnd->m_pTipOwnerWnd = 0;
			}

		}
		RecalcLayout();
		if (Result != bState)
			Update((WISP_RECT*)0);
		return Result;
	}

	bool CWispBaseWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
		//WISP_ADV_HINT
		if ((m_AdvStyle & 2) && pMsg->KeyEvent.KeyType == VK_LBUTTON && pMsg->KeyEvent.bKeyDown)
		{
			RemoveTimer(WISP_ID_HELP_TIP);
			m_pWispBase->m_pTipWnd->Show(WISP_SH_HIDDEN);
			m_pWispBase->m_pTipWnd->m_pTipOwnerWnd = 0;
		}

		if (m_OwnerWnd && m_OwnerWnd->m_CtrlType == WISP_CTRL_FORM)
			SendCommand(
				pMsg->KeyEvent.bKeyDown != 0 ? 0x80000119 : 0x80000120, //WISP_ID_FORM_KEYDOWN : WISP_ID_FORM_KEYUP
				pMsg->KeyEvent.KeyType,
				this);
		return true;

	}
	bool CWispBaseWnd::OnBeginMove(WISP_MSG *pMsg)
	{
		//WISP_ADV_HINT
		if ((m_AdvStyle & 2) && m_pWispBase->m_pTipWnd->IsWindow())
		{
			RemoveTimer(WISP_ID_HELP_TIP);
			CWispTipWnd *pWispTipWnd = m_pWispBase->m_pTipWnd;
			if (pWispTipWnd->m_ShowMode != WISP_SH_HIDDEN)
			{
				pWispTipWnd->m_ShowMode = WISP_SH_HIDDEN;
				if (pWispTipWnd->m_pWispBase->m_pModalWnd == pWispTipWnd)
				{
					pWispTipWnd->m_pWispBase->m_pModalWnd = pWispTipWnd->m_PrevModalWnd;
					pWispTipWnd->m_PrevModalWnd = 0;
				}
				if (pWispTipWnd->m_ShowMode == WISP_SH_HIDDEN)
				{
					if (pWispTipWnd->m_pWispBase->m_pMouseEventWnd == pWispTipWnd)
					{
						WISP_MSG Msg;
						Msg.hWnd = pWispTipWnd;
						Msg.Msg = WISP_WM_MOUSE_LEAVE;

						Msg.MouseEvent.HitArea.Type = 0;
						Msg.MouseEvent.HitArea.State = 0;
						Msg.MouseEvent.hWndMouseAt = pWispTipWnd;
						Msg.MouseEvent.DeltaX = 0;
						Msg.MouseEvent.DeltaY = 0;

						pWispTipWnd->m_pWispBase->SendMessage(&Msg);
					}
					pWispTipWnd->Hide();
				}
				pWispTipWnd->Update((WISP_RECT*)0);
			}
			m_pWispBase->m_pTipWnd->m_pTipOwnerWnd = 0;
		}

		return true;
	}
	bool CWispBaseWnd::OnTimer(WISP_MSG *pMsg)
	{
		//WISP_ADV_HINT
		if (m_AdvStyle & 2)
		{
			if (pMsg->Timer.ID == WISP_ID_HELP_TIP)
			{
				if (PtInRect(&m_ScrWindowRect, &m_pWispBase->m_MousePT))
				{
					RemoveTimer(WISP_ID_HELP_TIP);
					//???
					if ( !((m_Style >> 9) & 1) )
					{
						m_pWispBase->m_pTipWnd->SetWindowText(m_HelpTipString);
						m_pWispBase->m_pTipWnd->AutoAdjustPostion((CWispWnd*)this);
						m_pWispBase->m_pTipWnd->Show(WISP_SH_MINSIZE);
					}
				}
			}
		}

		return true;
	}
	bool CWispBaseWnd::OnMouseEnter(WISP_MSG *pMsg)
	{
		//WISP_ADV_HINT
		if (m_AdvStyle & 2)
			InsertTimer(WISP_ID_HELP_TIP, 500, 0, 0);

		return true;
	}
	bool CWispBaseWnd::OnMouseLeave(WISP_MSG *pMsg)
	{
		m_pWispBase->CaptureMouseEvent((CWispWnd*)pMsg->MouseEvent.hWndMouseAt);

		//WISP_ADV_HINT
		if (m_AdvStyle & 2)
		{
			RemoveTimer(WISP_ID_HELP_TIP);
			CWispTipWnd *pWispTipWnd = m_pWispBase->m_pTipWnd;
			if (pWispTipWnd->m_ShowMode != WISP_SH_HIDDEN)
			{
				pWispTipWnd->m_ShowMode = WISP_SH_HIDDEN;
				if (pWispTipWnd->m_pWispBase->m_pModalWnd == pWispTipWnd)
				{
					pWispTipWnd->m_pWispBase->m_pModalWnd = pWispTipWnd->m_PrevModalWnd;
					pWispTipWnd->m_PrevModalWnd = 0;
				}

				if (pWispTipWnd->m_ShowMode == WISP_SH_HIDDEN)
				{
					if (pWispTipWnd->m_pWispBase->m_pMouseEventWnd == pWispTipWnd)
					{
						WISP_MSG Msg;
						Msg.hWnd = pWispTipWnd;
						Msg.Msg = WISP_WM_MOUSE_LEAVE;
						Msg.MouseEvent.HitArea.Type = 0;
						Msg.MouseEvent.HitArea.State = 0;
						Msg.MouseEvent.hWndMouseAt = pWispTipWnd;
						Msg.MouseEvent.DeltaX = 0;
						Msg.MouseEvent.DeltaY = 0;

						pWispTipWnd->m_pWispBase->SendMessage(&Msg);
					}
					pWispTipWnd->Hide();
				}
				pWispTipWnd->Update((WISP_RECT*)0);
			}
			m_pWispBase->m_pTipWnd->m_pTipOwnerWnd = 0;
		}

		return true;

	}

	WISP_EVENT_MAP_BEGIN(CWispBaseWnd)
//		WISP_EVENT_MAP(..., ...)
	WISP_EVENT_MAP_END

	bool CWispBaseWnd::OnClose(WISP_MSG *pMsg)
	{
		return Destroy();
	}

	int CWispBaseWnd::MsgBox(WCHAR *pText, WCHAR *pCaption, unsigned int uType, CWispBaseWnd *pParentWnd)
	{
		return CWispMsgBox::StaticShowMsgBox(pText, pCaption, uType, pParentWnd);
	}


#include "wispsplitwnd.hpp"
#include "wispbase.hpp"
#include "utility.hpp"

	CWispSplitWnd::CWispSplitWnd()
	{
		m_SplitBorderSize = 4;
		m_CtrlType = WISP_CTRL_SPLIT_WND;
	}
	CWispSplitWnd::~CWispSplitWnd()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispSplitWnd)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
		WISP_MSG_MAP(WISP_WM_MOUSE_MOVE, OnMouseMove)
		WISP_MSG_MAP(WISP_WM_MOUSE_LEAVE, OnMouseLeave)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP(WISP_WM_MOUSE_DBLCLICK, OnMouseDblClick)
		WISP_MSG_MAP(WISP_WM_RESIZE_CHILD, OnResizeChild)
	WISP_MSG_MAP_END(CWispWnd)

	bool CWispSplitWnd::OnDestroy(WISP_MSG *pMsg)
	{
		m_WndList.Clear();
		return true;
	}
	bool CWispSplitWnd::OnCreate(WISP_MSG *pMsg)
	{
		m_Size.cx = m_ParentWnd->m_WindowRect.cx - m_WindowRect.cx;
		m_Size.cy = m_ParentWnd->m_WindowRect.cy - m_WindowRect.cy;
		return true;
	}
	bool CWispSplitWnd::OnUpdate(WISP_MSG *pMsg)
	{
		TListIter<WISP_SPLIT_WND> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			m_pWispBase->m_pWispDrawObj->DrawSplitRect(&m_WindowDC, &It->SplitRect, (m_Style & 0x20000)?1:0);
			++It;
		}
		return true;
	}
	bool CWispSplitWnd::OnUpdateClient(WISP_MSG *pMsg)
	{
		return false;
	}
	bool CWispSplitWnd::OnMouseMove(WISP_MSG *pMsg)
	{
		WISP_POINT MouseCltPT = pMsg->MsgMouseWndPT;
		PointToRect(&MouseCltPT, &m_ClientRect);
		int Delta = (m_Style & 0x20000) ? pMsg->MouseEvent.DeltaX : pMsg->MouseEvent.DeltaY;
		if (m_CurSplitWndIt)
		{
			bool bMove = false;
			if (m_Style & 0x20000)
			{
				int x = m_CurSplitWndIt->SplitRect.x + m_CurSplitWndIt->SplitRect.cx/2;
				if (Delta>=0)
					bMove = (pMsg->MsgMouseWndPT.x<x);
				else	bMove = (pMsg->MsgMouseWndPT.x>=x);
			} else
			{
				int y = m_CurSplitWndIt->SplitRect.y + m_CurSplitWndIt->SplitRect.cy/2;
				if (Delta>=0)
					bMove = (pMsg->MsgMouseWndPT.y<y);
				else	bMove = (pMsg->MsgMouseWndPT.y>=y);
			}
			if (bMove) return true;
			m_NextSplitWndIt = m_CurSplitWndIt+1;
			if (m_NextSplitWndIt != m_WndList.End() &&
				m_CurSplitWndIt->SizeXorY  + Delta > 0 &&
				m_NextSplitWndIt->SizeXorY - Delta > 0)
			{
				m_CurSplitWndIt->SizeXorY += Delta;
				if (m_NextSplitWndIt != m_WndList.End())
					m_NextSplitWndIt->SizeXorY -= Delta;
				AdjustWndPos();
			}
		} else
		{
			TListIter<WISP_SPLIT_WND> It = m_WndList.Begin();
			while (It != m_WndList.End())
			{
				if (PtInRect(&It->SplitRect, &pMsg->MsgMouseWndPT))
					m_pWispBase->ChangeCursor((m_Style & 0x20000) ? WISP_CT_RESIZE_HORZ:WISP_CT_RESIZE_VERT);
				++It;
			}			
		}
		return true;
	}
	bool CWispSplitWnd::OnMouseLeave(WISP_MSG *pMsg)
	{
		int Delta = (m_Style & 0x20000) ? pMsg->MouseEvent.DeltaX : pMsg->MouseEvent.DeltaY;
		if (m_CurSplitWndIt)
		{
			bool bMove = false;
			if (m_Style & 0x20000)
			{
				int x = m_CurSplitWndIt->SplitRect.x + m_CurSplitWndIt->SplitRect.cx;
				if (Delta>=0)
					bMove = (pMsg->MsgMouseWndPT.x<m_CurSplitWndIt->SplitRect.x);
				else	bMove = (pMsg->MsgMouseWndPT.x>=x);
			} else
			{
				int y = m_CurSplitWndIt->SplitRect.y + m_CurSplitWndIt->SplitRect.cy;
				if (Delta>=0)
					bMove = (pMsg->MsgMouseWndPT.y<m_CurSplitWndIt->SplitRect.y);
				else	bMove = (pMsg->MsgMouseWndPT.y>=y);
			}
			if (bMove) return false;
			m_NextSplitWndIt = m_CurSplitWndIt+1;
			if (m_NextSplitWndIt != m_WndList.End() &&
				m_CurSplitWndIt->SizeXorY  + Delta > 0 &&
				m_NextSplitWndIt->SizeXorY - Delta > 0)
			{
				m_CurSplitWndIt->SizeXorY += Delta;
				if (m_NextSplitWndIt != m_WndList.End())
					m_NextSplitWndIt->SizeXorY -= Delta;
				AdjustWndPos();
			}
			return false;
		}

		m_pWispBase->ChangeCursor(WISP_CT_ARROW);
		return true;
	}
	bool CWispSplitWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.KeyType != VK_LBUTTON)
			return true;

		if (!pMsg->KeyEvent.bKeyDown)
		{
			m_CurSplitWndIt.Reset();// = 0;
			return true;
		}

		WISP_POINT MouseCltPT = pMsg->MsgMouseWndPT;
		PointToRect(&MouseCltPT, &m_ClientRect);
		TListIter<WISP_SPLIT_WND> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			if (PtInRect(&It->SplitRect, &pMsg->MsgMouseWndPT))
			{
				m_CurSplitWndIt = It;
				break;
			}
			++It;
		}
		return true;
	}
	bool CWispSplitWnd::OnSize(WISP_MSG *pMsg)
	{
		if (m_Style & 0x40000)
		{
			m_Size.cx = m_ParentWnd->m_WindowRect.cx - m_WindowRect.cx;
			m_Size.cy = m_ParentWnd->m_WindowRect.cy - m_WindowRect.cy;
		}
		AdjustWndPos();
		return true;
	}
	bool CWispSplitWnd::OnMouseDblClick(WISP_MSG *pMsg)
	{
		WISP_POINT MouseCltPT = pMsg->MsgMouseWndPT;
		PointToRect(&MouseCltPT, &m_ClientRect);
		int Index = 0;
		TListIter<WISP_SPLIT_WND> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			if (PtInRect(&It->SplitRect, &pMsg->MsgMouseWndPT))
			{
				if (pMsg->KeyEvent.KeyType == VK_LBUTTON)
					SendCommand(0x80000118, Index, &*It); //WISP_ID_LBUTTON_DBLCLICK
				else
				if (pMsg->KeyEvent.KeyType == VK_RBUTTON)
					SendCommand(0x80000117, Index, &*It); //WISP_ID_RBUTTON_DBLCLICK
				break;
			}
			++It;
			++Index;
		}
		return true;
	}
	bool CWispSplitWnd::OnResizeChild(WISP_MSG *pMsg)
	{
		if (m_Style & 0x40000)
			Resize(pMsg->Sizing.Size.cx - m_Size.cx, pMsg->Sizing.Size.cy - m_Size.cy, true);
		return true;
	}
	bool CWispSplitWnd::InsertWnd(CWispWnd *pWnd, unsigned int Style, int SizeXorY, int InsertPos)
	{	
		if (InsertPos > m_WndList.Size())
			return false;
		WISP_SPLIT_WND WSWnd;
		WSWnd.pWnd = pWnd;
		WSWnd.Style = Style;
		WSWnd.SizeXorY = SizeXorY;
		if (InsertPos >= 0)
		{
			TListIter<WISP_SPLIT_WND> It;
			if (m_WndList.Size() > 0)
				It = m_WndList[InsertPos];
			m_WndList.InsertBefore(It, &WSWnd);
		} else
		{
			m_WndList.Append(&WSWnd);
		}
		return true;
	}
	bool CWispSplitWnd::RemoveWnd(CWispWnd *pWnd)
	{
		TListIter<WISP_SPLIT_WND> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			if (It->pWnd == pWnd)
			{
				m_WndList.Remove(It);
				return true;
			}
			++It;
		}
		return false;
	}
	void CWispSplitWnd::AdjustWnd(int Pos)
	{
		int SizeXorY = (m_Style & 0x20000) ? m_ClientRect.cx : m_ClientRect.cy;
		int Index = 0;
		TListIter<WISP_SPLIT_WND> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			if (Index == Pos)
				It->SizeXorY = SizeXorY - (m_SplitBorderSize + 1) * (m_WndList.Size() - 1);
			else	It->SizeXorY = 1;
			++Index;
			++It;
		}
		AdjustWndPos();
	}
	void CWispSplitWnd::AdjustWndPos()
	{
		int SizeXorY = 0;
		int Index = 0;
		TListIter<WISP_SPLIT_WND> It = m_WndList.Begin();
		while (Index < m_WndList.Size() && It != m_WndList.End())
		{
			if (m_Style & 0x20000)
			{
				if (Index == m_WndList.Size()-1)
					It->SizeXorY = m_ClientRect.cx - SizeXorY;
				It->pWnd->MoveToClient(SizeXorY, 0, true);
				It->pWnd->Resize(It->SizeXorY, m_ClientRect.cy, true);
				It->SplitRect.y = m_ClientRect.y;
				It->SplitRect.cy = m_ClientRect.cy;
				It->SplitRect.x = SizeXorY + m_ClientRect.x + It->pWnd->m_WindowRect.cx;
				It->SplitRect.cx = m_SplitBorderSize;
			} else
			{
				if (Index == m_WndList.Size()-1)
					It->SizeXorY = m_ClientRect.cy - SizeXorY;
				It->pWnd->MoveToClient(0, SizeXorY, true);
				It->pWnd->Resize(m_ClientRect.cx, It->SizeXorY, true);
				It->SplitRect.x = m_ClientRect.x;
				It->SplitRect.cx = m_ClientRect.cx;
				It->SplitRect.y = SizeXorY + m_ClientRect.y + It->pWnd->m_WindowRect.cy;
				It->SplitRect.cy = m_SplitBorderSize;
			}
			SizeXorY += m_SplitBorderSize + It->SizeXorY;
			++Index;
			++It;
		}		
		Update((WISP_RECT*)0);
	}
	int CWispSplitWnd::SetSize(int Pos, int SizeXorY)
	{
		if (Pos >= m_WndList.Size())
			return 0;
		m_WndList[Pos]->SizeXorY = SizeXorY;
		return 1;
	}
	int CWispSplitWnd::GetSize(int Pos, int *pSizeXorY)
	{
		if (Pos >= m_WndList.Size())
			return 0;
		*pSizeXorY = m_WndList[Pos]->SizeXorY;
		return 1;
	}

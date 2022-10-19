
#include "wispbase.hpp"
#include "wisptabwnd.hpp"
#include "wispbasewnd.hpp"
#include "wispbutton.hpp"

#define ID_BUTTON 0x80000000

	CWispTabWnd::CWispTabWnd()
	{
		m_ActiveWndIndex = -1;
		m_nHoverIndex = -1;
		m_TabHeight = 18;
		m_TabSpaceHeight = 20;
		m_pActiveWnd = 0;
	}
	CWispTabWnd::~CWispTabWnd()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispTabWnd)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
		WISP_MSG_MAP(WISP_WM_MOUSE_MOVE, OnMouseMove)
		WISP_MSG_MAP(WISP_WM_MOUSE_LEAVE, OnMouseLeave)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP(WISP_WM_GET_FOCUS, OnGetFocus)
		WISP_MSG_MAP_ON_EVENT
		WISP_MSG_MAP(WISP_WM_RECALC_LAYOUT, OnRecalcLayout)
	WISP_MSG_MAP_END(CWispWnd)

	WISP_EVENT_MAP_BEGIN(CWispTabWnd)
		WISP_EVENT_MAP(ID_BUTTON, OnEventButtonClose)
	WISP_EVENT_MAP_END

	bool CWispTabWnd::OnEventButtonClose(WISP_MSG *pMsg)
	{
		WISP_TAB_WND_ITEM *pTabItem = pMsg->hParentWnd->m_pTabWndItem;
		if (!pTabItem)
			return false;
		RemoveTab(pTabItem, pTabItem->pWnd->m_AdvStyle & 1);
		Update((WISP_RECT*)0);
		return true;
	}

	bool CWispTabWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (!pMsg->KeyEvent.bKeyDown)
			return true;

		if (pMsg->KeyEvent.KeyType == VK_LBUTTON)
		{
			int Index = 0;
			TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
			while (It != m_WndList.End())
			{	
				if (PtInRect(&It->TabRC, &pMsg->m_LogicMousePosWnd))
				{
					if (Index != m_ActiveWndIndex)
					{
						SetActiveWnd(Index);
					}
					break;
				}
				++Index;
				++It;
			}
		} else
		if (pMsg->KeyEvent.KeyType == VK_TAB && m_WndList.Size() != 0)
		{
			if (m_pWispBase->m_KeyMap[VK_SHIFT])
				ActivePrevPage();
			else	ActiveNextPage();
		}
		Update((WISP_RECT*)0);
		return true;
	}
	bool CWispTabWnd::OnUpdateClient(WISP_MSG *pMsg)
	{
		if (!m_WndList.Size())
			CWispWnd::OnUpdateClient(pMsg);

		if ( !(m_Style & 0x800000) || m_WndList.Size() != 1)
		{
			switch (m_Style & 0xF0000)
			{
			case 0x10000:
				DrawTabTop();
				break;
			case 0x20000:
				DrawTabButtom();
				break;
			case 0x40000:
				DrawTabLeft();
				break;
			case 0x80000:
				DrawTabRight();
				break;
			}
		}
		return false;
	}
	bool CWispTabWnd::OnDestroy(WISP_MSG *pMsg)
	{
		m_WndList.Clear();
		m_pActiveWnd = 0;
		m_nHoverIndex = -1;
		m_ActiveWndIndex = -1;
		return true;
	}
	bool CWispTabWnd::OnSize(WISP_MSG *pMsg)
	{
		if (m_pActiveWnd)
		{
			switch (m_Style & 0xF0000)
			{
			case 0x10000:
			case 0x20000:
				m_pActiveWnd->Resize(m_ClientRect.cx, m_ClientRect.cy - m_TabSpaceHeight, true);
				break;
			case 0x40000:
			case 0x80000:
				m_pActiveWnd->Resize(m_ClientRect.cx - m_TabSpaceHeight, m_ClientRect.cy, true);
				break;
			}
		}
		return true;
	}
	bool CWispTabWnd::OnRecalcLayout(WISP_MSG *pMsg)
	{
		switch (m_Style & 0xF0000)
		{
		case 0x10000:
			ResizeTabTop();
			break;
		case 0x20000:
			ResizeTabButtom();
			break;
		case 0x40000:
			ResizeTabLeft();
			break;
		case 0x80000:
			ResizeTabRight();
			break;
		}
		return true;
	}
	bool CWispTabWnd::OnGetFocus(WISP_MSG *pMsg)
	{
		m_pWispBase->m_pFocusWnd = m_pActiveWnd;
		return true;
	}
	bool CWispTabWnd::OnMouseMove(WISP_MSG *pMsg)
	{
		int Index = 0;
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			if (PtInRect(&It->TabRC, &pMsg->m_LogicMousePosWnd))
			{
				Update(&m_Rect);
				m_nHoverIndex = Index;
				return false;
			}
			++Index;
			++It;
		}
		return OnMouseLeave(pMsg);
	}
	bool CWispTabWnd::OnMouseLeave(WISP_MSG *pMsg)
	{
		if (m_nHoverIndex != -1)
		{
			Update(&m_Rect);
			m_nHoverIndex = -1;
		}
		return true;
	}
	bool CWispTabWnd::InitWnd()
	{
		if (!CWispWnd::InitWnd())
			return false;
		if (!(m_Style & 0xF0000))
			m_Style |= 0x10000;
		m_FixedTabWidth = 0;
		m_TextSpaceWidth = 0;
		m_MiniCloseDIBList = gpCurWisp->m_DIBLib.LoadDIBList("\\Skin\\Default\\MiniClose.bmp", 10, 10, 0, 0xFF00FF);
		m_ButtonSize.cy = 10;
		m_ButtonSize.cx = 10;
		if ((m_Style & 0x200000))
			m_TextSpaceWidth += 10;
		m_FullWidth = 0;
		m_TextColor = m_pWispBase->m_pWispDrawObj->m_crSystem[79];
		return true;
	}
	void CWispTabWnd::DrawTabTop()
	{
		WISP_RECT Rect;
		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = m_ClientRect.cx;
		Rect.cy = m_TabSpaceHeight;
		m_pWispBase->m_pWispDrawObj->DrawTabBackground(&m_ClientDC, &Rect);
		m_ClientDC.DrawHLine(Rect.x, Rect.x2() - 1, Rect.cy + Rect.y - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[28]);
		int Index = 0;
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			WISP_RECT Rect = It->TabRC;
			m_ClientDC.SetTextColor(m_pWispBase->m_pWispDrawObj->m_crSystem[46]);
			int State = 0;
			if (Index == m_ActiveWndIndex)
				State = 2;
			else
			if (Index == m_nHoverIndex)
				State = 1;
			switch (State)
			{
			case 0: //NORMAL
				m_ClientDC.DrawVLine(Rect.x, Rect.y + 1, Rect.y2() - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				m_ClientDC.DrawVLine(Rect.x2() - 1, Rect.y + 1, Rect.y2() - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				m_ClientDC.DrawHLine(Rect.x + 1, Rect.x2() - 2, Rect.y, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				++Rect.x;
				++Rect.y;
				Rect.cx -= 2;
				Rect.cy -= 2;
				m_ClientDC.DrawFullRect(&Rect, m_pWispBase->m_pWispDrawObj->m_crSystem[39]);
				break;
			case 1: //HOVER
				m_ClientDC.DrawVLine(Rect.x, Rect.y + 3, Rect.y2() - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				m_ClientDC.DrawVLine(Rect.x2() - 1, Rect.y + 3, Rect.y2() - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				m_ClientDC.DrawHLine(Rect.x + 2, Rect.x2() - 3, Rect.y, m_pWispBase->m_pWispDrawObj->m_crSystem[79]);
				m_ClientDC.DrawHLine(Rect.x + 1, Rect.x2() - 2, Rect.y + 1, m_pWispBase->m_pWispDrawObj->m_crSystem[80]);
				m_ClientDC.DrawHLine(Rect.x + 1, Rect.x2() - 2, Rect.y + 2, m_pWispBase->m_pWispDrawObj->m_crSystem[81]);

				Rect.x += 1;
				Rect.cx -= 2;
				Rect.y += 3;
				Rect.cy -= 4;
				m_ClientDC.DrawFullRect(&Rect, m_pWispBase->m_pWispDrawObj->m_crSystem[39]);
				break;
			case 2: //ACTIVE
				m_ClientDC.DrawVLine(Rect.x, Rect.y + 3, Rect.y2() - 4, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				m_ClientDC.DrawVLine(Rect.x2() - 1, Rect.y + 3, Rect.y2() - 4, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				m_ClientDC.DrawHLine(Rect.x + 2, Rect.x2() - 3, Rect.y, m_pWispBase->m_pWispDrawObj->m_crSystem[79]);
				m_ClientDC.DrawHLine(Rect.x + 1, Rect.x2() - 2, Rect.y + 1, m_pWispBase->m_pWispDrawObj->m_crSystem[80]);
				m_ClientDC.DrawHLine(Rect.x + 1, Rect.x2() - 2, Rect.y + 2, m_pWispBase->m_pWispDrawObj->m_crSystem[81]);
				Rect.x += 1;
				Rect.cx -= 2;
				Rect.y += 3;
				Rect.cy -= 3;
				m_ClientDC.DrawFullRect(&Rect, m_pWispBase->m_pWispDrawObj->m_crSystem[38]);
				break;
			}
			if (It->pDIB)
			{
				m_ClientDC.DrawDIB(Rect.x + 2, Rect.y + (Rect.cy - It->pDIB->m_PixelBuf.Height) / 2, It->pDIB);
				Rect.x += It->pDIB->m_PixelBuf.Width;
				Rect.cx -= It->pDIB->m_PixelBuf.Width;
			}
			Rect.x += 4;
			Rect.cx += -4 - m_TextSpaceWidth;
			if (m_pWispBase->m_pFocusWnd &&
				(m_pWispBase->m_pFocusWnd == It->pWnd ||
				m_pWispBase->m_pFocusWnd->IsChildWndOf(It->pWnd)))
			{
				m_ClientDC.SetTextColor(m_TextColor);
				m_ClientDC.DrawShadowText(&Rect, It->Name, 1, -1, 0);
				m_ClientDC.SetTextColor();
			} else
			{
				m_ClientDC.DrawString(&Rect, It->Name, -1, 0);
			}
			++Index;
			++It;
		}
	}
	void CWispTabWnd::DrawTabButtom()
	{
		WISP_RECT Rect;
		Rect.x = 0;
		Rect.y = m_ClientRect.cy - m_TabSpaceHeight;
		Rect.cx = m_ClientRect.cx;
		Rect.cy = m_TabSpaceHeight;
		m_pWispBase->m_pWispDrawObj->DrawTabBackground(&m_ClientDC, &Rect);
		m_ClientDC.DrawHLine(Rect.x, Rect.x2() - 1, Rect.y, m_pWispBase->m_pWispDrawObj->m_crSystem[28]);
		int Index = 0;
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			WISP_RECT Rect = It->TabRC;
			m_ClientDC.SetTextColor(m_pWispBase->m_pWispDrawObj->m_crSystem[46]);
			int State = 0;
			if (Index == m_ActiveWndIndex)
				State = 2;
			else
			if (Index == m_nHoverIndex)
				State = 1;
			switch (State)
			{
			case 0: //NORMAL
				m_ClientDC.DrawVLine(Rect.x, Rect.y, Rect.y2() - 2, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				m_ClientDC.DrawVLine(Rect.x2() - 1, Rect.y, Rect.y2() - 2, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				m_ClientDC.DrawHLine(Rect.x + 1, Rect.x2() - 2, Rect.y2() - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				++Rect.x;
				++Rect.y;
				Rect.cx -= 2;
				Rect.cy -= 2;
				m_ClientDC.DrawFullRect(&Rect, m_pWispBase->m_pWispDrawObj->m_crSystem[39]);
				break;
			case 1: //HOVER
				m_ClientDC.DrawVLine(Rect.x, Rect.y, Rect.y2() - 4, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				m_ClientDC.DrawVLine(Rect.x2() - 1, Rect.y, Rect.y2() - 4, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				m_ClientDC.DrawHLine(Rect.x + 2, Rect.x2() - 3, Rect.y2() - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[79]);
				m_ClientDC.DrawHLine(Rect.x + 1, Rect.x2() - 2, Rect.y2() - 2, m_pWispBase->m_pWispDrawObj->m_crSystem[80]);
				m_ClientDC.DrawHLine(Rect.x + 1, Rect.x2() - 2, Rect.y2() - 3, m_pWispBase->m_pWispDrawObj->m_crSystem[81]);
				++Rect.x;
				++Rect.y;
				Rect.cx -= 2;
				Rect.cy -= 4;
				m_ClientDC.DrawFullRect(&Rect, m_pWispBase->m_pWispDrawObj->m_crSystem[39]);
				break;
			case 2: //ACTIVE
				m_ClientDC.DrawVLine(Rect.x, Rect.y, Rect.y2() - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				m_ClientDC.DrawVLine(Rect.x2() - 1, Rect.y, Rect.y2() - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[35]);
				m_ClientDC.DrawHLine(Rect.x + 2, Rect.x2() - 3, Rect.y2() - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[79]);
				m_ClientDC.DrawHLine(Rect.x + 1, Rect.x2() - 2, Rect.y2() - 2, m_pWispBase->m_pWispDrawObj->m_crSystem[80]);
				m_ClientDC.DrawHLine(Rect.x + 1, Rect.x2() - 2, Rect.y2() - 3, m_pWispBase->m_pWispDrawObj->m_crSystem[81]);
				++Rect.x;
				Rect.cx -= 2;
				Rect.cy -= 3;
				m_ClientDC.DrawFullRect(&Rect, m_pWispBase->m_pWispDrawObj->m_crSystem[38]);
				break;
			}
			if (It->pDIB)
			{
				m_ClientDC.DrawDIB(Rect.x + 2, Rect.y + (Rect.cy - It->pDIB->m_PixelBuf.Height) / 2, It->pDIB);
				Rect.x += It->pDIB->m_PixelBuf.Width;
				Rect.cx -= It->pDIB->m_PixelBuf.Width;
			}
			Rect.x += 4;
			Rect.cx += -4 - m_TextSpaceWidth;
			if (m_pWispBase->m_pFocusWnd &&
				(m_pWispBase->m_pFocusWnd == It->pWnd ||
				m_pWispBase->m_pFocusWnd->IsChildWndOf(It->pWnd)))
			{
				m_ClientDC.SetTextColor(m_TextColor);
				m_ClientDC.DrawShadowText(&Rect, It->Name, 1, -1, 0);
				m_ClientDC.SetTextColor();
			} else
			{
				m_ClientDC.DrawString(&Rect, It->Name, -1, 0);
			}
			++Index;
			++It;
		}
	}
	void CWispTabWnd::DrawTabLeft()
	{	
		//
	}
	void CWispTabWnd::DrawTabRight()
	{
		//
	}

	void CWispTabWnd::ResizeTabTop()
	{
		int Start = 3;
		m_Rect.x = Start;
		m_Rect.y = m_TabSpaceHeight - m_pWispBase->m_Metrics[WISP_SM_TOOLBAR_CY];
		m_Rect.cx = m_ClientRect.cx;
		m_Rect.cy = m_pWispBase->m_Metrics[WISP_SM_TOOLBAR_CY];
		m_FixedTabWidth = 0;
		if (m_Style & 0x100000)
		{
			TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
			while (It != m_WndList.End())
			{	
				int w = m_ClientDC.GetTextExtent(It->Name, -1, 0) + 8;
				if (It->pDIB)
					w += It->pDIB->m_PixelBuf.Width + 4;
				MIN_LIMIT(m_FixedTabWidth, w);
				++It;
			}
		}

		int Index = 0;
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			int w = m_ClientDC.GetTextExtent(It->Name, -1, 0) + 8 + m_TextSpaceWidth;
			if (It->pDIB)
				w += It->pDIB->m_PixelBuf.Width + 4;
			MIN_LIMIT(w, m_FixedTabWidth);
			
			It->TabRC.x = Start;
			It->TabRC.y = m_Rect.y;
			It->TabRC.cx = w;
			It->TabRC.cy = m_TabHeight;
			if (Index == m_ActiveWndIndex)
			{
				It->TabRC.x -= 2;
				It->TabRC.y -= 2;
				It->TabRC.cx += 4;
				It->TabRC.cy += 2;
			}
			if (m_Style & 0x200000)
				It->pButton->MoveToClient(
					It->TabRC.cx - It->pButton->m_WindowRect.cx + It->TabRC.x - 2,
					It->TabRC.y + (m_Rect.cy - It->pButton->m_WindowRect.cy)/2, true);
			Start += w + 1;
			++Index;
			++It;
		}
		m_FullWidth = Start;
	}
	void CWispTabWnd::ResizeTabButtom()
	{
		int Start = 3;
		m_Rect.x = Start;
		m_Rect.y = m_ClientRect.cy - m_TabSpaceHeight;
		m_Rect.cx = m_ClientRect.cx;
		m_Rect.cy = m_pWispBase->m_Metrics[WISP_SM_TOOLBAR_CY];
		m_FixedTabWidth = 0;
		if (m_Style & 0x100000)
		{
			TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
			while (It != m_WndList.End())
			{	
				int w = m_ClientDC.GetTextExtent(It->Name, -1, 0) + 8;
				if (It->pDIB)
					w += It->pDIB->m_PixelBuf.Width + 4;
				MIN_LIMIT(m_FixedTabWidth, w);
				++It;
			}
		}

		int Index = 0;
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			int w = m_ClientDC.GetTextExtent(It->Name, -1, 0) + 8 + m_TextSpaceWidth;
			if (It->pDIB)
				w += It->pDIB->m_PixelBuf.Width + 4;
			MIN_LIMIT(w, m_FixedTabWidth);
			
			It->TabRC.x = Start;
			It->TabRC.y = m_Rect.y;
			It->TabRC.cx = w;
			It->TabRC.cy = m_TabHeight;
			if (Index == m_ActiveWndIndex)
			{
				It->TabRC.x -= 2;

				It->TabRC.cx += 4;
				It->TabRC.cy += 2;
			}
			if (m_Style & 0x200000)
				It->pButton->MoveToClient(
					It->TabRC.cx - It->pButton->m_WindowRect.cx + It->TabRC.x - 2,
					It->TabRC.y + 3, true);
			Start += w + 1;
			++Index;
			++It;
		}
		m_FullWidth = Start;
	}
	void CWispTabWnd::ResizeTabLeft()
	{
		//
	}
	void CWispTabWnd::ResizeTabRight()
	{
		//
	}

	unsigned int CWispTabWnd::InsertWnd(const WCHAR *Name, CWispWnd *pWnd, CWispDIB *pDIB)
	{
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Append();
		InsertTab(&*It, Name, pWnd, pDIB);
		return m_WndList.Size()-1;
	}
	unsigned int CWispTabWnd::InsertWndAfter(int Index, const WCHAR *Name, CWispWnd *pWnd, CWispDIB *pDIB)
	{
		if (Index >= m_WndList.Size())
			return InsertWnd(Name, pWnd, pDIB);
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.InsertAfter(m_WndList[Index]);
		InsertTab(&*It, Name, pWnd, pDIB);
		return m_WndList.Size()-1;
	}
	unsigned int CWispTabWnd::InsertWndBefore(int Index, const WCHAR *Name, CWispWnd *pWnd, CWispDIB *pDIB)
	{
		if (Index >= m_WndList.Size())
			return InsertWnd(Name, pWnd, pDIB);
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.InsertBefore(m_WndList[Index]);
		InsertTab(&*It, Name, pWnd, pDIB);
		return m_WndList.Size()-1;
	}
	unsigned int CWispTabWnd::RemoveWnd(CWispWnd *pWnd, bool bDestroy)
	{
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			if (It->pWnd == pWnd)
			{
				RemoveTab(&*It, bDestroy);
				break;
			}
			++It;
		}
		return m_WndList.Size();
	}
	unsigned int CWispTabWnd::RemoveWnd(int Index, bool bDestroy)
	{
		if (Index < m_WndList.Size())
			RemoveTab(&*m_WndList[Index], bDestroy);
		return m_WndList.Size();
	}
	CWispWnd *CWispTabWnd::GetActiveWnd()
	{
		return m_pActiveWnd;
	}
	unsigned int CWispTabWnd::GetActiveWndIndex()
	{
		return m_ActiveWndIndex;
	}
	CWispWnd *CWispTabWnd::SetActiveWnd(unsigned int Index)
	{
		if (m_ActiveWndIndex == Index)
		{
			m_pActiveWnd->Focus();
			return m_pActiveWnd;
		}
	
		if (Index >= m_WndList.Size())
			return nullptr;

		TListIter<WISP_TAB_WND_ITEM> It = m_WndList[Index];
		if (It == m_WndList.End())
			return nullptr;
		ChangeActiveWnd(It->pWnd);
		return m_pActiveWnd;
	}
	bool CWispTabWnd::SetTabText(unsigned int Index, const WCHAR *pStr)
	{
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList[Index];
		if (It == m_WndList.End())
			return false;
		TStrOmit(pStr, It->Name, 32);
		Update((WISP_RECT*)0);
		return true;
	}
	void CWispTabWnd::ChangeActiveWnd(CWispWnd *pActiveWindow)
	{
		int Index = 0;
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			if (It->pWnd == pActiveWindow)
				break;
			++Index;
			++It;
		}
		if (Index < m_WndList.Size())
		{
			OnActiveWnd(pActiveWindow);
			if (SendCommand(0x80000800, Index, pActiveWindow)) //WISP_ID_TAB_SELECT
			{
				if (m_pActiveWnd)
					m_pActiveWnd->Show(WISP_SH_HIDDEN);
				m_pActiveWnd = pActiveWindow;
				switch (m_Style & 0xF0000)
				{
				case 0x10000:
					pActiveWindow->MoveToClient(0,m_TabSpaceHeight, true);
					m_pActiveWnd->Resize(m_ClientRect.cx, m_ClientRect.cy - m_TabSpaceHeight, true);
					break;
				case 0x20000:
					pActiveWindow->MoveToClient(0,0,true);
					m_pActiveWnd->Resize(m_ClientRect.cx, m_ClientRect.cy - m_TabSpaceHeight, true);
					break;
				case 0x40000:
					pActiveWindow->MoveToClient(m_TabSpaceHeight,0,true);
					m_pActiveWnd->Resize(m_ClientRect.cx - m_TabSpaceHeight, m_ClientRect.cy, true);
					break;
				case 0x80000:
					pActiveWindow->MoveToClient(0,0,true);
					m_pActiveWnd->Resize(m_ClientRect.cx - m_TabSpaceHeight, m_ClientRect.cy, true);
					break;
				}
				m_pActiveWnd->Focus();
				m_pActiveWnd->Show(WISP_SH_MINSIZE);
				m_ActiveWndIndex = Index;
				RecalcLayout();
				Update((WISP_RECT*)0);
			}
		}
	}
	void CWispTabWnd::ActiveNextPage()
	{
		int Index = m_ActiveWndIndex + 1;
		if (Index >= m_WndList.Size())
			Index = 0;
		SetActiveWnd(Index);
	}
	void CWispTabWnd::ActivePrevPage()
	{
		int Index = m_ActiveWndIndex - 1;
		if (Index < 0)
			Index = m_WndList.Size() - 1;
		SetActiveWnd(Index);
	}
	unsigned int CWispTabWnd::GetWndCount()
	{
		return m_WndList.Size();
	}

	CWispWnd *CWispTabWnd::GetWnd(int Index)
	{
		if (Index < m_WndList.Size())
			return m_WndList[Index]->pWnd;
		return nullptr;
	}
	CWispButton *CWispTabWnd::GetButton(int Index)
	{
		if (Index < m_WndList.Size())
			return m_WndList[Index]->pButton;
		return nullptr;
	}

	//+0x38
	void CWispTabWnd::InsertTab(WISP_TAB_WND_ITEM *pTabItem, const WCHAR *pString, CWispWnd *pWnd, CWispDIB *pDIB)
	{
		if (m_Style & 0x800000 && m_WndList.Size() == 1)
			m_TabSpaceHeight = 0;
		else	m_TabSpaceHeight = m_TabHeight + 2;

		pTabItem->pDIB = pDIB;
		pTabItem->pWnd = pWnd;

		if (pString)
			TStrCpy(pTabItem->Name, pString);

		if (m_Style & 0x200000)
		{
			pTabItem->pButton = new CWispButton;
			pTabItem->pButton->CreateEx(m_MiniCloseDIBList, 0, 0, this, ID_BUTTON, 0x800000);
			pTabItem->pButton->m_AdvStyle |= 1;
			pTabItem->pButton->m_pTabWndItem = pTabItem;
		} else
		{
			pTabItem->pButton = 0;
		}

		RecalcLayout();
		if (m_WndList.Size() == 1)
			SetActiveWnd(0);
		else	pTabItem->pWnd->Show(WISP_SH_HIDDEN);

		if ((m_Style & 0x200000) && (m_Style & 0x400000))
		{
			if (m_WndList.Size() == 1)
				m_WndList[0]->pButton->EnableWindow(0);

			if (m_WndList.Size() == 2)
				m_WndList[0]->pButton->EnableWindow(1);
		}		
	}
	//+0x3C
	void CWispTabWnd::RemoveTab(WISP_TAB_WND_ITEM *pTabItem, bool bDestroy)
	{
		if (OnRemoveTab(pTabItem->pWnd))
		{
			if (pTabItem->pButton)
				pTabItem->pButton->Destroy();
			if (bDestroy)
				pTabItem->pWnd->Destroy();
			TListIter<WISP_TAB_WND_ITEM> It = pTabItem;
			if (pTabItem->pWnd == m_pActiveWnd)
			{
				m_WndList.Remove(It);
				m_ActiveWndIndex = -1;
				m_pActiveWnd = 0;
				OnRemoved();
				if (m_WndList.Size())
					SetActiveWnd(m_WndList.Size()-1);
			} else
			{
				m_WndList.Remove(It);
				OnRemoved();
			}

			if ((m_Style & 0x200000) && (m_Style & 0x400000) && m_WndList.Size() == 1)
			{
				GetButton(0)->EnableWindow(0);
			}

			if ((m_Style & 0x800000) && m_WndList.Size() == 1)
			{
				m_TabSpaceHeight = 0;
				m_ActiveWndIndex = -1;
				SetActiveWnd(0);
			}
			RecalcLayout();
			Update(&m_Rect);
		}
	}

	//+0x40
	//bool CWispTabWnd::OnRemoveTab(CWispWnd *pWnd) { return true; }
	//+0x44
	//void CWispTabWnd::OnRemoved() { }
	//+0x48
	//void CWispTabWnd::OnActiveWnd(CWispWnd *pWnd) { }
	
	void CWispTabWnd::RemoveAllTab(bool bDestroy)
	{
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			if (It->pButton)
				It->pButton->Destroy();
			if (bDestroy)
				It->pWnd->Destroy();
			++It;
		}
		m_WndList.Clear();
		m_pActiveWnd = 0;
		m_nHoverIndex = -1;
		m_ActiveWndIndex = -1;
	}

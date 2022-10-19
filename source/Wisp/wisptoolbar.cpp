
#include "wisptoolbar.hpp"
#include "wispbase.hpp"
#include "wispstatic.hpp"
#include "wispbutton.hpp"
#include "wispedit.hpp"

	CWispToolbar::CWispToolbar()
	{
		m_CtrlType = WISP_CTRL_TOOL_BAR;
		m_pDIBList = 0;
		m_Margin = 3;
		m_SeparatorWidth = 2;
	}
	CWispToolbar::~CWispToolbar()
	{
		//EMPTY
	}

	void CWispToolbar::SetOwner(CWispBaseWnd *pOwnerWnd)
	{
		TListIter<WISP_TOOLBAR_ITEM> It = m_BTList.Begin();
		while (It != m_BTList.End())
		{
			if (It->pItem)
				It->pItem->SetOwner(pOwnerWnd);
			++It;
		}
		CWispBaseWnd::SetOwner(pOwnerWnd);
	}

	WISP_MSG_MAP_BEGIN(CWispToolbar)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
		WISP_MSG_MAP(WISP_WM_RECALC_LAYOUT, OnRecalcLayout)
		WISP_MSG_MAP(WISP_WM_RESIZE_CHILD, OnResizeChild)
	WISP_MSG_MAP_END(CWispWnd)

	bool CWispToolbar::OnDestroy(WISP_MSG *pMsg)
	{
		m_BTList.Clear();
		return true;
	}
	bool CWispToolbar::OnUpdateClient(WISP_MSG *pMsg)
	{
		if ( !(m_Style & 0x40))
			m_pWispBase->m_pWispDrawObj->DrawToolbarBK(&m_ClientDC, &m_ClientRect);

		int x = m_Margin;
		TListIter<WISP_TOOLBAR_ITEM> It = m_BTList.Begin();
		while (It != m_BTList.End())
		{
			if (It->pItem)
			{
				x += m_Margin + It->pItem->m_WindowRect.cx;
			} else
			{
				WISP_RECT rc;
				rc.x = x + m_SeparatorWidth / 2;
				rc.y = m_Margin;
				rc.cx = m_SeparatorWidth;
				rc.cy = m_ClientRect.cy - m_Margin*2;
				m_ClientDC.DrawFrameRect(&rc, 0, 1);
				x += m_Margin + m_SeparatorWidth;
			}
			++It;
		}
		m_ClientDC.DrawHLine(0, m_ClientRect.cx - 1, m_ClientRect.cy - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[28]);
		return false;
	}
	bool CWispToolbar::OnRecalcLayout(WISP_MSG *pMsg)
	{
		int x = m_Margin;
		int y = m_ClientRect.cx - x;
		TListIter<WISP_TOOLBAR_ITEM> It = m_BTList.Begin();
		while (It != m_BTList.End())
		{
			if (It->pItem)
			{
				int cy = m_WindowRect.cy - It->pItem->m_WindowRect.cy;
				if (It->Type & 0x20000) //WISP_TOOLBAR_ALIGN
				{
					int cx = y - It->pItem->m_WindowRect.cx;
					It->pItem->MoveToClient(cx, cy / 2, false);
					//It->pItem->Show(2 * (cx < x) + 1); //???
					if (x > cx)
						It->pItem->Show(WISP_SH_HIDDEN);
					else	It->pItem->Show(WISP_SH_MINSIZE);
					y = cx - m_Margin;
				} else
				{
					It->pItem->MoveToClient(x, cy / 2, true);
					x += m_Margin + It->pItem->m_WindowRect.cx;
				}
			} else
			{
				x += m_Margin + m_SeparatorWidth;
			}
			MIN_LIMIT(y, x);
			++It;
		}
		return true;
	}
	bool CWispToolbar::OnResizeChild(WISP_MSG *pMsg)
	{
		if (!(m_Style & 0x10000))
			CWispBaseWnd::Resize(((CWispWnd*)m_ParentWnd)->m_ClientRect.cx, m_WindowRect.cy, true);
		return true;
	}
	bool CWispToolbar::CreateEx(CWispWnd *pParentWnd, int y, int cy, unsigned int CmdID, unsigned int Style)
	{
		m_pDIBList = 0;

		if (cy == -1) cy = gpCurWisp->m_Metrics[WISP_SM_TOOLBAR_CY]+m_Margin*2;
		return CreateWnd(0, 0,y, pParentWnd->m_ClientRect.cx, cy, pParentWnd, CmdID, Style, WISP_SH_MINSIZE);
	}
	CWispBaseWnd *CWispToolbar::GetItem(unsigned int CmdID)
	{
		TListIter<WISP_TOOLBAR_ITEM> It = m_BTList.Begin();
		while (It != m_BTList.End())
		{
			if (It->pItem &&
				It->pItem->m_CmdID == CmdID)
				return It->pItem;
			++It;
		}
		return nullptr;
	}
	bool CWispToolbar::LoadToolbar(CWispDIBList *pDIBList, WISP_TOOLBAR_RES_ITEM *pResItem)
	{
		m_pDIBList = pDIBList;

		while (pResItem->CmdID || pResItem->Type)
		{
			if (pResItem->Type & WISP_TOOLBAR_SEPARATOR)
				InsertSeparator(-1, pResItem->Type);
			else
			if (pResItem->Type & WISP_TOOLBAR_EDIT)
				InsertEdit(-1, pResItem->Type, pResItem->Width, pResItem->CmdID,
					pResItem->Style, pResItem->pString);
			else
			if (pResItem->Type & WISP_TOOLBAR_DIB)
				InsertStaticDIB(-1, pResItem->Type, pResItem->CmdID,
					pDIBList->GetDIB(pResItem->DIBIndex));
			else
			if (pResItem->Type & WISP_TOOLBAR_STR)
				InsertStaticStr(-1, pResItem->Type, pResItem->CmdID,
					pResItem->pString);
			else
				InsertButton(-1, pResItem->Type, pResItem->CmdID,
					pResItem->Style, pDIBList->GetDIB(pResItem->DIBIndex), pResItem->pString);
			++pResItem;
		}
		if (m_WindowRect.cy == 0)
			Resize(m_WindowRect.cx, pDIBList->m_DIBList->m_PaintRect.cy, true);
		RecalcLayout();
		return true;
	}
	bool CWispToolbar::InsertButton(int Pos, unsigned int Type, unsigned int CmdID, unsigned int Style, CWispDIB *pDIB, const WCHAR *HelpString)
	{
		CWispButton *pButton = new CWispButton;
		pButton->CreateEx(pDIB, 0, 0, this, CmdID, Style | 0x1000040);
		pButton->Resize(pDIB->m_PixelBuf.Width + 4, pDIB->m_PixelBuf.Height + 4, true);
		pButton->SetOwner(m_OwnerWnd);
		pButton->EnableHelpTip(HelpString);
		pButton->m_AdvStyle |= 1;

		TListIter<WISP_TOOLBAR_ITEM> It;
		if (Pos < 0)
			It = m_BTList.Append();
		else	It = m_BTList.InsertBefore(m_BTList[Pos]);
		It->Type = Type;
		It->pItem = pButton;
		It->pDIB = pDIB;
		RecalcLayout();
		return true;
	}
	bool CWispToolbar::InsertEdit(int Pos, unsigned int Type, int Width, unsigned int CmdID, unsigned int Style, const WCHAR *HelpString)
	{
		CWispEdit *pEdit = new CWispEdit;
		pEdit->CreateWnd(0, 0,0,Width,m_ClientDC.m_pFont->m_Height + 2 * m_Margin, this, CmdID, Style, WISP_SH_MINSIZE);
		pEdit->SetOwner(m_OwnerWnd);
		pEdit->EnableHelpTip(HelpString);
		pEdit->m_AdvStyle |= 1;

		TListIter<WISP_TOOLBAR_ITEM> It;
		if (Pos < 0)
			It = m_BTList.Append();
		else	It = m_BTList.InsertBefore(m_BTList[Pos]);
		It->Type = Type;
		It->pItem = pEdit;
		It->pDIB = 0;
		RecalcLayout();
		return true;
	}
	bool CWispToolbar::InsertStaticDIB(int Pos, unsigned int Type, unsigned int CmdID, CWispDIB *pDIB)
	{
		CWispStaticDIB *pStaticDIB = new CWispStaticDIB;
		pStaticDIB->CreateEx(pDIB, 0, 0, this, CmdID);
		pStaticDIB->m_AdvStyle |= 1;

		TListIter<WISP_TOOLBAR_ITEM> It;
		if (Pos < 0)
			It = m_BTList.Append();
		else	It = m_BTList.InsertBefore(m_BTList[Pos]);
		It->Type = Type;
		It->pItem = pStaticDIB;
		It->pDIB = pDIB;
		RecalcLayout();
		return true;
	}
	bool CWispToolbar::InsertStaticStr(int Pos, unsigned int Type, unsigned int CmdID, const WCHAR *HelpString)
	{
		CWispStaticStr *pStaticStr = new CWispStaticStr;
		pStaticStr->Create(HelpString, 0,0,0,0, this, CmdID, 0xBB, 1);
		pStaticStr->m_AdvStyle |= 1;

		TListIter<WISP_TOOLBAR_ITEM> It;
		if (Pos < 0)
			It = m_BTList.Append();
		else	It = m_BTList.InsertBefore(m_BTList[Pos]);
		It->Type = Type;
		It->pItem = pStaticStr;
		It->pDIB = 0;
		int y = pStaticStr->m_WindowDC.m_pFont->m_Height;
		int x = m_WindowDC.GetTextExtent(HelpString, -1, 0);
		pStaticStr->Resize(x, y, true);
		RecalcLayout();
		return true;
	}
	bool CWispToolbar::InsertSeparator(int Pos, unsigned int Type)
	{
		TListIter<WISP_TOOLBAR_ITEM> It;
		if (Pos < 0)
			It = m_BTList.Append();
		else	It = m_BTList.InsertBefore(m_BTList[Pos]);
		It->Type = Type;
		It->pItem = 0;
		It->pDIB = 0;
		RecalcLayout();
		Update((WISP_RECT*)0);
		return true;
	}
	bool CWispToolbar::RemoveItem(int Pos) //???
	{
		return false; //???
	}
	bool CWispToolbar::Enable(unsigned int nID, bool bEnable)
	{
		CWispBaseWnd *pWnd = GetItem(nID);
		if (!pWnd)
			return false;
		pWnd->EnableWindow(bEnable);
		return true;
	}

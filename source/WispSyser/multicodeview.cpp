
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "multicodeview.hpp"
#include "resource.hpp"

	WISP_MSG_MAP_BEGIN(CMultiCodeView)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
	WISP_MSG_MAP_END(CWispMultiTabView)

	CMultiCodeView::CMultiCodeView()
	{
	}
	CMultiCodeView::~CMultiCodeView()
	{
	}

	bool CMultiCodeView::OnCreate(WISP_MSG *pMsg)
	{
		InsertView(0);
		SetOwner(this);
		return true;
	}

	void CMultiCodeView::OnActiveWnd(CWispWnd *pWnd)
	{
		gpSyser->m_MainFrame.m_SystemExplorer.m_pCodeView = (CCodeView*)pWnd;
		gpSyser->m_MainFrame.m_SystemExplorer.UpdateMenu();
	}

	CCodeView *CMultiCodeView::InsertView(CCodeView *pSrcView)
	{
		if (m_WndList.Size() >= 9) return nullptr;
		CCodeView *pNewView = new CCodeView;
		pNewView->CreateWnd(0, 0, 0, 0, 0, this, m_WndList.Size() + CMD_ID_CV0, 0x41010000, WISP_SH_MINSIZE);
		pNewView->m_AdvStyle |= 1;
		pNewView->m_pSyserCodeDoc = &gpSyser->m_SyserUI.m_SyserCodeDoc;
		InsertWnd(WSTR("" UNREADSTR ""), pNewView, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x7E));
		SetActiveWnd(m_WndList.Size() - 1);
		if (pSrcView)
			pNewView->GotoAddr(pSrcView->m_CurAddr, 0);
		return pNewView;
	}

	void CMultiCodeView::UpdateContext()
	{
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
		        static_cast<CCodeView*>(It->pWnd)->UpdateDasm();
		        ++It;
		}
	}
	void CMultiCodeView::ResetContext()
	{
		RemoveAllTab(true);
		InsertView(0);
	}

	ULONG_PTR CMultiCodeView::GetActiveAddress()
	{
		CCodeView *pWnd = (CCodeView *)GetActiveWnd();
		if (pWnd) return pWnd->m_CurAddr;
		return 0;
	}

	void CMultiCodeView::SetActiveAddress(ULONG_PTR Address, bool bHistory)
	{
		CCodeView *pWnd = (CCodeView *)GetActiveWnd();
		if (pWnd) pWnd->GotoAddr(Address, bHistory);
	}

	void CMultiCodeView::ClearBM()
	{
		m_Bookmark.clear();
		UpdateContext();
		gpSyser->m_MainFrame.m_SystemExplorer.UpdateMenu();
	}
	bool CMultiCodeView::IsBM(ULONG_PTR Address)
	{
		return m_Bookmark.find(Address) != m_Bookmark.end();
	}
	void CMultiCodeView::ToggleBM(ULONG_PTR Address)
	{
		map<ULONG_PTR,ULONG_PTR>::IT it = m_Bookmark.find(Address);
		if (it == m_Bookmark.end())
			m_Bookmark.insert(_Tmap_pair<ULONG_PTR, ULONG_PTR>(Address, (ULONG_PTR*)0));
		else	m_Bookmark.erase(it);
		UpdateContext();
	}
	void CMultiCodeView::ToggleBM()
	{
		CCodeView *pWnd = (CCodeView *)GetActiveWnd();
		if (pWnd)
		{
			CListStringItem *Item = pWnd->FindItem(0, 8);
			if (Item)
			{
				ULONG_PTR Address = pWnd->GetItemValue(Item, CV_ADDR);
				ToggleBM(Address);
				gpSyser->m_MainFrame.m_SystemExplorer.UpdateMenu();
			}
		}
	}
	void CMultiCodeView::NextBM()
	{
		CCodeView *pWnd = (CCodeView *)GetActiveWnd();
		if (pWnd)
		{
			map<ULONG_PTR,ULONG_PTR>::IT it = m_Bookmark.find_near(pWnd->m_CurAddr);
			if (it) it++;
			if (!it) it = m_Bookmark.begin();
			if (it)
			gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(it->first, false);
			
		}
	}
	void CMultiCodeView::PrevBM()
	{
		CCodeView *pWnd = (CCodeView *)GetActiveWnd();
		if (pWnd)
		{
			map<ULONG_PTR,ULONG_PTR>::IT it = m_Bookmark.find_near(pWnd->m_CurAddr);
			if (it && it->first == pWnd->m_CurAddr)
				--it;
			if (!it) it = m_Bookmark.rbegin();
			if (it)
			gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(it->first, false);
		}
	}

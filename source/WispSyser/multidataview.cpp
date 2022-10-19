
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "multidataview.hpp"
#include "../Wisp/wispsplitwnd.hpp"
#include "resource.hpp"

	WISP_MSG_MAP_BEGIN(CMultiDataView)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_COMMAND, CWispWnd::OnCommand)
//		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispMultiTabView)


	WISP_EVENT_MAP_BEGIN(CMultiDataView)
		WISP_EVENT_MAP(0x40000000, OnEventFullScr)
		WISP_EVENT_MAP(0x40000001, OnEventNewView)
//		WISP_EVENT_MAP(0x40000002, OnEventRemoveView)
	WISP_EVENT_MAP_END

//	WISP_COMMAND_MAP_BEGIN(CMultiDataView)
//		case default: return CWispWnd::OnCommand(pMsg);
//	WISP_COMMAND_MAP_END

	CMultiDataView::CMultiDataView()
	{
	}

	CMultiDataView::~CMultiDataView()
	{
	}

	bool CMultiDataView::OnCreate(WISP_MSG *pMsg)
	{
		InsertView(0);
		return true;
	}

	bool CMultiDataView::OnEventFullScr(WISP_MSG *pMsg)
	{
		if (m_WindowRect.cy <= static_cast<CWispSplitWnd*>(m_ParentWnd)->m_WindowRect.cy - 20)
			static_cast<CWispSplitWnd*>(m_ParentWnd)->AdjustWnd(0);
		else
			gpSyser->m_MainFrame.m_SystemExplorer.AdjustSplit();
		return true;
	}

	bool CMultiDataView::OnEventNewView(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000002)
		{
			InsertView((CDataView *)GetActiveWnd());
		}
		return true;
	}

	CDataView *CMultiDataView::InsertView(CDataView *pSrcView)
	{
		if (m_WndList.Size() >= 9) return nullptr;
		CDataView *pNewView = new CDataView;
		WISP_RECT Rect;
		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		pNewView->CreateWnd(0, Rect, this, m_WndList.Size() + CMD_ID_DV0, 0, WISP_SH_MINSIZE);
		pNewView->m_AdvStyle |= 1;
		InsertWnd(WSTR("" UNREADSTR ""), pNewView, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x7F));
		SetActiveWnd(m_WndList.Size() - 1);
		SetActiveAddress(pSrcView ? pSrcView->m_CurAddr:0, 0);
		if (pSrcView)
			pNewView->m_ShowLength = pSrcView->m_ShowLength;
		return pNewView;
	}

	void CMultiDataView::UpdateContext()
	{
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			It->pWnd->Update((WISP_RECT*)0);
			++It;
		}
	}

	void CMultiDataView::DataWidthUpdate()
	{
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			static_cast<CDataView*>(It->pWnd)->DataWidthUpdate();
			++It;
		}
	}

	void CMultiDataView::ResetContext()
	{
		RemoveAllTab(true);
		InsertView(0);
	}

	int CMultiDataView::GetActiveDataWidth()
	{
		CDataView *pWnd = (CDataView *)GetActiveWnd();
		if (pWnd) return pWnd->GetDataWidth();
		return 1;
	}

	void CMultiDataView::SetActiveDataWidth(int Width)
	{
		CDataView *pWnd = (CDataView *)GetActiveWnd();
		if (pWnd) pWnd->SetDataWidth(Width);
	}

	ULONG_PTR CMultiDataView::GetActiveAddress()
	{
		CDataView *pWnd = (CDataView *)GetActiveWnd();
		if (pWnd) return pWnd->m_CurAddr;
		return 0;
	}
	void CMultiDataView::SetActiveAddress(ULONG_PTR Address, const WCHAR *pStrAddress)
	{
		CDataView *pWnd = (CDataView *)GetActiveWnd();
		if (pWnd)
		{
			pWnd->ViewAddress(Address, true);
			if (pStrAddress)
				pWnd->SetFloatAddress(pStrAddress);
		}
	}

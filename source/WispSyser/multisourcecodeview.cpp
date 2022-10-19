
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "multisourcecodeview.hpp"
#include "syserconfig.hpp"

	WISP_MSG_MAP_BEGIN(CMultiSourceCodeView)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispMultiTabView)

	WISP_CMD_MAP_BEGIN(CMultiSourceCodeView)
	WISP_CMD_MAP_END

	WISP_EVENT_MAP_BEGIN(CMultiSourceCodeView)
	WISP_EVENT_MAP_END

	bool CMultiSourceCodeView::OnCreate(WISP_MSG *pMsg)
	{
		SetBGColor(gSyserColors.Color[0]);
		return true;
	}

	bool CMultiSourceCodeView::OnRemoveTab(CWispWnd *pWnd)
	{
		CSourceCodeWnd *pSrcWnd = (CSourceCodeWnd *)pWnd;
		SCWKEY Key;
		Key.FileID = pSrcWnd->m_CurrentFileID;
		Key.pSDSMod = pSrcWnd->m_pSDSMod;
		map<SCWKEY, CSourceCodeWnd*>::IT it = m_SourceCodeWndMap.find(Key);
		if (it != m_SourceCodeWndMap.end())
			m_SourceCodeWndMap.erase(Key);
		CListStringItem *Item = gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_SDSModuleList.FullFindItem(0, 8);
		if (Item && gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_SDSModuleList.GetItemLevel(Item) == 1 &&
			it->first.pSDSMod == (CSDSModule *)gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_SDSModuleList.GetItemValue(Item, 0))
		{
			gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_SDSModuleList.Unselect(Item);
		}
		return true;
	}
	void CMultiSourceCodeView::OnRemoved()
	{
		if (!m_WndList.Size())
			gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateMenu();
	}

	void CMultiSourceCodeView::OnActiveWnd(CWispWnd *pWnd)
	{
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateMenu();
	}

	void CMultiSourceCodeView::UpdateContext()
	{
		TListIter<WISP_TAB_WND_ITEM> It = m_WndList.Begin();
		while (It != m_WndList.End())
		{
			static_cast<CSourceCodeWnd*>(It->pWnd)->UpdateContextSrc();
			static_cast<CSourceCodeWnd*>(It->pWnd)->UpdateContextPics();
			++It;
		}
	}

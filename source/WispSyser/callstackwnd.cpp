
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "callstackwnd.hpp"
#include "syserconfig.hpp"

	CCallStackWnd::CCallStackWnd()
	{
	}
	CCallStackWnd::~CCallStackWnd()
	{
	}

	WISP_MSG_MAP_BEGIN(CCallStackWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_CLOSE, OnClose)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)


	WISP_EVENT_MAP_BEGIN(CCallStackWnd)
//		WISP_EVENT_MAP(..., OnEvent...)
	WISP_EVENT_MAP_END


	bool CCallStackWnd::OnCreate(WISP_MSG *pMsg)
	{
		AttachTitleDIB(gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x12));
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		m_ColorFocus = gSyserColors.Color[2];
		InsertColumn(WSTR("Call Return"), 100, 0, 0, -1);
		InsertColumn(WSTR("Function"), 200, 0, 0, -1);
		m_Style |= 0x3000000;
		return true;
	}
	bool CCallStackWnd::OnClose(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.FocusActive();
		return true;
	}

	void CCallStackWnd::Create()
	{
		if (IsWindow())
		{
			Destroy();
		} else
		{
			CreateWnd(WSTR("Call Stack"), 0,0,400,200, 0, 0xC, 0x100BB, WISP_SH_MINSIZE);
			Center(0);
			UpdateContext();
		}
	}

	void CCallStackWnd::OnMouseDblClick(CListStringItem *Item, int nCol)
	{
		SetLineItem(Item);
		CCallStack::Frame *pFrame = (CCallStack::Frame *)GetItemValue(m_pItem, 0);
		ULONG_PTR Addr = pFrame->Return;

		CSDSModule *pSDSMod = gpSyser->m_SyserUI.FindSDSModule(Addr);
		if (pSDSMod)	
		{
			TList<FilesLN*> ListFilesLN;
			gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_SDSModuleList.ExpandSDSModule(pSDSMod);
			if (pSDSMod->GetFilesLN(Addr - pSDSMod->m_pDbgModule->m_ImageBase, &ListFilesLN))
			{
				gpSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(SRC_TAB); //SRC_TAB(2)
				FilesLN *fln = *ListFilesLN[0];
				if (gpSyser->m_MainFrame.m_SourceDebugFrameWnd.OpenSrcView(pSDSMod,
					fln->FileID,
					fln->lineNumber))
				{
					return;
				}
			}
		}
		gpSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(CV_TAB); //CV_TAB(0)
		gpSyser->m_MainFrame.m_SystemExplorer.SelectCodeAddress(Addr);
	}

	void CCallStackWnd::SetLineItem(CListStringItem *Item)
	{
		if (m_pItem != Item)
		{
			WCHAR Text[128];

			if (m_pItem != 0)
			{
				SetItemColor(m_pItem, -1, gSyserColors.Color[1]);
				CCallStack::Frame *pFrame = (CCallStack::Frame *)GetItemValue(m_pItem, 0);
				TSPrintf(Text, WSTR("  " F0ADDR "X"), pFrame->Return);
				SetItemText(m_pItem, 0, Text);
			}
			m_pItem = Item;
			SetItemColor(Item, -1, gSyserColors.Color[5]);
			CCallStack::Frame *pFrame = (CCallStack::Frame *)GetItemValue(m_pItem, 0);
			TSPrintf(Text, WSTR("->" F0ADDR "X"), pFrame->Return);
			SetItemText(Item, 0, Text);
		}
	}

	void CCallStackWnd::UpdateContext()
	{
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && IsWindowShow())
		{
			m_pItem = 0;
			DeleteAllChildrenItems(0);
			TListIter<CCallStack::Frame> It = gpSyser->m_SyserUI.m_CallStack.m_Frames.Begin();
			while (It != gpSyser->m_SyserUI.m_CallStack.m_Frames.End())
			{
				WCHAR szReturn[128];
				TSPrintf(szReturn, WSTR("  " F0ADDR "X"), It->Return);
				CListStringItem *Item = InsertItem(szReturn, 0, 0, 0, -1);
				SetItemText(Item, 1, It->FuncName.operator const char*());
				SetItemValue(Item, 0, (unsigned long long)&*It);

				if (gpSyser->m_SyserUI.m_CallStack.m_HeadIt == It)
					SetLineItem(Item);
				++It;
			}
		}
	}

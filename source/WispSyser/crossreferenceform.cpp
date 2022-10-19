
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "crossreferenceform.hpp"
#include "syserconfig.hpp"

WISP_FORM_RES_ITEM CrossReferenceForm[] =
{
	{WISP_CTRL_FORM, {0, 0, 400, 240}, 0, 0xB, WSTR("xrefs to ..."), (void*)"\\SyserIcon\\Syser.ico", 0},
	{WISP_CTRL_LIST, {5, 5, 380, 160}, 1, 0x40010006, 0, 0, 0},
	{WISP_CTRL_BUTTON, {80, 180, 70, 20}, 2, 0, WSTR("OK"), 0, 0},
	{WISP_CTRL_BUTTON, {250, 180, 70, 20}, WISP_ID_CANCEL, 0, WSTR("Cancel"), 0, 0},
	{0},
};

	WISP_MSG_MAP_BEGIN(CCrossReferenceForm)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispForm)

        WISP_CMD_MAP_BEGIN(CCrossReferenceForm)
		WISP_CMD_MAP(1, OnCmdSelect)
        WISP_CMD_MAP_END

        WISP_EVENT_MAP_BEGIN(CCrossReferenceForm)
		WISP_EVENT_MAP(2, OnEventGoto)
        WISP_EVENT_MAP_END

	CCrossReferenceForm::CCrossReferenceForm()
	{
		m_Resource = CrossReferenceForm;
	}
	CCrossReferenceForm::~CCrossReferenceForm()
	{
	}

	bool CCrossReferenceForm::OnCreateForm(WISP_MSG *pMsg)
	{
		m_pListWnd = (CWispList *)GetFormItem(1);
		m_pListWnd->InsertColumn(WSTR("Direction"), 40, 0, 0, -1);
		m_pListWnd->InsertColumn(WSTR("Type"), 35, 0, 0, -1);
		m_pListWnd->InsertColumn(WSTR("Address"), 100, 0, 0, -1);
		m_pListWnd->InsertColumn(WSTR("Instruction"), 200, 0, 0, -1);
		m_pListWnd->m_ItemColor = gSyserColors.Color[1];
		m_pListWnd->SetBGColor(gSyserColors.Color[0]);
		return true;
	}

	bool CCrossReferenceForm::OnCmdSelect(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000118)
			OnEventGoto(0);
		return true;
	}

	bool CCrossReferenceForm::OnEventGoto(WISP_MSG *pMsg)
	{
		CListStringItem *Item = m_pListWnd->FullFindItem(0, 8);
		if (Item)
		{
			ULONG_PTR Addr = m_pListWnd->GetItemValue(Item, 0);
			CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
			if (pWnd)
				pWnd->GotoAddr(Addr, true);
		}
		if (pMsg)
			Destroy();
		return true;
	}

	bool CCrossReferenceForm::OpenTRefList(TList<ULONG_PTR> & Addrs, ULONG_PTR Address)
	{
		bool bSelect = true;
		m_pListWnd->DeleteAllChildrenItems(0);
		CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
		if (!pWnd) return false;

		TListIter<ULONG_PTR> It = Addrs.Begin();
		while (It != Addrs.End())
		{
			CListStringItem *Item;
			ULONG_PTR Addr = *It;
			if (Addr < Address)
				Item = m_pListWnd->InsertItemStr(WSTR("UP"), 0, 0, 0, -1);
			else	Item = m_pListWnd->InsertItemStr(WSTR("DOWN"), 0, 0, 0, -1);
			m_pListWnd->SetItemValue(Item, 0, Addr);
			m_pListWnd->SetItemColor(Item, 0, gpCurWisp->m_pWispDrawObj->m_crSystem[1]);
			m_pListWnd->SetItemColor(Item, 1, gpCurWisp->m_pWispDrawObj->m_crSystem[1]);
			m_pListWnd->SetItemColor(Item, 2, gpCurWisp->m_pWispDrawObj->m_crSystem[1]);
			m_pListWnd->SetItemFormat(Item, 2, Addr, WSTR("%X"));
			unsigned long ColorString[256];
			pWnd->ColorDasm(Addr, 16, ColorString, 0, 0, 0);
			m_pListWnd->SetItemTextColor(Item, 3, ColorString);
			if (bSelect)
				m_pListWnd->Select(Item);
			++It;
		}
		return true;
	}

	bool CCrossReferenceForm::OpenOpRefList(TList<ULONG_PTR> & Addrs, ULONG_PTR Address)
	{
		bool bSelect = true;
		m_pListWnd->DeleteAllChildrenItems(0);
		CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
		if (!pWnd) return false;

		TListIter<ULONG_PTR> It = Addrs.Begin();
		while (It != Addrs.End())
		{
			CListStringItem *Item;
			ULONG_PTR Addr = *It;
			ULONG_PTR Return;
			if ((pWnd->m_pSyserCodeDoc->GetReturn(--Addr, &Return) && Return == Address) ||
			    (pWnd->m_pSyserCodeDoc->GetReturn(--Addr, &Return) && Return == Address))
			{
				if (Addr < Address)
					Item = m_pListWnd->InsertItemStr(WSTR("UP"), 0, 0, 0, -1);
				else	Item = m_pListWnd->InsertItemStr(WSTR("DOWN"), 0, 0, 0, -1);
				m_pListWnd->SetItemValue(Item, 0, Addr);
				m_pListWnd->SetItemColor(Item, 0, gpCurWisp->m_pWispDrawObj->m_crSystem[1]);
				m_pListWnd->SetItemColor(Item, 1, gpCurWisp->m_pWispDrawObj->m_crSystem[1]);
				m_pListWnd->SetItemColor(Item, 2, gpCurWisp->m_pWispDrawObj->m_crSystem[1]);
				m_pListWnd->SetItemFormat(Item, 2, Addr, WSTR("%X"));
				unsigned long ColorString[256];
				pWnd->ColorDasm(Addr, 16, ColorString, 0, 0, 0);
				m_pListWnd->SetItemTextColor(Item, 3, ColorString);
				if (bSelect)
					m_pListWnd->Select(Item);
				bSelect = false;
			}
			++It;
		}
		return true;
	}

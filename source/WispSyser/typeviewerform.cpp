
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "typeviewerform.hpp"
#include "syserconfig.hpp"

	//TODO CHOOSE MODULE SYMBOL MENU

WISP_FORM_RES_ITEM TypeViewerForm[] =
{

	{WISP_CTRL_FORM, {0, 0, 386, 268}, 0, 0xB, WSTR("Attach Type"), (void*)"\\SyserIcon\\Plugin.ico", 0},
	{WISP_CTRL_STATIC_GROUP, {166, 10, 204, 56}, 0, 0, WSTR("Description"), 0, 0},
	{WISP_CTRL_STATIC_STRING, {174, 30, 204, 56}, 8, 0, WSTR(""), 0, 0},
	{WISP_CTRL_STATIC_GROUP, {166, 75, 204, 106}, 0, 0, WSTR("Options"), 0, 0},
	{WISP_CTRL_LIST, {5, 5, 155, 230}, 3, 0x10006, 0, 0, 0},
	{WISP_CTRL_RADIO_BOX, {176, 90, 100, 16}, 4, 0x1000006, WSTR("Floating Type"), 0, 0},
	{WISP_CTRL_RADIO_BOX, {176, 110, 90, 16}, 5, 0x1000006, WSTR("Lock Type"), 0, 0},
	{WISP_CTRL_STATIC_STRING, {194, 135, 45, 16}, 0xC, 0, WSTR("Address:"), 0, 0},
	{WISP_CTRL_EDIT, {245, 135, 93, 16}, 0xD, 0x1000006, 0, 0, 0},
	{WISP_CTRL_RADIO_BOX, {245, 155, 45, 16}, 0xA, 0x1000006, WSTR("Dec"), 0, 0},
	{WISP_CTRL_RADIO_BOX, {301, 155, 45, 16}, 9, 0x1000006, WSTR("Hex"), 0, 0},
	{WISP_CTRL_BUTTON, {185, 197, 68, 25}, 1, 0, WSTR("OK"), 0, 0},
	{WISP_CTRL_BUTTON, {280, 197, 68, 25}, 2, 0, WSTR("Cancel"), 0, 0},
	{0},
};

	WISP_MSG_MAP_BEGIN(CStructViewerForm)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispForm)

        WISP_CMD_MAP_BEGIN(CStructViewerForm)
		WISP_CMD_MAP(3, OnCmdSelect)
        WISP_CMD_MAP_END

        WISP_EVENT_MAP_BEGIN(CStructViewerForm)
		WISP_EVENT_MAP(1, OnEventAdd)
		WISP_EVENT_MAP(2, OnEventClose)

		WISP_EVENT_MAP(4, OnEventFloating)
		WISP_EVENT_MAP(5, OnEventLock)

		WISP_EVENT_MAP(9, OnEventLock) //+++
		WISP_EVENT_MAP(10, OnEventLock) //+++
        WISP_EVENT_MAP_END

	CStructViewerForm::CStructViewerForm()
	{
		m_Resource = TypeViewerForm;
	}
	CStructViewerForm::~CStructViewerForm()
	{
	}

	bool CStructViewerForm::OnCreateForm(WISP_MSG *pMsg)
	{
		m_DIBList.Load("\\sysericon\\SourceCodeIcon.bmp", 16, 16, 80);
		m_DIBList.SetColorKey(0xFF00);

		m_ItemOk = (CWispButton*)GetFormItem(1);
		m_ItemCancel = (CWispButton*)GetFormItem(2);

		m_ItemFloatingType = (CWispRadioBox*)GetFormItem(4);
		m_ItemLockType = (CWispRadioBox*)GetFormItem(5);

		m_ItemDescription = (CWispStaticStr*)GetFormItem(8);

		m_ItemDec = (CWispRadioBox*)GetFormItem(0xA);
		m_ItemHex = (CWispRadioBox*)GetFormItem(9);

		m_ItemAddress = (CWispEdit*)GetFormItem(0xD);
		m_ItemAddressStr = (CWispStaticStr*)GetFormItem(0xC);
		m_ItemOptions = (CWispList*)GetFormItem(3);
		m_ItemOptions->InsertColumn(WSTR(""), 500, 0, 0, -1);

		m_ItemHex->Enable(1);
		m_ItemFloatingType->Enable(1);

		m_ItemAddress->EnableWindow(0);
		m_ItemHex->EnableWindow(0);
		m_ItemDec->EnableWindow(0);
		m_ItemAddressStr->EnableWindow(0);
		m_ItemOk->EnableWindow(0);

		m_ItemDescription = (CWispStaticStr*)GetFormItem(8);
		m_ItemDescription->SetWindowText(WSTR("Attach type to address."));

		ULONG_PTR Address = gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP?*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP:0;
		m_pSDSMod = gpSyser->m_SyserUI.FindSDSModule(Address);

		UpdateContext();
 		return true;
	}

	bool CStructViewerForm::OnDestroy(WISP_MSG *pMsg)
	{
		//
		return true;
	}

	bool CStructViewerForm::OnCmdSelect(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000105)
			m_ItemOk->EnableWindow(1);
		return true;
	}

	bool CStructViewerForm::OnEventAdd(WISP_MSG *pMsg)	
	{
		CListStringItem *Item = m_ItemOptions->GetNext(0, 8);
		if (Item)
		{
			unsigned long ID = m_ItemOptions->GetItemValue(Item, 0);
			if (!gpSyser->m_MainFrame.m_SystemExplorer.m_pTypeViewerWnd)
				gpSyser->m_MainFrame.m_SystemExplorer.OnEventTypeViewerWnd(nullptr);
			if (gpSyser->m_MainFrame.m_SystemExplorer.m_pTypeViewerWnd)
			{
				//OLD CSDSModule *pSDSMod = gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_pSDSMod;
				//ULONG_PTR Address = gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP?*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP:0;
				//CSDSModule *pSDSMod = gpSyser->m_SyserUI.FindSDSModule(Address);
				gpSyser->m_MainFrame.m_SystemExplorer.m_pTypeViewerWnd->Show(m_pSDSMod, ID, m_Address, TV_FUNC_MASK|TV_HEX_MASK|TV_STATIC_MASK, nullptr, nullptr);
			}
		}
		CWispBaseWnd::Close();
		return true;
	}
	bool CStructViewerForm::OnEventClose(WISP_MSG *pMsg)
	{
		CWispBaseWnd::Close();
		return true;
	}
	bool CStructViewerForm::OnEventLock(WISP_MSG *pMsg)
	{
		m_ItemAddress->EnableWindow(1);
		m_ItemHex->EnableWindow(1);
		m_ItemDec->EnableWindow(1);
		m_ItemAddressStr->EnableWindow(1);

		WCHAR Buffer[30];
		if (m_ItemHex->m_Style & 0x400)
			TSPrintf(Buffer, WSTR("%X"), m_Address);
		else	TSPrintf(Buffer, WSTR("%d"), m_Address);
		m_ItemAddress->SetWindowText(Buffer);

		return true;
	}
	//bool CStructViewerForm::OnEventLockUpdate(WISP_MSG *pMsg)
	bool CStructViewerForm::OnEventFloating(WISP_MSG *pMsg)
	{
		m_ItemAddress->EnableWindow(0);
		m_ItemHex->EnableWindow(0);
		m_ItemDec->EnableWindow(0);
		m_ItemAddressStr->EnableWindow(0);
		const WCHAR *Str = m_ItemAddress->GetWindowText().operator const WCHAR*();
		ULONG_PTR Result = 0;
		if (m_ItemHex->m_Style & 0x400)
		{
			if (USHexStrToNum(Str, &Result)) m_Address = Result;
		} else
		{
			if (USDecStrToNum(Str, &Result)) m_Address = Result;
		}
		m_ItemAddress->SetWindowText(WSTR(""));
		return true;
	}
	bool CStructViewerForm::Create(CWispWnd *pParentWnd, int ShowMode, ULONG_PTR Address)
	{
		m_Address = Address;
		return CWispForm::Create(pParentWnd, ShowMode);
	}

	void CStructViewerForm::UpdateContext()
	{
		//OLD CSDSModule *pSDSMod = gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_pSDSMod;
		//ULONG_PTR Address = gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP?*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP:0;
		//CSDSModule *pSDSMod = gpSyser->m_SyserUI.FindSDSModule(Address);
		if (m_pSDSMod)
		{
			m_ItemOptions->DeleteAllChildrenItems(nullptr);
			unsigned long Len = 0;
			BASICTYPE **type = m_pSDSMod->GetGlobal(&Len);

			for (unsigned long n = 1; n < Len; ++n)
			{
				if (type[n]->Elem.Type == ELEM_CLASS)
				{
					CListStringItem *Item = m_ItemOptions->InsertItemStr(type[n]->Elem.Name, 0, 0, m_DIBList.GetDIB(0), -1);
					m_ItemOptions->SetItemValue(Item, 0, n);
				}
			}

			for (unsigned long n = 1; n < Len; ++n)
			{
				if (type[n]->Elem.Type == ELEM_STRUCT)
				{
					CListStringItem *Item = m_ItemOptions->InsertItemStr(type[n]->Elem.Name, 0, 0, m_DIBList.GetDIB(0x36), -1);
					m_ItemOptions->SetItemValue(Item, 0, n);
				}
			}

			for (unsigned long n = 1; n < Len; ++n)
			{
				if (type[n]->Elem.Type == ELEM_UNION)
				{
					CListStringItem *Item = m_ItemOptions->InsertItemStr(type[n]->Elem.Name, 0, 0, m_DIBList.GetDIB(0x48), -1);
					m_ItemOptions->SetItemValue(Item, 0, n);
				}
			}
		}
	}


#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "breakpointform.hpp"
#include "codebpedit.hpp"
#include "databpedit.hpp"

#define ID_EDIT 1
#define ID_NEW 2
#define ID_REMOVE 3
#define ID_REMOVE_ALL 4
#define ID_ENABLE_ALL 5
#define ID_DISABLE_ALL 6
#define ID_TAB 7
#define ID_CODE_BP 8
#define ID_DATA_BP 9

enum
{
  BPF_NAME = 0,
  BPF_OWNER = 1,
  BPF_CONDITION = 2,
};

WISP_FORM_RES_ITEM BreakPointForm[] =
{
	{WISP_CTRL_FORM, {0, 0, 450, 300}, 0, 0xB, WSTR("Breakpoint"), (void*)"\\SyserIcon\\Syser.ico", 0},
	{WISP_CTRL_TAB, {5, 5, 350, 260}, ID_TAB, 6, 0, 0, 0},
	{WISP_CTRL_STATIC_DIB, {380, 30, 70, 20}, 0, 0, 0, (void*)"\\SyserIcon\\Syser.ico:1", 0},
	{WISP_CTRL_BUTTON, {362, 90, 70, 20}, ID_EDIT, 0, WSTR("Edit"), 0, WSTR("Edit break point")},
	{WISP_CTRL_BUTTON, {362, 120, 70, 20}, ID_NEW, 0, WSTR("New"), 0, WSTR("Insert New break point")},
	{WISP_CTRL_BUTTON, {362, 150, 70, 20}, ID_REMOVE, 0, WSTR("Remove"), 0, WSTR("Remove break point")},
	{WISP_CTRL_BUTTON, {362, 180, 70, 20}, ID_REMOVE_ALL, 0, WSTR("RemoveAll"), 0, WSTR("Remove All break point")},
	{WISP_CTRL_BUTTON, {362, 210, 70, 20}, ID_ENABLE_ALL, 0, WSTR("EnableAll"), 0, WSTR("Enable all break point")},
	{WISP_CTRL_BUTTON, {362, 240, 70, 20}, ID_DISABLE_ALL, 0, WSTR("DisableAll"), 0, WSTR("Disable all break point")},
	{0},
};

	WISP_MSG_MAP_BEGIN(CBreakPointForm)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
//TODO BP	WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispForm)

	WISP_CMD_MAP_BEGIN(CBreakPointForm)
		WISP_CMD_MAP(ID_TAB, OnCmdTabSelect)
		WISP_CMD_MAP(ID_CODE_BP, OnCmdListSelect)
		WISP_CMD_MAP(ID_DATA_BP, OnCmdListSelect)
	WISP_CMD_MAP_END

	WISP_EVENT_MAP_BEGIN(CBreakPointForm)
		WISP_EVENT_MAP(ID_EDIT, OnEventEdit)
		WISP_EVENT_MAP(ID_NEW, OnEventNew)
		WISP_EVENT_MAP(ID_REMOVE, OnEventRemove)
		WISP_EVENT_MAP(ID_REMOVE_ALL, OnEventRemoveAll)
		WISP_EVENT_MAP(ID_ENABLE_ALL, OnEventEnableAll)
		WISP_EVENT_MAP(ID_DISABLE_ALL, OnEventDisableAll)
	WISP_EVENT_MAP_END

	CBreakPointForm::CBreakPointForm()
	{
		m_Resource = BreakPointForm;
	}
	CBreakPointForm::~CBreakPointForm()
	{
	}
	
	bool CBreakPointForm::OnCreateForm(WISP_MSG *pMsg)
	{
		m_pTabWnd = (CWispTabWnd *)GetFormItem(ID_TAB);

		m_pEdit = (CWispButton *)GetFormItem(ID_EDIT);

		m_pRemove = (CWispButton *)GetFormItem(ID_REMOVE);
		m_pRemoveAll = (CWispButton *)GetFormItem(ID_REMOVE_ALL);
		m_pEnableAll = (CWispButton *)GetFormItem(ID_ENABLE_ALL);
		m_pDisableAll = (CWispButton *)GetFormItem(ID_DISABLE_ALL);

		m_pCodeBP = new CWispList;

		m_pCodeBP->CreateWnd(0, 0,0,0,0, m_pTabWnd, ID_CODE_BP, 0x40010000, WISP_SH_MINSIZE);
		m_pCodeBP->SetOwner(this);
		m_pTabWnd->InsertWnd(WSTR("Code Breakpoint"), m_pCodeBP, 0);
		InitContext(m_pCodeBP);

		m_pCurList = m_pCodeBP;

		m_pDataBP = new CWispList;
		m_pDataBP->CreateWnd(0, 0,0,0,0, m_pTabWnd, ID_DATA_BP, 0x40010000, WISP_SH_MINSIZE);
		m_pDataBP->SetOwner(this);
		m_pTabWnd->InsertWnd(WSTR("Data Breakpoint"), m_pDataBP, 0);
		InitContext(m_pDataBP);

		ShowCode();
		ShowData();
 		
		return true;
	}

	bool CBreakPointForm::OnCmdTabSelect(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000800) //WISP_ID_TAB_SELECT
		{
			m_pCurList = (CWispList *)pMsg->Command.Param2;
			UpdateButton();
		}
		return true;
	}
	bool CBreakPointForm::OnCmdListSelect(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000105) //WISP_ID_SELECT
		{
			if ((m_pRemove->m_Style >> 9) & 1)
			{
				m_pEdit->EnableWindow(true);
				m_pRemove->EnableWindow(true);
			}
		}
		return true;
	}

	bool CBreakPointForm::OnEventEdit(WISP_MSG *pMsg)
	{
		CListStringItem *Item = m_pCurList->GetNext(0, 8);
		if (Item)
		{
			if (m_pCurList == m_pCodeBP)
			{
				CCodeBPEdit CodeEdit;
				if (m_pCurList->GetItemValue(Item, BPF_OWNER))
					CodeEdit.m_pModuleBP = (BREAK_POINT::MODULE *)m_pCurList->GetItemValue(Item, BPF_NAME);
				else	CodeEdit.m_pBP = (BREAK_POINT *)m_pCurList->GetItemValue(Item, BPF_NAME);
				CodeEdit.Create(0, WISP_SH_MODAL_BLOCK);
				if (CodeEdit.m_Result == WISP_ID_OK)
					ShowCode();
			} else
			{
				CDataBPEdit DataEdit;
				DataEdit.m_pBP = (BREAK_POINT *)m_pCurList->GetItemValue(Item, BPF_NAME);
				DataEdit.Create(0, WISP_SH_MODAL_BLOCK);
				if (DataEdit.m_Result == WISP_ID_OK)
					ShowData();
			}
		}
		return true;
	}
	bool CBreakPointForm::OnEventNew(WISP_MSG *pMsg)
	{
		if (m_pCurList == m_pCodeBP)
		{
			CCodeBPEdit CodeEdit;
			CodeEdit.Create(0, WISP_SH_MODAL_BLOCK);
			if (CodeEdit.m_Result == WISP_ID_OK)
				ShowCode();
		} else
		{
			CDataBPEdit DataEdit;
			DataEdit.Create(0, WISP_SH_MODAL_BLOCK);
			if (DataEdit.m_Result == WISP_ID_OK)
				ShowData();
		}
		return true;
	}
	bool CBreakPointForm::OnEventRemove(WISP_MSG *pMsg)
	{
		CListStringItem *Item = m_pCurList->GetNext(0, 8);
		if (Item)
		{
			if (m_pCurList == m_pCodeBP)
			{
				if (m_pCurList->GetItemValue(Item, BPF_OWNER))
				{
					BREAK_POINT::MODULE *BreakPoint = (BREAK_POINT::MODULE *)m_pCurList->GetItemValue(Item, BPF_NAME);
	typedef _Tmap_pair<ULONG_PTR, BREAK_POINT::MODULE> PBP;
	PBP *p = CONTAINING_RECORD_MY1(BreakPoint, PBP, second);
					//unsigned long Address = p->first; //BP-4 TODO
					gpSyser->m_SyserUI.RemoveBP(p->first);//Address);
				} else
				{
					BREAK_POINT *BreakPoint = (BREAK_POINT *)m_pCurList->GetItemValue(Item, BPF_NAME);
					gpSyser->m_pDebugger->DelCodeBP(*BreakPoint);
				}
				ShowCode();
			} else
			{
				BREAK_POINT *BreakPoint = (BREAK_POINT *)m_pCurList->GetItemValue(Item, BPF_NAME);
				gpSyser->m_pDebugger->DelDataBP(*BreakPoint);
				ShowData();
			}
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
			gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
		}
		return true;
	}
	bool CBreakPointForm::OnEventRemoveAll(WISP_MSG *pMsg)
	{
		if (m_pCurList == m_pCodeBP)
		{
			gpSyser->m_pDebugger->DelAllCodeBP(0, 0x300);
			gpSyser->m_SyserUI.m_ModuleBPMap.clear();
			ShowCode();
		} else
		{
			gpSyser->m_pDebugger->DelAllDataBP(0, 0x300);
			ShowData();
		}
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
		return true;
	}
	bool CBreakPointForm::OnEventEnableAll(WISP_MSG *pMsg)
	{
		if (m_pCurList == m_pCodeBP)
		{
			gpSyser->m_pDebugger->EnableAllCodeBP(0, 0x300);
			gpSyser->m_SyserUI.EnableAllModuleBP(true);
			ShowCode();
		} else
		{
			gpSyser->m_pDebugger->EnableAllDataBP(0, 0x300);
			ShowData();
		}
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
		return true;
	}
	bool CBreakPointForm::OnEventDisableAll(WISP_MSG *pMsg)
	{
		if (m_pCurList == m_pCodeBP)
		{
			gpSyser->m_pDebugger->DisableAllCodeBP(0, 0x300);
			gpSyser->m_SyserUI.EnableAllModuleBP(false);
			ShowCode();
		} else
		{
			gpSyser->m_pDebugger->DisableAllDataBP(0, 0x300);
			ShowData();
		}
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
		return true;
	}

	void CBreakPointForm::CreateShow()
	{
		if (IsWindowShow())
		{
			ShowCode();
			ShowData();
		}
	}
	int CBreakPointForm::FillCode()
	{
		WCHAR szName[256];
		int Count = 0;
		for (map<unsigned long long, BREAK_POINT>::IT it = gpSyser->m_pDebugger->m_pCodeBPMap->begin();
			it != gpSyser->m_pDebugger->m_pCodeBPMap->end(); ++it)
		{
			if (!(it->second.Type & 0x300) && !it->second.pModule)
			{
				if (it->second.Name.m_nLength)
				{
					AnsiToUnicode(it->second.Name.operator const char*(), szName, lenof(szName));
				} else
				{
					uNumToStr(it->second.Address, szName, 16);
				}
				CListStringItem *Item = m_pCodeBP->InsertItem(szName, 0, 0, 0, -1);
				DrawBP(m_pCodeBP, Item, &it->second);
				++Count;
			}
		}
		{
		map<ULONG_PTR, BREAK_POINT::MODULE>::IT it = gpSyser->m_SyserUI.m_ModuleBPMap.begin();
		while (it != gpSyser->m_SyserUI.m_ModuleBPMap.end())
		//wile (it)
		{
			AnsiToUnicode(it->second.Name.operator const char*(), szName, lenof(szName));
			CListStringItem *Item = m_pCodeBP->InsertItem(szName, 0, 0, 0, -1);
			DrawModuleBP(m_pCodeBP, Item, &it->second);
			++it;
		}
		}
		return Count;
	}
	int CBreakPointForm::ShowCode()
	{
		int Count = 0;
		if (m_pCodeBP)
			m_pCodeBP->DeleteAllChildrenItems(0);
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
			Count = FillCode();
		UpdateButton();
		Update(&m_ClientRect);
		return Count;
	}
	int CBreakPointForm::FillData()
	{
		WCHAR szName[256];
		int Count = 0;
		TListIter<BREAK_POINT> It = gpSyser->m_pDebugger->m_pDataBPList->Begin();
		while (It != gpSyser->m_pDebugger->m_pDataBPList->End())
		{
			if (!(It->Type & 0x300))
			{
				if (It->Name.m_nLength)
				{
					AnsiToUnicode(It->Name.operator const char*(), szName, lenof(szName));
				} else
				{
					uNumToStr(It->Address, szName, 16);
				}
				CListStringItem *Item = m_pDataBP->InsertItem(szName, 0, 0, 0, -1);
				DrawBP(m_pDataBP, Item, &*It);
				++Count;
			}
			++It;
		}
		return Count;
	}
	int CBreakPointForm::ShowData()
	{
		int Count = 0;
		if (m_pDataBP)
			m_pDataBP->DeleteAllChildrenItems(0);
		//if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		if (gpSyser->m_pDebugger)
			Count = FillData();
		UpdateButton();
		Update(&m_ClientRect);
		return Count;
	}
	void CBreakPointForm::UpdateButton()
	{
		bool bBP = m_pCurList->GetItemCount(0, true) > 0;
		m_pRemoveAll->EnableWindow(bBP);
		m_pEnableAll->EnableWindow(bBP);
		m_pDisableAll->EnableWindow(bBP);

		bool bPresent = m_pCurList->GetNext(0, 8) != 0;
		m_pRemove->EnableWindow(bPresent);
		m_pEdit->EnableWindow(bPresent);

		Update((WISP_RECT*)0);
	}

	void CBreakPointForm::InitContext(CWispList *pList)
	{
		pList->m_AdvStyle |= 1;
		pList->InsertColumn(WSTR("Name"), 100, 0, 0, -1);
		pList->InsertColumn(WSTR("Owner Process"), 120, 0, 0, -1);
		pList->InsertColumn(WSTR("Condition"), 120, 0, 0, -1);
	}

	void CBreakPointForm::DrawBP(CWispList *pList, CListStringItem *Item, BREAK_POINT *BreakPoint)
	{
		WCHAR Buffer[256];
		pList->SetItemValue(Item, BPF_NAME, (unsigned long long)BreakPoint);
		pList->SetItemValue(Item, BPF_OWNER, 0);
		AnsiToUnicode(BreakPoint->Owner.operator const char*(), Buffer, lenof(Buffer));
		pList->SetItemText(Item, BPF_OWNER, Buffer);
		AnsiToUnicode(BreakPoint->Condition.operator const char*(), Buffer, lenof(Buffer));
		pList->SetItemText(Item, BPF_CONDITION, Buffer);
		if (BreakPoint->State == BP_STATE_DISABLE)
		{
			pList->SetItemColor(Item, BPF_NAME, m_pWispBase->m_pWispDrawObj->m_crSystem[12]);
			pList->SetItemColor(Item, BPF_OWNER, m_pWispBase->m_pWispDrawObj->m_crSystem[12]);
			pList->SetItemColor(Item, BPF_CONDITION, m_pWispBase->m_pWispDrawObj->m_crSystem[12]);
		}
	}

	void CBreakPointForm::DrawModuleBP(CWispList *pList, CListStringItem *Item, BREAK_POINT::MODULE *BreakPoint)
	{
		WCHAR Buffer[256];
		pList->SetItemValue(Item, BPF_NAME, (unsigned long long)BreakPoint);
		pList->SetItemValue(Item, BPF_OWNER, 1);
		pList->SetItemText(Item, BPF_OWNER, WSTR("<All Process>"));
		AnsiToUnicode(BreakPoint->Condition.operator const char*(), Buffer, lenof(Buffer));
		pList->SetItemText(Item, BPF_CONDITION, Buffer);
		if (BreakPoint->State == BP_STATE_DISABLE)
		{
			pList->SetItemColor(Item, BPF_NAME, m_pWispBase->m_pWispDrawObj->m_crSystem[12]);
			pList->SetItemColor(Item, BPF_OWNER, m_pWispBase->m_pWispDrawObj->m_crSystem[12]);
			pList->SetItemColor(Item, BPF_CONDITION, m_pWispBase->m_pWispDrawObj->m_crSystem[12]);
		}
	}

	void CBreakPointForm::Create()
	{
		if (IsWindow())
		{
			CreateShow();
			Show(WISP_SH_MINSIZE);
		} else
		{
			CWispForm::Create(0, WISP_SH_MINSIZE);
		}
	}

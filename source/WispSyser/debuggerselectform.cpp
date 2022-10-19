
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "../Code/sysdep.hpp"
#include "debuggerselectform.hpp"

WISP_FORM_RES_ITEM DebuggerSelectForm[] =
{
	{WISP_CTRL_FORM, {0, 0, 400, 200}, 0, 0xB, WSTR("Debugger Select ..."), (void*)("\\SyserIcon\\Syser.ico"), WSTR("Select Debugger Plugin Module!")},
	{WISP_CTRL_LIST, {5, 5, 300, 160}, 1, 0x40010006, 0, 0, 0},
	{WISP_CTRL_STATIC_DIB, {315, 20, 64, 64}, 0, 0, 0, (void*)("\\SyserIcon\\Syser.ico"), 0},
	{WISP_CTRL_BUTTON, {312, 100, 70, 20}, 2, 0, WSTR("Plugin"), 0, WSTR("Plug Debugger into Syser")},
	{WISP_CTRL_BUTTON, {312, 140, 70, 20}, 3, 0, WSTR("Refresh"), 0, WSTR("Refresh Debugger Module list")},
	{0},
};

	CDebuggerSelectForm::CDebuggerSelectForm()
	{
		m_Resource = DebuggerSelectForm;
	}
	CDebuggerSelectForm::~CDebuggerSelectForm()
	{
	}

	WISP_MSG_MAP_BEGIN(CDebuggerSelectForm)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispForm)

	bool CDebuggerSelectForm::OnCreateForm(WISP_MSG *pMsg)
	{
		m_pList = (CWispList *)GetFormItem(1);
		m_pList->InsertColumn(WSTR("Module Name"), 95, 0, 0, -1);
		m_pList->InsertColumn(WSTR("Information"), 200, 0, 0, -1);
		OnEventModuleList(0);
		return true;
	}

	WISP_EVENT_MAP_BEGIN(CDebuggerSelectForm)
		WISP_EVENT_MAP(0x2, OnEventPlug)
		WISP_EVENT_MAP(0x3, OnEventModuleList)
	WISP_EVENT_MAP_END

	bool CDebuggerSelectForm::OnEventPlug(WISP_MSG *pMsg)
	{
		CListStringItem *pItem = m_pList->GetNext(0, 8);
		if (pItem)
		{
			if (!gpSyser->LoadDebugger(m_pList->GetItemContent(pItem, 0)->String.operator const WCHAR*()))
			{
				gpSyser->m_MainFrame.m_SystemExplorer.UpdateMenu();
				gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateMenu();
			}
			Destroy();
		}
		return true;
	}

	bool CDebuggerSelectForm::OnEventModuleList(WISP_MSG *pMsg)
	{
		typedef void *(*FPCreateDebugger)(CSyserUI *pSyserUI);
		typedef void (*FPGetFileFilter)(WCHAR *Filter);
		typedef void (*FPGetInfo)(char *pInfo);

		m_pList->DeleteAllChildrenItems(0);

		char szModule[MAX_FN_LEN];
		char szModulePath[MAX_FN_LEN];
		GetModulePath(szModulePath, true);
		TStrCpy(szModule, szModulePath);
		TStrCat(szModule, "*.dll");
		FILE_FIND_DATA FindData;
		if (gpFileIO->FindFirstFile(szModule, &FindData))
		{
			do {
				TStrCpy(szModule, szModulePath);
				TStrCat(szModule, FindData.FileName);
				HMODULE hLib = LoadLibraryA(szModule);
				if (hLib)
				{
					FPCreateDebugger fCreateDebugger;
					FPGetFileFilter fGetFileFilter;
					FPGetInfo fGetInfo;

					(FARPROC&)fGetInfo = GetProcAddress(hLib, "GetInfo");
					(FARPROC&)fCreateDebugger = GetProcAddress(hLib, "CreateDebugger");
					if (fGetInfo && fCreateDebugger)
					{
						WCHAR szFileName[64];
						AnsiToUnicode(FindData.FileName, szFileName, lenof(szFileName));
						CListStringItem *pItem = m_pList->InsertItem(szFileName, 0, 0, 0, -1);
						char szInfo[128];
						WCHAR uInfo[128];
						fGetInfo(szInfo);
						AnsiToUnicode(szInfo, uInfo, lenof(uInfo));
						m_pList->SetItemText(pItem, 1, uInfo);
						if (gpSyser->m_hDebuggerModule == hLib)
							m_pList->SetItemColor(pItem, -1, m_pWispBase->m_pWispDrawObj->m_crSystem[3]);
					}
					FreeLibrary(hLib);
				}
			} while (gpFileIO->FindNextFile(&FindData));
		}
		gpFileIO->FindClose(&FindData);
		return true;
	}

	void CDebuggerSelectForm::Create()
	{
		if (IsWindow())
			Show(WISP_SH_MINSIZE);
		else	CWispForm::Create(0, WISP_SH_MINSIZE);
	}

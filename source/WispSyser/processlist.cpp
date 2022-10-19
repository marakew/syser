
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "processlist.hpp"
#include "syserconfig.hpp"

	WISP_MSG_MAP_BEGIN(CProcessList)
	//	WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_CLOSE, OnClose)
	WISP_MSG_MAP_END(CWispList)

	CProcessList::CProcessList()
	{
	}

	CProcessList::~CProcessList()
	{
	}

	bool CProcessList::OnUpdate(WISP_MSG *pMsg)
	{
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		UpdateItemColor(0);
		return true;
	}

	bool CProcessList::OnCreate(WISP_MSG *pMsg)
	{
		AttachTitleDIB(gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0xF));
		m_ItemColor = gSyserColors.Color[1];
		m_Style |= 0x1000000;
		SetBGColor(gSyserColors.Color[0]);
		InsertColumn(WSTR("Name"), 200, 0, 0, -1);
		InsertColumn(WSTR("ID"), 80, 0, 0, -1);
		InsertColumn(WSTR("User Name"), 80, 0, 0, -1);
		DeleteAllChildrenItems(0);
		m_pSystemList = InsertItem(WSTR("System Process List"), 0, 0, 0, -1);
		SetItemText(m_pSystemList, 1, WSTR("SYSTEM"));
		UpdateContext();
		return true;
	}

	bool CProcessList::OnClose(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.FocusActive();
		return true;
	}

	void CProcessList::OnLButton(CListStringItem *Item, int nCol)
	{
		if (Item != m_pSystemList)
		{
			WCHAR Text[256];
			GetItemText(Item, 1, Text, 256);
			if (Text[0] == '0' && Text[1] == 'x')
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("close"), 0);
				WCHAR szCmd[256];
				TSPrintf(szCmd, WSTR("open \\PID:%s"), &Text[2]);
				gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(szCmd, 0);
				Destroy();
			}
		}
	}
#ifndef _RING0_
#include <comdef.h>
#include <winnt.h>

	BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
	{
	//	HANDLE hProcess = GetCurrentProcess();
	//	HANDLE hToken;
	//	OpenProcessToken(hProcess, TOKEN_QUERY|TOKEN_ADJUST_PRIVILEGES, &hToken);

		LUID luid;
		if ( !LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
		{
			//printf("LookupPrivilegeValue error: %u\n", GetLastError() );
			return FALSE;
		}

		TOKEN_PRIVILEGES tp;

		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;

		if (bEnablePrivilege)
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		else	tp.Privileges[0].Attributes = 0;

		if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES) NULL, (PDWORD) NULL))
		{
			//printf("AdjustTokenPrivileges error: %u\n", GetLastError() );
			return FALSE;
		}

		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		{
			//printf("The token does not have the specified privilege. \n");
			return FALSE;
		}
		return TRUE;
	}

	bool GetOwnerName(HANDLE hProcess, WCHAR *pOwnerName, DWORD MaxSize)
	{
		*pOwnerName = 0;

		HANDLE hToken;
		if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
		{
	//		if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
				return false;

	//		SetPrivilege(hToken, SE_DEBUG_NAME, TRUE);
			//TODO
		}

		DWORD Size = 0;
		PTOKEN_USER ptu = NULL;
		GetTokenInformation(hToken, TokenUser, (LPVOID)ptu, 0, &Size);

		if (Size == 0)
		{
			CloseHandle(hToken);
			return false;
		}

		ptu = (PTOKEN_USER)(new BYTE[Size]);
		if (!GetTokenInformation(hToken, TokenUser, (LPVOID)ptu, Size, &Size))
		{
			delete []ptu;
			CloseHandle(hToken);
			return false;
		}

		SID_NAME_USE SidType;
		WCHAR Domain[MAX_FN_LEN];
		DWORD dwSize = MaxSize;
		Size = MAX_FN_LEN;
		bool ret = LookupAccountSidW(NULL, ptu->User.Sid, pOwnerName, &dwSize, Domain, &Size, &SidType);

		delete []ptu;
		CloseHandle(hToken);
	
		return ret;
	}

#include <tlhelp32.h>
#include <psapi.h>
#include <wow64apiset.h>
#endif
	bool CProcessList::UpdateContext()
	{
#ifndef _RING0_
		int SubListSize = 0;
		int MainListSize = 0;

		HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
			return false;

		CListStringItem *Item = GetItem(0, m_pSystemList);
		CListStringItem *NextItem = GetNext(m_pSystemList, 0);

		PROCESSENTRY32W pe32;
		pe32.dwSize = sizeof(pe32);
		if(Process32FirstW(hProcessSnap, &pe32))
		{
		do {
		//	SetPrivilege(GetCurrentProcess(), SE_DEBUG_NAME, TRUE);
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);

			WCHAR OwnerName[MAX_FN_LEN];
			GetOwnerName(hProcess, OwnerName, MAX_FN_LEN);
	
			if (!OwnerName[0])
				TStrCpy(OwnerName, WSTR("SYSTEM"));

			CListStringItem *pSystemList = NULL;
			if (!TStrCmp(OwnerName, WSTR("SYSTEM")))
			{
				pSystemList = m_pSystemList;
				++SubListSize;
			} else
			{
				pSystemList = 0;
				++MainListSize;
			}

			WCHAR szModule[MAX_FN_LEN];
			TStrCpy(szModule, TGetFileName(pe32.szExeFile));
		#if 1
			BOOL bIsWow64 = FALSE;
			if (IsWow64Process(hProcess, &bIsWow64));
			if (bIsWow64)
				TStrCat(szModule, "*32");
		#endif

			CListStringItem *Item = InsertItem(szModule, pSystemList, 0, 0, -1);

			PROCESS_MEMORY_COUNTERS pmc;
			pmc.cb = 0;
			pmc.PageFaultCount = 0;
			pmc.PeakWorkingSetSize = 0;
			pmc.WorkingSetSize = 0;
			pmc.QuotaPeakPagedPoolUsage = 0;
			pmc.QuotaPagedPoolUsage = 0;
			pmc.QuotaPeakNonPagedPoolUsage = 0;
			pmc.QuotaNonPagedPoolUsage = 0;
			pmc.PagefileUsage = 0;
			pmc.PeakPagefileUsage = 0;
			GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
			
			SetItemValue(Item, 0, pe32.th32ProcessID);
			SetItemFormat(Item, 1, pe32.th32ProcessID, WSTR("0x%X"));
			SetItemText(Item, 2, OwnerName);
			GetNext(Item, 0);

			CloseHandle(hProcess);
		} while (Process32NextW(hProcessSnap, &pe32));
		}

		CloseHandle(hProcessSnap);

		Adjust(0, MainListSize + 1);
		Adjust(m_pSystemList, SubListSize);
#endif
		return true;
	}

	void CProcessList::Create()
	{
		if (IsWindow())
		{
			Destroy();
		} else
		{
			WISP_RECT Rect;
			Rect.x = 0;
			Rect.y = 0;
			Rect.cx = 500;
			Rect.cy = 350;
			CreateWnd(WSTR("Processes"), Rect, 0, 0, 0x300BB, WISP_SH_MINSIZE);
			Center(0);
		}
	}

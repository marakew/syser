
#include <stdio.h>
#include "SyserRing3.hpp"
#include "../Code/sysdep.hpp"
#include "../Code/txtfile.hpp"
#include "syserconfig.hpp"
#include "keymappingpage.hpp"

	CSyserPluginUI *gpSyserPluginUI = NULL;

	CSyser *gpSyser = NULL;

	CSyser Syser;

	int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
	{
		gpSyser = &Syser;
		return Syser.WinMainStart(hInstance, lpCmdLine, nShowCmd);
	}

	static void CreateProgress(unsigned long MaxPos)
	{
		gpSyser->m_MainFrame.CreateProgress(WSTR("Loading PDB"), MaxPos);
	}

	static void UpdateProgress(unsigned long Pos)
	{
		gpSyser->m_MainFrame.UpdateProgress(Pos);
	}

	static void UpdateCaption(const WCHAR *Text)
	{
		gpSyser->m_MainFrame.UpdateCaption(Text);
	}

	static void DestroyProgress()
	{
		gpSyser->m_MainFrame.DestroyProgress();
	}

	CSyser::CSyser()
	{
		m_hDebuggerModule = NULL;
		m_pDebugger = NULL;
		gpSyserPluginUI = &m_SyserUI;
		m_bInitializeOK = true;

		TIME_ZONE_INFORMATION tz;
		GetTimeZoneInformation(&tz);
		m_TimeOffset = tz.Bias * 60;

		m_SourceDebug.m_CreateProgress = CreateProgress;
		m_SourceDebug.m_UpdateProgress = UpdateProgress;
		m_SourceDebug.m_UpdateCaption = UpdateCaption;
		m_SourceDebug.m_DestroyProgress = DestroyProgress;
	}

	CSyser::~CSyser()
	{
	}

	void CSyser::LoadConfig()
	{
		char szConfig[MAX_FN_LEN];
		GetModulePath(szConfig, true);
		TStrCat(szConfig, "Syser.cfg");

		if (!ReadConfig(szConfig, &gSyserConfig, sizeof(gSyserConfig)) )
		{
			WriteConfig(szConfig, &gSyserDefConfig, sizeof(gSyserConfig));
			memcpy(&gSyserConfig, &gSyserDefConfig, sizeof(gSyserConfig));
		}

		char szColorConfig[MAX_FN_LEN];
		GetModulePath(szColorConfig, true);
		TStrCat(szColorConfig, "SyserColor.cfg");

		if (!ReadConfig(szColorConfig, &gSyserColors, sizeof(gSyserColors)) )
		{
			WriteConfig(szColorConfig, &gSyserDefColors, sizeof(gSyserColors));
			memcpy(&gSyserColors, &gSyserDefColors, sizeof(gSyserColors));
		}
	}

	bool CSyser::Start()
	{
		LoadConfig();

		m_szWispResource = "Wisp.dat;Syser.dat";
		m_FontIndex = gSyserConfig.iBigFont ? WISP_FONT_8X16 : WISP_FONT_6X12;

		if (!CWinWisp::Start())
			return false;

		if (TStrLen(gSyserConfig.szFontFileName))
		{
			char szFontFileName[MAX_FN_LEN];
			TStrCpy(szFontFileName, gSyserConfig.szFontFileName);
			if (m_FontList[WISP_FONT_BDF].LoadBDF(szFontFileName))
				m_FontIndex = WISP_FONT_BDF;
		}

		m_dwExStyle |= WS_EX_ACCEPTFILES;

		m_bHideMouse = true;

		m_hWindowClassIcon = LoadIconA(m_hInstance, (LPCSTR)101); //???
		m_hCursorCurCIP = LoadCursorA(m_hInstance, (LPCSTR)103); //???
		int Height = MAX( 5*GetSystemMetrics(SM_CYFULLSCREEN)/6, 190);
		int Width = MAX( 5*GetSystemMetrics(SM_CXFULLSCREEN)/6, 720);

		CreateFrame(WSTR("Syser Debugger"), Width, Height, 0);
		LoadHotKey();
		m_MainFrame.CreateWnd(0, 0,0, 
			m_FrameBufferInfo.Width, m_FrameBufferInfo.Height,
			0, WISP_ID_MAIN_FRAME, 0x40, WISP_SH_MINSIZE);
		m_MainFrame.m_AdvStyle |= 4; //WISP_ADV_CANMOVE
		m_MainFrame.m_pWispBase->m_KeyLangMap = 0;
		LoadPlugins();
		if (!gpSyser->LoadDebugger(gSyserConfig.szDebuggerFileName))
		{
			m_MainFrame.m_SystemExplorer.UpdateMenu();
			m_MainFrame.m_SourceDebugFrameWnd.UpdateMenu();
		}
	//	if (!m_pDebugger)
	//		m_MainFrame.m_DebuggerSelectForm.CWispForm::Create(0, WISP_SH_MODAL_BLOCK);
		m_SyserUI.LoadAPIDef();
		LoadHistory();
		m_MainFrame.m_ConsoleWnd.Run("Syser.cmd", 0, 0); //"-s"

		if (m_lpCmdLine && TStrLen(m_lpCmdLine))
		{
			WCHAR szCmdLine[MAX_FN_LEN];
			AnsiToUnicode(m_lpCmdLine, szCmdLine, lenof(szCmdLine));
			WCHAR szCmdBuffer[MAX_FN_LEN];
			TSPrintf(szCmdBuffer, WSTR("open %s"), szCmdLine);
			gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(szCmdBuffer, 0);
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("open %s"), szCmdLine);
		}
		return true;
	}

	bool CSyser::Stop()
	{
		UnloadDebugger();
		UnloadPlugins();

		m_MainFrame.Destroy();

		DestroyFrame();

		DestroyIcon(m_hWindowClassIcon);
		DestroyIcon(m_hCursorCurCIP);
		return CWinWisp::Stop();
	}

	bool CSyser::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_DROPFILES)
		{
			char szFileName[MAX_FN_LEN];
			DragQueryFileA((HDROP)wParam, 0, szFileName, MAX_FN_LEN);
			if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
				gpSyser->m_SyserUI.m_SyserCodeDoc.Close();
			if (!gpSyser->m_SyserUI.m_SyserCodeDoc.Open(szFileName))
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Fail to open dropped file !\n"));
			return false;
		}

		return CWinWisp::WindowProc(hWnd, uMsg, wParam, lParam);
	}

	HOT_KEY_CFG *CSyser::GetHotKeyMap()
	{
		HOT_KEY_CFG *pHotKey = new HOT_KEY_CFG; //4096
		if (!pHotKey) return nullptr;
		
		char HotKeyFileName[MAX_FN_LEN];
		GetModulePath(HotKeyFileName, true);
		TStrCat(HotKeyFileName, "SyserHotKey.cfg");
	#if 0
		unsigned long Size;
		if (ReadConfigSize(HotKeyFileName, pHotKey, sizeof(HOT_KEY_CFG), &Size)) //4096
		{
			DbgPrint("Syser : Load HotKey configure ok!\n");
			return pHotKey;
		}
	#endif
		int i = 0;
		while(i < 127 && gGlobalKeyMap[i].Name)
		{
			pHotKey->Keys[i].CmdID = gGlobalKeyMap[i].CmdID;
			pHotKey->Keys[i].Repeat = 1;
			pHotKey->Keys[i].FirstKey = gGlobalKeyMap[i].FirstKey;
			pHotKey->Keys[i].NextKey = gGlobalKeyMap[i].NextKey;
			++i;
		}
		pHotKey->KeyCount = i;
		pHotKey->Keys[i].CmdID = 0;
		pHotKey->Keys[i].Repeat = 0;
		pHotKey->Keys[i].FirstKey = 0;
		pHotKey->Keys[i].NextKey = 0;
		
		return pHotKey;
	}

	void CSyser::LoadHotKey()
	{
		HOT_KEY_CFG *pHotKey = GetHotKeyMap();
		if (pHotKey)
		{
			TList<HOT_KEY> HK;
			for (int i = 0; i < pHotKey->KeyCount; ++i)
			{
				map<unsigned int, TList<HOT_KEY> >::IT it = m_HotKeyMap.insert(_Tmap_pair<unsigned int, TList<HOT_KEY> >(pHotKey->Keys[i].CmdID, HK));
				if (it == m_HotKeyMap.end()) break;

				for (int x = 0; x < pHotKey->Keys[i].Repeat; ++x)
				{
					HOT_KEY ev;
					ev.FirstKey = pHotKey->Keys[i].FirstKey;
					ev.NextKey = pHotKey->Keys[i].NextKey;
					it->second.Append(&ev);
				}
			}
			delete pHotKey;
		}
	}

	int CSyser::GetHotKey(unsigned int CmdID, map<unsigned int, unsigned int> & HotKeyMap)
	{
		if (CmdID == 0) return 0;
		map<unsigned int, TList<HOT_KEY> >::IT It = m_HotKeyMap.find(CmdID);
		if (It == m_HotKeyMap.end()) return 0;
		TListIter<HOT_KEY> Itc = It->second.Begin();
		while (Itc != It->second.End())
		{
			HotKeyMap.insert(_Tmap_pair<unsigned int, unsigned int>((Itc->FirstKey | Itc->NextKey), CmdID));
			++Itc;
		}
		return It->second.Size();
	}

	void CSyser::RegisterHotKeyEvent(unsigned int CmdID, CWispBaseWnd *pWnd)
	{
		if (CmdID != 0)
		{
			map<unsigned int, TList<HOT_KEY> >::IT It = m_HotKeyMap.find(CmdID);
			if (It == m_HotKeyMap.end()) return;

			TListIter<HOT_KEY> Itc = It->second.Begin();
			while (Itc != It->second.End())
			{
				if (Itc->NextKey)
					pWnd->RegisterKeyEvent(CmdID, Itc->NextKey, Itc->FirstKey, 1);
				else	pWnd->RegisterKeyEvent(CmdID, Itc->FirstKey, 0, 1);
				++Itc;
			}
		}
	}

	void CSyser::UnloadPlugins()
	{
		TListIter<HMODULE> It = m_PluginHandles.Begin();
		while (It != m_PluginHandles.End())
		{
			FreeLibrary(*It);
			++It;
		}
		m_PluginMap.clear();
	}
	int CSyser::LoadPlugin(CFileIO *This, const char *FullFileName, int FileLength, void *pData)
	{
		if (!TIMultiMatchWithPattern("*.dll", FullFileName))
			return 2; //???
		CSyser *pSyser = (CSyser*)pData;
		HMODULE hPlugin = LoadLibraryA(FullFileName);
		pSyser->m_PluginHandles.Append(&hPlugin);
		return 0;
	}
	void CSyser::LoadPlugins()
	{
		char szModulePath[MAX_FN_LEN];
		TStrCat(szModulePath, "Plugin\\*.dll");
		gpFileIO->XFind(szModulePath, CSyser::LoadPlugin, this, 0, 0);
	}

	void CSyser::UnloadDebugger()
	{
		if (m_pDebugger && m_hDebuggerModule)
		{
			if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
				gpSyser->m_SyserUI.m_SyserCodeDoc.Close();
			m_pDebugger->Release();
			m_pDebugger = nullptr;
			FreeLibrary(m_hDebuggerModule);
			gpSyser->m_szDbgFileNameFilter.Empty();
			m_MainFrame.m_SystemExplorer.ResetView();
			UnloadPlugins();
			m_MainFrame.m_SystemExplorer.UpdateMenu();
			m_MainFrame.m_SourceDebugFrameWnd.UpdateMenu();
		}
	}
	bool CSyser::LoadDebugger(const WCHAR *szModuleName)
	{
		typedef void *(*FPCreateDebugger)(CSyserUI *pSyserUI);
		typedef void (*FPGetFileFilter)(WCHAR *Filter);
		typedef void (*FPGetInfo)(char *pInfo);
		
		UnloadDebugger();
		char szFullName[MAX_FN_LEN];
		GetModulePath(szFullName, true);
		int Length = TStrLen(szFullName);
		if (Length>=MAX_FN_LEN) return false;
		UnicodeToAnsi(szModuleName, &szFullName[Length], MAX_FN_LEN - Length);
		m_hDebuggerModule = LoadLibraryA(szFullName);
		if (!m_hDebuggerModule) return false;
		FPCreateDebugger fCreateDebugger;
		(FARPROC&)fCreateDebugger = GetProcAddress(m_hDebuggerModule, "CreateDebugger");
		if (!fCreateDebugger) return false;
		FPGetFileFilter fGetFileFilter;
		(FARPROC&)fGetFileFilter = GetProcAddress(m_hDebuggerModule, "GetFileFilter");
		FPGetInfo fGetInfo;
		(FARPROC&)fGetInfo = GetProcAddress(m_hDebuggerModule, "GetInfo");

		if (fGetFileFilter)
		{
			WCHAR szDbgFileNameFilter[MAX_FN_LEN];
			fGetFileFilter(szDbgFileNameFilter);
			gpSyser->m_szDbgFileNameFilter = szDbgFileNameFilter;
		}

		if (fGetInfo)
		{
			char szTitle[256];
			TStrCpy(szTitle, "Syser Debugger - ");
			fGetInfo(&szTitle[TStrLen(szTitle)]);
			SetWindowTextA(m_hWnd, szTitle);
		}

		m_pDebugger = (CX86Debugger *)fCreateDebugger(&m_SyserUI);
		if (!m_pDebugger)
		{
			FreeLibrary(m_hDebuggerModule);
			m_hDebuggerModule = nullptr;
			return false;
		}
		m_pSysDebugger = m_pDebugger;
		m_pSysDebugger->Init(&gpSyser->m_SyserUI.m_DebugInterface);
		//m_pDebugger->???();

		m_MainFrame.m_SystemExplorer.UpdateMenu();
		m_MainFrame.m_SourceDebugFrameWnd.UpdateMenu();

		return true;
	}

	void CSyser::AddHistory(const char *FileName, unsigned long uFlag)
	{
		RECENT_HISTORY_ITEM FileHistory;

		//TODO get full path from loaded module !!!
		//TODO full path name
		//char szPath[MAX_FN_LEN];
		//const char *pPath = TStrChr(FileName, '\\');
		//if (!pPath)
		//{
		//	char *pStr += GetModulePath(szPath, true);
		//	TStrCpy(pStr, FileName);
		//	FileName = szPath;
		//}

		FileHistory.FileName = FileName;
		FileHistory.uFlag = uFlag;

		TListIter<RECENT_HISTORY_ITEM> It = m_HistoryList.Find(FileHistory);
		if (It != m_HistoryList.End())
		{
			if (It->uFlag == uFlag) return;
			m_HistoryList.Remove(It);
		}

		if (m_HistoryList.Size() > 9)
		{
			m_HistoryList.InsertBefore(m_HistoryList.Begin(), &FileHistory);
			m_HistoryList.Remove(m_HistoryList.End()-1);
		} else
		if (m_HistoryList.Size() > 0)
		{
			m_HistoryList.InsertBefore(m_HistoryList.Begin(), &FileHistory);
		} else
		{
			m_HistoryList.Append(&FileHistory);
		}
		m_bHistoryModified = true;
		SaveHistory();
		LoadHistory();
	}

	void CSyser::SaveHistory()
	{
		CImageFileStream File;
		char szFile[MAX_FN_LEN];
		GetModulePath(szFile, true);
		TStrCat(szFile, "History.lst");
		if (File.Create(szFile, 0))
		{
			TListIter<RECENT_HISTORY_ITEM> It = m_HistoryList.Begin();
			while (It != m_HistoryList.End())
			{
				File.Write(It->FileName.m_pData, It->FileName.m_nLength);
				char szFlag[256];
				int len = TSPrintf(szFlag, "|%X\r\n", It->uFlag);
				File.Write(szFlag, len);
				++It;
			}
			File.Close();
		}
	}

	void CSyser::LoadHistory()
	{
		CTXTFile TXTFile;
		char szFile[MAX_FN_LEN];
		GetModulePath(szFile, true);
		TStrCat(szFile, "History.lst");
		if (!TXTFile.Open(szFile, 0))
			return;

		m_HistoryList.Clear();

		TListIter<char *> It = TXTFile.m_StrList.Begin();
		while (It != TXTFile.m_StrList.End())
		{
			unsigned long uFlag = 0;
			char *pStr = TStrRChr(*It, '|');
			if (pStr)
			{
				*pStr++ = 0;
				if (!USHexStrToNum(pStr, &uFlag))
					uFlag = 0;
			}
			RECENT_HISTORY_ITEM FileHistory;
			FileHistory.FileName = *It;
			FileHistory.uFlag = uFlag;

			if (gpFileIO->IsFileExist(FileHistory.FileName.operator const char*()))
				m_HistoryList.Append(&FileHistory);
			++It;
		}
		TXTFile.Close();

		m_bHistoryModified = false;

		CWispMenu::MenuItem *Item = m_MainFrame.m_SystemExplorer.m_MainMenu.GetItem(WSTR("Recent Files"));
		if (Item && m_HistoryList.Size())
		{
			CWispMenu *RecentMenu = m_MainFrame.m_SystemExplorer.m_MainMenu.GetMenu(Item);
			RecentMenu->Clear();
			TListIter<RECENT_HISTORY_ITEM> It = m_HistoryList.Begin();
			int nID = 0x72;
			while (It != m_HistoryList.End() && nID <= 0x79)
			{
				WCHAR uFile[MAX_FN_LEN];
				AnsiToUnicode(It->FileName.operator const char*(), uFile, lenof(uFile));
				char *pExt = TGetFileExt(It->FileName.operator const char*());
				CWispDIB *pDIB;
				if (pExt && !TStrICmp(pExt, "exe"))
					pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x16);
				else	pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0xF);
				RecentMenu->AppendMenu(-1, uFile, 0, nID, pDIB, 0);
				++nID;
				++It;
			}
			RecentMenu->Adjust();
		}
	}

	WCHAR g_OpenExeArgs[520] = {};
	LPCWSTR g_UiProcessStartDir = NULL;
	const WCHAR *g_CreateProcessStartDir = NULL;

	UINT_PTR CALLBACK /*__stdcall*/ OpenExeWithArgsHookProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
	#if 1
		if (uMsg == WM_INITDIALOG) //0x110 272
		{
			::SendDlgItemMessageW(hDlg, 2251, 0xC5, 0x206, 0);
			if (g_OpenExeArgs[0])
				::SetDlgItemTextW(hDlg, 2251, &g_OpenExeArgs[1]);
			if (g_UiProcessStartDir)
				::SetDlgItemTextW(hDlg, 2253, g_UiProcessStartDir);
			::CheckDlgButton(hDlg, 2252, 0);
			return 1;
		}
	#endif
	#if 0
		if (uMsg == WM_NOTIFY) //0x4E 78
		{
			::GetDlgItemTextW(hDlg, 2253, g_OpenExeArgs, 520);
			DupAllocString(&g_UiProcessStartDir, g_OpenExeArgs);
			g_CreateProcessStartDir = g_UiProcessStartDir;

			g_OpenExeArgs[0] = ' ';
			if (!::GetDlgItemTextW(hDlg, 2251, &g_OpenExeArgs[1], 519))
				g_OpenExeArgs[0] = 0;

			if (::IsDlgButtonChecked(hDlg, 2252) == 1)
				g_DebugCreateOptions.CreateFlags &= ~2 | 1;// DEBUG_PROCESS
			else	g_DebugCreateOptions.CreateFlags &= ~1 | 2;// DEBUG_ONLY_THIS_PROCESS
			return 0;
		}
	#endif
		return 0;
	}

	bool CSyser::OpenFileDialog(char *szFileName, bool arguments)
	{
		//bool arguments = true;
		if (m_szDbgFileNameFilter.m_nLength)
		{
			OPENFILENAMEA ofn;
			memset(&ofn, 0, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hInstance = m_hInstance;
			ofn.hwndOwner = m_hWnd;

			ofn.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR;

			if (arguments)
			{
				ofn.Flags |= OFN_ENABLETEMPLATE;
				ofn.lpTemplateName = (LPCSTR)2250;
			} else
			{
				ofn.Flags |= OFN_EXPLORER;
			}

			if (arguments)
			{
				ofn.Flags |= OFN_ENABLEHOOK;
				ofn.lpfnHook = OpenExeWithArgsHookProc;
			}

			char szFilter[MAX_FN_LEN];
			UnicodeToAnsi(m_szDbgFileNameFilter.operator const WCHAR*(), szFilter, MAX_FN_LEN);

			int Len = TStrLen(szFilter); //TStrLen(szFilter, MAX_FN_LEN);
			MAX_LIMIT(Len, MAX_FN_LEN-1);

			for (int i = Len; i >= 0; --i) //TStrReplace(szFilter, Len, '|', '\0');
			{
				if (szFilter[i] == '|')
					szFilter[i] = 0;
			}

			ofn.lpstrFilter = szFilter;
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_FN_LEN;
			szFileName[0] = 0;
			gpSyser->LockTimer();
			if (!::GetOpenFileNameA(&ofn))
			{
				UnlockTimer();
				return false;
			}
			UnlockTimer();

			TStrCpy(szFileName, ofn.lpstrFile);
		} else
		{
			szFileName[0] = 0;
		}
		return true;
	}

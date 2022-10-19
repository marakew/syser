
#ifndef _SYSER_HPP_
#define _SYSER_HPP_

#include "../Code/alt/altlist.hpp"
#include "../Code/alt/altmap.hpp"
#include "winwisp.hpp"
#include "mainframe.hpp"
#include "plugin.hpp"
#include "syserui.hpp"
#include "../EXEAnalyzer/x86debugger.hpp"
#include "currentfunction.hpp"
#include "../EXEAnalyzer/sourcedebug.hpp"

	struct RECENT_HISTORY_ITEM
	{
		TAnsiString FileName;
		unsigned long uFlag;
		bool operator!=(const RECENT_HISTORY_ITEM&other) const { return FileName != other.FileName; }
	};

	struct HOT_KEY
	{
		unsigned int FirstKey;
		unsigned int NextKey;
	};

	struct HOT_KEY_CFG
	{
		unsigned int ID;

		unsigned int KeyCount;
		struct KEY
		{
			unsigned int CmdID;
			unsigned int Repeat;

			unsigned int FirstKey;
			unsigned int NextKey;
		} Keys[255];
	};

	struct CSyser : public CWinWisp
	{
		map<TWideString, SYSER_PLUGIN_MODULE> m_PluginMap;
		TList<HMODULE> m_PluginHandles;

		HMODULE m_hDebuggerModule;
		CX86Debugger *m_pDebugger;
		CX86Debugger *m_pSysDebugger;
		HCURSOR m_hCursorCurCIP;
		TWideString m_szDbgFileNameFilter;
		CMainFrame m_MainFrame;
		CSyserUI m_SyserUI;
		CSourceDebug m_SourceDebug;
		CCurrentFunction m_CurrentFunctionContext;
		unsigned int m_TimeOffset;
		unsigned long m_TID;
		unsigned long m_PID;

		bool m_bInitializeOK;

		TList<RECENT_HISTORY_ITEM> m_HistoryList;
		bool m_bHistoryModified;
		
		map<unsigned int, TList<HOT_KEY> > m_HotKeyMap;

		struct BPR
		{
			unsigned long Address;
			unsigned long Size;
			unsigned long State;
			unsigned long Type;
			unsigned long PageAddr;
			unsigned long PageAlign;
			unsigned long PageSize;
			TAnsiString Condition;
			TAnsiString Command;
		};

		map<unsigned long, TList<BPR>> m_ProcsBPR;
	public:
		CSyser();
		virtual ~CSyser();

		void LoadConfig();

		virtual bool Start() override;
		virtual bool Stop() override;
		virtual bool WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		HOT_KEY_CFG *GetHotKeyMap();
		void LoadHotKey();

		int GetHotKey(unsigned int CmdID, map<unsigned int, unsigned int> & HotKeyMap);
		void RegisterHotKeyEvent(unsigned int CmdID, CWispBaseWnd *pWnd);

		void UnloadPlugins();
		static int LoadPlugin(CFileIO *This, const char *FullFileName, int FileLength, void *pData);
		void LoadPlugins();

		void UnloadDebugger();
		bool LoadDebugger(const WCHAR *szModuleName);

		void AddHistory(const char *FileName, unsigned long uFlag);
		void SaveHistory();
		void LoadHistory();
		bool OpenFileDialog(char *szFileName, bool arguments = true);
	};

	extern CSyser *gpSyser;
#endif
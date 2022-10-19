
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "consolewnd.hpp"
#include "../Code/sysdep.hpp"
#include "syserconfig.hpp"
#include "syserdefine.hpp"

	CConsoleWnd::CConsoleWnd()
	{
		m_StartCmd = 0;
	}
	CConsoleWnd::~CConsoleWnd()
	{
	}

	WISP_MSG_MAP_BEGIN(CConsoleWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
	WISP_MSG_MAP_END(CWispConsoleWnd)

SETOPTION SetEnv[] =
{
	{WSTR("iFollowMode"), 2, &gSyserConfig.iFollowMode, 0},
	{WSTR("iShowStartSplash"), 2, &gSyserConfig.iShowStartSplash, 0},
	{WSTR("iShowFullSym"), 2, &gSyserConfig.iShowFullSym, 0},
	{WSTR("iShowCCByte"), 2, &gSyserConfig.iShowCCByte, 0},
	{WSTR("iMouseSensitivity"), 2, &gSyserConfig.iMouseSensitivity, 0},
	{WSTR("iSourceCodeShowTips"), 2, &gSyserConfig.iSourceCodeShowTips, 0},
	{WSTR("iSourceCodeSyntaxColors"), 2, &gSyserConfig.iSourceCodeSyntaxColors, 0},
	{WSTR("iSourceCodeShowLineNumbers"), 2, &gSyserConfig.iSourceCodeShowLineNumbers, 0},
	{WSTR("iConnectType"), 2, &gSyserConfig.iConnectType, 0},
	{WSTR("iComPort"), 2, &gSyserConfig.iComPort, 0},
	{WSTR("iHardTimerEnable"), 2, &gSyserConfig.iHardTimerEnable, 0},
	{WSTR("szDebuggerFileName"), 1, gSyserConfig.szDebuggerFileName, MAX_FN_LEN},
	{0}
};

	bool CConsoleWnd::OnCreate(WISP_MSG *pMsg)
	{
		for (int i = 0; SysCmdTable[i].CmdStr; ++i)
		{
			CCmdParser::InsertCmd(SysCmdTable[i].CmdStr,
				(CMDPROC)SysCmdTable[i].CmdProc, gpSyser,
				SysCmdTable[i].CmdCmt,
				SysCmdTable[i].CmdUsage);
		}

		for (int i = 0; ShareCmdTable[i].CmdStr; ++i)
		{
			CCmdParser::InsertCmd(ShareCmdTable[i].CmdStr,
				(CMDPROC)ShareCmdTable[i].CmdProc, gpSyser,
				ShareCmdTable[i].CmdCmt,
				ShareCmdTable[i].CmdUsage);
		}

		for (int i = 0; OtherCmdTable[i].CmdStr; ++i)
		{
			CCmdParser::InsertCmd(OtherCmdTable[i].CmdStr,
				(CMDPROC)OtherCmdTable[i].CmdProc, gpSyser,
				OtherCmdTable[i].CmdCmt,
				OtherCmdTable[i].CmdUsage);
		}

		m_Options.Parse(SetEnv);
		SetHistoryLines(gSyserConfig.iHistoryLines);
		return true;
	}

	bool CConsoleWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.KeyType == VK_TAB && pMsg->KeyEvent.bKeyDown)
		{
			const WCHAR *pStr = TStrRChr(m_InputStr.operator const WCHAR*(), 0x20);
			if (pStr && TStrLen(pStr)>2)
			{
				char szStr[64];
				int Len = TStrCpy(szStr, pStr+1);
				if (gpSyser->m_SyserUI.GetModuleName(szStr) > Len)
				{
					m_InputStr.SetAt(PTR_DELTA(pStr,m_InputStr.m_pData)/2+1, 0);
					WCHAR szuStr[64];
					AnsiToUnicode(szStr, szuStr, lenof(szuStr));
					m_InputStr += szuStr;
					m_CaretPos.x = m_InputStr.m_nLength;
				}
			}
		}
		return true;
	}

	void CConsoleWnd::OnBeginCmd(const WCHAR *szCmd)
	{
		m_StartCmd = 1;
	}

	//virtual void OnCmd(const WCHAR *szCmd)

	void CConsoleWnd::OnEndCmd(const WCHAR *szCmd)
	{
		m_StartCmd = 0;
	}

	bool CConsoleWnd::Run(const char *szFile, const char *szParam, unsigned long *pResult)
	{
		WCHAR uCmd[MAX_FN_LEN];
		char szCmd[MAX_FN_LEN];

		int Lenght = TStrCpy(szCmd, "bat ");
		char *pStr = &szCmd[Lenght];
		if (szParam)
		{
			pStr += TStrCpy(&szCmd[Lenght], szParam);
			pStr += TStrCpy(pStr, " ");
		}
		pStr += GetModulePath(pStr, true);
		TStrCpy(pStr, szFile);

		AnsiToUnicode(szCmd, uCmd, lenof(uCmd)); //??? Rtl
		return m_pConsoleWnd->CCmdParser::ParseCmd(uCmd, pResult);
	}

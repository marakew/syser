
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"

#include "terminalwnd.hpp"

	WISP_MSG_MAP_BEGIN(CTerminalWnd)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
	WISP_MSG_MAP_END(CWispTerminalWnd)

	bool CTerminalWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.KeyType == VK_TAB && pMsg->KeyEvent.bKeyDown)
		{
			WCHAR *pStr = TStrRChr(m_InputStr.operator const WCHAR*(), 0x20);
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

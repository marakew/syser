
#include "cmdparser.hpp"

	CCmdParser::CCmdParser()
	{
		//EMPTY
	}

	CCmdParser::~CCmdParser()
	{
		//EMPTY
	}

	void CCmdParser::PrintString(const WCHAR *szStr)
	{
		//EMPTY
	}

	void CCmdParser::Reset()
	{
		m_CmdMap.clear();
	}

	bool CCmdParser::InsertCmd(const WCHAR *szCmd, CMDPROC pCmdProc, void *pUserData, const WCHAR *pComment, const WCHAR *pUsage)
	{
		WCHAR CmdStr[258];
		TStrCpy(CmdStr, szCmd);
		TStrLwr(CmdStr);

		map<TWideNoCaseString, COMMAND_ENTRY>::IT it = m_CmdMap.Insert(CmdStr);
		if (it == m_CmdMap.end())
			return false;

		it->second.pCmdProc = pCmdProc;
		it->second.pUserData = pUserData;
		it->second.pComment = pComment;
		it->second.pUsage = pUsage;

		return true;
	}

	void CCmdParser::RemoveCmd(const WCHAR *szCmd)
	{
		TWideNoCaseString Cmd(szCmd);
		Cmd.MakeLower();
		m_CmdMap.erase(Cmd);
	}

	int CCmdParser::GetParam(WCHAR *pStr, const WCHAR **pParam)
	{
		int Len = TStrLen(pStr);
		while (Len--)
		{
			if (pStr[Len] == '\n')
				pStr[Len] = 0;
		}
		int Count = 1;
		WCHAR *pPrm = pStr;
		*pParam = pStr;
		while (*pPrm)
		{
			pPrm = TStrChr(pPrm, ' ');
			if (!pPrm)
				break;
			while (*pPrm == ' ')
				*pPrm++ = 0;
			if (*pPrm)
				pParam[Count++] = pPrm;
		}
		return Count;
	}

	bool CCmdParser::CompareParam(const WCHAR *pParam1, const WCHAR *pParam2)
	{
		WCHAR Param1[256];
		WCHAR Param2[256];
		TStrCpyLimit(Param1, pParam1, lenof(Param1));
		TStrCpyLimit(Param2, pParam2, lenof(Param2));

		const WCHAR *Prm1[33];
		const WCHAR *Prm2[33];

		int nParam1 = GetParam(Param1, Prm1);
		int nParam2 = GetParam(Param2, Prm2);
		if (nParam1 != nParam2)
			return false;
		for (int n = 0; n < nParam1; ++n)
		{
			if (TStrCmp(Prm1[n], Prm2[n]))
				return false;
		}
		return true;
	}

	bool CCmdParser::ParseCmd(const WCHAR *szCmd, unsigned long *pResult)
	{
		WCHAR CmdStr[258];
		const WCHAR *ParamStr[32];

		while (*szCmd == 32 || *szCmd == 9) ++szCmd; //is_space

		if (TStrCpyLimit(CmdStr, szCmd, 256) == 0)
			return false;

		int ParamCount = GetParam(CmdStr, ParamStr);

		map<TWideNoCaseString, COMMAND_ENTRY>::IT it = m_CmdMap.find(CmdStr);
		if (it == m_CmdMap.end())
			return false;

		unsigned long ret = it->second.pCmdProc(ParamCount, ParamStr, szCmd, it->second.pUserData);
		if (pResult) *pResult = ret;
		return true;
	}

	void CCmdParser::ParseCmds(const WCHAR *szCmds, WCHAR Delim)
	{
		WCHAR CmdStr[514];
		while (szCmds && *szCmds)
		{
			while (*szCmds == Delim) ++szCmds;
			WCHAR *pStrDelim = TStrChr(szCmds, Delim);
			if (pStrDelim)
			{
				TStrCpyLimit(CmdStr, szCmds, MIN(pStrDelim - szCmds, 512));
				szCmds = pStrDelim;
			} else
			{
				TStrCpyLimit(CmdStr, szCmds, 512);
				szCmds = NULL;
			}
			ParseCmd(CmdStr, NULL);
		}
	}


#ifndef _CMDPARSER_HPP_
#define _CMDPARSER_HPP_

#include "define.h"
#include "alt/altmap.hpp"
#include "tstrmem.hpp"

typedef int (*CMDPROC)(int argc, const WCHAR *argv[], const WCHAR *szCmd, void *pUserData);

struct COMMAND_ENTRY
{
	const WCHAR *pComment;
	const WCHAR *pUsage;
	CMDPROC pCmdProc;
	void *pUserData;
};

struct CCmdParser
{
	CCmdParser();

	virtual ~CCmdParser(); //???

	virtual void PrintString(const WCHAR *szStr); //???

	map<TWideNoCaseString,COMMAND_ENTRY> m_CmdMap;

	virtual void Reset();
	virtual bool InsertCmd(const WCHAR *szCmd, CMDPROC pCmdProc, void *pUserData, const WCHAR *pComment, const WCHAR *pUsage);
	virtual void RemoveCmd(const WCHAR *szCmd);

	bool CompareParam(const WCHAR *pParam1, const WCHAR *pParam2);
	int GetParam(WCHAR *pStr, const WCHAR **pParam);
	bool ParseCmd(const WCHAR *szCmd, unsigned long *pResult);
	void ParseCmds(const WCHAR *szCmds, WCHAR Delim);
};

#endif

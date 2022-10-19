
#ifndef _SYSERDEFINE_HPP_
#define _SYSERDEFINE_HPP_

#include "../Code/define.h"
//#include "../Code/cmdparser.hpp"

struct SYSER_CMD_ENTRY
{
	const WCHAR *CmdStr;
	const WCHAR *CmdCmt;
	void *CmdProc; //CMDPROC
	const WCHAR *CmdUsage;
};

extern SYSER_CMD_ENTRY SysCmdTable[];
extern SYSER_CMD_ENTRY ShareCmdTable[];
extern SYSER_CMD_ENTRY OtherCmdTable[];

#endif

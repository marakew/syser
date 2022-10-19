
#ifndef _OTHERCMD_HPP_
#define _OTHERCMD_HPP_

#include "../Code/define.h"

struct IOCTL_ENTRY
{
	const char *Name;
	unsigned long ControlCode;
};

extern IOCTL_ENTRY stIoctlList[];

struct NTSTATUS_ENTRY
{
	const char *MsgDesc;
	const char *MsgName;
	unsigned long MsgNumber;
};

extern NTSTATUS_ENTRY stNtStatusList[];

struct WINERROR_ENTRY
{
	unsigned long MsgNumber;
	const char *MsgDesc;
	const char *MsgName;
};

extern WINERROR_ENTRY stWinErrorList[];

struct BUGCHECKCODE_ENTRY
{
	unsigned long MsgNumber;
	const char *MsgName;
	const char *MsgDesc;
};

extern BUGCHECKCODE_ENTRY stBugCheckCodeList[];

struct WINDOWS_MSG_ENTRY
{
	unsigned long MsgNumber;
	const char *MsgName;
};

extern WINDOWS_MSG_ENTRY stMsgList[];

#endif
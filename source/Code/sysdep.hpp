
#ifndef _SYSDEP_HPP_
#define _SYSDEP_HPP_

#include "define.h"

int GetModulePath(char *szPathName, bool bSeparator);
int GetModulePath(WCHAR *szPathName, bool bSeparator);
int DebugPrintf(const char *format, ...);
int DebugPrintf(const WCHAR *format, ...);
#endif
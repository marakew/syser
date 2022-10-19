
#ifndef _SETOPTIONS_HPP_
#define _SETOPTIONS_HPP_

#include "define.h"
#include "alt/altmap.hpp"
#include "tstrmem.hpp"

enum
{
	SETOPTION_INVALID = 0,
	SETOPTION_STR = 1,
	SETOPTION_INT = 2,
	SETOPTION_UINT = 3,
	SETOPTION_VOID = 4,
};

struct SETOPTION
{
	const WCHAR *Key;
	int Type;
	void *Value;
	int Size;
};

struct COptions : public map<TWideNoCaseString, SETOPTION>
{
//	map<TWideNoCaseString, OPTION> m_Options;

	void Parse(SETOPTION *Opt);
	bool SetStr(const WCHAR *Key, void *Str, int Size);
	bool SetInt(const WCHAR *Key, void *Int);
	bool SetUInt(const WCHAR *Key, void *UInt);
	bool SetVoid(const WCHAR *Key, void *Void);

	bool Remove(const WCHAR *Key);

	int Get(const WCHAR *Key, void *Value, int *pSize);
	WCHAR * GetStr(const WCHAR *Key, int *pSize);
	int GetInt(const WCHAR *Key);
	unsigned int GetUInt(const WCHAR *Key);
	void *GetVoid(const WCHAR *Key);
	
	bool SetStr(const char *Key, void *Str, int Size);
	bool SetInt(const char *Key, void *Int);
	bool SetUInt(const char *Key, void *UInt);
	bool SetVoid(const char *Key, void *Void);
	bool Remove(const char *Key);
};

#endif

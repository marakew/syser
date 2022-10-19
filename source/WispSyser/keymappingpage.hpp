
#ifndef _KEYMAPPINGPAGE_HPP_
#define _KEYMAPPINGPAGE_HPP_

#include "../Wisp/wispform.hpp"

struct KEYMAP
{
	const char *Name;
	unsigned int FirstKey;
	unsigned int NextKey;
	unsigned int CmdID;
	unsigned int r;
	unsigned int s;
};

extern KEYMAP gGlobalKeyMap[];

struct CKeyMappingPage : public CWispForm
{
	WCHAR *GetHotKey(unsigned int KeyType, WCHAR *szKeys);
};

#endif
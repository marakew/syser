
#include "options.hpp"

	void COptions::Parse(SETOPTION *Opt)
	{
		while (Opt->Key)
		{
			switch (Opt->Type)
			{
			case SETOPTION_STR: SetStr(Opt->Key, Opt->Value, Opt->Size); break;
			case SETOPTION_INT: SetInt(Opt->Key, Opt->Value); break;
			case SETOPTION_UINT: SetUInt(Opt->Key, Opt->Value); break;
			case SETOPTION_VOID: SetVoid(Opt->Key, Opt->Value); break;
			default:;
			}
			++Opt;
		}
	}
	bool COptions::SetStr(const WCHAR *Key, void *Str, int Size)
	{
		map<TWideNoCaseString, SETOPTION>::IT it = insert(_Tmap_pair<TWideNoCaseString, SETOPTION>(Key, SETOPTION()));
		if (it == end()) return false;
		it->second.Type = 1;
		it->second.Value = (void*)Str;
		it->second.Size = Size;
		return true;
	}
	bool COptions::SetInt(const WCHAR *Key, void *Int)
	{
		map<TWideNoCaseString, SETOPTION>::IT it = insert(_Tmap_pair<TWideNoCaseString, SETOPTION>(Key, SETOPTION()));
		if (it == end()) return false;
		it->second.Type = 2;
		it->second.Value = (void*)Int;
		it->second.Size = 4;
		return true;
	}
	bool COptions::SetUInt(const WCHAR *Key, void *UInt)
	{
		map<TWideNoCaseString, SETOPTION>::IT it = insert(_Tmap_pair<TWideNoCaseString, SETOPTION>(Key, SETOPTION()));
		if (it == end()) return false;
		it->second.Type = 3;
		it->second.Value = (void*)UInt;
		it->second.Size = 4;
		return true;
	}
	bool COptions::SetVoid(const WCHAR *Key, void *Void)
	{
		map<TWideNoCaseString, SETOPTION>::IT it = insert(_Tmap_pair<TWideNoCaseString, SETOPTION>(Key, SETOPTION()));
		if (it == end()) return false;
		it->second.Type = 4;
		it->second.Value = (void*)Void;
		it->second.Size = 4;
		return true;
	}

	bool COptions::Remove(const WCHAR *Key)
	{
		return erase(Key) != 0;
	}

	int COptions::Get(const WCHAR *Key, void *Value, int *pSize)
	{
		map<TWideNoCaseString, SETOPTION>::IT It = find(Key);
		if (It == end()) return 0;
		//*((unsigned long*)Value) = (unsigned long)It->second.Value;
		memcpy(Value, &(It->second.Value), sizeof(ULONG_PTR));
		if (pSize) *pSize = It->second.Size;
		return It->second.Type;
	}
	WCHAR * COptions::GetStr(const WCHAR *Key, int *pSize)
	{
		map<TWideNoCaseString, SETOPTION>::IT It = find(Key);
		if (It == end()) return nullptr;
		if (It->second.Type != 1) return nullptr;
		if (pSize) *pSize = It->second.Size;
		return (WCHAR*)It->second.Value;
	}
	int COptions::GetInt(const WCHAR *Key)
	{
		map<TWideNoCaseString, SETOPTION>::IT It = find(Key);
		if (It == end()) return 0;
		if (It->second.Type != 2) return 0;
		return *(int*)It->second.Value;
	}
	unsigned int COptions::GetUInt(const WCHAR *Key)
	{
		map<TWideNoCaseString, SETOPTION>::IT It = find(Key);
		if (It == end()) return 0;
		if (It->second.Type != 3) return 0;
		return *(unsigned int*)It->second.Value;
	}
	void *COptions::GetVoid(const WCHAR *Key)
	{
		map<TWideNoCaseString, SETOPTION>::IT It = find(Key);
		if (It == end()) return nullptr;
		if (It->second.Type != 4) return nullptr;
		return It->second.Value;
	}
	
	bool COptions::SetStr(const char *Key, void *Str, int Size)
	{
		WCHAR wszKey[130];
		AnsiToUnicode(Key, wszKey, lenof(wszKey)-2);
		return SetStr(wszKey, Str, Size);
	}
	bool COptions::SetInt(const char *Key, void *Int)
	{
		WCHAR wszKey[130];
		AnsiToUnicode(Key, wszKey, lenof(wszKey)-2);
		return SetInt(wszKey, Int);
	}
	bool COptions::SetUInt(const char *Key, void *UInt)
	{
		WCHAR wszKey[130];
		AnsiToUnicode(Key, wszKey, lenof(wszKey)-2);
		return SetUInt(wszKey, UInt);
	}
	bool COptions::SetVoid(const char *Key, void *Void)
	{
		WCHAR wszKey[130];
		AnsiToUnicode(Key, wszKey, lenof(wszKey)-2);
		return SetVoid(wszKey, Void);
	}
	bool COptions::Remove(const char *Key)
	{
		WCHAR wszKey[130];
		AnsiToUnicode(Key, wszKey, lenof(wszKey)-2);
		return Remove(wszKey);
	}

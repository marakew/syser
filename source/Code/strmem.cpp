
#include "tstrmem.hpp"
//#include <windows.h>
#include "define.h"
//#include "osdefine.hpp"

int TVSPrintf(char *buffer, const char *format, va_list ParamList)
{
	return TVSPrintf_(buffer, format, ParamList);
}

int TVSPrintf(WCHAR *buffer, const WCHAR *format, va_list ParamList)
{
	return TVSPrintf_(buffer, format, ParamList);
}

/*
static char * InvalidVarStr = " \|-={}[]/?*,:.&^%$#@!~`();"<>^"
.data:0006D8F8 ?InvalidVarStr@@3PADA db ' \|-={}[]/?*,:.&^%$#@!~`();"<>^',0

int TStrPrint(char *pStr, int Count)
{
	int Len = 0;

	while ( *pStr && Count )
	{
		if (TStrChr(InvalidVarStr, *pStr))
			*pStr = '_';
		++pStr;
		--Count;
		++Len;
	}
	*pStr = 0;
	return Len;
}
*/

int UnicodeToAnsi(const WCHAR *WszBuf, char *szBuf, int MaxLen)
{
	int Length = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)WszBuf, -1, szBuf, MaxLen, nullptr, nullptr);
	if (Length)
		return Length - 1;
	return 0;
}

int AnsiToUnicode(const char *szBuf, WCHAR *WszBuf, int MaxLen)
{
	int Length = MultiByteToWideChar(CP_ACP, 0, szBuf, -1, (LPWSTR)WszBuf, MaxLen);
	if (Length)
		return Length - 1;
	return 0;
}

	bool IS_PRINT(unsigned char Char)
	{
		return Char >= 0x20 && Char <= 0x7F;
	}

	bool UNormalize(char *InBuffer, unsigned long InLen, char *OutBuffer, unsigned long MaxOutLen, unsigned long *Len)
	{
		unsigned long total = 0;
		*OutBuffer = 0;
		bool rOK = true;
		unsigned long i = 0;
		while (i < InLen && total < MaxOutLen - 2)
		{
			char Char = InBuffer[i];
			if (Char == 0 && InBuffer[i+1] == 0) break;
			switch (Char)
			{
			case '\r': *OutBuffer++ = '\\'; *OutBuffer++ = 0;
				   *OutBuffer++ = 'r'; *OutBuffer++ = 0; total += 4; break;
			case '\n': *OutBuffer++ = '\\'; *OutBuffer++ = 0;
				   *OutBuffer++ = 'n'; *OutBuffer++ = 0; total += 4; break;
			case '\t': *OutBuffer++ = '\\'; *OutBuffer++ = 0;
				   *OutBuffer++ = 't'; *OutBuffer++ = 0; total += 4; break;
			default:
				if (IS_PRINT(Char))
				{
					*OutBuffer++ = Char;
					if (InBuffer[i+1] == 0)
					{
						*OutBuffer++ = 0; total += 2;
					} else
					{
						rOK = false;
						break;
					}
				}
				break;
			}
			i += 2;
		}

		if (total == 0)
			rOK = false;
		*OutBuffer++ = 0;
		*OutBuffer++ = 0;
		if (rOK)
		{
			if (i < InLen)
			{
				total += 2;
			} else
			{
				TStrCpy(&OutBuffer[InLen - 2], " ...");
				total = InLen;
			}
		}
		if (Len) *Len = total;
		return rOK;
	}

	bool ANormalize(char *InBuffer, unsigned long InLen, char *OutBuffer, unsigned long MaxOutLen, unsigned long *Len)
	{
		unsigned long total = 0;
		*OutBuffer = 0;
		bool rOK = true;
		unsigned long  i = 0;
		while (i < InLen && total < MaxOutLen - 2)
		{
			char Char = InBuffer[i];
			if (Char == 0) break;
			switch (Char)
			{
			case '\r': *OutBuffer++ = '\\'; *OutBuffer++ = 'r'; total += 2; break;
			case '\n': *OutBuffer++ = '\\'; *OutBuffer++ = 'n'; total += 2; break;
			case '\t': *OutBuffer++ = '\\'; *OutBuffer++ = 't'; total += 2; break;
			default:
				if (IS_PRINT(Char))
				{
					*OutBuffer++ = Char; total += 1;
				} else
				{
					rOK = false;
				}
				break;
			}
			i += 1;
		}

		if (total == 0)
			rOK = false;
		*OutBuffer = 0;
		if (rOK)
		{
			if (i < InLen)
			{
				total += 1;
			} else
			{
				TStrCpy(&OutBuffer[InLen - 4], " ...");
				total = InLen;
			}
		}
		if (Len) *Len = total;
		return rOK;
	}

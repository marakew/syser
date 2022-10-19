
#include "sysdep.hpp"
#include "tstrmem.hpp"
#include <windows.h>

int GetModulePath(char *szPathName, bool bSeparator)
{
	*szPathName = 0;
	GetModuleFileNameA(0, szPathName, MAX_FN_LEN);
	if (szPathName[1] == ':')
	{
		MAKE_CHAR_UPPER(szPathName[0]);
	}

	char *sep = TStrRChr(szPathName, '\\');
	if (sep)
	{
		if (bSeparator) ++sep;
		*sep = 0;
	}
	return TStrLen(szPathName);
}

int GetModulePath(WCHAR *szPathName, bool bSeparator)
{
	*szPathName = 0;
	GetModuleFileNameW(0, szPathName, MAX_FN_LEN);
	if (szPathName[1] == ':')
	{
		MAKE_CHAR_UPPER(szPathName[0]);
	}

	WCHAR *sep = TStrRChr(szPathName, L'\\');
	if (sep)
	{
		if (bSeparator) ++sep;
		*sep = 0;
	}
	return TStrLen(szPathName);
}

int DebugPrintf(const char *format, ...)
{
	char OutputString[1024];
	va_list Next;
	va_start(Next, format);
	int Len = TVSPrintf(OutputString, format, Next);
	va_end(Next);
	OutputDebugStringA(OutputString);
	return Len;
}

int DebugPrintf(const WCHAR *format, ...)
{
	WCHAR OutputString[1024];
	va_list Next;
	va_start(Next, format);
	int Len = TVSPrintf(OutputString, format, Next);
	va_end(Next);
	OutputDebugStringW(OutputString);
	return Len;
}

//?gRandomMem@@3PAKA 27x5 = 135 + 2
unsigned long gRandomMem[] = {
	0x5A2B2E15, 0x15562E15, 0x9742A171, 0x8515582E, 0x0F510693,
	0x0708C533, 0x14C80E0B, 0xFE161780, 0x712A3DC2, 0xEB494746,
	0x08F9900A, 0xF7B908D0, 0x0384D5B6, 0x1B212C2B, 0x2022743F,
	0x3416E197, 0x06777BC9, 0xBAB7C768, 0xE9837D6B, 0x2AF10303,
	0xA6D7EB26, 0xE103EB3E, 0x90B7D0BD, 0x59CDE23D, 0x25EBEB07,
	0x0A4A0690, 0x1207C337, 0x6920EE83, 0x690B739E, 0xEDDA9417,
	0x6010EB06, 0x55F2E920, 0xE959CBF4, 0x089DF8FD, 0x525706F9,
	0x43C35351, 0xFE9BB757, 0x5BE2809A, 0x4315191D, 0x34DBC2FB,
	0x0F740504, 0x74280B52, 0x0D820552, 0x4C5CDFFC, 0x1E8DAB6E,
	0x058A0E79, 0x0B72803C, 0xBADF5154, 0x776B6E07, 0x30D77509,
	0x36E68A24, 0x76B669A1, 0x3B03FDB3, 0x18721A44, 0x721E6708,
	0x650A1F0F, 0x6C0BFF46, 0xEB2D721C, 0x58F99002, 0x5F5A595B,
	0xE23BB494, 0x1E0CECB7, 0x160AB907, 0x0DEA75AE, 0x1B7FB26C,
	0xE0090666, 0x0000DAEB, 0x76DA06FF, 0x02F9DB83, 0xFE012A40,
	0x6A0B0807, 0x0FEC5A2E, 0xC9706854, 0x0B40C803, 0x6D555419,
	0x0ADDB179, 0x2073037D, 0x6968116F, 0x1F6E6407, 0xFFDE61A1,
	0x54204350, 0x59736C27, 0x2DE40D43, 0x2EA36370, 0x9D5DC358,
	0x1E2F06D4, 0x8071B569, 0x2F701B11, 0x7F014B70, 0x1A7307C2,
	0x2A7550D9, 0xC52C2C52, 0xEAC518B8, 0x16431995, 0x63226066,
	0xDC0C0BB7, 0x08CE0EE2, 0x0283C010, 0xDB40CCE9, 0x5A0C940C,
	0xB78A50B8, 0x8781B15E, 0xD6821CB4, 0xBDA35C16, 0xFE15EEFD,
	0x0E8920C2, 0xF9810352, 0xB90373CE, 0xB6EC4F04, 0x320E7037,
	0x8A0354DD, 0x095C0507, 0x36230327, 0xEF1C49DD, 0x085F7789,
	0x2DAF4E97, 0x2C1EC524, 0xF8A67060, 0x28E09740, 0x032A9303,
	0xDB80AD79, 0x730470B6, 0xBB23A886, 0x0E6E344C, 0x4D1544DE,
	0x6B754EA6, 0x1ADB6E82, 0x6A39A52C, 0x0D176109, 0xDA1B7313,
	0x570954B6, 0x745F0E13, 0x1D22E44F, 0xFE7F8DDD, 0x123C1975,
	0, 0
	};

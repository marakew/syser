
#include "keymappingpage.hpp"
#include "resource.hpp"
#include "../wisp/wispbase.hpp"

struct KEYS
{
	const char *Name;
	unsigned char ScanCode;
} keys [] =
{
{ "", 255 },
{ "Esc", 27 },
{ "1", 49 },
{ "2", 50 },
{ "3", 51 },
{ "4", 52 },
{ "5", 53 },
{ "6", 54 },
{ "7", 55 },
{ "8", 56 },
{ "9", 57 },
{ "0", 48 },
{ "-", 189 },
{ "+", 187 },
{ "Backspace", 8 },
{ "Tab", 9 },
{ "Q", 81 },
{ "W", 87 },
{ "E", 69 },
{ "R", 82 },
{ "T", 84 },
{ "Y", 89 },
{ "U", 85 },
{ "I", 73 },
{ "O", 79 },
{ "P", 80 },
{ "[", 219 },
{ "]", 221 },
{ "Enter", 13 },
{ "Ctrl", 17 },
{ "A", 65 },
{ "S", 83 },
{ "D", 68 },
{ "F", 70 },
{ "G", 71 },
{ "H", 72 },
{ "J", 74 },
{ "K", 75 },
{ "L", 76 },
{ ";", 186 },
{ "'", 222 },
{ "", 255 },
{ "Shift", 16 },
{ "\\", 220 },
{ "Z", 90 },
{ "X", 88 },
{ "C", 67 },
{ "V", 86 },
{ "B", 66 },
{ "N", 78 },
{ "M", 77 },
{ ",", 188 },
{ ".", 190 },
{ "/", 191 },
{ "Shift", 16 },
{ "*", 106 },
{ "Alt", 18 },
{ "Space", 32 },
{ "Caps Lock", 20 },
{ "F1", 112 },
{ "F2", 113 },
{ "F3", 114 },
{ "F4", 115 },
{ "F5", 116 },
{ "F6", 117 },
{ "F7", 118 },
{ "F8", 119 },
{ "F9", 120 },
{ "F10", 121 },
{ "Num Lock", 144 },
{ "", 145 },
{ "Numeric pad 7", 103 },
{ "Numeric pad 8", 104 },
{ "Numeric pad 9", 105 },
{ "", 109 },
{ "Numeric pad 4", 100 },
{ "Numeric pad 5", 101 },
{ "Numeric pad 6", 102 },
{ "", 107 },
{ "Numeric pad 1", 97 },
{ "Numeric pad 2", 98 },
{ "Numeric pad 3", 99 },
{ "Numeric pad 0", 96 },
{ "Decimal", 110 },
{ "", 255 },
{ "", 255 },
{ "", 255 },
{ "F11", 122 },
{ "F12", 123 },
{ "", 255 },
{ "", 255 },
{ "", 255 },
{ "", 255 },
{ "", 255 },
{ "", 255 },
{ "", 255 },
{ "", 255 },
{ "", 19 },
{ "", 255 },
{ "", 255 },
{ "Up", 38 },
{ "Down", 40 },
};
	WCHAR *CKeyMappingPage::GetHotKey(unsigned int KeyType, WCHAR *szKeys)
	{
		if (!szKeys) return 0;
		*szKeys = 0;

		if (KeyType & 0xFFFFFF)
		{
			if (KeyType & WISP_SK_CTRL) TStrCpy(szKeys, "Ctrl+"); //??? TStrCat
			if (KeyType & WISP_SK_ALT) TStrCat(szKeys, "Alt+");
			if (KeyType & WISP_SK_SHIFT) TStrCat(szKeys, "Shift+");

			for (int i = 0; i < lenof(keys); ++i)
			{
				if ((KeyType & 0xFFFFFF) == keys[i].ScanCode)
				{
					TStrCat(szKeys, keys[i].Name);
					return szKeys;
				}
			}

			
		} else
		{
			for (int i = 0; i < lenof(keys); ++i)
			{
				if ((KeyType & 0xFF000000) == keys[i].ScanCode)
				{
					TStrCpy(szKeys, keys[i].Name);
					return szKeys;
				}
			}
		}
		return szKeys;
	}

KEYMAP gGlobalKeyMap[] =
{
	{"ActiveHotKey", WISP_SK_CTRL, 0x7B, 0, 0, 0},
	{"Debug:Continue", 0x74, 0, EVENT_ID_GO, 0, 0},
	{"Debug:Return", 0x75, 0, EVENT_ID_DBG_RETURN, 0, 0},
	{"Debug:Step", 0x77, 0, EVENT_ID_DBG_STEP, 0, 0},
	{"Debug:Proceed", 0x79, 0, EVENT_ID_DBG_PROCEED, 0, 0},
	{"Debug:Step Branch", 0x7A, 0, EVENT_ID_DBG_STEPB, 0, 0},
	{"Debug:Break Point", WISP_SK_CTRL, 0x78, EVENT_ID_BREAK_POINT, 0, 0},
	{"Monitor:Register View", WISP_SK_ALT, 0x47, EVENT_ID_REGVIEW, 0, 0},
	{"Monitor:Data View", WISP_SK_ALT, 0x44, EVENT_ID_DATAVIEW, 0, 0},
	{"Monitor:Call Stack", WISP_SK_ALT, 0x43, EVENT_ID_CALLSTACK, 0, 0},
	{"View:Toggle BookMark", WISP_SK_CTRL, 0x4B, EVENT_ID_TOGGLE_BM, 0, 0},
	{"View:Previous BookMark", WISP_SK_CTRL, 0x50, EVENT_ID_PREV_BM, 0, 0},
	{"View:Next BookMark", WISP_SK_CTRL, 0x4E, EVENT_ID_NEXT_BM, 0, 0},
	{"View:Clear BookMark", WISP_SK_CTRL, 0x4C, EVENT_ID_CLEAR_BM, 0, 0},
	{0},
};

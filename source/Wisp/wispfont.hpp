
#ifndef _WISPFONT_HPP_
#define _WISPFONT_HPP_

#include "../Code/define.h"
//#include "../Code/osdefine.hpp"

enum WISP_FONT_ID
{
	WISP_FONT_8X16 = 0,
	WISP_FONT_6X12,// = 1,
	WISP_FONT_ASCI_12X16,// = 2,
	WISP_FONT_EBCD_12X16,// = 3,
	WISP_FONT_ANSI_12X16,// = 4,
	WISP_FONT_BDF,
	WISP_FONT_COUNT,// = 5,
};

struct WISP_PHYS_FONT_INFO
{
	int Width;
	int Height;
	const char *FontFN;
	const char *WideFontFN;
};

struct CWispFont
{
	CWispFont();
	~CWispFont();

	bool LoadBDF(const char *FileName);
	bool LoadChar(WISP_PHYS_FONT_INFO *pPhysFont);
	bool LoadWideChar(WISP_PHYS_FONT_INFO *pPhysFont);

	void Destroy();

	int m_CharLenAry[256];

	int m_Height;
	int m_Width;
	unsigned char *m_Buffer;
	int m_BufSize;

	int m_WideHeight;
	int m_WideWidth;
	unsigned char *m_WideBuffer;
	int m_WideBufSize;

	int m_BytesPerChar;

	unsigned char *GetCharPixelBuffer(WCHAR Char);

	int GetCharWidth(WCHAR Char);
	int GetCharHeight(WCHAR Char);
};

#endif


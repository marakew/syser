
#ifndef _WISPDC_HPP_
#define _WISPDC_HPP_

#include "../Code/define.h"
#include "utility.hpp"
#include "wispfont.hpp"
#include "wispdib.hpp"
#include "wispdraw.hpp"

	//forward decl

	struct CWispBase; //???
	struct CWispBaseWnd; //???

struct CWispDC
{
	CWispBaseWnd *m_pWispWnd;
	CWispBase *m_pWispBase;
	WISP_FRAME_BUFFER_INFO *m_pFrameBuffer;
	WISP_RECT *m_pRect;
	unsigned long *m_pColor;
	CWispFont *m_pFont;
	unsigned long m_TextColor;
	unsigned long m_TextShadowColor;
	unsigned long m_TextBKColor;

	unsigned long m_OldTextColor;
	unsigned long m_OldTextShadowColor;
	unsigned long m_OldTextBKColor;

	unsigned int m_TextStyle;
	WISP_RECT *m_pClipRect;
	unsigned long m_TabAlign;

	unsigned long GetTextColor() { return m_TextColor; }
	void SetTextColor(unsigned long TextColor)
	{
		m_OldTextColor = m_TextColor;
		m_TextColor = TextColor;
	}
	void SetTextColor() { m_TextColor = m_OldTextColor; }


	unsigned long GetTextShadowColor() { return m_TextShadowColor; }
	void SetTextShadowColor(unsigned long TextShadowColor)
	{
		m_OldTextShadowColor = m_TextShadowColor;
		m_TextShadowColor = TextShadowColor;
	}
	void SetTextShadowColor() { m_TextShadowColor = m_OldTextShadowColor; }

	unsigned long GetTextBKColor() { return m_TextBKColor; }
	void SetTextBKColor(unsigned long TextBKColor)
	{
		m_OldTextBKColor = m_TextBKColor;
		m_TextBKColor = TextBKColor;
	}
	void SetTextBKColor() { m_TextBKColor = m_OldTextBKColor; }

	void SetTextStyle(unsigned int TextStyle);

	void SetTextType(unsigned int FontIndex);

	CWispDC();
	~CWispDC();

	bool Create(CWispBaseWnd *pWispWnd, WISP_RECT *pRect);
	void Destroy();

	void SetFontAndColor();

	bool Create(WISP_FRAME_BUFFER_INFO *pFrameBuffer, WISP_RECT *pRect);
	CWispDC *CreateNull(WISP_FRAME_BUFFER_INFO *pFrameBuffer, WISP_RECT *pRect);
	bool Create(CWispDIB *pDIB);

	bool IsRectVisible(WISP_RECT *pRect);
	bool ClipDCRect(WISP_RECT *pRect);
	bool ClipDCLine(int *pX1, int *pY1, int *pX2, int *pY2);
	bool ClipDCPoint(WISP_POINT *pPT);

	void DrawPixel(int x, int y, unsigned long color);
	void DrawVLine(int x, int y1, int y2, unsigned long color);
	void DrawHLine(int x1, int x2, int y, unsigned long color);
	void DrawVDashLine(int x, int y1, int y2, unsigned long color);
	void DrawHDashLine(int x1, int x2, int y, unsigned long color);
	void DrawLine(int x1, int y1, int x2, int y2, unsigned long color);
	void DrawCircle(int, int, int, unsigned long);

	void DrawCircleRightTop(int x, int y, int radius, unsigned long color);
	void DrawCircleRightBottom(int x, int y, int radius, unsigned long color);
	void DrawCircleLeftTop(int x, int y, int radius, unsigned long color);
	void DrawCircleLeftBottom(int x, int y, int radius, unsigned long color);
//!!
	void CirclePlotPointsRightTop(int xCenter, int yCenter, int x, int y, unsigned long color, WISP_RECT *pClipRect);
	void CirclePlotPointsRightBottom(int xCenter, int yCenter, int x, int y, unsigned long color, WISP_RECT *pClipRect);
	void CirclePlotPointsLeftTop(int xCenter, int yCenter, int x, int y, unsigned long color, WISP_RECT *pClipRect);
	void CirclePlotPointsLeftBottom(int xCenter, int yCenter, int x, int y, unsigned long color, WISP_RECT *pClipRect);

	void DrawRect(WISP_RECT *pRc, unsigned long color);
	void DrawRect(int x, int y, int cx, int cy, unsigned long color);

	void DrawRoundRect(WISP_RECT *pRc, unsigned long color);
	void Draw3DRect(WISP_RECT *pRc, unsigned long crTopLeft, unsigned long crBottomRight);
	void DrawDashRect(WISP_RECT *pRc, unsigned long color);

	void DrawFullRect(WISP_RECT *pRc, unsigned long color);
	void DrawFullRect(int x, int y, int cx, int cy, unsigned long color);

	unsigned long DrawXGrayChgFullRect(WISP_RECT *pRc, unsigned long color, int nChg);
	unsigned long DrawYGrayChgFullRect(WISP_RECT *pRc, unsigned long color, int nChg);

	void DrawXGrayColorFullRect(unsigned long color1, unsigned long color2, WISP_RECT *pRc);
	void DrawYGrayColorFullRect(unsigned long color1, unsigned long color2, WISP_RECT *pRc);

	void DrawXGrayColorFullRect2(unsigned long color1, unsigned long color2, WISP_RECT *pRc);
	void DrawYGrayColorFullRect2(unsigned long color1, unsigned long color2, WISP_RECT *pRc);

	void DrawFrameRect(WISP_RECT *pRc, bool bRaised, int Deep);
	void DrawFrameRect(int x, int y, int cx, int cy, bool bRaised, int Deep);

	void DrawDIBTiled(CWispDIB *pDIB, WISP_RECT *pRect);

	void DrawDIB(int x, int y, CWispDIB *pDIB);
	void DrawDIB(WISP_RECT *pRect, CWispDIB *pDIB);

	void DrawDIBGray(int x, int y, CWispDIB *pDIB);
	void DrawDIBGray(WISP_RECT *pRect, CWispDIB *pDIB);

	void DrawSystemStandardIcon(WISP_RECT *rcBoundary, unsigned int uStyle, unsigned long color);

	int GetTextHeight(const WCHAR Char);
	int GetTextLen(const WCHAR *lpString, int nStrLen);
	int GetTextExtent(const WCHAR *lpString, int nStrLen, int Count);
	int GetTextExtentTabExpend(const WCHAR *lpString, int nTabPixelWidth, int nCount);
	int GetTextExtentLineTabExpend(const WCHAR *lpString, int nTabPixelWidth, int nCount);


//	int DrawSingleLineText(WCHAR*, int, WISP_RECT*, unsigned int); //private


	int CalcTextLineNum(const WCHAR *lpString, int nCount);
	int CalcDrawTextRect(const WCHAR *lpString, WISP_RECT *lpRect, int nStrLen, unsigned int uFormat, int nTabPixelWidth);
	int GetMaxShowCharNum(const WCHAR *lpString, int nMaxWidth, int nTabPixelWidth, int nCount);
	int DrawTabExpendString(int x, int y, const WCHAR *lpString, int nStringRealLen, WISP_RECT rc, int nTabPixelWidth);

	void AdjustNoClipRect(WISP_RECT *lpRect, unsigned int uFormat);

	int DrawString(const WCHAR *lpString, WISP_RECT *lpRect, unsigned int uFormat, unsigned int nShowLen, int nPreLineMaxCharNum);
//??	void DrawString(int x, int y, const WCHAR *string, int Len, WISP_RECT *pClipRect);

//??	void DrawColorString(int x, int y, const unsigned long  *string, int Len, WISP_RECT *pClipRect);
//??	void DrawRightString(int, int, const WCHAR*, int, WISP_RECT*);
//??	void DrawShadowText(int, int, const WCHAR*, int, int);


	int DrawString(CWispFont *pLogFont, int x, int y, const WCHAR *pStr, unsigned long TextColor, WISP_RECT *pClipRect, unsigned long *pBKColor, int Len, int nTabPixelWidth, int bFlag);
	int DrawString(int x, int y, const WCHAR *pStr, int Len, int nTabPixelWidth, int bFlag);
	int DrawString(WISP_RECT *pRect, const WCHAR *pStr, int Len, int nTabPixelWidth);
	void DrawShadowText(WISP_RECT *pRect, const WCHAR *pStr, int Offset, int Len, int nTabPixelWidth);

	int DrawColorString(CWispFont *pLogFont, int x, int y, const unsigned long *pStr, WISP_RECT *pClipRect, int Len, int nTabPixelWidth);
	int DrawColorString(int x, int y, const unsigned long *pStr, int Len, int nTabPixelWidth);
	int DrawColorString(WISP_RECT *pRect, const unsigned long *pStr, int Len, int nTabPixelWidth);
};

#endif
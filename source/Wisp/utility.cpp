
#include "utility.hpp"
#include "../Code/tstrmem.hpp"

bool ClipXLine(int *pX1, int *pX2, int *pY1, const WISP_RECT *pRect)
{
	if (*pX1 < pRect->x && *pX2 < pRect->x)
		return false;

	if (*pX1 >= pRect->x2() && *pX2 >= pRect->x2())
		return false;

	if (*pY1 < pRect->y)
		return false;

	if (*pY1 >= pRect->y2())
		return false;

	*pX1 = CLAMP(*pX1, pRect->x, pRect->x2() - 1);
	*pX2 = CLAMP(*pX2, pRect->x, pRect->x2() - 1);
	
	return *pX1 != *pX2;
}

bool ClipYLine(int *pX1, int *pY1, int *pY2, const WISP_RECT *pRect)
{
	if (*pY1 < pRect->y && *pY2 < pRect->y)
		return false;

	if (*pY1 >= pRect->y2() && *pY2 >= pRect->y2())
		return false;

	if (*pX1 < pRect->x)
		return false;

	if (*pX1 >= pRect->x2())
		return false;

	*pY1 = CLAMP(*pY1, pRect->y, pRect->y2() - 1);
	*pY2 = CLAMP(*pY2, pRect->y, pRect->y2() - 1);
	
	return *pY1 != *pY2;
}

void ClipPointX(int *pX, int *pY, const WISP_RECT *pRect, int deltaX, int deltaY)
{
	if (*pX < pRect->x)
	{
		int delta = pRect->x - *pX;
		*pX = pRect->x;
		*pY += (delta * ((deltaY << 16) / deltaX)) >> 16;
	} else
	if (*pX > pRect->x2() - 1)
	{
		int delta = pRect->x2() - 1 - *pX;
		*pX = pRect->x2() - 1;
		*pY += (delta * ((deltaY << 16) / deltaX)) >> 16;
	}
}

void ClipPointY(int *pX, int *pY, const WISP_RECT *pRect, int deltaX, int deltaY)
{
	if (*pY >= pRect->y)
	{
		int delta = pRect->y - *pY;
		*pY = pRect->y;
		*pX += (delta * ((deltaX << 16) / deltaY)) >> 16;
	} else
	if (*pY > pRect->y2() - 1)
	{
		int delta = pRect->y2() - 1 - *pY;
		*pY = pRect->y2() - 1;
		*pX += (delta * ((deltaX << 16) / deltaY)) >> 16;
	}
}

bool ClipLine(int *pX1, int *pY1, int *pX2, int *pY2, const WISP_RECT *pRect)
{
	if (*pX1 == *pX2)
		return ClipYLine(pX1, pY1, pY2, pRect);
	if (*pY1 == *pY2)
		return ClipXLine(pX1, pX2, pY1, pRect);

	if (*pX1 < pRect->x && *pX2 < pRect->x)
		return false;
	if (*pX1 >= pRect->x2() && *pX2 >= pRect->x2())
		return false;
	if (*pY1 < pRect->y && *pY2 < pRect->y)
		return false;
	if (*pY1 >= pRect->y2() && *pY2 >= pRect->y2())
		return false;

	int deltaX = *pX2 - *pX1;
	int deltaY = *pY2 - *pY1;
//X1
	//ClipPointX(pX1,pY1,pRect,deltaX,deltaY);
	if (*pX1 < pRect->x)
	{
		int cx = pRect->x - *pX1;
		*pX1 = pRect->x;
		*pY1 += (cx * ((deltaY << 16) / deltaX)) >> 16;
	} else
	if (*pX1 > pRect->x2() - 1)
	{
		int cx = pRect->x2() - 1 - *pX1;
		*pX1 = pRect->x2() - 1;
		*pY1 += (cx * ((deltaY << 16) / deltaX)) >> 16;
	}
	
//X2
	//ClipPointX(pX2,pY2,pRect,deltaX,deltaY);
	if (*pX2 < pRect->x)
	{
		int cx = pRect->x - *pX2;
		*pX2 = pRect->x;
		*pY2 += (cx * ((deltaY << 16) / deltaX)) >> 16;
	} else
	if (*pX2 > pRect->x2() - 1)
	{
		int cx = pRect->x2() - 1 - *pX2;
		*pX2 = pRect->x2() - 1;
		*pY2 += (cx * ((deltaY << 16) / deltaX)) >> 16;
	}
	
//Y1
	//ClipPointY(pX1,pY1,pRect,deltaX,deltaY);
	if (*pY1 < pRect->y)
	{
		int cy = pRect->y - *pY1;
		*pY1 = pRect->y;
		*pX1 += (cy * ((deltaX << 16) / deltaY)) >> 16;
	} else
	if (*pY1 > pRect->y2() - 1)
	{
		int cy = pRect->y2() - 1 - *pY1;
		*pY1 = pRect->y2() - 1;
		*pX1 += (cy * ((deltaX << 16) / deltaY)) >> 16;
	}
//Y2
	//ClipPointY(pX2,pY2,pRect,deltaX,deltaY);
	if (*pY2 < pRect->y)
	{
		int cy = pRect->y - *pY2;
		*pY2 = pRect->y;
		*pX2 += (cy * ((deltaX << 16) / deltaY)) >> 16;
	} else
	if (*pY2 > pRect->y2() - 1)
	{
		int cy = pRect->y2() - 1 - *pY2;
		*pY2 = pRect->y2() - 1;
		*pX2 += (cy * ((deltaX << 16) / deltaY)) >> 16;
	}

	return *pX1 != *pX2 || *pY1 != *pY2;
}

//Intersec
bool ClipRect(WISP_RECT *pRect, const WISP_RECT *pClipRect)
{
#if 0
	int xx = MAX(pClipRect->x, pRect->x);
	int yy = MAX(pClipRect->y, pRect->y);

	int xcx = MIN(pClipRect->x2(), pRect->x2());
	int ycy = MIN(pClipRect->y2(), pRect->y2());

	pRect->x = xx;
	pRect->y = yy;
	pRect->cx = xcx - xx;
	pRect->cy = ycy - yy;
#else
	if (pRect->x < pClipRect->x)
	{
		pRect->cx += pRect->x - pClipRect->x;
		pRect->x = pClipRect->x;
	}

	if (pRect->y < pClipRect->y)
	{
		pRect->cy += pRect->y - pClipRect->y;
		pRect->y = pClipRect->y;
	}

	if (pRect->x + pRect->cx > pClipRect->x2()) //MAX_LIMIT
		pRect->cx = pClipRect->x2() - pRect->x;

	if (pRect->y + pRect->cy > pClipRect->y2()) //MAX_LIMIT
		pRect->cy = pClipRect->y2() - pRect->y;
#endif
	return !IsInvalidRect(pRect);
}

//Intersec
bool HasClipRect(const WISP_RECT *pRect, const WISP_RECT *pClipRect)
{
	WISP_RECT RectTest = *pRect;
	return ClipRect(&RectTest, pClipRect);
}

unsigned long FindFitColor(unsigned long Color, unsigned long *Pal)
{
	long MinDelta = 0x7FFFFFFF; //MAX_SINT32
	long nDelta = 0;

	for (int n = 0; n < 256; ++n)
	{
		long Delta = (long)(Color - Pal[n]) < 0 ? Pal[n] - Color : Color - Pal[n]; //abs(Color - Pal[n]) ???
		if (!Delta) return n;
		if (MinDelta > Delta)
		{
			MinDelta = Delta;
			nDelta = n;
		}
	}
	return nDelta;
}

int ColorStrNCpy(unsigned long *Des, WCHAR const *Src, int Count, unsigned char TXTColor, unsigned char BKColor)
{
	int len = 0;
	while (*Src && Count--)
	{
		*Des = MAKE_DWORD(MAKE_WORD(BKColor,TXTColor),*Src);
		++Src;
		++Des;
		++len;
	}
	*Des = 0;
	return len;
}

int ColorStrNCpy(unsigned long *Des, char const *Src, int Count, unsigned char TXTColor, unsigned char BKColor)
{
	int len = 0;
	while (*Src && Count--)
	{
		*Des = MAKE_DWORD(MAKE_WORD(BKColor,TXTColor),*Src);
		++Src;
		++Des;
		++len;
	}
	*Des = 0;
	return len;
}

int ColorStrCpy(unsigned long *Des, WCHAR const *Src, unsigned char TXTColor, unsigned char BKColor)
{
	int len = 0;
	while (*Src)
	{
		*Des = MAKE_DWORD(MAKE_WORD(BKColor,TXTColor),*Src);
		++Src;
		++Des;
		++len;
	}
	*Des = 0;
	return len;
}

int ColorStrCpy(unsigned long *Des, char const *Src, unsigned char TXTColor, unsigned char BKColor)
{
	int len = 0;
	while (*Src)
	{
		*Des = MAKE_DWORD(MAKE_WORD(BKColor,TXTColor),*Src);
		++Src;
		++Des;
		++len;
	}
	*Des = 0;
	return len;
}

int ColorStrNCat(unsigned long *Des, WCHAR const *Src, int Count, unsigned char TXTColor, unsigned char BKColor)
{
	Des += TStrLen(Des);
	return ColorStrNCpy(Des, Src, Count, TXTColor, BKColor);
}

int ColorStrNCat(unsigned long *Des, char const *Src, int Count, unsigned char TXTColor, unsigned char BKColor)
{
	Des += TStrLen(Des);
	return ColorStrNCpy(Des, Src, Count, TXTColor, BKColor);
}

int ColorStrCat(unsigned long *Des, WCHAR const *Src, unsigned char TXTColor, unsigned char BKColor)
{
	Des += TStrLen(Des);
	return ColorStrCpy(Des, Src, TXTColor, BKColor);
}

int ColorStrCat(unsigned long *Des, char const *Src, unsigned char TXTColor, unsigned char BKColor)
{
	Des += TStrLen(Des);
	return ColorStrCpy(Des, Src, TXTColor, BKColor);
}

//bool IsInvalidRect(WISP_RECT *pRect) { return pRect->cx == 0 || pRect->cy == 0; }
//bool IsInvalidRect(WISP_RECT *pRect) { return pRect->cx <= 0 || pRect->cy <= 0; }

bool PtInRect(const WISP_RECT *pRect, int x, int y)
	{
	return (!IsInvalidRect(pRect) &&
		x >= pRect->x && x < pRect->x2() &&
		y >= pRect->y && y < pRect->y2());
	}

//bool PtInRect(WISP_RECT *pRect, WISP_POINT & pPoint) { return PtInRect(pRect, pPoint.x, pPoint.y); }
//bool PtInRect(WISP_RECT *pRect, WISP_POINT * pPoint) { return PtInRect(pRect, *pPoint); }

//Union
void RectToRect(const WISP_RECT *pRect, WISP_RECT *pToRect)
{
	if (IsInvalidRect(pRect))
	{
		return;
	}

	if (IsInvalidRect(pToRect))
	{
		*pToRect = *pRect;
		return;
	}

	int xx = MIN(pRect->x, pToRect->x);
	int yy = MIN(pRect->y, pToRect->y);

	int xcx = MAX(pRect->x2(), pToRect->x2());
	int ycy = MAX(pRect->y2(), pToRect->y2());

	pToRect->x = xx;
	pToRect->y = yy;
	pToRect->cx = xcx - xx;
	pToRect->cy = ycy - yy;

	//WISP_RECT ToRect;
	//RectToRect(pRect, pToRect, &ToRect);
	//*pToRect = ToRect;
}

//Union
void RectToRect(const WISP_RECT *lpRect1, const WISP_RECT *lpRect2, WISP_RECT *pToRect)
{
	if (!lpRect1 || !lpRect2) return;
	
	if (IsInvalidRect(lpRect1))
	{
		*pToRect = *lpRect2;
		return;
	}

	if (IsInvalidRect(lpRect2))
	{
		*pToRect = *lpRect1;
		return;
	}

	pToRect->x = MIN(lpRect1->x, lpRect2->x);
	pToRect->y = MIN(lpRect1->y, lpRect2->y);

	pToRect->cx = MAX(lpRect1->x2(), lpRect2->x2()) - pToRect->x;
	pToRect->cy = MAX(lpRect1->y2(), lpRect2->y2()) - pToRect->y;
}

//Intersec
void RectFromRect(const WISP_RECT *pRect, WISP_RECT *pToRect)
{
	if (IsInvalidRect(pRect))
	{
		return;
	}

	if (IsInvalidRect(pToRect))
	{
		*pToRect = *pRect;
		return;
	}

	int xx = MAX(pRect->x, pToRect->x);
	int yy = MAX(pRect->y, pToRect->y);

	int xcx = MIN(pRect->x2(), pToRect->x2());
	int ycy = MIN(pRect->y2(), pToRect->y2());

	pToRect->x = xx;
	pToRect->y = yy;
	pToRect->cx = xcx - xx;
	pToRect->cy = ycy - yy;

	//WISP_RECT ToRect;
	//RectToRect(pRect, pToRect, &ToRect);
	//*pToRect = ToRect;
}

//Intersec
void RectFromRect(const WISP_RECT *lpRect1, const WISP_RECT *lpRect2, WISP_RECT *pToRect)
{
	if (!lpRect1 || !lpRect2) return;
	
	if (IsInvalidRect(lpRect1))
	{
		*pToRect = *lpRect2;
		return;
	}

	if (IsInvalidRect(lpRect2))
	{
		*pToRect = *lpRect1;
		return;
	}

	pToRect->x = MAX(lpRect1->x, lpRect2->x);
	pToRect->y = MAX(lpRect1->y, lpRect2->y);

	pToRect->cx = MIN(lpRect1->x2(), lpRect2->x2()) - pToRect->x;
	pToRect->cy = MIN(lpRect1->y2(), lpRect2->y2()) - pToRect->y;
}


void PointToRect(WISP_POINT *pPoint, const WISP_RECT *pRect) //???
	{
		pPoint->x -= pRect->x;
		pPoint->y -= pRect->y;
	}

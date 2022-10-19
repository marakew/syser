
#ifndef _UTILITY_HPP_
#define _UTILITY_HPP_

#include "../Code/define.h"

struct WISP_POINT
{
	int x;
	int y;
};

struct WISP_SIZE
{
	int cx;
	int cy;
};

struct WISP_RECT
{
	int x;
	int y;
	int cx;
	int cy;
//???
	int x2() const { return x+cx; }
	int y2() const { return y+cy; }

	bool Valid() const { return cx>0&&cy>0; }
	bool Invalid() const { return !Valid(); }
};

typedef WISP_RECT CWispRect;

#if 0
struct CWispRect : public WISP_RECT
{
	CWispRect(const WISP_POINT & PTL, const WISP_POINT & PTR)
	{
		x = PTL.x; y = PTL.y; cx = PTR.x; cy = PTR.y;
		
	}
	CWispRect(const WISP_POINT & PT, const WISP_SIZE & SZ)
	{
		cx = (x = point.x)+size.cx;
		cy = (y = point.y)+size.cy;
	}
	CWispRect(const WISP_RECT * pRC) { *this = *pRC; }
	CWispRect(const WISP_RECT & rc) { *this = rc; }
	CWispRect(long nx, long ny, long ncx, long ncy) { x=nx;y=ny;cx=ncx;cy=ncy; }
	CWispRect() {} //???

	void DeflateRect(int nX, int nY) //Decreases the width and height of CWispRect.
	{
		cx -= nX;
		cy -= nY;
	}
	void OffsetRect(int nX, int nY) //Moves CWispRect by the specified offsets.
	{
		x += nX;
		y += nY;
		cx += nX;
		cy += nY;
	}

	void operator=(const WISP_RECT & RC) { *this = RC; }

	bool operator==(const WISP_RECT & RC) { return x == RC.x && y == RC.y && cx == RC.cy && cy == RC.cy; }
	bool operator!=(const WISP_RECT & RC) { return !operator==(RC); }

	void operator+=(WISP_RECT & RC) { x += RC.x; y += RC.y; cx += RC.cx; cy += RC.cy; }
	void operator+=(WISP_SIZE SZ) { cx += size.cx; cy += size.cy; }
	void operator+=(WISP_POINT PT) { x += point.x; y += point.y; cx += point.x; cy += point.y; }

	void operator-=(WISP_RECT & RC) { x -= RC.x; y -= RC.y; cx -= RC.cx; cy -= RC.cy; }
	void operator-=(WISP_SIZE SZ) { cx -= size.cx; cy -= size.cy; }
	void operator-=(WISP_POINT PT) { x -= point.x; y -= point.y; cx -= point.x; cy -= point.y; }

	void operator&=(const WISP_RECT & RC);
	void operator|=(const WISP_RECT & RC);

	CWispRect operator+(WISP_SIZE SZ) { return CWispRect(x,y,cx+SZ.cx,cy+SZ.cy); }
	CWispRect operator+(WISP_RECT RC) { return CWispRect(x+RC.x,y+RC.y,cx+RC.cx,cy+RC.cy); }
	CWispRect operator+(WISP_RECT*RC) { return operator+(*RC); }
	CWispRect operator+(WISP_POINT PT) { return CWispRect(x+PT.x,y+PT.y,cx,cy); }

	CWispRect operator-(WISP_SIZE SZ) { return CWispRect(x,y,cx-SZ.cx,cy-SZ.cy); }
	CWispRect operator-(WISP_RECT & RC) { return CWispRect(x-RC.x,y-RC.y,cx-RC.cx,cy-RC.cy); }
	CWispRect operator-(WISP_RECT*RC) { return operator-(*RC); }
	CWispRect operator-(WISP_POINT PT) { return CWispRect(x-PT.x,y-PT.y,cx,cy); }

	CWispRect operator&(const WISP_RECT & RC);
	CWispRect operator|(const WISP_RECT & RC);
	int y2() { return y+cy; }
	int x2() { return x+cx; }
	operator WISP_RECT *() { return this; }
	void SetRect(int nx, int ny, int ncx, int ncy) //Sets the dimensions of CRect.
	{
		x = nx; y = ny; cx = ncx; cy = ncy;
	}
};
#endif

bool ClipLine(int *pX1, int *pY1, int *pX2, int *pY2, const WISP_RECT *pRect);
bool ClipRect(WISP_RECT *pRect, const WISP_RECT *pClipRect);
bool HasClipRect(const WISP_RECT *pRect, const WISP_RECT *pClipRect);

unsigned long FindFitColor(unsigned long Color, unsigned long *Pal);
int ColorStrNCpy(unsigned long *Des, WCHAR const *Src, int Count, unsigned char TXTColor, unsigned char BKColor);
int ColorStrNCpy(unsigned long *Des, char const *Src, int Count, unsigned char TXTColor, unsigned char BKColor);
int ColorStrCpy(unsigned long *Des, WCHAR const *Src, unsigned char TXTColor, unsigned char BKColor);
int ColorStrCpy(unsigned long *Des, char const *Src, unsigned char TXTColor, unsigned char BKColor);
int ColorStrNCat(unsigned long *Des, WCHAR const *Src, int Count, unsigned char TXTColor, unsigned char BKColor);
int ColorStrNCat(unsigned long *Des, char const *Src, int Count, unsigned char TXTColor, unsigned char BKColor);
int ColorStrCat(unsigned long *Des, WCHAR const *Src, unsigned char TXTColor, unsigned char BKColor);
int ColorStrCat(unsigned long *Des, char const *Src, unsigned char TXTColor, unsigned char BKColor);

inline unsigned long *ColorString(const WCHAR *lpString) { return (unsigned long *)(lpString+2); }
inline bool IsColorString(const WCHAR *lpString) { return lpString && lpString[0] == 0xFFFF; }

//inline bool IsInvalidRect(WISP_RECT *pRect); { return pRect->cx == 0 || pRect->cy == 0; }
inline bool IsInvalidRect(const WISP_RECT *pRect) { return pRect->cx <= 0 || pRect->cy <= 0; }

bool PtInRect(const WISP_RECT *pRect, int x, int y);
inline bool PtInRect(const WISP_RECT *pRect, const WISP_POINT & pPoint) { return PtInRect(pRect, pPoint.x, pPoint.y); }
inline bool PtInRect(const WISP_RECT *pRect, const WISP_POINT * pPoint) { return PtInRect(pRect, *pPoint); }
//Union
void RectToRect(const WISP_RECT *pRect, WISP_RECT *pToRect);
void RectToRect(const WISP_RECT *lpRect1, const WISP_RECT *lpRect2, WISP_RECT *pToRect);
//Intersec
void RectFromRect(const WISP_RECT *pRect, WISP_RECT *pToRect);
void RectFromRect(const WISP_RECT *lpRect1, const WISP_RECT *lpRect2, WISP_RECT *pToRect);

void PointToRect(WISP_POINT *pPoint, const WISP_RECT *pRect); //???

#endif

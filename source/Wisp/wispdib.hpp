
#ifndef _WISPDIB_HPP_
#define _WISPDIB_HPP_

#include "../Code/define.h"
#include "../Code/alt/altmap.hpp"
#include "../Code/tstrmem.hpp"
#include "utility.hpp"
#include "wispdraw.hpp"

enum PIXEL_FORMAT
{
  COLORREF_PF_DEFAULT = 0,
  COLORREF_PF_PAL = 1,
  COLORREF_PF_555 = 2, //15
  COLORREF_PF_565 = 3, //16
  COLORREF_PF_888 = 4, //24
  COLORREF_PF_COUNT = 5,
};

struct CWispDIB
{
	CWispDIB();
	~CWispDIB();

	WISP_FRAME_BUFFER_INFO m_PixelBuf;
	WISP_RECT m_PaintRect;
	unsigned int m_Type; //0=?,1=BMP,2=ICO,3=?
	unsigned long m_ColorKey;
	WISP_POINT m_HotPT;
	unsigned long *m_Pal;
	bool m_bAutoDelete;
	int m_RefCount;

	void CreatePaint();
	void FillColor(unsigned long Color);
	bool Create(int Width, int Height, unsigned int Type, WISP_POINT *pHotPT, unsigned long ColorKey);
	void SetType(unsigned int Type);
	void SetColorKey(unsigned long ColorKey);
	void SetHotPoint(WISP_POINT *pHotPT);
	void SetHotPoint(int x, int y);
	bool GetPixel(int PosX, int PosY, unsigned long *pPixel);
	void Blt(const WISP_RECT & SrcRect, CWispDIB *pDesDIB, int x, int y);
	bool Load(const char *FileName, int Index);
//	void LoadIcon(const ICON_DIR_ENTRY & IconDirEntry);
	bool LoadIcon(const char *FileName, int Index);
	void Destroy();
	bool Write(const char *FileName);
	void BltTiled(CWispDIB *pDIB); //???
};

enum WISP_DIBLIST_ID
{
  WISP_DIBLIST_MIN = 0,
  WISP_DIBLIST_MAX = 1,
  WISP_DIBLIST_NORMAL = 2,
  WISP_DIBLIST_CLOSE = 3,
  WISP_DIBLIST_HORZ_DEC = 4,
  WISP_DIBLIST_HORZ_INC = 5,
  WISP_DIBLIST_VERT_DEC = 6,
  WISP_DIBLIST_VERT_INC = 7,
  WISP_DIBLIST_COUNT = 8,
};

struct CWispDIBList
{
	CWispDIBList();
	~CWispDIBList();

	CWispDIB *m_DIBList;
	int m_RefCount;
	int m_Count;


	CWispDIB *GetDIB(int ID);

	bool Load(const char *FileName, int TileWidth, int TileHeight, int Count);
	bool LoadIcon(const char *FileName);
	void Destroy();
	void SetType(unsigned int Type);
	void SetColorKey(unsigned long ColorKey);
};

struct CWispDIBLib
{
	map<TAnsiString, CWispDIB> m_DIBMap;
	map<TAnsiString, CWispDIBList> m_DIBListMap;

	void ReleaseAll();

	void Remove(const char *FileName);

	CWispDIBList *GetDIBList(const char *FileName, int TileWidth, int TileHeight, int Count); //LoadDIBList
	//16,16,0,0xFF00FF
	CWispDIBList *LoadDIBList(const char *FileName, int TileWidth, int TileHeight, int Count, unsigned long ColorKey);

	CWispDIB *GetDIB(const char *FileName, int Index); //LoadDIB
	CWispDIB *LoadDIB(const char *FileName, int Index, unsigned long ColorKey);
};

#endif


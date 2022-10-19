
#ifndef _WISPDRAW_HPP_
#define _WISPDRAW_HPP_

#include "../Code/define.h"

struct CWispDrawHAL;

struct WISP_FRAME_BUFFER_INFO
{
	int Width;
	int Height;
	int Bpp;		//Bpp/8
	int PixelFormat;
	unsigned int Flag;
	unsigned char *VideoBuffer; //void *
	unsigned int VideoBufferSize;
	int LineDistance;
	CWispDrawHAL *pWispDrawHAL;


	void *GetStart(int x, int y, int n)
	{ 
		return (void*)((unsigned char*)VideoBuffer+x*(32/n) + y*LineDistance);
	}
};

struct CWispDrawHAL
{
	virtual void DrawPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color);
	virtual void DrawXorPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color);
	virtual void SetHLinePixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, int Count, unsigned long Color);
	virtual unsigned long GetPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y);
	virtual void CopyToPixel(void *pDes, void *pSrc, int Count);
	virtual void CopyFromPixel(void *pDes, void *pSrc, int Count);
};

struct CWispDraw4Bit : public CWispDrawHAL
{
	virtual void DrawPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color) override;
	virtual void DrawXorPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color) override;
	virtual void SetHLinePixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, int Count, unsigned long Color) override;
	virtual unsigned long GetPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y) override;
	virtual void CopyToPixel(void *pDes, void *pSrc, int Count) override;
	virtual void CopyFromPixel(void *pDes, void *pSrc, int Count) override;
};

struct CWispDraw8Bit : public CWispDrawHAL
{
	virtual void DrawPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color) override;
	virtual void DrawXorPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color) override;
	virtual void SetHLinePixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, int Count, unsigned long Color) override;
	virtual unsigned long GetPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y) override;
	virtual void CopyToPixel(void *pDes, void *pSrc, int Count) override;
	virtual void CopyFromPixel(void *pDes, void *pSrc, int Count) override;
};

struct CWispDraw16Bit : public CWispDrawHAL
{
	virtual void DrawPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color) override;
	virtual void DrawXorPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color) override;
	virtual void SetHLinePixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, int Count, unsigned long Color) override;
	virtual unsigned long GetPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y) override;
	virtual void CopyToPixel(void *pDes, void *pSrc, int Count) override;
	virtual void CopyFromPixel(void *pDes, void *pSrc, int Count) override;
};

struct CWispDraw24Bit : public CWispDrawHAL
{
	virtual void DrawPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color) override;
	virtual void DrawXorPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color) override;
	virtual void SetHLinePixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, int Count, unsigned long Color) override;
	virtual unsigned long GetPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y) override;
	virtual void CopyToPixel(void *pDes, void *pSrc, int Count) override;
	virtual void CopyFromPixel(void *pDes, void *pSrc, int Count) override;
};


struct CWispDraw32Bit : public CWispDrawHAL
{
	virtual void DrawPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color) override;
	virtual void DrawXorPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color) override;
	virtual void SetHLinePixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, int Count, unsigned long Color) override;
	virtual unsigned long GetPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y) override;
	virtual void CopyToPixel(void *pDes, void *pSrc, int Count) override;
	virtual void CopyFromPixel(void *pDes, void *pSrc, int Count) override;
};


#endif


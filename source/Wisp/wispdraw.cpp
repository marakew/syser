
#include <string.h>
#include "../Code/define.h"
#include "wispdraw.hpp"

	void CWispDrawHAL::DrawPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color)
		{
		}
	void CWispDrawHAL::DrawXorPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color)
		{
		}
	void CWispDrawHAL::SetHLinePixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, int Count, unsigned long Color)
		{
		}
	unsigned long CWispDrawHAL::GetPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y)
		{
			return 0; 
		}
	void CWispDrawHAL::CopyToPixel(void *pDes, void *pSrc, int Count)
		{
		}
	void CWispDrawHAL::CopyFromPixel(void *pDes, void *pSrc, int Count)
		{
		}
	//4

	void CWispDraw4Bit::DrawPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(4/8)+y*pFrameBuffer->LineDistance;
			if (x%2)
				*buf |= (Color&0xf);
			else	*buf |= (Color&0xf)<<4;
		}
	void CWispDraw4Bit::DrawXorPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(4/8)+y*pFrameBuffer->LineDistance;
			if (x%2)
				*buf ^= (Color&0xf);
			else	*buf ^= (Color&0xf)<<4;
		}
	void CWispDraw4Bit::SetHLinePixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, int Count, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(4/8)+y*pFrameBuffer->LineDistance;
			if (x%2)
			{
				*buf++ |= (Color&0xf);
				--Count;
			}

			while (Count--)
			{
				*buf++ |= (Color&0xf)<<4| (Color&0xf);
			}

		}
	unsigned long CWispDraw4Bit::GetPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(4/8)+y*pFrameBuffer->LineDistance;
			return x%2 ? (*buf&0xf) : (*buf&0xf)<<4; //???
		}
	void CWispDraw4Bit::CopyToPixel(void *pDes, void *pSrc, int Count)
		{
			BYTE *Des=(BYTE*)pDes;
			DWORD *Src=(DWORD*)pSrc;
			int Index = 0;
			while (Index < Count)
			{
				BYTE Color = (Src[Index] & 0xf)<<4;
				++Index;
				if (Index<Count)
					Color |= (Src[Index] & 0xf);
				++Index;
				*Des++=Color;
			}
		}
	void CWispDraw4Bit::CopyFromPixel(void *pDes, void *pSrc, int Count)
		{
			DWORD *Des=(DWORD*)pDes;
			BYTE *Src=(BYTE*)pSrc;
			int Index = 0;
			while (Index < Count)
			{
				Des[Index] = (*Src>>4)&0xf;
				++Index;
				if (Index<Count)
					Des[Index] |= (*Src & 0xf);
				++Index;
				++Src;
			}
		}

	//8

	void CWispDraw8Bit::DrawPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color)
			{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(8/8)+y*pFrameBuffer->LineDistance;
			*buf++ = (Color>>0)&0xff; //???
		}
	void CWispDraw8Bit::DrawXorPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(8/8)+y*pFrameBuffer->LineDistance;
			*buf++ ^= (Color>>0)&0xff;

		}
	void CWispDraw8Bit::SetHLinePixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, int Count, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(8/8)+y*pFrameBuffer->LineDistance;
			while (Count--)
			{
				*buf++= (Color>>0)&0xff;
			}
		}
	unsigned long CWispDraw8Bit::GetPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(8/8)+y*pFrameBuffer->LineDistance;
			return (unsigned long)buf[0];
		}
	void CWispDraw8Bit::CopyToPixel(void *pDes, void *pSrc, int Count)
		{
			BYTE *Des=(BYTE*)pDes;
			BYTE *Src=(BYTE*)pSrc;
			while (Count--)
			{
				*Des++ = *Src++;
				Src++;
				Src++;
				Src++;
			}
		}
	void CWispDraw8Bit::CopyFromPixel(void *pDes, void *pSrc, int Count)
		{
			BYTE *Des=(BYTE*)pDes;
			BYTE *Src=(BYTE*)pSrc;
			while (Count--)
			{
				*Des++ = *Src++;
				*Des++ = 0;
				*Des++ = 0;
				*Des++ = 0;
			}
		}

	//16

	void CWispDraw16Bit::DrawPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(16/8)+y*pFrameBuffer->LineDistance;
			*(WORD*)buf = ((Color >> 3) & 0x1F) | ((Color >> 5) & 0x7E0) | ((Color >> 8) & 0xF800);
		}
	void CWispDraw16Bit::DrawXorPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(16/8)+y*pFrameBuffer->LineDistance;
			*(WORD*)buf ^= Color & 0xffff;
		}
	void CWispDraw16Bit::SetHLinePixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, int Count, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(16/8)+y*pFrameBuffer->LineDistance;
			while (Count--)
			{
				*(WORD*)buf = ((Color >> 3) & 0x1f) | ((Color >> 5) & 0x7E0) | ((Color >> 8) & 0xF800);
				buf += 2;
			}
		}
//The RGB565 pixel data:
//    a red pixel   (1111 1000 0000 0000) F800  11
//    a green pixel (0000 0111 1110 0000) 7E0 5
//    a blue pixel  (0000 0000 0001 1111) 1F 0

	unsigned long CWispDraw16Bit::GetPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(16/8)+y*pFrameBuffer->LineDistance;
			WORD word = *(WORD*)buf;
			return ((word & 0xF8)<<12) | ((word & 0x7E)<<4) | ((word & 0x1F)<<0); //????
		}
	void CWispDraw16Bit::CopyToPixel(void *pDes, void *pSrc, int Count)
		{
			WORD *Des=(WORD*)pDes;
			DWORD *Src=(DWORD*)pSrc;
			while (Count--)
			{
				*Des=((*Src >> 3) & 0x1F) | ((*Src >> 5) & 0x7E0) | ((*Src >> 8) & 0xF800);
				++Des;
				++Src;
			}
		}
	void CWispDraw16Bit::CopyFromPixel(void *pDes, void *pSrc, int Count)
		{
			DWORD *Des=(DWORD*)pDes;
			WORD *Src=(WORD*)pSrc;
			while (Count--)
			{
				*Des=((*Src & 0x7C00) << 12) | ((*Src & 0x3E0) << 8) | ((*Src & 0x1F) << 4);
				++Des;
				++Src;
			}
		}

	//24

	void CWispDraw24Bit::DrawPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(24/8)+y*pFrameBuffer->LineDistance;
			*buf++ = (Color>>0)&0xff;
			*buf++ = (Color>>8)&0xff;
			*buf++ = (Color>>16)&0xff;
		}
	void CWispDraw24Bit::DrawXorPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(24/8)+y*pFrameBuffer->LineDistance;
			*buf++ ^= (Color>>0)&0xff;
			*buf++ ^= (Color>>8)&0xff;
			*buf++ ^= (Color>>16)&0xff;
		}
	void CWispDraw24Bit::SetHLinePixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, int Count, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(24/8)+y*pFrameBuffer->LineDistance;
			while (Count--)
			{
				*buf++= (Color>>0)&0xff;
				*buf++= (Color>>8)&0xff;
				*buf++= (Color>>16)&0xff;
			}
		}
	unsigned long CWispDraw24Bit::GetPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(24/8)+y*pFrameBuffer->LineDistance;
			return  ((unsigned long)buf[2]<<16) |
				((unsigned long)buf[1]<< 8) |
				((unsigned long)buf[0]<< 0);
		}
	void CWispDraw24Bit::CopyToPixel(void *pDes, void *pSrc, int Count)
		{
			BYTE *Des=(BYTE*)pDes;
			BYTE *Src=(BYTE*)pSrc;
			while (Count--)
			{
				*Des++ = *Src++;
				*Des++ = *Src++;
				*Des++ = *Src++;
					  Src++;
			}
		}                              	
	void CWispDraw24Bit::CopyFromPixel(void *pDes, void *pSrc, int Count)
		{
			BYTE *Des=(BYTE*)pDes;
			BYTE *Src=(BYTE*)pSrc;
			while (Count--)
			{
				*Des++ = *Src++;
				*Des++ = *Src++;
				*Des++ = *Src++;
				 Des++;
			}
		}

	//32

	void CWispDraw32Bit::DrawPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(32/8)+y*pFrameBuffer->LineDistance;
			*buf++ = (Color>>0)&0xff;
			*buf++ = (Color>>8)&0xff;
			*buf++ = (Color>>16)&0xff;
			*buf++ = (Color>>24)&0xff;
		}
	void CWispDraw32Bit::DrawXorPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(32/8)+y*pFrameBuffer->LineDistance;
			*buf++ ^= (Color>>0)&0xff;
			*buf++ ^= (Color>>8)&0xff;
			*buf++ ^= (Color>>16)&0xff;
			*buf++ ^= (Color>>24)&0xff;
		}
	void CWispDraw32Bit::SetHLinePixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y, int Count, unsigned long Color)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(32/8)+y*pFrameBuffer->LineDistance;
			while (Count--)
			{
				*buf++= (Color>>0)&0xff;
				*buf++= (Color>>8)&0xff;
				*buf++= (Color>>16)&0xff;
				*buf++= (Color>>24)&0xff;
			}
		}
	unsigned long CWispDraw32Bit::GetPixel(WISP_FRAME_BUFFER_INFO *pFrameBuffer, int x, int y)
		{
			BYTE *buf = (BYTE*)pFrameBuffer->VideoBuffer+x*(32/8)+y*pFrameBuffer->LineDistance;
			return  ((unsigned long)buf[3]<<24) |
				((unsigned long)buf[2]<<16) |
				((unsigned long)buf[1]<< 8) |
				((unsigned long)buf[0]<< 0);
		}
	void CWispDraw32Bit::CopyToPixel(void *pDes, void *pSrc, int Count)
		{
			BYTE *Des=(BYTE*)pDes;
			BYTE *Src=(BYTE*)pSrc;
			while (Count--)
			{
				*Des++ = *Src++;
				*Des++ = *Src++;
				*Des++ = *Src++;
				*Des++ = *Src++;
			}
		}
	void CWispDraw32Bit::CopyFromPixel(void *pDes, void *pSrc, int Count)
		{
			BYTE *Des=(BYTE*)pDes;
			BYTE *Src=(BYTE*)pSrc;
			while (Count--)
			{
				*Des++ = *Src++;
				*Des++ = *Src++;
				*Des++ = *Src++;
				*Des++ = *Src++;
			}
		}


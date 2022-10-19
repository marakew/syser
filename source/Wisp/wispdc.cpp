
#include "wispdc.hpp"
#include "wispbase.hpp"
#include "wispbasewnd.hpp"
#include "wispwnd.hpp"

	CWispDC::CWispDC()
	{
		m_pWispWnd = 0;
		m_pRect = 0;
		m_pClipRect = 0;
		m_TabAlign = 4;
		m_pColor = 0;
	}

	CWispDC::~CWispDC()
	{
		Destroy();
	}

	void CWispDC::Destroy()
	{
		//
	}

	void CWispDC::SetFontAndColor()
	{
		m_pWispBase = gpCurWisp;
		m_TextStyle = 0;
		m_TextColor = m_pWispBase->m_pWispDrawObj->m_crSystem[0];
		m_TextShadowColor = m_pWispBase->m_pWispDrawObj->m_crSystem[31];
		m_TextBKColor = m_pWispBase->m_pWispDrawObj->m_crSystem[1];
		m_pFont = &m_pWispBase->m_FontList[m_pWispBase->m_FontIndex];

		if (m_pWispWnd && m_pWispWnd->m_ParentWnd &&
			m_pWispWnd->m_ParentWnd->m_CtrlType == WISP_CTRL_FORM)
			SetTextType(WISP_FONT_6X12);
	}

	bool CWispDC::Create(WISP_FRAME_BUFFER_INFO *pFrameBuffer, WISP_RECT *pRect)
	{
		m_pWispWnd = 0;
		m_pWispBase = gpCurWisp;
		m_pFrameBuffer = pFrameBuffer;
		m_pRect = pRect;
		SetFontAndColor();
		return true;
	}

	CWispDC *CWispDC::CreateNull(WISP_FRAME_BUFFER_INFO *pFrameBuffer, WISP_RECT *pRect)
	{
		Create(pFrameBuffer, pRect);
		m_pClipRect = 0;
		return this;
	}

	bool CWispDC::Create(CWispDIB *pDIB)
	{
		return Create(&pDIB->m_PixelBuf, &pDIB->m_PaintRect);
	}

	void CWispDC::SetTextType(unsigned int FontIndex)
	{
		m_pFont = &m_pWispBase->m_FontList[FontIndex];
	}

	bool CWispDC::Create(CWispBaseWnd *pWispWnd, WISP_RECT *pRect)
	{
		m_pWispBase = pWispWnd->m_pWispBase;
		m_pWispWnd = pWispWnd;
		m_pRect = pRect;
		m_pFrameBuffer = &m_pWispBase->m_FrameBufferInfo;
		SetFontAndColor();
		return true;
	}

	bool CWispDC::IsRectVisible(WISP_RECT *pRect)
	{
		return !IsInvalidRect(pRect);
	}

	bool CWispDC::ClipDCRect(WISP_RECT *pRect)
	{
		if (m_pClipRect && !ClipRect(pRect, m_pClipRect))
			return false;

		if (!m_pWispWnd)
			return ClipRect(pRect, m_pRect);

		if (!m_pWispWnd->m_pUpdateRC)
			return false;

		WISP_RECT rc = *m_pWispWnd->m_pUpdateRC;

		rc.x += m_pWispWnd->m_ScrWindowRect.x;
		rc.y += m_pWispWnd->m_ScrWindowRect.y;

		if (!ClipRect(pRect, &rc) ||
		    !ClipRect(pRect, m_pRect))
			return false;

		if (m_pWispWnd == m_pWispBase->m_pRootWnd)
			return true;

		CWispBaseWnd *pWnd = m_pWispWnd->m_ParentWnd;

		if (!pWnd)
		{
		//	CWispBaseWnd *
			pWnd = m_pWispBase->m_pRootWnd;
		//	if (!pWnd)
		//		return true;

		//	if (!ClipRect(pRect, pWnd->m_Style & 0x8000 ? 
		//			&((CWispWnd*)pWnd)->m_ScrClientRect :
		//			&pWnd->m_ScrWindowRect))
		//		return false;
		//	return true;
		}

		while (pWnd)
		{
			if (!ClipRect(pRect, pWnd->m_Style & 0x8000 ? 
				&((CWispWnd*)pWnd)->m_ScrClientRect :
				&pWnd->m_ScrWindowRect))
				return false;
			//if (!pWnd) break;
			pWnd = pWnd->m_ParentWnd;
		}
		return true;
	}

	bool CWispDC::ClipDCLine(int *pX1, int *pY1, int *pX2, int *pY2)
	{
		if (m_pClipRect && !ClipLine(pX1, pY1, pX2, pY2, m_pClipRect))
			return false;

		if (!m_pWispWnd)
			return ClipLine(pX1, pY1, pX2, pY2, m_pRect);

		if (!m_pWispWnd->m_pUpdateRC)
			return false;

		WISP_RECT rc = *m_pWispWnd->m_pUpdateRC;

		rc.x += m_pWispWnd->m_ScrWindowRect.x;
		rc.y += m_pWispWnd->m_ScrWindowRect.y;

		if (!ClipLine(pX1, pY1, pX2, pY2, &rc) ||
		    !ClipLine(pX1, pY1, pX2, pY2, m_pRect))
			return false;

		if (m_pWispWnd == m_pWispBase->m_pRootWnd)
			return true;

		CWispBaseWnd *pWnd = m_pWispWnd->m_ParentWnd;

		if (!pWnd)
		{
		//	CWispBaseWnd *
			pWnd = m_pWispBase->m_pRootWnd;
		//	if (!pWnd)
		//		return true;

		//	if (!ClipLine(pX1, pY1, pX2, pY2, pWnd->m_Style & 0x8000 ? 
		//			&((CWispWnd*)pWnd)->m_ScrClientRect :
		//			&pWnd->m_ScrWindowRect))
		//		return false;
		//	return true;
		}

		while (pWnd)
		{
			if (!ClipLine(pX1, pY1, pX2, pY2, pWnd->m_Style & 0x8000 ? 
				&((CWispWnd*)pWnd)->m_ScrClientRect :
				&pWnd->m_ScrWindowRect))
				return false;
			//if (!pWnd) break;
			pWnd = pWnd->m_ParentWnd;
		}
		return true;
	}

	bool CWispDC::ClipDCPoint(WISP_POINT *pPT)
	{
		if (m_pClipRect && !PtInRect(m_pClipRect, pPT))
			return false;

		if (!m_pWispWnd)
			return PtInRect(m_pRect, pPT);

		if (!m_pWispWnd->m_pUpdateRC)
			return false;

		WISP_RECT rc = *m_pWispWnd->m_pUpdateRC;

		rc.x += m_pWispWnd->m_ScrWindowRect.x;
		rc.y += m_pWispWnd->m_ScrWindowRect.y;

		if (!PtInRect(&rc, pPT) ||
		    !PtInRect(m_pRect, pPT))
			return false;

		if (m_pWispWnd == m_pWispBase->m_pRootWnd)
			return true;

		CWispBaseWnd *pWnd = m_pWispWnd->m_ParentWnd;

		if (!pWnd)
		{
		//	CWispBaseWnd *
			pWnd = m_pWispBase->m_pRootWnd;
		//	if (!pWnd)
		//		return true;

		//	if (!PtInRect(pWnd->m_Style & 0x8000 ? 
		//			&((CWispWnd*)pWnd)->m_ScrClientRect :
		//			&pWnd->m_ScrWindowRect, pPT))
		//		return false;
		//	return true;
		}

		while (pWnd)
		{
			if (!PtInRect(pWnd->m_Style & 0x8000 ? 
				&((CWispWnd*)pWnd)->m_ScrClientRect :
				&pWnd->m_ScrWindowRect, pPT))
				return false;
			//if (!pWnd) break;
			pWnd = pWnd->m_ParentWnd;
		}
		return true;
	}

	void CWispDC::DrawSystemStandardIcon(WISP_RECT *rcBoundary, unsigned int uStyle, unsigned long color)
	{
		m_pWispBase->m_pWispDrawObj->DrawSystemStandardIcon(this, rcBoundary, uStyle, 0);
	}


	void CWispDC::CirclePlotPointsRightTop(int xCenter, int yCenter, int x, int y, unsigned long color, WISP_RECT *pRect)
	{
		if (PtInRect(pRect, xCenter+x, yCenter-y))
			m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, xCenter+x, yCenter-y, color);
		if (PtInRect(pRect, xCenter+y, yCenter-x))
			m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, xCenter+y, yCenter-x, color);
	}

	void CWispDC::CirclePlotPointsRightBottom(int xCenter, int yCenter, int x, int y, unsigned long color, WISP_RECT *pRect)
	{
		if (PtInRect(pRect, xCenter+y, yCenter+x))
			m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, xCenter+y, yCenter+x, color);
		if (PtInRect(pRect, xCenter+x, yCenter+y))
			m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, xCenter+x, yCenter+y, color);
	}

	void CWispDC::CirclePlotPointsLeftTop(int xCenter, int yCenter, int x, int y, unsigned long color, WISP_RECT *pRect)
	{
		if (PtInRect(pRect, xCenter-y, yCenter-x))
			m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, xCenter-y, yCenter-x, color);
		if (PtInRect(pRect, xCenter-x, yCenter-y))
			m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, xCenter-x, yCenter-y, color);
	}

	void CWispDC::CirclePlotPointsLeftBottom(int xCenter, int yCenter, int x, int y, unsigned long color, WISP_RECT *pRect)
	{
		if (PtInRect(pRect, xCenter-x, yCenter+y))
			m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, xCenter-x, yCenter+y, color);
		if (PtInRect(pRect, xCenter-y, yCenter+x))
			m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, xCenter-y, yCenter+x, color);
	}


	//...

	void CWispDC::DrawCircleRightTop(int x, int y, int radius, unsigned long color)
	{
		WISP_RECT rc = *m_pRect;

		int xCenter = m_pRect->x + x;
		int yCenter = m_pRect->y + y;
		ClipDCRect(&rc);

		int _x = 0;
		int _y = radius;
		int p = 1 - radius;
		while ( _x < _y )
		{
			++_x;
			if (p >= 0)
				p += 2 * (_x - --_y) +1;
			else	p += 2 * (_x) +1;
			CirclePlotPointsRightTop(xCenter, yCenter, _x, _y, color, &rc);
		}
	}

	void CWispDC::DrawCircleRightBottom(int x, int y, int radius, unsigned long color)
	{
		WISP_RECT rc = *m_pRect;

		int xCenter = m_pRect->x + x;
		int yCenter = m_pRect->y + y;
		ClipDCRect(&rc);

		int _x = 0;
		int _y = radius;
		int p = 1 - radius;
		while ( _x < _y )
		{
			++_x;
			if (p >= 0)
				p += 2 * (_x - --_y) +1;
			else	p += 2 * (_x) +1;
			CirclePlotPointsRightBottom(xCenter, yCenter, _x, _y, color, &rc);
		}
	}

	void CWispDC::DrawCircleLeftTop(int x, int y, int radius, unsigned long color)
	{
		WISP_RECT rc = *m_pRect;

		int xCenter = m_pRect->x + x;
		int yCenter = m_pRect->y + y;
		ClipDCRect(&rc);

		int _x = 0;
		int _y = radius;
		int p = 1 - radius;
		while ( _x < _y )
		{
			++_x;
			if (p >= 0)
				p += 2 * (_x - --_y) +1;
			else	p += 2 * (_x) +1;

			CirclePlotPointsLeftTop(xCenter, yCenter, _x, _y, color, &rc);
		}
	}
	void CWispDC::DrawCircleLeftBottom(int x, int y, int radius, unsigned long color)
	{
		WISP_RECT rc = *m_pRect;

		int xCenter = m_pRect->x + x;
		int yCenter = m_pRect->y + y;
		ClipDCRect(&rc);

		int _x = 0;
		int _y = radius;
		int p = 1 - radius;
		while ( _x < _y )
		{
			++_x;
			if (p >= 0)
				p += 2 * (_x - --_y) +1;
			else	p += 2 * (_x) +1;

			CirclePlotPointsLeftBottom(xCenter, yCenter, _x, _y, color, &rc);
		}
	}


	void CWispDC::DrawPixel(int x, int y, unsigned long color)
	{
		WISP_POINT PT;

		PT.x = x + m_pRect->x;
		PT.y = y + m_pRect->y;

		if (ClipDCPoint(&PT))
			m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, x, y, color);
	}

	void CWispDC::DrawLine(int x1, int y1, int x2, int y2, unsigned long color)
	{
		int Error;
		int XDelta;
		int YDelta;
		int step;

		x1 += m_pRect->x;
		x2 += m_pRect->x;

		y1 += m_pRect->y;
		y2 += m_pRect->y;

		Error = 0;

		if (ClipDCLine(&x1, &y1, &x2, &y2))
		{
			XDelta = ABS(x2-x1);
			YDelta = ABS(y2-y1);

			if (XDelta >= YDelta)
			{
				if (x1 > x2)
				{
					SWAP(x1, x2);
					SWAP(y1, y2);
				}
				step = (y1<y2)?1:-1;
				for (; x1 <= x2; ++x1)
				{
					if (Error >= XDelta)
					{
						Error -= XDelta;
						y1 += step;
					}
					Error += YDelta;
					m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, x1, y1, color);
				}
			} else
			{
				if (y1 > y2)
				{
					SWAP(x1, x2);
					SWAP(y1, y2);
				}
				step = (x1<x2)?1:-1;
				for (; y1 <= y2; ++y1)
				{
					if (Error >= YDelta)
					{
						Error -= YDelta;
						x1 += step;
					}
					Error += XDelta;
					m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, x1, y1, color);
				}
			}
		}
	}

	void CWispDC::DrawVLine(int x, int y1, int y2, unsigned long color)
	{
		x += m_pRect->x;
		y1 += m_pRect->y;
		y2 += m_pRect->y;

		if (ClipDCLine(&x, &y1, &x, &y2))
		{
			if (y1 > y2)
			{
				SWAP(y1,y2);
			}

			while (y1 <= y2)
			{
				m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer,
					x, y1++, color);
			}
		}
	}
	void CWispDC::DrawHLine(int x1, int x2, int y, unsigned long color)
	{
		x1 += m_pRect->x;
		x2 += m_pRect->x;
		y += m_pRect->y;

		if (ClipDCLine(&x1, &y, &x2, &y))
		{
			if (x2 < x1)
			{
				SWAP(x2,x1);
			}

			m_pFrameBuffer->pWispDrawHAL->SetHLinePixel(m_pFrameBuffer,
					x1, y, x2 - x1 + 1, color);
		}
	}

	void CWispDC::DrawVDashLine(int x, int y1, int y2, unsigned long color)
	{
		x += m_pRect->x;
		y1 += m_pRect->y;
		y2 += m_pRect->y;

		if (ClipDCLine(&x, &y1, &x, &y2))
		{
			for (int y = y1; y <= y2; y += 2)
				m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer,
						x, y, color);
		}
	}

	void CWispDC::DrawHDashLine(int x1, int x2, int y, unsigned long color)
	{
		x1 += m_pRect->x;
		x2 += m_pRect->x;
		y += m_pRect->y;

		if (ClipDCLine(&x1, &y, &x2, &y))
		{
			for (int x = x1; x <= x2; x += 2)
				m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer,
						x,y, color);
		}
	}


//	void CWispDC::DrawCircle(int, int, int, unsigned long);


	void CWispDC::DrawRect(WISP_RECT *pRc, unsigned long color)
	{
		DrawRect(pRc->x, pRc->y, pRc->cx, pRc->cy, color);
	}

	void CWispDC::DrawRect(int x, int y, int cx, int cy, unsigned long color)
	{
		DrawVLine(x, y, y+cy-1, color);
		DrawVLine(x+cx-1, y, y+cy-1, color);
		DrawHLine(x, x+cx-1, y, color);
		DrawHLine(x, x+cx-1, y+cy-1, color);
	}

	void CWispDC::DrawRoundRect(WISP_RECT *pRc, unsigned long color)
	{
		DrawVLine(pRc->x, pRc->y + 1, pRc->y2() - 2, color);
		DrawVLine(pRc->x2() - 1, pRc->y + 1, pRc->y2() - 2, color);
		DrawHLine(pRc->x + 1, pRc->x2() - 2, pRc->y, color);
		DrawHLine(pRc->x + 1, pRc->x2() - 2, pRc->y2() - 1, color);
	}

	void CWispDC::Draw3DRect(WISP_RECT *pRc, unsigned long crTopLeft, unsigned long crBottomRight)
	{
		DrawVLine(pRc->x, pRc->y, pRc->y2() - 1, crTopLeft);
		DrawVLine(pRc->x2() - 1, pRc->y, pRc->y2() - 1, crBottomRight);
		DrawHLine(pRc->x, pRc->x2() - 1, pRc->y, crTopLeft);
		DrawHLine(pRc->x, pRc->x2() - 1, pRc->y2() - 1, crBottomRight);
	}

	void CWispDC::DrawDashRect(WISP_RECT *pRc, unsigned long color)
	{
		DrawVDashLine(pRc->x, pRc->y, pRc->y2() - 1, color);
		DrawVDashLine(pRc->x2() - 1, pRc->y, pRc->y2() - 1, color);
		DrawHDashLine(pRc->x, pRc->x2() - 1, pRc->y, color);
		DrawHDashLine(pRc->x, pRc->x2() - 1, pRc->y2() - 1, color);
	}

	void CWispDC::DrawFullRect(WISP_RECT *pRc, unsigned long color)
	{
		DrawFullRect(pRc->x, pRc->y, pRc->cx, pRc->cy, color);
	}

	void CWispDC::DrawFullRect(int x, int y, int cx, int cy, unsigned long color)
	{
		WISP_RECT rc;

		rc.x = x + m_pRect->x;
		rc.y = y + m_pRect->y;
		rc.cy = cy;
		rc.cx = cx;
		if (ClipDCRect(&rc)) //&& IsRectVisible(&rc)
		{
			//gpWispBase->DrawFullRect(&rc, color);
			for (int v = rc.y; v < rc.y + rc.cy; ++v)
				m_pFrameBuffer->pWispDrawHAL->SetHLinePixel(m_pFrameBuffer,
					rc.x, v, rc.cx, color);
		}
	}

	inline COLOR GrayColor(COLOR color, char nChg, unsigned char start, unsigned char end)
	{
			unsigned char R = GetRColor(color);
			unsigned char G = GetGColor(color);
			unsigned char B = GetBColor(color);

			unsigned char Delta = (start - end);//*nChg;
			Delta *= nChg;
			R += Delta*3;
			G += Delta*5;
			B += Delta*1;

		return RGBColor(R,G,B);
	}

	unsigned long CWispDC::DrawXGrayChgFullRect(WISP_RECT *pRc, unsigned long color, int nChg)
	{
		WISP_RECT rc = *pRc;

		rc.x += m_pRect->x;
		rc.y += m_pRect->y;

		if (!ClipDCRect(&rc))
			return GrayColor(color,nChg,pRc->cx,0);

		int X = pRc->x + m_pRect->x;
		int CX = X + pRc->cx;

		for (int x = X; x < CX; ++x)
		{
			unsigned long Color = GrayColor(color,nChg,x,X);
			if (x >= rc.x && x < rc.x+rc.cx)
			{
				for (int Y = rc.y; Y < rc.y+rc.cy; ++Y)
				{
					m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer,	x, Y, Color);
				}
			}
		}
		return color;
	}
	void CWispDC::DrawXGrayColorFullRect(unsigned long color1, unsigned long color2, WISP_RECT *pRc)
	{
		WISP_RECT rc = *pRc;

		rc.x += m_pRect->x;
		rc.y += m_pRect->y;

		if (!ClipDCRect(&rc))
			return;

		unsigned long sC16 = ((unsigned long)((unsigned char)(color1>>16)))<<16;
		unsigned long sC08 = ((unsigned long)((unsigned char)(color1>> 8)))<<16;
		unsigned long sC00 = ((unsigned long)((unsigned char)(color1>> 0)))<<16;

		unsigned long dc16 = (((unsigned char)(color2>>16) - (unsigned char)(color1>>16))<<16)/pRc->cx;
		unsigned long dc08 = (((unsigned char)(color2>> 8) - (unsigned char)(color1>> 8))<<16)/pRc->cx;
		unsigned long dc00 = (((unsigned char)(color2>> 0) - (unsigned char)(color1>> 0))<<16)/pRc->cx;

		int X = pRc->x + m_pRect->x;
		int CX = X + pRc->cx;

		for (int x = X; x < CX; ++x)
		{
			if (x >= rc.x && X < rc.x+rc.cx)
			{
				sC16 += dc16;
				sC08 += dc08;
				sC00 += dc00;
				unsigned long color = (sC16>>16)<<16 | (sC08>>16)<<8 | (sC00>>16);

				for (int Y = rc.y; Y < rc.y+rc.cy; ++Y)
				{
					m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer,
						x, Y, color);
				}
			}
		}
	}

	unsigned long CWispDC::DrawYGrayChgFullRect(WISP_RECT *pRc, unsigned long color, int nChg)
	{
		WISP_RECT rc = *pRc;

		rc.x += m_pRect->x;
		rc.y += m_pRect->y;

		if (!ClipDCRect(&rc))
			return GrayColor(color,nChg,pRc->cy,0);

		int Y = pRc->y + m_pRect->y;
		int CY = Y + pRc->cy;

		for (int y = Y; y < CY; ++y)
		{
			unsigned long Color = GrayColor(color,nChg,y,Y);
			if (y >= rc.y && y < rc.y+rc.cy)
				m_pFrameBuffer->pWispDrawHAL->SetHLinePixel(m_pFrameBuffer, rc.x, y, rc.cx, Color);
		}
		return color;
	}
	void CWispDC::DrawYGrayColorFullRect(unsigned long color1, unsigned long color2, WISP_RECT *pRc)
	{
		WISP_RECT rc = *pRc;

		rc.x += m_pRect->x;
		rc.y += m_pRect->y;

		if (!ClipDCRect(&rc))
			return;

		unsigned long sC16 = ((unsigned long)((unsigned char)(color1>>16)))<<16;
		unsigned long sC08 = ((unsigned long)((unsigned char)(color1>> 8)))<<16;
		unsigned long sC00 = ((unsigned long)((unsigned char)(color1>> 0)))<<16;

		unsigned long dc16 = (((unsigned char)(color2>>16) - (unsigned char)(color1>>16))<<16)/pRc->cy;
		unsigned long dc08 = (((unsigned char)(color2>> 8) - (unsigned char)(color1>> 8))<<16)/pRc->cy;
		unsigned long dc00 = (((unsigned char)(color2>> 0) - (unsigned char)(color1>> 0))<<16)/pRc->cy;

		int Y = pRc->y + m_pRect->y;
		int CY = Y + pRc->cy;

		for (int y = Y; y < CY; ++y)
		{
			sC16 += dc16;
			sC08 += dc08;
			sC00 += dc00;
			unsigned long color = (sC16>>16)<<16 | (sC08>>16)<<8 | (sC00>>16);

			if (y >= rc.y && y < rc.y+rc.cy)
				m_pFrameBuffer->pWispDrawHAL->SetHLinePixel(m_pFrameBuffer,
					rc.x, y, rc.cx, color);
		}
	}


	void CWispDC::DrawXGrayColorFullRect2(unsigned long color1, unsigned long color2, WISP_RECT *pRc)
	{
		WISP_RECT rc = *pRc;

		rc.cx /= 2;
		DrawXGrayColorFullRect(color1, color2, &rc);

		rc.x += rc.cx;
		rc.cx = pRc->x + pRc->cx - rc.x;
		DrawXGrayColorFullRect(color2, color1, &rc);
	}
	void CWispDC::DrawYGrayColorFullRect2(unsigned long color1, unsigned long color2, WISP_RECT *pRc)
	{
		WISP_RECT rc = *pRc;

		rc.cy /= 2;
		DrawYGrayColorFullRect(color1, color2, &rc);

		rc.y += rc.cy;
		rc.cy = pRc->y + pRc->cy - rc.y;
		DrawYGrayColorFullRect(color2, color1, &rc);
	}

	void CWispDC::DrawFrameRect(WISP_RECT *pRc, bool bRaised, int Deep)
	{
		//if (IsRectVisible(pRc))
		DrawFrameRect(pRc->x, pRc->y, pRc->cx, pRc->cy, bRaised, Deep);
	}

	void CWispDC::DrawFrameRect(int x, int y, int cx, int cy, bool bRaised, int Deep)
	{
		unsigned long ColTop;
		unsigned long ColBottom;

		if (bRaised)
		{
			ColTop = m_pWispBase->m_pWispDrawObj->m_crSystem[29]; //swap
			ColBottom = m_pWispBase->m_pWispDrawObj->m_crSystem[30]; //swap
		} else
		{
			ColTop = m_pWispBase->m_pWispDrawObj->m_crSystem[30]; //swap
			ColBottom = m_pWispBase->m_pWispDrawObj->m_crSystem[29]; //swap
		}

		int Sign;

		if (Deep >= 0)
		{
			Sign = 1;
		} else
		{
			Deep = -Deep;
			Sign = -1;
		}

		if (Deep > 0)
		{
			int Inc = 0;
			do {

		//DrawHLine(x, x+cx-1, y, color);
		//DrawHLine(x, x+cx-1, y+cy-1, color);

		//DrawVLine(x, y, y+cy-1, color);
		//DrawVLine(x+cx-1, y, y+cy-1, color);

		DrawHLine(Inc + x, x - Inc + cx - 1, Inc + y, ColTop);
		DrawHLine(Inc + x, x - Inc + cx - 1, cy - Inc + y - 1, ColBottom);

		DrawVLine(Inc + x,          Inc + y, cy - Inc + y - 1, ColTop);
		DrawVLine(x - Inc + cx - 1, Inc + y, cy - Inc + y - 1, ColBottom);

				Inc += Sign;

			} while (--Deep);
		}
	}

	void CWispDC::DrawDIBTiled(CWispDIB *pDIB, WISP_RECT *pRect)
	{
		WISP_RECT DefRect;

		if (!pRect)
		{
			DefRect.y = 0;
			DefRect.x = 0;
			DefRect.cx = m_pRect->cx;
			DefRect.cy = m_pRect->cy;
			pRect = &DefRect;
		}

		for (int x = pRect->x; x < pRect->x + pRect->cx; x+=pDIB->m_PixelBuf.Width)
			for (int y = pRect->y; y < pRect->y + pRect->cy; y+=pDIB->m_PixelBuf.Height)
				DrawDIB(x, y, pDIB);
	}

	void CWispDC::DrawDIB(int x, int y, CWispDIB *pDIB)
	{
		if (pDIB->m_PixelBuf.VideoBuffer == 0)
			return;

		WISP_RECT rect;

		int xoff = x - pDIB->m_HotPT.x;
		int yoff = y - pDIB->m_HotPT.y;

		rect.x = xoff + m_pRect->x;
		rect.y = yoff + m_pRect->y;

		rect.cx = pDIB->m_PixelBuf.Width;
		rect.cy = pDIB->m_PixelBuf.Height;

		if (!ClipDCRect(&rect))
			return;

		unsigned long *pBuf = (unsigned long*)((unsigned char*)pDIB->m_PixelBuf.VideoBuffer+
				(rect.x-m_pRect->x-xoff)*(32/8) +
				(rect.y-m_pRect->y-yoff)*pDIB->m_PixelBuf.LineDistance);

		//if (rect.cy <= 0)
		//	return;

		for (int ny = 0; ny < rect.cy; ++ny)
		{
			if (pDIB->m_Type == 1) //BMP COLORREF_PF_PAL
			{
				m_pFrameBuffer->pWispDrawHAL->CopyToPixel(
						(char*)m_pFrameBuffer->VideoBuffer
						+ rect.x * (m_pFrameBuffer->Bpp/8)
						+ (rect.y + ny)* m_pFrameBuffer->LineDistance,
						pBuf,
						rect.cx);
				pBuf += pDIB->m_PixelBuf.LineDistance/4;
			} else
			if (pDIB->m_Type == 2) //ICO COLORREF_PF_555
			{
				for (int nx = 0; nx < rect.cx; ++nx, ++pBuf)
				{
					if (*pBuf != pDIB->m_ColorKey)
					{
						m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer,
							nx+rect.x, ny+rect.y, *pBuf);
					}
				}
				pBuf += pDIB->m_PixelBuf.LineDistance/4 - rect.cx;

			} else
			if (pDIB->m_Type == 3) //??? COLORREF_PF_565
			{
				for (int nx = 0; nx < rect.cx; ++nx, ++pBuf)
				{
					if (*pBuf != pDIB->m_ColorKey)
					{
						m_pFrameBuffer->pWispDrawHAL->DrawXorPixel(m_pFrameBuffer,
							nx+rect.x, ny+rect.y, *pBuf);
					}
				}
				pBuf += pDIB->m_PixelBuf.LineDistance/4 - rect.cx;
			}
		}
	}
	void CWispDC::DrawDIB(WISP_RECT *pRect, CWispDIB *pDIB)
	{
		DrawDIB(
			pRect->x + (pRect->cx - pDIB->m_PaintRect.cx) / 2,
			pRect->y + (pRect->cy - pDIB->m_PaintRect.cy) / 2,
			pDIB);
	}

	void CWispDC::DrawDIBGray(int x, int y, CWispDIB *pDIB)
	{
		if (pDIB->m_PixelBuf.VideoBuffer == 0)
			return;

		WISP_RECT rect;

		int xoff = x - pDIB->m_HotPT.x;
		int yoff = y - pDIB->m_HotPT.y;

		rect.x = xoff + m_pRect->x;
		rect.y = yoff + m_pRect->y;

		rect.cx = pDIB->m_PixelBuf.Width;
		rect.cy = pDIB->m_PixelBuf.Height;

		if (!ClipDCRect(&rect))
			return;

		unsigned long *pBuf = (unsigned long*)((unsigned char*)pDIB->m_PixelBuf.VideoBuffer+
				(rect.x - m_pRect->x - xoff)*(32/8) +
				(rect.y - m_pRect->y - yoff)*pDIB->m_PixelBuf.LineDistance);

		//if (rect.cy <= 0)
		//	return;

		for (int ny = 0; ny < rect.cy; ++ny)
		{
			if (pDIB->m_Type == 0 || //??? COLORREF_PF_DEFAULT
			    pDIB->m_Type == 1 || //BMP COLORREF_PF_PAL
			    pDIB->m_Type == 2) //ICO COLORREF_PF_555
			{
				for (int nx = 0; nx < rect.cx; ++nx, ++pBuf)
				{
					if (*pBuf != pDIB->m_ColorKey)
					{
						unsigned char TextClr = LOW_BYTE(pBuf[0]);
						unsigned char BKClr = HIGH_BYTE(pBuf[0]);
						unsigned long Clr = BKClr*5+TextClr*4;

						m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer,
							nx+rect.x, ny+rect.y, 
							Clr <= 0x480 ?
							m_pWispBase->m_pWispDrawObj->m_crSystem[20] :
							m_pWispBase->m_pWispDrawObj->m_crSystem[115]);
					}
				}
				pBuf += pDIB->m_PixelBuf.LineDistance/4 - rect.cx;

			} else
			if (pDIB->m_Type == 3) //??? COLORREF_PF_565
			{
				for (int nx = 0; nx < rect.cx; ++nx, ++pBuf)
				{
					if (*pBuf != pDIB->m_ColorKey)
					{
						m_pFrameBuffer->pWispDrawHAL->DrawXorPixel(m_pFrameBuffer,
							nx+rect.x, ny+rect.y, *pBuf);
					}
				}
				pBuf += pDIB->m_PixelBuf.LineDistance/4 - rect.cx;
			}
		}
	}
	void CWispDC::DrawDIBGray(WISP_RECT *pRect, CWispDIB *pDIB)
	{
		DrawDIBGray(
			pRect->x + (pRect->cx - pDIB->m_PaintRect.cx) / 2,
			pRect->y + (pRect->cy - pDIB->m_PaintRect.cy) / 2,
			pDIB);
	}

// text
	int CWispDC::GetTextHeight(WCHAR Char)
	{
		return m_pFont->m_Height;
	}

	int CWispDC::GetTextLen(const WCHAR *lpString, int nStrLen) //nCharsBits
	{
		int nRetLen = 0;
		if (lpString == 0)
			return 0;

		if (!IsColorString(lpString))
		{
			int nCount = 0;
			int Len = TStrLen(lpString);
			int nWidth = 0; //= nStrLen ??? TODO
			int nMaxLen = 0;
			for (int i = 0; i < Len; ++i)
			{
				const WCHAR Char = lpString[i];
				if (Char == 9)
				{
					int nTmp = ROUNDREVERSE(nCount, m_TabAlign);
					nWidth = nTmp * m_pFont->GetCharWidth(0x20);
					nMaxLen += nWidth;
					nCount += nTmp;
				} else
				if (Char != 10)
				{
					nWidth = m_pFont->GetCharWidth(Char);
					nMaxLen += nWidth;
					++nCount;
				}
				if (nStrLen >= nMaxLen - nWidth && nStrLen < nMaxLen)
					return i;
			}
			return -1;
		}

		//ColorStr
		const unsigned long *lpColorString = (const unsigned long *)lpString;
		++lpColorString; //skip Color

			int nCount = 0;
			int Len = TStrLen(lpColorString);
			int nWidth = 0; //= nStrLen ??? TODO
			int nMaxLen = 0;
			for (int i = 0; i < Len; ++i)
			{
				WCHAR Char = lpColorString[i];
				if (Char == 9)
				{
					int nTmp = ROUNDREVERSE(nCount, m_TabAlign);
					nWidth = nTmp * m_pFont->GetCharWidth(0x20);
					nMaxLen += nWidth;
					nCount += nTmp;
				} else
				if (Char != 10)
				{
					nWidth = m_pFont->GetCharWidth(Char);
					nMaxLen += nWidth;
					++nCount;
				}
				if (nStrLen >= nMaxLen - nWidth && nStrLen < nMaxLen)
					return i;
			}
			return -1;
	}

	int CWispDC::GetTextExtent(const WCHAR *lpString, int nStrLen, int nCount)
	{
		if (lpString == 0)
			return 0;

		if (!IsColorString(lpString))
		{
			if (nStrLen == -1)
				nStrLen = TStrLen(lpString);
			int nRetLen = 0;
			for (int i = 0; i < nStrLen; ++i)
			{
				WCHAR Char = lpString[i];
				if (Char == 9)
				{
					int nTmp = ROUNDREVERSE(nCount, m_TabAlign);
					nRetLen += nTmp * m_pFont->GetCharWidth(0x20);
					nCount += nTmp;
				} else
				if (Char != 10)
				{
					nRetLen += m_pFont->GetCharWidth(Char);
					++nCount;
				}
			}
			return nRetLen;
		}

		//ColorStr
		const unsigned long *lpColorString = (const unsigned long *)lpString;
		++lpColorString; //skip Color

			if (nStrLen == -1)
				nStrLen = TStrLen(lpColorString);
			int nRetLen = 0;
			for (int i = 0; i < nStrLen; ++i)
			{
				WCHAR Char = lpColorString[i];
				if (Char == 9)
				{
					int nTmp = ROUNDREVERSE(nCount, m_TabAlign);
					nRetLen += nTmp * m_pFont->GetCharWidth(0x20);
					nCount += nTmp;
				} else
				if (Char != 10)
				{
					nRetLen += m_pFont->GetCharWidth(Char);
					++nCount;
				}
			}
			return nRetLen;
	}

	int CWispDC::GetTextExtentTabExpend(const WCHAR *lpString, int nTabPixelWidth, int nCount)
	{
		int nRetLen = 0;
		if (nCount == -1)
			nCount = TStrLen(lpString);
		for (int i = 0; i < nCount; ++i)
		{
			if (lpString[i] < 256)
			{
				if (lpString[i] == 9)
					nRetLen += ROUNDREVERSE(nRetLen, nTabPixelWidth);
				else	nRetLen += m_pFont->m_CharLenAry[lpString[i]];
			} else
			{
				nRetLen += 16;
			}
		}
		return nRetLen;
	}

	int CWispDC::GetTextExtentLineTabExpend(const WCHAR *lpString, int nTabPixelWidth, int nCount)
	{
		int nRetLen = 0;
		if (nCount == -1)
			nCount = TStrLen(lpString);
		for (int i = 0; i < nCount; ++i)
		{
			if (lpString[i] == 10 || lpString[i] == 13)
				break;

			if (lpString[i] < 256)
			{
				if (lpString[i] == 9)
					nRetLen += ROUNDREVERSE(nRetLen, nTabPixelWidth);
				else	nRetLen += m_pFont->m_CharLenAry[lpString[i]];
			} else
			{
				nRetLen += 16;
			}
		}
		return nRetLen;
	}

	int CWispDC::CalcTextLineNum(const WCHAR *lpString, int nStrLen)
	{
		int nLineNum = 0;
		int j = 0;
		int i = 0;
		while (j++ < nStrLen && i < nStrLen)
		{
			int start = i;
			int len = 0;
			while (i < nStrLen &&
				(lpString[i] != 13 && lpString[i] != 10))
			{
				++i;
				++len;
			}
			if (len == 0 && i >= nStrLen) break;
			++nLineNum;

			if (++i >= nStrLen) break;

			if ((lpString[i] == 10 || lpString[i] == 13) && lpString[i-1] != lpString[i])
				++i;
		}
		return nLineNum;
	}

	int CWispDC::CalcDrawTextRect(const WCHAR *lpString, WISP_RECT *lpRect, int nStrLen, unsigned int uFormat, int nTabPixelWidth)
	{
		if (uFormat & 0x20)
		{
			if (uFormat & 0x40)
				lpRect->cx = GetTextExtentTabExpend(lpString, nTabPixelWidth, nStrLen);
			else	lpRect->cx = GetTextExtent(lpString, nStrLen, 0);
			lpRect->cy = m_pFont->m_Height;
			return lpRect->cy;
		}

		int nWidth = 0;
		int nLineNum = 0;
		int i = 0;
		while (i < nStrLen)
		{
			int start = i;
			int len = 0;
			while (i < nStrLen && 
				(lpString[i] != 13 && lpString[i] != 10))
			{
				++i;
				++len;
			}
			if (len == 0 && i >= nStrLen) break;
			++nLineNum;

			int nExten;
			if (uFormat & 0x40)
				nExten = GetTextExtentLineTabExpend(&lpString[start], nTabPixelWidth, len);
			else	nExten = GetTextExtent(&lpString[start], len, 0);
			if (uFormat & 0x00400000 && lpRect->cx < nExten)
			{
				while (len>0)
				{
					int n = GetMaxShowCharNum(&lpString[start], lpRect->cx, 0, len);
					++nLineNum;
					len -= n;
					start += n;
				}
				--nLineNum;
			}
			MIN_LIMIT(nWidth, nExten);

			if (++i >= nStrLen) break;
			if ((lpString[i] == 10 || lpString[i] == 13) && lpString[i-1] != lpString[i])
				++i;
		}
		lpRect->cx = nWidth;
		lpRect->cy = m_pFont->m_Height * nLineNum;
		return lpRect->cy;
	}

	int CWispDC::GetMaxShowCharNum(const WCHAR *lpString, int nMaxWidth, int nTabPixelWidth, int nCount)
	{
		int nRetLen = 0;
		int sum = 0;
		if (nCount == -1)
			nCount = TStrLen(lpString);
		if (nTabPixelWidth)
		{
			for (int i = 0; i < nCount; ++i)
			{	
				if (lpString[i] == 9)
					sum += ROUNDREVERSE(sum, nTabPixelWidth);
				else	sum += GetTextExtent(&lpString[i], 1, 0);
				if (sum >= nMaxWidth)
					break;
				++nRetLen;
				if (nRetLen >= nCount)
					return nRetLen;
			}
			if (lpString[nRetLen] == 9)
				++nRetLen;
		} else
		{
			for (int i = 0; i < nCount; ++i)
			{	
				sum += GetTextExtent(&lpString[i], 1, 0);
				if (sum >= nMaxWidth)
					break;
				++nRetLen;
			}
		}
		return nRetLen;
	}

	int CWispDC::DrawTabExpendString(int x, int y, const WCHAR *lpString, int nStringRealLen, WISP_RECT rc, int nTabPixelWidth)
	{
		WCHAR TabExpendString[9];
		TMemSet(TabExpendString, (WCHAR)0x20, 9);
		int TabExpendStringPixelWidth = GetTextExtent(TabExpendString, -1, 0);

		int i = 0;
		int nStartx = x;
		while (i < nStringRealLen)
		{
			int start = i;
			int nLen = 0;
			while (i < nStringRealLen &&
				(lpString[i] != 9))
			{
				++i;
				++nLen;
			}

			int nx = nStartx + GetTextExtent(&lpString[start], nLen, 0);
			if (nx < 0 || nStartx > m_pRect->cx)
				break;
			DrawString(nStartx, y, &lpString[start], nLen, 0, 0);

			int tab = ROUNDREVERSE((nx - x), nTabPixelWidth);
			int tabAlign = ALIGNUP(tab, TabExpendStringPixelWidth);
			nStartx = nx + tab;
			for (int k = 0; k < tabAlign; ++k)
			{
				int n = nx + TabExpendStringPixelWidth;
				if (n < 0) break;
				if (nx > m_pRect->cx) break;
				DrawString(nx, y, TabExpendString, -1, 0, 0);
				nx = n;
			}
			++i;
		}
		return 0;
	}

	void CWispDC::AdjustNoClipRect(WISP_RECT *lpRect, unsigned int uFormat)
	{
		if (uFormat & 2)
		{
			if (lpRect->x > 0)
			{
				lpRect->cx += lpRect->x;
				lpRect->x = 0;
			}
		} else
		if (uFormat & 1)
		{
			if (lpRect->x > 0)
			{
				lpRect->cx += lpRect->x;
				lpRect->x = 0;
			}

			if (lpRect->x + lpRect->cx < m_pRect->cx)
				lpRect->cx = m_pRect->cx - lpRect->x;
		} else
		{
			if (lpRect->x + lpRect->cx < m_pRect->cx)
				lpRect->cx = m_pRect->cx - lpRect->x;
		}
	}

//0x0000 0001	X
//0x0000 0002	X
//0x0000 0004	Y
//0x0000 0008	Y

//0x0000 0010
//0x0000 0020 STR
//0x0000 0040 TAB
//0x0000 0080

//0x0000 0100 CLIP
//0x0000 0200
//0x0000 0400 CALCRECT
//0x0000 0800

//0x0000 1000
//0x0000 2000
//0x0000 4000 FILE PATH
//0x0000 8000 DIR PATH

//0x0001 0000 PATH ELIPSIS
//0x0002 0000
//0x0004 0000 FS
//0x0008 0000

//0x0010 0000
//0x0020 0000
//0x0040 0000 MAX
//0x0080 0000

	int CWispDC::DrawString(const WCHAR *lpString, WISP_RECT *lpRect, unsigned int uFormat, unsigned int nShowLen, int nPreLineMaxCharNum)
	{
		struct stShowString
		{
			int coordinateX;
			const WCHAR *ShowString;
			int nShowLen;
			int nPixelWidth;
		} ShowString[3];

		if (lpString == 0) return 0;
		int nStringRealLen = TStrLen(lpString);
		if (nStringRealLen == 0) return 0;
		if (nShowLen && nShowLen < nStringRealLen)
			nStringRealLen = nShowLen;

		WISP_RECT rc = *lpRect;

		int nTabPixelWidth = 6;
		if (uFormat & 0x40)
		{
			int n8 = 8;
			if (uFormat & 0x80)
			{
				n8 = (uFormat >> 8) & 0xff; //???
				if (n8==0) n8 = 8;
				uFormat &= ~0x1F00;
			}
			nTabPixelWidth *= n8;
		}
		int nLineNum = CalcTextLineNum(lpString, nStringRealLen);

		if (uFormat & 0x400)
			return CalcDrawTextRect(lpString, lpRect, nStringRealLen, uFormat, nTabPixelWidth);

		int nCenterPos = rc.x + rc.cx/2;

		int nEllipsisStringPixelWidth; //??? = 0;
		if (uFormat & 0x4C000)
			nEllipsisStringPixelWidth = GetTextExtent(WSTR("..."), -1, 0);

		if (uFormat & 0x100)
			AdjustNoClipRect(&rc, uFormat);

		if ((uFormat & 0x20) == 0)
		{
			if (uFormat & 4)
			{
				if (rc.cy > nLineNum * m_pFont->m_Height)
					rc.y += (rc.cy - nLineNum * m_pFont->m_Height)/2;
				uFormat &= ~4;
			}
			uFormat = (uFormat & ~8) | 0x20;
			int nTotalLen = nStringRealLen;
			int nCurLine = 0;
			int n = 0;
			int i = 0;
			while (i < nStringRealLen && n++ < nLineNum)
			{
				int len = 0;
				int start = i;
				while (i < nStringRealLen &&
					(lpString[i] != 13 && lpString[i] != 10))
				{
					++i;
					++len;
				}
				if (len==0)
				{
					if (len>=nStringRealLen) break;
					goto skip;
				}

				if (nTotalLen <= 0) break;
				MAX_LIMIT(len, nTotalLen);
				nTotalLen -= len;
			//move under
				int nStringPixelWidth;
				if (uFormat & 0x40)
					nStringPixelWidth = GetTextExtentLineTabExpend(&lpString[start], nTabPixelWidth, len);
				else	nStringPixelWidth = GetTextExtent(&lpString[start], len, 0);

				if (uFormat & 0x400000)
				{
					if (nStringPixelWidth > lpRect->cx)
					{
						while (len>0)
						{
							int MaxShow = GetMaxShowCharNum(&lpString[start], lpRect->cx, uFormat&0x40?nTabPixelWidth:0, len);
							DrawString(&lpString[start], &rc, uFormat, MaxShow, -1);
							++nCurLine;
							len -= MaxShow;
							start += MaxShow;
							if (len)
							{
								rc.y += m_pFont->m_Height;
								rc.cy -= m_pFont->m_Height;
							}
							if (rc.cy <= 0)break;
						}
						--nCurLine;
					}
				} else
				if (uFormat & 0x800000)
				{
						while (len>=nPreLineMaxCharNum)
						{
							DrawString(&lpString[start], &rc, uFormat, nPreLineMaxCharNum, -1);
							++nCurLine;
							len -= nPreLineMaxCharNum;
							start += nPreLineMaxCharNum;
							if (len)
							{
								rc.y += m_pFont->m_Height;
								rc.cy -= m_pFont->m_Height;
							}
							if (rc.cy <= 0) break;
						}
						if (len==0)
							--nCurLine;
						else	DrawString(&lpString[start], &rc, uFormat, len, -1);

				} else
				if (len)
				{
					DrawString(&lpString[start], &rc, uFormat, len, -1);
				}
		skip:
				if (++i>=nStringRealLen) break;
				--nTotalLen;
				if ((lpString[i] == 10 || lpString[i] == 13) && lpString[i-1] != lpString[i])
				{
					++i;
					--nTotalLen;
				}
				rc.y += m_pFont->m_Height;
				rc.cy -= m_pFont->m_Height;
				if (rc.cy <= 0) break;
				++nCurLine;
			}
			return m_pFont->m_Height * nCurLine;
		}

		//0x20

		if (rc.x + rc.cx < 0 || rc.x > m_pRect->cx)
			return m_pFont->m_Height;

		int coordinateX = rc.x;
		int coordinateY = rc.y;

		int nStringPixelWidth;
		if (uFormat & 0x40)
			nStringPixelWidth = GetTextExtentTabExpend(lpString, nTabPixelWidth, nStringRealLen);
		else	nStringPixelWidth = GetTextExtent(lpString, nStringRealLen, 0);
		//
		if (uFormat & 1)	coordinateX = nCenterPos - nStringPixelWidth/2;
		else
		if (uFormat & 2)	coordinateX = rc.x + rc.cx - nStringPixelWidth;
		//
		if (uFormat & 4)	coordinateY = rc.y + (rc.cy - m_pFont->m_Height)/2;
		else
		if (uFormat & 8)	coordinateY = rc.y + (rc.cy - m_pFont->m_Height);

		ShowString[0].coordinateX = coordinateX;
		ShowString[0].ShowString = lpString;
		ShowString[0].nShowLen = nStringRealLen;
		ShowString[0].nPixelWidth = nStringPixelWidth;
		int nShowStringCounter = 1;

		if (nStringPixelWidth > rc.cx)
		{
			if (uFormat & 0x4000)
			{
				int nPathStrLen = nStringRealLen - 2;
				int nFileNameStrLen = 1;
				while (nPathStrLen>=0)
				{
					if (lpString[nPathStrLen] != '\\')
					{
						--nPathStrLen;
						++nFileNameStrLen;
						continue;
					}

					if (nPathStrLen >= 3)
					{
						const WCHAR *PathName = nPathStrLen != 0 ? lpString : 0;
						++nFileNameStrLen;
						if (&lpString[nPathStrLen] && nPathStrLen > 3)
						{
							int nEllipsesBegineOffset = 0; //???
							int nFileNamePixelWidth;
							if (uFormat & 0x40)
								nFileNamePixelWidth = GetTextExtentTabExpend(&lpString[nPathStrLen], nStringPixelWidth, nFileNameStrLen);
							else	nFileNamePixelWidth = GetTextExtent(&lpString[nPathStrLen], nFileNameStrLen, 0);

							if (nFileNamePixelWidth + nEllipsisStringPixelWidth >= rc.cx)
							{
								if (uFormat & 2) coordinateX = rc.x + rc.cx - nFileNamePixelWidth;
								else
								if (uFormat & 1) coordinateX = nCenterPos + nFileNamePixelWidth/2;

								nEllipsesBegineOffset = nStringPixelWidth;

								ShowString[0].coordinateX = coordinateX;
								ShowString[0].ShowString = WSTR("...");
								ShowString[0].nShowLen = 3;
								ShowString[0].nPixelWidth = nEllipsisStringPixelWidth;

								ShowString[1].coordinateX = ShowString[0].coordinateX + ShowString[0].nPixelWidth;
								ShowString[1].ShowString = &lpString[nPathStrLen];
								ShowString[1].nShowLen = nFileNameStrLen;
								ShowString[1].nPixelWidth = nFileNamePixelWidth;
								nShowStringCounter = 2;
							} else
							{
								int w = rc.cx - nFileNamePixelWidth - nEllipsisStringPixelWidth;
								int nPathShowStrLen;
								int nPathNamePixelWidth;
								if (uFormat & 0x40)
								{
									nPathShowStrLen = GetMaxShowCharNum(PathName, w, nTabPixelWidth, nPathStrLen);
									nPathNamePixelWidth = GetTextExtentTabExpend(PathName, nStringPixelWidth, nPathShowStrLen);
								} else
								{
									nPathShowStrLen = GetMaxShowCharNum(PathName, w, 0, nPathStrLen);
									nPathNamePixelWidth = GetTextExtent(PathName, nPathShowStrLen, 0);
								}
								int delta = w - nPathNamePixelWidth;

								nEllipsesBegineOffset = nPathShowStrLen;

								if (nPathStrLen - nPathShowStrLen < 3) break;

								if (uFormat & 2) coordinateX += delta;
								else
								if (uFormat & 1) coordinateX += delta/2;

								ShowString[0].coordinateX = coordinateX;
								ShowString[0].ShowString = PathName;
								ShowString[0].nShowLen = nPathShowStrLen;
								ShowString[0].nPixelWidth = nPathNamePixelWidth;

								ShowString[1].coordinateX = ShowString[0].coordinateX + ShowString[0].nPixelWidth;
								ShowString[1].ShowString = WSTR("...");
								ShowString[1].nShowLen = 3;
								ShowString[1].nPixelWidth = nEllipsisStringPixelWidth;

								ShowString[2].coordinateX = ShowString[1].coordinateX + ShowString[1].nPixelWidth;
								ShowString[2].ShowString = &lpString[nPathStrLen];
								ShowString[2].nShowLen = nFileNameStrLen;
								ShowString[2].nPixelWidth = nFileNamePixelWidth;
								nShowStringCounter = 3;
							}

							if (uFormat & 0x10000)
							{
						#if 0 //TODO ASSERT const*
								PathName[nEllipsesBegineOffset+0]='.';
								PathName[nEllipsesBegineOffset+1]='.';
								PathName[nEllipsesBegineOffset+2]='.';
								int i = 0;
								for (i = 0; i < nFileNameStrLen; ++i)
								{
									PathName[nEllipsesBegineOffset+3+i]=lpString[i];
								}
								lpString[i]=0;
						#endif
							}
						}
					}
					break;
				}
			} else
			if (uFormat & 0x8000)
			{
				int l;
				if (uFormat & 3)
					l = rc.cx - nEllipsisStringPixelWidth;
				else	l = rc.cx - nEllipsisStringPixelWidth/2 + 1;

				int Len = 0; //???
				int PixelWidth = 0; //???
				if (uFormat & 0x40)
				{
					Len = GetMaxShowCharNum(lpString, l, nStringRealLen, nTabPixelWidth);
					PixelWidth = GetTextExtentTabExpend(lpString, nStringPixelWidth, Len);
				} else
				{
					Len = GetMaxShowCharNum(lpString, l, nStringRealLen, -1);
					PixelWidth = GetTextExtent(lpString, Len, 0);
				}

				if (uFormat & 1) coordinateX += (l - PixelWidth)/2;
				else
				if (uFormat & 2) coordinateX += (l - PixelWidth);

				int v32 = PixelWidth + ShowString[0].coordinateX;

				ShowString[1].coordinateX = v32;
				ShowString[1].ShowString = WSTR("...");
				ShowString[1].nShowLen = 3;
				ShowString[1].nPixelWidth = nEllipsisStringPixelWidth;

				ShowString[0].coordinateX = coordinateX;

				ShowString[0].nShowLen = Len;
				
				nShowStringCounter = 2;

				if (uFormat & 0x10000)
				{
			#if 0 //TODO ASSERT const*
					for (int i = 0; i < 3; ++i)
					{
						if (Len+i>=nStringRealLen) break;
						lpString[Len+i] = '.';
					}
			#endif
				}
			}
		}

		if (uFormat & 0x40)
		{
			for (int i = 0; i < nShowStringCounter; ++i)
				DrawTabExpendString(ShowString[i].coordinateX, coordinateY,
					ShowString[i].ShowString, ShowString[i].nShowLen, rc, nTabPixelWidth);
		} else
		{
			for (int i = 0; i < nShowStringCounter; ++i)
				DrawString(ShowString[i].coordinateX, coordinateY,
					ShowString[i].ShowString, ShowString[i].nShowLen, 0, 0);
		}		
		return m_pFont->m_Height; //GetTextHeight
	}

	int CWispDC::DrawString(CWispFont *pLogFont, int x, int y, const WCHAR *pStr, unsigned long TextColor, WISP_RECT *pClipRect, unsigned long *pBKColor, int Len, int nTabPixelWidth, int bFlag)
	{
		WCHAR szBuffer[MAX_SHORT_NAME_LEN+1];
		//int BitOffset = 0;
		int nStartx = x;
		while (Len)
		{
			const WCHAR Char = pStr[0];
			if (Char == 0) break;
			if (bFlag)
			{
				szBuffer[0] = Char;
				szBuffer[1] = 0;
			} else
			if (Char == 9)
			{
				int n = ROUNDREVERSE(nTabPixelWidth, m_TabAlign); //??? 32
				MAX_LIMIT(n, MAX_SHORT_NAME_LEN);
				szBuffer[n] = 0;
				while (n-- > 0)
					szBuffer[n] = 0x20;
			} else
			if (Char != 10)
			{
				szBuffer[0] = Char;
				szBuffer[1] = 0;
			} else
				break;
			
			int i = 0;
			while (szBuffer[i] != 0)
			{
				unsigned char *pFont = pLogFont->GetCharPixelBuffer(szBuffer[i]);
				int Width = pLogFont->GetCharWidth(szBuffer[i]);
				int BitOffset = 0; //???
				for (int h = 0; h < pLogFont->m_Height; ++h) //??? =
				{
					int PosX = x;
					int PosY = y + h;
					for (int w = 0; w < Width; ++w) //??? =
					{
						if (PtInRect(pClipRect, PosX ,PosY))
						{
							if (GET_BIT(pFont, BitOffset))
							{
								m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, PosX, PosY, TextColor);
							} else
							if (pBKColor)
							{
								m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, PosX, PosY, *pBKColor);
							}
						}
						++BitOffset;
						++PosX;
					}
				}
				x += Width;
				++nTabPixelWidth;
				//BitOffset = 0;
				++i;
			}
			++pStr;
			--Len;
		}
		return x - nStartx;
	}
	int CWispDC::DrawString(int x, int y, const WCHAR *pStr, int Len, int nTabPixelWidth, int bFlag)
	{
		WISP_RECT rect;

		rect.x = x + m_pRect->x;
		rect.y = y + m_pRect->y;
		rect.cx = GetTextExtent(pStr, Len, 0);
		rect.cy = m_pFont->m_Height;

		if (!ClipDCRect(&rect))
			return 0;

		return DrawString(m_pFont, x + m_pRect->x, y + m_pRect->y, pStr, m_TextColor, &rect,
			(m_TextStyle & 1 ) ? &m_TextBKColor : 0, Len, nTabPixelWidth, bFlag);
	}
	int CWispDC::DrawString(WISP_RECT *pRect, const WCHAR *pStr, int Len, int nTabPixelWidth)
	{
		WISP_RECT rc = *pRect;

		rc.x += m_pRect->x;
		rc.y += m_pRect->y;

		if (!ClipDCRect(&rc))
			return 0;

		return DrawString(m_pFont,
				(pRect->x + m_pRect->x),
				(pRect->y + m_pRect->y) + (pRect->cy - m_pFont->m_Height) / 2,
				pStr,
				m_TextColor, &rc, 0, Len, nTabPixelWidth, 0);
	}
	void CWispDC::DrawShadowText(WISP_RECT *pRect, const WCHAR *pStr, int Offset, int Len, int nTabPixelWidth)
	{
		WISP_RECT rc = *pRect;

		rc.x += m_pRect->x;
		rc.y += m_pRect->y;

		if (ClipDCRect(&rc))
		{
			unsigned long shadowColor = m_TextShadowColor;

			unsigned long OrgTextColor = m_TextColor;
			m_TextColor = shadowColor;
			int y = (pRect->y + m_pRect->y) + (pRect->cy - m_pFont->m_Height) / 2;
			int x = (pRect->x + m_pRect->x);

			DrawString(m_pFont, x + Offset, y + Offset,
						pStr, m_TextColor, &rc, 0, Len, nTabPixelWidth, 0);

			m_TextColor = OrgTextColor;

			DrawString(m_pFont, x, y, pStr, m_TextColor, &rc, 0, Len, nTabPixelWidth, 0);
		}
	}

	int CWispDC::DrawColorString(CWispFont *pLogFont, int x, int y, const unsigned long *pStr, WISP_RECT *pClipRect, int Len, int nTabPixelWidth)
	{
		WCHAR szBuffer[MAX_SHORT_NAME_LEN+1]; //???
	//	int BitOffset = 0;
		int nStartx = x;

		if (m_pColor == 0)
			m_pColor = m_pWispBase->m_pWispDrawObj->m_crSystem;

		while (Len)
		{
			const WCHAR Char = LOW_WORD(pStr[0]);
			if (Char == 0) break;
			if (Char == 9)
			{
				int n = ROUNDREVERSE(nTabPixelWidth, m_TabAlign); //??? 32
				MAX_LIMIT(n, MAX_SHORT_NAME_LEN);
				szBuffer[n] = 0;
				while (n-- > 0)
					szBuffer[n] = 0x20;
			} else
			if (Char != 10)
			{
				szBuffer[0] = Char;
				szBuffer[1] = 0;
			} else
				break;

			unsigned short Color = HIGH_WORD(pStr[0]);
			unsigned char BKClr = HIGH_BYTE(Color);
			unsigned char TextClr = LOW_BYTE(Color);
			
			int i = 0;
			while (szBuffer[i] != 0)
			{
				unsigned char *pFont = pLogFont->GetCharPixelBuffer(szBuffer[i]);
				int Width = pLogFont->GetCharWidth(szBuffer[i]);

				if (x + Width >= pClipRect->x && x < pClipRect->x + pClipRect->cx)
				{
					int BitOffset = 0;
					for (int h = 0; h < pLogFont->m_Height; ++h) //??? =
					{
						int PosX = x;
						int PosY = y + h;
						for (int w = 0; w < Width; ++w)
						{
							if (PtInRect(pClipRect, PosX, PosY))
							{
								if (GET_BIT(pFont, BitOffset))
								{
									m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, PosX, PosY, m_pColor[TextClr]);
								} else
								if (BKClr)
								{
									m_pFrameBuffer->pWispDrawHAL->DrawPixel(m_pFrameBuffer, PosX, PosY, m_pColor[BKClr]);
								}
							}
							++PosX;
							++BitOffset;
						}
					}
				}
				x += Width;
				++nTabPixelWidth;				
				++i;
			}
			++pStr;
			--Len;
		}
		return x - nStartx;
	}
	int CWispDC::DrawColorString(int x, int y, const unsigned long *pStr, int Len, int nTabPixelWidth)
	{
		WISP_RECT rect;

		rect.x = x + m_pRect->x;
		rect.y = y + m_pRect->y;
		rect.cx = m_pRect->x + m_pRect->cx - x;
		rect.cy = m_pFont->m_Height;

		if (!ClipDCRect(&rect))
			return 0;

		return DrawColorString(m_pFont, x + m_pRect->x, y + m_pRect->y,
				pStr, &rect, Len, nTabPixelWidth);
	}
	int CWispDC::DrawColorString(WISP_RECT *pRect, const unsigned long *pStr, int Len, int nTabPixelWidth)
	{
		WISP_RECT rc = *pRect;

		rc.x += m_pRect->x;
		rc.y += m_pRect->y;

		if (!ClipDCRect(&rc))
			return 0;

		return DrawColorString(m_pFont,
				pRect->x + m_pRect->x,
				pRect->y + m_pRect->y + (pRect->cy - m_pFont->m_Height) / 2,
				pStr, &rc, Len, nTabPixelWidth);
	}

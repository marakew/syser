
#include "../Code/tstrmem.hpp"
#include "wispdrawobj.hpp"
#include "wispbase.hpp"
#include "wispbasewnd.hpp"

	CWispDrawObj::CWispDrawObj()
	{
		InitSystemColor();
	}

	CWispDrawObj::~CWispDrawObj()
	{
		//???
	}

	bool CWispDrawObj::Init()
	{
		gpCurWisp->m_MouseCursorSize.cy = 16;
		gpCurWisp->m_MouseCursorSize.cx = 16;
		return true;
	}

	void CWispDrawObj::Release()
	{
		//???
	}

	void CWispDrawObj::InitSystemColor()
	{
		m_crSystem[0] = 0;
		m_crSystem[1] = 0xFFFFFF;
		m_crSystem[2] = 0xC00000;
		m_crSystem[3] = 0xFF0000;
		m_crSystem[4] = 0xC000;
		m_crSystem[5] = 0xFF80;
		m_crSystem[6] = 0xC0;
		m_crSystem[7] = 0x80FF;
		m_crSystem[8] = 0x804040;
		m_crSystem[9] = 0xC86464;
		m_crSystem[10] = 0xC0C0;
		m_crSystem[11] = 0xFFFF;
		m_crSystem[12] = 0x808080;
		m_crSystem[13] = 0xC8C8C8;
		m_crSystem[14] = 0xC0C000;
		m_crSystem[15] = 0xFFFF00;
		m_crSystem[16] = 0x804000;
		m_crSystem[17] = 0xFF8000;

		m_crSystem[18] = 0;
		m_crSystem[19] = 0xC8C8C8;
		m_crSystem[20] = 0xECE9D8;
		m_crSystem[21] = 0;
		m_crSystem[22] = 0xFFFFFF;
		m_crSystem[23] = 0x2398C8;
		m_crSystem[24] = 0x50B0FF;
		m_crSystem[25] = 0x80FF;
		m_crSystem[26] = 0x64C8FA;
		m_crSystem[27] = 0x3C8CDC;
		m_crSystem[28] = 0x82E6;
		m_crSystem[29] = 0xDCDCDC;
		m_crSystem[30] = 0x555555;
		m_crSystem[31] = 0xAAAAAA;
		m_crSystem[32] = 0x50E6FF;
		m_crSystem[33] = 0x32B4E6;
		m_crSystem[34] = 0xFF;

		m_crSystem[35] = 0x7F9DB9;
		m_crSystem[36] = 0x7F9DB9;
		m_crSystem[37] = 0x1C5180;
		m_crSystem[38] = 0xFFFFFF;
		m_crSystem[39] = 0xF3F3EF;
		m_crSystem[40] = 0xE2E1DA;
		m_crSystem[41] = 0xA0A0A0;
		m_crSystem[42] = 0xC0C0C0;
		m_crSystem[43] = 0xF8B330;
		m_crSystem[44] = 0xFFD664;
		m_crSystem[45] = 0xFFFFFF;
		m_crSystem[46] = 0;
		m_crSystem[47] = 0x80;
		m_crSystem[48] = 0xFF;
		m_crSystem[49] = 0x316AC5;

		m_crSystem[50] = 0xB4C8E4;
		m_crSystem[51] = 0xD2E6FF;
		m_crSystem[52] = 0x8A867A;

		m_crSystem[53] = 0x50B0FF;
		m_crSystem[54] = 0xFFFF;
		m_crSystem[55] = 0xFFFFFF;
		m_crSystem[56] = 0x21A121;
		m_crSystem[57] = 0;

		m_crSystem[58] = 0x76B1E5;
		m_crSystem[59] = 0xB4D6F1;
		m_crSystem[60] = 0x6492B9;

		m_crSystem[61] = 0xEBEADB;
		m_crSystem[62] = 0xFFFFFF;
		m_crSystem[63] = 0xC7C5B2;
		m_crSystem[64] = 0xE2DECD;
		m_crSystem[65] = 0xD6D2C2;
		m_crSystem[66] = 0xCBC7B8;

		m_crSystem[67] = 0xF3F3F3;
		m_crSystem[68] = 0xF8A900;
		m_crSystem[69] = 0xF9C654;
		m_crSystem[70] = 0xF8B31F;

		m_crSystem[71] = 0xABEDAC;
		m_crSystem[72] = 0x95E996;
		m_crSystem[73] = 0x7CE47E;
		m_crSystem[74] = 0x66DF68;
		m_crSystem[75] = 0x4EDA50;
		m_crSystem[76] = 0x35D538;
		m_crSystem[77] = 0x28D22B;

		m_crSystem[78] = 0xC88200;
		m_crSystem[79] = 0xE59700;
		m_crSystem[80] = 0xF8B330;
		m_crSystem[81] = 0xFDD889;

		m_crSystem[82] = 0x3D95FF;
		m_crSystem[83] = 0x2B90FF;
		m_crSystem[84] = 0x55EA;
		m_crSystem[85] = 0x46E0;
		m_crSystem[86] = 0x143CF;
		m_crSystem[87] = 0x3DDC;
		m_crSystem[88] = 0x1DA0;
		m_crSystem[89] = 0x138C;

		m_crSystem[90] = 0x7B7B7B;
		m_crSystem[91] = 0xE0E8FE;
		m_crSystem[92] = 0xF1F1F1;
		m_crSystem[93] = 0xE7EBFF;
		m_crSystem[94] = 0x6B79BD;

		m_crSystem[95] = 0xACA899;
		m_crSystem[96] = 0xF1EFE2;
		m_crSystem[97] = 0x716F64;
		m_crSystem[98] = 0xDEDFD8;
		m_crSystem[99] = 0xA5A597;
		m_crSystem[100] = 0xC1C2B8;
		m_crSystem[101] = 0xD0D1C9;
		m_crSystem[102] = 0xB4FAD4;
		m_crSystem[103] = 0x64C8FF;
		m_crSystem[104] = 0xFFAAAA;
		m_crSystem[105] = 0x9BFF;
		m_crSystem[106] = 0xB42828;
		m_crSystem[107] = 0x70A2F3;
		m_crSystem[108] = 0xC5C2B8;
		m_crSystem[109] = 0xFF00;
		m_crSystem[110] = 0xFF00FF;
		m_crSystem[111] = 0xFAFACC;
		m_crSystem[112] = 0x21A221;
		m_crSystem[113] = 0x7BA2E7;
		m_crSystem[114] = 0x6375D6;
		m_crSystem[115] = 0xB4B1A3;
	}
	void CWispDrawObj::DrawBorder(CWispDC *pDC, WISP_RECT *pRect, int nSize)
	{
		WISP_RECT rc = *pRect;

		if (nSize >= 1)
		{
			pDC->DrawRoundRect(&rc, m_crSystem[28]);
			++rc.x;
			++rc.y;
			rc.cx -= 2;
			rc.cy -= 2;
		}

		if (nSize >= 2)
		{
			pDC->DrawRect(&rc, m_crSystem[28]);
			++rc.x;
			++rc.y;
			rc.cx -= 2;
			rc.cy -= 2;
		}

		if (nSize >= 3)
		{
			nSize -= 3;
			if (nSize > 0)
			{
				int n = nSize;
				while (n)
				{
					pDC->DrawRect(&rc, m_crSystem[28]);
					++rc.x;
					++rc.y;
					rc.cx -= 2;
					rc.cy -= 2;
					--n;
				}
			}
		}

		if (nSize >= 1)
			pDC->DrawRect(&rc, m_crSystem[28]);
	}
	void CWispDrawObj::DrawCaption(CWispDC *pDC, WISP_RECT *pRect)
	{
		WISP_RECT rc = *pRect;
		pDC->DrawFullRect(&rc, m_crSystem[23]);
	}
	void CWispDrawObj::DrawToolbarBK(CWispDC *pDC, WISP_RECT *pRect)
	{
		pDC->DrawFullRect(pRect, m_crSystem[24]);
	}
	void CWispDrawObj::DrawClient(CWispDC *pDC, WISP_RECT *pRect)
	{
		pDC->DrawFullRect(pRect, m_crSystem[20]);
	}
	void CWispDrawObj::DrawCtrlBorder(CWispDC *pDC, WISP_RECT *pRect, int nSize)
	{
		WISP_RECT rc = *pRect;
		while (nSize > 1)
		{
			pDC->Draw3DRect(&rc, m_crSystem[35], m_crSystem[36]);
			rc.cx -= 2;
			rc.cy -= 2;
			++rc.x;
			++rc.y;
			--nSize;
		}
		pDC->DrawRect(&rc, m_crSystem[38]);

	}
	void CWispDrawObj::DrawCtrlClient(CWispDC *pDC, WISP_RECT *pRect)
	{
		if (pDC->m_pWispWnd->m_Style & 0x200)	//???
			pDC->DrawFullRect(pRect, m_crSystem[19]);
		else	pDC->DrawFullRect(pRect, m_crSystem[38]);

	}
	void CWispDrawObj::DrawCtrlStaticEdge(CWispDC *pDC, WISP_RECT *pRect)
	{
		unsigned long rcBottomRight = m_crSystem[95];
		unsigned long rcTopLef = m_crSystem[1];
		pDC->DrawHLine(pRect->x, pRect->x2() - 2, pRect->y, rcBottomRight);
		pDC->DrawHLine(pRect->x, pRect->x2() - 1, pRect->y2() - 1, rcTopLef);
		pDC->DrawVLine(pRect->x, pRect->y, pRect->y2() - 2, rcBottomRight);
		pDC->DrawVLine(pRect->x2() - 1, pRect->y, pRect->y2() - 1, rcTopLef);
	}
	void CWispDrawObj::DrawCtrlClientEdge(CWispDC *pDC, WISP_RECT *pRect)
	{
		unsigned long rcTopLeft = m_crSystem[95];
		unsigned long rcBottomRight = m_crSystem[97];
		unsigned long rcBottomRight1 = m_crSystem[96];
		unsigned long rcTopLeft1 = m_crSystem[1];

		pDC->DrawHLine(pRect->x, pRect->x2() - 2, pRect->y, rcTopLeft);
		pDC->DrawHLine(pRect->x, pRect->x2() - 1, pRect->y2() - 1, rcTopLeft1);
		pDC->DrawVLine(pRect->x, pRect->y, pRect->y2() - 2, rcTopLeft);
		pDC->DrawVLine(pRect->x2() - 1, pRect->y, pRect->y2() - 1, rcTopLeft1);
		pDC->DrawHLine(pRect->x + 1, pRect->x2() - 3, pRect->y + 1, rcBottomRight);
		pDC->DrawHLine(pRect->x + 1, pRect->x2() - 2, pRect->y2() - 2, rcBottomRight1);
		pDC->DrawVLine(pRect->x + 1, pRect->y + 1, pRect->y2() - 3, rcBottomRight);
		pDC->DrawVLine(pRect->x2() - 2, pRect->y + 1, pRect->y2() - 2, rcBottomRight1);
	}
	void CWispDrawObj::DrawCtrlModalFrame(CWispDC *pDC, WISP_RECT *pRect)
	{
		unsigned long rcTopLeft2;
		unsigned long rcBottomRight2;
		unsigned long rcTopLeft;
		unsigned long rcBottomRight;
		unsigned long rcBottomRight1;
		unsigned long rcTopLeft1;

		unsigned long Color20;
		unsigned long Color96;
		unsigned long Color1;
		unsigned long Color95;
		unsigned long Color97;

		Color20 = m_crSystem[20];
		Color96 = m_crSystem[96];
		Color1 = m_crSystem[1];
		Color95 = m_crSystem[95];
		Color97 = m_crSystem[97];

		pDC->DrawHLine(pRect->x, pRect->x2() - 2, pRect->y, Color96);
		pDC->DrawHLine(pRect->x, pRect->x2() - 1, pRect->y2() - 1, Color97);
		pDC->DrawVLine(pRect->x, pRect->y, pRect->y2() - 2, Color96);
		pDC->DrawVLine(pRect->x2() - 1, pRect->y, pRect->y2() - 1, Color97);
		pDC->DrawHLine(pRect->x + 1, pRect->x2() - 3, pRect->y + 1, Color1);
		pDC->DrawHLine(pRect->x + 1, pRect->x2() - 2, pRect->y2() - 2, Color95);
		pDC->DrawVLine(pRect->x + 1, pRect->y + 1, pRect->y2() - 3, Color1);
		pDC->DrawVLine(pRect->x2() - 2, pRect->y + 1, pRect->y2() - 2, Color95);
		pDC->DrawHLine(pRect->x + 2, pRect->x2() - 4, pRect->y + 2, Color20);
		pDC->DrawHLine(pRect->x + 2, pRect->x2() - 3, pRect->y2() - 3, Color20);
		pDC->DrawVLine(pRect->x + 2, pRect->y + 2, pRect->y2() - 4, Color20);
		pDC->DrawVLine(pRect->x2() - 3, pRect->y + 2, pRect->y2() - 3, Color20);
	}
	void CWispDrawObj::DrawCtrlFocusRect(CWispDC *pDC, WISP_RECT *pRect)
	{
		pDC->DrawDashRect(pRect, m_crSystem[48]);
	}
	void CWispDrawObj::DrawVertScrollRect(CWispDC *pDC, WISP_RECT *pRect)
	{
		pDC->DrawFullRect(pRect, m_crSystem[39]);
	}
	void CWispDrawObj::DrawVertScrollSlideRect(CWispDC *pDC, WISP_RECT *pRect)
	{
		int i;
		WISP_RECT rc = *pRect;

		pDC->DrawRoundRect(&rc, m_crSystem[58]);

		++rc.x;
		++rc.y;
		rc.cx -= 2;
		rc.cy -= 2;
		pDC->DrawRect(&rc, m_crSystem[1]);

		++rc.x;
		++rc.y;
		rc.cx -= 2;
		rc.cy -= 2;
		pDC->DrawFullRect(&rc, m_crSystem[26]);
		if (rc.cy >= 10)
		{
			i = 0;
			do {
				pDC->DrawHLine(
					rc.x + 2,
					rc.cx + rc.x - 3,
					i + rc.y + (rc.cy - 8) / 2, m_crSystem[13]);
				pDC->DrawHLine(
					rc.x + 2,
					rc.cx + rc.x - 3,
					i + rc.y + (rc.cy - 8) / 2 + 1, m_crSystem[12]);
				i += 2;
			} while (i < 8);
		}
	}
	void CWispDrawObj::DrawHorzScrollRect(CWispDC *pDC, WISP_RECT *pRect)
	{
		pDC->DrawFullRect(pRect, m_crSystem[39]);
	}
	void CWispDrawObj::DrawHorzScrollSlideRect(CWispDC *pDC, WISP_RECT *pRect)
	{
		int i;
		WISP_RECT rc = *pRect;

		pDC->DrawRoundRect(&rc, m_crSystem[58]);

		++rc.x;
		++rc.y;
		rc.cx -= 2;
		rc.cy -= 2;
		pDC->DrawRect(&rc, m_crSystem[1]);

		++rc.x;
		++rc.y;
		rc.cx -= 2;
		rc.cy -= 2;
		pDC->DrawFullRect(&rc, m_crSystem[26]);
		if (rc.cx >= 10)
		{
			i = 0;
			do {
				pDC->DrawVLine(
					i + rc.x + (rc.cx - 8) / 2,
					rc.y + 2,
					rc.cy + rc.y - 3, m_crSystem[13]);
				pDC->DrawVLine(
					i + rc.x + (rc.cx - 8) / 2 + 1,
					rc.y + 2,
					rc.cy + rc.y - 3, m_crSystem[12]);
				i += 2;
			} while (i < 8);
		}
	}
	void CWispDrawObj::DrawSplitRect(CWispDC *pDC, WISP_RECT *pRect, bool bHorz)
	{
		pDC->DrawFullRect(pRect, m_crSystem[28]);
	}
	void CWispDrawObj::DrawButtonClient(CWispDC *pDC, WISP_RECT *pRect, unsigned int State)
	{
		if (State == 2) //WISP_BT_DIB_DOWN
			pDC->DrawFullRect(pRect, m_crSystem[40]);
		else	pDC->DrawFullRect(pRect, m_crSystem[39]);
	}
	void CWispDrawObj::DrawButtonFrameRect(CWispDC *pDC, WISP_RECT *pRect, unsigned int State)
	{
		if (State & 0x40)
		{
			if (State == 2) //WISP_BT_DIB_DOWN
			{
				pDC->DrawFrameRect(pRect, 0, 1);
			} else
			if (State == 1) //WISP_BT_DIB_HOVER
			{
				pDC->DrawFrameRect(pRect, 1, 1);
			}
		} else
		{
			if (State == 0) //WISP_BT_DIB_NORMAL
			{
				pDC->DrawRoundRect(pRect, m_crSystem[37]);
			} else
			if (State == 1) //WISP_BT_DIB_HOVER
			{
				WISP_RECT rc = *pRect;
				pDC->DrawRoundRect(&rc, m_crSystem[37]);

				++rc.x;
				++rc.y;
				rc.cx -= 2;
				rc.cy -= 2;
				pDC->DrawRect(&rc, m_crSystem[80]);

				++rc.x;
				++rc.y;
				rc.cx -= 2;
				rc.cy -= 2;
				pDC->DrawRect(&rc, m_crSystem[81]);
			} else
			if (State == 2) //WISP_BT_DIB_DOWN
			{
				WISP_RECT rc = *pRect;

				++rc.x;
				++rc.y;
				rc.cx -= 2;
				rc.cy -= 2;
				pDC->DrawRoundRect(&rc, m_crSystem[37]);
				pDC->DrawRect(&rc, m_crSystem[38]);
			}
		}
	}
	void CWispDrawObj::DrawTabBackground(CWispDC *pDC, WISP_RECT *pRect)
	{
		DrawToolbarBK(pDC, pRect);
	}
	void CWispDrawObj::DrawTabItem(CWispDC *pDC, WISP_RECT *pRect, unsigned int State)
	{
		if (State == 0) //WISP_BT_DIB_NORMAL
		{
			pDC->DrawRoundRect(pRect, m_crSystem[37]);
		} else
		if (State == 1) //WISP_BT_DIB_HOVER
		{
			WISP_RECT rc = *pRect;
			++rc.x;
			++rc.y;
			rc.cx -= 2;
			rc.cy -= 2;

			pDC->DrawRect(&rc, m_crSystem[80]);

			++rc.x;
			++rc.y;
			rc.cx -= 2;
			rc.cy -= 2;
			pDC->DrawRect(&rc, m_crSystem[81]);
			pDC->DrawRoundRect(pRect, m_crSystem[37]);
		}
		if (State == 2) //WISP_BT_DIB_DOWN
		{
			pDC->DrawRoundRect(pRect, m_crSystem[37]);
		}

	}
	void CWispDrawObj::DrawHeaderFrameRect(CWispDC *pDC, WISP_RECT *pRect, unsigned int State)
	{
		unsigned long Color61;
		unsigned long Color64;
		unsigned long Color65;
		unsigned long Color99;
		unsigned long Color98;
		unsigned long Color66;
		unsigned long Color62;
		unsigned long Color63;

		Color61 = m_crSystem[61];
		Color62 = m_crSystem[62];
		Color64 = m_crSystem[64];
		Color63 = m_crSystem[63];
		Color65 = m_crSystem[65];
		Color66 = m_crSystem[66];

		if (State == 1) //WISP_BT_DIB_HOVER
		{
			Color61 = m_crSystem[67];
			Color64 = m_crSystem[68];
			Color65 = m_crSystem[69];
			Color66 = m_crSystem[70];
		} else
		if (State == 2) //WISP_BT_DIB_DOWN
		{
			Color99 = m_crSystem[99];
			Color98 = m_crSystem[98];
			pDC->DrawHLine(pRect->x + 1, pRect->x2(), pRect->y2() - 1, Color99);
			pDC->DrawVLine(pRect->x + 1, pRect->y, pRect->y2() - 1, Color99);
			pDC->DrawFullRect(pRect, Color98);
			return;
		}
		pDC->DrawFullRect(pRect, Color61);
		pDC->DrawHLine(pRect->x, pRect->x2() - 1, pRect->y2() - 3, Color64);
		pDC->DrawHLine(pRect->x, pRect->x2() - 1, pRect->y2() - 2, Color65);
		pDC->DrawHLine(pRect->x, pRect->x2() - 1, pRect->y2() - 1, Color66);
		pDC->DrawLine(pRect->x, pRect->y + 2, pRect->x, pRect->y2() - 5, Color62);
		pDC->DrawLine(pRect->x2() - 1, pRect->y + 2, pRect->x2() - 1, pRect->y2() - 5, Color63);
	}
	void CWispDrawObj::DrawProgress(CWispDC *pDC, WISP_RECT *pRect, unsigned int uStyle, int nPersent)
	{
		pDC->Draw3DRect(pRect, m_crSystem[35], m_crSystem[36]);
		pRect->y += 2;
		pRect->x += 2;
		pRect->cx -= 4;
		pRect->cy -= 4;

		WISP_RECT rcLine;

		rcLine.y = pRect->y;
		rcLine.x = pRect->x;
		rcLine.cy = pRect->cy;
		rcLine.cx = nPersent * pRect->cx / 1000;

		if (rcLine.cx > 0)
		{
			int n = 0;
			while (rcLine.cy > 0) //&& n<6
			{
				pDC->DrawHLine(rcLine.x, rcLine.x2() - 1, rcLine.y, m_crSystem[71+n]);
				pDC->DrawHLine(rcLine.x, rcLine.x2() - 1, rcLine.y2() - 1, m_crSystem[71+n]);
				++rcLine.y;
				rcLine.cy -= 2;
				++n;
				if (n >= 6)
					break;
			}

			pDC->DrawFullRect(&rcLine, m_crSystem[77]);
			for (int i = 6; i < rcLine.cx; i += 7)
				pDC->DrawVLine(i+pRect->x, pRect->y, pRect->y2()-1, m_crSystem[38]);
		}

	}
	void CWispDrawObj::DrawSystemStandardIcon(CWispDC *pDC, WISP_RECT *pRect, unsigned int uStyle, unsigned int State)
	{
		WISP_RECT rect;
		unsigned long color; //???

		switch (uStyle)
		{
		case 0:	//WISP_SSI_VERT_INC_SPIN
		case 1: //WISP_SSI_VERT_DEC_SPIN
		case 2: //WISP_SSI_HORZ_INC_SPIN
		case 3:	//WISP_SSI_HORZ_DEC_SPIN

			if (State == 0) //WISP_BT_DIB_NORMAL
				color = m_crSystem[58];
			else
			if (State == 1) //WISP_BT_DIB_HOVER
				color = m_crSystem[59];
			else
			if (State == 2) //WISP_BT_DIB_DOWN
				color = m_crSystem[60];

			//???=
			rect = *pRect;

			pDC->DrawRect(&rect, color);

			++rect.x;
			++rect.y;
			rect.cx -= 2;
			rect.cy -= 2;
			pDC->DrawRect(&rect, m_crSystem[1]);

			rect.cx -= 2;
			rect.cy -= 2;
			++rect.x;
			++rect.y;
			pDC->DrawFullRect(&rect, color);

			rect.y += 2;
			rect.x += 1;
			rect.cx -= 2;
			rect.cy -= 4;

			switch (uStyle)
			{
			case 0: //WISP_SSI_VERT_INC_SPIN
			{
				int _ii_v43 = -1;
				do {
					pDC->DrawLine(rect.x + rect.cx/2 - rect.cy/2,
						_ii_v43 + rect.cy / 4 + rect.y,
						rect.cx/2 + rect.x, _ii_v43 + rect.cy/4 + rect.y + rect.cy/2, m_crSystem[6]);
	
					pDC->DrawLine(rect.x + rect.cy/2 + rect.cx/2,
						_ii_v43 + rect.cy / 4 + rect.y,
						rect.cx/2 + rect.x, _ii_v43 + rect.cy/4 + rect.y + rect.cy/2, m_crSystem[6]);
					++_ii_v43;
				} while ( _ii_v43 < 1);
				int _i_v44 = 2;
				do {
					pDC->DrawLine(rect.x + rect.cx/2 - rect.cy/2,
						_i_v44 + rect.cy / 4 + rect.y,
						rect.cx/2 + rect.x, _i_v44 + rect.cy/4 + rect.y + rect.cy/2, m_crSystem[6]);

					pDC->DrawLine(rect.x + rect.cy/2 + rect.cx/2,
						_i_v44 + rect.cy / 4 + rect.y,
						rect.cx/2 + rect.x, _i_v44 + rect.cy/4 + rect.y + rect.cy/2, m_crSystem[6]);
					++_i_v44;
				} while ( _i_v44 < 4);
			}
				break;
			case 1: //WISP_SSI_VERT_DEC_SPIN
			{
				int i_v40 = -1;
				do {
					pDC->DrawLine(rect.x + rect.cx/2 - rect.cy/2,
						rect.y - rect.cy/4 - i_v40 + rect.cy - 1,
						rect.cx/2 + rect.x,
						rect.y - rect.cy/4 - i_v40 - rect.cy/2 + rect.cy - 1, m_crSystem[6]);

					pDC->DrawLine(rect.x + rect.cy/2 + rect.cx/2,
						rect.y - rect.cy/4 - i_v40 + rect.cy - 1,
						rect.cx/2 + rect.x,
						rect.y - rect.cy/4 - i_v40 - rect.cy/2 + rect.cy - 1, m_crSystem[6]);
					++i_v40;
				} while (i_v40 < 1);
				int i_v41 = 2;
				do {
					pDC->DrawLine(rect.x + rect.cx/2 - rect.cy/2,
						rect.y - rect.cy/4 - i_v41 + rect.cy - 1,
						rect.cx/2 + rect.x,
						rect.y - rect.cy/4 - i_v41 - rect.cy/2 + rect.cy - 1, m_crSystem[6]);
					pDC->DrawLine(rect.x + rect.cy/2 + rect.cx/2,
						rect.y - rect.cy/4 - i_v41 + rect.cy - 1,
						rect.cx/2 + rect.x,
						rect.y - rect.cy/4 - i_v41 - rect.cy/2 + rect.cy - 1, m_crSystem[6]);
					++i_v41;
				} while (i_v41 < 4);
			}
				break;
			case 2: //WISP_SSI_HORZ_INC_SPIN
			{
				int i_v38 = -1;
				do {
					pDC->DrawLine(rect.x + rect.cx/4 + i_v38,
						rect.y + rect.cy/2 - rect.cy/2,
						rect.x + rect.cx/4 + i_v38 + rect.cy/2,
						rect.cy/2 + rect.y, m_crSystem[6]);

					pDC->DrawLine(rect.x + rect.cx/4 + i_v38,
						rect.y + rect.cy/2 + rect.cy/2,
						rect.x + rect.cx/4 + i_v38 + rect.cy/2,
						rect.cy/2 + rect.y, m_crSystem[6]);
					++i_v38;
				} while (i_v38 < 1);
				int i_v39 = 2;
				do {
					pDC->DrawLine(rect.x + rect.cx/4 + i_v39,
						rect.y + rect.cy/2 - rect.cy/2,
						rect.x + rect.cx/4 + i_v39 + rect.cy/2,
						rect.cy/2 + rect.y, m_crSystem[6]);

					pDC->DrawLine(rect.x + rect.cx/4 + i_v39,
						rect.y + rect.cy/2 + rect.cy/2,
						rect.x + rect.cx/4 + i_v39 + rect.cy/2,
						rect.cy/2 + rect.y, m_crSystem[6]);
					++i_v39;
				} while (i_v39 < 4);
			}
				break;
			case 3: //WISP_SSI_HORZ_DEC_SPIN
			{
				int i_v36 = -1;
				do {
					pDC->DrawLine(rect.cx - rect.cx/4 - i_v36 + rect.x - 1,
						rect.y + rect.cy/2 - rect.cy/2,
						rect.cx - rect.cx/4 - i_v36 - rect.cy/2 + rect.x-1,
						rect.cy/2 + rect.y, m_crSystem[6]);

					pDC->DrawLine(rect.cx - rect.cx/4 - i_v36 + rect.x - 1,
						rect.y + rect.cy/2 + rect.cy/2,
						rect.cx - rect.cx/4 - i_v36 - rect.cy/2 + rect.x-1,
						rect.cy/2 + rect.y, m_crSystem[6]);
					++i_v36;
				} while (i_v36 < 1);
				int _ixi_v37 = 2;
				do {
					pDC->DrawLine(rect.cx - rect.cx/4 - _ixi_v37 + rect.x - 1,
						rect.y + rect.cy/2 - rect.cy/2,
						rect.cx - rect.cx/4 - _ixi_v37 - rect.cy/2 + rect.x-1,
						rect.cy/2 + rect.y, m_crSystem[6]);

					pDC->DrawLine(rect.cx - rect.cx/4 - _ixi_v37 + rect.x - 1,
						rect.y + rect.cy/2 + rect.cy/2,
						rect.cx - rect.cx/4 - _ixi_v37 - rect.cy/2 + rect.x-1,
						rect.cy/2 + rect.y, m_crSystem[6]);
					++_ixi_v37;
				} while (_ixi_v37 < 4);
			}
				break;
			default:
				break;
			}
			break;

		case 4: //WISP_SSI_CLOSE
		case 5: //WISP_SSI_MINIMIZE
		case 6: //WISP_SSI_MAXIMIZE
		case 7: //WISP_SSI_RESTORE

			if (State == 0) //WISP_BT_DIB_NORMAL
				color = m_crSystem[55];
			else
			if (State == 1) //WISP_BT_DIB_HOVER
				color = m_crSystem[54];
			else
			if (State == 2) //WISP_BT_DIB_DOWN
				color = m_crSystem[53];

			//???=
			rect = *pRect;

			pDC->DrawRoundRect(&rect, color);

			rect.x += 4;
			rect.y += 4;
			rect.cx -= 8;
			rect.cy -= 8;

			if (uStyle == 4) //WISP_SSI_CLOSE
			{
				pDC->DrawLine(rect.x + 1, rect.y,               rect.cx + rect.x - 1, rect.cy + rect.y - 2, color);
				pDC->DrawLine(rect.x + 1, rect.y + 1,           rect.cx + rect.x - 2, rect.cy + rect.y - 2, color);
				pDC->DrawLine(rect.x,     rect.y + 1,           rect.cx + rect.x - 2, rect.cy + rect.y - 1, color);
				pDC->DrawLine(rect.x,     rect.cy + rect.y - 2, rect.cx + rect.x - 2, rect.y,               color);
				pDC->DrawLine(rect.x + 1, rect.cy + rect.y - 2, rect.cx + rect.x - 2, rect.y + 1,           color);
				pDC->DrawLine(rect.x + 1, rect.cy + rect.y - 1, rect.cx + rect.x - 1, rect.y + 1,           color);
			} else
			if (uStyle == 5) //WISP_SSI_MINIMIZE
			{
				pDC->DrawLine(rect.x, rect.cy+rect.y-3, rect.cx+rect.x-1, rect.cy+rect.y-3, color);
				pDC->DrawLine(rect.x, rect.cy+rect.y-2, rect.cx+rect.x-1, rect.cy+rect.y-2, color);
				pDC->DrawLine(rect.x, rect.cy+rect.y-1, rect.cx+rect.x-1, rect.cy+rect.y-1, color);
			} else
			if (uStyle == 6) //WISP_SSI_MAXIMIZE
			{
				pDC->DrawRect(&rect, color);
				pDC->DrawLine(rect.x + 1, rect.y + 1, rect.cx + rect.x - 2, rect.y + 1, color);
				pDC->DrawLine(rect.x + 1, rect.y + 2, rect.cx + rect.x - 2, rect.y + 2, color);
	       		} else
			if (uStyle == 7) //WISP_SSI_RESTORE
			{
				rect.x += 2;
				rect.cx -= 2;
				rect.cy -= 2;
				pDC->DrawRect(&rect, color);
				pDC->DrawLine(rect.x + 1, rect.y + 1, rect.cx + rect.x - 2, rect.y + 1, color);
				rect.x -= 3;
				rect.y += 4;
				rect.cy -= 1;
				pDC->DrawRect(&rect, color);
				pDC->DrawLine(rect.x + 1, rect.y + 1, rect.cx + rect.x - 2, rect.y + 1, color);
			}
			break;
		case 9: //WISP_SSI_EXPANDBUTTON_EXPAND
		case 10: //WISP_SSI_EXPANDBUTTON_COLLAPSE

			rect.x = pRect->x + (pRect->cx - 9) / 2;
			rect.y = pRect->y + (pRect->cy - 9) / 2;
			rect.cx = 9;
			rect.cy = 9;
			pDC->DrawRect(&rect, m_crSystem[53]);
			rect.x += 2;
			rect.y += 2;
			pDC->DrawHLine(rect.x, rect.x + 4, rect.y + 2, m_crSystem[53]);
	
			if (uStyle == 10) //WISP_SSI_EXPANDBUTTON_COLLAPSE
			{
				pDC->DrawVLine(rect.x + 2, rect.y, rect.y + 4, m_crSystem[53]);
			}
			break;

		case 11: //WISP_SSI_CHECKBOX_UNCHECKED
		case 12: //WISP_SSI_CHECKBOX_CHECKED
		case 13: //WISP_SSI_CHECKBOX_SEMICHECKED
		case 14: //WISP_SSI_CHECKBOX_CHECKMARK

			if (uStyle == 11 || uStyle == 12 || uStyle == 13)
			{
				rect.x = (pRect->cx - 11) / 2 + pRect->x;
				rect.y = (pRect->cy - 11) / 2 + pRect->y;
				rect.cx = 11;
				rect.cy = 11;
				pDC->DrawRect(&rect, m_crSystem[53]);

				if (uStyle == 13) //WISP_SSI_CHECKBOX_SEMICHECKED
				{
					rect.x += 3;
					rect.y += 3;
					rect.cx = 5;
					rect.cy = 5;
					pDC->DrawFullRect(&rect, m_crSystem[56]);
				}
			}

			if (uStyle == 12 || uStyle == 14) //WISP_SSI_CHECKBOX_CHECKED WISP_SSI_CHECKBOX_CHECKMARK
			{
				int v22_x = pRect->x + (pRect->cx - 7) / 2;
				int v21_y = pRect->y + (pRect->cy - 7) / 2;

				int v23 = 0;
				while (1)
				{
					pDC->DrawLine(v22_x,     v21_y + v23 + 2, v22_x + 2, v21_y + v23 + 4, m_crSystem[56]);
					pDC->DrawLine(v22_x + 2, v21_y + v23 + 4, v22_x + 6, v21_y + v23,     m_crSystem[56]);
	
					if (++v23 >= 3)
						break;
				}
			}
			break;

		case 15: //WISP_SSI_REDIOBOX_SELECTED
		case 16: //WISP_SSI_REDIOBOX_UNSELECTED
		case 17: //WISP_SSI_REDIOBOX_SELECTED_DRAK
		case 18: //WISP_SSI_REDIOBOX_UNSELECTED_DRAK

			pDC->DrawHLine(pRect->x + 4,  pRect->x + 7, pRect->y,      m_crSystem[0]);
			pDC->DrawHLine(pRect->x + 4,  pRect->x + 7, pRect->y + 11, m_crSystem[0]);
			pDC->DrawHLine(pRect->x + 2,  pRect->x + 9, pRect->y + 1,  m_crSystem[0]);
			pDC->DrawHLine(pRect->x + 2,  pRect->x + 9, pRect->y + 10, m_crSystem[0]);

			pDC->DrawVLine(pRect->x,      pRect->y + 4, pRect->y + 7,  m_crSystem[0]);
			pDC->DrawVLine(pRect->x + 11, pRect->y + 4, pRect->y + 7,  m_crSystem[0]);
			pDC->DrawVLine(pRect->x + 1,  pRect->y + 2, pRect->y + 9,  m_crSystem[0]);
			pDC->DrawVLine(pRect->x + 10, pRect->y + 2, pRect->y + 9,  m_crSystem[0]);

			pDC->DrawPixel(pRect->x + 3,  pRect->y + 2, m_crSystem[0]);
			pDC->DrawPixel(pRect->x + 8,  pRect->y + 2, m_crSystem[0]);
			pDC->DrawPixel(pRect->x + 3,  pRect->y + 9, m_crSystem[0]);
			pDC->DrawPixel(pRect->x + 8,  pRect->y + 9, m_crSystem[0]);
			pDC->DrawPixel(pRect->x + 2,  pRect->y + 3, m_crSystem[0]);
			pDC->DrawPixel(pRect->x + 2,  pRect->y + 8, m_crSystem[0]);
			pDC->DrawPixel(pRect->x + 9,  pRect->y + 3, m_crSystem[0]);
			pDC->DrawPixel(pRect->x + 9,  pRect->y + 8, m_crSystem[0]);
			pDC->DrawPixel(pRect->x + 2,  pRect->y + 2, m_crSystem[0]);
			pDC->DrawPixel(pRect->x + 9,  pRect->y + 2, m_crSystem[0]);
			pDC->DrawPixel(pRect->x + 2,  pRect->y + 9, m_crSystem[0]);
			pDC->DrawPixel(pRect->x + 9,  pRect->y + 9, m_crSystem[0]);

			if (uStyle == 15 || uStyle == 17) //WISP_SSI_REDIOBOX_SELECTED WISP_SSI_REDIOBOX_SELECTED_DRAK
			{
				pDC->DrawVLine(pRect->x + 5, pRect->y + 4, pRect->y + 7, m_crSystem[0]);
				pDC->DrawVLine(pRect->x + 6, pRect->y + 4, pRect->y + 7, m_crSystem[0]);
				pDC->DrawHLine(pRect->x + 4, pRect->x + 7, pRect->y + 5, m_crSystem[0]);
				pDC->DrawHLine(pRect->x + 4, pRect->x + 7, pRect->y + 6, m_crSystem[0]);
			}
			break;
		default:
			break;
		}
	}

	void CWispDrawObj::DrawCursorArrow(CWispDC *pDC, WISP_POINT *pHotPT)
	{
		if (pDC)
		{

		//	y1_v3 = pHotPT->y;
		//	color_v4 = &this->m_crSystem[77];
		//	x1_v5 = pHotPT->x;
		//	y2_v6 = y1_v3 + 15;
		//	C_v14 = &this->m_crSystem[77];
		//	vc = 3;
		//	do {
		//		pDC->DrawVLine(x1_v5++, y1_v3++, y2_v6--, *color_v4);
		//		color_v4 -= 2;
		//		--vc;
		//	} while (vc);

			pDC->DrawVLine(pHotPT->x+0, pHotPT->y+0, pHotPT->y+15-0, m_crSystem[77-0]);
			pDC->DrawVLine(pHotPT->x+1, pHotPT->y+1, pHotPT->y+15-1, m_crSystem[77-2]);
			pDC->DrawVLine(pHotPT->x+2, pHotPT->y+2, pHotPT->y+15-2, m_crSystem[77-4]);

		//	x1_L_v7 = pHotPT->x;
		//	y1_L_v8 = pHotPT->y;
		//	x2_L_v9 = pHotPT->x + 15;
		//	y2_L_v10 = y1_L_v8 + 15;
		//	color_v12 = C_v14;
		//	lc = 4;
		//	do {
		//		pDC->DrawLine(x1_L_v7, y1_L_v8, x2_L_v9, y2_L_v10, *color_v12);
		//		color_v12 -= 2;
		//		++x1_L_v7;
		//		y1_L_v8 += 2;
		//		x2_L_v9 -= 2;
		//		--y2_L_v10;
		//		--lc;
		//	} while (lc);

			pDC->DrawLine(pHotPT->x+0, pHotPT->y+0, pHotPT->x+15-0, pHotPT->y+15-0, m_crSystem[77-0]);
			pDC->DrawLine(pHotPT->x+1, pHotPT->y+2, pHotPT->x+15-2, pHotPT->y+15-1, m_crSystem[77-2]);
			pDC->DrawLine(pHotPT->x+2, pHotPT->y+4, pHotPT->x+15-4, pHotPT->y+15-2, m_crSystem[77-4]);
			pDC->DrawLine(pHotPT->x+3, pHotPT->y+6, pHotPT->x+15-6, pHotPT->y+15-3, m_crSystem[77-6]);

			pDC->DrawLine(pHotPT->x + 15, pHotPT->y + 15, pHotPT->x + 2, pHotPT->y + 11, m_crSystem[77]);
		} else
		{
			gpCurWisp->m_MouseCursorPos.x = 0;
			gpCurWisp->m_MouseCursorPos.y = 0;
			gpCurWisp->m_MouseCursorSize.cx = 16;
			gpCurWisp->m_MouseCursorSize.cy = 16;
		}
	}
	void CWispDrawObj::DrawCursorDrag(CWispDC *pClipDC, WISP_POINT *pHotPT)
	{
		//EMPTY
	}
	void CWispDrawObj::DrawCursorResizeVert(CWispDC *pDC, WISP_POINT *pHotPT)
	{
		if (pDC)
		{
			pDC->DrawLine(pHotPT->x - 2, pHotPT->y - 4, pHotPT->x, pHotPT->y - 6, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x - 2, pHotPT->y - 5, pHotPT->x, pHotPT->y - 7, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x + 2, pHotPT->y - 4, pHotPT->x, pHotPT->y - 6, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x + 2, pHotPT->y - 5, pHotPT->x, pHotPT->y - 7, m_crSystem[75]);
			pDC->DrawVLine(pHotPT->x - 1, pHotPT->y - 6, pHotPT->y + 6, m_crSystem[75]);
			pDC->DrawVLine(pHotPT->x, pHotPT->y - 7, pHotPT->y + 7, m_crSystem[72]);
			pDC->DrawVLine(pHotPT->x + 1, pHotPT->y - 6, pHotPT->y + 6, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x - 2, pHotPT->y + 4, pHotPT->x, pHotPT->y + 6, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x - 2, pHotPT->y + 5, pHotPT->x, pHotPT->y + 7, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x + 2, pHotPT->y + 4, pHotPT->x, pHotPT->y + 6, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x + 2, pHotPT->y + 5, pHotPT->x, pHotPT->y + 7, m_crSystem[75]);
		} else
		{
			gpCurWisp->m_MouseCursorPos.x = 6;
			gpCurWisp->m_MouseCursorPos.y = 7;
			gpCurWisp->m_MouseCursorSize.cx = 13;
			gpCurWisp->m_MouseCursorSize.cy = 15;
		}
	}
	void CWispDrawObj::DrawCursorResizeHorz(CWispDC *pDC, WISP_POINT *pHotPT)
	{
		if (pDC)
		{
			pDC->DrawLine(pHotPT->x - 4, pHotPT->y - 2, pHotPT->x - 6, pHotPT->y, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x - 5, pHotPT->y - 2, pHotPT->x - 7, pHotPT->y, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x - 4, pHotPT->y + 2, pHotPT->x - 6, pHotPT->y, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x - 5, pHotPT->y + 2, pHotPT->x - 7, pHotPT->y, m_crSystem[75]);
			pDC->DrawHLine(pHotPT->x - 6, pHotPT->x + 6, pHotPT->y - 1, m_crSystem[75]);
			pDC->DrawHLine(pHotPT->x - 7, pHotPT->x + 7, pHotPT->y, m_crSystem[72]);
			pDC->DrawHLine(pHotPT->x + 6, pHotPT->x - 6, pHotPT->y + 1, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x + 4, pHotPT->y - 2, pHotPT->x + 6, pHotPT->y, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x + 5, pHotPT->y - 2, pHotPT->x + 7, pHotPT->y, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x + 4, pHotPT->y + 2, pHotPT->x + 6, pHotPT->y, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x + 5, pHotPT->y + 2, pHotPT->x + 7, pHotPT->y, m_crSystem[75]);
		} else
		{
			gpCurWisp->m_MouseCursorPos.x = 7;
			gpCurWisp->m_MouseCursorPos.y = 6;
			gpCurWisp->m_MouseCursorSize.cx = 15;
			gpCurWisp->m_MouseCursorSize.cy = 13;
		}
	}
	void CWispDrawObj::DrawCursorResizeLeftLean(CWispDC *pDC, WISP_POINT *pHotPT)
	{
		if (pDC)
		{
			pDC->DrawLine(pHotPT->x - 5, pHotPT->y - 5, pHotPT->x - 5, pHotPT->y, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x - 5, pHotPT->y - 5, pHotPT->x, pHotPT->y - 5, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x - 3, pHotPT->y - 4, pHotPT->x + 4, pHotPT->y + 3, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x - 4, pHotPT->y - 4, pHotPT->x + 4, pHotPT->y + 4, m_crSystem[72]);
			pDC->DrawLine(pHotPT->x - 4, pHotPT->y - 3, pHotPT->x + 3, pHotPT->y + 4, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x, pHotPT->y + 5, pHotPT->x + 5, pHotPT->y + 5, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x + 5, pHotPT->y, pHotPT->x + 5, pHotPT->y + 5, m_crSystem[75]);
		} else
		{
			gpCurWisp->m_MouseCursorPos.x = 6;
			gpCurWisp->m_MouseCursorPos.y = 6;
			gpCurWisp->m_MouseCursorSize.cx = 11;
			gpCurWisp->m_MouseCursorSize.cy = 11;
		}
	}
	void CWispDrawObj::DrawCursorResizeRightLean(CWispDC *pDC, WISP_POINT *pHotPT)
	{
		if (pDC)
		{
			pDC->DrawLine(pHotPT->x, pHotPT->y - 5, pHotPT->x + 5, pHotPT->y - 5, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x + 5, pHotPT->y, pHotPT->x + 5, pHotPT->y - 5, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x - 3, pHotPT->y + 4, pHotPT->x + 4, pHotPT->y - 3, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x - 4, pHotPT->y + 4, pHotPT->x + 4, pHotPT->y - 4, m_crSystem[72]);
			pDC->DrawLine(pHotPT->x - 4, pHotPT->y + 3, pHotPT->x + 3, pHotPT->y - 4, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x - 5, pHotPT->y, pHotPT->x - 5, pHotPT->y + 5, m_crSystem[75]);
			pDC->DrawLine(pHotPT->x, pHotPT->y + 5, pHotPT->x - 5, pHotPT->y + 5, m_crSystem[75]);
		} else
		{
			gpCurWisp->m_MouseCursorPos.x = 6;
			gpCurWisp->m_MouseCursorPos.y = 6;
			gpCurWisp->m_MouseCursorSize.cx = 11;
			gpCurWisp->m_MouseCursorSize.cy = 11;
		}
	}

	void CWispDrawObj::DrawListBK(CWispDC *pDC, WISP_RECT *pRect)
	{
		DrawToolbarBK(pDC, pRect);
	}

	void CWispDrawObj::DrawListFocusRect(CWispDC *pDC, WISP_RECT *pRect)
	{
		pDC->DrawFullRect(pRect, m_crSystem[41]);
	}

	void CWispDrawObj::X88(CWispDC *pDC, WISP_RECT *pRect) //DrawPopupMenuBK
	{
		pDC->DrawFullRect(pRect, m_crSystem[50]);
	}

	void CWispDrawObj::DrawMenuItem(CWispDC *pDC, WISP_RECT *pRect, unsigned int State)
	{
		if (State == 1) //WISP_BT_DIB_HOVER
			pDC->DrawFullRect(pRect, m_crSystem[43]);
		else	pDC->DrawFullRect(pRect, m_crSystem[50]);
	}
	void CWispDrawObj::DrawMenuItemText(CWispDC *pDC, WISP_RECT *pRect, const WCHAR *Text, unsigned int Type)
	{
		pDC->DrawShadowText(pRect, Text, 1, -1, 0);
	}
	void CWispDrawObj::X94(CWispDC *pDC, WISP_RECT *pRect) //DrawPopupMenu
	{
		X88(pDC, pRect);
	}
	void CWispDrawObj::X98(CWispDC *pDC, WISP_RECT *pRect) //DrawPopupMenu
	{
		X88(pDC, pRect);
	}

	void CWispDrawObj::DrawCursor(WISP_POINT *pHotPT, CWispDC *pClipDC, unsigned int CursorType, CWispDIB *pDIB)
	{
		switch (CursorType)
		{
		case WISP_CT_ARROW:
			DrawCursorArrow(pClipDC, pHotPT);
			break;
		case WISP_CT_RESIZE_VERT:
			DrawCursorResizeVert(pClipDC, pHotPT);
			break;
		case WISP_CT_RESIZE_HORZ:
			DrawCursorResizeHorz(pClipDC, pHotPT);
			break;
		case WISP_CT_DRAG:
			DrawCursorDrag(pClipDC, pHotPT);
			break;
		case WISP_CT_RESIZE_LEFT_LEAN:
			DrawCursorResizeLeftLean(pClipDC, pHotPT);
			break;
		case WISP_CT_RESIZE_RIGHT_LEAN:
			DrawCursorResizeRightLean(pClipDC, pHotPT);
			break;
		default:
			if (pDIB)
				pClipDC->DrawDIB(pHotPT->x, pHotPT->y, pDIB);
			else	DrawCursorArrow(pClipDC, pHotPT);
			break;
		}
	}

	//


	CWispDIBDrawObj::CWispDIBDrawObj()
	{
		m_szSkin = "\\Skin\\Default";
	}

	CWispDIBDrawObj::~CWispDIBDrawObj()
	{
		//???
	}


	bool CWispDIBDrawObj::Init()
	{
		char szPicPath[MAX_PATH];


		TStrCpy(szPicPath, m_szSkin);
		TStrCat(szPicPath, "\\VertInc.bmp");
		m_DIBVertIncSpin.Load(szPicPath, 16, 16, 4);

		TStrCpy(szPicPath, m_szSkin);
		TStrCat(szPicPath, "\\VertDec.bmp");
		m_DIBVertDecSpin.Load(szPicPath, 16, 16, 4);

		TStrCpy(szPicPath, m_szSkin);
		TStrCat(szPicPath, "\\HorzInc.bmp");
		m_DIBHorzIncSpin.Load(szPicPath, 16, 16, 4);

		TStrCpy(szPicPath, m_szSkin);
		TStrCat(szPicPath, "\\HorzDec.bmp");
		m_DIBHorzDecSpin.Load(szPicPath, 16, 16, 4);

		TStrCpy(szPicPath, m_szSkin);
		TStrCat(szPicPath, "\\Min.bmp");
		m_DIBMin.Load(szPicPath, 16, 16, 4);

		TStrCpy(szPicPath, m_szSkin);
		TStrCat(szPicPath, "\\Max.bmp");
		m_DIBMax.Load(szPicPath, 16, 16, 4);

		TStrCpy(szPicPath, m_szSkin);
		TStrCat(szPicPath, "\\Normal.bmp");
		m_DIBRestore.Load(szPicPath, 16, 16, 4);

		TStrCpy(szPicPath, m_szSkin);
		TStrCat(szPicPath, "\\Close.bmp");
		m_DIBClose.Load(szPicPath, 16, 16, 4);

		TStrCpy(szPicPath, m_szSkin);
		TStrCat(szPicPath, "\\RedioBox.bmp");
		m_DIBRedioBoxIcon.Load(szPicPath, 13, 13, 6);
		m_DIBRedioBoxIcon.SetType(2);

		m_DIBArrow.Load("\\Cursor\\Arrow.bmp", -1);
		m_DIBArrow.SetType(2);

		m_DIBDrag.Load("\\Cursor\\Drag.bmp", -1);
		m_DIBDrag.SetType(2);
		m_DIBDrag.SetHotPoint(m_DIBDrag.m_PixelBuf.Width/2, m_DIBDrag.m_PixelBuf.Height/2);

		m_DIBHResize.Load("\\Cursor\\HResize.bmp", -1);
		m_DIBHResize.SetType(2);
		m_DIBHResize.SetHotPoint(m_DIBHResize.m_PixelBuf.Width/2, m_DIBHResize.m_PixelBuf.Height/2);

		m_DIBVResize.Load("\\Cursor\\VResize.bmp", -1);
		m_DIBVResize.SetType(2);
		m_DIBVResize.SetHotPoint(m_DIBVResize.m_PixelBuf.Width/2, m_DIBVResize.m_PixelBuf.Height/2);

		m_DIBLResize.Load("\\Cursor\\LResize.bmp", -1);
		m_DIBLResize.SetType(2);
		m_DIBLResize.SetHotPoint(m_DIBLResize.m_PixelBuf.Width/2, m_DIBLResize.m_PixelBuf.Height/2);

		m_DIBRResize.Load("\\Cursor\\RResize.bmp", -1);
		m_DIBRResize.SetType(2);
		m_DIBRResize.SetHotPoint(m_DIBRResize.m_PixelBuf.Width/2, m_DIBRResize.m_PixelBuf.Height/2);

		gpCurWisp->m_MouseCursorPos.x = m_DIBArrow.m_HotPT.x;
		gpCurWisp->m_MouseCursorPos.y = m_DIBArrow.m_HotPT.y;
		gpCurWisp->m_MouseCursorSize.cx = m_DIBArrow.m_PixelBuf.Width;
		gpCurWisp->m_MouseCursorSize.cy = m_DIBArrow.m_PixelBuf.Height;

		return true;
	}
	void CWispDIBDrawObj::Release()
	{
		m_DIBVertIncSpin.Destroy();
		m_DIBVertDecSpin.Destroy();
		m_DIBHorzIncSpin.Destroy();
		m_DIBHorzDecSpin.Destroy();
		m_DIBMin.Destroy();
		m_DIBMax.Destroy();
		m_DIBRestore.Destroy();
		m_DIBClose.Destroy();
		m_DIBArrow.Destroy();
		m_DIBRedioBoxIcon.Destroy();
		m_DIBHResize.Destroy();
		m_DIBVResize.Destroy();
		m_DIBLResize.Destroy();
		m_DIBRResize.Destroy();

	}
	void CWispDIBDrawObj::DrawBorder(CWispDC *pDC, WISP_RECT *pRect, int nSize)
	{
		for (int i = nSize-1; i >= 0; --i)
		{
			//unsigned long color = GrayColor(m_crSystem[28], 1, nSize, i);

			unsigned long color = m_crSystem[28];

			unsigned char v1 = 9+9*(nSize-i); //9*(1+nSize-i)
			unsigned char v2 = 15+15*(nSize-i);//15*(1+nSize-i)
			unsigned char v3 = 3+3*(nSize-i);//3*(1+nSize-i)
			
			unsigned long r16 = v1+(unsigned char)(color>>16);
			unsigned long r8 = v2+(unsigned char)(color>>8);
			unsigned long r0 = v3+(unsigned char)(color>>0);

			unsigned long color1 = ((unsigned long)r16<<16)|((unsigned long)r8<<8)|(r0<<0);

			pDC->DrawVLine(pRect->x + i, pRect->y + i, pRect->y2() - i - 1, color1);
			pDC->DrawVLine(pRect->x2() - i - 1, pRect->y + i, pRect->y2() - i - 1, color1);

			pDC->DrawHLine(pRect->x + i, pRect->x2() - i - 1, pRect->y + i, color1);
			pDC->DrawHLine(pRect->x + i, pRect->x2() - i - 1, pRect->y2() - i - 1, color1);
		}

	}
	void CWispDIBDrawObj::DrawCaption(CWispDC *pDC, WISP_RECT *pRect)
	{
		pDC->DrawYGrayColorFullRect2(m_crSystem[23], m_crSystem[24], pRect);
	}
	void CWispDIBDrawObj::DrawToolbarBK(CWispDC *pDC, WISP_RECT *pRect)
	{
		pDC->DrawYGrayColorFullRect2(0xACD6FF, 0x3E9EFF, pRect);
	}

	void CWispDIBDrawObj::DrawVertScrollSlideRect(CWispDC *pDC, WISP_RECT *pRect)
	{
		WISP_RECT rc = *pRect;

		pDC->DrawRoundRect(&rc, m_crSystem[58]);

		++rc.x;
		++rc.y;
		rc.cx -= 2;
		rc.cy -= 2;
		pDC->DrawRect(&rc, m_crSystem[1]);

		++rc.x;
		++rc.y;
		rc.cx -= 2;
		rc.cy -= 2;
		pDC->DrawXGrayColorFullRect2(m_crSystem[27], m_crSystem[26], &rc);
		if (rc.cy >= 10)
		{
			int i = 0;
			while (i < 8)
			{
				pDC->DrawHLine(rc.x + 2, rc.x2() - 3, rc.y + (rc.cy - 8) / 2 + i + 0, m_crSystem[13]);
				pDC->DrawHLine(rc.x + 2, rc.x2() - 3, rc.y + (rc.cy - 8) / 2 + i + 1, m_crSystem[12]);
				i += 2;
			}
		}
	}
	void CWispDIBDrawObj::DrawHorzScrollSlideRect(CWispDC *pDC, WISP_RECT *pRect)
	{
		WISP_RECT rc = *pRect;

		pDC->DrawRoundRect(&rc, m_crSystem[58]);

		++rc.x;
		++rc.y;
		rc.cx -= 2;
		rc.cy -= 2;
		pDC->DrawRect(&rc, m_crSystem[1]);

		++rc.x;
		++rc.y;
		rc.cx -= 2;
		rc.cy -= 2;
		pDC->DrawYGrayColorFullRect2(m_crSystem[27], m_crSystem[26], &rc);
		if (rc.cx >= 10)
		{
			int i = 0;
			while (i < 8)
			{
				pDC->DrawVLine(rc.x + (rc.cx - 8) / 2 + i + 0, rc.y + 2, rc.y2() - 3, m_crSystem[13]);
				pDC->DrawVLine(rc.x + (rc.cx - 8) / 2 + i + 1, rc.y + 2, rc.y2() - 3, m_crSystem[12]);
				i += 2;
			}
		}
	}

	void CWispDIBDrawObj::DrawSplitRect(CWispDC *pDC, WISP_RECT *pRect, bool bHorz)
	{
		if (bHorz)
			pDC->DrawXGrayChgFullRect(pRect, m_crSystem[28], 4);
		else	pDC->DrawYGrayChgFullRect(pRect, m_crSystem[28], 4);
	}

	void CWispDIBDrawObj::DrawSystemStandardIcon(CWispDC *pDC, WISP_RECT *pRect, unsigned int uStyle, unsigned int State)
	{
		switch (uStyle)
		{
		case WISP_SSI_VERT_INC_SPIN:
			pDC->DrawDIB(pRect->x, pRect->y, &m_DIBVertIncSpin.m_DIBList[State]);
			break;
		case WISP_SSI_VERT_DEC_SPIN:
			pDC->DrawDIB(pRect->x, pRect->y, &m_DIBVertDecSpin.m_DIBList[State]);
			break;
		case WISP_SSI_HORZ_INC_SPIN:
			pDC->DrawDIB(pRect->x, pRect->y, &m_DIBHorzIncSpin.m_DIBList[State]);
			break;
		case WISP_SSI_HORZ_DEC_SPIN:
			pDC->DrawDIB(pRect->x, pRect->y, &m_DIBHorzDecSpin.m_DIBList[State]);
			break;
		case WISP_SSI_CLOSE:
			pDC->DrawDIB(pRect->x, pRect->y, &m_DIBClose.m_DIBList[State]);
			break;
		case WISP_SSI_MINIMIZE:
			pDC->DrawDIB(pRect->x, pRect->y, &m_DIBMin.m_DIBList[State]);
			break;
		case WISP_SSI_MAXIMIZE:
			pDC->DrawDIB(pRect->x, pRect->y, &m_DIBMax.m_DIBList[State]);
			break;
		case WISP_SSI_RESTORE:
			pDC->DrawDIB(pRect->x, pRect->y, &m_DIBRestore.m_DIBList[State]);
			break;
		case WISP_SSI_REDIOBOX_SELECTED:
			pDC->DrawDIB(pRect->x, pRect->y, &m_DIBRedioBoxIcon.m_DIBList[3]);
			break;
		case WISP_SSI_REDIOBOX_UNSELECTED:
			pDC->DrawDIB(pRect->x, pRect->y, &m_DIBRedioBoxIcon.m_DIBList[0]);
			break;
		case WISP_SSI_REDIOBOX_SELECTED_DRAK:
			pDC->DrawDIB(pRect->x, pRect->y, &m_DIBRedioBoxIcon.m_DIBList[5]);
			break;
		case WISP_SSI_REDIOBOX_UNSELECTED_DRAK:
			pDC->DrawDIB(pRect->x, pRect->y, &m_DIBRedioBoxIcon.m_DIBList[2]);
			break;
		default:
			CWispDrawObj::DrawSystemStandardIcon(pDC, pRect, uStyle, State);
			break;
		}
	}

	void CWispDIBDrawObj::DrawCursorArrow(CWispDC *pDC, WISP_POINT *pHotPT)
	{
		if (pDC)
		{
			pDC->DrawDIB(pHotPT->x, pHotPT->y, &m_DIBArrow);
		} else
		{
			gpCurWisp->m_MouseCursorPos.x = m_DIBArrow.m_HotPT.x;
			gpCurWisp->m_MouseCursorPos.y = m_DIBArrow.m_HotPT.y;
			gpCurWisp->m_MouseCursorSize.cx = m_DIBArrow.m_PixelBuf.Width;
			gpCurWisp->m_MouseCursorSize.cy = m_DIBArrow.m_PixelBuf.Height;
		}
	}
	void CWispDIBDrawObj::DrawCursorDrag(CWispDC *pDC, WISP_POINT *pHotPT)
	{
		if (pDC)
		{
			pDC->DrawDIB(pHotPT->x, pHotPT->y, &m_DIBDrag);
		} else
		{
			gpCurWisp->m_MouseCursorPos.x = m_DIBDrag.m_HotPT.x;
			gpCurWisp->m_MouseCursorPos.y = m_DIBDrag.m_HotPT.y;
			gpCurWisp->m_MouseCursorSize.cx = m_DIBDrag.m_PixelBuf.Width;
			gpCurWisp->m_MouseCursorSize.cy = m_DIBDrag.m_PixelBuf.Height;
		}

	}
	void CWispDIBDrawObj::DrawCursorResizeVert(CWispDC *pDC, WISP_POINT *pHotPT)
	{
		if (pDC)
		{
			pDC->DrawDIB(pHotPT->x, pHotPT->y, &m_DIBVResize);
		} else
		{
			gpCurWisp->m_MouseCursorPos.x = m_DIBVResize.m_HotPT.x;
			gpCurWisp->m_MouseCursorPos.y = m_DIBVResize.m_HotPT.y;
			gpCurWisp->m_MouseCursorSize.cx = m_DIBVResize.m_PixelBuf.Width;
			gpCurWisp->m_MouseCursorSize.cy = m_DIBVResize.m_PixelBuf.Height;
		}

	}
	void CWispDIBDrawObj::DrawCursorResizeHorz(CWispDC *pDC, WISP_POINT *pHotPT)
	{
		if (pDC)
		{
			pDC->DrawDIB(pHotPT->x, pHotPT->y, &m_DIBHResize);
		} else
		{
			gpCurWisp->m_MouseCursorPos.x = m_DIBHResize.m_HotPT.x;
			gpCurWisp->m_MouseCursorPos.y = m_DIBHResize.m_HotPT.y;
			gpCurWisp->m_MouseCursorSize.cx = m_DIBHResize.m_PixelBuf.Width;
			gpCurWisp->m_MouseCursorSize.cy = m_DIBHResize.m_PixelBuf.Height;
		}
	}

	void CWispDIBDrawObj::DrawCursorResizeLeftLean(CWispDC *pDC, WISP_POINT *pHotPT)
	{
		if (pDC)
		{
			pDC->DrawDIB(pHotPT->x, pHotPT->y, &m_DIBLResize);
		} else
		{
			gpCurWisp->m_MouseCursorPos.x = m_DIBLResize.m_HotPT.x;
			gpCurWisp->m_MouseCursorPos.y = m_DIBLResize.m_HotPT.y;
			gpCurWisp->m_MouseCursorSize.cx = m_DIBLResize.m_PixelBuf.Width;
			gpCurWisp->m_MouseCursorSize.cy = m_DIBLResize.m_PixelBuf.Height;
		}

	}
	void CWispDIBDrawObj::DrawCursorResizeRightLean(CWispDC *pDC, WISP_POINT *pHotPT)
	{
		if (pDC)
		{
			pDC->DrawDIB(pHotPT->x, pHotPT->y, &m_DIBRResize);
		} else
		{
			gpCurWisp->m_MouseCursorPos.x = m_DIBRResize.m_HotPT.x;
			gpCurWisp->m_MouseCursorPos.y = m_DIBRResize.m_HotPT.y;
			gpCurWisp->m_MouseCursorSize.cx = m_DIBRResize.m_PixelBuf.Width;
			gpCurWisp->m_MouseCursorSize.cy = m_DIBRResize.m_PixelBuf.Height;
		}
	}

	void CWispDIBDrawObj::DrawListFocusRect(CWispDC *pDC, WISP_RECT *pRect)
	{
		pDC->DrawYGrayColorFullRect2(m_crSystem[41], m_crSystem[42], pRect);
	}

	void CWispDIBDrawObj::X88(CWispDC *pDC, WISP_RECT *pRect) //DrawPopupMenuBK
	{
		CWispDrawObj::X88(pDC, pRect);
	}

	void CWispDIBDrawObj::DrawMenuItem(CWispDC *pDC, WISP_RECT *pRect, unsigned int State)
	{
		if (State == 1) //WISP_BT_DIB_HOVER
			pDC->DrawYGrayColorFullRect2(m_crSystem[43], m_crSystem[44], pRect);
		else	pDC->DrawYGrayColorFullRect2(m_crSystem[50], m_crSystem[51], pRect);
	}
	void CWispDIBDrawObj::DrawMenuItemText(CWispDC *pDC, WISP_RECT *pRect, const WCHAR *Text, unsigned int Type)
	{
		if (Type == 0x40)
		{
			pDC->SetTextColor(m_crSystem[13]);
			pDC->SetTextShadowColor(m_crSystem[31]);
			pDC->DrawShadowText(pRect, Text, 1, -1, 0);
			pDC->SetTextColor();
			pDC->SetTextShadowColor();
		} else
		{
			pDC->DrawShadowText(pRect, Text, 1, -1, 0);
		}
	}

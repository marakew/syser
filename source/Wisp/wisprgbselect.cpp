
#include "wisprgbselect.hpp"


#define ID_RGB 2
#define ID_RED 3
#define ID_GREEN 4
#define ID_BLUE 5

WISP_FORM_RES_ITEM WispRGBSelect[] =
{
	{ WISP_CTRL_FORM, {0, 0, 350, 210}, 0, 0x0B, WSTR("RGB"), 0, 0},
	{ WISP_CTRL_STATIC_GROUP, {5, 5, 250, 170}, ID_RGB, 0, WSTR("RGB"), 0, 0},
	{ WISP_CTRL_BUTTON, {270, 130, 60, 18}, WISP_ID_CANCEL, 0, WSTR("Cancel"), 0, 0},
	{ WISP_CTRL_BUTTON, {270, 155, 60, 18}, WISP_ID_OK, 0, WSTR("OK"), 0, 0},
	{ WISP_CTRL_STATIC_STRING, {270, 56, 30, 16}, 0, 0, WSTR("Red"), 0, 0},
	{ WISP_CTRL_EDIT, {305, 55, 25, 16}, ID_RED, 0x9000006, 0, 0, 0},
	{ WISP_CTRL_STATIC_STRING, {270, 76, 30, 16}, 0, 0, WSTR("Green"), 0, 0},
	{ WISP_CTRL_EDIT, {305, 75, 25, 16}, ID_GREEN, 0x9000006, 0, 0, 0},
	{ WISP_CTRL_STATIC_STRING, {270, 96, 30, 16}, 0, 0, WSTR("Blue"), 0, 0},
	{ WISP_CTRL_EDIT, {305, 95, 25, 16}, ID_BLUE, 0x9000006, 0, 0, 0},
	{ 0 },
};

	CWispRGBSelect::CWispRGBSelect()
	{
		m_Resource = WispRGBSelect;
		m_Color = 0;
	}

	CWispRGBSelect::~CWispRGBSelect()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispRGBSelect)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
		WISP_MSG_MAP(WISP_WM_MOUSE_MOVE, OnMouseMove)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
		WISP_MSG_MAP_ON_COMMAND
	WISP_MSG_MAP_END(CWispForm)

	bool CWispRGBSelect::OnDestroy(WISP_MSG *pMsg)
	{
		m_GammaDIB.Destroy();
		m_ScaleDIB.Destroy();
		return true;
	}
	bool CWispRGBSelect::OnUpdateClient(WISP_MSG *pMsg)
	{
		if (!CWispWnd::OnUpdateClient(pMsg))
			return false;

		m_ClientDC.DrawDIB(&m_GammaRC, &m_GammaDIB);
		m_ClientDC.DrawFrameRect(&m_GammaFrameRC, 0, 1);
		
		m_ClientDC.DrawDIB(&m_ScaleRC, &m_ScaleDIB);
		m_ClientDC.DrawFrameRect(&m_ScaleFrameRC, 0, 1);

		m_ClientDC.DrawHLine(m_GammaPoint.x - 5, m_GammaPoint.x - 2, m_GammaPoint.y, 0);
		m_ClientDC.DrawHLine(m_GammaPoint.x + 2, m_GammaPoint.x + 5, m_GammaPoint.y, 0);
		m_ClientDC.DrawVLine(m_GammaPoint.x, m_GammaPoint.y - 5, m_GammaPoint.y - 2, 0);
		m_ClientDC.DrawVLine(m_GammaPoint.x, m_GammaPoint.y + 2, m_GammaPoint.y + 5, 0);

		m_ClientDC.DrawHLine(m_ScaleRC.x2(), m_ScaleRC.x2() + 5, m_nScale, 0);
		m_ClientDC.DrawLine(m_ScaleRC.x2(), m_nScale, m_ScaleRC.x2() + 3, m_nScale - 3, 0);
		m_ClientDC.DrawLine(m_ScaleRC.x2(), m_nScale, m_ScaleRC.x2() + 3, m_nScale + 3, 0);

		m_ClientDC.DrawFullRect(&m_ResultRC, m_Color);
		m_ClientDC.DrawFrameRect(&m_ResultRC, 1, 1);
		return false;
	}
	bool CWispRGBSelect::OnMouseMove(WISP_MSG *pMsg)
	{
		if (pMsg->bMsgLBTDown)
		{
			if (PtInRect(&m_GammaRC, &pMsg->m_LogicMousePosWnd))
			{
				m_GammaPoint = pMsg->m_LogicMousePosWnd;
				ReDrawGamma(true);
				Update(&m_ClientRect);
				return true;
			}

			if (PtInRect(&m_ScaleFullRC, &pMsg->m_LogicMousePosWnd))
			{
				m_nScale = pMsg->m_LogicMousePosWnd.y;
				MIN_LIMIT(m_nScale, m_ScaleRC.y);
				MAX_LIMIT(m_nScale, m_ScaleRC.y2() - 1);
				DrawScale();
				Update(&m_ClientRect);
				return true;
			}
		}
		return true;
	}
	bool CWispRGBSelect::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.bKeyDown && pMsg->KeyEvent.KeyType == VK_LBUTTON)
		{
			if (PtInRect(&m_GammaRC, &pMsg->m_LogicMousePosWnd))
			{
				m_GammaPoint = pMsg->m_LogicMousePosWnd;
				ReDrawGamma(true);
				Update(&m_ClientRect);
				return true;
			}

			if (PtInRect(&m_ScaleFullRC, &pMsg->m_LogicMousePosWnd))
			{
				m_nScale = pMsg->m_LogicMousePosWnd.y;
				DrawScale();
				Update(&m_ClientRect);
				return true;
			}
		}
		return true;
	}
	bool CWispRGBSelect::OnCreateForm(WISP_MSG *pMsg)
	{
		m_pStaticGroup = (CWispStaticGroup*)GetFormItem(ID_RGB);
		m_pRedEdit = (CWispEdit*)GetFormItem(ID_RED);
		m_pGreenEdit = (CWispEdit*)GetFormItem(ID_GREEN);
		m_pBlueEdit = (CWispEdit*)GetFormItem(ID_BLUE);

		m_GammaDIB.Create(m_pStaticGroup->m_WindowRect.cx - 55, m_pStaticGroup->m_WindowRect.cy - 25, 1, 0, 0);
		m_ScaleDIB.Create(20, m_GammaDIB.m_PaintRect.cy, 1, 0, 0);
		DrawGamma(&m_GammaDIB);

		int GammaCX = m_GammaDIB.m_PaintRect.cx;
		int GammaCY = m_GammaDIB.m_PaintRect.cy;

		int ScaleCY = m_ScaleDIB.m_PaintRect.cy;
		int ScaleCX = m_ScaleDIB.m_PaintRect.cx;

		m_GammaRC.x = m_pStaticGroup->m_WindowRect.x + 15;
		m_GammaRC.y = m_pStaticGroup->m_WindowRect.y + 20;

		m_GammaFrameRC.x = m_GammaRC.x - 1;
		m_GammaFrameRC.y = m_GammaRC.y - 1;
		m_GammaFrameRC.cx = GammaCX + 2;
		m_GammaFrameRC.cy = GammaCY + 2;

		m_ScaleRC.x = m_GammaRC.x + GammaCX + 10;
		m_ScaleRC.y = m_GammaRC.y;
		m_ScaleRC.cx = ScaleCX;
		m_ScaleRC.cy = ScaleCY;

		m_ScaleFullRC.x = m_ScaleRC.x;
		m_ScaleFullRC.y = m_GammaRC.y;
		m_ScaleFullRC.cx = m_ScaleRC.cx + 5;
		m_ScaleFullRC.cy = ScaleCY;

		m_GammaRC.cx = GammaCX;
		m_GammaRC.cy = GammaCY;

		m_ScaleFrameRC.x = m_GammaRC.x + GammaCX + 9;
		m_ScaleFrameRC.y = m_GammaRC.y - 1;
		m_ScaleFrameRC.cx = ScaleCX + 2;
		m_ScaleFrameRC.cy = ScaleCY + 2;

		m_GammaPoint.x = m_GammaRC.x + GammaCX/2;
		m_GammaPoint.y = m_GammaRC.y + GammaCY/2;

		m_ScaleFullRC.y -= 5;
		m_ScaleFullRC.cy += 10;

		m_ResultRC.x = 270;
		m_ResultRC.y = 10;
		m_ResultRC.cx = 60;
		m_ResultRC.cy = 40;

		m_nScale = m_GammaRC.y + ScaleCY/2;
		
		ReDrawGamma(false);
		ShowColorValue();
		return true;
	}
#if 0
	WISP_CMD_MAP_BEGIN(CWispRGBSelect)
		WISP_CMD_MAP(WISP_ID_..., OnCmd...)
	WISP_CMD_MAP_END
#else	
	bool CWispRGBSelect::OnCommand(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdID && pMsg->Command.CmdMsg == 0x80000704)
			ReadColorValue();
		return true;
	}
#endif
	void CWispRGBSelect::ReadColorValue()
	{
		COLOR R,G,B;
		if (!USDecStrToNum(m_pRedEdit->m_WndText.operator const WCHAR*(), &R) || R>255)
			return;
		if (!USDecStrToNum(m_pGreenEdit->m_WndText.operator const WCHAR*(), &G) || G>255)
			return;
		if (!USDecStrToNum(m_pBlueEdit->m_WndText.operator const WCHAR*(), &B) || B>255)
			return;
		m_Color = RGBColor(R,G,B);
		Update((WISP_RECT*)0);
	}

	void CWispRGBSelect::ShowColorValue()
	{
		TWideString String;

		String.Format(WSTR("%d"), GetRColor(m_Color));
		m_pRedEdit->SetWindowText(String.operator const WCHAR*());

		String.Format(WSTR("%d"), GetGColor(m_Color));
		m_pGreenEdit->SetWindowText(String.operator const WCHAR*());

		String.Format(WSTR("%d"), GetBColor(m_Color));
		m_pBlueEdit->SetWindowText(String.operator const WCHAR*());
	}

	void CWispRGBSelect::DrawScale()
	{
		m_ScaleDIB.GetPixel(0, m_nScale - m_ScaleRC.y, &m_Color);
		ShowColorValue();
	}

	void //CWispRGBSelect::
	RGB2HSL(unsigned long Color, double &H, double &S, double &L)
	{
		double R = (double)GetRColor(Color)/255.;
		double G = (double)GetGColor(Color)/255.;
		double B = (double)GetBColor(Color)/255.;

		double var_Min = MIN(MIN(R, G), B);
		double var_Max = MAX(MAX(R, G), B);
		double del_Max = var_Max - var_Min;

		L = (var_Max + var_Min)/2;

		if (del_Max == 0)
		{
			H = 0;
			S = 0;
		} else
		{
			if (L < 0.5) S = del_Max/(var_Max + var_Min);
			else         S = del_Max/(2 - var_Max - var_Min);

			double del_R = ((var_Max-R)/6 + del_Max/2)/del_Max;
			double del_G = ((var_Max-G)/6 + del_Max/2)/del_Max;
			double del_B = ((var_Max-B)/6 + del_Max/2)/del_Max;

			if      (R == var_Max) H = del_B - del_G;
			else if (G == var_Max) H = (1./3.) + del_R - del_B;
			else if (B == var_Max) H = (2./3.) + del_G - del_R;

			if (H < 0) H += 1;
			if (H > 1) H -= 1;
		}
	}

	long double //CWispRGBSelect::
	Hue2RGB(double v1, double v2, double vH)
	{
		if (vH < 0.) vH += 1.;
		if (vH > 1.) vH -= 1.;
		if (vH*6. < 1.) return v1+ (v2-v1)*6.*vH;
		if (vH*2. < 1.) return v2;
		if (vH*3. < 2.) return v1+ (v2-v1)*(2./3. - vH)*6.;
		return v1;	
	}

#ifdef _RING0_
#if 0
unsigned int DoubleToU32(double x)
{
    return (unsigned int)_mm_cvtsd_si32(_mm_set_sd(x));
}

//If you need to cover whole [0..2^32) range for unsigned int conversion, you can add a bit more code to get it to work. In pseudocode:
unsigned int DoubleToU32(double x)
{
    if (x >= double(2^31))
    {
        return 2^31 + _mm_cvtsd_si32(x - double(2^31));
    }
    return _mm_cvtsd_si32(x);
}
#endif
	unsigned long double2uint(double v)
	{
		//TODO
		return 0;
	}
#else
	//unsigned int _dtoui3(double x) { ... }

	unsigned long double2uint(double v)
	{
		return v;
	}
#endif

	unsigned long //CWispRGBSelect::
	HLS2RGB(double H, double L, double S)
	{
		double R,G,B;

		if (S == 0)
		{
			R = L;
			G = L;
			B = L;
		} else
		{
			double var2;
			if (L <= 0.5)
				var2 = (S+1.)*L;
			else	var2 = L+S-S*L;
			double var1 = L*2 - var2;
			R = Hue2RGB(var1, var2, H+1./3);
			G = Hue2RGB(var1, var2, H);
			B = Hue2RGB(var1, var2, H-1./3);
		}

		return RGBColor(double2uint(R*255),double2uint(G*255),double2uint(B*255));
	}

	void CWispRGBSelect::ReDrawGamma(bool bGetPixel)
	{
		CWispDC WispDC;
		if (bGetPixel)
			m_GammaDIB.GetPixel(m_GammaPoint.x - m_GammaRC.x, m_GammaPoint.y - m_GammaRC.y, &m_Color);

		double H,S,L;
		RGB2HSL(m_Color, H, S, L);
		WispDC.Create(&m_ScaleDIB);
		WispDC.DrawHLine(0, m_ScaleRC.cx - 1, 0, 0xFFFFFF);
		for (int y = 1; y < m_ScaleRC.cy - 1; ++y)
		{
			unsigned long Color = HLS2RGB(H, (double)(m_ScaleRC.cy-y)/(double)m_ScaleRC.cy, S);
			WispDC.DrawHLine(0, m_ScaleRC.cx - 1, y, Color);
		}
		WispDC.DrawHLine(0, m_ScaleRC.cx - 1, m_ScaleRC.cy - 1, 0);
		ShowColorValue();
	}

	void CWispRGBSelect::DrawGamma(CWispDIB *pDIB)
	{
		CWispDC DrawDC;
		DrawDC.Create(pDIB);
		for (int PosY = 0; PosY < pDIB->m_PaintRect.cy; ++PosY)
		{
			for (int PosX = 0; PosX < pDIB->m_PaintRect.cx; ++PosX)
			{
				unsigned long Color = HLS2RGB((double)PosX/(double)pDIB->m_PaintRect.cx, 1./2., (double)PosY/(double)pDIB->m_PaintRect.cy);
				DrawDC.DrawPixel(PosX, pDIB->m_PaintRect.cy-PosY-1, Color);
			}
		}
	}

	bool CWispRGBSelect::StaticCreate(unsigned long *pRetColor, unsigned long *pColor)
	{
		CWispRGBSelect RGBSelect;
		if (pColor)
			RGBSelect.m_Color = *pColor;
		RGBSelect.Create(0, WISP_SH_MODAL_BLOCK);
		if (RGBSelect.m_Result == WISP_ID_OK)
		{
			*pRetColor = RGBSelect.m_Color;
			return true;
		}
		return false;
	}
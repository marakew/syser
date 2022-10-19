
#ifndef _WISPRGBSELECT_HPP_
#define _WISPRGBSELECT_HPP_

#include "../Code/define.h"
#include "wispform.hpp"
#include "wispdib.hpp"
#include "wispedit.hpp"
#include "wispstatic.hpp"

struct CWispRGBSelect : public CWispForm
{
	WISP_RECT m_GammaFrameRC;
	WISP_RECT m_GammaRC;
	WISP_RECT m_ScaleFrameRC;
	WISP_RECT m_ScaleRC;
	WISP_RECT m_ScaleFullRC;
	WISP_RECT m_ResultRC;
	WISP_POINT m_GammaPoint;
	int m_nScale;
	CWispDIB m_GammaDIB;
	CWispDIB m_ScaleDIB;
	CWispStaticGroup *m_pStaticGroup;
	CWispEdit *m_pRedEdit;
	CWispEdit *m_pGreenEdit;
	CWispEdit *m_pBlueEdit;
	unsigned long m_Color;

	CWispRGBSelect();
	virtual ~CWispRGBSelect();
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnUpdateClient(WISP_MSG *pMsg);
	bool OnMouseMove(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnCreateForm(WISP_MSG *pMsg);

	bool OnCommand(WISP_MSG *pMsg);

	void ReadColorValue();
	void ShowColorValue();
	void DrawScale();

	//void RGB2HSL(unsigned long Color, double &H, double &S, double &L);
	//long double Hue2RGB(double v1, double v2, double vH);
	//unsigned long HLS2RGB(double H, double L, double S);

	void ReDrawGamma(bool bGetPixel);
	void DrawGamma(CWispDIB *pDIB);

	static bool StaticCreate(unsigned long *pRetColor, unsigned long *pColor);
};

#endif

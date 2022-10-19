
#ifndef _WISPDRAWOBJ_HPP_
#define _WISPDRAWOBJ_HPP_

#include "../Code/define.h"
#include "utility.hpp"
#include "wispdc.hpp"
#include "wispdib.hpp"

enum WISP_SSI_ENUM
{
	WISP_SSI_VERT_INC_SPIN  = 0,
	WISP_SSI_VERT_DEC_SPIN  = 1,
	WISP_SSI_HORZ_INC_SPIN  = 2,
	WISP_SSI_HORZ_DEC_SPIN  = 3,
	WISP_SSI_CLOSE   = 4,
	WISP_SSI_MINIMIZE  = 5,
	WISP_SSI_MAXIMIZE  = 6,
	WISP_SSI_RESTORE  = 7,
	WISP_SSI_DISABLED  = 8,
	WISP_SSI_EXPANDBUTTON_EXPAND  = 9,
	WISP_SSI_EXPANDBUTTON_COLLAPSE  = 10,
	WISP_SSI_CHECKBOX_UNCHECKED  = 11,
	WISP_SSI_CHECKBOX_CHECKED  = 12,
	WISP_SSI_CHECKBOX_SEMICHECKED  = 13,
	WISP_SSI_CHECKBOX_CHECKMARK  = 14,
	WISP_SSI_REDIOBOX_SELECTED  = 15,
	WISP_SSI_REDIOBOX_UNSELECTED  = 16,
	WISP_SSI_REDIOBOX_SELECTED_DRAK  = 17,
	WISP_SSI_REDIOBOX_UNSELECTED_DRAK  = 18,
};

enum WISP_BT_DIB
{
	WISP_BT_DIB_NORMAL = 0,
	WISP_BT_DIB_HOVER = 1,
	WISP_BT_DIB_DOWN = 2,
	WISP_BT_DIB_DISABLE = 3,
	WISP_BT_DIB_COUNT = 4,
};

enum SYSTEM_COLOR
{
	SC_BLACK = 0,
	SC_WHITE = 1,
	SC_RED = 2,
	SC_LIGHT_RED = 3,
	SC_GREEN = 4,
	SC_LIGHT_GREEN = 5,
	SC_BLUE = 6,
	SC_LIGHT_BLUE = 7,
	SC_BROWN = 8,
	SC_LIGHT_BROWN = 9,
	SC_CYAN = 10,
	SC_LIGHT_CYAN = 11,
	SC_GRAY = 12,
	SC_LIGHT_GRAY = 13,
	SC_YELLOW = 14,
	SC_LIGHT_YELLOW = 15,
	SC_ORANGE_RED = 16,
	SC_LIGHT_ORANGE_RED = 17,

	SC_DESKTOP_BG = 18,
	SC_DISABLE_BG = 19,
	SC_CLIENT_BG = 20,
	SC_CLIENT_TEXT = 21,
	SC_CAPTION_TEXT = 22,
	SC_CAPTION_BG = 23,
	SC_CAPTION_LIGHT_BG = 24,
	SC_SELECT_BG = 25,
	SC_SCROLL_SLIDE_LIGHT_BG = 26,
	SC_SCROLL_SLIDE_BG = 27,
	SC_BORDER_BG = 28,
	SC_BRIGHT = 29,
	SC_SHADOW = 30,
	SC_SHADOW_TEXT = 31,
	SC_SCROLL_BG = 32,
	SC_SCROLL_BT_BG = 33,
	SC_CURSOR = 34,

	SC_CTRL_NORMAL_BORDER_TOPLEFT = 35,
	SC_CTRL_NORMAL_BORDER_BOTTOMRIGHT = 36,
	SC_CTRL_NORMAL_DARK_BORDER = 37,
	SC_CTRL_NORMAL_CLIENT_BG = 38,
	SC_CTRL_NORMAL_DARK_CLIENT_BG = 39,
	SC_CTRL_NORMAL_DARK_CLIENT_BG2 = 40,
	SC_CTRL_FOCUS_BG = 41,
	SC_CTRL_FOCUS_LIGHT_BG = 42,
	SC_CTRL_SELECTED_BG = 43,
	SC_CTRL_SELECTED_LIGHT_BG = 44,
	SC_CTRL_SELECTED_TEXT = 45,
	SC_CTRL_NORMAL_TEXT = 46,
	SC_CTRL_HOVER_TEXT = 47,
	SC_CTRL_NORMAL_FOCUS = 48,
	SC_CTRL_HOVER_FOCUS = 49,

	SC_MENU_BG = 50,
	SC_MENU_LIGHT_BG = 51,
	SC_MENU_BORDER = 52,

	SC_STDICON_NORMAL_BORDER = 53,
	SC_STDICON_HOVER_BORDER = 54,
	SC_STDICON_DOWN_BORDER = 55,
	SC_STDICON_NORMAL_CHECKMARK = 56,
	SC_STDICON_NORMAL_PLUS = 57,

	SC_SCROLL_NORMAL_BUTTON = 58,
	SC_SCROLL_HOVER_BUTTON = 59,
	SC_SCROLL_DOWN_BUTTON = 60,

	SC_FRAME_NORMAL_BG = 61,
	SC_FRAME_NORMAL_LEFT = 62,
	SC_FRAME_NORMAL_RIGHT = 63,
	SC_FRAME_NORMAL_BOTTOM1 = 64,
	SC_FRAME_NORMAL_BOTTOM2 = 65,
	SC_FRAME_NORMAL_BOTTOM3 = 66,

	SC_FRAME_HOVER_BG = 67,
	SC_FRAME_HOVER_BOTTOM1 = 68,
	SC_FRAME_HOVER_BOTTOM2 = 69,
	SC_FRAME_HOVER_BOTTOM3 = 70,

	SC_GRADIENT_GREEN_1 = 71,
	SC_GRADIENT_GREEN_2 = 72,
	SC_GRADIENT_GREEN_3 = 73,
	SC_GRADIENT_GREEN_4 = 74,
	SC_GRADIENT_GREEN_5 = 75,
	SC_GRADIENT_GREEN_6 = 76,
	SC_GRADIENT_GREEN_7 = 77,

	SC_GRADIENT_YELLOW_0 = 78,
	SC_GRADIENT_YELLOW_1 = 79,
	SC_GRADIENT_YELLOW_2 = 80,
	SC_GRADIENT_YELLOW_3 = 81,

	SC_GRADIENT_BULE_1 = 82,
	SC_GRADIENT_BULE_2 = 83,
	SC_GRADIENT_BULE_3 = 84,
	SC_GRADIENT_BULE_4 = 85,
	SC_GRADIENT_BULE_5 = 86,
	SC_GRADIENT_BULE_6 = 87,
	SC_GRADIENT_BULE_7 = 88,
	SC_GRADIENT_BULE_8 = 89,

	SC_RAV_1 = 90,
	SC_RAV_2 = 91,
	SC_RAV_3 = 92,
	SC_RAV_4 = 93,
	SC_RAV_5 = 94,

	CTRL_STATIC_EDGE__WISP_COLOR_95 = 95,
	CTRL_STATIC_EDGE__WISP_COLOR_96 = 96,
	CTRL_STATIC_EDGE__WISP_COLOR_97 = 97,
	SC_FRAME_DOWN_BG = 98,
	SC_FRAME_DOWN = 99,
	WISP_COLOR_100 = 100,
	WISP_COLOR_101 = 101,
	WISP_COLOR_102 = 102,
	WISP_COLOR_103 = 103,
	WISP_COLOR_104 = 104,
	WISP_COLOR_105 = 105,
	WISP_COLOR_106 = 106,
	WISP_COLOR_107 = 107,
	WISP_COLOR_108 = 108,
	WISP_COLOR_109 = 109,
	WISP_COLOR_110 = 110,
	TIP__WISP_COLOR_111 = 111,
	WISP_COLOR_112 = 112,
	WISP_COLOR_113 = 113,
	WISP_COLOR_114 = 114,
	WISP_COLOR_115 = 115,
	WISP_COLOR_116 = 116,
};

struct CWispDrawObj
{
	CWispDrawObj();

	virtual ~CWispDrawObj();

	unsigned long m_crSystem[116];

	virtual bool Init();
	virtual void Release();
	virtual void InitSystemColor();
	virtual void DrawBorder(CWispDC *pDC, WISP_RECT *pRect, int nSize);
	virtual void DrawCaption(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawToolbarBK(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawClient(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawCtrlBorder(CWispDC *pDC, WISP_RECT *pRect, int nSize);
	virtual void DrawCtrlClient(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawCtrlStaticEdge(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawCtrlClientEdge(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawCtrlModalFrame(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawCtrlFocusRect(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawVertScrollRect(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawVertScrollSlideRect(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawHorzScrollRect(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawHorzScrollSlideRect(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawSplitRect(CWispDC *pDC, WISP_RECT *pRect, bool bHorz);
	virtual void DrawButtonClient(CWispDC *pDC, WISP_RECT *pRect, unsigned int State);
	virtual void DrawButtonFrameRect(CWispDC *pDC, WISP_RECT *pRect, unsigned int State);
	virtual void DrawTabBackground(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawTabItem(CWispDC *pDC, WISP_RECT *pRect, unsigned int State);
	virtual void DrawHeaderFrameRect(CWispDC *pDC, WISP_RECT *pRect, unsigned int State);
	virtual void DrawProgress(CWispDC *pDC, WISP_RECT *pRect, unsigned int uStyle, int nPersent);
	virtual void DrawSystemStandardIcon(CWispDC *pDC, WISP_RECT *pRect, unsigned int uStyle, unsigned int State);

	virtual void DrawCursorArrow(CWispDC *pDC, WISP_POINT *pHotPT);
	virtual void DrawCursorDrag(CWispDC *pDC, WISP_POINT *pHotPT);
	virtual void DrawCursorResizeVert(CWispDC *pDC, WISP_POINT *pHotPT);
	virtual void DrawCursorResizeHorz(CWispDC *pDC, WISP_POINT *pHotPT);
	virtual void DrawCursorResizeLeftLean(CWispDC *pDC, WISP_POINT *pHotPT);
	virtual void DrawCursorResizeRightLean(CWispDC *pDC, WISP_POINT *pHotPT);

	virtual void DrawListBK(CWispDC *pDC, WISP_RECT *pRect);
	virtual void DrawListFocusRect(CWispDC *pDC, WISP_RECT *pRect);

	virtual void X88(CWispDC *pDC, WISP_RECT *pRect); //DrawPopupMenuBK

	virtual void DrawMenuItem(CWispDC *pDC, WISP_RECT *pRect, unsigned int State);
	virtual void DrawMenuItemText(CWispDC *pDC, WISP_RECT *pRect, const WCHAR *Text, unsigned int Type);

	virtual void X94(CWispDC *pDC, WISP_RECT *pRect); //DrawPopupMenu
	virtual void X98(CWispDC *pDC, WISP_RECT *pRect); //DrawPopupMenu

	void DrawCursor(WISP_POINT *pHotPT, CWispDC *pClipDC, unsigned int CursorType, CWispDIB *pDIB);
};


struct CWispDIBDrawObj : public CWispDrawObj
{
	CWispDIBDrawObj();

	virtual ~CWispDIBDrawObj();

	CWispDIBList m_DIBVertIncSpin;
	CWispDIBList m_DIBVertDecSpin;
	CWispDIBList m_DIBHorzIncSpin;
	CWispDIBList m_DIBHorzDecSpin;
	CWispDIBList m_DIBClose;
	CWispDIBList m_DIBMin;
	CWispDIBList m_DIBMax;
	CWispDIBList m_DIBRestore;
	CWispDIBList m_DIBRedioBoxIcon;
	CWispDIB m_DIBArrow;
	CWispDIB m_DIBHResize;
	CWispDIB m_DIBVResize;
	CWispDIB m_DIBLResize;
	CWispDIB m_DIBRResize;
	CWispDIB m_DIBDrag;
	const char *m_szSkin;

	virtual bool Init() override;
	virtual void Release() override;
	virtual void DrawBorder(CWispDC *pDC, WISP_RECT *pRect, int nSize) override;
	virtual void DrawCaption(CWispDC *pDC, WISP_RECT *pRect) override;
	virtual void DrawToolbarBK(CWispDC *pDC, WISP_RECT *pRect) override;

	virtual void DrawVertScrollSlideRect(CWispDC *pDC, WISP_RECT *pRect) override;
	virtual void DrawHorzScrollSlideRect(CWispDC *pDC, WISP_RECT *pRect) override;

	virtual void DrawSplitRect(CWispDC *pDC, WISP_RECT *pRect, bool bHorz) override;

	virtual void DrawSystemStandardIcon(CWispDC *pDC, WISP_RECT *pRect, unsigned int uStyle, unsigned int State) override; //WISP_SSI_ENUM uStyle

	virtual void DrawCursorArrow(CWispDC *pDC, WISP_POINT *pHotPT) override;
	virtual void DrawCursorDrag(CWispDC *pDC, WISP_POINT *pHotPT) override;
	virtual void DrawCursorResizeVert(CWispDC *pDC, WISP_POINT *pHotPT) override;
	virtual void DrawCursorResizeHorz(CWispDC *pDC, WISP_POINT *pHotPT) override;

	virtual void DrawCursorResizeLeftLean(CWispDC *pDC, WISP_POINT *pHotPT) override;
	virtual void DrawCursorResizeRightLean(CWispDC *pDC, WISP_POINT *pHotPT) override;

	virtual void DrawListFocusRect(CWispDC *pDC, WISP_RECT *pRect) override;

	virtual void X88(CWispDC *pDC, WISP_RECT *pRect) override; //DrawPopupMenuBK

	virtual void DrawMenuItem(CWispDC *pDC, WISP_RECT *pRect, unsigned int State) override;
	virtual void DrawMenuItemText(CWispDC *pDC, WISP_RECT *pRect, const WCHAR *Text, unsigned int Type) override;
};

#endif


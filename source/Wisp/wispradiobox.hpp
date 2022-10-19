
#ifndef _WISPRADIOBOX_HPP_
#define _WISPRADIOBOX_HPP_

#include "../Code/define.h"
#include "wispbasewnd.hpp"

struct CWispRadioBox : public CWispBaseWnd
{
	CWispRect m_TextRect;
	CWispRect m_RadioBoxRect;
//	bool m_bIsThreeState;
//	int m_CurrentState;
//	unsigned int m_uStyle;
	bool m_bIsMultiLine;//???
	bool m_bFocus;//???
	bool m_bMouseDown;
	bool m_bMouseIn;
//	bool m_EnableState;

	CWispRadioBox();
	virtual ~CWispRadioBox();
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnUpdate(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnMouseLDown(WISP_MSG *pMsg);
	bool OnMouseLUp(WISP_MSG *pMsg);
//	bool OnGetFocus(WISP_MSG *pMsg) { m_bFocus = 1; return true; }
//	bool OnLostFocus(WISP_MSG *pMsg) { m_bFocus = 0; return true; }
	bool OnMouseLeave(WISP_MSG *pMsg);
	bool OnMouseMove(WISP_MSG *pMsg);
	virtual bool InitWnd() override;
//	void DrawFocusRect(WISP_MSG *pMsg);
//	void DrawBorder();
//	void DrawCheckBox(WISP_MSG *pMsg);
	void DrawCaption();
//	int SetState(int newState);
//	int GetState();
	void Enable(bool bEnable);
	void UpdateForm();
};

#endif

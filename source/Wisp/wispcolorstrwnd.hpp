
#ifndef _WISPCOLORSTRWND_HPP_
#define _WISPCOLORSTRWND_HPP_

#include "../Code/define.h"
#include "wispwnd.hpp"

#define WISP_TERMINAL_LINE_SIZE 256

struct CWispColorStrWnd : public CWispWnd
{
	CWispColorStrWnd();
	virtual ~CWispColorStrWnd();
	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreate(WISP_MSG *pMsg);
	bool OnUpdateClient(WISP_MSG *pMsg);
	bool OnScrollEvent(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnRecalcLayout(WISP_MSG *pMsg);
	bool OnMouseMove(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	bool OnEventCopy(WISP_MSG *pMsg);

	virtual bool InitWnd() override;
	virtual unsigned char GetLineStr(int LineIndex, unsigned long *pColorStr) = 0;
	virtual void SetMaxLines(int nLine);
	virtual void SetMaxChars(int nChar);
	void CalcPonitStrPos(WISP_POINT *pPoint, WISP_POINT *pStrPT);
	void UpdateCaretState();
	void UpdateSelect();
	void ResetSelect();

	WISP_POINT m_ScrPos;
	WISP_POINT m_CaretPos;
	WISP_POINT m_RealSelStartPos;
	WISP_POINT m_RealSelEndPos;
	WISP_POINT *m_pSelStartPos;
	WISP_POINT *m_pSelEndPos;
	bool m_bSelect;
	
	int m_Margin;
	int m_nCharsPerLine;
	int m_nLinesPerPage;
	int m_MaxChars;
	int m_MaxLines;

	bool IsSelectChar(int SrcX, int ScrY);
};


#endif
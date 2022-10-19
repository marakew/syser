
#ifndef _WISPEDIT_HPP_
#define _WISPEDIT_HPP_

#include "../Code/define.h"
#include "wispwnd.hpp"

struct _SELECTPOS
{
	int begin;
	int end;
};

struct CWispEdit : public CWispWnd
{
	int m_CharLenArray[320];
	int m_CurrentCaretHot;
	int m_CurrentCaretLine;
	int m_CurrentStringLen;
	char m_bIsMultiLine;
	int m_StringMaxLen;
	int m_LineMaxLen;
	unsigned int m_TextRowNum;
	unsigned int m_TextColNum;
	int m_CurrentLineCharIndex;
	int m_CurrentLineNum;
	WISP_RECT m_CurrentShowRect;
	int m_DebugValue;
	char m_bSelectFlag;
	int m_SelectBegin;
	int m_SelectEnd;
	bool bMouseLeftButtonDown;
	WISP_RECT m_SelectRect;
	unsigned int m_SelectColor;

	CWispEdit();
	virtual ~CWispEdit();
	
	virtual bool CreateWnd(const WCHAR *Name, int x, int y, int cx, int cy, CWispBaseWnd *pParentWnd, unsigned int CmdID, unsigned int uStyle, unsigned int ShowMode) override;
	bool Create(WISP_RECT &RectAtWnd, unsigned int uStyle, CWispWnd *pParentWnd, unsigned int CmdID);

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnEventCopy(WISP_MSG *pMsg);
	bool OnEventPaste(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	bool OnGetFocus(WISP_MSG *pMsg);
	bool OnLostFocus(WISP_MSG *pMsg);
	bool OnChar(WISP_MSG *pMsg);
	bool OnUpdateClient(WISP_MSG *pMsg);
	bool OnUpdateBorder(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnMouseEvent(WISP_MSG *pMsg);
	bool OnMouseLeave(WISP_MSG *pMsg);
	bool OnMouseLeftButtonUp(WISP_MSG *pMsg);
	bool OnMouseLeftButtonDown(WISP_MSG *pMsg);
	bool OnMouseMove(WISP_MSG *pMsg);
//	void OnKeyBack();
	void OnLeftKey(WISP_MSG *pMsg);
	void OnRightKey(WISP_MSG *pMsg);
//	void OnUpKey(WISP_MSG *pMsg);
//	void OnDownKey(WISP_MSG *pMsg);
//	void OnReturnKey(WISP_MSG *pMsg);
	void OnBackKey(WISP_MSG *pMsg);
	void OnHomeKey(WISP_MSG *pMsg);
	void OnEndKey(WISP_MSG *pMsg);
	void OnDeleteKey(WISP_MSG *pMsg);
	//void OnPageUpKey(WISP_MSG *pMsg);
	//void OnPageDownKey(WISP_MSG *pMsg);
	//bool OnShiftKey(WISP_MSG *pMsg);
	//int GetMaxShowCharNum(WCHAR*, int, int*, char);
	bool IsInsertChar(WCHAR wch);
	int InsertChar(const WCHAR *pChar, int nPosition, int nCount);
	int DeleteChar(int nPosition, int nCount, WISP_MSG *pMsg);
	int GetCurrentLinePosition(int nIndex);
	//int GetPrevLineBeginPosition(int, int*);
	//int GetCaretLineCharWidth();
	int GetLineWidth(const WCHAR *pSrc);
	//int CalcLineNum(const WCHAR* pSrc, int nCount, int nPerLineMaxCharNum);
	//int CalcLineNumForWidth(const WCHAR* pSrc, int nCount, int nLineWidth);
	//int CaretLeftMove();
	//int CaretRightMove();
	void GetCurrentPosition(WISP_POINT *pPoint, int Len);
	virtual bool SetWindowText(const WCHAR *pString) override;
	int GetCharIndexByPoint(WISP_POINT point, WISP_POINT &retpoint);
	int Select(int Pos, int Len);
};

#endif

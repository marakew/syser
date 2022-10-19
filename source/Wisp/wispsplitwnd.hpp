
#ifndef _WISPSPLITWND_HPP_
#define _WISPSPLITWND_HPP_

#include "../Code/define.h"
#include "../Code/alt/altlist.hpp"
#include "wispwnd.hpp"

struct WISP_SPLIT_WND
{
	CWispWnd *pWnd;
	int SizeXorY;
	unsigned int Style;
	WISP_RECT SplitRect;
};

struct CWispSplitWnd : public CWispWnd
{
	CWispSplitWnd();
	virtual ~CWispSplitWnd();
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnUpdate(WISP_MSG *pMsg);
	bool OnUpdateClient(WISP_MSG *pMsg);
	bool OnMouseMove(WISP_MSG *pMsg);
	bool OnMouseLeave(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnSize(WISP_MSG *pMsg);
	bool OnMouseDblClick(WISP_MSG *pMsg);
	bool OnResizeChild(WISP_MSG *pMsg);
	WISP_SIZE m_Size;
	int m_SplitBorderSize;
	TList<WISP_SPLIT_WND> m_WndList;
	TListIter<WISP_SPLIT_WND> m_CurSplitWndIt;
	TListIter<WISP_SPLIT_WND> m_NextSplitWndIt;
	bool InsertWnd(CWispWnd *pWnd, unsigned int Style, int SizeXorY, int InsertPos);
	bool RemoveWnd(CWispWnd *pWnd);
	void AdjustWnd(int Pos);
	void AdjustWndPos();
	int SetSize(int Pos, int SizeXorY);
	int GetSize(int Pos, int *pSizeXorY);
};

#endif

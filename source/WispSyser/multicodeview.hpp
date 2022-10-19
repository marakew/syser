
#ifndef _MULTICODEVIEW_HPP_
#define _MULTICODEVIEW_HPP_

#include "../Code/alt/altmap.hpp"
#include "../Wisp/wispmultitabview.hpp"
#include "codeview.hpp"

struct CMultiCodeView : public CWispMultiTabView
{
	map<ULONG_PTR, ULONG_PTR> m_Bookmark;

	CMultiCodeView();
	virtual ~CMultiCodeView();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreate(WISP_MSG *pMsg);

	virtual void OnActiveWnd(CWispWnd *pWnd) override;

	CCodeView *InsertView(CCodeView *pSrcView);

	void UpdateContext();
	void ResetContext();

	ULONG_PTR GetActiveAddress();
	void SetActiveAddress(ULONG_PTR Address, bool bHistory);

	void ClearBM();
	bool IsBM(ULONG_PTR Address);
	void ToggleBM(ULONG_PTR Address);
	void ToggleBM();
	void NextBM();
	void PrevBM();
};

#endif
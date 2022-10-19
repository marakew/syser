
#ifndef _MULTIDATAVIEW_HPP_
#define _MULTIDATAVIEW_HPP_

#include "../Wisp/wispmultitabview.hpp"
#include "dataview.hpp"

struct CMultiDataView : public CWispMultiTabView
{

	CMultiDataView();
	virtual ~CMultiDataView();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreate(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	bool OnEventFullScr(WISP_MSG *pMsg);
	bool OnEventNewView(WISP_MSG *pMsg);

	CDataView *InsertView(CDataView *pSrcView);

	void UpdateContext();

	void DataWidthUpdate();

	void ResetContext();

	int GetActiveDataWidth();
	void SetActiveDataWidth(int Width);
	ULONG_PTR GetActiveAddress();
	void SetActiveAddress(ULONG_PTR Address, const WCHAR *pStrAddress);
};

#endif
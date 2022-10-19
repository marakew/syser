
#ifndef _CROSSREFERENCEFORM_HPP_
#define _CROSSREFERENCEFORM_HPP_

#include "../Wisp/wispform.hpp"
#include "../Wisp/wisplist.hpp"

struct CCrossReferenceForm : public CWispForm
{
	CWispList *m_pListWnd;

	CCrossReferenceForm();
	virtual ~CCrossReferenceForm();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreateForm(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdSelect(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	bool OnEventGoto(WISP_MSG *pMsg);
	bool OpenTRefList(TList<ULONG_PTR> & Addrs, ULONG_PTR Address);
	bool OpenOpRefList(TList<ULONG_PTR> & Addrs, ULONG_PTR Address);
};

#endif
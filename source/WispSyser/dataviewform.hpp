
#ifndef _DATAVIEWFORM_HPP_
#define _DATAVIEWFORM_HPP_

#include "../Wisp/wispform.hpp"
#include "../Wisp/wispedit.hpp"
#include "../Wisp/wispstatic.hpp"
#include "../Wisp/wispbutton.hpp"
#include "dataview.hpp"

struct CDataViewForm : public CWispForm
{
	CDataView m_DataView;
	CWispEdit *m_pAddressName;
	CWispButton *m_pGoto;
	CWispStaticGroup *m_pAddress;

	CDataViewForm();
	virtual ~CDataViewForm();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreateForm(WISP_MSG *pMsg);
	bool OnClose(WISP_MSG *pMsg);
	bool OnSize(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdEdit(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	bool OnEventGoto(WISP_MSG *pMsg);
	void UpdateContext();
	void Create();
};

#endif
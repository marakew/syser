
#ifndef _DATAOPERATORDLG_HPP_
#define _DATAOPERATORDLG_HPP_

#include "../Wisp/wispform.hpp"
#include "../Wisp/wispstatic.hpp"
#include "../Wisp/wispbutton.hpp"
#include "../Wisp/wispradiobox.hpp"
#include "../Wisp/wispcheckbox.hpp"
#include "../Wisp/wispedit.hpp"

struct CDataOperatorDlg : CWispForm
{
	CWispStaticStr *m_pNameDesc;
	CWispButton *m_pOK;
	CWispButton *m_pCancel;
	CWispRadioBox *m_pWidth8;
	CWispRadioBox *m_pWidth16;
	CWispRadioBox *m_pWidth32;
	CWispRadioBox *m_pWidth64;
	CWispRadioBox *m_pDec;
	CWispRadioBox *m_pHex;
	CWispCheckBox *m_pCount;
	CWispStaticStr *m_pValueName;
	CWispEdit *m_pBegin;
	CWispEdit *m_pEnd;
	CWispEdit *m_pValue;

	ULONG_PTR m_Begin;
	ULONG_PTR m_End;
	unsigned long m_Len;
	unsigned int m_Op;

	CDataOperatorDlg();
	virtual ~CDataOperatorDlg();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreateForm(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	bool Create(CWispWnd *pParentWnd, int ShowMode, ULONG_PTR Start, ULONG_PTR End, unsigned long Length, unsigned int CmdID);
	int GetWidth();
	bool OnCmd(WISP_MSG *pMsg);
	bool OnEventExecOp(WISP_MSG *pMsg);
	bool OnEventClose(WISP_MSG *pMsg);
};

#endif
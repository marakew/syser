
#ifndef _DATABPEDIT_HPP_
#define _DATABPEDIT_HPP_

#include "../Wisp/wispform.hpp"
#include "../Wisp/wispedit.hpp"
#include "../Wisp/wispcheckbox.hpp"
#include "../Wisp/wispradiobox.hpp"
#include "../EXEAnalyzer/debugger.hpp"

struct CDataBPEdit : public CWispForm
{
	BREAK_POINT *m_pBP;
	CWispEdit *m_pName;
	CWispEdit *m_pAddress;
	CWispEdit *m_pCondition;
	CWispEdit *m_pCommand;
	CWispCheckBox *m_pEnable;
	CWispRadioBox *m_pWriteOnly;
	CWispRadioBox *m_pReadWrite;
	CWispRadioBox *m_pExecute;
	CWispRadioBox *m_pIO;
	CWispRadioBox *m_pLenAll;
	CWispRadioBox *m_pLenByte;
	CWispRadioBox *m_pLenWord;
	CWispRadioBox *m_pLenDWord;
	bool m_bEnableBP;

	CDataBPEdit();
	virtual ~CDataBPEdit();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreateForm(WISP_MSG *pMsg);

	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdEdit(WISP_MSG *pMsg);
	bool OnCmdOK(WISP_MSG *pMsg);
};

#endif
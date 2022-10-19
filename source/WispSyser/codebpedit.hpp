
#ifndef _CODEBPEDIT_HPP_
#define _CODEBPEDIT_HPP_

#include "../Wisp/wispform.hpp"
#include "../Wisp/wispedit.hpp"
#include "../Wisp/wispcheckbox.hpp"
#include "../EXEAnalyzer/debugger.hpp"

struct CCodeBPEdit : public CWispForm
{
	BREAK_POINT *m_pBP;
	BREAK_POINT::MODULE *m_pModuleBP;
	CWispEdit *m_pName;
	CWispEdit *m_pAddress;
	CWispEdit *m_pCondition;
	CWispEdit *m_pCommand;
	CWispCheckBox *m_pEnable;
	bool m_bEnableBP;

	CCodeBPEdit();
	virtual ~CCodeBPEdit();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreateForm(WISP_MSG *pMsg);

	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdEdit(WISP_MSG *pMsg);
	bool OnCmdOK(WISP_MSG *pMsg);
};

#endif

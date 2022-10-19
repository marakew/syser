
#ifndef _WISPPROGRESSFORM_HPP_
#define _WISPPROGRESSFORM_HPP_

#include "../Code/define.h"
#include "wispform.hpp"
#include "wispprogress.hpp"
#include "wispstatic.hpp"

struct CWispProgressForm : public CWispForm
{
	CWispProgress *m_pProgress;
	CWispStaticStr *m_pStaticStr;

	CWispProgressForm();
	virtual ~CWispProgressForm();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreateForm(WISP_MSG *pMsg);

	void SetText(const WCHAR *Text);
};

#endif
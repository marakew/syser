
#ifndef _DEBUGGERSELECTFORM_HPP_
#define _DEBUGGERSELECTFORM_HPP_

#include "../Wisp/wispform.hpp"
#include "../Wisp/wisplist.hpp"

struct CDebuggerSelectForm : public CWispForm
{
	CWispList *m_pList;

	CDebuggerSelectForm();
	virtual ~CDebuggerSelectForm();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreateForm(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	bool OnEventPlug(WISP_MSG *pMsg);
	bool OnEventModuleList(WISP_MSG *pMsg);
	void Create();
};

#endif
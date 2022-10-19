
#ifndef _PLUGINLISTFORM_HPP_
#define _PLUGINLISTFORM_HPP_

#include "../Wisp/wispform.hpp"
#include "../Wisp/wisplist.hpp"

struct CPluginListForm : public CWispForm
{
	CWispList *m_pList;

	CPluginListForm();
	virtual ~CPluginListForm();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	void UpdateContext();
	bool OnCreateForm(WISP_MSG *pMsg);
	void Create();
};

#endif


#ifndef _ABOUTFORM_HPP_
#define _ABOUTFORM_HPP_

#include "../Wisp/wispform.hpp"

struct CAboutForm : public CWispForm
{
	CAboutForm();
	virtual ~CAboutForm();
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreateForm(WISP_MSG *pMsg);
};

#endif
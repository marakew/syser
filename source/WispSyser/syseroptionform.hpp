
#ifndef _SYSEROPTIONFORM_HPP_
#define _SYSEROPTIONFORM_HPP_

#include "../Wisp/wispoptionform.hpp"

struct CSyserOptionForm : public CWispOptionForm
{
	CSyserOptionForm();
	virtual ~CSyserOptionForm();
	virtual bool InitWnd() override;
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreateForm(WISP_MSG *pMsg);
	virtual void OnApply() override;

        virtual bool OnDefaulting() override;
        virtual void OnDefault() override;
};

#endif
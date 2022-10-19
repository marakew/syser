
#ifndef _WISPCOMBOBOX_HPP_
#define _WISPCOMBOBOX_HPP_

#include "../Code/define.h"
#include "wispedit.hpp"
#include "wisplist.hpp"
#include "wispbutton.hpp"

struct CWispComboBox : public CWispEdit
{
	unsigned int m_nCurSel;
	CWispList m_ComboList;
	CWispButton m_Button;

	CWispComboBox();
	virtual ~CWispComboBox();
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	bool OnEventButton(WISP_MSG *pMsg);
	bool OnEventComboList(WISP_MSG *pMsg);
	bool OnCmdComboList(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);

	void SetCurSel(int nSelect);
	int GetCurSel();
};

#endif

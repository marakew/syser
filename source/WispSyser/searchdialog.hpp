
#ifndef _SEARCHDIALOG_HPP_
#define _SEARCHDIALOG_HPP_

#include "../Wisp/wispform.hpp"
#include "../Wisp/wispedit.hpp"
#include "../Wisp/wispcheckbox.hpp"
#include "../Wisp/wispcombobox.hpp"

struct CSearchDialog : public CWispForm
{
	CWispComboBox *m_pWidth;
	CWispEdit *m_pBegin;
	CWispEdit *m_pEnd;
	CWispEdit *m_pValue;
	CWispCheckBox *m_pCase;
	ULONG_PTR m_Begin;
	ULONG_PTR m_End;
	ULONG_PTR m_SearchResult;
	bool m_bCase;
	char m_PatternBuffer[64];
	unsigned long m_SearchResultLen;

	CSearchDialog();
	virtual ~CSearchDialog();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreateForm(WISP_MSG *pMsg);

	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmd1(WISP_MSG *pMsg);

	bool OnEvent(WISP_MSG *pMsg);
	bool OnEventModule(WISP_MSG *pMsg);
	bool OnEventOK(WISP_MSG *pMsg);
	bool OnEventSearch(WISP_MSG *pMsg);
};

#endif
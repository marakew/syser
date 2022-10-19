
#ifndef _FINDSTRINGFORM_HPP_
#define _FINDSTRINGFORM_HPP_

#include "../Wisp/wispform.hpp"
#include "../Wisp/wispedit.hpp"
#include "../Wisp/wispbutton.hpp"

#include "../Code/alt/altmap.hpp"
#include "../Wisp/wisplist.hpp"
#include "../Wisp/wispmenu.hpp"

struct CStringReferenceList;

struct CFindStringForm : public CWispForm
{
	CWispButton *m_pFindNext;
	CWispButton *m_pClose;
	CWispEdit *m_pFindWhat;
	CStringReferenceList *m_pStringReferenceList;

	CFindStringForm();
	virtual ~CFindStringForm();
	
	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreateForm(WISP_MSG *pMsg);

	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdFindWhat(WISP_MSG *pMsg);

	bool OnEvent(WISP_MSG *pMsg);
	bool OnEvent1(WISP_MSG *pMsg);
};

struct CStringReferenceList : public CWispList
{
	unsigned long m_ShowFormat;
	int m_nSearchLen;
	CWispMenu m_PopupMenu;
	TWideString m_SearchString;
	map<unsigned int, unsigned int> m_KeyMap;
	CFindStringForm m_FindStringForm;
	CWispMenu::MenuItem *m_SearchAgainMenuItem;

	CStringReferenceList();
	virtual ~CStringReferenceList();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	void LoadKeyMap();

	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);

	bool OnCommand(WISP_MSG *pMsg);

	bool OnEvent(WISP_MSG *pMsg);
	bool OnEventSearch(WISP_MSG *pMsg);
	bool OnEventSearchAgain(WISP_MSG *pMsg);

	bool OnChar(WISP_MSG *pMsg);
	void PopupMenu();
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnMouseDblClick(WISP_MSG *pMsg);

	void InitContext(int,int);
	bool Search(const WCHAR *pString);
};

#endif

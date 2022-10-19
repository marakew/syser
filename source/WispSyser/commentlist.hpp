
#ifndef _COMMENTLIST_HPP_
#define _COMMENTLIST_HPP_

#include "../Wisp/wisplist.hpp"
#include "../Wisp/wispmenu.hpp"

struct CCommentList : public CWispList
{
	CWispMenu m_PopupMenu;
	CListStringItem *m_pItem;
	CWispMenu::MenuItem *m_pGotoItem;

	CCommentList();
	virtual ~CCommentList();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnClose(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnMouseDblClick(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	bool OnEventGoto(WISP_MSG *pMsg);
	bool OnEventRemove(WISP_MSG *pMsg);

	virtual bool Edit(CListStringItem *Item, int nCol, TWideString& String) override; //Content? 0x80000111 //+54
	virtual bool OnEditing(CListStringItem *Item, int nCol, TWideString& String) override; //80000113,80000112 //+58

	void Create();
	void UpdateContext();
};

#endif
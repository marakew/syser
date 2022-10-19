
#ifndef _WATCHLIST_HPP_
#define _WATCHLIST_HPP_

#include "../Wisp/wisplist.hpp"
#include "../Wisp/wispmenu.hpp"

struct CWatchList : public CWispList
{
	void ResetContext();
	void UpdateLine(CListStringItem *pItem);
	void UpdateContext();

	CListStringItem *m_pFirstItem;
	CWispMenu m_PopupMenu;
	CWispMenu::MenuItem *m_pMenuItemRemove;

	CWatchList();
	virtual ~CWatchList();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnUpdate(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnEventAdd(WISP_MSG *pMsg);
	bool OnEventDel(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	virtual bool Edit(CListStringItem *Item, int nCol, TWideString& String) override;
	virtual bool OnEditing(CListStringItem *Item, int nCol, TWideString& String) override;
	virtual void OnEdit(CListStringItem *Item, int nCol, TWideString& String) override;
};

#endif
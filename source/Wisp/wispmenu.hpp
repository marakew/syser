
#ifndef _WISPMENU_HPP_
#define _WISPMENU_HPP_

#include "../Code/define.h"
#include "../Code/tstrmem.hpp"
#include "../Code/alt/altlist.hpp"
#include "wispwnd.hpp"

//enum
//{
	//1<<0   0x0001 MENU_SUBMENU
	//1<<1   0x0002 MENU_CHECKBOX
	//1<<2   0x0004 MENU_DELIM
	//1<<3   0x0008

	//1<<4   0x0010
	//1<<5   0x0020
	//1<<6   0x0040
	//1<<7   0x0080

	//1<<8   0x0100
	//1<<9   0x0200
	//1<<10  0x0400
	//1<<11  0x0800

	//1<<12  0x1000
	//1<<13  0x2000
	//1<<14  0x4000
	//1<<15  0x8000

	//1<<16  0x10000
	//1<<17  0x20000
	//1<<18  0x40000
	//1<<19  0x80000 MENU_CHECKED

	//1<<20 0x100000 MENU_DISABLED
	//1<<21 0x200000
	//1<<22 0x400000
	//1<<23 0x800000

//};

struct WISP_MENU_GROUP_ITEM
{
	const WCHAR *pszItemText;
	unsigned int nItemID;
	unsigned int nDIBIndex;
	unsigned int nFlags;
	WISP_MENU_GROUP_ITEM *pSubMenu;
};

struct CWispMenu : public CWispWnd
{
	struct MenuItem
	{
		TWideString string;
		CWispDIB *pDIB;
		unsigned int nID;
		unsigned int nFlags;
		WISP_RECT Rect;
		unsigned long Color;
		union
		{
			//CWispMenu* pSubMenu;
			unsigned long long dwData; //UNION
		};
	};

	TList<MenuItem> m_MenuItemList;
	int m_SubMenuOpenHeight;
	int m_OpenMenuHeight;
	TListIter<MenuItem> m_CurMenuItemIter;
	int m_MenuOffset;
	int m_MenuBorderSize;
	int m_SubMenuOffset;
	int m_SubMenuBorderSize;
	int m_MenuHeight;
	int m_SubMenuHeight;
	int m_FullMenuHeight;
	int m_FullMenuWidth;
	bool m_bShowed;
	CWispMenu *pParentMenu;

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnUpdateClient(WISP_MSG *pMsg);
	bool OnRecalcLayout(WISP_MSG *pMsg);
	bool OnResizeChild(WISP_MSG *pMsg);
	bool OnLostFocus(WISP_MSG *pMsg);
	bool OnUpdateBorder(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnMouseMove(WISP_MSG *pMsg);
	bool OnMouseLeave(WISP_MSG *pMsg);

	bool CheckExist(CWispMenu *pMenu);
	CWispMenu *GetRootMenu();

	bool SetDIB(CWispMenu::MenuItem *Item, CWispDIB *pDIB);
	bool SetData(CWispMenu::MenuItem *Item, unsigned long long dwData);

	CWispMenu *GetMenu(CWispMenu::MenuItem *Item)
	{
		return (CWispMenu*)(Item->dwData);
	}

	bool CheckedItem(CWispMenu::MenuItem *Item);
	bool UnCheckedItem(CWispMenu::MenuItem *Item);

	bool EnableItem(CWispMenu::MenuItem *Item);
	bool DisableItem(CWispMenu::MenuItem *Item);

	bool Enable(CWispMenu::MenuItem *Item, bool bEnable);

	bool Checked(CWispMenu::MenuItem *Item);

	virtual bool InitWnd() override;

	void DrawMenu();
	void DrawPopupMenu();

	void NextItem();
	void PrevItem();
	void Adjust();

	virtual void SetOwner(CWispBaseWnd *pWnd) override;

	CWispMenu::MenuItem *GetItemIndex(int Pos);
	CWispMenu::MenuItem *GetItem(const WCHAR *Text);
	CWispMenu::MenuItem *GetItem(int nID);

	bool SetItemText(CWispMenu::MenuItem *Item, const WCHAR *Text);

	bool CheckedItem(int nID);
	bool UnCheckedItem(int nID);
	bool Checked(unsigned int nID, bool bChecked);
	bool EnableItem(int nID);
	bool DisableItem(int nID);
	bool Enable(unsigned int nID, bool bEnable);
	bool Point(WISP_POINT *pPoint, bool bCurent);
	void Open(CWispMenu::MenuItem *Item, bool bCurrent); //SetHoverItem/PopupMenu
	void HideMenu();

	virtual bool OnActiveMenu(CWispMenu::MenuItem *Item, int nID);

	void SwitchItem(CWispMenu::MenuItem *Item);
	void UpdateIter(TListIter<MenuItem> It);
	bool UpdateItem(CWispMenu::MenuItem *Item);
	int Clear();

	CWispMenu();
	virtual ~CWispMenu();
	
	bool RemoveMenu(int nID);
	bool RemoveMenu(CWispMenu::MenuItem *Item);
	CWispMenu::MenuItem * AppendMenu(int Pos, const WCHAR *Text, int nFlags, int nID, CWispDIB *pDIB, unsigned long long dwData);

	bool LoadPopupMenuGroup(WISP_MENU_GROUP_ITEM *pMenuGroup, CWispBaseWnd *pParentWnd, CWispDIBList *pDIBList);
	bool LoadMenuGroup(CWispWnd *pParentWnd, int CmdID, WISP_MENU_GROUP_ITEM *pMenuGroup, CWispDIBList *pDIBList);
};


#endif

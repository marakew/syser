
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "watchlist.hpp"
#include "syserconfig.hpp"

enum
{
  WL_EXPR = 0,
  WL_VALUE = 1,
  WL_CONTEXT = 2,
};

	void CWatchList::ResetContext()
	{
		DeleteAllChildrenItems(0);
		m_pFirstItem = InsertItem(0, 0, 0, 0, -1);
		Select(m_pFirstItem);
	}

	void CWatchList::UpdateLine(CListStringItem *pItem)
	{
		WCHAR Buffer[256];
		WCHAR *pStr = Buffer;
		char Encode = 0;
		ULONG_PTR Result;
		if (gpSyser->m_SyserUI.CalcExp(GetItemContent(pItem, WL_EXPR)->String.operator const WCHAR*(), &Result))
		{
			SetItemFormat(pItem, WL_VALUE, Result, WSTR("" HEXFMT ""));
			char szSym[256];
			if (gpSyser->m_SyserUI.GetSymbol(Result, szSym, 256) || // 64/256 ???
				(!gpSyser->m_SyserUI.ReadString(Result, Buffer, 256, &Encode) &&
				gpSyser->m_SyserUI.GetSymbolOffset(Result, szSym)))
			{
				TStrCpy(Buffer, szSym);
				pStr = Buffer;
			}
		} else
		{
			SetItemText(pItem, WL_VALUE, WSTR("" UNREADSTR ""));
		}
		SetItemText(pItem, WL_CONTEXT, pStr);
	}

	void CWatchList::UpdateContext()
	{
		CListStringItem *pItem = GetItem(0, 0);
		while (pItem && pItem != m_pFirstItem)
		{
			UpdateLine(pItem);
			pItem = GetNext(pItem, 0);
		}
		Update(&m_ClientRect);
	}

	CWatchList::CWatchList()
	{
		m_pFirstItem = 0;
	}

	CWatchList::~CWatchList()
	{
	}

	WISP_MSG_MAP_BEGIN(CWatchList)
	//	WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

WISP_MENU_GROUP_ITEM WatchListPopupMenu[] = 
{
	{WSTR("Add"), 0xF1, 0x80, 0, 0 }, //EVENT_ID_WATCH_ADD
	{WSTR("Remove"), 0xF4, 0xCA, 0, 0}, //EVENT_ID_WATCH_DEL
	{0},
};

	bool CWatchList::OnUpdate(WISP_MSG *pMsg)
	{
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		UpdateItemColor(0);
		//TODO
		return true;
	}

	bool CWatchList::OnCreate(WISP_MSG *pMsg)
	{
		m_ItemColor = gSyserColors.Color[1];
		m_Style |= 0x2000000;
		SetBGColor(gSyserColors.Color[0]);
		m_PopupMenu.LoadPopupMenuGroup(WatchListPopupMenu, this, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF));
		m_pMenuItemRemove = m_PopupMenu.GetItem(0xF4);
		InsertColumn(WSTR("Expression[F2]"), 90, 1, 0, -1);
		InsertColumn(WSTR("Value"), 70, 0, 0, -1);
		InsertColumn(WSTR("Context"), 60, 0, 0, -1);
		ResetContext();
		return true;
	}

	bool CWatchList::OnDestroy(WISP_MSG *pMsg)
	{
		m_PopupMenu.Destroy();
		return true;
	}

	bool CWatchList::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && !pMsg->KeyEvent.bKeyDown)
		{
			if (pMsg->KeyEvent.KeyType == VK_RBUTTON)
			{
				CListStringItem *Item = GetNext(0, 8);
				m_PopupMenu.Enable(m_pMenuItemRemove, Item && Item != m_pFirstItem);
				m_PopupMenu.Point(0, 1);
			} else
			if (pMsg->KeyEvent.KeyType == VK_F2)
			{
				CListStringItem *Item = GetNext(0, 8);
				if (Item)
					EditItem(Item, WL_EXPR);
			}
		}
		return true;
	}

	bool CWatchList::OnEventAdd(WISP_MSG *pMsg)
	{
		EditItem(m_pFirstItem, WL_EXPR);
		return true;
	}

	bool CWatchList::OnEventDel(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetNext(0, 8);
		if (Item && Item != m_pFirstItem)
			DeleteItem(Item);
		return true;
	}

	WISP_EVENT_MAP_BEGIN(CWatchList)
		WISP_EVENT_MAP(0xF1, OnEventAdd) //EVENT_ID_WATCH_ADD
		WISP_EVENT_MAP(0xF4, OnEventDel) //EVENT_ID_WATCH_DEL
	WISP_EVENT_MAP_END

	bool CWatchList::Edit(CListStringItem *Item, int nCol, TWideString& String)
	{
		return gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen != 0;
	}
	bool CWatchList::OnEditing(CListStringItem *Item, int nCol, TWideString& String)
	{
		if (Item == m_pFirstItem)
		{
			if (String.m_nLength)
			{
				m_pFirstItem = InsertItem(0, 0, 0, 0, -1);
				return true;
			}
		} else
		{
			if (String.m_nLength) return true;
			DeleteItem(Item);
		}
		return false;
	}
	void CWatchList::OnEdit(CListStringItem *Item, int nCol, TWideString& String)
	{
		if (nCol == WL_EXPR)
		{
			UpdateLine(Item);
		}
	}


#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "commentlist.hpp"
#include "syserconfig.hpp"

	WISP_MSG_MAP_BEGIN(CCommentList)
	//	WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_CLOSE, OnClose)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_MOUSE_DBLCLICK, OnMouseDblClick)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

	WISP_EVENT_MAP_BEGIN(CCommentList)
		WISP_EVENT_MAP(0xF2, OnEventGoto)
		WISP_EVENT_MAP(0xF4, OnEventRemove)
	WISP_EVENT_MAP_END

WISP_MENU_GROUP_ITEM CommentListPopupMenu[] =
{
	{WSTR("Goto"), 0xF2, 0x80, 0, 0},
	{WSTR("Remove"), 0xF4, 0xCA, 0, 0},
	{0},
};

	CCommentList::CCommentList()
	{
	}
	CCommentList::~CCommentList()
	{
	}
#if 0
	bool CCommentList::OnUpdate(WISP_MSG *pMsg)
	{
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		UpdateItemColor(0);
		return true;
	}
#endif
	bool CCommentList::OnCreate(WISP_MSG *pMsg)
	{
		m_pItem = 0;
		AttachTitleDIB(gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x93));
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		m_PopupMenu.LoadPopupMenuGroup(CommentListPopupMenu, this, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF));
		m_pGotoItem = m_PopupMenu.GetItem(0xF2);
		InsertColumn(WSTR("Module/Offset"), 120, 0, 0, -1);
		InsertColumn(WSTR("TimeStamp/Comment"), 150, 1, 0, -1);
		
		UpdateContext();
		return true;
	}

	bool CCommentList::OnDestroy(WISP_MSG *pMsg)
	{
		m_PopupMenu.Destroy();
		return true;
	}

	bool CCommentList::OnClose(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.FocusActive();
		return true;
	}

	bool CCommentList::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		if (!pMsg->KeyEvent.bKeyDown && pMsg->KeyEvent.KeyType == VK_RBUTTON)
		{
			m_pItem = FullFindItem(0, 8);
			m_PopupMenu.Enable(0xF2, m_pItem && (GetItemLevel(m_pItem)==1));
			if (m_pItem)
				m_PopupMenu.Point(0, 1);
		}
		return true;
	}

	bool CCommentList::OnMouseDblClick(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.KeyType != VK_LBUTTON) return true;

		{
			m_pItem = FullFindItem(0, 8);
			if (m_pItem && GetItemLevel(m_pItem) == 1)
			{
				unsigned long long Addr = GetItemValue(m_pItem, 1);
				CCommentModule *pCommentModule = (CCommentModule*)GetItemValue(GetItemParent(m_pItem), 0);
				CDbgModule *DbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(pCommentModule->m_ModuleName.operator const char*());
				if (!DbgMod)
					DbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(pCommentModule->m_ModuleName.operator const char*());
				if (DbgMod)
					gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(Addr + DbgMod->m_ImageBase, false);
			}
		}
		return true;
	}

	bool CCommentList::OnEventGoto(WISP_MSG *pMsg)
	{
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		{
			unsigned long long Addr = GetItemValue(m_pItem, 1);
			CCommentModule *pCommentModule = (CCommentModule*)GetItemValue(GetItemParent(m_pItem), 0);
			CDbgModule *DbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(pCommentModule->m_ModuleName.operator const char*());
			if (!DbgMod)
				DbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(pCommentModule->m_ModuleName.operator const char*());
			if (DbgMod)
				gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(Addr + DbgMod->m_ImageBase, true);
		}
		return true;
	}

	bool CCommentList::OnEventRemove(WISP_MSG *pMsg)
	{
		if (GetItemLevel(m_pItem))
		{
			TAnsiString *pString = (TAnsiString*)GetItemValue(m_pItem, 0);

			//typedef _Tmap_pair<const ULONG_PTR, TAnsiString> PCM;
			//PCM *p = CONTAINING_RECORD_MY1(pString, PCM, second);
			map<ULONG_PTR, TAnsiString>::IT It = pString;//p;//pString-0x18;
			CCommentModule *pCommentModule = (CCommentModule*)GetItemValue(GetItemParent(m_pItem), 0);
			pCommentModule->m_CommentMap.erase(It);
			DeleteItem(m_pItem);
		} else
		{
			CCommentModule *pCommentModule = (CCommentModule *)GetItemValue(m_pItem, 0);
			if (pCommentModule)
			{
				gpSyser->m_SyserUI.RemoveCommentModule(pCommentModule);
				DeleteItem(m_pItem);
			}
		}
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		return true;
	}

	bool CCommentList::Edit(CListStringItem *Item, int nCol, TWideString& String)
	{
		if (nCol != 1) return false;
		if (GetItemLevel(Item) != 1) return false;
		return true;
	}

	bool CCommentList::OnEditing(CListStringItem *Item, int nCol, TWideString& String)
	{
		if (nCol != 1) return false;
		if (GetItemLevel(Item) != 1) return false;
		TAnsiString *pString = (TAnsiString*)GetItemValue(Item, 0);
		char Buffer[256];
		UnicodeToAnsi(String.operator const WCHAR*(), Buffer, lenof(Buffer));
		*pString = Buffer;
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		return true;
	}

	void CCommentList::Create()
	{
		if (IsWindow())
		{
			Destroy();
		} else
		{
			CreateWnd(WSTR("Comment List"), 0,0,400,380, 0, 0xD, 0x300BB, WISP_SH_MINSIZE);
			Center(0);
		}
	}

	void CCommentList::UpdateContext()
	{
		if (IsWindow())
		{
			DeleteAllChildrenItems(0);

			for (map<unsigned long long, CCommentModule>::IT it = gpSyser->m_SyserUI.m_CommentContainer.m_CommentModulesMap.begin();
				it != gpSyser->m_SyserUI.m_CommentContainer.m_CommentModulesMap.end(); ++it)
			{
				CListStringItem *Item = InsertItemStr(TGetFileName(it->second.m_ModuleName.operator const char*()), 0, 0, 0, -1);
				SetItemFormat(Item, 1, it->second.m_TimeStamp, WSTR("%08X"));
				SetItemValue(Item, 0, (unsigned long long)&it->second);
				for (map<ULONG_PTR, TAnsiString>::IT it2 = it->second.m_CommentMap.begin();
					it2 != it->second.m_CommentMap.end(); ++it2)
				{
					CListStringItem *ItemNext = InsertItemValue(it2->first, Item, WSTR("" F0ADDR "X"), 0, nullptr, -1);
					SetItemText(ItemNext, 1, it2->second.operator const char*());
					SetItemValue(ItemNext, 0, (unsigned long long)&it2->second);
					SetItemValue(ItemNext, 1, it2->first);
				}
			}
		}
	}


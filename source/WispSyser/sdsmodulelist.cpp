
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "sdsmodulelist.hpp"
#include "syserconfig.hpp"

WISP_MENU_GROUP_ITEM SDSModuleListPopupMenu[] =
{
	{WSTR("Set As Active Module"), 0x7A, 0x38, 0, 0}, //EVENT_ID_ACTIVE_MODULE
	{WSTR("Unload"), 0xF4, 0x17, 0, 0}, //EVENT_ID_WATCH_DEL ???
	{0},
};

	WISP_MSG_MAP_BEGIN(CSDSModuleList)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)


	WISP_EVENT_MAP_BEGIN(CSDSModuleList)
		WISP_EVENT_MAP(0x7A, OnEventActive)
		WISP_EVENT_MAP(0xF4, OnEventUnload)
	WISP_EVENT_MAP_END

	bool CSDSModuleList::OnCreate(WISP_MSG *pMsg)
	{
		m_ColorFocus = gSyserColors.Color[2];
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		CWispDIBList *pDIBList = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF);
		m_pDibC = pDIBList->GetDIB(0xF8);
		m_pDibCPP = pDIBList->GetDIB(0xF6);
		m_pDibH = pDIBList->GetDIB(0xF7);
		m_pDibMod = pDIBList->GetDIB(0xE3);
		m_PopupMenu.LoadPopupMenuGroup(SDSModuleListPopupMenu, this, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF));
		InsertColumn(WSTR("Module Name"), 150, 0, 0, -1);
		InsertColumn(WSTR("File Count / Size"), 150, 0, 0, -1);
		SetOwner(this);
		m_pItem = 0;
		return true;
	}
	bool CSDSModuleList::OnDestroy(WISP_MSG *pMsg)
	{
		m_PopupMenu.Destroy();
		return true;
	}

	bool CSDSModuleList::OnEventActive(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FindItem(0, 8);
		if (Item && !GetItemLevel(Item))
		{
			CSDSModule *pSDSMod = (CSDSModule *)GetItemValue(Item, 0);
			ExpandSDSModule(pSDSMod);
		}
		return true;
	}


	bool CSDSModuleList::OnEventUnload(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FindItem(0, 8);
		if (Item && !GetItemLevel(Item))
		{
			CSDSModule *pSDSMod = (CSDSModule *)GetItemValue(Item, 0);
			gpSyser->m_SyserUI.UnloadSDSModule(pSDSMod);
		}
		return true;
	}


	void CSDSModuleList::ExpandModule(CListStringItem *Item)
	{
		if (m_pItem)
		{
			SetItemColor(m_pItem, 0, m_ItemColor);
			Collapse(m_pItem);
		}
		m_pItem = Item;
		m_pSDSMod = (CSDSModule *)GetItemValue(Item, 0);
		SetItemColor(m_pItem, 0, gSyserColors.Color[2]);
		Expand(m_pItem);
	}

	void CSDSModuleList::ShowPopupMenu()
	{
		CListStringItem *Item = FindItem(0, 8);
		m_PopupMenu.Enable(0x7A, Item && Item != m_pItem);
		m_PopupMenu.Point(0, true);
	}

	bool CSDSModuleList::OnKeyEvent(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FullFindItem(0, 8);
		if (Item)
		{
			if (pMsg->KeyEvent.KeyType == VK_RETURN &&
				pMsg->KeyEvent.bKeyDown && GetItemLevel(Item) == 1)
				OnMouseDblClick(Item, 1);

			if (pMsg->KeyEvent.KeyType == VK_RBUTTON &&
				!pMsg->KeyEvent.bKeyDown && GetItemLevel(Item) == 0)
				ShowPopupMenu();
		}
		return true;
	}

	void CSDSModuleList::OnMouseDblClick(CListStringItem *Item, int nCol)
	{
		int level = GetItemLevel(Item);
		if (level == 0)
		{
			if (m_pSDSMod == (CSDSModule *)GetItemValue(Item, 0))
				return;
			ExpandModule(Item);
		}

		if (level == 1)
		{
			CSDSModule *pSDSMod = (CSDSModule *)GetItemValue(Item, 0);
			unsigned long FileID = GetItemValue(Item, 1);
			gpSyser->m_MainFrame.m_SourceDebugFrameWnd.OpenSrcView(pSDSMod, FileID, 1);
			Focus();
		}
	}

	void CSDSModuleList::AppendSrcFiles(CListStringItem *ParentItem, CSDSModule *pSDSMod)
	{
		unsigned long Len = 0;
		SrcFileNameID *SrcFileName = pSDSMod->GetSrcFiles(&Len);
		if (!SrcFileName) return;

		for (unsigned long n = 0; n < Len; ++n)
		{
			if (SrcFileName[n].FileSize)
			{
				CWispDIB *pDib = 0;
				const char *pFileName = TStrRChr(SrcFileName[n].FileName, '\\');
				const char *pFileExt = TGetFileExt(pFileName + 1);
				if (pFileExt && !TStrICmp(pFileExt, "c"))
				{
					pDib = m_pDibC;
				} else
				if (pFileExt && !TStrICmp(pFileExt, "cpp"))
				{
					pDib = m_pDibCPP;
				} else
				if (pFileExt && (TStrICmp(pFileExt, "h") && TStrICmp(pFileExt, "hpp")))
				{
					pDib = 0;
				} else
				{
					pDib = m_pDibH;
				}
				CListStringItem *Item = 0;
				if (pFileName)
					Item = InsertItemStr(pFileName+1, ParentItem, 0, pDib, -1);
				else	Item = InsertItemStr(SrcFileName[n].FileName, ParentItem, 0, pDib, -1);
				SetItemFormat(Item, 1, SrcFileName[n].FileSize, WSTR("%d Byte(s)"));
				SetItemValue(Item, 0, (unsigned long long)pSDSMod);
				SetItemValue(Item, 1, (unsigned long long)SrcFileName[n].FileID);
			}
		}
	}

	void CSDSModuleList::AppendSDSModule(CSDSModule *pSDSMod)
	{
		CListStringItem *Item = InsertItemStr(TGetFileName(pSDSMod->m_ExeFileName.operator const char*()), 0, 0, m_pDibMod, -1);
		SetItemFormat(Item, 1, pSDSMod->GetSrcFilesCount(), WSTR("%d Source Code File(s)"));
		SetItemValue(Item, 0, (unsigned long long)pSDSMod);
		if (!m_pSDSMod)
			ExpandModule(Item);
		AppendSrcFiles(Item, pSDSMod);
	}

	bool CSDSModuleList::RemoveSDSModule(CSDSModule *pSDSMod)
	{
		CListStringItem *Item = GetNext(0, 0);
		if (!Item)
			return false;
		while (pSDSMod != (CSDSModule *)GetItemValue(Item, 0))
		{
			Item = GetNext(Item, 0);
			if (!Item)
				return false;
		}

		if (m_pSDSMod == pSDSMod)
		{
			m_pSDSMod = 0;
			m_pItem = 0;
		}

		map<SCWKEY, CSourceCodeWnd*>::IT it = gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_MultiSourceCodeView.m_SourceCodeWndMap.begin();
		while (it)
		{
			if (it->first.pSDSMod == pSDSMod)
			{
				gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_MultiSourceCodeView.RemoveWnd(it++->second, true);//->pSrcWnd);
				gpSyser->m_MainFrame.m_SourceDebugFrameWnd.RemoveSDSModule(pSDSMod);
			} else
			{
				++it;
			}
		}
		DeleteItem(Item);
		if (gpSyser->m_bActive)
			Update((WISP_RECT*)0);
		return true;
	}

	void CSDSModuleList::ExpandSDSModule(CSDSModule *pSDSMod)
	{
		if (m_pSDSMod != pSDSMod)
		{
			CListStringItem *Item = GetNext(0, 0);
			while (Item)
			{
				if (pSDSMod == (CSDSModule *)GetItemValue(Item, 0))
				{
					ExpandModule(Item);
					return;
				}
				Item = GetNext(Item, 0);
			}
		}
	}

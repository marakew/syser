
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "modulelist.hpp"
#include "syserconfig.hpp"

enum
{
  ML_NAME = 0,
  ML_BASE = 1,
  ML_SIZE = 2,
  ML_SYM = 3,
  ML_PATH = 4,
};

	WISP_MSG_MAP_BEGIN(CModuleListWnd)
	//	WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_CLOSE, OnClose)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

	WISP_EVENT_MAP_BEGIN(CModuleListWnd)
		WISP_EVENT_MAP(0x56, OnEventOpenPEExplorer)
		WISP_EVENT_MAP(0xF2, OnEventGotoBase) //EVENT_ID_GOTO_MODULE_BASE
		WISP_EVENT_MAP(0xF3, OnEventAnalyze) //EVENT_ID_ANALYZE_FUNCTION
	WISP_EVENT_MAP_END

WISP_MENU_GROUP_ITEM ModuleListPopupMenu[] =
{
	{WSTR("Goto Module Base"), 0xF2, 8, 0, 0}, //
	{WSTR("Analyze Module"), 0xF3, 8, 0, 0}, //
	{WSTR("PE Explorer "), 0x56, 0xE, 0, 0},
	{0},
};

	CModuleListWnd::CModuleListWnd()
	{
		m_pDbgMod = 0;
		m_bClose = false;
	}
	CModuleListWnd::~CModuleListWnd()
	{
	}

	bool CModuleListWnd::OnUpdate(WISP_MSG *pMsg)
	{
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		UpdateItemColor(0);
		return true;
	}

	bool CModuleListWnd::OnCreate(WISP_MSG *pMsg)
	{
		AttachTitleDIB(gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0xDC));
		m_ItemColor = gSyserColors.Color[1];
		m_Style |= 0x1000000;
		SetBGColor(gSyserColors.Color[0]);
		m_PopupMenu.LoadPopupMenuGroup(ModuleListPopupMenu, this, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF));

		int Ext = m_ClientDC.GetTextExtent(WSTR("X"), -1, 0);

		InsertColumn(WSTR("Name"), 100, 0, 0, -1);
		//76 +16
		InsertColumn(WSTR("Base"), (3+ADDR_SIZE)*Ext+3, 0, 0, -1); //60
		InsertColumn(WSTR("Size"), 68, 0, 0, -1);
		InsertColumn(WSTR("Symbols"), 50, 0, 0, -1);
		InsertColumn(WSTR("Path"), 150, 0, 0, -1);
		
		UpdateContext();
		return true;
	}
	bool CModuleListWnd::OnDestroy(WISP_MSG *pMsg)
	{
		m_PopupMenu.Destroy();
		return true;
	}
	bool CModuleListWnd::OnClose(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.FocusActive();
		return true;
	}
	bool CModuleListWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.bKeyDown && pMsg->KeyEvent.KeyType == VK_RBUTTON)
		{
			if (FindItem(0, 8))
				m_PopupMenu.Point(0, 1);
		}
		return true;
	}

	bool CModuleListWnd::OnEventOpenPEExplorer(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FindItem(0, 8);
		if (Item)
		{
			ULONG_PTR Addr = GetItemValue(Item, ML_BASE);
			gpSyser->m_MainFrame.m_PEExplorerForm.Create();
			gpSyser->m_MainFrame.m_PEExplorerForm.Open(Addr);
		}
		return true;
	}
	bool CModuleListWnd::OnEventGotoBase(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FindItem(0, 8);
		if (Item)
		{
			ULONG_PTR Addr = GetItemValue(Item, ML_BASE);
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.SetActiveAddress(Addr, 0);
		}
		return true;
	}
	bool CModuleListWnd::OnEventAnalyze(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FindItem(0, 8);
		if (Item)
		{
			ULONG_PTR Addr = GetItemValue(Item, ML_BASE);
			CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(Addr);
			if (pDbgMod)
			{
				//TODO gpSyser->m_SyserUI.m_SymAnalyzer.???(pDbgMod, pDbgMod->+30); //pDbgMod->m_ImageBase
				//TODO gpSyser->m_MainFrame.m_SystemExplorer.m_SymAnalyzer.???(pDbgMod, pDbgMod->+30);
				gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
			}
		}
		return true;
	}

	void CModuleListWnd::OnLButton(CListStringItem *Item, int nCol)
	{
		if (m_bClose)
		{
			m_pDbgMod = (CDbgModule*)GetItemValue(Item, ML_NAME);
			Destroy();
		}
	}

	void CModuleListWnd::Create(bool bClose)
	{
		if (bClose)
		{
			m_bClose = true;
			CreateWnd(WSTR("Modules"), 0,0,500,350, 0, 0xE, 0x400100BB, WISP_SH_MINSIZE);
			SetOwner(&gpSyser->m_MainFrame.m_SystemExplorer);
			Center(0);
			Show(WISP_SH_MODAL_BLOCK);
		} else
		if (IsWindow())
		{
			Destroy();
		} else
		{
			CreateWnd(WSTR("Modules"), 0,0,500,350, 0, 0xE, 0x400100BB, WISP_SH_MINSIZE);
			SetOwner(&gpSyser->m_MainFrame.m_SystemExplorer);
			Center(0);
		}
	}
	void CModuleListWnd::UpdateContext()
	{
		if (gpSyser->m_pDebugger)
		{
			DeleteAllChildrenItems(0);

			map<CMemoryImage, CDbgModule>::IT it = gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
			//while (it != gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.end())
			while (it)
			{
				CListStringItem *Item = InsertItemStr(it->second.m_FileTitle.operator const char*(), 0, 0, 0, -1);
				WCHAR Buffer[MAX_FN_LEN];
				uNumToStr(it->second.m_ImageBase, Buffer, 16);
				SetItemText(Item, ML_BASE, Buffer);

				SetItemFormat(Item, ML_SIZE, it->second.m_ImageSize, WSTR("%08X"));

				SetItemFormat(Item, ML_SYM, (it->second.m_pSymbolModule?it->second.m_pSymbolModule->m_SymbolMap.size():0), WSTR("%d"));
				SetItemText(Item, ML_PATH, it->second.m_FullFileName.operator const char*());
				SetItemValue(Item, ML_NAME, (unsigned long long)&it->second);

				SetItemValue(Item, ML_BASE, it->second.m_ImageBase);
				++it;
			}

			if (gpSyser->m_pDebugger != gpSyser->m_pSysDebugger)
			{
				map<CMemoryImage, CDbgModule>::IT it = gpSyser->m_pSysDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
				//while (it != gpSyser->m_pSysDebugger->m_DbgModuleList.m_DbgModuleMap.end())
				while (it)
				{
					CListStringItem *Item = InsertItemStr(it->second.m_FileTitle.operator const char*(), 0, 0, 0, -1);
					WCHAR Buffer[MAX_FN_LEN];
					uNumToStr(it->second.m_ImageBase, Buffer, 16);
					SetItemText(Item, ML_BASE, Buffer);

					SetItemFormat(Item, ML_SIZE, it->second.m_ImageSize, WSTR("%08X"));

					SetItemFormat(Item, ML_SYM, (it->second.m_pSymbolModule?it->second.m_pSymbolModule->m_SymbolMap.size():0), WSTR("%d"));
					SetItemText(Item, ML_PATH, it->second.m_FullFileName.operator const char*());
					SetItemValue(Item, ML_NAME, (unsigned long long)&it->second);

					SetItemValue(Item, ML_BASE, it->second.m_ImageBase);
					++it;
				}
			}
		}
	}


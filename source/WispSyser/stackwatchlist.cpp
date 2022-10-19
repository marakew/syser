
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "stackwatchlist.hpp"
#include "syserconfig.hpp"
#include "resource.hpp"

	WISP_MSG_MAP_BEGIN(CStackWatchList)
	//	WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP(WISP_WM_SCROLL_EVENT, OnScrollEvent)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

	WISP_EVENT_MAP_BEGIN(CStackWatchList)
		WISP_EVENT_MAP(EVENT_ID_SHOW_CODE, OnEventShowCode)
		WISP_EVENT_MAP(EVENT_ID_SHOW_CODE_AT, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_SHOW_CSP_ADDR, OnEventShowCSP)
		WISP_EVENT_MAP(EVENT_ID_SHOW_CBP_ADDR, OnEventShowCBP)
		WISP_EVENT_MAP(EVENT_ID_SHOW_DATA, OnEventShowData)
		WISP_EVENT_MAP(EVENT_ID_SHOW_DATA_AT, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_CV_ADDR_MODE, OnEventAddressMode)
		WISP_EVENT_MAP(EVENT_ID_CV_OFFSET_MODE, OnEventOffsetMode)
		WISP_EVENT_MAP(EVENT_ID_REL_CSP, OnEventRelCSP)
		WISP_EVENT_MAP(EVENT_ID_REL_CBP, OnEventRelCBP)
		WISP_EVENT_MAP(EVENT_ID_LOW_HIGH_ADDR, OnEventToggleOrder)
	WISP_EVENT_MAP_END

	WISP_CMD_MAP_BEGIN(CStackWatchList)
		WISP_CMD_MAP(WISP_ID_VSPIN_SCROLL, OnCmdVSpinScroll)
	WISP_CMD_MAP_END

WISP_MENU_GROUP_ITEM StackWatchListPopupMenu[] =
{
	{WSTR("Show Data "), EVENT_ID_SHOW_DATA, 0x7F, 0, 0},
	{WSTR("Show Code "), EVENT_ID_SHOW_CODE, 0x7E, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Show " CSPSTR " Address"), EVENT_ID_SHOW_CSP_ADDR, 0x77, 0, 0},
	{WSTR("Show " CBPSTR " Address"), EVENT_ID_SHOW_CBP_ADDR, 0x77, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Address Mode"), EVENT_ID_CV_ADDR_MODE, 0, 2, 0},
	{WSTR("Offset  Mode"), EVENT_ID_CV_OFFSET_MODE, 0, 2, 0},
	{WSTR("Relative to " CSPSTR ""), EVENT_ID_REL_CSP, 0, 2, 0},
	{WSTR("Relative to " CBPSTR ""), EVENT_ID_REL_CBP, 0, 2, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Low->High Address"), EVENT_ID_LOW_HIGH_ADDR, 0, 0, 0},
	{0},
};

enum
{
  SWL_ADDR = 0,
  SWL_VALUE = 1,
  SWL_CONTEXT = 2,
};


	CStackWatchList::CStackWatchList()
	{
		m_nScrollLine = 2;
		m_bOrder = true;
	}
	CStackWatchList::~CStackWatchList()
	{
	}

	bool CStackWatchList::OnUpdate(WISP_MSG *pMsg)
	{
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		UpdateItemColor(0);
		//TODO
		return true;
	}

	bool CStackWatchList::OnCreate(WISP_MSG *pMsg)
	{
		m_Style |= 0x2000000;
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		int Ext = m_ClientDC.GetTextExtent(WSTR("X"), -1, 0);
		InsertColumn(WSTR("Address"), (1+ADDR_SIZE) * Ext, 0, 0, -1);
		InsertColumn(WSTR("Value[F2]"), (2+ADDR_SIZE) * Ext, 1, 0, -1);
		InsertColumn(WSTR("Context"), 100, 0, 0, -1);
                m_CurAddr = 0;
		for (int i = 0; i < m_nVSBPage; ++i)
		{
			CListStringItem *Item = InsertItem(WSTR("" UNREADSTR ""), 0, 0, 0, -1);
			SetItemText(Item, SWL_VALUE, WSTR("" UNREADSTR ""));
		}
		
		m_pScrollBar[0]->Style = 0;
		m_pScrollBar[0]->bHide = 0;
		m_PopupMenu.LoadPopupMenuGroup(StackWatchListPopupMenu, this, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF));
		m_pMenuShowCode = m_PopupMenu.GetItem(EVENT_ID_SHOW_CODE);
		m_pMenuShowData = m_PopupMenu.GetItem(EVENT_ID_SHOW_DATA);
		m_pMenuOrder = m_PopupMenu.GetItem(EVENT_ID_LOW_HIGH_ADDR);
		LoadKeyMap();
		return true;
	}
	bool CStackWatchList::OnDestroy(WISP_MSG *pMsg)
	{
		m_PopupMenu.Destroy();
		return true;
	}
	bool CStackWatchList::OnSize(WISP_MSG *pMsg)
	{
		if (pMsg->Sizing.ScrSize.cy)
		{
			ResetContext();
			if (gpSyser->m_pDebugger && gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
				UpdateContext(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP, false);
		}
		return true;
	}
	bool CStackWatchList::OnScrollEvent(WISP_MSG *pMsg)
	{
		if (!gpSyser->m_pDebugger || !gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
			return false;

		if (pMsg->ScrollEvent.Type == WISP_SCROLL_BAR_VERT)
		{
			ULONG_PTR Addr;
			switch (pMsg->ScrollEvent.Event)
			{
			case WISP_SB_INC:
				Addr = m_bOrder?(m_CurAddr+sizeof(Addr)):(m_CurAddr-sizeof(Addr));
				UpdateContext(Addr, true);
				SetScrollBarCurPos(0, m_nVSBPage * m_nScrollLine, 1);
				break;
			case WISP_SB_DEC:
				Addr = m_bOrder?(m_CurAddr-sizeof(Addr)):(m_CurAddr+sizeof(Addr));
				UpdateContext(Addr, true);
				SetScrollBarCurPos(0, m_nVSBPage * m_nScrollLine, 1);
				break;
			case WISP_SB_PAGE_UP:
				Addr = m_bOrder?(m_CurAddr-m_pScrollBar[0]->Page*sizeof(Addr)):(m_CurAddr+m_pScrollBar[0]->Page*sizeof(Addr));
				UpdateContext(Addr, true);
				SetScrollBarCurPos(0, m_nVSBPage * m_nScrollLine, 1);
				break;
			case WISP_SB_PAGE_DOWN:
				Addr = m_bOrder?(m_CurAddr+m_pScrollBar[0]->Page*sizeof(Addr)):(m_CurAddr-m_pScrollBar[0]->Page*sizeof(Addr));
				UpdateContext(Addr, true);
				SetScrollBarCurPos(0, m_nVSBPage * m_nScrollLine, 1);
				break;
			case WISP_SB_POS_CHANGE:
				Addr = m_bOrder?(m_CurAddr+pMsg->ScrollEvent.Delta*sizeof(Addr)):(m_CurAddr-pMsg->ScrollEvent.Delta*sizeof(Addr));
				UpdateContext(Addr, true);
				break;
			default:
				SetScrollBarCurPos(0, m_nVSBPage * m_nScrollLine, 1);
				break;
			}
			return false;
		}
		return true;
	}
	void CStackWatchList::OnKeyRButton(WISP_MSG *pMsg)
	{
		WCHAR szMenu[128];
		WCHAR szHotKey[50];

		bool bReadOK = false;
	#if 0 //+++ UNUSE
		if (pMsg)
		{
			WISP_RECT Rect;
			CListStringItem *Item = GetItem(0, 0);
			GetItemRect(Item, SWL_ADDR, &Rect);
		}
	#endif
		CListStringItem *Item = GetNext(0, 8);
		if (Item)
		{
			ULONG_PTR Addr = GetItemValue(Item, SWL_VALUE);
			unsigned char Mem;
			if (gpSyser->m_pDebugger->ReadMemory(Addr, &Mem, 1) == 1)
				bReadOK = true;
			CListStringItem::CONTENT *Content = GetItemContent(Item, SWL_VALUE);

			gpSyser->m_MainFrame.m_KeyMappingPage.GetHotKey(GetKeyMap(EVENT_ID_SHOW_CODE_AT), szHotKey);
			TSPrintf(szMenu, WSTR("Show code at:%s  %s"), Content->String.operator const WCHAR*(), szHotKey);
			m_PopupMenu.SetItemText(m_pMenuShowCode, szMenu);

			gpSyser->m_MainFrame.m_KeyMappingPage.GetHotKey(GetKeyMap(EVENT_ID_SHOW_DATA_AT), szHotKey);
			TSPrintf(szMenu, WSTR("Show data at:%s  %s"), Content->String.operator const WCHAR*(), szHotKey);
			m_PopupMenu.SetItemText(m_pMenuShowData, szMenu);
		}
		m_PopupMenu.Enable(m_pMenuShowCode, bReadOK);
		m_PopupMenu.Enable(m_pMenuShowData, Item && bReadOK);
	
		m_PopupMenu.Checked(EVENT_ID_CV_ADDR_MODE, m_Mode == SWL_MODE_ADDR);
		m_PopupMenu.Checked(EVENT_ID_CV_OFFSET_MODE, m_Mode == SWL_MODE_REL);
		m_PopupMenu.Checked(EVENT_ID_REL_CSP, m_Mode == SWL_MODE_CSP);
		m_PopupMenu.Checked(EVENT_ID_REL_CBP, m_Mode == SWL_MODE_CBP);

		CWispDIB *pDIB;
		if (m_bOrder)
		{
			pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0xAF);
			TStrCpy(szMenu, WSTR("High-->Low Address"));
		} else
		{
			pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0xAE);
			TStrCpy(szMenu, WSTR("Low-->High Address"));
		}
		m_PopupMenu.SetItemText(m_pMenuOrder, szMenu);
		m_PopupMenu.SetDIB(m_pMenuOrder, pDIB);
		m_PopupMenu.Point(0, 1);
	}
	unsigned int CStackWatchList::GetKeyMap(unsigned int CmdID)
	{
		for (map<unsigned int, unsigned int>::IT it = m_KeyMap.begin();
			it != m_KeyMap.end(); ++it)
		{
			if (it->second == CmdID)
				return it->first;
		}
		return 0;
	}
	bool CStackWatchList::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen) return true;

		if (!pMsg->KeyEvent.bKeyDown) return true;

		map<unsigned int, unsigned int>::IT it = m_KeyMap.find(pMsg->KeyEvent.KeyType);
		if (it != m_KeyMap.end())
		{
			WISP_MSG Msg;
			Msg.Command.CmdID = it->second;
			return OnEvent(&Msg);
		}

		switch (pMsg->KeyEvent.KeyType)
		{
		case VK_RBUTTON:
			OnKeyRButton(pMsg); //Menu
			break;
		case 0x53: //S
		case 0x57: //W
			OnEventToggleOrder(0);
			break;
		case VK_F2:
			CListStringItem *Item = FullFindItem(0, 8);
			if (Item) EditItem(Item, SWL_VALUE);
			break;
		}
		return true;
	}

	bool CStackWatchList::OnEventShowCode(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetNext(0, 8);
		if (Item)
		{
			ULONG_PTR Addr = GetItemValue(Item, SWL_VALUE);
			ULONG_PTR Mem;
			if (gpSyser->m_pDebugger->ReadMemory(Addr, &Mem, sizeof(Mem)) == sizeof(Mem))
			{
				Addr = GetItemValue(Item, SWL_VALUE);
				gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(Addr, true);
			}
		}
		return true;
	}
	bool CStackWatchList::OnEventGotoCode(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetNext(0, 8);
		if (Item)
		{
			ULONG_PTR Addr = GetItemValue(Item, SWL_ADDR);
			ULONG_PTR Mem;
			if (gpSyser->m_pDebugger->ReadMemory(Addr, &Mem, sizeof(Mem)) == sizeof(Mem))
			{
				Addr = GetItemValue(Item, SWL_ADDR);
				gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(Addr, true);
			}
		}
		return true;
	}

	bool CStackWatchList::OnEventShowData(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetNext(0, 8);
		if (Item)
		{
			ULONG_PTR Addr = GetItemValue(Item, SWL_VALUE);
			ULONG_PTR Mem;
			if (gpSyser->m_pDebugger->ReadMemory(Addr, &Mem, sizeof(Mem)) == sizeof(Mem))
			{
				Addr = GetItemValue(Item, SWL_VALUE);
				gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(Addr, 0);
			}
		}
		return true;
	}
	bool CStackWatchList::OnEventGotoData(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetNext(0, 8);
		if (Item)
		{
			ULONG_PTR Addr = GetItemValue(Item, SWL_ADDR);
			ULONG_PTR Mem;
			if (gpSyser->m_pDebugger->ReadMemory(Addr, &Mem, sizeof(Mem)) == sizeof(Mem))
			{
				Addr = GetItemValue(Item, SWL_ADDR);
				gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(Addr, 0);
			}
		}
		return true;
	}

	bool CStackWatchList::OnEventAddressMode(WISP_MSG *pMsg)
	{
		m_Mode = SWL_MODE_ADDR;
		UpdateContext(m_CurAddr, true);
		return true;
	}
	bool CStackWatchList::OnEventOffsetMode(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FindItem(0, 8);
		if (Item)
		{
			m_RelCIP = GetItemValue(Item, SWL_ADDR);
			m_Mode = SWL_MODE_REL;
			UpdateContext(m_CurAddr, true);
		}
		return true;
	}
	bool CStackWatchList::OnEventRelCSP(WISP_MSG *pMsg)
	{
		m_Mode = SWL_MODE_CSP;
		UpdateContext(m_CurAddr, true);
		return true;
	}
	bool CStackWatchList::OnEventRelCBP(WISP_MSG *pMsg)
	{
		m_Mode = SWL_MODE_CBP;
		UpdateContext(m_CurAddr, true);
		return true;
	}
	bool CStackWatchList::OnEventToggleOrder(WISP_MSG *pMsg)
	{
		ULONG_PTR Addr = m_bOrder ? (m_CurAddr + sizeof(Addr) * m_nVSBPage) : (m_CurAddr - sizeof(Addr) * m_nVSBPage);
		m_bOrder = !m_bOrder;
		UpdateContext(Addr, true);
		return true;
	}
	bool CStackWatchList::OnEventShowCBP(WISP_MSG *pMsg)
	{
		UpdateContext(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBP, 0);
		return true;
	}

	bool CStackWatchList::OnEventShowCSP(WISP_MSG *pMsg)
	{
		UpdateContext(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP, 0);
		return true;
	}

	bool CStackWatchList::OnCmdVSpinScroll(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000002)
			SetScrollBarCurPos(0, m_nVSBPage * m_nScrollLine, 0); //m_nItemCount
		return true;
	}

	void CStackWatchList::OnLButton(CListStringItem *Item, int nCol)
	{
		if (nCol == SWL_ADDR)
		{
			switch (m_Mode)
			{
			case SWL_MODE_ADDR: OnEventOffsetMode(0); break;
			case SWL_MODE_REL: OnEventRelCSP(0); break;
			case SWL_MODE_CSP: OnEventRelCBP(0); break;
			case SWL_MODE_CBP: OnEventAddressMode(0); break;
			}
		}
	}

	bool CStackWatchList::Edit(CListStringItem *Item, int nCol, TWideString& String)
	{
		if (gpSyser->m_pDebugger && gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		{
			if (nCol == SWL_VALUE)
			{
				CListStringItem::CONTENT *Content = GetItemContent(Item, SWL_VALUE);
				if (!TStrCmp(Content->String.operator const WCHAR*(), WSTR("" UNREADSTR "")))
					return false;
			}
			return true;
		}
		return false;
	}
	bool CStackWatchList::OnEditing(CListStringItem *Item, int nCol, TWideString& String)
	{
		ULONG_PTR Result;
		if (!gpSyser->m_SyserUI.CalcExp(String.operator const WCHAR*(), &Result))
			return false;
		if (nCol == SWL_ADDR)
			return false;
		if (nCol != SWL_VALUE)
			return false;

		CListStringItem::CONTENT *Content = GetItemContent(Item, SWL_VALUE);
		if (Content->String == String)
			return false;

		ULONG_PTR Addr = GetItemValue(Item, SWL_ADDR);
		ULONG_PTR Mem;
		if (gpSyser->m_pDebugger->WriteMemory(Addr, &Mem, sizeof(Mem)) != sizeof(Mem))
			return false;
		m_EditWnd.m_WndText.Format(WSTR("" F0ADDR "X"), Mem);
		
		unsigned long ColorString[256];
		unsigned char Type;
		DrawContext(Addr, Mem, ColorString, 256, &Type);
		SetItemTextColor(Item, SWL_CONTEXT, ColorString);
		return true;
	}

	void CStackWatchList::DrawContextSEH(ULONG_PTR Address, ULONG_PTR Value, unsigned long *pColorString, int MaxSize)
	{
#ifndef _X64_
		map<unsigned long, unsigned long>::IT it = m_SEHMap_1.find(Address);
		if (it == m_SEHMap_1.end())
		{
			if (m_SEHMap_2.find(Value) != m_SEHMap_2.end())
			{
				Address -= 4;
				if (m_SEHMap_1.find(Address) != m_SEHMap_1.end())
				{
					ColorStrCat(pColorString, "SEH Handler", 2, 0);
				}
			}
		} else
		if (it == m_SEHMap_1.rbegin()) //???
		{
			ColorStrCat(pColorString, "End of SEH chain", 2, 0);
		} else
		{
			ColorStrCat(pColorString, "Pointer to next SEH record", 2, 0);
		}
#endif
	}

	bool CStackWatchList::DrawContext(ULONG_PTR Address, ULONG_PTR Value, unsigned long *pColorString, int MaxSize, unsigned char *pType)
	{
		*pColorString = 0;
		WCHAR szString[256];
		szString[0] = 0;
		int Len = 0;
		if (Address == *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP)
			Len += ColorStrCpy(pColorString, "<-" CSPSTR " ", 1, 3); //??? Cat
		if (Address == *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBP)
			Len += ColorStrCat(pColorString, "<-" CBPSTR " ", 1, 5);

		DrawContextSEH(Address, Value, pColorString, MaxSize);

		char Encode;
		if (gpSyser->m_SyserUI.ReadString(Value, szString, MaxSize - Len - 1, &Encode))
		{
			if (Encode == 0)
				ColorStrCat(pColorString, szString, 6, 0);
			else
			if (Encode == 1)
				ColorStrCat(pColorString, szString, 9, 0);
			if (pType) *pType = Encode;
			return true;
		}

		char Buffer[256];
		Buffer[0] = 0;

		char szSym[256];
		if (gpSyser->m_SyserUI.GetSymbolOffset(Value, szSym))
		{
			if (pType) *pType = 2;
			if (TStrChr(szSym, '+'))
			{
				unsigned long CodeLen = gpSyser->m_SyserUI.m_SyserCodeDoc.GetCall(Value, Buffer, 0);
				if (CodeLen)
				{
					ULONG_PTR FromAddr;
					if (gpSyser->m_SyserUI.m_SyserCodeDoc.GetReturn(Value - CodeLen, &FromAddr))
						TSPrintf(Buffer, "Return to " F0ADDR "X from " F0ADDR "X ", Value, FromAddr);
					else	TSPrintf(Buffer, "Return to " F0ADDR "X ", Value);
				} else
				{
					Buffer[0] = 0;
				}
			}
			ColorStrCat(pColorString, Buffer, 2, 0);
			ColorStrCat(pColorString, szSym, 1, 0);
		} else
		{
			szSym[0] = 0;
			unsigned long CodeLen = gpSyser->m_SyserUI.m_SyserCodeDoc.GetCall(Value, Buffer, 0);
			if (CodeLen == 0) return false;

			ULONG_PTR FromAddr;
			if (gpSyser->m_SyserUI.m_SyserCodeDoc.GetReturn(Value - CodeLen, &FromAddr))
				TSPrintf(Buffer, "Return to " F0ADDR "X from " F0ADDR "X ", Value, FromAddr);
			else	TSPrintf(Buffer, "(return to " F0ADDR "X)", Value);
			ColorStrCat(pColorString, Buffer, 2, 0);
			if (pType) *pType = 3;
		}
		return true;
	}
	void CStackWatchList::GetAddress(ULONG_PTR Address, WCHAR *pString)
	{
		WCHAR Buffer[32];
		LONG_PTR Offset = 0;
		if (m_Mode == SWL_MODE_ADDR)
		{
			TSPrintf(pString, WSTR("" F0ADDR "X"), Address);
			return;
		} else
		if (m_Mode == SWL_MODE_REL)
		{
			Offset = Address - m_RelCIP;
			TStrCpy(Buffer, "$");
		} else
		if (m_Mode == SWL_MODE_CSP)
		{
			Offset = Address - *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP;
			TStrCpy(Buffer, CSPSTR);
		} else
		if (m_Mode == SWL_MODE_CBP)
		{
			Offset = Address - *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBP;
			TStrCpy(Buffer, CBPSTR);
		}
		if (Offset>0) TSPrintf(pString, WSTR("%s+%X"), Buffer, Offset);
		else
		if (Offset<0) TSPrintf(pString, WSTR("%s-%X"), Buffer, -Offset);
		else
		{
			TStrCpy(pString, Buffer);
			TStrCat(pString, "==>");
		}
	}
	void CStackWatchList::LoadKeyMap()
	{
		if (!gpSyser->GetHotKey(EVENT_ID_SHOW_CODE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0xD,       EVENT_ID_SHOW_CODE)); //VK_RETURN
		if (!gpSyser->GetHotKey(EVENT_ID_SHOW_DATA_AT, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x4000000D,EVENT_ID_SHOW_DATA_AT)); //WISP_SK_CTRL VK_RETURN
		if (!gpSyser->GetHotKey(EVENT_ID_SHOW_DATA, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x1000000D,EVENT_ID_SHOW_DATA)); //WISP_SK_SHIFT VK_RETURN
		if (!gpSyser->GetHotKey(EVENT_ID_SHOW_CODE_AT, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x2000000D,EVENT_ID_SHOW_CODE_AT)); //WISP_SK_ALT VK_RETURN
	}
	void CStackWatchList::ResetContext()
	{
		Adjust(0, m_nVSBPage);
		ClearItem(0);
		CListStringItem *Item = GetItem(0, 0);
		if (Item) Select(Item);
		SetScrollBarInfo(0, m_nVSBPage * m_nScrollLine, m_nVSBPage * (2 * m_nScrollLine + 1), m_nVSBPage, 0, 1);
#ifndef _X64_
		m_SEHMap_1.clear();
		m_SEHMap_2.clear();
#endif
	}
	void CStackWatchList::UpdateContextSEH()
	{
#ifndef _X64_
		m_SEHMap_1.clear();
		m_SEHMap_2.clear();

		ULONG_PTR AddressTIB;
		unsigned long Limit;
		if (gpSyser->m_pDebugger->GetSegRegBase(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pFS, &AddressTIB, &Limit))
		{
			ULONG_PTR ExceptionList;
			if (gpSyser->m_SyserUI.m_SyserCodeDoc.ReadMemory(AddressTIB, &ExceptionList, 4) == 4)
			{
				for (int records = 0; records < 100; ++records)
				{
					unsigned long Mem[2];
					if (gpSyser->m_SyserUI.m_SyserCodeDoc.ReadMemory(ExceptionList, Mem, 8) != 8) break;

					m_SEHMap_1.insert(_Tmap_pair<unsigned long,unsigned long>(ExceptionList, Mem[1]));
					m_SEHMap_2.insert(_Tmap_pair<unsigned long,unsigned long>(Mem[1], ExceptionList));
					ExceptionList = Mem[0];
					if (ExceptionList == -1) break;
				}
			}
		}
#endif
	}
	void CStackWatchList::UpdateContext(ULONG_PTR Addr, bool bCurAddr)
	{
		UpdateContextSEH();
		if (bCurAddr)
		{
			m_CurAddr = Addr;
		} else
		{
			if (m_bOrder)
			{
				if (Addr < m_CurAddr + (m_nVSBPage*sizeof(Addr))/sizeof(Addr) ||
				    Addr >= m_CurAddr + (m_nVSBPage*3*sizeof(Addr))/sizeof(Addr))
					m_CurAddr = Addr - sizeof(Addr) * ((int)m_nVSBPage / 2);
			} else
			{
				if (Addr > m_CurAddr - (m_nVSBPage*sizeof(Addr))/sizeof(Addr) ||
				    Addr <= m_CurAddr - (m_nVSBPage*3*sizeof(Addr))/sizeof(Addr))
					m_CurAddr = Addr + sizeof(Addr) * ((int)m_nVSBPage / 2);
			}
			Addr = m_CurAddr;
		}

		unsigned long ColorString[256];
		WCHAR Buffer[256];

		CListStringItem *Item = GetNext(0, 0);
		while (Item)
		{
			GetAddress(Addr, Buffer);
			if (Addr == *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP)
			{
				ColorStrCpy(ColorString, Buffer, 1, 3);
				SetItemTextColor(Item, SWL_ADDR, ColorString);
			} else
			if (Addr == *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBP)
			{
				ColorStrCpy(ColorString, Buffer, 1, 5);
				SetItemTextColor(Item, SWL_ADDR, ColorString);
			} else
			{
				SetItemText(Item, SWL_ADDR, Buffer);
			}

			ULONG_PTR Value;
			if (gpSyser->m_pDebugger->ReadMemory(Addr, &Value, sizeof(Value)) == sizeof(Value))
				TSPrintf(Buffer, WSTR("" F0ADDR "X"), Value);
			else	TStrCpy(Buffer, WSTR("" UNREADSTR ""));

			SetItemValue(Item, SWL_ADDR, Addr);
			SetItemText(Item, SWL_VALUE, Buffer);
			SetItemValue(Item, SWL_VALUE, Value);
			unsigned char Type;
			DrawContext(Addr, Value, ColorString, 256, &Type);
			SetItemTextColor(Item, SWL_CONTEXT, ColorString);
			if (m_bOrder)
				Addr += sizeof(Addr);
			else	Addr -= sizeof(Addr);
			Item = GetNext(Item, 0);
		}
		Update(&m_ClientRect);
	}

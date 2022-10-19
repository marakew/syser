
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "dataview.hpp"
#include "resource.hpp"

	WISP_MSG_MAP_BEGIN(CDataView)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispHexWnd)


	WISP_EVENT_MAP_BEGIN(CDataView)
		WISP_EVENT_MAP(EVENT_ID_SHOW_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_NEW_DATA, OnEventNewData)
		WISP_EVENT_MAP(EVENT_ID_DISASM, OnEventGotoDasm)
		WISP_EVENT_MAP(EVENT_ID_NEW_CODE, OnEventNewDasm)
	#ifdef _RING0_
		WISP_EVENT_MAP(EVENT_ID_DASM_PHYS_ADDR, OnEventShowPhysAddr)
		WISP_EVENT_MAP(EVENT_ID_SHOW_PHYS_ADDR, OnEventDasmPhysAddr)
	#endif
		WISP_EVENT_MAP(EVENT_ID_SHOW_OP, OnEventToggleToolbar)

		WISP_EVENT_MAP(EVENT_ID_ATTACH_TYPE, OnEventAttachType)
		WISP_EVENT_MAP(EVENT_ID_FIND_FORM, OnEventFindForm)

		WISP_EVENT_MAP(EVENT_ID_SHOW_OFFSET, OnEventToggleOffsetMode)

		WISP_EVENT_MAP(EVENT_ID_DV_ADDR_MODE, OnEventToggleAddressMode)

		WISP_EVENT_MAP(EVENT_ID_DATA_WIDTH, OnEventDataWidth);

		WISP_EVENT_MAP(EVENT_ID_DV0, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_DV1, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_DV2, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_DV3, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_DV4, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_DV5, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_DV6, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_DV7, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_DV8, OnEventNewView)

		WISP_EVENT_MAP(EVENT_ID_OP_BFLIP, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_BINV, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_SHL, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_SHR, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_ROL, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_ROR, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_BSHL, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_BSHR, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_XOR, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_OR, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_AND, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_SIG, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_ADD, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_SUB, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_MUL, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_DIV, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_MOD, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_UPR, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_LWR, OnEventOp)
		WISP_EVENT_MAP(EVENT_ID_OP_INV, OnEventOp)

		WISP_EVENT_MAP(EVENT_ID_DV_SHOW_DATA_AT, OnEventShowDataAt)
		WISP_EVENT_MAP(EVENT_ID_CV_OPEN, OnEventShowDasm)
		WISP_EVENT_MAP(EVENT_ID_DV_SHOW_DATA_AT_REL, OnEventShowDataAt)
		WISP_EVENT_MAP(EVENT_ID_COPYVAL, OnEventCopyVal)
		WISP_EVENT_MAP(EVENT_ID_COPYADDR, OnEventCopyAddr)

		WISP_EVENT_MAP(EVENT_ID_COPYSEL, OnEventCopySelect)
		WISP_EVENT_MAP(EVENT_ID_COPYSEL_CLANG, OnEventCopySelect)
		WISP_EVENT_MAP(EVENT_ID_COPYSEL_SPACE, OnEventCopySelect)

		WISP_EVENT_MAP(EVENT_ID_FLOAT_MODE, OnEventToggleFloatAddressMode)

		WISP_EVENT_MAP(EVENT_ID_HISTORY_NEXT, OnEventNextHistory)
		WISP_EVENT_MAP(EVENT_ID_HISTORY_PREV, OnEventPrevHistory)
	WISP_EVENT_MAP_END

WISP_MENU_GROUP_ITEM DataViewPopupMenu[] = 
{
	{WSTR("Open new data view at "), EVENT_ID_NEW_DATA, 0xE0, 0, 0},
	{WSTR("Show Data                  Enter"), EVENT_ID_SHOW_DATA, 0xE0, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Open new code view at "), EVENT_ID_NEW_CODE, 0xDF, 0, 0},
	{WSTR("Disassemble code at " F0ADDR "X   Shift+Enter"), EVENT_ID_DISASM, 0xDF, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Data Operator Toolbar          Ctrl+T"), EVENT_ID_SHOW_OP, 0x76, 0, 0},
	{WSTR("Attach type to Addr                  "), EVENT_ID_ATTACH_TYPE, 0xE2, 0, 0}, //??? ICON ID
	{WSTR("Find                                 "), EVENT_ID_FIND_FORM, 0xC, 0, 0}, //??? ICON ID
	{0, 0, 0, 4, 0},
	{WSTR("Show offset                    Ctrl+S"), EVENT_ID_SHOW_OFFSET, 0xE2, 0, 0},
	{WSTR("Width                                "), EVENT_ID_DATA_WIDTH, 0xE2, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Address Mode                   Ctrl+O"), EVENT_ID_DV_ADDR_MODE, 0xE2, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Float mode                     Ctrl+M"), EVENT_ID_FLOAT_MODE, 0xE2, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Copy selected data (WO space)  Shift+X"), EVENT_ID_COPYSEL, 0x23, 0, 0},
	{WSTR("Copy selected data             Ctrl+X"), EVENT_ID_COPYSEL_SPACE, 0x82, 0, 0},
	{WSTR("Copy data as C source          Ctrl+Z"), EVENT_ID_COPYSEL_CLANG, 0x82, 0, 0},
	{WSTR("Copy value                     Ctrl+Enter"), EVENT_ID_COPYVAL, 0x100, 0, 0},
	{0},
};

WISP_TOOLBAR_RES_ITEM DataViewToolbar[] =
{
	{EVENT_ID_OP_BFLIP, 0, 0, WSTR("ByteFlip"), 0, 0},
	{EVENT_ID_OP_BINV, 1, 0, WSTR("Inverse Bits"), 0, 0},
	{EVENT_ID_OP_SHL, 2, 0, WSTR("Shift Left"), 0, 0},
	{EVENT_ID_OP_SHR, 3, 0, WSTR("Shift Right"), 0, 0},
	{EVENT_ID_OP_ROL, 4, 0, WSTR("Rotate Left"), 0, 0},
	{EVENT_ID_OP_ROR, 5, 0, WSTR("Rotate Right"), 0, 0},
	{EVENT_ID_OP_BSHL, 6, 0, WSTR("Block Shift Left"), 0, 0},
	{EVENT_ID_OP_BSHR, 7, 0, WSTR("Block Shift Right"), 0, 0},
	{EVENT_ID_OP_XOR, 8, 0, WSTR("XOR"), 0, 0},
	{EVENT_ID_OP_OR, 9, 0, WSTR("OR"), 0, 0},
	{EVENT_ID_OP_AND, 0xA, 0, WSTR("AND"), 0, 0},
	{EVENT_ID_OP_SIG, 0xB, 0, WSTR("Change Sign"), 0, 0},
	{EVENT_ID_OP_ADD, 0xC, 0, WSTR("Add"), 0, 0},
	{EVENT_ID_OP_SUB, 0xD, 0, WSTR("Subtract"), 0, 0},
	{EVENT_ID_OP_MUL, 0xE, 0, WSTR("Multiply"), 0, 0},
	{EVENT_ID_OP_DIV, 0xF, 0, WSTR("Divide"), 0, 0},
	{EVENT_ID_OP_MOD, 0x10, 0, WSTR("Mod"), 0, 0},
	{EVENT_ID_OP_UPR, 0x11, 0, WSTR("Upper Case"), 0, 0},
	{EVENT_ID_OP_LWR, 0x12, 0, WSTR("Lower Case"), 0, 0},
	{EVENT_ID_OP_INV, 0x13, 0, WSTR("Inverse Case"), 0, 0},
	{0},
};

	CDataView::CDataView()
	{
	}
	CDataView::~CDataView()
	{
	}

	bool CDataView::OnCreate(WISP_MSG *pMsg)
	{
		m_RButtonAddress = -1;
		m_LButtonAddress = -1;
		m_PopupMenu.LoadPopupMenuGroup(DataViewPopupMenu, this, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF));
		m_Style |= 0x10000;
		m_Toolbar.CreateEx(this, 0, -1, EVENT_ID_SHOW_OP, 0);
		m_Toolbar.LoadToolbar(gpCurWisp->m_DIBLib.LoadDIBList("\\SyserApp\\Operation.bmp", 16, 16, 0, 0xFF00FF), DataViewToolbar);
		m_Toolbar.Show(WISP_SH_HIDDEN);
		LoadKeyMap();
		return true;
	}
	bool CDataView::OnDestroy(WISP_MSG *pMsg)
	{
		m_PopupMenu.Destroy();
		return true;
	}

	void CDataView::OnKeyLButton(WISP_MSG *pMsg)
	{
		if (!CalcPointPos(&pMsg->MsgMouseWndPT, &m_LButtonAddress, 0))
			m_LButtonAddress = m_CurAddr;
	}

	void CDataView::OnKeyRButton(WISP_MSG *pMsg)
	{
		WCHAR szHotKey[50];
		WCHAR szMenu[256];

		m_RButtonAddress = -1;
		bool bValue = false;
		bool bAddr = false;
		LONG_PTR Offset = 0;
		if (CalcPointPos(&pMsg->MsgMouseWndPT, &m_RButtonAddress, 0))
		{
			if (gpSyser->m_KeyMap[VK_SHIFT])
				Offset = m_RButtonAddress - m_CurAddr;
			bValue = ReadMemory(m_RButtonAddress, &m_RButtonAddress, sizeof(m_RButtonAddress)) == sizeof(m_RButtonAddress);
			if (bValue)
			{
				ULONG_PTR Val;
				bAddr = ReadMemory(m_RButtonAddress, &Val, sizeof(Val)) == sizeof(Val);
				if (bAddr)
				{
					m_RButtonAddress -= Offset;

					TSPrintf(szMenu, WSTR("Open new data view at " F0ADDR "X"), m_RButtonAddress);
					m_PopupMenu.SetItemText(m_PopupMenu.GetItem(EVENT_ID_NEW_DATA), szMenu);

					gpSyser->m_MainFrame.m_KeyMappingPage.GetHotKey(GetKeyMap(EVENT_ID_DV_SHOW_DATA_AT), szHotKey);
					TSPrintf(szMenu, WSTR("Display data at " F0ADDR "X           %s"), m_RButtonAddress, szHotKey);
					m_PopupMenu.SetItemText(m_PopupMenu.GetItem(EVENT_ID_SHOW_DATA), szMenu);

					TSPrintf(szMenu, WSTR("Open new code view at " F0ADDR "X"), m_RButtonAddress);
					m_PopupMenu.SetItemText(m_PopupMenu.GetItem(EVENT_ID_NEW_CODE), szMenu);

					gpSyser->m_MainFrame.m_KeyMappingPage.GetHotKey(GetKeyMap(EVENT_ID_CV_OPEN), szHotKey);
					TSPrintf(szMenu, WSTR("Disassemble code at " F0ADDR "X       %s"), m_RButtonAddress, szHotKey);
					m_PopupMenu.SetItemText(m_PopupMenu.GetItem(EVENT_ID_DISASM), szMenu);
				}
			}
		}
		
		gpSyser->m_MainFrame.m_KeyMappingPage.GetHotKey(GetKeyMap(EVENT_ID_DV_ADDR_MODE), szHotKey);
		if (GetAddressMode())
			TSPrintf(szMenu, WSTR("Address Mode                   %s"), szHotKey);
		else	TSPrintf(szMenu, WSTR("Offset Mode                    %s"), szHotKey);
		m_PopupMenu.SetItemText(m_PopupMenu.GetItem(EVENT_ID_DV_ADDR_MODE), szMenu);

		gpSyser->m_MainFrame.m_KeyMappingPage.GetHotKey(GetKeyMap(EVENT_ID_COPYSEL), szHotKey);
		TSPrintf(szMenu, WSTR("Copy selected data (WO space)  %s"), szHotKey);
		m_PopupMenu.SetItemText(m_PopupMenu.GetItem(EVENT_ID_COPYSEL), szMenu);

		gpSyser->m_MainFrame.m_KeyMappingPage.GetHotKey(GetKeyMap(EVENT_ID_COPYSEL_SPACE), szHotKey);
		TSPrintf(szMenu, WSTR("Copy selected data             %s"), szHotKey);
		m_PopupMenu.SetItemText(m_PopupMenu.GetItem(EVENT_ID_COPYSEL_SPACE), szMenu);

		gpSyser->m_MainFrame.m_KeyMappingPage.GetHotKey(GetKeyMap(EVENT_ID_COPYSEL_CLANG), szHotKey);
		TSPrintf(szMenu, WSTR("Copy data as C source          %s"), szHotKey);
		m_PopupMenu.SetItemText(m_PopupMenu.GetItem(EVENT_ID_COPYSEL_CLANG), szMenu);

		gpSyser->m_MainFrame.m_KeyMappingPage.GetHotKey(GetKeyMap(EVENT_ID_COPYVAL), szHotKey);
		TSPrintf(szMenu, WSTR("Copy value                     %s"), szHotKey);
		m_PopupMenu.SetItemText(m_PopupMenu.GetItem(EVENT_ID_COPYVAL), szMenu);

		gpSyser->m_MainFrame.m_KeyMappingPage.GetHotKey(GetKeyMap(EVENT_ID_SHOW_OP), szHotKey);
		TSPrintf(szMenu, WSTR("Data Operator Toolbar          %s"), szHotKey);
		m_PopupMenu.SetItemText(m_PopupMenu.GetItem(EVENT_ID_SHOW_OP), szMenu);

		gpSyser->m_MainFrame.m_KeyMappingPage.GetHotKey(GetKeyMap(EVENT_ID_SHOW_OFFSET), szHotKey);
		TSPrintf(szMenu, WSTR("Show offset                    %s"), szHotKey);
		m_PopupMenu.SetItemText(m_PopupMenu.GetItem(EVENT_ID_SHOW_OFFSET), szMenu);

		gpSyser->m_MainFrame.m_KeyMappingPage.GetHotKey(GetKeyMap(EVENT_ID_FLOAT_MODE), szHotKey);
		if (GetFloatAddressMode())
			TSPrintf(szMenu, WSTR("Fixed mode                     %s"), szHotKey);
		else	TSPrintf(szMenu, WSTR("Float mode                     %s"), szHotKey);
		m_PopupMenu.SetItemText(m_PopupMenu.GetItem(EVENT_ID_FLOAT_MODE), szMenu);

		gpSyser->m_MainFrame.m_KeyMappingPage.GetHotKey(GetKeyMap(EVENT_ID_DATA_WIDTH), szHotKey);
		if (m_nByte_Line == 16)
			TSPrintf(szMenu, WSTR("Width 32                       %s"), szHotKey);
		else
		if (m_nByte_Line == 32)
			TSPrintf(szMenu, WSTR("Width 16                       %s"), szHotKey);
		m_PopupMenu.SetItemText(m_PopupMenu.GetItem(EVENT_ID_DATA_WIDTH), szMenu);

		m_PopupMenu.Enable(EVENT_ID_FLOAT_MODE, !m_FloatAddressStr.IsEmpty());
		m_PopupMenu.Enable(EVENT_ID_NEW_DATA, bAddr);
		m_PopupMenu.Enable(EVENT_ID_SHOW_DATA, bAddr);
		m_PopupMenu.Enable(EVENT_ID_NEW_CODE, bAddr);
		m_PopupMenu.Enable(EVENT_ID_DISASM, bAddr);
		m_PopupMenu.Enable(EVENT_ID_COPYSEL, m_bSelect);
		m_PopupMenu.Enable(EVENT_ID_COPYSEL_SPACE, m_bSelect);
		m_PopupMenu.Enable(EVENT_ID_COPYSEL_CLANG, m_bSelect);
		m_PopupMenu.Enable(EVENT_ID_COPYVAL, bValue);

		m_PopupMenu.Point(0, 1);
	}

	unsigned int CDataView::GetKeyMap(unsigned int CmdID)
	{
		for (map<unsigned int, unsigned int>::IT it = m_KeyMap.begin();
			it != m_KeyMap.end(); ++it)
		{
			if (it->second == CmdID)
				return it->first;
		}
		return 0;
	}

	bool CDataView::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (!pMsg->KeyEvent.bKeyDown) return true;

		map<unsigned int, unsigned int>::IT it = m_KeyMap.find(pMsg->KeyEvent.KeyType);
		if (it != m_KeyMap.end())
		{
			WISP_MSG Msg;
			Msg.Command.CmdID = it->second;
			return OnEvent(&Msg);
		}
		int Size;
		switch (pMsg->KeyEvent.KeyType)
		{
		case VK_LBUTTON:
			OnKeyLButton(pMsg); //Data Range
			break;
		case VK_RBUTTON:
			OnKeyRButton(pMsg); //Menu
			return false;
			break;
		case 0x53: //S
			gpSyser->m_MainFrame.m_SystemExplorer.m_ViewSplitWnd.GetSize(0, &Size);
			Size += 10;
			gpSyser->m_MainFrame.m_SystemExplorer.m_ViewSplitWnd.SetSize(0, Size);
			gpSyser->m_MainFrame.m_SystemExplorer.m_ViewSplitWnd.AdjustWndPos();
			return false;
			break;
		case 0x57: //W
			gpSyser->m_MainFrame.m_SystemExplorer.m_ViewSplitWnd.GetSize(0, &Size);
			if (Size <= 10)
				Size = 0;
			else	Size -= 10;
			gpSyser->m_MainFrame.m_SystemExplorer.m_ViewSplitWnd.SetSize(0, Size);
			gpSyser->m_MainFrame.m_SystemExplorer.m_ViewSplitWnd.AdjustWndPos();
			return false;
			break;
		}
		return true;
	}

	bool CDataView::OnEventGotoData(WISP_MSG *pMsg)
	{
		ULONG_PTR Val;
		if (m_RButtonAddress != -1 && gpSyser->m_pDebugger->ReadMemory(m_RButtonAddress, &Val, sizeof(Val)) == sizeof(Val))
			ViewAddress(m_RButtonAddress, true);
		return true;
	}
	bool CDataView::OnEventNewData(WISP_MSG *pMsg)
	{
		static_cast<CMultiDataView*>(m_ParentWnd)->InsertView(this);
		static_cast<CMultiDataView*>(m_ParentWnd)->SetActiveAddress(m_RButtonAddress, 0);
		return true;
	}
	bool CDataView::OnEventGotoDasm(WISP_MSG *pMsg)
	{
		if (m_RButtonAddress != -1)
		{
			CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
			if (pWnd) pWnd->GotoAddr(m_RButtonAddress, true);
		}
		return true;
	}
	bool CDataView::OnEventNewDasm(WISP_MSG *pMsg)
	{
		if (m_RButtonAddress != -1)
		{
			CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.InsertView(0);
			if (pWnd) pWnd->GotoAddr(m_RButtonAddress, true);
		}
		return true;
	}
	bool CDataView::OnEventShowPhysAddr(WISP_MSG *pMsg)
	{
		return true;
	}
	bool CDataView::OnEventDasmPhysAddr(WISP_MSG *pMsg)
	{
		return true;
	}
	bool CDataView::OnEventToggleToolbar(WISP_MSG *pMsg)
	{
		if (m_Toolbar.IsWindowShow())
			m_Toolbar.Show(WISP_SH_HIDDEN);
		else	m_Toolbar.Show(WISP_SH_MINSIZE);
		return true;
	}

	bool CDataView::OnEventAttachType(WISP_MSG *pMsg)
	{
		ULONG_PTR Addr = m_LButtonAddress;
		if (Addr == -1)
			Addr = m_CurAddr;
		m_StructViewerForm.Create(nullptr, 5, Addr); //SHOW_SH_???
		return true;
	}
	bool CDataView::OnEventFindForm(WISP_MSG *pMsg)
	{
		//TODO
		return true;
	}

	bool CDataView::OnEventToggleOffsetMode(WISP_MSG *pMsg)
	{
		ToggleHexOffset();
		return true;
	}
	bool CDataView::OnEventToggleAddressMode(WISP_MSG *pMsg)
	{
		SetAddressMode(1-GetAddressMode(), m_CurAddr);
		Update((WISP_RECT*)0);
		return true;
	}

	bool CDataView::OnEventDataWidth(WISP_MSG *pMsg)
	{
		if (m_nByte_Line == 16)
			m_nByte_Line = 32;
		else
		if (m_nByte_Line == 32)
			m_nByte_Line = 16;
		RecalcLayout();
		Update((WISP_RECT*)0);
		return true;
	}

	bool CDataView::OnEventNewView(WISP_MSG *pMsg)
	{
		CWispWnd *pWnd = (CWispWnd *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetWnd(pMsg->Command.CmdID - EVENT_ID_DV0);
		if (pWnd)
		{
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.ChangeActiveWnd(pWnd);
			return true;
		}

		if (!gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.InsertView(
				(CDataView*)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveWnd()))
			return false;
		return true;
	}

	bool CDataView::OnEventOp(WISP_MSG *pMsg)
	{
		ULONG_PTR Start;
		ULONG_PTR End;
		unsigned long Len;
		GetDataRange(&Start, &End, &Len);
		m_OperatorDlg.Create(0, WISP_SH_MODAL_BLOCK, Start, End, Len, pMsg->Command.CmdID);
		return true;
	}

	bool CDataView::OnEventShowDataAt(WISP_MSG *pMsg)
	{
		LONG_PTR Offset = 0;
		ULONG_PTR TargetAddr;
		ULONG_PTR Addr;
		if (CalcPointPos(&m_CaretPos, &Addr, 0))
		{
			if (pMsg && pMsg->Command.CmdID == EVENT_ID_DV_SHOW_DATA_AT_REL)
				 Offset = Addr - m_CurAddr;

			Addr += m_ShowLength - m_CaretIndex % (3 * m_ShowLength)/2 - 1;
			if (ReadMemory(Addr, &TargetAddr, sizeof(TargetAddr)))
			{
				ULONG_PTR Val;
				if (TargetAddr != -1)
				{
					TargetAddr -= Offset;
					if (gpSyser->m_pDebugger->ReadMemory(TargetAddr, &Val, sizeof(Val)) == sizeof(Val))
						ViewAddress(TargetAddr, true);
				}
			}
		}
		return true;
	}

	bool CDataView::OnEventShowDasm(WISP_MSG *pMsg)
	{
		ULONG_PTR TargetAddr;
		ULONG_PTR Addr;
		if (CalcPointPos(&m_CaretPos, &Addr, 0))
		{
			Addr += m_ShowLength - m_CaretIndex % (3 * m_ShowLength)/2 - 1;
			if (ReadMemory(Addr, &TargetAddr, sizeof(TargetAddr)))
			{
				ULONG_PTR Val;
				if (TargetAddr != -1 && gpSyser->m_pDebugger->ReadMemory(TargetAddr, &Val, sizeof(Val)) == sizeof(Val))
				{
					CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
					if (pWnd) pWnd->GotoAddr(TargetAddr, true);
				}
			}
		}
		return true;
	}

	bool CDataView::OnEventCopyVal(WISP_MSG *pMsg)
	{
		TWideString String;
		ULONG_PTR Addr;
		if (CalcPointPos(&m_CaretPos, &Addr, 0))
		{
			Addr += m_ShowLength - m_CaretIndex % (3 * m_ShowLength)/2 - 1;
			ULONG_PTR Value;
			if (ReadMemory(Addr, &Value, sizeof(Value)) && Value != -1)
			{
				WCHAR Buffer[10];
				TSPrintf(Buffer, WSTR("" F0ADDR "X"), Value); //??? LOWER CASE
				String = Buffer;
				m_pWispBase->SetClipboard(String.operator const WCHAR*());
			}
		}
		return true;
	}
	bool CDataView::OnEventCopyAddr(WISP_MSG *pMsg)
	{
		TWideString String;
		ULONG_PTR Addr;
		if (CalcPointPos(&m_CaretPos, &Addr, 0))
		{
			WCHAR Buffer[10];
			TSPrintf(Buffer, WSTR("" F0ADDR "X"), Addr); //??? LOWER CASE
			String = Buffer;
			m_pWispBase->SetClipboard(String.operator const WCHAR*());
		}
		return true;
	}

	bool CDataView::OnEventCopySelect(WISP_MSG *pMsg)
	{
		WCHAR Buffer[256];
		TWideString String;
		bool bSpace = true;

		if (m_bSelect)
		{
			if (pMsg->Command.CmdID == EVENT_ID_COPYSEL_SPACE)
				bSpace = false;

			int Len = m_SelEnd - m_SelStart + 1;

			if (pMsg->Command.CmdID == EVENT_ID_COPYSEL_CLANG)
			{
				//String += WSTR("// Generated by Syser software's Syser Debugger v");
				//TStrCpy(Buffer, "1.99.1900.1220");
				//String += Buffer;
				//String += WSTR("\r\n//   http://www.sysersoft.com\r\n");

				String += WSTR("// Generated by Syser Debugger");
				String += WSTR("\r\n//   http://exelab.ru/f/\r\n");

				TSPrintf(Buffer, WSTR("unsigned char rawData[%d] =\r\n{"), Len);
				String += Buffer;
				for (int i = 0; i < Len; ++i)
				{
					if (!(i&0xf)) //%
						String += WSTR("\r\n\t");
					unsigned char Byte;
					if (gpSyser->m_pDebugger->ReadMemory(m_SelStart+i, &Byte, 1) != 1)
						break;
					if (i == Len-1)
						TSPrintf(Buffer, WSTR("0x%02X"), Byte);
					else	TSPrintf(Buffer, WSTR("0x%02X, "), Byte);
					String += Buffer;
				}
				String += WSTR("\r\n};\n");
				
			} else
			{
				for (int i = 0; i < Len/m_ShowLength; ++i)
				{
					ULONG_PTR Data;
					if (gpSyser->m_pDebugger->ReadMemory(m_SelStart + i * m_ShowLength, &Data, m_ShowLength) != m_ShowLength)
						break;
					switch (m_ShowLength)
					{
					case 1: TSPrintf(Buffer, WSTR("%02x"), Data&0xFF); break;
					case 2: TSPrintf(Buffer, WSTR("%04x"), Data&0xFFFF); break;
					case 4: TSPrintf(Buffer, WSTR("%08x"), Data&0xFFFFFFFF); break;
				#ifdef _X64_
					case 8: TSPrintf(Buffer, WSTR("%016I64x"), Data&0xFFFFFFFFFFFFFFFF); break;
				#endif
					}
					String += Buffer;
					if (bSpace && i != Len-1)
						String += WSTR(" ");
				}
			}
			m_pWispBase->SetClipboard(String.operator const WCHAR*());
		}

		return true;
	}

	bool CDataView::OnEventToggleFloatAddressMode(WISP_MSG *pMsg)
	{
		ToggleFloatAddressMode();
		return true;
	}

	bool CDataView::OnEventNextHistory(WISP_MSG *pMsg)
	{
		NextPos(); //???
		return true;
	}
	bool CDataView::OnEventPrevHistory(WISP_MSG *pMsg)
	{
		PrevPos(); //???
		return true;
	}

	unsigned long CDataView::WriteMemory(ULONG_PTR Address, void *Buffer, unsigned long Size)
	{
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
			return gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size);
		return 0;
	}

	unsigned long CDataView::ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size)
	{
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
			return gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size);
		return 0;
	}

	unsigned int CDataView::GetHexByteAttribute(ULONG_PTR Address)
	{
		unsigned int HexByte = CWispHexWnd::GetHexByteAttribute(Address);
		if (gpSyser->m_pDebugger)
		{
			if (gpSyser->m_pDebugger->GetCodeBP(Address, 0, 0)) return HEX_BYTE_BPX;
			if (gpSyser->m_pDebugger->GetDataBP1(Address, 0, 0)) return HEX_BYTE_BPX;
		}
		return HexByte;
	}

	ULONG_PTR CDataView::GetFloatAddress(const TWideString & FloatAddress)
	{
		ULONG_PTR Result;
		if (!FloatAddress.IsEmpty() &&
			gpSyser->m_SyserUI.CalcExp(FloatAddress.operator const WCHAR*(), &Result))
			return Result;
		return m_CurAddr;
	}


	void CDataView::OnChangeView()
	{
		int Index = 0;
		TListIter<WISP_TAB_WND_ITEM> It = gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.m_WndList.Begin();
		while (It != gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.m_WndList.End())
		{
			if (It->pWnd == this)
			{
				WCHAR Buffer[64];
				TSPrintf(Buffer, WSTR("" F0ADDR "X"), m_CurAddr);
				gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.SetTabText(Index, Buffer);
			}
			++It;
			++Index;
		}
	}
	void CDataView::OnDataUpdated()
	{
		gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
	}
	int CDataView::GetStartLine()
	{
		int nStartLine = 0;
		if (m_Toolbar.IsWindowShow())
			nStartLine += 2;
		if (m_bShowOffset)
			nStartLine += 1;
		return nStartLine;
	}

	void CDataView::LoadKeyMap()
	{
		if (!gpSyser->GetHotKey(EVENT_ID_DV0, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000031, EVENT_ID_DV0)); //WISP_SK_ALT 1
		if (!gpSyser->GetHotKey(EVENT_ID_DV1, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000032, EVENT_ID_DV1)); //WISP_SK_ALT 2
		if (!gpSyser->GetHotKey(EVENT_ID_DV2, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000033, EVENT_ID_DV2)); //WISP_SK_ALT 3
		if (!gpSyser->GetHotKey(EVENT_ID_DV3, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000034, EVENT_ID_DV3)); //WISP_SK_ALT 4
		if (!gpSyser->GetHotKey(EVENT_ID_DV4, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000035, EVENT_ID_DV4)); //WISP_SK_ALT 5
		if (!gpSyser->GetHotKey(EVENT_ID_DV5, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000036, EVENT_ID_DV5)); //WISP_SK_ALT 6
		if (!gpSyser->GetHotKey(EVENT_ID_DV6, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000037, EVENT_ID_DV6)); //WISP_SK_ALT 7
		if (!gpSyser->GetHotKey(EVENT_ID_DV7, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000038, EVENT_ID_DV7)); //WISP_SK_ALT 8
		if (!gpSyser->GetHotKey(EVENT_ID_DV8, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000039, EVENT_ID_DV8)); //WISP_SK_ALT 9

		if (!gpSyser->GetHotKey(EVENT_ID_DV_SHOW_DATA_AT, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0xD       , EVENT_ID_DV_SHOW_DATA_AT)); //VK_RETURN
		if (!gpSyser->GetHotKey(EVENT_ID_DV_SHOW_DATA_AT_REL, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x6000000D, EVENT_ID_DV_SHOW_DATA_AT_REL)); //WISP_SK_CTRL WISP_SK_ALT VK_RETURN
		if (!gpSyser->GetHotKey(EVENT_ID_CV_OPEN, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x1000000D, EVENT_ID_CV_OPEN)); //WISP_SK_SHIFT VK_RETURN
		if (!gpSyser->GetHotKey(EVENT_ID_COPYVAL, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x4000000D, EVENT_ID_COPYVAL)); //WISP_SK_CTRL VK_RETURN
		if (!gpSyser->GetHotKey(EVENT_ID_COPYADDR, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x2000000D, EVENT_ID_COPYADDR)); //WISP_SK_ALT VK_RETURN
		if (!gpSyser->GetHotKey(EVENT_ID_COPYSEL, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x40000058, EVENT_ID_COPYSEL)); //WISP_SK_CTRL X
		if (!gpSyser->GetHotKey(EVENT_ID_COPYSEL_SPACE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x10000058, EVENT_ID_COPYSEL_SPACE)); //WISP_SK_SHIFT X
		if (!gpSyser->GetHotKey(EVENT_ID_DV_ADDR_MODE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x4000004F, EVENT_ID_DV_ADDR_MODE)); //WISP_SK_CTRL O
		if (!gpSyser->GetHotKey(EVENT_ID_COPYSEL_CLANG, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x4000005A, EVENT_ID_COPYSEL_CLANG)); //WISP_SK_CTRL Z
		if (!gpSyser->GetHotKey(EVENT_ID_HISTORY_NEXT, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0xBB      , EVENT_ID_HISTORY_NEXT)); //VK_OEM_PLUS
		if (!gpSyser->GetHotKey(EVENT_ID_HISTORY_PREV, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0xBD      , EVENT_ID_HISTORY_PREV)); //VK_OEM_MINUS
		if (!gpSyser->GetHotKey(EVENT_ID_FLOAT_MODE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x4000004D, EVENT_ID_FLOAT_MODE)); //WISP_SK_CTRL M
		if (!gpSyser->GetHotKey(EVENT_ID_SHOW_OFFSET, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x40000053, EVENT_ID_SHOW_OFFSET)); //WISP_SK_CTRL S
		if (!gpSyser->GetHotKey(EVENT_ID_SHOW_OP, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x40000054, EVENT_ID_SHOW_OP)); //WISP_SK_CTRL T
	}

	bool CDataView::GetDataRange(ULONG_PTR *Start, ULONG_PTR *End, unsigned long *Len)
	{
		if (m_bSelect)
		{
			*Start = m_SelStart;
			*End = m_SelEnd;
			if (m_SelStart == m_SelEnd)
			{
				++*End;
				*Len = m_ShowLength;
			}
		} else
		{
			*Start = m_LButtonAddress;
			if (*Start == -1)
				*Start = m_CurAddr;
			*End = *Start + m_ShowLength;
		}
		*Len = m_ShowLength;
		return true;
	}

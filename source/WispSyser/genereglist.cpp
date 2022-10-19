
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "genereglist.hpp"
#include "syserconfig.hpp"
#include "resource.hpp"

enum
{
  GL_REG = 0,
  GL_VALUE = 1,
  GL_CONTEXT = 2,
};

WISP_MENU_GROUP_ITEM GeneRegListPopupMenu[] =
{
	{WSTR("Show Code "), EVENT_ID_SHOW_CODE, 0xE0, 0, 0},
	{WSTR("Show Data "), EVENT_ID_SHOW_DATA, 0xDF, 0, 0},
	{0},
};

	WISP_MSG_MAP_BEGIN(CGeneRegList)
	//	WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_CLOSE, OnClose)
//		WISP_MSG_MAP(WISP_WM_GET_FOCUS, OnGetFocus)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

	WISP_EVENT_MAP_BEGIN(CGeneRegList)
		WISP_EVENT_MAP(EVENT_ID_CAX_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_CBX_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_CCX_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_CDX_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_CSP_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_CBP_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_CSI_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_CDI_DATA, OnEventGotoData)
	#ifdef _X64_
		WISP_EVENT_MAP(EVENT_ID_R8_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_R9_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_R10_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_R11_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_R12_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_R13_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_R14_DATA, OnEventGotoData)
		WISP_EVENT_MAP(EVENT_ID_R15_DATA, OnEventGotoData)
	#endif
		WISP_EVENT_MAP(EVENT_ID_CIP_DATA, OnEventGotoData)

		WISP_EVENT_MAP(EVENT_ID_CAX_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_CBX_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_CCX_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_CDX_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_CSP_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_CBP_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_CSI_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_CDI_CODE, OnEventGotoCode)
	#ifdef _X64_
		WISP_EVENT_MAP(EVENT_ID_R8_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_R9_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_R10_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_R11_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_R12_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_R13_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_R14_CODE, OnEventGotoCode)
		WISP_EVENT_MAP(EVENT_ID_R15_CODE, OnEventGotoCode)
	#endif
		WISP_EVENT_MAP(EVENT_ID_CIP_CODE, OnEventGotoCode)

		WISP_EVENT_MAP(EVENT_ID_SHOW_CODE, OnEventCode)

		WISP_EVENT_MAP(EVENT_ID_SHOW_DATA, OnEventData)
	WISP_EVENT_MAP_END


	CGeneRegList::CGeneRegList()
	{
	}
	CGeneRegList::~CGeneRegList()
	{
	}

	bool CGeneRegList::OnUpdate(WISP_MSG *pMsg)
	{
		m_ColorChanges = gSyserColors.Color[4];
		m_ColorSame = gSyserColors.Color[1];
		m_ColorFocus = gSyserColors.Color[2];

		if (m_Style & 1)
		{
			AttachTitleDIB(gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x34));
		}
		m_ItemColor = m_ColorSame;
		SetBGColor(gSyserColors.Color[0]);
		UpdateItemColor(0);
	//	for (int i = 0; i < REG_GENE_REGCOUNT; ++i)
	//	{
	//		SetItemColor(m_hGeneRegItem[i], GL_REG, m_ItemColor);
	//	}
		return true;//CWispWnd::OnUpdate(pMsg);
	}

	bool CGeneRegList::OnCreate(WISP_MSG *pMsg)
	{
		m_ColorChanges = gSyserColors.Color[4];
		m_ColorSame = gSyserColors.Color[1];
		m_ColorFocus = gSyserColors.Color[2];

		memcpy(&m_CPUREGPTR, &gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR, sizeof(m_CPUREGPTR));

		if (m_Style & 1)
		{
			AttachTitleDIB(gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x34));
		}
		m_ItemColor = m_ColorSame;
		SetBGColor(gSyserColors.Color[0]);
		m_PopupMenu.LoadPopupMenuGroup(GeneRegListPopupMenu, this, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF));
		m_MenuItemShowCode = m_PopupMenu.GetItem(EVENT_ID_SHOW_CODE);
		m_MenuItemShowData = m_PopupMenu.GetItem(EVENT_ID_SHOW_DATA);

		int Ext = m_ClientDC.GetTextExtent(WSTR("X"), -1, 0);

		InsertColumn(WSTR("Register"), 9 * Ext, 0, 0, -1);
		InsertColumn(WSTR("Value[F2]"), (2+ADDR_SIZE) * Ext, 1, 0, -1);
		InsertColumn(WSTR("Context"), 100, 0, 0, -1);
	#ifdef _X64_
		m_hGeneRegItem[REG_GENE_CAX] = InsertItem(WSTR("RAX"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CBX] = InsertItem(WSTR("RBX"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CCX] = InsertItem(WSTR("RCX"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CDX] = InsertItem(WSTR("RDX"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CSI] = InsertItem(WSTR("RSI"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CDI] = InsertItem(WSTR("RDI"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CBP] = InsertItem(WSTR("RBP"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CSP] = InsertItem(WSTR("RSP"), 0, 0, 0, -1);

		m_hGeneRegItem[REG_GENE_R8] = InsertItem(WSTR("R8"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_R9] = InsertItem(WSTR("R9"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_R10] = InsertItem(WSTR("R10"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_R11] = InsertItem(WSTR("R11"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_R12] = InsertItem(WSTR("R12"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_R13] = InsertItem(WSTR("R13"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_R14] = InsertItem(WSTR("R14"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_R15] = InsertItem(WSTR("R15"), 0, 0, 0, -1);

		m_hGeneRegItem[REG_GENE_CIP] = InsertItem(WSTR("RIP"), 0, 0, 0, -1);
	#else
		m_hGeneRegItem[REG_GENE_CAX] = InsertItem(WSTR("EAX"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CBX] = InsertItem(WSTR("EBX"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CCX] = InsertItem(WSTR("ECX"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CDX] = InsertItem(WSTR("EDX"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CSI] = InsertItem(WSTR("ESI"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CDI] = InsertItem(WSTR("EDI"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CBP] = InsertItem(WSTR("EBP"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CSP] = InsertItem(WSTR("ESP"), 0, 0, 0, -1);

		m_hGeneRegItem[REG_GENE_CIP] = InsertItem(WSTR("EIP"), 0, 0, 0, -1);
	#endif
		m_hGeneRegItem[REG_GENE_EFLAG] = InsertItem(WSTR("EFLAG"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_AF] = InsertItem(WSTR("AF"), m_hGeneRegItem[REG_GENE_EFLAG], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_CF] = InsertItem(WSTR("CF"), m_hGeneRegItem[REG_GENE_EFLAG], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_DF] = InsertItem(WSTR("DF"), m_hGeneRegItem[REG_GENE_EFLAG], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_IF] = InsertItem(WSTR("IF"), m_hGeneRegItem[REG_GENE_EFLAG], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_OF] = InsertItem(WSTR("OF"), m_hGeneRegItem[REG_GENE_EFLAG], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_PF] = InsertItem(WSTR("PF"), m_hGeneRegItem[REG_GENE_EFLAG], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_SF] = InsertItem(WSTR("SF"), m_hGeneRegItem[REG_GENE_EFLAG], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_ZF] = InsertItem(WSTR("ZF"), m_hGeneRegItem[REG_GENE_EFLAG], 0, 0, -1);

		m_hGeneRegItem[REG_GENE_CS] = InsertItem(WSTR("CS"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_DS] = InsertItem(WSTR("DS"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_ES] = InsertItem(WSTR("ES"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_FS] = InsertItem(WSTR("FS"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_GS] = InsertItem(WSTR("GS"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_SS] = InsertItem(WSTR("SS"), 0, 0, 0, -1);

		m_hGeneRegItem[REG_GENE_BASE_CS] = InsertItem(WSTR("Base"), m_hGeneRegItem[REG_GENE_CS], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_BASE_DS] = InsertItem(WSTR("Base"), m_hGeneRegItem[REG_GENE_DS], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_BASE_ES] = InsertItem(WSTR("Base"), m_hGeneRegItem[REG_GENE_ES], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_BASE_FS] = InsertItem(WSTR("Base"), m_hGeneRegItem[REG_GENE_FS], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_BASE_GS] = InsertItem(WSTR("Base"), m_hGeneRegItem[REG_GENE_GS], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_BASE_SS] = InsertItem(WSTR("Base"), m_hGeneRegItem[REG_GENE_SS], 0, 0, -1);
	#ifdef _RING0_
		m_hGeneRegItem[REG_GENE_GDTR] = InsertItem(WSTR("GDTR"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_IDTR] = InsertItem(WSTR("IDTR"), 0, 0, 0, -1);

		m_hGeneRegItem[REG_GENE_LDTR] = InsertItem(WSTR("LDTR"), 0, 0, 0, -1);
		m_hGeneRegItem[REG_GENE_TR] = InsertItem(WSTR("TR"), 0, 0, 0, -1);

		m_hGeneRegItem[REG_GENE_BASE_GDTR] = InsertItem(WSTR("Base"), m_hGeneRegItem[REG_GENE_GDTR], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_LIMIT_GDTR] = InsertItem(WSTR("Limit"), m_hGeneRegItem[REG_GENE_GDTR], 0, 0, -1);

		m_hGeneRegItem[REG_GENE_BASE_IDTR] = InsertItem(WSTR("Base"), m_hGeneRegItem[REG_GENE_IDTR], 0, 0, -1);
		m_hGeneRegItem[REG_GENE_LIMIT_IDTR] = InsertItem(WSTR("Limit"), m_hGeneRegItem[REG_GENE_IDTR], 0, 0, -1);
	#endif
		m_pEditItem = m_hGeneRegItem[REG_GENE_CAX];

		for (int i = 0; i < REG_GENE_REGCOUNT; ++i)
		{
			SetItemValue(m_hGeneRegItem[i], 0, i);
		}
		ResetContext();
		LoadKeyMap();
		return true;
	}
	bool CGeneRegList::OnClose(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.FocusActive();
		return true;
	}
	bool CGeneRegList::OnDestroy(WISP_MSG *pMsg)
	{
		m_PopupMenu.Destroy();
		return true;
	}

//	bool CGeneRegList::OnGetFocus(WISP_MSG *pMsg)
//	{
//		CListStringItem *Item = GetNext(0, 0);
//		if (Item)
//		{
//			Select(Item);
//		}
//		return true;
//	}

	bool CGeneRegList::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen) return true;
		if (pMsg->KeyEvent.bKeyDown) return true;

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
		case VK_RBUTTON:{
			CListStringItem *Item = FullFindItem(0, 8);
			bool bRead = false;
			if (Item)
			{
				ULONG_PTR Addr = GetItemValue(Item, GL_VALUE);
				unsigned char Byte;
				if (gpSyser->m_pDebugger->ReadMemory(Addr, &Byte, 1) == 1)
					bRead = true;
				CListStringItem::CONTENT *Content = GetItemContent(Item, GL_VALUE);
				WCHAR Buffer[128];
				TStrCpy(Buffer, WSTR("Show Code "));
				TStrCat(Buffer, Content->String.operator const WCHAR*());
				m_PopupMenu.SetItemText(m_MenuItemShowCode, Buffer);
				TStrCpy(Buffer, WSTR("Show Data "));
				TStrCat(Buffer, Content->String.operator const WCHAR*());
				m_PopupMenu.SetItemText(m_MenuItemShowData, Buffer);
			}
			m_PopupMenu.Enable(m_MenuItemShowCode, bRead);
			m_PopupMenu.Enable(m_MenuItemShowData, Item && bRead);
			m_PopupMenu.Point(0, 1);
			}break;
		case 0x41: //'A'
			gpSyser->m_MainFrame.m_SystemExplorer.m_MainSplitWnd.GetSize(0, &Size);
			if (Size <= 10)
				Size = 0;
			else	Size -= 10;
			gpSyser->m_MainFrame.m_SystemExplorer.m_MainSplitWnd.SetSize(0, Size);
			gpSyser->m_MainFrame.m_SystemExplorer.m_MainSplitWnd.AdjustWndPos();
			return false;
			break;
		case 0x44: //'D'
			gpSyser->m_MainFrame.m_SystemExplorer.m_MainSplitWnd.GetSize(0, &Size);
			Size += 10;
			gpSyser->m_MainFrame.m_SystemExplorer.m_MainSplitWnd.SetSize(0, Size);
			gpSyser->m_MainFrame.m_SystemExplorer.m_MainSplitWnd.AdjustWndPos();
			return false;
			break;
		case 0x53: //'S'
			gpSyser->m_MainFrame.m_SystemExplorer.m_MoniteSplitWnd.GetSize(0, &Size);
			Size += 10;
			gpSyser->m_MainFrame.m_SystemExplorer.m_MoniteSplitWnd.SetSize(0, Size);
			gpSyser->m_MainFrame.m_SystemExplorer.m_MoniteSplitWnd.AdjustWndPos();
			return false;
			break;
		case 0x57: //'W'
			gpSyser->m_MainFrame.m_SystemExplorer.m_MoniteSplitWnd.GetSize(0, &Size);
			if (Size <= 10)
				Size = 0;
			else	Size -= 10;
			gpSyser->m_MainFrame.m_SystemExplorer.m_MoniteSplitWnd.SetSize(0, Size);
			gpSyser->m_MainFrame.m_SystemExplorer.m_MoniteSplitWnd.AdjustWndPos();
			return false;
			break;
		case VK_F2: {
			CListStringItem *Item = FullFindItem(0, 8);
			if (Item)
			{
				Scroll(Item, -1);
				EditItem(Item, GL_VALUE);
			}
			return false;
			} break;
		}
		return true;
	}

	bool CGeneRegList::OnEventGotoData(WISP_MSG *pMsg)
	{
		switch (pMsg->Command.CmdID)
		{
		case EVENT_ID_CAX_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pCAX, 0); break;
		case EVENT_ID_CBX_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pCBX, 0); break;
		case EVENT_ID_CCX_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pCCX, 0); break;
		case EVENT_ID_CDX_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pCDX, 0); break;
		case EVENT_ID_CSP_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pCSP, 0); break;
		case EVENT_ID_CBP_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pCBP, 0); break;
		case EVENT_ID_CSI_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pCSI, 0); break;
		case EVENT_ID_CDI_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pCDI, 0); break;
	#ifdef _X64_
		case EVENT_ID_R8_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pR8, 0); break;
		case EVENT_ID_R9_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pR9, 0); break;
		case EVENT_ID_R10_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pR10, 0); break;
		case EVENT_ID_R11_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pR11, 0); break;
		case EVENT_ID_R12_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pR12, 0); break;
		case EVENT_ID_R13_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pR13, 0); break;
		case EVENT_ID_R14_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pR14, 0); break;
		case EVENT_ID_R15_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pR15, 0); break;
	#endif
		case EVENT_ID_CIP_DATA: gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(*m_CPUREGPTR.pCIP, 0); break;
		}
		return true;
	}

	bool CGeneRegList::OnEventGotoCode(WISP_MSG *pMsg)
	{
		switch (pMsg->Command.CmdID)
		{
		case EVENT_ID_CAX_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pCAX, true); break;
		case EVENT_ID_CBX_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pCBX, true); break;
		case EVENT_ID_CCX_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pCCX, true); break;
		case EVENT_ID_CDX_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pCDX, true); break;
		case EVENT_ID_CSP_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pCSP, true); break;
		case EVENT_ID_CBP_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pCBP, true); break;
		case EVENT_ID_CSI_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pCSI, true); break;
		case EVENT_ID_CDI_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pCDI, true); break;
	#ifdef _X64_
		case EVENT_ID_R8_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pR8, true); break;
		case EVENT_ID_R9_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pR9, true); break;
		case EVENT_ID_R10_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pR10, true); break;
		case EVENT_ID_R11_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pR11, true); break;
		case EVENT_ID_R12_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pR12, true); break;
		case EVENT_ID_R13_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pR13, true); break;
		case EVENT_ID_R14_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pR14, true); break;
		case EVENT_ID_R15_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pR15, true); break;
	#endif
		case EVENT_ID_CIP_CODE: gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(*m_CPUREGPTR.pCIP, true); break;
		}
		return true;
	}

	bool CGeneRegList::OnEventCode(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FullFindItem(0, 8);
		if (Item)
		{
			CListStringItem::CONTENT *Content = GetItemContent(Item, GL_VALUE);
			ULONG_PTR Addr;
			if (USHexStrToNum(Content->String.operator const WCHAR*(), &Addr))
			{
				gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(Addr, true);
			}
		}
		return true;
	}

	bool CGeneRegList::OnEventData(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FullFindItem(0, 8);
		if (Item)
		{
			CListStringItem::CONTENT *Content = GetItemContent(Item, GL_VALUE);
			ULONG_PTR Addr;
			if (USHexStrToNum(Content->String.operator const WCHAR*(), &Addr))
			{
				gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(Addr, 0);
			}
		}
		return true;
	}

	void CGeneRegList::OnLButton(CListStringItem *Item, int nCol)
	{
		if (nCol == GL_REG)
		{
			CListStringItem::CONTENT *Content = GetItemContent(Item, GL_VALUE);
			ULONG_PTR Addr;
			if (USHexStrToNum(Content->String.operator const WCHAR*(), &Addr))
			{
				gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(Addr, 0);
			}
		}
	}

	bool CGeneRegList::Edit(CListStringItem *Item, int nCol, TWideString& String)
	{
		if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen || GetActiveCPU() != m_nCPU)
			return false;

		int Index = GetItemValue(Item, GL_REG);
		if (Index < REG_GENE_AF || Index > REG_GENE_ZF)
			return true;

		bool bValue = false;
		bool bEqual = false;
		switch (Index)
		{
		case REG_GENE_AF:
			m_CPUREGPTR.pEFL->AF = !m_CPUREGPTR.pEFL->AF;
			bValue = m_CPUREGPTR.pEFL->AF;
			bEqual = m_CPUREGPTR.pEFL->AF == m_CPUREG.EFL.AF;
			break;
		case REG_GENE_CF:
			m_CPUREGPTR.pEFL->CF = !m_CPUREGPTR.pEFL->CF;
			bValue = m_CPUREGPTR.pEFL->CF;
			bEqual = m_CPUREGPTR.pEFL->CF == m_CPUREG.EFL.CF;
			break;
		case REG_GENE_DF:
			m_CPUREGPTR.pEFL->DF = !m_CPUREGPTR.pEFL->DF;
			bValue = m_CPUREGPTR.pEFL->DF;
			bEqual = m_CPUREGPTR.pEFL->DF == m_CPUREG.EFL.DF;
			break;
		case REG_GENE_IF:
			//IF flag can't modify from EFLAGS
			return false;
			m_CPUREGPTR.pEFL->IF = !m_CPUREGPTR.pEFL->IF;
			bValue = m_CPUREGPTR.pEFL->IF;
			bEqual = m_CPUREGPTR.pEFL->IF == m_CPUREG.EFL.IF;
			break;
		case REG_GENE_OF:
			m_CPUREGPTR.pEFL->OF = !m_CPUREGPTR.pEFL->OF;
			bValue = m_CPUREGPTR.pEFL->OF;
			bEqual = m_CPUREGPTR.pEFL->OF == m_CPUREG.EFL.OF;
			break;
		case REG_GENE_PF:
			m_CPUREGPTR.pEFL->PF = !m_CPUREGPTR.pEFL->PF;
			bValue = m_CPUREGPTR.pEFL->PF;
			bEqual = m_CPUREGPTR.pEFL->PF == m_CPUREG.EFL.PF;
			break;
		case REG_GENE_SF:
			m_CPUREGPTR.pEFL->SF = !m_CPUREGPTR.pEFL->SF;
			bValue = m_CPUREGPTR.pEFL->SF;
			bEqual = m_CPUREGPTR.pEFL->SF == m_CPUREG.EFL.SF;
			break;
		case REG_GENE_ZF:
			m_CPUREGPTR.pEFL->ZF = !m_CPUREGPTR.pEFL->ZF;
			bValue = m_CPUREGPTR.pEFL->ZF;
			bEqual = m_CPUREGPTR.pEFL->ZF == m_CPUREG.EFL.ZF;
			break;
		}

		SetItemFormat(m_hGeneRegItem[REG_GENE_EFLAG], GL_VALUE, *(unsigned long*)m_CPUREGPTR.pEFL, WSTR("%08X"));
		SetItemColor(m_hGeneRegItem[REG_GENE_EFLAG], GL_VALUE, (*(unsigned long*)m_CPUREGPTR.pEFL == *(unsigned long*)&m_CPUREG.EFL)?m_ColorSame:m_ColorChanges);

		SetItemText(m_hGeneRegItem[Index], GL_VALUE, bValue ? WSTR("1"):WSTR("0"));
		SetItemColor(m_hGeneRegItem[Index], GL_VALUE, bEqual ? m_ColorSame:m_ColorChanges);

		Update(&m_ClientRect);
		gpSyser->m_pDebugger->UpdateRegisters();
		gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
		return false;
	}

	bool CGeneRegList::OnEditing(CListStringItem *Item, int nCol, TWideString& String)
	{
		ULONG_PTR Result;
		if (!gpSyser->m_SyserUI.CalcExp(String.operator const WCHAR*(), &Result))
			return false;

		int Index = GetItemValue(Item, GL_REG);
		if (Index < REG_GENE_CAX || Index >= REG_GENE_REGCOUNT)
			return false;

		ULONG_PTR *pReg = 0;
		switch (Index)
		{
		case REG_GENE_CAX: pReg = m_CPUREGPTR.pCAX; break;
		case REG_GENE_CBX: pReg = m_CPUREGPTR.pCBX; break;
		case REG_GENE_CCX: pReg = m_CPUREGPTR.pCCX; break;
		case REG_GENE_CDX: pReg = m_CPUREGPTR.pCDX; break;
		case REG_GENE_CSI: pReg = m_CPUREGPTR.pCSI; break;
		case REG_GENE_CDI: pReg = m_CPUREGPTR.pCDI; break;
		case REG_GENE_CBP: pReg = m_CPUREGPTR.pCBP; break;
		case REG_GENE_CSP: pReg = m_CPUREGPTR.pCSP; break;
	#ifdef _X64_
		case REG_GENE_R8: pReg = m_CPUREGPTR.pR8; break;
		case REG_GENE_R9: pReg = m_CPUREGPTR.pR9; break;
		case REG_GENE_R10: pReg = m_CPUREGPTR.pR10; break;
		case REG_GENE_R11: pReg = m_CPUREGPTR.pR11; break;
		case REG_GENE_R12: pReg = m_CPUREGPTR.pR12; break;
		case REG_GENE_R13: pReg = m_CPUREGPTR.pR13; break;
		case REG_GENE_R14: pReg = m_CPUREGPTR.pR14; break;
		case REG_GENE_R15: pReg = m_CPUREGPTR.pR15; break;
	#endif
		case REG_GENE_CIP: pReg = m_CPUREGPTR.pCIP; break;
		case REG_GENE_EFLAG: pReg = (ULONG_PTR*)m_CPUREGPTR.pEFL; break;
		default:
			return false;
		}

		//copy IF flag
		if (Index == REG_GENE_EFLAG)
		{
			if (*pReg & 0x200)
				Result |= 0x200;
			else	Result &= ~0x200;
		}

		if (*pReg == Result)
			return false;

		*pReg = Result;

		SetItemColor(m_hGeneRegItem[Index], GL_VALUE, m_ColorChanges);
		if (Index == REG_GENE_EFLAG)
			String.Format(WSTR("%08X"), (unsigned long)*pReg);
		else	String.Format(WSTR("" F0ADDR "X"), *pReg);
		gpSyser->m_pDebugger->UpdateRegisters();
		if (Index == REG_GENE_EFLAG)
		{
			UpdateFlags();
		} else
		if (Index == REG_GENE_CIP)
		{
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		}
		m_pEditItem = m_hGeneRegItem[Index];
		gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
		return true;
	}

	void CGeneRegList::ResetContext()
	{
		if (IsWindow())
		{
			for (int i = 0; i < REG_GENE_REGCOUNT; ++i)
			{
				switch (i)
				{
				case REG_GENE_CAX:
				case REG_GENE_CBX:
				case REG_GENE_CCX:
				case REG_GENE_CDX:
				case REG_GENE_CSI:
				case REG_GENE_CDI:
				case REG_GENE_CBP:
				case REG_GENE_CSP:
			#ifdef _X64_
				case REG_GENE_R8:
				case REG_GENE_R9:
				case REG_GENE_R10:
				case REG_GENE_R11:
				case REG_GENE_R12:
				case REG_GENE_R13:
				case REG_GENE_R14:
				case REG_GENE_R15:
			#endif
				case REG_GENE_CIP:
					SetItemText(m_hGeneRegItem[i], GL_VALUE, WSTR("" UNREADSTR ""));
					break;
				case REG_GENE_CS:
				case REG_GENE_DS:
				case REG_GENE_ES:
				case REG_GENE_FS:
				case REG_GENE_GS:
				case REG_GENE_SS:
					SetItemText(m_hGeneRegItem[i], GL_VALUE, WSTR("????"));
					break;
				case REG_GENE_AF:
				case REG_GENE_CF:
				case REG_GENE_DF:
				case REG_GENE_IF:
				case REG_GENE_OF:
				case REG_GENE_PF:
				case REG_GENE_SF:
				case REG_GENE_ZF:
					SetItemText(m_hGeneRegItem[i], GL_VALUE, WSTR("?"));
					break;
				case REG_GENE_EFLAG:
				case REG_GENE_BASE_CS:
				case REG_GENE_BASE_DS:
				case REG_GENE_BASE_ES:
				case REG_GENE_BASE_FS:
				case REG_GENE_BASE_GS:
				case REG_GENE_BASE_SS:
					SetItemText(m_hGeneRegItem[i], GL_VALUE, WSTR("????????"));
					break;
				}
				SetItemText(m_hGeneRegItem[i], GL_CONTEXT, (WCHAR*)0);
				SetItemColor(m_hGeneRegItem[i], GL_VALUE, m_ColorSame);

			}
			Select(m_hGeneRegItem[REG_GENE_CAX]);
			memset(&m_CPUREG, 0, sizeof(m_CPUREG));
		}
	}

	void CGeneRegList::UpdateFlags()
	{
		SetItemText(m_hGeneRegItem[REG_GENE_AF], GL_VALUE, m_CPUREGPTR.pEFL->AF ? WSTR("1"):WSTR("0"));
		SetItemColor(m_hGeneRegItem[REG_GENE_AF], GL_VALUE, (m_CPUREG.EFL.AF ^ m_CPUREGPTR.pEFL->AF) ? m_ColorChanges:m_ColorSame);

		SetItemText(m_hGeneRegItem[REG_GENE_CF], GL_VALUE, m_CPUREGPTR.pEFL->CF ? WSTR("1"):WSTR("0"));
		SetItemColor(m_hGeneRegItem[REG_GENE_CF], GL_VALUE, (m_CPUREG.EFL.CF ^ m_CPUREGPTR.pEFL->CF) ? m_ColorChanges:m_ColorSame);

		SetItemText(m_hGeneRegItem[REG_GENE_DF], GL_VALUE, m_CPUREGPTR.pEFL->DF ? WSTR("1"):WSTR("0"));
		SetItemColor(m_hGeneRegItem[REG_GENE_DF], GL_VALUE, (m_CPUREG.EFL.DF ^ m_CPUREGPTR.pEFL->DF) ? m_ColorChanges:m_ColorSame);

		SetItemText(m_hGeneRegItem[REG_GENE_IF], GL_VALUE, m_CPUREGPTR.pEFL->IF ? WSTR("1"):WSTR("0"));
		SetItemColor(m_hGeneRegItem[REG_GENE_IF], GL_VALUE, (m_CPUREG.EFL.IF ^ m_CPUREGPTR.pEFL->IF) ? m_ColorChanges:m_ColorSame);

		SetItemText(m_hGeneRegItem[REG_GENE_OF], GL_VALUE, m_CPUREGPTR.pEFL->OF ? WSTR("1"):WSTR("0"));
		SetItemColor(m_hGeneRegItem[REG_GENE_OF], GL_VALUE, (m_CPUREG.EFL.OF ^ m_CPUREGPTR.pEFL->OF) ? m_ColorChanges:m_ColorSame);

		SetItemText(m_hGeneRegItem[REG_GENE_PF], GL_VALUE, m_CPUREGPTR.pEFL->PF ? WSTR("1"):WSTR("0"));
		SetItemColor(m_hGeneRegItem[REG_GENE_PF], GL_VALUE, (m_CPUREG.EFL.PF ^ m_CPUREGPTR.pEFL->PF) ? m_ColorChanges:m_ColorSame);

		SetItemText(m_hGeneRegItem[REG_GENE_SF], GL_VALUE, m_CPUREGPTR.pEFL->SF ? WSTR("1"):WSTR("0"));
		SetItemColor(m_hGeneRegItem[REG_GENE_SF], GL_VALUE, (m_CPUREG.EFL.SF ^ m_CPUREGPTR.pEFL->SF) ? m_ColorChanges:m_ColorSame);

		SetItemText(m_hGeneRegItem[REG_GENE_ZF], GL_VALUE, m_CPUREGPTR.pEFL->ZF ? WSTR("1"):WSTR("0"));
		SetItemColor(m_hGeneRegItem[REG_GENE_ZF], GL_VALUE, (m_CPUREG.EFL.ZF ^ m_CPUREGPTR.pEFL->ZF) ? m_ColorChanges:m_ColorSame);
	}

	void CGeneRegList::ReadString(X86_CPU_REG_PTR *pCPURegPtr)
	{
		WCHAR Buffer[256];
		char szSym[256];
		char Encode;

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pCAX, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pCAX, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pCAX, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CAX], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pCBX, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pCBX, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pCBX, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CBX], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pCCX, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pCCX, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pCCX, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CCX], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pCDX, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pCDX, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pCDX, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CDX], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pCSI, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pCSI, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pCSI, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CSI], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pCDI, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pCDI, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pCDI, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CDI], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pCBP, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pCBP, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pCBP, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CBP], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		//if (*m_CPUREGPTR.pESP == 0x7FFB8)
                //	Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pCSP, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pCSP, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pCSP, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CSP], GL_CONTEXT, Buffer);
	#ifdef _X64_
		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pR8, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pR8, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pR8, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R8], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pR9, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pR9, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pR9, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R9], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pR10, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pR10, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pR10, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R10], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pR11, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pR11, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pR11, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R11], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pR12, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pR12, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pR12, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R12], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pR13, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pR13, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pR13, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R13], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pR14, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pR14, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pR14, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R14], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(*pCPURegPtr->pR15, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(*pCPURegPtr->pR15, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pR15, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R15], GL_CONTEXT, Buffer);
	#endif
		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbolOffset(*pCPURegPtr->pCIP, szSym))
			TStrCpy(Buffer, szSym);
		SetItemText(m_hGeneRegItem[REG_GENE_CIP], GL_CONTEXT, Buffer);
	}

	void CGeneRegList::UpdateContext()
	{
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && IsWindow())
		{
			if (GetActiveCPU() == m_nCPU)
				memcpy(&m_CPUREGPTR, &gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR, sizeof(m_CPUREGPTR));
			else	gpSyser->m_pDebugger->GetX86RegPtr(&m_CPUREGPTR, m_nCPU);

			SetItemFormat(m_hGeneRegItem[REG_GENE_CAX], GL_VALUE, *m_CPUREGPTR.pCAX, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_CAX], GL_VALUE, *m_CPUREGPTR.pCAX);
			SetItemColor(m_hGeneRegItem[REG_GENE_CAX], GL_VALUE, (*m_CPUREGPTR.pCAX == m_CPUREG.CAX)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_CBX], GL_VALUE, *m_CPUREGPTR.pCBX, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_CBX], GL_VALUE, *m_CPUREGPTR.pCBX);
			SetItemColor(m_hGeneRegItem[REG_GENE_CBX], GL_VALUE, (*m_CPUREGPTR.pCBX == m_CPUREG.CBX)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_CCX], GL_VALUE, *m_CPUREGPTR.pCCX, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_CCX], GL_VALUE, *m_CPUREGPTR.pCCX);
			SetItemColor(m_hGeneRegItem[REG_GENE_CCX], GL_VALUE, (*m_CPUREGPTR.pCCX == m_CPUREG.CCX)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_CDX], GL_VALUE, *m_CPUREGPTR.pCDX, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_CDX], GL_VALUE, *m_CPUREGPTR.pCDX);
			SetItemColor(m_hGeneRegItem[REG_GENE_CDX], GL_VALUE, (*m_CPUREGPTR.pCDX == m_CPUREG.CDX)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_CSI], GL_VALUE, *m_CPUREGPTR.pCSI, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_CSI], GL_VALUE, *m_CPUREGPTR.pCSI);
			SetItemColor(m_hGeneRegItem[REG_GENE_CSI], GL_VALUE, (*m_CPUREGPTR.pCSI == m_CPUREG.CSI)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_CDI], GL_VALUE, *m_CPUREGPTR.pCDI, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_CDI], GL_VALUE, *m_CPUREGPTR.pCDI);
			SetItemColor(m_hGeneRegItem[REG_GENE_CDI], GL_VALUE, (*m_CPUREGPTR.pCDI == m_CPUREG.CDI)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_CBP], GL_VALUE, *m_CPUREGPTR.pCBP, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_CBP], GL_VALUE, *m_CPUREGPTR.pCBP);
			SetItemColor(m_hGeneRegItem[REG_GENE_CBP], GL_VALUE, (*m_CPUREGPTR.pCBP == m_CPUREG.CBP)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_CSP], GL_VALUE, *m_CPUREGPTR.pCSP, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_CSP], GL_VALUE, *m_CPUREGPTR.pCSP);
			SetItemColor(m_hGeneRegItem[REG_GENE_CSP], GL_VALUE, (*m_CPUREGPTR.pCSP == m_CPUREG.CSP)?m_ColorSame:m_ColorChanges);
		#ifdef _X64_
			SetItemFormat(m_hGeneRegItem[REG_GENE_R8], GL_VALUE, *m_CPUREGPTR.pR8, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_R8], GL_VALUE, *m_CPUREGPTR.pR8);
			SetItemColor(m_hGeneRegItem[REG_GENE_R8], GL_VALUE, (*m_CPUREGPTR.pR8 == m_CPUREG.R8)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_R9], GL_VALUE, *m_CPUREGPTR.pR9, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_R9], GL_VALUE, *m_CPUREGPTR.pR9);
			SetItemColor(m_hGeneRegItem[REG_GENE_R9], GL_VALUE, (*m_CPUREGPTR.pR9 == m_CPUREG.R9)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_R10], GL_VALUE, *m_CPUREGPTR.pR10, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_R10], GL_VALUE, *m_CPUREGPTR.pR10);
			SetItemColor(m_hGeneRegItem[REG_GENE_R10], GL_VALUE, (*m_CPUREGPTR.pR10 == m_CPUREG.R10)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_R11], GL_VALUE, *m_CPUREGPTR.pR11, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_R11], GL_VALUE, *m_CPUREGPTR.pR11);
			SetItemColor(m_hGeneRegItem[REG_GENE_R11], GL_VALUE, (*m_CPUREGPTR.pR11 == m_CPUREG.R11)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_R12], GL_VALUE, *m_CPUREGPTR.pR12, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_R12], GL_VALUE, *m_CPUREGPTR.pR12);
			SetItemColor(m_hGeneRegItem[REG_GENE_R12], GL_VALUE, (*m_CPUREGPTR.pR12 == m_CPUREG.R12)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_R13], GL_VALUE, *m_CPUREGPTR.pR13, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_R13], GL_VALUE, *m_CPUREGPTR.pR13);
			SetItemColor(m_hGeneRegItem[REG_GENE_R13], GL_VALUE, (*m_CPUREGPTR.pR13 == m_CPUREG.R13)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_R14], GL_VALUE, *m_CPUREGPTR.pR14, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_R14], GL_VALUE, *m_CPUREGPTR.pR14);
			SetItemColor(m_hGeneRegItem[REG_GENE_R14], GL_VALUE, (*m_CPUREGPTR.pR14 == m_CPUREG.R14)?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hGeneRegItem[REG_GENE_R15], GL_VALUE, *m_CPUREGPTR.pR15, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_R15], GL_VALUE, *m_CPUREGPTR.pR15);
			SetItemColor(m_hGeneRegItem[REG_GENE_R15], GL_VALUE, (*m_CPUREGPTR.pR15 == m_CPUREG.R15)?m_ColorSame:m_ColorChanges);
		#endif
			SetItemFormat(m_hGeneRegItem[REG_GENE_CIP], GL_VALUE, *m_CPUREGPTR.pCIP, WSTR("" F0ADDR "X"));
			SetItemValue(m_hGeneRegItem[REG_GENE_CIP], GL_VALUE, *m_CPUREGPTR.pCIP);
			SetItemColor(m_hGeneRegItem[REG_GENE_CIP], GL_VALUE, (*m_CPUREGPTR.pCIP == m_CPUREG.CIP)?m_ColorSame:m_ColorChanges);

			ReadString(&m_CPUREGPTR);

			SetItemFormat(m_hGeneRegItem[REG_GENE_EFLAG], GL_VALUE, *(unsigned long*)m_CPUREGPTR.pEFL, WSTR("%08X"));
			if (*(unsigned long*)m_CPUREGPTR.pEFL == *(unsigned long*)&m_CPUREG.EFL)
				SetItemColor(m_hGeneRegItem[REG_GENE_EFLAG], GL_VALUE, m_ColorSame);
			else	SetItemColor(m_hGeneRegItem[REG_GENE_EFLAG], GL_VALUE, m_ColorChanges);

			ULONG_PTR Seg;

			SetItemFormat(m_hGeneRegItem[REG_GENE_CS], GL_VALUE, *m_CPUREGPTR.pCS, WSTR("%04X"));
			if (*m_CPUREGPTR.pCS == m_CPUREG.CS)
			{
				SetItemColor(m_hGeneRegItem[REG_GENE_CS], GL_VALUE, m_ColorSame);
			} else
			{
				SetItemColor(m_hGeneRegItem[REG_GENE_CS], GL_VALUE, m_ColorChanges);
				if (gpSyser->m_pDebugger->GetSegRegBase(*m_CPUREGPTR.pCS, &Seg, 0))
					SetItemFormat(m_hGeneRegItem[REG_GENE_BASE_CS], GL_VALUE, Seg, WSTR("%08X"));
				else	SetItemText(m_hGeneRegItem[REG_GENE_BASE_CS], GL_VALUE, WSTR("????????"));
			}

			SetItemFormat(m_hGeneRegItem[REG_GENE_DS], GL_VALUE, *m_CPUREGPTR.pDS, WSTR("%04X"));
			if (*m_CPUREGPTR.pDS == m_CPUREG.DS)
			{
				SetItemColor(m_hGeneRegItem[REG_GENE_DS], GL_VALUE, m_ColorSame);
			} else
			{
				SetItemColor(m_hGeneRegItem[REG_GENE_DS], GL_VALUE, m_ColorChanges);
				if (gpSyser->m_pDebugger->GetSegRegBase(*m_CPUREGPTR.pDS, &Seg, 0))
					SetItemFormat(m_hGeneRegItem[REG_GENE_BASE_DS], GL_VALUE, Seg, WSTR("%08X"));
				else	SetItemText(m_hGeneRegItem[REG_GENE_BASE_DS], GL_VALUE, WSTR("????????"));
			}

			SetItemFormat(m_hGeneRegItem[REG_GENE_ES], GL_VALUE, *m_CPUREGPTR.pES, WSTR("%04X"));
			if (*m_CPUREGPTR.pES == m_CPUREG.ES)
			{
				SetItemColor(m_hGeneRegItem[REG_GENE_ES], GL_VALUE, m_ColorSame);
			} else
			{
				SetItemColor(m_hGeneRegItem[REG_GENE_ES], GL_VALUE, m_ColorChanges);
				if (gpSyser->m_pDebugger->GetSegRegBase(*m_CPUREGPTR.pES, &Seg, 0))
				{
					SetItemFormat(m_hGeneRegItem[REG_GENE_BASE_ES], GL_VALUE, Seg, WSTR("%08X"));
					SetItemValue(m_hGeneRegItem[REG_GENE_BASE_ES], GL_VALUE, Seg);
				} else
				{
					SetItemText(m_hGeneRegItem[REG_GENE_BASE_ES], GL_VALUE, WSTR("????????"));
				}
			}

			SetItemFormat(m_hGeneRegItem[REG_GENE_FS], GL_VALUE, *m_CPUREGPTR.pFS, WSTR("%04X"));
			if (*m_CPUREGPTR.pFS == m_CPUREG.FS)
			{
				SetItemColor(m_hGeneRegItem[REG_GENE_FS], GL_VALUE, m_ColorSame);
			} else
			{
				SetItemColor(m_hGeneRegItem[REG_GENE_FS], GL_VALUE, m_ColorChanges);
				if (gpSyser->m_pDebugger->GetSegRegBase(*m_CPUREGPTR.pFS, &Seg, 0))
				{
					SetItemValue(m_hGeneRegItem[REG_GENE_BASE_FS], GL_VALUE, Seg);
					SetItemFormat(m_hGeneRegItem[REG_GENE_BASE_FS], GL_VALUE, Seg, WSTR("%08X"));
				} else
				{
					SetItemText(m_hGeneRegItem[REG_GENE_BASE_FS], GL_VALUE, WSTR("????????"));
				}
			}

			SetItemFormat(m_hGeneRegItem[REG_GENE_GS], GL_VALUE, *m_CPUREGPTR.pGS, WSTR("%04X"));
			if (*m_CPUREGPTR.pGS == m_CPUREG.GS)
			{
				SetItemColor(m_hGeneRegItem[REG_GENE_GS], GL_VALUE, m_ColorSame);
			} else
			{
				SetItemColor(m_hGeneRegItem[REG_GENE_GS], GL_VALUE, m_ColorChanges);
				if (gpSyser->m_pDebugger->GetSegRegBase(*m_CPUREGPTR.pGS, &Seg, 0))
				{
					SetItemFormat(m_hGeneRegItem[REG_GENE_BASE_GS], GL_VALUE, Seg, WSTR("%08X"));
					SetItemValue(m_hGeneRegItem[REG_GENE_BASE_GS], GL_VALUE, Seg);
				} else
				{
					SetItemText(m_hGeneRegItem[REG_GENE_BASE_GS], GL_VALUE, WSTR("????????"));
				}
			}

			SetItemFormat(m_hGeneRegItem[REG_GENE_SS], GL_VALUE, *m_CPUREGPTR.pSS, WSTR("%04X"));
			if (*m_CPUREGPTR.pSS == m_CPUREG.SS)
			{
				SetItemColor(m_hGeneRegItem[REG_GENE_SS], GL_VALUE, m_ColorSame);
			} else
			{
				SetItemColor(m_hGeneRegItem[REG_GENE_SS], GL_VALUE, m_ColorChanges);
				if (gpSyser->m_pDebugger->GetSegRegBase(*m_CPUREGPTR.pSS, &Seg, 0))
				{
					SetItemFormat(m_hGeneRegItem[REG_GENE_BASE_SS], GL_VALUE, Seg, WSTR("%08X"));
					SetItemValue(m_hGeneRegItem[REG_GENE_BASE_SS], GL_VALUE, Seg);
				} else
				{
					SetItemText(m_hGeneRegItem[REG_GENE_BASE_SS], GL_VALUE, WSTR("????????"));
				}
			}

		#ifdef _RING0_
			SetItemFormat(m_hGeneRegItem[REG_GENE_GDTR], GL_VALUE, *m_CPUREGPTR.pGDT, WSTR("%08X"));
			SetItemFormat(m_hGeneRegItem[REG_GENE_BASE_GDTR], GL_VALUE, *m_CPUREGPTR.pGDT, WSTR("%08X"));
			SetItemFormat(m_hGeneRegItem[REG_GENE_LIMIT_GDTR], GL_VALUE, *m_CPUREGPTR.pGDTLimit, WSTR("%04X"));

			SetItemFormat(m_hGeneRegItem[REG_GENE_IDTR], GL_VALUE, *m_CPUREGPTR.pIDT, WSTR("%08X"));
			SetItemFormat(m_hGeneRegItem[REG_GENE_BASE_IDTR], GL_VALUE, *m_CPUREGPTR.pIDT, WSTR("%08X"));
			SetItemFormat(m_hGeneRegItem[REG_GENE_LIMIT_IDTR], GL_VALUE, *m_CPUREGPTR.pIDTLimit, WSTR("%04X"));

			SetItemFormat(m_hGeneRegItem[REG_GENE_LDTR], GL_VALUE, *m_CPUREGPTR.pLDT, WSTR("%04X"));
			SetItemFormat(m_hGeneRegItem[REG_GENE_TR], GL_VALUE, *m_CPUREGPTR.pTSS, WSTR("%04X"));
		#endif
			UpdateFlags();

			m_CPUREG.CAX = *m_CPUREGPTR.pCAX;
			m_CPUREG.CBX = *m_CPUREGPTR.pCBX;
			m_CPUREG.CCX = *m_CPUREGPTR.pCCX;
			m_CPUREG.CDX = *m_CPUREGPTR.pCDX;
			m_CPUREG.CDI = *m_CPUREGPTR.pCDI;
			m_CPUREG.CSI = *m_CPUREGPTR.pCSI;
			m_CPUREG.CBP = *m_CPUREGPTR.pCBP;
			m_CPUREG.CSP = *m_CPUREGPTR.pCSP;
		#ifdef _X64_
			m_CPUREG.R8 = *m_CPUREGPTR.pR8;
			m_CPUREG.R9 = *m_CPUREGPTR.pR9;
			m_CPUREG.R10 = *m_CPUREGPTR.pR10;
			m_CPUREG.R11 = *m_CPUREGPTR.pR11;
			m_CPUREG.R12 = *m_CPUREGPTR.pR12;
			m_CPUREG.R13 = *m_CPUREGPTR.pR13;
			m_CPUREG.R14 = *m_CPUREGPTR.pR14;
			m_CPUREG.R15 = *m_CPUREGPTR.pR15;
		#endif
			m_CPUREG.CIP = *m_CPUREGPTR.pCIP;

			*(unsigned long*)&m_CPUREG.EFL = *(unsigned long*)m_CPUREGPTR.pEFL;

			m_CPUREG.CS = *m_CPUREGPTR.pCS;
			m_CPUREG.DS = *m_CPUREGPTR.pDS;
			m_CPUREG.ES = *m_CPUREGPTR.pES;
			m_CPUREG.FS = *m_CPUREGPTR.pFS;
			m_CPUREG.GS = *m_CPUREGPTR.pGS;
			m_CPUREG.SS = *m_CPUREGPTR.pSS;

			Update(&m_ClientRect);
		}
	}
	void CGeneRegList::LoadKeyMap()
	{
		if (!gpSyser->GetHotKey(EVENT_ID_CAX_DATA, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x10000031, EVENT_ID_CAX_DATA)); //WISP_SK_SHIFT 1
		if (!gpSyser->GetHotKey(EVENT_ID_CBX_DATA, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x10000032, EVENT_ID_CBX_DATA)); //WISP_SK_SHIFT 2
		if (!gpSyser->GetHotKey(EVENT_ID_CCX_DATA, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x10000033, EVENT_ID_CCX_DATA)); //WISP_SK_SHIFT 3
		if (!gpSyser->GetHotKey(EVENT_ID_CDX_DATA, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x10000034, EVENT_ID_CDX_DATA)); //WISP_SK_SHIFT 4
		if (!gpSyser->GetHotKey(EVENT_ID_CSP_DATA, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x10000035, EVENT_ID_CSP_DATA)); //WISP_SK_SHIFT 5
		if (!gpSyser->GetHotKey(EVENT_ID_CBP_DATA, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x10000036, EVENT_ID_CBP_DATA)); //WISP_SK_SHIFT 6
		if (!gpSyser->GetHotKey(EVENT_ID_CSI_DATA, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x10000037, EVENT_ID_CSI_DATA)); //WISP_SK_SHIFT 7
		if (!gpSyser->GetHotKey(EVENT_ID_CDI_DATA, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x10000038, EVENT_ID_CDI_DATA)); //WISP_SK_SHIFT 8
		if (!gpSyser->GetHotKey(EVENT_ID_CIP_DATA, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x10000039, EVENT_ID_CIP_DATA)); //WISP_SK_SHIFT 9
	#ifdef _X64_
	#endif
		if (!gpSyser->GetHotKey(EVENT_ID_CAX_CODE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000031, EVENT_ID_CAX_CODE)); //WISP_SK_ALT 1
		if (!gpSyser->GetHotKey(EVENT_ID_CBX_CODE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000032, EVENT_ID_CBX_CODE)); //WISP_SK_ALT 2
		if (!gpSyser->GetHotKey(EVENT_ID_CCX_CODE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000033, EVENT_ID_CCX_CODE)); //WISP_SK_ALT 3
		if (!gpSyser->GetHotKey(EVENT_ID_CDX_CODE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000034, EVENT_ID_CDX_CODE)); //WISP_SK_ALT 4
		if (!gpSyser->GetHotKey(EVENT_ID_CSP_CODE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000035, EVENT_ID_CSP_CODE)); //WISP_SK_ALT 5
		if (!gpSyser->GetHotKey(EVENT_ID_CBP_CODE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000036, EVENT_ID_CBP_CODE)); //WISP_SK_ALT 6
		if (!gpSyser->GetHotKey(EVENT_ID_CSI_CODE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000037, EVENT_ID_CSI_CODE)); //WISP_SK_ALT 7
		if (!gpSyser->GetHotKey(EVENT_ID_CDI_CODE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000038, EVENT_ID_CDI_CODE)); //WISP_SK_ALT 8
		if (!gpSyser->GetHotKey(EVENT_ID_CIP_CODE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000039, EVENT_ID_CIP_CODE)); //WISP_SK_ALT 9
	#ifdef _X64_
	#endif
	}

	void CGeneRegList::ReadStringTrace(X86_CPU_REG *pCPUReg)
	{
		WCHAR Buffer[256];
		char szSym[256];
		char Encode;

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->CAX, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->CAX, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->CAX, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CAX], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->CCX, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->CCX, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->CCX, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CCX], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->CDX, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->CDX, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->CDX, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CDX], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->CBX, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->CBX, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->CBX, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CBX], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->CSP, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->CSP, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->CSP, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CSP], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->CBP, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->CBP, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->CBP, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CBP], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->CSI, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->CSI, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->CSI, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CSI], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->CDI, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->CDI, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->CDI, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_CDI], GL_CONTEXT, Buffer);
	#ifdef _X64_
		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->R8, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->R8, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->R8, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R8], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->R9, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->R9, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->R9, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R9], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->R10, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->R10, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->R10, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R10], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->R11, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->R11, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->R11, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R11], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->R12, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->R12, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->R12, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R12], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->R13, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->R13, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->R13, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R13], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->R14, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->R14, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->R14, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R14], GL_CONTEXT, Buffer);

		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbol(pCPUReg->R15, szSym, lenof(szSym)) ||
			(!gpSyser->m_SyserUI.ReadString(pCPUReg->R15, Buffer, 256, &Encode) &&
			gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->R15, szSym)))
		{
			TStrCpy(Buffer, szSym);
		}
		SetItemText(m_hGeneRegItem[REG_GENE_R15], GL_CONTEXT, Buffer);
	#endif
		Buffer[0] = 0;
		if (gpSyser->m_SyserUI.GetSymbolOffset(pCPUReg->CIP, szSym))
			TStrCpy(Buffer, szSym);
		SetItemText(m_hGeneRegItem[REG_GENE_CIP], GL_CONTEXT, Buffer);
	}

	void CGeneRegList::UpdateContextTrace(unsigned long CPUType, X86_CPU_REG *pCPUReg, ULONG_PTR CIP)
	{
		SetItemFormat(m_hGeneRegItem[REG_GENE_CIP], GL_VALUE, CIP, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_CIP], GL_VALUE, CIP);
		SetItemColor(m_hGeneRegItem[REG_GENE_CIP], GL_VALUE, (CPUType & 0x20000)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_CAX], GL_VALUE, pCPUReg->CAX, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_CAX], GL_VALUE, pCPUReg->CAX);
		SetItemColor(m_hGeneRegItem[REG_GENE_CAX], GL_VALUE, (CPUType & 1)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_CBX], GL_VALUE, pCPUReg->CBX, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_CBX], GL_VALUE, pCPUReg->CBX);
		SetItemColor(m_hGeneRegItem[REG_GENE_CBX], GL_VALUE, (CPUType & 2)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_CCX], GL_VALUE, pCPUReg->CCX, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_CCX], GL_VALUE, pCPUReg->CCX);
		SetItemColor(m_hGeneRegItem[REG_GENE_CCX], GL_VALUE, (CPUType & 4)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_CDX], GL_VALUE, pCPUReg->CDX, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_CDX], GL_VALUE, pCPUReg->CDX);
		SetItemColor(m_hGeneRegItem[REG_GENE_CDX], GL_VALUE, (CPUType & 8)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_CSP], GL_VALUE, pCPUReg->CSP, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_CSP], GL_VALUE, pCPUReg->CSP);
		SetItemColor(m_hGeneRegItem[REG_GENE_CSP], GL_VALUE, (CPUType & 0x10)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_CBP], GL_VALUE, pCPUReg->CBP, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_CBP], GL_VALUE, pCPUReg->CBP);
		SetItemColor(m_hGeneRegItem[REG_GENE_CBP], GL_VALUE, (CPUType & 0x20)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_CSI], GL_VALUE, pCPUReg->CSI, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_CSI], GL_VALUE, pCPUReg->CSI);
		SetItemColor(m_hGeneRegItem[REG_GENE_CSI], GL_VALUE, (CPUType & 0x40)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_CDI], GL_VALUE, pCPUReg->CDI, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_CDI], GL_VALUE, pCPUReg->CDI);
		SetItemColor(m_hGeneRegItem[REG_GENE_CDI], GL_VALUE, (CPUType & 0x80)?m_ColorChanges:m_ColorSame);
	#ifdef _X64_
		SetItemFormat(m_hGeneRegItem[REG_GENE_R8], GL_VALUE, pCPUReg->R8, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_R8], GL_VALUE, pCPUReg->R8);
		SetItemColor(m_hGeneRegItem[REG_GENE_R8], GL_VALUE, (CPUType & 0x100)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_R9], GL_VALUE, pCPUReg->R9, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_R9], GL_VALUE, pCPUReg->R9);
		SetItemColor(m_hGeneRegItem[REG_GENE_R9], GL_VALUE, (CPUType & 0x200)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_R10], GL_VALUE, pCPUReg->R10, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_R10], GL_VALUE, pCPUReg->R10);
		SetItemColor(m_hGeneRegItem[REG_GENE_R10], GL_VALUE, (CPUType & 0x400)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_R11], GL_VALUE, pCPUReg->R11, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_R11], GL_VALUE, pCPUReg->R11);
		SetItemColor(m_hGeneRegItem[REG_GENE_R11], GL_VALUE, (CPUType & 0x800)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_R12], GL_VALUE, pCPUReg->R12, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_R12], GL_VALUE, pCPUReg->R12);
		SetItemColor(m_hGeneRegItem[REG_GENE_R12], GL_VALUE, (CPUType & 0x1000)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_R13], GL_VALUE, pCPUReg->R13, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_R13], GL_VALUE, pCPUReg->R13);
		SetItemColor(m_hGeneRegItem[REG_GENE_R13], GL_VALUE, (CPUType & 0x2000)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_R14], GL_VALUE, pCPUReg->R14, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_R14], GL_VALUE, pCPUReg->R14);
		SetItemColor(m_hGeneRegItem[REG_GENE_R14], GL_VALUE, (CPUType & 0x4000)?m_ColorChanges:m_ColorSame);

		SetItemFormat(m_hGeneRegItem[REG_GENE_R15], GL_VALUE, pCPUReg->R15, WSTR("" F0ADDR "X"));
		SetItemValue(m_hGeneRegItem[REG_GENE_R15], GL_VALUE, pCPUReg->R15);
		SetItemColor(m_hGeneRegItem[REG_GENE_R15], GL_VALUE, (CPUType & 0x8000)?m_ColorChanges:m_ColorSame);
	#endif
		ReadStringTrace(pCPUReg);
	}

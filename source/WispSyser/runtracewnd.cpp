
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "runtracewnd.hpp"
#include "syserconfig.hpp"
#include "codeview.hpp"

enum
{
  RT_BACK = 0,
  RT_ADDR = 1,
  RT_INST = 2,
  RT_REGS = 3,
};

	WISP_MSG_MAP_BEGIN(CRunTraceWnd)
	//	WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_CLOSE, OnClose)
		WISP_MSG_MAP(WISP_WM_MOUSE_DBLCLICK, OnMouseDblClick)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_COMMAND, CWispList::OnCommand)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

	WISP_EVENT_MAP_BEGIN(CRunTraceWnd)
//		WISP_EVENT_MAP(0x, OnEvent)
	WISP_EVENT_MAP_END

	CRunTraceWnd::CRunTraceWnd()
	{
	}
	CRunTraceWnd::~CRunTraceWnd()
	{
	}

	bool CRunTraceWnd::OnUpdate(WISP_MSG *pMsg)
	{
		//TODO
		return true;
	}

	bool CRunTraceWnd::OnCreate(WISP_MSG *pMsg)
	{
		m_ColorFocus = gSyserColors.Color[2];

		m_Color[1] = gSyserColors.Color[6];
		m_Color[2] = gSyserColors.Color[9];
		m_Color[3] = gSyserColors.Color[10];
		m_Color[4] = gSyserColors.Color[11];
		m_Color[5] = gSyserColors.Color[12];
		m_Color[6] = gSyserColors.Color[13];
		m_Color[7] = gSyserColors.Color[14];
		m_Color[8] = gSyserColors.Color[15];
		m_Color[9] = gSyserColors.Color[16];
		m_Color[10] = gSyserColors.Color[17];
		m_Color[11] = gSyserColors.Color[18];
		m_Color[12] = gSyserColors.Color[19];
		m_Color[13] = gSyserColors.Color[20];
		m_Color[14] = gSyserColors.Color[21];
		m_Color[15] = gSyserColors.Color[22];
		m_ClientDC.m_pColor = m_Color;

		AttachTitleDIB(gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0xB5));
		m_ItemColor = gSyserColors.Color[1];

		SetBGColor(gSyserColors.Color[0]);

		int Ext = m_ClientDC.GetTextExtent(WSTR("X"), -1, 0);

		InsertColumn(WSTR("Back"), 40, 0, 0, -1);
		InsertColumn(WSTR("Address"), (3+ADDR_SIZE)*Ext+3, 0, 0, -1); //60
		InsertColumn(WSTR("Instruction"), 180, 0, 0, -1);
		InsertColumn(WSTR("Modified Registers"), 120, 0, 0, -1);

		SetBGColor(0);

		UpdateContext();
		return true;
	}
	bool CRunTraceWnd::OnClose(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.FocusActive();
		return true;
	}

	bool CRunTraceWnd::OnMouseDblClick(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetNext(0, 8);
		if (Item)
		{
			unsigned long Addr = GetItemValue(Item, RT_BACK);
			gpSyser->m_MainFrame.m_SystemExplorer.TracePoint(Addr);
		}
		return true;
	}
	bool CRunTraceWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.bKeyDown && pMsg->KeyEvent.KeyType == VK_RETURN)
			OnMouseDblClick(0);
		return true;
	}
	//bool CRunTraceWnd::OnCommand(WISP_MSG *pMsg)
	//bool CRunTraceWnd::OnEvent(WISP_MSG *pMsg)

	void CRunTraceWnd::UpdateContext()
	{
		if (IsWindow())
		{
			DeleteAllChildrenItems(0);
			CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();

			CListStringItem *Item = 0;
			unsigned long TracePoint = 0;

			WCHAR Buffer[200];
			WCHAR szReg[20];

			unsigned long ColorString[256];

			unsigned long Pos = gpSyser->m_MainFrame.m_SystemExplorer.m_CIPRegWndTrace - 1;
			CTracePoint *pTrace;
			while((pTrace = gpSyser->m_MainFrame.m_SystemExplorer.m_RunTrace.GetTracePoint(TracePoint)))
			{
				ColorString[0] = 0;

				ULONG_PTR Address = pTrace->Address + pTrace->Offset;

				TSPrintf(Buffer, WSTR("%d"), Pos);
				Item = InsertItem(Buffer, 0, 0, 0, -1);
				SetItemColor(Item, RT_BACK, gSyserColors.Color[1]);
				SetItemValue(Item, RT_BACK, TracePoint);
				SetItemFormat(Item, RT_ADDR, Address, WSTR("" F0ADDR "X"));
				SetItemColor(Item, RT_ADDR, gSyserColors.Color[9]);

				if (pWnd)
					pWnd->ColorDasm(Address, 16, ColorString, 0, 0, 0);
				SetItemTextColor(Item, RT_INST, ColorString);
				Buffer[0] = 0;
				X86_CPU_REG CPUReg;
				unsigned long CPUType;
				gpSyser->m_MainFrame.m_SystemExplorer.m_RunTrace.ReadContext(TracePoint, &CPUReg, &CPUType);
				if (CPUType)
				{
					if (CPUType & 1)
					{
						TSPrintf(szReg, WSTR("EAX=" F0ADDR "X "), CPUReg.CAX);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 2)
					{
						TSPrintf(szReg, WSTR("EBX=" F0ADDR "X "), CPUReg.CBX);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 4)
					{
						TSPrintf(szReg, WSTR("ECX=" F0ADDR "X "), CPUReg.CCX);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 8)
					{
						TSPrintf(szReg, WSTR("EDX=" F0ADDR "X "), CPUReg.CDX);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 0x40)
					{
						TSPrintf(szReg, WSTR("ESI=" F0ADDR "X "), CPUReg.CSI);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 0x80)
					{
						TSPrintf(szReg, WSTR("EDI=" F0ADDR "X "), CPUReg.CDI);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 0x10)
					{
						TSPrintf(szReg, WSTR("ESP=" F0ADDR "X "), CPUReg.CSP);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 0x20)
					{
						TSPrintf(szReg, WSTR("EBP=" F0ADDR "X "), CPUReg.CBP);
						TStrCat(Buffer, szReg);
					}
				#ifdef _X64_
					if (CPUType & 0x100)
					{
						TSPrintf(szReg, WSTR("R8=" F0ADDR "X "), CPUReg.R8);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 0x200)
					{
						TSPrintf(szReg, WSTR("R9=" F0ADDR "X "), CPUReg.R9);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 0x400)
					{
						TSPrintf(szReg, WSTR("R10=" F0ADDR "X "), CPUReg.R10);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 0x800)
					{
						TSPrintf(szReg, WSTR("R11=" F0ADDR "X "), CPUReg.R11);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 0x1000)
					{
						TSPrintf(szReg, WSTR("R12=" F0ADDR "X "), CPUReg.R12);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 0x2000)
					{
						TSPrintf(szReg, WSTR("R13=" F0ADDR "X "), CPUReg.R13);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 0x4000)
					{
						TSPrintf(szReg, WSTR("R14=" F0ADDR "X "), CPUReg.R14);
						TStrCat(Buffer, szReg);
					}
					if (CPUType & 0x8000)
					{
						TSPrintf(szReg, WSTR("R15=" F0ADDR "X "), CPUReg.R15);
						TStrCat(Buffer, szReg);
					}
				#endif
				}
				SetItemText(Item, RT_REGS, Buffer);
				SetItemColor(Item, RT_REGS, gSyserColors.Color[4]);
				--Pos;
				++TracePoint;
			}

			if (Item)
			{
				if (!CheckItemPage(Item))
					Scroll(Item, -1);
			}
		}
	}

	void CRunTraceWnd::Create()
	{
		if (IsWindow())
		{
			Destroy();
		} else
		{
			CreateWnd(WSTR("Run Trace List"), 0, 0, 500, 280, 0, 0xB, 0x10100BB, WISP_SH_MINSIZE);
			Center(0);
		}
	}

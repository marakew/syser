
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "sehchainwnd.hpp"
#include "syserconfig.hpp"

	CSEHChainWnd::CSEHChainWnd()
	{
	}
	CSEHChainWnd::~CSEHChainWnd()
	{
	}

	WISP_MSG_MAP_BEGIN(CSEHChainWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_CLOSE, OnClose)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)


	WISP_EVENT_MAP_BEGIN(CSEHChainWnd)
//		WISP_EVENT_MAP(..., OnEvent...)
	WISP_EVENT_MAP_END


	bool CSEHChainWnd::OnCreate(WISP_MSG *pMsg)
	{
		InsertColumn(WSTR("Address"), 70, 0, 0, -1);
		InsertColumn(WSTR("SE Handler"), 200, 0, 0, -1);
		UpdateContext();
		return true;
	}
	bool CSEHChainWnd::OnClose(WISP_MSG *pMsg)
	{
		Show(WISP_SH_HIDDEN);
		return false;
	}

	void CSEHChainWnd::Create()
	{
		if (IsWindow())
		{
			Show(WISP_SH_MINSIZE);
		} else
		{
			CreateWnd(WSTR("SEH Chain"), 0,0,620,280, 0, 0x10, 0x100BB, WISP_SH_MINSIZE);
			Center(0);
		}
	}

	void CSEHChainWnd::UpdateContext()
	{
		WCHAR display[20];

		X86_CPU_REG_PTR CpuReg;
		gpSyser->m_pDebugger->GetX86RegPtr(&CpuReg);

		ULONG_PTR AddressTIB;
		unsigned long Limit;
		gpSyser->m_pDebugger->GetSegRegBase(*CpuReg.pFS, &AddressTIB, &Limit);

		unsigned long Mem[2];
		if (gpSyser->m_pDebugger->ReadMemory(AddressTIB, Mem, 8) == 8)
		{
			while (Mem[0] != -1)
			{
				AddressTIB = Mem[0];
				if (gpSyser->m_pDebugger->ReadMemory(AddressTIB, Mem, 8) != 8) break;
				TSPrintf(display, WSTR("%08X"), AddressTIB);
				CListStringItem *pItem = InsertItem(display, 0, 0, 0, -1);
				SetItemFormat(pItem, 1, Mem[1], WSTR("%X"));
			}
		}
	}

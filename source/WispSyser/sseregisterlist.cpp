
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "sseregisterlist.hpp"
#include "syserconfig.hpp"

	WISP_MSG_MAP_BEGIN(CSSERegisterList)
	//	WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_GET_FOCUS, OnGetFocus)
	WISP_MSG_MAP_END(CWispList)

	CSSERegisterList::CSSERegisterList()
	{
		memset(m_OldXMMRegister, 0, sizeof(m_OldXMMRegister));
	}
	CSSERegisterList::~CSSERegisterList()
	{
	}

	bool CSSERegisterList::OnUpdate(WISP_MSG *pMsg)
	{
		m_ColorChanges = gSyserColors.Color[4];
		m_ColorSame = gSyserColors.Color[1];
		m_ColorFocus = gSyserColors.Color[2];
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		UpdateItemColor(0);
		return true;
	}

	bool CSSERegisterList::OnCreate(WISP_MSG *pMsg)
	{
		m_ColorChanges = gSyserColors.Color[4];
		m_ColorSame = gSyserColors.Color[1];
		m_ColorFocus = gSyserColors.Color[2];
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		InsertColumn(WSTR("Register"), 60, 0, 0, -1);
		InsertColumn(WSTR("Value"), 240, 1, 0, -1);
		m_hSSE128RegItem[SSE_REG_0] = InsertItem(WSTR("XMM0"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_1] = InsertItem(WSTR("XMM1"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_2] = InsertItem(WSTR("XMM2"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_3] = InsertItem(WSTR("XMM3"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_4] = InsertItem(WSTR("XMM4"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_5] = InsertItem(WSTR("XMM5"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_6] = InsertItem(WSTR("XMM6"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_7] = InsertItem(WSTR("XMM7"), 0, 0, 0, -1);
	#ifdef _X64_
		m_hSSE128RegItem[SSE_REG_8] = InsertItem(WSTR("XMM8"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_9] = InsertItem(WSTR("XMM9"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_10] = InsertItem(WSTR("XMM10"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_11] = InsertItem(WSTR("XMM11"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_12] = InsertItem(WSTR("XMM12"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_13] = InsertItem(WSTR("XMM13"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_14] = InsertItem(WSTR("XMM14"), 0, 0, 0, -1);
		m_hSSE128RegItem[SSE_REG_15] = InsertItem(WSTR("XMM15"), 0, 0, 0, -1);
	#endif
		ResetContext();
		return true;
	}
	bool CSSERegisterList::OnGetFocus(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetNext(0, 0);
		if (Item) Select(Item);
		return true;
	}
	void CSSERegisterList::ResetContext()
	{
		Select(m_hSSE128RegItem[SSE_REG_0]);
		SetItemText(m_hSSE128RegItem[SSE_REG_0], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_1], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_2], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_3], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_4], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_5], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_6], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_7], 1, WSTR("????????"));
	#ifdef _X64_
		SetItemText(m_hSSE128RegItem[SSE_REG_8], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_9], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_10], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_11], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_12], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_13], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_14], 1, WSTR("????????"));
		SetItemText(m_hSSE128RegItem[SSE_REG_15], 1, WSTR("????????"));
	#endif
		memset(m_OldXMMRegister, 0, sizeof(m_OldXMMRegister));
	}

	void CSSERegisterList::UpdateContext()
	{
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && IsWindow())
		{
			if (GetActiveCPU() == m_nCPU)
				memcpy(&m_CPUREGPTR, &gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR, sizeof(m_CPUREGPTR));
			else	gpSyser->m_pDebugger->GetX86RegPtr(&m_CPUREGPTR, m_nCPU);

			WCHAR Buffer[64];

			for (int n = 0; n < SSE_REG_COUNT; ++n)
			{
				unsigned long *pXMM = (unsigned long*)m_CPUREGPTR.GetXMM(n);
				TSPrintf(Buffer, WSTR("%08x %08x %08x %08x"), pXMM[3], pXMM[2], pXMM[1], pXMM[0]);
				SetItemText(m_hSSE128RegItem[n], 1, Buffer);

				bool Same = !memcmp(&m_OldXMMRegister[0][n], pXMM, 16);
				if (!Same) memcpy(&m_OldXMMRegister[0][n], pXMM, 16);
				
				SetItemColor(m_hSSE128RegItem[n], 1, Same ? m_ColorSame:m_ColorChanges);
			}
			Update(&m_ClientRect);
		}
	}

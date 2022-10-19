
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "reghelpwnd.hpp"

	WISP_MSG_MAP_BEGIN(CRegHelpWnd)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
	WISP_MSG_MAP_END(CWispWnd)

	CRegHelpWnd::CRegHelpWnd()
	{
		m_RegisterGroup = -1;
	}
	CRegHelpWnd::~CRegHelpWnd()
	{
	}

//
	stRegisterBitName stCR4Name[] = {
			{WSTR("Reserved (Set to 0)"), 31, 21, NULL},
			{WSTR(""),10, 1,WSTR("OSXMMEX")},
			{WSTR(""),9, 1, WSTR("OSFXSR")},
			{WSTR("P\nC\nE"),8,1,NULL},
			{WSTR("P\nG\nE"),7,1,NULL},
			{WSTR("M\nC\nE"),6,1,NULL},
			{WSTR("P\nA\nE"),5,1,NULL},
			{WSTR("P\nS\nE"),4,1,NULL},
			{WSTR("D\nE"),3,1,NULL},
			{WSTR("T\nS\nD"),2,1,NULL},
			{WSTR("P\nV\nI"),1,1,NULL},
			{WSTR("V\nM\nE"),0,1,NULL},
			{NULL,0,0,NULL}};
	stRegisterBitName stCR3Name[] = {
			{WSTR("Page-Directory Base"),31,20,NULL},
			{WSTR("P\nC\nD"),4,1,NULL},
			{WSTR("P\nW\nT"),3,1,NULL},
			{NULL,0,0,NULL}};
	stRegisterBitName stCR2Name[] = {
			{WSTR("Page-Fault Linear Address"),31,32,NULL},
			{NULL,0,0,NULL}};
	stRegisterBitName stCR0Name[] = {
			{WSTR("P\nG"),31,1,NULL},
			{WSTR("C\nD"),30,1,NULL},
			{WSTR("N\nW"),29,1,NULL},
			{WSTR("A\nM"),18,1,NULL},
			{WSTR("W\nP"),16,1,NULL},
			{WSTR("N\nE"),5,1,NULL},
			{WSTR("E\nT"),4,1,NULL},
			{WSTR("T\nS"),3,1,NULL},
			{WSTR("E\nM"),2,1,NULL},
			{WSTR("M\nP"),1,1,NULL},
			{WSTR("P\nE"),0,1,NULL},
			{NULL,0,0,NULL}};
//
	stRegisterBitName stDR7Name[] = {
			{WSTR("L\n0"),0,1,NULL},
			{WSTR("G\n0"),1,1,NULL},
			{WSTR("L\n1"),2,1,NULL},
			{WSTR("G\n1"),3,1,NULL},
			{WSTR("L\n2"),4,1,NULL},
			{WSTR("G\n2"),5,1,NULL},
			{WSTR("L\n3"),6,1,NULL},
			{WSTR("G\n3"),7,1,NULL},
			{WSTR("L\nE"),8,1,NULL},
			{WSTR("G\nE"),9,1,NULL},
			{WSTR("1"),10,1,NULL},
			{WSTR("0"),11,1,NULL},
			{WSTR("0"),12,1,NULL},
			{WSTR("G\nD"),13,1,NULL},
			{WSTR("0"),14,1,NULL},
			{WSTR("0"),15,1,NULL},
			{WSTR("R/W\n0"),17,2,NULL},
			{WSTR("LEN\n0"),19,2,NULL},
			{WSTR("R/W\n1"),21,2,NULL},
			{WSTR("LEN\n1"),23,2,NULL},
			{WSTR("R/W\n2"),25,2,NULL},
			{WSTR("LEN\n2"),27,2,NULL},
			{WSTR("R/W\n3"),29,2,NULL},
			{WSTR("LEN\n4"),31,2,NULL},
			{NULL,0,0,NULL}};

	stRegisterBitName stDR6Name[] = {
			{WSTR("B\n0"),0,1,NULL},
			{WSTR("B\n1"),1,1,NULL},
			{WSTR("B\n2"),2,1,NULL},
			{WSTR("B\n3"),3,1,NULL},
			{WSTR("1"),4,1,NULL},
			{WSTR("1"),5,1,NULL},
			{WSTR("1"),6,1,NULL},
			{WSTR("1"),7,1,NULL},
			{WSTR("1"),8,1,NULL},
			{WSTR("1"),9,1,NULL},
			{WSTR("1"),10,1,NULL},
			{WSTR("1"),11,1,NULL},
			{WSTR("0"),12,1,NULL},
			{WSTR("B\nD"),13,1,NULL},
			{WSTR("B\nS"),14,1,NULL},
			{WSTR("B\nT"),15,1,NULL},
			{WSTR("Reserved(set to 1)"),31,16,NULL},
			{NULL,0,0,NULL}};

	stRegisterBitName stDR5Name[] = {
			{WSTR("Reserved"),31,32,NULL},
			{NULL,0,0,NULL}};

	stRegisterBitName stDR4Name[] = {
			{WSTR("Reserved"),31,32,NULL},
			{NULL,0,0,NULL}};

	stRegisterBitName stDR3Name[] = {
			{WSTR("Breakpoint Linear Address"),31,32,NULL},
			{NULL,0,0,NULL}};

	stRegisterBitName stDR2Name[] = {
			{WSTR("Breakpoint 2 Linear Address"),31,32,NULL},
			{NULL,0,0,NULL}};

	stRegisterBitName stDR1Name[] = {
			{WSTR("Breakpoint 1 Linear Address"),31,32,NULL},
			{NULL,0,0,NULL}};

	stRegisterBitName stDR0Name[] = {
			{WSTR("Breakpoint 0 Linear Address"),31,32,NULL},
			{NULL,0,0,NULL}};
//
	stRegisterBitName SystemSegmentDesc0[] = {
			{WSTR("Base 31:24"),31,8,NULL},
			{WSTR("G"),23,1,NULL},
			{WSTR("B"),22,1,NULL},
			{WSTR("0"),21,1,NULL},
			{WSTR("A\nV\nL"),20,1,NULL},
			{WSTR("Limit\n19:16"),19,4,NULL},
			{WSTR("P"),15,1,NULL},
			{WSTR("D\nP\nL"),14,2,NULL},
			{WSTR("1"),12,1,NULL},
			{WSTR("Type"),11,4,NULL},
			{WSTR("Base 23:16"),7,8,NULL},
			{NULL,0,0,NULL}};
	stRegisterBitName SystemSegmentDesc1[] = {
			{WSTR("Base Address 15:00"),31,16,NULL},
			{WSTR("Segment Limit 15:00"),15,16,NULL},			
			{NULL,0,0,NULL}};

	stRegisterBitName CodeSegmentDesc0[] = {
			{WSTR("Base 31:24"),31,8,NULL},
			{WSTR("G"),23,1,NULL},
			{WSTR("D"),22,1,NULL},
			{WSTR("0"),21,1,NULL},
			{WSTR("A\nV\nL"),20,1,NULL},
			{WSTR("Limit\n19:16"),19,4,NULL},
			{WSTR("P"),15,1,NULL},
			{WSTR("D\nP\nL"),14,2,NULL},
			{WSTR("1"),12,1,NULL},
			{WSTR("Type"),11,4,NULL},
			{WSTR("Base 23:16"),7,8,NULL},
			{NULL,0,0,NULL}};
	stRegisterBitName CodeSegmentDesc1[] = {
			{WSTR("Base Address 15:00"),31,16,NULL},
			{WSTR("Segment Limit 15:00"),15,16,NULL},			
			{NULL,0,0,NULL}};

	stRegisterBitName DataSegmentDesc0[] = {
			{WSTR("Base 31:24"),31,8,NULL},
			{WSTR("G"),23,1,NULL},
			{WSTR("0"),21,1,NULL},
			{WSTR("Limit\n19:16"),19,4,NULL},
			{WSTR("P"),15,1,NULL},
			{WSTR("D\nP\nL"),14,2,NULL},
			{WSTR("0"),12,1,NULL},
			{WSTR("Type"),11,4,NULL},
			{WSTR("Base 23:16"),7,8,NULL},
			{NULL,0,0,NULL}};
	stRegisterBitName DataSegmentDesc1[] = {
			{WSTR("Base Address 15:00"),31,16,NULL},
			{WSTR("Segment Limit 15:00"),15,16,NULL},			
			{NULL,0,0,NULL}};

//
	stRegisterBitName stEFlagsName[] = {
			{WSTR("Reserved(set to 0)"),31,10,NULL},
			{WSTR("I\nD"),21,1,WSTR("ID   -- Identification Flag")},
			{WSTR("V\nI\nP"),20,1,WSTR("VIP  -- Virtual Interrupt Pending")},
			{WSTR("V\nI\nF"),19,1,WSTR("VIF  -- Virtual Interrupt Flag")},
			{WSTR("A\nC"),18,1,WSTR("AC   -- Alignment Check")},
			{WSTR("V\nM"),17,1,WSTR("VM   -- Virtual-8086 Mode")},
			{WSTR("R\nF"),16,1,WSTR("RF   -- Resume Flag")},
			{WSTR("0"),15,1,NULL},
			{WSTR("N\nT"),14,1,WSTR("NT   -- Nested Task Flag")},
			{WSTR("IOPL"),13,2,WSTR("IOPL -- I/O Privilege Level")},
			{WSTR("O\nF"),11,1,NULL},
			{WSTR("O\nF"),10,1,NULL},
			{WSTR("I\nF"),9,1,WSTR("IF   -- Interrupt Enable Flag")},
			{WSTR("T\nF"),8,1,WSTR("TF   -- Trap Flag")},
			{WSTR("S\nF"),7,1,WSTR("SF   -- Sign Flag")},
			{WSTR("Z\nF"),6,1,WSTR("ZF   -- Zero Flag")},
			{WSTR("0"),5,1,NULL},
			{WSTR("A\nF"),4,1,WSTR("AF   -- Auxiliary Carry Flag")},
			{WSTR("0"),3,1,NULL},
			{WSTR("P\nF"),2,1,WSTR("PF   -- Parity Flag")},
			{WSTR("1"),1,1,NULL},
			{WSTR("C\nF"),0,1,WSTR("CF   -- Carry Flag")},
			{NULL,0,0,NULL}};
//
	stRegisterBitName PageDirEntry4M[] = {
			{WSTR("Page Base Address"),31,10,NULL},
			{WSTR("Reserved"),21,9,NULL},
			{WSTR("P\nA\nT"),12,1,WSTR("Page Table Attribute Index")},
			{WSTR("Avail."),11,3,WSTR("Available for system programmer's use")},
			{WSTR("G"),8,1,WSTR("Global page")},
			{WSTR("P\nS"),7,1,WSTR("Page size(1 indicates 4 MBytes)")},
			{WSTR("D"),6,1,WSTR("Dirty")},
			{WSTR("A"),5,1,WSTR("Accessed")},
			{WSTR("P\nC\nD"),4,1,WSTR("Cache disabled")},
			{WSTR("P\nW\nT"),3,1,WSTR("Write-through")},
			{WSTR("U\n/\nS"),2,1,WSTR("User/Supervisor")},
			{WSTR("R\n/\nW"),1,1,WSTR("Read/Write")},
			{WSTR("P"),0,1,WSTR("Present")},
			{NULL,0,0,NULL}};
//
	stRegisterBitName PageDirEntry4K[] = {
			{WSTR("Page-Table Base Address"),31,20,NULL},
			{WSTR("Avail"),11,3,WSTR("Available for system programmer's use")},
			{WSTR("G"),8,1,WSTR("Global page (Ignored)")},
			{WSTR("P\nS"),7,1,WSTR("Page size(0 indicates 4 KBytes)")},
			{WSTR("0"),6,1,WSTR("Reserved (set to 0)")},
			{WSTR("A"),5,1,WSTR("Accessed")},
			{WSTR("P\nC\nD"),4,1,WSTR("Cache disabled")},
			{WSTR("P\nW\nT"),3,1,WSTR("Write-through")},
			{WSTR("U\n/\nS"),2,1,WSTR("User/Supervisor")},
			{WSTR("R\n/\nW"),1,1,WSTR("Read/Write")},
			{WSTR("P"),0,1,WSTR("Present")},
			{NULL,0,0,NULL}};

	stRegisterBitName PageTabEntry4K[] = {
			{WSTR("Page Base Address"),31,20,NULL},
			{WSTR("Avail"),11,3,WSTR("Available for system programmer's use")},
			{WSTR("G"),8,1,WSTR("Global page (Ignored)")},
			{WSTR("P\nA\nT"),7,1,WSTR("Page Table Attribute Index")},
			{WSTR("D"),6,1,WSTR("Dirty")},
			{WSTR("A"),5,1,WSTR("Accessed")},
			{WSTR("P\nC\nD"),4,1,WSTR("Cache disabled")},
			{WSTR("P\nW\nT"),3,1,WSTR("Write-through")},
			{WSTR("U\n/\nS"),2,1,WSTR("User/Supervisor")},
			{WSTR("R\n/\nW"),1,1,WSTR("Read/Write")},
			{WSTR("P"),0,1,WSTR("Present")},
			{NULL,0,0,NULL}};

//
	bool CRegHelpWnd::OnUpdateClient(WISP_MSG *pMsg)
	{

		int FontHeight = m_ClientDC.GetTextHeight((WCHAR)0);//WSTR("M"));
		m_ClientDC.DrawFullRect(&m_ClientRect, gpCurWisp->m_pWispDrawObj->m_crSystem[1]);
		int y = 10;
		//CWispDC WispDC;

		if (m_RegisterGroup == 0)
		{
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			DrawRegister(10, 10, 0x80000FFF, 0xFFFFF800, 0x7FF, 0, m_ClientDC, stCR4Name, WSTR("CR4"));
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			DrawRegister(10, FontHeight + 100, 0x8000183D, 0xFE7, 0x81C, 0, m_ClientDC, stCR3Name, WSTR("CR3"));
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			y = 2 * FontHeight + 145;
			DrawRegister(10, y, 0x80000001, 0, 0, 0, m_ClientDC, stCR2Name, WSTR("CR2"));
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			y += FontHeight + 45;
			DrawRegister(10, y, 0x80000001, 0xFFFFFFFF, 0, 0, m_ClientDC, 0, WSTR("CR1"));
			y += FontHeight + 45;
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			DrawRegister(10, y, 0xF00F807F, 0x1FFAFFC0, 0x7007803F, 0, m_ClientDC, stCR0Name, WSTR("CR0"));
		}

		if (m_RegisterGroup == 1)
		{
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			DrawRegister(10, y, 0xFFFFFFFF, 0xDC00, 0xAAAAB3FF, 0, m_ClientDC, stDR7Name, WSTR("DR7"));
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			y += FontHeight + 45;
			DrawRegister(10, y, 0x8001FFFF, 0xFFFF1FF0, 0xF00F, 0, m_ClientDC, stDR6Name, WSTR("DR6"));
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			y += FontHeight + 45;
			DrawRegister(10, y, 0x80000001, 0xFFFFFFFF, 0, 0, m_ClientDC, stDR5Name, WSTR("DR5"));
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			y += FontHeight + 45;
			DrawRegister(10, y, 0x80000001, 0xFFFFFFFF, 0, 0, m_ClientDC, stDR4Name, WSTR("DR4"));
			y += FontHeight + 45;
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			DrawRegister(10, y, 0x80000001, 0, 0, 0, m_ClientDC, stDR3Name, WSTR("DR0 - DR3"));
		}

		if (m_RegisterGroup == 2)
		{
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			DrawRegister(10, y, 0x81F9F981, 0x200000, 0x80F8D880, 0, m_ClientDC, SystemSegmentDesc0, 0);
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			y += FontHeight + 45;
			DrawRegister(10, y, 0x80018001, 0, 0x8000, 0, m_ClientDC, SystemSegmentDesc1, 0);
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			y += FontHeight + 45;
			DrawRegister(10, y, 0x81F9F981, 0x200000, 0x80F8D880, 0, m_ClientDC, CodeSegmentDesc0, 0);
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			y += FontHeight + 45;
			DrawRegister(10, y, 0x80018001, 0, 0x8000, 0, m_ClientDC, CodeSegmentDesc1, 0);
			y += FontHeight + 45;
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			DrawRegister(10, y, 0x81F9F981, 0x700000, 0x80F8D880, 0, m_ClientDC, DataSegmentDesc0, 0);
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			y += FontHeight + 45;
			DrawRegister(10, y, 0x80018001, 0, 0x8000, 0, m_ClientDC, DataSegmentDesc1, 0);
		}

		if (m_RegisterGroup == 3)
		{
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			DrawRegister(40, y, 0x807FFFFF, 0xFFC0802A, 0x3FEFFFu, 1, m_ClientDC, stEFlagsName, 0);
		}

		if (m_RegisterGroup == 4)
		{
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			DrawRegister(10, y, 0x80603BFF, 0x3FE000, 0x2019FF, 0, m_ClientDC, PageDirEntry4M, 0);
		}

		if (m_RegisterGroup == 5)
		{
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			DrawRegister(10, y, 0x80003BFF, 0, 0x9FF, 0, m_ClientDC, PageDirEntry4K, 0);
			//memcpy(&WispDC, &m_ClientDC, sizeof(WispDC));
			DrawRegister(10, 225, 0x80003BFF, 0, 0x9FF, 0, m_ClientDC, PageTabEntry4K, 0);
		}
 
		return false;
	}

	bool CRegHelpWnd::DrawRegister(int x, int y, unsigned int NumberMask, unsigned int ReservedMask, unsigned int LineMask, unsigned int OtherMask, CWispDC DrawDC, stRegisterBitName *RegBitName, const WCHAR *RegisterName)
	{
		int FontHeight = DrawDC.GetTextHeight(0);//L"M");
		WISP_RECT rc;

		rc.x = x;
		rc.cy = FontHeight + 45;
		rc.y = y;
		rc.cx = 481;
		DrawDC.DrawFullRect(&rc, gpCurWisp->m_pWispDrawObj->m_crSystem[1]);

		int posy = FontHeight + y;

		rc.y = posy;
		rc.cx = 15;
		rc.cy = 45;
		int v27 = 31;
		int v25 = x;
		do {
			rc.x = v25;
			if ( ((ReservedMask >> v27) & 1) != 0)
				DrawDC.DrawFullRect(&rc, gpCurWisp->m_pWispDrawObj->m_crSystem[13]);
			--v27;
			v25 += 15;
		} while (v27 >= 0);

		rc.y = y;
		rc.cx = 15;
		rc.cy = FontHeight;
		int v11 = 31;
		int v12_v1 = x;
		do {
			rc.x = v12_v1;
			if ( ((NumberMask >> v11) & 1) != 0)
			{
				WCHAR DisplayOut[4];
				TSPrintf(DisplayOut, WSTR("%d"), v11);
				DrawDC.DrawString(DisplayOut, &rc, 0x25, 0, -1);
			}
			if ( ((LineMask >> v11) & 1) != 0 && v11 != 31)
				DrawDC.DrawVLine(rc.x, FontHeight + rc.y, rc.y + FontHeight + 44, gpCurWisp->m_pWispDrawObj->m_crSystem[0]);
			v12_v1 += 15;
			--v11;
		} while (v11 >= 0);

		if (RegBitName)
		{
			bool bDescriptorExist = false;
			rc.y = posy;
			rc.cy = 45;

			int i = 0;

			int DescriptorFieldMaxLen = 0;
			stRegisterBitName *regBitName = RegBitName;
			while (regBitName->BitLen != 0)
			{
				if (i > 31) break;
				rc.x = x + 15 * (31 - regBitName->BeginBitIndex);
				rc.cx = 15 * regBitName->BitLen;
				if (regBitName->RegName)
					DrawDC.DrawString(regBitName->RegName, &rc, 5, 0, -1);
				if (regBitName->Descriptor)
				{
					bDescriptorExist = true;
					int nTmp = 15 * (regBitName->BeginBitIndex - 31) + DrawDC.GetTextExtent(regBitName->Descriptor, -1, 0);
					if (DescriptorFieldMaxLen < nTmp)
						DescriptorFieldMaxLen = nTmp;
				}
				++i;
				++regBitName;
			}

			if (bDescriptorExist)
			{
				rc.cy = FontHeight;
				rc.x = x - DescriptorFieldMaxLen;

				int i = 0;
				int tmpy = 0;

				stRegisterBitName *regBitName = RegBitName;
				while (regBitName->BitLen != 0)
				{
					if (i > 31) break;
					if (regBitName->Descriptor)
					{
						rc.y = FontHeight + (FontHeight + 2)*tmpy + y + 65;
						int tlen = DrawDC.GetTextExtent(regBitName->Descriptor, -1, 0);
						int px = x - 15 * regBitName->BeginBitIndex;
						rc.cx = tlen;
						int py = (FontHeight + 2) / 2 + rc.y - 2;
						px += 472;
						DrawDC.DrawHLine(rc.x + tlen, px, py, gpCurWisp->m_pWispDrawObj->m_crSystem[0]);
						DrawDC.DrawVLine(px, posy + 47, py, gpCurWisp->m_pWispDrawObj->m_crSystem[0]);
						DrawDC.DrawString(regBitName->Descriptor, &rc, 0x24, 0, -1);
						++tmpy;
					}
					++i;
					++regBitName;
				}
			}
		}

		rc.x = x;
		rc.y = posy;
		rc.cx = 481;
		rc.cy = 45;
		DrawDC.DrawRect(&rc, gpCurWisp->m_pWispDrawObj->m_crSystem[0]);
		if (RegisterName)
		{
			rc.x = x + 490;
			rc.y = posy;
			DrawDC.DrawString(RegisterName, &rc, 0x104, 0, -1);
		}
		return false;
	}

	int CRegHelpWnd::SetRegisterGroup(int index)
	{
		if (index >= 0 && m_RegisterGroup < 6)
			m_RegisterGroup = index;
		return 0;
	}


	WISP_MSG_MAP_BEGIN(CX86RegHelpTabWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
	WISP_MSG_MAP_END(CWispTabWnd)


	bool CX86RegHelpTabWnd::OnCreate(WISP_MSG *pMsg)
	{
		WISP_RECT Rect;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		m_RegisterGroup[0].CreateWnd(WSTR("Control Register"), Rect, this, 0, 0, WISP_SH_MINSIZE);
		m_RegisterGroup[0].SetRegisterGroup(0);
		InsertWnd(WSTR("Control"), &m_RegisterGroup[0], 0);

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		m_RegisterGroup[1].CreateWnd(WSTR("Debug Register"), Rect, this, 0, 0, WISP_SH_MINSIZE);
		m_RegisterGroup[1].SetRegisterGroup(1);
		InsertWnd(WSTR("Debug"), &m_RegisterGroup[1], 0);

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		m_RegisterGroup[2].CreateWnd(WSTR("Segment Descriptor"), Rect, this, 0, 0, WISP_SH_MINSIZE);
		m_RegisterGroup[2].SetRegisterGroup(2);
		InsertWnd(WSTR("Segment Descriptor"), &m_RegisterGroup[2], 0);

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		m_RegisterGroup[3].CreateWnd(WSTR("Flags Register"), Rect, this, 0, 0, WISP_SH_MINSIZE);
		m_RegisterGroup[3].SetRegisterGroup(3);
		InsertWnd(WSTR("Flags Register"), &m_RegisterGroup[3], 0);

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		m_RegisterGroup[4].CreateWnd(WSTR("Flags Register"), Rect, this, 0, 0, WISP_SH_MINSIZE);
		m_RegisterGroup[4].SetRegisterGroup(4);
		InsertWnd(WSTR("Page Directory Entry (4M)"), &m_RegisterGroup[4], 0);

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		m_RegisterGroup[5].CreateWnd(WSTR("Flags Register"), Rect, this, 0, 0, WISP_SH_MINSIZE);
		m_RegisterGroup[5].SetRegisterGroup(5);
		InsertWnd(WSTR("Page Map (4K)"), &m_RegisterGroup[5], 0);
 
		return true;
	}

	void CX86RegHelpTabWnd::Create()
	{
		if (IsWindow())
		{
			Show(WISP_SH_MINSIZE);
		} else
		{
			WISP_RECT Rect;
			Rect.x = 0;
			Rect.y = 0;
			Rect.cx = 560;
			Rect.cy = 400;
			CreateWnd(WSTR("Register Help Wnd"), Rect, 0, 0, 0xBB, WISP_SH_MINSIZE);
			Center(nullptr);
		}
	}

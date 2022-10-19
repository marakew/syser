
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "fpuregisterlist.hpp"
#include "syserconfig.hpp"

//#define FPUFMT "%+1.17e"
#define FPUFMT "%1.17e"
extern "C" void _cdecl REAL10toDOUBLE(unsigned long long *pFpuReg, double *Value);

	WISP_MSG_MAP_BEGIN(CFpuRegisterList)
	//	WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_GET_FOCUS, OnGetFocus)
	WISP_MSG_MAP_END(CWispList)

	CFpuRegisterList::CFpuRegisterList()
	{
	}
	CFpuRegisterList::~CFpuRegisterList()
	{
	}

	bool CFpuRegisterList::OnUpdate(WISP_MSG *pMsg)
	{
		m_ColorChanges = gSyserColors.Color[4];
		m_ColorSame = gSyserColors.Color[1];
		m_ColorFocus = gSyserColors.Color[2];
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		UpdateItemColor(0);
		return true;
	}

	bool CFpuRegisterList::OnCreate(WISP_MSG *pMsg)
	{
		m_ColorChanges = gSyserColors.Color[4];
		m_ColorSame = gSyserColors.Color[1];
		m_ColorFocus = gSyserColors.Color[2];
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		InsertColumn(WSTR("Register"), 90, 0, 0, -1);
		InsertColumn(WSTR("Value"), 180, 1, 0, -1);

		m_hFpuRegItem[FPU_REG_ST_0] = InsertItem(WSTR("ST(0)"), 0, 0, 0, -1);
		m_hFpuRegItem[FPU_REG_ST_1] = InsertItem(WSTR("ST(1)"), 0, 0, 0, -1);
		m_hFpuRegItem[FPU_REG_ST_2] = InsertItem(WSTR("ST(2)"), 0, 0, 0, -1);
		m_hFpuRegItem[FPU_REG_ST_3] = InsertItem(WSTR("ST(3)"), 0, 0, 0, -1);
		m_hFpuRegItem[FPU_REG_ST_4] = InsertItem(WSTR("ST(4)"), 0, 0, 0, -1);
		m_hFpuRegItem[FPU_REG_ST_5] = InsertItem(WSTR("ST(5)"), 0, 0, 0, -1);
		m_hFpuRegItem[FPU_REG_ST_6] = InsertItem(WSTR("ST(6)"), 0, 0, 0, -1);
		m_hFpuRegItem[FPU_REG_ST_7] = InsertItem(WSTR("ST(7)"), 0, 0, 0, -1);

		m_hFpuRegItem[FPU_REG_STATE] = InsertItem(WSTR("FPU Status Word"), 0, 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_B] = InsertItem(WSTR("FPU Busy"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_C3] = InsertItem(WSTR("Condition Code 3"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_TOP] = InsertItem(WSTR("Top of Stack Pointer"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_C2] = InsertItem(WSTR("Condition Code 2"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_C1] = InsertItem(WSTR("Condition Code 1"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_C0] = InsertItem(WSTR("Condition Code 0"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_ES] = InsertItem(WSTR("Error Summary Status"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_SF] = InsertItem(WSTR("Stack Fault"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_PE] = InsertItem(WSTR("Precision Exception"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_UE] = InsertItem(WSTR("Underflow Exception"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_OE] = InsertItem(WSTR("Overflow Exception"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_ZE] = InsertItem(WSTR("Zero Divide Exception"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_DE] = InsertItem(WSTR("Denormalized Exception"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_STATUS_IE] = InsertItem(WSTR("Invalid Operation Exception"), m_hFpuRegItem[FPU_REG_STATE], 0, 0, -1);

		m_hFpuRegItem[FPU_REG_CONTROL] = InsertItem(WSTR("FPU Control Word"), 0, 0, 0, -1);
		m_hFpuRegItem[FPU_REG_CONTROL_X] = InsertItem(WSTR("Infinity Control"), m_hFpuRegItem[FPU_REG_CONTROL], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_CONTROL_RC] = InsertItem(WSTR("Rounding Control"), m_hFpuRegItem[FPU_REG_CONTROL], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_CONTROL_PC] = InsertItem(WSTR("Precision Control"), m_hFpuRegItem[FPU_REG_CONTROL], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_CONTROL_PM] = InsertItem(WSTR("Precision Mask"), m_hFpuRegItem[FPU_REG_CONTROL], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_CONTROL_UM] = InsertItem(WSTR("Underflow Mask"), m_hFpuRegItem[FPU_REG_CONTROL], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_CONTROL_OM] = InsertItem(WSTR("Overflow Mask"), m_hFpuRegItem[FPU_REG_CONTROL], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_CONTROL_ZM] = InsertItem(WSTR("Zero Divide Mask"), m_hFpuRegItem[FPU_REG_CONTROL], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_CONTROL_DM] = InsertItem(WSTR("Denormal Mask"), m_hFpuRegItem[FPU_REG_CONTROL], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_CONTROL_IM] = InsertItem(WSTR("Invalid Mask"), m_hFpuRegItem[FPU_REG_CONTROL], 0, 0, -1);

		m_hFpuRegItem[FPU_REG_TAG] = InsertItem(WSTR("FPU Tag word"), 0, 0, 0, -1);
		m_hFpuRegItem[FPU_REG_TAG_0] = InsertItem(WSTR("TAG(0)"), m_hFpuRegItem[FPU_REG_TAG], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_TAG_1] = InsertItem(WSTR("TAG(1)"), m_hFpuRegItem[FPU_REG_TAG], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_TAG_2] = InsertItem(WSTR("TAG(2)"), m_hFpuRegItem[FPU_REG_TAG], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_TAG_3] = InsertItem(WSTR("TAG(3)"), m_hFpuRegItem[FPU_REG_TAG], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_TAG_4] = InsertItem(WSTR("TAG(4)"), m_hFpuRegItem[FPU_REG_TAG], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_TAG_5] = InsertItem(WSTR("TAG(5)"), m_hFpuRegItem[FPU_REG_TAG], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_TAG_6] = InsertItem(WSTR("TAG(6)"), m_hFpuRegItem[FPU_REG_TAG], 0, 0, -1);
		m_hFpuRegItem[FPU_REG_TAG_7] = InsertItem(WSTR("TAG(7)"), m_hFpuRegItem[FPU_REG_TAG], 0, 0, -1);
		ResetContext();
		return true;
	}
	bool CFpuRegisterList::OnGetFocus(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetNext(0, 0);
		if (Item) Select(Item);
		return true;
	}
	void CFpuRegisterList::ResetContext()
	{
		Select(m_hFpuRegItem[FPU_REG_ST_0]);
		SetItemText(m_hFpuRegItem[FPU_REG_ST_0], 1, WSTR("?? ?? ?? ?? ?? ?? ?? ?? ?? ??"));
		SetItemText(m_hFpuRegItem[FPU_REG_ST_1], 1, WSTR("?? ?? ?? ?? ?? ?? ?? ?? ?? ??"));
		SetItemText(m_hFpuRegItem[FPU_REG_ST_2], 1, WSTR("?? ?? ?? ?? ?? ?? ?? ?? ?? ??"));
		SetItemText(m_hFpuRegItem[FPU_REG_ST_3], 1, WSTR("?? ?? ?? ?? ?? ?? ?? ?? ?? ??"));
		SetItemText(m_hFpuRegItem[FPU_REG_ST_4], 1, WSTR("?? ?? ?? ?? ?? ?? ?? ?? ?? ??"));
		SetItemText(m_hFpuRegItem[FPU_REG_ST_5], 1, WSTR("?? ?? ?? ?? ?? ?? ?? ?? ?? ??"));
		SetItemText(m_hFpuRegItem[FPU_REG_ST_6], 1, WSTR("?? ?? ?? ?? ?? ?? ?? ?? ?? ??"));
		SetItemText(m_hFpuRegItem[FPU_REG_ST_7], 1, WSTR("?? ?? ?? ?? ?? ?? ?? ?? ?? ??"));

		SetItemText(m_hFpuRegItem[FPU_REG_STATE], 1, WSTR("????"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_B], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_C3], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_TOP], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_C2], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_C1], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_C0], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_ES], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_SF], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_PE], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_UE], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_OE], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_ZE], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_DE], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_STATUS_IE], 1, WSTR("?"));

		SetItemText(m_hFpuRegItem[FPU_REG_CONTROL], 1, WSTR("????"));
		SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_X], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_RC], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_PC], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_PM], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_UM], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_OM], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_ZM], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_DM], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_IM], 1, WSTR("?"));

		SetItemText(m_hFpuRegItem[FPU_REG_TAG], 1, WSTR("????"));
		SetItemText(m_hFpuRegItem[FPU_REG_TAG_0], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_TAG_1], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_TAG_2], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_TAG_3], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_TAG_4], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_TAG_5], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_TAG_6], 1, WSTR("?"));
		SetItemText(m_hFpuRegItem[FPU_REG_TAG_7], 1, WSTR("?"));
		memset(&m_FPUREG, 0, sizeof(m_FPUREG));
	}
	void CFpuRegisterList::UpdateContext()
	{
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && IsWindow())
		{
			if (GetActiveCPU() == m_nCPU)
				memcpy(&m_CPUREGPTR, &gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR, sizeof(m_CPUREGPTR));
			else	gpSyser->m_pDebugger->GetX86RegPtr(&m_CPUREGPTR, m_nCPU);

			//memcpy(&m_FPUREG, &FpuRegisterImage[CurrentFpuImageIndex ^ 1], sizeof(m_FPUREG));

			WCHAR Buffer[5000];

			for (int n = 0; n < 8; ++n)
			{
				unsigned char *pFPU = m_CPUREGPTR.GetFPU(n);

			//	TSPrintf(Buffer, WSTR("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x"),
			//		pFPU[0], pFPU[1], pFPU[2], pFPU[3], pFPU[4],
			//		pFPU[5], pFPU[6], pFPU[7], pFPU[8], pFPU[9]);
				double FPUValue = 0.0;
				REAL10toDOUBLE((unsigned long long *)pFPU, &FPUValue); //0xFFF80000
			#if 0
//unsigned long NaN[2] = {0xffffffff, 0x7fffffff}
//A quiet NaN is represented by any bit pattern between X'7FF80000 00000000' and X'7FFFFFFF FFFFFFFF' 
//or between X'FFF80000 00000000' and X'FFFFFFFF FFFFFFFF'.
//https://stackoverflow.com/questions/2249110/how-do-i-make-a-portable-isnan-isinf-function/2249173

				if (_fpclass(FPUValue) == _FPCLASS_QNAN || _fpclass(FPUValue) == _FPCLASS_SNAN)
					TSPrintf(Buffer, WSTR("Nan"));
				else
				if (_fpclass(FPUValue) == _FPCLASS_NINF)
					TSPrintf(Buffer, WSTR("-Inf"));
				else
				if (_fpclass(FPUValue) == _FPCLASS_PINF)
					TSPrintf(Buffer, WSTR("+Inf"));
				//_FPCLASS_NN
				//_FPCLASS_ND
				//_FPCLASS_NZ
				//_FPCLASS_PZ
				//_FPCLASS_PD
				//_FPCLASS_PN
				else
			#endif
					TSPrintf(Buffer, WSTR("" FPUFMT ""), FPUValue);

				SetItemText(m_hFpuRegItem[n], 1, Buffer);
				bool Same = !memcmp(pFPU, &m_FPUREG.RegisterArea[n*10], 10);
				SetItemColor(m_hFpuRegItem[n], 1, Same ? m_ColorSame:m_ColorChanges);
			}
		//STATUS
			TSPrintf(Buffer, WSTR("%04x"), *(unsigned short*)m_CPUREGPTR.FP_StatusWord);
			SetItemText(m_hFpuRegItem[FPU_REG_STATE], 1, Buffer);
			SetItemColor(m_hFpuRegItem[FPU_REG_STATE], 1, (*(unsigned short*)m_CPUREGPTR.FP_StatusWord==*(unsigned short*)&m_FPUREG.StatusWord)?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_IE], 1, m_CPUREGPTR.FP_StatusWord->IE?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_IE], 1, m_CPUREGPTR.FP_StatusWord->IE==m_FPUREG.StatusWord.IE?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_DE], 1, m_CPUREGPTR.FP_StatusWord->DE?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_DE], 1, m_CPUREGPTR.FP_StatusWord->DE==m_FPUREG.StatusWord.DE?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_ZE], 1, m_CPUREGPTR.FP_StatusWord->ZE?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_ZE], 1, m_CPUREGPTR.FP_StatusWord->ZE==m_FPUREG.StatusWord.ZE?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_OE], 1, m_CPUREGPTR.FP_StatusWord->OE?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_OE], 1, m_CPUREGPTR.FP_StatusWord->OE==m_FPUREG.StatusWord.OE?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_UE], 1, m_CPUREGPTR.FP_StatusWord->UE?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_UE], 1, m_CPUREGPTR.FP_StatusWord->UE==m_FPUREG.StatusWord.UE?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_PE], 1, m_CPUREGPTR.FP_StatusWord->PE?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_PE], 1, m_CPUREGPTR.FP_StatusWord->PE==m_FPUREG.StatusWord.PE?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_SF], 1, m_CPUREGPTR.FP_StatusWord->SF?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_SF], 1, m_CPUREGPTR.FP_StatusWord->SF==m_FPUREG.StatusWord.SF?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_ES], 1, m_CPUREGPTR.FP_StatusWord->ES?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_ES], 1, m_CPUREGPTR.FP_StatusWord->ES==m_FPUREG.StatusWord.ES?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_C0], 1, m_CPUREGPTR.FP_StatusWord->C0?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_C0], 1, m_CPUREGPTR.FP_StatusWord->C0==m_FPUREG.StatusWord.C0?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_C1], 1, m_CPUREGPTR.FP_StatusWord->C1?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_C1], 1, m_CPUREGPTR.FP_StatusWord->C1==m_FPUREG.StatusWord.C1?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_C2], 1, m_CPUREGPTR.FP_StatusWord->C2?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_C2], 1, m_CPUREGPTR.FP_StatusWord->C2==m_FPUREG.StatusWord.C2?m_ColorSame:m_ColorChanges);

			TSPrintf(Buffer, WSTR("%x"), m_CPUREGPTR.FP_StatusWord->TOP);
			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_TOP], 1, Buffer);
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_TOP], 1, m_CPUREGPTR.FP_StatusWord->TOP==m_FPUREG.StatusWord.TOP?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_C3], 1, m_CPUREGPTR.FP_StatusWord->C3?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_C3], 1, m_CPUREGPTR.FP_StatusWord->C3==m_FPUREG.StatusWord.C3?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_STATUS_B], 1, m_CPUREGPTR.FP_StatusWord->B?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_STATUS_B], 1, m_CPUREGPTR.FP_StatusWord->B==m_FPUREG.StatusWord.B?m_ColorSame:m_ColorChanges);
		//CONTROL
			TSPrintf(Buffer, WSTR("%04x"), *(unsigned short*)m_CPUREGPTR.FP_ControlWord);
			SetItemText(m_hFpuRegItem[FPU_REG_CONTROL], 1, Buffer);
			SetItemColor(m_hFpuRegItem[FPU_REG_CONTROL], 1, (*(unsigned short*)m_CPUREGPTR.FP_ControlWord==*(unsigned short*)&m_FPUREG.ControlWord)?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_X], 1, m_CPUREGPTR.FP_ControlWord->X?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_CONTROL_X], 1, m_CPUREGPTR.FP_ControlWord->X==m_FPUREG.ControlWord.X?m_ColorSame:m_ColorChanges);

			SetItemFormat(m_hFpuRegItem[FPU_REG_CONTROL_RC], 1, m_CPUREGPTR.FP_ControlWord->RC, WSTR("%X"));
			//TODO compare changes
			SetItemColor(m_hFpuRegItem[FPU_REG_CONTROL_RC], 1, m_ColorSame);

			SetItemFormat(m_hFpuRegItem[FPU_REG_CONTROL_PC], 1, m_CPUREGPTR.FP_ControlWord->PC, WSTR("%X"));
			//TODO compare changes
			SetItemColor(m_hFpuRegItem[FPU_REG_CONTROL_PC], 1, m_ColorSame);

			SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_PM], 1, m_CPUREGPTR.FP_ControlWord->PM?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_CONTROL_PM], 1, m_CPUREGPTR.FP_ControlWord->PM==m_FPUREG.ControlWord.PM?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_UM], 1, m_CPUREGPTR.FP_ControlWord->UM?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_CONTROL_UM], 1, m_CPUREGPTR.FP_ControlWord->UM==m_FPUREG.ControlWord.UM?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_OM], 1, m_CPUREGPTR.FP_ControlWord->OM?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_CONTROL_OM], 1, m_CPUREGPTR.FP_ControlWord->OM==m_FPUREG.ControlWord.OM?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_ZM], 1, m_CPUREGPTR.FP_ControlWord->ZM?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_CONTROL_ZM], 1, m_CPUREGPTR.FP_ControlWord->ZM==m_FPUREG.ControlWord.ZM?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_DM], 1, m_CPUREGPTR.FP_ControlWord->DM?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_CONTROL_DM], 1, m_CPUREGPTR.FP_ControlWord->DM==m_FPUREG.ControlWord.DM?m_ColorSame:m_ColorChanges);

			SetItemText(m_hFpuRegItem[FPU_REG_CONTROL_IM], 1, m_CPUREGPTR.FP_ControlWord->IM?WSTR("1"):WSTR("0"));
			SetItemColor(m_hFpuRegItem[FPU_REG_CONTROL_IM], 1, m_CPUREGPTR.FP_ControlWord->IM==m_FPUREG.ControlWord.IM?m_ColorSame:m_ColorChanges);

		//TAG
			TSPrintf(Buffer, WSTR("%04x"), *(unsigned short*)m_CPUREGPTR.FP_TagWord);
			SetItemText(m_hFpuRegItem[FPU_REG_TAG], 1, Buffer);
			SetItemColor(m_hFpuRegItem[FPU_REG_TAG], 1, (*(unsigned short*)m_CPUREGPTR.FP_TagWord==*(unsigned short*)&m_FPUREG.TagWord)?m_ColorSame:m_ColorChanges);
			//3 ?
			SetItemFormat(m_hFpuRegItem[FPU_REG_TAG_0], 1, m_CPUREGPTR.FP_TagWord->TAG0,WSTR("%X"));
			SetItemColor(m_hFpuRegItem[FPU_REG_TAG_0], 1, m_CPUREGPTR.FP_TagWord->TAG0==m_FPUREG.TagWord.TAG0?m_ColorSame:m_ColorChanges);
			//0xC
			SetItemFormat(m_hFpuRegItem[FPU_REG_TAG_1], 1, m_CPUREGPTR.FP_TagWord->TAG1,WSTR("%X"));
			SetItemColor(m_hFpuRegItem[FPU_REG_TAG_1], 1, m_CPUREGPTR.FP_TagWord->TAG1==m_FPUREG.TagWord.TAG1?m_ColorSame:m_ColorChanges);
			//0x30
			SetItemFormat(m_hFpuRegItem[FPU_REG_TAG_2], 1, m_CPUREGPTR.FP_TagWord->TAG2,WSTR("%X"));
			SetItemColor(m_hFpuRegItem[FPU_REG_TAG_2], 1, m_CPUREGPTR.FP_TagWord->TAG2==m_FPUREG.TagWord.TAG2?m_ColorSame:m_ColorChanges);
			//0xC0
			SetItemFormat(m_hFpuRegItem[FPU_REG_TAG_3], 1, m_CPUREGPTR.FP_TagWord->TAG3,WSTR("%X"));
			SetItemColor(m_hFpuRegItem[FPU_REG_TAG_3], 1, m_CPUREGPTR.FP_TagWord->TAG3==m_FPUREG.TagWord.TAG3?m_ColorSame:m_ColorChanges);
			//3 ?
			SetItemFormat(m_hFpuRegItem[FPU_REG_TAG_4], 1, m_CPUREGPTR.FP_TagWord->TAG4,WSTR("%X"));
			SetItemColor(m_hFpuRegItem[FPU_REG_TAG_4], 1, m_CPUREGPTR.FP_TagWord->TAG4==m_FPUREG.TagWord.TAG4?m_ColorSame:m_ColorChanges);
			//0xC00
			SetItemFormat(m_hFpuRegItem[FPU_REG_TAG_5], 1, m_CPUREGPTR.FP_TagWord->TAG5,WSTR("%X"));
			SetItemColor(m_hFpuRegItem[FPU_REG_TAG_5], 1, m_CPUREGPTR.FP_TagWord->TAG5==m_FPUREG.TagWord.TAG5?m_ColorSame:m_ColorChanges);
			//0x3000
			SetItemFormat(m_hFpuRegItem[FPU_REG_TAG_6], 1, m_CPUREGPTR.FP_TagWord->TAG6,WSTR("%X"));
			SetItemColor(m_hFpuRegItem[FPU_REG_TAG_6], 1, m_CPUREGPTR.FP_TagWord->TAG6==m_FPUREG.TagWord.TAG6?m_ColorSame:m_ColorChanges);
			//0xC000
			SetItemFormat(m_hFpuRegItem[FPU_REG_TAG_7], 1, m_CPUREGPTR.FP_TagWord->TAG7,WSTR("%X"));
			SetItemColor(m_hFpuRegItem[FPU_REG_TAG_7], 1, m_CPUREGPTR.FP_TagWord->TAG7==m_FPUREG.TagWord.TAG7?m_ColorSame:m_ColorChanges);

			for (int n = 0; n < 8; ++n)
			{
				memcpy(&m_FPUREG.RegisterArea[n*10], m_CPUREGPTR.GetFPU(n), 10);
			}

			memcpy(&m_FPUREG.StatusWord, m_CPUREGPTR.FP_StatusWord, sizeof(m_FPUREG.StatusWord));
			memcpy(&m_FPUREG.ControlWord, m_CPUREGPTR.FP_ControlWord, sizeof(m_FPUREG.ControlWord));
			memcpy(&m_FPUREG.TagWord, m_CPUREGPTR.FP_TagWord, sizeof(m_FPUREG.TagWord));

			Update(&m_ClientRect);
		}
	}

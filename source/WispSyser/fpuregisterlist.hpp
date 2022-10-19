
#ifndef _FPUREGISTERLIST_HPP_
#define _FPUREGISTERLIST_HPP_

#include "../Wisp/wisplist.hpp"
#include "../EXEAnalyzer/x86debugger.hpp"

enum FPU_REG_INDEX
{
  FPU_REG_ST_0 = 0,
  FPU_REG_ST_1 = 1,
  FPU_REG_ST_2 = 2,
  FPU_REG_ST_3 = 3,
  FPU_REG_ST_4 = 4,
  FPU_REG_ST_5 = 5,
  FPU_REG_ST_6 = 6,
  FPU_REG_ST_7 = 7,

  FPU_REG_STATE = 8,
  FPU_REG_STATUS_IE = 9,
  FPU_REG_STATUS_DE = 10,
  FPU_REG_STATUS_ZE = 11,
  FPU_REG_STATUS_OE = 12,
  FPU_REG_STATUS_UE = 13,
  FPU_REG_STATUS_PE = 14,
  FPU_REG_STATUS_SF = 15,
  FPU_REG_STATUS_ES = 16,
  FPU_REG_STATUS_C0 = 17,
  FPU_REG_STATUS_C1 = 18,
  FPU_REG_STATUS_C2 = 19,
  FPU_REG_STATUS_TOP = 20,
  FPU_REG_STATUS_C3 = 21,
  FPU_REG_STATUS_B = 22,

  FPU_REG_CONTROL = 23,
  FPU_REG_CONTROL_IM = 24,
  FPU_REG_CONTROL_DM = 25,
  FPU_REG_CONTROL_ZM = 26,
  FPU_REG_CONTROL_OM = 27,
  FPU_REG_CONTROL_UM = 28,
  FPU_REG_CONTROL_PM = 29,
  FPU_REG_CONTROL_PC = 30,
  FPU_REG_CONTROL_RC = 31,
  FPU_REG_CONTROL_X = 32,

  FPU_REG_TAG = 33,
  FPU_REG_TAG_0 = 34,
  FPU_REG_TAG_1 = 35,
  FPU_REG_TAG_2 = 36,
  FPU_REG_TAG_3 = 37,
  FPU_REG_TAG_4 = 38,
  FPU_REG_TAG_5 = 39,
  FPU_REG_TAG_6 = 40,
  FPU_REG_TAG_7 = 41,

  FPU_REG_COUNT = 42,
};

struct FPU_REG
{
	unsigned char RegisterArea[80]; //SIZE_OF_80387_REGISTERS
	stFPU_STATUS_REG StatusWord;
	stFPU_CONTROL_REG ControlWord;
	stFPU_TAG_REG TagWord;
};

struct CFpuRegisterList : public CWispList
{
	CListStringItem *m_hFpuRegItem[FPU_REG_COUNT];
	unsigned long m_ColorChanges;
	unsigned long m_ColorSame;
	int m_nCPU;
	X86_CPU_REG_PTR m_CPUREGPTR;
	FPU_REG m_FPUREG;

	CFpuRegisterList();
	virtual ~CFpuRegisterList();

	bool MsgProc(WISP_MSG *pMsg) override;
	bool OnUpdate(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnGetFocus(WISP_MSG *pMsg);
	void ResetContext();
	void UpdateContext();
};

#endif
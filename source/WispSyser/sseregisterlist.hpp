
#ifndef _SSEREGISTERLIST_HPP_
#define _SSEREGISTERLIST_HPP_

#include "../Wisp/wisplist.hpp"
#include "../EXEAnalyzer/x86debugger.hpp"

enum SSE_REG_INDEX
{
  SSE_REG_0 = 0,
  SSE_REG_1,
  SSE_REG_2,
  SSE_REG_3,
  SSE_REG_4,
  SSE_REG_5,
  SSE_REG_6,
  SSE_REG_7,
#ifdef _X64_
  SSE_REG_8,
  SSE_REG_9,
  SSE_REG_10,
  SSE_REG_11,
  SSE_REG_12,
  SSE_REG_13,
  SSE_REG_14,
  SSE_REG_15,
#endif
  SSE_REG_COUNT,
};

struct CSSERegisterList : public CWispList
{
	CListStringItem *m_hSSE128RegItem[SSE_REG_COUNT];
	unsigned long m_OldXMMRegister[4][SSE_REG_COUNT];
	unsigned long m_ColorChanges;
	unsigned long m_ColorSame;
	int m_nCPU;
	X86_CPU_REG_PTR m_CPUREGPTR;

	CSSERegisterList();
	virtual ~CSSERegisterList();

	bool MsgProc(WISP_MSG *pMsg) override;
	bool OnUpdate(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnGetFocus(WISP_MSG *pMsg);
	void ResetContext();
	void UpdateContext();
};

#endif
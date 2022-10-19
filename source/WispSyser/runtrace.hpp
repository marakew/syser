
#ifndef _RUNTRACE_HPP_
#define _RUNTRACE_HPP_

#include "../EXEAnalyzer/x86debugger.hpp"

struct CTracePoint
{
	ULONG_PTR Address;
	unsigned short Offset;
	unsigned short nWndIndex;
};

struct CRunTrace
{
	unsigned long m_MaxSize;
	unsigned long m_WriteIndex; //m_WriteTraceIndex
	CTracePoint *m_pTraces;
	bool m_bSaveREG;
	unsigned long m_LastWriteIndex;
	unsigned long m_CPURegWriteIndex;
	unsigned long XXX;
	unsigned long m_nTracesIndex;
	unsigned long m_CPURegReadIndex;
	X86_CPU_REG m_CurCPU;
	X86_CPU_REG m_LastCPU;
	unsigned long *m_pCPUType;
	ULONG_PTR *m_pCPUReg;
	unsigned long m_TracesContext;

	CRunTrace();
	~CRunTrace();

	unsigned long UpdateContext(ULONG_PTR Address, ULONG_PTR CIP, unsigned short nWndIndex);
	void ReadContext(unsigned long TracePoint, X86_CPU_REG *pCPUReg, unsigned long *pCPUType);
	unsigned long Count(unsigned long CpuType) const;

	void Rotate();
	CTracePoint *GetTracePoint(unsigned long TracePoint) const;
};

#endif

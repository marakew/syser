
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "runtrace.hpp"

	CRunTrace::CRunTrace()
	{
		m_MaxSize = 300;

		m_pTraces = new CTracePoint[m_MaxSize];
		memset(m_pTraces, 0, m_MaxSize*sizeof(CTracePoint));

		m_WriteIndex = 0;

		m_pCPUType = new unsigned long[m_MaxSize];
		memset(m_pCPUType, 0, m_MaxSize*sizeof(unsigned long));

		m_pCPUReg = new ULONG_PTR[m_MaxSize*2];
		memset(m_pCPUReg, 0, m_MaxSize*2*sizeof(ULONG_PTR));

		m_CPURegWriteIndex = 0;
		XXX = 0;
		m_nTracesIndex = 0;
		m_CPURegReadIndex = 0;
		m_bSaveREG = true;
	}
	CRunTrace::~CRunTrace()
	{
		delete []m_pTraces;
		m_pTraces = 0;
		delete []m_pCPUType;
		m_pCPUType = 0;
		delete []m_pCPUReg;
		m_pCPUReg = 0;
	}

	unsigned long CRunTrace::UpdateContext(ULONG_PTR Address, ULONG_PTR CIP, unsigned short nWndIndex)
	{
		if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
			return 0;

		if (m_WriteIndex >= m_MaxSize)
			Rotate();

		m_pTraces[m_WriteIndex%m_MaxSize].Address = Address;
		m_pTraces[m_WriteIndex%m_MaxSize].Offset = CIP - Address;
		m_pTraces[m_WriteIndex%m_MaxSize].nWndIndex = nWndIndex;

		unsigned long CpuType = 0;

		if (m_bSaveREG)
		{
			m_bSaveREG = false;

		//	memcpy(&m_CurCPU, &Reg, sizeof(m_CurCPU));
			m_CurCPU.CAX = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCAX;
			m_CurCPU.CCX = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCCX;
			m_CurCPU.CDX = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCDX;
			m_CurCPU.CBX = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBX;
			m_CurCPU.CSP = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP;
			m_CurCPU.CBP = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBP;
			m_CurCPU.CSI = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSI;
			m_CurCPU.CDI = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCDI;
		#ifdef _X64_
			m_CurCPU.R8 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR8;
			m_CurCPU.R9 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR9;
			m_CurCPU.R10 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR10;
			m_CurCPU.R11 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR11;
			m_CurCPU.R12 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR12;
			m_CurCPU.R13 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR13;
			m_CurCPU.R14 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR14;
			m_CurCPU.R15 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR15;
		#endif
			m_CurCPU.CIP = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP;
			m_CurCPU.EFL = *(unsigned long *)gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pEFL;


			memcpy(&m_LastCPU, &m_CurCPU, sizeof(m_LastCPU));
		} else
		{
			X86_CPU_REG CurCPU;
			//memcpy(&CurCPU, &Reg, sizeof(CurCPU));
			CurCPU.CAX = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCAX;
			CurCPU.CCX = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCCX;
			CurCPU.CDX = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCDX;
			CurCPU.CBX = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBX;
			CurCPU.CSP = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP;
			CurCPU.CBP = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBP;
			CurCPU.CSI = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSI;
			CurCPU.CDI = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCDI;
		#ifdef _X64_
			CurCPU.R8 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR8;
			CurCPU.R9 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR9;
			CurCPU.R10 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR10;
			CurCPU.R11 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR11;
			CurCPU.R12 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR12;
			CurCPU.R13 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR13;
			CurCPU.R14 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR14;
			CurCPU.R15 = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR15;
		#endif
			CurCPU.CIP = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP;
			CurCPU.EFL = *(unsigned long *)gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pEFL;

			if (CurCPU.CAX != m_LastCPU.CAX)
			{
				CpuType |= 1;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.CAX;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.CBX != m_LastCPU.CBX)
			{
				CpuType |= 2;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.CBX;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.CCX != m_LastCPU.CCX)
			{
				CpuType |= 4;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.CCX;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.CDX != m_LastCPU.CDX)
			{
				CpuType |= 8;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.CDX;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.CSP != m_LastCPU.CSP)
			{
				CpuType |= 0x10;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.CSP;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.CBP != m_LastCPU.CBP)
			{
				CpuType |= 0x20;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.CBP;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.CSI != m_LastCPU.CSI)
			{
				CpuType |= 0x40;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.CSI;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.CDI != m_LastCPU.CDI)
			{
				CpuType |= 0x80;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.CDI;
				++m_CPURegWriteIndex;
			}
		#ifdef _X64_
			if (CurCPU.R8 != m_LastCPU.R8)
			{
				CpuType |= 0x100;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.R8;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.R9 != m_LastCPU.R9)
			{
				CpuType |= 0x200;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.R9;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.R10 != m_LastCPU.R10)
			{
				CpuType |= 0x400;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.R10;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.R11 != m_LastCPU.R11)
			{
				CpuType |= 0x800;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.R11;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.R12 != m_LastCPU.R12)
			{
				CpuType |= 0x1000;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.R12;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.R13 != m_LastCPU.R13)
			{
				CpuType |= 0x2000;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.R13;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.R14 != m_LastCPU.R14)
			{
				CpuType |= 0x4000;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.R14;
				++m_CPURegWriteIndex;
			}

			if (CurCPU.R15 != m_LastCPU.R15)
			{
				CpuType |= 0x8000;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.R15;
				++m_CPURegWriteIndex;
			}
		#endif
			if (CurCPU.CIP != m_LastCPU.CIP)
			{
				CpuType |= 0x20000;
			//	m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.CIP;
			//	++m_CPURegWriteIndex;
			}

			if (CurCPU.EFL != m_LastCPU.EFL)
			{
				CpuType |= 0x10000;
				m_pCPUReg[m_CPURegWriteIndex % (m_MaxSize*2)] = CurCPU.EFL;
				++m_CPURegWriteIndex;
			}

			memcpy(&m_LastCPU, &CurCPU, sizeof(m_LastCPU));
			m_pCPUType[m_WriteIndex % m_MaxSize] = CpuType;
		}
		m_LastWriteIndex = ++m_WriteIndex % m_MaxSize;
		unsigned long TracesContext = ++m_TracesContext;
		MAX_LIMIT(TracesContext, m_MaxSize);
		return TracesContext;
	}

	void CRunTrace::ReadContext(unsigned long TracePoint, X86_CPU_REG *pCPUReg, unsigned long *pCPUType)
	{
		unsigned long CPURegReadIndex = m_CPURegReadIndex;
		unsigned long CPUType = 0; //???
		memcpy(pCPUReg, &m_CurCPU, sizeof(X86_CPU_REG));
		if (TracePoint)
		{
			unsigned long n;
			for (n = 1; n <= TracePoint; ++n)
			{
				CPUType = m_pCPUType[(n + m_CPURegReadIndex) % m_MaxSize];
				if (CPUType != 0)
				{
					if (CPUType & 1)     pCPUReg->CAX = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 2)     pCPUReg->CBX = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 4)     pCPUReg->CCX = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 8)     pCPUReg->CDX = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 0x10)  pCPUReg->CSP = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 0x20)  pCPUReg->CBP = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 0x40)  pCPUReg->CSI = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 0x80)  pCPUReg->CDI = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
				#ifdef _X64_
					if (CPUType & 0x100) pCPUReg->R8 = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 0x200) pCPUReg->R9 = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 0x400) pCPUReg->R10 = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 0x800) pCPUReg->R11 = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 0x1000) pCPUReg->R12 = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 0x2000) pCPUReg->R13 = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 0x4000) pCPUReg->R14 = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
					if (CPUType & 0x8000) pCPUReg->R15 = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
				#endif
					if (CPUType & 0x10000) pCPUReg->EFL = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
				//	if (CPUType & 0x20000) pCPUReg->CIP = m_pCPUReg[CPURegReadIndex++ % (m_MaxSize*2)];
				}
			}
			if (pCPUType)
				*pCPUType = CPUType;
		} else
		if (pCPUType)
		{
			*pCPUType = m_pCPUType[m_CPURegReadIndex % m_MaxSize];
		}
	}

	unsigned long CRunTrace::Count(unsigned long CpuType) const
	{
		unsigned long n = 0;
		for (int i = 0; i < 32; ++i)
		{
			if (CpuType & 1)
				++n;
			CpuType >>= 1;
		}
		return n;
	}

	void CRunTrace::Rotate()
	{
		X86_CPU_REG CpuReg;
		unsigned long CpuType;
		ReadContext(1, &CpuReg, &CpuType);
		m_nTracesIndex = (m_nTracesIndex+1)%m_MaxSize;
		m_CPURegReadIndex += Count(m_pCPUType[m_nTracesIndex]);
		memcpy(&m_CurCPU, &CpuReg, sizeof(m_CurCPU));
	}

	CTracePoint *CRunTrace::GetTracePoint(unsigned long TracePoint) const
	{
		if (TracePoint >= m_MaxSize || TracePoint >= m_WriteIndex)
			return nullptr;
		return &m_pTraces[(TracePoint+m_nTracesIndex)%m_MaxSize];
	}



#include "x86debugger.hpp"
#include "inteldis.hpp"

	CX86Debugger::CX86Debugger()
	{
	#ifdef _X64_
		m_CodeMode = CODE_MODE_64;
	#else
		m_CodeMode = CODE_MODE_32;
	#endif
	}

	CX86Debugger::~CX86Debugger()
	{
	}

	void CX86Debugger::GetX86RegPtr(X86_CPU_REG_PTR *pCPURegPtr) { }
	void CX86Debugger::GetX86RegPtr(X86_CPU_REG_PTR *pCPURegPtr, int nCPU) { }

	bool CX86Debugger::GetSegRegBase(unsigned long Selector, ULONG_PTR *pAddress, unsigned long *pBits) { return false; }

	bool CX86Debugger::SetX86CodeBreakPoint(BREAK_POINT & BP)
	{
		static const unsigned char int3[] = { 0xCC };

		if (BP.State == BP_STATE_ENABLE)
			return true;

		if (!ReadMemory(BP.Address, (void*)&BP.CCBackup, 1))
			return false;

		if (BP.CCBackup == 0xCC)
			return false;

		return WriteMemory(BP.Address, int3, 1) != 0;
	}

	bool CX86Debugger::ClearX86CodeBreakPoint(BREAK_POINT & BP)
	{
		if (BP.State == BP_STATE_RECOV || BP.State == BP_STATE_DISABLE)
			return true;

		return WriteMemory(BP.Address, (void *)&BP.CCBackup, 1) != 0;
	}


	bool CX86Debugger::SetRecoverBP(BREAK_POINT & BP)
	{
		static const unsigned char int3[] = { 0xCC };

		unsigned char CCBackup;

		if (BP.State != BP_STATE_RECOV)
			return false;

		if (!ReadMemory(BP.Address, &CCBackup, 1))
				return false;

		if (CCBackup != BP.CCBackup)
				return false;

		return WriteMemory(BP.Address, int3, 1) != 0;
	}

	bool CX86Debugger::SetX86DataBreakPoint(BREAK_POINT & BP, ULONG_PTR DRx[], ULONG_PTR *pDR7)
	{
		unsigned long EnableGL = 3; //G,L
		unsigned long DRIndex = 0;
		while (DRIndex < 4)
		{
			if (!(EnableGL & *pDR7))
				break;
			EnableGL *= 4;
			++DRIndex;
		}

		if (DRIndex >= 4)
			return false;

		unsigned long Type = 0;

		if (BP.Type & 0x30)
		{
			Type = 2;
			BP.TypeName = "IO";
		}

		if (BP.Type & 2)
		{
			Type = 0;
			BP.TypeName = "X";
		}

		if ((BP.Type & 4) && !(BP.Type & 8))
		{
			Type = 1;
			BP.TypeName = "W";

		} else
		if (BP.Type & 0xC)
		{
			Type = 3;
			BP.TypeName = "RW";
		}

		if (BP.Size == 1)
		{
			Type |= 0; //0<<2
		} else
		if (BP.Size == 2)
		{
			Type |= 4; //1<<2
		} else
		if (BP.Size == 4)
		{
			Type |= 0xC; //2<<2
		} else
		if ( !(BP.Type & 2) )
		{
			Type |= 8; //
		}

		*(unsigned long*)pDR7 &= ~(EnableGL | (((unsigned long)0xF) << (4 * DRIndex + 16)) | 0xDF00); //11011 1 11 0000 0000
		*(unsigned long*)pDR7 |= (EnableGL | (Type << (4 * DRIndex + 16)) | 0x700); //       1 11 0000 0000
		DRx[DRIndex] = BP.Address;
		BP.X86_DR.DRIndex = DRIndex;
		UpdateRegisters();
		return true;
	}

	bool CX86Debugger::ClearX86DataBreakPoint(BREAK_POINT & BP, ULONG_PTR DRx[], ULONG_PTR *pDR7)
	{
		unsigned long DRIndex = 0;
		while (DRIndex < 4)
		{
			if (DRx[DRIndex] == BP.Address)
				break;
			++DRIndex;
		}

		if (DRIndex < 4)
		{

			*(unsigned long*)pDR7 &= ~((((unsigned long)3) << (2 * DRIndex)) | (((unsigned long)0xF) << (4 * DRIndex + 16)) | 0xDF00);

			if (*pDR7 & 0xFF)
				*pDR7 |= 0x700; //1 11 0000 0000
			DRx[DRIndex] = 0;
		}
		return true;
	}

	bool CX86Debugger::Halt(unsigned long Type)
	{
		if (Type & 0xE000)
		{
			for (unsigned int i = 1; ; i *= 2)
			{
				//???
			}
		}
		return false;
	}

	bool CX86Debugger::IsRecoverBreakPoint()
	{
		map<unsigned long long, BREAK_POINT>::IT it = m_pCodeBPMap->begin();
		//for (; it != m_pCodeBPMap->end(); ++it)
		while (it)
		{
			if (it->second.State == BP_STATE_RECOV)
				return true;
			++it;
		}
		return false;
	}

	unsigned int CX86Debugger::RecoverBreakPoint()
	{
		unsigned int Count = 0;

		map<unsigned long long, BREAK_POINT>::IT it = m_pCodeBPMap->begin();
		//for (; it != m_pCodeBPMap->end(); ++it)
		while (it)
		{
			if (it->second.State == BP_STATE_RECOV)
			{
				if (OpenProcess(it->second.ProcessID))
				{
					if (SetRecoverBP(it->second))
						it->second.State = BP_STATE_ENABLE;
					CloseProcess();
				}
				++Count;
			}
			++it;
		}
		return Count;
	}


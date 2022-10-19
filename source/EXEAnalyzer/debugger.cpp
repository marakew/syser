
#include "debugger.hpp"


	bool CDebugger::CheckAllowType(unsigned int TypeInc, unsigned int TypeExc, const BREAK_POINT & BP)
	{
		if (TypeInc & 0x80000000)
		{
			if (((TypeInc & ~0x80000000) & BP.Type) != (TypeInc & ~0x80000000))
				return false;
		} else
		if (TypeInc && !(TypeInc & BP.Type))
		{
			return false;
		}

		if (TypeExc & 0x80000000)
		{
			if (((TypeExc & ~0x80000000) & BP.Type) == (TypeExc & ~0x80000000))
				return false;
		} else
		if (TypeExc & BP.Type)
		{
			return false;
		}
		return true;
	}

	//CODE

	BREAK_POINT *CDebugger::GetCodeBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc)
	{
		unsigned long long RealAddress;
		if (!GetMemoryAddress(Address, &RealAddress))
			return nullptr;

		map<unsigned long long, BREAK_POINT>::IT it = m_pCodeBPMap->find(RealAddress);
		if (!it)
		//if (it == m_pCodeBPMap->end())
			return nullptr;

		if (!CheckAllowType(TypeInc, TypeExc, it->second))
			return nullptr;

		return &it->second;
	}

	//
	BREAK_POINT *CDebugger::SetCodeBP(ULONG_PTR Address, unsigned int Type, unsigned int State)
	{
		if (!OpenProcess(m_dwProcessId))
		{
			m_Status = BP_STATUS_NO_PID;
			return nullptr;
		}

		unsigned long long RealAddress;
		if (!GetMemoryAddress(Address, &RealAddress))
		{
			CloseProcess();
			m_Status = BP_STATUS_NO_ADDR;
			return nullptr;
		}

		map<unsigned long long, BREAK_POINT>::IT it = m_pCodeBPMap->find(RealAddress);
		if (it)
		//if (it != m_pCodeBPMap->end())
		{
			CloseProcess();
			m_Status = BP_STATUS_EXIST_BP;
			return nullptr;
		}

		map<unsigned long long, BREAK_POINT>::IT iit = m_pCodeBPMap->insert(_Tmap_pair<unsigned long long, BREAK_POINT>(RealAddress,BREAK_POINT()));
		iit->second.Address = Address;
		iit->second.Type = Type;
		iit->second.State = BP_STATE_DISABLE;
		iit->second.Size = 0;
		iit->second.PatchFN = 0;
		iit->second.ProcessID = m_dwProcessId;
		iit->second.Owner = m_MainModuleName;
		iit->second.pModule = nullptr;
		iit->second.HitCount = 0;

		if (State != BP_STATE_ENABLE || SetCodeBreakPoint(iit->second))
		{
			iit->second.State = State;
			CloseProcess();
			m_Status = BP_STATUS_NEW_BP;
			return &iit->second;
		} else
		{
			m_pCodeBPMap->erase(iit);
			CloseProcess();
			m_Status = BP_STATUS_FAIL_ADD_BP;
			return nullptr;
		}
		//???
	}

	//
	bool CDebugger::DelCodeBP(BREAK_POINT & BP)
	{
#if 1
		map<unsigned long long, BREAK_POINT>::IT it = &BP; //???

		if (!OpenProcess(BP.ProcessID))
			return false;

		unsigned long long RealAddress;
		if (!GetMemoryAddress(it->second.Address, &RealAddress) || (RealAddress != it->first)) //???
		{
			CloseProcess();
			return false;
		}
#else
		map<unsigned long long, BREAK_POINT>::IT it = m_pCodeBPMap->begin();
		while (it != m_pCodeBPMap->end())
		{
			if (it->second == BP) break;
			++it;
		}

		if (it == m_pCodeBPMap->end())
			return false;
#endif
		if (it->second.State != BP_STATE_ENABLE || ClearCodeBreakPoint(it->second))
		{
			m_pCodeBPMap->erase(it);
			CloseProcess();
			return true;
		} else
		{
			CloseProcess();
			return false;
		}
	}
	bool CDebugger::DelCodeBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc)
	{
		if (!OpenProcess(m_dwProcessId))
		{
			m_Status = BP_STATUS_NO_PID;
			return false;
		}

		unsigned long long RealAddress;
		if (!GetMemoryAddress(Address, &RealAddress))
		{
			CloseProcess();
			m_Status = BP_STATUS_NO_ADDR;
			return false;
		}

		map<unsigned long long, BREAK_POINT>::IT it = m_pCodeBPMap->find(RealAddress);
		if (!it)
		//if (it == m_pCodeBPMap->end())
		{
			CloseProcess();
			m_Status = BP_STATUS_NOT_EXIST_BP;
			return false;
		}

		if (!CheckAllowType(TypeInc, TypeExc, it->second))
		{
			CloseProcess();
			return false;
		}

		if (it->second.State != BP_STATE_ENABLE || ClearCodeBreakPoint(it->second))
		{
			m_pCodeBPMap->erase(it);
			CloseProcess();
			m_Status = 0; //STATUS_OK
			return true;
		} else
		{
			CloseProcess();
			m_Status = BP_STATUS_FAIL_DEL_BP;
			return false;
		}
	}

	//
	int CDebugger::DelAllCodeBP(unsigned int TypeInc, unsigned int TypeExc)
	{
		int Count = 0;
		map<unsigned long long, BREAK_POINT>::IT it = m_pCodeBPMap->begin();
		//for (; it != m_pCodeBPMap->end(); )
		while (it)
		{
			if (CheckAllowType(TypeInc, TypeExc, it->second))
			{
				if (DelCodeBP(it++->second))
					Count++;
			} else
			{
				it++;
			}
		}
		return Count;
	}

	//
	int CDebugger::EnableAllCodeBP(unsigned int TypeInc, unsigned int TypeExc)
	{
		int Count = 0;
		for (map<unsigned long long, BREAK_POINT>::IT it = m_pCodeBPMap->begin();
			it != m_pCodeBPMap->end(); ++it)
		{
			if (CheckAllowType(TypeInc, TypeExc, it->second))
			{
				if (EnableCodeBP(it->second))
					Count++;
			}
		}
		return Count;
	}
	int CDebugger::DisableAllCodeBP(unsigned int TypeInc, unsigned int TypeExc)
	{
		int Count = 0;
		for (map<unsigned long long, BREAK_POINT>::IT it = m_pCodeBPMap->begin();
			it != m_pCodeBPMap->end(); ++it)
		{
			if (CheckAllowType(TypeInc, TypeExc, it->second))
			{
				if (DisableCodeBP(it->second))
					Count++;
			}
		}
		return Count;
	}

	//
	bool CDebugger::EnableCodeBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc)
	{
		BREAK_POINT *BP = GetCodeBP(Address, TypeInc, TypeExc);
		if (BP)
			return EnableCodeBP(*BP);
		return false;
	}
	bool CDebugger::EnableCodeBP(BREAK_POINT & BP)
	{
		if (BP.State == BP_STATE_ENABLE || BP.State == BP_STATE_RECOV)
			return true;

		if (!OpenProcess(BP.ProcessID))
			return false;

		if (SetCodeBreakPoint(BP))
		{
			BP.State = BP_STATE_ENABLE;
			CloseProcess();
			return true;
		} else
		{
			CloseProcess();
			return false;
		}
	}

	//
	bool CDebugger::DisableCodeBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc)
	{
		BREAK_POINT *BP = GetCodeBP(Address, TypeInc, TypeExc);
		if (BP)
			return DisableCodeBP(*BP);
		return false;
	}

	bool CDebugger::DisableCodeBP(BREAK_POINT & BP)
	{
		if (BP.State == BP_STATE_DISABLE)
			return true;

		if (BP.State == BP_STATE_ENABLE)
		{
			if (!OpenProcess(BP.ProcessID))
				return false;

			if (!ClearCodeBreakPoint(BP))
			{
				CloseProcess();
				return false;
			}
			CloseProcess();
		}
		BP.State = BP_STATE_DISABLE;
		return true;
	}

	//DATA
	BREAK_POINT *CDebugger::GetDataBP1(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc)
	{
		TListIter<BREAK_POINT> it = m_pDataBPList->Begin();
		while (it != m_pDataBPList->End())
		{
			if (it->Address == Address && CheckAllowType(TypeInc, TypeExc, *it))
			{
				return &*it;
			}
			++it;
		}
		return nullptr;
	}

	BREAK_POINT *CDebugger::SetDataBP(ULONG_PTR Address, unsigned int Type, unsigned int State, unsigned int Size)
	{
		TListIter<BREAK_POINT> it = m_pDataBPList->Append();
		it->Address = Address;
		it->Type = Type;
		it->State = BP_STATE_DISABLE;
		it->Size = Size;
		it->PatchFN = 0;
		it->ProcessID = m_dwProcessId;
		it->HitCount = 0;
		it->Owner = m_MainModuleName;

		if (State != BP_STATE_ENABLE || SetDataBreakPoint(*it))
		{
			it->State = State;
			return &*it;
		} else
		{
			m_pDataBPList->Remove(it);
			return nullptr;
		}
	}

	//
	bool CDebugger::DelDataBP(BREAK_POINT & BP)
	{
		TListIter<BREAK_POINT> it = &BP;

		if (BP.State == BP_STATE_ENABLE && !ClearDataBreakPoint(BP))
			return false;

		m_pDataBPList->Remove(it);
		return true;

	}
	int CDebugger::DelDataBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc)
	{
		int Count = 0;
		TListIter<BREAK_POINT> it = m_pDataBPList->Begin();
		for (; it != m_pDataBPList->End(); ++it)
		{
			if (it->Address == Address && CheckAllowType(TypeInc, TypeExc, *it))
			{
				DelDataBP(*it);
				Count++;
			}
		}
		return Count;
	}

	//
	int CDebugger::DelAllDataBP(unsigned int TypeInc, unsigned int TypeExc)
	{
		int Count = 0;
		TListIter<BREAK_POINT> it = m_pDataBPList->Begin();
		for (; it != m_pDataBPList->End(); )
		{
			if (CheckAllowType(TypeInc, TypeExc, *it))
			{
				DelDataBP(*it++);
				Count++;
			} else
			{
				it++;
			}
		}
		return Count;

	}

	//
	int CDebugger::EnableAllDataBP(unsigned int TypeInc, unsigned int TypeExc)
	{
		int Count = 0;
		TListIter<BREAK_POINT> it = m_pDataBPList->Begin();
		for (; it != m_pDataBPList->End(); ++it)
		{
			if (CheckAllowType(TypeInc, TypeExc, *it))
			{
				if (EnableDataBP(*it))
					Count++;
			}
		}
		return Count;
	}
	int CDebugger::DisableAllDataBP(unsigned int TypeInc, unsigned int TypeExc)
	{
		int Count = 0;
		TListIter<BREAK_POINT> it = m_pDataBPList->Begin();
		for (; it != m_pDataBPList->End(); ++it)
		{
			if (CheckAllowType(TypeInc, TypeExc, *it))
			{
				if (DisableDataBP(*it))
					Count++;
			}
		}
		return Count;
	}

	//
	int CDebugger::EnableDataBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc)
	{
		int Count = 0;
		TListIter<BREAK_POINT> it = m_pDataBPList->Begin();
		for (; it != m_pDataBPList->End(); ++it)
		{
			if (it->Address == Address && CheckAllowType(TypeInc, TypeExc, *it))
			{
				if (EnableDataBP(*it))
					Count++;
			}
		}
		return Count;
	}
	bool CDebugger::EnableDataBP(BREAK_POINT & BP)
	{
		if (BP.State == BP_STATE_ENABLE || BP.State == BP_STATE_RECOV)
			return true;
		if (!SetDataBreakPoint(BP))
			return false;
		BP.State = BP_STATE_ENABLE;
		return true;
	}

	//
	int CDebugger::DisableDataBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc)
	{
		int Count = 0;
		TListIter<BREAK_POINT> it = m_pDataBPList->Begin();
		for (; it != m_pDataBPList->End(); ++it)
		{
			if (it->Address == Address && CheckAllowType(TypeInc, TypeExc, *it))
			{
				if (DisableDataBP(*it))
					Count++;
			}
		}
		return Count;
	}
	bool CDebugger::DisableDataBP(BREAK_POINT & BP)
	{
		if (BP.State == BP_STATE_DISABLE)
			return true;
		if (BP.State == BP_STATE_ENABLE && !ClearDataBreakPoint(BP))
			return false;
		BP.State = BP_STATE_DISABLE;
		return true;
	}

	//
	int CDebugger::GetDataBP(unsigned int TypeInc, unsigned int TypeExc, TList<BREAK_POINT> *pBPList)
	{
		int Count = 0;
		TListIter<BREAK_POINT> it = m_pDataBPList->Begin();
		for (; it != m_pDataBPList->End(); ++it)
		{
			if (CheckAllowType(TypeInc, TypeExc, *it))
			{
				if (pBPList) pBPList->Append(&*it);
				Count++;
			}
		}
		return Count;
	}

	CDebugger::CDebugger()
		: m_dwProcessId(0)
		, m_dwThreadId(0)
		, m_pCIP(nullptr)
		, m_pDebugInterface(nullptr)
		, m_Status(0)
		, m_pCodeBPMap(&m_CodeBPMap)
		, m_pDataBPList(&m_DataBPList)
	{
	}

	CDebugger::~CDebugger()
	{
	}

	unsigned long CDebugger::Read(ULONG_PTR Address, void *Buffer, unsigned long Size, void *Obj)
	{
		CDebugger *pDebugger = (CDebugger *)Obj;
		return pDebugger->ReadMemory(Address, Buffer, Size);
	}

	bool CDebugger::Init(CDebugInterface *pDebugInterface)
	{
		m_pDebugInterface = pDebugInterface;
		return true;
	}

	void CDebugger::Release()
	{
		m_pDebugInterface = nullptr;
	}

	bool CDebugger::Open(const char *Name) //TODO PARAM
	{
		return true;
	}

	bool CDebugger::Close()
	{
		for (map<CMemoryImage, CDbgModule>::IT it = m_DbgModuleList.m_DbgModuleMap.begin();
			it != m_DbgModuleList.m_DbgModuleMap.end(); ++it)
		{
			if (it->second.m_pSymbolModule) (it->second.m_pSymbolModule->m_RefCount)--;
			if (it->second.m_pCommentModule) (it->second.m_pCommentModule->m_RefCount)--;
		}

		if (m_pDebugInterface)
		{
			if (m_pDebugInterface->m_pSymbolContainer)
				m_pDebugInterface->m_pSymbolContainer->Reset();
			if (m_pDebugInterface->m_pCommentContainer)
				m_pDebugInterface->m_pCommentContainer->Reset();
		}
		m_DbgModuleList.m_DbgModuleMap.clear();
		m_MainModuleName.Empty();
		return true;
	}

	unsigned long CDebugger::WriteMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size)
	{
		return 0;
	}

	unsigned long CDebugger::ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size)
	{
		return 0;
	}

	unsigned long CDebugger::MoveMemory(ULONG_PTR Des, ULONG_PTR Src, unsigned long Length)
	{
		unsigned char Buffer[256];
		unsigned long Count = 0;
		while (Length)
		{
			unsigned long Size = MIN(Length, lenof(Buffer));

			unsigned long RSize = ReadMemory(Src + Count, Buffer, Size);
			if (RSize != Size)
				return Count;

			unsigned long WSize = WriteMemory(Des + Count, Buffer, Size);
			if (WSize != Size)
				return Count;

			Count += Size;
			if (Length < Size)
				break;
			Length -= Size;
		}
		return Count;
	}

	unsigned long CDebugger::CompareMemory(ULONG_PTR Des, ULONG_PTR Src, unsigned long Length)
	{
		if (Length == 0)
			return 0;

		unsigned char *pSrcBuf = new unsigned char[Length];
		unsigned char *pDesBuf = new unsigned char[Length];

		unsigned long RSrcSize = ReadMemory(Src, pSrcBuf, Length);
		MAX_LIMIT(Length, RSrcSize);

		unsigned long RDesSize = ReadMemory(Des, pDesBuf, Length);
		MAX_LIMIT(Length, RDesSize);

		unsigned long i;
		for (i = 0; i < Length; ++i)
		{
			if (pSrcBuf[i] != pDesBuf[i])
				break;
		}

		delete []pSrcBuf;
		delete []pDesBuf;
		return i;
	}

	ULONG_PTR CDebugger::SearchMemory(ULONG_PTR Address, unsigned long Length, const void *Pattern, unsigned long PatternLength, bool bCaseCmp)
	{
		unsigned char PatternBuffer[4096];

		if (PatternLength > 4096)
			return -1;

		unsigned long next = 0;
		unsigned long last = 0;
		for (unsigned long i = 0; i < Length - PatternLength; ++i)
		{
			if (next < PatternLength + i)
			{
				unsigned long RDSize = MIN(Length - i, 4096);
				ULONG_PTR offset = Address + i;
				last = i;
				RDSize = ReadMemory(offset, PatternBuffer, RDSize);
				if (RDSize == 0)
				{
					i = (offset & ~((ULONG_PTR)4096-1)) + 4096 - Address - 1;
					next = i;
					continue;
				}
				next = RDSize + i;
			}

			if (bCaseCmp)
			{
				if (!memcmp(&PatternBuffer[i - last], Pattern, PatternLength))
					return Address + i;
			} else
			if (!TMemICmp((char*)&PatternBuffer[i - last], (char*)Pattern, PatternLength))
			{
				return Address + i;
			}
		}
		return -1;
	}

	bool CDebugger::SetSingleStep()
	{
		return true;
	}
	bool CDebugger::RemoveSingleStep()
	{
		return true;
	}

	bool CDebugger::SaveRegisters()
	{
		return true;
	}

	bool CDebugger::UpdateRegisters()
	{
		return true;
	}

	bool CDebugger::SetCodeBreakPoint(BREAK_POINT & BP)
	{
		return true;
	}
	bool CDebugger::ClearCodeBreakPoint(BREAK_POINT & BP)
	{
		return true;
	}
	bool CDebugger::SetDataBreakPoint(BREAK_POINT & BP)
	{
		return true;
	}
	bool CDebugger::ClearDataBreakPoint(BREAK_POINT & BP)
	{
		return true;
	}

	bool CDebugger::GetMemoryAddress(ULONG_PTR Address, unsigned long long *RealAddress)
	{
		*RealAddress = (unsigned long long)Address;
		return true;
	}

	bool CDebugger::OpenProcess(unsigned long dwProcessId)
	{
		return true;
	}

	void CDebugger::CloseProcess()
	{
	}

	bool CDebugger::Pause()
	{
		return true;
	}

	bool CDebugger::ContinueDebug(bool bRestoreScr)
	{
		return false;
	}
#if 0
	bool CDebugger::GetStack(STACK_FRAME *pStackFrame)
	{
		return false;
	}
#endif
///
	bool CDebugInterface::Continue()
	{
		switch (m_State)
		{
		case 0:
			return true;

		case TRACE_RANGE_IN:
			if (*GetDebugger()->m_pCIP < m_MinCIP || *GetDebugger()->m_pCIP >= m_MaxCIP)
			{
				RunCmd("T");
				return false;
			}
			Reset();
			return true;
		case TRACE_RANGE_OUT:
			if (*GetDebugger()->m_pCIP >= m_MinCIP && *GetDebugger()->m_pCIP < m_MaxCIP)
			{
				RunCmd("T");
				return false;
			}
			Reset();
			return true;

		case STEP_RANGE_IN:
			if (*GetDebugger()->m_pCIP < m_MinCIP || *GetDebugger()->m_pCIP >= m_MaxCIP)
			{
				RunCmd("P");
				return false;
			}
			Reset();
			return true;
		case STEP_RANGE_OUT:
			if (*GetDebugger()->m_pCIP >= m_MinCIP && *GetDebugger()->m_pCIP < m_MaxCIP)
			{
				RunCmd("P");
				return false;
			}
			Reset();
			return true;

		case TRACE_UNTIL:
			if (--m_Until > 0)
			{
				RunCmd("T");
				return false;
			}
			Reset();
			return true;

		case STEP_UNTIL:
			if (--m_Until > 0)
			{
				RunCmd("P");
				return false;
			}
			Reset();
			return true;

		case TRACE_CONDITION:
			OnTestCondition();
			if (!TestCondition(m_Condition, 0))
			{
				if (++m_Until < 0x100000)
				{
					RunCmd("T");
					return false;
				}
				Reset();
				return false;
			}
			Reset();
			return true;

		case STEP_CONDITION:
			OnTestCondition();
			if (!TestCondition(m_Condition, 0))
			{
				if (++m_Until < 0x100000)
				{
					RunCmd("P");
					return false;
				}
				Reset();
				return false;
			}
			Reset();
			return true;

		case RETURN_UNTIL:
			unsigned char Data;

			if (m_pCodeDoc->ReadMemory(*GetDebugger()->m_pCIP, &Data, 1))
			{
				//ProcessPCommandRetCode
				switch (Data)
				{
				case 0xC2: //RETN
				case 0xC3: //RETN imm16
				case 0xCA: //RETF imm16
				case 0xCB: //RETF
				case 0xCF: //IRET/IRETD
					Reset();
					return true;
				}
			}

			if (++m_Until < 0x100000)
			{
				RunCmd("P");
				return false;
			}

			Reset();
			return false;

		case BPR_IN:
			return BreakBPR();

		case RESET_STATE:
			Reset();
			return false;

		default:
			return false;
		}
		return false; //???
	}

	void CDebugInterface::Trace(ULONG_PTR MinCIP, ULONG_PTR MaxCIP, bool State)
	{
		m_State = State ? TRACE_RANGE_IN:TRACE_RANGE_OUT;
		m_MinCIP = MinCIP;
		m_MaxCIP = MaxCIP;
	}

	void CDebugInterface::Step(ULONG_PTR MinCIP, ULONG_PTR MaxCIP, bool State)
	{
		m_State = State ? STEP_RANGE_IN:STEP_RANGE_OUT;
		m_MinCIP = MinCIP;
		m_MaxCIP = MaxCIP;
	}

	void CDebugInterface::Trace(const char *Condition)
	{
		m_State = TRACE_CONDITION;
		m_Condition = Condition;
		m_Until = 0;
	}

	void CDebugInterface::Step(const char *Condition)
	{
		m_State = STEP_CONDITION;
		m_Condition = Condition;
		m_Until = 0;
	}

	void CDebugInterface::SetBPR(ULONG_PTR PageAddress, ULONG_PTR Address) //++
	{
		m_State = BPR_IN;
		m_PageAddress = PageAddress;
		m_Address = Address;
	}

	void CDebugInterface::Trace(unsigned long Count)
	{
		m_State = TRACE_UNTIL;
		m_Until = Count;
	}

	void CDebugInterface::Step(unsigned long Count)
	{
		m_State = STEP_UNTIL;
		m_Until = Count;
	}

	void CDebugInterface::Return(CDebugger *pDebugger)
	{
		m_State = RETURN_UNTIL;
		m_Until = 0;
		m_pReturn = pDebugger;
	}

	void CDebugInterface::Reset()
	{
		m_State = 0;
		m_Until = 0;
		m_MaxCIP = 0;
		m_MinCIP = 0;
		m_Condition.Empty();
	}

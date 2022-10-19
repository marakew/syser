
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "syserui.hpp"
#include "../EXEAnalyzer/pefile.hpp"
#include "syserconfig.hpp"
#include "../Code/sysdep.hpp"
#include "../Code/imagefile.hpp"

	CSyserCodeDoc::CSyserCodeDoc()
	{
		AddFunc(":", 3, Reg, this);
	}

	TExpCalc<char,ULONG_PTR,1>::EXPITEM * CSyserCodeDoc::Reg(TListIter<TExpCalc<char,ULONG_PTR,1>::EXPITEM> It, TList<TExpCalc<char,ULONG_PTR,1>::EXPITEM> & Expression, TExpCalc<char,ULONG_PTR,1> *pThis)
	{
		if (!TExpCalc<char,ULONG_PTR,1>::CheckResult(0, It, 1, Expression))
			return nullptr;
		if (It-1 == Expression.End())
			return nullptr;
		TExpCalc<char,ULONG_PTR,1>::EXPITEM *ExpItem = It.Prev();
		for (unsigned long i = 0; i < 5; ++i) //SEG_IDX_MAX lenof(CInstrSym::m_SegStr)
		{
			if (!TStrNICmp(CInstrSym::m_SegStr[i], ExpItem->Str, 2))
			{
				ExpItem->Value = 0;
				gpSyser->m_pDebugger->GetSegRegBase(i, &ExpItem->Value, 0);
				ExpItem->Value += It.Next()->Value;
				TExpCalc<char,ULONG_PTR,1>::RemoveExpr(It, 2, Expression);
				return ExpItem;
			}
		}
		return nullptr;
	}

	void CSyserCodeDoc::InsertX86RegSym(X86_CPU_REG_PTR *pCPURegPtr)
	{
	#ifdef _X64_
		TExpCalc<char,ULONG_PTR,1>::InsertSym("RAX", pCPURegPtr->pCAX, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("RBX", pCPURegPtr->pCBX, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("RCX", pCPURegPtr->pCCX, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("RDX", pCPURegPtr->pCDX, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("RBP", pCPURegPtr->pCBP, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("RSP", pCPURegPtr->pCSP, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("RSI", pCPURegPtr->pCSI, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("RDI", pCPURegPtr->pCDI, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("RIP", pCPURegPtr->pCIP, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R8", pCPURegPtr->pR8, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R9", pCPURegPtr->pR9, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R10", pCPURegPtr->pR10, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R11", pCPURegPtr->pR11, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R12", pCPURegPtr->pR12, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R13", pCPURegPtr->pR13, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R14", pCPURegPtr->pR14, 8);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R15", pCPURegPtr->pR15, 8);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("EAX", pCPURegPtr->pCAX, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("EBX", pCPURegPtr->pCBX, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("ECX", pCPURegPtr->pCCX, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("EDX", pCPURegPtr->pCDX, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("EBP", pCPURegPtr->pCBP, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("ESP", pCPURegPtr->pCSP, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("ESI", pCPURegPtr->pCSI, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("EDI", pCPURegPtr->pCDI, 4);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("EIP", pCPURegPtr->pCIP, 4);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("EFLAG", (unsigned long long*)pCPURegPtr->pEFL, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("EFL", (unsigned long long*)pCPURegPtr->pEFL, 4);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("R8D", pCPURegPtr->pR8, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R9D", pCPURegPtr->pR9, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R10D", pCPURegPtr->pR10, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R11D", pCPURegPtr->pR11, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R12D", pCPURegPtr->pR12, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R13D", pCPURegPtr->pR13, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R14D", pCPURegPtr->pR14, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R15D", pCPURegPtr->pR15, 4);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("AX", pCPURegPtr->pCAX, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("BX", pCPURegPtr->pCBX, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("CX", pCPURegPtr->pCCX, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("DX", pCPURegPtr->pCDX, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("BP", pCPURegPtr->pCBP, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("SP", pCPURegPtr->pCSP, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("SI", pCPURegPtr->pCSI, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("DI", pCPURegPtr->pCDI, 2);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("IP", pCPURegPtr->pCIP, 2);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("R8W", pCPURegPtr->pR8, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R9W", pCPURegPtr->pR9, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R10W", pCPURegPtr->pR10, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R11W", pCPURegPtr->pR11, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R12W", pCPURegPtr->pR12, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R13W", pCPURegPtr->pR13, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R14W", pCPURegPtr->pR14, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R15W", pCPURegPtr->pR15, 2);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("AH", pCPURegPtr->pCAX + 1, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("BH", pCPURegPtr->pCBX + 1, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("CH", pCPURegPtr->pCCX + 1, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("DH", pCPURegPtr->pCDX + 1, 1);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("AL", pCPURegPtr->pCAX, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("BL", pCPURegPtr->pCBX, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("CL", pCPURegPtr->pCCX, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("DL", pCPURegPtr->pCDX, 1);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("BPL", pCPURegPtr->pCBP, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("SPL", pCPURegPtr->pCSP, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("SIL", pCPURegPtr->pCSI, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("DIL", pCPURegPtr->pCDI, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R8B", pCPURegPtr->pR8, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R9B", pCPURegPtr->pR9, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R10B", pCPURegPtr->pR10, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R11B", pCPURegPtr->pR11, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R12B", pCPURegPtr->pR12, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R13B", pCPURegPtr->pR13, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R14B", pCPURegPtr->pR14, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("R15B", pCPURegPtr->pR15, 1);
	#else
		TExpCalc<char,ULONG_PTR,1>::InsertSym("EAX", pCPURegPtr->pCAX, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("EBX", pCPURegPtr->pCBX, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("ECX", pCPURegPtr->pCCX, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("EDX", pCPURegPtr->pCDX, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("EBP", pCPURegPtr->pCBP, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("ESP", pCPURegPtr->pCSP, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("ESI", pCPURegPtr->pCSI, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("EDI", pCPURegPtr->pCDI, 4);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("EIP", pCPURegPtr->pCIP, 4);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("EFLAG", (ULONG_PTR*)pCPURegPtr->pEFL, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("EFL", (ULONG_PTR*)pCPURegPtr->pEFL, 4);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("AX", pCPURegPtr->pCAX, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("BX", pCPURegPtr->pCBX, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("CX", pCPURegPtr->pCCX, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("DX", pCPURegPtr->pCDX, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("BP", pCPURegPtr->pCBP, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("SP", pCPURegPtr->pCSP, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("SI", pCPURegPtr->pCSI, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("DI", pCPURegPtr->pCDI, 2);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("IP", pCPURegPtr->pCIP, 2);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("AH", pCPURegPtr->pCAX + 1, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("BH", pCPURegPtr->pCBX + 1, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("CH", pCPURegPtr->pCCX + 1, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("DH", pCPURegPtr->pCDX + 1, 1);

		TExpCalc<char,ULONG_PTR,1>::InsertSym("AL", pCPURegPtr->pCAX, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("BL", pCPURegPtr->pCBX, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("CL", pCPURegPtr->pCCX, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("DL", pCPURegPtr->pCDX, 1);
	#endif
	}

	bool CSyserCodeDoc::Open(const char *szModule) //04
	{
		if (!gpSyser->m_pDebugger || m_bOpen)
			return false;
		gpSyser->m_SyserUI.m_DebugInterface.Reset();
		if (!gpSyser->m_pDebugger->Open(szModule))
			return false;
		gpSyser->m_pSysDebugger = gpSyser->m_pDebugger;
		CCodeDoc::Open(szModule);
		gpSyser->m_pDebugger->GetX86RegPtr(&m_CPUREGPTR);
		m_ExeFilePath = szModule;
		InsertX86RegSym(&m_CPUREGPTR);
		
		TExpCalc<char,ULONG_PTR,1>::InsertSym("OPCODE", (ULONG_PTR*)&m_OPCODE, 1);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("OPCODE2", (ULONG_PTR*)&m_OPCODE, 2);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("OPCODE3", (ULONG_PTR*)&m_OPCODE, 3);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("OPCODE4", (ULONG_PTR*)&m_OPCODE, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("PID", (ULONG_PTR*)&m_PID, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("TID", (ULONG_PTR*)&m_TID, 4);
		TExpCalc<char,ULONG_PTR,1>::InsertSym("BPCOUNT", (ULONG_PTR*)&m_BPCOUNT, 4);

		for (map<TWideString, SYSER_PLUGIN_MODULE>::IT it = gpSyser->m_PluginMap.begin();
			it != gpSyser->m_PluginMap.end(); ++it)
		{
			if (it->second.fpOnDebuggerOpen)
				it->second.fpOnDebuggerOpen();
		}

		gpSyser->m_MainFrame.m_SystemExplorer.UpdateMenu();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateMenu();

		gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);

		gpSyser->AddHistory(szModule, 0);

		return true;
	}
	void CSyserCodeDoc::Close()			//08
	{
		if (m_bOpen)
		{
			for (map<TWideString, SYSER_PLUGIN_MODULE>::IT it = gpSyser->m_PluginMap.begin();
				it != gpSyser->m_PluginMap.end(); ++it)
			{
				if (it->second.fpOnDebuggerClose)
					it->second.fpOnDebuggerClose();
			}

			if (gpSyser->m_pDebugger)
				gpSyser->m_pDebugger->Close();

			for (map<FILESIG, CSDSModule>::IT it = gpSyser->m_SyserUI.m_SDSModulesMap.begin();
				it != gpSyser->m_SyserUI.m_SDSModulesMap.end();++it)
			{
				gpSyser->m_SyserUI.UnloadSDSModule(&it->second);
			}

			TExpCalc<char,ULONG_PTR,1>::Reset();

			CCodeDoc::Close();

			if (gpSyser->m_bActive && gpSyser->m_MainFrame.IsWindow())
			{
				gpSyser->m_MainFrame.m_SystemExplorer.ResetContext();
				gpSyser->m_MainFrame.m_SystemExplorer.UpdateMenu();
				gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateMenu();
				gpSyser->m_MainFrame.Update(&gpSyser->m_MainFrame.m_ClientRect);
				gpSyser->m_MainFrame.m_StaticText.SetWindowText(WSTR(""));
			}

			memset(&m_CPUREGPTR, 0, sizeof(m_CPUREGPTR));

			//m_ExeFilePath.Empty(); //???
		}
	}
	unsigned long CSyserCodeDoc::ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size) //0C
	{
		if (!m_bOpen)
			return 0;

		unsigned long Len = gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size);
		if (Len)
		{
			if (!gSyserConfig.iShowCCByte && Size)
			{
				for (unsigned long i = 0; i < Size; ++i)
				{
					if (static_cast<unsigned char *>(Buffer)[i] == 0xCC)
					{
						BREAK_POINT *BreakPoint = gpSyser->m_pDebugger->GetCodeBP(Address+i, 0, 0);
						if (BreakPoint && BreakPoint->State == BP_STATE_ENABLE)
							static_cast<unsigned char *>(Buffer)[i] = BreakPoint->CCBackup;
					}
				}
			}
		}
		return Len;
	}
	ULONG_PTR CSyserCodeDoc::GetInstrAddress(ULONG_PTR Address, long Count) //10
	{
		if (Count < 0)
		{
			Count = -Count;
			do {
				unsigned long Len = CInstrSym::PrevInstrLen(Address);
				MIN_LIMIT(Len, 1);
				Address -= Len;
				--Count;
			} while (Count);
		} else
		if (Count > 0)
		{
			Count = Count;
			do {
				unsigned long Len = CInstrSym::InstrLen(Address, MAX_INSTR_LEN);
				MIN_LIMIT(Len, 1);
				Address += Len;
				--Count;
			} while (Count);
		}
		return Address;
	}

	bool CSyserCodeDoc::GetSymbol(ULONG_PTR Address, WCHAR *szName, int Length) //14
	{
		char Buffer[256];
		if (gpSyser->m_SyserUI.GetSymbol(Address, Buffer, 256))
		{
			AnsiToUnicode(Buffer, szName, Length);
			return true;
		}
		return false;
	}
	bool CSyserCodeDoc::GetComment(ULONG_PTR Address, WCHAR *szName, int Length) //18
	{
		if (!m_bOpen)
			return false;
		const char *pStr = gpSyser->m_pDebugger->m_DbgModuleList.GetComment(Address);
		if (!pStr)
		{
			if (gpSyser->m_pDebugger == gpSyser->m_pSysDebugger)
				return false;
			pStr = gpSyser->m_pSysDebugger->m_DbgModuleList.GetComment(Address);
			if (!pStr)
				return false;
		}
		TStrCpyLimit(szName, pStr, Length);
		return true;
	}
	bool CSyserCodeDoc::SetComment(ULONG_PTR Address, const WCHAR *szName) //1C
	{
		if (!m_bOpen)
			return false;
		CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(Address);
		if (!pDbgMod)
			return false;
		if (!pDbgMod->m_pCommentModule && !gpSyser->m_SyserUI.m_CommentContainer.FindModule(pDbgMod->m_TimeStamp, pDbgMod->m_CheckSum))
		{
			CCommentModule *cmod = gpSyser->m_SyserUI.m_CommentContainer.InsertModule(pDbgMod->m_TimeStamp, pDbgMod->m_CheckSum);
			if (!cmod)
				return false;
			cmod->m_ModuleName = pDbgMod->m_FullFileName.operator const char*(); //???
			gpSyser->m_SyserUI.SetCommentModule(cmod);
		}
		char Buffer[128];
		UnicodeToAnsi(szName, Buffer, lenof(Buffer));
		if (!pDbgMod->m_pCommentModule)
			return false;
		return pDbgMod->m_pCommentModule->Set(Address - pDbgMod->m_ImageBase, Buffer);
	}

	//bool CSyserCodeDoc::GetFunction(ULONG_PTR Address, WCHAR *szName, int Length) //20

	unsigned long CSyserCodeDoc::GetPic(ULONG_PTR Address) //24
	{
		unsigned long Type = 0;
		if (!m_bOpen)
			return Type;

		if (Address == *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP)
			Type |= 1;

		BREAK_POINT *BreakPoint = gpSyser->m_pDebugger->GetCodeBP(Address, 0, 0x300);
		if (BreakPoint)
		{
			if (BreakPoint->State == BP_STATE_ENABLE)
				Type |= 2;
			else
			if (BreakPoint->State == BP_STATE_RECOV)
				Type |= 2;
			else
			if (BreakPoint->State == BP_STATE_DISABLE)
				Type |= 4;
			
		} else
		{
			map<ULONG_PTR, BREAK_POINT::MODULE>::IT it = gpSyser->m_SyserUI.m_ModuleBPMap.find(Address);
			if (it != gpSyser->m_SyserUI.m_ModuleBPMap.end())
				Type |= (it->second.State == BP_STATE_DISABLE) ? 4:2;
		}

		if (gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.IsBM(Address))
			Type |= 8;

		return Type;
	}

	bool CSyserCodeDoc::AddrToSym(ULONG_PTR Address, char *szSym, unsigned int SymLen)
	{
		return gpSyser->m_SyserUI.GetSymbol(Address, szSym, SymLen);
	}

	bool CSyserCodeDoc::GetSymbolBase(ULONG_PTR *BaseAddr)
	{
		if (gpSyser->m_pDebugger->m_DbgModuleList.GetSymbolBase(BaseAddr))
			return true;
		if (gpSyser->m_pDebugger != gpSyser->m_pSysDebugger)
		{
			if (gpSyser->m_pSysDebugger->m_DbgModuleList.GetSymbolBase(BaseAddr))
				return true;
		}
		return false;
	}

	bool CSyserCodeDoc::GetValue(const char *szItem, ULONG_PTR *pValue) const
	{
	//	if (!gpSyser->m_pDebugger)
	//		return false;

		if (TExpCalc<char,ULONG_PTR,1>::GetValue(szItem, pValue))
			return true;

		if (!gpSyser->m_pDebugger)
			return false;

		if (gpSyser->m_pDebugger != gpSyser->m_pSysDebugger)
		{
			for (map<CMemoryImage, CDbgModule>::IT it = gpSyser->m_pSysDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
				it != gpSyser->m_pSysDebugger->m_DbgModuleList.m_DbgModuleMap.end(); ++it)
			{
				if (it->second.GetSymboBaseAddr(szItem, pValue))
					return true;
			}
		}

			for (map<CMemoryImage, CDbgModule>::IT it = gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
				it != gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.end(); ++it)
			{
				if (it->second.GetSymboBaseAddr(szItem, pValue))
					return true;
			}

		for (map<unsigned long long, CSymbolModule>::IT it = gpSyser->m_SyserUI.m_SymbolContainer.m_SymbolModulesMap.begin();
			it; ++it)
		{
			if (it->second.m_ImageBase && it->second.GetAddr(szItem, pValue))
				return true;
		}
		return false;
	}

	ULONG_PTR CSyserCodeDoc::ReadValue(ULONG_PTR Address)
	{
		ULONG_PTR Value = 0;
		//CSyserCodeDoc::
		ReadMemory(Address, &Value, sizeof(Value)); //???
		return Value;
	}

	void CSyserCodeDoc::OnTestCondition()
	{
		if (gpSyser->m_pDebugger->ReadMemory(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP, &m_OPCODE, 4) != 4)
			m_OPCODE = -1;
	}

	void CSyserCodeDoc::GetInstrInfo(WCHAR *pStr)
	{
		if (m_bOpen)
		{
			gpSyser->m_pDebugger->SaveRegisters();
			X86_CPU_REG_PTR CpuReg;
			gpSyser->m_pDebugger->GetX86RegPtr(&CpuReg);
			DIS_CPU DisCPU;

		#ifdef _X64_
			DisCPU.gen_reg[EAX_IDX].rerx = *CpuReg.pCAX;
			DisCPU.gen_reg[EBX_IDX].rerx = *CpuReg.pCBX;
			DisCPU.gen_reg[ECX_IDX].rerx = *CpuReg.pCCX;
			DisCPU.gen_reg[EDX_IDX].rerx = *CpuReg.pCDX;
			DisCPU.gen_reg[ESI_IDX].rerx = *CpuReg.pCSI;
			DisCPU.gen_reg[EDI_IDX].rerx = *CpuReg.pCDI;
			DisCPU.gen_reg[EBP_IDX].rerx = *CpuReg.pCBP;
			DisCPU.gen_reg[ESP_IDX].rerx = *CpuReg.pCSP;
			DisCPU.gen_reg[R8_IDX].rerx = *CpuReg.pR8;
			DisCPU.gen_reg[R9_IDX].rerx = *CpuReg.pR9;
			DisCPU.gen_reg[R10_IDX].rerx = *CpuReg.pR10;
			DisCPU.gen_reg[R11_IDX].rerx = *CpuReg.pR11;
			DisCPU.gen_reg[R12_IDX].rerx = *CpuReg.pR12;
			DisCPU.gen_reg[R13_IDX].rerx = *CpuReg.pR13;
			DisCPU.gen_reg[R14_IDX].rerx = *CpuReg.pR14;
			DisCPU.gen_reg[R15_IDX].rerx = *CpuReg.pR15;
		#else
			DisCPU.gen_reg[EAX_IDX].rerx = *CpuReg.pCAX;
			DisCPU.gen_reg[EBX_IDX].rerx = *CpuReg.pCBX;
			DisCPU.gen_reg[ECX_IDX].rerx = *CpuReg.pCCX;
			DisCPU.gen_reg[EDX_IDX].rerx = *CpuReg.pCDX;
			DisCPU.gen_reg[ESI_IDX].rerx = *CpuReg.pCSI;
			DisCPU.gen_reg[EDI_IDX].rerx = *CpuReg.pCDI;
			DisCPU.gen_reg[EBP_IDX].rerx = *CpuReg.pCBP;
			DisCPU.gen_reg[ESP_IDX].rerx = *CpuReg.pCSP;
		#endif
			//DisCPU.EFL = *CpuReg.pEFL;

			DisCPU.SEG_BASE[CS_IDX] = 0;
			DisCPU.SEG_BASE[DS_IDX] = 0;
			DisCPU.SEG_BASE[ES_IDX] = 0;
			DisCPU.SEG_BASE[FS_IDX] = 0;
			DisCPU.SEG_BASE[GS_IDX] = 0;
			DisCPU.SEG_BASE[SS_IDX] = 0;

			gpSyser->m_pDebugger->GetSegRegBase(*CpuReg.pCS, &DisCPU.SEG_BASE[CS_IDX], 0);
			gpSyser->m_pDebugger->GetSegRegBase(*CpuReg.pDS, &DisCPU.SEG_BASE[DS_IDX], 0);
			gpSyser->m_pDebugger->GetSegRegBase(*CpuReg.pES, &DisCPU.SEG_BASE[ES_IDX], 0);
			gpSyser->m_pDebugger->GetSegRegBase(*CpuReg.pFS, &DisCPU.SEG_BASE[FS_IDX], 0);
			gpSyser->m_pDebugger->GetSegRegBase(*CpuReg.pGS, &DisCPU.SEG_BASE[GS_IDX], 0);
			gpSyser->m_pDebugger->GetSegRegBase(*CpuReg.pSS, &DisCPU.SEG_BASE[SS_IDX], 0);

			_INSTRUCTION_INFORMATION InstInfo;
			unsigned char CodeBuff[256];

			InstInfo.CodeBuff = CodeBuff;
			InstInfo.CodeMode = gpSyser->m_pDebugger->m_CodeMode;
			InstInfo.pasm = nullptr;
			InstInfo.eip = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP;

			CInstrSym::Dasm(&InstInfo, &DisCPU, MAX_INSTR_LEN);

			pStr[0] = 0;
			for (int i = 0; i < 3; ++i)
			{
				if (InstInfo.op[i].mode == OP_Address)
				{
					CInstrSym::OptItemSymToCS(&InstInfo.op[i], pStr, &InstInfo);
				}
			}
		}
	}

	unsigned long CSyserCodeDoc::ReadColorString(ULONG_PTR Address, unsigned long *pColorString, unsigned long MaxSize)
	{
		WCHAR Buffer[256];
		char Encode;
		if (gpSyser->m_SyserUI.ReadString(Address, Buffer, 256, &Encode))
		{
			if (Encode == 0)
				return ColorStrCat(pColorString, Buffer, 6, 0);
			else
			if (Encode == 1)
				return ColorStrCat(pColorString, Buffer, 9, 0);
			return 0;
		}

		char szSym[64+64]; //???
		if (AddrToSym(Address, szSym, 64))
		{
			return ColorStrCat(pColorString, szSym, 1, 0);
		}
		return 0;
	}

	void CSyserCodeDoc::ContextCS(unsigned long *pColorString, int MaxLen)
	{
		if (m_bOpen)
		{
			gpSyser->m_pDebugger->SaveRegisters();
			X86_CPU_REG_PTR CpuReg;
			gpSyser->m_pDebugger->GetX86RegPtr(&CpuReg);
			DIS_CPU DisCPU;

		#ifdef _X64_
			DisCPU.gen_reg[EAX_IDX].rerx = *CpuReg.pCAX;
			DisCPU.gen_reg[EBX_IDX].rerx = *CpuReg.pCBX;
			DisCPU.gen_reg[ECX_IDX].rerx = *CpuReg.pCCX;
			DisCPU.gen_reg[EDX_IDX].rerx = *CpuReg.pCDX;
			DisCPU.gen_reg[ESI_IDX].rerx = *CpuReg.pCSI;
			DisCPU.gen_reg[EDI_IDX].rerx = *CpuReg.pCDI;
			DisCPU.gen_reg[EBP_IDX].rerx = *CpuReg.pCBP;
			DisCPU.gen_reg[ESP_IDX].rerx = *CpuReg.pCSP;
			DisCPU.gen_reg[R8_IDX].rerx = *CpuReg.pR8;
			DisCPU.gen_reg[R9_IDX].rerx = *CpuReg.pR9;
			DisCPU.gen_reg[R10_IDX].rerx = *CpuReg.pR10;
			DisCPU.gen_reg[R11_IDX].rerx = *CpuReg.pR11;
			DisCPU.gen_reg[R12_IDX].rerx = *CpuReg.pR12;
			DisCPU.gen_reg[R13_IDX].rerx = *CpuReg.pR13;
			DisCPU.gen_reg[R14_IDX].rerx = *CpuReg.pR14;
			DisCPU.gen_reg[R15_IDX].rerx = *CpuReg.pR15;
		#else
			DisCPU.gen_reg[EAX_IDX].rerx = *CpuReg.pCAX;
			DisCPU.gen_reg[EBX_IDX].rerx = *CpuReg.pCBX;
			DisCPU.gen_reg[ECX_IDX].rerx = *CpuReg.pCCX;
			DisCPU.gen_reg[EDX_IDX].rerx = *CpuReg.pCDX;
			DisCPU.gen_reg[ESI_IDX].rerx = *CpuReg.pCSI;
			DisCPU.gen_reg[EDI_IDX].rerx = *CpuReg.pCDI;
			DisCPU.gen_reg[EBP_IDX].rerx = *CpuReg.pCBP;
			DisCPU.gen_reg[ESP_IDX].rerx = *CpuReg.pCSP;
		#endif
			//DisCPU.EFL = *CpuReg.pEFL;

			DisCPU.SEG_BASE[CS_IDX] = 0;
			DisCPU.SEG_BASE[DS_IDX] = 0;
			DisCPU.SEG_BASE[ES_IDX] = 0;
			DisCPU.SEG_BASE[FS_IDX] = 0;
			DisCPU.SEG_BASE[GS_IDX] = 0;
			DisCPU.SEG_BASE[SS_IDX] = 0;

			gpSyser->m_pDebugger->GetSegRegBase(*CpuReg.pCS, &DisCPU.SEG_BASE[CS_IDX], 0);
			gpSyser->m_pDebugger->GetSegRegBase(*CpuReg.pDS, &DisCPU.SEG_BASE[DS_IDX], 0);
			gpSyser->m_pDebugger->GetSegRegBase(*CpuReg.pES, &DisCPU.SEG_BASE[ES_IDX], 0);
			gpSyser->m_pDebugger->GetSegRegBase(*CpuReg.pFS, &DisCPU.SEG_BASE[FS_IDX], 0);
			gpSyser->m_pDebugger->GetSegRegBase(*CpuReg.pGS, &DisCPU.SEG_BASE[GS_IDX], 0);
			gpSyser->m_pDebugger->GetSegRegBase(*CpuReg.pSS, &DisCPU.SEG_BASE[SS_IDX], 0);

			_INSTRUCTION_INFORMATION InstInfo;
			unsigned char CodeBuff[256];

			InstInfo.CodeBuff = CodeBuff;
			InstInfo.CodeMode = gpSyser->m_pDebugger->m_CodeMode;
			InstInfo.pasm = nullptr;
			InstInfo.eip = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP;

			CInstrSym::Dasm(&InstInfo, &DisCPU, MAX_INSTR_LEN);

			pColorString[0] = 0;
			for (int i = 0; i < 3; ++i)
			{
				if (InstInfo.op[i].mode == OP_Address)
				{
					ReadColorString(InstInfo.op[i].addr.line_address, pColorString, MaxLen);
				} else
				if (InstInfo.op[i].mode == OP_Register)
				{
					switch (InstInfo.op[i].reg.reg_index)
					{
				#ifdef _X64_
					case EAX_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCAX, pColorString, MaxLen); break;
					case ECX_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCCX, pColorString, MaxLen); break;
					case EDX_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCDX, pColorString, MaxLen); break;
					case EBX_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBX, pColorString, MaxLen); break;
					case ESP_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP, pColorString, MaxLen); break;
					case EBP_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBP, pColorString, MaxLen); break;
					case ESI_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSI, pColorString, MaxLen); break;
					case EDI_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCDI, pColorString, MaxLen); break;

					case R8_IDX:  ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR8,  pColorString, MaxLen); break;
					case R9_IDX:  ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR9,  pColorString, MaxLen); break;
					case R10_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR10, pColorString, MaxLen); break;
					case R11_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR11, pColorString, MaxLen); break;
					case R12_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR12, pColorString, MaxLen); break;
					case R13_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR13, pColorString, MaxLen); break;
					case R14_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR14, pColorString, MaxLen); break;
					case R15_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pR15, pColorString, MaxLen); break;
				#else
					case EAX_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCAX, pColorString, MaxLen); break;
					case ECX_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCCX, pColorString, MaxLen); break;
					case EDX_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCDX, pColorString, MaxLen); break;
					case EBX_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBX, pColorString, MaxLen); break;
					case ESP_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP, pColorString, MaxLen); break;
					case EBP_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBP, pColorString, MaxLen); break;
					case ESI_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSI, pColorString, MaxLen); break;
					case EDI_IDX: ReadColorString(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCDI, pColorString, MaxLen); break;
				#endif
					}
				}
			}
		}
	}

	bool CSyserCodeDoc::IsDasm(ULONG_PTR Address, unsigned long Size)
	{
		unsigned long i = 0;
		while (i < Size)
		{
			if (Address+i == *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP)
				return true;
			unsigned long Len = CInstrSym::InstrLen(Address+i, MAX_INSTR_LEN);
			MIN_LIMIT(Len, 1);
			i += Len;
		}
		return false;
	}
// DI
	void CSyserDI::OnLoadModule(CDebugger *pDebugger, const char *szName, ULONG_PTR ImageBase, unsigned long ImageSize, unsigned long TimeStamp, unsigned long CheckSum)
	{
		CDbgModule *pDbgMod = pDebugger->m_DbgModuleList.InsertModule(szName, ImageBase, ImageSize, TimeStamp, CheckSum);
		if (pDbgMod)
		{
			CCommentModule *cmod = m_pCommentContainer->FindModule(TimeStamp, CheckSum);
			if (cmod)
				pDbgMod->SetCommentModule(cmod);

			CSymbolModule *pSymMod = m_pSymbolContainer->FindModule(TimeStamp, CheckSum);
			if (pSymMod)
			{
				pDbgMod->SetSymbolModule(pSymMod);
			} else
			{
				//TODO IsExist PDB, callback for allow to load
				//gpSyser->m_SyserUI.LoadPDBSym(szName, ImageBase, ImageSize); //delay load pdb on plunge
				//gpSyser->m_SyserUI.LoadPESym(szName, pDbgMod);
			}
		}
	}
	void CSyserDI::OnUnloadModule(CDebugger *pDebugger, ULONG_PTR ImageBase)
	{
		CDbgModule *pDbgMod = pDebugger->m_DbgModuleList.FindModule(ImageBase);
		if (pDbgMod)
		{
		//#ifdef _RING0_
			CSDSModule *pSDSMod = gpSyser->m_SyserUI.FindSDSModule(pDbgMod->m_ImageBase);
			if (pSDSMod
				)
				gpSyser->m_SyserUI.UnloadSDSModule(pSDSMod);
		//#endif
			map<unsigned long long, BREAK_POINT>::IT it = pDebugger->m_pCodeBPMap->begin();
			//while (it != pDebugger->m_pCodeBPMap->end())
			while (it)
			{
				if (pDebugger->m_dwProcessId == it->second.ProcessID &&
					it->second.Address >= pDbgMod->m_ImageBase &&
					it->second.Address <= pDbgMod->m_ImageBase + pDbgMod->m_ImageSize)
				{
					map<unsigned long long, BREAK_POINT>::IT p = it;
					p->second.State = BP_STATE_DISABLE;
					++it; //???
					pDebugger->DelCodeBP(p->second); //???
				} else
				{
					++it;
				}
			}
			pDebugger->m_DbgModuleList.RemoveModule(pDbgMod->m_ImageBase);
		}
	}
	void CSyserDI::OnTestCondition()
	{
		gpSyser->m_SyserUI.m_SyserCodeDoc.OnTestCondition();
	}

	bool CSyserDI::Close(bool bExit)
	{
		if (bExit)
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("close"), 0);
		} else
		{
			gpSyser->m_MainFrame.m_SystemExplorer.ViewCodeAddress(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP);
			gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
		}
		return true;
	}
	bool CSyserDI::Plunge(unsigned int Code)
	{
		if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
			return false;

		gpSyser->m_PID = gpSyser->m_pDebugger->m_dwProcessId;
		gpSyser->m_TID = gpSyser->m_pDebugger->m_dwThreadId;

		gpSyser->m_SyserUI.m_SyserCodeDoc.m_PID = gpSyser->m_pDebugger->m_dwProcessId;
		gpSyser->m_SyserUI.m_SyserCodeDoc.m_TID = gpSyser->m_pDebugger->m_dwThreadId;

		gpSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(gSyserConfig.iFollowMode == SRC_TAB ? SRC_TAB : CV_TAB); //SRC_TAB(2) CV_TAB(0)
		gpSyser->m_MainFrame.m_SystemExplorer.ViewCodeAddress(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP);
		gpSyser->m_MainFrame.m_SystemExplorer.UpdateRegView();
	//+++

		for (map<CMemoryImage, CDbgModule>::IT it = gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
			it != gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.end(); ++it)
		{
			if (it->second.m_LoadPDB)
			{
				it->second.m_LoadPDB = false;
				gpSyser->m_SyserUI.LoadPDBSym(it->second.m_FullFileName.operator const char*(), it->second.m_ImageBase, it->second.m_ImageSize); //delay load pdb on plunge
				gpSyser->m_SyserUI.LoadPESym(it->second.m_FullFileName.operator const char*(), &it->second);
			}
		}

		UpdateContext();

		for (map<TWideString, SYSER_PLUGIN_MODULE>::IT it = gpSyser->m_PluginMap.begin();
			it != gpSyser->m_PluginMap.end(); ++it)
		{
			if (it->second.fpOnDebuggerPlunge)
				it->second.fpOnDebuggerPlunge();
		}
		gpSyser->m_MainFrame.FocusActive();
		return true;
	}

	bool CSyserDI::OnExit(bool bExit)
	{
		WISP_MSG Msg;
		Msg.hWnd = &gpSyser->m_MainFrame;
		Msg.bExit = bExit;
		Msg.Msg = 31; //Close
		gpSyser->PostMessage(&Msg);
		return true;
	}
	bool CSyserDI::OnDebugException(unsigned int Code)
	{
		WISP_MSG Msg;
		Msg.hWnd = &gpSyser->m_MainFrame;
		Msg.Code = Code;
		Msg.Msg = 30; //Plunge
		gpSyser->PostMessage(&Msg);
		return true;
	}

	void CSyserDI::DisplayMsg(const char *szMsg)
	{
		WCHAR Buffer[512];
		AnsiToUnicode(szMsg, Buffer, lenof(Buffer));
		gpSyser->m_MainFrame.m_SystemExplorer.m_TerminalWnd.OutPut(Buffer);
	}
	void CSyserDI::UpdateContext()
	{
		gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(true);

		gpSyser->m_SyserUI.m_CallStack.UpdateContext(0);
		gpSyser->m_MainFrame.m_CallStackWnd.UpdateContext();
	}
	bool CSyserDI::TestCondition(const char *Condition, BREAK_POINT *pBP)
	{
		if (!Condition[0])
			return true;
		if (pBP) gpSyser->m_SyserUI.m_SyserCodeDoc.m_BPCOUNT = pBP->HitCount;
		ULONG_PTR Result;
		return gpSyser->m_SyserUI.m_SyserCodeDoc.CalcExpression(Condition, &Result) && Result;
	}
	bool CSyserDI::RunCmd(const char *szCmd)
	{
		WCHAR Buffer[256];
		AnsiToUnicode(szCmd, Buffer, lenof(Buffer));
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
		return true;
	}
	void CSyserDI::RunCmds(const char *szCmds)
	{
		if (szCmds[0])
		{
			WCHAR Buffer[1024];
			AnsiToUnicode(szCmds, Buffer, lenof(Buffer));
			gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmds(Buffer, ';');
		}
	}

	CDebugger *CSyserDI::GetDebugger()
	{
		return gpSyser->m_pDebugger;
	}

//STACK
	CCallStack::CCallStack()
	{
		m_Address = 0;
	}

	void CCallStack::UpdateContext(ULONG_PTR Address)
	{
		m_HeadIt.Reset(); //??? move under m_Address != Address
#if 0
		if (Address == 0)
			Address = gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP?*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP:0;

		if (m_Address != Address)
		{
			m_Address = Address;
			m_Frames.Clear();

			//gpSyser->m_pDebugger->InitGetStack(Address); //TODO frame.AddrStack.Offset = Address
			gpSyser->m_pDebugger->SaveRegisters();
			for(ULONG nFrame = 0; nFrame < 256; nFrame++)
			{
				STACK_FRAME StackFrame;

				if (!gpSyser->m_pDebugger->GetStack(&StackFrame)) break;

				if (StackFrame.AddrPC == StackFrame.AddrReturn) break;
				if (StackFrame.AddrPC == 0) break;
				if (StackFrame.AddrReturn == 0) break;

				if (StackFrame.AddrPC)
				{
					char szFuncName[64];
					ULONG_PTR Addr;
					if (gpSyser->m_SyserUI.m_SyserCodeDoc.GetCall(StackFrame.AddrReturn, szFuncName, &Addr))
					{
						if (!szFuncName[0])
						{
							CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(Addr);
							if (!pDbgMod)
							{
								if (gpSyser->m_pSysDebugger != gpSyser->m_pDebugger)
									pDbgMod = gpSyser->m_pSysDebugger->m_DbgModuleList.FindModule(Addr);
							}

							if (pDbgMod)
							{
								unsigned long Offset = Addr - pDbgMod->m_ImageBase;
								TSPrintf(szFuncName, "%s+0x%X", pDbgMod->m_FileTitle.operator const char*(), Offset);
							} else
							{
								TSPrintf(szFuncName, "unknown_function");
							}
						}
						TListIter<Frame> It = m_Frames.Append();
						It->Address = StackFrame.AddrFrame;//Address; //Frame
						It->Return = StackFrame.AddrReturn;
						It->FuncAddr = Addr;
						It->FuncName = szFuncName;
					}
		#if 0
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%2d from %016X   STACK %016X    FRAME %016X    RET %016X %s\n"),
					nFrame,
					StackFrame.AddrPC,//.Offset,
					StackFrame.AddrStack,//.Offset,
					StackFrame.AddrFrame,//.Offset,
					StackFrame.AddrReturn,//.Offset,
					WSTR(""));//name);
		#endif
				} else
				{
					//break;
				}
				if (StackFrame.AddrReturn == 0) break;
			}

			if (m_Frames.Size())
				m_HeadIt = m_Frames.Begin();

		}
#endif
#ifndef _X64_
		if (Address == 0)
			Address = gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBP?*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBP:0;

		if (m_Address != Address)
		{
			m_Address = Address;
			m_Frames.Clear();
			for (int i = 0; i < 256; ++i)
			{
				ULONG_PTR Mem[2];
				if (gpSyser->m_pDebugger->ReadMemory(Address, Mem, sizeof(Mem)) != sizeof(Mem) || Mem[0] <= Address)
					break;
				char szFuncName[64];
				ULONG_PTR Addr;
				if (gpSyser->m_SyserUI.m_SyserCodeDoc.GetCall(Mem[1], szFuncName, &Addr))
				{
					if (!szFuncName[0])
					{
						CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(Addr);
						if (!pDbgMod)
						{
							if (gpSyser->m_pSysDebugger != gpSyser->m_pDebugger)
								pDbgMod = gpSyser->m_pSysDebugger->m_DbgModuleList.FindModule(Addr);
						}

						if (pDbgMod)
						{
							unsigned long Offset = Addr - pDbgMod->m_ImageBase;
							TSPrintf(szFuncName, "%s+0x%X", pDbgMod->m_FileTitle.operator const char*(), Offset);
						} else
						{
							TSPrintf(szFuncName, "unknown_function");
						}
					}
					TListIter<Frame> It = m_Frames.Append();
					It->Address = Address; //Frame
					It->Return = Mem[1];
					It->FuncAddr = Addr;
					It->FuncName = szFuncName;
				}
				Address = Mem[0];
			}

			if (m_Frames.Size())
				m_HeadIt = m_Frames.Begin();
		}
#endif
	}

// UI
	bool CSyserUI::RegisterPluginModule(const WCHAR *ModuleName, SYSER_PLUGIN_MODULE *pPluginModule)
	{
		map<TWideString, SYSER_PLUGIN_MODULE>::IT it = gpSyser->m_PluginMap.insert(_Tmap_pair<const WCHAR*,SYSER_PLUGIN_MODULE>(ModuleName,SYSER_PLUGIN_MODULE()));
		if (it == gpSyser->m_PluginMap.end())
			return false;
		it->second = *pPluginModule;
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Syser : Register Plugin Module %s (%s)\n"), ModuleName, pPluginModule->PluginInfo);
		return true;
	}
	bool CSyserUI::UnregisterPluginModule(const WCHAR *ModuleName)
	{
		map<TWideString, SYSER_PLUGIN_MODULE>::IT it = gpSyser->m_PluginMap.find(ModuleName);
		if (it == gpSyser->m_PluginMap.end())
			return false;
		gpSyser->m_PluginMap.erase(it);
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Syser : Unregister Plugin Module %s\n"), ModuleName);
		return true;
	}

	int CSyserUI::GetInstrLen(ULONG_PTR Address)
	{
		return m_SyserCodeDoc.InstrLen(Address, MAX_INSTR_LEN);
	}

	bool CSyserUI::CalcExp(const char *szExp, ULONG_PTR *pResult)
	{
		return m_SyserCodeDoc.CalcExpression(szExp, pResult);
	}

	bool CSyserUI::CalcExp(const WCHAR *szExp, ULONG_PTR *pResult)
	{
		if (!szExp) return false;
		char Buffer[256];
		UnicodeToAnsi(szExp, Buffer, lenof(Buffer));
		return m_SyserCodeDoc.CalcExpression(Buffer, pResult);
	}

	bool CSyserUI::InsertCmd(const WCHAR *szCmd, FPCmd pCmdProc, void *pUserData, const WCHAR *pComment, const WCHAR *pUsage)
	{
		return gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::InsertCmd(szCmd, (CMDPROC)pCmdProc, pUserData, pComment, pUsage);
	}
	void CSyserUI::RemoveCmd(const WCHAR *szCmd)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::RemoveCmd(szCmd);
	}
	int CSyserUI::RunCmd(const WCHAR *szCmd)
	{
		return gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(szCmd, 0);
	}

	void CSyserUI::Outputf(const WCHAR *szFormat, ...)
	{
		va_list va;
		va_start(va, szFormat);
		gpSyser->m_MainFrame.m_ConsoleWnd.VPrintf(szFormat, va); //return Length TODO
		va_end(va);
	}
	void CSyserUI::Output(const unsigned long *szMsg)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.OutPut(szMsg);
	}

	void *CSyserUI::InsertMenu(void *hParentMenu, const WCHAR *szMenuName, FPMenuProc fpMenuProc)
	{
		CWispMenu::MenuItem *Item = gpSyser->m_MainFrame.m_SystemExplorer.m_MainMenu.GetItem(WSTR("Plugin"));
		if (!Item)
			return nullptr;
		CWispMenu *Menu = gpSyser->m_MainFrame.m_SystemExplorer.m_MainMenu.GetMenu(Item);
		if (!Menu)
			return nullptr;
		CWispMenu::MenuItem *r = Menu->AppendMenu(-1, szMenuName, 0, 0, 0, (unsigned long long)fpMenuProc);
		Menu->Adjust();
		return r;
	}
	bool CSyserUI::RemoveMenu(void *hMenu)
	{
		CWispMenu::MenuItem *Item = gpSyser->m_MainFrame.m_SystemExplorer.m_MainMenu.GetItem(WSTR("Plugin"));
		if (!Item)
			return false;
		CWispMenu *Menu = gpSyser->m_MainFrame.m_SystemExplorer.m_MainMenu.GetMenu(Item);
		if (!Menu)
			return false;
		bool r = Menu->RemoveMenu((CWispMenu::MenuItem *)hMenu);
		Menu->Adjust();
		return r;
	}
	bool CSyserUI::EnableMenu(void *hMenu, bool bEnable)
	{
		return gpSyser->m_MainFrame.m_SystemExplorer.m_MainMenu.GetMenu((CWispMenu::MenuItem *)hMenu)->Enable((CWispMenu::MenuItem *)hMenu, bEnable);
	}

	void *CSyserUI::GetMainTabWnd()
	{
		return &gpSyser->m_MainFrame.m_MainTabWnd;
	}
	void *CSyserUI::GetMainMenu()
	{
		return &gpSyser->m_MainFrame.m_SystemExplorer.m_MainMenu;
	}
	void *CSyserUI::GetWisp()
	{
		return gpCurWisp;
	}

	unsigned long CSyserUI::WriteMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size)
	{
		if (gpSyser && gpSyser->m_pDebugger && Size && Buffer)
			return gpSyser->m_pDebugger->WriteMemory(Address, Buffer, Size);
		return 0;
	}
	unsigned long CSyserUI::ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size)
	{
		if (gpSyser && gpSyser->m_pDebugger && Size && Buffer)
			return gpSyser->m_pDebugger->ReadMemory(Address, Buffer, Size);
		return 0;
	}

	bool CSyserUI::ReadUString(ULONG_PTR Address, WCHAR *pString, unsigned long Size)
	{
		char Buffer[256];
		char UBuffer[256*2];
		unsigned long Len = m_SyserCodeDoc.ReadMemory(Address, Buffer, 256);
		unsigned long RLen;
		if (Len >= 10 && UNormalize(Buffer, Len, UBuffer, 256*2, &RLen))
		{
			int PrefixLen = TStrCpy(pString, "UNICODE \"");
			TStrCpyLimit(&pString[PrefixLen], (WCHAR*)UBuffer, Size - PrefixLen - 1);
			TStrCat(pString, "\"");
			return true;
		}
		return false;
	}

	bool CSyserUI::ReadAString(ULONG_PTR Address, char *pString, unsigned long Size)
	{
		char Buffer[256];
		char ABuffer[256];
		unsigned long Len = m_SyserCodeDoc.ReadMemory(Address, Buffer, 256);
		unsigned long RLen;
		if (Len >= 10 && ANormalize(Buffer, Len, ABuffer, 256, &RLen))
		{
			int PrefixLen = TStrCpy(pString, "ASCII \"");
			TStrCpyLimit(&pString[PrefixLen], ABuffer, Size - PrefixLen - 1);
			TStrCat(pString, "\"");
			return true;
		}
		return false;

	}

	bool CSyserUI::ReadString(ULONG_PTR Address, WCHAR *pString, unsigned long Size, char *Encode)
	{
		if (ReadUString(Address, pString, Size))
		{
			*Encode = 1;
			return true;
		}

		char AString[256];
		if (ReadAString(Address, AString, 256))
		{
			TStrCpyLimit(pString, AString, Size); //???
			AnsiToUnicode(AString, pString, Size); //???
			*Encode = 0;
			return true;
		}
		return false;
	}

	bool CSyserUI::GetSymbol(ULONG_PTR Address, char *szName, int Length)
	{
		if (!gpSyser->m_pDebugger)
			return false;

		if (gSyserConfig.iShowFullSym)
		{
			if (gpSyser->m_pDebugger->m_DbgModuleList.GetFullSymbol(Address, szName, Length))
				return true;
			
			if (gpSyser->m_pDebugger != gpSyser->m_pSysDebugger && gpSyser->m_pSysDebugger)
			{
				if (gpSyser->m_pSysDebugger->m_DbgModuleList.GetFullSymbol(Address, szName, Length))
					return true;
			}

			if (gpSyser->m_SyserUI.m_SymbolContainer.GetFullSymbol(Address, szName, Length))
				return true;
			return false;
		}
		
		const char *pStr = gpSyser->m_pDebugger->m_DbgModuleList.GetSymbol(Address);
		if (pStr)
		{
			TStrCpyLimit(szName, pStr, Length);
			return true;
		}

		if (gpSyser->m_pDebugger != gpSyser->m_pSysDebugger && gpSyser->m_pSysDebugger)
		{
			pStr = gpSyser->m_pSysDebugger->m_DbgModuleList.GetSymbol(Address);
			if (pStr)
			{
				TStrCpyLimit(szName, pStr, Length);
				return true;
			}
		}

		pStr = gpSyser->m_SyserUI.m_SymbolContainer.GetSymbol(Address);
		if (pStr)
		{
			TStrCpyLimit(szName, pStr, Length);
			return true;
		}
		
		return false;
	}

	bool CSyserUI::GetSymbolOffset(ULONG_PTR Address, char *szName)
	{
		szName[0] = 0;
		ULONG_PTR Base = Address;

		if (!gpSyser->m_pDebugger->m_DbgModuleList.GetSymbolBase(&Base))
		{
			if (gpSyser->m_pDebugger == gpSyser->m_pSysDebugger)
				return false;

			if (!gpSyser->m_pSysDebugger->m_DbgModuleList.GetSymbolBase(&Base))
				return false;
		}

		char Buffer[256];
		if (!GetSymbol(Base, Buffer, 64))
			return false;

		if (Base == Address)
		{
			TStrCpy(szName, Buffer);
		} else
		if (Base < Address)
		{
			TSPrintf(szName, "%s+0x%X", Buffer, Address - Base);
		} else
		if (Base > Address)
		{
			TSPrintf(szName, "%s-0x%X", Buffer, Base - Address);
		}
		return true;
	}

	unsigned long CSyserUI::SetCommentModule(CCommentModule *pCommentModule)
	{
		unsigned long Count = 0;
			CDebugger *pDebugger = gpSyser->m_pDebugger;
			CDbgModule *pDbgMod = pDebugger->m_DbgModuleList.FindModule(pCommentModule->m_TimeStamp, pCommentModule->m_CheckSum);
			if (pDbgMod && !pDbgMod->m_pCommentModule)
			{
				pDbgMod->SetCommentModule(pCommentModule);
				++Count;
			}
		return Count;
	}

	unsigned long CSyserUI::RemoveCommentModule(CCommentModule *pCommentModule)
	{
		unsigned long Count = 0;
			CDebugger *pDebugger = gpSyser->m_pDebugger;
			for (map<CMemoryImage, CDbgModule>::IT it2 = pDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
				it2 != pDebugger->m_DbgModuleList.m_DbgModuleMap.end(); ++it2)
			{
				if (it2->second.m_pCommentModule == pCommentModule)
				{
					it2->second.RemoveCommentModule(&m_CommentContainer);
					++Count;
				}
			}
		return Count;
	}

	unsigned long CSyserUI::SetSymbolModule(CSymbolModule *pSymMod)
	{
		unsigned long Count = 0;
			CDebugger *pDebugger = gpSyser->m_pDebugger;
			CDbgModule *pDbgMod = pDebugger->m_DbgModuleList.FindModule(pSymMod->m_TimeStamp, pSymMod->m_CheckSum);
			if (pDbgMod && !pDbgMod->m_pSymbolModule)
			{
				pDbgMod->SetSymbolModule(pSymMod);
				++Count;
			}
		return Count;
	}

	unsigned long CSyserUI::LoadSDSModuleSym(CSDSModule *pSDSMod)
	{
		if (!pSDSMod)
			return 0;
		if (!pSDSMod->m_pSDSFile)
			return 0;

		CSymbolModule *pSymMod = m_SymbolContainer.FindModule(pSDSMod->GetFILESIG().TimeStamp, pSDSMod->GetFILESIG().CheckSum);
		if (!pSymMod)
			return 0;
		unsigned long Count = 0;
	//
	{
		unsigned long Len = 0;
		DATATYPE *data = pSDSMod->GetLocal(&Len);
		for (unsigned long n = 0; n < Len; ++n)
		{
			if (data[n].locationType == LocIsStatic)
			{
				if (data[n].Static.rva == 0) continue; //+++
				pSymMod->Add(data[n].Static.rva, data[n].name);
				++Count;
			}
		}
	}
	//	//skip first 1
	{
		unsigned long Len = 0;
		FUNCTION **func = pSDSMod->GetFunctions(&Len);
		for (unsigned long n = 1; n < Len; ++n)
		{
			if (func[n]->rva == 0) continue;
			pSymMod->Add(func[n]->rva, func[n]->name);
			++Count;
		}
	}
	//
	{
		unsigned long Len = 0;
		PUBSYM *pubsym = pSDSMod->GetPublicSymbols(&Len);
		for (unsigned long n = 1; n < Len; ++n)
		{
			if (pubsym[n].rva == 0) continue; //+++
			pSymMod->Add(pubsym[n].rva, pubsym[n].name);
			++Count;
		}
	}
		return Count;
	}

	CSDSModule *CSyserUI::FindSDSModule(ULONG_PTR ImageBase)
	{
		CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(ImageBase);
		if (!pDbgMod)
		{
			if (gpSyser->m_pDebugger == gpSyser->m_pSysDebugger)
				return nullptr;
			pDbgMod = gpSyser->m_pSysDebugger->m_DbgModuleList.FindModule(ImageBase);
			if (!pDbgMod)
				return nullptr;
		}
		map<FILESIG, CSDSModule>::IT it = m_SDSModulesMap.find(FILESIG(pDbgMod->m_TimeStamp, pDbgMod->m_CheckSum));
		if (!it)
		{
			it = m_SDSModulesMap.find(FILESIG(1, 1));
			if (!it)
				return nullptr;
		}
		it->second.m_pDbgModule = pDbgMod;
		return &it->second;
	}

	bool CSyserUI::UnloadSDSModule(CSDSModule *pSDSMod)
	{
		map<FILESIG, CSDSModule>::IT it = m_SDSModulesMap.find(pSDSMod->GetFILESIG());
		if (!it)
			return false;
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_SDSModuleList.RemoveSDSModule(&it->second);
		
		CSymbolModule *pSymMod = m_SymbolContainer.FindModule(pSDSMod->GetFILESIG().TimeStamp, pSDSMod->GetFILESIG().CheckSum);
		if (pSymMod)
			--pSymMod->m_RefCount;
		DebugPrintf("Syser : Unload SDS %s\n", TGetFileName(pSDSMod->m_SDSFileName.operator const char*()));
		m_SDSModulesMap.erase(it);
		return true;
	}

	CSDSModule *CSyserUI::LoadSDSModule(const char *SDSFullFileName)
	{
		FILESIG FileSig = CSDSModule::GetFILESIG(SDSFullFileName);
		if (FileSig.TimeStamp == 0 && FileSig.CheckSum == 0)
			return nullptr;
		map<FILESIG, CSDSModule>::IT it = m_SDSModulesMap.insert(_Tmap_pair<FILESIG, CSDSModule>(FileSig, (CSDSModule*)0));
		if (!it)
		{
			DebugPrintf("Syser : Same SDS is already loaded!\n");
			return nullptr;
		}
		if (!it->second.Load(SDSFullFileName))
		{
			DebugPrintf("Syser : Fail to load SDS %s\n", SDSFullFileName);
			m_SDSModulesMap.erase(it);
			return nullptr;
		}

		it->second.m_ExeFileName = TGetFileName(it->second.GetExeFileName());
		CSymbolModule *pSymMod = gpSyser->m_SyserUI.m_SymbolContainer.FindModule(FileSig.TimeStamp, FileSig.CheckSum);
		if (!pSymMod)
		{
			pSymMod = gpSyser->m_SyserUI.m_SymbolContainer.InsertModule(FileSig.TimeStamp, FileSig.CheckSum);
			if (!pSymMod)
			{
				DebugPrintf("Syser : Fail to insert %s Symbol Module\n", SDSFullFileName);
				m_SDSModulesMap.erase(it);
				return nullptr;
			}
			pSymMod->m_ModuleName = it->second.m_ExeFileName;
			gpSyser->m_SyserUI.SetSymbolModule(pSymMod);
		}
		int symbols = gpSyser->m_SyserUI.LoadSDSModuleSym(&it->second);
		int sources = it->second.GetSrcFilesCount();
		if (!sources)
		{
			m_SDSModulesMap.erase(it);
			DebugPrintf("Syser : Load SDS %s < %d symbols >\n", TGetFileName(SDSFullFileName), symbols);
			return nullptr;
		}
		++pSymMod->m_RefCount;
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_SDSModuleList.AppendSDSModule(&it->second);
		DebugPrintf("Syser : Load SDS %s < %d symbols , %d source code files >\n", TGetFileName(SDSFullFileName), symbols, sources);
		return &it->second;
	}

	bool CSyserUI::LoadPDBSym(const char *szModule, ULONG_PTR ImageBase, unsigned long ImageSize)
	{
		char SDSFullFileName[MAX_FN_LEN];
		if (!gpSyser->m_SourceDebug.CheckSDS(szModule, SDSFullFileName))
		{
			//TODO dialog "Try load pdb?"
			if (!gpSyser->m_SourceDebug.Load(szModule, SDSFullFileName, eUseSourcesExtern))
				return false;
		}

		CSDSModule *pSDSMod = LoadSDSModule(SDSFullFileName);
		if (!pSDSMod)
		{
			return false;
		}
	//	pSDSMod->m_dwProcessId = gpSyser->m_pSysDebugger->m_dwProcessId;
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_MultiSourceCodeView.UpdateContext();
		return true;
	}

struct DLLDEFMAP
{
	const char *FileDll;
	const char *FileDef;
};

DLLDEFMAP gDllDefTable[] =
	{
		{"mfc42.dll","mfc42.def"},
		{"mfc70.dll","mfc70.def"},
		{"mfc71.dll","mfc71.def"},
		{"mfc80.dll","mfc80.def"},
		{"mfc90.dll","mfc90.def"},

		{"mfc42d.dll","mfc42d.def"},
		{"mfc70d.dll","mfc70d.def"},
		{"mfc71d.dll","mfc71d.def"},
		{"mfc80d.dll","mfc80d.def"},
		{"mfc90d.dll","mfc90d.def"},

		{"mfc42u.dll","mfc42u.def"},
		{"mfc70u.dll","mfc70u.def"},
		{"mfc71u.dll","mfc71u.def"},
		{"mfc80u.dll","mfc80u.def"},
		{"mfc90u.dll","mfc90u.def"},

		{"mfc42ud.dll","mfc42ud.def"},
		{"mfc70ud.dll","mfc70ud.def"},
		{"mfc71ud.dll","mfc71ud.def"},
		{"mfc80ud.dll","mfc80ud.def"},
		{"mfc90ud.dll","mfc90ud.def"},
	};

	const char *GetDefFile(const char *DllFileName)
	{
		int l = TStrLen(DllFileName)-10; //??? WTF
		if (l <= 0) return nullptr;
		DllFileName += l; //??? WTF
		for (int n = 0; n < lenof(gDllDefTable); ++n)
		{
			if (TStrIStr(DllFileName, gDllDefTable[n].FileDll))
				return gDllDefTable[n].FileDef;
		}
		return nullptr;
	}

#include "../Code/fileiont.hpp"

	char *FileRead(const char *FileName, unsigned long *MemLen)
	{
		CLocalFileIO File;
		char *pMem = nullptr;
		*MemLen = 0;
		HANDLE Handle;
		if (File.OpenFile(FileName, &Handle, 0x1'0000))
		{
			unsigned long long Length = File.GetFileLength(Handle);
			if (Length <= 0x10'0000)
			{
				pMem = new char[Length+1];
				if (pMem)
				{
					*MemLen = File.ReadFile(Handle, pMem, Length, 0);
					if (*MemLen != Length)
					{
						*MemLen = 0;
						delete []pMem;
						pMem = nullptr;
					}
				}
			}
			File.CloseHandle(Handle);
		}
		return pMem;
	}

#define MFC_NAMES 10'000

	const char **ReadDefFile(char *Mem, int MemLen)
	{
		const char **pDefMem = new const char*[MFC_NAMES];
		if (pDefMem)
		{
			memset(pDefMem, 0, sizeof(char*)*MFC_NAMES);
			unsigned long Offset = 0;
			for (unsigned long n = 0; n < MemLen; ++n) //<= ???
			{
				if (Mem[n] == '\n')
				{
					Mem[n] = 0;
					const char *StrNum = &Mem[Offset];
					while (IS_DIGIT(Mem[Offset])) ++Offset;
					Mem[Offset] = 0;
					++Offset;
					unsigned long OrdFunc;
					if (USDecStrToNum(StrNum, &OrdFunc))
					{
						while (Mem[Offset] &&
							(Mem[Offset]==' '||Mem[Offset]=='\t')) ++Offset;
						if (OrdFunc < MFC_NAMES)
							pDefMem[OrdFunc] = &Mem[Offset];
					}
					Offset = n;// ???; //TODO
				}
			}
		}
		return pDefMem;
	}

	CSymbolModule * CSyserUI::LoadPESym(const char *szModule, CDbgModule *pDbgMod)
	{
		CPEFile PEFile;
		if (!PEFile.Open(szModule, 0))
			return nullptr;

		CSymbolModule *pSymMod = m_SymbolContainer.FindModule(PEFile.m_PEHead.TimeDateStamp, PEFile.m_PEHead.CheckSum);
		if (!pSymMod)
		{
			pSymMod = m_SymbolContainer.InsertModule(PEFile.m_PEHead.TimeDateStamp, PEFile.m_PEHead.CheckSum);
			if (pSymMod)
				pSymMod->m_ModuleName = szModule;
			if (pDbgMod)
				pDbgMod->SetSymbolModule(pSymMod);
			else	SetSymbolModule(pSymMod);
		}

		const char **DefNames = nullptr;
		const char *DefFileName = GetDefFile(szModule);
		char *Mem = nullptr;
		const char **DefFileMem = nullptr;
		if (DefFileName)
		{
			char szDefFileName[MAX_FN_LEN];
		//#ifdef _RING0_
		//	::GetWindowsDirectoryA(szDefFileName, MAX_FN_LEN);
		//	TStrCat(szDefFileName, "\\system32\\drivers\\plugin\\");
		//#else
			GetModulePath(szDefFileName, true); //+++
			TStrCat(szDefFileName, "\\plugin\\"); //+++
		//#endif
			TStrCat(szDefFileName, DefFileName);
			unsigned long MemLen = 0;
			Mem = FileRead(szDefFileName, &MemLen);
			if (Mem)
			{
				DefFileMem = ReadDefFile(Mem, MemLen);
				if (!DefFileMem)
				{
					delete []Mem;
					Mem = nullptr;
				}
			}
			szDefFileName[0] = 0;

			DefNames = DefFileMem;
		}

		for (int i = 0; i < PEFile.m_ExportFuncCount; ++i)
		{
			const char *FuncName = PEFile.m_ExportFunc[i].FuncName;
			if (DefNames)
			{
				if (PEFile.m_ExportFunc[i].FuncOrd < MFC_NAMES)
				{
					if (DefNames[PEFile.m_ExportFunc[i].FuncOrd])
						FuncName = DefNames[PEFile.m_ExportFunc[i].FuncOrd];
				}
			}
			pSymMod->Add(PEFile.m_ExportFunc[i].Address - PEFile.m_ImageBase, FuncName);
		}

		if (DefNames)
		{
			delete []DefNames;
			delete []Mem;
		}

		char szTitle[MAX_FN_LEN];
		TGetFileTitle(szModule, szTitle);
		pSymMod->Add(0, szTitle);
		PEFile.Close();
		return pSymMod;
	}

	CSymbolModule * CSyserUI::LoadMap(const char *szMapFile, const char *szModule, CDbgModule *pDbgMod)
	{
		CPEFile PEFile;
		if (!PEFile.Open(szModule, 0))
			return nullptr;

		CSymbolModule *pSymMod = m_SymbolContainer.FindModule(PEFile.m_PEHead.TimeDateStamp, PEFile.m_PEHead.CheckSum);
		if (!pSymMod)
		{
			pSymMod = m_SymbolContainer.InsertModule(PEFile.m_PEHead.TimeDateStamp, PEFile.m_PEHead.CheckSum);
			if (pSymMod)
				pSymMod->m_ModuleName = szModule;
			if (pDbgMod)
				pDbgMod->SetSymbolModule(pSymMod);
			else	SetSymbolModule(pSymMod);
		}

		CTXTFile MapFile;
		if (!MapFile.Open(szMapFile, 0))
			return nullptr;

		//TODO X64 MAP FILE
		TList<char *>::IT It = MapFile.m_StrList.Begin();
		while (It != MapFile.m_StrList.End())
		{
			char *Str = *It;

			while (*Str==' '||*Str=='\t') ++Str;
			if (Str[4]!=':') { ++It; continue; }
			Str[4] = 0;
			unsigned long SecNum = 0;
			if (!USHexStrToNum(Str, &SecNum)) { ++It; continue; }
			if (SecNum > PEFile.m_SectCount) { ++It; continue; }
			if (Str[13]=='H'||Str[13]=='h') { ++It; continue; }
			Str[13] = 0;
			ULONG_PTR Addr = 0;
			if (!USHexStrToNum(Str+5, &Addr)) { ++It; continue; }
			if (Addr > PEFile.m_Section[SecNum].VirtualSize) { ++It; continue; }
			Str += 14;
			while (*Str==' '||*Str=='\t') ++Str;
			pSymMod->Add(Addr, Str);
			++It;
		}

		char szTitle[MAX_FN_LEN];
		TGetFileTitle(szModule, szTitle);
		pSymMod->Add(0, szTitle);
		PEFile.Close();
		return pSymMod;
	}

	unsigned long CSyserUI::GetModuleName(char *szModule)
	{
		unsigned long Len = TStrLen(szModule);

		map<CMemoryImage, CDbgModule>::IT it = gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
		//while (it != gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.end())
		while (it)
		{
			if (it->second.m_pSymbolModule)
			{
				map<ULONG_PTR, TAnsiNoCaseString>::IT si = it->second.m_pSymbolModule->m_SymbolMap.begin();
				//while (si != it->second.m_pSymbolModule->m_SymbolMap.end())
				while (si)
				{
					if (!TStrNICmp(szModule, si->second.operator const char*(), Len))
					{
						return TStrCpyLimit(szModule, si->second.operator const char*(), 64);
					}
					++si;
				}
			}	
			++it;
		}

		if (gpSyser->m_pDebugger != gpSyser->m_pSysDebugger)
		{
			map<CMemoryImage, CDbgModule>::IT it = gpSyser->m_pSysDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
			//while (it != gpSyser->m_pSysDebugger->m_DbgModuleList.m_DbgModuleMap.end())
			while (it)
			{
				if (it->second.m_pSymbolModule)
				{
					map<ULONG_PTR, TAnsiNoCaseString>::IT si = it->second.m_pSymbolModule->m_SymbolMap.begin();
					//while (si != it->second.m_pSymbolModule->m_SymbolMap.end())
					while (si)
					{
						if (!TStrNICmp(szModule, si->second.operator const char*(), Len))
						{
							return TStrCpyLimit(szModule, si->second.operator const char*(), 64);
						}
						++si;
					}
				}	
				++it;
			}
		}
		return Len;
	}

	bool CSyserUI::IsGlobeModule(ULONG_PTR Address, const char *szFullFileName)
	{
		//TODO RING0
		return false;
	}

	bool CSyserUI::ToggleInsertBP(ULONG_PTR Address)
	{
		bool ret;
		BREAK_POINT *BreakPoint = gpSyser->m_pDebugger->GetCodeBP(Address, 0, 0x300);
		if (BreakPoint)
			ret = gpSyser->m_pDebugger->DelCodeBP(*BreakPoint);
		else	ret = gpSyser->m_pDebugger->SetCodeBP(Address, 0, BP_STATE_ENABLE) != 0;
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
		gpSyser->m_MainFrame.Update((WISP_RECT*)0);
		return ret;
	}

	bool CSyserUI::ToggleEnableBP(ULONG_PTR Address)
	{
		BREAK_POINT *BreakPoint = gpSyser->m_pDebugger->GetCodeBP(Address, 0, 0x300);
		if (!BreakPoint)
			return false;
		if (BreakPoint->State == BP_STATE_ENABLE)
			return gpSyser->m_pDebugger->DisableCodeBP(*BreakPoint);
		return gpSyser->m_pDebugger->EnableCodeBP(*BreakPoint);
	}

	bool CSyserUI::InsertBP(ULONG_PTR Address, const char *szName, const char *szCondition, const char *szCommand)
	{
		BREAK_POINT *BreakPoint = gpSyser->m_pDebugger->SetCodeBP(Address, 0, BP_STATE_ENABLE);
		if (!BreakPoint)
			return false;
		if (szCondition && *szCondition)
			BreakPoint->Condition = szCondition;
		if (szName && *szName)
			BreakPoint->Name = szName;
		if (szCommand && *szCommand)
			BreakPoint->Command = szCommand;
		return true;
	}

	bool CSyserUI::RemoveBP(ULONG_PTR Address)
	{
		return gpSyser->m_pDebugger->DelCodeBP(Address, 0, 0x300);
		//
	}

	bool CSyserUI::EnableModuleBP(ULONG_PTR Address, bool Enable)
	{
		return false;
	}

	void CSyserUI::EnableAllModuleBP(bool Enable)
	{
	}

	bool CSyserUI::InsertModuleOnloadBP(const char *szName, unsigned int Type, unsigned int State)
	{
		map<TAnsiNoCaseString, MODULE_LOAD>::IT it = m_ModuleLoadBPMap.insert(_Tmap_pair<TAnsiNoCaseString, MODULE_LOAD>(szName, (MODULE_LOAD*)0));
		if (!it) return false;
		it->second.Type = Type;
		it->second.State = State;
		return true;
	}

	void CSyserUI::EnableModuleOnloadBP(bool Enable)
	{
		map<TAnsiNoCaseString,MODULE_LOAD>::IT it = m_ModuleLoadBPMap.begin();
		//while (it != m_ModuleLoadBPMap.end())
		while (it)
		{
			if (!(it->second.Type & 0x300))
				it->second.State = Enable ? 1:0; //BP_STATE_ENABLE:BP_STATE_DISABLE;
			++it;
		}
	}

	void CSyserUI::LoadAPIDef()
	{
		char szModuleName[MAX_FN_LEN];
	//TODO _RING0_
		GetModulePath(szModuleName, true);
		TStrCat(szModuleName, "APIDef.lib");
		CImageFileStream File;
		if (!File.Open(szModuleName, 0))
		{
			DebugPrintf("Syser : Fail to load APIDef %s\n", szModuleName);
			return;
		}

		unsigned long LoadSize = m_APIDef.Read(File);
		DebugPrintf("Syser : Load API %d records\n", m_APIDef.size());
		unsigned long FileSize = File.GetFileLength();
		if (LoadSize != FileSize)
		{
			DebugPrintf("Syser : **********Invalid API Define Data , Load Size [%d] != File Size [%d]**********\n", LoadSize, FileSize);
		}

		File.Close();
	}

	bool CSyserUI::SaveComment()
	{
		char szFileName[MAX_FN_LEN];
		GetModulePath(szFileName, true);
	#if 0
		TStrCat(szFileName, "Syser.cmt");
	#else
		TStrCat(szFileName, TGetFileName(m_SyserCodeDoc.m_ExeFilePath.operator const char*()));
		TStrCat(szFileName, ".cmt");
	#endif
		if (m_CommentContainer.m_CommentModulesMap.size()==0)
		{
			gpFileIO->DeleteFile(szFileName);
			return true;
		}
		CImageFileStream File;
		if (!File.Create(szFileName, 0))
			return false;
		m_CommentContainer.Write(File);
		File.Close();

		for (map<unsigned long long, CCommentModule>::IT it = m_CommentContainer.m_CommentModulesMap.begin();
			it != m_CommentContainer.m_CommentModulesMap.end(); ++it) //???++
		{
			++(it->second.m_RefCount);
		}
		return true;
	}

	bool CSyserUI::LoadComment()
	{
		char szFileName[MAX_FN_LEN];
		GetModulePath(szFileName, true);
	#if 0
		TStrCat(szFileName, "Syser.cmt");
	#else
		TStrCat(szFileName, TGetFileName(m_SyserCodeDoc.m_ExeFilePath.operator const char*()));
		TStrCat(szFileName, ".cmt");
	#endif
		CImageFileStream File;
		if (!File.Open(szFileName, 0))
			return false;
	{
		map<unsigned long long, CCommentModule>::IT it = m_CommentContainer.m_CommentModulesMap.begin();
		while (it != m_CommentContainer.m_CommentModulesMap.end())
		{
			RemoveCommentModule(&it++->second);
		}
	}
		m_CommentContainer.Read(File);
		File.Close();
	{
		for (map<unsigned long long, CCommentModule>::IT it = m_CommentContainer.m_CommentModulesMap.begin();
			it != m_CommentContainer.m_CommentModulesMap.end(); ++it) //???++
		{
			SetCommentModule(&it->second);
			++(it->second.m_RefCount);
		}
	}
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		return true;
	}

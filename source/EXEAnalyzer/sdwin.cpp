#define MYLIBRARY_EXPORT

#include "sdwin.hpp"
#include "../Code/fileio.hpp"

#include <tlhelp32.h>

	MYAPI void GetFileFilter(WCHAR *pStrFilter)
	{
		TStrCpy(pStrFilter, WSTR("Executable Files|*.exe;*.dll;*.sys;*.scr|All Files|*.*|"));
	}

	MYAPI void GetInfo(char *pStrInfo)
	{
	#ifdef _X64_
		  TStrCpy(pStrInfo, "Win64 User Mode Debugger");
	#else
		  TStrCpy(pStrInfo, "Win32 User Mode Debugger");
	#endif
	}

	CSyserUI *gpPluginUI = 0;

	MYAPI void *CreateDebugger(CSyserUI *pSyserUI)
	{
		gpPluginUI = pSyserUI;
		return new CSDWin;
	}

	CSDWin::CSDWin()
	{
		m_dwCurentProcessId = GetCurrentProcess();
		m_hProcess = 0;
		m_bSuspend = false;
		m_Terminate = false;
		m_pCIP = &m_Context.Eip;
	}

	CSDWin::~CSDWin()
	{
	}
#if 0
	#define SE_DEBUG_PRIVILEGE	20L
	void SetPrivilege(bool Enable)
	{
		HANDLE hToken;
		TOKEN_PRIVILEGES tp;
		if(::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES ,&hToken)) //| TOKEN_QUERY
		{
			//LUID luid;
			//::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
			tp.PrivilegeCount = 1;
			//tp.Privileges[0].Luid = luid;
			tp.Privileges[0].Luid.LowPart = SE_DEBUG_PRIVILEGE;//Luid = luid;
			tp.Privileges[0].Luid.HighPart = 0;
			tp.Privileges[0].Attributes = Enable ? SE_PRIVILEGE_ENABLED : 0;
			::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
			::CloseHandle(hToken);
		}
	}
#endif
	bool CSDWin::Open(const char *Name)
	{
		if (!gpFileIO->IsFileExist(Name) && TStrNICmp(Name, "\\PID:", 5))
			return false;

		if (!CDebugger::Open(Name)) //CX86Debugger::Open
			return false;

		unsigned long ThreadId;

		m_ProcessFileName = Name;
		m_hWaitExitEvent = ::CreateEventA(0, 0, 0, 0);
		m_hDbgThread = ::CreateThread(0, 0,
			(LPTHREAD_START_ROUTINE)CSDWin::ThreadProc, this, 0, (LPDWORD)&ThreadId);

		if (!m_hDbgThread)
		{
			CloseHandle(m_hWaitExitEvent);
			CDebugger::Close(); //CX86Debugger::Close
			return false;
		}

		::WaitForSingleObject(m_hWaitExitEvent, INFINITE);
		if (!m_dwProcessId)
		{
			::CloseHandle(m_hWaitExitEvent);
			CDebugger::Close(); //CX86Debugger::Close
			return false;
		}
		return true;
	}

	bool CSDWin::Close()
	{
		if (m_hProcess)
		{
			::CloseHandle(m_hProcess);
			m_hProcess = ::OpenProcess(1, 0, m_dwProcessId);
			//if (m_hProcess)
			if (!::TerminateProcess(m_hProcess, -1)) //-1 ???
			{
				//???
				//return false;
			}

			::CloseHandle(m_hProcess);
			m_hProcess = 0;
			m_Terminate = true;
			ContinueDebug(true);
			::CloseHandle(m_hWaitExitEvent);
		}
		return CDebugger::Close(); //CX86Debugger::Close
	}

	void CSDWin::Release()
	{
		CDebugger::Release();
		delete this;
	}

	bool CSDWin::InsertHandle(unsigned long dwThreadId, HANDLE hThread)
	{
		map<unsigned long, HANDLE>::IT it =
			m_ThreadIdMap.insert(_Tmap_pair<unsigned long, HANDLE>(dwThreadId, (HANDLE*)0));

		if (it == m_ThreadIdMap.end())
			return false;

		  return ::DuplicateHandle(m_dwCurentProcessId, hThread, m_dwCurentProcessId,
				&it->second, 0x5A, 0, 0) != 0;

		//	PROCESS_DUP_HANDLE //0x40
		//	PROCESS_VM_READ //0x10
		//	PROCESS_VM_OPERATION //0x08
		//	PROCESS_CREATE_THREAD //0x02

	}

	bool CSDWin::RemoveHandle(unsigned long dwThreadId)
	{
		return m_ThreadIdMap.erase(dwThreadId) != 0;
	}

	void CSDWin::DebugException(unsigned int Type)
	{
		m_pDebugInterface->OnDebugException(Type);
	}

	bool CSDWin::SaveRegisters(unsigned long dwThreadId)
	{
		map<unsigned long, HANDLE>::IT It = m_ThreadIdMap.find(dwThreadId);
		if (It == m_ThreadIdMap.end())
			return false;

		  m_Context.ContextFlags = CONTEXT_ALL;
		return ::GetThreadContext(It->second, &m_Context) != 0;
	}

	bool CSDWin::UpdateRegisters(unsigned long dwThreadId)
	{
		map<unsigned long, HANDLE>::IT It = m_ThreadIdMap.find(dwThreadId);
		if (It == m_ThreadIdMap.end())
			return false;

		  m_Context.ContextFlags = CONTEXT_ALL;
		return ::SetThreadContext(It->second, &m_Context) != 0;
	}

	bool CSDWin::StepBack(unsigned long dwThreadId)
	{
		if (!SaveRegisters(dwThreadId))
			return false;

		--m_Context.Eip;

		return UpdateRegisters(dwThreadId) != 0;
	}

	unsigned long CSDWin::WriteMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size)
	{
		ULONG_PTR WRSize = 0;

		if (!::WriteProcessMemory(m_hProcess, (LPVOID)Address, Buffer, Size, (SIZE_T *)&WRSize))
			return 0;
		::FlushInstructionCache(m_hProcess, 0, 0); //Buffer, Size
		return WRSize;
	}

	unsigned long CSDWin::ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size)
	{
		ULONG_PTR RDSize = 0;

		if (!::ReadProcessMemory(m_hProcess, (LPCVOID)Address, Buffer, Size, (SIZE_T *)&RDSize))
			return 0;
		return RDSize;
	}


	bool CSDWin::SetCodeBreakPoint(BREAK_POINT & BP)
	{
		return SetX86CodeBreakPoint(BP);
	}

	bool CSDWin::ClearCodeBreakPoint(BREAK_POINT & BP)
	{
		return ClearX86CodeBreakPoint(BP);
	}

	bool CSDWin::SetDataBreakPoint(BREAK_POINT & BP)
	{
		return SetX86DataBreakPoint(BP, &m_Context.Dr0, &m_Context.Dr7);
	}

	bool CSDWin::ClearDataBreakPoint(BREAK_POINT & BP)
	{
		return ClearX86DataBreakPoint(BP, &m_Context.Dr0, &m_Context.Dr7);
	}

	bool CSDWin::SetTrace(bool trace)
	{
		for (map<unsigned long, HANDLE>::IT it = m_ThreadIdMap.begin();
			it != m_ThreadIdMap.end(); ++it)
		{
			CONTEXT regs;
			regs.ContextFlags = CONTEXT_FULL; //0x10000B; ???
			if (::GetThreadContext(it->second, &regs))
			{
				if (trace)
					regs.EFlags |= 0x100;
				else	regs.EFlags &= ~0x100;

				regs.ContextFlags = CONTEXT_FULL;
				if (!::SetThreadContext(it->second, &regs))
				{
					//TODO
				}
			}
		}
		return true;
	}

	bool CSDWin::SetSingleStep()
	{
		return SetTrace(true);
	}

	bool CSDWin::RemoveSingleStep()
	{
		return SetTrace(false);
	}

	bool CSDWin::SaveRegisters()
	{
		bool result = SaveRegisters(m_dwThreadId);
#if 0
		if (result)
		{
			memset(&m_StackFrame, 0, sizeof(m_StackFrame));
		#ifdef _X64_
			m_ImageType = IMAGE_FILE_MACHINE_AMD64;
			m_StackFrame.AddrPC.Offset	= m_Context.Rip;
			m_StackFrame.AddrPC.Mode	= AddrModeFlat;
			m_StackFrame.AddrStack.Offset	= m_Context.Rsp;
			m_StackFrame.AddrStack.Mode	= AddrModeFlat;
			m_StackFrame.AddrFrame.Offset	= m_Context.Rbp;
			m_StackFrame.AddrFrame.Mode	= AddrModeFlat;
		#else //X32
			m_ImageType = IMAGE_FILE_MACHINE_I386;
			m_StackFrame.AddrPC.Offset	= m_Context.Eip;
			m_StackFrame.AddrPC.Mode	= AddrModeFlat;
			m_StackFrame.AddrStack.Offset	= m_Context.Esp;
			m_StackFrame.AddrStack.Mode	= AddrModeFlat;
			m_StackFrame.AddrFrame.Offset	= m_Context.Ebp;
			m_StackFrame.AddrFrame.Mode	= AddrModeFlat;
		#endif
			m_StackFrameContext = m_Context;
		}
#endif
		return result;
	}

	bool CSDWin::UpdateRegisters()
	{
		return UpdateRegisters(m_dwThreadId);
	}

	bool CSDWin::Pause()
	{
		//TODO break at our debuggee app
		::DebugBreakProcess(m_hProcess); //TODO RETURN FALSE
		return true;
	}

	bool CSDWin::ContinueDebug(bool bRestoreScr)
	{
		return ::ResumeThread(m_hDbgThread) != 0;
	}

	//TODO StackReadMemory
	//TODO StackFunctionTableAccess64
	//TODO StackGetModuleBase64
	//TODO StackTranslateAddress64
#if 0
	bool CSDWin::GetStack(STACK_FRAME *pStackFrame)
	{
		if (!::StackWalk64(
			m_ImageType,		// __in      DWORD MachineType,
			m_hProcess,		// __in      HANDLE hProcess, //GetCurrentProcess(),
			m_hThread,		// __in      HANDLE hThread, //GetCurrentThread(),
			&m_StackFrame,		// __inout   LP STACKFRAME64 StackFrame,
			&m_StackFrameContext,		// __inout   PVOID ContextRecord,
			NULL,			// __in_opt  PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,
			SymFunctionTableAccess64,	// __in_opt  PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
			SymGetModuleBase64,	// __in_opt  PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,
			NULL			// __in_opt  PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress
			))
		{
			if (pStackFrame)
			{
				pStackFrame->AddrPC = 0;
				pStackFrame->AddrReturn = 0;
				pStackFrame->AddrFrame = 0;
				pStackFrame->AddrStack = 0;

			}
			return false;
		}
		pStackFrame->AddrPC = m_StackFrame.AddrPC.Offset;
		pStackFrame->AddrReturn = m_StackFrame.AddrReturn.Offset;
		pStackFrame->AddrFrame = m_StackFrame.AddrFrame.Offset;
		pStackFrame->AddrStack = m_StackFrame.AddrStack.Offset;
		return true;
	}
#endif
	void CSDWin::GetX86RegPtr(X86_CPU_REG_PTR *pCPURegPtr, int nCPU)
	{
		GetX86RegPtr(pCPURegPtr);
	}

	void CSDWin::GetX86RegPtr(X86_CPU_REG_PTR *pCPURegPtr)
	{
		pCPURegPtr->pCS = (unsigned short*)&m_Context.SegCs;
		pCPURegPtr->pDS = (unsigned short*)&m_Context.SegDs;
		pCPURegPtr->pES = (unsigned short*)&m_Context.SegEs;
		pCPURegPtr->pFS = (unsigned short*)&m_Context.SegFs;
		pCPURegPtr->pGS = (unsigned short*)&m_Context.SegGs;
		pCPURegPtr->pSS = (unsigned short*)&m_Context.SegSs;
	#ifdef _X64_
		pCPURegPtr->pCAX = &m_Context.Rax;
		pCPURegPtr->pCBX = &m_Context.Rbx;
		pCPURegPtr->pCCX = &m_Context.Rcx;
		pCPURegPtr->pCDX = &m_Context.Rdx;
		pCPURegPtr->pCSI = &m_Context.Rsi;
		pCPURegPtr->pCDI = &m_Context.Rdi;
		pCPURegPtr->pCBP = &m_Context.Rbp;
		pCPURegPtr->pCSP = &m_Context.Rsp;

		pCPURegPtr->pR8 = &m_Context.R8;
		pCPURegPtr->pR9 = &m_Context.R9;
		pCPURegPtr->pR10 = &m_Context.R10;
		pCPURegPtr->pR11 = &m_Context.R11;
		pCPURegPtr->pR12 = &m_Context.R12;
		pCPURegPtr->pR13 = &m_Context.R13;
		pCPURegPtr->pR14 = &m_Context.R14;
		pCPURegPtr->pR15 = &m_Context.R15;

		pCPURegPtr->pEFL = (EFL_REG*)&m_Context.EFlags;
		pCPURegPtr->pCIP = &m_Context.Rip;
	#else
		pCPURegPtr->pCAX = &m_Context.Eax;
		pCPURegPtr->pCBX = &m_Context.Ebx;
		pCPURegPtr->pCCX = &m_Context.Ecx;
		pCPURegPtr->pCDX = &m_Context.Edx;
		pCPURegPtr->pCSI = &m_Context.Esi;
		pCPURegPtr->pCDI = &m_Context.Edi;
		pCPURegPtr->pCBP = &m_Context.Ebp;
		pCPURegPtr->pCSP = &m_Context.Esp;

		pCPURegPtr->pEFL = (EFL_REG*)&m_Context.EFlags;
		pCPURegPtr->pCIP = &m_Context.Eip;
	#endif
		pCPURegPtr->pDR0 = &m_Context.Dr0;
		pCPURegPtr->pDR1 = &m_Context.Dr1;
		pCPURegPtr->pDR2 = &m_Context.Dr2;
		pCPURegPtr->pDR3 = &m_Context.Dr3;
		pCPURegPtr->pDR4 = 0;
		pCPURegPtr->pDR5 = 0;
		pCPURegPtr->pDR6 = &m_Context.Dr6;
		pCPURegPtr->pDR7 = &m_Context.Dr7;

	#ifdef _X64_
		pCPURegPtr->FP_ControlWord = (stFPU_CONTROL_REG *)&m_Context.FltSave.ControlWord;
		pCPURegPtr->FP_StatusWord = (stFPU_STATUS_REG *)&m_Context.FltSave.StatusWord;
		pCPURegPtr->FP_TagWord = (stFPU_TAG_REG *)&m_Context.FltSave.TagWord;
		pCPURegPtr->FP_RegisterArea = (unsigned char*)m_Context.FltSave.FloatRegisters; //10*8
		pCPURegPtr->pXMM = (void *)m_Context.FltSave.XmmRegisters; //16
	#else
		pCPURegPtr->FP_ControlWord = (stFPU_CONTROL_REG *)&m_Context.FloatSave.ControlWord;
		pCPURegPtr->FP_StatusWord = (stFPU_STATUS_REG *)&m_Context.FloatSave.StatusWord;
		pCPURegPtr->FP_TagWord = (stFPU_TAG_REG *)&m_Context.FloatSave.TagWord;
		pCPURegPtr->FP_RegisterArea = (unsigned char*)m_Context.FloatSave.RegisterArea; //10*8
		pCPURegPtr->pXMM = (void *)m_Context.ExtendedRegisters; //8
	#endif
	}



	bool CSDWin::GetSegRegBase(unsigned long Selector, ULONG_PTR *pAddress, unsigned long *pLimit)
	{
		map<unsigned long, HANDLE>::IT it = m_ThreadIdMap.find(m_dwThreadId);
		if (it == m_ThreadIdMap.end())
			return false;

		LDT_ENTRY ldt_entry;
		unsigned int Result = ::GetThreadSelectorEntry(it->second, Selector, &ldt_entry);
		if (Result)
		{
			if (pAddress)
			{
				*pAddress = ldt_entry.HighWord.Bytes.BaseHi << 24;
				*pAddress |= ldt_entry.HighWord.Bytes.BaseMid << 16;
				*pAddress |= ldt_entry.BaseLow;
			}
			if (pLimit)
			{
				*pLimit = (ldt_entry.HighWord.Bits.LimitHi << 16) + ldt_entry.LimitLow;
				if (ldt_entry.HighWord.Bits.Granularity)
					*pLimit = (*pLimit<<12) | 0xFFFF;
			}
		}
		return Result == 1;
	}

	bool CSDWin::ReadPE(MZHEAD *pMZHead, PE_HEAD *pPEHead)
	{
		memset(pPEHead, 0, sizeof(PE_HEAD));

		unsigned long PEOffset;
		if (ReadMemory((unsigned long long)&pMZHead->pfNewHead, &PEOffset, 4) != 4)
			return false;

		if (ReadMemory((ULONG_PTR)pMZHead+PEOffset, pPEHead, sizeof(PE_HEAD)) != sizeof(PE_HEAD))
			return false;

		return pPEHead->Signature == 0x4550;
	}

	unsigned int __stdcall CSDWin::ThreadProc(void *pData) //WINAPI
	{
		CSDWin *p = (CSDWin*)pData;
		
		//unsigned long continueStatus = 0;

		p->m_Terminate = 0;
		p->m_dwProcessId = 0;

		if (TStrStr(p->m_ProcessFileName.operator const char*(), "\\PID:"))
		{
			sscanf(p->m_ProcessFileName, "\\PID:%08X", &p->m_dwProcessId);
			p->m_hProcess = ::OpenProcess(0x1FFFFF, 0, p->m_dwProcessId);
			if (!p->m_hProcess)
			{
				p->m_dwProcessId = 0;
				::SetEvent(p->m_hWaitExitEvent);
				return 0;
			}
			if (!DebugActiveProcess(p->m_dwProcessId))
			{
				::SetEvent(p->m_hWaitExitEvent);
				return 0;
			}
			p->m_Attached = true;
		} else
		{
			_STARTUPINFOA startup_info;
			memset(&startup_info, 0, sizeof(_STARTUPINFOA));
			startup_info.cb = sizeof(_STARTUPINFOA);

			PROCESS_INFORMATION process_info;
			
			char *env = ::GetEnvironmentStringsA();

			char *cmdline = new char[MAX_FN_LEN];
			TStrCpy(cmdline, ""); //TODO

			if (!::CreateProcessA(
				0,//p->m_ProcessFileName,
				(LPSTR)p->m_ProcessFileName.operator const char*(),//0,//cmdline,
				0,
				0,
				TRUE,
				NORMAL_PRIORITY_CLASS|DEBUG_PROCESS,//|DEBUG_ONLY_THIS_PROCESS, //0x21,
				0,//env,
				0,
				&startup_info,
				&process_info))
			{
				::FreeEnvironmentStringsA(env);
				::SetEvent(p->m_hWaitExitEvent);

				if (GetLastError() == ERROR_ELEVATION_REQUIRED)
				{
					char Buffer[1024];
					sprintf(Buffer, "Unable to start: unsufficient privileges!\nRestart debugger as admin\n");
					p->m_pDebugInterface->DisplayMsg(Buffer);
				}
				return 0;
			}
			::FreeEnvironmentStringsA(env);
			p->m_dwProcessId = process_info.dwProcessId;
			p->m_hProcess = process_info.hProcess;
			p->m_hThread = process_info.hThread;
			p->m_Attached = false;
		}

		p->m_MainModuleName = TGetFileName(p->m_ProcessFileName.operator const char*());
		p->m_bSuspend = false;
		memset(&p->m_Context, 0, sizeof(CONTEXT));

		PE_HEAD pe_head;
		char AnsiBuffer[264];
		char DirName[264];
		WCHAR UnicodeBuffer[262];

		DEBUG_EVENT dbg_event;

		while (!p->m_Terminate && ::WaitForDebugEvent(&dbg_event, INFINITE))
		{
			unsigned int continueStatus = DBG_CONTINUE;

			switch (dbg_event.dwDebugEventCode)
			{
			case EXCEPTION_DEBUG_EVENT:
			//	sprintf(AnsiBuffer, "Debug Event : EXCEPTION_EVENT !\n");
			//	p->m_pDebugInterface->DisplayMsg(AnsiBuffer);

				continueStatus = p->ProcessDebugEvent(&dbg_event);
				break;

			case CREATE_THREAD_DEBUG_EVENT:
			//	sprintf(AnsiBuffer, "Debug Event : CREATE_THREAD !\n");
			//	p->m_pDebugInterface->DisplayMsg(AnsiBuffer);

				p->InsertHandle(dbg_event.dwThreadId, dbg_event.u.CreateThread.hThread);
				break;

			case CREATE_PROCESS_DEBUG_EVENT:
			//	sprintf(AnsiBuffer, "Debug Event : CREATE_PROCESS !\n");
			//	p->m_pDebugInterface->DisplayMsg(AnsiBuffer);

				p->m_StepCount = 0;
				p->InsertHandle(dbg_event.dwThreadId, dbg_event.u.CreateProcessInfo.hThread);

				if (!p->m_Attached)
					p->SetCodeBP((ULONG_PTR)dbg_event.u.CreateProcessInfo.lpStartAddress, 0x200, BP_STATE_ENABLE);

				p->ReadPE((MZHEAD*)dbg_event.u.CreateProcessInfo.lpBaseOfImage, &pe_head);


				p->m_pDebugInterface->OnLoadModule(p, p->m_ProcessFileName,
					(ULONG_PTR)dbg_event.u.CreateProcessInfo.lpBaseOfImage,
					pe_head.SizeOfImage,
					pe_head.TimeDateStamp,
					pe_head.CheckSum);

				CloseHandle(dbg_event.u.CreateProcessInfo.hFile);
				break;

			case EXIT_THREAD_DEBUG_EVENT:
			//	sprintf(AnsiBuffer, "Debug Event : EXIT_THREAD !\n");
			//	p->m_pDebugInterface->DisplayMsg(AnsiBuffer);

				p->RemoveHandle(dbg_event.dwThreadId);
				break;

			case EXIT_PROCESS_DEBUG_EVENT:

				p->DelAllCodeBP(0, 0);

				sprintf(AnsiBuffer, "Debug Event : Process Terminated , Exit Code = %d (0x%X) !\n",
					dbg_event.u.ExitProcess.dwExitCode,
					dbg_event.u.ExitProcess.dwExitCode);
				p->m_pDebugInterface->DisplayMsg(AnsiBuffer);

				p->m_pDebugInterface->OnExit(true);
				p->m_Terminate = false;
				break;

			case LOAD_DLL_DEBUG_EVENT:

			//	sprintf(AnsiBuffer, "Debug Event : LOAD_DLL hFile 0x%X lpImage %x!\n",
			//		(unsigned long)dbg_event.u.LoadDll.hFile, dbg_event.u.LoadDll.lpImageName);
			//	p->m_pDebugInterface->DisplayMsg(AnsiBuffer);

				if (dbg_event.u.LoadDll.lpImageName)
				{
					ULONG_PTR AddressName = 0;
					AnsiBuffer[0] = 0;

					p->ReadMemory((ULONG_PTR)dbg_event.u.LoadDll.lpImageName, &AddressName, sizeof(AddressName));
					if (AddressName)
					{
						if (dbg_event.u.LoadDll.fUnicode)
						{
							UnicodeBuffer[0] = 0;
							p->ReadMemory(AddressName, &UnicodeBuffer, MAX_FN_LEN*2);
							WideCharToMultiByte(0, 0, (LPCWSTR)&UnicodeBuffer, -1, AnsiBuffer, MAX_FN_LEN, 0, 0);
						} else
						{
							p->ReadMemory(AddressName, AnsiBuffer, MAX_FN_LEN);
						}
					}
			//		int Len = TStrLen(AnsiBuffer);
			//	if (Len)
			//	p->m_pDebugInterface->DisplayMsg(AnsiBuffer);
			//	sprintf(AnsiBuffer, " Debug Event : LOAD_DLL Len %d !\n",
			//		Len);
			//	p->m_pDebugInterface->DisplayMsg(AnsiBuffer);

					if (TGetFileName(AnsiBuffer) == AnsiBuffer)
					{
						GetSystemDirectoryA(DirName, MAX_PATH);
						TStrCat(DirName, "\\");
						TStrCat(DirName, AnsiBuffer);
						TStrCpy(AnsiBuffer, DirName);
					}

					p->ReadPE((MZHEAD*)dbg_event.u.LoadDll.lpBaseOfDll, &pe_head);

					if (dbg_event.u.LoadDll.lpBaseOfDll && AnsiBuffer[0])
						p->m_pDebugInterface->OnLoadModule(p, AnsiBuffer,
							(ULONG_PTR)dbg_event.u.LoadDll.lpBaseOfDll,
							pe_head.SizeOfImage,
							pe_head.TimeDateStamp,
							pe_head.CheckSum);
				}
				CloseHandle(dbg_event.u.LoadDll.hFile);
				break;

			case UNLOAD_DLL_DEBUG_EVENT:
			//	sprintf(AnsiBuffer, "Debug Event : UNLOAD_DLL !\n");
			//	p->m_pDebugInterface->DisplayMsg(AnsiBuffer);

					if (dbg_event.u.UnloadDll.lpBaseOfDll)
						p->m_pDebugInterface->OnUnloadModule(p,
							(ULONG_PTR)dbg_event.u.UnloadDll.lpBaseOfDll);
				break;

		//	case OUTPUT_DEBUG_STRING_EVENT: u.DebugString
		//	case RIP_EVENT:
			default:
			//	sprintf(AnsiBuffer, "Debug Event : default %x!\n", dbg_event.dwDebugEventCode);
			//	p->m_pDebugInterface->DisplayMsg(AnsiBuffer);

				continueStatus = DBG_EXCEPTION_NOT_HANDLED; //pass exception to debuggee
				break;
			}
			::ContinueDebugEvent(dbg_event.dwProcessId, dbg_event.dwThreadId, continueStatus);
		}

		DebugActiveProcessStop(p->m_dwProcessId);
		DebugSetProcessKillOnExit(0);
		p->m_hProcess = 0;
		::SetEvent(p->m_hWaitExitEvent);
		return 1;
	}

	unsigned int CSDWin::ProcessDebugEvent(DEBUG_EVENT *pEvent)
	{
		unsigned int continueStatus = DBG_CONTINUE;

		DWORD ExceptionCode = pEvent->u.Exception.ExceptionRecord.ExceptionCode;

		if (ExceptionCode == STATUS_BREAKPOINT) //STATUS_WX86_BREAKPOINT
		{
			continueStatus = DBG_CONTINUE;
			if (++m_StepCount == 1)
			{
				if (m_Attached)
				{
					m_dwThreadId = pEvent->dwThreadId;
					SaveRegisters(m_dwThreadId);

					DebugException(0);
					m_bSuspend = true;
					::SetEvent(m_hWaitExitEvent);
					::SuspendThread(m_hDbgThread);
					m_bSuspend = false;

					HANDLE hModules = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessId);
					if (hModules == INVALID_HANDLE_VALUE) //-1)
					{
						//???
						return 0;
					}

					MODULEENTRY32 mod;
					mod.dwSize = sizeof(MODULEENTRY32);
					PE_HEAD pe_head;
					if (::Module32First(hModules, &mod))
					{
						do {
							ReadPE((MZHEAD*)mod.modBaseAddr, &pe_head);

							m_pDebugInterface->OnLoadModule(
								this,
								mod.szExePath,
								(ULONG_PTR)mod.modBaseAddr,
								mod.modBaseSize,
								pe_head.TimeDateStamp,
								pe_head.CheckSum);

						} while (::Module32Next(hModules, &mod));
					}
					::CloseHandle(hModules);
				}
			} else
			{
			//HandleInt3(Address)
				m_dwThreadId = pEvent->dwThreadId;
				bool bHandle = false;
				BREAK_POINT *BreakPoint = GetCodeBP((ULONG_PTR)pEvent->u.Exception.ExceptionRecord.ExceptionAddress, 0, 0);
				if (BreakPoint)
				{
					++BreakPoint->HitCount;
					if (BreakPoint->State == BP_STATE_ENABLE)
					{
						StepBack(pEvent->dwThreadId);
						if (WriteMemory(m_Context.Eip,	&BreakPoint->CCBackup, 1))
							BreakPoint->State = BP_STATE_RECOV;

						if (m_pDebugInterface->TestCondition(BreakPoint->Condition, BreakPoint))
						{
							m_pDebugInterface->RunCmds(BreakPoint->Command);
							if (BreakPoint->Type & 0x100)
							{
							//	if (BreakPoint->OnPatchBreakPoint)
							//		BreakPoint->OnPatchBreakPoint();
							} else
							if ( !(BreakPoint->Type & 0x200) ) //0x300
							{
								//???
								m_pDebugInterface->Reset();
								bHandle = true;
							}
						//	if (BreakPoint->Type & 0x400)
						//		DelCodeBP(BreakPoint);
							if (m_pDebugInterface->Continue())
							{
								DelAllCodeBP(0x200, 0);

								DebugException(1);
								m_bSuspend = true;
								if (m_StepCount == 2)
									::SetEvent(m_hWaitExitEvent);
								::SuspendThread(m_hDbgThread);
								m_bSuspend = false;
							}
						}
						if (IsRecoverBreakPoint())
							SetSingleStep();
					}
				}
			}


		} else
		if (ExceptionCode == STATUS_SINGLE_STEP) //STATUS_WX86_SINGLE_STEP
		{
			continueStatus = DBG_CONTINUE;
			m_dwThreadId = pEvent->dwThreadId;
			SaveRegisters(pEvent->dwThreadId);
		//HandleInt1(Address)
			bool bHandle_ = RecoverBreakPoint()>0;
			bool bHandle = false;
			BREAK_POINT *BreakPoint = GetCodeBP(m_Context.Eip, 0, 0);
			if (BreakPoint)
				++BreakPoint->HitCount;
			{
				if (BreakPoint && BreakPoint->State == BP_STATE_ENABLE)
				{
					if (WriteMemory(m_Context.Eip, (char *)&BreakPoint->CCBackup, 1))
						BreakPoint->State = BP_STATE_RECOV;

					if (m_pDebugInterface->TestCondition(BreakPoint->Condition, BreakPoint))
					{
						m_pDebugInterface->RunCmds(BreakPoint->Command);
						if (BreakPoint->Type & 0x100)
						{
						//	if (BreakPoint->OnPatchBreakPoint)
						//		BreakPoint->OnPatchBreakPoint();
						} else
						if ( !(BreakPoint->Type & 0x200) ) //0x300
						{
							m_pDebugInterface->Reset();
							bHandle = true;
						}
					}
					//if (BreakPoint->Type & 0x400)
					//	DelCodeBP(BreakPoint);
				}
			}

			if (m_pDebugInterface->m_State && m_pDebugInterface->Continue())
			{
				bHandle = true;
			} else
			if (GetDataBP(0, 0, 0) > 0)
			{
				bHandle = true;
			}

			if (bHandle)
			{
				DelAllCodeBP(0x200, 0);

				DebugException(1);
				m_bSuspend = true;
				::SuspendThread(m_hDbgThread);
				m_bSuspend = false;
			}
			if (IsRecoverBreakPoint())
				SetSingleStep();
		} else
		if (ExceptionCode == DBG_CONTROL_C ||
		    ExceptionCode == DBG_CONTROL_BREAK)
		{
			continueStatus = DBG_CONTINUE;
			m_dwThreadId = pEvent->dwThreadId;
			SaveRegisters(pEvent->dwThreadId);

			DebugException(5);
			m_bSuspend = true;
			::SuspendThread(m_hDbgThread);
			m_bSuspend = false;
		} else
		if (ExceptionCode == STATUS_ACCESS_VIOLATION)
		{
			continueStatus = DBG_CONTINUE;
			m_dwThreadId = pEvent->dwThreadId;
			SaveRegisters(pEvent->dwThreadId);

			m_ExceptionAddress = (ULONG_PTR)pEvent->u.Exception.ExceptionRecord.ExceptionAddress;
			char Buffer[260];
			sprintf(Buffer, "Debug Event : Access Violation , Address = " F0ADDR "X !\n", m_ExceptionAddress);
			m_pDebugInterface->DisplayMsg(Buffer);

			DebugException(4);
			m_bSuspend = true;
			::SuspendThread(m_hDbgThread);
			SaveRegisters(pEvent->dwThreadId);
			m_bSuspend = false;

		} else
		{
			continueStatus = DBG_EXCEPTION_NOT_HANDLED;
			m_dwThreadId = pEvent->dwThreadId;
			SaveRegisters(pEvent->dwThreadId);

			DebugException(0);
			m_bSuspend = true;
			::SuspendThread(m_hDbgThread);
			SaveRegisters(pEvent->dwThreadId);
			m_bSuspend = false;
		}

		return continueStatus;
	}


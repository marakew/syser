
#ifndef _SDWIN_HPP_
#define _SDWIN_HPP_

#include "../Code/define.h"
#include "../Code/alt/altmap.hpp"
#include "x86debugger.hpp"
#include "mzhead.hpp"
#include "pehead.hpp"

//#include "../Code/osdefine.hpp"

//#include <windows.h>
#if 0
#include <dbghelp.h>
#endif
	struct CSDWin : public CX86Debugger
	{
		TAnsiString m_ProcessFileName;
		map<unsigned long, HANDLE> m_ThreadIdMap;

		HANDLE m_hDbgThread;
		HANDLE m_hThread;
		HANDLE m_hProcess;
		HANDLE m_dwCurentProcessId;
		CONTEXT m_Context;
		ULONG_PTR m_ExceptionAddress;
		HANDLE m_hWaitExitEvent;
		bool m_bSuspend;
		bool m_Terminate;
		bool m_Attached;

		unsigned long m_StepCount;
#if 0
		DWORD m_ImageType;
		STACKFRAME64 m_StackFrame;
		CONTEXT m_StackFrameContext;
#endif
	public:
		CSDWin();
		virtual ~CSDWin();

		bool InsertHandle(unsigned long dwThreadId, HANDLE hThread);
		bool RemoveHandle(unsigned long dwThreadId);

		void DebugException(unsigned int Type);

		bool ReadPE(MZHEAD *pMZHead, PE_HEAD *pPEHead);
		unsigned int ProcessDebugEvent(DEBUG_EVENT *pEvent);
		static unsigned int __stdcall ThreadProc(void *pData); //WINAPI

		virtual void Release() override;
		virtual bool Open(const char *Name) override;
		virtual bool Close() override;

		virtual unsigned long WriteMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size) override;
		virtual unsigned long ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size) override;

		bool SaveRegisters(unsigned long dwThreadId);
		bool UpdateRegisters(unsigned long dwThreadId);
		bool StepBack(unsigned long dwThreadId);

		virtual bool SaveRegisters() override;
		virtual bool UpdateRegisters() override;

		virtual bool SetCodeBreakPoint(BREAK_POINT & BP) override;
		virtual bool ClearCodeBreakPoint(BREAK_POINT & BP) override;
		virtual bool SetDataBreakPoint(BREAK_POINT & BP) override;
		virtual bool ClearDataBreakPoint(BREAK_POINT & BP) override;

		bool SetTrace(bool trace); //++
		virtual bool SetSingleStep() override;
		virtual bool RemoveSingleStep() override;
		virtual bool Pause() override;
		virtual bool ContinueDebug(bool bRestoreScr) override;
#if 0
		virtual bool GetStack(STACK_FRAME *pStackFrame) override;
#endif
		virtual void GetX86RegPtr(X86_CPU_REG_PTR *pCPURegPtr) override;
		virtual void GetX86RegPtr(X86_CPU_REG_PTR *pCPURegPtr, int nCPU) override;

		virtual bool GetSegRegBase(unsigned long Selector, ULONG_PTR *pAddress, unsigned long *pBits) override;
	};

	struct CSyserUI; //??? include "plugin.hpp" CSyserPluginUI

#if defined(MYLIBRARY_EXPORT)
#   define MYAPI   __declspec(dllexport)
#else
#   define MYAPI   __declspec(dllimport)
#endif

	extern "C" MYAPI void GetFileFilter(WCHAR *Filter);
	extern "C" MYAPI void GetInfo(char *pInfo);
	extern "C" MYAPI void *CreateDebugger(CSyserUI *pSyserUI);

#endif

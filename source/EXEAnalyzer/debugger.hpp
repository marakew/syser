
#ifndef _DEBUGGER_HPP_
#define _DEBUGGER_HPP_

//#define CODE_OS_DEFINED

//modules
#include "../Code/define.h"
#include "../Code/tstrmem.hpp"
#include "../Code/alt/altlist.hpp"
#include "../Code/alt/altmap.hpp"
#include "symbol.hpp"

	enum BP_STATE
	{
		BP_STATE_DISABLE = 0,
		BP_STATE_ENABLE = 1,
		BP_STATE_RECOV = 2,
		BP_STATE_REMOVE = 3,
	};

	//0x001 //BP_CODE ??? GLOBAL
	//0x002 //BP_EXECUTE
	//0x004 //BP_WRITE
	//0x008 //BP_READ ???
	//0x010 //IO
	//0x020 //IO
	//0x040
	//0x080
	//0x100 //BP_PATCH
	//0x200 //BP_SLNC
	//0x400 //BP_ONCE ONLOAD
	//0x800

	struct BREAK_POINT
	{
		struct MODULE
		{
			TAnsiString ModuleName;
			TAnsiString Condition;
			TAnsiString Name;
			TAnsiString Command;
			unsigned int State; //BP_STATE
			//???
		};

		TAnsiString Name;
		TAnsiString TypeName;
		TAnsiString Condition;
		TAnsiString Owner;
		TAnsiString Command;
		unsigned long HitCount;
		ULONG_PTR Address;
		unsigned long ProcessID;
		unsigned int State; //BP_STATE
		unsigned int Type;
		unsigned long Size;
		unsigned long long PatchFN;
		MODULE *pModule;
		union
		{
			struct 
			{
				unsigned char DRIndex;
				unsigned char Align;
			} X86_DR;
			unsigned char CCBackup;
		};
#if 0
		bool operator==(const BREAK_POINT & BP) const
		{
			if (Name != BP.Name) return false;
			if (TypeName != BP.TypeName) return false;
			if (Condition != BP.Condition) return false;
			if (Owner != BP.Owner) return false;
			if (Command != BP.Command) return false;
			if (HitCount != BP.HitCount) return false;
			if (Address != BP.Address) return false;
			if (ProcessID != BP.ProcessID) return false;
			if (State != BP.State) return false;
			if (Type != BP.Type) return false;
			if (Size != BP.Size) return false;
			if (PatchFN != BP.PatchFN) return false;
			if (pModule != BP.pModule) return false;
			//???
			return true;
		} //??? TODO std::map require
#endif
	};

	enum BP_STATUS
	{
		BP_STATUS_NEW_BP  = 0, //OK
		BP_STATUS_NO_PID  = 1,
		BP_STATUS_NO_ADDR  = 2,
		BP_STATUS_EXIST_BP  = 3,
		BP_STATUS_NOT_EXIST_BP  = 4,
		BP_STATUS_FAIL_ADD_BP  = 5,
		BP_STATUS_FAIL_DEL_BP  = 6,
	};

	struct STACK_FRAME
	{
		ULONG_PTR AddrPC;
		ULONG_PTR AddrReturn;
		ULONG_PTR AddrFrame;
		ULONG_PTR AddrStack;		
	};

	struct CDebugInterface;
	struct CDebugger
	{
		map<unsigned long long, BREAK_POINT> m_CodeBPMap;
		TList<BREAK_POINT> m_DataBPList;
		unsigned long m_dwProcessId;
		unsigned long m_dwThreadId;
		TAnsiString m_MainModuleName;
		ULONG_PTR *m_pCIP;
		CDebugInterface *m_pDebugInterface;
	//	unsigned long m_DebugParam;
	//	void *m_DebugParamPtr;
		CDbgModuleList m_DbgModuleList;
		map<unsigned long long, BREAK_POINT> *m_pCodeBPMap;
		TList<BREAK_POINT> *m_pDataBPList;
		unsigned int m_Status;
	public:
		CDebugger();
		virtual ~CDebugger();

		static unsigned long Read(ULONG_PTR Address, void *Buffer, unsigned long Size, void *Obj);

		virtual bool Init(CDebugInterface *pDebugInterface);
		virtual void Release();
		virtual bool Open(const char *Name); //{ return true; } //unsigned long DebugParam, void *DebugParamPtr
		virtual bool Close();
		virtual unsigned long WriteMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size); //{ return false; }
		virtual unsigned long ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size); //{ return false; }
		virtual unsigned long MoveMemory(ULONG_PTR Des, ULONG_PTR Src, unsigned long Length);
		virtual unsigned long CompareMemory(ULONG_PTR Des, ULONG_PTR Src, unsigned long Length);
		virtual ULONG_PTR SearchMemory(ULONG_PTR Address, unsigned long Length, const void *Pattern, unsigned long PatternLength, bool bCaseCmp);

		virtual bool SaveRegisters(); //{ return true; }
		virtual bool UpdateRegisters(); //{ return true; }

		virtual bool SetCodeBreakPoint(BREAK_POINT & BP); //{ return true; }
		virtual bool ClearCodeBreakPoint(BREAK_POINT & BP); //{ return true; }
		virtual bool SetDataBreakPoint(BREAK_POINT & BP); //{ return true; }
		virtual bool ClearDataBreakPoint(BREAK_POINT & BP); //{ return true; }

		virtual bool GetMemoryAddress(ULONG_PTR, unsigned long long *RealAddress);
		virtual bool OpenProcess(unsigned long dwProcessId); //{ return true; }
		virtual void CloseProcess(); //{ }

		virtual bool SetSingleStep(); //{ return true; }
		virtual bool RemoveSingleStep(); //{ return true; }
	//	virtual bool Reset();
		virtual bool Pause(); //{ return true; }
		virtual bool ContinueDebug(bool bRestoreScr); //{ return false; }
#if 0
		virtual bool GetStack(STACK_FRAME *pStackFrame);// { return false; }
#endif
		static bool CheckAllowType(unsigned int TypeInc, unsigned int TypeExc, const BREAK_POINT & BP);

		// CODE

		virtual BREAK_POINT *GetCodeBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc);

		virtual BREAK_POINT *SetCodeBP(ULONG_PTR Address, unsigned int Type, unsigned int State);

		virtual bool DelCodeBP(BREAK_POINT & BP);
		virtual bool DelCodeBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc);

		virtual int DelAllCodeBP(unsigned int TypeInc, unsigned int TypeExc);

		virtual int EnableAllCodeBP(unsigned int TypeInc, unsigned int TypeExc);
		virtual int DisableAllCodeBP(unsigned int TypeInc, unsigned int TypeExc);

		virtual bool EnableCodeBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc);
		virtual bool EnableCodeBP(BREAK_POINT & BP);

		virtual bool DisableCodeBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc);
		virtual bool DisableCodeBP(BREAK_POINT & BP);

		// DATA
		virtual BREAK_POINT *GetDataBP1(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc);

		virtual int GetDataBP(unsigned int TypeInc, unsigned int TypeExc, TList<BREAK_POINT> *pBPList);

		virtual BREAK_POINT *SetDataBP(ULONG_PTR Address, unsigned int Type, unsigned int State, unsigned int Size);

		virtual bool DelDataBP(BREAK_POINT & BP);
		virtual int DelDataBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc);

		virtual int DelAllDataBP(unsigned int TypeInc, unsigned int TypeExc);

		virtual int EnableAllDataBP(unsigned int TypeInc, unsigned int TypeExc);
		virtual int DisableAllDataBP(unsigned int TypeInc, unsigned int TypeExc);

		virtual int EnableDataBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc);
		virtual bool EnableDataBP(BREAK_POINT & BP);

		virtual int DisableDataBP(ULONG_PTR Address, unsigned int TypeInc, unsigned int TypeExc);
		virtual bool DisableDataBP(BREAK_POINT & BP);
	};

	struct CCodeDoc
	{
		bool m_bOpen;

		ULONG_PTR m_BeginAddr;
		ULONG_PTR m_EndAddr;
	public:
		CCodeDoc()
			: m_bOpen(false)
			, m_BeginAddr(0) //m_ImageBase
			, m_EndAddr(-1) //m_ImageHighBase
		{
		}

		virtual ~CCodeDoc()
		{
		}

		virtual bool Open(const char *szModule) //04 //TODO PARAM
		{
			m_bOpen = true;
			return true;
		}
		virtual void Close()			//08
		{
			m_bOpen = false;
		}
		virtual unsigned long ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size) = 0;	//0C
		virtual ULONG_PTR GetInstrAddress(ULONG_PTR Address, long Count) = 0;	//10

		virtual bool GetSymbol(ULONG_PTR Address, WCHAR *szName, int Length) { return false; }	//14
		virtual bool GetComment(ULONG_PTR Address, WCHAR *szName, int Length) { return false; }	//18
		virtual bool SetComment(ULONG_PTR Address, const WCHAR *szName) { return false; }			//1C
		virtual bool GetFunction(ULONG_PTR Address, WCHAR *szName, int Length) { return GetSymbol(Address, szName, Length); }	//20
		virtual unsigned long GetPic(ULONG_PTR Address) { return 1; } //24
	};

	enum
	{
		CONTINUE	= 0,
		TRACE_RANGE_IN	= 1,
		TRACE_RANGE_OUT	= 2,
		STEP_RANGE_IN	= 3,
		STEP_RANGE_OUT	= 4,
		
		TRACE_UNTIL	= 5,
		STEP_UNTIL	= 6,

		TRACE_CONDITION	= 7,
		STEP_CONDITION	= 8,

		RETURN_UNTIL	= 9,
		
		BPR_IN		= 10,
		RESET_STATE	= 11,
	};

	enum
	{
		PLUNGE_START = 0,
		PLUNGE_BREAK_POINT = 1,

		PLUNGE_ACCESS_VIOLATION = 4,
		PLUNGE_BREAK = 5,
	};

	struct CDebugInterface
	{
		CCodeDoc *m_pCodeDoc;
		CSymbolContainer *m_pSymbolContainer;
		CCommentContainer *m_pCommentContainer;
		TAnsiString m_Condition;
		CDebugger *m_pReturn;
		unsigned int m_Until;
		unsigned int m_State;
		ULONG_PTR m_MinCIP;
		ULONG_PTR m_MaxCIP;
		ULONG_PTR m_PageAddress;
		ULONG_PTR m_Address;
	public:
		virtual void OnLoadModule(CDebugger *pDebugger, const char *szName, ULONG_PTR ImageBase, unsigned long ImageSize, unsigned long TimeStamp, unsigned long CheckSum) = 0;
		virtual void OnUnloadModule(CDebugger *pDebugger, ULONG_PTR ImageBase) = 0;
		virtual void OnTestCondition() {}

		virtual bool Close(bool bExit) = 0;
		virtual bool Plunge(unsigned int Code) = 0;

		virtual bool OnExit(bool bExit) = 0;
		virtual bool OnDebugException(unsigned int Code) = 0;

		virtual void DisplayMsg(const char *szMsg) {}
		virtual void UpdateContext() {}
		virtual bool TestCondition(const char *Condition, BREAK_POINT *BreakPoint) { return true; }
		virtual bool RunCmd(const char *szCmd) { return false; }
		virtual void RunCmds(const char *szCmds) {}

		virtual bool Continue();
		virtual void Trace(ULONG_PTR MinCIP, ULONG_PTR MaxCIP, bool State);
		virtual void Step(ULONG_PTR MinCIP, ULONG_PTR MaxCIP, bool State);
		virtual void Trace(const char *Condition);
		virtual void Step(const char *Condition);
		virtual void SetBPR(ULONG_PTR PageAddress, ULONG_PTR Address); //++
		virtual void Trace(unsigned long Count);
		virtual void Step(unsigned long Count);
		virtual void Return(CDebugger *pDebugger);
		virtual void Reset();
		virtual bool BreakBPR() { return true; } //++
		virtual CDebugger *GetDebugger() = 0;
	};


#endif

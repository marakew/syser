
#ifndef _X86DEBUGGER_HPP_
#define _X86DEBUGGER_HPP_

#include "debugger.hpp"

	struct EFL_REG
	{
		unsigned long CF : 1;
		unsigned long b1 : 1;
		unsigned long PF : 1;
		unsigned long b3 : 1;

		unsigned long AF : 1;
		unsigned long b5 : 1;
		unsigned long ZF : 1;
		unsigned long SF : 1;

		unsigned long TF : 1;
		unsigned long IF : 1;
		unsigned long DF : 1;
		unsigned long OF : 1;

		unsigned long IOPL : 2;
		unsigned long NT : 1;
		unsigned long bf : 1;

		unsigned long RF : 1;
		unsigned long VM : 1;
		unsigned long AC : 1;
		unsigned long VIF : 1;

		unsigned long VIP : 1;
		unsigned long ID : 1;
		unsigned long _22_23 : 2;

		unsigned long _24_31 : 8;
	};

	struct stFPU_STATUS_REG
	{
		unsigned short IE : 1;
		unsigned short DE : 1;
		unsigned short ZE : 1;
		unsigned short OE : 1;
		unsigned short UE : 1;
		unsigned short PE : 1;
		unsigned short SF : 1;
		unsigned short ES : 1;
		unsigned short C0 : 1;
		unsigned short C1 : 1;
		unsigned short C2 : 1;
		unsigned short TOP : 3;
		unsigned short C3 : 1;
		unsigned short B : 1;
	};

	struct stFPU_CONTROL_REG
	{
		unsigned short IM : 1;
		unsigned short DM : 1;
		unsigned short ZM : 1;
		unsigned short OM : 1;
		unsigned short UM : 1;
		unsigned short PM : 1;
		unsigned short Reserved1 : 2;
		unsigned short PC : 2;
		unsigned short RC : 2;
		unsigned short X : 1;
		unsigned short Reserved2 : 3;
	};

	struct stFPU_TAG_REG
	{
		unsigned short TAG0 : 2;
		unsigned short TAG1 : 2;
		unsigned short TAG2 : 2;
		unsigned short TAG3 : 2;
		unsigned short TAG4 : 2;
		unsigned short TAG5 : 2;
		unsigned short TAG6 : 2;
		unsigned short TAG7 : 2;
	};

	struct X86_CPU_REG
	{
		ULONG_PTR CAX;
		ULONG_PTR CBX;
		ULONG_PTR CCX;
		ULONG_PTR CDX;
		ULONG_PTR CDI;
		ULONG_PTR CSI;
		ULONG_PTR CBP;
		ULONG_PTR CSP;
	#ifdef _X64_
		ULONG_PTR R8;
		ULONG_PTR R9;
		ULONG_PTR R10;
		ULONG_PTR R11;
		ULONG_PTR R12;
		ULONG_PTR R13;
		ULONG_PTR R14;
		ULONG_PTR R15;
	#endif
		ULONG_PTR CIP;
	//???	EFL_REG EFL;
		unsigned long EFL;
		unsigned short CS;
		unsigned short DS;
		unsigned short ES;
		unsigned short FS;
		unsigned short GS;
		unsigned short SS;
		ULONG_PTR DR0;
		ULONG_PTR DR1;
		ULONG_PTR DR2;
		ULONG_PTR DR3;
		ULONG_PTR DR4;
		ULONG_PTR DR5;
		ULONG_PTR DR6;
		ULONG_PTR DR7;
	};

	struct DECLSPEC_ALIGN(16) XMMREG
	{
		unsigned long long Low;
		long long High;
	};

	struct YMMREG
	{
		XMMREG Low; //XMM/SSE
		XMMREG High; //AVX
	};

	struct X86_CPU_REG_PTR
	{
		ULONG_PTR *pCAX;
		ULONG_PTR *pCBX;
		ULONG_PTR *pCCX;
		ULONG_PTR *pCDX;
		ULONG_PTR *pCDI;
		ULONG_PTR *pCSI;
		ULONG_PTR *pCBP;
		ULONG_PTR *pCSP;
	#ifdef _X64_
		ULONG_PTR *pR8;
		ULONG_PTR *pR9;
		ULONG_PTR *pR10;
		ULONG_PTR *pR11;
		ULONG_PTR *pR12;
		ULONG_PTR *pR13;
		ULONG_PTR *pR14;
		ULONG_PTR *pR15;
	#endif
		ULONG_PTR *pCIP;
		EFL_REG *pEFL;
		unsigned short *pCS;
		unsigned short *pDS;
		unsigned short *pES;
		unsigned short *pFS;
		unsigned short *pGS;
		unsigned short *pSS;
		ULONG_PTR *pDR0;
		ULONG_PTR *pDR1;
		ULONG_PTR *pDR2;
		ULONG_PTR *pDR3;
		ULONG_PTR *pDR4;
		ULONG_PTR *pDR5;
		ULONG_PTR *pDR6;
		ULONG_PTR *pDR7;
	#ifdef _RING0_
		unsigned long *pGDT;
		unsigned long *pGDTLimit;
		unsigned long *pIDT;
		unsigned long *pIDTLimit;
		unsigned long *pLDT;
		unsigned long *pTSS;
		unsigned long *pCR2;
	#endif
		stFPU_CONTROL_REG *FP_ControlWord;
		stFPU_STATUS_REG *FP_StatusWord;
		stFPU_TAG_REG *FP_TagWord;
		unsigned char *FP_RegisterArea; //MMX low 8 byte as "unsigned long long" 8-x86,16-x64
		void *pXMM; //8-x32/16-x64
		void *pYMM; //8-x32/16-x64

	#ifdef _X64_
		unsigned char *GetFPU(unsigned long Index)
			{ return (unsigned char*)FP_RegisterArea + (Index*16); }
	#else
		unsigned char *GetFPU(unsigned long Index)
			{ return (unsigned char*)FP_RegisterArea + (Index*10); }
	#endif

	#ifdef _X64_
		unsigned long *GetXMM(unsigned long Index)
			{ return (unsigned long*)pXMM + Index*4; }
	#else
		unsigned long *GetXMM(unsigned long Index)
			{ return (unsigned long*)((unsigned char*)pXMM + (Index+10)*16); }
	#endif
	};

	struct CX86Debugger : public CDebugger
	{
		unsigned int m_CodeMode;

	public:
		CX86Debugger();
		virtual ~CX86Debugger();

		virtual void GetX86RegPtr(X86_CPU_REG_PTR *pCPURegPtr); // { }
		virtual void GetX86RegPtr(X86_CPU_REG_PTR *pCPURegPtr, int nCPU); // { }

		virtual bool GetSegRegBase(unsigned long Selector, ULONG_PTR *pAddress, unsigned long *pLimit); // { return false; }

		virtual bool SetX86CodeBreakPoint(BREAK_POINT & BP);
		virtual bool ClearX86CodeBreakPoint(BREAK_POINT & BP);

		virtual bool SetRecoverBP(BREAK_POINT & BP);

		virtual bool SetX86DataBreakPoint(BREAK_POINT & BP, ULONG_PTR DRx[], ULONG_PTR *pDR7);
		virtual bool ClearX86DataBreakPoint(BREAK_POINT & BP, ULONG_PTR DRx[], ULONG_PTR *pDR7);

		virtual bool Halt(unsigned long Type);

		virtual bool IsRecoverBreakPoint();
		virtual unsigned int RecoverBreakPoint();
	};

#endif

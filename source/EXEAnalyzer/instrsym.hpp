
#ifndef _INSTRSYM_HPP_
#define _INSTRSYM_HPP_

#include "../Code/define.h"
#include "../Code/tstrmem.hpp"

#include "inteldis.hpp"

#define MAX_INSTR_LEN 16

unsigned int GetSize(unsigned long long Address);
bool GetSign(_DIS_ADDRESS *dis_a);

struct CInstrSym : public CIntelDisassembler
{

	unsigned char m_CodeBuffer[MAX_INSTR_LEN];
	bool m_bUseSym;

	ULONG_PTR m_ImageBase;
	ULONG_PTR m_ImageHighBase;
	unsigned long m_UxAlign;
	unsigned long m_Style;

	virtual bool AddrToSym(ULONG_PTR Address, char *szSym, unsigned int SymLen);
	virtual unsigned long ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size);
	virtual bool GetSymbolBase(ULONG_PTR *BaseAddr);

	WCHAR *OptItemToCS(_OPERAND_ITEM *OpArray, int nOp, WCHAR *pString, _INSTRUCTION_INFORMATION *pDasmInstr);
	void OptItemSymToCS(_OPERAND_ITEM *OpArray, WCHAR *pString, _INSTRUCTION_INFORMATION *pDasmInstr);
	WCHAR *InstrToCS(_INSTRUCTION_INFORMATION *pInstrInfo, WCHAR *pString);

	CInstrSym();
	virtual ~CInstrSym();

	void SetDasmStyle(unsigned long Style);
	unsigned long GetDasmStyle() const;

	static bool GetJump(unsigned char *CodeBuff, ULONG_PTR Address, ULONG_PTR *target);
	static bool CheckJump(unsigned char *CodeBuff, unsigned long eflags);

	unsigned long Dasm(_INSTRUCTION_INFORMATION *pDasmInstr, DIS_CPU *pDisCPU, unsigned long MaxSize);
	unsigned long Asm(char *pAsmString, unsigned char *CodeBuffer, ULONG_PTR Address);

	unsigned long PrevInstrLen(ULONG_PTR Address);

	unsigned long InstrCount(ULONG_PTR Address, unsigned long CodeSize, unsigned char *LenAry);

	bool IsProcessCode(ULONG_PTR Address);

	bool GetAddr(ULONG_PTR Address, ULONG_PTR *Addr);
	bool GetReturn(ULONG_PTR Address, ULONG_PTR *Value);

	int GetAddrImm(ULONG_PTR Address, ULONG_PTR *Addr, ULONG_PTR *Imm);

	bool GetAddrValue(ULONG_PTR Address, ULONG_PTR *Return);

	unsigned long GetCall(ULONG_PTR Address, char *FunctionName, ULONG_PTR *Return);

	bool GetFunctionName(ULONG_PTR Address, char *FunctionName);

	unsigned long InstrLen(ULONG_PTR Address, unsigned long MaxSize);

	static unsigned long InstrLen64(unsigned char *inbuff, unsigned long MaxSize);
	static unsigned long InstrLen32(unsigned char *inbuff, unsigned long MaxSize);
	static unsigned long InstrLen16(unsigned char *inbuff, unsigned long MaxSize);

	static const WCHAR *m_RegStr[3][GENE_IDX_MAX];
	static const WCHAR *m_ScaleStr[4];
	static const WCHAR *m_SizePtr[8+1];
	static const WCHAR *m_SizeSym[10+1];
	static const WCHAR *m_SegStr[SEG_IDX_MAX];
	static const WCHAR *m_LengthStr[4+1];
	static const WCHAR *m_DefaultOPSize[3];
};

#endif
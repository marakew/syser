
#include "instrsym.hpp"

	const WCHAR *CInstrSym::m_RegStr[3][GENE_IDX_MAX] = {
		{WSTR("AX"),WSTR("CX"),WSTR("DX"),WSTR("BX"),WSTR("SP"),WSTR("BP"),WSTR("SI"),WSTR("DI"),
		WSTR("R8W"),WSTR("R9W"),WSTR("R10W"),WSTR("R11W"),WSTR("R12W"),WSTR("R13W"),WSTR("R14W"),WSTR("R15W"),},

		{WSTR("EAX"),WSTR("ECX"),WSTR("EDX"),WSTR("EBX"),WSTR("ESP"),WSTR("EBP"),WSTR("ESI"),WSTR("EDI"),
		WSTR("R8D"),WSTR("R9D"),WSTR("R10D"),WSTR("R11D"),WSTR("R12D"),WSTR("R13D"),WSTR("R14D"),WSTR("R15D"),},

		{WSTR("RAX"),WSTR("RCX"),WSTR("RDX"),WSTR("RBX"),WSTR("RSP"),WSTR("RBP"),WSTR("RSI"),WSTR("RDI"),
		WSTR("R8"),WSTR("R9"),WSTR("R10"),WSTR("R11"),WSTR("R12"),WSTR("R13"),WSTR("R14"),WSTR("R15"),}};

	const WCHAR *CInstrSym::m_ScaleStr[4] = {
		WSTR("1"),WSTR("2"),WSTR("4"),WSTR("8")};

	const WCHAR *CInstrSym::m_SizePtr[8+1] = {
		WSTR("Err"),WSTR("%X"),WSTR("%X"),WSTR("Err"),WSTR("%X"),WSTR("Err"),WSTR("Err"),WSTR("Err"),WSTR("%I64X")};

	const WCHAR *CInstrSym::m_SizeSym[10+1] = {
		WSTR(""),WSTR("BYTE"),WSTR("WORD"),WSTR(""),WSTR("DWORD"),WSTR(""),WSTR("FWORD"),WSTR(""),WSTR("QWORD"),WSTR(""),WSTR("TWORD")};

	const WCHAR *CInstrSym::m_SegStr[SEG_IDX_MAX] = {
		WSTR("ES"),WSTR("CS"),WSTR("SS"),WSTR("DS"),WSTR("FS"),WSTR("GS")};

	const WCHAR *CInstrSym::m_LengthStr[4+1] = {
		WSTR("DB"),WSTR("DB"),WSTR("DW"),WSTR("Err"),WSTR("DD")};

	const WCHAR *CInstrSym::m_DefaultOPSize[3] = {
		WSTR("%04X"),WSTR("%08X"), WSTR("%016I64X")};

	unsigned int GetSize(unsigned long long Address)
	{
	//#ifdef _X64_
		if (Address & 0xFFFFFFFF00000000) return 8;
	//#endif
		if (Address & 0xFFFF0000) return 4;
		if (Address & 0xFF00) return 2;
		return 1;
	}

	bool GetSign(_DIS_ADDRESS *dis_a)
	{
		switch (dis_a->displacement_size)
		{
		case 1:
			//return dis_a->displacement < 0x80;
		case 2:
			return dis_a->displacement < 0xFFFFFF00;
		case 4:
			return dis_a->displacement < 0xFFFF0000;
		case 8:
			return true;
		default:;
		}
		return false;
	}

	bool CInstrSym::AddrToSym(ULONG_PTR Address, char *szSym, unsigned int SymLen)
	{
		return false;
	}

	unsigned long CInstrSym::ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size)
	{
		for (unsigned long i = 0; i < Size; ++i)
			static_cast<BYTE*>(Buffer)[i] = Address + i;
		return Size;
	}

	bool CInstrSym::GetSymbolBase(ULONG_PTR *BaseAddr)
	{
		return false;
	}

	WCHAR *CInstrSym::OptItemToCS(_OPERAND_ITEM *OpArray, int nOp, WCHAR *pString, _INSTRUCTION_INFORMATION *pDasmInstr)
	{
		WCHAR szBuffer[32];

		for (int i = 0; i < nOp; ++i)
		{
			_OPERAND_ITEM *pOp = &OpArray[i];
			if (OpArray[i].mode == OP_Invalid)
				break;

			if (i > 0)
				pString += TStrCpy(pString, ",");

			if (pOp->mode == OP_Address)
			{
				_DIS_ADDRESS *pAddr = (_DIS_ADDRESS *)&pOp->addr;
				pString += TStrCpy(pString, ",");
				if (pOp->opersize != -1)
				{
					pString += TStrCpy(pString, CInstrSym::m_SizeSym[pOp->opersize]);
					pString += TStrCpy(pString, " PTR ");
				}

				if (pDasmInstr->SegmentPrefix != -1)
				{
					pString += TStrCpy(pString, CInstrSym::m_SegStr[pDasmInstr->SegmentPrefix]);
					pString += TStrCpy(pString, ":");
				}

				pString += TStrCpy(pString, "[");
				if (pAddr->base != -1)
					pString += TStrCpy(pString, CInstrSym::m_RegStr[pAddr->address_size][pAddr->base]);

				if (pAddr->index != -1)
				{
					if (pAddr->base != -1)
						pString += TStrCpy(pString, "+");

					pString += TStrCpy(pString, CInstrSym::m_RegStr[pAddr->address_size][pAddr->index]);
					if (pAddr->scale > 1)
					{
						pString += TStrCpy(pString, "*");
						pString += TStrCpy(pString, CInstrSym::m_ScaleStr[pAddr->scale]);
					}
				}

				if (pAddr->displacement || (pAddr->base == -1 && pAddr->index == -1))
				{
					if (pAddr->index != -1 || pAddr->base != -1)
					{
						if (GetSign(pAddr))
						{
							pString += TStrCpy(pString, "+");
							TSPrintf(szBuffer, CInstrSym::m_SizePtr[GetSize(pAddr->displacement)], pAddr->displacement);
						} else
						{
							pString += TStrCpy(pString, "-");
							TSPrintf(szBuffer, CInstrSym::m_SizePtr[GetSize(pAddr->displacement)], -pAddr->displacement);
						}
					} else
					{
						TSPrintf(szBuffer, CInstrSym::m_SizePtr[GetSize(pAddr->displacement)], pAddr->displacement);
					}

					pString += TStrCpy(pString, szBuffer);
				}
				pString += TStrCpy(pString, "]");
			} else
			{
				switch (pOp->mode)
				{

				case OP_Register:
				case OP_Segment:
				case OP_Immed:
				case OP_Near:
					pString += TStrCpy(pString, pOp->string);
					break;
				case OP_Far:
					TSPrintf(szBuffer, WSTR("%04X"), pOp->farptr.segment);
					pString += TStrCpy(pString, szBuffer);
					pString += TStrCpy(pString, WSTR(":"));
					TSPrintf(szBuffer, CInstrSym::m_SizePtr[pOp->opersize+2], pOp->farptr.offset); //???
					pString += TStrCpy(pString, szBuffer);
					break;
				case OP_Datadup:
					if (i <= 0)
					for (i = 0; i < pOp->datadup.count; ++i)
					{
						if (i > 0)
							pString += TStrCpy(pString, WSTR(","));
						TSPrintf(szBuffer, WSTR("%02X"), pOp->datadup.buffer[i]);
						pString += TStrCpy(pString, szBuffer);
					}
					break;
				case OP_Symbol:
					if (pOp->symbol.boffset)
						pString += TStrCpy(pString, WSTR("OFFSET "));
					pString +=TStrCpy(pString, pOp->symbol.string);
					break;
				case OP_Align:
					if (i <= 0)
					{
						TSPrintf(szBuffer, WSTR("%X"), pOp->align.nBytes);
						pString += TStrCpy(pString, szBuffer);
					}
					break;
				default: break;
				}
			}
		}
		return pString;
	}

	void CInstrSym::OptItemSymToCS(_OPERAND_ITEM *OpArray, WCHAR *pString, _INSTRUCTION_INFORMATION *pDasmInstr)
	{
		pString = OptItemToCS(OpArray, 1, pString, pDasmInstr);
		pString += TStrCpy(pString, " = ");

		if (OpArray->addr.base != -1 || OpArray->addr.index != -1 || pDasmInstr->SegmentPrefix != -1)
		{
			pString += TStrCpy(pString, "[");
			pString += TSPrintf(pString, CInstrSym::m_SizePtr[GetSize(OpArray->addr.line_address)], OpArray->addr.line_address);
			pString += TStrCpy(pString, "] = ");
		}
		//TODO
		unsigned long Size = OpArray->opersize;
		if (Size < 1 || Size > 8)
			Size = 8;
		unsigned long long Value = 0;
		if (ReadMemory(OpArray->addr.line_address, &Value, Size) == Size)
		{
			pString += TSPrintf(pString, CInstrSym::m_SizePtr[GetSize(Value)], Value);
			char szSym[64+64];
			if (AddrToSym(Value, szSym, 64))
			{
				pString += TStrCpy(pString, " = ");
				TStrCpy(pString, szSym);
			}
		} else
		{
			TStrCpy(pString, "????");
		}		
	}

	WCHAR *CInstrSym::InstrToCS(_INSTRUCTION_INFORMATION *pInstrInfo, WCHAR *pString)
	{
		bool bFirstStr = true;
		if (pInstrInfo->Lock != -1)
		{
			TStrCpy(pString, pInstrInfo->LockName);
			TStrCat(pString, " ");
			pString += TStrFillTail(pString, 6, (WCHAR)0x20);
			bFirstStr = false;
		}

		if (pInstrInfo->Repeat != -1)
		{
			TStrCpy(pString, pInstrInfo->RepeatName);
			TStrCat(pString, " ");

			if (bFirstStr)
			{
				pString += TStrFillTail(pString, 6, (WCHAR)0x20);
				bFirstStr = false;
			} else
			{
				pString += TStrLen(pString);
			}
			pString += TStrLen(pString);
		}

		TStrCpy(pString, pInstrInfo->Name);
		TStrCat(pString, " ");

		if (pInstrInfo->Name[0] && bFirstStr)
			pString += TStrFillTail(pString, 6, (WCHAR)0x20);
		else	pString += TStrLen(pString);
		return OptItemToCS(pInstrInfo->op, 3, pString, pInstrInfo);
	}


	CInstrSym::CInstrSym()
		: m_bUseSym(true)
		, m_ImageBase(0)
		, m_ImageHighBase(0)
		, m_UxAlign(6)
		, m_Style(3) //2|1
	{
	#ifdef _X64_
		m_Mode = CODE_MODE_64;
	#else
		m_Mode = CODE_MODE_32;
	#endif
	}

	CInstrSym::~CInstrSym()
	{
	}

	void CInstrSym::SetDasmStyle(unsigned long Style)
	{
		m_Style = Style;
	}
	unsigned long CInstrSym::GetDasmStyle() const
	{
		return m_Style;
	}

	bool CInstrSym::GetJump(unsigned char *CodeBuff, ULONG_PTR Address, ULONG_PTR *target)
	{
		//jear
		if (CodeBuff[0] == 0xEB) //JMP rel8
		{
			if (target)
				*target = Address + (char)CodeBuff[1] + 2;
			return true;
		}

		if (CodeBuff[0] >= 0x70 && CodeBuff[0] <= 0x7F) //Jxx rel8
		{
			if (target)
				*target = Address + (char)CodeBuff[1] + 2;
			return true;
		}

		//short
		if (CodeBuff[0] == 0xF && CodeBuff[1] >= 0x80 && CodeBuff[1] <= 0x8F) //+++ Jxx near rel16/32
		{
			if (target)
				*target = Address + *(unsigned long*)(CodeBuff+2) + 6;
			return true;
		}

		//far
		if (CodeBuff[0] == 0xE8 || CodeBuff[0] == 0xE9) //+++
		{
			if (target)
				*target = Address + *(unsigned long*)(CodeBuff+1) + 5;
			return true;
		}
		return false;
	}

	bool CInstrSym::CheckJump(unsigned char *CodeBuff, unsigned long eflags)
	{
		bool result = false;
		switch (CodeBuff[0]) //TODO EFL
		{
		case 0x70:
			result = (eflags & 0x800) != 0;
			break;
		case 0x71:
			result = (eflags & 0x800) == 0;
			break;
		case 0x72:
			result = (eflags & 1) != 0;
			break;
		case 0x73:
			result = (eflags & 1) == 0;
			break;
		case 0x74:
			result = (eflags & 0x40) != 0;
			break;
		case 0x75:
			result = (eflags & 0x40) == 0;
			break;
		case 0x76:
			result = (eflags & 0x40) && (eflags & 1);
			break;
		case 0x77:
			result = !(eflags & 0x40) && !(eflags & 1);
			break;
		case 0x78:
			result = (eflags & 0x80) != 0;
			break;
		case 0x79:
			result = (eflags & 0x80) == 0;
			break;
		case 0x7A:
			result = (eflags & 4) != 0;
			break;
		case 0x7B:
			result = (eflags & 4) == 0;
			break;
		case 0x7C:
			result = (unsigned short)(eflags & 0x800) >> 11 != (unsigned char)(eflags & 0x80) >> 7;
			break;
		case 0x7D:
			result = (unsigned short)(eflags & 0x800) >> 11 == (unsigned char)(eflags & 0x80) >> 7;
			break;
		case 0x7E:
			result = (unsigned short)(eflags & 0x800) >> 11 != (unsigned char)(eflags & 0x80) >> 7 && (eflags & 0x40);
			break;
		case 0x7F:
			result = (unsigned short)(eflags & 0x800) >> 11 == (unsigned char)(eflags & 0x80) >> 7 && !(eflags & 0x40);
			break;
		default:
			result = 0;
			break;
		}
		return result;
	}

	unsigned long CInstrSym::Dasm(_INSTRUCTION_INFORMATION *pDasmInstr, DIS_CPU *pDisCPU, unsigned long MaxSize)
	{
		unsigned long Size = ReadMemory(pDasmInstr->eip, pDasmInstr->CodeBuff, MAX_INSTR_LEN);
		if (Size == 0)
		{
			pDasmInstr->OpCode = -1;
			strcpy(pDasmInstr->Name, "???");
			pDasmInstr->Lock = -1;
			pDasmInstr->Repeat = -1;
			pDasmInstr->SegmentPrefix = -1;
			pDasmInstr->op[0].mode = OP_Invalid;
			pDasmInstr->op[1].mode = OP_Invalid;
			pDasmInstr->op[2].mode = OP_Invalid;
			return 0;
		}

		MAX_LIMIT(Size, MaxSize);
		unsigned long CodeLen = CIntelDisassembler::Disassembler(pDasmInstr, pDisCPU);
		if (CodeLen == 0 || CodeLen > Size)
		{
			MAX_LIMIT(CodeLen, Size);
			pDasmInstr->OpCode = -1;
			strcpy(pDasmInstr->Name, "DB");
			pDasmInstr->Lock = -1;
			pDasmInstr->Repeat = -1;
			pDasmInstr->SegmentPrefix = -1;
			pDasmInstr->op[1].mode = OP_Invalid;
			pDasmInstr->op[2].mode = OP_Invalid;

			pDasmInstr->op[0].mode = OP_Datadup;
			pDasmInstr->op[0].datadup.count = 1;
			if (pDasmInstr->CodeBuff)
				memcpy(pDasmInstr->op[0].datadup.buffer, pDasmInstr->CodeBuff, 1);
			return 0;
		}

		if (m_Style & 1)
		{
#define X86_INSTR_CALL_IND      0x15FF      // call dword ptr[addr32]
#define X86_INSTR_JMP_IND       0x25FF      // jmp dword ptr[addr32]
#define X86_INSTR_JMP_REL32     0xE9        // jmp rel32
			ULONG_PTR Addr;

			bool Decode = false;

			if ((*(WORD*)pDasmInstr->CodeBuff == 0x15FF ||
			     *(WORD*)pDasmInstr->CodeBuff == 0x25FF) &&
				ReadMemory(*(DWORD*)(pDasmInstr->CodeBuff + 2), &Addr, sizeof(Addr)) == sizeof(Addr)
			   )
			{
				if (AddrToSym(Addr, pDasmInstr->op[0].symbol.string, 64))
				{
					pDasmInstr->op[0].symbol.mode = pDasmInstr->op[0].mode;
					pDasmInstr->op[0].mode = OP_Symbol;
					pDasmInstr->op[0].symbol.boffset = 0;
					pDasmInstr->op[0].symbol.offset = Addr;
					return CodeLen;
				}
			} else
				Decode = true;

			if (Decode || ((m_Style & 2) && !TStrCmp(pDasmInstr->Name, "CALL") &&
				pDasmInstr->op[0].mode == OP_Near))
			{
				unsigned char Buffer[MAX_INSTR_LEN]; 
				_INSTRUCTION_INFORMATION DasmInstr;

				DasmInstr.pasm = nullptr;
				DasmInstr.CodeMode = pDasmInstr->CodeMode;
				DasmInstr.eip = pDasmInstr->op[0].nearptr.label;
				DasmInstr.CodeBuff = Buffer;

				unsigned long OldStyle = m_Style;
				m_Style &= ~2;
				Dasm(&DasmInstr, 0, sizeof(Buffer));
				m_Style = OldStyle;

				if (*(WORD *)DasmInstr.CodeBuff == 0x15FF || 
				    *(WORD *)DasmInstr.CodeBuff == 0x25FF ||
					(*DasmInstr.CodeBuff == 0xE9 && DasmInstr.op[0].mode == OP_Symbol))
				{
					TStrCpy(pDasmInstr->op[0].symbol.string, DasmInstr.op[0].symbol.string);
					pDasmInstr->op[0].symbol.mode = pDasmInstr->op[0].mode;
					pDasmInstr->op[0].mode = OP_Symbol;
					pDasmInstr->op[0].symbol.boffset = 0;
					pDasmInstr->op[0].symbol.offset = DasmInstr.op[0].symbol.offset;
					return CodeLen;
				}
			}

			for (int i = 0; i < 3; ++i)
			{
				switch (pDasmInstr->op[i].mode)
				{
				case OP_Immed:
					Addr = pDasmInstr->op[i].immed.immed_value;
					if (AddrToSym(Addr, pDasmInstr->op[i].symbol.string, 64))
					{
						pDasmInstr->op[i].symbol.mode = pDasmInstr->op[i].mode;
						pDasmInstr->op[i].mode = OP_Symbol;
						pDasmInstr->op[i].symbol.boffset = 1;
						pDasmInstr->op[i].symbol.offset = Addr;
					}
					break;
				case OP_Near:
					Addr = CodeLen + pDasmInstr->op[i].nearptr.offset + pDasmInstr->eip;
					if (AddrToSym(Addr, pDasmInstr->op[i].symbol.string, 64))
					{
						pDasmInstr->op[i].symbol.mode = pDasmInstr->op[i].mode;
						pDasmInstr->op[i].mode = OP_Symbol;
						pDasmInstr->op[i].symbol.boffset = 0;
						pDasmInstr->op[i].symbol.offset = Addr;
					}
					break;
				case OP_Address:
					if (pDasmInstr->SegmentPrefix == -1 &&
					    pDasmInstr->op[i].addr.base == -1 &&
					    pDasmInstr->op[i].addr.index == -1)
					{
						Addr = pDasmInstr->op[i].addr.displacement; //effect_address;
						if (AddrToSym(Addr, pDasmInstr->op[i].symbol.string, 64))
						{
							pDasmInstr->op[i].symbol.mode = pDasmInstr->op[i].mode;
							pDasmInstr->op[i].mode = OP_Symbol;
							pDasmInstr->op[i].symbol.boffset = 0;
							pDasmInstr->op[i].symbol.offset = Addr;
						} else
						if (pDasmInstr->op[i].opersize == sizeof(ULONG_PTR) &&
							ReadMemory(pDasmInstr->op[i].addr.displacement, &Addr, sizeof(Addr)) == sizeof(Addr) && //effect_address
							AddrToSym(Addr, pDasmInstr->op[i].symbol.string, 64))
						{
							pDasmInstr->op[i].symbol.mode = pDasmInstr->op[i].mode;
							pDasmInstr->op[i].mode = OP_Symbol;
							pDasmInstr->op[i].symbol.boffset = 0;
							pDasmInstr->op[i].symbol.offset = Addr;
						}
						
					}
					break;
				default:;
				}
			}
		}
		return CodeLen;
	}
	unsigned long CInstrSym::Asm(char *pAsmString, unsigned char *CodeBuffer, ULONG_PTR Address)
	{
		return CIntelDisassembler::Assembler(pAsmString, CodeBuffer, Address, m_Mode);
	}

	unsigned long CInstrSym::PrevInstrLen(ULONG_PTR Address)
	{
		unsigned char Buffer[32];
		unsigned long Size = MIN(Address - m_ImageBase, 32);
		for (; Size; --Size)
		{
			if (ReadMemory(Address - Size, Buffer, Size) != 0) break;
		}
		if (Size == 0)
			return 1;

		unsigned char CountArry[32];
		int Count = 0;
		for (unsigned long i = 1; i <= Size; ++i)
		{
			unsigned char CodeLenArray[32];
			unsigned long MaxCount = InstrCount((ULONG_PTR)&Buffer[Size - i], i, CodeLenArray); //???
			if (MaxCount)
				CountArry[Count++] = CodeLenArray[MaxCount-1]; //???
		}
		if (Count == 0)
			return 1;

		unsigned char TestArray[32];
		memset(TestArray, 0, sizeof(TestArray));

		ULONG_PTR BaseAddr = Address;
		if (GetSymbolBase(&BaseAddr) != 0 && BaseAddr < Address && Address - BaseAddr < 32)
		{
			unsigned long Size = Address - BaseAddr;
			for (int i = 0; i < Count; ++i)
			{
				if (Size == CountArry[i])
					TestArray[CountArry[i]] = 33;
				else	++TestArray[CountArry[i]];
			}
		} else
		{
			for (int i = 0; i < Count; ++i)
				++TestArray[CountArry[i]];
		}
		unsigned long TestInstLen = 0;
		unsigned long InstrLen = 0;
		for (unsigned long i = 0; i < 32; ++i)
		{
			if (TestInstLen < TestArray[i])
			{
				InstrLen = i;
				TestInstLen = TestArray[i];
			}
		}
		return InstrLen;
	}

	unsigned long CInstrSym::InstrCount(ULONG_PTR Address, unsigned long CodeSize, unsigned char *LenAry)
	{
		unsigned long Off = 0;
		unsigned long Count = 0;
		while (Off < CodeSize)
		{
		#ifdef _X64_
			unsigned long CodeLen = CInstrSym::InstrLen64((unsigned char *)(Off + Address), MAX_INSTR_LEN);
		#else
			unsigned long CodeLen = CInstrSym::InstrLen32((unsigned char *)(Off + Address), MAX_INSTR_LEN);
		#endif
			if (CodeLen == 0)
				return 0;
			Off += CodeLen;
			if (LenAry)
				LenAry[Count] = CodeLen;
			++Count;
		}
		return Off == CodeSize ? Count : 0;
	}

	bool CInstrSym::IsProcessCode(ULONG_PTR Address)
	{
		_INSTRUCTION_INFORMATION InstInfo;
		unsigned char CodeBuff[MAX_INSTR_LEN];

		InstInfo.eip = Address;
		InstInfo.pasm = nullptr;
		InstInfo.CodeBuff = CodeBuff;
		unsigned long CodeLen = Dasm(&InstInfo, 0, sizeof(CodeBuff));
		if (CodeLen < 2)
			return false;
		if (TStrICmp(InstInfo.Name, "CALL") == 0) //eq
			return true;
	//	if (TStrICmp(InstInfo.Name, "PUSHF") == 0) //eq
	//		return true;
	//	if (TStrICmp(InstInfo.Name, "REP") == 0) //eq
	//		return true;
		return InstInfo.Repeat != -1;
	}

	bool CInstrSym::GetAddr(ULONG_PTR Address, ULONG_PTR *Addr)
	{
		_INSTRUCTION_INFORMATION InstInfo;
		unsigned char CodeBuff[MAX_INSTR_LEN];

		InstInfo.eip = Address;
		InstInfo.pasm = nullptr;
		InstInfo.CodeBuff = CodeBuff;
		unsigned long Size = ReadMemory(Address, CodeBuff, sizeof(CodeBuff));
		if (Size == 0)
			return false;

		unsigned long CodeLen = CIntelDisassembler::Disassembler(&InstInfo, 0);
		if (CodeLen == 0 || CodeLen > Size || CodeLen < 2)
			return false;

		if (InstInfo.op[0].mode == OP_Invalid || InstInfo.op[1].mode != OP_Invalid)
			return false;

		if (!CInstrSym::GetJump(CodeBuff, 0, nullptr))
		{
			static const char *Str[] = {"CALL","JMP",0};
			int i = 0;
			for (; Str[i]; ++i)
				if (!TStrICmp(InstInfo.Name, Str[i])) break;
			if (!Str[i])
				return false;
		}

		switch (InstInfo.op[0].mode)
		{
		case OP_Address:
			if (InstInfo.SegmentPrefix == -1
				&& InstInfo.op[0].addr.base == -1
				&& InstInfo.op[0].addr.index == -1
				&& InstInfo.op[0].addr.displacement_size == sizeof(ULONG_PTR))
			{
				*Addr = 0;
				return ReadMemory(InstInfo.op[0].addr.displacement, Addr, sizeof(*Addr)) == sizeof(*Addr); //effect_address
			}
			break;
		case OP_Immed:
			*Addr = InstInfo.op[0].immed.immed_value;
			return true;
		case OP_Near:
			*Addr = InstInfo.op[0].nearptr.label;
			return true;
		}
		return false;
	}
	
	bool CInstrSym::GetReturn(ULONG_PTR Address, ULONG_PTR *Value)
	{
		_INSTRUCTION_INFORMATION InstInfo;
		unsigned char CodeBuff[MAX_INSTR_LEN];

		InstInfo.eip = Address;
		InstInfo.pasm = nullptr;
		InstInfo.CodeBuff = CodeBuff;
		unsigned long Size = ReadMemory(Address, CodeBuff, sizeof(CodeBuff));
		if (Size == 0)
			return false;

		unsigned long CodeLen = CIntelDisassembler::Disassembler(&InstInfo, 0);
		if (CodeLen == 0 || CodeLen > Size || CodeLen < 2)
			return false;

		for (int i = 0; i < 3; ++i)
		{
			switch (InstInfo.op[i].mode)
			{
			case OP_Near:
				*Value = CodeLen + InstInfo.eip + InstInfo.op[i].nearptr.offset;
				return true;
			case OP_Address:
				if (InstInfo.SegmentPrefix == -1
					&& InstInfo.op[i].addr.base == -1
					&& InstInfo.op[i].addr.index == -1
					&& InstInfo.op[i].addr.displacement_size == sizeof(ULONG_PTR))
				{
					*Value = 0;
					return ReadMemory(InstInfo.op[i].addr.displacement, Value, sizeof(*Value)) == sizeof(*Value); //effect_address
				}
				break;
			case OP_Immed:
				*Value = InstInfo.op[i].immed.immed_value;
				return true;
			default:;
			}
		}
		return false;
	}

	int CInstrSym::GetAddrImm(ULONG_PTR Address, ULONG_PTR *Addr, ULONG_PTR *Imm)
	{
		int n = 0;
		_INSTRUCTION_INFORMATION InstInfo;
		unsigned char CodeBuff[MAX_INSTR_LEN];

		InstInfo.eip = Address;
		InstInfo.pasm = nullptr;
		InstInfo.CodeBuff = CodeBuff;
		unsigned long Size = ReadMemory(Address, CodeBuff, sizeof(CodeBuff));

		*Addr = -1;
		*Imm = -1;

		if (Size == 0)
			return n;

		unsigned long CodeLen = CIntelDisassembler::Disassembler(&InstInfo, 0);
		if (CodeLen == 0 || CodeLen > Size || CodeLen < 2)
			return n;

		ULONG_PTR Data;
		for (int i = 0; i < 3; ++i)
		{
			switch (InstInfo.op[i].mode)
			{
			case OP_Address:
				if (InstInfo.SegmentPrefix == -1 &&
					InstInfo.op[i].addr.base == -1 &&
					InstInfo.op[i].addr.index == -1 &&
					InstInfo.op[i].addr.displacement_size == sizeof(ULONG_PTR)) //???
				{
					*Addr = -1;
					if (ReadMemory(InstInfo.op[i].addr.displacement, &Data, sizeof(Data)) == sizeof(Data)) //effect_address
					{
						++n;
						*Addr = InstInfo.op[i].addr.displacement;	//effect_address
					}
				}
				break;
			case OP_Immed:
				if (ReadMemory(InstInfo.op[i].immed.immed_value, &Data, sizeof(Data)) == sizeof(Data))
				{
					++n;
					*Imm = InstInfo.op[i].immed.immed_value;
				}
				break;
			default:;
			}
		}
		return n;
	}

	bool CInstrSym::GetAddrValue(ULONG_PTR Address, ULONG_PTR *Return)
	{
		_INSTRUCTION_INFORMATION InstInfo;
		unsigned char CodeBuff[MAX_INSTR_LEN];

		InstInfo.eip = Address;
		InstInfo.pasm = nullptr;
		InstInfo.CodeBuff = CodeBuff;
		unsigned long Size = ReadMemory(Address, CodeBuff, sizeof(CodeBuff));

		if (Size == 0)
			return false;

		unsigned long CodeLen = CIntelDisassembler::Disassembler(&InstInfo, 0);
		if (CodeLen == 0 || CodeLen > Size || CodeLen < 2)
			return false;

		for (int i = 0; i < 3; ++i)
		{
			if (InstInfo.op[i].mode == OP_Address &&
				InstInfo.SegmentPrefix == -1 &&
				InstInfo.op[i].addr.base == -1 &&
				InstInfo.op[i].addr.index == -1)
			{
				*Return = InstInfo.op[i].addr.displacement;//effect_address;
				return true;
			}
		}

		if (*InstInfo.CodeBuff == 0x68)	// PUSH
		{
			*Return = InstInfo.op[0].immed.immed_value;
			return true;
		}
		return false;
	}

	unsigned long CInstrSym::GetCall(ULONG_PTR Address, char *FunctionName, ULONG_PTR *Return)
	{
		_INSTRUCTION_INFORMATION InstInfo;
		unsigned char CodeBuff[MAX_INSTR_LEN];
		unsigned long CodeLen;

		static const unsigned long CallCodeLen[] = {6, 5, 4, 3, 2}; //???

		for (int i = 0; i < 5; ++i) //lenof(CallCodeLen)
		{
			InstInfo.eip = Address - CallCodeLen[i];
			InstInfo.pasm = nullptr;
			InstInfo.CodeBuff = CodeBuff;
			CodeLen = Dasm(&InstInfo, 0, sizeof(CodeBuff));
			if (CodeLen == CallCodeLen[i] && !TStrICmp(InstInfo.Name, "CALL"))
			{
				switch (InstInfo.op[0].mode)
				{
				case OP_Address:
					if (FunctionName)
						*FunctionName = 0;
					if (Return)
						*Return = 0;
					if (InstInfo.SegmentPrefix == -1 &&
					    InstInfo.op[0].addr.base == -1 &&
					    InstInfo.op[0].addr.index == -1 &&
					    Return)
						ReadMemory(InstInfo.op[0].addr.displacement, Return, sizeof(*Return)); //effect_address
					break;
				case OP_Near:
					if (FunctionName)
						*FunctionName = 0;
					if (Return)
						*Return = InstInfo.op[0].nearptr.label;
					break;
				case OP_Symbol:
					if (FunctionName)
						TStrCpy(FunctionName, InstInfo.op[0].symbol.string);
					if (Return)
						*Return = InstInfo.op[0].symbol.offset;
					break;
				default:
					if (FunctionName)
						*FunctionName = 0;
					if (Return)
						*Return = 0;
					break;
				}
				return CodeLen;
			}
		}
		return 0;
	}

	bool CInstrSym::GetFunctionName(ULONG_PTR Address, char *FunctionName)
	{
		_INSTRUCTION_INFORMATION InstInfo;
		unsigned char CodeBuff[MAX_INSTR_LEN];

		InstInfo.eip = Address;
		InstInfo.pasm = nullptr;
		InstInfo.CodeBuff = CodeBuff;
		unsigned long CodeLen = Dasm(&InstInfo, 0, sizeof(CodeBuff));
		if (CodeLen < 2)
			return false;
		if (TStrICmp(InstInfo.Name, "CALL") == 0)
		{
			if (FunctionName)
			{
				FunctionName[0] = 0;
				if (InstInfo.op[0].mode == OP_Symbol)
					TStrCpy(FunctionName, InstInfo.op[0].symbol.string);
			}
			return true;
		}
		return false;
	}

	unsigned long CInstrSym::InstrLen(ULONG_PTR Address, unsigned long MaxSize)
	{
		unsigned char Buffer[MAX_INSTR_LEN];
		unsigned long Size = ReadMemory(Address, Buffer, MaxSize);
		if (!Size)
			return 0;
	#ifdef _X64_
		unsigned long CodeLen = CInstrSym::InstrLen64(Buffer, Size);
	#else
		unsigned long CodeLen = CInstrSym::InstrLen32(Buffer, Size);
	#endif
		MAX_LIMIT(CodeLen, Size);
		return CodeLen;
	}

	unsigned long CInstrSym::InstrLen64(unsigned char *inbuff, unsigned long MaxSize)
	{
		unsigned long CodeLen = Disassembly(0, inbuff, 0, CODE_MODE_64, 0, 8);
		return CodeLen <= MaxSize ? CodeLen : 0;
	}

	unsigned long CInstrSym::InstrLen32(unsigned char *inbuff, unsigned long MaxSize)
	{
		unsigned long CodeLen = Disassembly(0, inbuff, 0, CODE_MODE_32, 0, 8);
		return CodeLen <= MaxSize ? CodeLen : 0;
	}

	unsigned long CInstrSym::InstrLen16(unsigned char *inbuff, unsigned long MaxSize)
	{
		unsigned long CodeLen = Disassembly(0, inbuff, 0, CODE_MODE_16, 0, 8);
		return CodeLen <= MaxSize ? CodeLen : 0;
	}

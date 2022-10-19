
#include <stdio.h>
#include "inteldis.hpp"
#include "../Code/tstrmem.hpp"

struct _tagLOCKREPXX
{
	const char *Name;
	unsigned char Code;
} LockRepxx[] = {
	{"LOCK ", 0xF0},
	{"REP ",0xF3},
	{"REPZ ",0xF3},
	{"REPE ",0xF3},
	{"REPNE ",0xF2},
	{"REPNZ ",0xF2},
};

struct _OPDATAWIDE
{
	int Size;
	const char *Xptr;
} OpDataWide[] = {{1,"SHORT PTR"},
		{2,"NEAR PTR"},
		{4,"FAR PTR"},

		{1,"BYTE PTR"},
		{2,"WORD PTR"},
		{4,"DWORD PTR"},

		{6,"FWORD PTR"},
		{8,"QWORD PTR"},
		{10,"TBYTE PTR"},
		{16,"OWORD PTR"}};

const char *RegAddr16[] = {
	"[BX+SI]","[SI+BX]",
	"[BX][SI]","[SI][BX]",
	"[BX+DI]","[DI+BX]",
	"[BX][DI]","[DI][BX]",
	"[BP+SI]","[SI+BP]",
	"[BP][SI]","[SI][BP]",
	"[BP+DI]","[DI+BP]",
	"[BP][DI]","[DI][BP]",
	"[SI]","[SI]",
	"[SI]","[SI]",
	"[DI]","[DI]",
	"[DI]","[DI]",
	"[BP]","[BP]",
	"[BP]","[BP]",
	"[BX]","[BX]",
	"[BX]","[BX]"};

const char *xxxxptr[] = {0, "BYTE", "WORD", 0, "DWORD", 0, "FWORD", 0, "QWORD", 0, "TBYTE", 0,0,0,0,0, "OWORD"};

const char SegmentValue[] =
	{0x26,0x2E,0x36,0x3E,0x64,0x65,0};
const char *SegReg[] =
	{"ES","CS","SS","DS","FS","GS","??","??"};

const char *RegByte[] = {"AL","CL","DL","BL",
			"AH","CH","DH","BH",};

const char *RegSTx[] = {"ST(0)","ST(1)","ST(2)","ST(3)","ST(4)","ST(5)","ST(6)","ST(7)","ST"};

const char *RegByteRex[GENE_IDX_MAX] = 
{"AL","CL","DL","BL","SPL","BPL","SIL","DIL","R8B","R9B","R10B","R11B","R12B","R13B","R14B","R15B"};
const char *RegWord[GENE_IDX_MAX] = 
{"AX","CX","DX","BX","SP","BP","SI","DI","R8W","R9W","R10W","R11W","R12W","R13W","R14W","R15W"};
const char *RegQWord[GENE_IDX_MAX] = 
{"RAX","RCX","RDX","RBX","RSP","RBP","RSI","RDI","R8","R9","R10","R11","R12","R13","R14","R15"};
const char *RegDWord[GENE_IDX_MAX] = 
{"EAX","ECX","EDX","EBX","ESP","EBP","ESI","EDI","R8D","R9D","R10D","R11D","R12D","R13D","R14D","R15D"};

//80
const char *RegMMx[] = {"MM0","MM1","MM2","MM3","MM4","MM5","MM6","MM7"}; //ST()/MM()

//LIMIT 8
const char *RegCRx[] = {"CR0","CR1","CR2","CR3","CR4","CR5","CR6","CR7","CR8","CR9","CR10","CR11","CR12","CR13","CR14","CR15"};
const char *RegTRx[] = {"TR0","TR1","TR2","TR3","TR4","TR5","TR6","TR7","TR8","TR9","TR10","TR11","TR12","TR13","TR14","TR15"};
const char *RegDRx[] = {"DR0","DR1","DR2","DR3","DR4","DR5","DR6","DR7","DR8","DR9","DR10","DR11","DR12","DR13","DR14","DR15"};
//128 XMM31
const char *RegXMMx[] = {"XMM0","XMM1","XMM2","XMM3","XMM4","XMM5","XMM6","XMM7","XMM8","XMM9","XMM10","XMM11","XMM12","XMM13","XMM14","XMM15"};

const char *FlagS[] = {"NV","UP","DI","PL","NZ","NA","PO","NC","OV","DN","EI","NG","ZR","AC","PE","CY"};
const char *FlagICE[] = {"o","d","i","s","z","a","p","c",
		   "O","D","I","S","Z","A","P","C"};

#if 0
#ifndef _INTELDIS_HPP_
#define _INTELDIS_HPP_


enum _IMMEDIATE___
{
  Immediate_ = 0,
  Immediate_1 = 1,
  Immediate_CL = 2,
  Immediate_Ib = 3,
  Immediate_sIb = 4,
  Immediate_zIb = 5,
  Immediate_Iv = 6,
  Immediate_Iw = 7,
  Immediate_IvIw = 8,
  Immediate_IwIb = 9,
  Immediate_O = 10,
  Immediate_BrOff8 = 11,
  Immediate_BrOff16 = 12,
  Immediate_BrOff32 = 13,
  Immediate_Jb = 14,
  Immediate_Jv = 15,
  PREFIX_ES = 16,
  PREFIX_CS = 17,
  PREFIX_SS = 18,
  PREFIX_DS = 19,
  PREFIX_FS = 20,
  PREFIX_GS = 21,
  PREFIX_66 = 22,
  PREFIX_67 = 23,
  PREFIX_F0 = 24,
  PREFIX_F2 = 25,
  PREFIX_F3 = 26,
  STRING_Move = 27,
  STRING_Comp = 28,
  Extended_2byte = 29,
  Extended_Group = 30,
  Extended_Float = 31,
  Extended_Group1 = 32,
  OPCODE_UD = 33,
};


// asm

struct _OPER_ITEM
{
	unsigned short mode;
	unsigned char rwflag;

	int opersize;

	union
	{
		struct
		{
			int seg_index;
			int reg_size;
			int base_reg_index;
			int off_reg_index;
			int off_reg_scale;
			unsigned long off_value;
		} addr;

		struct
		{
			int reg_index;
		} reg;

		struct
		{
			int sreg_index;
		} sreg;

		struct
		{
			unsigned long immed_value;
		} immed;

		struct
		{
			unsigned long offset;
		} nearptr;

		struct
		{
			unsigned long segment;
			unsigned long offset;
		} farptr;

		char string[128];
	};
};


// asm

struct _DIS_MODR_M
{
	unsigned int r_m : 3;
	unsigned int reg : 3;
	unsigned int mod : 2;
	unsigned int base : 3;
	unsigned int index : 3;
	unsigned int scale : 2;
	unsigned int disp0 : 8;
	unsigned int disp1 : 8;
	unsigned int disp2 : 8;
	unsigned int disp3 : 8;
};

// asm

struct ASSEMBLY_ERROR
{
	unsigned int name : 1;
	unsigned int Type : 1;
	unsigned int P1 : 1;
	unsigned int P2 : 1;
	unsigned int P3 : 1;
	unsigned int P4 : 1;
	unsigned int PX : 1;
	unsigned int Size : 1;
};

// asm
struct _ASSEMBLY_INFOMATION
{
	int NameSize;
	unsigned char Lock;
	unsigned char Repeat;

	int OperandSize;
	int AddressSize;
	unsigned char SegmentPrefix;

	int OpcodeType;
	char *Name;
	_OPER_ITEM op[3];
	unsigned long eip;
	_DIS_MODR_M *pmodr_m;
	int InstLength;
	int PrefixLength;
	int CodeMode;
	int ByteCount;
	unsigned char *CodeBuff;
	ASSEMBLY_ERROR error;
};


#endif
#endif
/////

struct _DISP_INSTRUCTION_T
{
	unsigned int attr;
	unsigned int b1;
	unsigned int sse_prefix; //++
	unsigned int rep_used;
	unsigned int lock;
	unsigned int rex_prefix; //++

	//unsigned char/int
	unsigned char modrm;
	unsigned int mod;
	unsigned int nnn;
	unsigned int rm;

	unsigned short displ16u;
	unsigned long displ32u;

	unsigned int seg;

	//unsigned char/int
	unsigned char sib;
	unsigned int scale;
	unsigned int index;
	unsigned int base;

	unsigned int __unused;//++ ALIGN

	unsigned long long addr_displacement;
	unsigned long long rm_addr;
	unsigned long long Iq; //++
	unsigned long Id; //++
	unsigned short Iw;
	unsigned char Ib;
	unsigned char Ib2;
	unsigned short Iw2;
	unsigned short UNUSEDW; //???
	unsigned int ilen;
	unsigned int modrm_length;
	unsigned int displacement_size;
	unsigned int os_32;
	unsigned int as_32;
	unsigned int flags_in;
	unsigned int flags_out;
	unsigned int destination;
	unsigned int source;
	unsigned int other;

	unsigned int opcount; //++
	unsigned int prefixes; //++
	unsigned int regrm; //++
	unsigned int sib1; //++
	unsigned int osx_32; //++
	unsigned int x64; //++
};

struct _DIS_OPCODE_T
{
	unsigned int Attr;
	unsigned int OpcodeType;
	unsigned int Opdata1;
	unsigned int Opdata2;
	unsigned int Opdata3;
	const char *name;
//asm
	unsigned int CodePrefix; //unsigned char CodePrefix[4];
	unsigned int AttrCodePrefixLen;
	unsigned int Byte;
};

#define DECODE_MODRM(modrm_byte, mod, opcode, rm) { \
  rm     =  modrm_byte & 0x07;       \
  opcode = (modrm_byte >> 3) & 0x07; \
  mod    = (modrm_byte >> 6) & 0x03; \
}

#define DECODE_SIB(sib_byte, scale, index, base) { \
  base  =  sib_byte & 0x07;        \
  index = (sib_byte >> 3) & 0x07;  \
  scale = (sib_byte >> 6) & 0x03;  \
}

int GetEffectAddress32Dis(DIS_CPU *pDisCPU, unsigned char *pbuff, _DISP_INSTRUCTION_T *disp_i)
{
	//int mod;
	//int reg;
	unsigned long long base_register_val;
	unsigned long base_index;
	//int index;
	//int r_m;
	//int base;
	//int scale;
	int length;

	if (disp_i->rex_prefix & 8)
		disp_i->os_32 = CODE_MODE_64;

	disp_i->modrm = *pbuff++;
	DECODE_MODRM(disp_i->modrm, disp_i->mod, disp_i->nnn, disp_i->rm)
	//disp_i->rm = *(unsigned int*)pbuff & 7;
	//disp_i->nnn = (*(unsigned int*)pbuff >> 3) & 7;// | (2 * (disp_i->rex_prefix & 4));
	//disp_i->mod = (*(unsigned int*)pbuff >> 6) & 3;

	disp_i->nnn |= (2 * (disp_i->rex_prefix & 4)); //rex_r

	disp_i->regrm = 1;
	disp_i->sib1 = -1;

	if (disp_i->rm == 4)
	{
		disp_i->sib = *pbuff++;
		unsigned int base;
		unsigned int index;
		DECODE_SIB(disp_i->sib, disp_i->scale, index, base);
		//base = (*(unsigned int*)pbuff >> 8) & 7;
		//index = (*(unsigned int*)pbuff >> (8+3)) & 7;
		//disp_i->scale = (unsigned char)(*(unsigned short*)pbuff >> 8) >> 6;
		//pbuff += 2;

		if (disp_i->osx_32 == CODE_MODE_64)
		{
			index |= 4 * (disp_i->rex_prefix & 2); //rex_x
			base |= 8 * (disp_i->rex_prefix & 1); //rex_b
			disp_i->rm |= 8 * (disp_i->rex_prefix & 1); //rex_b
		}

		disp_i->sib1 = 1;

		disp_i->base = base;
		if ((disp_i->base & 7) == EBP_IDX && disp_i->mod == 0)
			disp_i->base = -1;

		disp_i->index = index;

		if (disp_i->base == -1)
			base_register_val = 0;
		else	base_register_val = pDisCPU->gen_reg[base].rerx;

		if (disp_i->index == ESP_IDX)
			disp_i->index = -1;
		else	base_register_val += (pDisCPU->gen_reg[disp_i->index].rerx << disp_i->scale);

		if (disp_i->base != ESP_IDX && disp_i->base != EBP_IDX)
			disp_i->seg = DS_IDX;
		else	disp_i->seg = SS_IDX;

		switch (disp_i->mod)
		{
		case 0: //effective address
			if ((base & 7) == 5) /* 32 bits displacement */ //EBP_IDX)
			{
				disp_i->addr_displacement = *(signed int*)pbuff;
				disp_i->displacement_size = 4;
				length = 6;
				if (index == ESP_IDX)
					base_register_val = 0;
				else	base_register_val = (pDisCPU->gen_reg[index].rerx << disp_i->scale);
				disp_i->seg = DS_IDX;
			} else
			//register address
			{
				disp_i->addr_displacement = 0;
				disp_i->displacement_size = 0;
				length = 2;
			}
			break;
		case 1: //effective address + 8 bits displacement
			disp_i->addr_displacement = *(signed char*)pbuff;
			disp_i->displacement_size = 1;
			length = 3;
			break;
		case 2: //effective address + 32 bits displacement
			disp_i->addr_displacement = *(signed int*)pbuff;
			disp_i->displacement_size = 4;
			length = 6;
			break;
		case 3: //registers
			disp_i->displacement_size = 0;
			disp_i->sib1 = -1;
			length = 1;
			break;
		default:
			//ASSERT disp_i->mod
			break;
		}
	} else
	{
		//++pbuff;

		base_index = disp_i->rm | (8 * (disp_i->rex_prefix & 1)); //rex_b
		disp_i->base = base_index;
		disp_i->index = -1;
		disp_i->scale = 0;
		disp_i->rm = base_index;

		base_register_val = pDisCPU->gen_reg[disp_i->base].rerx;

		if (disp_i->base != ESP_IDX && disp_i->base != EBP_IDX)
			disp_i->seg = DS_IDX;
		else	disp_i->seg = SS_IDX;

		if (disp_i->mod == 0) //effective address
		{
			if ((disp_i->base & 7) == 5) /* 32 bits displacement */ //EBP_IDX)
			{
				disp_i->addr_displacement = *(signed int*)pbuff;
				disp_i->displacement_size = 4;
				if (disp_i->osx_32 == CODE_MODE_64)
				{
					disp_i->seg = CS_IDX;
					disp_i->x64 = 1;
				} else
				{
					disp_i->seg = DS_IDX;
				}
				base_register_val = 0;
				disp_i->base = -1;
			} else
			//register address
			{
				disp_i->addr_displacement = 0;
				disp_i->displacement_size = 0;
			}
		} else
		if (disp_i->mod == 1) //effective address + 8 bits displacement
		{
			disp_i->addr_displacement = *(signed char*)pbuff;
			disp_i->displacement_size = 1;
		} else
		if (disp_i->mod == 2) //effective address + 32 bits displacement
		{
			disp_i->addr_displacement = *(signed int*)pbuff; //TODO will fix on x32 incorrect off
			disp_i->displacement_size = 4;
		} else
		//registers
		{
			disp_i->displacement_size = 0;
		}

		length = disp_i->displacement_size + 1;
	}
	disp_i->rm_addr = base_register_val + disp_i->addr_displacement;
	return length;
}

int GetEffectAddress16Dis(DIS_CPU *pDisCPU, unsigned char *pbuff, _DISP_INSTRUCTION_T *disp_i)
{
	//int mod;
	//int reg;
	unsigned long base_register_val;
	//int r_m;
	int length;

	disp_i->modrm = *pbuff++;
	DECODE_MODRM(disp_i->modrm, disp_i->mod, disp_i->nnn, disp_i->rm);
	//disp_i->rm = *(unsigned int*)pbuff & 7;
	//disp_i->nnn = (*(unsigned int*)pbuff >> 3) & 7;
	//disp_i->mod = (*(unsigned int*)pbuff >> 6) & 3;
	//++pbuff;

	disp_i->regrm = 1;
	disp_i->sib1 = -1;


	disp_i->as_32 = CODE_MODE_16;
	disp_i->scale = 0;

	switch (disp_i->rm)
	{
	case 0:
		base_register_val = pDisCPU->gen_reg[EBX_IDX].word.u_word.rx + pDisCPU->gen_reg[ESI_IDX].word.u_word.rx;
		disp_i->seg = DS_IDX;
		disp_i->base = EBX_IDX;
		disp_i->index = ESI_IDX;
		break;
	case 1:
		base_register_val = pDisCPU->gen_reg[EBX_IDX].word.u_word.rx + pDisCPU->gen_reg[EDI_IDX].word.u_word.rx;
		disp_i->seg = DS_IDX;
		disp_i->base = EBX_IDX;
		disp_i->index = EDI_IDX;
		break;
	case 2:
		base_register_val = pDisCPU->gen_reg[EBP_IDX].word.u_word.rx + pDisCPU->gen_reg[ESI_IDX].word.u_word.rx;
		disp_i->seg = SS_IDX;
		disp_i->base = EBP_IDX;
		disp_i->index = ESI_IDX;
		break;
	case 3:
		base_register_val = pDisCPU->gen_reg[EBP_IDX].word.u_word.rx + pDisCPU->gen_reg[EDI_IDX].word.u_word.rx;
		disp_i->seg = SS_IDX;
		disp_i->base = EBP_IDX;
		disp_i->index = EDI_IDX;
		break;
	case 4:
		base_register_val = pDisCPU->gen_reg[ESI_IDX].word.u_word.rx;
		disp_i->seg = DS_IDX;
		disp_i->base = ESI_IDX;
		disp_i->index = -1;
		break;
	case 5:
		base_register_val = pDisCPU->gen_reg[EDI_IDX].word.u_word.rx;
		disp_i->seg = DS_IDX;
		disp_i->base = EDI_IDX;
		disp_i->index = -1;
		break;
	case 6:
		base_register_val = pDisCPU->gen_reg[EBP_IDX].word.u_word.rx;
		disp_i->seg = SS_IDX;
		disp_i->base = EBP_IDX;
		disp_i->index = -1;
		break;
	case 7:
		base_register_val = pDisCPU->gen_reg[EBX_IDX].word.u_word.rx;
		disp_i->seg = DS_IDX;
		disp_i->base = EBX_IDX;
		disp_i->index = -1;
		break;
	default:
		//ASSERT disp_i->rm
		break;
	}

	if (disp_i->mod == 0) //effective address
	{
		if (disp_i->rm == 6) //16 bits displacement
		{
			disp_i->addr_displacement = *(unsigned short*)pbuff;
			disp_i->displacement_size = 2;
			base_register_val = 0;
			disp_i->seg = DS_IDX;
			disp_i->base = -1;
			disp_i->index = -1;
		} else
		{
			disp_i->addr_displacement = 0;
			disp_i->displacement_size = 0;
		}
	} else
	if (disp_i->mod == 1) //effective address + 8 bits displacement
	{
		disp_i->addr_displacement = *(signed char*)pbuff;
		disp_i->displacement_size = 1;
	} else
	if (disp_i->mod == 2) //effective address + 16 bits displacement
	{
		disp_i->addr_displacement = *(unsigned short*)pbuff; //??? signed short
		disp_i->displacement_size = 2;
	} else
	//registers
	{
		disp_i->displacement_size = 0;
	}
	length = disp_i->displacement_size + 1;
	disp_i->rm_addr = (unsigned short)(base_register_val + disp_i->addr_displacement);
	return length;
}

int GetImmediateData(unsigned int imm_mode, unsigned char *iptr, _DISP_INSTRUCTION_T *insruction)
{
	return 0;
}

void DispTranDis(_DIS_ADDRESS *dis_a, _DISP_INSTRUCTION_T *disp_i, unsigned long long eip)
{
	dis_a->mod = disp_i->mod;
	dis_a->reg = disp_i->nnn;
	dis_a->r_m = disp_i->rm;
	dis_a->base = disp_i->base;
	dis_a->index = disp_i->index;
	dis_a->scale = disp_i->scale;
	dis_a->segment = disp_i->seg;
	dis_a->address_size = disp_i->as_32;
	dis_a->displacement_size = disp_i->displacement_size;

	if (disp_i->x64)
	{
		disp_i->addr_displacement += eip + disp_i->ilen;
		dis_a->displacement_size = 8;
	}

	dis_a->displacement = disp_i->addr_displacement;
	dis_a->effect_address = disp_i->rm_addr;
}

bool ProcessName_dis(_INSTRUCTION_INFORMATION *pInstInfo, _DIS_OPCODE_T *pIntelOpcode, int OperandSize, int AddressSize)
{
	if (pInstInfo->Lock == 0xF0)
	{
		strcpy(pInstInfo->LockName, "LOCK");
	} else
	{
		pInstInfo->LockName[0] = 0;
	}
	
	if (pInstInfo->Repeat == 0xF2)
	{
		strcpy(pInstInfo->RepeatName, "REPNE");
	} else
	if (pInstInfo->Repeat == 0xF3)
	{
		strcpy(pInstInfo->RepeatName, "REP");
	} else
	{
		pInstInfo->RepeatName[0] = 0;
	}

	if (pIntelOpcode->OpcodeType == C_NULL)
		return false;

	strcpy(pInstInfo->Name, (const char *)pIntelOpcode->name);

	if ((pIntelOpcode->Opdata1 >= D__Xb && pIntelOpcode->Opdata1 <= D_wXb) ||
	    (pIntelOpcode->Opdata1 >= D__Yb && pIntelOpcode->Opdata1 <= D_wYb) ||

	    (pIntelOpcode->Opdata2 >= D__Xb && pIntelOpcode->Opdata2 <= D_wXb) ||
	    (pIntelOpcode->Opdata2 >= D__Yb && pIntelOpcode->Opdata2 <= D_wYb))
	{
		sprintf(pInstInfo->Name, "%sB", (const char *)pIntelOpcode->name);
	}

	if ((pIntelOpcode->Opdata1 >= D__Xv && pIntelOpcode->Opdata1 <= D_wXv) ||
	    (pIntelOpcode->Opdata1 >= D__Yv && pIntelOpcode->Opdata1 <= D_wYv) ||

	    (pIntelOpcode->Opdata2 >= D__Xv && pIntelOpcode->Opdata2 <= D_wXv) ||
	    (pIntelOpcode->Opdata2 >= D__Yv && pIntelOpcode->Opdata2 <= D_wYv))
	{
		switch (OperandSize)
		{
		case CODE_MODE_16: sprintf(pInstInfo->Name, "%sW", (const char *)pIntelOpcode->name); break;
		case CODE_MODE_32: sprintf(pInstInfo->Name, "%sD", (const char *)pIntelOpcode->name); break;
		case CODE_MODE_64: sprintf(pInstInfo->Name, "%sQ", (const char *)pIntelOpcode->name); break;
		}
	}

	if (OperandSize == CODE_MODE_32)
	{
		switch (pIntelOpcode->OpcodeType)
		{
		case C_MOVD: strcpy(pInstInfo->Name, "MOVD"); break;
		case C_PEXTRD: strcpy(pInstInfo->Name, "pextrd"); break;
		case C_PINSRD: strcpy(pInstInfo->Name, "pinsrd"); break;
		case C_IRET: strcpy(pInstInfo->Name, "IRETD"); break;
		case C_CWD: strcpy(pInstInfo->Name, "CDQ"); break;
		case C_PUSHF: strcpy(pInstInfo->Name, "PUSHFD"); break;
		case C_POPF: strcpy(pInstInfo->Name, "POPFD"); break;
		case C_CBW: strcpy(pInstInfo->Name, "CWDE"); break;
		case C_JCXZ: strcpy(pInstInfo->Name, "JECXZ"); break;
		case C_PUSHA: strcpy(pInstInfo->Name, "PUSHAD"); break;
		case C_POPA: strcpy(pInstInfo->Name, "POPAD"); break;
		}
	} else
	if (OperandSize == CODE_MODE_64)
	{
		switch (pIntelOpcode->OpcodeType)
		{
		case C_MOVD: strcpy(pInstInfo->Name, "MOVQ"); break;
		case C_PEXTRD: strcpy(pInstInfo->Name, "pextrq"); break;
		case C_PINSRD: strcpy(pInstInfo->Name, "pinsrq"); break;
		case C_IRET: strcpy(pInstInfo->Name, "IRETQ"); break;
		case C_CWD: strcpy(pInstInfo->Name, "CQO"); break;
		case C_PUSHF: strcpy(pInstInfo->Name, "PUSHFQ"); break;
		case C_POPF: strcpy(pInstInfo->Name, "POPFQ"); break;
		case C_CBW: strcpy(pInstInfo->Name, "CDQE"); break;
		case C_JCXZ: strcpy(pInstInfo->Name, "JRCXZ"); break;
		case C_PUSHA: strcpy(pInstInfo->Name, ""); break;
		case C_POPA: strcpy(pInstInfo->Name, ""); break;
		case C_CMPXCHG8B: if (pInstInfo->RexPrefix & 8) strcpy(pInstInfo->Name, "CMPXCHG16B"); break;
		}
	}	
	//TStrUpr(pInstInfo->Name); //TODO
	return true;
}

#define OP_READ 1
#define OP_WRITE 2

#define OP_SIZE(x,n)	if (!(x)->opersize) { (x)->opersize = (n); }

#define OP_SIZE_(x,n,a)	if (!(x)->opersize) { (x)->opersize = (n); (x)->rwflag = (a); }
#define OP_SIZE_R(x,n)	if (!(x)->opersize) { (x)->opersize = (n); (x)->rwflag = OP_READ; }
#define OP_SIZE_W(x,n)	if (!(x)->opersize) { (x)->opersize = (n); (x)->rwflag = OP_WRITE; }
#define OP_SIZE_RW(x,n)	if (!(x)->opersize) { (x)->opersize = (n); (x)->rwflag = OP_READ|OP_WRITE; }

int ProcessOpdata_dis(int opdata, _OPERAND_ITEM *op, unsigned long long eip, _DISP_INSTRUCTION_T *disp_i, DIS_CPU *dis_cpu)
{
	int OperandSize = disp_i->os_32;
	int AddressSize = disp_i->as_32;

	int SIZE_v;
	int SIZE_p;
	int SIZE_z;
	int SIZE_y;

	if (OperandSize == CODE_MODE_16)
	{
		SIZE_v = 2;
		SIZE_p = 4;
		SIZE_z = 2;
		SIZE_y = 4;
	} else
	if (OperandSize == CODE_MODE_32)
	{
		SIZE_v = 4;
		SIZE_p = 6;
		SIZE_z = 4;
		SIZE_y = 4;
	} else
	if (OperandSize == CODE_MODE_64)
	{
		SIZE_v = 8;
		SIZE_p = 10;
		SIZE_z = 4;
		SIZE_y = 8;
	}

	int opimmedlength = 0;
	op->mode = OP_Invalid;
	op->opersize = 0;

	switch (opdata)
	{
	case D__NONE:
	case D__Fv:
	case D__NTA:
		return opimmedlength;

	case D__Cd:
		if (disp_i->osx_32 == CODE_MODE_64)
			op->opersize = 8;
		else	op->opersize = 4;
		op->mode = OP_Register;
		op->reg.reg_index = disp_i->nnn;
		strcpy(op->string, RegCRx[op->reg.reg_index]);
		return opimmedlength;

	case D__Dd:
		if (disp_i->osx_32 == CODE_MODE_64)
			op->opersize = 8;
		else	op->opersize = 4;
		op->mode = OP_Register;
		op->reg.reg_index = disp_i->nnn;
		strcpy(op->string, RegDRx[op->reg.reg_index]);
		return opimmedlength;

	case D__Td:
		if (disp_i->osx_32 == CODE_MODE_64)
			op->opersize = 8;
		else	op->opersize = 4;
		op->mode = OP_Register;
		op->reg.reg_index = disp_i->nnn;
		strcpy(op->string, RegTRx[op->reg.reg_index]);
		return opimmedlength;

	case D__Gb: OP_SIZE_RW(op, 1);
	case D_rGb: OP_SIZE_R(op,1);
	case D_wGb: OP_SIZE_W(op,1);

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->nnn;
		if (disp_i->osx_32 == CODE_MODE_64 && disp_i->rex_prefix)
			strcpy(op->string, RegByteRex[op->reg.reg_index]);
		else	strcpy(op->string, RegByte[op->reg.reg_index]);
		return opimmedlength;

	case D__Gw: OP_SIZE_RW(op,2);
	case D_rGw: OP_SIZE_R(op, 2);
	case D_wGw: OP_SIZE_W(op, 2);

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->nnn;
		strcpy(op->string, RegWord[op->reg.reg_index]);
		return opimmedlength;

	case D__Gv: OP_SIZE_RW(op, SIZE_v);
	case D_rGv: OP_SIZE_R(op, SIZE_v);
	case D_wGv: OP_SIZE_W(op, SIZE_v);
	case D_dGv: OP_SIZE_RW(op, SIZE_v);

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->nnn;
		switch (op->opersize)
		{
		case 2:
			strcpy(op->string, RegWord[op->reg.reg_index]);
			break;
		case 4:
			strcpy(op->string, RegDWord[op->reg.reg_index]);
			break;
		case 8:
			strcpy(op->string, RegQWord[op->reg.reg_index]);
			break;
		}
		return opimmedlength;

	case D__Gd: OP_SIZE_RW(op, 4);
	case D_rGd: OP_SIZE_R(op, 4);
	case D_wGd: OP_SIZE_W(op, 4);
	case D_dGd: OP_SIZE_RW(op, 4);

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->nnn;
		strcpy(op->string, RegDWord[op->reg.reg_index]);
		return opimmedlength;

	case D__Gy: OP_SIZE_RW(op, SIZE_y);
	case D_rGy: OP_SIZE_R(op, SIZE_y);
	case D_wGy: OP_SIZE_W(op, SIZE_y);
	case D_dGy: OP_SIZE_RW(op, SIZE_y);

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->nnn;
		if (op->opersize == 4)
			strcpy(op->string, RegDWord[op->reg.reg_index]);
		else	strcpy(op->string, RegQWord[op->reg.reg_index]);
		return opimmedlength;

	case D__Ib:
		op->mode = OP_Immed;
		op->opersize = 1;
		opimmedlength = 1;
		op->immed.immed_value = disp_i->Ib;
		if ((int)disp_i->Ib < 0xA0)
			sprintf(op->string, "%X", (unsigned int)op->immed.immed_value);
		else	sprintf(op->string, "0%X", (unsigned int)op->immed.immed_value);
		return opimmedlength;

	case D_zIb:
		op->mode = OP_Immed;
		op->opersize = 1;
		opimmedlength = 1;
		op->immed.immed_value = disp_i->Ib2;
		if ((int)disp_i->Ib2 < 0xA0)
			sprintf(op->string, "%X", (unsigned int)op->immed.immed_value);
		else	sprintf(op->string, "0%X", (unsigned int)op->immed.immed_value);
		return opimmedlength;

	case D_sIb:
		op->mode = OP_Immed;
		op->opersize = 1;
		opimmedlength = 1;
		op->immed.immed_value = disp_i->Ib;
		if (op->immed.immed_value < 0x80) //???
			sprintf(op->string, "%X", (unsigned char)(op->immed.immed_value));
		else	sprintf(op->string, "-%X", (unsigned char)-(signed char)(op->immed.immed_value));
		return opimmedlength;

	case D__Iw:
		op->mode = OP_Immed;
		op->opersize = 2;
		opimmedlength = 2;
		op->immed.immed_value = disp_i->Iw;
		sprintf(op->string, "%X", (unsigned short)(op->immed.immed_value));
		return opimmedlength;

	case D__Iv:
		op->mode = OP_Immed;
		op->opersize = SIZE_v;
		opimmedlength = SIZE_v;

		if (SIZE_v == 2)
		{
			op->immed.immed_value = disp_i->Iw;
			if ((int)disp_i->Iw < 0xA000)
				sprintf(op->string, "%X", (unsigned int)op->immed.immed_value);
			else	sprintf(op->string, "0%X", (unsigned int)op->immed.immed_value);
		} else
		if (SIZE_v == 4)
		{
			op->immed.immed_value = disp_i->Id;
			if (disp_i->Id < 0xA0000000)
				sprintf(op->string, "%X", (unsigned int)op->immed.immed_value);
			else	sprintf(op->string, "0%X", (unsigned int)op->immed.immed_value);
		} else
		if (SIZE_v == 8)
		{
			op->immed.immed_value = disp_i->Iq;
			if (disp_i->Iq < 0xA000000000000000)
				sprintf(op->string, "%I64X", op->immed.immed_value);
			else	sprintf(op->string, "0%I64X", op->immed.immed_value);
		}
		return opimmedlength;

	case D__1:
		op->mode = OP_Immed;
		op->opersize = 1;
		op->immed.immed_value = 1;
		strcpy(op->string, "1");
		return opimmedlength;

	case D__Iz:
		op->mode = OP_Immed;
		op->opersize = SIZE_z;
		opimmedlength = SIZE_z;
		if (SIZE_z == 2)
		{
			op->immed.immed_value = disp_i->Iw;
			if ((int)disp_i->Iw < 0xA000)
				sprintf(op->string, "%X", (unsigned int)op->immed.immed_value);
			else	sprintf(op->string, "0%X", (unsigned int)op->immed.immed_value);
			return opimmedlength;
		}

		if (OperandSize == CODE_MODE_64)
		{
			//op->immed.immed_value = disp_i->Id; //???
			if (disp_i->Id >= 0)
				sprintf(op->string, "%I64X", (unsigned long long)disp_i->Id);
			else	sprintf(op->string, "0%I64X", (unsigned long long)disp_i->Id);
			return opimmedlength;
		} else
		{
			op->immed.immed_value = disp_i->Id;
			if (disp_i->Id < 0xA0000000)
				sprintf(op->string, "%X", (unsigned int)(op->immed.immed_value));
			else	sprintf(op->string, "0%X", (unsigned int)(op->immed.immed_value));
			return opimmedlength;
		}

	case D__Jb:
		op->mode = OP_Near;
		op->opersize = 1;
		op->nearptr.offset = (char)disp_i->Ib;
		opimmedlength = 1;
		op->nearptr.label = eip + op->nearptr.offset + disp_i->ilen;
		if (OperandSize == CODE_MODE_16)
		{
			sprintf(op->string, "%04X", (unsigned short)op->nearptr.label);
		} else
		if (OperandSize == CODE_MODE_32)
		{
			sprintf(op->string, "%08X", (unsigned int)op->nearptr.label);
		} else
		if (OperandSize == CODE_MODE_64)
		{
			sprintf(op->string, "%016I64X", (unsigned long long)op->nearptr.label);
		}
		return opimmedlength;

	case D__Jz:
		op->mode = OP_Near;
		op->opersize = SIZE_z;
		if (SIZE_z == 2)
		{
			op->nearptr.offset = (short)disp_i->Iw;
		} else
		if (SIZE_z == 4)
		{
			op->nearptr.offset = (int)disp_i->Id;
		}
		opimmedlength = SIZE_z;
		op->nearptr.label = eip + op->nearptr.offset + disp_i->ilen;
		if (OperandSize == CODE_MODE_16)
		{
			sprintf(op->string, "%04X", (unsigned short)op->nearptr.label);
		} else
		if (OperandSize == CODE_MODE_32)
		{
			sprintf(op->string, "%08X", (unsigned int)op->nearptr.label);
		} else
		if (OperandSize == CODE_MODE_64)
		{
			sprintf(op->string, "%016I64X", (unsigned long long)op->nearptr.label);
		}
		return opimmedlength;

	case D__Ob: OP_SIZE_RW(op, 1);
	case D_rOb: OP_SIZE_R(op, 1);
	case D_wOb: OP_SIZE_W(op, 1);
	case D_dOb: OP_SIZE_RW(op, 1);

	case D__Ov: OP_SIZE_RW(op, SIZE_v);
	case D_rOv: OP_SIZE_R(op, SIZE_v);
	case D_wOv: OP_SIZE_W(op, SIZE_v);
	case D_dOv: OP_SIZE_RW(op, SIZE_v);

		disp_i->base = -1;
		disp_i->index = -1;
		disp_i->scale = 0;
		disp_i->seg = 3; //DS_IDX

		if (AddressSize == CODE_MODE_64)
		{
			disp_i->as_32 = CODE_MODE_64;
			disp_i->displacement_size = 8;
			disp_i->addr_displacement = disp_i->Iq;
			opimmedlength = 8;
			sprintf(op->string, "%016I64X", disp_i->addr_displacement);
		} else
		if (AddressSize == CODE_MODE_32)
		{
			disp_i->as_32 = CODE_MODE_32;
			disp_i->displacement_size = 4;
			disp_i->addr_displacement = disp_i->Id;
			opimmedlength = 4;
			sprintf(op->string, "%08X", disp_i->Id); //???
		} else
		{
			disp_i->as_32 = CODE_MODE_16;
			disp_i->displacement_size = 2;
			disp_i->addr_displacement = disp_i->Iw;
			opimmedlength = 2;
			sprintf(op->string, "%04X", disp_i->Iw); //???
		}
		disp_i->rm_addr = disp_i->addr_displacement;
		op->mode = OP_Address;
		DispTranDis(&op->addr, disp_i, eip);
		return opimmedlength;

	case D__Ap:
		op->mode = OP_Far;
		if (OperandSize == CODE_MODE_32)
			op->farptr.offset = disp_i->Id;
		else	op->farptr.offset = disp_i->Iw;
		op->farptr.segment = disp_i->Iw2;
		op->opersize = SIZE_p;
		opimmedlength = SIZE_p;
		if (OperandSize == CODE_MODE_32)
			sprintf(op->string, "%04X:%08X", op->farptr.segment, op->farptr.offset);
		else	sprintf(op->string, "%04X:%04X", op->farptr.segment, op->farptr.offset);
		return opimmedlength;

	case D__Pd: OP_SIZE(op, 4);
	case D_rPd: OP_SIZE_R(op, 4);
	case D_wPd: OP_SIZE_W(op, 4);
	case D_dPd: OP_SIZE_RW(op, 4);

	case D__Pq: OP_SIZE(op, 8);
	case D_rPq: OP_SIZE_R(op, 8);
	case D_wPq: OP_SIZE_W(op, 8);
	case D_dPq: OP_SIZE_RW(op, 8);

	case D__Ppi: OP_SIZE(op, 8);
	case D_rPpi: OP_SIZE_R(op, 8);
	case D_wPpi: OP_SIZE_W(op, 8);
	case D_dPpi: OP_SIZE_RW(op, 8);

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->nnn; //???
		strcpy(op->string, RegMMx[disp_i->nnn & 7]);
		return opimmedlength;

	case D__Rv:
		if (disp_i->mod != 3)
			return -1;

		op->mode = OP_Register;
		op->opersize = 4;
		op->reg.reg_index = disp_i->rm;
		strcpy(op->string, RegDWord[op->reg.reg_index]);
		return opimmedlength;

	case D__Rd:
		if (disp_i->mod != 3)
			return -1;

		op->mode = OP_Register;
		op->opersize = SIZE_y;
		op->reg.reg_index = disp_i->rm;
		if (op->opersize == 4)
			strcpy(op->string, RegDWord[op->reg.reg_index]);
		else	strcpy(op->string, RegQWord[op->reg.reg_index]);
		return opimmedlength;

	case D__Sw:
		op->mode = OP_Segment;
		op->opersize = 2;
		op->seg.seg_index = disp_i->nnn & 7; //???
		strcpy(op->string, SegReg[op->seg.seg_index]);
		return opimmedlength;

	case D__T0:
	case D__T1:
	case D__T2:
		op->mode = OP_Register;
		op->opersize = 4;
		op->reg.reg_index = opdata - D__T0;
		sprintf(op->string, "T%x", opdata - D__T0);
		return opimmedlength;

	case D__Xb: OP_SIZE(op, 1);
	case D_rXb: OP_SIZE(op, 1);
	case D_wXb: OP_SIZE(op, 1);

	case D__Xw: OP_SIZE(op, 2);
	case D_rXw: OP_SIZE(op, 2);
	case D_wXw: OP_SIZE(op, 2);

	case D__Xv: OP_SIZE(op, SIZE_v);
	case D_rXv: OP_SIZE(op, SIZE_v);
	case D_wXv: OP_SIZE(op, SIZE_v);

	case D__Xd: OP_SIZE(op, 4);
	case D_rXd: OP_SIZE(op, 4);
	case D_wXd: OP_SIZE(op, 4);

	case D__Xz: OP_SIZE(op, SIZE_z);
	case D_rXz: OP_SIZE(op, SIZE_z);
	case D_wXz: OP_SIZE(op, SIZE_z);

		opimmedlength = 0;
		op->mode = OP_Address;
		disp_i->seg = 3; //DS_IDX
		disp_i->addr_displacement = 0;
		disp_i->displacement_size = 0;
		disp_i->index = -1;
		disp_i->base = 6;                         // ESI_IDX
		disp_i->scale = 0;
		disp_i->rm_addr = dis_cpu->gen_reg[disp_i->base].rerx;
		DispTranDis(&op->addr, disp_i, eip);
		return opimmedlength;

	case D__Yb: OP_SIZE(op, 1);
	case D_rYb: OP_SIZE(op, 1);
	case D_wYb: OP_SIZE(op, 1);

	case D__Yw: OP_SIZE(op, 2);
	case D_rYw: OP_SIZE(op, 2);
	case D_wYw: OP_SIZE(op, 2);

	case D__Yv: OP_SIZE(op, SIZE_v);
	case D_rYv: OP_SIZE(op, SIZE_v);
	case D_wYv: OP_SIZE(op, SIZE_v);

	case D__Yd: OP_SIZE(op, 4);
	case D_rYd: OP_SIZE(op, 4);
	case D_wYd: OP_SIZE(op, 4);

	case D__Yz: OP_SIZE(op, SIZE_z);
	case D_rYz: OP_SIZE(op, SIZE_z);
	case D_wYz: OP_SIZE(op, SIZE_z);

		opimmedlength = 0;
		op->mode = OP_Address;
		disp_i->seg = 0; //ES_IDX
		disp_i->addr_displacement = 0;
		disp_i->displacement_size = 0;
		disp_i->index = -1;
		disp_i->base = 7;                         // EDI_IDX
		disp_i->scale = 0;
		disp_i->rm_addr = dis_cpu->gen_reg[disp_i->base].rerx;
		DispTranDis(&op->addr, disp_i, eip);
		return opimmedlength;

	case D__AL:
	case D__CL:
	case D__DL:
	case D__BL:
	case D__AH:
	case D__CH:
	case D__DH:
	case D__BH:
		op->mode = OP_Register;
		op->opersize = 1;
		op->reg.reg_index = opdata - D__AL;
		strcpy(op->string, RegByte[op->reg.reg_index]);
		return opimmedlength;

	case D__RAL:
	case D__RCL:
	case D__RDL:
	case D__RBL:
	case D__RAH:
	case D__RCH:
	case D__RDH:
	case D__RBH:
		op->mode = OP_Register;
		op->opersize = 1;
		op->reg.reg_index = (8 * (disp_i->rex_prefix & 1)) | (opdata - D__RAL);
		if (disp_i->osx_32 == CODE_MODE_64)
			strcpy(op->string, RegByteRex[op->reg.reg_index]);
		else	strcpy(op->string, RegByte[op->reg.reg_index]);
		return opimmedlength;

	case D__AX:
	case D__CX:
	case D__DX:
	case D__BX:
	case D__SP:
	case D__BP:
	case D__SI:
	case D__DI:
		op->mode = OP_Register;
		op->opersize = 2;
		op->reg.reg_index = opdata - D__AX;
		strcpy(op->string, RegWord[op->reg.reg_index]);
		return opimmedlength;

	case D__eAX:
	case D__eCX:
	case D__eDX:
	case D__eBX:
	case D__eSP:
	case D__eBP:
	case D__eSI:
	case D__eDI:
		op->mode = OP_Register;
		op->opersize = SIZE_v;
		op->reg.reg_index = opdata - D__eAX;
		if (op->opersize == 2)
			strcpy(op->string, RegWord[op->reg.reg_index]);
		else	strcpy(op->string, RegDWord[op->reg.reg_index]);
		return opimmedlength;

	case D__erAX:
	case D__erCX:
	case D__erDX:
	case D__erBX:
	case D__erSP:
	case D__erBP:
	case D__erSI:
	case D__erDI:
		op->mode = OP_Register;
		op->opersize = SIZE_v;
		op->reg.reg_index = (8 * (disp_i->rex_prefix & 1)) | (opdata - D__erAX);
		switch (op->opersize)
		{
		case 2:
			strcpy(op->string, RegWord[op->reg.reg_index]);
			break;
		case 4:
			strcpy(op->string, RegDWord[op->reg.reg_index]);
			break;
		case 8:
			strcpy(op->string, RegQWord[op->reg.reg_index]);
			break;
		}
		return opimmedlength;

	case D__rAX:
	case D__rCX:
	case D__rDX:
	case D__rBX:
	case D__rSP:
	case D__rBP:
	case D__rSI:
	case D__rDI:
		op->mode = OP_Register;
		op->opersize = SIZE_v;
		op->reg.reg_index = opdata - D__rAX;
		switch (op->opersize)
		{
		case 2:
			strcpy(op->string, RegWord[op->reg.reg_index]);
			break;
		case 4:
			strcpy(op->string, RegDWord[op->reg.reg_index]);
			break;
		case 8:
			strcpy(op->string, RegQWord[op->reg.reg_index]);
			break;
		}
		return opimmedlength;

	case D__EAX:
	case D__ECX:
	case D__EDX:
	case D__EBX:
	case D__ESP:
	case D__EBP:
	case D__ESI:
	case D__EDI:
		op->mode = OP_Register;
		op->opersize = 4;
		op->reg.reg_index = opdata - D__EAX;
		strcpy(op->string, RegDWord[op->reg.reg_index]);
		return opimmedlength;

	case D__RAX:
	case D__RCX:
	case D__RDX:
	case D__RBX:
	case D__RSP:
	case D__RBP:
	case D__RSI:
	case D__RDI:
		op->mode = OP_Register;
		op->reg.reg_index = (8 * (disp_i->rex_prefix & 1)) | (opdata - D__RAX);
		if (disp_i->os_32 == CODE_MODE_64)
		{
			op->opersize = 8;
			strcpy(op->string, RegQWord[op->reg.reg_index]);
		} else
		{
			op->opersize = 4;
			strcpy(op->string, RegDWord[op->reg.reg_index]);
		}
		return opimmedlength;

	case D__ES:
	case D__CS:
	case D__SS:
	case D__DS:
	case D__FS:
	case D__GS:
		op->mode = OP_Segment;
		op->opersize = 2;
		op->seg.seg_index = opdata - D__ES;
		strcpy(op->string, SegReg[op->seg.seg_index]);
		return opimmedlength;

	case D__st:
		op->mode = OP_Register;
		op->opersize = 10;
		op->reg.reg_index = 0;
		strcpy(op->string, "ST");
		return opimmedlength;

	case D__st0:
	case D__st1:
	case D__st2:
	case D__st3:
	case D__st4:
	case D__st5:
	case D__st6:
	case D__st7:
		op->mode = OP_Register;
		op->opersize = 10;
		op->reg.reg_index = opdata - D__st0;
		sprintf(op->string, "ST(%x)", op->reg.reg_index);
		return opimmedlength;

	case D__Eb: OP_SIZE_RW(op, 1);
	case D_rEb: OP_SIZE_R(op, 1);
	case D_wEb: OP_SIZE_W(op, 1);
	case D_dEb: OP_SIZE_W(op, 1); //??? w/r

	case D__Ew: OP_SIZE_RW(op, 2);
	case D_rEw: OP_SIZE_R(op, 2);
	case D_wEw: OP_SIZE_W(op, 2);
	case D_dEw: OP_SIZE_W(op, 2); //??? w/r

	case D__Ev: OP_SIZE_RW(op, SIZE_v);
	case D_rEv: OP_SIZE_R(op, SIZE_v);
	case D_wEv: OP_SIZE_W(op, SIZE_v);
	case D_dEv: OP_SIZE_W(op, SIZE_v);

	case D__Ed: OP_SIZE_RW(op, 4);
	case D_rEd: OP_SIZE_R(op, 4);
	case D_wEd: OP_SIZE_W(op, 4);
	case D_dEd: OP_SIZE_RW(op, 4);

	case D__Ep: OP_SIZE_RW(op, SIZE_p);
	case D_rEp: OP_SIZE_R(op, SIZE_p);
	case D_wEp: OP_SIZE_W(op, SIZE_p);
	case D_dEp: OP_SIZE_RW(op, SIZE_p);

		if (disp_i->mod != 3)
		{
			op->mode = OP_Address;
			DispTranDis(&op->addr, disp_i, eip);
			return opimmedlength;
		}

		if (opdata == D__Ep || opdata == D_rEp)
			return -1;

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->rm;

		switch (op->opersize)
		{
		case 1:
			if (disp_i->osx_32 == CODE_MODE_64 && disp_i->rex_prefix)
				strcpy(op->string, RegByteRex[op->reg.reg_index]);
			else	strcpy(op->string, RegByte[op->reg.reg_index]);
			break;
		case 2:
			strcpy(op->string, RegWord[op->reg.reg_index]);
			break;
		case 4:
			strcpy(op->string, RegDWord[op->reg.reg_index]);
			break;
		case 8:
			strcpy(op->string, RegQWord[op->reg.reg_index]);
			break;
		}
		return opimmedlength;

	case D__Ey: OP_SIZE_RW(op, SIZE_y);
	case D_rEy: OP_SIZE_R(op, SIZE_y);
	case D_wEy: OP_SIZE_W(op, SIZE_y);
	case D_dEy: OP_SIZE_RW(op, SIZE_y);

		if (disp_i->mod != 3)
		{
			op->mode = OP_Address;
			DispTranDis(&op->addr, disp_i, eip);
			return opimmedlength;
		}

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->rm;
		if (op->opersize == 4)
			strcpy(op->string, RegDWord[op->reg.reg_index]);
		else	strcpy(op->string, RegQWord[op->reg.reg_index]);
		return opimmedlength;

	case D__Mxdq: OP_SIZE_RW(op, (disp_i->rex_prefix & 8) != 0 ? 16 : 8);
	case D_rMxdq: OP_SIZE_R(op, (disp_i->rex_prefix & 8) != 0 ? 16 : 8);
	case D_wMxdq: OP_SIZE_W(op, (disp_i->rex_prefix & 8) != 0 ? 16 : 8);
	case D_dMxdq: OP_SIZE_RW(op, (disp_i->rex_prefix & 8) != 0 ? 16 : 8);

		if (disp_i->mod == 3)
			return -1;
		
		op->mode = OP_Address;
		DispTranDis(&op->addr, disp_i, eip);
		return opimmedlength;

	case D_wEx: OP_SIZE_W(op, (disp_i->mod == 3) ? SIZE_v : 2);

		if (disp_i->mod != 3)
		{
			op->mode = OP_Address;
			DispTranDis(&op->addr, disp_i, eip);
			return opimmedlength;
		}

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->rm;
		switch (op->opersize)
		{
		case 2:
			strcpy(op->string, RegWord[op->reg.reg_index]);
			break;
		case 4:
			strcpy(op->string, RegDWord[op->reg.reg_index]);
			break;
		case 8:
			strcpy(op->string, RegQWord[op->reg.reg_index]);
			break;
		}
		return opimmedlength;

	case D__M: OP_SIZE_RW(op, -1);
	case D_rM: OP_SIZE_R(op, -1);
	case D_wM: OP_SIZE_W(op, -1);
	case D_dM: OP_SIZE_RW(op, -1);

	case D__Ma: OP_SIZE_RW(op, 8);
	case D_rMa: OP_SIZE_R(op, 8);
	case D_wMa: OP_SIZE_W(op, 8);
	case D_dMa: OP_SIZE_RW(op, 8);

	case D__Mb: OP_SIZE_RW(op, 1);
	case D_rMb: OP_SIZE_R(op, 1);
	case D_wMb: OP_SIZE_W(op, 1);
	case D_dMb: OP_SIZE_RW(op, 1);

	case D__Mp: OP_SIZE_RW(op, SIZE_p);
	case D_rMp: OP_SIZE_R(op, SIZE_p);
	case D_wMp: OP_SIZE_W(op, SIZE_p);
	case D_dMp: OP_SIZE_RW(op, SIZE_p);

	case D__Ms: OP_SIZE_RW(op, 6);
	case D_rMs: OP_SIZE_R(op, 6);
	case D_wMs: OP_SIZE_W(op, 6);
	case D_dMs: OP_SIZE_W(op, 6);

	case D__Mq: OP_SIZE_RW(op, 8);
	case D_rMq: OP_SIZE_R(op, 8);
	case D_wMq: OP_SIZE_W(op, 8);
	case D_dMq: OP_SIZE_RW(op, 8);

	case D__Mw: OP_SIZE_RW(op, 2);
	case D_rMw: OP_SIZE_R(op, 2);
	case D_wMw: OP_SIZE_W(op, 2);
	case D_dMw: OP_SIZE_RW(op, 2);

	case D__Md: OP_SIZE_RW(op, 4);
	case D_rMd: OP_SIZE_R(op, 4);
	case D_wMd: OP_SIZE_W(op, 4);
	case D_dMd: OP_SIZE_RW(op, 4);

	case D__My: OP_SIZE_RW(op, SIZE_y);
	case D_rMy: OP_SIZE_R(op, SIZE_y);
	case D_wMy: OP_SIZE_W(op, SIZE_y);
	case D_dMy: OP_SIZE_RW(op, SIZE_y);

	case D__cVV: OP_SIZE_RW(op, SIZE_v);
	case D_rcVV: OP_SIZE_R(op, SIZE_v);
	case D_wcVV: OP_SIZE_W(op, SIZE_v);
	case D_dcVV: OP_SIZE_RW(op, SIZE_v);

	case D__Mdq: OP_SIZE(op, 16);
	case D_rMdq: OP_SIZE_R(op, 16);
	case D_wMdq: OP_SIZE_W(op, 16);
	case D_dMdq: OP_SIZE_RW(op, 16);

	case D__Mpd: OP_SIZE_RW(op, 16);
	case D_rMpd: OP_SIZE_R(op, 16);
	case D_wMpd: OP_SIZE_W(op, 16);
	case D_dMpd: OP_SIZE_RW(op, 16);

	case D__Mps: OP_SIZE_RW(op, 8);
	case D_rMps: OP_SIZE_R(op, 8);
	case D_wMps: OP_SIZE_W(op, 8);
	case D_dMps: OP_SIZE_RW(op, 8);

		if (disp_i->mod == 3)
			return -1;
		
		op->mode = OP_Address;
		DispTranDis(&op->addr, disp_i, eip);
		return opimmedlength;

	case D__Hm: OP_SIZE(op, 4);
	case D_rHm: OP_SIZE_R(op, 4);
	case D_wHm: OP_SIZE_W(op, 4);
	case D_dHm: OP_SIZE_RW(op, 4);

	case D__Qq: OP_SIZE(op, 8);
	case D_rQq: OP_SIZE_R(op, 8);
	case D_wQq: OP_SIZE_W(op, 8);
	case D_dQq: OP_SIZE_RW(op, 8);

	case D__Qpi: OP_SIZE(op, 8);
	case D_rQpi: OP_SIZE_R(op, 8);
	case D_wQpi: OP_SIZE_W(op, 8);
	case D_dQpi: OP_SIZE_RW(op, 8);

		if (disp_i->mod != 3)
		{
			op->mode = OP_Address;
			DispTranDis(&op->addr, disp_i, eip);
			return opimmedlength;
		}

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->rm; //???
		strcpy(op->string, RegMMx[disp_i->rm & 7]);
		return opimmedlength;

	case D__Vq: OP_SIZE(op, 8);
	case D_rVq: OP_SIZE_R(op, 8);
	case D_wVq: OP_SIZE_W(op, 8);
	case D_dVq: OP_SIZE_RW(op, 8);

	case D__Vdq: OP_SIZE(op, 16);
	case D_rVdq: OP_SIZE_R(op, 16);
	case D_wVdq: OP_SIZE_W(op, 16);
	case D_dVdq: OP_SIZE_RW(op, 16);

	case D__Vps: OP_SIZE(op, 8);
	case D_rVps: OP_SIZE_R(op, 8);
	case D_wVps: OP_SIZE_W(op, 8);
	case D_dVps: OP_SIZE_RW(op, 8);

	case D__Vss: OP_SIZE(op, 16);
	case D_rVss: OP_SIZE_R(op, 16);
	case D_wVss: OP_SIZE_W(op, 16);
	case D_dVss: OP_SIZE_RW(op, 16);

	case D__Vsd: OP_SIZE(op, 16);
	case D_rVsd: OP_SIZE_R(op, 16);
	case D_wVsd: OP_SIZE_W(op, 16);
	case D_dVsd: OP_SIZE_RW(op, 16);

	case D__Vpd: OP_SIZE(op, 16);
	case D_rVpd: OP_SIZE_R(op, 16);
	case D_wVpd: OP_SIZE_W(op, 16);
	case D_dVpd: OP_SIZE_RW(op, 16);

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->nnn;
		strcpy(op->string, RegXMMx[op->reg.reg_index]);
		return opimmedlength;

	case D__sXX: OP_SIZE(op, SIZE_y);
	case D_rsXX: OP_SIZE_R(op, SIZE_y);
	case D_wsXX: OP_SIZE_W(op, SIZE_y);
	case D_dsXX: OP_SIZE_RW(op, SIZE_y);

		if (op->opersize != 4)
			op->opersize = 8;

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->nnn;
		strcpy(op->string, RegXMMx[op->reg.reg_index]);
		return opimmedlength;

	case D__Wq: OP_SIZE(op, 8);
	case D_rWq: OP_SIZE_R(op, 8);
	case D_wWq: OP_SIZE_W(op, 8);
	case D_dWq: OP_SIZE_RW(op, 8);

	case D__Wdq: OP_SIZE(op, 16);
	case D_rWdq: OP_SIZE_R(op, 16);
	case D_wWdq: OP_SIZE_W(op, 16);
	case D_dWdq: OP_SIZE_RW(op, 16);

	case D__Wps: OP_SIZE(op, 8);
	case D_rWps: OP_SIZE_R(op, 8);
	case D_wWps: OP_SIZE_W(op, 8);
	case D_dWps: OP_SIZE_RW(op, 8);

	case D__Wss: OP_SIZE(op, 16);
	case D_rWss: OP_SIZE_R(op, 16);
	case D_wWss: OP_SIZE_W(op, 16);
	case D_dWss: OP_SIZE_RW(op, 16);

	case D__Wpd: OP_SIZE(op, 16);
	case D_rWpd: OP_SIZE_R(op, 16);
	case D_wWpd: OP_SIZE_W(op, 16);
	case D_dWpd: OP_SIZE_RW(op, 16);

	case D__Wsd: OP_SIZE(op, 16);
	case D_rWsd: OP_SIZE_R(op, 16);
	case D_wWsd: OP_SIZE_W(op, 16);
	case D_dWsd: OP_SIZE_RW(op, 16);

		if (disp_i->mod != 3)
		{
			op->mode = OP_Address;
			DispTranDis(&op->addr, disp_i, eip);
			return opimmedlength;
		}

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->rm;
		strcpy(op->string, RegXMMx[op->reg.reg_index]);
		return opimmedlength;

	case D__Ups: OP_SIZE(op, 8);
	case D_rUps: OP_SIZE_R(op, 8);
	case D_wUps: OP_SIZE_W(op, 8);
	case D_dUps: OP_SIZE_RW(op, 8);

	case D__Upd: OP_SIZE(op, 16);
	case D_rUpd: OP_SIZE_R(op, 16);
	case D_wUpd: OP_SIZE_W(op, 16);
	case D_dUpd: OP_SIZE_RW(op, 16);


	case D__Udq: OP_SIZE(op, 16);
	case D_rUdq: OP_SIZE_R(op, 16);
	case D_wUdq: OP_SIZE_W(op, 16);
	case D_dUdq: OP_SIZE_RW(op, 16);

		if (disp_i->mod != 3)
			return -1;

		op->mode = OP_Register;
		op->reg.reg_index = disp_i->rm;
		strcpy(op->string, RegXMMx[op->reg.reg_index]); //sprintf
		return opimmedlength;

	case D__Nq: OP_SIZE(op, 8);
	case D_rNq: OP_SIZE_R(op, 8);
	case D_wNq: OP_SIZE_W(op, 8)
	case D_dNq: OP_SIZE_RW(op, 8);

		if (disp_i->mod != 3)
			return -1;

		op->mode = OP_Register;
		op->opersize = 8;
		op->reg.reg_index = disp_i->rm; //???
		strcpy(op->string, RegMMx[disp_i->rm & 7]);
		return opimmedlength;

	case D__Uq: OP_SIZE(op, 8);
	case D_rUq: OP_SIZE_R(op, 8);
	case D_wUq: OP_SIZE_W(op, 8);
	case D_dUq: OP_SIZE_RW(op, 8);

		if (disp_i->mod != 3)
			return -1;

		op->mode = OP_Register;
		op->opersize = 8;
		op->reg.reg_index = disp_i->rm;
		strcpy(op->string, RegXMMx[op->reg.reg_index]);
		return opimmedlength;

	case D__word_int: OP_SIZE(op, 2);
	case D__dword_int: OP_SIZE(op, 4);
	case D__qword_int: OP_SIZE(op, 8);
	case D__single_real: OP_SIZE(op, 4);
	case D__double_real: OP_SIZE(op, 8);
	case D__extend_real: OP_SIZE(op, 10);
	case D__packed_BCD: OP_SIZE(op, 10);
	case D__2b: OP_SIZE(op, 2);
	case D__14_28b: OP_SIZE(op, 10);
	case D__98_108b: OP_SIZE(op, 10);

		if (disp_i->mod == 3)
			return -1;

		op->mode = OP_Address;
		DispTranDis(&op->addr, disp_i, eip);
		return opimmedlength;
	default:
		opimmedlength = 0;
		op->mode = OP_Invalid;
		op->opersize = 0;
	}

	return opimmedlength;
}

int ProcessOpdata_immde(int opdata, unsigned char *inbuff, _DISP_INSTRUCTION_T *disp_i)
{
	int as; //SIZE_v
	int jp;

	if (disp_i->os_32 == CODE_MODE_16)
	{
		as = 2;
		jp = 2;
	} else
	if (disp_i->os_32 == CODE_MODE_32)
	{
		as = 4;
		jp = 4;
	} else
	if (disp_i->os_32 == CODE_MODE_64)
	{
		as = 8;
		jp = 4;
	}

	int opimmedlength = 0;

	switch (opdata)
	{
	case D__Ib:
	case D_sIb:
	case D__Jb:
		disp_i->Ib = *(unsigned char*)inbuff;
		opimmedlength = 1;
		break;

	case D_zIb:
		disp_i->Ib2 = *(unsigned char*)inbuff;
		opimmedlength = 1;
		break;

	case D__Iw:
		disp_i->Iw = *(unsigned short*)inbuff;
		opimmedlength = 2;
		break;

	case D__Iv:
		opimmedlength = as;
		switch (as)
		{
		case 2:
			disp_i->Iw = *(unsigned short*)inbuff;
			break;
		case 4:
			disp_i->Id = *(unsigned int*)inbuff;
			break;
		case 8:
			disp_i->Iq = *(unsigned long long*)inbuff;
			break;
		}
		break;

	case D__1:
		disp_i->Ib = 1;
		opimmedlength = 0;
		break;

	case D__Iz:
	case D__Jz:
		if (jp == 2)
			disp_i->Iw = *(unsigned short*)inbuff;
		else	disp_i->Id = *(unsigned int*)inbuff;
		opimmedlength = jp;
		break;

	case D__Ob:
	case D_rOb:
	case D_wOb:
	case D_dOb:

	case D__Ov:
	case D_rOv:
	case D_wOv:
	case D_dOv:

		if (disp_i->as_32 == CODE_MODE_16)
		{
			disp_i->Iw = *(unsigned short*)inbuff;
			opimmedlength = 2;
		} else
		if (disp_i->as_32 == CODE_MODE_32)
		{
			disp_i->Id = *(unsigned int*)inbuff;
			opimmedlength = 4;
		} else
		if (disp_i->as_32 == CODE_MODE_64)
		{
			disp_i->Iq = *(unsigned long long*)inbuff;
			opimmedlength = 8;
		}
		//ASSERT
		break;
	case D__Ap:
		opimmedlength = as+2;
		if (as == 2)
		{
			disp_i->Iw = *(unsigned short*)inbuff;
			disp_i->Iw2 = *((unsigned short*)inbuff + 1);
		} else
		//as == 4
		//as == 8
		{
			disp_i->Id = *(unsigned int*)inbuff;
			disp_i->Iw2 = *((unsigned short*)inbuff + 2);
		}
		break;
	}
	return opimmedlength;
}

int PutStr(char *buff, const char *s)
{
	int i;
	for (i = 0; s[i]; ++i)
		buff[i] = s[i];
	return i;
}

int PrintPasm(_INSTRUCTION_INFORMATION *pInstInfo, char *pasm, int u_x1, int u_x2)
{
	int i;
	char buff[32];
	int length;
	const char **preg;

	for (i = 0; i <= 80; ++i)
		pasm[i] = 0x20;
	pasm[i] = 0;

	if (pInstInfo->Lock != -1)
	{
		length = PutStr(&pasm[u_x1], pInstInfo->LockName);
		if (length)
			u_x1 += length + 1;
	}

	if (pInstInfo->Repeat != -1)
	{
		length = PutStr(&pasm[u_x1], pInstInfo->RepeatName);
		if (length)
			u_x1 += length + 1;
	}

	u_x1 += PutStr(&pasm[u_x1], pInstInfo->Name);

	_OPERAND_ITEM *op = pInstInfo->op;

	bool stack = false;
	for (i = 0; i < 3; ++i)
	{
		if (op[i].mode == OP_Register &&
			(op[i].opersize == 4 || op[i].opersize == 8) &&
			op[i].reg.reg_index == ESP_IDX)
		{
			stack = true;
			break;
		}
	}

	for (i = 0; i < 3; ++i)
	{
		if (op->mode == OP_Invalid) break;

		if (!i)
		{
			if (u_x1 >= u_x2)
				++u_x1;
			else	u_x1 = u_x2;
		}

		if (i > 0)
		{
			u_x1 += PutStr(&pasm[u_x1], ",");
		}

		if (op->mode == OP_Address)
		{
			if (pInstInfo->xxxxptr != -1 && pInstInfo->xxxxptr)
			{
				u_x1 += PutStr(&pasm[u_x1], xxxxptr[pInstInfo->xxxxptr]);
				u_x1 += PutStr(&pasm[u_x1], " PTR ");
			}

			if (pInstInfo->SegmentPrefix != -1)
			{
				u_x1 += PutStr(&pasm[u_x1], SegReg[pInstInfo->SegmentPrefix]);
				u_x1 += PutStr(&pasm[u_x1], ":");
			}

			if (op->addr.address_size == 1)
			{
				preg = RegDWord;
			} else
			if (op->addr.address_size == 2)
			{
				preg = RegQWord;
			} else
			{
				preg = RegWord;
			}

			u_x1 += PutStr(&pasm[u_x1], "[");

			if (op->addr.base != -1)
			{
				u_x1 += PutStr(&pasm[u_x1], preg[op->addr.base]);
			}

			if (op->addr.index != -1)
			{
				if (op->addr.base != -1)
				{
					u_x1 += PutStr(&pasm[u_x1], "+");
				}

				u_x1 += PutStr(&pasm[u_x1], preg[op->addr.index]);

				if (op->addr.scale)
				{
					sprintf(buff, "*%1x", 1 << op->addr.scale);
					u_x1 += PutStr(&pasm[u_x1], buff);
				}
			}

			if (op->addr.displacement_size >= 1 &&
				(op->addr.base != -1 || op->addr.index != -1) &&
				(op->addr.displacement_size != 1 || op->addr.displacement))
			{
				u_x1 += PutStr(&pasm[u_x1], "+");
			}

			//TODO reimplement

			switch (op->addr.displacement_size)
			{
			case 1:
				if (op->addr.displacement)
				{
					if (op->addr.displacement < 0x80)
					{
						sprintf(buff, "%X", (unsigned char)op->addr.displacement);
					} else
					{
						--u_x1;
						sprintf(buff, "-%X", -(unsigned char)op->addr.displacement);
					}
				} else
				{
					buff[0] = 0;
				}
				u_x1 += PutStr(&pasm[u_x1], buff);
				break;
			case 2:
				sprintf(buff, "%04X", (unsigned short)op->addr.displacement);
				u_x1 += PutStr(&pasm[u_x1], buff);
				break;
			case 4:
				sprintf(buff, "%08X", (unsigned int)op->addr.displacement);
				u_x1 += PutStr(&pasm[u_x1], buff);
				break;
			case 8:
				sprintf(buff, "%016I64X", (unsigned long long)op->addr.displacement);
				u_x1 += PutStr(&pasm[u_x1], buff);
				break;
			}
			u_x1 += PutStr(&pasm[u_x1], "]");
		} else
		{
			u_x1 += PutStr(&pasm[u_x1], op->string);
		}
		++op;
 
	}

	pasm[u_x1] = 0;

	for (i = 0; i < u_x1; ++i)
	{
		MAKE_CHAR_LOWER(pasm[i]);
	}
	return u_x1;
}

#define ATTR_B		1
#define ATTR_W		2
#define ATTR_D		4
#define ATTR_X64	8
#define ATTR_COND	0x10
#define ATTR_MODRM	0x200
//0x400
//0x800
//0x1000
//0x2000
//0x4000
//0x8000
//0x10000
//0x20000
//0x40000
//0x80000
//0x100000
//0x200000
//0x400000
//0x800000
//0x1000000
//0x2000000
#define ATTR_JMP	0x4000000
#define ATTR_SP		0x8000000
#define ATTR_X64_ONLY	 0x10000000
#define ATTR_X64_INVALID 0x20000000

_DIS_OPCODE_T GroupG6[] = {
      {ATTR_MODRM,C_SLDT,D_wEx,D__NONE,D__NONE,"SLDT",0xf,0x202,0x2},
      {ATTR_MODRM,C_STR,D_wEx,D__NONE,D__NONE,"STR",0xf,0x10202,0x2},
      {ATTR_MODRM,C_LLDT,D_rEw,D__NONE,D__NONE,"LLDT",0xf,0x20202,0x1},
      {ATTR_MODRM,C_LTR,D_rEw,D__NONE,D__NONE,"LTR",0xf,0x30202,0x1},
      {ATTR_MODRM,C_VERR,D_rEw,D__NONE,D__NONE,"VERR",0xf,0x40202,0x1},
      {ATTR_MODRM,C_VERW,D_rEw,D__NONE,D__NONE,"VERW",0xf,0x50202,0x1},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupG8EvIb[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_BT,D_rEv,D__Ib,D__NONE,"BT",0xba0f,0x40202,0x5},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_BTS,D__Ev,D__Ib,D__NONE,"BTS",0xba0f,0x50202,0x6},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_BTR,D__Ev,D__Ib,D__NONE,"BTR",0xba0f,0x60202,0x6},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_BTC,D__Ev,D__Ib,D__NONE,"BTC",0xba0f,0x70202,0x6},
};
_DIS_OPCODE_T GroupG1_80[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_ADD,D__Eb,D__Ib,D__NONE,"ADD",0x80,0x201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_OR,D__Eb,D__Ib,D__NONE,"OR",0x80,0x10201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_ADC,D__Eb,D__Ib,D__NONE,"ADC",0x80,0x20201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SBB,D__Eb,D__Ib,D__NONE,"SBB",0x80,0x30201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_AND,D__Eb,D__Ib,D__NONE,"AND",0x80,0x40201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SUB,D__Eb,D__Ib,D__NONE,"SUB",0x80,0x50201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_XOR,D__Eb,D__Ib,D__NONE,"XOR",0x80,0x60201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_CMP,D_rEb,D__Ib,D__NONE,"CMP",0x80,0x70201,0x5},
};
_DIS_OPCODE_T GroupG1_81[] = {
      {ATTR_X64|ATTR_MODRM,C_ADD,D__Ev,D__Iz,D__NONE,"ADD",0x81,0x201,0x7},
      {ATTR_X64|ATTR_MODRM,C_OR,D__Ev,D__Iz,D__NONE,"OR",0x81,0x10201,0x7},
      {ATTR_X64|ATTR_MODRM,C_ADC,D__Ev,D__Iz,D__NONE,"ADC",0x81,0x20201,0x7},
      {ATTR_X64|ATTR_MODRM,C_SBB,D__Ev,D__Iz,D__NONE,"SBB",0x81,0x30201,0x7},
      {ATTR_X64|ATTR_MODRM,C_AND,D__Ev,D__Iz,D__NONE,"AND",0x81,0x40201,0x7},
      {ATTR_X64|ATTR_MODRM,C_SUB,D__Ev,D__Iz,D__NONE,"SUB",0x81,0x50201,0x7},
      {ATTR_X64|ATTR_MODRM,C_XOR,D__Ev,D__Iz,D__NONE,"XOR",0x81,0x60201,0x7},
      {ATTR_X64|ATTR_MODRM,C_CMP,D_rEv,D__Iz,D__NONE,"CMP",0x81,0x70201,0x5},
};
_DIS_OPCODE_T GroupG1_82[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM|ATTR_X64_INVALID,C_ADD,D__Eb,D__Ib,D__NONE,"ADD",0x82,0x201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM|ATTR_X64_INVALID,C_OR,D__Eb,D__Ib,D__NONE,"OR",0x82,0x10201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM|ATTR_X64_INVALID,C_ADC,D__Eb,D__Ib,D__NONE,"ADC",0x82,0x20201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM|ATTR_X64_INVALID,C_SBB,D__Eb,D__Ib,D__NONE,"SBB",0x82,0x30201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM|ATTR_X64_INVALID,C_AND,D__Eb,D__Ib,D__NONE,"AND",0x82,0x40201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM|ATTR_X64_INVALID,C_SUB,D__Eb,D__Ib,D__NONE,"SUB",0x82,0x50201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM|ATTR_X64_INVALID,C_XOR,D__Eb,D__Ib,D__NONE,"XOR",0x82,0x60201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM|ATTR_X64_INVALID,C_CMP,D_rEb,D__Ib,D__NONE,"CMP",0x82,0x70201,0x5},
};
_DIS_OPCODE_T GroupG1_83[] = {
      {ATTR_D|ATTR_MODRM,C_ADD,D__Ev,D_sIb,D__NONE,"ADD",0x83,0x201,0x7},
      {ATTR_D|ATTR_MODRM,C_OR,D__Ev,D_sIb,D__NONE,"OR",0x83,0x10201,0x7},
      {ATTR_D|ATTR_MODRM,C_ADC,D__Ev,D_sIb,D__NONE,"ADC",0x83,0x20201,0x7},
      {ATTR_D|ATTR_MODRM,C_SBB,D__Ev,D_sIb,D__NONE,"SBB",0x83,0x30201,0x7},
      {ATTR_D|ATTR_MODRM,C_AND,D__Ev,D_sIb,D__NONE,"AND",0x83,0x40201,0x7},
      {ATTR_D|ATTR_MODRM,C_SUB,D__Ev,D_sIb,D__NONE,"SUB",0x83,0x50201,0x7},
      {ATTR_D|ATTR_MODRM,C_XOR,D__Ev,D_sIb,D__NONE,"XOR",0x83,0x60201,0x7},
      {ATTR_D|ATTR_MODRM,C_CMP,D_rEv,D_sIb,D__NONE,"CMP",0x83,0x70201,0x5},
};
_DIS_OPCODE_T GroupG2Eb[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_ROL,D__Eb,D__Ib,D__NONE,"ROL",0xc0,0x201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_ROR,D__Eb,D__Ib,D__NONE,"ROR",0xc0,0x10201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_RCL,D__Eb,D__Ib,D__NONE,"RCL",0xc0,0x20201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_RCR,D__Eb,D__Ib,D__NONE,"RCR",0xc0,0x30201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SHL,D__Eb,D__Ib,D__NONE,"SHL",0xc0,0x40201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SHR,D__Eb,D__Ib,D__NONE,"SHR",0xc0,0x50201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SAL,D__Eb,D__Ib,D__NONE,"SAL",0xc0,0x60201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SAR,D__Eb,D__Ib,D__NONE,"SAR",0xc0,0x70201,0x7},
};
_DIS_OPCODE_T GroupG2Ew[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_ROL,D__Ev,D__Ib,D__NONE,"ROL",0xc1,0x201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_ROR,D__Ev,D__Ib,D__NONE,"ROR",0xc1,0x10201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_RCL,D__Ev,D__Ib,D__NONE,"RCL",0xc1,0x20201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_RCR,D__Ev,D__Ib,D__NONE,"RCR",0xc1,0x30201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SHL,D__Ev,D__Ib,D__NONE,"SHL",0xc1,0x40201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SHR,D__Ev,D__Ib,D__NONE,"SHR",0xc1,0x50201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SAL,D__Ev,D__Ib,D__NONE,"SAL",0xc1,0x60201,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SAR,D__Ev,D__Ib,D__NONE,"SAR",0xc1,0x70201,0x7},
};
_DIS_OPCODE_T GroupG11Eb[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_MOV,D_wEb,D__Ib,D__NONE,"MOV",0xc6,0x201,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupG11Ew[] = {
      {ATTR_X64|ATTR_MODRM,C_MOV,D_wEv,D__Iz,D__NONE,"MOV",0xc7,0x201,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupG2EbI1[] = {
      {ATTR_B|ATTR_MODRM,C_ROL,D__Eb,D__1,D__NONE,"ROL",0xd0,0x201,0x7},
      {ATTR_B|ATTR_MODRM,C_ROR,D__Eb,D__1,D__NONE,"ROR",0xd0,0x10201,0x7},
      {ATTR_B|ATTR_MODRM,C_RCL,D__Eb,D__1,D__NONE,"RCL",0xd0,0x20201,0x7},
      {ATTR_B|ATTR_MODRM,C_RCR,D__Eb,D__1,D__NONE,"RCR",0xd0,0x30201,0x7},
      {ATTR_B|ATTR_MODRM,C_SHL,D__Eb,D__1,D__NONE,"SHL",0xd0,0x40201,0x7},
      {ATTR_B|ATTR_MODRM,C_SHR,D__Eb,D__1,D__NONE,"SHR",0xd0,0x50201,0x7},
      {ATTR_B|ATTR_MODRM,C_SAL,D__Eb,D__1,D__NONE,"SAL",0xd0,0x60201,0x7},
      {ATTR_B|ATTR_MODRM,C_SAR,D__Eb,D__1,D__NONE,"SAR",0xd0,0x70201,0x7},
};
_DIS_OPCODE_T GroupG2EvI1[] = {
      {ATTR_B|ATTR_MODRM,C_ROL,D__Ev,D__1,D__NONE,"ROL",0xd1,0x201,0x7},
      {ATTR_B|ATTR_MODRM,C_ROR,D__Ev,D__1,D__NONE,"ROR",0xd1,0x10201,0x7},
      {ATTR_B|ATTR_MODRM,C_RCL,D__Ev,D__1,D__NONE,"RCL",0xd1,0x20201,0x7},
      {ATTR_B|ATTR_MODRM,C_RCR,D__Ev,D__1,D__NONE,"RCR",0xd1,0x30201,0x7},
      {ATTR_B|ATTR_MODRM,C_SHL,D__Ev,D__1,D__NONE,"SHL",0xd1,0x40201,0x7},
      {ATTR_B|ATTR_MODRM,C_SHR,D__Ev,D__1,D__NONE,"SHR",0xd1,0x50201,0x7},
      {ATTR_B|ATTR_MODRM,C_SAL,D__Ev,D__1,D__NONE,"SAL",0xd1,0x60201,0x7},
      {ATTR_B|ATTR_MODRM,C_SAR,D__Ev,D__1,D__NONE,"SAR",0xd1,0x70201,0x7},
};
_DIS_OPCODE_T GroupG2EbCL[] = {
      {ATTR_MODRM,C_ROL,D__Eb,D__CL,D__NONE,"ROL",0xd2,0x201,0x7},
      {ATTR_MODRM,C_ROR,D__Eb,D__CL,D__NONE,"ROR",0xd2,0x10201,0x7},
      {ATTR_MODRM,C_RCL,D__Eb,D__CL,D__NONE,"RCL",0xd2,0x20201,0x7},
      {ATTR_MODRM,C_RCR,D__Eb,D__CL,D__NONE,"RCR",0xd2,0x30201,0x7},
      {ATTR_MODRM,C_SHL,D__Eb,D__CL,D__NONE,"SHL",0xd2,0x40201,0x7},
      {ATTR_MODRM,C_SHR,D__Eb,D__CL,D__NONE,"SHR",0xd2,0x50201,0x7},
      {ATTR_MODRM,C_SAL,D__Eb,D__CL,D__NONE,"SAL",0xd2,0x60201,0x7},
      {ATTR_MODRM,C_SAR,D__Eb,D__CL,D__NONE,"SAR",0xd2,0x70201,0x7},
};
_DIS_OPCODE_T GroupG2EvCL[] = {
      {ATTR_MODRM,C_ROL,D__Ev,D__CL,D__NONE,"ROL",0xd3,0x201,0x7},
      {ATTR_MODRM,C_ROR,D__Ev,D__CL,D__NONE,"ROR",0xd3,0x10201,0x7},
      {ATTR_MODRM,C_RCL,D__Ev,D__CL,D__NONE,"RCL",0xd3,0x20201,0x7},
      {ATTR_MODRM,C_RCR,D__Ev,D__CL,D__NONE,"RCR",0xd3,0x30201,0x7},
      {ATTR_MODRM,C_SHL,D__Ev,D__CL,D__NONE,"SHL",0xd3,0x40201,0x7},
      {ATTR_MODRM,C_SHR,D__Ev,D__CL,D__NONE,"SHR",0xd3,0x50201,0x7},
      {ATTR_MODRM,C_SAL,D__Ev,D__CL,D__NONE,"SAL",0xd3,0x60201,0x7},
      {ATTR_MODRM,C_SAR,D__Ev,D__CL,D__NONE,"SAR",0xd3,0x70201,0x7},
};
_DIS_OPCODE_T GroupG3Eb[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_TEST,D_rEb,D__Ib,D__NONE,"TEST",0xf6,0x201,0x5},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf6,0x10201,0x0},
      {ATTR_MODRM,C_NOT,D__Eb,D__NONE,D__NONE,"NOT",0xf6,0x20201,0x3},
      {ATTR_MODRM,C_NEG,D__Eb,D__NONE,D__NONE,"NEG",0xf6,0x30201,0x3},
      {ATTR_MODRM,C_MUL,D_rEb,D__NONE,D__NONE,"MUL",0xf6,0x40201,0x1},
      {ATTR_MODRM,C_IMUL,D_rEb,D__NONE,D__NONE,"IMUL",0xf6,0x50201,0x1},
      {ATTR_MODRM,C_DIV,D_rEb,D__NONE,D__NONE,"DIV",0xf6,0x60201,0x1},
      {ATTR_MODRM,C_IDIV,D_rEb,D__NONE,D__NONE,"IDIV",0xf6,0x70201,0x1},
};
_DIS_OPCODE_T GroupG3Ev[] = {
      {ATTR_X64|ATTR_MODRM,C_TEST,D_rEv,D__Iz,D__NONE,"TEST",0xf7,0x201,0x5},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf7,0x10201,0x0},
      {ATTR_MODRM,C_NOT,D__Ev,D__NONE,D__NONE,"NOT",0xf7,0x20201,0x3},
      {ATTR_MODRM,C_NEG,D__Ev,D__NONE,D__NONE,"NEG",0xf7,0x30201,0x3},
      {ATTR_MODRM,C_MUL,D_rEv,D__NONE,D__NONE,"MUL",0xf7,0x40201,0x1},
      {ATTR_MODRM,C_IMUL,D_rEv,D__NONE,D__NONE,"IMUL",0xf7,0x50201,0x1},
      {ATTR_MODRM,C_DIV,D_rEv,D__NONE,D__NONE,"DIV",0xf7,0x60201,0x1},
      {ATTR_MODRM,C_IDIV,D_rEv,D__NONE,D__NONE,"IDIV",0xf7,0x70201,0x1},
};
_DIS_OPCODE_T GroupG4[] = {
      {ATTR_MODRM,C_INC,D__Eb,D__NONE,D__NONE,"INC",0xfe,0x201,0x3},
      {ATTR_MODRM,C_DEC,D__Eb,D__NONE,D__NONE,"DEC",0xfe,0x10201,0x3},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupG5v[] = {
      {ATTR_MODRM,C_INC,D_dEv,D__NONE,D__NONE,"INC",0xff,0x201,0x3},
      {ATTR_MODRM,C_DEC,D_dEv,D__NONE,D__NONE,"DEC",0xff,0x10201,0x3},
      {ATTR_MODRM|ATTR_JMP,C_CALL,D_rEv,D__NONE,D__NONE,"CALL",0xff,0x20201,0x1},
      {ATTR_MODRM,C_CALL,D_rEp,D__NONE,D__NONE,"CALL",0xff,0x30201,0x1},
      {ATTR_MODRM|ATTR_JMP,C_JMP,D_rEv,D__NONE,D__NONE,"JMP",0xff,0x40201,0x1},
      {ATTR_MODRM,C_JMP,D_rEp,D__NONE,D__NONE,"JMP",0xff,0x50201,0x1},
      {ATTR_MODRM|ATTR_SP,C_PUSH,D_rEv,D__NONE,D__NONE,"PUSH",0xff,0x60201,0x1},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupG1_8f[] = {
      {ATTR_MODRM|ATTR_SP,C_POP,D_wEv,D__NONE,D__NONE,"POP",0x8f,0x201,0x2},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T FPGroupd8[] = {
      {ATTR_MODRM,C_FADD,D__single_real,D__NONE,D__NONE,"FADD",0xd8,0x101,0x1},
      {ATTR_MODRM,C_FMUL,D__single_real,D__NONE,D__NONE,"FMUL",0xd8,0x10101,0x1},
      {ATTR_MODRM,C_FCOM,D__single_real,D__NONE,D__NONE,"FCOM",0xd8,0x20101,0x1},
      {ATTR_MODRM,C_FCOMP,D__single_real,D__NONE,D__NONE,"FCOMP",0xd8,0x30101,0x1},
      {ATTR_MODRM,C_FSUB,D__single_real,D__NONE,D__NONE,"FSUB",0xd8,0x40101,0x1},
      {ATTR_MODRM,C_FSUBR,D__single_real,D__NONE,D__NONE,"FSUBR",0xd8,0x50101,0x1},
      {ATTR_MODRM,C_FDIV,D__single_real,D__NONE,D__NONE,"FDIV",0xd8,0x60101,0x1},
      {ATTR_MODRM,C_FDIVR,D__single_real,D__NONE,D__NONE,"FDIVR",0xd8,0x70101,0x1},
      {0,C_FADD,D__st,D__st0,D__NONE,"FADD",0xc0d8,0x80102,0x7},
      {0,C_FADD,D__st,D__st1,D__NONE,"FADD",0xc1d8,0x90102,0x7},
      {0,C_FADD,D__st,D__st2,D__NONE,"FADD",0xc2d8,0xa0102,0x7},
      {0,C_FADD,D__st,D__st3,D__NONE,"FADD",0xc3d8,0xb0102,0x7},
      {0,C_FADD,D__st,D__st4,D__NONE,"FADD",0xc4d8,0xc0102,0x7},
      {0,C_FADD,D__st,D__st5,D__NONE,"FADD",0xc5d8,0xd0102,0x7},
      {0,C_FADD,D__st,D__st6,D__NONE,"FADD",0xc6d8,0xe0102,0x7},
      {0,C_FADD,D__st,D__st7,D__NONE,"FADD",0xc7d8,0xf0102,0x7},
      {0,C_FMUL,D__st,D__st0,D__NONE,"FMUL",0xc8d8,0x100102,0x7},
      {0,C_FMUL,D__st,D__st1,D__NONE,"FMUL",0xc9d8,0x110102,0x7},
      {0,C_FMUL,D__st,D__st2,D__NONE,"FMUL",0xcad8,0x120102,0x7},
      {0,C_FMUL,D__st,D__st3,D__NONE,"FMUL",0xcbd8,0x130102,0x7},
      {0,C_FMUL,D__st,D__st4,D__NONE,"FMUL",0xccd8,0x140102,0x7},
      {0,C_FMUL,D__st,D__st5,D__NONE,"FMUL",0xcdd8,0x150102,0x7},
      {0,C_FMUL,D__st,D__st6,D__NONE,"FMUL",0xced8,0x160102,0x7},
      {0,C_FMUL,D__st,D__st7,D__NONE,"FMUL",0xcfd8,0x170102,0x7},
      {0,C_FCOM,D__st,D__st0,D__NONE,"FCOM",0xd0d8,0x180102,0x5},
      {0,C_FCOM,D__st,D__st1,D__NONE,"FCOM",0xd1d8,0x190102,0x5},
      {0,C_FCOM,D__st,D__st2,D__NONE,"FCOM",0xd2d8,0x1a0102,0x5},
      {0,C_FCOM,D__st,D__st3,D__NONE,"FCOM",0xd3d8,0x1b0102,0x5},
      {0,C_FCOM,D__st,D__st4,D__NONE,"FCOM",0xd4d8,0x1c0102,0x5},
      {0,C_FCOM,D__st,D__st5,D__NONE,"FCOM",0xd5d8,0x1d0102,0x5},
      {0,C_FCOM,D__st,D__st6,D__NONE,"FCOM",0xd6d8,0x1e0102,0x5},
      {0,C_FCOM,D__st,D__st7,D__NONE,"FCOM",0xd7d8,0x1f0102,0x5},
      {0,C_FCOMP,D__st,D__st0,D__NONE,"FCOMP",0xd8d8,0x200102,0x5},
      {0,C_FCOMP,D__st,D__st1,D__NONE,"FCOMP",0xd9d8,0x210102,0x5},
      {0,C_FCOMP,D__st,D__st2,D__NONE,"FCOMP",0xdad8,0x220102,0x5},
      {0,C_FCOMP,D__st,D__st3,D__NONE,"FCOMP",0xdbd8,0x230102,0x5},
      {0,C_FCOMP,D__st,D__st4,D__NONE,"FCOMP",0xdcd8,0x240102,0x5},
      {0,C_FCOMP,D__st,D__st5,D__NONE,"FCOMP",0xddd8,0x250102,0x5},
      {0,C_FCOMP,D__st,D__st6,D__NONE,"FCOMP",0xded8,0x260102,0x5},
      {0,C_FCOMP,D__st,D__st7,D__NONE,"FCOMP",0xdfd8,0x270102,0x5},
      {0,C_FSUB,D__st,D__st0,D__NONE,"FSUB",0xe0d8,0x280102,0x7},
      {0,C_FSUB,D__st,D__st1,D__NONE,"FSUB",0xe1d8,0x290102,0x7},
      {0,C_FSUB,D__st,D__st2,D__NONE,"FSUB",0xe2d8,0x2a0102,0x7},
      {0,C_FSUB,D__st,D__st3,D__NONE,"FSUB",0xe3d8,0x2b0102,0x7},
      {0,C_FSUB,D__st,D__st4,D__NONE,"FSUB",0xe4d8,0x2c0102,0x7},
      {0,C_FSUB,D__st,D__st5,D__NONE,"FSUB",0xe5d8,0x2d0102,0x7},
      {0,C_FSUB,D__st,D__st6,D__NONE,"FSUB",0xe6d8,0x2e0102,0x7},
      {0,C_FSUB,D__st,D__st7,D__NONE,"FSUB",0xe7d8,0x2f0102,0x7},
      {0,C_FSUBR,D__st,D__st0,D__NONE,"FSUBR",0xe8d8,0x300102,0x7},
      {0,C_FSUBR,D__st,D__st1,D__NONE,"FSUBR",0xe9d8,0x310102,0x7},
      {0,C_FSUBR,D__st,D__st2,D__NONE,"FSUBR",0xead8,0x320102,0x7},
      {0,C_FSUBR,D__st,D__st3,D__NONE,"FSUBR",0xebd8,0x330102,0x7},
      {0,C_FSUBR,D__st,D__st4,D__NONE,"FSUBR",0xecd8,0x340102,0x7},
      {0,C_FSUBR,D__st,D__st5,D__NONE,"FSUBR",0xedd8,0x350102,0x7},
      {0,C_FSUBR,D__st,D__st6,D__NONE,"FSUBR",0xeed8,0x360102,0x7},
      {0,C_FSUBR,D__st,D__st7,D__NONE,"FSUBR",0xefd8,0x370102,0x7},
      {0,C_FDIV,D__st,D__st0,D__NONE,"FDIV",0xf0d8,0x380102,0x7},
      {0,C_FDIV,D__st,D__st1,D__NONE,"FDIV",0xf1d8,0x390102,0x7},
      {0,C_FDIV,D__st,D__st2,D__NONE,"FDIV",0xf2d8,0x3a0102,0x7},
      {0,C_FDIV,D__st,D__st3,D__NONE,"FDIV",0xf3d8,0x3b0102,0x7},
      {0,C_FDIV,D__st,D__st4,D__NONE,"FDIV",0xf4d8,0x3c0102,0x7},
      {0,C_FDIV,D__st,D__st5,D__NONE,"FDIV",0xf5d8,0x3d0102,0x7},
      {0,C_FDIV,D__st,D__st6,D__NONE,"FDIV",0xf6d8,0x3e0102,0x7},
      {0,C_FDIV,D__st,D__st7,D__NONE,"FDIV",0xf7d8,0x3f0102,0x7},
      {0,C_FDIVR,D__st,D__st0,D__NONE,"FDIVR",0xf8d8,0x400102,0x7},
      {0,C_FDIVR,D__st,D__st1,D__NONE,"FDIVR",0xf9d8,0x410102,0x7},
      {0,C_FDIVR,D__st,D__st2,D__NONE,"FDIVR",0xfad8,0x420102,0x7},
      {0,C_FDIVR,D__st,D__st3,D__NONE,"FDIVR",0xfbd8,0x430102,0x7},
      {0,C_FDIVR,D__st,D__st4,D__NONE,"FDIVR",0xfcd8,0x440102,0x7},
      {0,C_FDIVR,D__st,D__st5,D__NONE,"FDIVR",0xfdd8,0x450102,0x7},
      {0,C_FDIVR,D__st,D__st6,D__NONE,"FDIVR",0xfed8,0x460102,0x7},
      {0,C_FDIVR,D__st,D__st7,D__NONE,"FDIVR",0xffd8,0x470102,0x7},
};
_DIS_OPCODE_T FPGroupd9[] = {
      {ATTR_MODRM,C_FLD,D__single_real,D__NONE,D__NONE,"FLD",0xd9,0x101,0x1},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_FST,D__single_real,D__NONE,D__NONE,"FST",0xd9,0x20101,0x2},
      {ATTR_MODRM,C_FSTP,D__single_real,D__NONE,D__NONE,"FSTP",0xd9,0x30101,0x2},
      {ATTR_MODRM,C_FLDENV,D__14_28b,D__NONE,D__NONE,"FLDENV",0xd9,0x40101,0x1},
      {ATTR_MODRM,C_FLDCW,D__2b,D__NONE,D__NONE,"FLDCW",0xd9,0x50101,0x1},
      {ATTR_MODRM,C_FSTENV,D__14_28b,D__NONE,D__NONE,"FSTENV",0xd9,0x60101,0x2},
      {ATTR_MODRM,C_FSTCW,D__2b,D__NONE,D__NONE,"FSTCW",0xd9,0x70101,0x2},
      {0,C_FLD,D__st,D__st0,D__NONE,"FLD",0xc0d9,0x80102,0x7},
      {0,C_FLD,D__st,D__st1,D__NONE,"FLD",0xc1d9,0x90102,0x7},
      {0,C_FLD,D__st,D__st2,D__NONE,"FLD",0xc2d9,0xa0102,0x7},
      {0,C_FLD,D__st,D__st3,D__NONE,"FLD",0xc3d9,0xb0102,0x7},
      {0,C_FLD,D__st,D__st4,D__NONE,"FLD",0xc4d9,0xc0102,0x7},
      {0,C_FLD,D__st,D__st5,D__NONE,"FLD",0xc5d9,0xd0102,0x7},
      {0,C_FLD,D__st,D__st6,D__NONE,"FLD",0xc6d9,0xe0102,0x7},
      {0,C_FLD,D__st,D__st7,D__NONE,"FLD",0xc7d9,0xf0102,0x7},
      {0,C_FXCH,D__st,D__st0,D__NONE,"FXCH",0xc8d9,0x100102,0xf},
      {0,C_FXCH,D__st,D__st1,D__NONE,"FXCH",0xc9d9,0x110102,0xf},
      {0,C_FXCH,D__st,D__st2,D__NONE,"FXCH",0xcad9,0x120102,0xf},
      {0,C_FXCH,D__st,D__st3,D__NONE,"FXCH",0xcbd9,0x130102,0xf},
      {0,C_FXCH,D__st,D__st4,D__NONE,"FXCH",0xccd9,0x140102,0xf},
      {0,C_FXCH,D__st,D__st5,D__NONE,"FXCH",0xcdd9,0x150102,0xf},
      {0,C_FXCH,D__st,D__st6,D__NONE,"FXCH",0xced9,0x160102,0xf},
      {0,C_FXCH,D__st,D__st7,D__NONE,"FXCH",0xcfd9,0x170102,0xf},
      {0,C_FNOP,D__NONE,D__NONE,D__NONE,"FNOP",0xd0d9,0x180102,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d8 "FSTP" sti0
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d9 "FSTP" sti1
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//da "FSTP" sti2
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//db "FSTP" sti3
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//dc "FSTP" sti4
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//dd "FSTP" sti5
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//de "FSTP" sti6
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//df "FSTP" sti7
      {0,C_FCHS,D__NONE,D__NONE,D__NONE,"FCHS",0xe0d9,0x280102,0x0},
      {0,C_FABS,D__NONE,D__NONE,D__NONE,"FABS",0xe1d9,0x290102,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_FTST,D__NONE,D__NONE,D__NONE,"FTST",0xe4d9,0x2c0102,0x0},
      {0,C_FXAM,D__NONE,D__NONE,D__NONE,"FXAM",0xe5d9,0x2d0102,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_FLD1,D__NONE,D__NONE,D__NONE,"FLD1",0xe8d9,0x300102,0x0},
      {0,C_FLDL2T,D__NONE,D__NONE,D__NONE,"FLDL2T",0xe9d9,0x310102,0x0},
      {0,C_FLDL2E,D__NONE,D__NONE,D__NONE,"FLDL2E",0xead9,0x320102,0x0},
      {0,C_FLDPI,D__NONE,D__NONE,D__NONE,"FLDPI",0xebd9,0x330102,0x0},
      {0,C_FLDLG2,D__NONE,D__NONE,D__NONE,"FLDLG2",0xecd9,0x340102,0x0},
      {0,C_FLDLN2,D__NONE,D__NONE,D__NONE,"FLDLN2",0xedd9,0x350102,0x0},
      {0,C_FLDZ,D__NONE,D__NONE,D__NONE,"FLDZ",0xeed9,0x360102,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_F2XM1,D__NONE,D__NONE,D__NONE,"F2XM1",0xf0d9,0x380102,0x0},
      {0,C_FYL2X,D__NONE,D__NONE,D__NONE,"FYL2X",0xf1d9,0x390102,0x0},
      {0,C_FPTAN,D__NONE,D__NONE,D__NONE,"FPTAN",0xf2d9,0x3a0102,0x0},
      {0,C_FPATAN,D__NONE,D__NONE,D__NONE,"FPATAN",0xf3d9,0x3b0102,0x0},
      {0,C_FXTRACT,D__NONE,D__NONE,D__NONE,"FXTRACT",0xf4d9,0x3c0102,0x0},
      {0,C_FPREM1,D__NONE,D__NONE,D__NONE,"FPREM1",0xf5d9,0x3d0102,0x0},
      {0,C_FDECSTP,D__NONE,D__NONE,D__NONE,"FDECSTP",0xf6d9,0x3e0102,0x0},
      {0,C_FINCSTP,D__NONE,D__NONE,D__NONE,"FINCSTP",0xf7d9,0x3f0102,0x0},
      {0,C_FPREM,D__NONE,D__NONE,D__NONE,"FPREM",0xf8d9,0x400102,0x0},
      {0,C_FYL2XP1,D__NONE,D__NONE,D__NONE,"FYL2XP1",0xf9d9,0x410102,0x0},
      {0,C_FSQRT,D__NONE,D__NONE,D__NONE,"FSQRT",0xfad9,0x420102,0x0},
      {0,C_FSINCOS,D__NONE,D__NONE,D__NONE,"FSINCOS",0xfbd9,0x430102,0x0},
      {0,C_FRNDINT,D__NONE,D__NONE,D__NONE,"FRNDINT",0xfcd9,0x440102,0x0},
      {0,C_FSCALE,D__NONE,D__NONE,D__NONE,"FSCALE",0xfdd9,0x450102,0x0},
      {0,C_FSIN,D__NONE,D__NONE,D__NONE,"FSIN",0xfed9,0x460102,0x0},
      {0,C_FCOS,D__NONE,D__NONE,D__NONE,"FCOS",0xffd9,0x470102,0x0},
};
_DIS_OPCODE_T FPGroupda[] = {
      {ATTR_MODRM,C_FIADD,D__dword_int,D__NONE,D__NONE,"FIADD",0xda,0x101,0x1},
      {ATTR_MODRM,C_FIMUL,D__dword_int,D__NONE,D__NONE,"FIMUL",0xda,0x10101,0x1},
      {ATTR_MODRM,C_FICOM,D__dword_int,D__NONE,D__NONE,"FICOM",0xda,0x20101,0x1},
      {ATTR_MODRM,C_FICOMP,D__dword_int,D__NONE,D__NONE,"FICOMP",0xda,0x30101,0x1},
      {ATTR_MODRM,C_FISUB,D__dword_int,D__NONE,D__NONE,"FISUB",0xda,0x40101,0x1},
      {ATTR_MODRM,C_FISUBR,D__dword_int,D__NONE,D__NONE,"FISUBR",0xda,0x50101,0x1},
      {ATTR_MODRM,C_FIDIV,D__dword_int,D__NONE,D__NONE,"FIDIV",0xda,0x60101,0x1},
      {ATTR_MODRM,C_FIDIVR,D__dword_int,D__NONE,D__NONE,"FIDIVR",0xda,0x70101,0x1},
      {0,C_FCMOVB,D__st,D__st0,D__NONE,"FCMOVB",0xc0da,0x80102,0x5},
      {0,C_FCMOVB,D__st,D__st1,D__NONE,"FCMOVB",0xc1da,0x90102,0x5},
      {0,C_FCMOVB,D__st,D__st2,D__NONE,"FCMOVB",0xc2da,0xa0102,0x5},
      {0,C_FCMOVB,D__st,D__st3,D__NONE,"FCMOVB",0xc3da,0xb0102,0x5},
      {0,C_FCMOVB,D__st,D__st4,D__NONE,"FCMOVB",0xc4da,0xc0102,0x5},
      {0,C_FCMOVB,D__st,D__st5,D__NONE,"FCMOVB",0xc5da,0xd0102,0x5},
      {0,C_FCMOVB,D__st,D__st6,D__NONE,"FCMOVB",0xc6da,0xe0102,0x5},
      {0,C_FCMOVB,D__st,D__st7,D__NONE,"FCMOVB",0xc7da,0xf0102,0x5},
      {0,C_FCMOVE,D__st,D__st0,D__NONE,"FCMOVE",0xc8da,0x100102,0x5},
      {0,C_FCMOVE,D__st,D__st1,D__NONE,"FCMOVE",0xc9da,0x110102,0x5},
      {0,C_FCMOVE,D__st,D__st2,D__NONE,"FCMOVE",0xcada,0x120102,0x5},
      {0,C_FCMOVE,D__st,D__st3,D__NONE,"FCMOVE",0xcbda,0x130102,0x5},
      {0,C_FCMOVE,D__st,D__st4,D__NONE,"FCMOVE",0xccda,0x140102,0x5},
      {0,C_FCMOVE,D__st,D__st5,D__NONE,"FCMOVE",0xcdda,0x150102,0x5},
      {0,C_FCMOVE,D__st,D__st6,D__NONE,"FCMOVE",0xceda,0x160102,0x5},
      {0,C_FCMOVE,D__st,D__st7,D__NONE,"FCMOVE",0xcfda,0x170102,0x5},
      {0,C_FCMOVBE,D__st,D__st0,D__NONE,"FCMOVBE",0xd0da,0x180102,0x5},
      {0,C_FCMOVBE,D__st,D__st1,D__NONE,"FCMOVBE",0xd1da,0x190102,0x5},
      {0,C_FCMOVBE,D__st,D__st2,D__NONE,"FCMOVBE",0xd2da,0x1a0102,0x5},
      {0,C_FCMOVBE,D__st,D__st3,D__NONE,"FCMOVBE",0xd3da,0x1b0102,0x5},
      {0,C_FCMOVBE,D__st,D__st4,D__NONE,"FCMOVBE",0xd4da,0x1c0102,0x5},
      {0,C_FCMOVBE,D__st,D__st5,D__NONE,"FCMOVBE",0xd5da,0x1d0102,0x5},
      {0,C_FCMOVBE,D__st,D__st6,D__NONE,"FCMOVBE",0xd6da,0x1e0102,0x5},
      {0,C_FCMOVBE,D__st,D__st7,D__NONE,"FCMOVBE",0xd7da,0x1f0102,0x5},
      {0,C_FCMOVU,D__st,D__st0,D__NONE,"FCMOVU",0xd8da,0x200102,0x5},
      {0,C_FCMOVU,D__st,D__st1,D__NONE,"FCMOVU",0xd9da,0x210102,0x5},
      {0,C_FCMOVU,D__st,D__st2,D__NONE,"FCMOVU",0xdada,0x220102,0x5},
      {0,C_FCMOVU,D__st,D__st3,D__NONE,"FCMOVU",0xdbda,0x230102,0x5},
      {0,C_FCMOVU,D__st,D__st4,D__NONE,"FCMOVU",0xdcda,0x240102,0x5},
      {0,C_FCMOVU,D__st,D__st5,D__NONE,"FCMOVU",0xddda,0x250102,0x5},
      {0,C_FCMOVU,D__st,D__st6,D__NONE,"FCMOVU",0xdeda,0x260102,0x5},
      {0,C_FCMOVU,D__st,D__st7,D__NONE,"FCMOVU",0xdfda,0x270102,0x5},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_FUCOMPP,D__NONE,D__NONE,D__NONE,"FUCOMPP",0xe9da,0x310102,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T FPGroupdb[] = {
      {ATTR_MODRM,C_FILD,D__dword_int,D__NONE,D__NONE,"FILD",0xdb,0x101,0x1},
      {ATTR_MODRM,C_FISTTP,D__dword_int,D__NONE,D__NONE,"FISTTP",0xdb,0x10101,0x2},
      {ATTR_MODRM,C_FIST,D__dword_int,D__NONE,D__NONE,"FIST",0xdb,0x20101,0x2},
      {ATTR_MODRM,C_FISTP,D__dword_int,D__NONE,D__NONE,"FISTP",0xdb,0x30101,0x2},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_FLD,D__extend_real,D__NONE,D__NONE,"FLD",0xdb,0x50101,0x1},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_FSTP,D__extend_real,D__NONE,D__NONE,"FSTP",0xdb,0x70101,0x2},
      {0,C_FCMOVNB,D__st,D__st0,D__NONE,"FCMOVNB",0xc0db,0x80102,0x4},
      {0,C_FCMOVNB,D__st,D__st1,D__NONE,"FCMOVNB",0xc1db,0x90102,0x4},
      {0,C_FCMOVNB,D__st,D__st2,D__NONE,"FCMOVNB",0xc2db,0xa0102,0x4},
      {0,C_FCMOVNB,D__st,D__st3,D__NONE,"FCMOVNB",0xc3db,0xb0102,0x4},
      {0,C_FCMOVNB,D__st,D__st4,D__NONE,"FCMOVNB",0xc4db,0xc0102,0x4},
      {0,C_FCMOVNB,D__st,D__st5,D__NONE,"FCMOVNB",0xc5db,0xd0102,0x4},
      {0,C_FCMOVNB,D__st,D__st6,D__NONE,"FCMOVNB",0xc6db,0xe0102,0x4},
      {0,C_FCMOVNB,D__st,D__st7,D__NONE,"FCMOVNB",0xc7db,0xf0102,0x4},
      {0,C_FCMOVNE,D__st,D__st0,D__NONE,"FCMOVNE",0xc8db,0x100102,0x4},
      {0,C_FCMOVNE,D__st,D__st1,D__NONE,"FCMOVNE",0xc9db,0x110102,0x4},
      {0,C_FCMOVNE,D__st,D__st2,D__NONE,"FCMOVNE",0xcadb,0x120102,0x4},
      {0,C_FCMOVNE,D__st,D__st3,D__NONE,"FCMOVNE",0xcbdb,0x130102,0x4},
      {0,C_FCMOVNE,D__st,D__st4,D__NONE,"FCMOVNE",0xccdb,0x140102,0x4},
      {0,C_FCMOVNE,D__st,D__st5,D__NONE,"FCMOVNE",0xcddb,0x150102,0x4},
      {0,C_FCMOVNE,D__st,D__st6,D__NONE,"FCMOVNE",0xcedb,0x160102,0x4},
      {0,C_FCMOVNE,D__st,D__st7,D__NONE,"FCMOVNE",0xcfdb,0x170102,0x4},
      {0,C_FCMOVNBE,D__st,D__st0,D__NONE,"FCMOVNBE",0xd0db,0x180102,0x4},
      {0,C_FCMOVNBE,D__st,D__st1,D__NONE,"FCMOVNBE",0xd1db,0x190102,0x4},
      {0,C_FCMOVNBE,D__st,D__st2,D__NONE,"FCMOVNBE",0xd2db,0x1a0102,0x4},
      {0,C_FCMOVNBE,D__st,D__st3,D__NONE,"FCMOVNBE",0xd3db,0x1b0102,0x4},
      {0,C_FCMOVNBE,D__st,D__st4,D__NONE,"FCMOVNBE",0xd4db,0x1c0102,0x4},
      {0,C_FCMOVNBE,D__st,D__st5,D__NONE,"FCMOVNBE",0xd5db,0x1d0102,0x4},
      {0,C_FCMOVNBE,D__st,D__st6,D__NONE,"FCMOVNBE",0xd6db,0x1e0102,0x4},
      {0,C_FCMOVNBE,D__st,D__st7,D__NONE,"FCMOVNBE",0xd7db,0x1f0102,0x4},
      {0,C_FCMOVNU,D__st,D__st0,D__NONE,"FCMOVNU",0xd8db,0x200102,0x4},
      {0,C_FCMOVNU,D__st,D__st1,D__NONE,"FCMOVNU",0xd9db,0x210102,0x4},
      {0,C_FCMOVNU,D__st,D__st2,D__NONE,"FCMOVNU",0xdadb,0x220102,0x4},
      {0,C_FCMOVNU,D__st,D__st3,D__NONE,"FCMOVNU",0xdbdb,0x230102,0x4},
      {0,C_FCMOVNU,D__st,D__st4,D__NONE,"FCMOVNU",0xdcdb,0x240102,0x4},
      {0,C_FCMOVNU,D__st,D__st5,D__NONE,"FCMOVNU",0xdddb,0x250102,0x4},
      {0,C_FCMOVNU,D__st,D__st6,D__NONE,"FCMOVNU",0xdedb,0x260102,0x4},
      {0,C_FCMOVNU,D__st,D__st7,D__NONE,"FCMOVNU",0xdfdb,0x270102,0x4},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_FCLEX,D__NONE,D__NONE,D__NONE,"FCLEX",0xe2db,0x2a0102,0x0},
      {0,C_FINIT,D__NONE,D__NONE,D__NONE,"FINIT",0xe3db,0x2b0102,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_FUCOMI,D__st,D__st0,D__NONE,"FUCOMI",0xe8db,0x300102,0x5},
      {0,C_FUCOMI,D__st,D__st1,D__NONE,"FUCOMI",0xe9db,0x310102,0x5},
      {0,C_FUCOMI,D__st,D__st2,D__NONE,"FUCOMI",0xeadb,0x320102,0x5},
      {0,C_FUCOMI,D__st,D__st3,D__NONE,"FUCOMI",0xebdb,0x330102,0x5},
      {0,C_FUCOMI,D__st,D__st4,D__NONE,"FUCOMI",0xecdb,0x340102,0x5},
      {0,C_FUCOMI,D__st,D__st5,D__NONE,"FUCOMI",0xeddb,0x350102,0x5},
      {0,C_FUCOMI,D__st,D__st6,D__NONE,"FUCOMI",0xeedb,0x360102,0x5},
      {0,C_FUCOMI,D__st,D__st7,D__NONE,"FUCOMI",0xefdb,0x370102,0x5},
      {0,C_FCOMI,D__st,D__st0,D__NONE,"FCOMI",0xf0db,0x380102,0x5},
      {0,C_FCOMI,D__st,D__st1,D__NONE,"FCOMI",0xf1db,0x390102,0x5},
      {0,C_FCOMI,D__st,D__st2,D__NONE,"FCOMI",0xf2db,0x3a0102,0x5},
      {0,C_FCOMI,D__st,D__st3,D__NONE,"FCOMI",0xf3db,0x3b0102,0x5},
      {0,C_FCOMI,D__st,D__st4,D__NONE,"FCOMI",0xf4db,0x3c0102,0x5},
      {0,C_FCOMI,D__st,D__st5,D__NONE,"FCOMI",0xf5db,0x3d0102,0x5},
      {0,C_FCOMI,D__st,D__st6,D__NONE,"FCOMI",0xf6db,0x3e0102,0x5},
      {0,C_FCOMI,D__st,D__st7,D__NONE,"FCOMI",0xf7db,0x3f0102,0x5},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T FPGroupdc[] = {
      {ATTR_MODRM,C_FADD,D__double_real,D__NONE,D__NONE,"FADD",0xdc,0x101,0x1},
      {ATTR_MODRM,C_FMUL,D__double_real,D__NONE,D__NONE,"FMUL",0xdc,0x10101,0x1},
      {ATTR_MODRM,C_FCOM,D__double_real,D__NONE,D__NONE,"FCOM",0xdc,0x20101,0x1},
      {ATTR_MODRM,C_FCOMP,D__double_real,D__NONE,D__NONE,"FCOMP",0xdc,0x30101,0x1},
      {ATTR_MODRM,C_FSUB,D__double_real,D__NONE,D__NONE,"FSUB",0xdc,0x40101,0x1},
      {ATTR_MODRM,C_FSUBR,D__double_real,D__NONE,D__NONE,"FSUBR",0xdc,0x50101,0x1},
      {ATTR_MODRM,C_FDIV,D__double_real,D__NONE,D__NONE,"FDIV",0xdc,0x60101,0x1},
      {ATTR_MODRM,C_FDIVR,D__double_real,D__NONE,D__NONE,"FDIVR",0xdc,0x70101,0x1},
      {0,C_FADD,D__st0,D__st,D__NONE,"FADD",0xc0dc,0x80102,0x7},
      {0,C_FADD,D__st1,D__st,D__NONE,"FADD",0xc1dc,0x90102,0x7},
      {0,C_FADD,D__st2,D__st,D__NONE,"FADD",0xc2dc,0xa0102,0x7},
      {0,C_FADD,D__st3,D__st,D__NONE,"FADD",0xc3dc,0xb0102,0x7},
      {0,C_FADD,D__st4,D__st,D__NONE,"FADD",0xc4dc,0xc0102,0x7},
      {0,C_FADD,D__st5,D__st,D__NONE,"FADD",0xc5dc,0xd0102,0x7},
      {0,C_FADD,D__st6,D__st,D__NONE,"FADD",0xc6dc,0xe0102,0x7},
      {0,C_FADD,D__st7,D__st,D__NONE,"FADD",0xc7dc,0xf0102,0x7},
      {0,C_FMUL,D__st0,D__st,D__NONE,"FMUL",0xc8dc,0x100102,0x7},
      {0,C_FMUL,D__st1,D__st,D__NONE,"FMUL",0xc9dc,0x110102,0x7},
      {0,C_FMUL,D__st2,D__st,D__NONE,"FMUL",0xcadc,0x120102,0x7},
      {0,C_FMUL,D__st3,D__st,D__NONE,"FMUL",0xcbdc,0x130102,0x7},
      {0,C_FMUL,D__st4,D__st,D__NONE,"FMUL",0xccdc,0x140102,0x7},
      {0,C_FMUL,D__st5,D__st,D__NONE,"FMUL",0xcddc,0x150102,0x7},
      {0,C_FMUL,D__st6,D__st,D__NONE,"FMUL",0xcedc,0x160102,0x7},
      {0,C_FMUL,D__st7,D__st,D__NONE,"FMUL",0xcfdc,0x170102,0x7},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d0 FCOM st sti0
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d1 FCOM st sti1
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d2 FCOM st sti2
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d3 FCOM st sti3
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d4 FCOM st sti4
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d5 FCOM st sti5
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d6 FCOM st sti6
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d7 FCOM st sti7
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d8 FCOMP st sti0
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d9 FCOMP st sti1
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//da FCOMP st sti2
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//db FCOMP st sti3
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//dc FCOMP st sti4
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//dd FCOMP st sti5
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//de FCOMP st sti6
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//df FCOMP st sti7
      {0,C_FSUBR,D__st0,D__st,D__NONE,"FSUBR",0xe0dc,0x280102,0x7},
      {0,C_FSUBR,D__st1,D__st,D__NONE,"FSUBR",0xe1dc,0x290102,0x7},
      {0,C_FSUBR,D__st2,D__st,D__NONE,"FSUBR",0xe2dc,0x2a0102,0x7},
      {0,C_FSUBR,D__st3,D__st,D__NONE,"FSUBR",0xe3dc,0x2b0102,0x7},
      {0,C_FSUBR,D__st4,D__st,D__NONE,"FSUBR",0xe4dc,0x2c0102,0x7},
      {0,C_FSUBR,D__st5,D__st,D__NONE,"FSUBR",0xe5dc,0x2d0102,0x7},
      {0,C_FSUBR,D__st6,D__st,D__NONE,"FSUBR",0xe6dc,0x2e0102,0x7},
      {0,C_FSUBR,D__st7,D__st,D__NONE,"FSUBR",0xe7dc,0x2f0102,0x7},
      {0,C_FSUB,D__st0,D__st,D__NONE,"FSUB",0xe8dc,0x300102,0x7},
      {0,C_FSUB,D__st1,D__st,D__NONE,"FSUB",0xe9dc,0x310102,0x7},
      {0,C_FSUB,D__st2,D__st,D__NONE,"FSUB",0xeadc,0x320102,0x7},
      {0,C_FSUB,D__st3,D__st,D__NONE,"FSUB",0xebdc,0x330102,0x7},
      {0,C_FSUB,D__st4,D__st,D__NONE,"FSUB",0xecdc,0x340102,0x7},
      {0,C_FSUB,D__st5,D__st,D__NONE,"FSUB",0xeddc,0x350102,0x7},
      {0,C_FSUB,D__st6,D__st,D__NONE,"FSUB",0xeedc,0x360102,0x7},
      {0,C_FSUB,D__st7,D__st,D__NONE,"FSUB",0xefdc,0x370102,0x7},
      {0,C_FDIVR,D__st0,D__st,D__NONE,"FDIVR",0xf0dc,0x380102,0x7},
      {0,C_FDIVR,D__st1,D__st,D__NONE,"FDIVR",0xf1dc,0x390102,0x7},
      {0,C_FDIVR,D__st2,D__st,D__NONE,"FDIVR",0xf2dc,0x3a0102,0x7},
      {0,C_FDIVR,D__st3,D__st,D__NONE,"FDIVR",0xf3dc,0x3b0102,0x7},
      {0,C_FDIVR,D__st4,D__st,D__NONE,"FDIVR",0xf4dc,0x3c0102,0x7},
      {0,C_FDIVR,D__st5,D__st,D__NONE,"FDIVR",0xf5dc,0x3d0102,0x7},
      {0,C_FDIVR,D__st6,D__st,D__NONE,"FDIVR",0xf6dc,0x3e0102,0x7},
      {0,C_FDIVR,D__st7,D__st,D__NONE,"FDIVR",0xf7dc,0x3f0102,0x7},
      {0,C_FDIV,D__st0,D__st,D__NONE,"FDIV",0xf8dc,0x400102,0x7},
      {0,C_FDIV,D__st1,D__st,D__NONE,"FDIV",0xf9dc,0x410102,0x7},
      {0,C_FDIV,D__st2,D__st,D__NONE,"FDIV",0xfadc,0x420102,0x7},
      {0,C_FDIV,D__st3,D__st,D__NONE,"FDIV",0xfbdc,0x430102,0x7},
      {0,C_FDIV,D__st4,D__st,D__NONE,"FDIV",0xfcdc,0x440102,0x7},
      {0,C_FDIV,D__st5,D__st,D__NONE,"FDIV",0xfddc,0x450102,0x7},
      {0,C_FDIV,D__st6,D__st,D__NONE,"FDIV",0xfedc,0x460102,0x7},
      {0,C_FDIV,D__st7,D__st,D__NONE,"FDIV",0xffdc,0x470102,0x7},
};
_DIS_OPCODE_T FPGroupdd[] = {
      {ATTR_MODRM,C_FLD,D__double_real,D__NONE,D__NONE,"FLD",0xdd,0x101,0x1},
      {ATTR_MODRM,C_FISTTP,D__double_real,D__NONE,D__NONE,"FISTTP",0xdd,0x10101,0x2},
      {ATTR_MODRM,C_FST,D__double_real,D__NONE,D__NONE,"FST",0xdd,0x20101,0x2},
      {ATTR_MODRM,C_FSTP,D__double_real,D__NONE,D__NONE,"FSTP",0xdd,0x30101,0x2},
      {ATTR_MODRM,C_FRSTOR,D__98_108b,D__NONE,D__NONE,"FRSTOR",0xdd,0x40101,0x1},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_FSAVE,D__98_108b,D__NONE,D__NONE,"FSAVE",0xdd,0x60101,0x2},
      {ATTR_MODRM,C_FSTSW,D__2b,D__NONE,D__NONE,"FSTSW",0xdd,0x70101,0x2},
      {0,C_FFREE,D__st0,D__NONE,D__NONE,"FFREE",0xc0dd,0x80102,0x0},
      {0,C_FFREE,D__st1,D__NONE,D__NONE,"FFREE",0xc1dd,0x90102,0x0},
      {0,C_FFREE,D__st2,D__NONE,D__NONE,"FFREE",0xc2dd,0xa0102,0x0},
      {0,C_FFREE,D__st3,D__NONE,D__NONE,"FFREE",0xc3dd,0xb0102,0x0},
      {0,C_FFREE,D__st4,D__NONE,D__NONE,"FFREE",0xc4dd,0xc0102,0x0},
      {0,C_FFREE,D__st5,D__NONE,D__NONE,"FFREE",0xc5dd,0xd0102,0x0},
      {0,C_FFREE,D__st6,D__NONE,D__NONE,"FFREE",0xc6dd,0xe0102,0x0},
      {0,C_FFREE,D__st7,D__NONE,D__NONE,"FFREE",0xc7dd,0xf0102,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//c8 FXCH st st0
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//c9 FXCH st st1
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//ca FXCH st st2
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//cb FXCH st st3
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//cc FXCH st st4
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//cd FXCH st st5
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//ce FXCH st st6
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//cf FXCH st st7
      {0,C_FST,D__st0,D__NONE,D__NONE,"FST",0xd0dd,0x180102,0x1},
      {0,C_FST,D__st1,D__NONE,D__NONE,"FST",0xd1dd,0x190102,0x1},
      {0,C_FST,D__st2,D__NONE,D__NONE,"FST",0xd2dd,0x1a0102,0x1},
      {0,C_FST,D__st3,D__NONE,D__NONE,"FST",0xd3dd,0x1b0102,0x1},
      {0,C_FST,D__st4,D__NONE,D__NONE,"FST",0xd4dd,0x1c0102,0x1},
      {0,C_FST,D__st5,D__NONE,D__NONE,"FST",0xd5dd,0x1d0102,0x1},
      {0,C_FST,D__st6,D__NONE,D__NONE,"FST",0xd6dd,0x1e0102,0x1},
      {0,C_FST,D__st7,D__NONE,D__NONE,"FST",0xd7dd,0x1f0102,0x1},
      {0,C_FSTP,D__st0,D__NONE,D__NONE,"FSTP",0xd8dd,0x200102,0x1},
      {0,C_FSTP,D__st1,D__NONE,D__NONE,"FSTP",0xd9dd,0x210102,0x1},
      {0,C_FSTP,D__st2,D__NONE,D__NONE,"FSTP",0xdadd,0x220102,0x1},
      {0,C_FSTP,D__st3,D__NONE,D__NONE,"FSTP",0xdbdd,0x230102,0x1},
      {0,C_FSTP,D__st4,D__NONE,D__NONE,"FSTP",0xdcdd,0x240102,0x1},
      {0,C_FSTP,D__st5,D__NONE,D__NONE,"FSTP",0xdddd,0x250102,0x1},
      {0,C_FSTP,D__st6,D__NONE,D__NONE,"FSTP",0xdedd,0x260102,0x1},
      {0,C_FSTP,D__st7,D__NONE,D__NONE,"FSTP",0xdfdd,0x270102,0x1},
      {0,C_FUCOM,D__st0,D__st,D__NONE,"FUCOM",0xe0dd,0x280102,0x5},
      {0,C_FUCOM,D__st1,D__st,D__NONE,"FUCOM",0xe1dd,0x290102,0x5},
      {0,C_FUCOM,D__st2,D__st,D__NONE,"FUCOM",0xe2dd,0x2a0102,0x5},
      {0,C_FUCOM,D__st3,D__st,D__NONE,"FUCOM",0xe3dd,0x2b0102,0x5},
      {0,C_FUCOM,D__st4,D__st,D__NONE,"FUCOM",0xe4dd,0x2c0102,0x5},
      {0,C_FUCOM,D__st5,D__st,D__NONE,"FUCOM",0xe5dd,0x2d0102,0x5},
      {0,C_FUCOM,D__st6,D__st,D__NONE,"FUCOM",0xe6dd,0x2e0102,0x5},
      {0,C_FUCOM,D__st7,D__st,D__NONE,"FUCOM",0xe7dd,0x2f0102,0x5},
      {0,C_FUCOMP,D__st0,D__NONE,D__NONE,"FUCOMP",0xe8dd,0x300102,0x1},
      {0,C_FUCOMP,D__st1,D__NONE,D__NONE,"FUCOMP",0xe9dd,0x310102,0x1},
      {0,C_FUCOMP,D__st2,D__NONE,D__NONE,"FUCOMP",0xeadd,0x320102,0x1},
      {0,C_FUCOMP,D__st3,D__NONE,D__NONE,"FUCOMP",0xebdd,0x330102,0x1},
      {0,C_FUCOMP,D__st4,D__NONE,D__NONE,"FUCOMP",0xecdd,0x340102,0x1},
      {0,C_FUCOMP,D__st5,D__NONE,D__NONE,"FUCOMP",0xeddd,0x350102,0x1},
      {0,C_FUCOMP,D__st6,D__NONE,D__NONE,"FUCOMP",0xeedd,0x360102,0x1},
      {0,C_FUCOMP,D__st7,D__NONE,D__NONE,"FUCOMP",0xefdd,0x370102,0x1},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T FPGroupde[] = {
      {ATTR_MODRM,C_FIADD,D__word_int,D__NONE,D__NONE,"FIADD",0xde,0x101,0x7},
      {ATTR_MODRM,C_FIMUL,D__word_int,D__NONE,D__NONE,"FIMUL",0xde,0x10101,0x7},
      {ATTR_MODRM,C_FICOM,D__word_int,D__NONE,D__NONE,"FICOM",0xde,0x20101,0x5},
      {ATTR_MODRM,C_FICOMP,D__word_int,D__NONE,D__NONE,"FICOMP",0xde,0x30101,0x5},
      {ATTR_MODRM,C_FISUB,D__word_int,D__NONE,D__NONE,"FISUB",0xde,0x40101,0x7},
      {ATTR_MODRM,C_FISUBR,D__word_int,D__NONE,D__NONE,"FISUBR",0xde,0x50101,0x7},
      {ATTR_MODRM,C_FIDIV,D__word_int,D__NONE,D__NONE,"FIDIV",0xde,0x60101,0x7},
      {ATTR_MODRM,C_FIDIVR,D__word_int,D__NONE,D__NONE,"FIDIVR",0xde,0x70101,0x7},
      {0,C_FADDP,D__st0,D__st,D__NONE,"FADDP",0xc0de,0x80102,0x7},
      {0,C_FADDP,D__st1,D__st,D__NONE,"FADDP",0xc1de,0x90102,0x7},
      {0,C_FADDP,D__st2,D__st,D__NONE,"FADDP",0xc2de,0xa0102,0x7},
      {0,C_FADDP,D__st3,D__st,D__NONE,"FADDP",0xc3de,0xb0102,0x7},
      {0,C_FADDP,D__st4,D__st,D__NONE,"FADDP",0xc4de,0xc0102,0x7},
      {0,C_FADDP,D__st5,D__st,D__NONE,"FADDP",0xc5de,0xd0102,0x7},
      {0,C_FADDP,D__st6,D__st,D__NONE,"FADDP",0xc6de,0xe0102,0x7},
      {0,C_FADDP,D__st7,D__st,D__NONE,"FADDP",0xc7de,0xf0102,0x7},
      {0,C_FMULP,D__st0,D__st,D__NONE,"FMULP",0xc8de,0x100102,0x7},
      {0,C_FMULP,D__st1,D__st,D__NONE,"FMULP",0xc9de,0x110102,0x7},
      {0,C_FMULP,D__st2,D__st,D__NONE,"FMULP",0xcade,0x120102,0x7},
      {0,C_FMULP,D__st3,D__st,D__NONE,"FMULP",0xcbde,0x130102,0x7},
      {0,C_FMULP,D__st4,D__st,D__NONE,"FMULP",0xccde,0x140102,0x7},
      {0,C_FMULP,D__st5,D__st,D__NONE,"FMULP",0xcdde,0x150102,0x7},
      {0,C_FMULP,D__st6,D__st,D__NONE,"FMULP",0xcede,0x160102,0x7},
      {0,C_FMULP,D__st7,D__st,D__NONE,"FMULP",0xcfde,0x170102,0x7},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d0 FCOMP sti0
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d1 FCOMP sti1
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d2 FCOMP sti2
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d3 FCOMP sti3
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d4 FCOMP sti4
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d5 FCOMP sti5
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d6 FCOMP sti6
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d7 FCOMP sti7
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_FCOMPP,D__NONE,D__NONE,D__NONE,"FCOMPP",0xd9de,0x210102,0x5},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_FSUBRP,D__st0,D__st,D__NONE,"FSUBRP",0xe0de,0x280102,0x7},
      {0,C_FSUBRP,D__st1,D__st,D__NONE,"FSUBRP",0xe1de,0x290102,0x7},
      {0,C_FSUBRP,D__st2,D__st,D__NONE,"FSUBRP",0xe2de,0x2a0102,0x7},
      {0,C_FSUBRP,D__st3,D__st,D__NONE,"FSUBRP",0xe3de,0x2b0102,0x7},
      {0,C_FSUBRP,D__st4,D__st,D__NONE,"FSUBRP",0xe4de,0x2c0102,0x7},
      {0,C_FSUBRP,D__st5,D__st,D__NONE,"FSUBRP",0xe5de,0x2d0102,0x7},
      {0,C_FSUBRP,D__st6,D__st,D__NONE,"FSUBRP",0xe6de,0x2e0102,0x7},
      {0,C_FSUBRP,D__st7,D__st,D__NONE,"FSUBRP",0xe7de,0x2f0102,0x7},
      {0,C_FSUBP,D__st0,D__st,D__NONE,"FSUBP",0xe8de,0x300102,0x7},
      {0,C_FSUBP,D__st1,D__st,D__NONE,"FSUBP",0xe9de,0x310102,0x7},
      {0,C_FSUBP,D__st2,D__st,D__NONE,"FSUBP",0xeade,0x320102,0x7},
      {0,C_FSUBP,D__st3,D__st,D__NONE,"FSUBP",0xebde,0x330102,0x7},
      {0,C_FSUBP,D__st4,D__st,D__NONE,"FSUBP",0xecde,0x340102,0x7},
      {0,C_FSUBP,D__st5,D__st,D__NONE,"FSUBP",0xedde,0x350102,0x7},
      {0,C_FSUBP,D__st6,D__st,D__NONE,"FSUBP",0xeede,0x360102,0x7},
      {0,C_FSUBP,D__st7,D__st,D__NONE,"FSUBP",0xefde,0x370102,0x7},
      {0,C_FDIVRP,D__st0,D__st,D__NONE,"FDIVRP",0xf0de,0x380102,0x7},
      {0,C_FDIVRP,D__st1,D__st,D__NONE,"FDIVRP",0xf1de,0x390102,0x7},
      {0,C_FDIVRP,D__st2,D__st,D__NONE,"FDIVRP",0xf2de,0x3a0102,0x7},
      {0,C_FDIVRP,D__st3,D__st,D__NONE,"FDIVRP",0xf3de,0x3b0102,0x7},
      {0,C_FDIVRP,D__st4,D__st,D__NONE,"FDIVRP",0xf4de,0x3c0102,0x7},
      {0,C_FDIVRP,D__st5,D__st,D__NONE,"FDIVRP",0xf5de,0x3d0102,0x7},
      {0,C_FDIVRP,D__st6,D__st,D__NONE,"FDIVRP",0xf6de,0x3e0102,0x7},
      {0,C_FDIVRP,D__st7,D__st,D__NONE,"FDIVRP",0xf7de,0x3f0102,0x7},
      {0,C_FDIVP,D__st0,D__st,D__NONE,"FDIVP",0xf8de,0x400102,0x7},
      {0,C_FDIVP,D__st1,D__st,D__NONE,"FDIVP",0xf9de,0x410102,0x7},
      {0,C_FDIVP,D__st2,D__st,D__NONE,"FDIVP",0xfade,0x420102,0x7},
      {0,C_FDIVP,D__st3,D__st,D__NONE,"FDIVP",0xfbde,0x430102,0x7},
      {0,C_FDIVP,D__st4,D__st,D__NONE,"FDIVP",0xfcde,0x440102,0x7},
      {0,C_FDIVP,D__st5,D__st,D__NONE,"FDIVP",0xfdde,0x450102,0x7},
      {0,C_FDIVP,D__st6,D__st,D__NONE,"FDIVP",0xfede,0x460102,0x7},
      {0,C_FDIVP,D__st7,D__st,D__NONE,"FDIVP",0xffde,0x470102,0x7},
};
_DIS_OPCODE_T FPGroupdf[] = {
      {ATTR_MODRM,C_FILD,D__word_int,D__NONE,D__NONE,"FILD",0xdf,0x101,0x1},//7
      {ATTR_MODRM,C_FISTTP,D__word_int,D__NONE,D__NONE,"FISTTP",0xdf,0x10101,0x1},//8
      {ATTR_MODRM,C_FIST,D__word_int,D__NONE,D__NONE,"FIST",0xdf,0x20101,0x2},//9
      {ATTR_MODRM,C_FISTP,D__word_int,D__NONE,D__NONE,"FISTP",0xdf,0x30101,0x2},//a
      {ATTR_MODRM,C_FBLD,D__packed_BCD,D__NONE,D__NONE,"FBLD",0xdf,0x40101,0x1},//b
      {ATTR_MODRM,C_FILD,D__qword_int,D__NONE,D__NONE,"FILD",0xdf,0x50101,0x1},//c
      {ATTR_MODRM,C_FBSTP,D__packed_BCD,D__NONE,D__NONE,"FBSTP",0xdf,0x60101,0x2},//d
      {ATTR_MODRM,C_FISTP,D__qword_int,D__NONE,D__NONE,"FISTP",0xdf,0x70101,0x2},//e
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//bf
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//c0
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//c1
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//c2
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//c3
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//c4
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//c5
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//c7
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//c8 FXCH
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//c9 FXCH
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//ca FXCH
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//cb FXCH
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//cc FXCH
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//cd FXCH
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//ce FXCH
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//cf FXCH
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d0 FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d1 FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d2 FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d3 FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d4 FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d5 FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d6 FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d7 FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d8 FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//d9 FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//da FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//db FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//dc FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//dd FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//de FSTP sti
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},//df FSTP sti
      {0,C_FSTSW,D__AX,D__NONE,D__NONE,"FSTSW",0xe0df,0x280102,0x2},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_FUCOMIP,D__st0,D__st,D__NONE,"FUCOMIP",0xe8df,0x300102,0x5},
      {0,C_FUCOMIP,D__st1,D__st,D__NONE,"FUCOMIP",0xe9df,0x310102,0x5},
      {0,C_FUCOMIP,D__st2,D__st,D__NONE,"FUCOMIP",0xeadf,0x320102,0x5},
      {0,C_FUCOMIP,D__st3,D__st,D__NONE,"FUCOMIP",0xebdf,0x330102,0x5},
      {0,C_FUCOMIP,D__st4,D__st,D__NONE,"FUCOMIP",0xecdf,0x340102,0x5},
      {0,C_FUCOMIP,D__st5,D__st,D__NONE,"FUCOMIP",0xeddf,0x350102,0x5},
      {0,C_FUCOMIP,D__st6,D__st,D__NONE,"FUCOMIP",0xeedf,0x360102,0x5},
      {0,C_FUCOMIP,D__st7,D__st,D__NONE,"FUCOMIP",0xefdf,0x370102,0x5},
      {0,C_FCOMIP,D__st0,D__st,D__NONE,"FCOMIP",0xf0df,0x380102,0x5},
      {0,C_FCOMIP,D__st1,D__st,D__NONE,"FCOMIP",0xf1df,0x390102,0x5},
      {0,C_FCOMIP,D__st2,D__st,D__NONE,"FCOMIP",0xf2df,0x3a0102,0x5},
      {0,C_FCOMIP,D__st3,D__st,D__NONE,"FCOMIP",0xf3df,0x3b0102,0x5},
      {0,C_FCOMIP,D__st4,D__st,D__NONE,"FCOMIP",0xf4df,0x3c0102,0x5},
      {0,C_FCOMIP,D__st5,D__st,D__NONE,"FCOMIP",0xf5df,0x3d0102,0x5},
      {0,C_FCOMIP,D__st6,D__st,D__NONE,"FCOMIP",0xf6df,0x3e0102,0x5},
      {0,C_FCOMIP,D__st7,D__st,D__NONE,"FCOMIP",0xf7df,0x3f0102,0x5},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupG7M[] = {
      {ATTR_MODRM,C_SGDT,D_wMs,D__NONE,D__NONE,"SGDT",0x10f,0x802,0x2},
      {ATTR_MODRM,C_SIDT,D_wMs,D__NONE,D__NONE,"SIDT",0x10f,0x10802,0x2},
      {ATTR_MODRM,C_LGDT,D_rMs,D__NONE,D__NONE,"LGDT",0x10f,0x20802,0x1},
      {ATTR_MODRM,C_LIDT,D_rMs,D__NONE,D__NONE,"LIDT",0x10f,0x30802,0x1},
      {ATTR_MODRM,C_SMSW,D_wEx,D__NONE,D__NONE,"SMSW",0x10f,0x40802,0x2},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_LMSW,D_rEw,D__NONE,D__NONE,"LMSW",0x10f,0x60802,0x1},
      {ATTR_MODRM,C_INVLPG,D__Mb,D__NONE,D__NONE,"INVLPG",0x10f,0x70802,0x1},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_VMCALL,D__NONE,D__NONE,D__NONE,"VMCALL",0x10f,0x90802,0x0},
      {ATTR_MODRM,C_VMLAUNCH,D__NONE,D__NONE,D__NONE,"VMLAUNCH",0x10f,0xa0802,0x0},
      {ATTR_MODRM,C_VMRESUME,D__NONE,D__NONE,D__NONE,"VMRESUME",0x10f,0xb0802,0x0},
      {ATTR_MODRM,C_VMXOFF,D__NONE,D__NONE,D__NONE,"VMXOFF",0x10f,0xc0802,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_MONITOR,D__NONE,D__NONE,D__NONE,"monitor",0x10f,0x100802,0x0},
      {ATTR_MODRM,C_MWAIT,D__NONE,D__NONE,D__NONE,"mwait",0x10f,0x110802,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_XGETBV,D__NONE,D__NONE,D__NONE,"xgetbv",0x10f,0x180802,0x0},
      {ATTR_MODRM,C_XSETBV,D__NONE,D__NONE,D__NONE,"xsetbv",0x10f,0x190802,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_VMRUN,D__EAX,D__NONE,D__NONE,"VMRUN",0x10f,0x200802,0x1},
      {ATTR_MODRM,C_VMMCALL,D__NONE,D__NONE,D__NONE,"VMMCALL",0x10f,0x210802,0x0},
      {ATTR_MODRM,C_VMLOAD,D__EAX,D__NONE,D__NONE,"VMLOAD",0x10f,0x220802,0x1},
      {ATTR_MODRM,C_VMSAVE,D__EAX,D__NONE,D__NONE,"VMSAVE",0x10f,0x230802,0x2},
      {ATTR_MODRM,C_STGI,D__NONE,D__NONE,D__NONE,"STGI",0x10f,0x240802,0x0},
      {ATTR_MODRM,C_CLGI,D__NONE,D__NONE,D__NONE,"CLGI",0x10f,0x250802,0x0},
      {ATTR_MODRM,C_SKINIT,D__EAX,D__NONE,D__NONE,"SKINIT",0x10f,0x260802,0x0},
      {ATTR_MODRM,C_INVLPGA,D__EAX,D__ECX,D__NONE,"INVLPGA",0x10f,0x270802,0x0},
      {ATTR_MODRM,C_SMSW,D_wEx,D__NONE,D__NONE,"SMSW",0x10f,0x280802,0x2},
      {ATTR_MODRM,C_SMSW,D_wEx,D__NONE,D__NONE,"SMSW",0x10f,0x290802,0x2},
      {ATTR_MODRM,C_SMSW,D_wEx,D__NONE,D__NONE,"SMSW",0x10f,0x2a0802,0x2},
      {ATTR_MODRM,C_SMSW,D_wEx,D__NONE,D__NONE,"SMSW",0x10f,0x2b0802,0x2},
      {ATTR_MODRM,C_SMSW,D_wEx,D__NONE,D__NONE,"SMSW",0x10f,0x2c0802,0x2},
      {ATTR_MODRM,C_SMSW,D_wEx,D__NONE,D__NONE,"SMSW",0x10f,0x2d0802,0x2},
      {ATTR_MODRM,C_SMSW,D_wEx,D__NONE,D__NONE,"SMSW",0x10f,0x2e0802,0x2},
      {ATTR_MODRM,C_SMSW,D_wEx,D__NONE,D__NONE,"SMSW",0x10f,0x2f0802,0x2},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_LMSW,D_rEw,D__NONE,D__NONE,"LMSW",0x10f,0x380802,0x1},
      {ATTR_MODRM,C_LMSW,D_rEw,D__NONE,D__NONE,"LMSW",0x10f,0x390802,0x1},
      {ATTR_MODRM,C_LMSW,D_rEw,D__NONE,D__NONE,"LMSW",0x10f,0x3a0802,0x1},
      {ATTR_MODRM,C_LMSW,D_rEw,D__NONE,D__NONE,"LMSW",0x10f,0x3b0802,0x1},
      {ATTR_MODRM,C_LMSW,D_rEw,D__NONE,D__NONE,"LMSW",0x10f,0x3c0802,0x1},
      {ATTR_MODRM,C_LMSW,D_rEw,D__NONE,D__NONE,"LMSW",0x10f,0x3d0802,0x1},
      {ATTR_MODRM,C_LMSW,D_rEw,D__NONE,D__NONE,"LMSW",0x10f,0x3e0802,0x1},
      {ATTR_MODRM,C_LMSW,D_rEw,D__NONE,D__NONE,"LMSW",0x10f,0x3f0802,0x1},
      {ATTR_MODRM,C_SWAPGS,D__NONE,D__NONE,D__NONE,"SWAPGS",0x10f,0x400802,0x0},
      {ATTR_MODRM,C_RDTSCP,D__NONE,D__NONE,D__NONE,"RDTSCP",0x10f,0x410802,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupG16[] = {
      {ATTR_MODRM,C_PREFETCHNTA,D__Mb,D__NONE,D__NONE,"prefetchnta",0x180f,0x202,0x1},
      {ATTR_MODRM,C_PREFETCHNTA,D__Mb,D__NONE,D__NONE,"prefetcht0",0x180f,0x10202,0x1},
      {ATTR_MODRM,C_PREFETCHNTA,D__Mb,D__NONE,D__NONE,"prefetcht1",0x180f,0x20202,0x1},
      {ATTR_MODRM,C_PREFETCHNTA,D__Mb,D__NONE,D__NONE,"prefetcht2",0x180f,0x30202,0x1},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_G1202[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSRLW,D__Nq,D__Ib,D__NONE,"psrlw",0x710f,0x24002,0x17},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSRLW,D__Udq,D__Ib,D__NONE,"psrlw",0x710f66,0x24003,0x17},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_G1204[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSRAW,D__Nq,D__Ib,D__NONE,"psraw",0x710f,0x44002,0x17},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSRAW,D__Udq,D__Ib,D__NONE,"psraw",0x710f66,0x44003,0x17},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_G1206[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSLLW,D__Nq,D__Ib,D__NONE,"psllw",0x710f,0x64002,0x17},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSLLW,D__Udq,D__Ib,D__NONE,"psllw",0x710f66,0x64003,0x17},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupG12[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_G1202,0x710f,0xa0202,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_G1204,0x710f,0xc0202,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_G1206,0x710f,0xe0202,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_G1302[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSRLD,D__Nq,D__Ib,D__NONE,"psrld",0x720f,0x24002,0x17},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSRLD,D__Udq,D__Ib,D__NONE,"psrld",0x720f66,0x24003,0x17},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_G1304[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSRAD,D__Nq,D__Ib,D__NONE,"psrad",0x720f,0x44002,0x17},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSRAD,D__Udq,D__Ib,D__NONE,"psrad",0x720f66,0x44003,0x17},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_G1306[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSLLD,D__Nq,D__Ib,D__NONE,"pslld",0x720f,0x64002,0x17},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSLLD,D__Udq,D__Ib,D__NONE,"pslld",0x720f66,0x64003,0x17},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupG13[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_GRPSSE,D__Pq,D__Ib,D__NONE,(const char*)GroupSSE_G1302,0x720f,0xa0402,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_GRPSSE,D__Pq,D__Ib,D__NONE,(const char*)GroupSSE_G1304,0x720f,0xc0402,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_GRPSSE,D__Pq,D__Ib,D__NONE,(const char*)GroupSSE_G1306,0x720f,0xe0402,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_G1402[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSRLQ,D__Nq,D__Ib,D__NONE,"psrlq",0x730f,0x24002,0x17},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSRLQ,D__Udq,D__Ib,D__NONE,"psrlq",0x730f66,0x24003,0x17},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_G1403[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSRLDQ,D__Udq,D__Ib,D__NONE,"psrldq",0x730f66,0x34003,0x17},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_G1406[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSLLQ,D__Nq,D__Ib,D__NONE,"psllq",0x730f,0x64002,0x17},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSLLQ,D__Udq,D__Ib,D__NONE,"psllq",0x730f66,0x64003,0x17},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_G1407[] = {
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSLLDQ,D__Udq,D__Ib,D__NONE,"pslldq",0x730f66,0x74003,0x17},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupG14[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_G1402,0x730f,0xa4002,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_G1403,0x730f,0xa4002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_G1406,0x730f,0xe4002,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_G1407,0x730f,0xe4002,0x0},
};
_DIS_OPCODE_T GroupG15[] = {
      {ATTR_MODRM,C_FXSAVE,D__M,D__NONE,D__NONE,"fxsave",0xae0f,0x402,0x1},
      {ATTR_MODRM,C_FXRSTOR,D__M,D__NONE,D__NONE,"fxrstor",0xae0f,0x10402,0x1},
      {ATTR_MODRM,C_LDMXCSR,D__Ed,D__NONE,D__NONE,"ldmxcsr",0xae0f,0x20402,0x1},
      {ATTR_MODRM,C_STMXCSR,D__Ed,D__NONE,D__NONE,"stmxcsr",0xae0f,0x30402,0x2},
      {ATTR_MODRM,C_XSAVE,D__M,D__NONE,D__NONE,"XSAVE",0xae0f,0x40402,0x2},
      {ATTR_MODRM,C_XRSTOR,D__M,D__NONE,D__NONE,"XRSTOR",0xae0f,0x50402,0x1},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_CLFLUSH,D__Eb,D__NONE,D__NONE,"clflush",0xae0f,0x70402,0x2},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_LFENCE,D__NONE,D__NONE,D__NONE,"lfence",0xae0f,0xd0402,0x0},
      {ATTR_MODRM,C_MFENCE,D__NONE,D__NONE,D__NONE,"mfence",0xae0f,0xe0402,0x0},
      {ATTR_MODRM,C_SFENCE,D__NONE,D__NONE,D__NONE,"sfence",0xae0f,0xf0402,0x0},
      {ATTR_MODRM|ATTR_JMP,C_CMPXCHG8B,D__Mxdq,D__NONE,D__NONE,"CMPXCHG8B",0xc70f,0x14002,0x3},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_G9VMX6[] = {
      {ATTR_MODRM,C_VMPTRLD,D__Mq,D__NONE,D__NONE,"VMPTRLD",0xc70f,0x64002,0x1},
      {ATTR_MODRM,C_VMCLEAR,D__Mq,D__NONE,D__NONE,"VMCLEAR",0xc70f66,0x64003,0x1},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_VMXON,D__Mq,D__NONE,D__NONE,"VMXON",0xc70ff3,0x64003,0x1},
};
_DIS_OPCODE_T GroupSSE_G9VMX7[] = {
      {ATTR_MODRM|ATTR_JMP,C_VMPTRST,D__Mq,D__NONE,D__NONE,"VMPTRST",0xc70f,0x74002,0x1},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupG9[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM|ATTR_JMP,C_CMPXCHG8B,D__Mxdq,D__NONE,D__NONE,"CMPXCHG8B",0xc70f,0x10202,0x3},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_G9VMX6,0xc70f,0x60202,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_G9VMX7,0xc70f,0x70202,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3800[] = {
      {ATTR_MODRM,C_PSHUFB,D__Pq,D__Qq,D__NONE,"pshufb",0x380f,0x1003,0x7},
      {ATTR_MODRM,C_PSHUFB,D__Vdq,D__Wdq,D__NONE,"pshufb",0x380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3801[] = {
      {ATTR_MODRM,C_PHADDW,D__Pq,D__Qq,D__NONE,"phaddw",0x1380f,0x1003,0x7},
      {ATTR_MODRM,C_PHADDW,D__Vdq,D__Wdq,D__NONE,"phaddw",0x1380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3802[] = {
      {ATTR_MODRM,C_PHADDD,D__Pq,D__Qq,D__NONE,"phaddd",0x2380f,0x1003,0x7},
      {ATTR_MODRM,C_PHADDD,D__Vdq,D__Wdq,D__NONE,"phaddd",0x2380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3803[] = {
      {ATTR_MODRM,C_PHADDSW,D__Pq,D__Qq,D__NONE,"phaddsw",0x3380f,0x1003,0x7},
      {ATTR_MODRM,C_PHADDSW,D__Vdq,D__Wdq,D__NONE,"phaddsw",0x3380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3804[] = {
      {ATTR_MODRM,C_PMADDUBSW,D__Pq,D__Qq,D__NONE,"pmaddubsw",0x4380f,0x1003,0x7},
      {ATTR_MODRM,C_PMADDUBSW,D__Vdq,D__Wdq,D__NONE,"pmaddubsw",0x4380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3805[] = {
      {ATTR_MODRM,C_PHSUBW,D__Pq,D__Qq,D__NONE,"phsubw",0x5380f,0x1003,0x7},
      {ATTR_MODRM,C_PHSUBW,D__Vdq,D__Wdq,D__NONE,"phsubw",0x5380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3806[] = {
      {ATTR_MODRM,C_PHSUBD,D__Pq,D__Qq,D__NONE,"phsubd",0x6380f,0x1003,0x7},
      {ATTR_MODRM,C_PHSUBD,D__Vdq,D__Wdq,D__NONE,"phsubd",0x6380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3807[] = {
      {ATTR_MODRM,C_PHSUBSW,D__Pq,D__Qq,D__NONE,"phsubsw",0x7380f,0x1003,0x7},
      {ATTR_MODRM,C_PHSUBSW,D__Vdq,D__Wdq,D__NONE,"phsubsw",0x7380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3808[] = {
      {ATTR_MODRM,C_PSIGNB,D__Pq,D__Qq,D__NONE,"psignb",0x8380f,0x1003,0x7},
      {ATTR_MODRM,C_PSIGNB,D__Vdq,D__Wdq,D__NONE,"psignb",0x8380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3809[] = {
      {ATTR_MODRM,C_PSIGNW,D__Pq,D__Qq,D__NONE,"psignw",0x9380f,0x1003,0x7},
      {ATTR_MODRM,C_PSIGNW,D__Vdq,D__Wdq,D__NONE,"psignw",0x9380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f380a[] = {
      {ATTR_MODRM,C_PSIGND,D__Pq,D__Qq,D__NONE,"psignd",0xa380f,0x1003,0x7},
      {ATTR_MODRM,C_PSIGND,D__Vdq,D__Wdq,D__NONE,"psignd",0xa380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f380b[] = {
      {ATTR_MODRM,C_PMULHRSW,D__Pq,D__Qq,D__NONE,"pmulhrsw",0xb380f,0x1003,0x7},
      {ATTR_MODRM,C_PMULHRSW,D__Vdq,D__Wdq,D__NONE,"pmulhrsw",0xb380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3810[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PBLENDVB,D__Vdq,D__Wdq,D__NONE,"pblendvb",0x10380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3814[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_BLENDVPS,D__Vdq,D__Wdq,D__NONE,"blendvps",0x14380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3815[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_BLENDVPD,D__Vdq,D__Wdq,D__NONE,"blendvpd",0x15380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3817[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PTEST,D__Vdq,D__Wdq,D__NONE,"ptest",0x17380f66,0x1004,0x5},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f381c[] = {
      {ATTR_MODRM,C_PABSB,D__Pq,D__Qq,D__NONE,"pabsb",0x1c380f,0x1003,0x6},
      {ATTR_MODRM,C_PABSB,D__Vdq,D__Wdq,D__NONE,"pabsb",0x1c380f66,0x1004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f381d[] = {
      {ATTR_MODRM,C_PABSW,D__Pq,D__Qq,D__NONE,"pabsw",0x1d380f,0x1003,0x6},
      {ATTR_MODRM,C_PABSW,D__Vdq,D__Wdq,D__NONE,"pabsw",0x1d380f66,0x1004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f381e[] = {
      {ATTR_MODRM,C_PABSD,D__Pq,D__Qq,D__NONE,"pabsd",0x1e380f,0x1003,0x6},
      {ATTR_MODRM,C_PABSD,D__Vdq,D__Wdq,D__NONE,"pabsd",0x1e380f66,0x1004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3820[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVSXBW,D__Vdq,D__Mq,D__NONE,"pmovsxbw",0x20380f66,0x12004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVSXBW,D__Vdq,D__Udq,D__NONE,"pmovsxbw",0x20380f66,0x52004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3821[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVSXBD,D__Vdq,D__Md,D__NONE,"pmovsxbd",0x21380f66,0x12004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVSXBW,D__Vdq,D__Udq,D__NONE,"pmovsxbd",0x21380f66,0x52004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3822[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVSXBQ,D__Vdq,D__Mw,D__NONE,"pmovsxbq",0x22380f66,0x12004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVSXBQ,D__Vdq,D__Udq,D__NONE,"pmovsxbq",0x22380f66,0x52004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3823[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVSXWD,D__Vdq,D__Mq,D__NONE,"pmovsxwd",0x23380f66,0x12004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVSXWD,D__Vdq,D__Udq,D__NONE,"pmovsxwd",0x23380f66,0x52004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3824[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVSXWQ,D__Vdq,D__Md,D__NONE,"pmovsxwq",0x24380f66,0x12004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVSXBQ,D__Vdq,D__Udq,D__NONE,"pmovsxwq",0x24380f66,0x52004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3825[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVSXDQ,D__Vdq,D__Mq,D__NONE,"pmovsxdq",0x25380f66,0x12004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVSXDQ,D__Vdq,D__Udq,D__NONE,"pmovsxdq",0x25380f66,0x52004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3828[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMULDQ,D__Vdq,D__Wdq,D__NONE,"pmuldq",0x28380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3829[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PCMPEQQ,D__Vdq,D__Wdq,D__NONE,"pcmpeqq",0x29380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f382a[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_MOVNTDQA,D__Vdq,D__Mdq,D__NONE,"movntdqa",0x2a380f66,0x1004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f382b[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PACKUSDW,D__Vdq,D__Wdq,D__NONE,"packusdw",0x2b380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3830[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVZXBW,D__Vdq,D__Mq,D__NONE,"pmovzxbw",0x30380f66,0x12004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVZXBW,D__Vdq,D__Udq,D__NONE,"pmovzxbw",0x30380f66,0x52004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3831[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVZXBD,D__Vdq,D__Md,D__NONE,"pmovzxbd",0x31380f66,0x12004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVZXBW,D__Vdq,D__Udq,D__NONE,"pmovzxbd",0x31380f66,0x52004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3832[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVZXBQ,D__Vdq,D__Mw,D__NONE,"pmovzxbq",0x32380f66,0x12004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVZXBQ,D__Vdq,D__Udq,D__NONE,"pmovzxbq",0x32380f66,0x52004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3833[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVZXWD,D__Vdq,D__Mq,D__NONE,"pmovzxwd",0x33380f66,0x12004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVZXWD,D__Vdq,D__Udq,D__NONE,"pmovzxwd",0x33380f66,0x52004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3834[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVZXWQ,D__Vdq,D__Md,D__NONE,"pmovzxwq",0x34380f66,0x12004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVZXBQ,D__Vdq,D__Udq,D__NONE,"pmovzxwq",0x34380f66,0x52004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3835[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVZXDQ,D__Vdq,D__Mq,D__NONE,"pmovzxdq",0x35380f66,0x12004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMOVZXDQ,D__Vdq,D__Udq,D__NONE,"pmovzxdq",0x35380f66,0x52004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3837[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PCMPGTQ,D__Vdq,D__Wdq,D__NONE,"pcmpgtq",0x37380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3838[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMINSB,D__Vdq,D__Wdq,D__NONE,"pminsb",0x38380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3839[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMINSD,D__Vdq,D__Wdq,D__NONE,"pminsd",0x39380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f383a[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMINUW,D__Vdq,D__Wdq,D__NONE,"pminuw",0x3a380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f383b[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMINUD,D__Vdq,D__Wdq,D__NONE,"pminud",0x3b380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f383c[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMAXSB,D__Vdq,D__Wdq,D__NONE,"pmaxsb",0x3c380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f383d[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMAXSD,D__Vdq,D__Wdq,D__NONE,"pmaxsd",0x3d380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f383e[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMAXUW,D__Vdq,D__Wdq,D__NONE,"pmaxuw",0x3e380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f383f[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMAXUD,D__Vdq,D__Wdq,D__NONE,"pmaxud",0x3f380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3840[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PMULLD,D__Vdq,D__Wdq,D__NONE,"pmulld",0x40380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3841[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PHMINPOSUW,D__Vdq,D__Wdq,D__NONE,"phminposuw",0x41380f66,0x1004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3880[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_INVEPT,D__Gy,D__Mdq,D__NONE,"invept",0x80380f66,0x12004,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3881[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_INVVPID,D__Gy,D__Mdq,D__NONE,"invvpid",0x81380f66,0x12004,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f38db[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_AESIMC,D__Vdq,D__Wdq,D__NONE,"aesimc",0xdb380f66,0x1004,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f38dc[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_AESENC,D__Vdq,D__Wdq,D__NONE,"aesenc",0xdc380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f38dd[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_AESENCLAST,D__Vdq,D__Wdq,D__NONE,"aesenclast",0xdd380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f38de[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_AESDEC,D__Vdq,D__Wdq,D__NONE,"aesdec",0xde380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f38df[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_AESDECLAST,D__Vdq,D__Wdq,D__NONE,"aesdeclast",0xdf380f66,0x1004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f38f0[] = {
      {ATTR_MODRM,C_MOVBE,D__Gy,D__My,D__NONE,"movbe",0xf0380f,0x2003,0x6},
      {ATTR_MODRM,C_MOVBE,D__Gw,D__Mw,D__NONE,"movbe",0xf0380f,0x12003,0x6},
      {ATTR_MODRM,C_CRC32,D__Gy,D__Eb,D__NONE,"crc32",0xf0380ff2,0x22004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_CRC32,D__Gy,D__Eb,D__NONE,"crc32",0xf0380ff2,0x62004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f38f1[] = {
      {ATTR_MODRM,C_MOVBE,D__My,D__Gy,D__NONE,"movbe",0xf1380f,0x2003,0x6},
      {ATTR_MODRM,C_MOVBE,D__Mw,D__Gw,D__NONE,"movbe",0xf1380f,0x12003,0x6},
      {ATTR_MODRM,C_CRC32,D__Gy,D__Ey,D__NONE,"crc32",0xf1380ff2,0x22004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_CRC32,D__Gy,D__Ev,D__NONE,"crc32",0xf1380ff2,0x62004,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T OpTable3Byte0f38[] = {
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f3800,0x380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f3801,0x1380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f3802,0x2380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f3803,0x3380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f3804,0x4380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f3805,0x5380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f3806,0x6380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f3807,0x7380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f3808,0x8380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f3809,0x9380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f380a,0xa380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f380b,0xb380f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3810,0x10380f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3814,0x14380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3815,0x15380f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3817,0x17380f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f381c,0x1c380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f381d,0x1d380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0f381e,0x1e380f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3820,0x20380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3821,0x21380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3822,0x22380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3823,0x23380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3824,0x24380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3825,0x25380f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3828,0x28380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3829,0x29380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f382a,0x2a380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f382b,0x2b380f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3830,0x30380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3831,0x31380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3832,0x32380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3833,0x33380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3834,0x34380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3835,0x35380f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3837,0x37380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3838,0x38380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3839,0x39380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f383a,0x3a380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f383b,0x3b380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f383c,0x3c380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f383d,0x3d380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f383e,0x3e380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f383f,0x3f380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3840,0x40380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3841,0x41380f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3880,0x80380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3881,0x81380f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f38db,0xdb380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f38dc,0xdc380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f38dd,0xdd380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f38de,0xde380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f38df,0xdf380f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f38f0,0xf0380f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f38f1,0xf1380f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a08[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_ROUNDPS,D__Vdq,D__Wdq,D__Ib,"roundps",0x83a0f66,0x1004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a09[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_ROUNDPD,D__Vdq,D__Wdq,D__Ib,"roundpd",0x93a0f66,0x1004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a0a[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_ROUNDSS,D__Vdq,D__Wdq,D__Ib,"roundss",0xa3a0f66,0x1004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a0b[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_ROUNDSD,D__Vdq,D__Wdq,D__Ib,"roundsd",0xb3a0f66,0x1004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a0c[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_BLENDPS,D__Vdq,D__Wdq,D__Ib,"blendps",0xc3a0f66,0x1004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a0d[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_BLENDPD,D__Vdq,D__Wdq,D__Ib,"blendpd",0xd3a0f66,0x1004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a0e[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PBLENDW,D__Vdq,D__Wdq,D__Ib,"pblendw",0xe3a0f66,0x1004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a0f[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PALIGNR,D__Pq,D__Qq,D__Ib,"palignr",0xf3a0f,0x1003,0x17},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PALIGNR,D__Vdq,D__Wdq,D__Ib,"palignr",0xf3a0f66,0x1004,0x17},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a14[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PEXTRB,D__Mb,D__Vdq,D__Ib,"pextrb",0x143a0f66,0x12004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM|ATTR_SP,C_PEXTRB,D__Rd,D__Vdq,D__Ib,"pextrb",0x143a0f66,0x52004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a15[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PEXTRW,D__Mw,D__Vdq,D__Ib,"pextrw",0x153a0f66,0x12004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM|ATTR_SP,C_PEXTRW,D__Rd,D__Vdq,D__Ib,"pextrw",0x153a0f66,0x52004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a16[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PEXTRD,D__Ey,D__Vdq,D__Ib,"pextrd",0x163a0f66,0x1004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a17[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_EXTRACTPS,D__Ed,D__Vdq,D__Ib,"extractps",0x173a0f66,0x1004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a20[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PINSRB,D__Vdq,D__Mb,D__Ib,"pinsrb",0x203a0f66,0x11004,0x17},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PINSRB,D__Vdq,D__Rd,D__Ib,"pinsrb",0x203a0f66,0x51004,0x17},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a21[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_INSERTPS,D__Vdq,D__Md,D__Ib,"insertps",0x213a0f66,0x12004,0x17},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_INSERTPS,D__Vdq,D__Udq,D__Ib,"insertps",0x213a0f66,0x52004,0x17},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a22[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PINSRD,D__Vdq,D__Ey,D__Ib,"pinsrd",0x223a0f66,0x1004,0x17},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a40[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_DPPS,D__Vdq,D__Wdq,D__Ib,"dpps",0x403a0f66,0x1004,0x17},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a41[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_DPPD,D__Vdq,D__Wdq,D__Ib,"dppd",0x413a0f66,0x1004,0x17},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a42[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_MPSADBW,D__Vdq,D__Wdq,D__Ib,"mpsadbw",0x423a0f66,0x1004,0x17},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a44[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PCLMULQDQ,D__Vdq,D__Wdq,D__Ib,"pclmulqdq",0x443a0f66,0x1004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a60[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PCMPESTRM,D__Vdq,D__Wdq,D__Ib,"pcmpestrm",0x603a0f66,0x1004,0x15},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a61[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PCMPESTRI,D__Vdq,D__Wdq,D__Ib,"pcmpestri",0x613a0f66,0x1004,0x15},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a62[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PCMPISTRM,D__Vdq,D__Wdq,D__Ib,"pcmpistrm",0x623a0f66,0x1004,0x15},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3a63[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PCMPISTRI,D__Vdq,D__Wdq,D__Ib,"pcmpistri",0x633a0f66,0x1004,0x15},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f3adf[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_AESKEYGENASSIST,D__Vdq,D__Wdq,D__Ib,"aeskeygenassist",0xdf3a0f66,0x1004,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T OpTable3Byte0f3a[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a08,0x83a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a09,0x93a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a0a,0xa3a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a0b,0xb3a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a0c,0xc3a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a0d,0xd3a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a0e,0xe3a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a0f,0xf3a0f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a14,0x143a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a15,0x153a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a16,0x163a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a17,0x173a0f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a20,0x203a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a21,0x213a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a22,0x223a0f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a40,0x403a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a41,0x413a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a42,0x423a0f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a44,0x443a0f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a60,0x603a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a61,0x613a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a62,0x623a0f,0x3,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3a63,0x633a0f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f3adf,0xdf3a0f,0x3,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f70[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSHUFW,D__Pq,D__Qq,D__Ib,"pshufw",0x700f,0x1002,0x16},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSHUFD,D__Vdq,D__Wdq,D__Ib,"pshufd",0x700f66,0x1003,0x16},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSHUFLW,D__Vdq,D__Wdq,D__Ib,"pshuflw",0x700ff2,0x1003,0x16},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PSHUFHW,D__Vdq,D__Wdq,D__Ib,"pshufhw",0x700ff3,0x1003,0x16},
};
_DIS_OPCODE_T GroupSSE_0f74[] = {
      {ATTR_MODRM,C_PCMPEQB,D__Pq,D__Qq,D__NONE,"pcmpeqb",0x740f,0x1002,0x7},
      {ATTR_MODRM,C_PCMPEQB,D__Vdq,D__Wdq,D__NONE,"pcmpeqb",0x740f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f75[] = {
      {ATTR_MODRM,C_PCMPEQW,D__Pq,D__Qq,D__NONE,"pcmpeqw",0x750f,0x1002,0x7},
      {ATTR_MODRM,C_PCMPEQW,D__Vdq,D__Wdq,D__NONE,"pcmpeqw",0x750f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f76[] = {
      {ATTR_MODRM,C_PCMPEQD,D__Pq,D__Qq,D__NONE,"pcmpeqd",0x760f,0x1002,0x7},
      {ATTR_MODRM,C_PCMPEQD,D__Vdq,D__Wdq,D__NONE,"pcmpeqd",0x760f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f78[] = {
      {ATTR_MODRM|ATTR_JMP,C_VMREAD,D__Ey,D__Gy,D__NONE,"VMREAD",0x780f,0x1002,0x6},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_EXTRQ,D__Vdq,D__Ib,D_zIb,"extrq",0x780f66,0x1003,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f79[] = {
      {ATTR_MODRM|ATTR_JMP,C_VMWRITE,D__Gy,D__Ey,D__NONE,"VMWRITE",0x790f,0x1002,0x5},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f7c[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_HADDPD,D__Vdq,D__Wdq,D__NONE,"haddpd",0x7c0f66,0x1003,0x7},
      {ATTR_MODRM,C_HADDPS,D__Vps,D__Wps,D__NONE,"haddps",0x7c0ff2,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f7d[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_HSUBPD,D__Vdq,D__Wdq,D__NONE,"hsubpd",0x7d0f66,0x1003,0x7},
      {ATTR_MODRM,C_HSUBPS,D__Vps,D__Wps,D__NONE,"hsubps",0x7d0ff2,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f7e[] = {
      {ATTR_MODRM,C_MOVD,D__Ey,D__Pd,D__NONE,"movd",0x7e0f,0x1002,0x6},
      {ATTR_MODRM,C_MOVD,D__Ey,D__Vdq,D__NONE,"movd",0x7e0f66,0x1003,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_MOVQ,D__Vq,D__Wq,D__NONE,"movq",0x7e0ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f7f[] = {
      {ATTR_MODRM,C_MOVQ,D__Qq,D__Pq,D__NONE,"movq",0x7f0f,0x1002,0x6},
      {ATTR_MODRM,C_MOVDQA,D__Wdq,D__Vdq,D__NONE,"movdqa",0x7f0f66,0x1003,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_MOVDQU,D__Wdq,D__Vdq,D__NONE,"movdqu",0x7f0ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f60[] = {
      {ATTR_MODRM,C_PUNPCKLBW,D__Pq,D__Qq,D__NONE,"punpcklbw",0x600f,0x1002,0x7},
      {ATTR_MODRM,C_PUNPCKLBW,D__Vdq,D__Wdq,D__NONE,"punpcklbw",0x600f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f61[] = {
      {ATTR_MODRM,C_PUNPCKLWD,D__Pq,D__Qq,D__NONE,"punpcklwd",0x610f,0x1002,0x7},
      {ATTR_MODRM,C_PUNPCKLWD,D__Vdq,D__Wdq,D__NONE,"punpcklwd",0x610f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f62[] = {
      {ATTR_MODRM,C_PUNPCKLDQ,D__Pq,D__Qq,D__NONE,"punpckldq",0x620f,0x1002,0x7},
      {ATTR_MODRM,C_PUNPCKLDQ,D__Vdq,D__Wdq,D__NONE,"punpckldq",0x620f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f63[] = {
      {ATTR_MODRM,C_PACKSSWB,D__Pq,D__Qq,D__NONE,"packsswb",0x630f,0x1002,0x7},
      {ATTR_MODRM,C_PACKSSWB,D__Vdq,D__Wdq,D__NONE,"packsswb",0x630f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f64[] = {
      {ATTR_MODRM,C_PCMPGTB,D__Pq,D__Qq,D__NONE,"pcmpgtb",0x640f,0x1002,0x7},
      {ATTR_MODRM,C_PCMPGTB,D__Vdq,D__Wdq,D__NONE,"pcmpgtb",0x640f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f65[] = {
      {ATTR_MODRM,C_PCMPGTW,D__Pq,D__Qq,D__NONE,"pcmpgtw",0x650f,0x1002,0x7},
      {ATTR_MODRM,C_PCMPGTW,D__Vdq,D__Wdq,D__NONE,"pcmpgtw",0x650f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f66[] = {
      {ATTR_MODRM,C_PCMPGTD,D__Pq,D__Qq,D__NONE,"pcmpgtd",0x660f,0x1002,0x7},
      {ATTR_MODRM,C_PCMPGTD,D__Vdq,D__Wdq,D__NONE,"pcmpgtd",0x660f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f67[] = {
      {ATTR_MODRM,C_PACKUSWB,D__Pq,D__Qq,D__NONE,"packuswb",0x670f,0x1002,0x7},
      {ATTR_MODRM,C_PACKUSWB,D__Vdq,D__Wdq,D__NONE,"packuswb",0x670f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f68[] = {
      {ATTR_MODRM,C_PUNPCKHBW,D__Pq,D__Qq,D__NONE,"punpckhbw",0x680f,0x1002,0x7},
      {ATTR_MODRM,C_PUNPCKHBW,D__Vdq,D__Wdq,D__NONE,"punpckhbw",0x680f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f69[] = {
      {ATTR_MODRM,C_PUNPCKHWD,D__Pq,D__Qq,D__NONE,"punpckhwd",0x690f,0x1002,0x7},
      {ATTR_MODRM,C_PUNPCKHWD,D__Vdq,D__Wdq,D__NONE,"punpckhwd",0x690f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f6a[] = {
      {ATTR_MODRM,C_PUNPCKHDQ,D__Pq,D__Qq,D__NONE,"punpckhdq",0x6a0f,0x1002,0x7},
      {ATTR_MODRM,C_PUNPCKHDQ,D__Vdq,D__Wdq,D__NONE,"punpckhdq",0x6a0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f6b[] = {
      {ATTR_MODRM,C_PACKSSDW,D__Pq,D__Qq,D__NONE,"packssdw",0x6b0f,0x1002,0x7},
      {ATTR_MODRM,C_PACKSSDW,D__Vdq,D__Wdq,D__NONE,"packssdw",0x6b0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f6c[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PUNPCKLQDQ,D__Vdq,D__Wdq,D__NONE,"punpcklqdq",0x6c0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f6d[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_PUNPCKHQDQ,D__Vdq,D__Wdq,D__NONE,"punpckhqdq",0x6d0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f6e[] = {
      {ATTR_MODRM,C_MOVD,D__Pq,D__Ey,D__NONE,"movd",0x6e0f,0x1002,0x6},
      {ATTR_MODRM,C_MOVD,D__Vdq,D__Ey,D__NONE,"movd",0x6e0f66,0x1003,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f6f[] = {
      {ATTR_MODRM,C_MOVQ,D__Pq,D__Qq,D__NONE,"movq",0x6f0f,0x1002,0x6},
      {ATTR_MODRM,C_MOVDQA,D__Vdq,D__Wdq,D__NONE,"movdqa",0x6f0f66,0x1003,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_MOVDQU,D__Vdq,D__Wdq,D__NONE,"movdqu",0x6f0ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f50[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_MOVMSKPS,D__Gy,D__Ups,D__NONE,"movmskps",0x500f,0x42002,0x6},
      {ATTR_MODRM,C_MOVMSKPD,D__Gy,D__Upd,D__NONE,"movmskpd",0x500f66,0x52003,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f51[] = {
      {ATTR_MODRM,C_SQRTPS,D__Vps,D__Wps,D__NONE,"sqrtps",0x510f,0x1002,0x6},
      {ATTR_MODRM,C_SQRTPD,D__Vpd,D__Wpd,D__NONE,"sqrtpd",0x510f66,0x1003,0x6},
      {ATTR_MODRM,C_SQRTSD,D__Vsd,D__Wsd,D__NONE,"sqrtsd",0x510ff2,0x1003,0x6},
      {ATTR_MODRM,C_SQRTSS,D__Vss,D__Wss,D__NONE,"sqrtss",0x510ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f52[] = {
      {ATTR_MODRM,C_RSQRTPS,D__Vps,D__Wps,D__NONE,"rsqrtps",0x520f,0x1002,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_RSQRTSS,D__Vss,D__Wss,D__NONE,"rsqrtss",0x520ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f53[] = {
      {ATTR_MODRM,C_RCPPS,D__Vps,D__Wps,D__NONE,"rcpps",0x530f,0x1002,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_RCPSS,D__Vss,D__Wss,D__NONE,"rcpss",0x530ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f54[] = {
      {ATTR_MODRM,C_ANDPS,D__Vps,D__Wps,D__NONE,"andps",0x540f,0x1002,0x7},
      {ATTR_MODRM,C_ANDPD,D__Vpd,D__Wpd,D__NONE,"andpd",0x540f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f55[] = {
      {ATTR_MODRM,C_ANDNPS,D__Vps,D__Wps,D__NONE,"andnps",0x550f,0x1002,0x7},
      {ATTR_MODRM,C_ANDNPD,D__Vpd,D__Wpd,D__NONE,"andnpd",0x550f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f56[] = {
      {ATTR_MODRM,C_ORPS,D__Vps,D__Wps,D__NONE,"orps",0x560f,0x1002,0x7},
      {ATTR_MODRM,C_ORPD,D__Vpd,D__Wpd,D__NONE,"orpd",0x560f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f57[] = {
      {ATTR_MODRM,C_XORPS,D__Vps,D__Wps,D__NONE,"xorps",0x570f,0x1002,0x7},
      {ATTR_MODRM,C_XORPD,D__Vpd,D__Wpd,D__NONE,"xorpd",0x570f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f58[] = {
      {ATTR_MODRM,C_ADDPS,D_dVps,D_rWps,D__NONE,"addps",0x580f,0x1002,0x7},
      {ATTR_MODRM,C_ADDPD,D_dVpd,D_rWpd,D__NONE,"addpd",0x580f66,0x1003,0x7},
      {ATTR_MODRM,C_ADDSD,D_dVsd,D_rWsd,D__NONE,"addsd",0x580ff2,0x1003,0x7},
      {ATTR_MODRM,C_ADDSS,D_dVss,D_rWss,D__NONE,"addss",0x580ff3,0x1003,0x7},
};
_DIS_OPCODE_T GroupSSE_0f59[] = {
      {ATTR_MODRM,C_MULPS,D_dVps,D_rWps,D__NONE,"mulps",0x590f,0x1002,0x7},
      {ATTR_MODRM,C_MULPD,D_dVpd,D_rWpd,D__NONE,"mulpd",0x590f66,0x1003,0x7},
      {ATTR_MODRM,C_MULSD,D_dVsd,D_rWsd,D__NONE,"mulsd",0x590ff2,0x1003,0x7},
      {ATTR_MODRM,C_MULSS,D_dVss,D_rWss,D__NONE,"mulss",0x590ff3,0x1003,0x7},
};
_DIS_OPCODE_T GroupSSE_0f5a[] = {
      {ATTR_MODRM,C_CVTPS2PD,D_wVpd,D_rWps,D__NONE,"cvtps2pd",0x5a0f,0x1002,0x6},
      {ATTR_MODRM,C_CVTPD2PS,D_wVps,D_rWpd,D__NONE,"cvtpd2ps",0x5a0f66,0x1003,0x6},
      {ATTR_MODRM,C_CVTSD2SS,D_wVsd,D_rWsd,D__NONE,"cvtsd2ss",0x5a0ff2,0x1003,0x6},
      {ATTR_MODRM,C_CVTSS2SD,D_wVss,D_rWss,D__NONE,"cvtss2sd",0x5a0ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f5b[] = {
      {ATTR_MODRM,C_CVTDQ2PS,D_wVps,D_rWdq,D__NONE,"cvtdq2ps",0x5b0f,0x1002,0x6},
      {ATTR_MODRM,C_CVTPS2DQ,D_wVdq,D_rWps,D__NONE,"cvtps2dq",0x5b0f66,0x1003,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_CVTTPS2DQ,D_wVss,D_rWss,D__NONE,"cvttps2dq",0x5b0ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f5c[] = {
      {ATTR_MODRM,C_SUBPS,D_dVps,D_rWps,D__NONE,"subps",0x5c0f,0x1002,0x7},
      {ATTR_MODRM,C_SUBPD,D_dVpd,D_rWpd,D__NONE,"subpd",0x5c0f66,0x1003,0x7},
      {ATTR_MODRM,C_SUBSD,D_dVsd,D_rWsd,D__NONE,"subsd",0x5c0ff2,0x1003,0x7},
      {ATTR_MODRM,C_SUBSS,D_dVss,D_rWss,D__NONE,"subss",0x5c0ff3,0x1003,0x7},
};
_DIS_OPCODE_T GroupSSE_0f5d[] = {
      {ATTR_MODRM,C_MINPS,D_dVps,D_rWps,D__NONE,"minps",0x5d0f,0x1002,0x7},
      {ATTR_MODRM,C_MINPD,D_dVpd,D_rWpd,D__NONE,"minpd",0x5d0f66,0x1003,0x7},
      {ATTR_MODRM,C_MINSD,D_dVsd,D_rWsd,D__NONE,"minsd",0x5d0ff2,0x1003,0x7},
      {ATTR_MODRM,C_MINSS,D_dVss,D_rWss,D__NONE,"minss",0x5d0ff3,0x1003,0x7},
};
_DIS_OPCODE_T GroupSSE_0f5e[] = {
      {ATTR_MODRM,C_DIVPS,D__Vps,D__Wps,D__NONE,"divps",0x5e0f,0x1002,0x7},
      {ATTR_MODRM,C_DIVPD,D__Vpd,D__Wpd,D__NONE,"divpd",0x5e0f66,0x1003,0x7},
      {ATTR_MODRM,C_DIVSD,D__Vsd,D__Wsd,D__NONE,"divsd",0x5e0ff2,0x1003,0x7},
      {ATTR_MODRM,C_DIVSS,D__Vss,D__Wss,D__NONE,"divss",0x5e0ff3,0x1003,0x7},
};
_DIS_OPCODE_T GroupSSE_0f5f[] = {
      {ATTR_MODRM,C_MAXPS,D__Vps,D__Wps,D__NONE,"maxps",0x5f0f,0x1002,0x7},
      {ATTR_MODRM,C_MAXPD,D__Vpd,D__Wpd,D__NONE,"maxpd",0x5f0f66,0x1003,0x7},
      {ATTR_MODRM,C_MAXSD,D__Vsd,D__Wsd,D__NONE,"maxsd",0x5f0ff2,0x1003,0x7},
      {ATTR_MODRM,C_MAXSS,D__Vss,D__Wss,D__NONE,"maxss",0x5f0ff3,0x1003,0x7},
};
_DIS_OPCODE_T GroupSSE_0f10[] = {
      {ATTR_MODRM,C_MOVUPS,D__Vps,D__Wps,D__NONE,"movups",0x100f,0x1002,0x6},
      {ATTR_MODRM,C_MOVUPD,D__Vpd,D__Wpd,D__NONE,"movupd",0x100f66,0x1003,0x6},
      {ATTR_MODRM,C_MOVSD,D__Vsd,D__Wsd,D__NONE,"movsd",0x100ff2,0x1003,0x6},
      {ATTR_MODRM,C_MOVSS,D__Vss,D__Wss,D__NONE,"movss",0x100ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f11[] = {
      {ATTR_MODRM,C_MOVUPS,D__Wps,D__Vps,D__NONE,"movups",0x110f,0x1002,0x6},
      {ATTR_MODRM,C_MOVUPD,D__Wpd,D__Vpd,D__NONE,"movupd",0x110f66,0x1003,0x6},
      {ATTR_MODRM,C_MOVSD,D__Wsd,D__Vsd,D__NONE,"movsd",0x110ff2,0x1003,0x6},
      {ATTR_MODRM,C_MOVSS,D__Wss,D__Vss,D__NONE,"movss",0x110ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f12[] = {
      {ATTR_MODRM,C_MOVLPS,D__Vq,D__Mq,D__NONE,"movlps",0x120f,0x2002,0x7},
      {ATTR_MODRM,C_MOVLPD,D__Vq,D__Mq,D__NONE,"movlpd",0x120f66,0x12003,0x7},
      {ATTR_MODRM,C_MOVDDUP,D__Vq,D__Wq,D__NONE,"movddup",0x120ff2,0x22003,0x6},
      {ATTR_MODRM,C_MOVSLDUP,D__Vq,D__Wq,D__NONE,"movsldup",0x120ff3,0x32003,0x6},
      {ATTR_MODRM,C_MOVHLPS,D__Vq,D__Uq,D__NONE,"movhlps",0x120f,0x42002,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_MOVDDUP,D__Vq,D__Wq,D__NONE,"movddup",0x120ff2,0x62003,0x6},
      {ATTR_MODRM,C_MOVSLDUP,D__Vq,D__Wq,D__NONE,"movsldup",0x120ff3,0x72003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f13[] = {
      {ATTR_MODRM,C_MOVLPS,D__Mq,D__Vq,D__NONE,"movlps",0x130f,0x2002,0x6},
      {ATTR_MODRM,C_MOVLPD,D__Mq,D__Vq,D__NONE,"movlpd",0x130f66,0x12003,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f14[] = {
      {ATTR_MODRM,C_UNPCKLPS,D__Vps,D__Wq,D__NONE,"unpcklps",0x140f,0x1002,0x7},
      {ATTR_MODRM,C_UNPCKLPD,D__Vpd,D__Wq,D__NONE,"unpcklpd",0x140f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f15[] = {
      {ATTR_MODRM,C_UNPCKHPS,D__Vps,D__Wq,D__NONE,"unpckhps",0x150f,0x1002,0x7},
      {ATTR_MODRM,C_UNPCKHPD,D__Vpd,D__Wq,D__NONE,"unpckhpd",0x150f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__Vq,D__Mq,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__Vq,D__Mq,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f16[] = {
      {ATTR_MODRM,C_MOVHPS,D__Vq,D__Mq,D__NONE,"movhps",0x160f,0x2002,0x7},
      {ATTR_MODRM,C_MOVHPD,D__Vpd,D__Mq,D__NONE,"movhpd",0x160f66,0x12003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_MOVSHDUP,D__Vq,D__Wq,D__NONE,"movshdup",0x160ff3,0x32003,0x6},
      {ATTR_MODRM,C_MOVLHPS,D__Vq,D__Uq,D__NONE,"movlhps",0x160f,0x42002,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_MOVSHDUP,D__Vq,D__Wq,D__NONE,"movshdup",0x160ff3,0x72003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f17[] = {
      {ATTR_MODRM,C_MOVHPS,D__Mq,D__Vq,D__NONE,"movhps",0x170f,0x1002,0x6},
      {ATTR_MODRM,C_MOVHPD,D__Mq,D__Vq,D__NONE,"movhpd",0x170f66,0x1003,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f28[] = {
      {ATTR_MODRM,C_MOVAPS,D__Vps,D__Wps,D__NONE,"movaps",0x280f,0x1002,0x6},
      {ATTR_MODRM,C_MOVAPD,D__Vpd,D__Wpd,D__NONE,"movapd",0x280f66,0x1003,0x6},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f29[] = {
      {ATTR_MODRM,C_MOVAPS,D__Wps,D__Vps,D__NONE,"movaps",0x290f,0x1002,0x6},
      {ATTR_MODRM,C_MOVAPD,D__Wpd,D__Vpd,D__NONE,"movapd",0x290f66,0x1003,0x6},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f2a[] = {
      {ATTR_MODRM,C_CVTPI2PS,D__Vps,D__Qpi,D__NONE,"cvtpi2ps",0x2a0f,0x1002,0x6},
      {ATTR_MODRM,C_CVTPI2PD,D__Vpd,D__Qpi,D__NONE,"cvtpi2pd",0x2a0f66,0x1003,0x6},
      {ATTR_MODRM,C_CVTSI2SD,D__Vsd,D__Ey,D__NONE,"cvtsi2sd",0x2a0ff2,0x1003,0x6},
      {ATTR_MODRM,C_CVTSI2SS,D__Vss,D__Ey,D__NONE,"cvtsi2ss",0x2a0ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f2b[] = {
      {ATTR_MODRM,C_MOVNTPS,D__Mps,D__Vps,D__NONE,"movntps",0x2b0f,0x1002,0x6},
      {ATTR_MODRM,C_MOVNTPD,D__Mpd,D__Vpd,D__NONE,"movntpd",0x2b0f66,0x11003,0x6},
      {ATTR_MODRM,C_MOVNTSD,D__Mq,D__Vsd,D__NONE,"movntsd",0x2b0ff2,0x21003,0x6},
      {ATTR_MODRM,C_MOVNTSS,D__Md,D__Vss,D__NONE,"movntss",0x2b0ff3,0x31003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f2c[] = {
      {ATTR_MODRM,C_CVTTPS2PI,D__Ppi,D__Wps,D__NONE,"cvttps2pi",0x2c0f,0x1002,0x6},
      {ATTR_MODRM,C_CVTTPD2PI,D__Ppi,D__Wpd,D__NONE,"cvttpd2pi",0x2c0f66,0x1003,0x6},
      {ATTR_MODRM,C_CVTTSD2SI,D__Gy,D__Wsd,D__NONE,"cvttsd2si",0x2c0ff2,0x1003,0x6},
      {ATTR_MODRM,C_CVTTSS2SI,D__Gy,D__Wss,D__NONE,"cvttss2si",0x2c0ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f2d[] = {
      {ATTR_MODRM,C_CVTPS2PI,D__Ppi,D__Wps,D__NONE,"cvtps2pi",0x2d0f,0x1002,0x6},
      {ATTR_MODRM,C_CVTPD2PI,D__Ppi,D__Wpd,D__NONE,"cvtpd2pi",0x2d0f66,0x1003,0x6},
      {ATTR_MODRM,C_CVTSD2SI,D__Gy,D__Wsd,D__NONE,"cvtsd2si",0x2d0ff2,0x1003,0x6},
      {ATTR_MODRM,C_CVTSS2SI,D__Gy,D__Wss,D__NONE,"cvtss2si",0x2d0ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0f2e[] = {
      {ATTR_MODRM,C_UCOMISS,D__Vss,D__Wss,D__NONE,"ucomiss",0x2e0f,0x1002,0x5},
      {ATTR_MODRM,C_UCOMISD,D__Vsd,D__Wsd,D__NONE,"ucomisd",0x2e0f66,0x1003,0x5},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0f2f[] = {
      {ATTR_MODRM,C_COMISS,D__Vss,D__Wss,D__NONE,"comiss",0x2f0f,0x1002,0x5},
      {ATTR_MODRM,C_COMISD,D__Vsd,D__Wsd,D__NONE,"comisd",0x2f0f66,0x1003,0x5},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fd0[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_ADDSUBPD,D_dVpd,D_rWpd,D__NONE,"addsubpd",0xd00f66,0x1003,0x7},
      {ATTR_MODRM,C_ADDSUBPS,D_dVps,D_rWps,D__NONE,"addsubps",0xd00ff2,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fd1[] = {
      {ATTR_MODRM,C_PSRLW,D__Pq,D__Qq,D__NONE,"psrlw",0xd10f,0x1002,0x7},
      {ATTR_MODRM,C_PSRLW,D__Vdq,D__Wdq,D__NONE,"psrlw",0xd10f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fd2[] = {
      {ATTR_MODRM,C_PSRLD,D__Pq,D__Qq,D__NONE,"psrld",0xd20f,0x1002,0x7},
      {ATTR_MODRM,C_PSRLD,D__Vdq,D__Wdq,D__NONE,"psrld",0xd20f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fd3[] = {
      {ATTR_MODRM,C_PSRLQ,D__Pq,D__Qq,D__NONE,"psrlq",0xd30f,0x1002,0x7},
      {ATTR_MODRM,C_PSRLQ,D__Vdq,D__Wdq,D__NONE,"psrlq",0xd30f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fd4[] = {
      {ATTR_MODRM,C_PADDQ,D__Pq,D__Qq,D__NONE,"paddq",0xd40f,0x1002,0x7},
      {ATTR_MODRM,C_PADDQ,D__Vdq,D__Wdq,D__NONE,"paddq",0xd40f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fd5[] = {
      {ATTR_MODRM,C_PMULLW,D__Pq,D__Qq,D__NONE,"pmullw",0xd50f,0x1002,0x7},
      {ATTR_MODRM,C_PMULLW,D__Vdq,D__Wdq,D__NONE,"pmullw",0xd50f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fd6[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_MOVQ,D__Wq,D__Vq,D__NONE,"movq",0xd60f66,0x52003,0x6},
      {ATTR_MODRM,C_MOVDQ2Q,D__Pq,D__Uq,D__NONE,"movdq2q",0xd60ff2,0x62003,0x6},
      {ATTR_MODRM,C_MOVQ2DQ,D__Vdq,D__Nq,D__NONE,"movq2dq",0xd60ff3,0x72003,0x6},
};
_DIS_OPCODE_T GroupSSE_0fd7[] = {
      {ATTR_MODRM,C_PMOVMSKB,D__Gy,D__Nq,D__NONE,"pmovmskb",0xd70f,0x42002,0x6},
      {ATTR_MODRM,C_PMOVMSKB,D__Gy,D__Udq,D__NONE,"pmovmskb",0xd70f66,0x52003,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fd8[] = {
      {ATTR_MODRM,C_PSUBUSB,D__Pq,D__Qq,D__NONE,"psubusb",0xd80f,0x1002,0x7},
      {ATTR_MODRM,C_PSUBUSB,D__Vdq,D__Wdq,D__NONE,"psubusb",0xd80f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fd9[] = {
      {ATTR_MODRM,C_PSUBUSW,D__Pq,D__Qq,D__NONE,"psubusw",0xd90f,0x1002,0x7},
      {ATTR_MODRM,C_PSUBUSW,D__Vdq,D__Wdq,D__NONE,"psubusw",0xd90f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fda[] = {
      {ATTR_MODRM,C_PMINUB,D__Pq,D__Qq,D__NONE,"pminub",0xda0f,0x1002,0x7},
      {ATTR_MODRM,C_PMINUB,D__Vdq,D__Wdq,D__NONE,"pminub",0xda0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fdb[] = {
      {ATTR_MODRM,C_PAND,D__Pq,D__Qq,D__NONE,"pand",0xdb0f,0x1002,0x7},
      {ATTR_MODRM,C_PAND,D__Vdq,D__Wdq,D__NONE,"pand",0xdb0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fdc[] = {
      {ATTR_MODRM,C_PADDUSB,D__Pq,D__Qq,D__NONE,"paddusb",0xdc0f,0x1002,0x7},
      {ATTR_MODRM,C_PADDUSB,D__Vdq,D__Wdq,D__NONE,"paddusb",0xdc0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fdd[] = {
      {ATTR_MODRM,C_PADDUSW,D__Pq,D__Qq,D__NONE,"paddusw",0xdd0f,0x1002,0x7},
      {ATTR_MODRM,C_PADDUSW,D__Vdq,D__Wdq,D__NONE,"paddusw",0xdd0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fde[] = {
      {ATTR_MODRM,C_PMAXUB,D__Pq,D__Qq,D__NONE,"pmaxub",0xde0f,0x1002,0x7},
      {ATTR_MODRM,C_PMAXUB,D__Vdq,D__Wdq,D__NONE,"pmaxub",0xde0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fdf[] = {
      {ATTR_MODRM,C_PANDN,D__Pq,D__Qq,D__NONE,"pandn",0xdf0f,0x1002,0x7},
      {ATTR_MODRM,C_PANDN,D__Vdq,D__Wdq,D__NONE,"pandn",0xdf0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fe0[] = {
      {ATTR_MODRM,C_PAVGB,D__Pq,D__Qq,D__NONE,"pavgb",0xe00f,0x1002,0x7},
      {ATTR_MODRM,C_PAVGB,D__Vdq,D__Wdq,D__NONE,"pavgb",0xe00f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fe1[] = {
      {ATTR_MODRM,C_PSRAW,D__Pq,D__Qq,D__NONE,"psraw",0xe10f,0x1002,0x7},
      {ATTR_MODRM,C_PSRAW,D__Vdq,D__Wdq,D__NONE,"psraw",0xe10f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fe2[] = {
      {ATTR_MODRM,C_PSRAD,D__Pq,D__Qq,D__NONE,"psrad",0xe20f,0x1002,0x7},
      {ATTR_MODRM,C_PSRAD,D__Vdq,D__Wdq,D__NONE,"psrad",0xe20f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fe3[] = {
      {ATTR_MODRM,C_PAVGW,D__Pq,D__Qq,D__NONE,"pavgw",0xe30f,0x1002,0x7},
      {ATTR_MODRM,C_PAVGW,D__Vdq,D__Wdq,D__NONE,"pavgw",0xe30f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fe4[] = {
      {ATTR_MODRM,C_PMULHUW,D__Pq,D__Qq,D__NONE,"pmulhuw",0xe40f,0x1002,0x7},
      {ATTR_MODRM,C_PMULHUW,D__Vdq,D__Wdq,D__NONE,"pmulhuw",0xe40f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fe5[] = {
      {ATTR_MODRM,C_PMULHW,D__Pq,D__Qq,D__NONE,"pmulhw",0xe50f,0x1002,0x7},
      {ATTR_MODRM,C_PMULHW,D__Vdq,D__Wdq,D__NONE,"pmulhw",0xe50f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fe6[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_CVTTPD2DQ,D__Vdq,D__Wpd,D__NONE,"cvttpd2dq",0xe60f66,0x1003,0x6},
      {ATTR_MODRM,C_CVTPD2DQ,D__Vdq,D__Wpd,D__NONE,"cvtpd2dq",0xe60ff2,0x1003,0x6},
      {ATTR_MODRM,C_CVTDQ2PD,D__Vpd,D__Wdq,D__NONE,"cvtdq2pd",0xe60ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0fe7[] = {
      {ATTR_MODRM,C_MOVNTQ,D__Mq,D__Pq,D__NONE,"movntq",0xe70f,0x2002,0x6},
      {ATTR_MODRM,C_MOVNTDQ,D__Mdq,D__Vdq,D__NONE,"movntdq",0xe70f66,0x12003,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fe8[] = {
      {ATTR_MODRM,C_PSUBSB,D__Pq,D__Qq,D__NONE,"psubsb",0xe80f,0x1002,0x7},
      {ATTR_MODRM,C_PSUBSB,D__Vdq,D__Wdq,D__NONE,"psubsb",0xe80f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fe9[] = {
      {ATTR_MODRM,C_PSUBSW,D__Pq,D__Qq,D__NONE,"psubsw",0xe90f,0x1002,0x7},
      {ATTR_MODRM,C_PSUBSW,D__Vdq,D__Wdq,D__NONE,"psubsw",0xe90f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fea[] = {
      {ATTR_MODRM,C_PMINSW,D__Pq,D__Qq,D__NONE,"pminsw",0xea0f,0x1002,0x7},
      {ATTR_MODRM,C_PMINSW,D__Vdq,D__Wdq,D__NONE,"pminsw",0xea0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0feb[] = {
      {ATTR_MODRM,C_POR,D__Pq,D__Qq,D__NONE,"por",0xeb0f,0x1002,0x7},
      {ATTR_MODRM,C_POR,D__Vdq,D__Wdq,D__NONE,"por",0xeb0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fec[] = {
      {ATTR_MODRM,C_PADDSB,D__Pq,D__Qq,D__NONE,"paddsb",0xec0f,0x1002,0x7},
      {ATTR_MODRM,C_PADDSB,D__Vdq,D__Wdq,D__NONE,"paddsb",0xec0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fed[] = {
      {ATTR_MODRM,C_PADDSW,D__Pq,D__Qq,D__NONE,"paddsw",0xed0f,0x1002,0x7},
      {ATTR_MODRM,C_PADDSW,D__Vdq,D__Wdq,D__NONE,"paddsw",0xed0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fee[] = {
      {ATTR_MODRM,C_PMAXSW,D__Pq,D__Qq,D__NONE,"pmaxsw",0xee0f,0x1002,0x7},
      {ATTR_MODRM,C_PMAXSW,D__Vdq,D__Wdq,D__NONE,"pmaxsw",0xee0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fef[] = {
      {ATTR_MODRM,C_PXOR,D__Pq,D__Qq,D__NONE,"pxor",0xef0f,0x1002,0x7},
      {ATTR_MODRM,C_PXOR,D__Vdq,D__Wdq,D__NONE,"pxor",0xef0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ff0[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_LDDQU,D__Vdq,D__Mdq,D__NONE,"lddqu",0xf00ff2,0x22003,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ff1[] = {
      {ATTR_MODRM,C_PSLLW,D__Pq,D__Qq,D__NONE,"psllw",0xf10f,0x1002,0x7},
      {ATTR_MODRM,C_PSLLW,D__Vdq,D__Wdq,D__NONE,"psllw",0xf10f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ff2[] = {
      {ATTR_MODRM,C_PSLLD,D__Pq,D__Qq,D__NONE,"pslld",0xf20f,0x1002,0x7},
      {ATTR_MODRM,C_PSLLD,D__Vdq,D__Wdq,D__NONE,"pslld",0xf20f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ff3[] = {
      {ATTR_MODRM,C_PSLLQ,D__Pq,D__Qq,D__NONE,"psllq",0xf30f,0x1002,0x7},
      {ATTR_MODRM,C_PSLLQ,D__Vdq,D__Wdq,D__NONE,"psllq",0xf30f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ff4[] = {
      {ATTR_MODRM,C_PMULUDQ,D__Pq,D__Qq,D__NONE,"pmuludq",0xf40f,0x1002,0x7},
      {ATTR_MODRM,C_PMULUDQ,D__Vdq,D__Wdq,D__NONE,"pmuludq",0xf40f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ff5[] = {
      {ATTR_MODRM,C_PMADDWD,D__Pq,D__Qq,D__NONE,"pmaddwd",0xf50f,0x1002,0x7},
      {ATTR_MODRM,C_PMADDWD,D__Vdq,D__Wdq,D__NONE,"pmaddwd",0xf50f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ff6[] = {
      {ATTR_MODRM,C_PSADBW,D__Pq,D__Qq,D__NONE,"psadbw",0xf60f,0x1002,0x7},
      {ATTR_MODRM,C_PSADBW,D__Vdq,D__Wdq,D__NONE,"psadbw",0xf60f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ff7[] = {
      {ATTR_MODRM,C_MASKMOVQ,D__Pq,D__Nq,D__NONE,"maskmovq",0xf70f,0x42002,0x5},
      {ATTR_MODRM,C_MASKMOVDQU,D__Vdq,D__Udq,D__NONE,"maskmovdqu",0xf70f66,0x52003,0x5},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ff8[] = {
      {ATTR_MODRM,C_PSUBB,D__Pq,D__Qq,D__NONE,"psubb",0xf80f,0x1002,0x7},
      {ATTR_MODRM,C_PSUBB,D__Vdq,D__Wdq,D__NONE,"psubb",0xf80f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ff9[] = {
      {ATTR_MODRM,C_PSUBW,D__Pq,D__Qq,D__NONE,"psubw",0xf90f,0x1002,0x7},
      {ATTR_MODRM,C_PSUBW,D__Vdq,D__Wdq,D__NONE,"psubw",0xf90f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ffa[] = {
      {ATTR_MODRM,C_PSUBD,D__Pq,D__Qq,D__NONE,"psubd",0xfa0f,0x1002,0x7},
      {ATTR_MODRM,C_PSUBD,D__Vdq,D__Wdq,D__NONE,"psubd",0xfa0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ffb[] = {
      {ATTR_MODRM,C_PSUBQ,D__Pq,D__Qq,D__NONE,"psubq",0xfb0f,0x1002,0x7},
      {ATTR_MODRM,C_PSUBQ,D__Vdq,D__Wdq,D__NONE,"psubq",0xfb0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ffc[] = {
      {ATTR_MODRM,C_PADDB,D__Pq,D__Qq,D__NONE,"paddb",0xfc0f,0x1002,0x7},
      {ATTR_MODRM,C_PADDB,D__Vdq,D__Wdq,D__NONE,"paddb",0xfc0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ffd[] = {
      {ATTR_MODRM,C_PADDW,D__Pq,D__Qq,D__NONE,"paddw",0xfd0f,0x1002,0x7},
      {ATTR_MODRM,C_PADDW,D__Vdq,D__Wdq,D__NONE,"paddw",0xfd0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0ffe[] = {
      {ATTR_MODRM,C_PADDD,D__Pq,D__Qq,D__NONE,"paddd",0xfe0f,0x1002,0x7},
      {ATTR_MODRM,C_PADDD,D__Vdq,D__Wdq,D__NONE,"paddd",0xfe0f66,0x1003,0x7},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fc2[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_CMPPS,D__Vps,D__Wps,D__Ib,"cmpps",0xc20f,0x1002,0x17},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_CMPPD,D__Vpd,D__Wpd,D__Ib,"cmppd",0xc20f66,0x1003,0x17},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_CMPSD,D__Vsd,D__Wsd,D__Ib,"cmpsd",0xc20ff2,0x1003,0x17},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_CMPSS,D__Vss,D__Wss,D__Ib,"cmpss",0xc20ff3,0x1003,0x17},
};
_DIS_OPCODE_T GroupSSE_0fb8[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_POPCNT,D__Gv,D__Ev,D__NONE,"popcnt",0xb80ff3,0x1003,0x6},
};
_DIS_OPCODE_T GroupSSE_0fc4[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PINSRW,D__Pq,D__Mw,D__Ib,"pinsrw",0xc40f,0x2002,0x16},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PINSRW,D__Vdq,D__Mw,D__Ib,"pinsrw",0xc40f66,0x12003,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PINSRW,D__Pq,D__Rv,D__Ib,"pinsrw",0xc40f,0x42002,0x16},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PINSRW,D__Vdq,D__Rv,D__Ib,"pinsrw",0xc40f66,0x52003,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fc5[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PEXTRW,D__Gy,D__Nq,D__Ib,"pextrw",0xc50f,0x42002,0x16},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_PEXTRW,D__Gy,D__Udq,D__Ib,"pextrw",0xc50f66,0x52003,0x16},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T GroupSSE_0fc6[] = {
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SHUFPS,D__Vps,D__Wps,D__Ib,"shufps",0xc60f,0x1002,0x17},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SHUFPD,D__Vpd,D__Wpd,D__Ib,"shufpd",0xc60f66,0x1003,0x17},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,_C_XCHG,D__rAX,D__erAX,D__NONE,"XCHG",0x90,0x1001,0xf},
      {0,_C_XCHG,D__rAX,D__erAX,D__NONE,"XCHG",0x9066,0x1002,0xf},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_PAUSE,D__NONE,D__NONE,D__NONE,"PAUSE",0x90f3,0x1002,0x0},
};
_DIS_OPCODE_T Group_amd3dnow[] = {
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PI2FW,D__Pq,D__Qq,D__NONE,"PI2FW",0xf0f,0xc8002,0x0},
      {ATTR_MODRM,C_PI2FD,D__Pq,D__Qq,D__NONE,"PI2FD",0xf0f,0xd8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PF2IW,D__Pq,D__Qq,D__NONE,"PF2IW",0xf0f,0x1c8002,0x0},
      {ATTR_MODRM,C_PF2ID,D__Pq,D__Qq,D__NONE,"PF2ID",0xf0f,0x1d8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFNACC,D__Pq,D__Qq,D__NONE,"PFNACC",0xf0f,0x8a8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFPNACC,D__Pq,D__Qq,D__NONE,"PFPNACC",0xf0f,0x8e8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFCMPGE,D__Pq,D__Qq,D__NONE,"PFCMPGE",0xf0f,0x908002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFMIN,D__Pq,D__Qq,D__NONE,"PFMIN",0xf0f,0x948002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFRCP,D__Pq,D__Qq,D__NONE,"PFRCP",0xf0f,0x968002,0x0},
      {ATTR_MODRM,C_PFRSQRT,D__Pq,D__Qq,D__NONE,"PFRSQRT",0xf0f,0x978002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFSUB,D__Pq,D__Qq,D__NONE,"PFSUB",0xf0f,0x9a8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFADD,D__Pq,D__Qq,D__NONE,"PFADD",0xf0f,0x9e8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFCMPGT,D__Pq,D__Qq,D__NONE,"PFCMPGT",0xf0f,0xa08002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFMAX,D__Pq,D__Qq,D__NONE,"PFMAX",0xf0f,0xa48002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFRCPIT1,D__Pq,D__Qq,D__NONE,"PFRCPIT1",0xf0f,0xa68002,0x0},
      {ATTR_MODRM,C_PFRSQIT1,D__Pq,D__Qq,D__NONE,"PFRSQIT1",0xf0f,0xa78002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFSUBR,D__Pq,D__Qq,D__NONE,"PFSUBR",0xf0f,0xaa8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFACC,D__Pq,D__Qq,D__NONE,"PFACC",0xf0f,0xae8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFCMPEQ,D__Pq,D__Qq,D__NONE,"PFCMPEQ",0xf0f,0xb08002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFMUL,D__Pq,D__Qq,D__NONE,"PFMUL",0xf0f,0xb48002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PFRCPIT2,D__Pq,D__Qq,D__NONE,"PFRCPIT2",0xf0f,0xb68002,0x0},
      {ATTR_MODRM,C_PMULHRW,D__Pq,D__Qq,D__NONE,"PMULHRW",0xf0f,0xb78002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PSWAPD,D__Pq,D__Qq,D__NONE,"PSWAPD",0xf0f,0xbb8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_PAVGUSB,D__Pq,D__Qq,D__NONE,"PAVGUSB",0xf0f,0xbf8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0xf0f,0x8002,0x0},
};
_DIS_OPCODE_T OpTable2Byte0f[] = {
      {ATTR_MODRM,C_GRP,D__NONE,D__NONE,D__NONE,(const char*)GroupG6,0xf,0x2,0x0},
      {ATTR_MODRM,C_GRPRM,D__NONE,D__NONE,D__NONE,(const char*)GroupG7M,0x10f,0x2,0x0},
      {ATTR_MODRM,C_LAR,D__Gv,D_rEv,D__NONE,"LAR",0x20f,0x2,0x6},
      {ATTR_MODRM,C_LSL,D__Gv,D_rEv,D__NONE,"LSL",0x30f,0x2,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_X64_ONLY,C_SYSCALL,D__NONE,D__NONE,D__NONE,"SYSCALL",0x50f,0x2,0x0},
      {0,C_CLTS,D__NONE,D__NONE,D__NONE,"CLTS",0x60f,0x2,0x0},
      {ATTR_X64_ONLY,C_SYSRET,D__NONE,D__NONE,D__NONE,"SYSRET",0x70f,0x2,0x0},
      {0,C_INVD,D__NONE,D__NONE,D__NONE,"INVD",0x80f,0x2,0x0},
      {0,C_WBINVD,D__NONE,D__NONE,D__NONE,"WBINVD",0x90f,0x2,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_UD2,D__NONE,D__NONE,D__NONE,"UD2",0xb0f,0x2,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NOP,D__Ev,D__NONE,D__NONE,"NOP",0xd0f,0x2,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_3DNOW,D__Pq,D__Qq,D__NONE,"",0xf0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f10,0x100f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f11,0x110f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f12,0x120f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f13,0x130f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f14,0x140f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f15,0x150f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f16,0x160f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f17,0x170f,0x2,0x0},
      {ATTR_MODRM,C_GRP,D__NONE,D__NONE,D__NONE,(const char*)GroupG16,0x180f,0x2,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NOP,D__Ev,D__NONE,D__NONE,"NOP",0x1f0f,0x2,0x0},
      {ATTR_MODRM|ATTR_JMP,C_MOV,D__Rd,D__Cd,D__NONE,"MOV",0x200f,0x2,0x6},
      {ATTR_MODRM|ATTR_JMP,C_MOV,D__Rd,D__Dd,D__NONE,"MOV",0x210f,0x2,0x6},
      {ATTR_MODRM|ATTR_JMP,C_MOV,D__Cd,D__Rd,D__NONE,"MOV",0x220f,0x2,0x6},
      {ATTR_MODRM|ATTR_JMP,C_MOV,D__Dd,D__Rd,D__NONE,"MOV",0x230f,0x2,0x6},
      {ATTR_MODRM|ATTR_JMP,C_MOV,D__Rd,D__Td,D__NONE,"MOV",0x240f,0x2,0x6},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM|ATTR_JMP,C_MOV,D__Td,D__Rd,D__NONE,"MOV",0x260f,0x2,0x6},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f28,0x280f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f29,0x290f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f2a,0x2a0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f2b,0x2b0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f2c,0x2c0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f2d,0x2d0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f2e,0x2e0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f2f,0x2f0f,0x2,0x0},
      {0,C_WRMSR,D__NONE,D__NONE,D__NONE,"WRMSR",0x300f,0x2,0x0},
      {0,C_RDTSC,D__NONE,D__NONE,D__NONE,"RDTSC",0x310f,0x2,0x0},
      {0,C_RDMSR,D__NONE,D__NONE,D__NONE,"RDMSR",0x320f,0x2,0x0},
      {0,C_RDPMC,D__NONE,D__NONE,D__NONE,"RDPMC",0x330f,0x2,0x0},
      {0,C_SYSENTER,D__NONE,D__NONE,D__NONE,"SYSENTER",0x340f,0x2,0x0},
      {0,C_SYSEXIT,D__NONE,D__NONE,D__NONE,"SYSEXIT",0x350f,0x2,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_GETSEC,D__NONE,D__NONE,D__NONE,"GETSEC",0x370f,0x2,0x0},
      {ATTR_MODRM,C_3BYTE_1,D__NONE,D__NONE,D__NONE,(const char*)OpTable3Byte0f38,0x380f,0x2,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_3BYTE_2,D__NONE,D__NONE,D__NONE,(const char*)OpTable3Byte0f3a,0x3a0f,0x2,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_MODRM,C_CMOVtO,D__Gv,D_rEv,D__tO,"CMOVO",0x400f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfO,D__Gv,D_rEv,D__fO,"CMOVNO",0x410f,0x2,0x7},
      {ATTR_MODRM,C_CMOVtC,D__Gv,D_rEv,D__tC,"CMOVB",0x420f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfC,D__Gv,D_rEv,D__fC,"CMOVNB",0x430f,0x2,0x7},
      {ATTR_MODRM,C_CMOVtZ,D__Gv,D_rEv,D__tZ,"CMOVZ",0x440f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfZ,D__Gv,D_rEv,D__fZ,"CMOVNZ",0x450f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfA,D__Gv,D_rEv,D__fA,"CMOVBE",0x460f,0x2,0x7},
      {ATTR_MODRM,C_CMOVtA,D__Gv,D_rEv,D__tA,"CMOVA",0x470f,0x2,0x7},
      {ATTR_MODRM,C_CMOVtS,D__Gv,D_rEv,D__fS,"CMOVS",0x480f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfS,D__Gv,D_rEv,D__fP,"CMOVNS",0x490f,0x2,0x7},
      {ATTR_MODRM,C_CMOVtP,D__Gv,D_rEv,D__tP,"CMOVP",0x4a0f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfP,D__Gv,D_rEv,D__tS,"CMOVNP",0x4b0f,0x2,0x7},
      {ATTR_MODRM,C_CMOVtL,D__Gv,D_rEv,D__tL,"CMOVL",0x4c0f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfL,D__Gv,D_rEv,D__fL,"CMOVGE",0x4d0f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfG,D__Gv,D_rEv,D__fG,"CMOVLE",0x4e0f,0x2,0x7},
      {ATTR_MODRM,C_CMOVtG,D__Gv,D_rEv,D__tG,"CMOVG",0x4f0f,0x2,0x7},
      {ATTR_MODRM,C_GRPSSEFIX,D__Ed,D__Vps,D__NONE,(const char*)GroupSSE_0f50,0x500f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wps,D__NONE,(const char*)GroupSSE_0f51,0x510f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wps,D__NONE,(const char*)GroupSSE_0f52,0x520f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wps,D__NONE,(const char*)GroupSSE_0f53,0x530f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wps,D__NONE,(const char*)GroupSSE_0f54,0x540f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wps,D__NONE,(const char*)GroupSSE_0f55,0x550f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wps,D__NONE,(const char*)GroupSSE_0f56,0x560f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wps,D__NONE,(const char*)GroupSSE_0f57,0x570f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wps,D__NONE,(const char*)GroupSSE_0f58,0x580f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wps,D__NONE,(const char*)GroupSSE_0f59,0x590f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vpd,D__Wps,D__NONE,(const char*)GroupSSE_0f5a,0x5a0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wdq,D__NONE,(const char*)GroupSSE_0f5b,0x5b0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wps,D__NONE,(const char*)GroupSSE_0f5c,0x5c0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wps,D__NONE,(const char*)GroupSSE_0f5d,0x5d0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wps,D__NONE,(const char*)GroupSSE_0f5e,0x5e0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Vps,D__Wps,D__NONE,(const char*)GroupSSE_0f5f,0x5f0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f60,0x600f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f61,0x610f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f62,0x620f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f63,0x630f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f64,0x640f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f65,0x650f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f66,0x660f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f67,0x670f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f68,0x680f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f69,0x690f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f6a,0x6a0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f6b,0x6b0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f6c,0x6c0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f6d,0x6d0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f6e,0x6e0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f6f,0x6f0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f70,0x700f,0x2,0x0},
      {ATTR_MODRM,C_GRP7,D__NONE,D__NONE,D__NONE,(const char*)GroupG12,0x710f,0x2,0x0},
      {ATTR_MODRM,C_GRP7,D__NONE,D__NONE,D__NONE,(const char*)GroupG13,0x720f,0x2,0x0},
      {ATTR_MODRM,C_GRP7,D__NONE,D__NONE,D__NONE,(const char*)GroupG14,0x730f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f74,0x740f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f75,0x750f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f76,0x760f,0x2,0x0},
      {0,C_EMMS,D__NONE,D__NONE,D__NONE,"emms",0x770f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f78,0x780f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f79,0x790f,0x2,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,"MMX UD",0x0,0x0,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,"MMX UD",0x0,0x0,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f7c,0x7c0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f7d,0x7d0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f7e,0x7e0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0f7f,0x7f0f,0x2,0x0},
      {ATTR_JMP,C_JtO,D__Jz,D__tO,D__NONE,"JO",0x800f,0x2,0x0},
      {ATTR_JMP,C_JfO,D__Jz,D__fO,D__NONE,"JNO",0x810f,0x2,0x0},
      {ATTR_JMP,C_JtC,D__Jz,D__tC,D__NONE,"JC",0x820f,0x2,0x0},
      {ATTR_JMP,C_JfC,D__Jz,D__fC,D__NONE,"JNC",0x830f,0x2,0x0},
      {ATTR_JMP,C_JtZ,D__Jz,D__tZ,D__NONE,"JZ",0x840f,0x2,0x0},
      {ATTR_JMP,C_JfZ,D__Jz,D__fZ,D__NONE,"JNZ",0x850f,0x2,0x0},
      {ATTR_JMP,C_JfA,D__Jz,D__fA,D__NONE,"JNA",0x860f,0x2,0x0},
      {ATTR_JMP,C_JtA,D__Jz,D__tA,D__NONE,"JA",0x870f,0x2,0x0},
      {ATTR_JMP,C_JtS,D__Jz,D__fS,D__NONE,"JS",0x880f,0x2,0x0},
      {ATTR_JMP,C_JfS,D__Jz,D__fP,D__NONE,"JNS",0x890f,0x2,0x0},
      {ATTR_JMP,C_JtP,D__Jz,D__tP,D__NONE,"JP",0x8a0f,0x2,0x0},
      {ATTR_JMP,C_JfP,D__Jz,D__tS,D__NONE,"JNP",0x8b0f,0x2,0x0},
      {ATTR_JMP,C_JtL,D__Jz,D__tL,D__NONE,"JL",0x8c0f,0x2,0x0},
      {ATTR_JMP,C_JfL,D__Jz,D__fL,D__NONE,"JNL",0x8d0f,0x2,0x0},
      {ATTR_JMP,C_JfG,D__Jz,D__fG,D__NONE,"JNG",0x8e0f,0x2,0x0},
      {ATTR_JMP,C_JtG,D__Jz,D__tG,D__NONE,"JG",0x8f0f,0x2,0x0},
      {ATTR_MODRM,C_SETtO,D_wEb,D__tO,D__NONE,"SETO",0x900f,0x2,0x1},
      {ATTR_MODRM,C_SETfO,D_wEb,D__fO,D__NONE,"SETNO",0x910f,0x2,0x1},
      {ATTR_MODRM,C_SETtC,D_wEb,D__tC,D__NONE,"SETC",0x920f,0x2,0x1},
      {ATTR_MODRM,C_SETfC,D_wEb,D__fC,D__NONE,"SETNC",0x930f,0x2,0x1},
      {ATTR_MODRM,C_SETtZ,D_wEb,D__tZ,D__NONE,"SETZ",0x940f,0x2,0x1},
      {ATTR_MODRM,C_SETfZ,D_wEb,D__fZ,D__NONE,"SETNZ",0x950f,0x2,0x1},
      {ATTR_MODRM,C_SETfA,D_wEb,D__fA,D__NONE,"SETNA",0x960f,0x2,0x1},
      {ATTR_MODRM,C_SETtA,D_wEb,D__tA,D__NONE,"SETA",0x970f,0x2,0x1},
      {ATTR_MODRM,C_SETtS,D_wEb,D__fS,D__NONE,"SETS",0x980f,0x2,0x1},
      {ATTR_MODRM,C_SETfS,D_wEb,D__fP,D__NONE,"SETNS",0x990f,0x2,0x1},
      {ATTR_MODRM,C_SETtP,D_wEb,D__tP,D__NONE,"SETP",0x9a0f,0x2,0x1},
      {ATTR_MODRM,C_SETfP,D_wEb,D__tS,D__NONE,"SETNP",0x9b0f,0x2,0x1},
      {ATTR_MODRM,C_SETtL,D_wEb,D__tL,D__NONE,"SETL",0x9c0f,0x2,0x1},
      {ATTR_MODRM,C_SETfL,D_wEb,D__fL,D__NONE,"SETNL",0x9d0f,0x2,0x1},
      {ATTR_MODRM,C_SETfG,D_wEb,D__fG,D__NONE,"SETNG",0x9e0f,0x2,0x1},
      {ATTR_MODRM,C_SETtG,D_wEb,D__tG,D__NONE,"SETG",0x9f0f,0x2,0x1},
      {ATTR_SP,C_PUSH,D__FS,D__NONE,D__NONE,"PUSH",0xa00f,0x2,0x1},
      {ATTR_SP,C_POP,D__FS,D__NONE,D__NONE,"POP",0xa10f,0x2,0x2},
      {0,C_CPUID,D__NONE,D__NONE,D__NONE,"CPUID",0xa20f,0x2,0x0},
      {ATTR_MODRM,C_BT,D_rEv,D__Gv,D__NONE,"BT",0xa30f,0x2,0x5},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SHLD,D__Ev,D__Gv,D__Ib,"SHLD",0xa40f,0x2,0x16},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SHLD,D__Ev,D__Gv,D__CL,"SHLD",0xa50f,0x2,0x16},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {0,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_SP,C_PUSH,D__GS,D__NONE,D__NONE,"PUSP",0xa80f,0x2,0x1},
      {ATTR_SP,C_POP,D__GS,D__NONE,D__NONE,"POP",0xa90f,0x2,0x2},
      {0,C_RSM,D__NONE,D__NONE,D__NONE,"RSM",0xaa0f,0x2,0x0},
      {ATTR_MODRM,C_BTS,D__Ev,D__Gv,D__NONE,"BTS",0xab0f,0x2,0x7},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_SHRD,D__Ev,D__Gv,D__Ib,"SHRD",0xac0f,0x2,0x16},
      {ATTR_MODRM,C_SHRD,D__Ev,D__Gv,D__CL,"SHRD",0xad0f,0x2,0x16},
      {ATTR_MODRM,C_GRPMOD,D__NONE,D__NONE,D__NONE,(const char*)GroupG15,0xae0f,0x2,0x0},
      {ATTR_MODRM,C_IMUL,D__Gv,D_rEv,D__NONE,"IMUL",0xaf0f,0x2,0x7},
      {ATTR_MODRM,C_CMPXCHG,D__Eb,D__Gb,D__NONE,"CMPXCHG",0xb00f,0x2,0xf},
      {ATTR_MODRM,C_CMPXCHG,D__Ev,D__Gv,D__NONE,"CMPXCHG",0xb10f,0x2,0xf},
      {ATTR_MODRM,C_LSS,D__Gv,D_rMp,D__NONE,"LSS",0xb20f,0x2,0x6},
      {ATTR_MODRM,C_BTR,D__Ev,D__Gv,D__NONE,"BTR",0xb30f,0x2,0x6},
      {ATTR_MODRM,C_LFS,D__Gv,D_rMp,D__NONE,"LFS",0xb40f,0x2,0x6},
      {ATTR_MODRM,C_LGS,D__Gv,D_rMp,D__NONE,"LGS",0xb50f,0x2,0x6},
      {ATTR_MODRM,C_MOVZX,D__Gv,D_rEb,D__NONE,"MOVZX",0xb60f,0x2,0x6},
      {ATTR_MODRM,C_MOVZX,D__Gv,D_rEw,D__NONE,"MOVZX",0xb70f,0x2,0x6},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0fb8,0xb80f,0x2,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_GRP,D__Ev,D__Ib,D__NONE,(const char*)GroupG8EvIb,0xba0f,0x2,0x0},
      {ATTR_MODRM,C_BTC,D__Ev,D__Gv,D__NONE,"BTC",0xbb0f,0x2,0x7},
      {ATTR_MODRM,C_BSF,D__Gv,D__Ev,D__NONE,"BSF",0xbc0f,0x2,0x6},
      {ATTR_MODRM,C_BSR,D__Gv,D__Ev,D__NONE,"BSR",0xbd0f,0x2,0x6},
      {ATTR_MODRM,C_MOVSX,D__Gv,D_rEb,D__NONE,"MOVSX",0xbe0f,0x2,0x6},
      {ATTR_MODRM,C_MOVSX,D__Gv,D_rEw,D__NONE,"MOVSX",0xbf0f,0x2,0x6},
      {ATTR_MODRM,C_XADD,D__Eb,D__Gb,D__NONE,"XADD",0xc00f,0x2,0x7},
      {ATTR_MODRM,C_XADD,D__Ev,D__Gv,D__NONE,"XADD",0xc10f,0x2,0x7},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0fc2,0xc20f,0x2,0x0},
      {ATTR_MODRM,C_MOVNTI,D__My,D__Gy,D__NONE,"movnti",0xc30f,0x2,0x6},
      {ATTR_MODRM,C_GRPSSEFIX,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0fc4,0xc40f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0fc5,0xc50f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0fc6,0xc60f,0x2,0x0},
      {ATTR_MODRM,C_GRP7,D__NONE,D__NONE,D__NONE,(const char*)GroupG9,0xc70f,0x2,0x0},
      {0,C_BSWAP,D__RAX,D__NONE,D__NONE,"BSWAP",0xc80f,0x2,0x3},
      {0,C_BSWAP,D__RCX,D__NONE,D__NONE,"BSWAP",0xc90f,0x2,0x3},
      {0,C_BSWAP,D__RDX,D__NONE,D__NONE,"BSWAP",0xca0f,0x2,0x3},
      {0,C_BSWAP,D__RBX,D__NONE,D__NONE,"BSWAP",0xcb0f,0x2,0x3},
      {0,C_BSWAP,D__RSP,D__NONE,D__NONE,"BSWAP",0xcc0f,0x2,0x3},
      {0,C_BSWAP,D__RBP,D__NONE,D__NONE,"BSWAP",0xcd0f,0x2,0x3},
      {0,C_BSWAP,D__RSI,D__NONE,D__NONE,"BSWAP",0xce0f,0x2,0x3},
      {0,C_BSWAP,D__RDI,D__NONE,D__NONE,"BSWAP",0xcf0f,0x2,0x3},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Pq,D__NONE,(const char*)GroupSSE_0fd0,0xd00f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fd1,0xd10f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fd2,0xd20f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fd3,0xd30f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fd4,0xd40f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fd5,0xd50f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Wq,D__Vq,D__NONE,(const char*)GroupSSE_0fd6,0xd60f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Gd,D__Pq,D__NONE,(const char*)GroupSSE_0fd7,0xd70f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fd8,0xd80f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fd9,0xd90f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fda,0xda0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fdb,0xdb0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fdc,0xdc0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fdd,0xdd0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fde,0xde0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fdf,0xdf0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fe0,0xe00f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fe1,0xe10f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fe2,0xe20f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fe3,0xe30f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fe4,0xe40f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fe5,0xe50f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__NONE,D__NONE,D__NONE,(const char*)GroupSSE_0fe6,0xe60f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Wq,D__Vq,D__NONE,(const char*)GroupSSE_0fe7,0xe70f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fe8,0xe80f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fe9,0xe90f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fea,0xea0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0feb,0xeb0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fec,0xec0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fed,0xed0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fee,0xee0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0fef,0xef0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSEFIX,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ff0,0xf00f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ff1,0xf10f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ff2,0xf20f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ff3,0xf30f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ff4,0xf40f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ff5,0xf50f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ff6,0xf60f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Ppi,D__Qpi,D__NONE,(const char*)GroupSSE_0ff7,0xf70f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ff8,0xf80f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ff9,0xf90f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ffa,0xfa0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ffb,0xfb0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ffc,0xfc0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ffd,0xfd0f,0x2,0x0},
      {ATTR_MODRM,C_GRPSSE,D__Pq,D__Qq,D__NONE,(const char*)GroupSSE_0ffe,0xfe0f,0x2,0x0},
      {ATTR_MODRM,C_NULL,D__NONE,D__NONE,D__NONE,0,0x0,0x0,0x0},
};
_DIS_OPCODE_T Group0x63[] = {
      {ATTR_MODRM|ATTR_X64_INVALID,C_ARPL,D__Ew,D__Gw,D__NONE,"ARPL",0x63,0x1,0x6},
      {ATTR_MODRM|ATTR_X64_ONLY,C_MOVSXD,D__Gv,D__Ev,D__NONE,"MOVSXD",0x63,0x1,0x6},
};
_DIS_OPCODE_T Group_main[] = {
      {ATTR_MODRM,C_ADD,D__Eb,D__Gb,D__NONE,"ADD",0x0,0x1,0x7},
      {ATTR_MODRM,C_ADD,D__Ev,D__Gv,D__NONE,"ADD",0x1,0x1,0x7},
      {ATTR_MODRM,C_ADD,D__Gb,D_rEb,D__NONE,"ADD",0x2,0x1,0x7},
      {ATTR_MODRM,C_ADD,D__Gv,D_rEv,D__NONE,"ADD",0x3,0x1,0x7},
      {ATTR_B|ATTR_W,C_ADD,D__AL,D__Ib,D__NONE,"ADD",0x4,0x1,0x7},
      {ATTR_X64,C_ADD,D__rAX,D__Iz,D__NONE,"ADD",0x5,0x1,0x7},
      {ATTR_X64_INVALID,C_PUSH,D__ES,D__NONE,D__NONE,"PUSH",0x6,0x1,0x1},
      {ATTR_X64_INVALID,C_POP,D__ES,D__NONE,D__NONE,"POP",0x7,0x1,0x2},
      {ATTR_MODRM,C_OR,D__Eb,D__Gb,D__NONE,"OR",0x8,0x1,0x7},
      {ATTR_MODRM,C_OR,D__Ev,D__Gv,D__NONE,"OR",0x9,0x1,0x7},
      {ATTR_MODRM,C_OR,D__Gb,D_rEb,D__NONE,"OR",0xa,0x1,0x7},
      {ATTR_MODRM,C_OR,D__Gv,D_rEv,D__NONE,"OR",0xb,0x1,0x7},
      {ATTR_B|ATTR_W,C_OR,D__AL,D__Ib,D__NONE,"OR",0xc,0x1,0x7},
      {ATTR_X64,C_OR,D__rAX,D__Iz,D__NONE,"OR",0xd,0x1,0x7},
      {ATTR_X64_INVALID,C_PUSH,D__CS,D__NONE,D__NONE,"PUSH",0xe,0x1,0x1},
      {0,C_2BYTE,D__CS,D__NONE,D__NONE,(const char*)OpTable2Byte0f,0xf,0x1,0x0},
      {ATTR_MODRM,C_ADC,D__Eb,D__Gb,D__NONE,"ADC",0x10,0x1,0x7},
      {ATTR_MODRM,C_ADC,D__Ev,D__Gv,D__NONE,"ADC",0x11,0x1,0x7},
      {ATTR_MODRM,C_ADC,D__Gb,D_rEb,D__NONE,"ADC",0x12,0x1,0x7},
      {ATTR_MODRM,C_ADC,D__Gv,D_rEv,D__NONE,"ADC",0x13,0x1,0x7},
      {ATTR_B|ATTR_W,C_ADC,D__AL,D__Ib,D__NONE,"ADC",0x14,0x1,0x7},
      {ATTR_X64,C_ADC,D__rAX,D__Iz,D__NONE,"ADC",0x15,0x1,0x7},
      {ATTR_X64_INVALID,C_PUSH,D__SS,D__NONE,D__NONE,"PUSH",0x16,0x1,0x1},
      {ATTR_X64_INVALID,C_POP,D__SS,D__NONE,D__NONE,"POP",0x17,0x1,0x2},
      {ATTR_MODRM,C_SBB,D__Eb,D__Gb,D__NONE,"SBB",0x18,0x1,0x7},
      {ATTR_MODRM,C_SBB,D__Ev,D__Gv,D__NONE,"SBB",0x19,0x1,0x7},
      {ATTR_MODRM,C_SBB,D__Gb,D_rEb,D__NONE,"SBB",0x1a,0x1,0x7},
      {ATTR_MODRM,C_SBB,D__Gv,D_rEv,D__NONE,"SBB",0x1b,0x1,0x7},
      {ATTR_B|ATTR_W,C_SBB,D__AL,D__Ib,D__NONE,"SBB",0x1c,0x1,0x7},
      {ATTR_X64,C_SBB,D__rAX,D__Iz,D__NONE,"SBB",0x1d,0x1,0x7},
      {ATTR_X64_INVALID,C_PUSH,D__DS,D__NONE,D__NONE,"PUSH",0x1e,0x1,0x1},
      {ATTR_X64_INVALID,C_POP,D__DS,D__NONE,D__NONE,"POP",0x1f,0x1,0x2},
      {ATTR_MODRM,C_AND,D__Eb,D__Gb,D__NONE,"AND",0x20,0x1,0x7},
      {ATTR_MODRM,C_AND,D__Ev,D__Gv,D__NONE,"AND",0x21,0x1,0x7},
      {ATTR_MODRM,C_AND,D__Gb,D_rEb,D__NONE,"AND",0x22,0x1,0x7},
      {ATTR_MODRM,C_AND,D__Gv,D_rEv,D__NONE,"AND",0x23,0x1,0x7},
      {ATTR_B|ATTR_W,C_AND,D__AL,D__Ib,D__NONE,"AND",0x24,0x1,0x7},
      {ATTR_X64,C_AND,D__rAX,D__Iz,D__NONE,"AND",0x25,0x1,0x7},
      {0,C_ES,D__NONE,D__NONE,D__NONE,"ES:",0x26,0x1,0x0},
      {ATTR_X64_INVALID,C_DAA,D__NONE,D__NONE,D__NONE,"DAA",0x27,0x1,0x0},
      {ATTR_MODRM,C_SUB,D__Eb,D__Gb,D__NONE,"SUB",0x28,0x1,0x7},
      {ATTR_MODRM,C_SUB,D__Ev,D__Gv,D__NONE,"SUB",0x29,0x1,0x7},
      {ATTR_MODRM,C_SUB,D__Gb,D_rEb,D__NONE,"SUB",0x2a,0x1,0x7},
      {ATTR_MODRM,C_SUB,D__Gv,D_rEv,D__NONE,"SUB",0x2b,0x1,0x7},
      {ATTR_B|ATTR_W,C_SUB,D__AL,D__Ib,D__NONE,"SUB",0x2c,0x1,0x7},
      {ATTR_X64,C_SUB,D__rAX,D__Iz,D__NONE,"SUB",0x2d,0x1,0x7},
      {0,C_CS,D__NONE,D__NONE,D__NONE,"CS:",0x2e,0x1,0x0},
      {ATTR_X64_INVALID,C_DAS,D__NONE,D__NONE,D__NONE,"DAS",0x2f,0x1,0x0},
      {ATTR_MODRM,C_XOR,D__Eb,D__Gb,D__NONE,"XOR",0x30,0x1,0x7},
      {ATTR_MODRM,C_XOR,D__Ev,D__Gv,D__NONE,"XOR",0x31,0x1,0x7},
      {ATTR_MODRM,C_XOR,D__Gb,D_rEb,D__NONE,"XOR",0x32,0x1,0x7},
      {ATTR_MODRM,C_XOR,D__Gv,D_rEv,D__NONE,"XOR",0x33,0x1,0x7},
      {ATTR_B|ATTR_W,C_XOR,D__AL,D__Ib,D__NONE,"XOR",0x34,0x1,0x7},
      {ATTR_X64,C_XOR,D__rAX,D__Iz,D__NONE,"XOR",0x35,0x1,0x7},
      {0,C_SS,D__NONE,D__NONE,D__NONE,"SS:",0x36,0x1,0x0},
      {ATTR_X64_INVALID,C_AAA,D__NONE,D__NONE,D__NONE,"AAA",0x37,0x1,0x0},
      {ATTR_MODRM,C_CMP,D_rEb,D__Gb,D__NONE,"CMP",0x38,0x1,0x5},
      {ATTR_MODRM,C_CMP,D_rEv,D__Gv,D__NONE,"CMP",0x39,0x1,0x5},
      {ATTR_MODRM,C_CMP,D__Gb,D_rEb,D__NONE,"CMP",0x3a,0x1,0x5},
      {ATTR_MODRM,C_CMP,D__Gv,D_rEv,D__NONE,"CMP",0x3b,0x1,0x5},
      {ATTR_B|ATTR_W,C_CMP,D__AL,D__Ib,D__NONE,"CMP",0x3c,0x1,0x5},
      {ATTR_X64,C_CMP,D__rAX,D__Iz,D__NONE,"CMP",0x3d,0x1,0x5},
      {0,C_DS,D__NONE,D__NONE,D__NONE,"DS:",0x3e,0x1,0x0},
      {ATTR_X64_INVALID,C_AAS,D__NONE,D__NONE,D__NONE,"AAS",0x3f,0x1,0x0},
      {ATTR_X64_INVALID,C_INC,D__eAX,D__NONE,D__NONE,"INC",0x40,0x1,0x3},
      {ATTR_X64_INVALID,C_INC,D__eCX,D__NONE,D__NONE,"INC",0x41,0x1,0x3},
      {ATTR_X64_INVALID,C_INC,D__eDX,D__NONE,D__NONE,"INC",0x42,0x1,0x3},
      {ATTR_X64_INVALID,C_INC,D__eBX,D__NONE,D__NONE,"INC",0x43,0x1,0x3},
      {ATTR_X64_INVALID,C_INC,D__eSP,D__NONE,D__NONE,"INC",0x44,0x1,0x3},
      {ATTR_X64_INVALID,C_INC,D__eBP,D__NONE,D__NONE,"INC",0x45,0x1,0x3},
      {ATTR_X64_INVALID,C_INC,D__eSI,D__NONE,D__NONE,"INC",0x46,0x1,0x3},
      {ATTR_X64_INVALID,C_INC,D__eDI,D__NONE,D__NONE,"INC",0x47,0x1,0x3},
      {ATTR_X64_INVALID,C_DEC,D__eAX,D__NONE,D__NONE,"DEC",0x48,0x1,0x3},
      {ATTR_X64_INVALID,C_DEC,D__eCX,D__NONE,D__NONE,"DEC",0x49,0x1,0x3},
      {ATTR_X64_INVALID,C_DEC,D__eDX,D__NONE,D__NONE,"DEC",0x4a,0x1,0x3},
      {ATTR_X64_INVALID,C_DEC,D__eBX,D__NONE,D__NONE,"DEC",0x4b,0x1,0x3},
      {ATTR_X64_INVALID,C_DEC,D__eSP,D__NONE,D__NONE,"DEC",0x4c,0x1,0x3},
      {ATTR_X64_INVALID,C_DEC,D__eBP,D__NONE,D__NONE,"DEC",0x4d,0x1,0x3},
      {ATTR_X64_INVALID,C_DEC,D__eSI,D__NONE,D__NONE,"DEC",0x4e,0x1,0x3},
      {ATTR_X64_INVALID,C_DEC,D__eDI,D__NONE,D__NONE,"DEC",0x4f,0x1,0x3},
      {ATTR_SP,C_PUSH,D__erAX,D__NONE,D__NONE,"PUSH",0x50,0x1,0x1},
      {ATTR_SP,C_PUSH,D__erCX,D__NONE,D__NONE,"PUSH",0x51,0x1,0x1},
      {ATTR_SP,C_PUSH,D__erDX,D__NONE,D__NONE,"PUSH",0x52,0x1,0x1},
      {ATTR_SP,C_PUSH,D__erBX,D__NONE,D__NONE,"PUSH",0x53,0x1,0x1},
      {ATTR_SP,C_PUSH,D__erSP,D__NONE,D__NONE,"PUSH",0x54,0x1,0x1},
      {ATTR_SP,C_PUSH,D__erBP,D__NONE,D__NONE,"PUSH",0x55,0x1,0x1},
      {ATTR_SP,C_PUSH,D__erSI,D__NONE,D__NONE,"PUSH",0x56,0x1,0x1},
      {ATTR_SP,C_PUSH,D__erDI,D__NONE,D__NONE,"PUSH",0x57,0x1,0x1},
      {ATTR_SP,C_POP,D__erAX,D__NONE,D__NONE,"POP",0x58,0x1,0x2},
      {ATTR_SP,C_POP,D__erCX,D__NONE,D__NONE,"POP",0x59,0x1,0x2},
      {ATTR_SP,C_POP,D__erDX,D__NONE,D__NONE,"POP",0x5a,0x1,0x2},
      {ATTR_SP,C_POP,D__erBX,D__NONE,D__NONE,"POP",0x5b,0x1,0x2},
      {ATTR_SP,C_POP,D__erSP,D__NONE,D__NONE,"POP",0x5c,0x1,0x2},
      {ATTR_SP,C_POP,D__erBP,D__NONE,D__NONE,"POP",0x5d,0x1,0x2},
      {ATTR_SP,C_POP,D__erSI,D__NONE,D__NONE,"POP",0x5e,0x1,0x2},
      {ATTR_SP,C_POP,D__erDI,D__NONE,D__NONE,"POP",0x5f,0x1,0x2},
      {ATTR_X64_INVALID,C_PUSHA,D__NONE,D__NONE,D__NONE,"PUSHA",0x60,0x1,0x1},
      {ATTR_X64_INVALID,C_POPA,D__NONE,D__NONE,D__NONE,"POPA",0x61,0x1,0x2},
      {ATTR_MODRM|ATTR_X64_INVALID,C_BOUND,D__Gv,D_rMa,D__NONE,"BOUND",0x62,0x1,0x5},
      {ATTR_MODRM,C_GRPX64,D__NONE,D__NONE,D__NONE,(const char*)Group0x63,0x63,0x1,0x0},
      {0,C_FS,D__NONE,D__NONE,D__NONE,"FS:",0x64,0x1,0x0},
      {0,C_GS,D__NONE,D__NONE,D__NONE,"GS:",0x65,0x1,0x0},
      {0,C_OPER,D__AH,D__Cd,D__NONE,0,0x0,0x0,0x0},
      {0,C_ADDR,D__CH,D__Cd,D__NONE,0,0x0,0x0,0x0},
      {ATTR_X64|ATTR_SP,C_PUSH,D__Iz,D__NONE,D__NONE,"PUSH",0x68,0x1,0x1},
      {ATTR_X64|ATTR_MODRM,C_IMUL,D__Gv,D_rEv,D__Iz,"IMUL",0x69,0x1,0x17},
      {ATTR_D|ATTR_SP,C_PUSH,D_sIb,D__NONE,D__NONE,"PUSH",0x6a,0x1,0x1},
      {ATTR_D|ATTR_MODRM,C_IMUL,D__Gv,D_rEv,D_sIb,"IMUL",0x6b,0x1,0x17},
      {0,C_INS,D__Yb,D__DX,D__NONE,"INS",0x6c,0x1,0x6},
      {0,C_INS,D__Yz,D__DX,D__NONE,"INS",0x6d,0x1,0x6},
      {0,C_OUTS,D__DX,D__Xb,D__NONE,"OUTS",0x6e,0x1,0x5},
      {0,C_OUTS,D__DX,D__Xz,D__NONE,"OUTS",0x6f,0x1,0x5},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JtO,D__Jb,D__tO,D__NONE,"JO",0x70,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JfO,D__Jb,D__fO,D__NONE,"JNO",0x71,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JtC,D__Jb,D__tC,D__NONE,"JC",0x72,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JfC,D__Jb,D__fC,D__NONE,"JNC",0x73,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JtZ,D__Jb,D__tZ,D__NONE,"JZ",0x74,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JfZ,D__Jb,D__fZ,D__NONE,"JNZ",0x75,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JfA,D__Jb,D__fA,D__NONE,"JNA",0x76,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JtA,D__Jb,D__tA,D__NONE,"JA",0x77,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JfS,D__Jb,D__tS,D__NONE,"JS",0x78,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JtS,D__Jb,D__fS,D__NONE,"JNS",0x79,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JtP,D__Jb,D__tP,D__NONE,"JP",0x7a,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JfP,D__Jb,D__fP,D__NONE,"JNP",0x7b,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JtL,D__Jb,D__tL,D__NONE,"JL",0x7c,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JfL,D__Jb,D__fL,D__NONE,"JNL",0x7d,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JfG,D__Jb,D__fG,D__NONE,"JNG",0x7e,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JtG,D__Jb,D__tG,D__NONE,"JG",0x7f,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_GRP,D__Eb,D__Ib,D__NONE,(const char*)GroupG1_80,0x80,0x1,0x0},
      {ATTR_X64|ATTR_MODRM,C_GRP,D__Ev,D__Iz,D__NONE,(const char*)GroupG1_81,0x81,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_GRP,D__Eb,D__Ib,D__NONE,(const char*)GroupG1_82,0x82,0x1,0x0},
      {ATTR_D|ATTR_MODRM,C_GRP,D__Ev,D_sIb,D__NONE,(const char*)GroupG1_83,0x83,0x1,0x0},
      {ATTR_MODRM,C_TEST,D_rEb,D__Gb,D__NONE,"TEST",0x84,0x1,0x5},
      {ATTR_MODRM,C_TEST,D_rEv,D__Gv,D__NONE,"TEST",0x85,0x1,0x5},
      {ATTR_MODRM,C_XCHG,D__Gb,D__Eb,D__NONE,"XCHG",0x86,0x1,0xf},
      {ATTR_MODRM,C_XCHG,D__Gv,D__Ev,D__NONE,"XCHG",0x87,0x1,0xf},
      {ATTR_MODRM,C_MOV,D_wEb,D__Gb,D__NONE,"MOV",0x88,0x1,0x6},
      {ATTR_MODRM,C_MOV,D_wEv,D__Gv,D__NONE,"MOV",0x89,0x1,0x6},
      {ATTR_MODRM,C_MOV,D__Gb,D_rEb,D__NONE,"MOV",0x8a,0x1,0x6},
      {ATTR_MODRM,C_MOV,D__Gv,D_rEv,D__NONE,"MOV",0x8b,0x1,0x6},
      {ATTR_MODRM,C_MOV,D_wEw,D__Sw,D__NONE,"MOV",0x8c,0x1,0x6},
      {ATTR_MODRM,C_LEA,D__Gv,D__M,D__NONE,"LEA",0x8d,0x1,0x6},
      {ATTR_MODRM,C_MOV,D__Sw,D_rEw,D__NONE,"MOV",0x8e,0x1,0x6},
      {ATTR_MODRM,C_GRP,D__Ev,D__NONE,D__NONE,(const char*)GroupG1_8f,0x8f,0x1,0x0},
      {0,_C_XCHG,D__rAX,D__erAX,D__NONE,"XCHG",0x90,0x1,0x0},
      {0,C_XCHG,D__rAX,D__erCX,D__NONE,"XCHG",0x91,0x1,0xf},
      {0,C_XCHG,D__rAX,D__erDX,D__NONE,"XCHG",0x92,0x1,0xf},
      {0,C_XCHG,D__rAX,D__erBX,D__NONE,"XCHG",0x93,0x1,0xf},
      {0,C_XCHG,D__rAX,D__erSP,D__NONE,"XCHG",0x94,0x1,0xf},
      {0,C_XCHG,D__rAX,D__erBP,D__NONE,"XCHG",0x95,0x1,0xf},
      {0,C_XCHG,D__rAX,D__erSI,D__NONE,"XCHG",0x96,0x1,0xf},
      {0,C_XCHG,D__rAX,D__erDI,D__NONE,"XCHG",0x97,0x1,0xf},
      {0,C_CBW,D__NONE,D__NONE,D__NONE,"CBW",0x98,0x1,0x0},
      {0,C_CWD,D__NONE,D__NONE,D__NONE,"CWD",0x99,0x1,0x0},
      {ATTR_B|ATTR_X64|ATTR_X64_INVALID,C_CALL,D__Ap,D__NONE,D__NONE,"CALL",0x9a,0x1,0x1},
      {0,C_WAIT,D__NONE,D__NONE,D__NONE,"WAIT",0x9b,0x1,0x0},
      {ATTR_SP,C_PUSHF,D__Fv,D__NONE,D__NONE,"PUSHF",0x9c,0x1,0x1},
      {ATTR_SP,C_POPF,D__Fv,D__NONE,D__NONE,"POPF",0x9d,0x1,0x2},
      {0,C_SAHF,D__NONE,D__NONE,D__NONE,"SAHF",0x9e,0x1,0x0},
      {0,C_LAHF,D__NONE,D__NONE,D__NONE,"LAHF",0x9f,0x1,0x0},
      {0,C_MOV,D__AL,D_rOb,D__NONE,"MOV",0xa0,0x1,0x6},
      {0,C_MOV,D__rAX,D_rOv,D__NONE,"MOV",0xa1,0x1,0x6},
      {0,C_MOV,D_wOb,D__AL,D__NONE,"MOV",0xa2,0x1,0x6},
      {0,C_MOV,D_wOv,D__rAX,D__NONE,"MOV",0xa3,0x1,0x6},
      {0,C_MOVS,D_wYb,D_rXb,D__NONE,"MOVS",0xa4,0x1,0x6},
      {0,C_MOVS,D_wYv,D_rXv,D__NONE,"MOVS",0xa5,0x1,0x6},
      {0,C_CMPS,D_rXb,D_rYb,D__NONE,"CMPS",0xa6,0x1,0x5},
      {0,C_CMPS,D_rXv,D_rYv,D__NONE,"CMPS",0xa7,0x1,0x5},
      {ATTR_B|ATTR_W,C_TEST,D__AL,D__Ib,D__NONE,"TEST",0xa8,0x1,0x5},
      {ATTR_X64,C_TEST,D__rAX,D__Iz,D__NONE,"TEST",0xa9,0x1,0x5},
      {0,C_STOS,D_wYb,D__AL,D__NONE,"STOS",0xaa,0x1,0x6},
      {0,C_STOS,D_wYv,D__rAX,D__NONE,"STOS",0xab,0x1,0x6},
      {0,C_LODS,D__AL,D_rXb,D__NONE,"LODS",0xac,0x1,0x6},
      {0,C_LODS,D__rAX,D_rXv,D__NONE,"LODS",0xad,0x1,0x6},
      {0,C_SCAS,D_rYb,D__AL,D__NONE,"SCAS",0xae,0x1,0x5},
      {0,C_SCAS,D_rYv,D__rAX,D__NONE,"SCAS",0xaf,0x1,0x5},
      {ATTR_B|ATTR_W,C_MOV,D__RAL,D__Ib,D__NONE,"MOV",0xb0,0x1,0x6},
      {ATTR_B|ATTR_W,C_MOV,D__RCL,D__Ib,D__NONE,"MOV",0xb1,0x1,0x6},
      {ATTR_B|ATTR_W,C_MOV,D__RDL,D__Ib,D__NONE,"MOV",0xb2,0x1,0x6},
      {ATTR_B|ATTR_W,C_MOV,D__RBL,D__Ib,D__NONE,"MOV",0xb3,0x1,0x6},
      {ATTR_B|ATTR_W,C_MOV,D__RAH,D__Ib,D__NONE,"MOV",0xb4,0x1,0x6},
      {ATTR_B|ATTR_W,C_MOV,D__RCH,D__Ib,D__NONE,"MOV",0xb5,0x1,0x6},
      {ATTR_B|ATTR_W,C_MOV,D__RDH,D__Ib,D__NONE,"MOV",0xb6,0x1,0x6},
      {ATTR_B|ATTR_W,C_MOV,D__RBH,D__Ib,D__NONE,"MOV",0xb7,0x1,0x6},
      {ATTR_W|ATTR_D,C_MOV,D__erAX,D__Iv,D__NONE,"MOV",0xb8,0x1,0x6},
      {ATTR_W|ATTR_D,C_MOV,D__erCX,D__Iv,D__NONE,"MOV",0xb9,0x1,0x6},
      {ATTR_W|ATTR_D,C_MOV,D__erDX,D__Iv,D__NONE,"MOV",0xba,0x1,0x6},
      {ATTR_W|ATTR_D,C_MOV,D__erBX,D__Iv,D__NONE,"MOV",0xbb,0x1,0x6},
      {ATTR_W|ATTR_D,C_MOV,D__erSP,D__Iv,D__NONE,"MOV",0xbc,0x1,0x6},
      {ATTR_W|ATTR_D,C_MOV,D__erBP,D__Iv,D__NONE,"MOV",0xbd,0x1,0x6},
      {ATTR_W|ATTR_D,C_MOV,D__erSI,D__Iv,D__NONE,"MOV",0xbe,0x1,0x6},
      {ATTR_W|ATTR_D,C_MOV,D__erDI,D__Iv,D__NONE,"MOV",0xbf,0x1,0x6},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_GRP,D__NONE,D__NONE,D__NONE,(const char*)GroupG2Eb,0xc0,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_MODRM,C_GRP,D__NONE,D__NONE,D__NONE,(const char*)GroupG2Ew,0xc1,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_JMP,C_RET,D__Iw,D__NONE,D__NONE,"RET",0xc2,0x1,0x1},
      {ATTR_JMP,C_RET,D__NONE,D__NONE,D__NONE,"RET",0xc3,0x1,0x0},
      {ATTR_MODRM|ATTR_X64_INVALID,C_LES,D__Gv,D_rMp,D__NONE,"LES",0xc4,0x1,0x6},
      {ATTR_MODRM|ATTR_X64_INVALID,C_LDS,D__Gv,D_rMp,D__NONE,"LDS",0xc5,0x1,0x6},
      {ATTR_MODRM,C_GRP,D__NONE,D__NONE,D__NONE,(const char*)GroupG11Eb,0xc6,0x1,0x0},
      {ATTR_MODRM,C_GRP,D__NONE,D__NONE,D__NONE,(const char*)GroupG11Ew,0xc7,0x1,0x0},
      {ATTR_W|ATTR_X64,C_ENTER,D__Iw,D__Ib,D__NONE,"ENTER",0xc8,0x1,0x5},
      {ATTR_SP,C_LEAVE,D__NONE,D__NONE,D__NONE,"LEAVE",0xc9,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D,C_RETF,D__Iw,D__NONE,D__NONE,"RETF",0xca,0x1,0x1},
      {0,C_RETF,D__NONE,D__NONE,D__NONE,"RETF",0xcb,0x1,0x0},
      {0,C_INT3,D__NONE,D__NONE,D__NONE,"INT3",0xcc,0x1,0x0},
      {ATTR_B|ATTR_W,C_INT,D__Ib,D__NONE,D__NONE,"INT",0xcd,0x1,0x1},
      {ATTR_X64_INVALID,C_INTO,D__NONE,D__NONE,D__NONE,"INTO",0xce,0x1,0x0},
      {0,C_IRET,D__NONE,D__NONE,D__NONE,"IRET",0xcf,0x1,0x0},
      {ATTR_MODRM,C_GRP,D__Eb,D__1,D__NONE,(const char*)GroupG2EbI1,0xd0,0x1,0x0},
      {ATTR_MODRM,C_GRP,D__Ev,D__1,D__NONE,(const char*)GroupG2EvI1,0xd1,0x1,0x0},
      {ATTR_MODRM,C_GRP,D__Eb,D__CL,D__NONE,(const char*)GroupG2EbCL,0xd2,0x1,0x0},
      {ATTR_MODRM,C_GRP,D__Ev,D__CL,D__NONE,(const char*)GroupG2EvCL,0xd3,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_X64_INVALID,C_AAM,D__Ib,D__NONE,D__NONE,"AAM",0xd4,0x1,0x1},
      {ATTR_B|ATTR_W|ATTR_X64_INVALID,C_AAD,D__Ib,D__NONE,D__NONE,"AAD",0xd5,0x1,0x1},
      {0,C_SALC,D__NONE,D__NONE,D__NONE,"SALC",0xd6,0x1,0x0},
      {0,C_XLAT,D__NONE,D__NONE,D__NONE,"XLAT",0xd7,0x1,0x0},
      {ATTR_MODRM,C_ESC,D__NONE,D__NONE,D__NONE,(const char*)FPGroupd8,0xd8,0x1,0x0},
      {ATTR_MODRM,C_ESC,D__NONE,D__NONE,D__NONE,(const char*)FPGroupd9,0xd9,0x1,0x0},
      {ATTR_MODRM,C_ESC,D__NONE,D__NONE,D__NONE,(const char*)FPGroupda,0xda,0x1,0x0},
      {ATTR_MODRM,C_ESC,D__NONE,D__NONE,D__NONE,(const char*)FPGroupdb,0xdb,0x1,0x0},
      {ATTR_MODRM,C_ESC,D__NONE,D__NONE,D__NONE,(const char*)FPGroupdc,0xdc,0x1,0x0},
      {ATTR_MODRM,C_ESC,D__NONE,D__NONE,D__NONE,(const char*)FPGroupdd,0xdd,0x1,0x0},
      {ATTR_MODRM,C_ESC,D__NONE,D__NONE,D__NONE,(const char*)FPGroupde,0xde,0x1,0x0},
      {ATTR_MODRM,C_ESC,D__NONE,D__NONE,D__NONE,(const char*)FPGroupdf,0xdf,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64,C_LOOPNZ,D__Jb,D__NONE,D__NONE,"LOOPNZ",0xe0,0x1,0x1},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64,C_LOOPZ,D__Jb,D__NONE,D__NONE,"LOOPZ",0xe1,0x1,0x1},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64,C_LOOP,D__Jb,D__NONE,D__NONE,"LOOP",0xe2,0x1,0x1},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64,C_JCXZ,D__Jb,D__NONE,D__NONE,"JCXZ",0xe3,0x1,0x1},
      {ATTR_B|ATTR_W,C_IN,D__AL,D__Ib,D__NONE,"IN",0xe4,0x1,0x6},
      {ATTR_B|ATTR_W,C_IN,D__eAX,D__Ib,D__NONE,"IN",0xe5,0x1,0x6},
      {ATTR_B|ATTR_W,C_OUT,D__Ib,D__AL,D__NONE,"OUT",0xe6,0x1,0x5},
      {ATTR_B|ATTR_W,C_OUT,D__Ib,D__eAX,D__NONE,"OUT",0xe7,0x1,0x5},
      {ATTR_JMP,C_CALL,D__Jz,D__NONE,D__NONE,"CALL",0xe8,0x1,0x1},
      {ATTR_JMP,C_JMP,D__Jz,D__NONE,D__NONE,"JMP",0xe9,0x1,0x1},
      {ATTR_B|ATTR_X64|ATTR_X64_INVALID,C_JMP,D__Ap,D__NONE,D__NONE,"JMP",0xea,0x1,0x1},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64|ATTR_JMP,C_JMP,D__Jb,D__NONE,D__NONE,"JMP",0xeb,0x1,0x1},
      {0,C_IN,D__AL,D__DX,D__NONE,"IN",0xec,0x1,0x6},
      {0,C_IN,D__eAX,D__DX,D__NONE,"IN",0xed,0x1,0x6},
      {0,C_OUT,D__DX,D__AL,D__NONE,"OUT",0xee,0x1,0x5},
      {0,C_OUT,D__DX,D__eAX,D__NONE,"OUT",0xef,0x1,0x5},
      {0,C_LOCK,D__NONE,D__NONE,D__NONE,"LOCK",0xf0,0x1,0x0},
      {0,C_INT1,D__NONE,D__NONE,D__NONE,"INT1",0xf1,0x1,0x0},
      {0,C_REPN,D__NONE,D__NONE,D__NONE,"REPNE",0xf2,0x1,0x0},
      {0,C_REPZ,D__NONE,D__NONE,D__NONE,"REP",0xf3,0x1,0x0},
      {0,C_HLT,D__NONE,D__NONE,D__NONE,"HLT",0xf4,0x1,0x0},
      {0,C_CMC,D__NONE,D__NONE,D__NONE,"CMC",0xf5,0x1,0x0},
      {ATTR_MODRM,C_GRP,D__Eb,D__NONE,D__NONE,(const char*)GroupG3Eb,0xf6,0x1,0x0},
      {ATTR_MODRM,C_GRP,D__Ev,D__NONE,D__NONE,(const char*)GroupG3Ev,0xf7,0x1,0x0},
      {0,C_CLC,D__NONE,D__NONE,D__NONE,"CLC",0xf8,0x1,0x0},
      {0,C_STC,D__NONE,D__NONE,D__NONE,"STC",0xf9,0x1,0x0},
      {0,C_CLI,D__NONE,D__NONE,D__NONE,"CLI",0xfa,0x1,0x0},
      {0,C_STI,D__NONE,D__NONE,D__NONE,"STI",0xfb,0x1,0x0},
      {0,C_CLD,D__NONE,D__NONE,D__NONE,"CLD",0xfc,0x1,0x0},
      {0,C_STD,D__NONE,D__NONE,D__NONE,"STD",0xfd,0x1,0x0},
      {ATTR_MODRM,C_GRP,D__Eb,D__NONE,D__NONE,(const char*)GroupG4,0xfe,0x1,0x0},
      {ATTR_MODRM,C_GRP,D__Ev,D__NONE,D__NONE,(const char*)GroupG5v,0xff,0x1,0x0},
      {ATTR_MODRM,C_INS,D__Yb,D__DX,D__NONE,"INSB",0x6c,0x1,0x6},
      {ATTR_MODRM,C_INS,D__Yw,D__DX,D__NONE,"INSW",0x6d,0x1,0x6},
      {ATTR_MODRM,C_INS,D__Yd,D__DX,D__NONE,"INSD",0x6d,0x1,0x6},
      {ATTR_MODRM,C_OUTS,D__DX,D__Xb,D__NONE,"OUTSB",0x6e,0x1,0x5},
      {ATTR_MODRM,C_OUTS,D__DX,D__Xw,D__NONE,"OUTSW",0x6f,0x1,0x5},
      {ATTR_MODRM,C_OUTS,D__DX,D__Xd,D__NONE,"OUTSD",0x6f,0x1,0x5},
      {ATTR_SP,C_PUSHF,D__NONE,D__NONE,D__NONE,"PUSHFD",0x9c,0x1,0x1},
      {ATTR_SP,C_POPF,D__NONE,D__NONE,D__NONE,"POPFD",0x9d,0x1,0x2},
      {0,C_PUSHA,D__NONE,D__NONE,D__NONE,"PUSHAD",0x60,0x1,0x1},
      {0,C_POPA,D__NONE,D__NONE,D__NONE,"POPAD",0x61,0x1,0x2},
      {ATTR_MODRM,C_MOVS,D__Yb,D__NONE,D__NONE,"MOVSB",0xa4,0x1,0x6},
      {ATTR_MODRM,C_MOVS,D__Yw,D__NONE,D__NONE,"MOVSW",0xa5,0x1,0x6},
      {ATTR_MODRM,C_MOVS,D__Yd,D__NONE,D__NONE,"MOVSD",0xa5,0x1,0x6},
      {ATTR_MODRM,C_MOVS,D__Yd,D__NONE,D__NONE,"MOVSQ",0xa5,0x1,0x6},
      {ATTR_MODRM,C_CMPS,D__Yb,D__NONE,D__NONE,"CMPSB",0xa6,0x1,0x5},
      {ATTR_MODRM,C_CMPS,D__Yw,D__NONE,D__NONE,"CMPSW",0xa7,0x1,0x5},
      {ATTR_MODRM,C_CMPS,D__Yd,D__NONE,D__NONE,"CMPSD",0xa7,0x1,0x5},
      {ATTR_MODRM,C_CMPS,D__Yd,D__NONE,D__NONE,"CMPSQ",0xa7,0x1,0x5},
      {ATTR_MODRM,C_STOS,D__Yb,D__NONE,D__NONE,"STOSB",0xaa,0x1,0x2},
      {ATTR_MODRM,C_STOS,D__Yw,D__NONE,D__NONE,"STOSW",0xab,0x1,0x2},
      {ATTR_MODRM,C_STOS,D__Yd,D__NONE,D__NONE,"STOSD",0xab,0x1,0x2},
      {ATTR_MODRM,C_STOS,D__Yd,D__NONE,D__NONE,"STOSQ",0xab,0x1,0x2},
      {ATTR_MODRM,C_LODS,D__AL,D__Xb,D__NONE,"LODSB",0xac,0x1,0x6},
      {ATTR_MODRM,C_LODS,D__AX,D__Xw,D__NONE,"LODSW",0xad,0x1,0x6},
      {ATTR_MODRM,C_LODS,D__EAX,D__Xd,D__NONE,"LODSD",0xad,0x1,0x6},
      {ATTR_MODRM,C_LODS,D__EAX,D__Xd,D__NONE,"LODSQ",0xad,0x1,0x6},
      {ATTR_MODRM,C_SCAS,D__Yb,D__NONE,D__NONE,"SCASB",0xae,0x1,0x5},
      {ATTR_MODRM,C_SCAS,D__Yw,D__NONE,D__NONE,"SCASW",0xaf,0x1,0x5},
      {ATTR_MODRM,C_SCAS,D__Yd,D__NONE,D__NONE,"SCASD",0xaf,0x1,0x5},
      {ATTR_MODRM,C_SCAS,D__Yd,D__NONE,D__NONE,"SCASQ",0xaf,0x1,0x5},
      {0,C_XLAT,D__NONE,D__NONE,D__NONE,"XLATB",0xd7,0x1,0x0},
      {0,C_REPN,D__NONE,D__NONE,D__NONE,"REPNZ",0xf2,0x1,0x0},
      {0,C_REPZ,D__NONE,D__NONE,D__NONE,"REPZ",0xf3,0x1,0x0},
      {0,C_REPZ,D__NONE,D__NONE,D__NONE,"REPE",0xf3,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64,C_JtC,D__Jb,D__NONE,D__NONE,"JB",0x72,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64,C_JtC,D__Jb,D__NONE,D__NONE,"JNAE",0x72,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64,C_JfC,D__Jb,D__NONE,D__NONE,"JAE",0x73,0x1,0x0},
      {ATTR_B|ATTR_W,C_JfC,D__Jb,D__NONE,D__NONE,"JNB",0x73,0x1,0x0},
      {ATTR_B|ATTR_W,C_JtZ,D__Jb,D__NONE,D__NONE,"JE",0x74,0x1,0x0},
      {ATTR_B|ATTR_W,C_JfZ,D__Jb,D__NONE,D__NONE,"JNE",0x75,0x1,0x0},
      {ATTR_B|ATTR_W,C_JfA,D__Jb,D__NONE,D__NONE,"JBE",0x76,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64,C_JtA,D__Jb,D__NONE,D__NONE,"JNBE",0x77,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64,C_JtP,D__Jb,D__NONE,D__NONE,"JPE",0x7a,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64,C_JfP,D__Jb,D__NONE,D__NONE,"JPO",0x7b,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64,C_JtL,D__Jb,D__NONE,D__NONE,"JNGE",0x7c,0x1,0x0},
      {ATTR_B|ATTR_W,C_JfL,D__Jb,D__NONE,D__NONE,"JGE",0x7d,0x1,0x0},
      {ATTR_B|ATTR_W,C_JfG,D__Jb,D__NONE,D__NONE,"JLE",0x7e,0x1,0x0},
      {ATTR_B|ATTR_W,C_JtG,D__Jb,D__NONE,D__NONE,"JNLE",0x7f,0x1,0x0},
      {ATTR_JMP,C_JtC,D__Jz,D__tC,D__NONE,"JB",0x820f,0x2,0x0},
      {ATTR_JMP,C_JtC,D__Jz,D__tC,D__NONE,"JNAE",0x820f,0x2,0x0},
      {ATTR_JMP,C_JfC,D__Jz,D__fC,D__NONE,"JNB",0x830f,0x2,0x0},
      {ATTR_JMP,C_JfC,D__Jz,D__fC,D__NONE,"JAE",0x830f,0x2,0x0},
      {ATTR_JMP,C_JtZ,D__Jz,D__tZ,D__NONE,"JE",0x840f,0x2,0x0},
      {ATTR_JMP,C_JfZ,D__Jz,D__fZ,D__NONE,"JNE",0x850f,0x2,0x0},
      {ATTR_JMP,C_JfA,D__Jz,D__fA,D__NONE,"JBE",0x860f,0x2,0x0},
      {ATTR_JMP,C_JtA,D__Jz,D__tA,D__NONE,"JNBE",0x870f,0x2,0x0},
      {ATTR_JMP,C_JtP,D__Jz,D__tP,D__NONE,"JPE",0x8a0f,0x2,0x0},
      {ATTR_JMP,C_JfP,D__Jz,D__tS,D__NONE,"JPO",0x8b0f,0x2,0x0},
      {ATTR_JMP,C_JtL,D__Jz,D__tL,D__NONE,"JNGE",0x8c0f,0x2,0x0},
      {ATTR_JMP,C_JfL,D__Jz,D__fL,D__NONE,"JGE",0x8d0f,0x2,0x0},
      {ATTR_JMP,C_JfG,D__Jz,D__fG,D__NONE,"JLE",0x8e0f,0x2,0x0},
      {ATTR_JMP,C_JtG,D__Jz,D__tG,D__NONE,"JNLE",0x8f0f,0x2,0x0},
      {ATTR_MODRM,C_SETtC,D_wEb,D__tC,D__NONE,"SETB",0x920f,0x2,0x1},
      {ATTR_MODRM,C_SETtC,D_wEb,D__tC,D__NONE,"SETNAE",0x920f,0x2,0x1},
      {ATTR_MODRM,C_SETfC,D_wEb,D__fC,D__NONE,"SETNB",0x930f,0x2,0x1},
      {ATTR_MODRM,C_SETfC,D_wEb,D__fC,D__NONE,"SETAE",0x930f,0x2,0x1},
      {ATTR_MODRM,C_SETtZ,D_wEb,D__tZ,D__NONE,"SETE",0x940f,0x2,0x1},
      {ATTR_MODRM,C_SETfZ,D_wEb,D__fZ,D__NONE,"SETNE",0x950f,0x2,0x1},
      {ATTR_MODRM,C_SETfA,D_wEb,D__fA,D__NONE,"SETBE",0x960f,0x2,0x1},
      {ATTR_MODRM,C_SETtA,D_wEb,D__tA,D__NONE,"SETNBE",0x970f,0x2,0x1},
      {ATTR_MODRM,C_SETtP,D_wEb,D__tP,D__NONE,"SETPE",0x9a0f,0x2,0x1},
      {ATTR_MODRM,C_SETfP,D_wEb,D__tS,D__NONE,"SETPO",0x9b0f,0x2,0x1},
      {ATTR_MODRM,C_SETtL,D_wEb,D__tL,D__NONE,"SETNGE",0x9c0f,0x2,0x1},
      {ATTR_MODRM,C_SETfL,D_wEb,D__fL,D__NONE,"SETGE",0x9d0f,0x2,0x1},
      {ATTR_MODRM,C_SETfG,D_wEb,D__fG,D__NONE,"SETLE",0x9e0f,0x2,0x1},
      {ATTR_MODRM,C_SETtG,D_wEb,D__tG,D__NONE,"SETNLE",0x9f0f,0x2,0x1},
      {ATTR_MODRM,C_CMOVtC,D__Gv,D_rEv,D__tC,"CMOVNAE",0x420f,0x2,0x7},
      {ATTR_MODRM,C_CMOVtC,D__Gv,D_rEv,D__tC,"CMOVC",0x420f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfC,D__Gv,D_rEv,D__fC,"CMOVNC",0x430f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfC,D__Gv,D_rEv,D__fC,"CMOVAE",0x430f,0x2,0x7},
      {ATTR_MODRM,C_CMOVtZ,D__Gv,D_rEv,D__tZ,"CMOVE",0x440f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfZ,D__Gv,D_rEv,D__fZ,"CMOVNE",0x450f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfA,D__Gv,D_rEv,D__fA,"CMOVNA",0x460f,0x2,0x7},
      {ATTR_MODRM,C_CMOVtA,D__Gv,D_rEv,D__tA,"CMOVNBE",0x470f,0x2,0x7},
      {ATTR_MODRM,C_CMOVtP,D__Gv,D_rEv,D__tP,"CMOVPE",0x4a0f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfP,D__Gv,D_rEv,D__tS,"CMOVPO",0x4b0f,0x2,0x7},
      {ATTR_MODRM,C_CMOVtL,D__Gv,D_rEv,D__tL,"CMOVNGE",0x4c0f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfL,D__Gv,D_rEv,D__fL,"CMOVNL",0x4d0f,0x2,0x7},
      {ATTR_MODRM,C_CMOVfG,D__Gv,D_rEv,D__fG,"CMOVNG",0x4e0f,0x2,0x7},
      {ATTR_MODRM,C_CMOVtG,D__Gv,D_rEv,D__tG,"CMOVNLE",0x4f0f,0x2,0x7},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64,C_LOOPNZ,D__Jb,D__NONE,D__NONE,"LOOPNE",0xe0,0x1,0x0},
      {ATTR_B|ATTR_W|ATTR_D|ATTR_X64,C_LOOPZ,D__Jb,D__NONE,D__NONE,"LOOPE",0xe1,0x1,0x0},
      {0,C_NOP,D__NONE,D__NONE,D__NONE,"NOP",0x90,0x1,0x0},
      {ATTR_MODRM,C_DBYTE,D_DBYTE,D__NONE,D__NONE,"DB",0x0,0x0,0x0},
      {ATTR_MODRM,C_DWORD,D_DWORD,D__NONE,D__NONE,"DW",0x0,0x0,0x0},
      {ATTR_MODRM,C_DDWORD,D_DDWORD,D__NONE,D__NONE,"DD",0x0,0x0,0x0},
      {ATTR_MODRM,C_DBYTE,D_STRING,D__NONE,D__NONE,"DB",0x0,0x0,0x0},
      {0,C_TAIL,D__NONE,D__NONE,D__NONE,"",0x0,0x0,0x0},
};

#define SSE_PREFIX_NONE 0
#define SSE_PREFIX_66   1
#define SSE_PREFIX_F3   3
#define SSE_PREFIX_F2   2

#define OPCODE(entry) ((_DIS_OPCODE_T*)(entry)->name)

#define MODRM_REG(x) (((x)>>3)&0x7)

//extern
int Disassembly(_INSTRUCTION_INFORMATION *pInstInfo, unsigned char *inbuff, unsigned long long eip, int mode, DIS_CPU *DisCPU)
{

	DIS_CPU DisCPU_T;
	if (!DisCPU)
		DisCPU = &DisCPU_T;

	unsigned int ByteCount = 0;
	unsigned int Segment = -1;

	_DISP_INSTRUCTION_T disp_i;
	disp_i.opcount = 1;
	disp_i.as_32 = mode;
	disp_i.os_32 = mode;
	disp_i.osx_32 = mode;
	if (mode == CODE_MODE_64)
		disp_i.os_32 = CODE_MODE_32;

	int os_32 = disp_i.os_32;

	disp_i.rep_used = -1;
	disp_i.lock = -1;
	disp_i.x64 = 0;
	disp_i.sse_prefix = SSE_PREFIX_NONE;
	disp_i.prefixes = 0;
	disp_i.regrm = -1;
	disp_i.sib1 = -1;
	disp_i.rex_prefix = 0;

	pInstInfo->AMD3DNow = -1;

	_DIS_OPCODE_T *pIntelOpcode;

	for (;;)
	{
		if (ByteCount >= 15)
			return 0;

		disp_i.b1 = inbuff[ByteCount++];
		pIntelOpcode = &Group_main[disp_i.b1];

		if (mode == CODE_MODE_64 && disp_i.b1 >= 0x40 && disp_i.b1 <= 0x4F)// INC DEC
		{
			disp_i.rex_prefix = disp_i.b1;
			pIntelOpcode = &Group_main[sizeof(Group_main)/sizeof(Group_main[0])-1];
		}
	again:
		switch (pIntelOpcode->OpcodeType)
		{
		case C_ES:
		case C_CS:
		case C_SS:
		case C_DS:
		case C_FS:
		case C_GS:
			++disp_i.prefixes;
			Segment = pIntelOpcode->OpcodeType - 1;
			disp_i.rex_prefix = 0;
			disp_i.sse_prefix = SSE_PREFIX_NONE;
			continue;
		case C_TAIL:
			++disp_i.prefixes;
			if (disp_i.rex_prefix & 8)
			{
				disp_i.os_32 = CODE_MODE_64;
				os_32 = CODE_MODE_64;
			}
			continue;
		case C_OPER:
			if (mode == CODE_MODE_16)
				disp_i.os_32 = CODE_MODE_32;
			else
			if (mode == CODE_MODE_32)
				disp_i.os_32 = CODE_MODE_16;
			else
			if (mode == CODE_MODE_64)
				disp_i.os_32 = CODE_MODE_16;
			++disp_i.prefixes;
			disp_i.sse_prefix = SSE_PREFIX_66;
			disp_i.rex_prefix = 0;
			continue;
		case C_ADDR:
			if (mode == CODE_MODE_16)
				disp_i.as_32 = CODE_MODE_32;
			else
			if (mode == CODE_MODE_32)
				disp_i.as_32 = CODE_MODE_16;
			else
			if (mode == CODE_MODE_64)
				disp_i.as_32 = CODE_MODE_32;
			++disp_i.prefixes;
			disp_i.sse_prefix = SSE_PREFIX_NONE;
			disp_i.rex_prefix = 0;
			continue;
		case C_LOCK:
			disp_i.lock = 0xF0;
			++disp_i.prefixes;
			disp_i.sse_prefix = SSE_PREFIX_NONE;
			disp_i.rex_prefix = 0;
			continue;
		case C_REPN:
			++disp_i.prefixes;
			disp_i.rep_used = 0xF2;
			disp_i.sse_prefix = SSE_PREFIX_F2;
			disp_i.rex_prefix = 0;
			continue;
		case C_REPZ:
			disp_i.rep_used = 0xF3;
			disp_i.sse_prefix = SSE_PREFIX_F3;
			++disp_i.prefixes;
			disp_i.rex_prefix = 0;
			continue;
		case C_GRP:
			pIntelOpcode = &(OPCODE(pIntelOpcode)[MODRM_REG((int)inbuff[ByteCount])]);
			break;
		case C_ESC:
			disp_i.opcount = 2;
			if ((int)inbuff[ByteCount] >= 0xC0)
				pIntelOpcode = &(OPCODE(pIntelOpcode)[inbuff[ByteCount++] - 0xB8]);
			else	pIntelOpcode = &(OPCODE(pIntelOpcode)[MODRM_REG((int)inbuff[ByteCount])]);
			break;
		case C_2BYTE:
			disp_i.opcount = 2;
			disp_i.b1 = inbuff[ByteCount++];
			pIntelOpcode = &(OPCODE(pIntelOpcode)[disp_i.b1]);
			goto again;
		case C_GRPMOD:
			if ((int)inbuff[ByteCount] >= 0xC0)
				pIntelOpcode = &(OPCODE(pIntelOpcode)[MODRM_REG((int)inbuff[ByteCount]) + 8]);
			else	pIntelOpcode = &(OPCODE(pIntelOpcode)[MODRM_REG((int)inbuff[ByteCount])]);
			break;
		case C_GRPRM:
			if ((int)inbuff[ByteCount] >= 0xC0)
				pIntelOpcode = &(OPCODE(pIntelOpcode)[MODRM_REG((int)inbuff[ByteCount]) * 8 + 8 + (inbuff[ByteCount] & 7)]);
			else	pIntelOpcode = &(OPCODE(pIntelOpcode)[MODRM_REG((int)inbuff[ByteCount])]);
			break;
		case C_3BYTE_1:
			disp_i.opcount = 3;
			disp_i.b1 = inbuff[ByteCount++];
			pIntelOpcode = &(OPCODE(pIntelOpcode)[disp_i.b1]);
			goto again;
		case C_3BYTE_2:
			disp_i.opcount = 3;
			disp_i.b1 = inbuff[ByteCount++];
			pIntelOpcode = &(OPCODE(pIntelOpcode)[disp_i.b1]);
			goto again;
		case C_GRPSSE:
			pIntelOpcode = &(OPCODE(pIntelOpcode)[disp_i.sse_prefix]);
			if (disp_i.sse_prefix == SSE_PREFIX_F2 ||
			    disp_i.sse_prefix == SSE_PREFIX_F3)
				disp_i.rep_used = -1;
			else
			if (disp_i.sse_prefix == SSE_PREFIX_66)
				disp_i.os_32 = os_32;
			break;
		case C_GRPSSEFIX:
			if ((int)inbuff[ByteCount] >= 0xC0)
				pIntelOpcode = &(OPCODE(pIntelOpcode)[disp_i.sse_prefix + 4]);
			else	pIntelOpcode = &(OPCODE(pIntelOpcode)[disp_i.sse_prefix]);
			if (disp_i.sse_prefix == SSE_PREFIX_F2 ||
			    disp_i.sse_prefix == SSE_PREFIX_F3)
				disp_i.rep_used = -1;
			else
			if (disp_i.sse_prefix == SSE_PREFIX_66)
				disp_i.os_32 = os_32;
			break;
		case C_GRP7:
			pIntelOpcode = &(OPCODE(pIntelOpcode)[MODRM_REG((int)inbuff[ByteCount])]);
			if (pIntelOpcode->OpcodeType == C_GRPSSE)
			{
				pIntelOpcode = &(OPCODE(pIntelOpcode)[disp_i.sse_prefix]);
				if (disp_i.sse_prefix == SSE_PREFIX_F2 ||
				    disp_i.sse_prefix == SSE_PREFIX_F3)
					disp_i.rep_used = -1;
				else
				if (disp_i.sse_prefix == SSE_PREFIX_66)
					disp_i.os_32 = os_32;
			}
			break;
		case C_GRPX64:
			if (mode == CODE_MODE_64)
				pIntelOpcode = OPCODE(pIntelOpcode)+1;
			else	pIntelOpcode = OPCODE(pIntelOpcode);
			break;
		default:
			break;
		}
		break;
	}

	if (!pIntelOpcode->name)
		return 0;

	if (pIntelOpcode->Attr & ATTR_X64_INVALID && mode == CODE_MODE_64)
		return 0;

	if (pIntelOpcode->Attr & ATTR_X64_ONLY && mode != CODE_MODE_64)
		return 0;

	if (pIntelOpcode->Attr & ATTR_SP && mode == CODE_MODE_64 && disp_i.os_32 == CODE_MODE_32)
		disp_i.os_32 = CODE_MODE_64;

	if (pIntelOpcode->Attr & ATTR_JMP && mode == CODE_MODE_64)
		disp_i.os_32 = CODE_MODE_64;

	disp_i.attr = pIntelOpcode->Attr;
	disp_i.destination = pIntelOpcode->Opdata1;
	disp_i.source = pIntelOpcode->Opdata2;
	disp_i.other = pIntelOpcode->Opdata3;

	if (disp_i.attr & ATTR_MODRM)
	{
		if (disp_i.as_32 == CODE_MODE_16)
			ByteCount += GetEffectAddress16Dis(DisCPU, &inbuff[ByteCount], &disp_i);
		else	ByteCount += GetEffectAddress32Dis(DisCPU, &inbuff[ByteCount], &disp_i);
	}

	ByteCount += ProcessOpdata_immde(disp_i.destination, &inbuff[ByteCount], &disp_i);
	ByteCount += ProcessOpdata_immde(disp_i.source, &inbuff[ByteCount], &disp_i);
	ByteCount += ProcessOpdata_immde(disp_i.other, &inbuff[ByteCount], &disp_i);
	
	if (ByteCount >= 15)
		return 0;

	disp_i.ilen = ByteCount;

	pInstInfo->OpCode = pIntelOpcode->OpcodeType;
	pInstInfo->OpType = 0;
	pInstInfo->CodeType = 0;
	pInstInfo->Length = disp_i.ilen;
	pInstInfo->Repeat = disp_i.rep_used;
	pInstInfo->Lock = disp_i.lock;
	pInstInfo->OpCount = disp_i.opcount;
	pInstInfo->Prefixes = disp_i.prefixes;
	pInstInfo->RegRM = disp_i.regrm;
	pInstInfo->Sib1 = disp_i.sib1;
	pInstInfo->RexPrefix = disp_i.rex_prefix;

	if (!ProcessName_dis(pInstInfo, pIntelOpcode, disp_i.os_32, disp_i.as_32))
		return 0;


        if (ProcessOpdata_dis(pIntelOpcode->Opdata1, &pInstInfo->op[0], eip, &disp_i, DisCPU) == -1)
		return 0;

        if (ProcessOpdata_dis(pIntelOpcode->Opdata2, &pInstInfo->op[1], eip, &disp_i, DisCPU) == -1)
		return 0;

        if (ProcessOpdata_dis(pIntelOpcode->Opdata3, &pInstInfo->op[2], eip, &disp_i, DisCPU) == -1)
		return 0;

	if (disp_i.seg == Segment || Segment == -1)
	{
		pInstInfo->SegmentPrefix = -1;
		for (int j = 0; j < 3; ++j)
		{
			if (pInstInfo->op[j].mode == OP_Address)
				pInstInfo->op[j].addr.line_address = pInstInfo->op[j].addr.effect_address + DisCPU->SEG_BASE[disp_i.seg];
		}
	} else
	{
		pInstInfo->SegmentPrefix = Segment;
		for (int j = 0; j < 3; ++j)
		{
			if (pInstInfo->op[j].mode == OP_Address)
				pInstInfo->op[j].addr.line_address = pInstInfo->op[j].addr.effect_address + DisCPU->SEG_BASE[Segment];
		}
	}

	pInstInfo->xxxxptr = -1;

	if (pInstInfo->OpCode == C_3DNOW)
	{
		if (!Group_amd3dnow[inbuff[ByteCount]].OpcodeType)
			return 0;
		if (!Group_amd3dnow[inbuff[ByteCount]].name)
			return 0;
		
		strcpy(pInstInfo->Name, (const char *)Group_amd3dnow[inbuff[ByteCount]].name);
		pInstInfo->OpCode = Group_amd3dnow[inbuff[ByteCount]].OpcodeType;
		pInstInfo->AMD3DNow = 1;
		++pInstInfo->Length;
		++ByteCount;
	}

	if (pInstInfo->OpCode == _C_XCHG)
	{
		if (pInstInfo->op[1].reg.reg_index == 0)
		{
			if (disp_i.sse_prefix == SSE_PREFIX_F3 && disp_i.rex_prefix == 0)
			{
				pInstInfo->OpCode = C_PAUSE;
				pInstInfo->Repeat = -1;
				strcpy(pInstInfo->Name, "PAUSE");
				pInstInfo->op[0].mode = OP_Invalid;
				pInstInfo->op[1].mode = OP_Invalid;
			} else
			if (pInstInfo->Prefixes == 0)
			{
				pInstInfo->OpCode = C_NOP;
				strcpy(pInstInfo->Name, "NOP");
				pInstInfo->op[0].mode = OP_Invalid;
				pInstInfo->op[1].mode = OP_Invalid;
			} else
			{
				pInstInfo->OpCode = C_XCHG;
			}
		} else
		{
			pInstInfo->OpCode = C_XCHG;
		}
	}

	if (pInstInfo->pasm)	
		pInstInfo->U_x3 = PrintPasm(pInstInfo, pInstInfo->pasm, pInstInfo->U_x1, pInstInfo->U_x2);
	TStrUpr(pInstInfo->Name); //TODO
	return ByteCount;
}

//extern
int Disassembly(char *pasm, unsigned char *inbuff, unsigned long long eip, int mode, int U_x1, int U_x2)
{
	DIS_CPU DisCPU;
	_INSTRUCTION_INFORMATION InstInfo;

	InstInfo.pasm = pasm;
	InstInfo.U_x1 = U_x1;
	InstInfo.U_x2 = U_x2;
	return Disassembly(&InstInfo, inbuff, eip, mode, &DisCPU);
}

int Assembly(const char *AsmString, unsigned char *CodeBuffer, unsigned long eip, int mode)
{
	//TODO
	return 0;
}

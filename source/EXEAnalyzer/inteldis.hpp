
#ifndef _INTELDIS_HPP_
#define _INTELDIS_HPP_

#include "../Code/define.h"

	enum
	{
		CODE_MODE_16 = 0,
		CODE_MODE_32 = 1,
		CODE_MODE_64 = 2,
	};

enum enum_1
{
  D__NONE = 0,
  D__Cd = 1,
  D__Dd = 2,
  D__Td = 3,
  D__Fv = 4,
  D__Gb = 5,
  D_rGb = 6,
  D_wGb = 7,
  D_dGb = 8,
  D__Gw = 9,
  D_rGw = 10,
  D_wGw = 11,
  D_dGw = 12,
  D__Gv = 13,
  D_rGv = 14,
  D_wGv = 15,
  D_dGv = 16,
  D__Gd = 17,
  D_rGd = 18,
  D_wGd = 19,
  D_dGd = 20,
  D__Gy = 21,
  D_rGy = 22,
  D_wGy = 23,
  D_dGy = 24,
  D__Ib = 25,
  D_zIb = 26,
  D_sIb = 27,
  D__Iw = 28,
  D__Id = 29,
  D__Iv = 30,
  D__1 = 31,
  D__Iz = 32,
  D__Jb = 33,
  D__Jz = 34,
  D__Ob = 35,
  D_rOb = 36,
  D_wOb = 37,
  D_dOb = 38,
  D__Ov = 39,
  D_rOv = 40,
  D_wOv = 41,
  D_dOv = 42,
  D__Ap = 43,
  D__Pd = 44,
  D_rPd = 45,
  D_wPd = 46,
  D_dPd = 47,
  D__Pq = 48,
  D_rPq = 49,
  D_wPq = 50,
  D_dPq = 51,
  D__Ppi = 52,
  D_rPpi = 53,
  D_wPpi = 54,
  D_dPpi = 55,
  D__Rv = 56,
  D__Rd = 57,
  D__Sw = 58,
  D__NTA = 59,
  D__T0 = 60,
  D__T1 = 61,
  D__T2 = 62,
  D__T3 = 63,
  D__T4 = 64,
  D__T5 = 65,
  D__T6 = 66,
  D__T7 = 67,
  D__Xb = 68,
  D_rXb = 69,
  D_wXb = 70,
  D__Xw = 71,
  D_rXw = 72,
  D_wXw = 73,
  D__Xv = 74,
  D_rXv = 75,
  D_wXv = 76,
  D__Xd = 77,
  D_rXd = 78,
  D_wXd = 79,
  D__Xz = 80,
  D_rXz = 81,
  D_wXz = 82,
  D__Yb = 83,
  D_rYb = 84,
  D_wYb = 85,
  D__Yw = 86,
  D_rYw = 87,
  D_wYw = 88,
  D__Yv = 89,
  D_rYv = 90,
  D_wYv = 91,
  D__Yd = 92,
  D_rYd = 93,
  D_wYd = 94,
  D__Yz = 95,
  D_rYz = 96,
  D_wYz = 97,
  D__AL = 98,
  D__CL = 99,
  D__DL = 100,
  D__BL = 101,
  D__AH = 102,
  D__CH = 103,
  D__DH = 104,
  D__BH = 105,
  D__RAL = 106,
  D__RCL = 107,
  D__RDL = 108,
  D__RBL = 109,
  D__RAH = 110,
  D__RCH = 111,
  D__RDH = 112,
  D__RBH = 113,
  D__AX = 114,
  D__CX = 115,
  D__DX = 116,
  D__BX = 117,
  D__SP = 118,
  D__BP = 119,
  D__SI = 120,
  D__DI = 121,
  D__eAX = 122,
  D__eCX = 123,
  D__eDX = 124,
  D__eBX = 125,
  D__eSP = 126,
  D__eBP = 127,
  D__eSI = 128,
  D__eDI = 129,
  D__erAX = 130,
  D__erCX = 131,
  D__erDX = 132,
  D__erBX = 133,
  D__erSP = 134,
  D__erBP = 135,
  D__erSI = 136,
  D__erDI = 137,
  D__rAX = 138,
  D__rCX = 139,
  D__rDX = 140,
  D__rBX = 141,
  D__rSP = 142,
  D__rBP = 143,
  D__rSI = 144,
  D__rDI = 145,
  D__EAX = 146,
  D__ECX = 147,
  D__EDX = 148,
  D__EBX = 149,
  D__ESP = 150,
  D__EBP = 151,
  D__ESI = 152,
  D__EDI = 153,
  D__RAX = 154,
  D__RCX = 155,
  D__RDX = 156,
  D__RBX = 157,
  D__RSP = 158,
  D__RBP = 159,
  D__RSI = 160,
  D__RDI = 161,
  D__ES = 162,
  D__CS = 163,
  D__SS = 164,
  D__DS = 165,
  D__FS = 166,
  D__GS = 167,
  D__st = 168,
  D__st0 = 169,
  D__st1 = 170,
  D__st2 = 171,
  D__st3 = 172,
  D__st4 = 173,
  D__st5 = 174,
  D__st6 = 175,
  D__st7 = 176,
  D__Eb = 177,
  D_rEb = 178,
  D_wEb = 179,
  D_dEb = 180,
  D__Ew = 181,
  D_rEw = 182,
  D_wEw = 183,
  D_dEw = 184,
  D__Ev = 185,
  D_rEv = 186,
  D_wEv = 187,
  D_dEv = 188,
  D__Ed = 189,
  D_rEd = 190,
  D_wEd = 191,
  D_dEd = 192,
  D__Ep = 193,
  D_rEp = 194,
  D_wEp = 195,
  D_dEp = 196,
  D__Ey = 197,
  D_rEy = 198,
  D_wEy = 199,
  D_dEy = 200,
  D__M = 201,
  D_rM = 202,
  D_wM = 203,
  D_dM = 204,
  D__Ma = 205,
  D_rMa = 206,
  D_wMa = 207,
  D_dMa = 208,
  D__Mb = 209,
  D_rMb = 210,
  D_wMb = 211,
  D_dMb = 212,
  D__Mp = 213,
  D_rMp = 214,
  D_wMp = 215,
  D_dMp = 216,
  D__Ms = 217,
  D_rMs = 218,
  D_wMs = 219,
  D_dMs = 220,
  D__Mq = 221,
  D_wMq = 222,
  D_rMq = 223,
  D_dMq = 224,
  D__Mxdq = 225,
  D_wMxdq = 226,
  D_rMxdq = 227,
  D_dMxdq = 228,
  D__Mps = 229,
  D_wMps = 230,
  D_rMps = 231,
  D_dMps = 232,
  D__Mpd = 233,
  D_wMpd = 234,
  D_rMpd = 235,
  D_dMpd = 236,
  D__Mdq = 237,
  D_wMdq = 238,
  D_rMdq = 239,
  D_dMdq = 240,
  D__Md = 241,
  D_rMd = 242,
  D_wMd = 243,
  D_dMd = 244,
  D__Mw = 245,
  D_rMw = 246,
  D_wMw = 247,
  D_dMw = 248,
  D_wEx = 249,
  D__cVV = 250,
  D_rcVV = 251,
  D_wcVV = 252,
  D_dcVV = 253,
  D__My = 254,
  D_rMy = 255,
  D_wMy = 256,
  D_dMy = 257,
  D__Hm = 258,
  D_wHm = 259,
  D_rHm = 260,
  D_dHm = 261,
  D__Qq = 262,
  D_wQq = 263,
  D_rQq = 264,
  D_dQq = 265,
  D__Qpi = 266,
  D_wQpi = 267,
  D_rQpi = 268,
  D_dQpi = 269,
  D__xXX = 270,
  D_wxXX = 271,
  D_rxXX = 272,
  D_dxXX = 273,
  D__Vpd = 274,
  D_rVpd = 275,
  D_wVpd = 276,
  D_dVpd = 277,
  D__Vdq = 278,
  D_rVdq = 279,
  D_wVdq = 280,
  D_dVdq = 281,
  D__Vsd = 282,
  D_wVsd = 283,
  D_rVsd = 284,
  D_dVsd = 285,
  D__Vq = 286,
  D_wVq = 287,
  D_rVq = 288,
  D_dVq = 289,
  D__Vps = 290,
  D_wVps = 291,
  D_rVps = 292,
  D_dVps = 293,
  D__Vss = 294,
  D_wVss = 295,
  D_rVss = 296,
  D_dVss = 297,
  D__sXX = 298,
  D_wsXX = 299,
  D_rsXX = 300,
  D_dsXX = 301,
  D__Wsd = 302,
  D_wWsd = 303,
  D_rWsd = 304,
  D_dWsd = 305,
  D__Wdq = 306,
  D_rWdq = 307,
  D_wWdq = 308,
  D_dWdq = 309,
  D__Wpd = 310,
  D_wWpd = 311,
  D_rWpd = 312,
  D_dWpd = 313,
  D__Wq = 314,
  D_wWq = 315,
  D_rWq = 316,
  D_dWq = 317,
  D__Wps = 318,
  D_wWps = 319,
  D_rWps = 320,
  D_dWps = 321,
  D__Wss = 322,
  D_wWss = 323,
  D_rWss = 324,
  D_dWss = 325,
  D__Ups = 326,
  D_wUps = 327,
  D_rUps = 328,
  D_dUps = 329,
  D__Upd = 330,
  D_wUpd = 331,
  D_rUpd = 332,
  D_dUpd = 333,
  D__Uq = 334,
  D_wUq = 335,
  D_rUq = 336,
  D_dUq = 337,
  D__Udq = 338,
  D_wUdq = 339,
  D_rUdq = 340,
  D_dUdq = 341,
  D__Nq = 342,
  D_wNq = 343,
  D_rNq = 344,
  D_dNq = 345,
  D__word_int = 346,
  D__dword_int = 347,
  D__qword_int = 348,
  D__single_real = 349,
  D__double_real = 350,
  D__extend_real = 351,
  D__packed_BCD = 352,
  D__2b = 353,
  D__14_28b = 354,
  D__98_108b = 355,
  D__tO = 356,
  D__fO = 357,
  D__tC = 358,
  D__fC = 359,
  D__tZ = 360,
  D__fZ = 361,
  D__fA = 362,
  D__tA = 363,
  D__fS = 364,
  D__fP = 365,
  D__tP = 366,
  D__tS = 367,
  D__tL = 368,
  D__fL = 369,
  D__fG = 370,
  D__tG = 371,
  D__LOOPNZ = 372,
  D__LOOPZ = 373,
  D__LOOP = 374,
  D__JCXZ = 375,
  D__JMP = 376,
  D_NEAR = 377,
  D_MFAR = 378,
  D_DBYTE = 379,
  D_DWORD = 380,
  D_DDWORD = 381,
  D_STRING = 382,
};

enum _OPCODETYPE
{
C_NULL		= 0,
C_ES		= 1,
C_CS		= 2,
C_SS		= 3,
C_DS		= 4,
C_FS		= 5,
C_GS		= 6,

C_TAIL           = 7,

C_OPER           = 8,
C_ADDR           = 9,
C_LOCK           = 10,
C_REPN           = 11,
C_REPZ           = 12,
C_GRP            = 13,
C_ESC            = 14,
C_2BYTE          = 15,
C_GRPMOD         = 16,
C_GRPRM          = 17,
C_3BYTE_1        = 18,
C_3BYTE_2        = 19,
C_GRPSSE         = 20,
C_GRPSSEFIX      = 21,
C_GRP7           = 22,
C_GRP8           = 23,
C_GRPX64         = 24,

C_MOVS           = 25,
C_STOS           = 26,
C_LODS           = 27,
C_INS            = 28,
C_OUTS           = 29,
C_CMPS           = 30,
C_SCAS           = 31,
C_IN             = 32,
C_OUT            = 33,
C_LES            = 34,
C_LCS            = 35,
C_LSS            = 36,
C_LDS            = 37,
C_LFS            = 38,
C_LGS            = 39,
C_POPCNT         = 40,//	++
C_LOOPNZ         = 42,
C_LOOPZ          = 43,
C_LOOP           = 44,
C_JCXZ           = 45,
C_JMP            = 46,
C_MOVUPS         = 47,
C_MOVUSS         = 48,
C_MOVLPS         = 49,
C_UNPCKLPS       = 50,
C_UNPCKHPS       = 51,
C_MOVHLPS        = 52,
C_MOVHPS         = 53,
C_CVTPS2PS       = 54,//+
C_CVTPS2SS       = 55,//+
C_CVTTPS2PI      = 56,
C_CVTTPS2SI      = 57,//+
C_CVTPS2PI       = 58,
C_CVTPS2SI       = 59,//+
C_SQRTPS         = 60,
C_SQRTSS         = 61,
C_RSQRTPS        = 62,
C_RSQRTSS        = 63,
C_RCPPS          = 64,
C_RCPSS          = 65,
C_ADDPS          = 66,
C_ADDSS          = 67,
C_MULPS          = 68,
C_MULSS          = 69,
C_SUBPS          = 70,
C_SUBSS          = 71,
C_MINPS          = 72,
C_MINSS          = 73,
C_DIVPS          = 74,
C_DIVSS          = 75,
C_MAXPS          = 76,
C_MAXSS          = 77,
C_CMPPS          = 78,
C_CMPSS          = 79,
C_ADD            = 80,
C_OR             = 81,
C_ADC            = 82,
C_SBB            = 83,
C_AND            = 84,
C_SUB            = 85,
C_XOR            = 86,
C_CMP            = 87,
C_ROL            = 88,
C_ROR            = 89,
C_RCL            = 90,
C_RCR            = 91,
C_SHL            = 92,
C_SHR            = 93,
C_SAL            = 94,
C_SAR            = 95,
C_DAA            = 96,
C_DAS            = 97,
C_AAA            = 98,
C_AAS            = 99,
C_INC            = 100,
C_DEC            = 101,
C_PUSH           = 102,
C_POP            = 103,
C_PUSHA          = 104,
C_POPA           = 105,
C_BOUND          = 106,
C_ARPL           = 107,
C_IMUL           = 108,
//--> 64 bits instruction
C_MOVSXD         = 109,//	++
//<--
C_JtO            = 110,
C_JfO            = 111,
C_JtC            = 112,
C_JfC            = 113,
C_JtZ            = 114,
C_JfZ            = 115,
C_JfA            = 116,
C_JtA            = 117,
C_JtS            = 118,
C_JfS            = 119,
C_JtP            = 120,
C_JfP            = 121,
C_JtL            = 122,
C_JfL            = 123,
C_JfG            = 124,
C_JtG            = 125,
C_SETtO		= 126,
C_SETfO		= 127,
C_SETtC		= 128,
C_SETfC		= 129,
C_SETtZ		= 130,
C_SETfZ		= 131,
C_SETfA		= 132,
C_SETtA		= 133,
C_SETtS		= 134,
C_SETfS		= 135,
C_SETtP		= 136,
C_SETfP		= 137,
C_SETtL		= 138,
C_SETfL		= 139,
C_SETfG		= 140,
C_SETtG		= 141,
C_CMOVtO	= 142,
C_CMOVfO	= 143,
C_CMOVtC	= 144,
C_CMOVfC	= 145,
C_CMOVtZ	= 146,
C_CMOVfZ	= 147,
C_CMOVfA	= 148,
C_CMOVtA	= 149,
C_CMOVtS	= 150,
C_CMOVfS	= 151,
C_CMOVtP	= 152,
C_CMOVfP	= 153,
C_CMOVtL	= 154,
C_CMOVfL	= 155,
C_CMOVfG	= 156,
C_CMOVtG	= 157,
C_TEST           = 158,
C_NOP            = 159,
C_XCHG           = 160,
C_MOV            = 161,
C_LEA            = 162,
C_CBW            = 163,
C_CWD            = 164,
C_CALL           = 165,
C_WAIT           = 166,
_C_XCHG          = 167,	//++
C_PUSHF          = 168,
C_POPF           = 169,
C_SAHF           = 170,
C_LAHF           = 171,
C_RET            = 172,
C_ENTER          = 173,
C_LEAVE          = 174,
C_RETF           = 175,
C_INT1           = 176,
C_INT3           = 177,
C_INT            = 178,
C_INTO           = 179,
C_IRET           = 180,
C_AAM            = 181,
C_AAD            = 182,
C_SALC           = 183,
C_XLAT           = 184,
C_HLT            = 185,
C_CMC            = 186,
C_CLC            = 187,
C_STC            = 188,
C_CLI            = 189,
C_STI            = 190,
C_CLD            = 191,
C_STD            = 192,
C_NOT            = 193,
C_NEG            = 194,
C_MUL            = 195,
C_DIV            = 196,
C_IDIV           = 197,

//--> Floating point ops
C_FSTCW          = 198,
C_FSTENV         = 199,
C_FCLEX          = 200,
C_FINIT          = 201,
C_FSAVE          = 202,
C_FSTSW          = 203,
C_FWAIT          = 204,//+
C_FADD           = 205,
C_FMUL           = 206,
C_FCOM           = 207,
C_FCOMP          = 208,
C_FSUB           = 209,
C_FSUBR          = 210,
C_FDIV           = 211,
C_FDIVR          = 212,
C_FLD            = 213,
C_FXCH           = 214,
C_FNOP           = 215,
C_FCHS           = 216,
C_FABS           = 217,
C_FTST           = 218,
C_FLD1           = 219,
C_FLDL2T         = 220,
C_FLDL2E         = 221,
C_FLDPI          = 222,
C_FLDLG2         = 223,
C_FLDLN2         = 224,
C_FLDZ           = 225,
C_F2XM1          = 226,
C_FPTAN          = 227,
C_FPATAN         = 228,
C_FXTRACT        = 229,
C_FPREM1         = 230,
C_FDECSTP        = 231,
C_FINCSTP        = 232,
C_FYL2XP1        = 233,
C_FSQRT          = 234,
C_FSINCOS        = 235,
C_FRNDINT        = 236,
C_FSCALE         = 237,
C_FSIN           = 238,
C_FCOS           = 239,
C_FLDCW          = 240,
C_FLDENV         = 241,
C_FNSTCW         = 242,//+
C_FNSTENV        = 243,//+
C_FST            = 244,
C_FSTP           = 245,
C_FCMOVB         = 246,
C_FCMOVE         = 247,
C_FCMOVBE        = 248,
C_FCMOVU         = 249,
C_FUCOMPP        = 250,
C_FIADD          = 251,
C_FICOM          = 252,
C_FICOMP         = 253,
C_FIDIV          = 254,
C_FIDIVR         = 255,
C_FIMUL          = 256,
C_FISUB          = 257,
C_FISUBR         = 258,
C_FCMOVNB        = 259,
C_FCMOVNE        = 260,
C_FCMOVNBE       = 261,
C_FCMOVNU        = 262,
C_FNCLEX         = 263,//+
C_FNINIT         = 264,//+
C_FUCOMI         = 265,
C_FCOMI          = 266,
C_FILD           = 267,
C_FISTTP         = 268,//	++
C_FIST           = 269,
C_FISTP          = 270,
C_FFREE          = 271,
C_FUCOM          = 272,
C_FUCOMP         = 273,
C_FNSAVE         = 274,//+
C_FNSTSW         = 275,//+
C_FRSTOR         = 276,
C_FADDP          = 277,
C_FMULP          = 278,
C_FCOMPP         = 279,
C_FSUBRP         = 280,
C_FSUBP          = 281,
C_FDIVRP         = 282,
C_FDIVP          = 283,
C_FUCOMIP        = 284,
C_FCOMIP         = 285,
C_FBLD           = 286,
C_FBSTP          = 287,
C_FXAM           = 288,
//C_FLD2T/C_FLDP1  = 289,//+
//-
C_FYL2X          = 290,
C_FPREM          = 291,
//<--

C_SLDT           = 292,
C_STR            = 293,
C_LLDT           = 294,
C_LTR            = 295,
C_VERR           = 296,
C_VERW           = 297,
C_SGDT           = 298,
C_SIDT           = 299,
C_LGDT           = 300,
C_LIDT           = 301,
C_SMSW           = 302,
C_LMSW           = 303,
C_LAR            = 304,
C_LSL            = 305,
C_MOVZX          = 306,
C_MOVSX          = 307,
C_RSM            = 308,
C_BT             = 309,
C_BTR            = 310,
C_BTS            = 311,
C_BTC            = 312,
C_BSF            = 313,
C_BSR            = 314,
C_SHLD           = 315,
C_SHRD           = 316,
C_CMPXCHG_XBTS   = 317,//+
C_CMPXCHG_IBTS   = 318,//+
C_CMPXCHG        = 319,
C_XADD           = 320,
C_BSWAP          = 321,
C_CLTS           = 322,
C_CPUID          = 323,
C_MOVAPS         = 324,
C_CVTPI2PS       = 325,
C_MOVNTPS        = 326,
C_UCOMISS        = 327,
C_COMISS         = 328,
C_MOVMSKPS       = 329,
C_ANDPS          = 330,
C_ANDNPS         = 331,
C_ORPS           = 332,
C_XORPS          = 333,
C_XORPD          = 334,//	++
C_MOVNTI         = 335,
C_PINSRW         = 336,
C_PEXTRW         = 337,
C_SHUFPS         = 338,
C_PSRLW          = 339,
C_PSRLD          = 340,
C_PSRLQ          = 341,
C_PADDQ          = 342,
C_PMULLW         = 343,
C_MOVQ           = 344,
C_PMOVMSKB       = 345,
C_CVTPS2PD       = 346,
C_CVTDQ2PS       = 347,
C_PSHUFW         = 348,
C_PCMPEQB        = 349,
C_PCMPEQW        = 350,
C_PCMPEQD        = 351,
C_EMMS           = 352,
C_PREFETCHNTA    = 353,
C_FXSAVE         = 354,
C_FXRSTOR        = 355,
C_LDMXCSR        = 356,
C_STMXCSR        = 357,
C_XSAVE          = 358,//	++
C_XRSTOR         = 359,//	++
C_WRMSR          = 360,
C_RDTSC          = 361,
C_RDMSR          = 362,
C_RDPMC          = 363,
C_SYSCALL        = 364,
C_SYSRET         = 365,
C_SYSENTER       = 366,
C_SYSEXIT        = 367,
C_GETSEC         = 368,//	++
C_INVD           = 369,
C_WBINVD         = 370,
C_INVLPG         = 371,
C_CMPXCHG8B      = 372,
C_PUNPCKLBW      = 374,
C_PUNPCKLWD      = 375,
C_PUNPCKLDQ      = 376,
C_PACKSSWB       = 377,
C_PCMPGTB        = 378,
C_PCMPGTW        = 379,
C_PCMPGTD        = 380,
C_PACKUSWB       = 381,
C_PUNPCKHBW      = 382,
C_PUNPCKHWD      = 383,
C_PUNPCKHDQ      = 384,
C_PACKSSDW       = 385,
C_PUNPCKLQDQ     = 386,
C_PUNPCKHQDQ     = 387,
C_MOVD           = 388,
C_PSUBB          = 389,
C_PSUBW          = 390,
C_PSUBD          = 391,
C_PSUBQ          = 392,
C_PADDB          = 393,
C_PADDW          = 394,
C_PADDD          = 395,
C_PSUBUSB        = 396,
C_PMINUB         = 398,
C_PAND           = 399,
C_PADDUSB        = 400,
C_PADDUSW        = 401,
C_PMAXUB         = 402,
C_PANDN          = 403,
C_PAVGB          = 404,
C_PSRAW          = 405,
C_PSRAD          = 406,
C_PAVGW          = 407,
C_PMULHUW        = 408,
C_PMULHW         = 409,
C_CVTPD2DQ       = 410,
C_MOVNTQ         = 411,
C_PSUBSB         = 412,
C_PSUBSW         = 413,
C_PMINSW         = 414,
C_POR            = 415,
C_PADDSB         = 416,
C_PADDSW         = 417,
C_PMAXSW         = 418,
C_PXOR           = 419,
C_PSLLW          = 420,
C_PSLLD          = 421,
C_PSLLQ          = 422,
C_PMULUDQ        = 423,
C_PMADDWD        = 424,
C_PSADBW         = 425,
C_MASKMOVQ       = 426,
C_CLFLUSH        = 427,
C_LFENCE         = 428,
C_MFENCE         = 429,
C_SFENCE         = 430,

//++

C_UD2            = 431,
//--> VT-x instructions
C_VMPTRLD        = 432,
C_VMCLEAR        = 433,
C_VMXON          = 434,
C_VMPTRST        = 435,
C_VMCALL         = 436,
C_VMLAUNCH       = 437,
C_VMRESUME       = 438,
C_VMXOFF         = 439,
C_VMREAD         = 440,
C_VMWRITE        = 441,
//<--
C_MONITOR        = 442,
C_MWAIT          = 443,
C_SWAPGS         = 444,
C_PSHUFB         = 445,
C_PHADDW         = 446,
C_PHADDD         = 447,
C_PHADDSW        = 448,
C_PHSUBW         = 450,
C_PHSUBD         = 451,
C_PHSUBSW        = 452,
C_PSIGNB         = 453,
C_PSIGNW         = 454,
C_PSIGND         = 455,
C_PMULHRSW       = 456,
C_PABSB          = 457,
C_PABSW          = 458,
C_PABSD          = 459,
C_PALIGNR        = 460,
C_PSHUFD         = 461,
C_PSHUFHW        = 462,
C_PSHUFLW        = 463,
C_HADDPD         = 464,
C_HADDPS         = 465,
C_HSUBPD         = 466,
C_HSUBPS         = 467,
C_MOVDQA         = 468,
C_MOVDQU         = 469,
C_MOVMSKPD       = 470,
C_SQRTPD         = 471,
C_SQRTSD         = 472,
C_ANDPD          = 473,
C_ANDNPD         = 474,
C_ORPD           = 475,
C_ADDPD          = 476,
C_ADDSD          = 477,
C_MULPD          = 478,
C_MULSD          = 479,
C_CVTPD2PS       = 480,
C_CVTSD2SS       = 481,
C_CVTSS2SD       = 482,
C_CVTPS2DQ       = 483,
C_CVTTPS2DQ      = 484,
C_SUBPD          = 485,
C_SUBSD          = 486,
C_MINPD          = 487,
C_MINSD          = 488,
C_DIVPD          = 489,
C_DIVSD          = 490,
C_MAXPD          = 491,
C_MAXSD          = 492,
C_MOVUPD         = 493,
C_MOVSD          = 494,
C_MOVSS          = 495,
C_MOVLPD         = 496,
C_MOVDDUP        = 497,
C_MOVSLDUP       = 498,
C_UNPCKLPD       = 499,
C_UNPCKHPD       = 500,
C_MOVHPD         = 501,
C_MOVSHDUP       = 502,
C_MOVLHPS        = 503,
C_MOVAPD         = 504,
C_CVTPI2PD       = 505,
C_CVTSI2SD       = 506,
C_CVTSI2SS       = 507,
C_MOVNTPD        = 508,
C_MOVNTSD        = 509,
C_MOVNTSS        = 510,
C_CVTTPD2PI      = 511,
C_CVTTSD2SI      = 512,
C_CVTTSS2SI      = 513,
C_CVTPD2PI       = 514,
C_CVTSD2SI       = 515,
C_CVTSS2SI       = 516,
C_UCOMISD        = 517,
C_COMISD         = 518,
C_ADDSUBPD       = 519,
C_ADDSUBPS       = 520,
C_MOVDQ2Q        = 521,
C_MOVQ2DQ        = 522,
C_PSUBUSW        = 523,
C_CVTTPD2DQ      = 524,
C_CVTDQ2PD       = 525,
C_MOVNTDQ        = 526,
C_LDDQU          = 527,
C_MASKMOVDQU     = 528,
C_PMADDUBSW      = 529,
C_PMOVSXBW       = 530,
C_PMOVSXBD       = 531,
C_PMOVSXBQ       = 532,
C_PMOVSXWD       = 533,
C_PMOVSXWQ       = 534,
C_PMOVSXDQ       = 535,
C_PMULDQ         = 536,
C_PCMPEQQ        = 537,
C_MOVNTDQA       = 538,
C_PACKUSDW       = 539,
C_PMOVZXBW       = 540,
C_PMOVZXBD       = 541,
C_PMOVZXBQ       = 542,
C_PMOVZXWD       = 543,
C_PMOVZXWQ       = 544,
C_PMOVZXDQ       = 545,
C_PCMPGTQ        = 546,
C_PMINSB         = 547,
C_PMINSD         = 548,
C_PMINUW         = 549,
C_PMINUD         = 550,
C_PMAXSB         = 551,
C_PMAXSD         = 552,
C_PMAXUW         = 553,
C_PMAXUD         = 554,
C_PMULLD         = 555,
C_PHMINPOSUW     = 556,
C_CRC32          = 557,
C_PBLENDVB       = 558,
C_BLENDVPS       = 559,
C_BLENDVPD       = 560,
C_PTEST          = 561,
C_ROUNDPS        = 562,
C_ROUNDPD        = 563,
C_ROUNDSS        = 564,
C_ROUNDSD        = 565,
C_BLENDPS        = 566,
C_BLENDPD        = 567,
C_PBLENDW        = 568,
C_PEXTRB         = 569,
C_PEXTRD         = 570,
C_EXTRACTPS      = 572,
C_PINSRB         = 573,
C_INSERTPS       = 574,
C_PINSRD         = 575,
C_DPPS           = 576,
C_DPPD           = 577,
C_MPSADBW        = 578,
C_PCLMULQDQ      = 579,
C_PCMPESTRM      = 580,
C_PCMPESTRI      = 581,
C_PCMPISTRM      = 582,
C_PCMPISTRI      = 583,
C_SHUFPD         = 584,
//--> VT-x instructions
C_INVEPT         = 585,
C_INVVPID        = 586,
//<--
C_MOVBE          = 587,
C_AESKEYGENASSIST  = 588,
C_AESIMC         = 589,
C_AESENC         = 590,
C_AESENCLAST     = 591,
C_AESDEC         = 592,
C_AESDECLAST     = 593,
C_XGETBV         = 594,
C_XSETBV         = 595,
C_RDTSCP         = 596,
C_PAUSE          = 597,
C_CMPPD          = 598,
C_CMPSD          = 599,
C_PSRLDQ         = 600,
C_PSLLDQ         = 601,

//--> AMD-V instructions
C_VMRUN          = 602,
C_VMMCALL        = 603,
C_VMLOAD         = 604,
C_VMSAVE         = 605,
C_STGI           = 606,
C_CLGI           = 607,
C_SKINIT         = 608,
C_INVLPGA        = 609,
//<--
C_EXTRQ          = 610,

//--> 3DNow!
C_PI2FW          = 611,
C_PI2FD          = 612,
C_PF2IW          = 613,
C_PF2ID          = 614,
C_PFNACC         = 615,
C_PFPNACC        = 616,
C_PFCMPGE        = 617,
C_PFMIN          = 618,
C_PFRCP          = 619,
C_PFRSQRT        = 620,
C_PFSUB          = 621,
C_PFADD          = 622,
C_PFCMPGT        = 623,
C_PFMAX          = 624,
C_PFRCPIT1       = 625,
C_PFRSQIT1       = 626,
C_PFSUBR         = 627,
C_PFACC          = 628,
C_PFCMPEQ        = 629,
C_PFMUL          = 630,
C_PFRCPIT2       = 631,
C_PMULHRW        = 632,
C_PSWAPD         = 633,
C_PAVGUSB        = 634,
//<--

//--> AVX instructions
//TODO
//<--

C_3DNOW          = 635,
C_DBYTE          = 636,
C_DWORD          = 637,
C_DDWORD         = 638,
};

// disasm

struct _DIS_ADDRESS
{
	unsigned int address_size;
	unsigned int r_m;
	unsigned int reg;
	unsigned int mod;
	unsigned int base;
	unsigned int index;
	unsigned int scale;
	unsigned int segment;
	unsigned int displacement_size;
	unsigned int __unused; //ALIGN
	unsigned long long displacement;
	unsigned long long effect_address;
	unsigned long long line_address;
};

// disasm

enum
{
	OP_Invalid = 0,
	OP_Address = 1,
	OP_Register = 2,
	OP_Segment = 3,
	OP_Immed = 4,
	OP_Near = 5,
	OP_Far = 6,
	OP_String = 7,

	OP_Datadup = 20,
	OP_Symbol = 21,
	OP_Align = 22,
};

enum RWFLAG
{
	FREAD = 1,
	FWRITE = 2,
};

enum GENE_REG_INDEX
{
	EAX_IDX = 0,
	ECX_IDX = 1,
	EDX_IDX = 2,
	EBX_IDX = 3,
	ESP_IDX = 4,
	EBP_IDX = 5,
	ESI_IDX = 6,
	EDI_IDX = 7,
	R8_IDX = 8,
	R9_IDX = 9,
	R10_IDX = 10,
	R11_IDX = 11,
	R12_IDX = 12,
	R13_IDX = 13,
	R14_IDX = 14,
	R15_IDX = 15,
	GENE_IDX_MAX = 16,
};

enum SEG_REG_INDEX
{
	ES_IDX = 0,
	CS_IDX = 1,
	SS_IDX = 2,
	DS_IDX = 3,
	FS_IDX = 4,
	GS_IDX = 5,
	SEG_IDX_MAX = 6,
};

struct _OPERAND_ITEM
{
	int mode;
	int rwflag;
	int opersize;
	char string[32];

	union
	{
		_DIS_ADDRESS addr;	//1

		struct
		{
			int reg_index;	//GENE_REG_INDEX
		} reg;			//2

		struct
		{
			int seg_index;	//SEG_REG_INDEX
		} seg;			//3

		struct
		{
			unsigned long long immed_value;
		} immed;		//4

		struct
		{
			unsigned long long offset;
			unsigned long long label;
		} nearptr;		//5

		struct
		{
			unsigned long offset;
			unsigned long segment;
		} farptr;		//6

		struct
		{
			int count;
			unsigned char buffer[32];
		} datadup;		//7,20

		struct
		{
			char string[64];
			int boffset;
			int mode;
			unsigned long offset;
		} symbol;		//21

		struct
		{
			unsigned long nBytes;
		} align;		//22
	};
};

struct _INSTRUCTION_INFORMATION
{
	ULONG_PTR eip;
	int CodeMode;
	int Length;
	int OpCode;
	int OpCount;
	int OpType;
	int CodeType;
	char Name[16];
	int Lock;
	char LockName[8];
	int Repeat;
	char RepeatName[8];
	int SegmentPrefix;
	int xxxxptr;
	//
	_OPERAND_ITEM op[3];
	unsigned char *CodeBuff;
	char *pasm;
	int U_x1;
	int U_x2;
	int U_x3;

	int Prefixes;
	int RegRM;
	int Sib1;
	int AMD3DNow;
	int RexPrefix;
};

union DIS_GEN_REG_T
{
	unsigned long long rerx;

	union
	{
		unsigned long erx;
		struct
		{
			union
			{
				union
				{
					unsigned short rx;
					struct
					{
						unsigned char rl;
						unsigned char rh;
					} byte;
				};
			} u_word;
			unsigned short word_filler;
		} word;
	};
};


struct DIS_CPU
{
	DIS_GEN_REG_T gen_reg[GENE_IDX_MAX];
	unsigned long long r[6]; //???
	ULONG_PTR SEG_BASE[SEG_IDX_MAX];
};

int Disassembly(_INSTRUCTION_INFORMATION *pInstInfo, unsigned char *inbuff, unsigned long long eip, int mode, DIS_CPU *DisCPU);

int Disassembly(char *pasm, unsigned char *inbuff, unsigned long long eip, int mode, int U_x1, int U_x2);

int Assembly(const char *AsmString, unsigned char *CodeBuffer, unsigned long eip, int mode);

struct CIntelDisassembler
{
	int m_U_x1;
	int m_U_x2;
	int m_U_x3;
	int m_Mode;

	CIntelDisassembler()
		: m_U_x1(6)
		, m_U_x2(12)
		, m_U_x3(20)
		, m_Mode(CODE_MODE_32)
	{
	}
	~CIntelDisassembler()
	{
	}

	void SetStyle(int u_x1, int u_x2, int u_x3)
	{
		m_U_x1 = u_x1;
		m_U_x2 = u_x2;
		m_U_x3 = u_x3;
	}
	void SetMode(int mode)
	{
		m_Mode = mode;
	}

	static int GetLength(unsigned char *inbuff, int mode)
	{
		return Disassembly(0, inbuff, 0x100, mode, 0, 8);
	}
	static char ** GetIntelConstName(int *size)
	{
		return 0;
	}

	int Disassembler(_INSTRUCTION_INFORMATION *pInstInfo, DIS_CPU *dis_cpu)
	{
		pInstInfo->CodeMode = m_Mode;
		pInstInfo->U_x1 = m_U_x1;
		pInstInfo->U_x2 = m_U_x2;
		return Disassembly(pInstInfo, pInstInfo->CodeBuff, pInstInfo->eip, m_Mode, dis_cpu);
	}

	static int Assembler(const char *AsmString, unsigned char *CodeBuffer, unsigned long eip, int mode)
	{
		return Assembly(AsmString, CodeBuffer, eip, mode);
	}
};

#endif

#ifndef _SYSERCONFIG_HPP_
#define _SYSERCONFIG_HPP_

#include "../Code/define.h"

#pragma pack(push, 1)
struct SYSER_CONFIG
{
	unsigned int iID;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SYSER_CFG //: public SYSER_CONFIG
{
	unsigned int iID;
	unsigned int iSplitRight;
	unsigned int iBigFont;
	WCHAR szFontFileName[MAX_FN_LEN];
	unsigned int iFollowMode;
	unsigned int iShowStartSplash;
	unsigned int iShowFullSym;
	unsigned int iShowCCByte;
	unsigned int iMouseSensitivity;
	unsigned int iHistoryLines;
	unsigned int iSourceCodeShowTips;
	unsigned int iSourceCodeSyntaxColors;
	unsigned int iSourceCodeShowLineNumbers;
	unsigned int iConnectType;
	unsigned int iComPort;
	unsigned int iVideoDetectMode;
	unsigned int iStackSize;
	unsigned int iHardTimerEnable;
	WCHAR szDebuggerFileName[MAX_FN_LEN];
	unsigned int iWidth;
	unsigned int iHeight;
	unsigned int iWidthMax;
	unsigned int iHeightMax;
	unsigned int iGeneralHeapSize;
	unsigned int iSourceHeapSize;
	unsigned int iAzertyKeyboard;
	unsigned int i3here;
	unsigned int i1here;
	unsigned int faultsCmdStatus;
};
#pragma pack(pop)

extern SYSER_CFG gSyserConfig;
extern SYSER_CFG gSyserDefConfig;

enum CV_COLOR
{
  COLOR_NULL = 0,
  COLOR_BG = 1,
  COLOR_ADDRESS = 2,
  COLOR_PREFIX = 3,
  COLOR_OPCODE = 4,
  COLOR_REGISTER = 5,
  COLOR_SEG_REG = 6,
  COLOR_IMMED = 7,
  COLOR_OPTR = 8,
  COLOR_SYMBOL = 9,
  COLOR_COMMENT = 10,
  COLOR_KEYWORD = 11,
  COLOR_STRING = 12,
  COLOR_BPX_STRIP = 13,
  COLOR_EIP_STRIP = 14,
  COLOR_BPX_EIP_STRIP = 15,
  COLOR_API_NAME = 16,
  COLOR_PARAM_TYPE = 17,
  COLOR_PARAM_NAME = 18,
  COLOR_JMP_LINE = 19,
  COLOR_ACTIVE_JMP_LINE = 20,
  COLOR_PREFIX_BYTE = 21,
  COLOR_OPCODE_BYTE = 22,
  COLOR_REGRM_BYTE = 23,
  COLOR_SIB_BYTE = 24,
  COLOR_OTHER_OPCODE_BYTE = 25,
  COLOR_AMD3DNOW_BYTE = 26,
  COLOR_MAX = 27,
};
#if 0
enum SV_COLOR
{
  COLOR_NULL = 0,
  COLOR_BG = 1,
//TODO
};
#endif
#pragma pack(push, 1)
struct SYSER_COLORS //: public SYSER_CONFIG
{
	unsigned int iID;
	unsigned long Color[51];
};
#pragma pack(pop)

extern SYSER_COLORS gSyserColors;
extern SYSER_COLORS gSyserDefColors;

bool ReadConfig(const char *FileName, void *Buffer, unsigned long Size);
bool ReadConfigSize(const char *FileName, void *Buffer, unsigned long Size, unsigned long *FileSize);
bool WriteConfig(const char *FileName, void *Buffer, unsigned long Size);

#endif

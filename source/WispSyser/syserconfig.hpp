
#ifndef _SYSERCONFIG_HPP_
#define _SYSERCONFIG_HPP_

#include "../Code/define.h"

#pragma pack(push, 1)
struct SYSER_CFG
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

#if 0
enum SV_COLOR
{
  COLOR_NULL = 0,
  COLOR_BG = 1,
//TODO
};
#endif
#pragma pack(push, 1)
struct SYSER_COLORS
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


#include "syserconfig.hpp"
#include "../Code/imagefile.hpp"

SYSER_CFG gSyserConfig;

SYSER_CFG gSyserDefConfig =
{
	0,			//; iID
	0,			//; iSplitRight
	0,			//; iBigFont
	WSTR(""),		//; szFontFileName
	0,			//; iFollowMode
	1,			//; iShowStartSplash
	0,			//; iShowFullSym
	0,			//; iShowCCByte
	10,			//; iMouseSensitivity
	500,			//; iHistoryLines
	1,			//; iSourceCodeShowTips
	1,			//; iSourceCodeSyntaxColors
	1,			//; iSourceCodeShowLineNumbers
	1,			//; iConnectType
	1,			//; iComPort
	0,			//; iVideoDetectMode
	512,			//; iStackSize
	1,			//; iHardTimerEnable
#ifdef _X64_
	WSTR("sdx64.dll"),	//; szDebuggerFileName
#else
	WSTR("sdx32.dll"),	//; szDebuggerFileName
#endif
	1024,			//; iWidth
	768,			//; iHeight
	1024,			//; iWidthMax
	768,			//; iHeightMax
	12,			//; iGeneralHeapSize
	15,			//; iSourceHeapSize
	0,			//; iAzertyKeyboard
	2,			//; i3here
	0,			//; i1here
	1,			//; faultsCmdStatus
};

SYSER_COLORS gSyserColors;

SYSER_COLORS gSyserDefColors = 
{0,
	{
//MONITOR
	0,		//Background
	0xFFFFFF,	//Item text
	0x50B0FF,	//Selected frame
	0x58587F,	//Selected frame when lost focus
	0xFF0000,	//Changed item text
	0x80C4FF,	
//CV
	0,		//Background
	0x50B0FF,	//Selected frame
	0x58587F,	//Selected frame when lost focus

	0xFFFFFF,	//Address
	0xFFFFFF,	//Prefix
	0xFFFFFF,	//Operate Code
	0x00FFFF,	//Register
	0x00C0C0,	//Segment Register
	0x00FF80,	//Immed
	0xFFFFFF,	//Operater
	0xFFFF00,	//Symbol
	0xFFFF00,	//Comment
	0xFFFFFF,	//Keyword
	0xFFFF00,	//String
	0xFF0000,	//BPX Strip
	0x808080,	//EIP Strip
	0xFF8000,	//BPX EIP Strip
	0xFF8000,	//API Name
	0xFF7D7D,	//Param Type
	0xC0C0C0,	//Param Name
	0xFFFFFF,	//Jump Line
	0xFFFF00,	//Actived Jump Line
	0x6838C6,	//Prefix Byte
	0x991622,	//Opcode Byte
	0x1A33E4,	//RegRM Byte
	0x39EF04,	//SIB Byte
	0xD01FDF,	//Other Opcode Byte
	0x707070,	//3dNow! Immediate Byte
//SV
	0,		//Background
	0x50B0FF,	//Selected frame
	0x58587F,	//Selected frame when lost focus

	0xFFFFFF,	//Keyword
	0xC0C0C0,	//Identifier
	0x4080FF,	//Preprocessor Keyword
	0x808080,	//Comment block
	0x808080,	//Comment line
	0xFFFF00,	//String
	0x00FF80,	//Numerical
	0xFFFFFF,	//Operation
	0xE17D7D,	//Class name
	0xE17D7D,	//Struct name
	0xE17D7D,	//Union name
	0xE17D7D,	//Typedef name
	0xFF8000,	//Function name
	0x7B7B7B,	//Unknown
	}
};

bool ReadConfig(const char *FileName, void *Buffer, unsigned long Size)
{
	CImageFile File;
	if (!File.Open(FileName, 0))
	{
		return false;
	}

	if (Size != File.m_FileSize)
	{
		File.Close();
		return false;
	}

	if (!File.ReadFile(Buffer, Size))
	{
		File.Close();
		return false;
	}

	unsigned long CodeID = TGetCodeID((unsigned char*)Buffer + 4, Size - 4);
	if (CodeID == 0)
	{
		File.Close();
		return false;
	}

	if (CodeID != *(unsigned long*)Buffer)
	{
		File.Close();
		return false;
	}
	File.Close();
	return true;	
}

bool ReadConfigSize(const char *FileName, void *Buffer, unsigned long Size, unsigned long *FileSize)
{
	CImageFile File;
	if (!File.Open(FileName, 0))
	{
		return false;
	}

	if (Size < File.m_FileSize)
	{
		File.Close();
		return false;
	}

	*FileSize = File.m_FileSize;

	if (!File.ReadFile(Buffer, Size))
	{
		File.Close();
		return false;
	}

	unsigned long CodeID = TGetCodeID((unsigned char*)Buffer + 4, Size - 4);
	if (CodeID == 0)
	{
		File.Close();
		return false;
	}

	if (CodeID != *(unsigned long*)Buffer)
	{
		File.Close();
		return false;
	}
	File.Close();
	return true;	
}

bool WriteConfig(const char *FileName, void *Buffer, unsigned long Size)
{
	CImageFile File;
	if (!File.Create(FileName, 0))
	{
		return false;
	}

	unsigned long CodeID = TGetCodeID((unsigned char*)Buffer + 4, Size - 4);
	*(unsigned long*)Buffer = CodeID;

	if (!File.WriteFile(Buffer, Size))
	{
		File.Close();
		return false;
	}

	File.Close();
	return true;
}

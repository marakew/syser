
#include "syseroptionform.hpp"
#include "syserconfig.hpp"
#include "../Code/sysdep.hpp"
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"

unsigned int ConfigChanged = 0;
unsigned int ColorsChanged = 0;

bool CheckMouseSensitivity(unsigned int *Value)
{
        return *Value >= 1 && *Value <= 30;
}

const WCHAR *Select[] =
{
	WSTR("No"),
	WSTR("Yes"),
	0,
};

const WCHAR *EIPFollowMode[] =
{
	WSTR("Automatic (Relative to EIP)"),
	WSTR("System Explorer"),
	WSTR("Source Explorer"),
	0,
};

bool CheckHistoryLines(unsigned int *Value)
{
        return *Value >= 100 && *Value <= 1000;
}

const WCHAR *SelectMonitor[] =
{
	WSTR("Left"),
	WSTR("Right"),
	0,
};

OPTION_FORM Debugger[] =
{
	{WSTR("Monitor Position"), OPTION_INT, &gSyserConfig.iSplitRight, 4, SelectMonitor, 0, &ConfigChanged, 0},
	{WSTR("Big Font (Restart Syser to apply)"), OPTION_INT, &gSyserConfig.iBigFont, 4, Select, 0, &ConfigChanged, 0},
	{WSTR("BDF Font File Name"), OPTION_STR, &gSyserConfig.szFontFileName, MAX_FN_LEN, 0, 0, &ConfigChanged, 0},
	{WSTR("EIP Follow Mode"), OPTION_INT, &gSyserConfig.iFollowMode, 4, EIPFollowMode, 0, &ConfigChanged, 0},
	{WSTR("Show Full Symbol (Include Module Name)"), OPTION_INT, &gSyserConfig.iShowFullSym, 4, Select, 0, &ConfigChanged, 0},
//	{WSTR("Mouse Sensitivity [1 - 30]"), OPTION_STRNUM, &gSyserConfig.iMouseSensitivity, 4, 0, CheckMouseSensitivity, &ConfigChanged, 0},
	{WSTR("Command Histroy lines [100 - 10000]"), OPTION_STRNUM, &gSyserConfig.iHistoryLines, 4, 0, CheckHistoryLines, &ConfigChanged, 0},
	{WSTR("Default Debugger Module"), OPTION_STR, &gSyserConfig.szDebuggerFileName, MAX_FN_LEN, 0, 0, &ConfigChanged, 0},
	{0},
};

OPTION_FORM SourceCodeDebugger[] =
{
	{WSTR("Syntax Color"), OPTION_INT, &gSyserConfig.iSourceCodeSyntaxColors, 4, Select, 0, &ConfigChanged, 0},
	{WSTR("Show Tips"), OPTION_INT, &gSyserConfig.iSourceCodeShowTips, 4, Select, 0, &ConfigChanged, 0},
	{WSTR("Show Line Numbers"), OPTION_INT, &gSyserConfig.iSourceCodeShowLineNumbers, 4, Select, 0, &ConfigChanged, 0},
	{0},
};

bool CheckHeapSize(unsigned int *Value)
{
        return *Value >= 15 && *Value <= 40;
}

bool CheckSourceHeapSize(unsigned int *Value)
{
        return *Value >= 6 && *Value <= 60;
}

bool CheckStackSize(unsigned int *Value)
{
        return *Value >= 512 && *Value <= 4096;
}

const WCHAR *SelectEnable[] =
{
	WSTR("Disable"),
	WSTR("Enable"),
	0,
};

bool CheckWidth(unsigned int *Value)
{
        return *Value >= 640 && *Value <= 2048;
}

bool CheckHeight(unsigned int *Value)
{
        return *Value >= 480 && *Value <= 2048;
}

OPTION_FORM Advance[] =
{
	{WSTR("Geneal heap size [15 - 40]M"), OPTION_STRNUM, &gSyserConfig.iGeneralHeapSize, 4, 0, CheckHeapSize, &ConfigChanged, 0},
	{WSTR("Source Code heap size [6 - 40]M"), OPTION_STRNUM, &gSyserConfig.iSourceHeapSize, 4, 0, CheckSourceHeapSize, &ConfigChanged, 0},
	{WSTR("Stack size [512 - 4096]K"), OPTION_STRNUM, &gSyserConfig.iStackSize, 4, 0, CheckStackSize, &ConfigChanged, 0},
	{WSTR("Enable Hardware Timer"), OPTION_INT, &gSyserConfig.iHardTimerEnable, 4, SelectEnable, 0, &ConfigChanged, 0},
	{WSTR("Max window width  [640 - 2048]"), OPTION_STRNUM, &gSyserConfig.iWidthMax, 4, 0, CheckWidth, &ConfigChanged, 0},
	{WSTR("Max window height [480 - 2048]"), OPTION_STRNUM, &gSyserConfig.iHeightMax, 4, 0, CheckHeight, &ConfigChanged, 0},
	{0},
};

//colors
OPTION_FORM SyserOptionMonitorColors[] =
{
	{WSTR("Background"), OPTION_COLOR, &gSyserColors.Color[0], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Item text"), OPTION_COLOR, &gSyserColors.Color[1], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Selected frame"), OPTION_COLOR, &gSyserColors.Color[2], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Selected frame when lost focus"), OPTION_COLOR, &gSyserColors.Color[3], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Changed item text"), OPTION_COLOR, &gSyserColors.Color[4], 4, 0, 0, &ColorsChanged, 0},
	{0},
};

OPTION_FORM SyserOptionCodeViewColors[] =
{
	{WSTR("Background"), OPTION_COLOR, &gSyserColors.Color[6], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Selected frame"), OPTION_COLOR, &gSyserColors.Color[7], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Selected frame when lost focus"), OPTION_COLOR, &gSyserColors.Color[8], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Address"), OPTION_COLOR, &gSyserColors.Color[9], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Prefix"), OPTION_COLOR, &gSyserColors.Color[10], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Operate Code"), OPTION_COLOR, &gSyserColors.Color[11], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Register"), OPTION_COLOR, &gSyserColors.Color[12], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Segment Register"), OPTION_COLOR, &gSyserColors.Color[13], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Immed"), OPTION_COLOR, &gSyserColors.Color[14], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Operater"), OPTION_COLOR, &gSyserColors.Color[15], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Symbol"), OPTION_COLOR, &gSyserColors.Color[16], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Comment"), OPTION_COLOR, &gSyserColors.Color[17], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Keyword"), OPTION_COLOR, &gSyserColors.Color[18], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("String"), OPTION_COLOR, &gSyserColors.Color[19], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("BPX Strip"), OPTION_COLOR, &gSyserColors.Color[20], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("EIP Strip"), OPTION_COLOR, &gSyserColors.Color[21], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("BPX EIP Strip"), OPTION_COLOR, &gSyserColors.Color[22], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("API Name"), OPTION_COLOR, &gSyserColors.Color[23], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Param Type"), OPTION_COLOR, &gSyserColors.Color[24], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Param Name"), OPTION_COLOR, &gSyserColors.Color[25], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Jump Line"), OPTION_COLOR, &gSyserColors.Color[26], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Actived Jump Line"), OPTION_COLOR, &gSyserColors.Color[27], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Prefix Byte"), OPTION_COLOR, &gSyserColors.Color[28], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Opcode Byte"), OPTION_COLOR, &gSyserColors.Color[29], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("RegRM Byte"), OPTION_COLOR, &gSyserColors.Color[30], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("SIB Byte"), OPTION_COLOR, &gSyserColors.Color[31], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Other Opcode Byte"), OPTION_COLOR, &gSyserColors.Color[32], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("3dNow! Immediate Byte"), OPTION_COLOR, &gSyserColors.Color[33], 4, 0, 0, &ColorsChanged, 0},
	{0},
};

OPTION_FORM SyserOptionSrcCodeColors[] =
{
	{WSTR("Background"), OPTION_COLOR, &gSyserColors.Color[34], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Selected frame"), OPTION_COLOR, &gSyserColors.Color[35], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Selected frame when lost focus"), OPTION_COLOR, &gSyserColors.Color[36], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Keyword"), OPTION_COLOR, &gSyserColors.Color[37], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Identifier"), OPTION_COLOR, &gSyserColors.Color[38], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Preprocessor Keyword"), OPTION_COLOR, &gSyserColors.Color[39], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Comment block"), OPTION_COLOR, &gSyserColors.Color[40], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Comment line"), OPTION_COLOR, &gSyserColors.Color[41], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("String"), OPTION_COLOR, &gSyserColors.Color[42], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Numerical"), OPTION_COLOR, &gSyserColors.Color[43], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Operation"), OPTION_COLOR, &gSyserColors.Color[44], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Class name"), OPTION_COLOR, &gSyserColors.Color[45], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Struct name"), OPTION_COLOR, &gSyserColors.Color[46], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Union name"), OPTION_COLOR, &gSyserColors.Color[47], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Typedef name"), OPTION_COLOR, &gSyserColors.Color[48], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Function name"), OPTION_COLOR, &gSyserColors.Color[49], 4, 0, 0, &ColorsChanged, 0},
	{WSTR("Unknown"), OPTION_COLOR, &gSyserColors.Color[50], 4, 0, 0, &ColorsChanged, 0},
	{0},
};

OPTION_FORM SyserOptionColors[] =
{
	{WSTR("Monitor window"), OPTION_SUB, SyserOptionMonitorColors, 0, 0, 0, 0, 0},
	{WSTR("Code View"), OPTION_SUB, SyserOptionCodeViewColors, 0, 0, 0, 0, 0},
	{WSTR("Source Code View"), OPTION_SUB, SyserOptionSrcCodeColors, 0, 0, 0, 0, 0},
	{0},
};

OPTION_FORM SyserOptionForm[] =
{
	{WSTR("Debugger"), OPTION_SUB, Debugger, 0, 0, 0, 0, 0},
	{WSTR("Source Code Debugger"), OPTION_SUB, SourceCodeDebugger, 0, 0, 0, 0, 0},
#ifdef _RING0_
	{WSTR("Developer Advance Option (Restart Syser to apply)"), OPTION_SUB, &Advance, 0, 0, 0, 0, 0},
#endif
	{WSTR("Colors"), OPTION_SUB, SyserOptionColors, 0, 0, 0, 0, 0},
	{0},
};

	CSyserOptionForm::CSyserOptionForm()
	{
		m_OptionForm = SyserOptionForm;
	}

	CSyserOptionForm::~CSyserOptionForm()
	{
	}

	bool CSyserOptionForm::InitWnd()
	{
		m_Style |= 0xB;
		return CWispWnd::InitWnd();
	}

	WISP_MSG_MAP_BEGIN(CSyserOptionForm)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
	WISP_MSG_MAP_END(CWispOptionForm)

	bool CSyserOptionForm::OnCreateForm(WISP_MSG *pMsg)
	{
		CWispOptionForm::OnCreateForm(pMsg);
		CWispOptionForm::LoadTexts(0);
		Resize(m_BorderSize*2 + m_WindowRect.cx, m_BorderSize*2 + m_WindowRect.cy + m_CaptionRect.cy, true);
		SetWindowText(WSTR("Option"));
		return false;
	}

	void CSyserOptionForm::OnApply()
	{
		if (ConfigChanged)
		{
			ConfigChanged = 0;
			char szConfig[MAX_FN_LEN];
			GetModulePath(szConfig, true);
			TStrCat(szConfig, "Syser.cfg");
			WriteConfig(szConfig, &gSyserConfig, sizeof(gSyserConfig));
		}

		if (ColorsChanged)
		{
			ColorsChanged = 0;
			char szConfig[MAX_FN_LEN];
			GetModulePath(szConfig, true);
			TStrCat(szConfig, "SyserColor.cfg");
			WriteConfig(szConfig, &gSyserColors, sizeof(gSyserColors));
		}
		gpSyser->m_MainFrame.Update((WISP_RECT*)0);
	}

	bool CSyserOptionForm::OnDefaulting()
	{
		memcpy(&gSyserConfig, &gSyserDefConfig, sizeof(gSyserConfig));
		ConfigChanged = 0;

		memcpy(&gSyserColors, &gSyserDefColors, sizeof(gSyserColors));
		ColorsChanged = 0;

		gpSyser->m_MainFrame.Update((WISP_RECT*)0);
		return true;
	}

	void CSyserOptionForm::OnDefault()
	{
		char szConfig[MAX_FN_LEN];
		GetModulePath(szConfig, true);
		TStrCat(szConfig, "Syser.cfg");
		WriteConfig(szConfig, &gSyserConfig, sizeof(gSyserConfig));

		char szColorConfig[MAX_FN_LEN];
		GetModulePath(szColorConfig, true);
		TStrCat(szColorConfig, "SyserColor.cfg");
		WriteConfig(szColorConfig, &gSyserColors, sizeof(gSyserColors));
	}

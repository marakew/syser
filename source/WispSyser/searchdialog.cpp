
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "searchdialog.hpp"
#include "modulelist.hpp"

WISP_FORM_RES_ITEM SearchDialogForm[] =
{
	{WISP_CTRL_FORM, {0, 0, 391, 220}, 0, 0xB, WSTR("Search"), (void*)"\\Toolbar.bmp#16#16:119", 0},
	{WISP_CTRL_STATIC_GROUP, {10, 10, 364, 80}, 0, 0, WSTR("Search Method"), 0, 0},
	{WISP_CTRL_STATIC_DIB, {20, 32, 16, 16}, 0, 0, 0, (void*)"\\Toolbar.bmp#16#16:119", 0},
	{WISP_CTRL_STATIC_STRING, {40, 32, 45, 16}, 0, 0, WSTR("Type"), 0, 0},
	{WISP_CTRL_COMBO_BOX, {80, 30, 135, 20}, 1, 0x9040006, 0, 0, 0},
	{WISP_CTRL_CHECK_BOX, {230, 30, 100, 20}, 3, 0x20000000, WSTR("Match case"), 0, 0},
	{WISP_CTRL_STATIC_DIB, {20, 62, 16, 16}, 0, 0, 0, (void*)"\\Toolbar.bmp#16#16:5", 0},
	{WISP_CTRL_STATIC_STRING, {40, 62, 45, 18}, 0, 0, WSTR("Value"), 0, 0},
	{WISP_CTRL_EDIT, {80, 60, 280, 18}, 2, 0x1000006, 0, 0, 0},
	{WISP_CTRL_STATIC_DIB, {20, 116, 16, 16}, 0, 0, 0, (void*)"\\Toolbar.bmp#16#16:13*16+12", 0},
	{WISP_CTRL_STATIC_GROUP, {10, 95, 364, 50}, 0, 0, WSTR("Range"), 0, 0},
	{WISP_CTRL_STATIC_STRING, {40, 116, 40, 16}, 0, 0, WSTR("Begin"), 0, 0},
	{WISP_CTRL_EDIT, {80, 115, 70, 16}, 6, 0x1000006, 0, 0, 0},
	{WISP_CTRL_STATIC_STRING, {160, 116, 25, 16}, 0, 0, WSTR("End"), 0, 0},
	{WISP_CTRL_EDIT, {190, 115, 70, 16}, 7, 0x1000006, 0, 0, 0},
	{WISP_CTRL_BUTTON, {270, 113, 90, 20}, 8, 0, WSTR("Select Module"), 0, 0},
	{WISP_CTRL_BUTTON, {100, 160, 68, 20}, 9, 0, WSTR("Find"), 0, 0},
	{WISP_CTRL_BUTTON, {210, 160, 68, 20}, 0xA, 0x200, WSTR("Find Next"), 0, 0},
	{0},
};

	WISP_MSG_MAP_BEGIN(CSearchDialog)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispForm)

	WISP_CMD_MAP_BEGIN(CSearchDialog)
		WISP_CMD_MAP(1, OnCmd1)
	WISP_CMD_MAP_END

	WISP_EVENT_MAP_BEGIN(CSearchDialog)
		WISP_EVENT_MAP(8, OnEventModule)
		WISP_EVENT_MAP(9, OnEventOK)
		WISP_EVENT_MAP(10, OnEventSearch)
	WISP_EVENT_MAP_END


	CSearchDialog::CSearchDialog()
	{
		m_Resource = SearchDialogForm;
	}
	CSearchDialog::~CSearchDialog()
	{
	}

	bool CSearchDialog::OnCreateForm(WISP_MSG *pMsg)
	{
		m_pBegin = (CWispEdit *)GetFormItem(6);
		m_pEnd = (CWispEdit *)GetFormItem(7);
		m_pValue = (CWispEdit *)GetFormItem(2);
		m_pWidth = (CWispComboBox *)GetFormItem(1);
		m_pCase = (CWispCheckBox *)GetFormItem(3);

		static const WCHAR *Select[] =
			{
				WSTR("Text (Ansi)"),
				WSTR("Text (Unicode)"),
				WSTR("BYTE (s)"),
				WSTR("WORD  Value"),
				WSTR("DWORD Value"),
				WSTR("QWORD Value"),
				//NULL,
			};

		for (unsigned long i = 0; i < lenof(Select); ++i)
		{
			m_pWidth->m_ComboList.InsertItem(Select[i], 0, 0, 0, -1);
		}
		m_pWidth->SetCurSel(0);
		CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP);
		if (!pDbgMod)
			pDbgMod = gpSyser->m_pSysDebugger->m_DbgModuleList.FindModule(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP);
		if (pDbgMod)
		{
			WCHAR Buffer[32];

			uNumToStr(pDbgMod->m_ImageBase, Buffer, 16);
			m_pBegin->SetWindowText(Buffer);

			uNumToStr(pDbgMod->m_ImageBase + pDbgMod->m_ImageSize, Buffer, 16);
			m_pEnd->SetWindowText(Buffer);
		}
		return true;
	}

	bool CSearchDialog::OnCmd1(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000400)
			m_pCase->EnableWindow(pMsg->Command.Param1 <= 1);
		return true;
	}

	bool CSearchDialog::OnEventModule(WISP_MSG *pMsg)
	{
		CModuleListWnd ModuleList;
		ModuleList.Create(true);
		if (ModuleList.m_pDbgMod)
		{
			WCHAR szNumber[32];

			uNumToStr(ModuleList.m_pDbgMod->m_ImageBase, szNumber, 16);
			m_pBegin->SetWindowText(szNumber);

			uNumToStr(ModuleList.m_pDbgMod->m_ImageBase + ModuleList.m_pDbgMod->m_ImageSize, szNumber, 16);
			m_pEnd->SetWindowText(szNumber);

			Update(&m_ClientRect);
		}
		return true;
	}

	bool CSearchDialog::OnEventOK(WISP_MSG *pMsg)
	{
		int width = m_pWidth->GetCurSel();
		if (width != -1)
		{
			if (m_pValue->m_WndText.m_nLength == 0)
			{
				gpCurWisp->MsgBox(WSTR("Invalid Search Data !"), WSTR("Error"), 0x40001, 0);
				return true;
			}

			if (!USHexStrToNum(m_pBegin->GetWindowText().operator const WCHAR*(), &m_Begin))
			{
				gpCurWisp->MsgBox(WSTR("Invalid Search Range !"), WSTR("Error"), 0x40001, 0);
				return true;
			}

			if (!USHexStrToNum(m_pEnd->GetWindowText().operator const WCHAR*(), &m_End))
			{
				gpCurWisp->MsgBox(WSTR("Invalid Search Range !"), WSTR("Error"), 0x40001, 0);
				return true;
			}

			if (m_Begin >= m_End)
			{
				gpCurWisp->MsgBox(WSTR("Invalid Search Range !"), WSTR("Error"), 0x40001, 0);
				return true;
			}

			m_bCase = true;
			memset(m_PatternBuffer, 0, sizeof(m_PatternBuffer));
			if (width == 0)
			{
				m_SearchResultLen = UnicodeToAnsi(m_pValue->m_WndText.operator const WCHAR*(), m_PatternBuffer, 64);
				m_bCase = (m_pCase->m_Style & 0x400) != 0;
			} else
			if (width == 1)
			{
				m_SearchResultLen = TStrCpyLimit(m_PatternBuffer, m_pValue->m_WndText.operator const WCHAR*(), 32)*2;
				m_bCase = (m_pCase->m_Style & 0x400) != 0;
			} else
			if (width == 2)
			{
				m_SearchResultLen = THexBytesToASCII(m_pValue->m_WndText.operator const WCHAR*(), m_PatternBuffer, 64);
				if (m_SearchResultLen == 0)
				{
					gpCurWisp->MsgBox(WSTR("Invalid Search Data !"), WSTR("Error"), 0x40001, 0);
					return true;
				}
			} else
			if (width == 3)
			{
				if (!USHexStrToNum(m_pValue->GetWindowText().operator const WCHAR*(), (unsigned short*)m_PatternBuffer))
				{
					gpCurWisp->MsgBox(WSTR("Invalid Search Data !"), WSTR("Error"), 0x40001, 0);
					return true;
				}
				m_SearchResultLen = 2;
			} else
			if (width == 4)
			{
				if (!USHexStrToNum(m_pValue->GetWindowText().operator const WCHAR*(), (unsigned long*)m_PatternBuffer))
				{
					gpCurWisp->MsgBox(WSTR("Invalid Search Data !"), WSTR("Error"), 0x40001, 0);
					return true;
				}
				m_SearchResultLen = 4;
			} else
			if (width == 5)
			{
				if (!USHexStrToNum(m_pValue->GetWindowText().operator const WCHAR*(), (ULONG_PTR*)m_PatternBuffer))
				{
					gpCurWisp->MsgBox(WSTR("Invalid Search Data !"), WSTR("Error"), 0x40001, 0);
					return true;
				}
				m_SearchResultLen = 8;
			}

			m_SearchResult = gpSyser->m_pDebugger->SearchMemory(m_Begin, m_End - m_Begin, m_PatternBuffer, m_SearchResultLen, m_bCase);
			if (m_SearchResult == -1)
			{
				gpCurWisp->MsgBox(WSTR("The special data was not found !"), WSTR("Data Search"), 0x40001, 0);
			} else
			{
				CWispBaseWnd *pWnd = (CWispBaseWnd *)GetFormItem(10);
				pWnd->EnableWindow(1);
				gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(m_SearchResult, m_SearchResultLen);
			}
			return true;
		}
		return true;
	}

	bool CSearchDialog::OnEventSearch(WISP_MSG *pMsg)
	{
		m_SearchResult = gpSyser->m_pDebugger->SearchMemory(m_SearchResultLen + m_SearchResult, m_End - m_SearchResult, m_PatternBuffer, m_SearchResultLen, m_bCase);
		if (m_SearchResult == -1)
		{
			CWispBaseWnd *pWnd = (CWispBaseWnd *)GetFormItem(10);
			pWnd->EnableWindow(0);
			gpCurWisp->MsgBox(WSTR("The special data was not found !"), WSTR("Data Search"), 0x40001, 0);
		} else
		{
			gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(m_SearchResult, m_SearchResultLen); //ULONG_PTR,unsigned long
		}
		return true;
	}

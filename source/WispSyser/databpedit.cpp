
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "databpedit.hpp"

WISP_FORM_RES_ITEM DataBPEditForm[] =
{
	{WISP_CTRL_FORM, {0, 0, 355, 290}, 0, 0xB, WSTR("Data break point"), 0, 0},
	{WISP_CTRL_STATIC_GROUP, {10, 5, 80, 40}, 0, 0, WSTR("Address"), 0, 0},
	{WISP_CTRL_EDIT, {20, 20, 60, 16}, 1, 0x1000006, 0, 0, WSTR("Address of break point")},
	{WISP_CTRL_STATIC_GROUP, {100, 5, 100, 40}, 0, 0, WSTR("Name"), 0, 0},
	{WISP_CTRL_EDIT, {110, 20, 80, 16}, 2, 0x1000006, 0, 0, WSTR("Name of break point (if needed)")},
	{WISP_CTRL_CHECK_BOX, {210, 17, 60, 20}, 3, 0x20000000, WSTR("Enabled"), 0, WSTR("Is break point enabled")},
	{WISP_CTRL_BUTTON, {280, 20, 55, 20}, WISP_ID_OK, 0, WSTR("OK"), 0, WSTR("Save modify setting")}, 
	{WISP_CTRL_STATIC_GROUP, {10, 50, 325, 40}, 0, 0, WSTR("Type"), 0, 0},
	{WISP_CTRL_RADIO_BOX, {20, 65, 75, 20}, 5, 0x20000000, WSTR("ReadWrite"), 0, WSTR("break on access memory")},
	{WISP_CTRL_RADIO_BOX, {100, 65, 75, 20}, 4, 0x20000000, WSTR("WriteOnly"), 0, WSTR("break on write memory")},
	{WISP_CTRL_RADIO_BOX, {180, 65, 75, 20}, 6, 0x20000000, WSTR("Execute"), 0, WSTR("break on execute")},
	{WISP_CTRL_RADIO_BOX, {260, 65, 75, 20}, 7, 0x20000000, WSTR("I/O"), 0, WSTR("break on IO access,address is IO Num")},
	{WISP_CTRL_STATIC_GROUP, {10, 95, 325, 45}, 0, 0, WSTR("Length"), 0, 0},
	{WISP_CTRL_RADIO_BOX, {20, 110, 75, 20}, 0xD, 0x20000000, WSTR("ALL"), 0, 0},
	{WISP_CTRL_RADIO_BOX, {100, 110, 75, 20}, 0xA, 0x20000000, WSTR("BYTE"), 0, 0},
	{WISP_CTRL_RADIO_BOX, {180, 110, 75, 20}, 0xB, 0x20000000, WSTR("WORD"), 0, 0},
	{WISP_CTRL_RADIO_BOX, {260, 110, 75, 20}, 0xC, 0x20000000, WSTR("DWORD"), 0, 0},
	{WISP_CTRL_STATIC_GROUP, {10, 150, 325, 50}, 0, 0, WSTR("Condition"), 0, 0},
	{WISP_CTRL_STATIC_DIB, {20, 170, 16, 16}, 0, 0, 0, (void*)"\\Toolbar.bmp#16#16:29", 0},
	{WISP_CTRL_EDIT, {40, 170, 285, 16}, 8, 0x1000006, 0, 0, WSTR("break condition when break point touche")},
	{WISP_CTRL_STATIC_GROUP, {10, 210, 325, 40}, 0, 0, WSTR("Do Command  (Use ; to delimit command lines)"), 0, 0},
	{WISP_CTRL_STATIC_DIB, {20, 225, 16, 16}, 0, 0, 0, (void*)"\\Toolbar.bmp#16#16:63", 0},
	{WISP_CTRL_EDIT, {40, 225, 285, 16}, 9, 0x1000006, 0, 0, WSTR("Do command when break point touched")},
	{0},
};

	WISP_MSG_MAP_BEGIN(CDataBPEdit)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
		WISP_MSG_MAP_ON_COMMAND
	WISP_MSG_MAP_END(CWispForm)

	WISP_CMD_MAP_BEGIN(CDataBPEdit)
		WISP_CMD_MAP(1, OnCmdEdit)
		WISP_CMD_MAP(WISP_ID_OK, OnCmdOK)
	WISP_CMD_MAP_END

	CDataBPEdit::CDataBPEdit()
	{
		m_pBP = 0;
		m_Resource = DataBPEditForm;
	}

	CDataBPEdit::~CDataBPEdit()
	{
	}

	bool CDataBPEdit::OnCreateForm(WISP_MSG *pMsg)
	{
		WCHAR Buffer[256];

		m_pName = (CWispEdit *)GetFormItem(2);
		m_pAddress = (CWispEdit *)GetFormItem(1);
		m_pCondition = (CWispEdit *)GetFormItem(8);
		m_pCommand = (CWispEdit *)GetFormItem(9);

		m_pEnable = (CWispCheckBox *)GetFormItem(3);

		m_pWriteOnly = (CWispRadioBox *)GetFormItem(4);
		m_pReadWrite = (CWispRadioBox *)GetFormItem(5);
		m_pExecute = (CWispRadioBox *)GetFormItem(6);
		m_pIO = (CWispRadioBox *)GetFormItem(7);
		m_pLenAll = (CWispRadioBox *)GetFormItem(0xD);
		m_pLenByte = (CWispRadioBox *)GetFormItem(0xA);
		m_pLenWord = (CWispRadioBox *)GetFormItem(0xB);
		m_pLenDWord = (CWispRadioBox *)GetFormItem(0xC);

		m_bEnableBP = true;
		if (m_pBP)
		{
			uNumToStr(m_pBP->Address, Buffer, 16);
			m_pAddress->SetWindowText(Buffer);

			AnsiToUnicode(m_pBP->Name.operator const char*(), Buffer, lenof(Buffer));
			m_pName->SetWindowText(Buffer);

			AnsiToUnicode(m_pBP->Condition.operator const char*(), Buffer, lenof(Buffer));
			m_pCondition->SetWindowText(Buffer);

			AnsiToUnicode(m_pBP->Command.operator const char*(), Buffer, lenof(Buffer));
			m_pCommand->SetWindowText(Buffer);

			m_bEnableBP = m_pBP->State != BP_STATE_DISABLE;

			if ((m_pBP->Type & 0xC) == 0xC)
				m_pReadWrite->Enable(1);
			else
			if (m_pBP->Type & 4)
				m_pWriteOnly->Enable(1);
			else
			if (m_pBP->Type & 2)
				m_pExecute->Enable(1);
			else
			if (m_pBP->Type & 0x30)
				m_pIO->Enable(1);

			switch (m_pBP->Size)
			{
			case 1:	m_pLenByte->Enable(1); break;
			case 2:	m_pLenWord->Enable(1); break;
			case 4: m_pLenDWord->Enable(1); break;
			default:m_pLenAll->Enable(1); break;
			}
		} else
		{
			m_pLenAll->Enable(1);
			m_pReadWrite->Enable(1);
		}
		m_pEnable->Enable(m_bEnableBP);
		return true;
	}

	bool CDataBPEdit::OnCmdEdit(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000705) //WISP_ID_EDIT_CHANGED
		{
			m_pName->SetWindowText(m_pAddress->m_WndText.operator const WCHAR*());
		}
		return true;
	}
	bool CDataBPEdit::OnCmdOK(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg != 0x80000002)
			return true;

		BREAK_POINT BreakPoint;
		if (!m_pAddress->m_WndText.m_nLength)
		{
			gpCurWisp->MsgBox(WSTR("Invalid Address !"), WSTR("Error"), 0x40001, 0);
			return false;
		}

		if (!gpSyser->m_SyserUI.CalcExp(m_pAddress->m_WndText.operator const WCHAR*(), &BreakPoint.Address))
		{
			gpCurWisp->MsgBox(WSTR("Invalid Address !"), WSTR("Error"), 0x40001, 0);
			return false;
		}

		char Buffer[256];

		UnicodeToAnsi(m_pName->m_WndText.operator const WCHAR*(), Buffer, lenof(Buffer));
		BreakPoint.Name = Buffer;
		if (BreakPoint.Name.IsEmpty())
			BreakPoint.Name.Format("" F0ADDR "X", BreakPoint.Address);

		ULONG_PTR Result;
		if (!gpSyser->m_SyserUI.CalcExp(m_pCondition->m_WndText.operator const WCHAR*(), &Result))
		{
			gpCurWisp->MsgBox(WSTR("Invalid Address !"), WSTR("Error"), 0x40001, 0);
			return false;
		}

		UnicodeToAnsi(m_pCondition->m_WndText.operator const WCHAR*(), Buffer, lenof(Buffer));
		BreakPoint.Condition = Buffer;

		UnicodeToAnsi(m_pCommand->m_WndText.operator const WCHAR*(), Buffer, lenof(Buffer));
		BreakPoint.Command = Buffer;
		
		m_bEnableBP = (m_pEnable->m_Style >> 10) & 1;

		BreakPoint.Type = 1;
		BreakPoint.State = m_bEnableBP ? BP_STATE_ENABLE : BP_STATE_DISABLE;

		if ((m_pReadWrite->m_Style >> 10) & 1)
			BreakPoint.Type = 0xD; //8|4|1
		else
		if ((m_pWriteOnly->m_Style >> 10) & 1)
			BreakPoint.Type = 5;
		else
		if ((m_pExecute->m_Style >> 10) & 1)
			BreakPoint.Type = 3;
		else
		if ((m_pIO->m_Style >> 10) & 1)
			BreakPoint.Type = 0x31;

		if ((m_pLenByte->m_Style >> 10) & 1)
			BreakPoint.Size = 1;
		else
		if ((m_pLenWord->m_Style >> 10) & 1)
			BreakPoint.Size = 2;
		else
		if ((m_pLenDWord->m_Style >> 10) & 1)
			BreakPoint.Size = 4;
		else	BreakPoint.Size = 0;

		if (m_pBP)
		{
			if (BreakPoint.Address == m_pBP->Address &&
			    BreakPoint.Type == m_pBP->Type &&
			    BreakPoint.State == m_pBP->State &&
			    BreakPoint.Size == m_pBP->Size)
			{
				m_pBP->Name = BreakPoint.Name;
				m_pBP->Condition = BreakPoint.Condition;
				m_pBP->Command = BreakPoint.Command;
				return true;
			}
			gpSyser->m_pSysDebugger->DelDataBP(*m_pBP); //???
		}

		m_pBP = gpSyser->m_pSysDebugger->SetDataBP(BreakPoint.Address,
						BreakPoint.Type,
						BreakPoint.State,
						BreakPoint.Size);
		if (!m_pBP)
		{
			gpCurWisp->MsgBox(WSTR("Fail to insert breakpoint !"), WSTR("Error"), 0x40001, 0);
			return false;
		}
		m_pBP->Name = BreakPoint.Name;
		m_pBP->Condition = BreakPoint.Condition;
		m_pBP->Command = BreakPoint.Command;
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
		return true;
	}

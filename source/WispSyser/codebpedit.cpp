
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "codebpedit.hpp"

WISP_FORM_RES_ITEM CodeBPEditForm[] =
{
	{WISP_CTRL_FORM, {0, 0, 385, 280}, 0, 0xB, WSTR("Code break point"), 0, 0},
	{WISP_CTRL_STATIC_GROUP, {10, 10, 325, 40}, 0, 0, WSTR("Address"), 0, 0},
	{WISP_CTRL_STATIC_DIB, {20, 25, 16, 16}, 0, 0, 0, (void*)"\\Toolbar.bmp#16#16:243", 0},
	{WISP_CTRL_EDIT, {40, 25, 285, 16}, 1, 0x1000006, 0, 0, WSTR("Address of break point")},
	{WISP_CTRL_STATIC_GROUP, {10, 60, 325, 40}, 0, 0, WSTR("Name"), 0, 0},
	{WISP_CTRL_STATIC_DIB, {20, 75, 16, 16}, 0, 0, 0, (void*)"\\Toolbar.bmp#16#16:10", 0},
	{WISP_CTRL_EDIT, {40, 75, 285, 16}, 2, 0x1000006, 0, 0, WSTR("Name of break point (if needed)")},
	{WISP_CTRL_STATIC_GROUP, {10, 110, 325, 40}, 0, 0, WSTR("Condition"), 0, 0},
	{WISP_CTRL_STATIC_DIB, {20, 125, 16, 16}, 0, 0, 0, (void*)"\\Toolbar.bmp#16#16:29", 0},
	{WISP_CTRL_EDIT, {40, 125, 285, 16}, 4, 0x1000006, 0, 0, WSTR("break condition when break point touched")},
	{WISP_CTRL_STATIC_GROUP, {10, 160, 325, 40}, 0, 0, WSTR("Do Command (Use ; to delimit command lines)"), 0, 0},
	{WISP_CTRL_STATIC_DIB, {20, 175, 16, 16}, 0, 0, 0, (void*)"\\Toolbar.bmp#16#16:63", 0},
	{WISP_CTRL_EDIT, {40, 175, 285, 16}, 5, 0x1000006, 0, 0, WSTR("Do command when break point touched")},
	{WISP_CTRL_CHECK_BOX, {20, 210, 60, 20}, 3, 0x20000000, WSTR("Enabled"), 0, WSTR("Is break point enabled")},
	{WISP_CTRL_BUTTON, {275, 210, 55, 20}, WISP_ID_OK, 0, WSTR("OK"), 0, WSTR("Save modify setting")},
	{0},
};

	WISP_MSG_MAP_BEGIN(CCodeBPEdit)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
		WISP_MSG_MAP_ON_COMMAND
	WISP_MSG_MAP_END(CWispForm)

	WISP_CMD_MAP_BEGIN(CCodeBPEdit)
		WISP_CMD_MAP(1, OnCmdEdit)
		WISP_CMD_MAP(WISP_ID_OK, OnCmdOK)
	WISP_CMD_MAP_END

	CCodeBPEdit::CCodeBPEdit()
	{
		m_pModuleBP = 0;
		m_pBP = 0;
		m_Resource = CodeBPEditForm;
	}

	CCodeBPEdit::~CCodeBPEdit()
	{
	}

	bool CCodeBPEdit::OnCreateForm(WISP_MSG *pMsg)
	{
		WCHAR Buffer[256];

		m_pName = (CWispEdit *)GetFormItem(2);
		m_pAddress = (CWispEdit *)GetFormItem(1);
		m_pCondition = (CWispEdit *)GetFormItem(4);
		m_pCommand = (CWispEdit *)GetFormItem(5);
		m_pEnable = (CWispCheckBox *)GetFormItem(3);

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
		} else
		if (m_pModuleBP)
		{
			typedef _Tmap_pair<ULONG_PTR, BREAK_POINT::MODULE> PBP;
			PBP *p = CONTAINING_RECORD_MY1(m_pModuleBP, PBP, second);
			ULONG_PTR Address = p->first; //-4 m_pModuleBP-4 TODO
			uNumToStr(Address, Buffer, 16);
			m_pAddress->SetWindowText(Buffer);
			AnsiToUnicode(m_pModuleBP->Name.operator const char*(), Buffer, lenof(Buffer));
			m_pName->SetWindowText(Buffer);
			AnsiToUnicode(m_pModuleBP->Condition.operator const char*(), Buffer, lenof(Buffer));
			m_pCondition->SetWindowText(Buffer);
			AnsiToUnicode(m_pModuleBP->Command.operator const char*(), Buffer, lenof(Buffer));
			m_pCommand->SetWindowText(Buffer);
			m_bEnableBP = m_pModuleBP->State != BP_STATE_DISABLE;
		}
		m_pEnable->Enable(m_bEnableBP);
		return true;
	}

	bool CCodeBPEdit::OnCmdEdit(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000705) //WISP_ID_EDIT_CHANGED
		{
			m_pName->SetWindowText(m_pAddress->m_WndText.operator const WCHAR*());
		}
		return true;
	}

	bool CCodeBPEdit::OnCmdOK(WISP_MSG *pMsg)
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

		if (m_pBP)
		{
			if (BreakPoint.Address == m_pBP->Address &&
			    BreakPoint.State == m_pBP->State)
			{
				m_pBP->Name = BreakPoint.Name;
				m_pBP->Condition = BreakPoint.Condition;
				m_pBP->Command = BreakPoint.Command;
				return true;
			}
			if (!gpSyser->m_SyserUI.RemoveBP(m_pBP->Address))
			{
				gpCurWisp->MsgBox(WSTR("Fail to remove breakpoint !"), WSTR("Error"), 0x40001, 0);
				return false;
			}
		} else
		if (m_pModuleBP)
		{
			typedef _Tmap_pair<ULONG_PTR, BREAK_POINT::MODULE> PBP;
			PBP *p = CONTAINING_RECORD_MY1(m_pModuleBP, PBP, second);
			ULONG_PTR Address = p->first; //-4 m_pModuleBP-4 TODO
			if (BreakPoint.Address == Address &&
			    BreakPoint.State == m_pModuleBP->State)
			{
				m_pModuleBP->Name = BreakPoint.Name;
				m_pModuleBP->Condition = BreakPoint.Condition;
				m_pModuleBP->Command = BreakPoint.Command;
				return true;
			}
			if (!gpSyser->m_SyserUI.RemoveBP(Address))
			{
				gpCurWisp->MsgBox(WSTR("Fail to remove breakpoint !"), WSTR("Error"), 0x40001, 0);
				return false;
			}
		}


		if (!gpSyser->m_SyserUI.InsertBP(BreakPoint.Address,
				BreakPoint.Name.operator const char*(),
				BreakPoint.Condition.operator const char*(),
				BreakPoint.Command.operator const char*()))
		{
			gpCurWisp->MsgBox(WSTR("Fail to insert breakpoint !"), WSTR("Error"), 0x40001, 0);
			return false;
		}

		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
		return true;
	}

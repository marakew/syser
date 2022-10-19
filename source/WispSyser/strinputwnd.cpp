
#include "strinputwnd.hpp"

	WISP_MSG_MAP_BEGIN(CStrInputWnd)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
		WISP_MSG_MAP_ON_COMMAND
	WISP_MSG_MAP_END(CWispForm)

	WISP_CMD_MAP_BEGIN(CStrInputWnd)
		WISP_CMD_MAP(WISP_ID_OK, OnCmdOK)
	WISP_CMD_MAP_END

WISP_FORM_RES_ITEM StrInput[] =
{
	{WISP_CTRL_FORM, {0, 0, 300, 100}, 0, 0xB, WSTR("Please Input ..."), 0, 0},
	{WISP_CTRL_STATIC_GROUP, {10, 10, 270, 50}, 2, 0, WSTR("Text"), 0, 0},
	{WISP_CTRL_EDIT, {20, 30, 200, 16}, 1, 0x1000006, 0, 0, 0},
	{WISP_CTRL_BUTTON, {230, 30, 40, 16}, WISP_ID_OK, 0, WSTR("OK"), 0, 0},
	{0}
};

	CStrInputWnd::CStrInputWnd()
	{
		m_szCaption = 0;
		m_szDefault = 0;
		m_Resource = StrInput;
	}

	CStrInputWnd::~CStrInputWnd()
	{
	}

	bool CStrInputWnd::OnCreateForm(WISP_MSG *pMsg)
	{
		if (m_szDefault)
		{
			CWispWnd *pWnd = (CWispWnd *)GetFormItem(2);
			pWnd->SetWindowText(m_szDefault);
		}
		if (m_szCaption)
			SetWindowText(m_szCaption);
		return true;
	}

	bool CStrInputWnd::OnCmdOK(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000002)
		{
			CWispWnd *pWnd = (CWispWnd *)GetFormItem(1);
			*m_pResult = pWnd->GetWindowText();
		}
		return true;
	}

	bool CStrInputWnd::Input(TWideString *Result, const WCHAR *szDefault, const WCHAR *szCaption)
	{
		CStrInputWnd InputWnd;
		InputWnd.m_pResult = Result;
		InputWnd.m_szDefault = szDefault;
		InputWnd.m_szCaption = szCaption;
		InputWnd.Create(0, 5);
		return InputWnd.m_Result == WISP_ID_OK;
	}


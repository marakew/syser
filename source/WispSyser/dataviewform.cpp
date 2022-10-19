
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "dataviewform.hpp"

WISP_FORM_RES_ITEM DataViewForm[] =
{
	{WISP_CTRL_FORM, {0, 0, 500, 300}, 0, 0xAB, WSTR("Data View"), 0, 0},
	{WISP_CTRL_STATIC_GROUP, {10, 10, 470, 40}, 2, 0, WSTR("Address"), 0, 0},
	{WISP_CTRL_EDIT, {20, 25, 400, 16}, 1, 0x1000006, 0, 0, 0},
	{WISP_CTRL_BUTTON, {430, 25, 40, 16}, 4, 0, WSTR("Goto"), 0, 0},
	{0},
};

	WISP_MSG_MAP_BEGIN(CDataViewForm)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
		WISP_MSG_MAP(WISP_WM_CLOSE, OnClose)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispForm)

        WISP_CMD_MAP_BEGIN(CDataViewForm)
		WISP_CMD_MAP(1, OnCmdEdit)
        WISP_CMD_MAP_END

        WISP_EVENT_MAP_BEGIN(CDataViewForm)
		WISP_EVENT_MAP(4, OnEventGoto)
        WISP_EVENT_MAP_END

	CDataViewForm::CDataViewForm()
	{
		m_Resource = DataViewForm;
	}

	CDataViewForm::~CDataViewForm()
	{
	}

	bool CDataViewForm::OnCreateForm(WISP_MSG *pMsg)
	{
		if (m_Style & 1)
		{
			AttachTitleDIB(gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x59));
		}
		m_pAddressName = (CWispEdit *)GetFormItem(1);
		m_pAddress = (CWispStaticGroup *)GetFormItem(2);
		m_pGoto = (CWispButton *)GetFormItem(4);
		m_DataView.CreateWnd(0,10,
				m_pAddressName->m_ScrWindowRect.x - m_ScrWindowRect.x + 30,
				m_pAddress->m_WindowRect.cx,
				m_ClientRect.cy - (m_pAddressName->m_ScrWindowRect.x - m_ScrWindowRect.x + 30) - 10, this, 3, 0, WISP_SH_MINSIZE);
		OnSize(0);
		return true;
	}
	bool CDataViewForm::OnClose(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.FocusActive();
		return true;
	}
	bool CDataViewForm::OnSize(WISP_MSG *pMsg)
	{
		m_pAddress->Resize(m_ClientRect.cx - 20, m_pAddress->m_WindowRect.cy, 1);
		m_pAddressName->Resize(m_pAddress->m_WindowRect.cx - m_pGoto->m_WindowRect.cx - 30,
					m_pAddressName->m_WindowRect.cy, 1);
		m_pGoto->MoveToWindow(m_pAddress->m_ScrWindowRect.x + m_pAddress->m_WindowRect.cx - m_ScrWindowRect.x - 50,
					m_pGoto->m_ScrWindowRect.y - m_ScrWindowRect.y, 1);
		m_DataView.Resize(m_pAddress->m_WindowRect.cx,
					m_ClientRect.cy - m_pAddressName->m_ScrWindowRect.x + m_ScrWindowRect.x - 40, 1);
		return true;
	}

	bool CDataViewForm::OnCmdEdit(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000703 && //WISP_ID_EDIT_KEYDOWN
		    pMsg->Command.Param1 == 13)
			UpdateContext();
		return true;
	}

	bool CDataViewForm::OnEventGoto(WISP_MSG *pMsg)
	{
		UpdateContext();
		return false;
	}
	void CDataViewForm::UpdateContext()
	{
		if (IsWindowShow())
		{
			ULONG_PTR Result;
			if (gpSyser->m_SyserUI.CalcExp(m_pAddressName->GetWindowText().operator const WCHAR*(), &Result))
			{
				SetWindowText(m_pAddressName->GetWindowText().operator const WCHAR*());
				m_DataView.ViewAddress(Result, true);
			}
		}
	}
	void CDataViewForm::Create()
	{
		if (IsWindow())
		{
			Destroy();
		} else
		{
			CWispForm::Create(0, WISP_SH_MINSIZE);
			Center(0);
		}
	}

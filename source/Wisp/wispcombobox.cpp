
#include "wispcombobox.hpp"
#include "wispbase.hpp"

#define ID_BUTTON	0x40000000
#define ID_LIST		0x40000001

	CWispComboBox::CWispComboBox()
	{
		m_nCurSel = -1;
		m_CtrlType = WISP_CTRL_COMBO_BOX;
	}
	CWispComboBox::~CWispComboBox()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispComboBox)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP_ON_EVENT
		WISP_MSG_MAP_ON_COMMAND
	WISP_MSG_MAP_END(CWispEdit)

	bool CWispComboBox::OnCreate(WISP_MSG *pMsg)
	{
		CWispDIBList *pDIBList = gpCurWisp->m_DIBLib.GetDIBList("\\Skin\\Default\\VertInc.bmp", 16, 16, 0);
		m_Button.CreateEx(pDIBList, m_ClientRect.cx - pDIBList->m_DIBList->m_PaintRect.cx, 0, this, ID_BUTTON, 0x800000);
		m_ComboList.CreateWnd(0, 0,0,m_WindowRect.cx,pDIBList->m_DIBList->m_PaintRect.cy, 0, ID_LIST, 0x10000006, WISP_SH_HIDDEN);
		m_ComboList.SetOwner(this);
		m_ComboList.InsertColumn(0, -1, 0, 0, -1);
		return true;
	}
	bool CWispComboBox::OnDestroy(WISP_MSG *pMsg)
	{
		m_ComboList.Destroy();
		return true;
	}

	WISP_EVENT_MAP_BEGIN(CWispComboBox)
		WISP_EVENT_MAP(ID_BUTTON, OnEventButton)
		WISP_EVENT_MAP(ID_LIST, OnEventComboList)
	WISP_EVENT_MAP_END

	bool CWispComboBox::OnEventButton(WISP_MSG *pMsg)
	{
		if (m_ComboList.IsWindowShow())
		{
			m_ComboList.Show(WISP_SH_HIDDEN);
		} else
		{
			m_ComboList.MoveToScreen(m_ScrWindowRect.x, m_ScrWindowRect.y + m_ScrWindowRect.cy, false);
			int Count = m_ComboList.GetItemCount(0, true);
			m_ComboList.Resize(m_ComboList.m_WindowRect.cx, m_ComboList.m_nLineHeight * Count + m_BorderSize*2, false);
			m_ComboList.Show(WISP_SH_MINSIZE);
			m_ComboList.Top(true);
			m_ComboList.Focus();
		}
		return true;
	}
	bool CWispComboBox::OnEventComboList(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000203) //WISP_ID_LBUTTON
		{
			CListStringItem *pItem = (CListStringItem *)pMsg->Command.Param2;
			m_nCurSel = m_ComboList.GetPosition(pItem);
			SetWindowText(m_ComboList.GetItemContent(pItem, 0)->String.operator const WCHAR*());
			m_ComboList.Show(WISP_SH_HIDDEN);
			Focus();
			SendCommand(0x80000400, m_nCurSel, pItem); //WISP_ID_COMBO_SELECT
		}
		return true;
	}
	bool CWispComboBox::OnCmdComboList(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000202 && (CWispButton *)pMsg->Command.Param2 != &m_Button) //WISP_ID_CHILD_LOST_FOCUS
			m_ComboList.Show(WISP_SH_HIDDEN);
		return true;
	}

	WISP_CMD_MAP_BEGIN(CWispComboBox)
		WISP_CMD_MAP(ID_LIST, OnCmdComboList)
	WISP_CMD_MAP_END

	void CWispComboBox::SetCurSel(int nSelect)
	{
		CListStringItem *pItem = m_ComboList.GetItem(nSelect, 0);
		if (pItem)
		{
			SetWindowText(m_ComboList.GetItemContent(pItem, 0)->String.operator const WCHAR*());
			m_nCurSel = nSelect;
			Update((WISP_RECT*)0);
		}
	}
	int CWispComboBox::GetCurSel()
	{
		return m_nCurSel;
	}

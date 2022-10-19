
#include "wispmsgbox.hpp"
#include "wispbutton.hpp"
#include "wispbase.hpp"

	CWispMsgBox::CWispMsgBox()
	{
		m_nReturnStatus = 0;
		m_nBtnWidth = 50;
		m_CtrlType = WISP_CTRL_MSG_BOX;
	}
	CWispMsgBox::~CWispMsgBox()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispMsgBox)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
		WISP_MSG_MAP_ON_COMMAND
	WISP_MSG_MAP_END(CWispWnd)

	bool CWispMsgBox::OnUpdateClient(WISP_MSG *pMsg)
	{
		CWispWnd::OnUpdateClient(pMsg);
		WISP_RECT rcText;
		rcText.x = m_dibIcon.m_PixelBuf.VideoBuffer != 0 ? 50 : 20;
		rcText.y = 10;
		rcText.cx = m_ClientRect.cx - rcText.x;
		rcText.cy = m_ClientRect.cy;
		m_ClientDC.DrawString(m_strText.operator const WCHAR*(), &rcText, 0, 0, -1);
		if (m_dibIcon.m_PixelBuf.VideoBuffer)
			m_ClientDC.DrawDIB(10, 10, &m_dibIcon);
		return false;
	}
	bool CWispMsgBox::OnCommand(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CtrlType == WISP_CTRL_BUTTON && pMsg->Command.CmdMsg == 0x80000002) //WISP_ID_KEYUP
		{
			m_nReturnStatus = pMsg->Command.CmdID;
			Show(WISP_SH_HIDDEN);
		}
		return true;
	}
	bool CWispMsgBox::Create(CWispBaseWnd *pParentWnd)
	{
		return CreateWnd(0,0,0,0,0, pParentWnd, 0, 0xB, WISP_SH_MINSIZE);
	}
	int CWispMsgBox::StaticShowMsgBox(const WCHAR *pText, const WCHAR *pCaption, unsigned int uType, CWispBaseWnd *pParentWnd)
	{
		CWispMsgBox MsgBox;
		if (!MsgBox.Create(pParentWnd))
			return 0;
		return MsgBox.ShowMsgBox(pText,pCaption,uType);
	}
	int CWispMsgBox::ShowMsgBox(const WCHAR *pText, const WCHAR *pCaption, unsigned int uType)
	{
		m_strText = pText;
		m_WndText = pCaption;
		
		if ( !(uType & 0x1000) )
		{
			AddDefaultButton((uType & 0xFFF) ? (uType & 0xFFF) : 1);
		}

		if (uType & 0xF0000)
			AddDefaultIcon(uType);

		WISP_RECT rcText;
		rcText.x = 0;
		rcText.y = 0;
		rcText.cx = 0;
		rcText.cy = 0;
		
		m_ClientDC.DrawString(pText, &rcText, 0x400, 0, -1);
		int TextWidth = rcText.cx + (m_dibIcon.m_PixelBuf.VideoBuffer != 0 ? 20 : 0);
		int CapWidth = m_ClientDC.GetTextExtent(pCaption, -1, 0);

		if (MAX(TextWidth, CapWidth) < m_lButtons.Size() * (m_nBtnWidth + 4))
			TextWidth = m_lButtons.Size() * (m_nBtnWidth + 4);
		else
		if (TextWidth <= CapWidth)
			TextWidth = CapWidth;

		int cx = TextWidth + 60;
		if (cx < 150) cx = 150; //MIN_LIMIT

		int cy = rcText.cy + 50;
		if (cy <= 120) cy = 120; //MIN_LIMIT

		Resize(cx, cy, true);
		Center(0);

		__int64 NextA = (cx - m_lButtons.Size() * (m_nBtnWidth + 4));
		int Next = NextA-(unsigned long)NextA; //ALIGN
		Next /= 2;

		TListIter<BTNPAIR> It = m_lButtons.Begin();
		while (It != m_lButtons.End())
		{
			Next += 2;
			CWispButton *pButton = new CWispButton;
			pButton->Create(It->pBtnText, Next, cy-50, m_nBtnWidth, m_ClientDC.m_pFont->m_Height + 6, this, It->nID, 0, 1);
			if (It->nID == WISP_ID_OK || It->nID == WISP_ID_YES)
				pButton->Focus();
			pButton->m_AdvStyle |= 1;
			Next += m_nBtnWidth;
			++It;
		}
		Show(WISP_SH_MODAL_BLOCK);
		Destroy();
		m_lButtons.Clear();
		return m_nReturnStatus;
	}
	void CWispMsgBox::AddButton(const WCHAR *pText, int nID)
	{
		BTNPAIR stPair;
		stPair.pBtnText = pText;
		stPair.nID = nID;
		int nBtnWidth = m_ClientDC.GetTextExtent(pText, -1, 0) + 4;
		MIN_LIMIT(m_nBtnWidth, nBtnWidth);
		m_lButtons.Append(&stPair);
	}
	void CWispMsgBox::AddDefaultButton(unsigned int uType)
	{
		if (uType & 0x001)
			AddButton(WSTR("OK"), WISP_ID_OK);
		if (uType & 0x020)
			AddButton(WSTR("YES"), WISP_ID_YES);
		if (uType & 0x040)
			AddButton(WSTR("NO"), WISP_ID_NO);
		if (uType & 0x002)
			AddButton(WSTR("CANCEL"), WISP_ID_CANCEL);
		if (uType & 0x100)
			AddButton(WSTR("TRY"), WISP_ID_TRY);
		if (uType & 0x080)
			AddButton(WSTR("CONTINUE"), WISP_ID_CONTINUE);
		if (uType & 0x004)
			AddButton(WSTR("ABORT"), WISP_ID_ABORT);
		if (uType & 0x008)
			AddButton(WSTR("RETRY"), WISP_ID_RETRY);
		if (uType & 0x010)
			AddButton(WSTR("IGNORE"), WISP_ID_IGNORE);
	}
	void CWispMsgBox::AddDefaultIcon(unsigned int uType)
	{
		if (uType & 0x10000)
		{
			m_dibIcon.Load("\\WispApp\\MsgBox\\Information.bmp", -1);
		} else
		if (uType & 0x20000)
		{
			m_dibIcon.Load("\\WispApp\\MsgBox\\Question.bmp", -1);
		} else
		if (uType & 0x30000)
		{
			m_dibIcon.Load("\\WispApp\\MsgBox\\Alert.bmp", -1);
		} else
		if (uType & 0x40000)
		{
			m_dibIcon.Load("\\WispApp\\MsgBox\\Error.bmp", -1);
		}
		m_dibIcon.m_Type = 2; //
		m_dibIcon.m_ColorKey = m_pWispBase->m_pWispDrawObj->m_crSystem[109];
	}
	void CWispMsgBox::RemoveAllButtons()
	{
		m_lButtons.Clear();
	}

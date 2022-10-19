
#include "wisptipwnd.hpp"
#include "wispbase.hpp"

	CWispTipWnd::CWispTipWnd()
	{
		m_CtrlType = WISP_CTRL_TIP_WND;
		m_pTipOwnerWnd = 0;
	}

	bool CWispTipWnd::OnUpdate(WISP_MSG *pMsg)
	{
		WISP_RECT rc = m_WindowRect;
		m_WindowDC.DrawFrameRect(&rc, 1, 1);

		rc.x += 1;
		rc.y += 1;
		rc.cx -= 2;
		rc.cy -= 2;
		m_WindowDC.DrawFullRect(&rc, m_pWispBase->m_pWispDrawObj->m_crSystem[111]);
		if (m_WndText.m_nLength)
		{
			rc.x += 2;
			rc.y += 1;
			rc.cx -= 4;
			rc.cy -= 2;
			m_WindowDC.DrawString(m_WndText.operator const WCHAR*(), &rc, 0, 0, -1);
		}
		return false;
	}

	void CWispTipWnd::AutoAdjustPostion(CWispWnd *pWnd)
	{
		WISP_RECT rc;
		m_WindowDC.DrawString(m_WndText.operator const WCHAR*(), &rc, 0x400, 0, -1);
		rc.cy += 5;
		rc.cx += 8;
		m_pTipOwnerWnd = pWnd;
		m_WindowDC.GetTextExtent(m_WndText.operator const WCHAR*(), -1, 0);


		rc.x = m_pWispBase->m_MousePT.x - rc.cx / 2;
		rc.y = m_pWispBase->m_MousePT.y + 20;
		if (rc.x <= 0)
			rc.x = 10;

		if (rc.y <= 10)
			rc.y = 10;

		if (rc.x2()-1 >= m_pWispBase->m_FrameBufferInfo.Width)
			rc.x = m_pWispBase->m_FrameBufferInfo.Width - rc.cx - 10;
		if (rc.y2()-1 >= m_pWispBase->m_FrameBufferInfo.Height)
			rc.y = m_pWispBase->m_MousePT.y-30;

		MoveToScreen(rc.x, rc.y, true);
		Resize(rc.cx, rc.cy, true);
		Top(true);
	}

	WISP_MSG_MAP_BEGIN(CWispTipWnd)
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
	WISP_MSG_MAP_END(CWispBaseWnd)


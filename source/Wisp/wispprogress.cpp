
#include "wispprogress.hpp"
#include "wispbase.hpp"

	CWispProgress::CWispProgress()
	{
		m_nPos = 0;
		m_nLower = 0;
		m_nStep = 1;
		m_nUpper = 9;
		m_CtrlType = WISP_CTRL_PROGRESS;
	}

	CWispProgress::~CWispProgress()
	{
		//EMPTY
	}

	WISP_MSG_MAP_BEGIN(CWispProgress)
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
	WISP_MSG_MAP_END(CWispBaseWnd)

	bool CWispProgress::OnUpdate(WISP_MSG *pMsg)
	{
		WISP_RECT rcItem;
		rcItem.y = 0;
		rcItem.x = 0;
		rcItem.cx = m_WindowRect.cx;
		rcItem.cy = m_WindowRect.cy;
		if ( (m_Style & 0x40) == 0 )
			m_pWispBase->m_pWispDrawObj->DrawCtrlClient(&m_WindowDC, &rcItem);

		int nPersent = 1000 * m_nPos / (m_nUpper - m_nLower);
		m_pWispBase->m_pWispDrawObj->DrawProgress(&m_WindowDC, &rcItem, m_Style, nPersent);
		return true;
	}

	bool CWispProgress::Create(unsigned int Style, const WISP_RECT & rect, CWispBaseWnd *pParentWnd, unsigned int nID)
	{
		return CWispBaseWnd::Create(0, rect, pParentWnd, nID, Style, 0);
	}
	
	int CWispProgress::GetPos()
	{
		return m_nPos;
	}
	void CWispProgress::SetPos(int nPos)
	{
		m_nPos = nPos;
		//Update((WISP_RECT*)0);
	}

	void CWispProgress::GetRange(int & nLower, int & nUpper)
	{
		nLower = m_nLower;
		nUpper = m_nUpper;
	}
	void CWispProgress::SetRange(int nLower, int nUpper)
	{
		m_nLower = nLower;
		m_nUpper = nUpper;
		Update((WISP_RECT*)0);
	}

	void CWispProgress::SetStep(int nStep)
	{
		m_nStep = nStep;
		//Update((WISP_RECT*)0);
	}

	void CWispProgress::StepIt()
	{
		m_nPos += m_nStep;
		//Update((WISP_RECT*)0);
	}

	void CWispProgress::OffsetPos(int nOffet)
	{
		m_nPos += nOffet;
		//Update((WISP_RECT*)0);
	}

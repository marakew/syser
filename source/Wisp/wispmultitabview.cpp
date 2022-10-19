
#include "wispmultitabview.hpp"

#define ID_TOOLBAR	0x40000003

	CWispMultiTabView::CWispMultiTabView()
	{
		m_nWithToolbar = 0;
	}
	CWispMultiTabView::~CWispMultiTabView()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispMultiTabView)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
	WISP_MSG_MAP_END(CWispTabWnd)

	bool CWispMultiTabView::OnCreate(WISP_MSG *pMsg)
	{
		if (m_nWithToolbar)
			m_Toolbar.CreateEx(this, 0, 0, ID_TOOLBAR, 0x10000);
		return true;
	}
	bool CWispMultiTabView::OnSize(WISP_MSG *pMsg)
	{
		int y = (m_TabSpaceHeight - m_Toolbar.m_WindowRect.cy)/2;
		if (!(m_Style & 0x10000))
			y += m_ClientRect.cy - m_TabSpaceHeight;
		int x = m_ClientRect.cx - m_Toolbar.m_WindowRect.cx - 2;
		if (m_Toolbar.IsWindow())
			m_Toolbar.MoveToClient(x, y, true);
		return true;
	}
	bool CWispMultiTabView::OnDestroy(WISP_MSG *pMsg)
	{
		return true;
	}

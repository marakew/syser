
#include "wispprogressform.hpp"

WISP_FORM_RES_ITEM WispProgressForm[] =
{
	{WISP_CTRL_FORM, {0, 0, 325, 60}, 0, 6, 0, 0, 0},
	{WISP_CTRL_STATIC_STRING, {10, 10, 300, 16}, 2, 0, 0, 0, 0},
	{WISP_CTRL_PROGRESS, {10, 30, 300, 16}, 1, 0, 0, 0, 0},
	{0},
};

	CWispProgressForm::CWispProgressForm()
	{
		m_pProgress = NULL;
		m_pStaticStr = NULL;
		m_Resource = WispProgressForm;
	}
	CWispProgressForm::~CWispProgressForm()
	{
	}

        WISP_MSG_MAP_BEGIN(CWispProgressForm)
                WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
        WISP_MSG_MAP_END(CWispForm)

	bool CWispProgressForm::OnCreateForm(WISP_MSG *pMsg)
	{
		m_pProgress = (CWispProgress *)GetFormItem(1);
		m_pProgress->SetRange(0, 99);
		m_pProgress->m_nPos = 0;
		m_pStaticStr = (CWispStaticStr *)GetFormItem(2);
		return true;
	}

	void CWispProgressForm::SetText(const WCHAR *Text)
	{
		m_pStaticStr->SetWindowText(Text);
	}

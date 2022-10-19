
#ifndef _WISPFORM_HPP_
#define _WISPFORM_HPP_

#include "../Code/define.h"
#include "../Code/alt/altmap.hpp"
#include "wispwnd.hpp"

struct WISP_FORM_RES_ITEM
{
	unsigned int Type;
	WISP_RECT rc;
	unsigned int CmdID;
	unsigned int Style;
	const WCHAR *pName;
	void *pData;
	const WCHAR *pTipString;
};

struct CWispForm : public CWispWnd
{
	CWispForm();
	virtual ~CWispForm();
	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnEvent(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnCmdOk(WISP_MSG *pMsg);
	bool OnCmdCancel(WISP_MSG *pMsg);
	bool OnCmdDefault(WISP_MSG *pMsg);

	bool LoadForm(WISP_FORM_RES_ITEM *ItemAry);
	bool Create(CWispWnd *pParentWnd, int ShowMode);
	virtual bool Destroy() override;
	CWispBaseWnd *GetFormItem(unsigned int CmdID);
	map<unsigned int, CWispBaseWnd *> m_FormItemMap;

	unsigned int GetFormResult();
	unsigned int m_Result;
	WISP_FORM_RES_ITEM *m_Resource;
};

#endif
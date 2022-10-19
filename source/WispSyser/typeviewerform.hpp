
#ifndef _TYPEVIEWERFORM_HPP_
#define _TYPEVIEWERFORM_HPP_

#include "../Wisp/wispform.hpp"
#include "../Wisp/wisplist.hpp"
#include "../Wisp/wispedit.hpp"
#include "../Wisp/wispbutton.hpp"
#include "../Wisp/wispradiobox.hpp"
#include "../Wisp/wispstatic.hpp"
#include "../EXEAnalyzer/sdsmodule.hpp"

struct CStructViewerForm : public CWispForm
{
	CWispStaticStr *m_ItemDescription;
	CWispStaticStr *m_ItemAddressStr;
	CWispList *m_ItemOptions;
	CWispButton *m_ItemOk;
	CWispButton *m_ItemCancel;
	CWispRadioBox *m_ItemFloatingType;
	CWispRadioBox *m_ItemLockType;
	CWispRadioBox *m_ItemDec;
	CWispRadioBox *m_ItemHex;
	unsigned long X;
	CWispEdit *m_ItemAddress;
	CWispDIBList m_DIBList;
	ULONG_PTR m_Address;
	CSDSModule *m_pSDSMod;

	CStructViewerForm();
	virtual ~CStructViewerForm();

        virtual bool MsgProc(WISP_MSG *pMsg) override;

        bool OnCreateForm(WISP_MSG *pMsg);
        bool OnDestroy(WISP_MSG *pMsg);
        bool OnCommand(WISP_MSG *pMsg);
        bool OnCmdSelect(WISP_MSG *pMsg);
        bool OnEvent(WISP_MSG *pMsg);
        bool OnEventAdd(WISP_MSG *pMsg);
        bool OnEventClose(WISP_MSG *pMsg);
        bool OnEventLock(WISP_MSG *pMsg);
        bool OnEventFloating(WISP_MSG *pMsg);

	bool Create(CWispWnd *pParentWnd, int ShowMode, ULONG_PTR Address);
	void UpdateContext();
};

#endif
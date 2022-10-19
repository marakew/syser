
#ifndef _DATAVIEW_HPP_
#define _DATAVIEW_HPP_

#include "../Wisp/wisphexwnd.hpp"
#include "../Wisp/wispmenu.hpp"
#include "../Wisp/wisptoolbar.hpp"
#include "dataoperatordlg.hpp"
#include "typeviewerform.hpp"

struct CDataView : public CWispHexWnd
{
	ULONG_PTR m_RButtonAddress;
	ULONG_PTR m_LButtonAddress;
	unsigned long XXX;
	CWispMenu m_PopupMenu;
	CWispToolbar m_Toolbar;
	CDataOperatorDlg m_OperatorDlg;
	CStructViewerForm m_StructViewerForm;
	//CFindStringForm m_FindStringForm;
	map<unsigned int, unsigned int> m_KeyMap;

	CDataView();
	virtual ~CDataView();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	void OnKeyLButton(WISP_MSG *pMsg);
	void OnKeyRButton(WISP_MSG *pMsg);
	unsigned int GetKeyMap(unsigned int CmdID);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	bool OnEventGotoData(WISP_MSG *pMsg);
	bool OnEventNewData(WISP_MSG *pMsg);
	bool OnEventGotoDasm(WISP_MSG *pMsg);
	bool OnEventNewDasm(WISP_MSG *pMsg);
	bool OnEventDasmPhysAddr(WISP_MSG *pMsg);
	bool OnEventShowPhysAddr(WISP_MSG *pMsg);
	bool OnEventToggleToolbar(WISP_MSG *pMsg);
	bool OnEventAttachType(WISP_MSG *pMsg);
	bool OnEventFindForm(WISP_MSG *pMsg);
	bool OnEventToggleOffsetMode(WISP_MSG *pMsg);
	bool OnEventToggleAddressMode(WISP_MSG *pMsg);
	bool OnEventDataWidth(WISP_MSG *pMsg);

	bool OnEventNewView(WISP_MSG *pMsg);

	bool OnEventOp(WISP_MSG *pMsg);

	bool OnEventShowDataAt(WISP_MSG *pMsg);

	bool OnEventShowDasm(WISP_MSG *pMsg);

	bool OnEventCopyVal(WISP_MSG *pMsg);
	bool OnEventCopyAddr(WISP_MSG *pMsg);

	bool OnEventCopySelect(WISP_MSG *pMsg);

	bool OnEventToggleFloatAddressMode(WISP_MSG *pMsg);

	bool OnEventNextHistory(WISP_MSG *pMsg);
	bool OnEventPrevHistory(WISP_MSG *pMsg);

	virtual unsigned long WriteMemory(ULONG_PTR Address, void *Buffer, unsigned long Size) override;
	virtual unsigned long ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size) override;

	virtual unsigned int GetHexByteAttribute(ULONG_PTR Address) override;

	virtual ULONG_PTR GetFloatAddress(const TWideString & FloatAddress) override;

	virtual void OnChangeView() override;
	virtual void OnDataUpdated() override;
	virtual int GetStartLine() override;

	bool GetDataRange(ULONG_PTR *Start, ULONG_PTR *End, unsigned long *Len);
	void LoadKeyMap();
};

#endif
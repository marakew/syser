
#ifndef _STACKWATCHLIST_HPP_
#define _STACKWATCHLIST_HPP_

#include "../Code/alt/altmap.hpp"
#include "../Wisp/wisplist.hpp"
#include "../Wisp/wispmenu.hpp"

enum
{
  SWL_MODE_ADDR = 0,
  SWL_MODE_REL = 1,
  SWL_MODE_CSP = 2,
  SWL_MODE_CBP = 3,
};

struct CStackWatchList : public CWispList
{
	int m_nScrollLine;
	ULONG_PTR m_CurAddr;
	CWispMenu::MenuItem *m_pMenuShowCode;
	CWispMenu::MenuItem *m_pMenuShowData;
	CWispMenu::MenuItem *m_pMenuOrder;
	CWispMenu m_PopupMenu;
	int m_Mode;
	ULONG_PTR m_RelCIP;
	bool m_bOrder;
	map<unsigned int, unsigned int> m_KeyMap;
#ifndef _X64_
	map<unsigned long, unsigned long> m_SEHMap_1;
	map<unsigned long, unsigned long> m_SEHMap_2;
#endif

	CStackWatchList();
	virtual ~CStackWatchList();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnUpdate(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnSize(WISP_MSG *pMsg);
	bool OnScrollEvent(WISP_MSG *pMsg);
	void OnKeyRButton(WISP_MSG *pMsg);
	unsigned int GetKeyMap(unsigned int CmdID);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	bool OnEventShowCode(WISP_MSG *pMsg);
	bool OnEventGotoCode(WISP_MSG *pMsg);
	bool OnEventShowData(WISP_MSG *pMsg);
	bool OnEventGotoData(WISP_MSG *pMsg);
	bool OnEventAddressMode(WISP_MSG *pMsg);
	bool OnEventOffsetMode(WISP_MSG *pMsg);
	bool OnEventRelCSP(WISP_MSG *pMsg);
	bool OnEventRelCBP(WISP_MSG *pMsg);
	bool OnEventToggleOrder(WISP_MSG *pMsg);
	bool OnEventShowCBP(WISP_MSG *pMsg);
	bool OnEventShowCSP(WISP_MSG *pMsg);

	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdVSpinScroll(WISP_MSG *pMsg);

	virtual void OnLButton(CListStringItem *Item, int nCol) override;

	virtual bool Edit(CListStringItem *Item, int nCol, TWideString& String) override;
	virtual bool OnEditing(CListStringItem *Item, int nCol, TWideString& String) override;

	void DrawContextSEH(ULONG_PTR Address, ULONG_PTR Value, unsigned long *pColorString, int MaxSize);
	bool DrawContext(ULONG_PTR Address, ULONG_PTR Value, unsigned long *pColorString, int MaxSize, unsigned char *pType);
	void GetAddress(ULONG_PTR Address, WCHAR *pString);
	void LoadKeyMap();
	void ResetContext();
	void UpdateContextSEH();
	void UpdateContext(ULONG_PTR Addr, bool bCurAddr);
};

#endif
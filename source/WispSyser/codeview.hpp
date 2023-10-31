
#ifndef _CODEVIEW_HPP_
#define _CODEVIEW_HPP_

#include "../Code/alt/altlist.hpp"
#include "../Code/alt/altmap.hpp"
#include "../Wisp/wisplist.hpp"
#include "../Wisp/wispmenu.hpp"
#include "../Wisp/wispdib.hpp"
#include "syserui.hpp"
#include "syserconfig.hpp"

enum
{
  CV_START = 0,
  CV_ADDR = 1,
  CV_CODE = 2,
  CV_INST = 3,
  CV_CMT = 4,
  CV_MAX = 5,
};

struct LINEDIB
{
	ULONG_PTR eip;
	ULONG_PTR inst;
	unsigned long index;
	unsigned long offs;
};

#define MAX_LINEDIB 5

enum CV_COLOR
{
  COLOR_NULL = 0,
  COLOR_BG = 1,
  COLOR_ADDRESS = 2,
  COLOR_PREFIX = 3,
  COLOR_OPCODE = 4,
  COLOR_REGISTER = 5,
  COLOR_SEG_REG = 6,
  COLOR_IMMED = 7,
  COLOR_OPTR = 8,
  COLOR_SYMBOL = 9,
  COLOR_COMMENT = 10,
  COLOR_KEYWORD = 11,
  COLOR_STRING = 12,
  COLOR_BPX_STRIP = 13,
  COLOR_EIP_STRIP = 14,
  COLOR_BPX_EIP_STRIP = 15,
  COLOR_API_NAME = 16,
  COLOR_PARAM_TYPE = 17,
  COLOR_PARAM_NAME = 18,
  COLOR_JMP_LINE = 19,
  COLOR_ACTIVE_JMP_LINE = 20,
  COLOR_PREFIX_BYTE = 21,
  COLOR_OPCODE_BYTE = 22,
  COLOR_REGRM_BYTE = 23,
  COLOR_SIB_BYTE = 24,
  COLOR_OTHER_OPCODE_BYTE = 25,
  COLOR_AMD3DNOW_BYTE = 26,
  COLOR_MAX = 27,
};

struct CCodeView : public CWispList
{
	CWispDIBList *m_pLineMarkDIBList;
	TWideString Str;
	unsigned long m_nScrollLine;
	unsigned long m_Mode;
	ULONG_PTR m_RelAddr;
	ULONG_PTR m_CurAddr;
	ULONG_PTR m_EndAddr;
	unsigned long m_Color[COLOR_MAX];
	CSyserCodeDoc *m_pSyserCodeDoc;
	CWispDIBList m_DIBList;
	TList<ULONG_PTR> m_History;
	TListIter<ULONG_PTR> m_HistoryIter;
	CListStringItem *m_pRButtonItem;
	int m_nRButtonCol;
//	XXXXXXXXXXXXXXXXXXXXfield_DB0;
	CWispMenu m_PopupMenu;
//	XXXXXXXXXXXXXXXXXXXXfield_1034;
	LINEDIB m_LineDIB[MAX_LINEDIB];
	unsigned long m_LineJmpCount;
	int m_CurLineJmp;
	unsigned long m_LineJmpWidth;
	WCHAR m_szKeyword[256];
//	XXXXXXXXXXXXXXXXXXXXfield_1294;
	map<unsigned int, unsigned int> m_KeyMap;

	CCodeView();
	virtual ~CCodeView();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnUpdate(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);

	void OnKeyLButton(WISP_MSG *pMsg);
	void OnKeyRButton(WISP_MSG *pMsg);

	bool OnKeyEvent(WISP_MSG *pMsg);

	bool OnEventShowAt(WISP_MSG *pMsg);

	bool OnMouseDblClick(WISP_MSG *pMsg);
	bool OnSize(WISP_MSG *pMsg);
	bool OnUpdateClient(WISP_MSG *pMsg);
	bool OnChar(WISP_MSG *pMsg);
	bool OnGetFocus(WISP_MSG *pMsg);
	bool OnScrollEvent(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdFindString(WISP_MSG *pMsg);
	bool OnCmdVSpinScroll(WISP_MSG *pMsg);

	bool OnEvent(WISP_MSG *pMsg);
	bool OnEventResetPos(WISP_MSG *pMsg);
	bool OnEventSavePos(WISP_MSG *pMsg);
	bool OnEventNextPos(WISP_MSG *pMsg);
	bool OnEventPrevPos(WISP_MSG *pMsg);

	bool OnEventSearchOpRef(WISP_MSG *pMsg);
	bool OnEventSearchTrRef(WISP_MSG *pMsg);

	bool GetOpRefList(ULONG_PTR Address, TList<ULONG_PTR> & Addrs, ULONG_PTR & Return);
	bool OpenOpRefList(ULONG_PTR Address);
	unsigned long GetTRefList(ULONG_PTR Address, TList<ULONG_PTR> & Addrs);
	bool OpenTRefList(ULONG_PTR Address);

	bool OnEventToggleBM(WISP_MSG *pMsg);

	bool OnEventFollowAuto(WISP_MSG *pMsg);
	bool OnEventFollowCodeView(WISP_MSG *pMsg);
	bool OnEventFollowSourceView(WISP_MSG *pMsg);

	bool OnEventAnalyze(WISP_MSG *pMsg);

	bool OnEventModeAddr(WISP_MSG *pMsg);
	bool OnEventModeOffs(WISP_MSG *pMsg);

	bool OnEventSymSwitch(WISP_MSG *pMsg);
	bool OnEventFullSym(WISP_MSG *pMsg);

	bool OnEventModeRelCIP(WISP_MSG *pMsg);

	bool OnEventEditCode(WISP_MSG *pMsg);
	bool OnEventEditComment(WISP_MSG *pMsg);

	bool OnEventStrRef(WISP_MSG *pMsg);
	bool OnEventStrFind(WISP_MSG *pMsg);

	bool OnEventNextCIPTrace(WISP_MSG *pMsg);
	bool OnEventPrevCIPTrace(WISP_MSG *pMsg);

	bool OnEventNextAddr(WISP_MSG *pMsg);
	bool OnEventPrevAddr(WISP_MSG *pMsg);

	bool OnEventNewView(WISP_MSG *pMsg);
	bool OnEventGotoHere(WISP_MSG *pMsg);

	bool OnEventToggleAddBP(WISP_MSG *pMsg);
	bool OnEventToggleEnableBP(WISP_MSG *pMsg);
	bool OnEventCopy(WISP_MSG *pMsg);

	virtual void OnLButton(CListStringItem *Item, int nCol) override;
	virtual void OnRButton(CListStringItem *Item, int nCol) override;

	virtual bool Edit(CListStringItem *Item, int nCol, TWideString& String) override;
	virtual bool OnEditing(CListStringItem *Item, int nCol, TWideString& String) override;

	bool EditAsm(CListStringItem *Item, TWideString& String);

	unsigned long *OptItemToCS(_OPERAND_ITEM *OpArray, int nOp, unsigned long *pColorString, _INSTRUCTION_INFORMATION *pDasmInstr);
	unsigned long *InstrToCS(_INSTRUCTION_INFORMATION *pDasmInstr, unsigned long *pColorString);
	unsigned long ColorDasm(ULONG_PTR Address, unsigned long MaxSize, unsigned long *ColorString, unsigned long nCodeAlign, _INSTRUCTION_INFORMATION *pInstInfo, unsigned char *pCodeBuff);
	void GetAddress(ULONG_PTR Address, WCHAR *szAddress);
	void ShowAPICall(ULONG_PTR Address, const char *string, CListStringItem *Item, unsigned int nLine);
	bool UpdateDasm();
	void UpdateStaticText();
	void GotoAddr(ULONG_PTR Address, bool bHistory);
	void SelectAddress(ULONG_PTR Address);
	void SelectAddr(ULONG_PTR Address);
	void LoadKeyMap();
	void GetCodeRange(ULONG_PTR *pStart, ULONG_PTR *pEnd);
	bool GetJumpPos(ULONG_PTR Address, unsigned long *pJmpPos);
	void AddLineJmp();
	void DrawLineJmp(LINEDIB *LineDIB, int Pos, unsigned long Color);

	void AddPos(ULONG_PTR Addr, ULONG_PTR PrevAddr);
	void ResetPos();
	void SavePos();
	void NextPos();
	void PrevPos();
};

#endif
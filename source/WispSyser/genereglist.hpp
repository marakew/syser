
#ifndef _GENEREGLIST_HPP_
#define _GENEREGLIST_HPP_

#include "../Wisp/wisplist.hpp"
#include "../Wisp/wispmenu.hpp"
#include "../EXEAnalyzer/x86debugger.hpp"

enum REG_GENE_INDEX
{
  REG_GENE_CAX = 0,
  REG_GENE_CBX,
  REG_GENE_CCX,
  REG_GENE_CDX,
  REG_GENE_CSI,
  REG_GENE_CDI,
  REG_GENE_CBP,
  REG_GENE_CSP,
#ifdef _X64_
  REG_GENE_R8,
  REG_GENE_R9,
  REG_GENE_R10,
  REG_GENE_R11,
  REG_GENE_R12,
  REG_GENE_R13,
  REG_GENE_R14,
  REG_GENE_R15,
#endif
  REG_GENE_CIP,

  REG_GENE_EFLAG,

  REG_GENE_CS,
  REG_GENE_DS,
  REG_GENE_ES,
  REG_GENE_FS,
  REG_GENE_GS,
  REG_GENE_SS,

  REG_GENE_AF,
  REG_GENE_CF,
  REG_GENE_DF,
  REG_GENE_IF,
  REG_GENE_OF,
  REG_GENE_PF,
  REG_GENE_SF,
  REG_GENE_ZF,

  REG_GENE_BASE_CS,
  REG_GENE_BASE_DS,
  REG_GENE_BASE_ES,
  REG_GENE_BASE_FS,
  REG_GENE_BASE_GS,
  REG_GENE_BASE_SS,
#ifdef _RING0_
  REG_GENE_GDTR,
  REG_GENE_IDTR,
  REG_GENE_LDTR,
  REG_GENE_TR,

  REG_GENE_BASE_GDTR,
  REG_GENE_LIMIT_GDTR,

  REG_GENE_BASE_IDTR,
  REG_GENE_LIMIT_IDTR,
#endif
  REG_GENE_REGCOUNT,
};

struct CPUREG
{
	ULONG_PTR CAX;
	ULONG_PTR CBX;
	ULONG_PTR CCX;
	ULONG_PTR CDX;
	ULONG_PTR CDI;
	ULONG_PTR CSI;
	ULONG_PTR CBP;
	ULONG_PTR CSP;
#ifdef _X64_
	ULONG_PTR R8;
	ULONG_PTR R9;
	ULONG_PTR R10;
	ULONG_PTR R11;
	ULONG_PTR R12;
	ULONG_PTR R13;
	ULONG_PTR R14;
	ULONG_PTR R15;
#endif
	ULONG_PTR CIP;
//	unsigned long EFL;
	EFL_REG EFL;
	unsigned short CS;
	unsigned short DS;
	unsigned short ES;
	unsigned short FS;
	unsigned short GS;
	unsigned short SS;

	ULONG_PTR DR[8];
};

struct CGeneRegList : public CWispList
{
	CListStringItem *m_hGeneRegItem[REG_GENE_REGCOUNT];
	CPUREG m_CPUREG;
	unsigned long m_ColorChanges;
	unsigned long m_ColorSame;
	CListStringItem *m_pEditItem;
	CWispMenu m_PopupMenu;
	CWispMenu::MenuItem *m_MenuItemShowCode;
	CWispMenu::MenuItem *m_MenuItemShowData;
	int m_nCPU;
	X86_CPU_REG_PTR m_CPUREGPTR;
	map<unsigned int, unsigned int> m_KeyMap;

	CGeneRegList();
	virtual ~CGeneRegList();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnUpdate(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnClose(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
//	bool OnGetFocus(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	
	bool OnEventGotoData(WISP_MSG *pMsg);
	bool OnEventGotoCode(WISP_MSG *pMsg);
	bool OnEventCode(WISP_MSG *pMsg);
	bool OnEventData(WISP_MSG *pMsg);

	virtual void OnLButton(CListStringItem *Item, int nCol) override;

	virtual bool Edit(CListStringItem *Item, int nCol, TWideString& String) override;
	virtual bool OnEditing(CListStringItem *Item, int nCol, TWideString& String) override;

	void ResetContext();
	void UpdateFlags();
	void ReadString(X86_CPU_REG_PTR *pCPURegPtr);
	void UpdateContext();
	void LoadKeyMap();
	void ReadStringTrace(X86_CPU_REG *pCPUReg);
	void UpdateContextTrace(unsigned long CPUType, X86_CPU_REG *pCPUReg, ULONG_PTR CIP);
};

#endif
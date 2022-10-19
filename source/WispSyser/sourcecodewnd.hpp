
#ifndef _SOURCECODEWND_HPP_
#define _SOURCECODEWND_HPP_

//#include "../Wisp/wispbase.hpp"
//#include "SyserRing3.hpp"
#include "syserui.hpp"
#include "../Code/alt/altmap.hpp"
#include "../Code/srctxtfile.hpp"
#include "../Wisp/wisplist.hpp"
#include "currentfunction.hpp"
#include "../EXEAnalyzer/sdsmodule.hpp"

struct CSourceCodeWnd : public CWispList
{
	unsigned long m_Color[41];
	CSrcTXTFile m_SrcTxtFile;
	bool m_FileOpen;
	unsigned long m_TxtFileLineNumber;
	map<unsigned long, MAPFilesLN> m_FileLNMap;
	CCurrentFunction m_CurrentFunction;
	bool m_bComment;
	//???
	CSyserCodeDoc *m_pSyserCodeDoc;
	CWispDIBList *m_pLineMarkDIBList;
	unsigned long m_CurrentFileID;
	CSDSModule *m_pSDSMod;
	CWispMenu m_PopupMenu;
	WISP_POINT m_LogicMousePosWnd;
	map<unsigned long,CListStringItem *> m_Bookmark;
	ULONG_PTR m_CurAddr;

	TAnsiString m_WatchVar;
	TAnsiString m_WatchVarComplex;

	CSourceCodeWnd();
	virtual ~CSourceCodeWnd();

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool IsAddWatch();
	void OnPopupMenu(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	bool OnUpdateClient(WISP_MSG *pMsg);

	bool OnEventExpansion(WISP_MSG *pMsg);
	bool OnEventCollapse(WISP_MSG *pMsg);
	bool OnEventGotoLine(WISP_MSG *pMsg);
	bool OnEventAddWatch(WISP_MSG *pMsg);
	bool OnEventToggleBM(WISP_MSG *pMsg);
	unsigned long GetAddr(CListStringItem *Item);
	bool OnEventInsDelBP(WISP_MSG *pMsg);
	bool OnEventEnDisBP(WISP_MSG *pMsg);
	bool OnEventGotoHere(WISP_MSG *pMsg);
	bool OnEventShowHere(WISP_MSG *pMsg);

	virtual void OnLButton(CListStringItem *Item, int nCol) override;
	virtual void OnRButton(CListStringItem *Item, int nCol) override;

	virtual bool GetTipText(CListStringItem *Item, int nCol, int *pPosX, int *pPosY, WCHAR *pOutText) override;

	virtual bool Edit(CListStringItem *Item, int nCol, TWideString& String) override;
	virtual bool OnEditing(CListStringItem *Item, int nCol, TWideString& String) override;

	void ClearBM();
	bool IsBM(unsigned long Address);
	void ToggleBM();
	void NextBM();
	void PrevBM();

	void ShowLineNum(unsigned long LineNumber);

	unsigned long *OptItemToCS(_OPERAND_ITEM *OpArray, int nOp, unsigned long *pColorString, _INSTRUCTION_INFORMATION *pDasmInstr);
	unsigned long *InstrToCS(_INSTRUCTION_INFORMATION *pDasmInstr, unsigned long *pColorString);
	unsigned long *ColorDasm(ULONG_PTR ImageBase, ULONG_PTR Address, unsigned int MaxSize, unsigned long *pColorString, int nCodeAlign);
	void GetColorKeyword(char *String, unsigned long *pColorString, int *pLen);
	void OpenSrcFile(const char *Text, unsigned int Size, SrcFileNameID *SrcFile);
	void UpdateContextSrc();
	bool UpdateContextPics();

	bool GetOutput(char *Str, CListStringItem *Item, TAnsiString & String);
};

#endif


#ifndef _WISPCONSOLEWND_HPP_
#define _WISPCONSOLEWND_HPP_

#include "../Code/define.h"
#include "../Code/alt/altqueue.hpp"
#include "../Code/alt/altstack.hpp"
#include "../Code/cmdparser.hpp"
#include "../Code/options.hpp"
#include "wispcolorstrwnd.hpp"

#define WISP_MAX_CMD_SIZE 257

struct CWispTermnialDelegate
{
	virtual void Write(void *Buffer, int Length) = 0;
};

struct CWispConsoleWnd;

struct CWispTerminalWnd : public CWispColorStrWnd
{
	CWispTerminalWnd();
	virtual ~CWispTerminalWnd();

	void SetHistoryLines(int nLine);
	void ClearScreen();
	void OutPut(WCHAR ch);
	void OutPut(const WCHAR *szLine);
	void OutPut(const unsigned long *szLine);
	int VPrintf(const WCHAR *format, va_list ParamList);
	void Printf(const WCHAR *szLine, ...);
	void AttachConsole(CWispConsoleWnd* pConsoleWnd);
	void SetFontColor(unsigned char ColorIndex);
	void SaveFontColor();
	void RestoreFontColor();
	void EnsureLineVisible();
	void EnsureInputVisible();
	void EnsureInputVisible2();
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnChar(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdGetCmd(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);
	bool OnEventPaste(WISP_MSG *pMsg);
	virtual unsigned char GetLineStr(int LineIndex, unsigned long *pColorStr) override;

	bool SaveHistory(const char *FileName);

	virtual bool OnAcceptingCmd(const WCHAR *szCmd) { return true; }
	virtual void OnAcceptCmd(const WCHAR *szCmd) { }

	CWispConsoleWnd *m_pConsoleWnd;
	TQueue<TColorString,64> m_LineList;
	TStack<TWideString,64> m_CmdHistroy;
	TWideString m_CommandStr;
	TWideString m_InputStr;
	TWideString m_Prefix;
	TWideString m_OutPutStr;
	unsigned int m_uFlags;
	int m_CurHisIndex;
	unsigned char m_TextColor;
	unsigned char m_OldTextColor;
	bool m_bNewLine;
	CWispTermnialDelegate *m_pDelegate;
};

struct CWispConsoleWnd : public CWispTerminalWnd, public CCmdParser
{
	COptions m_Options;
	CWispTerminalWnd *m_pTerminalWnd;
	
	CWispConsoleWnd();
	virtual ~CWispConsoleWnd();

	virtual void OnBeginCmd(const WCHAR *szCmd);// {}
	virtual void OnCmd(const WCHAR *szCmd);// {}
	virtual void OnEndCmd(const WCHAR *szCmd);// {}

	virtual void PrintString(const WCHAR *pString) override;

	static int Cls(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd);
	static int WispVer(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd);
	static int HL(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd);
	static int HCC(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd);
	static int HC(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd);
	static int Color(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd);
	static int BKColor(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd);
	static int Help(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd);
	static int Bat(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd);
	static int Set(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd);
	static int Rem(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd);
	static int Echo(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd);

	static int HeapInfo(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd);

	void DisplayColor(CWispTerminalWnd *pTermWnd);

	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);
};

#endif

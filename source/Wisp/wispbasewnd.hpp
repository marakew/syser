
#ifndef _WISPBASEWND_HPP_
#define _WISPBASEWND_HPP_

#include "../Code/define.h"
#include "../Code/tstrmem.hpp"

#include "utility.hpp"
#include "wispdc.hpp"
#include "wispmsg.hpp"

	//forward decl

	struct CWispBase; //???
	struct WISP_TAB_WND_ITEM; //???
	struct WISP_KEY_EVENT; //???
	struct WISP_TIMER; //???
	//==========

	//ShowMode

	enum WISP_SH
	{
		WISP_SH_NORMAL = 0,
		WISP_SH_MINSIZE = 1,
		WISP_SH_FULL_SCR = 2,
		WISP_SH_HIDDEN = 3,
		WISP_SH_MODAL = 4,
		WISP_SH_MODAL_BLOCK = 5,
	};

	//m_Style

	enum WISP_WSTYLE
	{
		WISP_WSTYLE_CAPTION = 1,	//1<<0
		WISP_WSTYLE_BORDER = 2,		//1<<1
		WISP_WSTYLE_TIP_BORDER = 4,	//1<<2 0x4 ???
		WISP_WSTYLE_BT_CLOSE = 8,	//1<<3
		WISP_WSTYLE_BT_MAX = 0x10,	//1<<4
		WISP_WSTYLE_BT_MIN = 0x20,	//1<<5
		WISP_WSTYLE_DISABLE_BK = 0x40,	//1<<6
		WISP_WSTYLE_RESIZE = 0x80,	//1<<7
		WISP_WSTYLE_CHILD = 0x100,	//1<<8
		WISP_WSTYLE_DISABLE = 0x200,	//1<<9
		//CHECKED			//1<<10 0x400
		WISP_WSTYLE_FOCUS = 0x800,	//1<<11 0x800
						//1<<12 0x1000
						//1<<13 0x2000
						//1<<14 0x4000
		WISP_WSTYLE_WND = 0x8000,	//1<<15 0x8000
		//1<<16 0x010000 WISP_WSTYLE_VERT
		//1<<17 0x020000 WISP_WSTYLE_HORZ LIST BOX EXPAND !
		//1<<18 0x040000 WISP_WSTYLE_STATIC LIST BOX CHILD RESIZE VERT DISABLED CONST
		//1<<19 0x080000
		//1<<20 0x100000 AUTO WIDTH		SCROLL H
		//1<<21 0x200000 FOCUSED / CLOSE BUTTON SCROLL V
		//1<<22 0x400000 LIST BOX CHECKED !
		//1<<23 0x800000 LIST BOX CTRL UNSELECT / FIXED

		//1<<24 0x1000000 LIST ICON
		//1<<25 0x2000000 LIST BOX DRAG
		//1<<26 0x4000000 LIST BOX PROLOG-EPILOG ADDING END
		//1<<27 0x8000000

		//1<<28 0x10000000 LIST BOX AUTO SELECT
		//1<<29 0x20000000 ADD END
		//1<<30 0x40000000 LIST BOX AUTO UNSELECT/SELECT FOCUS
		//1<<21 0x80000000 WISP_WSTYLE_MULTILINE
	};

	//m_AdvStyle

	enum WISP_ADV_STYLE
	{
		WISP_ADV_AUTODEL = 1,		//1<<0
		WISP_ADV_HINT	 = 2,		//1<<1
		WISP_ADV_CANMOVE = 4,		//1<<2
						//1<<3
	};

	//m_State

	enum WISP_WSTATE
	{
		WISP_WSTATE_RESET	= 0,
		WISP_WSTATE_MOVE	= 1,	//1<<0
		WISP_WSTATE_RESIZE	= 2,	//1<<1
		WISP_WSTATE_MAXIMIZE	= 4,	//1<<2
		WISP_WSTATE_MINIMIZE	= 8,	//1<<3
		WISP_WSTATE_INIT = 0xffffffff,
	};

	struct CWispBaseWnd
	{
		CWispBase *m_pWispBase;
		unsigned int m_CtrlType;
		unsigned int m_State;
		WISP_RECT m_ScrWindowRect;
		WISP_RECT m_WindowRect;
		unsigned int m_Style;
		unsigned int m_AdvStyle;
		unsigned int m_ShowMode;
		TWideString m_WndText;
		unsigned int m_CmdID;
		CWispDC m_WindowDC;
		CWispBaseWnd *m_PrevWnd;
		CWispBaseWnd *m_NextWnd;
		CWispBaseWnd *m_ChildWnd;
		CWispBaseWnd *m_ChildTopWnd;
		CWispBaseWnd *m_ParentWnd;
		CWispBaseWnd *m_PrevModalWnd;
		CWispBaseWnd *m_OwnerWnd;
		WISP_TAB_WND_ITEM *m_pTabWndItem;
		TWideString m_HelpTipString;
		WISP_RECT m_UpdateRC;
		WISP_RECT *m_pUpdateRC;
		int m_LockUpdateRef;
		unsigned int m_IsWindow;

	public:
		CWispBaseWnd();
		virtual ~CWispBaseWnd();

		virtual bool Create(const WCHAR *Name, int x, int y, int cx, int cy, CWispBaseWnd *pParentWnd, unsigned int CmdID, unsigned int Style, unsigned int ShowMode);
		virtual bool Destroy();
		virtual void RecalcLayout();
		virtual bool InitWnd();
		virtual void SetOwner(CWispBaseWnd *pOwnerWnd);
		virtual bool SetWindowText(const WCHAR *pString);
		virtual TWideString & GetWindowText();
		virtual void Update(WISP_RECT *pRect);
		virtual bool IsRect(WISP_POINT *pPT);
		virtual void Hide();
		virtual void Show();
		virtual bool MsgProc(WISP_MSG *pMsg);

		bool Create(const WCHAR *Name, const CWispRect & pRC, CWispBaseWnd *pParentWnd, unsigned int CmdID, unsigned int Style, unsigned int ShowMode);

		bool SendCommand(unsigned int CmdMsg, int nParam1, void* pParam2);
		bool SendEvent(unsigned int CmdMsg, int nParam1, void* pParam2);
		void Close();
		void Top(bool Update);
		void Update(WISP_RECT *pRect, CWispBaseWnd *pWnd);

		void Move(int DeltaX, int DeltaY, bool bUpdate);
		void MoveToWindow(int PosX, int PosY, bool bUpdate);
		void MoveToClient(int PosX, int PosY, bool bUpdate);
		void MoveToScreen(int PosX, int PosY, bool bUpdate);
		bool Resize(int cx, int cy, bool Update);
		void Center(CWispBaseWnd *pCenterWnd);
		CWispBaseWnd *Focus();
		bool IsWindow();
		bool IsChildWndOf(CWispBaseWnd *pParentWnd);
		bool IsWindowShow();

		bool OnGetFocus(WISP_MSG *pMsg);
		bool OnLostFocus(WISP_MSG *pMsg);

		bool Show(unsigned int ShowMode);

		bool MoveWindowToWindow(const WISP_RECT & Rect, bool bUpdate);
		bool MoveWindowToWindow(int x, int y, int cx, int cy, bool bUpdate);
		bool MoveWindowToClient(const WISP_RECT & Rect, bool bUpdate);
		bool MoveWindowToClient(int x, int y, int cx, int cy, bool bUpdate);

		bool HookKeyEvent();
		bool UnhookKeyEvent();

		bool HookMouseEvent();
		bool UnhookMouseEvent();

		WISP_KEY_EVENT * RegisterKeyEvent(unsigned int CmdID, unsigned int KeyType, unsigned int SpecKey, int Disable);
		bool UnregisterKeyEvent(unsigned int CmdID);

		bool EnableHelpTip(const WCHAR *pString);
		bool DisableHelpTip();

		void *GetTimer(unsigned int TimerID);
		WISP_TIMER *InsertTimer(unsigned int TimerID, int Period, unsigned int UserData, void *UserPtr);
		bool RemoveTimer(unsigned int TimerID);

		bool EnableWindow(bool bState);

		bool OnKeyEvent(WISP_MSG *pMsg);
		bool OnBeginMove(WISP_MSG *pMsg);
		bool OnTimer(WISP_MSG *pMsg);
		bool OnMouseEnter(WISP_MSG *pMsg);
		bool OnMouseLeave(WISP_MSG *pMsg);
		bool OnEvent(WISP_MSG *pMsg);
		bool OnClose(WISP_MSG *pMsg);

		int MsgBox(WCHAR *pText, WCHAR *pCaption, unsigned int uType, CWispBaseWnd* pParentWnd);
	};

#endif

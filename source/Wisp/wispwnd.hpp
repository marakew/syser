
#ifndef _WISPWND_HPP_
#define _WISPWND_HPP_

#include "../Code/define.h"
//#include "../Code/osdefine.hpp"
#include "utility.hpp"
#include "wispbasewnd.hpp"

	struct WISP_SCROLL_BAR
	{
		int MinPos;
		int MaxPos;
		int CurPos;
		int Page;
		int AdjustPage;
		int DragPos;

		unsigned int State;
		unsigned int Style;
		unsigned int BTSpinIncState;
		unsigned int BTSpinDecState;
		WISP_RECT BarRect;
		WISP_RECT TrackRect;
		WISP_RECT SlideRect;
		WISP_RECT PageIncRect;
		WISP_RECT PageDecRect;
		int BarSize;
		int HotPos;
		bool bHide;
	};

	struct WISP_WND_CARET
	{
		int x;
		int y;
		unsigned long BlinkTime;
		CWispDIB *pDIB;
		bool bEnable;
	};


	//METRIC
	//	0 caption cy
	//	1 border size
	//	2 tip border size
	//	3 resize width
	//	4 resize height
	//	5 min-max-close-button size

	struct CWispWnd : public CWispBaseWnd
	{
		WISP_RECT m_NormalRect;
		WISP_RECT m_ScrCaptionRect;
		WISP_RECT m_ScrClientRect;
		WISP_RECT m_CaptionRect;
		WISP_RECT m_ClientRect;
		WISP_RECT *m_pBarRect;
		int m_BarCount;
		WISP_SIZE m_MinWndSize;
		WISP_SIZE m_MaxWndSize;
		bool m_bInResizeArea;
		int m_BorderSize;
		WISP_WND_CARET m_Caret;
		CWispDC m_ClientDC;
		CWispDC m_CaptionDC;
		CWispDIB *m_pTitleDIB;
		CWispDIB *m_pBKDIB;
		unsigned long m_crBGColor;
		unsigned long m_CaretColor;
		bool m_bBGColor;
		int m_SignResizeX;
		int m_SignResizeY;
		int m_SignResizeDX;
		int m_SignResizeDY;
		int m_CloseBTState;
		int m_MaxBTState;
		int m_MinBTState;
		WISP_RECT m_CloseBTRect;
		WISP_RECT m_MaxBTRect;
		WISP_RECT m_MinBTRect;
		WISP_SCROLL_BAR *m_pScrollBar[2];

	public:
		CWispWnd();
		virtual ~CWispWnd();

		//	virtual bool Create(const WCHAR *Name, int x, int y, int cx, int cy, CWispBaseWnd *pParentWnd, unsigned int CmdID, unsigned int Style, unsigned int ShowMode);

	//	virtual bool Destroy();
	//	virtual void RecalcLayout();
	//	virtual bool InitWnd();

		//	virtual void SetOwner(CWispBaseWnd *pOwnerWnd);

	//	virtual void SetWindowText(const WCHAR *pString);

		//	virtual TWideString & GetWindowText();
		//	virtual void Update(const WISP_RECT *pRect);
		//	virtual bool IsRect(WISP_POINT *pPT);
		//	virtual void Hide();
		//	virtual void Show();

		virtual bool MsgProc(WISP_MSG *pMsg) override;

		bool OnMouseMove(WISP_MSG *pMsg);
		bool OnMouseLeave(WISP_MSG *pMsg);
		bool OnKeyEvent(WISP_MSG *pMsg);
		bool OnUpdate(WISP_MSG *pMsg);
		bool OnUpdateClient(WISP_MSG *pMsg);
		bool OnUpdateCaption(WISP_MSG *pMsg);
		bool OnUpdateBorder(WISP_MSG *pMsg);
		bool IsBlinkSlowTime(unsigned long BlinkTime);
		bool OnUpdateCaret(WISP_MSG *pMsg);
		bool OnUpdateHorzScrollBar(WISP_MSG *pMsg);
		bool OnUpdateVertScrollBar(WISP_MSG *pMsg);
		bool OnScrollEvent(WISP_MSG *pMsg);
		bool OnHorzScrollMove(WISP_MSG *pMsg);
		bool OnVertScrollMove(WISP_MSG *pMsg);
		bool OnSize(WISP_MSG *pMsg);
		bool OnSizing(WISP_MSG *pMsg);
		bool OnMove(WISP_MSG *pMsg);
		bool OnMoving(WISP_MSG *pMsg);
		bool OnHitTest(WISP_MSG *pMsg);
		bool OnEvent(WISP_MSG *pMsg); //++

		bool OnCreating(WISP_MSG *pMsg);
		bool OnTimer(WISP_MSG *pMsg);
		bool OnMouseWheel(WISP_MSG *pMsg);

		bool OnCommand(WISP_MSG *pMsg);
		bool OnCmdClose(WISP_MSG *pMsg);
		bool OnCmdMax(WISP_MSG *pMsg);
		bool OnCmdMin(WISP_MSG *pMsg);

//	PG_INC     PG_DEC     SCROLL     BT_INC     BT_DEC
//V 0	80001003h, 80001004h, 80001005h, 80001006h, 80001007h	 1,   2,   3,   4,   5
//H 1	80001008h, 80001009h, 8000100Ah, 8000100Bh, 8000100Ch	11h, 12h, 13h, 14h, 15h


		//0x80001003 VP UP
		//0x80001008
		bool OnCmdPageDec(WISP_MSG *pMsg);	//P UP		DEC	2

		//0x80001004 VP DOWN
		//0x80001009 HP RIGHT
		bool OnCmdPageInc(WISP_MSG *pMsg);	//P DOWN	INC	3

		//0x80001005 VS
		//0x8000100A HS
		bool OnCmdScrollBarDrag(WISP_MSG *pMsg);

		//0x80001006 VB UP
		//0x8000100B
		bool OnCmdSpinDec(WISP_MSG *pMsg);	//V UP		DEC	1

		//0x80001007 VB DOWN
		//0x8000100C HB RIGHT
		bool OnCmdSpinInc(WISP_MSG *pMsg);	//V DOWN	INC	0


		bool CreateWnd(const WCHAR *Name, const CWispRect & pRC, CWispBaseWnd *pParentWnd, unsigned int CmdID, unsigned int Style, unsigned int ShowMode);
		virtual bool CreateWnd(const WCHAR *Name, int x, int y, int cx, int cy, CWispBaseWnd *pParentWnd, unsigned int CmdID, unsigned int Style, unsigned int ShowMode);

		virtual bool Destroy() override;
		virtual void RecalcLayout() override;
		virtual bool InitWnd() override;
		virtual bool SetWindowText(const WCHAR *Text) override;

		//+++
		void ChangeColor(unsigned int, unsigned long crBGColor);
		void SetBGColor(unsigned long crBGColor)
		{
			m_crBGColor = crBGColor;
			m_bBGColor = true;
		}
		void ClearBGColor()
		{
			m_crBGColor = 0;
			m_bBGColor = false;
		}

		void RecalcMinWndSize();

		//+++
		void ScreenToClient(WISP_POINT *pPT);
		void ClientToScreen(WISP_POINT *pPT);
		void ScreenToWindow(WISP_POINT *pPT);
		void WindowToScreen(WISP_POINT *pPT);

		//+++
		bool AttachBKDIB(CWispDIB *pDIB);
		void DetachBKDIB();
		bool LoadBKDIB(const char *FileName);

		bool AttachTitleDIB(CWispDIB *pDIB);
		void DetachTitleDIB();
		bool LoadTitleDIB(const char *FileName, int Index);

		bool CreateCaret(unsigned int ID);
		bool SetCaretPos(int PosX, int PosY);
		bool GetCaretPos(WISP_POINT *pPT);
		void ShowCaret(bool bShow);
		void BlinkCaret(unsigned long BlinkTime);
		void DestroyCaret(); //???

		void LimitScrollInfo(int Type, bool bSendEvent);
		void UpdateScrollBarState(int Type);
		void CalcScrollSlideWithPos(int Type);

		bool EnableScrollBar(int Type, bool bEnable, int Style);

		void SetScrollBarInfo(int Type, int CurPos, int Max, int Page, int Min, bool bSendEvent);
		void SetScrollBarCurPos(int Type, int CurPos, bool bSendEvent);
		void SetScrollBarRange(int Type, int MaxPos, int MinPos);
		void SetScrollBarPage(int Type, int Page);
		void AdjustScrollBarMaxPos(int Type, int MaxPos);

		void ProcessCaptionBTMouseMove(WISP_POINT & pPT);
		void ProcessCaptionBTMouseKeyUp(WISP_POINT & pPoint);
		unsigned int IsPointInCaptionBT(WISP_POINT & ScrMousePT);

		void ProcessScrollBTSpinKey(int Type, WISP_MSG *pMsg);
		void ProcessScrollMouseKey(int Type, WISP_MSG *Msg);
		void ProcessVertScrollMouseMove(WISP_MSG *pMsg);
		void ProcessHorzScrollMouseMove(WISP_MSG *pMsg);

		bool BeginDrag();
		void EndDrag();
		void CalcVertScrollSlideWithPos();
		void CalcHorzScrollSlideWithPos();

		void HorzScrollLineUp() { SendScrollEvent(1,1); } //WISP_SB_DEC
		void VertScrollLineUp() { SendScrollEvent(0,1); } //WISP_SB_DEC
		void HorzScrollLineDown() { SendScrollEvent(1,0); } //WISP_SB_INC
		void VertScrollLineDown() { SendScrollEvent(0,0); } //WISP_SB_INC

		void SendScrollEvent(int Type, int Event);
	};

#endif

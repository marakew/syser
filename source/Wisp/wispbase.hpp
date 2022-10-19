
#ifndef _WISPBASE_HPP_
#define _WISPBASE_HPP_

#include "../Code/define.h"
//#include "../Code/osdefine.hpp"
#include "../Code/alt/altqueue.hpp"
#include "../Code/alt/altlist.hpp"
#include "../Code/alt/altmap.hpp"
#include "../Code/tstrmem.hpp"
#include "../Code/polyfs.hpp"
#include "utility.hpp"
#include "wispdraw.hpp"
#include "wispdib.hpp"
#include "wispfont.hpp"
#include "wispdrawobj.hpp"
#include "wispmsg.hpp"

	//forward decl

	struct CWispBaseWnd; //TODO
	struct CWispWnd; //???
	struct CWispTipWnd; //???

	//=====

	#define WISP_SK_SHIFT	0x10000000
	#define WISP_SK_ALT	0x20000000
	#define WISP_SK_CTRL	0x40000000


	struct WISP_INPUT
	{
		unsigned int Type;

		#define WISP_INPUT_KEY		0x70000001
		#define WISP_INPUT_POS		0x70000002
		#define WISP_INPUT_MOVE		0x70000003
		#define WISP_INPUT_WHEEL	0x70000004
		#define WISP_INPUT_DBLCLICK	0x70000005
		#define WISP_INPUT_TIMER	0x70000006

		union 
		{
			struct {
				unsigned int KeyType;
				unsigned char bKeyDown;
				unsigned char ScanCode;
			} KeyEvent;

			struct {
				unsigned int PosX;
				unsigned int PosY;
			} MousePos;

			struct {

				unsigned int DeltaX;
				unsigned int DeltaY;
			} MouseMove;

			struct {
				int Delta;
			} MouseWheel;

			struct {
				unsigned int Button;
			} MouseDblClick;

			struct {
				unsigned int MilliSec;
			} Timer;
		} u;
	};

	struct WISP_TIMER_KEY
	{
		CWispBaseWnd *pWnd;
		unsigned int ID;

		bool operator<(const WISP_TIMER_KEY & other) const
		{
			if (pWnd < other.pWnd) return true;
			if (pWnd > other.pWnd) return false;
			return ID < other.ID;
		}
	};

	struct WISP_TIMER
	{
		unsigned long Elapse; //int ???
		unsigned long Period; //int ???
		unsigned int UserData;
		void *UserPtr;
	};

	struct WISP_KEY_EVENT
	{
		unsigned long KeyType;
		unsigned long SpecKey;
		unsigned long CmdID;
		unsigned long Disable;
		CWispWnd *pWnd;
	};

	enum WISP_HT_TYPE
	{
		WISP_HT_LEFT	= 0x10000,	//1<<16
		WISP_HT_RIGHT	= 0x20000,	//1<<17
		WISP_HT_TOP	= 0x40000,	//1<<18
		WISP_HT_BOTTOM	= 0x80000,	//1<<19

		WISP_HT_TOPLEFT		= WISP_HT_TOP|WISP_HT_LEFT,	//0x50000,
		WISP_HT_TOPRIGHT	= WISP_HT_TOP|WISP_HT_RIGHT,	//0x60000,
		WISP_HT_BOTTOMLEFT	= WISP_HT_BOTTOM|WISP_HT_LEFT,	//0x90000,
		WISP_HT_BOTTOMRIGHT	= WISP_HT_BOTTOM|WISP_HT_RIGHT,	//0xA0000,

		WISP_HT_MASK = 0xF << 16,
	};

	//m_Metrics[]

	enum WISP_SM
	{
		WISP_SM_CAPTION_CY = 0,
		WISP_SM_BORDER_SIZE = 1,
		WISP_SM_TIP_BORDER_SIZE = 2,
		WISP_SM_SCREEN_CX = 3,
		WISP_SM_SCREEN_CY = 4,
		WISP_SM_CAPTION_BT_SIZE = 5,
		WISP_SM_SCROLL_BAR_SIZE = 6,
		WISP_SM_TOOLBAR_CY = 7,
		WISP_SM_RESIZE_SIZE = 8,
		WISP_SM_COUNT = 9,
	};

	enum WISP_DIB_TYPE
	{
		WISP_DI_CARET = 0,
		WISP_DI_CURSOR = 1,
		WISP_DI_RESIZE = 2,
		WISP_DI_COUNT = 3,
	};

	enum 
	{
		WISP_CT_ARROW = 0,
		WISP_CT_RESIZE_VERT = 1,
		WISP_CT_RESIZE_HORZ = 2,
		WISP_CT_DRAG = 3,
		WISP_CT_RESIZE_LEFT_LEAN = 4,
		WISP_CT_RESIZE_RIGHT_LEAN = 5,
		WISP_CT_PENDING = 6,
	};

	//============

	struct CWispBase
	{
		bool m_bActive;
		bool m_bInCriticalSection;

		CWispDrawObj *m_pWispDrawObj;
		CWispDrawObj m_DefDrawObj;
		CWispDIBDrawObj m_DIBDrawObj;

		CWispDrawHAL m_DrawHAL;
		CWispDraw32Bit m_Draw32Bit;
		CWispDraw24Bit m_Draw24Bit;
		CWispDraw16Bit m_Draw16Bit;
		CWispDraw8Bit m_Draw8Bit;
		CWispDraw4Bit m_Draw4Bit;

		TWideString m_ClipString;

		bool m_KeyMap[256];
		bool m_LockMap[3];	//0 CAPSLOCK,
		static unsigned short m_VKKeyChar[2][256][2];

		WISP_POINT m_MousePT;
		WISP_POINT m_PrevMousePT;
		WISP_POINT m_MouseCursorPos;
		WISP_SIZE m_MouseCursorSize;
		WISP_RECT m_MouseRectUpd;
		WISP_HIT_AREA m_HitArea;

		bool m_bUpdate;
		int m_UpdateLockRef;
		unsigned int m_CursorType;
		unsigned int m_CursorTypeBak;
		CWispDIB *m_pCursorDIB;
		CWispDIB *m_pCursorDIBBak;
		int m_PumpLockRef;

		CWispBaseWnd *m_pRootWnd;

		CWispWnd *m_pTopMostRootWnd;
		CWispTipWnd *m_pTipWnd;
		char m_szModuleName[MAX_PATH];
		const char *m_szWispResource;
		CWispBaseWnd *m_pMoveWnd;
		CWispBaseWnd *m_pDragWnd;
		WISP_POINT m_BeginPT;
		CPFSFileIO m_PFSFileIO;
		CWispFont m_FontList[WISP_FONT_COUNT];
		static WISP_PHYS_FONT_INFO m_PhysFontInfo[WISP_FONT_COUNT];

		CWispBaseWnd *m_pMouseEventWnd;
		CWispBaseWnd *m_pFocusWnd;
		CWispBaseWnd *m_pModalWnd;
		CWispDIB m_DefDIB[WISP_DI_COUNT];
		CWispDIBLib m_DIBLib;
		int m_Metrics[WISP_SM_COUNT];

		WISP_FRAME_BUFFER_INFO m_FrameBufferInfo;

		TQueue<WISP_INPUT,128> m_InputQueue; //TStack
		TQueue<WISP_MSG,128> m_MsgQueue; //TStack
		map<WISP_TIMER_KEY, WISP_TIMER> m_TimerList;
		TList<WISP_KEY_EVENT> m_KeyEventList;
		bool m_bNeedUpdate;

		unsigned long m_PalColors[256];
		static unsigned long m_DefColorPal[256];

		unsigned int m_RepKeyIndex;
		int m_RepKeyElapse;
		int m_RepKeyPeriod;
		unsigned int m_SpecKey;
		TList<CWispBaseWnd*> m_KeyHookWndList;
		TList<CWispBaseWnd*> m_MouseHookWndList;

		bool m_KeySimulateMouse;
		bool m_bHideMouse;
		int m_KeySimDelta;

		int m_BaseWndCount;
		int m_WndCount;
		int m_bKeyMutex;
		int m_TimerLockRef;
		int m_FontIndex;
		int m_KeyLangMap;

	public:
		CWispBase();
		virtual ~CWispBase();

		virtual bool Init();
		virtual void Release();

		virtual bool CreateFrame(const WCHAR *WndName, int Width, int Height, int Bpp) = 0;
		virtual void DestroyFrame() = 0;

		virtual bool EndResize(int Width, int Height) { return false; }
		virtual bool BeginResize(int Width, int Height) { return false; }
		virtual bool MovingFrame(int PosX, int PosY) { return false; }

		virtual void BeginDrag() { }
		virtual void EndDrag() { }

		void RedrawNextTime();
		virtual bool WakeUpMsgThread() { return true; }
		virtual bool SleepMsgThread() { return true; }

		virtual void EnterMsgCriticalSection() { }
		virtual void LeaveMsgCriticalSection() { }

		virtual void BeginDraw(WISP_RECT *pRect) { }
		virtual void EndDraw(WISP_RECT *pRect) { }

		virtual void SetCursor(unsigned int CursorType, WISP_POINT *pHotPT, WISP_SIZE *pSize, CWispDIB *pDIB);
		virtual void ChangeCursor(LONG_PTR Style, unsigned int CursorType, WISP_POINT *pHotPT, WISP_SIZE *pSize) { }
		void ChangeCursor(unsigned int CursorType);
		void ChangeCursor(CWispDIB *pCursorDIB);

		virtual void Update();
		virtual void UpdateCursor();

		virtual bool InsertTimer(const WISP_TIMER &) { return true; }
		virtual bool RemoveTimer(const WISP_TIMER &) { return true; }

		virtual unsigned long GetPalColor(int Index);
		virtual bool SetPalColor(int Index, unsigned long Color);
		virtual void LoadPalette();

		void RemoveWnd(CWispBaseWnd *pWnd);
		void RemoveWndMsg(CWispBaseWnd *pWnd);
		void InsertWnd(CWispBaseWnd *pParentWnd, CWispBaseWnd *pNewWnd);
		CWispBaseWnd *WindowFromPoint(WISP_POINT *pPoint, CWispBaseWnd *pParentWnd);

		bool MergeMessage(WISP_MSG *pNewMsg, WISP_MSG *pMergeMsg);
		//GetMessage
		bool SendMessage(WISP_MSG *pMsg);
		bool PostMessage(WISP_MSG *pMsg);
		bool DispatchMessage(WISP_MSG *pMsg);

		void DispatchMsgQueue();
		void DispatchInputQueue();

		void LockUpdate();
		void UnlockUpdate();

		virtual void Pump();
		void PumpQueue();
		virtual void BlockPump(CWispBaseWnd *pWnd);
		virtual bool PumpCondition();

		virtual const WCHAR *GetClipboard();
		virtual bool SetClipboard(const WCHAR *Text);

		virtual void GetFrameBuffer(WISP_FRAME_BUFFER_INFO *pFrameBuffer);

		void CaptureMouseEvent(CWispBaseWnd *pWnd);

		void BeginMove(CWispBaseWnd *pWnd);
		void EndMove();

		void ResetInput();
		void StopMouseKeyRep();
		void StartMouseKeyRep(unsigned int RepKeyIndex);

		bool KeySimulateMouse(unsigned int KeyType, bool bKeyDown);
		unsigned short IsCharKey(unsigned int KeyType);

		virtual void OnMousePosChg(int PosX, int PosY);
		virtual void SendMousePosChg(int PosX, int PosY);

		virtual void OnMouseMove(int DeltaX, int DeltaY);
		virtual void SendMouseMove(int DeltaX, int DeltaY);

		virtual void OnMouseWheel(int Delta);
		virtual void SendMouseWheel(int Delta);

		virtual void OnMouseDblClick(unsigned int Button);
		virtual void SendMouseDblClick(unsigned int Button);

		bool OnProcessKeyEvent(unsigned int KeyType, bool bKeyDown, int ScanCode);
		bool OnProcessMouseKeyEvent(unsigned int KeyType, bool bKeyDown, int ScanCode);

		virtual void OnKeyEvent(unsigned int KeyType, bool bKeyDown, int ScanCode);
		virtual void SendKeyEvent(unsigned int KeyType, bool bKeyDown, int ScanCode);

		void LockTimer();
		void UnlockTimer();

		virtual void OnTimer(int MilliSec);
		virtual void SendTimer(int MilliSec);

		void AdjustColor();
		virtual void OnFrameBufferBppChg(int Bpp);

		virtual void OnCreateMousePos(int PosX, int PosY);

		virtual bool GetOpenFolderName(WCHAR *Path, const WCHAR *Title) { return false; }
		virtual bool GetOpenFileName(const WCHAR *InitialDir, const WCHAR *Filter, int, WCHAR *File, int MaxFile) { return false; }

		virtual void ReportMsg(const WCHAR *Format, ...) { }

		virtual int GetMetrics(unsigned int Type);

		bool SelectDrawObject(CWispDrawObj *pDrawObj);
		CWispDIB *GetDefDIB(unsigned int ID);

		void *GetScreenBuffer(int x, int y);
		bool CopyToScreen(WISP_FRAME_BUFFER_INFO *pRealScreen, WISP_POINT *pPT, WISP_RECT *pRect);
		bool CopyScreenToFile(const char *FileName);

		int MsgBox(const WCHAR *pText, const WCHAR *pCaption, unsigned int uType, CWispBaseWnd *pParentWnd);
	};

	extern CWispBase *gpCurWisp;
#endif

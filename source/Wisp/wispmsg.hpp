
#ifndef _WISPMSG_HPP_
#define _WISPMSG_HPP_

#include "../Code/define.h"
#include "utility.hpp"

	enum WISP_CTRL_TYPE : unsigned int
	{
		WISP_CTRL_NULL   = 0,
		WISP_CTRL_SYS_TYPE  = 1,

		WISP_CTRL_BASE_WND_BEGIN  = 2,
		WISP_CTRL_STATIC_STRING  = 3,	//
		WISP_CTRL_STATIC_DIB  = 4,	//
		WISP_CTRL_STATIC_GROUP  = 5,	//
		WISP_CTRL_STATIC_URL  = 6,	//
		WISP_CTRL_BUTTON  = 7,		//
		WISP_CTRL_BASE_WND  = 8,	//

		WISP_CTRL_WND_BEGIN  = 9,
		WISP_CTRL_EDIT   = 10,		//
		WISP_CTRL_LIST   = 11,		//
		WISP_CTRL_CHECK_BOX  = 12,	//
		WISP_CTRL_TAB    = 13,		//???
		WISP_CTRL_MENU   = 14,		//
		WISP_CTRL_RADIO_BOX  = 15,	//
		WISP_CTRL_MSG_BOX  = 16,	//
		WISP_CTRL_PROGRESS  = 17,	//
		WISP_CTRL_SPLIT_WND  = 18,	//
		WISP_CTRL_TOOL_BAR  = 19,	//
		WISP_CTRL_FORM   = 20,		//
		WISP_CTRL_HEX_WND  = 21,	//
		WISP_CTRL_COMBO_BOX  = 22,	//
		WISP_CTRL_TERMINAL_WND = 23,	//++
		WISP_CTRL_CONSOLE_WND = 24,	//++
		WISP_CTRL_TIP_WND  = 25,

		WISP_CTRL_WND    = 27,
	};

	struct CWispBaseWnd;

	enum WISP_MOUSE_EVENT
	{
		WISP_MOUSE_LEAVE = 0,
		WISP_MOUSE_ENTER = 1,
		WISP_MOUSE_MOVE = 2,
		WISP_MOUSE_KEY = 3,
		WISP_MOUSE_EVENT_COUNT = 4,
	};

	enum 
	{
		WISP_SCROLL_BAR_VERT = 0,
		WISP_SCROLL_BAR_HORZ = 1,
		WISP_SCROLL_BAR_COUNT = 2,
	};

	enum WISP_SB_TYPE
	{
		WISP_SB_INC = 0, //DOWN
		WISP_SB_DEC = 1, //UP
		WISP_SB_PAGE_UP = 2,
		WISP_SB_PAGE_DOWN = 3,
		WISP_SB_POS_CHANGE = 4,
		WISP_SB_MIN = 5,
		WISP_SB_MAX = 6,
	};

 //WISP_HT_*	1-ON_SCRCLIENT,2-ON_CAP,3-ON_RCCLIENT,4-ON_CLOSE,5-ON_MAX,6-ON_MIN,

	struct WISP_HIT_AREA
	{
		unsigned int Type;
		unsigned int State;
	};

	struct WISP_MSG
	{
		CWispBaseWnd *hWnd;
		CWispBaseWnd *hParentWnd;
		unsigned int Msg;
		bool bMsgLBTDown;
		bool bMsgRBTDown;
		WISP_POINT MsgMouseScrPT;
		WISP_POINT MsgMouseWndPT;
		WISP_POINT m_LogicMousePosWnd;
		union
		{
			struct
			{
				unsigned int ID;
				unsigned int UserData;
				void* UserPtr;
			} Timer;

			//WISP_WM_DRAG_BEGIN ?
			//WISP_WM_DRAG_END ?

			//WISP_WM_DRAG_MOVE
			struct
			{
				WISP_POINT BeginMouseWndPT;
				unsigned int UserData;
				void* UserPtr;
				int DeltaX;
				int DeltaY;
			} DragEvent;

			//WISP_WM_KEY_EVENT
			//WISP_WM_MOUSE_DBLCLICK

			struct
			{
				WISP_HIT_AREA HitArea;
				unsigned int KeyType; //KeyCode
				bool bKeyDown;
				unsigned int ScanCode;
			} KeyEvent;

			//WISP_WM_MOUSE_WHEEL
			//WISP_WM_MOUSE_MOVE
			//WISP_WM_MOUSE_LEAVE
			//WISP_WM_VERT_SCROLL_MOVE
			//WISP_WM_HORZ_SCROLL_MOVE

			struct
			{
				WISP_HIT_AREA HitArea;
				void* hWndMouseAt;
				int DeltaX;
				int DeltaY;
				int Wheel;
			} MouseEvent;

			//WISP_WM_EVENT
			//WISP_WM_COMMAND
			struct
			{
				unsigned int CtrlType; //WISP_CTRL_TYPE
				unsigned int CmdMsg;
				unsigned int CmdID;
				int Param1;
				void* Param2;
			} Command;

			//WISP_WM_MOVING
			//WISP_WM_MOVE
			struct
			{
				int DeltaX;
				int DeltaY;
			} Move;

			//WISP_WM_CHAR
			struct
			{
				int nRepeat;
				unsigned short Char;
			} Char;

			//WISP_WM_SCROLL_EVENT
			struct
			{
				unsigned int Event; //WISP_SB_* INC,DEC,UP,DOWN
				unsigned int Type; //WISP_SCROLL_BAR_* 0,1,
				unsigned int CurPos;
				long Delta;
			} ScrollEvent;

			struct
			{
				WISP_RECT Rect;
			} Update;

			struct
			{
				WISP_HIT_AREA HitArea;
				bool bKeyDown;
			} HitTest;

			//WISP_WM_SIZING
			//WISP_WM_SIZE ???
			//WISP_WM_RESIZE_CHILD ???
			struct
			{
				WISP_SIZE Size;
				WISP_SIZE ScrSize;
			} Sizing;

			//WISP_SIZE Size;

			CWispBaseWnd *hOtherWnd;
			unsigned int EventCmdID;

			bool bExit;
			unsigned int Code;

			struct
			{
				unsigned int field_0;
				unsigned int field_4;
				unsigned int field_8;
				unsigned int field_C;
				unsigned int field_10;
				unsigned int field_14;
			};
		};// u;
	};

	enum : unsigned int
	{
		WISP_WM_CREATING  = 0x80000010,
		WISP_WM_CREATE   = 0x80000011,
		WISP_WM_CREATE_FORM  = 0x80000012,
		WISP_WM_DESTROY  = 0x80000013,
		WISP_WM_CLOSE    = 0x80000014,

		WISP_WM_KEY_EVENT  = 0x80000020,

		WISP_WM_MOUSE_ENTER  = 0x80000030,
		WISP_WM_MOUSE_MOVE  = 0x80000031,
		WISP_WM_MOUSE_LEAVE  = 0x80000032,
		WISP_WM_MOUSE_WHEEL  = 0x80000033,
		WISP_WM_MOUSE_DBLCLICK  = 0x80000034,

		WISP_WM_COMMAND  = 0x80000040,
		WISP_WM_EVENT    = 0x80000041,

		WISP_WM_UPDATE   = 0x80000100,
		WISP_WM_UPDATE_BORDER  = 0x80000101,
		WISP_WM_UPDATE_CAPTION  = 0x80000102,
		WISP_WM_UPDATE_CLIENT  = 0x80000103,
		WISP_WM_UPDATE_CARET  = 0x80000104,
		WISP_WM_UPDATE_HORZ_SCROLL_BAR  = 0x80000105,
		WISP_WM_UPDATE_VERT_SCROLL_BAR  = 0x80000106,

		WISP_WM_BEGIN_MOVE  = 0x80000120,
		WISP_WM_END_MOVE  = 0x80000121,
		WISP_WM_MOVE     = 0x80000122,
		WISP_WM_MOVING   = 0x80000123,
		WISP_WM_SIZE     = 0x80000124,
		WISP_WM_SIZING   = 0x80000125,
		WISP_WM_RECALC_LAYOUT  = 0x80000126,
		WISP_WM_RESIZE_CHILD  = 0x80000127,

		WISP_WM_CHAR     = 0x80000140,

		WISP_WM_SCROLL_EVENT  = 0x80000150,

		WISP_WM_VERT_SCROLL_MOVE  = 0x80000152,
		WISP_WM_HORZ_SCROLL_MOVE  = 0x80000153,

		WISP_WM_TIMER_EVENT  = 0x80000161,

		WISP_WM_GET_FOCUS  = 0x80000170,
		WISP_WM_LOST_FOCUS  = 0x80000171,

		WISP_WM_HIT_TEST  = 0x80000180,

		WISP_WM_DRAG_BEGIN  = 0x80000190,
		WISP_WM_DRAG_MOVE  = 0x80000191,
		WISP_WM_DRAG_END  = 0x80000192,

	//	
		WISP_ID_TOP_MOST_ROOT = 0x80000000,

	//	WISP_ID_STATIC_STR = 0x80000001,
		WISP_ID_STATIC_DIB = 0x80000002,
	//	WISP_ID_STATIC_GROUP = 0x80000003,
		WISP_ID_MAIN_FRAME = 0x80000004,
		WISP_ID_MAIN_MENU = 0x80000005,

		WISP_ID_MAIN_TOOLBAR = 0x80000007,
		WISP_ID_SOFT_KB = 0x80000008,
		WISP_ID_HELP_TIP = 0x80000009,
	//	WISP_ID_SUB_MENU = 0x8000000A,
		WISP_ID_CARET = 0x8000000B,


		WISP_ID_KEYDOWN		= 0x80000001,
		WISP_ID_KEYUP		= 0x80000002,

		WISP_ID_CMD		= 0x80000010,

		WISP_ID_EXPANDING	= 0x80000100,
		WISP_ID_EXPAND		= 0x80000101,

		WISP_ID_COLLAPSING	= 0x80000102,
		WISP_ID_COLLAPSE	= 0x80000103,

		WISP_ID_SELECTING	= 0x80000104,
		WISP_ID_SELECT		= 0x80000105,

		WISP_ID_UNSELECTING	= 0x80000106,
		WISP_ID_UNSELECT	= 0x80000107,

		WISP_ID_CHECKING	= 0x80000108,
		WISP_ID_CHECKED		= 0x80000109,

		WISP_ID_UNCHECKING	= 0x8000010A,
		WISP_ID_UNCHECKED	= 0x8000010B,

		//0x8000010C ?
		//0x8000010D ?

		WISP_ID_ITEM_EDIT_UPDATE	= 0x8000010E,

		WISP_ID_ENABLE		= 0x8000010F,
		WISP_ID_DISABLE		= 0x80000110,

		WISP_ID_ITEM_EDITITING	= 0x80000111,
		WISP_ID_ITEM_EDIT_LBUTTON	= 0x80000112,
		WISP_ID_ITEM_EDIT_RBUTTON	= 0x80000113,

		WISP_ID_ENTER		= 0x80000114,
		WISP_ID_LEAVE		= 0x80000115,

		//0x80000116 WISP_ID_?

		WISP_ID_RBUTTON_DBLCLICK	= 0x80000117,
		WISP_ID_LBUTTON_DBLCLICK	= 0x80000118,

		WISP_ID_FORM_KEYDOWN	= 0x80000119,
		WISP_ID_FORM_KEYUP	= 0x80000120,

		WISP_ID_LBUTTON_LAYOUT	= 0x80000121,
		WISP_ID_RBUTTON_LAYOUT	= 0x80000122,

		WISP_ID_CHILD_GET_FOCUS		= 0x80000201,
		WISP_ID_CHILD_LOST_FOCUS	= 0x80000202,
		WISP_ID_LBUTTON		= 0x80000203,
		WISP_ID_RBUTTON		= 0x80000204,

		WISP_ID_BOX_SELECT	= 0x80000300,
		WISP_ID_COMBO_SELECT	= 0x80000400,
		WISP_ID_OPEN_URL	= 0x80000500,

		WISP_ID_EDIT_CHANGING	= 0x80000700,
		WISP_ID_EDIT_LOST_FOCUS	= 0x80000701,
		WISP_ID_EDIT_GET_FOCUS	= 0x80000702,
		WISP_ID_EDIT_KEYDOWN	= 0x80000703,
		WISP_ID_EDIT_KEYUP	= 0x80000704,
		WISP_ID_EDIT_CHANGED	= 0x80000705,

		WISP_ID_TAB_SELECT	= 0x80000800,


		WISP_ID_CLOSE    = 0x80001000,
		WISP_ID_MAX      = 0x80001001,
		WISP_ID_MIN      = 0x80001002,

		WISP_ID_VPAGE_DEC  = 0x80001003,
		WISP_ID_VPAGE_INC  = 0x80001004,
		WISP_ID_VSPIN_SCROLL  = 0x80001005,
		WISP_ID_VSPIN_DEC  = 0x80001006,
		WISP_ID_VSPIN_INC  = 0x80001007,

		WISP_ID_HPAGE_DEC  = 0x80001008,
		WISP_ID_HPAGE_INC  = 0x80001009,
		WISP_ID_HSPIN_SCROLL  = 0x8000100A,
		WISP_ID_HSPIN_DEC  = 0x8000100B,
		WISP_ID_HSPIN_INC  = 0x8000100C,

		WISP_ID_FORM_CLOSE_ID_START = 0x80002000,
		WISP_ID_OK       = 0x80002001,
		WISP_ID_CANCEL   = 0x80002002,
		WISP_ID_YES      = 0x80002003,
		WISP_ID_NO       = 0x80002004,
		WISP_ID_RETRY    = 0x80002005,
		WISP_ID_CONTINUE = 0x80002006,
		WISP_ID_IGNORE   = 0x80002007,
		WISP_ID_ABORT    = 0x80002008,
		WISP_ID_TRY      = 0x80002009,
		WISP_ID_FORM_CLOSE_ID_END  = 0x8000200A,

		WISP_ID_COPY     = 0x8000200B,
		WISP_ID_CUT      = 0x8000200C,
		WISP_ID_PASTE    = 0x8000200D,
	};
#if 0
	WISP_MSG_MAP_BEGIN(CSystemExplorer)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP(WISP_WM_GET_FOCUS, OnGetFocus)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispWnd)
	
	WISP_CMD_MAP_BEGIN(CSystemExplorer)
		WISP_CMD_MAP(
	WISP_CMD_MAP_END

	WISP_EVENT_MAP_BEGIN(CSystemExplorer)
		WISP_EVENT_MAP(
	WISP_EVENT_MAP_END
#endif

//MSG
#define WISP_MSG_MAP_BEGIN(x)	bool x::MsgProc(WISP_MSG *pMsg) \
				{ \
					switch (pMsg->Msg) \
					{ \
					case 0: \
				        	return false;

#define WISP_MSG_MAP(i,f)		case i: \
						if (!f(pMsg)) return false; \
						break;

#define WISP_MSG_MAP_ON_COMMAND	WISP_MSG_MAP(WISP_WM_COMMAND, OnCommand)
#define WISP_MSG_MAP_ON_EVENT	WISP_MSG_MAP(WISP_WM_EVENT, OnEvent)

#define WISP_MSG_MAP_END_	\
					} \
					return true; \
				}

#define WISP_MSG_MAP_END(x)	\
					} \
					return x::MsgProc(pMsg); \
				}
//CMD
#define WISP_CMD_MAP_BEGIN(x)	bool x::OnCommand(WISP_MSG *pMsg) \
				{ \
					switch (pMsg->Command.CmdID) \
					{ \
					case 0: \
				        	return true;

#define WISP_CMD_MAP(i,f)		case i: \
						if (!f(pMsg)) return false; \
						break;

#define WISP_CMD_MAP_DEFAULT(f)		default: \
						if (!f(pMsg)) return false; \
						break;

#define WISP_CMD_MAP_END	\
					} \
					return true; \
				}

//EVENT
#define WISP_EVENT_MAP_BEGIN(x)	bool x::OnEvent(WISP_MSG *pMsg) \
				{ \
					switch (pMsg->Command.CmdID) \
					{ \
					case 0: \
				        	return false;

#define WISP_EVENT_MAP(i,f)		case i: \
						if (!f(pMsg)) return false; \
						break;

#define WISP_EVENT_MAP_DEFAULT(f)	default: \
						if (!f(pMsg)) return false; \
						break;

#define WISP_EVENT_MAP_END	\
					} \
					return true; \
				}

#endif
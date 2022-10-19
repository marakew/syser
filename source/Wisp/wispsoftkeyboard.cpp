
#include "wispsoftkeyboard.hpp"
#include "wispbase.hpp"
#include "wispcheckbox.hpp"

	WISP_SKB_KEY CWispSoftKeyboard::m_SoftKeyTable[] = {
			{ WSTR("ESC"), 0x1B, {2, 2, 22, 20}},
			{ WSTR("F1"), 0x70, {46, 2, 22, 20}},
			{ WSTR("F2"), 0x71, {70, 2, 22, 20}},
			{ WSTR("F3"), 0x72, {94, 2, 22, 20}},
			{ WSTR("F4"), 0x73, {118, 2, 22, 20}},
			{ WSTR("F5"), 0x74, {158, 2, 22, 20}},
			{ WSTR("F6"), 0x75, {182, 2, 22, 20}},
			{ WSTR("F7"), 0x76, {206, 2, 22, 20}},
			{ WSTR("F8"), 0x77, {230, 2, 22, 20}},
			{ WSTR("F9"), 0x78, {266, 2, 22, 20}},
			{ WSTR("F10"), 0x79, {290, 2, 22, 20}},
			{ WSTR("F11"), 0x7A, {314, 2, 22, 20}},
			{ WSTR("F12"), 0x7B, {338, 2, 22, 20}},
			{ WSTR("`"), 0xC0, {2, 28, 22, 20}},
			{ WSTR("1"), 0x31, {26, 28, 22, 20}},
			{ WSTR("2"), 0x32, {50, 28, 22, 20}},
			{ WSTR("3"), 0x33, {74, 28, 22, 20}},
			{ WSTR("4"), 0x34, {98, 28, 22, 20}},
			{ WSTR("5"), 0x35, {122, 28, 22, 20}},
			{ WSTR("6"), 0x36, {146, 28, 22, 20}},
			{ WSTR("7"), 0x37, {170, 28, 22, 20}},
			{ WSTR("8"), 0x38, {194, 28, 22, 20}},
			{ WSTR("9"), 0x39, {218, 28, 22, 20}},
			{ WSTR("0"), 0x30, {242, 28, 22, 20}},
			{ WSTR("-"), 0x6D, {266, 28, 22, 20}},
			{ WSTR("="), 0xBB, {290, 28, 22, 20}},
			{ WSTR("\\"), 0x6C, {314, 28, 22, 20}},
			{ WSTR("<-"), 8, {338, 28, 22, 20}},
			{ WSTR("TAB"), 9, {2, 50, 32, 20}},
			{ WSTR("Q"), 0x51, {36, 50, 22, 20}},
			{ WSTR("W"), 0x57, {60, 50, 22, 20}},
			{ WSTR("E"), 0x45, {84, 50, 22, 20}},
			{ WSTR("R"), 0x52, {108, 50, 22, 20}},
			{ WSTR("T"), 0x54, {132, 50, 22, 20}},
			{ WSTR("Y"), 0x59, {156, 50, 22, 20}},
			{ WSTR("U"), 0x55, {180, 50, 22, 20}},
			{ WSTR("I"), 0x49, {204, 50, 22, 20}},
			{ WSTR("O"), 0x4F, {228, 50, 22, 20}},
			{ WSTR("P"), 0x50, {252, 50, 22, 20}},
			{ WSTR("["), 0xDB, {276, 50, 22, 20}},
			{ WSTR("]"), 0xDD, {300, 50, 22, 20}},
			{ WSTR("Caps"), 0, {2, 72, 40, 20}},
			{ WSTR("A"), 0x41, {44, 72, 22, 20}},
			{ WSTR("S"), 0x53, {68, 72, 22, 20}},
			{ WSTR("D"), 0x44, {92, 72, 22, 20}},
			{ WSTR("F"), 0x46, {116, 72, 22, 20}},
			{ WSTR("G"), 0x47, {140, 72, 22, 20}},
			{ WSTR("H"), 0x48, {164, 72, 22, 20}},
			{ WSTR("J"), 0x4A, {188, 72, 22, 20}},
			{ WSTR("K"), 0x4B, {212, 72, 22, 20}},
			{ WSTR("L"), 0x4C, {236, 72, 22, 20}},
			{ WSTR(";"), 0xBA, {260, 72, 22, 20}},
			{ WSTR("'"), 0xDE, {284, 72, 22, 20}},
			{ WSTR("Enter"), 0x0D, {308, 72, 52, 20}},
			{ WSTR("Shift"), 0xA0, {2, 94, 50, 20}},
			{ WSTR("Z"), 0x5A, {54, 94, 22, 20}},
			{ WSTR("X"), 0x58, {78, 94, 22, 20}},
			{ WSTR("C"), 0x43, {102, 94, 22, 20}},
			{ WSTR("V"), 0x56, {126, 94, 22, 20}},
			{ WSTR("B"), 0x42, {150, 94, 22, 20}},
			{ WSTR("N"), 0x4E, {174, 94, 22, 20}},
			{ WSTR("M"), 0x4D, {198, 94, 22, 20}},
			{ WSTR(","), 0xBC, {222, 94, 22, 20}},
			{ WSTR("."), 0xBE, {246, 94, 22, 20}},
			{ WSTR("/"), 0x6F, {270, 94, 22, 20}},
			{ WSTR("Shift"), 0xA1, {294, 94, 66, 20}},
			{ WSTR("Ctrl"), 0xA2, {2, 116, 32, 20}},
			{ WSTR("Win"), 0x5B, {36, 116, 32, 20}},
			{ WSTR("Alt"), 0xA4, {70, 116, 32, 20}},
			{ WSTR("Space"), 0x20, {104, 116, 120, 20}},
			{ WSTR("Alt"), 0xA5, {226, 116, 32, 20}},
			{ WSTR("Win"), 0x5C, {260, 116, 32, 20}},
			{ WSTR("Apps"), 0x5D, {294, 116, 32, 20}},
			{ WSTR("Ctrl"), 0xA3, {328, 116, 32, 20}},
			{ WSTR("Ins"), 0x2D, {366, 50, 24, 20}},
			{ WSTR("Hom"), 0x24, {392, 50, 24, 20}},
			{ WSTR("PUp"), 0x21, {418, 50, 24, 20}},
			{ WSTR("Del"), 0x2E, {366, 72, 24, 20}},
			{ WSTR("End"), 0x23, {392, 72, 24, 20}},
			{ WSTR("PDo"), 0x22, {418, 72, 24, 20}},
			{ WSTR("Up"), 0x25, {392, 94, 24, 20}},
			{ WSTR("Le"), 0x25, {366, 116, 24, 20}},
			{ WSTR("Do"), 0x28, {392, 116, 24, 20}},
			{ WSTR("Ri"), 0x27, {418, 116, 24, 20}},
		};

	CWispSoftKeyboard::CWispSoftKeyboard()
	{
	}
	CWispSoftKeyboard::~CWispSoftKeyboard()
	{
	}

	bool CWispSoftKeyboard::CreateEx(int x, int y, CWispWnd *pParentWnd, unsigned int Style, unsigned int ShowMode, unsigned int CmdID)
	{
		WISP_RECT rc;
		rc.x = x;
		rc.y = y;
		rc.cx = 453;
		rc.cy = 166;
		return CreateWnd(WSTR("Soft Keyboard"), rc, pParentWnd, CmdID, Style, ShowMode);
	}

	WISP_MSG_MAP_BEGIN(CWispSoftKeyboard)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP(WISP_WM_GET_FOCUS, OnGetFocus)
	WISP_MSG_MAP_END(CWispWnd)

	bool CWispSoftKeyboard::OnCreate(WISP_MSG *pMsg)
	{
		#define BTN_COUNT lenof(m_SoftKeyTable)

		LoadTitleDIB("\\WispApp\\SoftKeyboard\\SoftKeyboard.ico", 0);

		CWispDIB *pDIB = gpCurWisp->m_DIBLib.GetDIB("\\WispApp\\SoftKeyboard\\SoftKeyboard.ico", 0);
		m_StaticDIB.CreateEx(pDIB, 387, 8, this, WISP_ID_STATIC_DIB);
		m_BTAry = new CWispBaseWnd*[BTN_COUNT];
		
		for (int n = 0; n < BTN_COUNT; ++n)
		{
			if (m_SoftKeyTable[n].VKCmdID == 0xA1 || //Shift
			    m_SoftKeyTable[n].VKCmdID == 0xA0) //Shift
			{
				CWispCheckBox *pButton = new CWispCheckBox;
				pButton->Create(m_SoftKeyTable[n].szKeyName, m_SoftKeyTable[n].BTRect, this, m_SoftKeyTable[n].VKCmdID, 0x40000000, 1);
				m_BTAry[n] = (CWispBaseWnd*)pButton;
			} else

			{
				CWispButton *pButton = new CWispButton;
				pButton->Create(m_SoftKeyTable[n].szKeyName, m_SoftKeyTable[n].BTRect, this, m_SoftKeyTable[n].VKCmdID, 0, 1);
				m_BTAry[n] = (CWispBaseWnd*)pButton;
			}
		}
		return true;
	}
	bool CWispSoftKeyboard::OnDestroy(WISP_MSG *pMsg)
	{
		for (int n = 0; n < BTN_COUNT; ++n)
		{
			m_BTAry[n]->Destroy();
			delete m_BTAry[n];
		}
		delete []m_BTAry;
		return true;
	}
	bool CWispSoftKeyboard::OnKeyEvent(WISP_MSG *pMsg)
	{
		//
		return true;
	}
	bool CWispSoftKeyboard::OnGetFocus(WISP_MSG *pMsg)
	{
		Top(true);
		return false;
	}
	bool CWispSoftKeyboard::OnCommand(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000201) //WISP_ID_CHILD_GET_FOCUS
			return false;

		if (pMsg->Command.CmdID < 256)
			m_pWispBase->OnKeyEvent(pMsg->Command.CmdID, pMsg->Command.CmdMsg == 0x80000001, 0); //WISP_ID_KEYDOWN

		return true;
	}
	void CWispSoftKeyboard::Create()
	{
		if (IsWindow())
		{
			Show(WISP_SH_MINSIZE);
		} else
		{
			CreateEx(0,0, 0, 0x2B, WISP_SH_MINSIZE, WISP_ID_SOFT_KB); //0x3B
			Center(0);
		}
	}

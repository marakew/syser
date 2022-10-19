#include "../Code/define.h"
//#include "../Code/osdefine.hpp"
#include "../Code/sysdep.hpp"
#include "wispbase.hpp"
#include "wispbasewnd.hpp"
#include "wispwnd.hpp"
#include "wispmsgbox.hpp"


	extern unsigned long BTCaret[24]; //dibdata.cpp

	CWispBase *gpCurWisp;

	unsigned short CWispBase::m_VKKeyChar[2][256][2] = 
	{
		{
		{0,0},    {0,0},   {0,0},   {0,0},   {0,0},       {0,0},    {0,0},    {0,0},    {8,8},   {9,9},
		{0,0},    {0,0},   {0,0},  {0xA,0xA}, {0,0},      {0,0},   {0,0},    {0,0},    {0,0},     {0,0}, 
		{0,0},    {0,0},   {0,0},   {0,0},   {0,0},       {0,0},    {0,0},   {0,0},    {0,0},    {0,0},
		{0,0},    {0,0},  {' ',' '},  {0,0},  {0,0},      {0,0},    {0,0},    {0,0},   {0,0},   {0,0},
		{0,0},    {0,0},   {0,0},   {0,0},    {0,0},     {0,0},    {0,0},    {0,0},   {')','0'},  {'!','1'},
		{'@','2'},  {'#','3'},  {'$','4'},  {'%','5'},  {'^','6'}, {'&','7'},  {'*','8'},  {'(','9'},  {0,0}, {0,0}, 
		{0,0},   {0,0},   {0,0}, {0,0},  {0,0}, {'A','a'}, {'B','b'}, {'C','c'}, {'D','d'}, {'E','e'},
		{'F','f'}, {'G','g'}, {'H','h'}, {'I','i'}, {'J','j'}, {'K','k'}, {'L','l'}, {'M','m'}, {'N','n'}, {'O','o'},
		{'P','p'}, {'Q','q'}, {'R','r'}, {'S','s'}, {'T','t'}, {'U','u'}, {'V','v'}, {'W','w'}, {'X','x'}, {'Y','y'},
		{'Z','z'}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {'0','0'}, {'1','1'}, {'2','2'}, {'3','3'},
		{'4','4'}, {'5','5'}, {'6','6'}, {'7','7'}, {'8','8'}, {'9','9'}, {'*','*'}, {'+','+'}, {'\\','\\'}, {'-','-'},
		{'.','.'}, {'/','/'}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {':',';'}, {'+','='}, {'<',','}, {'_','-'},
		{'>','.'}, {'?','/'}, {'~','`'}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {'{','['},
		{'|','\\'}, {'}',']'}, {'"',0x27}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		},

		{
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {8,8}, {9,9},
		{0,0}, {0,0}, {0,0}, {0xA,0xA}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {' ',' '}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {'0',0xFFE0}, {'1','&'},
		{'2',0xFFE9}, {'3','"'}, {'4',0x27}, {'5','('}, {'6','-'}, {'7',0xFFE0}, {'8','_'}, {'9',0xFFE7}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {'A','a'}, {'B','b'}, {'C','c'}, {'D','d'}, {'E','e'},
		{'F','f'}, {'G','g'}, {'H','h'}, {'I','i'}, {'J','j'}, {'K','k'}, {'L','l'}, {'M','m'}, {'N','n'}, {'O','o'},
		{'P','p'}, {'Q','q'}, {'R','r'}, {'S','s'}, {'T','t'}, {'U','u'}, {'V','v'}, {'W','w'}, {'X','x'}, {'Y','y'},
		{'Z','z'}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {'0','0'}, {'1','1'}, {'2','2'}, {'3','3'},
		{'4','4'}, {'5','5'}, {'6','6'}, {'7','7'}, {'8','8'}, {'9','9'}, {'*','*'}, {'+','+'}, {'\\','\\'}, {'-','-'},
		{'.','.'}, {'/','/'}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{'?',','}, {'+','='}, {'.',';'}, {0xFFB0,')'}, {'/',':'}, {0xFFA7,'!'}, {'~','`'}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0xFFA8,'^'}, {0xFFB5,'*'}, {0xFFA3,'$'}, {'%',0xFFF9}, {0,0}, {0,0}, {0,0}, {'>','<'}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}
		},
	};

	WISP_PHYS_FONT_INFO CWispBase::m_PhysFontInfo[WISP_FONT_COUNT] = 
	{ { 8, 16, "\\Font\\Font8x16.dat", "\\Font\\WF8x16.dat" },
	  { 6, 12, "\\Font\\Font6x12.dat", "\\Font\\WF6x12.dat" },
	  { 12, 16, "\\Font\\ASCI1216.dat", "\\Font\\ASCI1216.dat" },
	  { 12, 16, "\\Font\\EBCD1216.dat", "\\Font\\EBCD1216.dat" },
	  { 12, 16, "\\Font\\ANSI1216.dat", "\\Font\\ANSI1216.dat" },
	  { 8, 16, "\\Font\\Font8x16.dat", "\\Font\\WF8x16.dat" }, //???
	};

	unsigned long CWispBase::m_DefColorPal[256] =
	{
		0x000000, 0xFFFFFF, 0xC00000, 0xFF0000, 0x00C000, 0x00FF80, 0x0000C0, 0x0080FF,
		0x804040, 0xC86464, 0x00C0C0, 0x00FFFF, 0x808080, 0xC8C8C8, 0xC0C000, 0xFFFF00,
		0x804000, 0xFF8000, 0x000000, 0xC8C8C8, 0xECE9D8, 0x000000, 0xFFFFFF, 0x2398C8,
		0x50B0FF, 0x0080FF, 0x64C8FA, 0x0082E6, 0xDCDCDC, 0x555555, 0xAAAAAA, 0x50E6FF,
		0x32B4E6, 0x0000FF, 0x7F9DB9, 0x7F9DB9, 0x1C5180, 0xFFFFFF, 0xF3F3EF, 0xE2E1DA,
		0x3169C6, 0xF8B330, 0xFFD664, 0xFFFFFF, 0x000000, 0x000080, 0x0000FF, 0x316AC5,
		0xB4C8E4, 0xD2E6FF, 0x8A867A, 0x7898B5, 0x00FFFF, 0xFFFFFF, 0x21A121, 0x000000,
		0x76B1E5, 0xB4D6F1, 0x6492B9, 0xEBEADB, 0xFFFFFF, 0xC7C5B2, 0xE2DECD, 0xD6D2C2,
		0xCBC7B8, 0xF3F3F3, 0xF8A900, 0xF9C654, 0xF8B31F, 0xABEDAC, 0x95E996, 0x7CE47E,
		0x66DF68, 0x4EDA50, 0x35D538, 0x28D22B, 0xE59700, 0xF8B330, 0xFDD889, 0x3D95FF,
		0x2B90FF, 0x0055EA, 0x0046E0, 0x0143CF, 0x003DDC, 0x001DA0, 0x00138C, 0x7B7B7B,
		0xE0E8FE, 0xF1F1F1, 0xE7EBFF, 0x6B79BD, 0xACA899, 0xF1EFE2, 0x716F64, 0xDEDFD8,
		0xA5A597, 0xC1C2B8, 0xD0D1C9, 0xB4FAD4, 0x64C8FF, 0xFFAAAA, 0x009BFF, 0xB42828,
		0x70A2F3, 0xC5C2B8, 0x00FF00, 0xFF00FF, 0xFAFACC, 0x21A221, 0x7BA2E7, 0x6375D6,
		0xB4B1A3, 0xFEFEFF, 0xF0F0F1, 0xE6EAFF, 0xE0E4F9, 0xD2D8F7, 0x6878BF, 0x28080C,
		0x8B3377, 0x2D182C, 0xE8C8E8, 0xF8E8F8, 0x18081D, 0xB888C8, 0xD8B0E8, 0xC098D8,
		0x9A6AC8, 0xD8C8E8, 0x2D2C34, 0xB9B7C8, 0x6C679F, 0x080888, 0x080857, 0x080818,
		0x383880, 0x6868B8, 0xA8A8D8, 0xC8C8F8, 0xB8B8D8, 0xC8C8E8, 0xD8D8F8, 0xD8D8E8,
		0xE8E8F8, 0xC9CAD8, 0x5E6074, 0x92939C, 0x343856, 0x283898, 0x7788E9, 0x283888,
		0x384898, 0x4858A9, 0x5868B8, 0x7888D8, 0x8898E8, 0x5868A8, 0x6878B8, 0x7A88C7,
		0x384886, 0x8898D1, 0xABB8EB, 0x9AA4C7, 0x0837B9, 0x101830, 0x6878A8, 0x95ABEA,
		0x7A86A7, 0x183888, 0x28489A, 0x507FF5, 0x3858A8, 0x4868B9, 0x5878C8, 0x6888D8,
		0x1649B8, 0x16306C, 0x485C8A, 0x586B98, 0x296DF6, 0x284888, 0x385898, 0x4868A8,
		0x5878B8, 0x6888C8, 0x7898D8, 0x687897, 0xA8B8D8, 0xC8D8F8, 0x192840, 0x6E88B3,
		0x3868AE, 0x7898C8, 0x4778BB, 0x5285CC, 0x385985, 0x6799DE, 0xB4D1F8, 0x275998,
		0x587CA8, 0x37485C, 0x8EB1D8, 0xC8D8E8, 0x4FA8F7, 0x6899C8, 0xA8C9E8, 0xB6C8D8,
		0xA8B8C7, 0x084E87, 0x285A84, 0x78A8D0, 0x71BEF8, 0xD8E8F4, 0x8FD1F6, 0x097BB3,
		0x477F9D, 0x47ABC8, 0x5EDCF8, 0x11A7BC, 0x1BCDE7, 0x2BDFF8, 0x85E9F7, 0xB1EFF8,
		0x6DC9CE, 0x2E8587, 0x081818, 0xE8F8F8, 0x146A60, 0x24924C, 0x137C1E, 0xE5F8E7,
		0x69AE69, 0xBFE0BD, 0x42E90F, 0x6FBF31, 0xA9E36E, 0xD9EDA8, 0xF8F8E8, 0xB3A24F,
		0xF8EBA6, 0xF8F2D5, 0x8C6E11, 0xDEC17B, 0xBE811D, 0x522C1F, 0xF2C2BE, 0xF5544B,
		0xEB8581, 0x180808, 0xF8E8E8, 0x191818, 0xF8F8F8, 0xE8E8E8, 0x484848, 0x080808,
	};

	CWispBase::CWispBase()
	{
		m_Metrics[WISP_SM_CAPTION_CY] = 18;
		m_Metrics[WISP_SM_BORDER_SIZE] = 4;
		m_Metrics[WISP_SM_TIP_BORDER_SIZE] = 2;

		m_Metrics[WISP_SM_CAPTION_BT_SIZE] = 16;
		m_Metrics[WISP_SM_SCROLL_BAR_SIZE] = 16;
		m_Metrics[WISP_SM_TOOLBAR_CY] = 18;
		m_Metrics[WISP_SM_RESIZE_SIZE] = 6;

		m_pRootWnd = 0;

		memset(m_PalColors, 0, sizeof(m_PalColors));

		//memset(&m_FrameBufferInfo, 0, sizeof(m_FrameBufferInfo));
		m_FrameBufferInfo.Width = 0;
		m_FrameBufferInfo.Height = 0;
		m_FrameBufferInfo.Bpp = 0;
		m_FrameBufferInfo.PixelFormat = 0;
		m_FrameBufferInfo.Flag = 0;
		m_FrameBufferInfo.VideoBuffer = 0;
		m_FrameBufferInfo.VideoBufferSize = 0;
		m_FrameBufferInfo.LineDistance = 0;
		m_FrameBufferInfo.pWispDrawHAL = 0;

		memcpy(m_PalColors, CWispBase::m_DefColorPal, sizeof(m_PalColors));
		m_bHideMouse = 0;
		m_szWispResource = 0;
		m_bActive = 0;
		GetModulePath(m_szModuleName, true);
		m_FontIndex = WISP_FONT_6X12; //WISP_FONT_8X16
		m_KeyLangMap = 0;
	}

	CWispBase::~CWispBase()
	{
	}

	bool CWispBase::Init()
	{
		gpCurWisp = this;
		m_bActive = 0;
		m_pWispDrawObj = 0;
		m_KeySimulateMouse = 0;
		if (m_szWispResource == 0) m_szWispResource = "Wisp.dat";

		m_PFSFileIO.ChangeFileIO(gpFileIO);

		char szWispDatPath[MAX_PATH];
		int n = TStrCpy(szWispDatPath, m_szModuleName);
		if (n && szWispDatPath[n-1] != '\\')
			TStrCat(szWispDatPath, "\\");

		char *szWispDatPlace = szWispDatPath+TStrLen(szWispDatPath);
		const char *szWispResource = m_szWispResource;

		while (*szWispResource != 0)
		{
			char *delim = TStrChr(szWispResource, ';');
			if (delim)
			{
				TStrCpyLimit(szWispDatPlace, szWispResource, delim+1-szWispResource);
				szWispResource = delim+1;
			} else
			{
				TStrCpy(szWispDatPlace, szWispResource);
				szWispResource += TStrLen(szWispResource);
			}

			if (!m_PFSFileIO.OpenDisk(szWispDatPath, 1))
			{
				WCHAR szWispDatPathW[MAX_FN_LEN];
				AnsiToUnicode(szWispDatPath, szWispDatPathW, lenof(szWispDatPathW));
				ReportMsg(WSTR("Error : Fail to open %s!\n"), szWispDatPathW);
				m_PFSFileIO.CloseDisk();
				return false;
			}
		}

		memset(m_KeyMap, 0, sizeof(m_KeyMap));
		m_LockMap[0] = 0;
		m_LockMap[1] = 0;
		m_LockMap[2] = 0;

		m_MousePT.x = 0;
		m_MousePT.y = 0;
		m_PrevMousePT.x = 0;
		m_PrevMousePT.y = 0;
		m_bUpdate = 0;
		m_UpdateLockRef = 0;

		m_DefDIB[WISP_DI_CARET].Create(2, 12, 3, 0, 0); //COLORREF_PF_565
		memcpy(m_DefDIB[WISP_DI_CARET].m_PixelBuf.VideoBuffer, BTCaret, m_DefDIB[WISP_DI_CARET].m_PixelBuf.VideoBufferSize);

		//m_DefDIB[WISP_DI_CURSOR].Load("\\Arrow.bmp", -1);
		//m_DefDIB[WISP_DI_CURSOR].m_Type = 2; //COLORREF_PF_555
		//m_DefDIB[WISP_DI_CURSOR].m_ColorKey = 0;

		m_pRootWnd = 0;
		m_bNeedUpdate = 0;

		for (int i = 0; i < lenof(CWispBase::m_PhysFontInfo); ++i)
		{
			m_FontList[i].LoadChar(&CWispBase::m_PhysFontInfo[i]); //TODO error
			m_FontList[i].LoadWideChar(&CWispBase::m_PhysFontInfo[i]); //TODO error
		}

		m_pTopMostRootWnd = 0;
		m_pTipWnd = 0;
		m_pModalWnd = 0;
		m_pDragWnd = 0;
		m_pMoveWnd = 0;
		m_RepKeyIndex = 0;
		m_CursorType = 0;
		m_CursorTypeBak = 0;
		m_WndCount = 0;
		m_BaseWndCount = 0;
		m_TimerLockRef = 0;
		m_bKeyMutex = 0;
		m_SpecKey = 0;
		m_PumpLockRef = 0;

		SelectDrawObject(&m_DIBDrawObj);

		return true;
	}

	void CWispBase::Release()
	{
		if (m_pRootWnd)
		{
			m_pRootWnd->Destroy();
			m_pRootWnd = 0;
		}

		if (m_pWispDrawObj)
			m_pWispDrawObj->Release();

		for (int i = 0; i < lenof(m_DefDIB); ++i)
			m_DefDIB[i].Destroy();

		for (int i = 0; i < lenof(m_FontList); ++i)
			m_FontList[i].Destroy();

		m_TimerList.clear();
		m_KeyEventList.Clear();
		m_DIBLib.ReleaseAll();
		m_PFSFileIO.CloseDisk();
	}

//	bool CreateFrame(const WCHAR *WndName, int Width, int Height, int Bpp) = 0;
//	void DestroyFrame() = 0;

//	bool CWispBase::EndResize(int Width, int Height) { return false; }
//	bool CWispBase::BeginResize(int Width, int Height) { return false; }
//	bool CWispBase::MoveFrame(int PosX, int PosY) { return false; }

//	void CWispBase::BeginDrag() { }
//	void CWispBase::EndDrag() { }

	void CWispBase::RedrawNextTime()
	{
		if (!m_bNeedUpdate)
		{
			m_bNeedUpdate = true;
			if (m_bInCriticalSection)
			{
				m_bInCriticalSection = false;
				WakeUpMsgThread();
			}
		}
	}
//	bool CWispBase::WakeUpMsgThread() { return true; }
//	bool CWispBase::SleepMsgThread() { return true; }

//	void CWispBase::EnterMsgCriticalSection() { }
//	void CWispBase::LeaveMsgCriticalSection() { }

//	void CWispBase::BeginDraw(WISP_RECT *pRect) { }
//	void CWispBase::EndDraw(WISP_RECT *pRect) { }

	void CWispBase::SetCursor(unsigned int CursorType, WISP_POINT *pHotPT, WISP_SIZE *pSize, CWispDIB *pDIB)
	{
		switch (CursorType)
		{
		case WISP_CT_ARROW:
			m_pWispDrawObj->DrawCursorArrow(0, 0);
			break;
		case WISP_CT_RESIZE_VERT:
			m_pWispDrawObj->DrawCursorResizeVert(0, 0);
			break;
		case WISP_CT_RESIZE_HORZ:
			m_pWispDrawObj->DrawCursorResizeHorz(0, 0);
			break;
		case WISP_CT_DRAG:
			m_pWispDrawObj->DrawCursorDrag(0, 0);
			break;
		case WISP_CT_RESIZE_LEFT_LEAN:
			m_pWispDrawObj->DrawCursorResizeLeftLean(0, 0);
			break;
		case WISP_CT_RESIZE_RIGHT_LEAN:
			m_pWispDrawObj->DrawCursorResizeRightLean(0, 0);
			break;
		default:
			if (pDIB)
			{
				if (pHotPT)
				{
					pHotPT->x = pDIB->m_HotPT.x;
					pHotPT->y = pDIB->m_HotPT.y;
				}

				if (pSize)
				{
					pSize->cx = pDIB->m_PaintRect.cx;
					pSize->cy = pDIB->m_PaintRect.cy;
				}
			}
			break;
		}

	}
//	void CWispBase::ChangeCursor(LONG_PTR Style, unsiged int CursorType, WISP_POINT *pHotPT, WISP_SIZE *pSize) { }

	void CWispBase::ChangeCursor(unsigned int CursorType)
	{
		if (m_CursorType != CursorType)
		{
			m_CursorTypeBak = m_CursorType;
			m_pCursorDIBBak = m_pCursorDIB;
			m_CursorType = CursorType;
			m_pCursorDIB = 0;
			SetCursor(m_CursorType, &m_MouseCursorPos, &m_MouseCursorSize, m_pCursorDIB);
			if (!m_bHideMouse)
				RedrawNextTime();
		}
	}

	void CWispBase::ChangeCursor(CWispDIB *pCursorDIB)
	{
		if (m_pCursorDIB != pCursorDIB)
		{
			m_CursorTypeBak = m_CursorType;
			m_pCursorDIBBak = m_pCursorDIB;
			m_CursorType = WISP_CT_PENDING;
			m_pCursorDIB = pCursorDIB;
			SetCursor(m_CursorType, &m_MouseCursorPos, &m_MouseCursorSize, m_pCursorDIB);
			if (!m_bHideMouse)
				RedrawNextTime();
		}
	}

	void CWispBase::Update()
	{
		WISP_RECT DrawRect;
		WISP_RECT MouseRect;
		WISP_MSG Msg;

		if (m_pRootWnd)
		{
			Msg.hWnd = m_pRootWnd;

			if (m_pRootWnd->m_pUpdateRC)
			{
				DrawRect = *m_pRootWnd->m_pUpdateRC;
			} else
			if (m_bHideMouse)
			{
				return;
			}

			if (!m_bHideMouse)
			{
				MouseRect.y = m_MousePT.y - m_MouseCursorPos.y;
				MouseRect.x = m_MousePT.x - m_MouseCursorPos.x;
				MouseRect.cx = m_MouseCursorSize.cx;
				MouseRect.cy = m_MouseCursorSize.cy;

				RectToRect(&MouseRect, &m_MouseRectUpd); //Union |
				m_pRootWnd->Update(&m_MouseRectUpd, 0);

				if (!m_pRootWnd->m_pUpdateRC)
					return;

				DrawRect = *m_pRootWnd->m_pUpdateRC;

				m_MouseRectUpd = MouseRect;
			}

			BeginDraw(&DrawRect);
			Msg.Msg = WISP_WM_UPDATE;
			SendMessage(&Msg);
			UpdateCursor();
			EndDraw(&DrawRect);
		}
	}

	void CWispBase::UpdateCursor()
	{
		if (!m_bHideMouse)
		{
			m_pRootWnd->m_pUpdateRC = &m_pRootWnd->m_WindowRect;
			m_pWispDrawObj->DrawCursor(&m_MousePT, &m_pRootWnd->m_WindowDC,
						m_CursorType, m_pCursorDIB);
			m_pRootWnd->m_pUpdateRC = 0;
		}
	}

//	bool CWispBase::InsertTimer(const WISP_TIMER &) { return true; }
//	bool CWispBase::RemoveTimer(const WISP_TIMER &) { return true; }

	unsigned long CWispBase::GetPalColor(int Index)
	{
		return m_PalColors[Index];
	}
	bool CWispBase::SetPalColor(int Index, unsigned long Color)
	{
		m_PalColors[Index] = Color;
		return true;
	}
	void CWispBase::LoadPalette()
	{
		for (int i = 0; i < 256; ++i)
			SetPalColor(i, m_PalColors[i]);
	}

	void CWispBase::RemoveWnd(CWispBaseWnd *pWnd)
	{
		if (!m_pRootWnd)
			return;
		if (m_pRootWnd == pWnd)
		{
			m_pRootWnd = 0;
			m_bActive = 0;
			return;
		}
		
		if (pWnd->m_PrevWnd)
			pWnd->m_PrevWnd->m_NextWnd = pWnd->m_NextWnd;

		if (pWnd->m_NextWnd)
			pWnd->m_NextWnd->m_PrevWnd = pWnd->m_PrevWnd;

		if (pWnd->m_ParentWnd->m_ChildWnd == pWnd)
			pWnd->m_ParentWnd->m_ChildWnd = pWnd->m_PrevWnd ? pWnd->m_PrevWnd : pWnd->m_NextWnd;

		if (pWnd->m_ParentWnd->m_ChildTopWnd == pWnd)
			pWnd->m_ParentWnd->m_ChildTopWnd = pWnd->m_PrevWnd;

		if (pWnd->m_Style & 0x8000)
			--m_WndCount;
		else	--m_BaseWndCount;
	}

	void CWispBase::RemoveWndMsg(CWispBaseWnd *pWnd)
	{
		for (int i = 0; i < m_MsgQueue.Count; ++i)
		{
			if (m_MsgQueue[i].hWnd == pWnd)
				m_MsgQueue[i].hWnd = 0;
		}
	}
	void CWispBase::InsertWnd(CWispBaseWnd *pParentWnd, CWispBaseWnd *pNewWnd)
	{
		if (m_pRootWnd == pNewWnd)
			return;
		
		if (!pParentWnd)
			pParentWnd = m_pRootWnd;

		if (pNewWnd->m_Style & 0x8000)
			++m_WndCount;
		else	++m_BaseWndCount;

		if (pParentWnd->m_ChildWnd)
		{
			pNewWnd->m_NextWnd = 0;
			pParentWnd->m_ChildTopWnd->m_NextWnd = pNewWnd;
			pNewWnd->m_PrevWnd = pParentWnd->m_ChildTopWnd;
			pNewWnd->m_ParentWnd = pParentWnd;
			pParentWnd->m_ChildTopWnd = pNewWnd;

			if (pParentWnd == m_pRootWnd && pNewWnd != gpCurWisp->m_pTopMostRootWnd)
				gpCurWisp->m_pTopMostRootWnd->Top(false);
		} else
		{
			pParentWnd->m_ChildWnd = pNewWnd;
			pParentWnd->m_ChildTopWnd = pNewWnd;
			pNewWnd->m_PrevWnd = 0;
			pNewWnd->m_NextWnd = 0;
			pNewWnd->m_ParentWnd = pParentWnd;
		}
		
	}
	CWispBaseWnd *CWispBase::WindowFromPoint(WISP_POINT *pPoint, CWispBaseWnd *pParentWnd)
	{
		CWispBaseWnd *pWnd = pParentWnd ? pParentWnd : m_pRootWnd;

		if (!pWnd->IsRect(pPoint))
			return nullptr;

		for (CWispBaseWnd *pChildWnd = pWnd->m_ChildTopWnd; pChildWnd; pChildWnd = pChildWnd->m_PrevWnd)
		{
			//TODO assert infinet loop when CreateWnd twice
			if (pChildWnd->m_ShowMode != WISP_SH_HIDDEN &&
			    pChildWnd->m_CtrlType != WISP_CTRL_STATIC_GROUP)
			{
				if (pWnd->IsRect(pPoint))
				{
					if (pChildWnd->IsRect(pPoint))
					{
						CWispBaseWnd *res = WindowFromPoint(pPoint, pChildWnd);
						if (!(pChildWnd->m_Style & 0x100) || (res && res != pChildWnd))
							return res;
					}
				}
			}
		}
		return (pWnd->m_Style & 0x100) == 0 ? pWnd : nullptr;
	}

	bool CWispBase::MergeMessage(WISP_MSG *pNewMsg, WISP_MSG *pMergeMsg)
	{
		if (pNewMsg->Msg == pMergeMsg->Msg && pNewMsg->Msg == WISP_WM_MOUSE_MOVE)
		{
			pMergeMsg->MouseEvent.hWndMouseAt = pNewMsg->MouseEvent.hWndMouseAt;
			pMergeMsg->MouseEvent.DeltaX += pNewMsg->MouseEvent.DeltaX;
			pMergeMsg->MouseEvent.DeltaY += pNewMsg->MouseEvent.DeltaY;
			return true;
		}
		return false;
	}

	//GetMessage

	bool CWispBase::SendMessage(WISP_MSG *pMsg)
	{
		if (!pMsg->hWnd)
			return true;

		pMsg->bMsgLBTDown = m_KeyMap[VK_LBUTTON];
		pMsg->bMsgRBTDown = m_KeyMap[VK_RBUTTON];
		pMsg->MsgMouseScrPT = m_MousePT;
		pMsg->MsgMouseWndPT = m_MousePT;
		PointToRect(&pMsg->MsgMouseWndPT, &pMsg->hWnd->m_ScrWindowRect);

		pMsg->m_LogicMousePosWnd = pMsg->MsgMouseWndPT;
		if (pMsg->hWnd->m_Style & 0x8000) //WispWnd
			PointToRect(&pMsg->m_LogicMousePosWnd, &((CWispWnd*)pMsg->hWnd)->m_ClientRect);
			
		bool r = DispatchMessage(pMsg); //pMsg->hWnd->MsgProc(pMsg);// if (!DispatchMessage(pMsg)) return false;

		if (m_bInCriticalSection)
		{
			WakeUpMsgThread();
		}
		return r;
	}
	bool CWispBase::PostMessage(WISP_MSG *pMsg)
	{
		if (!pMsg->hWnd)
			return false;
		
		pMsg->bMsgLBTDown = m_KeyMap[VK_LBUTTON];
		pMsg->bMsgRBTDown = m_KeyMap[VK_RBUTTON];
		pMsg->MsgMouseScrPT = m_MousePT;
		pMsg->MsgMouseWndPT = m_MousePT;
		PointToRect(&pMsg->MsgMouseWndPT, &pMsg->hWnd->m_ScrWindowRect);

		if (m_MsgQueue.Count)
		{
			WISP_MSG *pLastMsg = &m_MsgQueue[m_MsgQueue.Count-1];
			if (pLastMsg->hWnd != pMsg->hWnd || !MergeMessage(pMsg, pLastMsg))
				m_MsgQueue.Append(pMsg);
		} else
		{
			m_MsgQueue.Append(pMsg);
		}

		if (m_bInCriticalSection)
		{
			m_bInCriticalSection = false;
			WakeUpMsgThread();
		}
		return true;
	}
#if 0
	const char *Msg2String(unsigned int Msg)
	{
		switch (Msg)
		{
		case WISP_WM_CREATING:
				return "WISP_WM_CREATING";
		case WISP_WM_CREATE:
				return "WISP_WM_CREATE";
		case WISP_WM_CREATE_FORM:
				return "WISP_WM_CREATE_FORM";
		case WISP_WM_DESTROY:
				return "WISP_WM_DESTROY";
		case WISP_WM_CLOSE:
				return "WISP_WM_CLOSE";

		case WISP_WM_KEY_EVENT:
				return "WISP_WM_KEY_EVENT";

		case WISP_WM_MOUSE_ENTER:
				return "WISP_WM_MOUSE_ENTER";
		case WISP_WM_MOUSE_MOVE:
				return "WISP_WM_MOUSE_MOVE";
		case WISP_WM_MOUSE_LEAVE:
				return "WISP_WM_MOUSE_LEAVE";
		case WISP_WM_MOUSE_WHEEL:
				return "WISP_WM_MOUSE_WHEEL";
		case WISP_WM_MOUSE_DBLCLICK:
				return "WISP_WM_MOUSE_DBLCLICK";

		case WISP_WM_COMMAND:
				return "WISP_WM_COMMAND";
		case WISP_WM_EVENT:
				return "WISP_WM_EVENT";

		case WISP_WM_UPDATE:
				return "WISP_WM_UPDATE";
		case WISP_WM_UPDATE_BORDER:
				return "WISP_WM_UPDATE_BORDER";
		case WISP_WM_UPDATE_CAPTION:
				return "WISP_WM_UPDATE_CAPTION";
		case WISP_WM_UPDATE_CLIENT:
				return "WISP_WM_UPDATE_CLIENT";
		case WISP_WM_UPDATE_CARET:
				return "WISP_WM_UPDATE_CARET";
		case WISP_WM_UPDATE_HORZ_SCROLL_BAR:
				return "WISP_WM_UPDATE_HORZ_SCROLL_BAR";
		case WISP_WM_UPDATE_VERT_SCROLL_BAR:
				return "WISP_WM_UPDATE_VERT_SCROLL_BAR";

		case WISP_WM_BEGIN_MOVE:
				return "WISP_WM_BEGIN_MOVE";
		case WISP_WM_END_MOVE:
				return "WISP_WM_END_MOVE";
		case WISP_WM_MOVE:
				return "WISP_WM_MOVE";
		case WISP_WM_MOVING:
				return "WISP_WM_MOVING";
		case WISP_WM_SIZE:
				return "WISP_WM_SIZE";
		case WISP_WM_SIZING:
				return "WISP_WM_SIZING";
		case WISP_WM_RECALC_LAYOUT:
				return "WISP_WM_RECALC_LAYOUT";
		case WISP_WM_RESIZE_CHILD:
				return "WISP_WM_RESIZE_CHILD";

		case WISP_WM_CHAR:
				return "WISP_WM_CHAR";

		case WISP_WM_SCROLL_EVENT:
				return "WISP_WM_SCROLL_EVENT";

		case WISP_WM_VERT_SCROLL_MOVE:
				return "WISP_WM_VERT_SCROLL_MOVE";
		case WISP_WM_HORZ_SCROLL_MOVE:
				return "WISP_WM_HORZ_SCROLL_MOVE";

		case WISP_WM_TIMER_EVENT:
				return "WISP_WM_TIMER_EVENT";

		case WISP_WM_GET_FOCUS:
				return "WISP_WM_GET_FOCUS";
		case WISP_WM_LOST_FOCUS:
				return "WISP_WM_LOST_FOCUS";

		case WISP_WM_HIT_TEST:
				return "WISP_WM_HIT_TEST";

		case WISP_WM_DRAG_BEGIN:
				return "WISP_WM_DRAG_BEGIN";
		case WISP_WM_DRAG_MOVE:
				return "WISP_WM_DRAG_MOVE";
		case WISP_WM_DRAG_END:
				return "WISP_WM_DRAG_END";
		}
		return "UNKNOWN";
	}
#endif
	//int Level = 0;
	//char Buffer[1024];

	bool CWispBase::DispatchMessage(WISP_MSG *pMsg)
	{
		if (pMsg->hWnd)
		{
			//++Level;
			//int i = 0;
			//for (i = 0; i < Level; i++) //TSPrintf(Buffer, " "); //DebugPrintf(" ");
			//	Buffer[i] = 0x20;
			//TSPrintf(Buffer, "> MSG %x\n", pMsg->Msg);
			//DebugPrintf("MSG %x %s\n", pMsg->Msg, Msg2String(pMsg->Msg));
			//DebugPrintf("%s\n", Buffer);
			return pMsg->hWnd->MsgProc(pMsg); //???
			//for (int i = 0; i < Level; i++) //DebugPrintf(" ");
			//	Buffer[i] = 0x20;
			//DebugPrintf(&Buffer[i], "\n");
			//DebugPrintf("<\n");
			//--Level;
		}

		return false;
	}

	void CWispBase::DispatchMsgQueue()
	{
		while (m_MsgQueue.Count)
		{
			WISP_MSG Msg;
			memcpy(&Msg, &m_MsgQueue[0], sizeof(WISP_MSG)); //Pop()
			m_MsgQueue.Remove();

			DispatchMessage(&Msg);
		}
	}
	void CWispBase::DispatchInputQueue()
	{
		while (m_InputQueue.Count)
		{
			WISP_INPUT *Input = &m_InputQueue[0]; //Pop()
			m_InputQueue.Remove();

			switch (Input->Type)
			{
			case WISP_INPUT_KEY:
				OnKeyEvent(Input->u.KeyEvent.KeyType,
					Input->u.KeyEvent.bKeyDown, Input->u.KeyEvent.ScanCode);
				break;

			case WISP_INPUT_POS:
				OnMousePosChg(Input->u.MousePos.PosX, Input->u.MousePos.PosY);
				break;

			case WISP_INPUT_MOVE:
				OnMouseMove(Input->u.MouseMove.DeltaX, Input->u.MouseMove.DeltaY);
				break;

			case WISP_INPUT_WHEEL:
				OnMouseWheel(Input->u.MouseWheel.Delta);
				break;

			case WISP_INPUT_DBLCLICK:
				OnMouseDblClick(Input->u.MouseDblClick.Button);
				break;

			case WISP_INPUT_TIMER:
				OnTimer(Input->u.Timer.MilliSec);
				break;

			default:
				break;
			}
		}
	}

	void CWispBase::LockUpdate()
	{
		++m_UpdateLockRef;
	}
	void CWispBase::UnlockUpdate()
	{
		if (m_UpdateLockRef > 0)
			--m_UpdateLockRef;
	}

	void CWispBase::Pump()
	{
		m_bActive = true;

		while (m_bActive)
		{
			if (!PumpCondition())
				break;

			EnterMsgCriticalSection();

			DispatchInputQueue();
			DispatchMsgQueue();

			if (m_bActive && m_bNeedUpdate && !m_UpdateLockRef)
			{
				Update();
				m_bNeedUpdate = 0;
			}

			LeaveMsgCriticalSection();

			m_bInCriticalSection = true;
			SleepMsgThread();
			m_bInCriticalSection = false;
		}

		EnterMsgCriticalSection();
		Update();
		LeaveMsgCriticalSection();

		m_bActive = false;
	}

	void CWispBase::PumpQueue()
	{
		if (!m_bActive) return;

		DispatchInputQueue();
		DispatchMsgQueue();

		if (m_bActive && m_bNeedUpdate && !m_UpdateLockRef)
		{
			Update();
			m_bNeedUpdate = 0;
		}

		LeaveMsgCriticalSection();
		m_bInCriticalSection = true;
		SleepMsgThread();
		m_bInCriticalSection = false;
		EnterMsgCriticalSection();
	}

	void CWispBase::BlockPump(CWispBaseWnd *pWnd)
	{
		++m_PumpLockRef;
		while (m_bActive)
		{
			if (!pWnd->IsWindow() || !pWnd->IsWindowShow())
				break;

			DispatchInputQueue();
			DispatchMsgQueue();

			if (m_bActive && m_bNeedUpdate && !m_UpdateLockRef)
			{
				Update();
				m_bNeedUpdate = 0;
			}

			LeaveMsgCriticalSection();
			m_bInCriticalSection = true;
			SleepMsgThread();
			m_bInCriticalSection = false;
			EnterMsgCriticalSection();
		}
		--m_PumpLockRef;
	}
	bool CWispBase::PumpCondition()
	{
		return !m_KeyMap[VK_F12] || !m_KeyMap[VK_CONTROL];
	}

	const WCHAR * CWispBase::GetClipboard()
	{
		return m_ClipString.operator const WCHAR*();
	}
	bool CWispBase::SetClipboard(const WCHAR *Text)
	{
		m_ClipString = Text;
		return true;
	}

	void CWispBase::GetFrameBuffer(WISP_FRAME_BUFFER_INFO *pFrameBuffer)
	{
		switch (pFrameBuffer->Bpp)
		{
		case 4:
			pFrameBuffer->pWispDrawHAL = &m_Draw4Bit;
			break;
		case 8:
			pFrameBuffer->pWispDrawHAL = &m_Draw8Bit;
			break;
		case 16:
			pFrameBuffer->pWispDrawHAL = &m_Draw16Bit;
			break;
		case 24:
			pFrameBuffer->pWispDrawHAL = &m_Draw24Bit;
			break;
		case 32:
			pFrameBuffer->pWispDrawHAL = &m_Draw32Bit;
			break;
		default:
			pFrameBuffer->pWispDrawHAL = &m_DrawHAL;
			break;
		}
	}

	void CWispBase::CaptureMouseEvent(CWispBaseWnd *pWnd)
	{
		m_pMouseEventWnd = pWnd;
	}

	void CWispBase::BeginMove(CWispBaseWnd *pWnd)
	{
		if (!pWnd)
			pWnd = m_pRootWnd;
		m_pMoveWnd = pWnd;
		WISP_MSG Msg;
		Msg.hWnd = pWnd;
		Msg.Msg = WISP_WM_BEGIN_MOVE;
		pWnd->m_State |= 1; //WISP_WSTATE_MOVE
		SendMessage(&Msg);
	}
	void CWispBase::EndMove()
	{
		if (m_pMoveWnd && m_pMoveWnd->IsWindow())
		{
			WISP_MSG Msg;
			Msg.hWnd = m_pMoveWnd;
			Msg.Msg = WISP_WM_END_MOVE;
			Msg.hWnd->m_State &= ~1; //~WISP_WSTATE_MOVE
			SendMessage(&Msg);
			m_pMoveWnd = 0;
		}
	}

	void CWispBase::ResetInput()
	{
		
		memset(m_KeyMap, 0, sizeof(m_KeyMap));
		m_LockMap[0] = 0;
		m_LockMap[1] = 0;
		m_LockMap[2] = 0;

		m_RepKeyIndex = 0;
		m_RepKeyElapse = 0;
		m_RepKeyPeriod = 0;
	}

	void CWispBase::StopMouseKeyRep()
	{
		m_RepKeyIndex = 0;
	}
	void CWispBase::StartMouseKeyRep(unsigned int RepKeyIndex)
	{
		if (m_RepKeyIndex != RepKeyIndex)
		{
			m_RepKeyIndex = RepKeyIndex;
			m_RepKeyElapse = 0;
			m_RepKeyPeriod = 500;
		}
	}

	bool CWispBase::KeySimulateMouse(unsigned int KeyType, bool bKeyDown)
	{
		switch (KeyType)
		{
		case VK_PRIOR:
		case VK_END:
			OnKeyEvent(VK_LBUTTON, bKeyDown, 0);
			break;

		case VK_NEXT:
			OnKeyEvent(VK_RBUTTON, bKeyDown, 0);
			break;

		case VK_LEFT:
		case VK_UP:
		case VK_RIGHT:
		case VK_DOWN:
		{
			int y = 0;
			int x = 0;

			if (m_KeyMap[VK_UP])
				y = -m_KeySimDelta;
			if (m_KeyMap[VK_DOWN])
				y = m_KeySimDelta;
			if (m_KeyMap[VK_LEFT])
				x = -m_KeySimDelta;
			if (m_KeyMap[VK_RIGHT])
				x += m_KeySimDelta;

			if (m_KeySimDelta < 20)
				++m_KeySimDelta;

			OnMousePosChg(x + m_PrevMousePT.x, y + m_PrevMousePT.y);
			break;
		}
		default:
			return false;
			break;
		}
		return true;
	}
	unsigned short CWispBase::IsCharKey(unsigned int KeyType)
	{
		bool bLowCase = m_KeyMap[VK_SHIFT] == false;
		if (m_LockMap[0])	// CAPS LOCK ?
			bLowCase = bLowCase == 0;
		return CWispBase::m_VKKeyChar[m_KeyLangMap][KeyType][bLowCase];
	}

	void CWispBase::OnMousePosChg(int PosX, int PosY)
	{
		if (m_MousePT.x == PosX && m_MousePT.y == PosY)
			return;

		m_MousePT.x = PosX;
		m_MousePT.y = PosY;

		MIN_LIMIT(m_MousePT.x, 0);
		MIN_LIMIT(m_MousePT.y, 0);

		MAX_LIMIT(m_MousePT.x, m_FrameBufferInfo.Width-1);
		MAX_LIMIT(m_MousePT.y, m_FrameBufferInfo.Height-1);

		int DeltaX = m_MousePT.x - m_PrevMousePT.x;
		int DeltaY = m_MousePT.y - m_PrevMousePT.y;

		m_PrevMousePT = m_MousePT;

		if (!m_bHideMouse)
			RedrawNextTime();

		if (m_pMoveWnd && m_pMoveWnd->m_State & 1) //WISP_WSTATE_MOVE
		{
			m_pMoveWnd->Move(DeltaX, DeltaY, true);
			return;
		}

		CWispWnd *pWispWnd = (CWispWnd *)WindowFromPoint(&m_MousePT, m_pRootWnd);
		if (!pWispWnd)
			return;

		//0x200
		if ((pWispWnd->m_Style >> 9) & 1)
			return;

		if (gpCurWisp->m_pDragWnd == m_pMouseEventWnd)
		{
			WISP_MSG MsgMove;
			MsgMove.hWnd = m_pMouseEventWnd;
			MsgMove.Msg = WISP_WM_DRAG_MOVE;
			MsgMove.DragEvent.BeginMouseWndPT = m_BeginPT;
			MsgMove.DragEvent.DeltaX = DeltaX;
			MsgMove.DragEvent.DeltaY = DeltaY;

			gpCurWisp->SendMessage(&MsgMove); //??? ->
			return;
		}


		if (pWispWnd->m_State & 2) //WISP_WSTATE_RESIZE
		{
			pWispWnd->Move(
				-(DeltaX * pWispWnd->m_SignResizeX),
				-(DeltaY * pWispWnd->m_SignResizeY),  true);

			if (!pWispWnd->Resize(
				pWispWnd->m_WindowRect.cx - DeltaX * pWispWnd->m_SignResizeDX,
				pWispWnd->m_WindowRect.cy - DeltaY * pWispWnd->m_SignResizeDY, true))
			{
				m_pMouseEventWnd->m_State &= ~2; //~WISP_WSTATE_RESIZE
				ChangeCursor(WISP_CT_ARROW);
			}
			return;
		}

		WISP_MSG Msg;

		//CWispWnd *pScrollWnd = (CWispWnd*)m_pMouseEventWnd;

		if (pWispWnd != m_pMouseEventWnd)
		{
			if (m_pMouseEventWnd->m_State & 1) //WISP_WSTATE_MOVE
			{
				m_pMouseEventWnd->Move(DeltaX, DeltaY, true);
				return;
			}

			if (m_pMouseEventWnd->m_State & 2) //WISP_WSTATE_RESIZE
			{
				CWispWnd *pScrollWnd = (CWispWnd*)m_pMouseEventWnd;

				m_pMouseEventWnd->Move(
					-(DeltaX * pScrollWnd->m_SignResizeX),
					-(DeltaY * pScrollWnd->m_SignResizeY),
					true);

				if (!m_pMouseEventWnd->Resize(
					m_pMouseEventWnd->m_WindowRect.cx - DeltaX * pScrollWnd->m_SignResizeDX,
					m_pMouseEventWnd->m_WindowRect.cy - DeltaY * pScrollWnd->m_SignResizeDY,
					true))
				{
					m_pMouseEventWnd->m_State &= ~2; //~WISP_WSTATE_RESIZE
					ChangeCursor(WISP_CT_ARROW);
				}
				return;
			}

			Msg.hWnd = pWispWnd;
			Msg.Msg = WISP_WM_HIT_TEST;
			Msg.HitTest.HitArea.Type = 0;
			Msg.HitTest.HitArea.State = 0;
			Msg.HitTest.bKeyDown = m_KeyMap[VK_LBUTTON];

			SendMessage(&Msg);

			WISP_HIT_AREA HitArea = Msg.HitTest.HitArea;

			if (m_pMouseEventWnd->m_Style & 0x8000)
			{
				CWispWnd *pScrollWnd = (CWispWnd*)m_pMouseEventWnd;
				if (pScrollWnd->m_bInResizeArea)
					ChangeCursor(WISP_CT_ARROW);
			}

			Msg.hWnd = m_pMouseEventWnd;
			Msg.Msg = WISP_WM_MOUSE_LEAVE;

			if (m_pMouseEventWnd->m_Style & 0x8000)
			{
				CWispWnd *pScrollWnd = (CWispWnd*)m_pMouseEventWnd;
				if (pScrollWnd->m_pScrollBar[0] &&
					pScrollWnd->m_pScrollBar[0]->State == 2)
					Msg.Msg = WISP_WM_VERT_SCROLL_MOVE;
				else
				if (pScrollWnd->m_pScrollBar[1] &&
					pScrollWnd->m_pScrollBar[1]->State == 2)
					Msg.Msg = WISP_WM_HORZ_SCROLL_MOVE;
			}

			Msg.MouseEvent.HitArea = HitArea;
			Msg.MouseEvent.hWndMouseAt = pWispWnd;
			Msg.MouseEvent.DeltaX = DeltaX;
			Msg.MouseEvent.DeltaY = DeltaY;
			Msg.MouseEvent.Wheel = 0;

			SendMessage(&Msg);

			if (m_pMouseEventWnd == pWispWnd)
			{
				Msg.hWnd = m_pMouseEventWnd;
				Msg.Msg = WISP_WM_MOUSE_ENTER;
				SendMessage(&Msg);
				return;
			}
			return;
		}

		if (m_pModalWnd	&& !pWispWnd->IsChildWndOf(m_pModalWnd)	&& pWispWnd != m_pModalWnd)
			return;

		Msg.hWnd = pWispWnd;
		Msg.Msg = WISP_WM_HIT_TEST;
		Msg.HitTest.HitArea.Type = 0;
		Msg.HitTest.HitArea.State = 0;
		Msg.HitTest.bKeyDown = false;
		SendMessage(&Msg);

		WISP_HIT_AREA HitArea = Msg.HitTest.HitArea;

		unsigned long Type = HitArea.Type & WISP_HT_MASK;

		if (Type == 0)
		{
			if (m_pMouseEventWnd->m_Style & 0x8000)
			{
				CWispWnd *pScrollWnd = (CWispWnd*)m_pMouseEventWnd;
				if (pScrollWnd->m_bInResizeArea)
				{
					ChangeCursor(WISP_CT_ARROW);
					pScrollWnd->m_bInResizeArea = 0;
				}
			}
		} else
		{
			unsigned int CursorType = WISP_CT_ARROW;

			switch (Type)
			{
			case WISP_HT_LEFT: //0x10000:
			case WISP_HT_RIGHT: //0x20000:
				CursorType = WISP_CT_RESIZE_HORZ;
				break;
			case WISP_HT_TOP: //0x40000:
			case WISP_HT_BOTTOM: //0x80000:
				CursorType = WISP_CT_RESIZE_VERT;
				break;
			case WISP_HT_TOPLEFT: //0x50000:
			case WISP_HT_BOTTOMRIGHT: //0xA0000:
				CursorType = WISP_CT_RESIZE_LEFT_LEAN;
				break;
			case WISP_HT_TOPRIGHT: //0x60000:
			case WISP_HT_BOTTOMLEFT: //0x90000:
				CursorType = WISP_CT_RESIZE_RIGHT_LEAN;
				break;

		//	case 0x80000:
		//		CursorType = 1;
		//		break;
		//	case 0x90000:
		//		CursorType = 5;
		//		break;
		//	case 0xA0000:
		//		CursorType = 4;
		//		break;
			}

			if (m_pMouseEventWnd->m_Style & 0x8000)
			{
				CWispWnd *pScrollWnd = (CWispWnd*)m_pMouseEventWnd;
				pScrollWnd->m_bInResizeArea = 1;
				ChangeCursor(CursorType);
			}
		}

		Msg.hWnd = pWispWnd;
		Msg.Msg = WISP_WM_MOUSE_MOVE;
		Msg.MouseEvent.HitArea = HitArea;
		Msg.MouseEvent.hWndMouseAt = pWispWnd;
		Msg.MouseEvent.DeltaX = DeltaX;
		Msg.MouseEvent.DeltaY = DeltaY;
		Msg.MouseEvent.Wheel = 0;

		if (m_MouseHookWndList.Size())
		{
			TListIter<CWispBaseWnd*> it = m_MouseHookWndList.Begin();
			for (; it != m_MouseHookWndList.End(); ++it)
			{
				if (Msg.hWnd != *it)
				{
					Msg.hWnd = *it;
					if (!SendMessage(&Msg))
						return;
				}
			}
		}

		Msg.hWnd = pWispWnd;
		if (pWispWnd->m_Style & 0x8000)
		{
			if (pWispWnd->m_pScrollBar[0] && pWispWnd->m_pScrollBar[0]->State == 2)
				Msg.Msg = WISP_WM_VERT_SCROLL_MOVE;
			else
			if (pWispWnd->m_pScrollBar[1] && pWispWnd->m_pScrollBar[1]->State == 2)
				Msg.Msg = WISP_WM_HORZ_SCROLL_MOVE;
		}

		SendMessage(&Msg);
		return;

	}
	void CWispBase::SendMousePosChg(int PosX, int PosY)
	{
		WISP_INPUT *p = m_InputQueue.Append(0);
		p->Type = WISP_INPUT_POS;
		p->u.MousePos.PosX = PosX;
		p->u.MousePos.PosY = PosY;
	}

	void CWispBase::OnMouseMove(int DeltaX, int DeltaY)
	{
		SendMousePosChg(DeltaX + m_MousePT.x, DeltaY + m_MousePT.y);
	}
	void CWispBase::SendMouseMove(int DeltaX, int DeltaY)
	{
		WISP_INPUT *p = m_InputQueue.Append(0);
		p->Type = WISP_INPUT_MOVE;
		p->u.MouseMove.DeltaX = DeltaX;
		p->u.MouseMove.DeltaY = DeltaY;
	}

	void CWispBase::OnMouseWheel(int Delta)
	{
		if (m_pFocusWnd)
		{
			WISP_MSG Msg;
			Msg.hWnd = m_pFocusWnd;
			Msg.Msg = WISP_WM_MOUSE_WHEEL;
			Msg.MouseEvent.HitArea.Type = 0;
			Msg.MouseEvent.HitArea.State = 0;
			Msg.MouseEvent.hWndMouseAt = Msg.hWnd;
			Msg.MouseEvent.DeltaX = 0;
			Msg.MouseEvent.DeltaY = 0;
			Msg.MouseEvent.Wheel = Delta;
			SendMessage(&Msg);
		}
	}
	void CWispBase::SendMouseWheel(int Delta)
	{
		WISP_INPUT *p = m_InputQueue.Append(0);
		p->Type = WISP_INPUT_WHEEL;
		p->u.MouseWheel.Delta = Delta;
	}

	void CWispBase::OnMouseDblClick(unsigned int Button) //L=1,R=2
	{
		if (m_pFocusWnd)
		{
			if (m_HitArea.Type == 1 ||
			    m_HitArea.Type == 3)
			{
				WISP_MSG Msg;
				Msg.hWnd = m_pFocusWnd;
				Msg.Msg = WISP_WM_MOUSE_DBLCLICK;
				Msg.KeyEvent.HitArea = m_HitArea;
				Msg.KeyEvent.KeyType = Button;
				Msg.KeyEvent.bKeyDown = true;
				Msg.KeyEvent.ScanCode = 0;
				SendMessage(&Msg);
			}
		}
	}
	void CWispBase::SendMouseDblClick(unsigned int Button) //L=1,R=2
	{
		WISP_INPUT *p = m_InputQueue.Append(0);
		p->Type = WISP_INPUT_DBLCLICK;
		p->u.MouseDblClick.Button = Button;
	}

	bool CWispBase::OnProcessKeyEvent(unsigned int KeyType, bool bKeyDown, int ScanCode)
	{
		if (!m_pFocusWnd)
			return false;

		CWispBaseWnd *pWispWnd = m_pFocusWnd;

		if (m_pModalWnd
			&& !pWispWnd->IsChildWndOf(m_pModalWnd)
			&& pWispWnd != m_pModalWnd)
		{
			pWispWnd = m_pModalWnd;
		}

		m_KeyMap[KeyType] = bKeyDown;

		if (KeyType == VK_LSHIFT || KeyType == VK_RSHIFT)
			m_KeyMap[VK_SHIFT] = m_KeyMap[VK_LSHIFT] | m_KeyMap[VK_RSHIFT];

		if (KeyType == VK_LMENU || KeyType == VK_RMENU)
			m_KeyMap[VK_MENU] = m_KeyMap[VK_LMENU] | m_KeyMap[VK_RMENU];

		if (KeyType == VK_LCONTROL || KeyType == VK_RCONTROL)
			m_KeyMap[VK_CONTROL] = m_KeyMap[VK_LCONTROL] | m_KeyMap[VK_RCONTROL];

		if (m_KeyMap[VK_SHIFT])
			m_SpecKey |= WISP_SK_SHIFT;
		else	m_SpecKey &= ~WISP_SK_SHIFT;

		if (m_KeyMap[VK_MENU])
			m_SpecKey |= WISP_SK_ALT;
		else	m_SpecKey &= ~WISP_SK_ALT;

		if (m_KeyMap[VK_CONTROL])
			m_SpecKey |= WISP_SK_CTRL;
		else	m_SpecKey &= ~WISP_SK_CTRL;


		if (m_KeySimulateMouse)
		{
			if (bKeyDown)
			{
				if (KeyType != VK_END &&
				    KeyType != VK_NEXT &&
				    KeyType != VK_PRIOR)
				{
					m_RepKeyIndex = KeyType;
					m_RepKeyElapse = 0;
					m_RepKeyPeriod = 600;
					m_KeySimDelta = 1;
				}
			} else
			if (m_RepKeyIndex == KeyType)
			{
				m_RepKeyIndex = 0;	//StopMouseKeyRep()
			}

			if (KeySimulateMouse(KeyType, bKeyDown))
				return true;
		}

		if (bKeyDown && m_KeyEventList.Size() != 0)
		{
			TListIter<WISP_KEY_EVENT> it = m_KeyEventList.Begin();
			for (; it != m_KeyEventList.End(); ++it)
			{
				if (it->KeyType == KeyType)
				{
					if (it->SpecKey == 0 || it->SpecKey == m_SpecKey)
					{
						WISP_MSG MsgEvent;
						MsgEvent.hWnd = (CWispWnd*)it->pWnd;
						MsgEvent.Msg = WISP_WM_EVENT;
						MsgEvent.Command.CmdMsg = 0x80002000; //WISP_ID_FORM_CLOSE_ID_START
						MsgEvent.Command.CmdID = it->CmdID;

						if ( (it->Disable & 1) == 0
							|| pWispWnd->IsChildWndOf((CWispWnd*)it->pWnd)
							|| pWispWnd == it->pWnd)
						{
							SendMessage(&MsgEvent);
						}
					}
				}
			}
		}

		WISP_MSG Msg;
		Msg.hWnd = pWispWnd;
		Msg.Msg = WISP_WM_KEY_EVENT;
		Msg.KeyEvent.KeyType = m_SpecKey | KeyType;
		Msg.KeyEvent.bKeyDown = bKeyDown;
		Msg.KeyEvent.ScanCode = ScanCode;

		if (m_KeyHookWndList.Size() != 0)
		{
			TListIter<CWispBaseWnd*> it = m_KeyHookWndList.Begin();
			for (; it != m_KeyHookWndList.End(); ++it)
			{
				if (Msg.hWnd != *it)
				{
					Msg.hWnd = *it;
					SendMessage(&Msg);
				}
			}
		}

		if (!((pWispWnd->m_Style>>9)&1)) //0x200
		{
			Msg.hWnd = pWispWnd;
			if (!SendMessage(&Msg))
				return true;
		}

		if (!bKeyDown)
			return true;
		
		if (m_KeyMap[VK_CONTROL] && m_KeyMap[VK_MENU])
		{
			if (m_KeyMap[VK_F1])
			{
				SelectDrawObject(&m_DefDrawObj);
				m_pRootWnd->Update((WISP_RECT*)0); //???
			} else
			if (m_KeyMap[VK_F2])
			{
				SelectDrawObject(&m_DIBDrawObj);
				m_pRootWnd->Update((WISP_RECT*)0); //???
			}
		}


		if (m_SpecKey == 0 || m_SpecKey == WISP_SK_SHIFT)
		{
			if (KeyType == VK_CAPITAL)
				m_LockMap[0] = m_LockMap[0] == 0;

			unsigned short Char = IsCharKey(KeyType);

			if (Char)
			{
				CWispBaseWnd *v21 = m_pFocusWnd;
				if (m_pModalWnd
					&& !v21->IsChildWndOf(m_pModalWnd)
					&& v21 != m_pModalWnd)
				{
					v21 = m_pModalWnd;
				}

				Msg.hWnd = v21;
				Msg.Msg = WISP_WM_CHAR;
				Msg.Char.nRepeat = 1;
				Msg.Char.Char = Char;

				PostMessage(&Msg);

				TListIter<CWispBaseWnd*> it = m_KeyHookWndList.Begin();
				for (; it != m_KeyHookWndList.End(); ++it)
				{
					if (Msg.hWnd != *it)
					{
						Msg.hWnd = *it;
						PostMessage(&Msg);
					}
				}
			}
		}
		return true;
	}

	bool CWispBase::OnProcessMouseKeyEvent(unsigned int KeyType, bool bKeyDown, int ScanCode)
	{
		//TODO???

//		CWispWnd *v4;
		CWispWnd *pWispWnd_v8;
//		int v8;
//		int v9;
//		int v10;
//		int v11;

		WISP_MSG Msg;

		m_KeyMap[KeyType] = bKeyDown;

		if (m_pMoveWnd && m_pMoveWnd->m_State & 1) //WISP_WSTATE_MOVE
		{
			if (KeyType == VK_LBUTTON)
			{
				if (!bKeyDown)
				{
					EndMove();
					return true;
				}
//				goto LABEL_8;
			}
		}
//		else
		if (KeyType == VK_LBUTTON && bKeyDown)
		{
//LABEL_8:
			pWispWnd_v8 = (CWispWnd*)WindowFromPoint(&m_MousePT, m_pRootWnd);
			if (m_pModalWnd
				&& !pWispWnd_v8->IsChildWndOf(m_pModalWnd)
				&& pWispWnd_v8 != m_pModalWnd)
			{
				pWispWnd_v8 = (CWispWnd*)m_pModalWnd;
			}

			if (!pWispWnd_v8)
				return false;

			//???
			if ( (pWispWnd_v8->m_Style >> 9) & 1 )
				return true;

			if (pWispWnd_v8 != m_pFocusWnd)
				pWispWnd_v8->Focus();
		} else
		{
			pWispWnd_v8 = (CWispWnd*)m_pMouseEventWnd;
			if (m_pModalWnd
				&& !pWispWnd_v8->IsChildWndOf(m_pModalWnd)
				&& pWispWnd_v8 != m_pModalWnd)
			{
				pWispWnd_v8 = (CWispWnd*)m_pModalWnd;
			}

			//???
			if ( (pWispWnd_v8->m_Style >> 9) & 1 )
				return true;
		}

		Msg.hWnd = pWispWnd_v8;

		if (!bKeyDown)
		{
			if (m_RepKeyIndex == KeyType)
				StopMouseKeyRep();

			Msg.Msg = WISP_WM_HIT_TEST;
			Msg.HitTest.HitArea.Type = 0;
			Msg.HitTest.HitArea.State = 0;
			Msg.HitTest.bKeyDown = false;
			SendMessage(&Msg);

			Msg.KeyEvent.HitArea = Msg.HitTest.HitArea;
			Msg.hWnd = pWispWnd_v8;
			Msg.Msg = WISP_WM_KEY_EVENT;
			Msg.KeyEvent.KeyType = KeyType;
			Msg.KeyEvent.bKeyDown = false;
			Msg.KeyEvent.ScanCode = ScanCode;
			SendMessage(&Msg);

			if (KeyType == VK_LBUTTON)
			{
				if (pWispWnd_v8->m_State & 2) //WISP_WSTATE_RESIZE
				{
					pWispWnd_v8->m_State &= ~2; //~WISP_WSTATE_RESIZE
					ChangeCursor(WISP_CT_ARROW);
				}
			}
			return true;
		}

		if (pWispWnd_v8->m_AdvStyle & 4) //WISP_ADV_CANMOVE
		{
			if (pWispWnd_v8->m_State & 5) //WISP_WSTATE_MAXIMIZE | WISP_WSTATE_MOVE
				return true;

			pWispWnd_v8->m_State |= 1;	//WISP_WSTATE_MOVE
			Msg.Msg = WISP_WM_BEGIN_MOVE;
			SendMessage(&Msg);
			return true;
		}

		Msg.Msg = WISP_WM_HIT_TEST;
		Msg.HitTest.HitArea.Type = 0;
		Msg.HitTest.HitArea.State = 0;
		Msg.HitTest.bKeyDown = bKeyDown;

		if ((pWispWnd_v8->m_Style & 0x8000) == 0)
		{
			SendMessage(&Msg);

			Msg.KeyEvent.HitArea = Msg.HitTest.HitArea;
			Msg.hWnd = pWispWnd_v8;
			Msg.Msg = WISP_WM_KEY_EVENT;
			Msg.KeyEvent.KeyType = KeyType;
			Msg.KeyEvent.bKeyDown = bKeyDown;
			Msg.KeyEvent.ScanCode = ScanCode;
			SendMessage(&Msg);
			return true;
		}

		SendMessage(&Msg);

		m_HitArea = Msg.HitTest.HitArea;

		//1-ON_SCRCLIENT,2-ON_CAP,3-ON_RCCLIENT,4-ON_CLOSE,5-ON_MAX,6-ON_MIN,
		switch (m_HitArea.Type)
		{
		case 2:
			if (KeyType == VK_LBUTTON && !(pWispWnd_v8->m_State & 5) ) //WISP_WSTATE_MAXIMIZE | WISP_WSTATE_MOVE
			{
				BeginMove(pWispWnd_v8);
				return true;
			}
			return true;
		case 1:
		case 3:
//LABEL_55:
			Msg.Msg = WISP_WM_KEY_EVENT;
			Msg.KeyEvent.KeyType = KeyType;
			Msg.KeyEvent.bKeyDown = bKeyDown;
			Msg.KeyEvent.ScanCode = ScanCode;
			SendMessage(&Msg);
			return true;
		}

		if (KeyType == VK_LBUTTON && (m_HitArea.Type & WISP_HT_MASK)) //0xF0000)) //MASK
		{
			pWispWnd_v8->m_SignResizeDY = 0;
			pWispWnd_v8->m_SignResizeY = 0;
			pWispWnd_v8->m_SignResizeDX = 0;
			pWispWnd_v8->m_SignResizeX = 0;

			if (m_HitArea.Type & WISP_HT_LEFT) //0x10000)	//HORZ	LEFT
			{
				pWispWnd_v8->m_SignResizeX = -1;
				pWispWnd_v8->m_SignResizeDX = 1;
			}

			if (m_HitArea.Type & WISP_HT_RIGHT) //0x20000)	//HORZ	RIGHT
			{
				pWispWnd_v8->m_SignResizeX = 0;
				pWispWnd_v8->m_SignResizeDX = -1;
			}

			if (m_HitArea.Type & WISP_HT_TOP) //0x40000)	//VERT	TOP
			{
				pWispWnd_v8->m_SignResizeY = -1;
				pWispWnd_v8->m_SignResizeDY = 1;
			}

			if (m_HitArea.Type & WISP_HT_BOTTOM) //0x80000)	//VERT BOTTOM
			{
				pWispWnd_v8->m_SignResizeY = 0;
				pWispWnd_v8->m_SignResizeDY = -1;
			}
			pWispWnd_v8->m_State |= 2; //WISP_WSTATE_RESIZE
		}
		return true;
	}

	void CWispBase::OnKeyEvent(unsigned int KeyType, bool bKeyDown, int ScanCode)
	{
		if (!m_bKeyMutex)
		{
			if (KeyType <= 7) //??? MOUSE VS KEY
				OnProcessMouseKeyEvent(KeyType, bKeyDown, ScanCode);
			else	OnProcessKeyEvent(KeyType, bKeyDown, ScanCode);
		}

	}
	void CWispBase::SendKeyEvent(unsigned int KeyType, bool bKeyDown, int ScanCode)
	{
		WISP_INPUT *p = m_InputQueue.Append(0);
		p->Type = WISP_INPUT_KEY;
		p->u.KeyEvent.KeyType = KeyType;
		p->u.KeyEvent.bKeyDown = bKeyDown;
		p->u.KeyEvent.ScanCode = ScanCode;
	}

	void CWispBase::LockTimer()
	{
		++m_TimerLockRef;
	}
	void CWispBase::UnlockTimer()
	{
		if (m_TimerLockRef > 0)
			--m_TimerLockRef;
	}

	void CWispBase::OnTimer(int MilliSec)
	{
		WISP_MSG MsgHit;
		WISP_MSG Msg;

		if (m_TimerLockRef > 0)
			return;
		
		if (m_RepKeyIndex)
		{
			m_RepKeyElapse += MilliSec;
			if (m_KeySimulateMouse)
				KeySimulateMouse(m_RepKeyIndex, 1);

			if (m_RepKeyElapse >= m_RepKeyPeriod)
			{
				m_RepKeyElapse -= m_RepKeyPeriod;
				if (m_RepKeyIndex < 7)	//??? MOUSE VS KEY
				{
					if (m_RepKeyPeriod == 500)
						m_RepKeyPeriod = 50;
				} else
				{
					if (m_RepKeyPeriod == 600)
						m_RepKeyPeriod = 15;
				}

				Msg.hWnd = m_pFocusWnd;
				Msg.Msg = WISP_WM_KEY_EVENT;

				if (m_RepKeyIndex == VK_LBUTTON)
				{
					MsgHit.hWnd = m_pFocusWnd;
					MsgHit.Msg = WISP_WM_HIT_TEST;
					MsgHit.HitTest.HitArea.Type = 0;
					MsgHit.HitTest.HitArea.State = 0;
					MsgHit.HitTest.bKeyDown = true;

					SendMessage(&MsgHit);

					Msg.KeyEvent.HitArea = MsgHit.KeyEvent.HitArea;
				}

				Msg.KeyEvent.KeyType = m_RepKeyIndex;
				Msg.KeyEvent.bKeyDown = true;
				SendMessage(&Msg);

				unsigned short Char = IsCharKey(m_RepKeyIndex);

				if (Char)
				{
					CWispWnd *hWnd = (CWispWnd *)m_pFocusWnd;
					if (m_pModalWnd
						&& !hWnd->IsChildWndOf(m_pModalWnd)
						&& hWnd != m_pModalWnd)
					{
						hWnd = (CWispWnd *)m_pModalWnd;
					}
					Msg.hWnd = hWnd;
					Msg.Msg = WISP_WM_CHAR;
					Msg.Char.nRepeat = 1;
					Msg.Char.Char = Char;

					SendMessage(&Msg);
				}
			}
		}

		map<WISP_TIMER_KEY, WISP_TIMER>::IT it = m_TimerList.begin();
		while (it != m_TimerList.end())
		{
			map<WISP_TIMER_KEY, WISP_TIMER>::IT cur = it++;
			if (MilliSec <= cur->second.Period)
			{
				cur->second.Elapse += MilliSec;
				if (cur->second.Elapse > cur->second.Period)
				{
					cur->second.Elapse -= cur->second.Period;
					Msg.hWnd = cur->first.pWnd;
					Msg.Msg = WISP_WM_TIMER_EVENT;
					Msg.Timer.ID = cur->first.ID;
					Msg.Timer.UserData = cur->second.UserData;
					Msg.Timer.UserPtr = cur->second.UserPtr;
					SendMessage(&Msg);
				}
			}
		}
	}

	void CWispBase::SendTimer(int MilliSec)
	{
		WISP_INPUT *p = m_InputQueue.Append(0);
		p->Type = WISP_INPUT_TIMER;
		p->u.Timer.MilliSec = MilliSec;
	}

	void CWispBase::AdjustColor()
	{
		for (int i = 0; i < lenof(m_pWispDrawObj->m_crSystem); ++i)
			m_pWispDrawObj->m_crSystem[i] = FindFitColor(m_pWispDrawObj->m_crSystem[i], m_PalColors);
	}

	void CWispBase::OnFrameBufferBppChg(int Bpp)
	{
		m_FrameBufferInfo.Bpp = Bpp;
		switch (Bpp)
		{
		case 4:
			m_FrameBufferInfo.pWispDrawHAL = &m_Draw4Bit;
			break;
		case 8:
			m_FrameBufferInfo.pWispDrawHAL = &m_Draw8Bit;
			break;
		case 16:
			m_FrameBufferInfo.pWispDrawHAL = &m_Draw16Bit;
			break;
		case 24:
			m_FrameBufferInfo.pWispDrawHAL = &m_Draw24Bit;
			break;
		case 32:
			m_FrameBufferInfo.pWispDrawHAL = &m_Draw32Bit;
			break;
		default:
			m_FrameBufferInfo.pWispDrawHAL = &m_DrawHAL;
			break;
		}

		if (Bpp == 8)
		{
			LoadPalette();
			AdjustColor();
		}
	}

	void CWispBase::OnCreateMousePos(int PosX, int PosY)
	{
		m_MousePT.x = PosX;
		m_MousePT.y = PosY;

		m_PrevMousePT.x = PosX;
		m_PrevMousePT.y = PosY;
	}

//	bool CWispBase::GetOpenFolderName(WCHAR *Path, const WCHAR *Title) { return false; }
//	bool CWispBase::GetOpenFileName(const WCHAR *InitialDir, const WCHAR *Filter, int, WCHAR *File, int MaxFile) { return false; }

//	void CWispBase::ReportMsg(const WCHAR *Format, ...) { }

	int CWispBase::GetMetrics(unsigned int Type)
	{
		return (Type < WISP_SM_COUNT) ? m_Metrics[Type] : 0;
	}

	bool CWispBase::SelectDrawObject(CWispDrawObj *pDrawObj)
	{
		if (m_pWispDrawObj == pDrawObj)
		{
			if (m_pWispDrawObj)
			{
				m_pWispDrawObj->Release();
				return m_pWispDrawObj->Init();
			}
			return false;
		}

		if (pDrawObj && !pDrawObj->Init())
			return false;

		if (m_pWispDrawObj)
			m_pWispDrawObj->Release();
		m_pWispDrawObj = pDrawObj;
		return true;
	}

	CWispDIB *CWispBase::GetDefDIB(unsigned int ID)
	{
		return (ID < WISP_DI_COUNT) ? &m_DefDIB[ID] : 0;
	}

	void *CWispBase::GetScreenBuffer(int x, int y)
	{
		return (char*)m_FrameBufferInfo.VideoBuffer
				+ y * m_FrameBufferInfo.LineDistance
				+ x * m_FrameBufferInfo.Bpp / 8;
	}

	bool CWispBase::CopyToScreen(WISP_FRAME_BUFFER_INFO *pRealScreen, WISP_POINT *pPT, WISP_RECT *pRect)
	{
		CWispDrawHAL *pDrawHAL;
		WISP_RECT rcClip;
		WISP_RECT rc;

		  if (!pRealScreen->VideoBuffer)
			return false;

		int xoff, yoff;

		if (pPT)
		{
			xoff = pPT->x;
			yoff = pPT->y;
		} else
		{
			xoff = (pRealScreen->Width - m_FrameBufferInfo.Width) / 2; //CENTER_ALIGN
			yoff = (pRealScreen->Height - m_FrameBufferInfo.Height) / 2;
			MIN_LIMIT(xoff, 0);
			MIN_LIMIT(yoff, 0);
		}

		if (pRect)
		{
			xoff += pRect->x;
			yoff += pRect->y;

			rc.x = xoff;
			rc.y = yoff;
			rc.cx = pRect->cx;
			rc.cy = pRect->cy;
		} else
		{
			rc.x = xoff;
			rc.y = yoff;
			rc.cx = m_FrameBufferInfo.Width;
			rc.cy = m_FrameBufferInfo.Height;
		}

		switch (pRealScreen->Bpp)
		{
		case 4:
			pDrawHAL = &m_Draw4Bit;
			break;
		case 8:
			pDrawHAL = &m_Draw8Bit;
			break;
		case 16:
			pDrawHAL = &m_Draw16Bit;
			break;
		case 24:
			pDrawHAL = &m_Draw24Bit;
			break;
		case 32:
			pDrawHAL = &m_Draw32Bit;
			break;
		default:
			pDrawHAL = &m_DrawHAL;
			break;
		}

		if (pRect)
		{
			xoff -= pRect->x;
			yoff -= pRect->y;
		}


		rcClip.x = 0;
		rcClip.y = 0;
		rcClip.cx = pRealScreen->Width;
		rcClip.cy = pRealScreen->Height;

		if (ClipRect(&rc, &rcClip))
		{
			yoff = -yoff;
			for (int y = 0; y < rc.cy; ++y)
			{
				pDrawHAL->CopyToPixel((char*)pRealScreen->VideoBuffer +
							pRealScreen->LineDistance * (rc.y + y) +
							rc.x * pRealScreen->Bpp / 8,

							(char*)m_FrameBufferInfo.VideoBuffer +
							m_FrameBufferInfo.LineDistance * (rc.y + yoff) +
							m_FrameBufferInfo.Bpp * (rc.x - xoff) / 8,

							rc.cx);
				++yoff;
			}
		}

		return true;
	}

	bool CWispBase::CopyScreenToFile(const char *FileName)
	{
		CWispDIB DIB;
		DIB.m_PixelBuf = m_FrameBufferInfo;
		bool ret = DIB.Write(FileName);
		memset(&DIB.m_PixelBuf, 0, sizeof(DIB.m_PixelBuf));
		return ret;
	}

	int CWispBase::MsgBox(const WCHAR *pText, const WCHAR *pCaption, unsigned int uType, CWispBaseWnd *pParentWnd)
	{
		return CWispMsgBox::StaticShowMsgBox(pText, pCaption, uType, pParentWnd);
	}

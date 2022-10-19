
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "codeview.hpp"
#include "syserconfig.hpp"
#include "resource.hpp"

	WISP_MSG_MAP_BEGIN(CCodeView)
	//	WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_CREATING, CWispList::OnCreating)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_MOUSE_DBLCLICK, OnMouseDblClick)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
		WISP_MSG_MAP(WISP_WM_CHAR, OnChar)
		WISP_MSG_MAP(WISP_WM_GET_FOCUS, OnGetFocus)
		WISP_MSG_MAP(WISP_WM_SCROLL_EVENT, OnScrollEvent)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

	WISP_EVENT_MAP_BEGIN(CCodeView)
		WISP_EVENT_MAP(EVENT_ID_NEXTPOS, OnEventNextPos)
		WISP_EVENT_MAP(EVENT_ID_PREVPOS, OnEventPrevPos)
		WISP_EVENT_MAP(EVENT_ID_SAVEPOS, OnEventSavePos)
		WISP_EVENT_MAP(EVENT_ID_CLRPOS, OnEventResetPos)
		WISP_EVENT_MAP(EVENT_ID_SEARCH_TRANSFER_REF, OnEventSearchTrRef)
		WISP_EVENT_MAP(EVENT_ID_SEARCH_OPERAND_REF, OnEventSearchOpRef)

		WISP_EVENT_MAP(EVENT_ID_TOGGLE_BM, OnEventToggleBM)

		WISP_EVENT_MAP(EVENT_ID_FOLLOW_CIP_AUTO, OnEventFollowAuto)
		WISP_EVENT_MAP(EVENT_ID_FOLLOW_CIP_SYSEXPLORER, OnEventFollowCodeView)
		WISP_EVENT_MAP(EVENT_ID_FOLLOW_CIP_SRCEXPLORER, OnEventFollowSourceView)

		WISP_EVENT_MAP(EVENT_ID_IDENTIFY_FUNCTION, OnEventAnalyze)

		WISP_EVENT_MAP(EVENT_ID_CV_ADDR_MODE, OnEventModeAddr)
		WISP_EVENT_MAP(EVENT_ID_CV_OFFSET_MODE, OnEventModeOffs)
		WISP_EVENT_MAP(EVENT_ID_CV_SWITCH_SYM, OnEventSymSwitch)
		WISP_EVENT_MAP(EVENT_ID_CV_FULL_SYM, OnEventFullSym)

		WISP_EVENT_MAP(EVENT_ID_REL_CIP, OnEventModeRelCIP)
		WISP_EVENT_MAP(EVENT_ID_EDIT_CODE, OnEventEditCode)
		WISP_EVENT_MAP(EVENT_ID_EDIT_CMT, OnEventEditComment)
		WISP_EVENT_MAP(EVENT_ID_STRREF, OnEventStrRef)
//		WISP_EVENT_MAP(EVENT_ID_STRFIND, OnEventStrFind)
		WISP_EVENT_MAP(EVENT_ID_CV_NEXT_CIP, OnEventNextCIPTrace)
		WISP_EVENT_MAP(EVENT_ID_CV_PREV_CIP, OnEventPrevCIPTrace)
		WISP_EVENT_MAP(EVENT_ID_CV_SHOW_AT, OnEventShowAt)
		WISP_EVENT_MAP(EVENT_ID_CV_NEXT, OnEventNextAddr)
		WISP_EVENT_MAP(EVENT_ID_CV_PREV, OnEventPrevAddr)

		WISP_EVENT_MAP(EVENT_ID_CV0, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_CV1, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_CV2, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_CV3, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_CV4, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_CV5, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_CV6, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_CV7, OnEventNewView)
		WISP_EVENT_MAP(EVENT_ID_CV8, OnEventNewView)

		WISP_EVENT_MAP(EVENT_ID_GOTO_HERE, OnEventGotoHere)

		WISP_EVENT_MAP(EVENT_ID_INSREMBP, OnEventToggleAddBP)
		WISP_EVENT_MAP(EVENT_ID_ENDISMBP, OnEventToggleEnableBP)
		WISP_EVENT_MAP(WISP_ID_COPY, OnEventCopy)
	WISP_EVENT_MAP_END

	WISP_CMD_MAP_BEGIN(CCodeView)
		WISP_CMD_MAP(EVENT_ID_STRFIND, OnCmdFindString)
		WISP_CMD_MAP(WISP_ID_VSPIN_SCROLL, OnCmdVSpinScroll)
	WISP_CMD_MAP_END


	CCodeView::CCodeView()
	{
		m_CurAddr = 0;
		m_szKeyword[0] = 0;
		m_HistoryIter = m_History.End(); //???
		m_nScrollLine = 2;
	}

	CCodeView::~CCodeView()
	{
	}

WISP_MENU_GROUP_ITEM CodeViewPopupMenuCVMode[] =
{
	{WSTR("Symbol Switch        Tab"), EVENT_ID_CV_SWITCH_SYM, 0, 2, 0},
	{WSTR("Full Symbol          Ctrl+Tab"), EVENT_ID_CV_FULL_SYM, 0, 2, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Address Mode"), EVENT_ID_CV_ADDR_MODE, 0, 2, 0},
	{WSTR("Offset  Mode"), EVENT_ID_CV_OFFSET_MODE, 0, 2, 0},
	{WSTR("Relative to " CIPSTR ""), EVENT_ID_REL_CIP, 0, 2, 0},
	{0},
};

WISP_MENU_GROUP_ITEM CodeViewPopupMenuFolowMode[] =
{
	{WSTR("Automatic"), EVENT_ID_FOLLOW_CIP_AUTO, 0, 2, 0},
	{WSTR("System Explorer"), EVENT_ID_FOLLOW_CIP_SYSEXPLORER, 0, 2, 0},
	{WSTR("Source Code Explorer"), EVENT_ID_FOLLOW_CIP_SRCEXPLORER, 0, 2, 0},
	{0},
};

WISP_MENU_GROUP_ITEM CodeViewPopupMenu[] =
{
	{WSTR("Insert/Remove  Breakpoint      F9"), EVENT_ID_INSREMBP, 0xF3, 0, 0},
	{WSTR("Enable/Disable Breakpoint"), EVENT_ID_ENDISMBP, 0xF4, 0, 0},
	{WSTR("Toggle BookMark                Ctrl+K"), EVENT_ID_TOGGLE_BM, 0xD0, 0, 0},
	{WSTR("Save Position                  Shift+Enter"), EVENT_ID_SAVEPOS, 0x18, 0, 0},
	{WSTR("Identify standard C(++) Library Function"), EVENT_ID_IDENTIFY_FUNCTION, 0x6F, 0, 0},
	{WSTR("Go To Here                     F7"), EVENT_ID_GOTO_HERE, 0x10D, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Search Transfer Reference"), EVENT_ID_SEARCH_TRANSFER_REF, 0x118, 0, 0},
	{WSTR("Search Operand Reference"), EVENT_ID_SEARCH_OPERAND_REF, 0x119, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Code View Mode"), 0, 0x3D, 1, CodeViewPopupMenuCVMode},
	{WSTR("" CIPSTR " Follow Mode"), 0, 0x10, 1, CodeViewPopupMenuFolowMode},
	{WSTR("Copy"), WISP_ID_COPY, 0x23, 0, 0},
	{WSTR("Edit Code"), EVENT_ID_EDIT_CODE, 0x7E, 0, 0},
	{WSTR("Edit Comment"), EVENT_ID_EDIT_CMT, 0x93, 0, 0},
	{WSTR("String Reference"), EVENT_ID_STRREF, 0x93, 0, 0},
	{0},
};

	bool CCodeView::OnUpdate(WISP_MSG *pMsg)
	{
		m_Color[1] = gSyserColors.Color[6];

		m_Color[2] = gSyserColors.Color[9];
		m_Color[3] = gSyserColors.Color[10];
		m_Color[4] = gSyserColors.Color[11];
		m_Color[5] = gSyserColors.Color[12];
		m_Color[6] = gSyserColors.Color[13];
		m_Color[7] = gSyserColors.Color[14];
		m_Color[8] = gSyserColors.Color[15];
		m_Color[9] = gSyserColors.Color[16];
		m_Color[10] = gSyserColors.Color[17];
		m_Color[11] = gSyserColors.Color[18];
		m_Color[12] = gSyserColors.Color[19];
		m_Color[13] = gSyserColors.Color[20];
		m_Color[14] = gSyserColors.Color[21];
		m_Color[15] = gSyserColors.Color[22];
		m_Color[16] = gSyserColors.Color[23];
		m_Color[17] = gSyserColors.Color[24];
		m_Color[18] = gSyserColors.Color[25];
		m_Color[19] = gSyserColors.Color[26];
		m_Color[20] = gSyserColors.Color[27];
		m_Color[21] = gSyserColors.Color[28];
		m_Color[22] = gSyserColors.Color[29];
		m_Color[23] = gSyserColors.Color[30];
		m_Color[24] = gSyserColors.Color[31];
		m_Color[25] = gSyserColors.Color[32];
		m_Color[26] = gSyserColors.Color[33];

		m_ColorFocus = gSyserColors.Color[7];
		m_ColorUnFocus = gSyserColors.Color[8];

		m_ClientDC.m_pColor = m_Color;

		SetBGColor(m_Color[COLOR_BG]);

		UpdateDasm();
		return true;//CWispWnd::OnUpdate(pMsg);
	}

	bool CCodeView::OnCreate(WISP_MSG *pMsg)
	{
		m_Color[1] = gSyserColors.Color[6];

		m_Color[2] = gSyserColors.Color[9];
		m_Color[3] = gSyserColors.Color[10];
		m_Color[4] = gSyserColors.Color[11];
		m_Color[5] = gSyserColors.Color[12];
		m_Color[6] = gSyserColors.Color[13];
		m_Color[7] = gSyserColors.Color[14];
		m_Color[8] = gSyserColors.Color[15];
		m_Color[9] = gSyserColors.Color[16];
		m_Color[10] = gSyserColors.Color[17];
		m_Color[11] = gSyserColors.Color[18];
		m_Color[12] = gSyserColors.Color[19];
		m_Color[13] = gSyserColors.Color[20];
		m_Color[14] = gSyserColors.Color[21];
		m_Color[15] = gSyserColors.Color[22];
		m_Color[16] = gSyserColors.Color[23];
		m_Color[17] = gSyserColors.Color[24];
		m_Color[18] = gSyserColors.Color[25];
		m_Color[19] = gSyserColors.Color[26];
		m_Color[20] = gSyserColors.Color[27];
		m_Color[21] = gSyserColors.Color[28];
		m_Color[22] = gSyserColors.Color[29];
		m_Color[23] = gSyserColors.Color[30];
		m_Color[24] = gSyserColors.Color[31];
		m_Color[25] = gSyserColors.Color[32];
		m_Color[26] = gSyserColors.Color[33];

		m_ColorFocus = gSyserColors.Color[7];
		m_ColorUnFocus = gSyserColors.Color[8];

		m_ClientDC.m_pColor = m_Color;

		m_PopupMenu.LoadPopupMenuGroup(CodeViewPopupMenu, this, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF));
		m_DIBList.Load("\\SyserIcon\\CodeView.bmp", 16, 16, 4);
		m_DIBList.SetType(2);
		m_DIBList.SetColorKey(0xFFFF);

		int Ext = m_ClientDC.GetTextExtent(WSTR("X"), -1, 0);
		InsertColumn(WSTR("Pic"), 20, 2, 0, -1);// CV_START = 0
		InsertColumn(WSTR("Address"), (1+ADDR_SIZE) * Ext + 3, 0, 0, -1);// CV_ADDR = 1
		InsertColumn(WSTR("Bin"), (7+ADDR_SIZE) * Ext, 0, 0, -1);// CV_CODE = 2
		InsertColumn(WSTR("Code"), (27+ADDR_SIZE) * Ext, 0, 0, -1);// CV_INST = 3
		InsertColumn(WSTR("Comment"), 200, 1, 0, -1);// CV_CMT = 4
		Recalc(0);
		m_Style |= 0x3000000;
		SetBGColor(m_Color[COLOR_BG]);
		m_pScrollBar[0]->Style = 0;
		m_pScrollBar[0]->bHide = 0;
		m_Mode = 0;
		m_LineJmpWidth = 13;
		m_pLineMarkDIBList = gpCurWisp->m_DIBLib.LoadDIBList("\\SyserApp\\LineMark.bmp", 16, 16, 0, 0xFF00FF);
		m_pLineMarkDIBList->SetColorKey(0);
		LoadKeyMap();
		return true;
	}
	bool CCodeView::OnDestroy(WISP_MSG *pMsg)
	{
		m_DIBList.Destroy();
		m_PopupMenu.Destroy();
		return true;
	}

	void CCodeView::OnKeyLButton(WISP_MSG *pMsg)
	{
		TListIter<CListStringItem> Iter;
		int Len = TStrLen(m_szKeyword);
		int nCol = Point2Iter(&pMsg->m_LogicMousePosWnd, Iter);
		//if (nCol >= 1 && nCol <= 4096)
		if (nCol == CV_INST)
		{
			CListStringItem *Item = &*Iter;
			if (Item)
			{
				CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
				const WCHAR *pStr = Content->String.operator const WCHAR*();
				if (pStr)
				{
					WCHAR Buffer[512];
					if (IsColorString(pStr))
					{
						TStrCpyLimit(Buffer, ((unsigned long*)pStr)+1, 512);
						pStr = Buffer;
					}

					int l = TStrLen(pStr);
					WISP_RECT Rect;
					GetItemRect(Item, nCol, &Rect);
		#define IS_KEYWORD(c) (IS_DIGIT(c) || IS_LETTER(c) || (c)=='_')
					int Ext = m_ClientDC.GetTextLen(pStr, m_pWispBase->m_MousePT.x - m_ScrClientRect.x - Rect.x);

					if (Ext != -1 && IS_KEYWORD(pStr[Ext]))
					{
						int found = 0;
						int i = 0;
						for (i = 0; i < l; ++i)
						{
							if (!IS_KEYWORD(pStr[i]))
							{
								if (i>Ext) break;
								found = i+1;
							}
						}
						int klen = i - found;
						TStrNCpy(m_szKeyword, &Buffer[found], klen);
						m_szKeyword[klen]=0;
						UpdateDasm();
						return; //???
					}
				}
			}
		}

		if (Len)
		{
			m_szKeyword[0] = 0;
			UpdateDasm();
		}
	}

	void CCodeView::OnKeyRButton(WISP_MSG *pMsg)
	{
		if (Point2Index(&pMsg->m_LogicMousePosWnd, &m_nRButtonCol))
		{
			if (m_nRButtonCol >= 1 && m_nRButtonCol <= 4096)
			{
				m_pRButtonItem = FindItem(0, 8);
				if (m_pRButtonItem)
				{
					ULONG_PTR Addr = GetItemValue(m_pRButtonItem, CV_ADDR);

					bool bEnableEdit = GetItemContent(m_pRButtonItem, CV_CODE)->String.m_nLength != 0;
					m_PopupMenu.Enable(EVENT_ID_EDIT_CODE, bEnableEdit);
					m_PopupMenu.Enable(EVENT_ID_EDIT_CMT, bEnableEdit);

					m_PopupMenu.Enable(EVENT_ID_PREVPOS, m_HistoryIter != m_History.Begin());
					m_PopupMenu.Enable(EVENT_ID_NEXTPOS, m_HistoryIter != m_History.End()-1);
					m_PopupMenu.Enable(EVENT_ID_CLRPOS, m_History.Size() > 1);

					m_PopupMenu.Checked(EVENT_ID_CV_SWITCH_SYM, m_pSyserCodeDoc->m_Style & 1);

					m_PopupMenu.Checked(EVENT_ID_CV_FULL_SYM, gSyserConfig.iShowFullSym != 0);

					m_PopupMenu.Checked(EVENT_ID_CV_ADDR_MODE, m_Mode == 0);
					m_PopupMenu.Checked(EVENT_ID_CV_OFFSET_MODE, m_Mode == 1);
					m_PopupMenu.Checked(EVENT_ID_REL_CIP, m_Mode == 2);

					m_PopupMenu.Checked(EVENT_ID_FOLLOW_CIP_AUTO, gSyserConfig.iFollowMode == 0);
					m_PopupMenu.Checked(EVENT_ID_FOLLOW_CIP_SYSEXPLORER, gSyserConfig.iFollowMode == 1);
					m_PopupMenu.Checked(EVENT_ID_FOLLOW_CIP_SRCEXPLORER, gSyserConfig.iFollowMode == 2);

					m_PopupMenu.Point(0, 1);
				}
			}
		}
	}

	bool CCodeView::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (!pMsg->KeyEvent.bKeyDown) return true;

		map<unsigned int, unsigned int>::IT it = m_KeyMap.find(pMsg->KeyEvent.KeyType);
		if (it != m_KeyMap.end())
		{
			WISP_MSG Msg;
			Msg.Command.CmdID = it->second;
			return OnEvent(&Msg);
		}
		int Size;
		switch (pMsg->KeyEvent.KeyType)
		{
		case VK_LBUTTON:
			OnKeyLButton(pMsg); //Keyword
			break;
		case VK_RBUTTON:
			OnKeyRButton(pMsg); //Menu
			break;
		case 0x53: //S
			gpSyser->m_MainFrame.m_SystemExplorer.m_ViewSplitWnd.GetSize(1, &Size);
			Size += 10;
			gpSyser->m_MainFrame.m_SystemExplorer.m_ViewSplitWnd.SetSize(1, Size);
			gpSyser->m_MainFrame.m_SystemExplorer.m_ViewSplitWnd.AdjustWndPos();
			return false;
			break;
		case 0x57: //W
			gpSyser->m_MainFrame.m_SystemExplorer.m_ViewSplitWnd.GetSize(1, &Size);
			if (Size <= 10)
				Size = 0;
			else	Size -= 10;
			gpSyser->m_MainFrame.m_SystemExplorer.m_ViewSplitWnd.SetSize(1, Size);
			gpSyser->m_MainFrame.m_SystemExplorer.m_ViewSplitWnd.AdjustWndPos();
			return false;
			break;
		}
		return true;
	}

	bool CCodeView::OnEventShowAt(WISP_MSG *pMsg)
	{
		if (m_pSyserCodeDoc && m_pSyserCodeDoc->m_bOpen)
		{
			CListStringItem *Item = GetNext(0, 8);
			if (Item)
			{
				ULONG_PTR Addr = GetItemValue(Item, CV_ADDR);
				ULONG_PTR NewAddr;
				ULONG_PTR NewImm;
				ULONG_PTR Mem;
				if (m_pSyserCodeDoc->GetAddr(Addr, &NewAddr))
				{
					if (m_HistoryIter != m_History.End()-1)
					{
						m_HistoryIter++;
						m_History.Remove(m_HistoryIter, m_History.End());
						m_HistoryIter = m_History.End();
					}
					GotoAddr(NewAddr, true);
				} else
				if (m_pSyserCodeDoc->GetAddrImm(Addr, &NewAddr, &NewImm))
				{
					if (NewImm != -1)
					{
						CDataView *pWnd = (CDataView*)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveWnd();
						pWnd->ViewAddress(NewImm, true);
					} else
					if (NewAddr != -1 && m_pSyserCodeDoc->ReadMemory(NewAddr, &Mem, sizeof(Mem)) == sizeof(Mem))
					{
						CDataView *pWnd = (CDataView*)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveWnd();
						pWnd->ViewAddress(NewAddr, true);
					}
				}
			}
		}
		return true;
	}

	bool CCodeView::OnMouseDblClick(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.KeyType == VK_LBUTTON)
		{
			CListStringItem *Item = GetItem(0, 0);
			WISP_RECT Rect;
			GetItemRect(Item, CV_INST, &Rect);
			if (pMsg->MsgMouseWndPT.x >= Rect.x && pMsg->MsgMouseWndPT.x <= Rect.x + Rect.cx)
			{
                        	OnEventShowAt(pMsg);
			}
		}
		return true;
	}
	bool CCodeView::OnSize(WISP_MSG *pMsg)
	{
		SetScrollBarInfo(0, m_nVSBPage * m_nScrollLine, m_nVSBPage * (2 * m_nScrollLine + 1), m_nVSBPage, 0, 0);
		UpdateDasm();
		return true;
	}
	bool CCodeView::OnUpdateClient(WISP_MSG *pMsg)
	{
		CWispList::OnUpdateClient(pMsg);
		
		WISP_RECT Rect;
		Rect.cx = 16;
		Rect.cy = 16;
		Rect.x = 2;
		Rect.y = m_nLayoutHeight;

		CListStringItem *Item = GetRootItem();
		while (Item)
		{
			unsigned char pic = GetItemValue(Item, CV_START);
			if (pic & 8)
				m_ClientDC.DrawDIB(&Rect, m_pLineMarkDIBList->GetDIB(1));
			//??? else
			if (pic & 1)
				m_ClientDC.DrawDIB(&Rect, m_pLineMarkDIBList->GetDIB(0));
			//??? else
			if (pic & 2)
				m_ClientDC.DrawDIB(&Rect, m_pLineMarkDIBList->GetDIB(2));
			else
			if (pic & 4)
				m_ClientDC.DrawDIB(&Rect, m_pLineMarkDIBList->GetDIB(4));
			Rect.y += m_nLineHeight;
			Item = GetNext(Item, 0);
		}

		ULONG_PTR Addr;
		Item = FindItem(0, 8);
		if (Item) Addr = GetItemValue(Item, CV_ADDR);
		else Addr = 0;

		int LineJmp = -1;
		int CurLineJmp = -1;

		for (int i = 0; i < m_LineJmpCount; ++i)
		{
			if (i != m_CurLineJmp)
			{	
				if (Addr && Addr == m_LineDIB[i].eip)
					CurLineJmp = i;
				else	DrawLineJmp(&m_LineDIB[i], i, m_Color[COLOR_JMP_LINE]);
				LineJmp = CurLineJmp;
			}
		}
		if (LineJmp >= 0)
			DrawLineJmp(&m_LineDIB[LineJmp], LineJmp, gSyserColors.Color[7]); //FOCUS

		if (m_CurLineJmp >= 0 && m_CurLineJmp < m_LineJmpCount)
			DrawLineJmp(&m_LineDIB[m_CurLineJmp], m_CurLineJmp, m_Color[COLOR_ACTIVE_JMP_LINE]);
		return false;
	}
	bool CCodeView::OnChar(WISP_MSG *pMsg)
	{
		if (!IS_LOWER(pMsg->Char.Char))
			return true;

		gpSyser->m_MainFrame.m_SystemExplorer.m_TerminalWnd.Focus();
		WISP_MSG Msg;
		memcpy(&Msg, pMsg, sizeof(Msg));
		Msg.hWnd = &gpSyser->m_MainFrame.m_SystemExplorer.m_TerminalWnd;
		m_pWispBase->SendMessage(&Msg);
		return false;
	}
	bool CCodeView::OnGetFocus(WISP_MSG *pMsg)
	{
		if (!FindItem(0, 8))
		{
			CListStringItem *Item = FindItem(0, 0);
			if (Item)
				Select(Item);
		}
		return true;
	}
	bool CCodeView::OnScrollEvent(WISP_MSG *pMsg)
	{
		if (pMsg->ScrollEvent.Type == WISP_SCROLL_BAR_VERT)
		{
			ULONG_PTR Addr;
			switch (pMsg->ScrollEvent.Event)
			{
			case WISP_SB_INC:
				Addr = m_pSyserCodeDoc->GetInstrAddress(m_CurAddr, 1);
			#if 0
				GotoAddr(Addr, false);
			#else
				if (m_CurAddr < Addr && Addr < (m_pSyserCodeDoc->m_EndAddr-m_nVSBPage+1))
					GotoAddr(Addr, false);
				else	GotoAddr(m_pSyserCodeDoc->m_EndAddr-m_nVSBPage+1, false);
			#endif
				SetScrollBarCurPos(0, m_nVSBPage * m_nScrollLine, 1);
				break;
			case WISP_SB_DEC:
				Addr = m_pSyserCodeDoc->GetInstrAddress(m_CurAddr, -1);
			#if 0
				GotoAddr(Addr, false);
			#else
				if (m_CurAddr > Addr && Addr > m_pSyserCodeDoc->m_BeginAddr)
					GotoAddr(Addr, false);
				else	GotoAddr(m_pSyserCodeDoc->m_BeginAddr, false);
			#endif
				SetScrollBarCurPos(0, m_nVSBPage * m_nScrollLine, 1);
				break;
			case WISP_SB_PAGE_UP:
				Addr = m_pSyserCodeDoc->GetInstrAddress(m_CurAddr, 1 - m_nVSBPage);
			#if 0
				GotoAddr(Addr, false);
			#else
				if (m_CurAddr > Addr && Addr > m_pSyserCodeDoc->m_BeginAddr)
					GotoAddr(Addr, false);
				else	GotoAddr(m_pSyserCodeDoc->m_BeginAddr, false);
			#endif
				SetScrollBarCurPos(0, m_nVSBPage * m_nScrollLine, 1);
				break;
			case WISP_SB_PAGE_DOWN:
				Addr = m_pSyserCodeDoc->GetInstrAddress(m_CurAddr, m_nVSBPage - 1);
			#if 0
				GotoAddr(Addr, false);
			#else
				if (m_CurAddr < Addr && Addr < (m_pSyserCodeDoc->m_EndAddr-m_nVSBPage+1))
					GotoAddr(Addr, false);
				else	GotoAddr(m_pSyserCodeDoc->m_EndAddr-m_nVSBPage+1, false);
			#endif
				SetScrollBarCurPos(0, m_nVSBPage * m_nScrollLine, 1);
				break;
			case WISP_SB_POS_CHANGE:
				Addr = m_pSyserCodeDoc->GetInstrAddress(m_CurAddr, pMsg->ScrollEvent.Delta);
				if (pMsg->ScrollEvent.Delta < 0)
				{
				#if 0
					GotoAddr(Addr, false);
				#else
					if (m_CurAddr > Addr && Addr > m_pSyserCodeDoc->m_BeginAddr)
						GotoAddr(Addr, false);
					else	GotoAddr(m_pSyserCodeDoc->m_BeginAddr, false);
				#endif
				} else
				if (pMsg->ScrollEvent.Delta > 0)
				{
				#if 0
					GotoAddr(Addr, false);
				#else
					if (m_CurAddr < Addr && Addr < (m_pSyserCodeDoc->m_EndAddr-m_nVSBPage+1))
						GotoAddr(Addr, false);
					else	GotoAddr(m_pSyserCodeDoc->m_EndAddr-m_nVSBPage+1, false);
				#endif
				}
				//GotoAddr(Addr, false);
				break;
			default:
				SetScrollBarCurPos(0, m_nVSBPage * m_nScrollLine, 1);
				break;
			}
		}
		return false;
	}

	bool CCodeView::OnCmdFindString(WISP_MSG *pMsg)
	{
		//TODO
		return true;
	}
	bool CCodeView::OnCmdVSpinScroll(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000002)
			SetScrollBarCurPos(0, m_nVSBPage * m_nScrollLine, 0); //m_nItemCount
		return true;
	}

	bool CCodeView::OnEventResetPos(WISP_MSG *pMsg)
	{
		ResetPos();
		return true;
	}
	bool CCodeView::OnEventSavePos(WISP_MSG *pMsg)
	{
		SavePos();
		return true;
	}
	bool CCodeView::OnEventNextPos(WISP_MSG *pMsg)
	{
		NextPos();
		return true;
	}
	bool CCodeView::OnEventPrevPos(WISP_MSG *pMsg)
	{
		PrevPos();
		return true;
	}

	bool CCodeView::OnEventSearchOpRef(WISP_MSG *pMsg)
	{
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		{
			CListStringItem *Item = FindItem(0, 8);
			if (Item)
			{
				ULONG_PTR Addr = GetItemValue(Item, CV_ADDR);
				OpenOpRefList(Addr);
			}
		}
		return true;
	}
	bool CCodeView::OnEventSearchTrRef(WISP_MSG *pMsg)
	{
		if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		{
			CListStringItem *Item = FindItem(0, 8);
			if (Item)
			{
				ULONG_PTR Addr = GetItemValue(Item, CV_ADDR);
				OpenTRefList(Addr);
			}
		}
		return true;
	}

	bool CCodeView::GetOpRefList(ULONG_PTR Address, TList<ULONG_PTR> & Addrs, ULONG_PTR & Return)
	{
		if (!m_pSyserCodeDoc)
			return false;

		if (!m_pSyserCodeDoc->m_bOpen)
			return false;

		ULONG_PTR Ret;
		if (!m_pSyserCodeDoc->GetAddrValue(Address, &Ret))
			return false;

		Return = Ret;
		CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(Address);
		if (!pDbgMod)
		{
	#ifdef _RING0_
			pDbgMod = gpSyser->m_pSysDebugger->GetModule(Address);
			if (!pDbgMod)
	#endif
				return false;
		}

		if (Ret < pDbgMod->m_ImageBase)
			return false;

		if (Ret >= pDbgMod->m_ImageBase + pDbgMod->m_ImageSize)
			return false;

	gpSyser->m_MainFrame.CreateProgress(WSTR(""), pDbgMod->m_ImageSize-3);
	unsigned long Progress = (pDbgMod->m_ImageSize-3)/100;

		for (unsigned long i = 0; i < pDbgMod->m_ImageSize-3; ++i)
		{
			ULONG_PTR Op;
			ULONG_PTR Offset = pDbgMod->m_ImageBase+i;

			if (m_pSyserCodeDoc->ReadMemory(Offset, &Op, sizeof(Op)) == sizeof(Op) && Op == Ret)
			{
				Addrs.Append(&Offset);
			}

	if (Progress && !(i%Progress))
	gpSyser->m_MainFrame.UpdateProgress(i);

		}
	gpSyser->m_MainFrame.DestroyProgress();
		return Addrs.Size();
	}

	bool CCodeView::OpenOpRefList(ULONG_PTR Address)
	{
		TList<ULONG_PTR> Addrs;
		if (GetOpRefList(Address, Addrs, Address))
		{
			if (gpSyser->m_MainFrame.m_CrossReferenceForm.IsWindow())
				gpSyser->m_MainFrame.m_CrossReferenceForm.Show(WISP_SH_MINSIZE);
			else	gpSyser->m_MainFrame.m_CrossReferenceForm.Create(0, WISP_SH_MINSIZE);
			gpSyser->m_MainFrame.m_CrossReferenceForm.OpenOpRefList(Addrs, Address);
		} else
		{
			gpSyser->m_MainFrame.m_SystemExplorer.m_TerminalWnd.Printf(WSTR("Can't find Operand Reference\n"));
		}
		return true;
	}

	unsigned long CCodeView::GetTRefList(ULONG_PTR Address, TList<ULONG_PTR> & Addrs)
	{
		CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(Address);
		if (!pDbgMod)
		{
			if (gpSyser->m_pSysDebugger != gpSyser->m_pDebugger)
				pDbgMod = gpSyser->m_pSysDebugger->m_DbgModuleList.FindModule(Address);
		}

		if (!pDbgMod) return 0;
		
		unsigned long CodeBufSize = 4096;
		unsigned char *pCodeBuf = new unsigned char[CodeBufSize+16];
		if (!pCodeBuf) return 0;

		//far jump
		ULONG_PTR CurAddr = pDbgMod->m_ImageBase;
		unsigned long n = 0;
		while (n < pDbgMod->m_ImageSize)
		{
			unsigned long remain = pDbgMod->m_ImageSize - n;
			MAX_LIMIT(CodeBufSize, remain);
			unsigned long result = gpSyser->m_pDebugger->ReadMemory(CurAddr, pCodeBuf, CodeBufSize);
			if (result == CodeBufSize)
			{
				unsigned long Size = CurAddr - (ULONG_PTR)pCodeBuf;
				unsigned long i = 0;
				while (i < (result-5))
				{
					//CInstrSym::GetJump
					if ((pCodeBuf[i] == 0xE9 || pCodeBuf[i] == 0xE8) &&
						Address == (i + *(unsigned long*)&pCodeBuf[i+1] + CurAddr + 5)) //??? long
					{
						ULONG_PTR p = (ULONG_PTR)pCodeBuf+n+Size;
						Addrs.Append(&p);
					} else
					if (pCodeBuf[i] == 0xF && (pCodeBuf[i+1] >= 0x80 && pCodeBuf[i+1] <= 0x8F) &&
						Address == (i + *(unsigned long*)&pCodeBuf[i+2] + CurAddr + 6)) //??? long
					{
						ULONG_PTR p = (ULONG_PTR)pCodeBuf+n+Size;
						Addrs.Append(&p);
					}
					++i;
				}

				unsigned long subi = i;
				while (subi < result)
				{
					if (pCodeBuf[subi] == 0xE9) break;
					if (pCodeBuf[subi] == 0xF) break;
					++subi;
				}

				if (subi != result)
				{
					*(unsigned long*)&pCodeBuf[CodeBufSize] = 0;
					unsigned long RSize = remain - CodeBufSize;
					MAX_LIMIT(RSize, 5);
					unsigned long newresult = gpSyser->m_pDebugger->ReadMemory(CurAddr + CodeBufSize, &pCodeBuf[CodeBufSize], RSize);
					if (newresult == RSize)
					{
						while (i < result)
						{
							//CInstrSym::GetJump
							if ((pCodeBuf[i] == 0xE9 || pCodeBuf[i] == 0xE8) &&
								Address == (i + *(unsigned long*)&pCodeBuf[i+1] + CurAddr + 5)) //??? long
							{
								ULONG_PTR p = (ULONG_PTR)pCodeBuf+n+Size;
								Addrs.Append(&p);
							} else
							if (pCodeBuf[i] == 0xF && (pCodeBuf[i+1] >= 0x80 && pCodeBuf[i+1] <= 0x8F) &&
								Address == (i + *(unsigned long*)&pCodeBuf[i+2] + CurAddr + 6)) //??? long
							{
								ULONG_PTR p = (ULONG_PTR)pCodeBuf+n+Size;
								Addrs.Append(&p);
							}
							++i;
						}
					}
				}
			}
			CurAddr += CodeBufSize;
			n += CodeBufSize;
		}

		//near jump
		CurAddr = Address - 0x80;
		if (gpSyser->m_pDebugger->ReadMemory(CurAddr, pCodeBuf, 256) == 256)
		{
			unsigned long Size = CurAddr - (ULONG_PTR)pCodeBuf;
			for (unsigned long n = 0; n < 255; ++n)
			{
				//CInstrSym::GetJump
				if ((pCodeBuf[n] == 0xEB || (pCodeBuf[n] >= 0x70 && pCodeBuf[n] <= 0x7F)) &&
				    (Address == n + (char)pCodeBuf[n+1] + CurAddr + 2))
				{
					ULONG_PTR p = (ULONG_PTR)pCodeBuf+n+Size;
					Addrs.Append(&p);
				}
			}
		}
		delete []pCodeBuf;
		
		return Addrs.Size();
	}

	bool CCodeView::OpenTRefList(ULONG_PTR Address)
	{
		TList<ULONG_PTR> Addrs;
		if (GetTRefList(Address, Addrs))
		{
			if (gpSyser->m_MainFrame.m_CrossReferenceForm.IsWindow())
				gpSyser->m_MainFrame.m_CrossReferenceForm.Show(WISP_SH_MINSIZE);
			else	gpSyser->m_MainFrame.m_CrossReferenceForm.Create(0, WISP_SH_MINSIZE);
			gpSyser->m_MainFrame.m_CrossReferenceForm.OpenTRefList(Addrs, Address);
		} else
		{
			gpSyser->m_MainFrame.m_SystemExplorer.m_TerminalWnd.Printf(WSTR("Can't find Transfer Reference\n"));
		}
		return true;
	}

	bool CCodeView::OnEventToggleBM(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.ToggleBM();
		return false;
	}

	bool CCodeView::OnEventFollowAuto(WISP_MSG *pMsg)
	{
		gSyserConfig.iFollowMode = 0;
		return true;
	}
	bool CCodeView::OnEventFollowCodeView(WISP_MSG *pMsg)
	{
		gSyserConfig.iFollowMode = 1;
		return true;
	}
	bool CCodeView::OnEventFollowSourceView(WISP_MSG *pMsg)
	{
		gSyserConfig.iFollowMode = 2;
		return true;
	}

	bool CCodeView::OnEventAnalyze(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FindItem(0, 8);
		if (Item)
		{
			ULONG_PTR Addr = GetItemValue(Item, CV_ADDR);
			CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(Addr);
			if (pDbgMod)
			{
				//TODO gpSyser->m_SyserUI.m_SymAnalyzer.???(pDbgMod, Addr); //pDbgMod->+30); //pDbgMod->m_ImageBase
				//TODO gpSyser->m_MainFrame.m_SystemExplorer.m_SymAnalyzer.???(pDbgMod, Addr); //pDbgMod->+30);
				UpdateDasm();
			}
		}
		return true;
	}

	bool CCodeView::OnEventModeAddr(WISP_MSG *pMsg)
	{
		m_Mode = 0;
		UpdateDasm();
		return true;
	}

	bool CCodeView::OnEventModeOffs(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FindItem(0, 8);
		if (Item)
		{
			m_RelAddr = GetItemValue(Item, CV_ADDR);
			m_Mode = 1;
			UpdateDasm();
		}
		return true;
	}

	bool CCodeView::OnEventSymSwitch(WISP_MSG *pMsg)
	{
		if (m_pSyserCodeDoc->m_Style & 1)
			m_pSyserCodeDoc->m_Style &= ~1;
		else	m_pSyserCodeDoc->m_Style |= 1;
		UpdateDasm();
		return true;
	}
	bool CCodeView::OnEventFullSym(WISP_MSG *pMsg)
	{
		gSyserConfig.iShowFullSym = gSyserConfig.iShowFullSym == 0;
		UpdateDasm();
		return true;
	}

	bool CCodeView::OnEventModeRelCIP(WISP_MSG *pMsg)
	{
		m_Mode = 2;
		UpdateDasm();
		return true;
	}

	bool CCodeView::OnEventEditCode(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FindItem(0, 8);
		if (Item) EditItem(Item, CV_INST);
		return true;
	}
	bool CCodeView::OnEventEditComment(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FindItem(0, 8);
		if (Item) EditItem(Item, CV_CMT);
		return true;
	}

	bool CCodeView::OnEventStrRef(WISP_MSG *pMsg)
	{
		if (gpSyser->m_MainFrame.m_FindListWnd.IsWindow())
			gpSyser->m_MainFrame.m_FindListWnd.Destroy();
		gpSyser->m_MainFrame.m_FindListWnd.CreateWnd(WSTR("String Reference"), 0,0,564,344, 0, EVENT_ID_STRFIND, 0x300BB, WISP_SH_MINSIZE);
		return true;
	}

	bool CCodeView::OnEventStrFind(WISP_MSG *pMsg)
	{
		//???
		return true;
	}

	bool CCodeView::OnEventNextCIPTrace(WISP_MSG *pMsg)
	{
		CTracePoint *pTrace = gpSyser->m_MainFrame.m_SystemExplorer.NextCIP();
		if (!pTrace) return false;
		GotoAddr(pTrace->Address, false);
		SelectAddr(pTrace->Address + pTrace->Offset);
		return true;
	}
	bool CCodeView::OnEventPrevCIPTrace(WISP_MSG *pMsg)
	{
		CTracePoint *pTrace = gpSyser->m_MainFrame.m_SystemExplorer.PrevCIP();
		if (!pTrace) return false;
		GotoAddr(pTrace->Address, false);
		SelectAddr(pTrace->Address + pTrace->Offset);
		return true;
	}

	bool CCodeView::OnEventNextAddr(WISP_MSG *pMsg)
	{
		GotoAddr(m_CurAddr + 1, false);
		return false;
	}

	bool CCodeView::OnEventPrevAddr(WISP_MSG *pMsg)
	{
		GotoAddr(m_CurAddr - 1, false);
		return false;
	}

	bool CCodeView::OnEventNewView(WISP_MSG *pMsg)
	{
		CWispWnd *pWnd = (CWispWnd *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetWnd(pMsg->Command.CmdID - EVENT_ID_CV0);
		if (pWnd)
		{
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.ChangeActiveWnd(pWnd);
			return true;
		}

		if (!gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.InsertView(
				(CCodeView*)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd()))
			return false;
		return true;
	}

	bool CCodeView::OnEventGotoHere(WISP_MSG *pMsg)
	{
		if (gpSyser->m_pDebugger && gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		{
			CListStringItem *Item = FindItem(0, 8);
			if (Item)
			{
				ULONG_PTR Addr = GetItemValue(Item, CV_ADDR);
				gpSyser->m_pDebugger->SetCodeBP(Addr, 0x200, BP_STATE_ENABLE);
				gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("x"), 0);
			}
		}
		return true;
	}

	bool CCodeView::OnEventToggleAddBP(WISP_MSG *pMsg)
	{
		if (!gpSyser->m_pDebugger || !gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
			return true;

		CListStringItem *Item = FindItem(0, 8);
		if (Item)
		{
			ULONG_PTR Addr = GetItemValue(Item, CV_ADDR);
			gpSyser->m_SyserUI.ToggleInsertBP(Addr);
		//???
			gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
		}
		return false;

	}
	bool CCodeView::OnEventToggleEnableBP(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FindItem(0, 8);
		if (Item)
		{
			ULONG_PTR Addr = GetItemValue(Item, CV_ADDR);
			gpSyser->m_SyserUI.ToggleEnableBP(Addr);
		//???
			UpdateDasm();
			gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
		}
		return false;
	}
	bool CCodeView::OnEventCopy(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FindItem(0, 8);
		if (Item == m_pRButtonItem && Item && m_nRButtonCol >= CV_ADDR && m_nRButtonCol <= CV_CMT)
		{
			CListStringItem::CONTENT *Content = GetItemContent(m_pRButtonItem, m_nRButtonCol);
			if (IsColorString(Content->String.m_pData))
			{
				WCHAR Buffer[256];
				TStrCpyLimit(Buffer, ((unsigned long*)Content->String.m_pData)+1, 256); //???
				m_pWispBase->SetClipboard(Buffer);
			} else
			if (Content->String.m_nLength)
			{
				m_pWispBase->SetClipboard(Content->String.m_pData);
			}
		}
		return true;
	}

	void CCodeView::OnLButton(CListStringItem *Item, int nCol)
	{
		if (nCol == CV_START)
		{
			OnEventToggleAddBP(0);
		} else
		if (nCol == CV_ADDR)
		{
			if (m_Mode == 0)
			{
				OnEventModeOffs(0);
			} else
			if (m_Mode == 1)
			{
				OnEventModeRelCIP(0);
			} else
			if (m_Mode == 2)
			{
				OnEventModeAddr(0);
			}
			UpdateDasm();
		}
	}
	void CCodeView::OnRButton(CListStringItem *Item, int nCol)
	{
		if (nCol == CV_START)
		{
			OnEventToggleBM(0);
		}
	}
	bool CCodeView::Edit(CListStringItem *Item, int nCol, TWideString& String)
	{
		if (m_pSyserCodeDoc && m_pSyserCodeDoc->m_bOpen)
		{
			if (nCol == CV_INST)
			{
				m_EditWnd.SetWindowText(WSTR(" "));
			} else
			if (nCol == CV_CMT)
			{
				if (IsColorString(String.operator const WCHAR*()))
					return false;
			}
			return true;
		}
		return false;
	}

	bool CCodeView::OnEditing(CListStringItem *Item, int nCol, TWideString& String)
	{
		if (nCol == CV_ADDR)
		{
			ULONG_PTR Result;
			CListStringItem::CONTENT *Content = GetItemContent(Item, CV_ADDR);
			if (Content->String != String &&
				gpSyser->m_SyserUI.CalcExp(String.operator const WCHAR*(), &Result))
				GotoAddr(Result, true);
			
			return false;
		} else
		if (nCol == CV_INST)
		{
			return EditAsm(Item, String);
		} else
		if (nCol == CV_CMT)
		{
			ULONG_PTR Addr = GetItemValue(Item, CV_ADDR);
			m_pSyserCodeDoc->SetComment(Addr, String.operator const WCHAR*());
			UpdateDasm();
		}
		return true;
	}

	bool CCodeView::EditAsm(CListStringItem *Item, TWideString& String)
	{
		ULONG_PTR Addr = GetItemValue(Item, CV_ADDR);
		char szInput[256];
		TStrCpyLimit(szInput, String.operator const WCHAR*(), 256);
		unsigned char CodeBuff[128];
	#ifdef _X64_
		unsigned long Size = CIntelDisassembler::Assembler(szInput, CodeBuff, Addr, CODE_MODE_64);
	#else
		unsigned long Size = CIntelDisassembler::Assembler(szInput, CodeBuff, Addr, CODE_MODE_32);
	#endif
		if (Size)
		{
			gpSyser->m_pDebugger->WriteMemory(Addr, CodeBuff, Size);
			UpdateDasm();
		}
		return false;
	}

	unsigned long *CCodeView::OptItemToCS(_OPERAND_ITEM *OpArray, int nOp, unsigned long *pColorString, _INSTRUCTION_INFORMATION *pDasmInstr)
	{
		WCHAR szBuffer[32];

		for (int n = 0; n < nOp; ++n)
		{
			_OPERAND_ITEM *pOp = &OpArray[n];
			if (pOp->mode == OP_Invalid) break;
			if (n > 0)
				pColorString += ColorStrCpy(pColorString, ",", COLOR_OPTR, 0);
			if (pOp->mode == OP_Address)
			{
				if (pOp->opersize != -1)
				{
					pColorString += ColorStrCpy(pColorString, CInstrSym::m_SizeSym[pOp->opersize], COLOR_KEYWORD, 0);
					pColorString += ColorStrCpy(pColorString, " PTR ", COLOR_KEYWORD, 0);
				}

				if (pDasmInstr->SegmentPrefix != -1)
				{
					pColorString += ColorStrCpy(pColorString, CInstrSym::m_SegStr[pDasmInstr->SegmentPrefix], COLOR_SEG_REG, 0);
					pColorString += ColorStrCpy(pColorString, ":", COLOR_OPTR, 0);
				}

				pColorString += ColorStrCpy(pColorString, "[", COLOR_OPTR, 0);
				if (pOp->addr.base != -1)
					pColorString += ColorStrCpy(pColorString, CInstrSym::m_RegStr[pOp->addr.address_size][pOp->addr.base], COLOR_REGISTER, 0);

				if (pOp->addr.index != -1)
				{
					if (pOp->addr.base != -1)
						pColorString += ColorStrCpy(pColorString, "+", COLOR_OPTR, 0);
					pColorString += ColorStrCpy(pColorString, CInstrSym::m_RegStr[pOp->addr.address_size][pOp->addr.index], COLOR_REGISTER, 0);
					if (pOp->addr.scale >= 1)
					{
						pColorString += ColorStrCpy(pColorString, "*", COLOR_OPTR, 0);
						pColorString += ColorStrCpy(pColorString, CInstrSym::m_ScaleStr[pOp->addr.scale], COLOR_OPTR, 0);
					}
				}

				if (pOp->addr.displacement || (pOp->addr.base == -1 && pOp->addr.index == -1))
				{
					if (pOp->addr.index != -1 || pOp->addr.base != -1)
					{
						if (GetSign(&pOp->addr))
						{
							pColorString += ColorStrCpy(pColorString, "+", COLOR_OPTR, 0);
							TSPrintf(szBuffer, CInstrSym::m_SizePtr[GetSize(pOp->addr.displacement)], pOp->addr.displacement);
						} else
						{
							pColorString += ColorStrCpy(pColorString, "-", COLOR_OPTR, 0);
							TSPrintf(szBuffer, CInstrSym::m_SizePtr[GetSize(pOp->addr.displacement)], -pOp->addr.displacement);
						}
					} else
					{
						TSPrintf(szBuffer, CInstrSym::m_SizePtr[GetSize(pOp->addr.displacement)], pOp->addr.displacement);
					}
					pColorString += ColorStrCpy(pColorString, szBuffer, COLOR_IMMED, 0);
				}
				pColorString += ColorStrCpy(pColorString, "]", COLOR_OPTR, 0);
			} else
			{
				switch (pOp->mode)
				{
				case OP_Register:
					pColorString += ColorStrCpy(pColorString, pOp->string, COLOR_REGISTER, 0);
					break;
				case OP_Segment:
					pColorString += ColorStrCpy(pColorString, pOp->string, COLOR_SEG_REG, 0);
					break;
				case OP_Immed:
				case OP_Near:
					pColorString += ColorStrCpy(pColorString, pOp->string, COLOR_IMMED, 0);
					break;
				case OP_Far:
					TSPrintf(szBuffer, WSTR("%04X"), pOp->farptr.segment);
					pColorString += ColorStrCpy(pColorString, szBuffer, COLOR_IMMED, 0);
					pColorString += ColorStrCpy(pColorString, WSTR(":"), COLOR_OPTR, 0);
					TSPrintf(szBuffer, CInstrSym::m_ScaleStr[pOp->opersize+2], pOp->farptr.offset); //???
					pColorString += ColorStrCpy(pColorString, szBuffer, COLOR_IMMED, 0);
					break;
				case OP_Datadup:
					if (n == 0)
					for (n = 0; n < pOp->datadup.count; ++n)
					{
						if (n > 0)
							pColorString += ColorStrCpy(pColorString, ",", COLOR_OPTR, 0);
						TSPrintf(szBuffer, WSTR("%02X"), pOp->datadup.buffer[n]);
						pColorString += ColorStrCpy(pColorString, szBuffer, COLOR_IMMED, 0);
					}
					break;
				case OP_Symbol:
					if (pOp->symbol.boffset)
						pColorString += ColorStrCpy(pColorString, "OFFSET ", COLOR_KEYWORD, 0);
					pColorString += ColorStrCpy(pColorString, pOp->symbol.string, COLOR_SYMBOL, 0);
					break;
				case OP_Align:
					if (n == 0)
					{
						TSPrintf(szBuffer, WSTR("%X"), pOp->align.nBytes);
						pColorString += ColorStrCpy(pColorString, szBuffer, COLOR_IMMED, 0);
					}
					break;
				default: break;
				}
			}
		}
		return pColorString;
	}

	unsigned long *CCodeView::InstrToCS(_INSTRUCTION_INFORMATION *pDasmInstr, unsigned long *pColorString)
	{
		bool bFirstLine = true;
		if (pDasmInstr->Lock != -1)
		{
			ColorStrCpy(pColorString, pDasmInstr->LockName, COLOR_PREFIX, 0);
			ColorStrCat(pColorString, " ", 0, 0);
			pColorString += TStrFillTail(pColorString, m_pSyserCodeDoc->m_UxAlign, (unsigned long)32);
			bFirstLine = false;
		}
		if (pDasmInstr->Repeat != -1)
		{
			ColorStrCpy(pColorString, pDasmInstr->RepeatName, COLOR_PREFIX, 0);
			ColorStrCat(pColorString, " ", 0, 0);
			if (bFirstLine)
			{
				pColorString += TStrFillTail(pColorString, m_pSyserCodeDoc->m_UxAlign, (unsigned long)32);
				pColorString += TStrLen(pColorString);
				bFirstLine = false;
			} else
			{
				pColorString += TStrLen(pColorString);
				pColorString += TStrLen(pColorString);
			}

		}
		ColorStrCpy(pColorString, pDasmInstr->Name, COLOR_OPCODE, 0);
		ColorStrCat(pColorString, " ", 0, 0);
		if (pDasmInstr->Name[0] && bFirstLine)
			pColorString += TStrFillTail(pColorString, m_pSyserCodeDoc->m_UxAlign, (unsigned long)32);
		else	pColorString += TStrLen(pColorString);
		return OptItemToCS(pDasmInstr->op, 3, pColorString, pDasmInstr);
	}

	unsigned long CCodeView::ColorDasm(ULONG_PTR Address, unsigned long MaxSize, unsigned long *pColorString, unsigned long nCodeAlign, _INSTRUCTION_INFORMATION *pInstInfo, unsigned char *pCodeBuff)
	{
		unsigned char CodeBuff[64];
		if (!pCodeBuff)
			pCodeBuff = CodeBuff;

		_INSTRUCTION_INFORMATION InstInfo;
		InstInfo.CodeBuff = pCodeBuff;
		InstInfo.eip = Address;
		InstInfo.pasm = nullptr;
		unsigned long CodeLen = m_pSyserCodeDoc->Dasm(&InstInfo, 0, MaxSize);
		if (nCodeAlign)
		{
			if (InstInfo.OpCode == -1)
			{
				ColorStrCpy(pColorString, WSTR("??"), COLOR_IMMED, 0);
			} else
			{
				WCHAR Buffer[64];
				THexBytesToStr(InstInfo.CodeBuff, Buffer, CodeLen, 1, (WCHAR*)0);
				ColorStrCpy(pColorString, Buffer, COLOR_IMMED, 0);
			}
			pColorString += TStrFillTail(pColorString, nCodeAlign, (unsigned long)32);
		}
		InstrToCS(&InstInfo, pColorString);
		if (pInstInfo)
		{
			memcpy(pInstInfo, &InstInfo, sizeof(_INSTRUCTION_INFORMATION));
			if (!pCodeBuff)
				pInstInfo->CodeBuff = 0;
		}
		return CodeLen;
	}

	void CCodeView::GetAddress(ULONG_PTR Address, WCHAR *szAddress)
	{
		if (m_Mode == 0)
		{
			TSPrintf(szAddress, WSTR("" F0ADDR "X"), Address);
		} else
		{
			WCHAR Buffer[32];
			LONG_PTR Offs;
			if (m_Mode == 1)
			{
				Offs = Address - m_RelAddr;
				TStrCpy(Buffer, "$");
			} else
			if (m_Mode == 2)
			{
				Offs = Address - (gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP?*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP:0);
				TStrCpy(Buffer, CIPSTR);
			}

			if (Offs > 0) TSPrintf(szAddress, WSTR("%s+%X"), Buffer, Offs);
			else
			if (Offs < 0) TSPrintf(szAddress, WSTR("%s-%X"), Buffer, -Offs);
			else
			{
				TStrCpy(szAddress, Buffer);
				TStrCat(szAddress, "==>");
			}
		}
	}

	int TStrCount(const char *Str, char Char)
	{
		int Count = 0;
		if (Str) while (*Str) { if (*Str == Char) ++Count; ++Str; }
		return Count;
	}

	void CCodeView::ShowAPICall(ULONG_PTR Address, const char *string, CListStringItem *Item, unsigned int nLine)
	{
		map<TAnsiNoCaseString, APIFunction>::IT it = gpSyser->m_SyserUI.m_APIDef.find(string);
		if (it == gpSyser->m_SyserUI.m_APIDef.end())
			return;

		int Len = 0;
		WCHAR szString[256];
		unsigned long ColorString[1024];

		Len += ColorStrCpy(ColorString, it->second.Return.operator const char*(), COLOR_PARAM_TYPE, COLOR_BG);
		Len += ColorStrCpy(&ColorString[Len], " ", COLOR_BG, COLOR_BG);

		Len += ColorStrCpy(&ColorString[Len], it->first.operator const char*(), COLOR_API_NAME, COLOR_BG);
		Len += ColorStrCpy(&ColorString[Len], "(", COLOR_OPTR, COLOR_BG);
	{
		TListIter<TAnsiString> It = it->second.Arguments.Begin();
		while (It != it->second.Arguments.End())
		{
			const char *pStrOptional = 0;
			const char *pStrParam = TStrRChr(It->operator const char*(), ' ');
			if (pStrParam)
			{
				if (!TStrCmp(pStrParam+1, "OPTIONAL"))
				{
					pStrOptional = pStrParam;
					pStrParam = TStrRChr(It->operator const char*(), pStrParam-1, ' ');
					if (!pStrParam)
						pStrParam = It->operator const char*();
				}

				int Pos = TStrCount(It->operator const char*(), ' ');
				if (Pos == 1)
				{
					if (!TStrNCmp(It->operator const char*(), "IN ", 3))
						pStrParam = 0;
					else
					if (!TStrNCmp(It->operator const char*(), "OUT ", 4))
						pStrParam = 0;
					
				} else
				if (Pos == 2)
				{
					if (!TStrNCmp(It->operator const char*(), "IN OUT ", 7))
						pStrParam = 0;
				}

				if (pStrParam)	
				{
					int l = pStrParam - It->operator const char*()+1;
					TStrCpyLimit(szString, It->operator const char*(), l);
					Len += ColorStrCpy(ColorString, szString, COLOR_PARAM_TYPE, COLOR_BG);
					if (pStrOptional)
						TStrCpyLimit(szString, pStrParam, pStrOptional - pStrParam+1);
					else	TStrCpy(szString, pStrParam);
					Len += ColorStrCpy(&ColorString[Len], szString, COLOR_PARAM_NAME, COLOR_BG);
					if (pStrOptional)
					{
						TStrCpy(szString, pStrOptional);
						Len += ColorStrCpy(&ColorString[Len], szString, COLOR_PARAM_TYPE, COLOR_BG);
					}
				}
			}

			if (!pStrParam)
			{
				Len += ColorStrCpy(ColorString, It->operator const char*(), COLOR_PARAM_TYPE, COLOR_BG);
			}


			if (It != it->second.Arguments.End()-1)
				Len += ColorStrCpy(&ColorString[Len], ",", COLOR_OPTR, COLOR_BG);
			++It;
		}
	}
		ColorStrCpy(&ColorString[Len], ")", COLOR_OPTR, COLOR_BG);
		SetItemTextColor(Item, CV_CMT, ColorString);

		if (it->second.Declaration!=0) return;
		
		TListIter<TAnsiString> It = it->second.Arguments.Begin();
		while (It != it->second.Arguments.End() && nLine>0)
		{
			--nLine;
			Item = GetPrev(Item, 0);
			if (!Item) return;

			CListStringItem::CONTENT *Content = GetItemContent(Item, CV_CMT);
			if (!IsColorString(Content->String.m_pData))
			{
				Content = GetItemContent(Item, CV_INST);
				if (Content->String.m_nLength > 0)
				{
					unsigned long long CodeBuf = GetItemValue(Item, CV_CODE);
					if (CInstrSym::GetJump((unsigned char*)&CodeBuf, 0, nullptr)) return;

					if (!TStrNICmp((WCHAR*)Content->String.m_pData+2, "PUSH", 4))
					{
						ULONG_PTR Addr = GetItemValue(Item, CV_ADDR);
						const char *pStrOptional = 0;
						const char *pStrParam = TStrRChr(It->operator const char*(), ' ');
						if (pStrParam)
						{
							if (!TStrCmp(pStrParam+1, "OPTIONAL"))
							{
								pStrOptional = pStrParam;
								pStrParam = TStrRChr(It->operator const char*(), pStrParam-1, ' ');
								if (!pStrParam)
									pStrParam = It->operator const char*();
							}

							int Pos = TStrCount(It->operator const char*(), ' ');
							if (Pos == 1)
							{
								if (!TStrNCmp(It->operator const char*(), "IN ", 3))
									pStrParam = 0;
								else
								if (!TStrNCmp(It->operator const char*(), "OUT ", 4))
									pStrParam = 0;
								
							} else
							if (Pos == 2)
							{
								if (!TStrNCmp(It->operator const char*(), "IN OUT ", 7))
									pStrParam = 0;
							}

							if (pStrParam)	
							{
								int l = pStrParam - It->operator const char*()+1;
								TStrCpyLimit(szString, It->operator const char*(), l);
								Len = ColorStrCpy(ColorString, szString, COLOR_PARAM_TYPE, COLOR_BG);
								if (pStrOptional)
									TStrCpyLimit(szString, pStrParam, pStrOptional - pStrParam+1);
								else	TStrCpy(szString, pStrParam);
								Len += ColorStrCpy(&ColorString[Len], szString, COLOR_PARAM_NAME, COLOR_BG);
								if (pStrOptional)
								{
									TStrCpy(szString, pStrOptional);
									ColorStrCpy(&ColorString[Len], szString, COLOR_PARAM_TYPE, COLOR_BG);
								}
							}
						}

						if (!pStrParam)
						{
							ColorStrCpy(ColorString, It->operator const char*(), COLOR_PARAM_TYPE, COLOR_BG);
						}
						SetItemTextColor(Item, CV_CMT, ColorString);
						++It;
					}
				}
			}
		}
		
	}

	bool CCodeView::UpdateDasm()
	{
		bool bContext = false;
		Adjust(0, m_nVSBPage); //_m_nItemCount);

		ULONG_PTR CurAddr = m_CurAddr;
		CListStringItem *Item = GetNext(0, 0);

		_INSTRUCTION_INFORMATION InstInfo;
		memset(&InstInfo, 0, sizeof(InstInfo));

		unsigned char CodeBuff[MAX_INSTR_LEN];
		memset(CodeBuff, 0xFF, sizeof(CodeBuff));

		unsigned long InstrColorString[256];

		int KeywordLen = TStrLen(m_szKeyword);

		unsigned int nLine = 0;
		while (nLine < m_nVSBPage && Item)
		{
			InstInfo.CodeBuff = CodeBuff;
			InstrColorString[0] = 0;
			unsigned long CodeLen = ColorDasm(CurAddr, sizeof(CodeBuff), InstrColorString, 0, &InstInfo, CodeBuff);
			ULONG_PTR JmpAddr = 0;
			if (CodeLen >= 2)
				CInstrSym::GetJump(CodeBuff, CurAddr, &JmpAddr);

			WCHAR szString[256];
			TStrCpy(szString, InstrColorString);

			if (KeywordLen>0)
			{
				WCHAR *pStr = szString;
			//	while ((pStr = TStrIStr(pStr, m_szKeyword)))
				while ((pStr = TStrIStrX(pStr, WSTR("[],+- "), m_szKeyword))) //:
				{
					int start = pStr - szString;
					for (int i = 0; i < KeywordLen; ++i)
					{
						InstrColorString[start+i] = (InstrColorString[start+i] & 0xFFFFFF) + 0xA000000;
					}
					pStr += KeywordLen;
				}
				
			}

			SetItemValue(Item, CV_ADDR, CurAddr);
			if (!bContext && m_pSyserCodeDoc->GetFunction(CurAddr, szString, 256))
			{
				SetItemValue(Item, CV_START, 0);
	                        SetItemValue(Item, CV_INST, 0);
	                        SetItemText(Item, CV_START, WSTR(""));
	                        SetItemText(Item, CV_ADDR, WSTR(""));
	                        SetItemText(Item, CV_CODE, WSTR(""));
	                        SetItemColor(Item, CV_INST, m_Color[COLOR_SYMBOL]);
	                        SetItemText(Item, CV_INST, szString);
	                        SetItemText(Item, CV_CMT, WSTR(""));
	                        bContext = true;
			} else
			{
				SetItemValue(Item, CV_START, m_pSyserCodeDoc->GetPic(CurAddr));

				SetItemColor(Item, CV_ADDR, m_Color[COLOR_ADDRESS]);
				GetAddress(CurAddr, szString);
				SetItemText(Item, CV_ADDR, szString);

				unsigned long ColorCode[120];

				if (CodeLen < 1 || InstInfo.OpCode == -1)
				{
					THexBytesToStr(CodeBuff, szString, 1, 1, (WCHAR*)0);
					ColorStrCpy(ColorCode, szString, COLOR_PARAM_TYPE, COLOR_BG);
				} else
				{
					int l = 0;
					bool bUnknown = false;
					for (int i = 0; i < CodeLen; ++i)
					{
						THexBytesToStr(&CodeBuff[i], szString, 1, 1, (WCHAR*)0);

						if (i < InstInfo.Prefixes)
						{
							l += ColorStrCpy(&ColorCode[l], szString, COLOR_PREFIX_BYTE, COLOR_BG);
						} else
						if (i < InstInfo.Prefixes + InstInfo.OpCount)
						{
							if (i == InstInfo.Prefixes && i)
								l += ColorStrCpy(&ColorCode[l], ":", COLOR_OPCODE_BYTE, COLOR_BG);
							l += ColorStrCpy(&ColorCode[l], szString, COLOR_OPCODE_BYTE, COLOR_BG);
						} else
						if (InstInfo.RegRM != -1 &&
							InstInfo.Prefixes + InstInfo.OpCount == i)
						{
							l += ColorStrCpy(&ColorCode[l], " ", COLOR_REGRM_BYTE, COLOR_BG);
							l += ColorStrCpy(&ColorCode[l], szString, COLOR_REGRM_BYTE, COLOR_BG);
						} else
						if (InstInfo.Sib1 != -1 &&
							InstInfo.Prefixes + InstInfo.OpCount+1 == i)
						{
							l += ColorStrCpy(&ColorCode[l], " ", COLOR_SIB_BYTE, COLOR_BG);
							l += ColorStrCpy(&ColorCode[l], szString, COLOR_SIB_BYTE, COLOR_BG);
						} else
						if (InstInfo.AMD3DNow != -1 && i == CodeLen - 1)
						{
							l += ColorStrCpy(&ColorCode[l], " ", COLOR_AMD3DNOW_BYTE, COLOR_BG);
							l += ColorStrCpy(&ColorCode[l], szString, COLOR_AMD3DNOW_BYTE, COLOR_BG);
						} else
						{
							if (!bUnknown)
							{
								l += ColorStrCpy(&ColorCode[l], " ", COLOR_OTHER_OPCODE_BYTE, COLOR_BG);
								bUnknown = true;
							}
							l += ColorStrCpy(&ColorCode[l], szString, COLOR_OTHER_OPCODE_BYTE, COLOR_BG);
						}
						
					}
				}

				SetItemTextColor(Item, CV_CODE, ColorCode);
				SetItemValue(Item, CV_CODE, *(unsigned long long*)CodeBuff);

				SetItemTextColor(Item, CV_INST, InstrColorString);
				SetItemValue(Item, CV_INST, JmpAddr);

				szString[0] = 0;

				SetItemColor(Item, CV_CMT, m_Color[COLOR_COMMENT]);
				SetItemText(Item, CV_CMT, szString);

				ULONG_PTR ADDR1,ADDR2;
				char Encode;

				if (m_pSyserCodeDoc->GetComment(CurAddr, szString, 256))
				{
					SetItemText(Item, CV_CMT, szString);
				} else
				if (!m_pSyserCodeDoc->GetAddrImm(CurAddr, &ADDR1, &ADDR2))
				{
					InstrColorString[0] = 0;
					if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen &&
						CurAddr == *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP)
					{
						m_pSyserCodeDoc->ContextCS(InstrColorString, 256);
						SetItemTextColor(Item, CV_CMT, InstrColorString);
					}
				} else
				if ((ADDR2 != -1 && gpSyser->m_SyserUI.ReadString(ADDR2, szString, 256, &Encode)) ||
				    (ADDR1 != -1 && gpSyser->m_SyserUI.ReadString(ADDR1, szString, 256, &Encode)) )
				{
					SetItemText(Item, CV_CMT, szString);
				}

				bContext = false;

				if (!TStrNICmp(InstrColorString, "CALL", 4) && InstInfo.op[0].mode == OP_Symbol)
					ShowAPICall(CurAddr, InstInfo.op[0].symbol.string, Item, nLine);

				m_EndAddr = CurAddr;
				CurAddr = m_pSyserCodeDoc->GetInstrAddress(CurAddr, 1);
			}
                        Item = GetNext(Item, 0);
			++nLine;
		}

		AddLineJmp();
		Update((WISP_RECT*)0);
		return true;
	}

	void CCodeView::UpdateStaticText()
	{
		WCHAR Buffer[512];
		if (gpSyser->m_pDebugger == 0)
		{
		//	TSPrintf(Buffer, WSTR("Debugger: %s not found"), gSyserConfig.szDebuggerFileName);
		//	gpSyser->m_MainFrame.m_StaticText.SetWindowText(Buffer);
			return;
		}
	#ifdef _RING0_
		int Len = TStrCpy(Buffer, gpSyser->m_pDebugger->m_ProcessName.operator const WCHAR*());
	#else
		int Len = AnsiToUnicode(TGetFileName(gpSyser->m_SyserUI.m_SyserCodeDoc.m_ExeFilePath.operator const char*()), Buffer, lenof(Buffer));
		if (Len==0) Buffer[0] = 0;
	#endif
		CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(m_CurAddr);
		if (!pDbgMod)
		{
			if (gpSyser->m_pDebugger != gpSyser->m_pSysDebugger)
				pDbgMod = gpSyser->m_pSysDebugger->m_DbgModuleList.FindModule(m_CurAddr);
		}
		if (pDbgMod)
		{
			Len += TStrCpy(&Buffer[Len], WSTR(" : "));
			Len += AnsiToUnicode(pDbgMod->m_FileTitle.operator const char*(), &Buffer[Len], lenof(Buffer) - Len);
			char szSym[256];
			if (gpSyser->m_SyserUI.GetSymbolOffset(m_CurAddr, szSym))
			{
				Len += TStrCpy(&Buffer[Len], WSTR("!"));
				Len += TStrCpy(&Buffer[Len], szSym);
			} else
			{
				TSPrintf(&Buffer[Len], WSTR("+%X"), m_CurAddr - pDbgMod->m_ImageBase);
			}
		}
		gpSyser->m_MainFrame.m_StaticText.SetWindowText(Buffer);
	}

	void CCodeView::GotoAddr(ULONG_PTR Address, bool bHistory)
	{
		ULONG_PTR PrevAddr = m_CurAddr;
		if (m_CurAddr != Address)
		{
			m_CurAddr = Address;
			unsigned int Index = 0;
			TListIter<WISP_TAB_WND_ITEM> It = gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.m_WndList.Begin();
			while (It != gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.m_WndList.End())
			{
				if (It->pWnd == this)
				{
					WCHAR Buffer[64];
					TSPrintf(Buffer, WSTR("" F0ADDR "X"), m_CurAddr);
					gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.SetTabText(Index, Buffer);
				}
				++It;
				++Index;
			}
			if (gpSyser->m_MainFrame.m_SystemExplorer.m_pCodeView == this)
			{
				UpdateStaticText();
			}
			if (m_CurAddr && bHistory)
			{
				AddPos(m_CurAddr, PrevAddr);
			}
			UpdateDasm();
			Update((WISP_RECT*)0);
		}
	}

	void CCodeView::SelectAddress(ULONG_PTR Address)
	{
		ULONG_PTR Addr = m_pSyserCodeDoc->GetInstrAddress(Address, m_nVSBPage/-2); //m_nItemCount
		GotoAddr(Addr, false);
		SelectAddr(Address);
	}

	void CCodeView::SelectAddr(ULONG_PTR Address)
	{
		CListStringItem *Item = GetRootItem();
		while (Item)
		{
			ULONG_PTR Addr = GetItemValue(Item, CV_ADDR);
			if (Addr == Address)
			{
				Select(Item);
				return;
			}
			Item = GetNext(Item, 0);
		}
	}

	void CCodeView::LoadKeyMap()
	{
		if (!gpSyser->GetHotKey(EVENT_ID_CV0, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000031, EVENT_ID_CV0)); //WISP_SK_ALT 0
		if (!gpSyser->GetHotKey(EVENT_ID_CV1, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000032, EVENT_ID_CV1)); //WISP_SK_ALT 1
		if (!gpSyser->GetHotKey(EVENT_ID_CV2, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000033, EVENT_ID_CV2)); //WISP_SK_ALT 2
		if (!gpSyser->GetHotKey(EVENT_ID_CV3, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000034, EVENT_ID_CV3)); //WISP_SK_ALT 3
		if (!gpSyser->GetHotKey(EVENT_ID_CV4, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000035, EVENT_ID_CV4)); //WISP_SK_ALT 4
		if (!gpSyser->GetHotKey(EVENT_ID_CV5, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000036, EVENT_ID_CV5)); //WISP_SK_ALT 5
		if (!gpSyser->GetHotKey(EVENT_ID_CV6, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000037, EVENT_ID_CV6)); //WISP_SK_ALT 6
		if (!gpSyser->GetHotKey(EVENT_ID_CV7, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000038, EVENT_ID_CV7)); //WISP_SK_ALT 7
		if (!gpSyser->GetHotKey(EVENT_ID_CV8, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x20000039, EVENT_ID_CV8)); //WISP_SK_ALT 8

		if (!gpSyser->GetHotKey(EVENT_ID_CV_SWITCH_SYM, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x9,        EVENT_ID_CV_SWITCH_SYM)); //VK_TAB
		if (!gpSyser->GetHotKey(EVENT_ID_CV_FULL_SYM, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x40000009, EVENT_ID_CV_FULL_SYM)); //WISP_SK_CTRL VK_TAB

		if (!gpSyser->GetHotKey(EVENT_ID_CV_NEXT_CIP, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0xBB,       EVENT_ID_CV_NEXT_CIP)); //VK_OEM_PLUS
		if (!gpSyser->GetHotKey(EVENT_ID_CV_PREV_CIP, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0xBD,       EVENT_ID_CV_PREV_CIP)); //VK_OEM_MINUS

		if (!gpSyser->GetHotKey(EVENT_ID_PREVPOS, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x8,        EVENT_ID_PREVPOS)); //VK_BACK
		if (!gpSyser->GetHotKey(EVENT_ID_NEXTPOS, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x4000000D, EVENT_ID_NEXTPOS)); //WISP_SK_CTRL VK_RETURN

		if (!gpSyser->GetHotKey(EVENT_ID_CV_SHOW_AT, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0xD,        EVENT_ID_CV_SHOW_AT)); //VK_RETURN
		if (!gpSyser->GetHotKey(EVENT_ID_SAVEPOS, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x1000000D, EVENT_ID_SAVEPOS)); //WISP_SK_SHIFT VK_RETURN
		if (!gpSyser->GetHotKey(EVENT_ID_CV_PREV, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x40000026, EVENT_ID_CV_PREV)); //WISP_SK_CTRL VK_UP
		if (!gpSyser->GetHotKey(EVENT_ID_CV_NEXT, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x40000028, EVENT_ID_CV_NEXT)); //WISP_SK_CTRL VK_DOWN
		if (!gpSyser->GetHotKey(EVENT_ID_INSREMBP, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x78,      EVENT_ID_INSREMBP)); //VK_F9
		if (!gpSyser->GetHotKey(EVENT_ID_GOTO_HERE, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x76,       EVENT_ID_GOTO_HERE)); //VK_F7
	}

	void CCodeView::GetCodeRange(ULONG_PTR *pStart, ULONG_PTR *pEnd)
	{
		*pStart = m_CurAddr;
		*pEnd = m_pSyserCodeDoc->GetInstrAddress(m_CurAddr, m_nVSBPage - 1); //m_nItemCount
	}

	bool CCodeView::GetJumpPos(ULONG_PTR Address, unsigned long *pJmpPos)
	{
		if (Address < m_CurAddr || Address > m_EndAddr)
			return false;
		unsigned long Pos = 0;
		CListStringItem *Item = GetRootItem();
		while (Item)
		{
			ULONG_PTR Addr = GetItemValue(Item, CV_ADDR);
			if (Address == Addr)
			{
				if (pJmpPos) *pJmpPos = Pos;
				return true;
			}
			Item = GetNext(Item, 0);
			++Pos;
		}
		return false;
	}

	void CCodeView::AddLineJmp()
	{
		m_LineJmpCount = 0;
		m_CurLineJmp = -1;
		int Index = 0;
		CListStringItem *Item = GetRootItem();
		while (Item && m_LineJmpCount < MAX_LINEDIB)
		{
			ULONG_PTR Inst = GetItemValue(Item, CV_INST);
			unsigned long JmpPos;
			if (Inst && GetJumpPos(Inst, &JmpPos))
			{
				m_LineDIB[m_LineJmpCount].eip = GetItemValue(Item, CV_ADDR);
				m_LineDIB[m_LineJmpCount].index = Index;
				m_LineDIB[m_LineJmpCount].inst = Inst;
				m_LineDIB[m_LineJmpCount].offs = JmpPos;
				if (m_LineDIB[m_LineJmpCount].eip == *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP)
				{
					unsigned long long Code = GetItemValue(Item, CV_CODE);
					if (CInstrSym::CheckJump((unsigned char*)&Code, *(unsigned long*)gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pEFL)) //??? TODO
						m_CurLineJmp = m_LineJmpCount;
				}
				++m_LineJmpCount;
			}
			Item = GetNext(Item, 0);
			++Index;
		}
	}

	void CCodeView::DrawLineJmp(LINEDIB *LineDIB, int Pos, unsigned long Color)
	{
		int X1 = Pos * (m_LineJmpWidth / m_LineJmpCount);
		int X2 = m_LineJmpWidth;

		int Y1 = m_nLineHeight/2 + m_nLineHeight * LineDIB->index;
		int Y2 = m_nLineHeight/2 + m_nLineHeight * LineDIB->offs;

		m_ClientDC.DrawHLine(X1, X2, Y1, Color);
		m_ClientDC.DrawVLine(X2, Y1 - 3, Y1 + 3, Color);
		m_ClientDC.DrawHLine(X1, X2, Y2, Color);
		m_ClientDC.DrawLine(X2 - 2, Y2 - 2, X2, Y2, Color);
		m_ClientDC.DrawLine(X2 - 2, Y2 + 2, X2, Y2, Color);
		m_ClientDC.DrawVLine(X1, Y1, Y2, Color);
	}

	void CCodeView::AddPos(ULONG_PTR Addr, ULONG_PTR PrevAddr)
	{
		if (m_History.Size()>100)
		{
			if (m_HistoryIter == m_History.Begin())
				m_HistoryIter++;
			m_History.Remove(m_History.Begin());
		}

		if (m_History.Size()==0 && Addr != PrevAddr)
			m_HistoryIter = m_History.Append(&PrevAddr);

		if (Addr != *(m_History.End()-1)) //???
			m_HistoryIter = m_History.Append(&Addr);

		gpSyser->m_MainFrame.m_SystemExplorer.UpdateMenu();
	}

	void CCodeView::ResetPos()
	{
		m_History.Clear();
		m_HistoryIter = m_History.Append(&m_CurAddr);
		gpSyser->m_MainFrame.m_SystemExplorer.UpdateMenu();
	}
	void CCodeView::SavePos()
	{
		if (m_HistoryIter != m_History.End()-1)
		{
			m_HistoryIter++;
			m_History.Remove(m_HistoryIter, m_History.End());
			m_HistoryIter = m_History.End();
		}
		if (m_History.Size()>100)
		{
			if (m_HistoryIter == m_History.Begin())
				m_HistoryIter++;
			m_History.Remove(m_History.Begin());
		}
		if (!m_History.Size() || m_CurAddr != *(m_History.End()-1))
			m_HistoryIter = m_History.Append(&m_CurAddr);

		gpSyser->m_MainFrame.m_SystemExplorer.UpdateMenu();
	}
	void CCodeView::NextPos()
	{
		if (m_pSyserCodeDoc && m_pSyserCodeDoc->m_bOpen)
		{
			if (m_HistoryIter == m_History.End() || *m_HistoryIter == m_CurAddr)
			{
				if (m_HistoryIter != m_History.End()-1)
				{
					m_HistoryIter++;
					GotoAddr(*m_HistoryIter, false);
				}
			} else
			{
				GotoAddr(*m_HistoryIter, false);
			}
			gpSyser->m_MainFrame.m_SystemExplorer.UpdateMenu();			
		}
	}
	void CCodeView::PrevPos()
	{
		if (m_pSyserCodeDoc && m_pSyserCodeDoc->m_bOpen)
		{
			if (m_HistoryIter == m_History.End() || *m_HistoryIter == m_CurAddr)
			{
				if (m_HistoryIter != m_History.Begin())
				{
					m_HistoryIter--;
					GotoAddr(*m_HistoryIter, false);
				}
			} else
			{
				GotoAddr(*m_HistoryIter, false);
			}

			gpSyser->m_MainFrame.m_SystemExplorer.UpdateMenu();			
		}
	}

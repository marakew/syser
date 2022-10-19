
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "sourcecodewnd.hpp"
#include "syserconfig.hpp"
#include "resource.hpp"
#include "strinputwnd.hpp"

//#include<vector>
//#include<string>

	extern const char *teststr;

	extern char *yytext;
	extern int yyleng;
	extern void yy_set_input(const char *str);
	extern int yylex();
	extern void yyrestart(FILE*);

WISP_MENU_GROUP_ITEM SourceCodePopupMenu[] =
{
	{WSTR("Insert/Remove  Breakpoint         F9"), EVENT_ID_INSREMBP, 0xF3, 0, 0},
	{WSTR("Enable/Disable Breakpoint"), EVENT_ID_ENDISMBP, 0xF4, 0, 0},
	{WSTR("Toggle BookMark                   Ctrl+K"), EVENT_ID_TOGGLE_BM, 0xD0, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Go To Line                        Ctrl+G"), EVENT_ID_SOURCECODEWND_GOTOLINE, 0x10B, 0, 0},
	{WSTR("Go To Here                        F7"), EVENT_ID_GOTO_HERE, 0x10D, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Add Watch"), EVENT_ID_SOURCECODEWND_ADDWATCH, 0x10C, 0, 0},
	{0, 0, 0, 4, 0},
	{WSTR("Expansion All"), EVENT_ID_SOURCECODEWND_EXPANSION, 0x109, 0, 0},
	{WSTR("Collapse  All"), EVENT_ID_SOURCECODEWND_COLLAPSE, 0x10A, 0, 0},
	{0},
};

	WISP_MSG_MAP_BEGIN(CSourceCodeWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

	
	WISP_EVENT_MAP_BEGIN(CSourceCodeWnd)
		WISP_EVENT_MAP(EVENT_ID_SOURCECODEWND_EXPANSION, OnEventExpansion)
		WISP_EVENT_MAP(EVENT_ID_SOURCECODEWND_COLLAPSE, OnEventCollapse)
		WISP_EVENT_MAP(EVENT_ID_SOURCECODEWND_GOTOLINE, OnEventGotoLine)
		WISP_EVENT_MAP(EVENT_ID_INSREMBP, OnEventInsDelBP)
		WISP_EVENT_MAP(EVENT_ID_ENDISMBP, OnEventEnDisBP)

		WISP_EVENT_MAP(EVENT_ID_GOTO_HERE, OnEventGotoHere)
		WISP_EVENT_MAP(EVENT_ID_SHOW_HERE, OnEventShowHere)

		WISP_EVENT_MAP(EVENT_ID_SOURCECODEWND_ADDWATCH, OnEventAddWatch)

		WISP_EVENT_MAP(EVENT_ID_TOGGLE_BM, OnEventToggleBM) //0x64
	//	WISP_EVENT_MAP(EVENT_ID_NEXT_BM, OnEventNextBM)
	//	WISP_EVENT_MAP(EVENT_ID_PREV_BM, OnEventPrevBM)
	//	WISP_EVENT_MAP(EVENT_ID_CLEAR_BM, OnEventClearBM)

	WISP_EVENT_MAP_END

	CSourceCodeWnd::CSourceCodeWnd()
	{
		m_CurrentFileID = 0;
		m_FileOpen = false;
		m_TxtFileLineNumber = 0;
		m_bComment = false;
		m_CurAddr = 0;
	}
	CSourceCodeWnd::~CSourceCodeWnd()
	{
	}


	bool CSourceCodeWnd::OnCreate(WISP_MSG *pMsg)
	{
		m_Style |= 0x2000000;
		InsertColumn(WSTR("Mark"), 40, 0, 0, -1);
		InsertColumn(WSTR("Source Code"), 512, 0, 0, -1);
		m_nCtrlCol = 1;
		Recalc(false);
		m_PopupMenu.LoadPopupMenuGroup(SourceCodePopupMenu, this, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF));

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

		m_Color[27] = gSyserColors.Color[37];  // 0xFFFFFF,   //Keyword
		m_Color[28] = gSyserColors.Color[38];  // 0xC0C0C0,   //Identifier
		m_Color[29] = gSyserColors.Color[39];  // 0x4080FF,   //Preprocessor Keyword
		m_Color[30] = gSyserColors.Color[40];  // 0x808080,   //Comment block
		m_Color[31] = gSyserColors.Color[41];  // 0x808080,   //Comment line
		m_Color[32] = gSyserColors.Color[42];  // 0xFFFF00,   //String
		m_Color[33] = gSyserColors.Color[43];  // 0x00FF80,   //Numerical
		m_Color[34] = gSyserColors.Color[44];  // 0xFFFFFF,   //Operation
		m_Color[35] = gSyserColors.Color[45];  // 0xE17D7D,   //Class name
		m_Color[36] = gSyserColors.Color[46];  // 0xE17D7D,   //Struct name
		m_Color[37] = gSyserColors.Color[47];  // 0xE17D7D,   //Union name
		m_Color[38] = gSyserColors.Color[48];  // 0xE17D7D,   //Typedef name
		m_Color[39] = gSyserColors.Color[49];  // 0xFF8000,   //Function name
		m_Color[40] = gSyserColors.Color[50];  // 0x7B7B7B,   //Unknown

		m_ClientDC.m_pColor = m_Color;
		m_ColorFocus = gSyserColors.Color[35];
		m_ColorUnFocus = gSyserColors.Color[36];
		m_crBGColor = gSyserColors.Color[34];
		m_bBGColor = true;
		m_ItemColor = gSyserColors.Color[43];
		m_pLineMarkDIBList = gpCurWisp->m_DIBLib.LoadDIBList("\\SyserApp\\LineMark.bmp", 16, 16, 0, 0xFF00FF);
		m_pLineMarkDIBList->SetColorKey(0);
		return true;
	}
	bool CSourceCodeWnd::OnDestroy(WISP_MSG *pMsg)
	{
		if (m_FileOpen)
			DeleteAllChildrenItems(0);
		m_PopupMenu.Destroy();
		return true;
	}

	bool CSourceCodeWnd::IsAddWatch()
	{
		m_WatchVar.Empty();
		m_WatchVarComplex.Empty();
		TListIter<CListStringItem> It;
		Point2Iter(&m_LogicMousePosWnd, It);
		if (!It) return true; //???
		CListStringItem *Item = &*It;

		WCHAR OutText[512];
		TStrCpy(OutText, "");

		WISP_RECT Rect;
		GetItemRect(Item, 1, &Rect);
		if (m_LogicMousePosWnd.x < Rect.x) return false;
		if (m_LogicMousePosWnd.x > (Rect.x + Rect.cx)) return false;

		unsigned long LineNumber = GetItemValue(Item, 1);
		unsigned int MaxLen = m_LogicMousePosWnd.x - m_nSpaceWidth - Rect.x;
		CListStringItem::CONTENT *Content = GetItemContent(Item, 1);
		const WCHAR *str = Content->String.operator const WCHAR*();
		unsigned int nLen = m_ClientDC.GetTextLen(str, MaxLen);
		if (nLen == -1) return false;

		char szString[512];
		TStrCpy(szString, ((unsigned long *)str)+1); //???

		yy_set_input(szString);

		int sol = 0;
		int eol = 0;

		yyrestart(0);

		for (;;)
		{
			int Lex = yylex();
		#if 1
			switch (Lex)
			{
			case 0:
				return true;
			case 660:
				break;
			case 679:
				break;
			case 705:
				if (sol == 0)
					sol = eol;
				break;
			default:
				sol = 0;
				break;
			}
		#else
			if (Lex == 0) return true;
			if (Lex != 660) // . L_PERIOD
			{
				if (Lex == 679) // -> L_POINTSTO
				{
				} else
				if (Lex == 705) // string
				{
					if (sol == 0)
						sol = eol;
				} else
				{
					sol = 0;
				}
			}
		#endif
			if (nLen >= eol && nLen < yyleng + eol)
			{
				if (Lex != 705) return false;
				break;
			}
			eol += yyleng;
		}

		TAnsiString String;

		if (sol != eol)
		{
			szString[eol + yyleng] = 0;
			bool bFunc = false;
			unsigned long offset = 0;
			void *ptr = m_pSDSMod->GetGlobal(&szString[sol], m_CurrentFileID, LineNumber, &bFunc, &offset);
			if (!ptr) return false;

			m_WatchVarComplex = &szString[sol];
			String.Empty();
			if (!bFunc)
			{
				DATATYPE *data = (DATATYPE*)ptr;
				char Output[512];
				int OutputLen = 512;
				memset(Output, 0, 512);
				String = m_pSDSMod->GetOutput(data->TypeID, Output, &OutputLen);
				String += " ";
				if (data->ClassID)
				{
					BASICTYPE *udt = m_pSDSMod->GetGlobal(data->ClassID);
					if (udt && (udt->Elem.Type == ELEM_STRUCT ||
						    udt->Elem.Type == ELEM_CLASS ||
						    udt->Elem.Type == ELEM_UNION))
					{
						String += m_pSDSMod->GetOutput(data->ClassID, Output, &OutputLen);
						String += "::";
					}
				}
				String += data->name;
				TStrCpy(OutText, String.operator const char*());
				m_WatchVar = yytext;
				return true;
			}
		} else
		{
		#if 1 //+++
			if (GetOutput(yytext, Item, String))
			{
				TStrCpy(OutText, String.operator const char*());
				m_WatchVar = yytext;
				return true;
			}
		#endif
			if (m_pSDSMod->GetGlobal(yytext, true))
			{
				m_WatchVar = yytext;
				return true;
			}
		#if 0 //+++			
			if (GetOutput(yytext, Item, String))
			{
				TStrCpy(OutText, String.operator const char*());
				m_WatchVar = yytext;
				return true;
			}
		#endif
		}
		return false;
	}

	void CSourceCodeWnd::OnPopupMenu(WISP_MSG *pMsg)
	{
		if (IsAddWatch())
			m_PopupMenu.EnableItem(EVENT_ID_SOURCECODEWND_ADDWATCH);
		else	m_PopupMenu.DisableItem(EVENT_ID_SOURCECODEWND_ADDWATCH);
		m_PopupMenu.Point(0, 1);
	}
	bool CSourceCodeWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.bKeyDown)
		{
			switch (pMsg->KeyEvent.KeyType)
			{
			case VK_RBUTTON:{
				m_LogicMousePosWnd = pMsg->MsgMouseWndPT;
				m_LogicMousePosWnd = pMsg->m_LogicMousePosWnd;
				int Index;
				Point2Index(&pMsg->m_LogicMousePosWnd, &Index);
				if (Index > 0 && Index < 4096)
					OnPopupMenu(pMsg);
				//TODO ???
				} break;
			case VK_F7:
				OnEventGotoLine(0);
				break;
			case VK_F9:
				OnEventInsDelBP(0);
				break;
			}
		}
		return true;
	}

	bool CSourceCodeWnd::OnUpdateClient(WISP_MSG *pMsg)
	{
		CWispList::OnUpdateClient(pMsg);
		int Index = 0;
		WISP_RECT Rect;
		Rect.x = 0;
		if (m_Columns.Size())
			Rect.x = m_Columns.Begin()->nRealWidth - 20;
		Rect.y = m_nLayoutHeight;
		Rect.cx = 16;
		Rect.cy = 16;
		if (m_RootIter != m_RootItem.End() && m_nVSBPage > 0)
		{
		TListIter<CListStringItem> It = m_RootIter;
		while (Index < m_nVSBPage)
		{
			CListStringItem *Item = &*It;
			unsigned long pic = GetItemValue(Item, 0);
			if (pic & 8) m_ClientDC.DrawDIB(&Rect, m_pLineMarkDIBList->m_DIBList + 1);
			if (pic & 1) m_ClientDC.DrawDIB(&Rect, m_pLineMarkDIBList->m_DIBList + 0);
			if (pic & 2) m_ClientDC.DrawDIB(&Rect, m_pLineMarkDIBList->m_DIBList + 2);
			else
			if (pic & 4) m_ClientDC.DrawDIB(&Rect, m_pLineMarkDIBList->m_DIBList + 3);
			Rect.y += m_nLineHeight;
			if (!NextOpen(It))
				break;
			++Index;
		}
		}
		return false;
	}

	bool CSourceCodeWnd::OnEventExpansion(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetItem(0, 0);
		while (Item)
		{
			if (GetItem(0, Item))
				Expand(Item);
			Item = GetNext(Item, 0);
		}
		Update((WISP_RECT*)0);
		return true;
	}
	bool CSourceCodeWnd::OnEventCollapse(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetItem(0, 0);
		CListStringItem *Parent = GetItemParent(GetRootItem());
		if (GetItemParent(Item) != Parent)
			Scroll(Parent, 0);
		while (Item)
		{
			if (GetItem(0, Item))
				Collapse(Item);
			Item = GetNext(Item, 0);
		}
		Update((WISP_RECT*)0);
		return true;
	}

	bool CSourceCodeWnd::OnEventGotoLine(WISP_MSG *pMsg)
	{
		TWideString String;
		WCHAR szString[64];
		TSPrintf(szString, WSTR("Line number (1 - %d):  "), m_TxtFileLineNumber - 1);
		CListStringItem *Item = GetNext(0, 8);
		if (Item)
		{
			unsigned long LineNum = GetItemValue(Item, 1);
			String.Format(WSTR("%d"), LineNum);
		} else
		{
			String = WSTR("1");
		}

		if (CStrInputWnd::Input(&String, szString, WSTR("Please Input ....")))
		{
			unsigned long LineNumber = 0;
			if (USDecStrToNum(String.operator const WCHAR*(), &LineNumber))
			{
				if (LineNumber >= 1)
				{
					CListStringItem *Item = GetItem(LineNumber - 1, 0);
					if (Item)
					{
						Scroll(Item, -1);
						Select(Item);
						Focus();
					}
				}
			}
		}
		return true;
	}
	bool CSourceCodeWnd::OnEventAddWatch(WISP_MSG *pMsg)
	{
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_TypeViewerWnd.AddWatch(m_WatchVar.operator const char*(), NULL);
		return true;
	}
	bool CSourceCodeWnd::OnEventToggleBM(WISP_MSG *pMsg)
	{
		ToggleBM();
		return true;
	}

	unsigned long CSourceCodeWnd::GetAddr(CListStringItem *Item)
	{
		unsigned long LineNumber = GetItemValue(Item, 1);
		if (!GetItemLevel(Item))
		{
			map<unsigned long, MAPFilesLN>::IT it = m_FileLNMap.find(LineNumber);
			if (it == m_FileLNMap.end())
			{
				FilesLN *fileln = m_pSDSMod->GetFilesLN(m_CurrentFileID, LineNumber);
				if (!fileln) return 0;
				LineNumber = fileln->address;
			} else
			{
				LineNumber = it->second.fileln->address;
			}
		}
		return LineNumber;
	}

	bool CSourceCodeWnd::OnEventInsDelBP(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FullFindItem(0, 8);
		if (Item)
		{
			unsigned long Addr = GetAddr(Item);
			CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(m_pSDSMod->GetFILESIG().TimeStamp,m_pSDSMod->GetFILESIG().CheckSum);
			if (pDbgMod)
			{
				gpSyser->m_SyserUI.ToggleInsertBP(Addr + pDbgMod->m_ImageBase);
			} else
			{
				TListIter<unsigned long> It = m_pSDSMod->m_BreakPoints.Find(Addr);
				if (It == m_pSDSMod->m_BreakPoints.End())
					m_pSDSMod->m_BreakPoints.Append(&Addr);
				else	m_pSDSMod->m_BreakPoints.Remove(It);
				UpdateContextPics();
			}
		}
		return true;
	}
	bool CSourceCodeWnd::OnEventEnDisBP(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FullFindItem(0, 8);
		if (Item)
		{
			unsigned long Addr = GetAddr(Item);
			CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(m_pSDSMod->GetFILESIG().TimeStamp,m_pSDSMod->GetFILESIG().CheckSum);
			if (pDbgMod)
			{
				gpSyser->m_SyserUI.ToggleEnableBP(Addr + pDbgMod->m_ImageBase);
			}
		}
		return true;
	}

	bool CSourceCodeWnd::OnEventGotoHere(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FullFindItem(0, 8);
		if (Item)
		{
			unsigned long Addr = GetAddr(Item);
			CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(m_pSDSMod->GetFILESIG().TimeStamp,m_pSDSMod->GetFILESIG().CheckSum);
			if (pDbgMod)
			{
				gpSyser->m_pDebugger->SetCodeBP(Addr + pDbgMod->m_ImageBase, 0x200, BP_STATE_ENABLE);
				gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("x"), 0);
			}
		}
		return true;
	}

	bool CSourceCodeWnd::OnEventShowHere(WISP_MSG *pMsg)
	{
		CListStringItem *Item = FullFindItem(0, 8);
		if (Item)
		{
			unsigned long Addr = GetAddr(Item);
			CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(m_pSDSMod->GetFILESIG().TimeStamp,m_pSDSMod->GetFILESIG().CheckSum);
			if (pDbgMod)
			{
				*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP = Addr + pDbgMod->m_ImageBase;
				gpSyser->m_pDebugger->UpdateRegisters();
				gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
				gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
			}
		}
		return true;
	}

	void CSourceCodeWnd::OnLButton(CListStringItem *Item, int nCol)
	{
		if (nCol == 0)
			OnEventInsDelBP(0);
	}

	void CSourceCodeWnd::OnRButton(CListStringItem *Item, int nCol)
	{
		if (nCol == 0)
			ToggleBM();
	}

	bool CSourceCodeWnd::GetTipText(CListStringItem *Item, int nCol, int *pPosX, int *pPosY, WCHAR *pOutText)
	{
		if (gSyserConfig.iSourceCodeShowTips == 0) return false;
		if (nCol != 1) return false;

		TStrCpy(pOutText, "");

		WISP_RECT Rect;
		GetItemRect(Item, 1, &Rect);

		unsigned long LineNumber = GetItemValue(Item, 1);
		unsigned int MaxLen = m_pWispBase->m_MousePT.x - m_nSpaceWidth - m_ScrClientRect.x - Rect.x;
		CListStringItem::CONTENT *Content = GetItemContent(Item, 1);
		const WCHAR *str = Content->String.operator const WCHAR*();
		unsigned int nLen = m_ClientDC.GetTextLen(str, MaxLen); //???
		if (nLen == -1) return false;

		char szString[512];
		TStrCpy(szString, ((unsigned long *)str)+1); //???
		*pPosX += MaxLen + 10;
		*pPosY += 16;

		yy_set_input(szString);

		int sol = 0;
		int eol = 0;

		yyrestart(0);

		for (;;)
		{
			int Lex = yylex();
		#if 1
			switch (Lex)
			{
			case 0:
				return true;
			case 660:
				break;
			case 679:
				break;
			case 705:
				if (sol == 0)
					sol = eol;
				break;
			default:
				sol = 0;
				break;
			}
		#else
			if (Lex == 0) return true;
			if (Lex != 660) // . L_PERIOD
			{
				if (Lex == 679) // -> L_POINTSTO
				{
				} else
				if (Lex == 705) // string
				{
					if (sol == 0)
						sol = eol;
				} else
				{
					sol = 0;
				}
			}
		#endif
			if (nLen >= eol && nLen < yyleng + eol)
			{
				if (Lex != 705) return false;
				break;
			}
			eol += yyleng;
		}

		TAnsiString String;

		if (sol != eol)
		{
			szString[eol + yyleng] = 0;
			bool bFunc = false;
			unsigned long offset = 0;
			void *ptr = m_pSDSMod->GetGlobal(&szString[sol], m_CurrentFileID, LineNumber, &bFunc, &offset);
			if (!ptr) return false;

			String.Empty();
			if (bFunc)
			{
				FUNCTION *func = (FUNCTION*)ptr;
				char Buffer[512];
				int Len = 512;
				memset(Buffer, 0, 512);
				m_pSDSMod->GetOutputFunc(func, Buffer, &Len);
				String = Buffer;
			} else
			{
				DATATYPE *data = (DATATYPE*)ptr;
				char Output[512];
				int OutputLen = 512;
				memset(Output, 0, 512);
				String = m_pSDSMod->GetOutput(data->TypeID, Output, &OutputLen);
				String += " ";
				if (data->ClassID)
				{
					BASICTYPE *udt = m_pSDSMod->GetGlobal(data->ClassID);
					if (udt && (udt->Elem.Type == ELEM_STRUCT ||
						    udt->Elem.Type == ELEM_CLASS ||
						    udt->Elem.Type == ELEM_UNION))
					{
						String += m_pSDSMod->GetOutput(data->ClassID, Output, &OutputLen);
						String += "::";
					}
				}
				String += data->name;
			}
			TStrCpy(pOutText, String.operator const char*());
			return true;
		} else
		{
		#if 1 //+++
			if (GetOutput(yytext, Item, String))
			{
				TStrCpy(pOutText, String.operator const char*());
				return true;
			}
		#endif
			BASICTYPE *type = m_pSDSMod->GetGlobal(yytext, true);
			if (type)
			{
				switch (type->Elem.Type)
				{
				case ELEM_ENUM:{
					String.Empty();
					m_pSDSMod->GetOutputEnum((ENUMTYPE*)type, String);
					TStrCpy(pOutText, String.operator const char*());
					break;}
				case ELEM_STRUCT:{
					TStrCpy(pOutText, "struct ");
					TStrCat(pOutText, yytext);
					break;}
				case ELEM_CLASS:{
					TStrCpy(pOutText, "class ");
					TStrCat(pOutText, yytext);
					break;}
				case ELEM_UNION:{
					TStrCpy(pOutText, "union ");
					TStrCat(pOutText, yytext);
					break;}
				case ELEM_TYPEDEF:{
					String.Empty();
					m_pSDSMod->GetOutputTypedef((TYPEDEFTYPE*)type, String);
					TStrCpy(pOutText, String.operator const char*());
					break;}
				default:
					goto func;
				}
				return true;
			}
		func:
		#if 0 //+++
			if (GetOutput(yytext, Item, String))
			{
				TStrCpy(pOutText, String.operator const char*());
				return true;
			}
		#endif
			return false;
		}
		return false;
	}

	bool CSourceCodeWnd::Edit(CListStringItem *Item, int nCol, TWideString& String)
	{
		if (IsColorString(String.operator const WCHAR*()))
		{	
			WCHAR Buffer[512];
			TStrCpyLimit(Buffer, ((unsigned long*)String.operator const WCHAR*())+1, 512);
			m_EditWnd.m_WndText = Buffer;
		}
		return true;
	}

	bool CSourceCodeWnd::OnEditing(CListStringItem *Item, int nCol, TWideString& String)
	{
		return false;
	}

	void CSourceCodeWnd::ClearBM()
	{
		m_Bookmark.clear();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateMenu();
		UpdateContextPics();
	}
	bool CSourceCodeWnd::IsBM(unsigned long LineNumber)
	{
		return m_Bookmark.find(LineNumber) != m_Bookmark.end();
	}
	void CSourceCodeWnd::ToggleBM()
	{
		CListStringItem *Item = FullFindItem(0, 8);
		if (Item)
		{
			if (GetItemLevel(Item))
				Item = GetItemParent(Item);
			unsigned long LineNumber = GetItemValue(Item, 1);
			map<unsigned long,CListStringItem *>::IT it = m_Bookmark.find(LineNumber);
			if (it)
				m_Bookmark.erase(it);
			else	m_Bookmark.insert(_Tmap_pair<unsigned long,CListStringItem *>(LineNumber, Item));
			gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateMenu();
			UpdateContextPics();
		}
	}
	void CSourceCodeWnd::NextBM()
	{
		CListStringItem *Item = FindItem(0, 8);
		if (!Item)
			Item = GetRootItem();
		if (Item)
		{
			if (GetItemLevel(Item))
				Item = GetItemParent(Item);
			unsigned long LineNumber = GetItemValue(Item, 1);
			map<unsigned long,CListStringItem *>::IT it = m_Bookmark.find_near(LineNumber);
			if (it) ++it;
			if (!it) it = m_Bookmark.begin();
			if (it)
			{
				Scroll(it->second, -1);
				Select(it->second);
			}
		}
	}
	void CSourceCodeWnd::PrevBM()
	{
		CListStringItem *Item = FindItem(0, 8);
		if (!Item)
			Item = GetRootItem();
		if (Item)
		{
			if (GetItemLevel(Item))
				Item = GetItemParent(Item);
			unsigned long LineNumber = GetItemValue(Item, 1);
			map<unsigned long,CListStringItem *>::IT it = m_Bookmark.find_near(LineNumber);
			if (it && FindItem(0, 8) == it->second)
				--it;
			if (!it) it = m_Bookmark.rbegin();
			if (it)
			{
				Scroll(it->second, -1);
				Select(it->second);
			}
		}
	}

	void CSourceCodeWnd::ShowLineNum(unsigned long LineNumber)
	{
		CListStringItem *Item = GetItem(0, 0);
		if (Item)
		{
			while (Item)
			{
				unsigned long lineNum = GetItemValue(Item, 1);
				if (lineNum == LineNumber)
					break;
				Item = GetNext(Item, 0);
			}
			if (Item && !CheckItemPage(Item))
			{
				Scroll(Item, -1);
				Select(Item);
			}
		}
	}

	unsigned long *CSourceCodeWnd::OptItemToCS(_OPERAND_ITEM *OpArray, int nOp, unsigned long *pColorString, _INSTRUCTION_INFORMATION *pDasmInstr)
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
						//TODO
						const char *Symbol;
						if (GetSign(&pOp->addr))
						{
							pColorString += ColorStrCpy(pColorString, "+", COLOR_OPTR, 0);
							if (/*pOp->addr.index == -1 &&*/ pOp->addr.address_size == CODE_MODE && /*pOp->addr.base == STACKREG &&*/
								(Symbol = m_CurrentFunction.GetSymbol(pOp->addr.displacement, pOp->addr.base/*STACKREG*/, 0)))
							{
								pColorString += ColorStrCpy(pColorString, Symbol, COLOR_SYMBOL, 0);
							} else
							{
								TSPrintf(szBuffer, CInstrSym::m_SizePtr[GetSize(pOp->addr.displacement)], pOp->addr.displacement);
								pColorString += ColorStrCpy(pColorString, szBuffer, COLOR_IMMED, 0);
							}
						} else
						{
							pColorString += ColorStrCpy(pColorString, "-", COLOR_OPTR, 0);
							if (/*pOp->addr.index == -1 &&*/ pOp->addr.address_size == CODE_MODE && /*pOp->addr.base == STACKREG &&*/
								(Symbol = m_CurrentFunction.GetSymbol(pOp->addr.displacement, pOp->addr.base/*STACKREG*/, 0)))
							{
								pColorString += ColorStrCpy(pColorString, Symbol, COLOR_SYMBOL, 0);
							} else
							{
								TSPrintf(szBuffer, CInstrSym::m_SizePtr[GetSize(pOp->addr.displacement)], -pOp->addr.displacement);
								pColorString += ColorStrCpy(pColorString, szBuffer, COLOR_IMMED, 0);
							}
						}
					} else
					{
						TSPrintf(szBuffer, CInstrSym::m_SizePtr[GetSize(pOp->addr.displacement)], pOp->addr.displacement);
						pColorString += ColorStrCpy(pColorString, szBuffer, COLOR_IMMED, 0);
					}
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

	unsigned long *CSourceCodeWnd::InstrToCS(_INSTRUCTION_INFORMATION *pDasmInstr, unsigned long *pColorString)
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

	unsigned long *CSourceCodeWnd::ColorDasm(ULONG_PTR ImageBase, ULONG_PTR Address, unsigned int MaxSize, unsigned long *pColorString, int nCodeAlign)
	{
		unsigned char CodeBuff[64];
		_INSTRUCTION_INFORMATION DasmInstr;
		DasmInstr.CodeBuff = CodeBuff;
		DasmInstr.eip = Address;
		DasmInstr.pasm = nullptr;

		if (Address < m_CurrentFunction.m_ImageBase || Address >= m_CurrentFunction.m_ImageHighBase)
		{
			m_CurrentFunction.Init(m_pSDSMod, m_pSDSMod->GetFunction(Address - ImageBase));
		}

		unsigned long CodeLen = m_pSyserCodeDoc->Dasm(&DasmInstr, 0, MaxSize);
		if (nCodeAlign)
		{
			if (DasmInstr.OpCode == -1)
			{
				ColorStrCpy(pColorString, WSTR("??"), COLOR_IMMED, 0);
			} else
			{
				WCHAR Buffer[64];
				THexBytesToStr(DasmInstr.CodeBuff, Buffer, CodeLen, 1, (WCHAR*)0);
				ColorStrCpy(pColorString, Buffer, COLOR_IMMED, 0);
			}
			pColorString += TStrFillTail(pColorString, nCodeAlign, (unsigned long)32);
		}
		InstrToCS(&DasmInstr, pColorString);
		return pColorString;
	}

	void CSourceCodeWnd::GetColorKeyword(char *String, unsigned long *pColorString, int *pLen)
	{
		if (gSyserConfig.iSourceCodeSyntaxColors == 0)
		{
			if (*pLen > 1)
			{
				ColorStrCpy(pColorString, "", 40, 0);// // 0x7B7B7B,   //Unknown
				*pLen -= 1;
			}

			if (*pLen > TStrLen(String))
			{
				ColorStrCat(pColorString, String, 27, 0);// // 0xFFFFFF,   //Keyword
				*pLen -= TStrLen(String);
			}
			return;
		}
		unsigned long nKeywords = 0;

		yy_set_input(String);

		if (*pLen > 1)
		{
			ColorStrCpy(pColorString, "", 40, 0);// // 0x7B7B7B,   //Unknown
			*pLen -= 1;
		}

		if (!m_bComment)
			yyrestart(0);

		unsigned long nLastPos = 0;
		for (int Lex = yylex(); Lex; Lex = yylex())
		{
			nLastPos += yyleng;
			if (m_bComment && Lex != 703) continue; //wait a end of cmt block
			++nKeywords;
			if (Lex >= 512 && Lex <= 631)
			{
				if (*pLen > yyleng)//TStrLen(yytext))
				{
					ColorStrCat(pColorString, yytext, 27, 0);// // 0xFFFFFF,   //Keyword
					*pLen -= yyleng;//TStrLen(yytext);
				}
			} else
			if (Lex >= 632 && Lex <= 645)
			{
				if (*pLen > yyleng)//TStrLen(yytext))
				{
					ColorStrCat(pColorString, yytext, 29, 0);// // 0x4080FF,   //Preprocessor Keyword
					*pLen -= yyleng;//TStrLen(yytext);
				}
			} else
			if (Lex >= 652 && Lex <= 700)
			{
				if (*pLen > yyleng)//TStrLen(yytext))
				{
					ColorStrCat(pColorString, yytext, 34, 0);// // 0xFFFFFF,   //Operation
					*pLen -= yyleng;//TStrLen(yytext);
				}
			} else
	
			switch (Lex)
			{
			//case 512 ... 631:
			//	ColorStrCat(pColorString, yytext, 27, 0);// // 0xFFFFFF,   //Keyword
			//	break;
			//case 632 ... 645:
			//	ColorStrCat(pColorString, yytext, 29, 0);// // 0x4080FF,   //Preprocessor Keyword
			//	break;
                        case 646: // numbers??? 8
                        case 647: // numbers??? 4461
                        case 648: // hex??? 0xff
                        case 649: // float??? 0.0
                        case 650: // 'string' ???
				if (*pLen > yyleng)//TStrLen(yytext))
				{
					ColorStrCat(pColorString, yytext, 33, 0);// // 0x00FF80,   //Numerical
					*pLen -= yyleng;//TStrLen(yytext);
				}
				break;
                        case 651: // "string"
				if (*pLen > yyleng)//TStrLen(yytext))
				{
					ColorStrCat(pColorString, yytext, 32, 0);// // 0xFFFF00,   //String
					*pLen -= yyleng;//TStrLen(yytext);
				}
				break;
			//case 652 ... 700:
			//	ColorStrCat(pColorString, yytext, 34, 0);// // 0xFFFFFF,   //Operation
			//	break;
                        case 701: // ...
				if (*pLen > yyleng)//TStrLen(yytext))
				{
					ColorStrCat(pColorString, yytext, 32, 0);// // 0xFFFF00,   //String
					*pLen -= yyleng;//TStrLen(yytext);
				}
				break;
			case 702: // /*
				if (*pLen > yyleng)//TStrLen(yytext))
				{
					ColorStrCat(pColorString, yytext, 30, 0);// // 0x808080,   //Comment block
					*pLen -= yyleng;//TStrLen(yytext);
				}
				m_bComment = true;
				break;
			case 703: // */
				if (*pLen > yyleng)//TStrLen(yytext))
				{
					ColorStrCat(pColorString, yytext, 30, 0);// // 0x808080,   //Comment block
					*pLen -= yyleng;//TStrLen(yytext);
				}
				m_bComment = false;
				break;
			case 704: // //
				if (*pLen > yyleng)//TStrLen(yytext))
				{
					ColorStrCat(pColorString, yytext, 31, 0);// // 0x808080,   //Comment line
					*pLen -= yyleng;//TStrLen(yytext);
				}
				break;
			case 705: {
				BASICTYPE *type = m_pSDSMod->GetGlobal(yytext, true);
				if (type)
				{
					switch (type->Elem.Type)
					{
					case ELEM_STRUCT:
						if (*pLen > yyleng)//TStrLen(yytext))
						{
							ColorStrCat(pColorString, yytext, 36, 0);// // 0xE17D7D,   //Struct name
							*pLen -= yyleng;//TStrLen(yytext);
						}
						break;
					case ELEM_CLASS:
						if (*pLen > yyleng)//TStrLen(yytext))
						{
							ColorStrCat(pColorString, yytext, 35, 0);// // 0xE17D7D,   //Class name
							*pLen -= yyleng;//TStrLen(yytext);
						}
						break;
					case ELEM_UNION:
						if (*pLen > yyleng)//TStrLen(yytext))
						{
							ColorStrCat(pColorString, yytext, 37, 0);// // 0xE17D7D,   //Union name
							*pLen -= yyleng;//TStrLen(yytext);
						}
						break;
					case ELEM_TYPEDEF:
						if (*pLen > yyleng)//TStrLen(yytext))
						{
							ColorStrCat(pColorString, yytext, 38, 0);// // 0xE17D7D,   //Typedef name
							*pLen -= yyleng;//TStrLen(yytext);
						}
						break;
					default:
						if (*pLen > yyleng)//TStrLen(yytext))
						{
							ColorStrCat(pColorString, yytext, 28, 0);// // 0xC0C0C0,   //Identifier
							*pLen -= yyleng;//TStrLen(yytext);
						}
						break;
					}
				} else
			#if 1 //TODO improve O(1)
				if (m_pSDSMod->GetFunction(yytext, true))
				{
					if (*pLen > yyleng)//TStrLen(yytext))
					{
						ColorStrCat(pColorString, yytext, 39, 0);// // 0xFF8000,   //Function name
						*pLen -= yyleng;//TStrLen(yytext);
					}
				} else
			#endif
				{
					if (*pLen > yyleng)//TStrLen(yytext))
					{
						ColorStrCat(pColorString, yytext, 28, 0);// // 0xC0C0C0,   //Identifier
						*pLen -= yyleng;//TStrLen(yytext);
					}
				}
				break; }
			default:
				if (*pLen > yyleng)//TStrLen(yytext))
				{
					ColorStrCat(pColorString, yytext, 40, 0);// // 0x7B7B7B,   //Unknown
					*pLen -= yyleng;//TStrLen(yytext);
				}
				break;
			}
		}

		if (m_bComment)
		{
			if (nKeywords == 0)
			{
				if (*pLen > yyleng)//TStrLen(String))
				{
					ColorStrCpy(pColorString, String, 30, 0);// // 0x808080,   //Comment block
					*pLen -= yyleng;//TStrLen(String);
				}
			} else
			{
				int len = nLastPos;//String-teststr;
				if (*pLen > len)//yyleng)//TStrLen(String))
				{
					ColorStrCat(pColorString, String+len, 30, 0);// // 0x808080,   //Comment block
					*pLen -= len;//yyleng;//TStrLen(String);
				}
			}
		}
	}
//#define TXTDEBUG
#ifdef TXTDEBUG
FILE *f = NULL;
#endif
	void CSourceCodeWnd::OpenSrcFile(const char *Text, unsigned int Size, SrcFileNameID *SrcFile)
	{
		if (m_FileOpen) return;

		m_pSyserCodeDoc = &gpSyser->m_SyserUI.m_SyserCodeDoc;

		CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(m_pSDSMod->GetFILESIG().TimeStamp, m_pSDSMod->GetFILESIG().CheckSum);
		if (!pDbgMod)
		{
			if (gpSyser->m_pDebugger != gpSyser->m_pSysDebugger)
					pDbgMod = gpSyser->m_pSysDebugger->m_DbgModuleList.FindModule(m_pSDSMod->GetFILESIG().TimeStamp, m_pSDSMod->GetFILESIG().CheckSum);
		}
		ULONG_PTR ImageBase = pDbgMod ? pDbgMod->m_ImageBase : 0;
		m_CurrentFileID = SrcFile->FileID;
		m_FileOpen = true;
		if (Text)
		{
			m_SrcTxtFile.Create(Size, (unsigned char*)Text, false);
			m_SrcTxtFile.OpenEx(0, 0, 3);
		} else
		{
			m_SrcTxtFile.OpenEx(SrcFile->FileName, 0, 3);
		}
		m_TxtFileLineNumber = m_SrcTxtFile.m_StrList.Size();

		int ColorStringLen = 256;
		unsigned long ColorString[256];
		unsigned long MaxSize = 1000;
		WCHAR *TextBufferW = new WCHAR[MaxSize];

#ifdef TXTDEBUG
	f = fopen("LOGSRC", "w");
//fprintf(f,"StringsLen %d\n", SDSFile->StringsLen);
//fflush(f);
#endif

		m_pSDSMod->GetFilesLN(m_CurrentFileID, &m_FileLNMap);
#if 0
	gpSyser->m_MainFrame.CreateProgress(WSTR(""), m_SrcTxtFile.m_StrList.Size());
	unsigned long Progress = m_SrcTxtFile.m_StrList.Size()/100;
#endif
		unsigned long nLineNum = 1;
		TListIter<char*> It = m_SrcTxtFile.m_StrList.Begin();
		while (It != m_SrcTxtFile.m_StrList.End())
		{
#ifdef TXTDEBUG
fprintf(f,"nLineNum %d, %s\n", nLineNum, *It);
fflush(f);
#endif
			ColorStringLen = 256;
			GetColorKeyword(*It, ColorString, &ColorStringLen);
			unsigned int NewSize = TStrLen(*It)+1;
			if (NewSize > MaxSize)
			{
				delete []TextBufferW;
				MaxSize = NewSize;
				TextBufferW = new WCHAR[NewSize];
			}
			unsigned long ulen = AnsiToUnicode(*It, TextBufferW, NewSize); //long

			for (unsigned long n = 0; n < ulen; ++n)
			{
				ColorString[n] = (ColorString[n] & 0xFFFF0000) | TextBufferW[n];
				if (!TextBufferW[n])
					ColorString[n] = 0;
			}

			WCHAR NumStr[20];
			uNumToStr(nLineNum, NumStr, 10);
			if (gSyserConfig.iSourceCodeShowLineNumbers == 0)
			{
				NumStr[0] = L' ';
				NumStr[1] = 0;
			}
			CListStringItem *Item = InsertItem(NumStr, 0, 0, 0, -1);
			SetItemValue(Item, 0, 0);
			SetItemTextColor(Item, 1, ColorString);
			SetItemValue(Item, 1, nLineNum);
			if (ImageBase)
			{
				map<unsigned long, MAPFilesLN>::IT it = m_FileLNMap.find(nLineNum);
				if (it != m_FileLNMap.end())
				{
					if (it->second.fileln->statement & 1)
					{
						unsigned long rva = it->second.fileln->address;
						ULONG_PTR Addr = ImageBase+rva;
						unsigned long length = it->second.fileln->length;
#ifdef TXTDEBUG
fprintf(f,"statement addr %08x rva %08x length %d\n", Addr, rva, length);
fflush(f);
#endif
						while(length>0)
						{
							char szString[256];
						#ifdef _X64_
							TSPrintf(szString, "%016I64x ", Addr); //14
						#else
							TSPrintf(szString, "%08x      ", Addr); //14
						#endif
							unsigned long CodeLen = m_pSyserCodeDoc->InstrLen(Addr, 16);
							if (!CodeLen) CodeLen = 1;
							unsigned char CodeBuf[52];
							m_pSyserCodeDoc->ReadMemory(Addr, CodeBuf, CodeLen);
							ColorStrCpy(ColorString, szString, 2, 1);
						#ifdef _X64_
							ColorDasm(ImageBase, Addr, 16, &ColorString[17], 0); //
						#else
							ColorDasm(ImageBase, Addr, 16, &ColorString[14], 0); //
						#endif
#ifdef TXTDEBUG
char szColorString[1024];
memset(szColorString, 0, sizeof(szColorString));
for (unsigned long n = 0; ColorString[n]; n++)
	szColorString[n] = ColorString[n]&0xff;
fprintf(f,"dasm %s\n", szColorString);
fflush(f);
#endif
							CListStringItem *DasmItem = InsertItem(WSTR(""), Item, 0, 0, -1);
							SetItemValue(DasmItem, 0, 0);
							SetItemTextColor(DasmItem, 1, ColorString);
							SetItemValue(DasmItem, 1, rva);
							if (CodeLen == 1 && CodeBuf[0] == 0xC3) break;
							if (CodeLen > length)
							{
								break; //???
							}
							length -= CodeLen;
							rva += CodeLen;
							Addr += CodeLen;
						}
					}
				}
			}
			++nLineNum;
#if 0
	if (Progress && !(nLineNum%Progress))
	gpSyser->m_MainFrame.UpdateProgress(nLineNum);
#endif
			++It;
		}
#if 0
	gpSyser->m_MainFrame.DestroyProgress();
#endif
#ifdef TXTDEBUG
fclose(f);
#endif
		if (ImageBase)
			m_CurAddr = ImageBase;
		m_SrcTxtFile.Close();
		delete []TextBufferW;
	}

	void CSourceCodeWnd::UpdateContextSrc()
	{
		if (!m_FileOpen)
			return;

		if (!m_pSDSMod)
			return;

		CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(m_pSDSMod->GetFILESIG().TimeStamp, m_pSDSMod->GetFILESIG().CheckSum);
		if (!pDbgMod)
		{
			if (gpSyser->m_pDebugger != gpSyser->m_pSysDebugger)
					pDbgMod = gpSyser->m_pSysDebugger->m_DbgModuleList.FindModule(m_pSDSMod->GetFILESIG().TimeStamp, m_pSDSMod->GetFILESIG().CheckSum);
		}
		ULONG_PTR ImageBase = pDbgMod ? pDbgMod->m_ImageBase : 0;
		unsigned long nLineNum = 1;
		CListStringItem *Item = FindItem(0, 0);
		while (Item)
		{
			if (FindItem(Item, 0))
			{
				if (ImageBase && m_CurAddr == ImageBase)
					break;
				DeleteAllChildrenItems(Item);
			}
			if (ImageBase)
			{
				map<unsigned long, MAPFilesLN>::IT it = m_FileLNMap.find(nLineNum);
				if (it != m_FileLNMap.end())
				{
					unsigned long rva = it->second.fileln->address;
					ULONG_PTR Addr = ImageBase+rva;
					unsigned long length = it->second.fileln->length;
					while(length>0)
					{
						char szString[256];
					#ifdef _X64_
						TSPrintf(szString, "%016I64x ", Addr); //14
					#else
						TSPrintf(szString, "%08x      ", Addr); //14
					#endif
						unsigned long CodeLen = m_pSyserCodeDoc->InstrLen(Addr, 16);
						if (!CodeLen) CodeLen = 1;
						unsigned char CodeBuf[52];
						m_pSyserCodeDoc->ReadMemory(Addr, CodeBuf, CodeLen);
						unsigned long ColorString[256];
						ColorStrCpy(ColorString, szString, 2, 1);
					#ifdef _X64_
						ColorDasm(ImageBase, Addr, 16, &ColorString[17], 0); //
					#else
						ColorDasm(ImageBase, Addr, 16, &ColorString[14], 0); //
					#endif
						CListStringItem *DasmItem = InsertItem(WSTR(""), Item, 0, 0, -1);
						SetItemValue(DasmItem, 0, 0);
						SetItemTextColor(DasmItem, 1, ColorString);
						SetItemValue(DasmItem, 1, rva);
						if (CodeLen == 1 && CodeBuf[0] == 0xC3) break;
						if (CodeLen > length)
						{
							break; //???
						}

						length -= CodeLen;
						rva += CodeLen;
						Addr += CodeLen;
					}
				}
			}
			SetItemValue(Item, 0, 0);
			Item = GetNext(Item, 0);
			++nLineNum;
		}

		if (ImageBase)
		{
			if (m_CurAddr != ImageBase)
				m_CurAddr = ImageBase;
		}
	}

	bool CSourceCodeWnd::UpdateContextPics()
	{
		if (!m_FileOpen)
			return false;

		if (!m_pSDSMod)
			return false;

		CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(m_pSDSMod->GetFILESIG().TimeStamp, m_pSDSMod->GetFILESIG().CheckSum);
		if (!pDbgMod)
		{
			if (gpSyser->m_pDebugger != gpSyser->m_pSysDebugger)
					pDbgMod = gpSyser->m_pSysDebugger->m_DbgModuleList.FindModule(m_pSDSMod->GetFILESIG().TimeStamp, m_pSDSMod->GetFILESIG().CheckSum);
		}
		ULONG_PTR ImageBase = pDbgMod ? pDbgMod->m_ImageBase : 0;

		bool ret = false;
		unsigned long nLineNum = 1;
		CListStringItem *Item = FindItem(0, 0);
		while (Item)
		{
			unsigned long pic = 0;

			map<unsigned long, MAPFilesLN>::IT it = m_FileLNMap.find(nLineNum);
			if (it)
			{
				if (ImageBase)
				{
					bool open = false;
					CListStringItem *SubItem = GetItem(0, Item);
					while (SubItem)
					{
						
						unsigned long rva = GetItemValue(SubItem, 1);
						pic = m_pSyserCodeDoc->GetPic(ImageBase + rva);
						if (pic & 1)
							open = true;
						SetItemValue(SubItem, 0, (unsigned long long)pic);
						SubItem = GetNext(SubItem, 0);
					}
					pic = m_pSyserCodeDoc->GetPic(ImageBase + it->second.fileln->address);
					if (open)
					{
						pic |= 1;
						ret = true;
					}
				} else
				{
					pic = 0;
					if (m_pSDSMod->m_BreakPoints.Size())
					{
						TListIter<unsigned long> It = m_pSDSMod->m_BreakPoints.Find(it->second.fileln->address);
						if (It != m_pSDSMod->m_BreakPoints.End())
							pic = 2;
					}
				}
			}
			
			if (IsBM(nLineNum))
				pic |= 8;
			SetItemValue(Item, 0, (unsigned long long)pic);
			
			++nLineNum;
			Item = GetNext(Item, 0);
		}
		Update((WISP_RECT*)0);
		return ret;
	}

	bool CSourceCodeWnd::GetOutput(char *Str, CListStringItem *Item, TAnsiString & String)
	{
		char Buffer[512];
		int Len = 512;

		FilesLN *filesLN = nullptr;
		TList<FilesLN*> ListFilesLN;
		
		unsigned long LineNumber = GetItemValue(Item, 1);
		map<unsigned long, MAPFilesLN>::IT it = m_FileLNMap.find(LineNumber);
		if (it == m_FileLNMap.end())
			filesLN = m_pSDSMod->GetFilesLN(m_CurrentFileID, LineNumber);
		else	filesLN = it->second.fileln;

		if (!filesLN) return false;
		
		FUNCTION *func = m_pSDSMod->GetFunction(filesLN->address);
		if (!func) return false;

		if (m_CurrentFunction.m_pFunc != func)
			m_CurrentFunction.Init(m_pSDSMod, func);

		DATATYPE *data = m_CurrentFunction.GetLocal(Str, true);
		if (data)
		{
			String = m_pSDSMod->GetOutput(data->TypeID, Buffer, &Len);
			String += " ";
			if (data->ClassID)
			{
				BASICTYPE *type = m_pSDSMod->GetGlobal(data->ClassID);
				if (type && (type->Elem.Type == ELEM_STRUCT ||
					     type->Elem.Type == ELEM_CLASS ||
					     type->Elem.Type == ELEM_UNION))
				{
					String += m_pSDSMod->GetOutput(data->ClassID, Buffer, &Len);
					String += "::";
				}
			}
			String += data->name;
		} else
		{
			func = m_CurrentFunction.GetFunction(Str, 1);
			if (!func) return false;

			memset(Buffer, 0, 512);
			m_pSDSMod->GetOutputFunc(func, Buffer, &Len);
			String = Buffer;
		}
		return true;
	}

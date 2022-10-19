
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "findstringform.hpp"
#include "syserconfig.hpp"
#include "resource.hpp"

WISP_FORM_RES_ITEM FindStringForm[] =
{
	{WISP_CTRL_FORM, {0, 0, 320, 120}, 0, 0xB, WSTR("Find"), (void*)"\\SyserIcon\\Plugin.ico", WSTR("Find")},
	{WISP_CTRL_STATIC_STRING, {10, 18, 70, 20}, 0, 0, WSTR("Find what:  "), 0, 0},
	{WISP_CTRL_EDIT, {80, 15, 220, 20}, 3, 0x1000006, 0, 0, 0},
	{WISP_CTRL_BUTTON, {54, 50, 78, 21}, 1, 0x200, WSTR("Find Next"), 0, 0},
	{WISP_CTRL_BUTTON, {186, 50, 78, 21}, 0x80002002, 0, WSTR("Close"), 0, 0},
	{0},
};

	WISP_MSG_MAP_BEGIN(CFindStringForm)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispForm)

	WISP_CMD_MAP_BEGIN(CFindStringForm)
		WISP_CMD_MAP(3, OnCmdFindWhat)
	WISP_CMD_MAP_END

	WISP_EVENT_MAP_BEGIN(CFindStringForm)
		WISP_EVENT_MAP(1, OnEvent1)
	WISP_EVENT_MAP_END

	CFindStringForm::CFindStringForm()
	{
		m_Resource = FindStringForm;
	}
	CFindStringForm::~CFindStringForm()
	{
	}
	
	bool CFindStringForm::OnCreateForm(WISP_MSG *pMsg)
	{
		m_pFindNext = (CWispButton *)GetFormItem(1);
		m_pClose = (CWispButton *)GetFormItem(2);
		m_pFindWhat = (CWispEdit *)GetFormItem(3); 		
		return true;
	}

	bool CFindStringForm::OnCmdFindWhat(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000703) //WISP_ID_EDIT_KEYDOWN
		{
			if (pMsg->Command.Param1 != 13) return true;
			m_pFindNext->Focus();
			return OnEvent1(0);
		}

		if (pMsg->Command.CmdMsg == 0x80000704) //WISP_ID_EDIT_KEYUP
		{
			if (pMsg->Command.Param1 != 13) return true;
			return false;
		}

		if (pMsg->Command.CmdMsg == 0x80000705) //WISP_ID_EDIT_CHANGED
		{
			m_pFindNext->EnableWindow(m_pFindWhat->GetWindowText().IsEmpty()==0);
			m_pFindNext->Update((WISP_RECT*)0);
		}

		return true;
	}

	bool CFindStringForm::OnEvent1(WISP_MSG *pMsg)
	{
		const TWideString &String = m_pFindWhat->GetWindowText();

		if (m_pStringReferenceList)
		{
			if (m_pStringReferenceList->m_SearchString == String)
			{
				m_pStringReferenceList->OnEventSearchAgain(0);
			} else
			{
				m_pStringReferenceList->Search(String.operator const WCHAR*());
			}
		}
		return true;
	}


	WISP_MSG_MAP_BEGIN(CStringReferenceList)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_MOUSE_DBLCLICK, OnMouseDblClick)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_CHAR, OnChar)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

	WISP_CMD_MAP_BEGIN(CStringReferenceList)
	WISP_CMD_MAP_END

	WISP_EVENT_MAP_BEGIN(CStringReferenceList)
		WISP_EVENT_MAP(EVENT_ID_SEARCH, OnEventSearch)
		WISP_EVENT_MAP(EVENT_ID_SEARCH_AGAIN, OnEventSearchAgain)
	WISP_EVENT_MAP_END

	CStringReferenceList::CStringReferenceList()
	{
		m_nSearchLen = 0;
	}
	CStringReferenceList::~CStringReferenceList()
	{
	}

	void CStringReferenceList::LoadKeyMap()
	{
		if (!gpSyser->GetHotKey(EVENT_ID_SEARCH_AGAIN, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x2000004E, EVENT_ID_SEARCH_AGAIN));
		if (!gpSyser->GetHotKey(EVENT_ID_SEARCH, m_KeyMap)) m_KeyMap.insert(_Tmap_pair<unsigned int, unsigned int>(0x4000004E, EVENT_ID_SEARCH));
	}

WISP_MENU_GROUP_ITEM StringReferenceListPopupMenu[] =
	{
		{WSTR("Search          Ctrl+N"), EVENT_ID_SEARCH, 0x77, 0, 0},
		{WSTR("Search again    Alt+N"), EVENT_ID_SEARCH_AGAIN, 0x77, 0, 0},
		{0},
	};

	bool CStringReferenceList::OnCreate(WISP_MSG *pMsg)
	{
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);
		m_SearchString.Empty();
		LoadKeyMap();
		CWispDIBList *pDIBList = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF);
		m_PopupMenu.LoadPopupMenuGroup(StringReferenceListPopupMenu, this, pDIBList);

		m_SearchAgainMenuItem = m_PopupMenu.GetItem(EVENT_ID_SEARCH_AGAIN);
		return true;
	}
	bool CStringReferenceList::OnDestroy(WISP_MSG *pMsg)
	{
		m_PopupMenu.Destroy();
		return true;
	}

	bool CStringReferenceList::OnMouseDblClick(WISP_MSG *pMsg)
	{
		CListStringItem *pItem = GetNext(0, 8);
		WISP_RECT Rect;
		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = 0;
		Rect.cy = 0;
		int nCol = (m_ShowFormat != 0)*2 + 1;
		if (pMsg->KeyEvent.KeyType == VK_LBUTTON && pItem)
		{
			GetItemRect(pItem, nCol, &Rect);
			int X = pMsg->MsgMouseWndPT.x;
			if (X >= Rect.x && X <= Rect.x + Rect.cx)
			{
				ULONG_PTR Addr = GetItemValue(pItem, 1);
				gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(Addr, 0);
			} else
			{
				ULONG_PTR Addr = GetItemValue(pItem, 0);
				gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(Addr, true);
			}
		}
		return true;
	}

	void CStringReferenceList::PopupMenu()
	{
		CListStringItem *pRootItem = GetNext(0, 8);
		m_PopupMenu.Enable(m_SearchAgainMenuItem, !m_SearchString.IsEmpty());
		m_PopupMenu.Point(nullptr, true);
	}

	bool CStringReferenceList::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (!pMsg->KeyEvent.bKeyDown) return true;

		map<unsigned int, unsigned int>::IT it = m_KeyMap.find(pMsg->KeyEvent.KeyType);
		if (it != m_KeyMap.end())
		{
		        WISP_MSG Msg;
		        Msg.Command.CmdID = it->second;
		        return OnEvent(&Msg);
		}

		if (pMsg->KeyEvent.KeyType == VK_RBUTTON)
			PopupMenu();

		return true;
	}
	bool CStringReferenceList::OnChar(WISP_MSG *pMsg)
	{
		int Len = m_SearchString.m_nLength;
		if (pMsg->Char.Char == 8 || Len > m_nSearchLen)
		{
			if (Len)
				m_SearchString.Delete(Len - 1, 1);
			if (pMsg->Char.Char != 8)
				m_SearchString += pMsg->Char.Char;
		} else
		{
			m_SearchString += pMsg->Char.Char;
		}
		if (pMsg->Char.Char == 8)
			SendCommand(0, 1, 0);
		else	Search(nullptr);
		return true;
	}

	bool CStringReferenceList::OnEventSearch(WISP_MSG *pMsg)
	{
		m_FindStringForm.Create(0, 1);
		m_FindStringForm.m_pStringReferenceList = this;
		return true;
	}
	bool CStringReferenceList::OnEventSearchAgain(WISP_MSG *pMsg)
	{
		CListStringItem *pRootItem = GetNext(0, 8);
		if (m_SearchString.m_nLength)
		{
			if (!pRootItem)
				pRootItem = FullFindItem(0, 0);
			int Len = m_SearchString.m_nLength + 5;
			if (Len > 255) Len = 255;
			int nCol = (m_ShowFormat != 0)*2 + 1;
			if (pRootItem)
			{
				CListStringItem *pItem = FullFindItem(pRootItem, 0);
				if (pItem)
				{
					WCHAR Text[256];
					do {
						GetItemText(pItem, nCol, Text, Len);
						
						if (!TStrNICmp((Text[0]!='L')?&Text[1]:((Text[1]!='"')?&Text[1]:&Text[2]), m_SearchString.operator const WCHAR*(), m_SearchString.m_nLength)) break;
						pItem = FullFindItem(pItem, 0);
					} while (pItem);

					if (pItem)
					{
						Select(pItem);
						Scroll(pItem, -1);
					}
				}
			}
		}
		return true;
	}

	struct FindStringSection
	{
		bool FindSection(ULONG_PTR VirtualAddress, unsigned long VirtualSize, ULONG_PTR Address, unsigned long Size)
		{
			char Buffer[256];
			char ABuffer[512];
			WCHAR szAsm[256];
			CInstrSym InstrSym;
			unsigned char CodeBuff[16];
			CodeBuff[0] = 0;
			_INSTRUCTION_INFORMATION InstrInfo;
			char pasm[256];
			InstrInfo.CodeBuff = CodeBuff;
		#ifdef _X64_
			InstrInfo.CodeMode = CODE_MODE_64;
		#else
			InstrInfo.CodeMode = CODE_MODE_32;
		#endif
			InstrInfo.pasm = pasm;

		gpSyser->m_MainFrame.CreateProgress(WSTR(""), VirtualSize);
		unsigned long Progress = VirtualSize/100;

			unsigned long Result;
			for (unsigned long pos = 0; pos < VirtualSize; pos += Result)
			{
				ULONG_PTR EIP = VirtualAddress + pos;
				Result = gpSyser->m_pDebugger->ReadMemory(EIP, CodeBuff, 16);
				if (Result == 0)
				{
					VirtualAddress = (EIP & ~(((ULONG_PTR)4096)-1)) + 4096;
					Result = 4096 - (VirtualAddress + pos) % (4096-1);
					continue;
				}

				InstrInfo.eip = EIP;
				unsigned long CodeLen = InstrSym.Disassembler(&InstrInfo, NULL);
				if (CodeLen == 0)
				{
					++Result;
					continue;
				}
				Result = CodeLen;

				unsigned long RLen;
				if (!TStrICmp(InstrInfo.Name, "mov"))
				{
					if (InstrInfo.op[1].mode == OP_Address &&
					    InstrInfo.op[1].opersize == sizeof(ULONG_PTR) && //???
					    InstrInfo.op[1].addr.displacement > Address &&
					    InstrInfo.op[1].addr.displacement < (Address + Size) &&
					    gpSyser->m_pDebugger->ReadMemory(InstrInfo.op[1].addr.displacement, Buffer, 256) == 256 &&
					    ANormalize(Buffer, 256, ABuffer, 256*2, &RLen))
					{
						if (RLen == 2)
						{
							if (UNormalize(Buffer, 256, ABuffer, 256*2, &RLen))
							{
								TStrCpy(szAsm, InstrInfo.pasm);
								gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.insert(_Tmap_pair<ULONG_PTR,StringRef>(EIP, StringRef(InstrInfo.op[1].addr.displacement, 0x80000000|RLen/2)));
								TStrCpy(szAsm, ABuffer);
								AnsiToUnicode(ABuffer, szAsm, lenof(szAsm));
							}
						} else
						{
							TStrCpy(szAsm, InstrInfo.pasm);
							gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.insert(_Tmap_pair<ULONG_PTR,StringRef>(EIP, StringRef(InstrInfo.op[1].addr.displacement, RLen)));
							TStrCpy(szAsm, ABuffer);
							AnsiToUnicode(ABuffer, szAsm, lenof(szAsm));
						}

					} else
					if (InstrInfo.op[1].mode == OP_Immed &&
					    InstrInfo.op[1].opersize == sizeof(ULONG_PTR) && //???
					    InstrInfo.op[1].immed.immed_value > Address &&
					    InstrInfo.op[1].immed.immed_value < (Address + Size) &&
					    gpSyser->m_pDebugger->ReadMemory(InstrInfo.op[1].immed.immed_value, Buffer, 256) == 256 &&
					    ANormalize(Buffer, 256, ABuffer, 256*2, &RLen))
					{
						if (RLen == 2)
						{
							if (UNormalize(Buffer, 256, ABuffer, 256*2, &RLen))
							{
								TStrCpy(szAsm, InstrInfo.pasm);
								gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.insert(_Tmap_pair<ULONG_PTR,StringRef>(EIP, StringRef(InstrInfo.op[1].immed.immed_value, 0x80000000|RLen/2)));
								TStrCpy(szAsm, ABuffer);
								AnsiToUnicode(ABuffer, szAsm, lenof(szAsm));
							}
						} else
						{
							TStrCpy(szAsm, InstrInfo.pasm);
							gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.insert(_Tmap_pair<ULONG_PTR,StringRef>(EIP, StringRef(InstrInfo.op[1].immed.immed_value, RLen)));
							TStrCpy(szAsm, ABuffer);
							AnsiToUnicode(ABuffer, szAsm, lenof(szAsm));
						}
					}
				} else
				if (!TStrICmp(InstrInfo.Name, "lea"))
				{
					//first X64 only
					if (InstrInfo.op[1].mode == OP_Address &&
					    InstrInfo.SegmentPrefix == -1 &&
					    InstrInfo.op[1].addr.base == -1 &&
					    InstrInfo.op[1].addr.index == -1 &&
					    InstrInfo.op[1].addr.displacement_size == sizeof(ULONG_PTR) &&
					    InstrInfo.op[1].addr.displacement > Address &&
					    InstrInfo.op[1].addr.displacement < (Address + Size) &&
					    gpSyser->m_pDebugger->ReadMemory(InstrInfo.op[1].addr.displacement, Buffer, 256) == 256 &&
					    ANormalize(Buffer, 256, ABuffer, 256*2, &RLen))
					{
						if (RLen == 2)
						{
							if (UNormalize(Buffer, 256, ABuffer, 256*2, &RLen))
							{
								TStrCpy(szAsm, InstrInfo.pasm);
								gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.insert(_Tmap_pair<ULONG_PTR,StringRef>(EIP, StringRef(InstrInfo.op[1].addr.displacement, 0x80000000|RLen/2)));
								TStrCpy(szAsm, ABuffer);
								AnsiToUnicode(ABuffer, szAsm, lenof(szAsm));
							}
						} else
						{
							TStrCpy(szAsm, InstrInfo.pasm);
							gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.insert(_Tmap_pair<ULONG_PTR,StringRef>(EIP, StringRef(InstrInfo.op[1].addr.displacement, RLen)));
							TStrCpy(szAsm, ABuffer);
							AnsiToUnicode(ABuffer, szAsm, lenof(szAsm));
						}
					} else
					if (InstrInfo.op[1].mode == OP_Immed && //???
					    InstrInfo.op[1].opersize == sizeof(ULONG_PTR) && //???
					    InstrInfo.op[1].immed.immed_value > Address &&
					    InstrInfo.op[1].immed.immed_value < (Address + Size) &&
					    gpSyser->m_pDebugger->ReadMemory(InstrInfo.op[1].immed.immed_value, Buffer, 256) == 256 &&
					    ANormalize(Buffer, 256, ABuffer, 256*2, &RLen))
					{
						if (RLen == 2)
						{
							if (UNormalize(Buffer, 256, ABuffer, 256*2, &RLen))
							{
								TStrCpy(szAsm, InstrInfo.pasm);
								gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.insert(_Tmap_pair<ULONG_PTR,StringRef>(EIP, StringRef(InstrInfo.op[1].immed.immed_value, 0x80000000|RLen/2)));
								TStrCpy(szAsm, ABuffer);
								AnsiToUnicode(ABuffer, szAsm, lenof(szAsm));
							}
						} else
						{
							TStrCpy(szAsm, InstrInfo.pasm);
							gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.insert(_Tmap_pair<ULONG_PTR,StringRef>(EIP, StringRef(InstrInfo.op[1].immed.immed_value, RLen)));
							TStrCpy(szAsm, ABuffer);
							AnsiToUnicode(ABuffer, szAsm, lenof(szAsm));
						}
					}
				} else
				if (!TStrICmp(InstrInfo.Name, "push"))
				{
					//if (InstrInfo.op[1].mode == OP_Address)
					//{
					//	//???
					//} else
					//if (InstrInfo.op[1].mode == OP_Immed && //???
					if (InstrInfo.op[0].opersize == sizeof(ULONG_PTR) && //???
					    InstrInfo.op[0].immed.immed_value > Address &&
					    InstrInfo.op[0].immed.immed_value < (Address + Size) &&
					    gpSyser->m_pDebugger->ReadMemory(InstrInfo.op[0].immed.immed_value, Buffer, 256) == 256 &&
					    ANormalize(Buffer, 256, ABuffer, 256*2, &RLen))
					{
						if (RLen == 2)
						{
							if (UNormalize(Buffer, 256, ABuffer, 256*2, &RLen))
							{
								TStrCpy(szAsm, InstrInfo.pasm);
								gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.insert(_Tmap_pair<ULONG_PTR,StringRef>(EIP, StringRef(InstrInfo.op[0].immed.immed_value, 0x80000000|RLen/2)));
								TStrCpy(szAsm, ABuffer);
								AnsiToUnicode(ABuffer, szAsm, lenof(szAsm));
							}
						} else
						{
							TStrCpy(szAsm, InstrInfo.pasm);
							gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.insert(_Tmap_pair<ULONG_PTR,StringRef>(EIP, StringRef(InstrInfo.op[0].immed.immed_value, RLen)));
							TStrCpy(szAsm, ABuffer);
							AnsiToUnicode(ABuffer, szAsm, lenof(szAsm));
						}
					}
				}

		if (Progress && !(pos%Progress))
		gpSyser->m_MainFrame.UpdateProgress(pos);

			}

		gpSyser->m_MainFrame.DestroyProgress();
			return true;
		}

		bool Find(ULONG_PTR Address)
		{
			//CMemPEFile PEFile;
			CPEFile PEFile;
			CDbgModule *pDbgMod = gpSyser->m_pSysDebugger->m_DbgModuleList.FindModule(Address);
			if (!pDbgMod)
				pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(Address);
			if (pDbgMod)
			{
				ULONG_PTR ImageBase = pDbgMod->m_ImageBase;
				if (ImageBase == 0)
					ImageBase = 0x400000;

				if (PEFile.Open(ImageBase, 0x30, CDebugger::Read, gpSyser->m_pDebugger))
				{
					for (int nsec = 0; nsec < PEFile.m_SectCount; ++nsec)
					{
						if (PEFile.m_Section[nsec].Characteristics & 0x20000000)
						{
							FindSection(ImageBase +
									PEFile.m_Section[nsec].VirtualAddress,
									PEFile.m_Section[nsec].VirtualSize,
									PEFile.m_ImageBase,
									PEFile.m_ImageSize);
						}
					}
					PEFile.Close();
					return true;
				}
			}
			return false;
		}
	};

	void CStringReferenceList::InitContext(int,int)
	{
		char Buffer[256];
		char UBuffer[256*2];
		char ABuffer[512];
		unsigned long RLen;

		unsigned long ColorString[256];
		WCHAR wString[256];
		char aString[256];
		FindStringSection findStringSection;

		CCodeView *pCodeView = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
		if (!pCodeView) return;
	
		gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.clear();
		CListStringItem *pRootItem = pCodeView->FindItem(0, 8);
		ULONG_PTR CIP = pRootItem ? GetItemValue(pRootItem, 1) : (gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP ? *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP : 0);
		if (m_ShowFormat)
		{
			findStringSection.Find(CIP);
			for (map<ULONG_PTR,StringRef>::IT it = gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.begin();
				it != gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.end(); ++it)
			{
				bool bUnicode = false;
				pCodeView->ColorDasm(it->first, 16, ColorString, 0, 0, 0);
				CListStringItem *pItem = InsertItemValue(it->first, nullptr, WSTR("" F0ADDR "X"), 0, nullptr, -1); //TODO
				SetItemTextColor(pItem, 2, ColorString);
				SetItemValue(pItem, 1, (unsigned long long)(/*((unsigned long long)it->second.Len<<32)|*/it->second.Address)); //???
				SetItemValue(pItem, 0, (unsigned long long)it->first);
				unsigned long Len = it->second.Len;
				if (Len & 0x80000000)
				{
					bUnicode = true;
					Len &= ~0x80000000;
				}

				if (Len >= 255) Len = 255;

				if (bUnicode)
				{
					//wString[0] = 'L';
					//wString[1] = '"';
					//gpSyser->m_pDebugger->ReadMemory(it->second.Address, &wString[2], Len * 2);
					//wString[Len + 1] = '"';
					//wString[Len + 2] = 0;
					//wString[255] = 0;

					//gpSyser->m_SyserUI.ReadUString(it->second.Address, wString, Len);

					unsigned long result = gpSyser->m_pDebugger->ReadMemory(it->second.Address, Buffer, Len*2);
					UNormalize(Buffer, 256, UBuffer, 256*2, &RLen);
					int PrefixLen = TStrCpy(wString, "L\"");
					TStrCpyLimit(&wString[PrefixLen], (WCHAR*)UBuffer, 256 - PrefixLen - 1);
					TStrCat(wString, "\"");

					SetItemText(pItem, 1, WSTR("U"));
				} else
				{
					//aString[0] = 0;
					//gpSyser->m_pDebugger->ReadMemory(it->second.Address, aString, Len);
					//TStrCpyLimit(&wString[1], aString, Len);
					//wString[0] = '"';
					//TStrCat(wString, "\"");

					//gpSyser->m_SyserUI.ReadAString(it->second.Address, aString, Len);
					//TStrCpyLimit(wString, aString, Len);

					unsigned long result = gpSyser->m_pDebugger->ReadMemory(it->second.Address, Buffer, Len);
					ANormalize(Buffer, 256, ABuffer, 256, &RLen);
					int PrefixLen = TStrCpy(wString, "\"");
					TStrCpyLimit(&wString[PrefixLen], ABuffer, 256 - PrefixLen - 1);
					TStrCat(wString, "\"");

					SetItemText(pItem, 1, WSTR("C"));
				}
				SetItemText(pItem, 3, wString);
			}
		} else
		{
			map<ULONG_PTR,CListStringItem *> Lists;

			findStringSection.Find(gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP?*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP:0);
			for (map<ULONG_PTR,StringRef>::IT it = gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.begin();
				it != gpSyser->m_MainFrame.m_SystemExplorer.m_FoundStringRefMap.end(); ++it)
			{
				bool bUnicode = false;
				pCodeView->ColorDasm(it->first, 16, ColorString, 0, 0, 0);

				CListStringItem *pItem = NULL;
				map<ULONG_PTR,CListStringItem *>::IT fit = Lists.find(it->second.Address);
				if (fit == Lists.end())
				{
					pItem = InsertItemValue(it->first, nullptr, WSTR("" F0ADDR "X"), 0, nullptr, -1); //TODO
					Lists.insert(_Tmap_pair<ULONG_PTR,CListStringItem *>(it->first, pItem));
					SetItemValue(pItem, 1, (unsigned long long)(/*((unsigned long long)it->second.Len<<32)|*/it->second.Address)); //???
					SetItemValue(pItem, 0, (unsigned long long)(/*((unsigned long long)0x80000000<<32)|*/it->first));
					unsigned long Len = it->second.Len;
					if (Len & 0x80000000)
					{
						bUnicode = true;
						Len &= ~0x80000000;
					}

					if (Len >= 255) Len = 255;

					if (bUnicode)
					{
						//wString[0] = 'L';
						//wString[1] = '"';
						//gpSyser->m_pDebugger->ReadMemory(it->second.Address, &wString[2], Len * 2);
						//wString[Len + 1] = '"';
						//wString[Len + 2] = 0;
						//wString[255] = 0;

						unsigned long result = gpSyser->m_pDebugger->ReadMemory(it->second.Address, Buffer, Len*2);
						UNormalize(Buffer, 256, UBuffer, 256*2, &RLen);
						int PrefixLen = TStrCpy(wString, "L\"");
						TStrCpyLimit(&wString[PrefixLen], (WCHAR*)UBuffer, 256 - PrefixLen - 1);
						TStrCat(wString, "\"");
					} else
					{
						//aString[0] = 0;
						//gpSyser->m_pDebugger->ReadMemory(it->second.Address, aString, Len);
						//TStrCpyLimit(&wString[1], aString, Len);
						//wString[0] = '"';
						//TStrCat(wString, "\"");

						unsigned long result = gpSyser->m_pDebugger->ReadMemory(it->second.Address, Buffer, Len);
						ANormalize(Buffer, 256, ABuffer, 256, &RLen);
						int PrefixLen = TStrCpy(wString, "\"");
						TStrCpyLimit(&wString[PrefixLen], ABuffer, 256 - PrefixLen - 1);
						TStrCat(wString, "\"");
					}
					SetItemText(pItem, 1, wString);
				} else
				{
					pItem = fit->second;
				}
				pItem = InsertItemValue(it->first, pItem, WSTR("" F0ADDR "X"), 0, nullptr, -1);
				SetItemTextColor(pItem, 1, ColorString);
				SetItemValue(pItem, 0, (unsigned long long)it->first);
				//SetItemValue(pItem, 1, *(unsigned long long*)&it->second);
				SetItemValue(pItem, 1, (unsigned long long)it->second.Address);
			}
		}
	}

	bool CStringReferenceList::Search(const WCHAR *pString)
	{
		int nCol = (m_ShowFormat != 0)*2 + 1;
		if (pString)
			m_SearchString = pString;
		WCHAR Text[255];
		CListStringItem *pItem = GetNext(0, 0);
		while (pItem)
		{
			GetItemText(pItem, nCol, Text, 511);
			if (TStrIStr((Text[0]!='L')?&Text[1]:((Text[1]!='"')?&Text[1]:&Text[2]), m_SearchString.operator const WCHAR*())) break;
			pItem = GetNext(pItem, 0);
		}

		if (pItem)
		{
			m_nSearchLen = m_SearchString.m_nLength;
			Select(pItem);
			Scroll(pItem, -1);
			SendCommand(0, 1, 0);
		} else
		{
			SendCommand(0, 0, 0);
		}
		return true;
	}

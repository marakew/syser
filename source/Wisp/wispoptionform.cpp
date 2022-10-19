
#include "wispoptionform.hpp"
#include "wisprgbselect.hpp"
#include "wispmenu.hpp"
#include "wispbase.hpp"
#include "../Code/sysdep.hpp"

struct CWispSelectMenu : public CWispMenu
{
	int m_nID;

	CWispSelectMenu()
	{
		m_nID = -1;
	}

	virtual bool OnActiveMenu(CWispMenu::MenuItem *Item, int nID) override
	{
		m_nID = nID - 1;
		Close();
		return true;
	}

	bool LoadPopupMenuGroup(const WCHAR *Text[])
	{
		if (!CreateWnd(0, 0,0,0,0, 0, 0, 0x10006, WISP_SH_HIDDEN))
			return false;
		
		SetOwner(this);
		int PosX = 0;
		int PosY = 0;
		int Index = 0;
		while (Text[Index])
		{
			if (!TStrNCmp(Text[Index], WSTR("%d"), 2))// && Text[Index+1] = 'd')
				AppendMenu(-1, &Text[Index][2], 0x100000, Index+1, 0, 0);
			else	AppendMenu(-1, Text[Index+0],        0, Index+1, 0, 0);
				
			int TextExt = m_ClientDC.GetTextExtent(Text[Index], -1, 0);
			MIN_LIMIT(PosX, TextExt);
			PosY += m_SubMenuOpenHeight;
			++Index;
		}
		int Border = (m_BorderSize + m_SubMenuBorderSize)*2;
		Resize(PosX + Border + m_MenuOffset + m_SubMenuOffset, PosY + Border, true);
		return true;
	}

	static bool StaticCreate(int PosX, int PosY, const WCHAR *Text[], unsigned int *pnID, int Index)
	{
		CWispSelectMenu SelectMenu;
		SelectMenu.LoadPopupMenuGroup(Text);
		WISP_POINT Point;
		Point.x = PosX;
		Point.y = PosY;
		CWispBaseWnd *pWnd = SelectMenu.m_pWispBase->m_pFocusWnd;
		SelectMenu.Point(&Point, 0);
		if (Index >= 0)
		{
			CWispMenu::MenuItem *Item = SelectMenu.GetItemIndex(Index);
			if (Item)
				SelectMenu.UpdateItem(Item);
		}
		SelectMenu.Show(WISP_SH_MODAL_BLOCK);
		SelectMenu.Destroy();
		pWnd->Focus();
		if (SelectMenu.m_nID >= 0)
		{
			*pnID = SelectMenu.m_nID;
			return true;
		}
		return false;
	}
};


	void CWispOptionList::DrawContent(int nCol, CListStringItem::CONTENT *Content, WISP_RECT *pRect) //+94
	{
		OPTION_FORM *Option = (OPTION_FORM*)Content->Number;
		if (nCol == 1 && Option->Type == OPTION_COLOR)
		{
			m_ClientDC.DrawFullRect(pRect->x, pRect->y + 2, 20, pRect->cy - 4, *(unsigned long*)Option->NewValue);
			m_ClientDC.DrawRect(pRect->x, pRect->y + 2, 20, pRect->cy - 4, m_pWispBase->m_pWispDrawObj->m_crSystem[0]);
		}
		return CWispList::DrawContent(nCol, Content, pRect);
	}

#define ID_OPTLIST	0x40000000
#define ID_APPLY	0x40000001
#define ID_RESTORE	0x40000002
#define ID_DEFAULT	0x40000003

WISP_FORM_RES_ITEM WispOptionForm[] =
{
	{ WISP_CTRL_FORM, {0, 0, 540, 330}, 0, 0, 0, 0, 0},
	{ WISP_CTRL_BUTTON, {430, 300, 100, 20}, ID_APPLY, 0x200, WSTR("Apply"), 0, 0},
	{ WISP_CTRL_BUTTON, {10, 300, 100, 20}, ID_RESTORE, 0x200, WSTR("Restore"), 0, 0},
	{ WISP_CTRL_BUTTON, {120, 300, 100, 20}, ID_DEFAULT, 0, WSTR("Default"), 0, 0},
	{ 0 },
};

	CWispOptionForm::CWispOptionForm()
	{
		m_Resource = WispOptionForm;
	}

	CWispOptionForm::~CWispOptionForm()
	{
	}

	WISP_MSG_MAP_BEGIN(CWispOptionForm)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispForm)

	bool CWispOptionForm::OnCreateForm(WISP_MSG *pMsg)
	{
		m_OptionList.CreateWnd(0, 10,10,520,280, this, ID_OPTLIST, 0x46070006, WISP_SH_MINSIZE);
		m_pApply = (CWispButton *)GetFormItem(ID_APPLY);
		m_pRestore = (CWispButton *)GetFormItem(ID_RESTORE);
		m_pDefault = (CWispButton *)GetFormItem(ID_DEFAULT);
		m_OptionList.InsertColumn(WSTR("Option Item"), m_OptionList.m_ClientRect.cx / 2, 0, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0xF), -1);
		m_OptionList.InsertColumn(WSTR("Value"), m_OptionList.m_ClientRect.cx / 2, 1, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x1A), -1);
		LoadItems(m_OptionForm, 0);
		if (m_Style & 0x10000)
			m_pDefault->Show(WISP_SH_HIDDEN);
		return true;
	}

	void CWispOptionForm::SetText(OPTION_FORM *Option, CListStringItem *Item)
	{
		switch (Option->Type)
		{
		case OPTION_STRNUM:{
			m_OptionList.SetItemFormat(Item, 1, *(unsigned int*)Option->DefaultValue, WSTR("%d"));
			break;}
		case OPTION_INT:{
			int Pos = 0;
			if (!TStrNCmp(Option->SelectValue[*(unsigned int*)Option->DefaultValue], WSTR("%d"),2)) Pos=2;
			m_OptionList.SetItemText(Item, 1, &Option->SelectValue[*(unsigned int*)Option->DefaultValue][Pos]);
			break;}
		case OPTION_STR:{
		case OPTION_DIR:
			m_OptionList.SetItemText(Item, 1, (WCHAR*)Option->DefaultValue);
			break;}
		default:;
		}
	}

	void CWispOptionForm::LoadTexts(CListStringItem *ParentItem)
	{
		CListStringItem *It = m_OptionList.GetItem(0, ParentItem);
		while (It)
		{
			OPTION_FORM *Option = (OPTION_FORM *)m_OptionList.GetItemValue(It, 0);
			if (Option->Type == OPTION_SUB)
			{
				LoadTexts(It);
			} else
			{
				SetText(Option, It);
				memcpy(Option->NewValue, Option->DefaultValue, Option->Size);
			}
			It = m_OptionList.GetNext(It, 0);
		}
	}

	void CWispOptionForm::LoadItems(OPTION_FORM *Option, CListStringItem *ParentItem)
	{
		if (Option)
		while (Option->Name)
		{
			CListStringItem *Item = m_OptionList.InsertItemStr(Option->Name, ParentItem, 0, 0, -1);
			m_OptionList.SetItemValue(Item, 0, (unsigned long long)Option);
			m_OptionList.SetItemValue(Item, 1, (unsigned long long)Option);
			if (Option->Type == OPTION_SUB)
			{
				LoadItems((OPTION_FORM *)Option->DefaultValue, Item);
			} else
			{
				Option->NewValue = new unsigned char[Option->Size];
				memcpy(Option->NewValue, Option->DefaultValue, Option->Size);
			}
			++Option;
		}
	}

	bool CWispOptionForm::OnDestroy(WISP_MSG *pMsg)
	{
		Destroy(0);
		return true;
	}

	bool CWispOptionForm::OnSize(WISP_MSG *pMsg)
	{
		m_OptionList.Resize(m_ClientRect.cx - 20, m_ClientRect.cy - 50, true);
		m_pRestore->MoveToWindow(m_OptionList.m_ScrWindowRect.x - m_ScrWindowRect.x,
			m_OptionList.m_ScrWindowRect.y + m_OptionList.m_WindowRect.cy - m_ScrWindowRect.y + 10, true);
		if (m_pDefault->IsWindowShow())
			m_pDefault->MoveToWindow(m_pRestore->m_ScrWindowRect.x + m_pRestore->m_WindowRect.cx - m_ScrWindowRect.x + 10,
						m_pRestore->m_ScrWindowRect.y - m_ScrWindowRect.y, true);
		m_pApply->MoveToWindow(m_OptionList.m_ScrWindowRect.x + m_OptionList.m_ScrWindowRect.cx - m_pApply->m_WindowRect.cx - m_ScrWindowRect.x,
					m_pRestore->m_ScrWindowRect.y - m_ScrWindowRect.y, true);
		return true;
	}

	bool CWispOptionForm::OnCmdOptionList(WISP_MSG *pMsg)
	{
		switch (pMsg->Command.CmdMsg)
		{
		case 0x80000111:{ //WISP_ID_ITEM_EDITITING
			CListStringItem *Item = (CListStringItem *)pMsg->Command.Param2;
			OPTION_FORM *tmp = (OPTION_FORM *)m_OptionList.GetItemValue(Item, 0);
			if (!tmp) return false;
			WISP_RECT Rect;
			if (!m_OptionList.GetItemRect(Item, pMsg->Command.Param1, &Rect))
				return false;
			return GetFormInput(Rect.x + m_OptionList.m_ScrClientRect.x,
					Rect.y + Rect.cy + m_OptionList.m_ScrClientRect.y, Item, tmp);}
		case 0x80000112:{ //WISP_ID_ITEM_EDIT_LBUTTON
			CListStringItem *Item = (CListStringItem *)pMsg->Command.Param2;
			OPTION_FORM *tmp = (OPTION_FORM *)m_OptionList.GetItemValue(Item, 0);
			if (!tmp) return false;
			return Process(Item, tmp);}
		case 0x80000119:{ //WISP_ID_FORM_KEYDOWN
			if (pMsg->Command.Param1 == 0x71) //VK_F2
			{
				CListStringItem *Item = m_OptionList.FullFindItem(0, 8);
				if (Item && m_OptionList.GetItemValue(Item, 0))
					m_OptionList.EditItem(Item, 1);
			}}
		}
		return true;
	}

	WISP_CMD_MAP_BEGIN(CWispOptionForm)
		WISP_CMD_MAP(ID_OPTLIST, OnCmdOptionList)
	WISP_CMD_MAP_END

	WISP_EVENT_MAP_BEGIN(CWispOptionForm)
		WISP_EVENT_MAP(ID_APPLY, OnEventApply)
		WISP_EVENT_MAP(ID_RESTORE, OnEventRestore)
		WISP_EVENT_MAP(ID_DEFAULT, OnEventDefault)
	WISP_EVENT_MAP_END

	bool CWispOptionForm::Process(CListStringItem *Item, OPTION_FORM *Option)
	{
		if (Option->Type == OPTION_STR)
		{
			if (TStrCmp(m_OptionList.m_EditWnd.m_WndText.operator const WCHAR*(), (WCHAR*)Option->DefaultValue))
			{
				memset(Option->NewValue, 0, Option->Size);
				TStrCpyLimit((WCHAR*)Option->NewValue, m_OptionList.m_EditWnd.m_WndText.operator const WCHAR*(), Option->Size);
				OnNew(Item);
			} else
			if (TStrCmp(m_OptionList.m_EditWnd.m_WndText.operator const WCHAR*(), (WCHAR*)Option->NewValue))
			{
				memset(Option->NewValue, 0, Option->Size);
				TStrCpyLimit((WCHAR*)Option->NewValue, m_OptionList.m_EditWnd.m_WndText.operator const WCHAR*(), Option->Size);
				OnOld(Item);
			}			
		} else
		if (Option->Type == OPTION_STRNUM)
		{
			unsigned int Num;
			if (!USDecStrToNum(m_OptionList.m_EditWnd.m_WndText.operator const WCHAR*(), &Num))
				return false;
			if (Option->CheckValue && !Option->CheckValue(&Num))
				return false;
			if (Num != *(unsigned int*)Option->DefaultValue)
			{
				*(unsigned int*)Option->NewValue = Num;
				OnNew(Item);
			} else
			if (Num != *(unsigned int*)Option->NewValue)
			{
				*(unsigned int*)Option->NewValue = Num;
				OnOld(Item);
			}
		} else
		if (Option->Type == OPTION_DIR)
		{
			if (TStrCmp(m_OptionList.m_EditWnd.m_WndText.operator const WCHAR*(), (WCHAR*)Option->DefaultValue))
			{
				char szPath[MAX_FN_LEN];
				UnicodeToAnsi(m_OptionList.m_EditWnd.m_WndText.operator const WCHAR*(), szPath, MAX_FN_LEN);
				if (!gpFileIO->IsDir(szPath))
					return false;
				
				memset(Option->NewValue, 0, Option->Size);
				TStrCpyLimit((WCHAR*)Option->NewValue, m_OptionList.m_EditWnd.m_WndText.operator const WCHAR*(), Option->Size);
				OnNew(Item);
			} else
			if (TStrCmp(m_OptionList.m_EditWnd.m_WndText.operator const WCHAR*(), (WCHAR*)Option->NewValue))
			{
				memset(Option->NewValue, 0, Option->Size);
				TStrCpyLimit((WCHAR*)Option->NewValue, m_OptionList.m_EditWnd.m_WndText.operator const WCHAR*(), Option->Size);
				OnOld(Item);
			}			
		}
		return true;
	}

	bool CWispOptionForm::OnEventApply(WISP_MSG *pMsg)
	{
		if (OnAppling())
		{
			EventApply(0);
			m_pApply->EnableWindow(false);
			m_pRestore->EnableWindow(false);
			m_nChanges = 0;
			SendCommand(0x40000000, 0, 0);
			OnApply();
			Update(&m_ClientRect);
		}
		return true;
	}

	bool CWispOptionForm::OnEventRestore(WISP_MSG *pMsg)
	{
		if (OnRestoring())
		{
			EventRestore(0);
			m_pApply->EnableWindow(false);
			m_pRestore->EnableWindow(false);
			m_nChanges = 0;
			OnRestore();
			Update(&m_ClientRect);
		}
		return true;
	}

	bool CWispOptionForm::OnEventDefault(WISP_MSG *pMsg)
	{
		if (OnDefaulting())
		{
			EventDefault(0);
			m_pApply->EnableWindow(false);
			m_pRestore->EnableWindow(false);
			m_pDefault->EnableWindow(false);
			m_nChanges = 0;
			OnDefault();
			Update(&m_ClientRect);
		}
		return true;
	}

	void CWispOptionForm::EventApply(CListStringItem *ParentItem)
	{
		CListStringItem *It = m_OptionList.GetItem(0, ParentItem);
		while (It)
		{
			OPTION_FORM *Option = (OPTION_FORM *)m_OptionList.GetItemValue(It, 0);
			if (Option->Type == OPTION_SUB)
			{
				EventApply(It);
			} else
			if (memcmp(Option->DefaultValue, Option->NewValue, Option->Size) != 0)
			{
				memcpy(Option->DefaultValue, Option->NewValue, Option->Size);
				if (Option->Update)
					++*Option->Update;
				SetText(Option, It);
				m_OptionList.SetItemColor(It, -1, m_OptionList.m_ItemColor);
			}
			It = m_OptionList.GetNext(It, 0);
		}
	}

	void CWispOptionForm::EventRestore(CListStringItem *ParentItem)
	{
		CListStringItem *It = m_OptionList.GetItem(0, ParentItem);
		while (It)
		{
			OPTION_FORM *Option = (OPTION_FORM *)m_OptionList.GetItemValue(It, 0);
			if (Option->Type == OPTION_SUB)
			{
				EventRestore(It);
			} else
			if (memcmp(Option->DefaultValue, Option->NewValue, Option->Size) != 0)
			{
				memcpy(Option->NewValue, Option->DefaultValue, Option->Size);
				SetText(Option, It);
				m_OptionList.SetItemColor(It, -1, m_OptionList.m_ItemColor);
			}
			It = m_OptionList.GetNext(It, 0);
		}
	}

	void CWispOptionForm::EventDefault(CListStringItem *ParentItem)
	{
		CListStringItem *It = m_OptionList.GetItem(0, ParentItem);
		while (It)
		{
			OPTION_FORM *Option = (OPTION_FORM *)m_OptionList.GetItemValue(It, 0);
			if (Option->Type == OPTION_SUB)
			{
				EventDefault(It);
			} else
			{
				memcpy(Option->NewValue, Option->DefaultValue, Option->Size);
				SetText(Option, It);
				m_OptionList.SetItemColor(It, -1, m_OptionList.m_ItemColor);
			}
			It = m_OptionList.GetNext(It, 0);
		}
	}

	void CWispOptionForm::Destroy(CListStringItem *ParentItem)
	{
		CListStringItem *It = m_OptionList.GetItem(0, ParentItem);
		while (It)
		{
			OPTION_FORM *Option = (OPTION_FORM *)m_OptionList.GetItemValue(It, 0);
			if (Option->Type == OPTION_SUB)
			{
				Destroy(It);
			} else
			{
				delete [](unsigned char*)Option->NewValue; //???
			}
			It = m_OptionList.GetNext(It, 0);
		}
	}

	bool CWispOptionForm::GetFormInput(int PosX, int PoxY, CListStringItem *Item, OPTION_FORM *pOptionGroup)
	{
		switch (pOptionGroup->Type)
		{
		case OPTION_SUB:
			return false;

		case OPTION_INT:{
			unsigned int nID;
			if (!CWispSelectMenu::StaticCreate(PosX, PoxY, pOptionGroup->SelectValue, &nID, *(unsigned int*)pOptionGroup->DefaultValue))
				return false;
			if (*(unsigned int*)pOptionGroup->DefaultValue == nID)
			{
				if (*(unsigned int*)pOptionGroup->NewValue != nID)
				{	
					*(unsigned int*)pOptionGroup->NewValue = nID;
					m_OptionList.SetItemText(Item, 1, pOptionGroup->SelectValue[nID]);
					OnOld(Item);
				}
			} else
			if (!pOptionGroup->CheckValue || !pOptionGroup->CheckValue(&nID))
			{
				*(unsigned int*)pOptionGroup->NewValue = nID;
				m_OptionList.SetItemText(Item, 1, pOptionGroup->SelectValue[nID]);
				OnNew(Item);
			}
			
			return false;
			}
		case OPTION_DIR:{
			unsigned int nID;
			if (!CWispSelectMenu::StaticCreate(PosX, PoxY, pOptionGroup->SelectValue, &nID, -1))
				return false;
			WCHAR FileName[MAX_FN_LEN];
			if (nID == 0)
			{
				if (!m_pWispBase->GetOpenFolderName(FileName, m_OptionList.GetItemContent(Item, 0)->String.operator const WCHAR*()))
					return false;

				if (TStrCmp(FileName, (WCHAR*)pOptionGroup->DefaultValue))
				{
					memset(pOptionGroup->NewValue, 0, pOptionGroup->Size);
					TStrCpyLimit((WCHAR*)pOptionGroup->NewValue, FileName, pOptionGroup->Size);
					OnNew(Item);
				} else
				if (TStrCmp(FileName, (WCHAR*)pOptionGroup->NewValue))
				{
					memset(pOptionGroup->NewValue, 0, pOptionGroup->Size);
					TStrCpyLimit((WCHAR*)pOptionGroup->NewValue, FileName, pOptionGroup->Size);
					OnOld(Item);
				}			
				return false;
			} else
			if (nID == 2)
			{
				char szPath[MAX_FN_LEN];
				GetModulePath(szPath, false);
				AnsiToUnicode(szPath, FileName, lenof(FileName));
				m_OptionList.m_EditWnd.m_WndText = FileName;
			}
			return true;
			}
		case OPTION_COLOR:{
			if (!CWispRGBSelect::StaticCreate((unsigned long*)pOptionGroup->NewValue, (unsigned long*)pOptionGroup->DefaultValue))
				return false;

			if (*(unsigned long*)pOptionGroup->NewValue != *(unsigned long*)pOptionGroup->DefaultValue)
			{
				OnNew(Item);
			}
			return false;
			}
		}
		return true;
	}

	bool CWispOptionForm::OnAppling() { return true; }
	void CWispOptionForm::OnApply() {}

	bool CWispOptionForm::OnRestoring() { return true; }
	void CWispOptionForm::OnRestore() {}

	bool CWispOptionForm::OnDefaulting() { return true; }
	void CWispOptionForm::OnDefault() {}


	void CWispOptionForm::OnNew(CListStringItem *Item)
	{
		m_OptionList.SetItemColor(Item, -1, m_pWispBase->m_pWispDrawObj->m_crSystem[2]);
		++m_nChanges;
		if ((m_pApply->m_Style >> 9) & 1)	m_pApply->EnableWindow(1);
		if ((m_pRestore->m_Style >> 9) & 1)	m_pRestore->EnableWindow(1);
		if ((m_pDefault->m_Style >> 9) & 1)	m_pDefault->EnableWindow(1);
	}
	void CWispOptionForm::OnOld(CListStringItem *Item)
	{
		m_OptionList.SetItemColor(Item, -1, m_OptionList.m_ItemColor);
		if (m_nChanges-- == 1)
		{
			if ((m_pApply->m_Style >> 9) & 1)	m_pApply->EnableWindow(0);
			if ((m_pRestore->m_Style >> 9) & 1)	m_pRestore->EnableWindow(0);
		}
	}

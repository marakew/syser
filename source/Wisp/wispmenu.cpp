
//#include "../Code/osdefine.hpp"
#include "../Code/define.h"

#include "wispmenu.hpp"
#include "wispbase.hpp"

#define SUBMENU(Item)	((CWispMenu*)(Item->dwData))

	WISP_MSG_MAP_BEGIN(CWispMenu)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
		WISP_MSG_MAP(WISP_WM_RECALC_LAYOUT, OnRecalcLayout)
		WISP_MSG_MAP(WISP_WM_RESIZE_CHILD, OnResizeChild)
		WISP_MSG_MAP(WISP_WM_LOST_FOCUS, OnLostFocus)
		WISP_MSG_MAP(WISP_WM_UPDATE_BORDER, OnUpdateBorder)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_MOUSE_MOVE, OnMouseMove)
		WISP_MSG_MAP(WISP_WM_MOUSE_LEAVE, OnMouseLeave)
	WISP_MSG_MAP_END(CWispWnd)

	bool CWispMenu::OnUpdateClient(WISP_MSG *pMsg)
	{
		if (m_Style & 0x10000) //POPUP MENU
			DrawPopupMenu();
		else	DrawMenu();
		return false;
	}
	bool CWispMenu::OnRecalcLayout(WISP_MSG *pMsg)
	{
		if (m_Style & 0x10000) //POPUP MENU
		{
			int y = m_SubMenuBorderSize;
			TListIter<MenuItem> It = m_MenuItemList.Begin();
			while (It != m_MenuItemList.End())
			{
				It->Rect.x = 0;
				It->Rect.y = y;
				It->Rect.cx = m_ClientRect.cx;
				if (It->nFlags & 4) //MENU_DELIM
					It->Rect.cy = 1;
				else	It->Rect.cy = m_SubMenuOpenHeight;
				y += It->Rect.cy;
				++It;
			}
		} else
		{
			int x = 0;
			TListIter<MenuItem> It = m_MenuItemList.Begin();
			while (It != m_MenuItemList.End())
			{
				It->Rect.x = x;
				It->Rect.y = (m_FullMenuHeight - m_OpenMenuHeight)/2;
				It->Rect.cy = m_OpenMenuHeight;
				if (It->nFlags & 4) //MENU_DELIM
				{
					It->Rect.cx = m_SubMenuBorderSize*2 + 1;
				} else
				{
					It->Rect.cx = m_ClientDC.GetTextExtent(It->string.operator const WCHAR*(), -1, 0) + m_SubMenuBorderSize*2;
					if (It->nFlags & 2) //MENU_CHECKBOX
						It->Rect.cx += m_SubMenuBorderSize + 11;
					else
					if (It->pDIB)
						It->Rect.cx += m_SubMenuBorderSize + It->pDIB->m_PaintRect.cx;
				}
				x += It->Rect.cx;
				++It;
			}
			m_FullMenuWidth = x;
		}
		return true;
	}
	bool CWispMenu::OnResizeChild(WISP_MSG *pMsg)
	{
		if (!(m_Style & 0x10000)) //POPUP MENU
			Resize(((CWispWnd*)m_ParentWnd)->m_ClientRect.cx, m_WindowRect.cy, true);
		return true;
	}
	bool CWispMenu::OnLostFocus(WISP_MSG *pMsg)
	{
		CWispMenu *pRootMenu = GetRootMenu();
		if (!pRootMenu->CheckExist((CWispMenu*)pMsg->hOtherWnd))
		{
			if (pRootMenu != pMsg->hOtherWnd)
				pRootMenu->HideMenu();
			m_CurMenuItemIter = m_MenuItemList.End();
		}
		return true;
	}
	bool CWispMenu::OnUpdateBorder(WISP_MSG *pMsg)
	{
		if (pParentMenu && !(pParentMenu->m_Style & 0x10000)) //POPUP MENU
		{
			m_WindowDC.DrawHLine(pParentMenu->m_CurMenuItemIter->Rect.cx - 1, m_WindowRect.cx - 1, 0, m_pWispBase->m_pWispDrawObj->m_crSystem[52]);
			m_WindowDC.DrawHLine(1, m_CurMenuItemIter->Rect.cx - 2, 0, m_pWispBase->m_pWispDrawObj->m_crSystem[50]);
			m_WindowDC.DrawHLine(0, m_WindowRect.cx - 1, m_WindowRect.cy - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[52]);
			m_WindowDC.DrawVLine(0, 0, m_WindowRect.cy - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[52]);
			m_WindowDC.DrawVLine(m_WindowRect.cx - 1, 0, m_WindowRect.cy - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[52]);
		} else
		{
			m_WindowDC.DrawRect(&m_WindowRect, m_pWispBase->m_pWispDrawObj->m_crSystem[52]);
		}
		return false;
	}
	bool CWispMenu::OnDestroy(WISP_MSG *pMsg)
	{
		TListIter<MenuItem> It = m_MenuItemList.Begin();
		while (It != m_MenuItemList.End())
		{
			if (It->nFlags & 1) //MENU_SUBMENU
				((CWispMenu*)It->dwData)->Destroy();
			++It;
		}
		m_MenuItemList.Clear();
		return true;
	}
	bool CWispMenu::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (!m_MenuItemList.Size() || !pMsg->KeyEvent.bKeyDown)
			return true;

		switch (pMsg->KeyEvent.KeyType)
		{
		case VK_LBUTTON:
			if (PtInRect(&m_WindowRect, &pMsg->m_LogicMousePosWnd))
			{
				if (m_bShowed && m_CurMenuItemIter == m_MenuItemList.End())
				{
					m_bShowed = 0;
					HideMenu();
				}
				if (m_CurMenuItemIter != m_MenuItemList.End())
				{
					if (!(m_CurMenuItemIter->nFlags & 0x100000) && //MENU_DISABLED
						!(m_CurMenuItemIter->nFlags & 4)) //MENU_DELIM
					{
						if (m_CurMenuItemIter->nFlags & 1) //MENU_SUBMENU
						{
							if (m_bShowed)
							{
								m_bShowed = 0;
								TListIter<MenuItem> It = m_CurMenuItemIter; //???
								HideMenu();
								m_CurMenuItemIter = It; //???
							} else
							{
								m_bShowed = 1;
								Open(&*m_CurMenuItemIter, 0);
								Update(&m_CurMenuItemIter->Rect);
							}
						} else
						{
							SwitchItem(&*m_CurMenuItemIter);
						}
					}
				}
			} else
			{
				m_bShowed = 0;
				HideMenu();
			}
			break;
		case VK_RETURN:
			if (m_CurMenuItemIter != m_MenuItemList.End())
			{
				if (!(m_CurMenuItemIter->nFlags & 1)) //MENU_SUBMENU
					SwitchItem(&*m_CurMenuItemIter);
				else	Open(&*m_CurMenuItemIter, 1);
			}
			break;			
		case VK_ESCAPE:
			if (pParentMenu)
			{
				HideMenu();
				pParentMenu->Focus();
			} else
			if (m_OwnerWnd != this)
				m_OwnerWnd->Focus();
			else	HideMenu();
			break;
		case VK_LEFT:
			if (!(m_Style & 0x10000)) //POPUP MENU
			{
				PrevItem();
			} else
			if (pParentMenu)
			{
				HideMenu();
				pParentMenu->Focus();
			}
			break;
		case VK_UP:
			if (m_Style & 0x10000) //POPUP MENU
			{
				PrevItem();
			} else
			if (pParentMenu)
			{
				HideMenu();
				pParentMenu->Focus();
			}
			break;
		case VK_RIGHT:
			if (!(m_Style & 0x10000)) //POPUP MENU
			{
				NextItem();
			} else
			if (m_CurMenuItemIter != m_MenuItemList.End() && (m_CurMenuItemIter->nFlags & 1)) //MENU_SUBMENU
				Open(&*m_CurMenuItemIter, 1);
			break;
		case VK_DOWN:
			if (m_Style & 0x10000) //POPUP MENU
			{
				NextItem();
			} else
			if (m_CurMenuItemIter != m_MenuItemList.End() && (m_CurMenuItemIter->nFlags & 1)) //MENU_SUBMENU
				Open(&*m_CurMenuItemIter, 1);
			break;
		default:;
		}
		return true;
	}
	bool CWispMenu::OnMouseMove(WISP_MSG *pMsg)
	{
		TListIter<MenuItem> It = m_MenuItemList.Begin();
		while (It != m_MenuItemList.End())
		{
			if (PtInRect(&It->Rect, &pMsg->m_LogicMousePosWnd))
			{
				if (m_CurMenuItemIter != It)
					UpdateIter(It);
				break;
			}
			++It;
		}
		if (It == m_MenuItemList.End() && !(m_Style & 0x10000) && m_CurMenuItemIter != It) //POPUP MENU
			UpdateIter(It);
		return true;
	}
	bool CWispMenu::OnMouseLeave(WISP_MSG *pMsg)
	{
		if (!(m_Style & 0x10000) && m_CurMenuItemIter != m_MenuItemList.End() && !m_bShowed) //POPUP MENU
			UpdateIter(m_MenuItemList.End());
		return true;
	}

	bool CWispMenu::CheckExist(CWispMenu *pMenu)
	{
		if (pMenu->m_CtrlType != WISP_CTRL_MENU)
			return false;
		while ((pMenu = pMenu->pParentMenu))
			if (pMenu == this) return true;
		return false;
	}
	CWispMenu *CWispMenu::GetRootMenu()
	{
		CWispMenu *parent = this;
		while (parent && parent->pParentMenu)
			parent = parent->pParentMenu;
		return parent;
	}

	bool CWispMenu::SetDIB(CWispMenu::MenuItem *Item, CWispDIB *pDIB)
	{
		Item->pDIB = pDIB;
		return true;
	}
	bool CWispMenu::SetData(CWispMenu::MenuItem *Item, unsigned long long dwData)
	{
		Item->dwData = dwData;
		return true;
	}

	bool CWispMenu::CheckedItem(CWispMenu::MenuItem *Item)
	{
		if (!(Item->nFlags & 0x80000)) //MENU_CHECKED
		{
			WISP_MSG Msg;
			Msg.hWnd = m_OwnerWnd;
			Msg.Msg = WISP_WM_COMMAND;
			Msg.Command.CmdMsg = 0x80000108; //WISP_ID_CHECKING
			Msg.Command.CmdID = m_CmdID;
			Msg.Command.Param1 = 0;
			Msg.Command.Param2 = Item;
			if (!m_pWispBase->SendMessage(&Msg))
				return false;
			Item->nFlags |= 0x80000; //MENU_CHECKED
			Msg.Command.CmdMsg = 0x80000109; //WISP_ID_CHECKED
			Msg.Command.Param1 = 1;
			m_pWispBase->SendMessage(&Msg);
			Update((WISP_RECT*)0);
		}
		return true;
	}
	bool CWispMenu::UnCheckedItem(CWispMenu::MenuItem *Item)
	{
		if ((Item->nFlags & 0x80000)) //MENU_CHECKED
		{
			WISP_MSG Msg;
			Msg.hWnd = m_OwnerWnd;
			Msg.Msg = WISP_WM_COMMAND;
			Msg.Command.CmdMsg = 0x8000010A; //WISP_ID_UNCHECKING
			Msg.Command.CmdID = m_CmdID;
			Msg.Command.Param1 = 1;
			Msg.Command.Param2 = Item;
			if (!m_pWispBase->SendMessage(&Msg))
				return false;
			Item->nFlags &= ~0x80000; //MENU_CHECKED
			Msg.Command.CmdMsg = 0x8000010B; //WISP_ID_UNCHECKED
			Msg.Command.Param1 = 0;
			m_pWispBase->SendMessage(&Msg);
			Update((WISP_RECT*)0);
		}
		return true;
	}

	bool CWispMenu::EnableItem(CWispMenu::MenuItem *Item)
	{
		if ((Item->nFlags & 0x100000)) //MENU_DISABLED
		{
			Item->nFlags &= ~0x100000; //MENU_DISABLED
			WISP_MSG Msg;
			Msg.hWnd = m_OwnerWnd;
			Msg.Msg = WISP_WM_COMMAND;
			Msg.Command.CmdMsg = 0x8000010F; //WISP_ID_ENABLE
			Msg.Command.CmdID = m_CmdID;
			Msg.Command.Param1 = 0;
			Msg.Command.Param2 = Item;
			m_pWispBase->SendMessage(&Msg);
			Update((WISP_RECT*)0);
		}
		return true;
	}
	bool CWispMenu::DisableItem(CWispMenu::MenuItem *Item)
	{
		if (!(Item->nFlags & 0x100000)) //MENU_DISABLED
		{
			Item->nFlags |= 0x100000; //MENU_DISABLED
			WISP_MSG Msg;
			Msg.hWnd = m_OwnerWnd;
			Msg.Msg = WISP_WM_COMMAND;
			Msg.Command.CmdMsg = 0x80000110; //WISP_ID_DISABLE
			Msg.Command.CmdID = m_CmdID;
			Msg.Command.Param1 = 1;
			Msg.Command.Param2 = Item;
			m_pWispBase->SendMessage(&Msg);
			Update((WISP_RECT*)0);
		}
		return true;
	}

	bool CWispMenu::Enable(CWispMenu::MenuItem *Item, bool bEnable)
	{
		if (bEnable)
			return EnableItem(Item);
		return DisableItem(Item);
	}

	bool CWispMenu::Checked(CWispMenu::MenuItem *Item)
	{
		if (Item->nFlags & 0x80000) //MENU_CHECKED
		{
			UnCheckedItem(Item);
			return true;
		}
		CheckedItem(Item);
		return false;
	}

	bool CWispMenu::InitWnd()
	{
		if (!CWispWnd::InitWnd())
			return false;

		m_CurMenuItemIter = m_MenuItemList.End();
		pParentMenu = 0;
		m_bShowed = 0;
		m_FullMenuWidth = 0;
		m_BorderSize = 1;
		return true;
	}

	void CWispMenu::DrawMenu()
	{
		if (!(m_Style & 0x40))
			m_pWispBase->m_pWispDrawObj->DrawMenuItem(&m_ClientDC, &m_ClientRect, 0);

		TListIter<MenuItem> It = m_MenuItemList.Begin();
		while (It != m_MenuItemList.End())
		{
			if (It->nFlags & 4) //MENU_DELIM
			{
				m_ClientDC.DrawVLine(m_SubMenuBorderSize + It->Rect.x, It->Rect.y, It->Rect.y + It->Rect.cy - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[52]);
			} else
			{
				WISP_RECT Rect;
				Rect.x = It->Rect.x + m_pWispBase->m_Metrics[2]; //WISP_WSTYLE_BORDER
				Rect.y = It->Rect.y;
				Rect.cx = It->Rect.cx - m_SubMenuBorderSize * 2;
				Rect.cy = It->Rect.cy;

				int State = It == m_CurMenuItemIter;

				if (State)
				{
					if (m_bShowed && It->nFlags & 1) //MENU_SUBMENU
					{
						m_ClientDC.DrawHLine(It->Rect.x, It->Rect.x + It->Rect.cx - 1, It->Rect.y, m_pWispBase->m_pWispDrawObj->m_crSystem[52]);
						m_ClientDC.DrawVLine(It->Rect.x, It->Rect.y, It->Rect.y + It->Rect.cy - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[52]);
						m_ClientDC.DrawVLine(It->Rect.x + It->Rect.cx - 1, It->Rect.y, It->Rect.y + It->Rect.cy - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[52]);
						if (((CWispMenu*)It->dwData)->m_WindowRect.cx < It->Rect.cx)
							m_ClientDC.DrawHLine(It->Rect.x + It->Rect.cx - 1, It->Rect.x + ((CWispMenu*)It->dwData)->m_WindowRect.cx - 1, It->Rect.y + It->Rect.cy - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[52]);
					} else
					if (!(It->nFlags & 0x100000)) //MENU_DISABLED
					{
						m_ClientDC.DrawFullRect(&It->Rect, m_pWispBase->m_pWispDrawObj->m_crSystem[44]);
						m_ClientDC.DrawRect(&It->Rect, m_pWispBase->m_pWispDrawObj->m_crSystem[49]);
					}
					State = 1;
				}

				if (It->nFlags & 2) //MENU_CHECKBOX
				{
					WISP_RECT RC;
					RC.x = Rect.x;
					RC.y = Rect.y;
					RC.cx = 11;
					RC.cy = Rect.cy;
					m_pWispBase->m_pWispDrawObj->DrawSystemStandardIcon(&m_ClientDC, &RC, ((It->nFlags & 0x80000) != 0) + 11, 0); //MENU_CHECKED WISP_SSI_CHECKBOX_UNCHECKED/WISP_SSI_CHECKBOX_CHECKED
					Rect.x += m_SubMenuBorderSize + 11;
					Rect.cx -= m_SubMenuBorderSize + 11;
				} else
				if (It->pDIB)
				{
					WISP_RECT RC;
					RC.x = Rect.x;
					RC.y = Rect.y;
					RC.cx = It->pDIB->m_PaintRect.cx;
					RC.cy = Rect.cy;
					if (It->nFlags & 0x100000) //MENU_DISABLED
						m_ClientDC.DrawDIBGray(&RC, It->pDIB);
					else	m_ClientDC.DrawDIB(&RC, It->pDIB);
					Rect.x += m_SubMenuBorderSize + It->pDIB->m_PaintRect.cx;
					Rect.cx -= m_SubMenuBorderSize + It->pDIB->m_PaintRect.cx;
				} else
				{
					Rect.x = m_SubMenuBorderSize/2 + Rect.x;
				}

				if (It->nFlags & 0x100000) //MENU_DISABLED
					State = 0x40;
				if (It->Color)
				{
					m_ClientDC.SetTextColor(It->Color);
					m_pWispBase->m_pWispDrawObj->DrawMenuItemText(&m_ClientDC, &Rect, It->string.operator const WCHAR*(), State);
					m_ClientDC.SetTextColor();
				} else
				{
					m_pWispBase->m_pWispDrawObj->DrawMenuItemText(&m_ClientDC, &Rect, It->string.operator const WCHAR*(), State);
				}
			}
			++It;
		}
		m_ClientDC.DrawHLine(0, m_ClientRect.cx - 1, m_ClientRect.cy - 1, m_pWispBase->m_pWispDrawObj->m_crSystem[28]);
	}
	void CWispMenu::DrawPopupMenu()
	{
		WISP_RECT Rect;
		Rect.y = 0;
		Rect.x = 0;
		Rect.cx = m_MenuBorderSize;
		Rect.cy = m_ClientRect.cy;

		WISP_RECT RC;
		RC.x = m_MenuOffset;
		RC.y = m_MenuHeight;
		RC.cx = m_ClientRect.cx - m_MenuOffset;
		RC.cy = m_SubMenuOpenHeight;

		int DEEP = m_ClientRect.cx - m_SubMenuBorderSize*2 - m_MenuOffset - m_SubMenuOffset;

		m_pWispBase->m_pWispDrawObj->X88(&m_ClientDC, &Rect);

		TListIter<MenuItem> It = m_MenuItemList.Begin();
		while (It != m_MenuItemList.End())
		{
			RC.cy = It->Rect.cy;
			if (It->nFlags & 4) //MENU_DELIM
			{
				m_ClientDC.DrawHLine(m_MenuBorderSize, m_ClientRect.cx-1, RC.y, m_pWispBase->m_pWispDrawObj->m_crSystem[52]);
			} else
			{
				Rect.cy = It->Rect.cy;
				Rect.y = RC.y;
				int State = It == m_CurMenuItemIter;
				m_pWispBase->m_pWispDrawObj->DrawMenuItem(&m_ClientDC, &RC, State);
				Rect.x = m_MenuOffset + m_SubMenuBorderSize;
				Rect.cx = DEEP;
				if (It->nFlags & 0x100000) //MENU_DISABLED
					State = 0x40;
				if (It->Color)
				{
					m_ClientDC.SetTextColor(It->Color);
					m_pWispBase->m_pWispDrawObj->DrawMenuItemText(&m_ClientDC, &Rect, It->string.operator const WCHAR*(), State);
					m_ClientDC.SetTextColor();
				} else
				{
					m_pWispBase->m_pWispDrawObj->DrawMenuItemText(&m_ClientDC, &Rect, It->string.operator const WCHAR*(), State);
				}

				if (It->nFlags & 2) //MENU_CHECKBOX
				{
					Rect.x = 0;
					Rect.cx = m_MenuOffset;
					m_pWispBase->m_pWispDrawObj->DrawSystemStandardIcon(&m_ClientDC, &Rect, ((It->nFlags & 0x80000) != 0) + 11, State); //MENU_CHECKED WISP_SSI_CHECKBOX_UNCHECKED/WISP_SSI_CHECKBOX_CHECKED
				} else
				if (It->pDIB)
				{
					Rect.x = 0;
					Rect.cx = m_MenuOffset;
					if (It->nFlags & 0x100000) //MENU_DISABLED
						m_ClientDC.DrawDIBGray(&Rect, It->pDIB);
					else	m_ClientDC.DrawDIB(&Rect, It->pDIB);
				}
				
				if (It->nFlags & 1) //MENU_SUBMENU
				{
					Rect.x = m_ClientRect.cx - m_SubMenuOffset;
					Rect.cx = m_SubMenuOffset;
					m_pWispBase->m_pWispDrawObj->DrawMenuItemText(&m_ClientDC, &Rect, WSTR(">>"), 0);
				}
			}
			RC.y += It->Rect.cy;
			++It;
		}

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = m_ClientRect.cx;
		Rect.cy = m_MenuHeight;
		m_pWispBase->m_pWispDrawObj->X94(&m_ClientDC, &Rect);

		Rect.x = 0;
		Rect.y = RC.y;
		Rect.cx = m_ClientRect.cx;
		Rect.cy = m_SubMenuHeight;
		m_pWispBase->m_pWispDrawObj->X98(&m_ClientDC, &Rect);
	}

	void CWispMenu::NextItem()
	{
		TListIter<MenuItem> It = m_CurMenuItemIter;
		if (It != m_MenuItemList.End() &&
			It+1 != m_MenuItemList.End())
		{
			++It;
		} else
		{
			It = m_MenuItemList.Begin();
			while (It != m_MenuItemList.End())
			{
				if (!(It->nFlags & 4)) //MENU_DELIM
					break;
				++It;
			}
		}
		m_CurMenuItemIter = It;
		Update((WISP_RECT*)0);
	}
	void CWispMenu::PrevItem()
	{
		TListIter<MenuItem> It = m_CurMenuItemIter;
		if (It != m_MenuItemList.End() &&
			It-1 != m_MenuItemList.End())
		{
			--It;
		} else
		{
			It = m_MenuItemList.End()-1;
			while (It != m_MenuItemList.End())
			{
				if (!(It->nFlags & 4)) //MENU_DELIM
					break;
				--It;
			}
		}
		m_CurMenuItemIter = It;
		Update((WISP_RECT*)0);
	}
	void CWispMenu::Adjust()
	{
		int PosX = 0;
		int PosY = 0;
		TListIter<MenuItem> It = m_MenuItemList.Begin();
		while (It != m_MenuItemList.End())
		{
			int TextExt = m_ClientDC.GetTextExtent(It->string.operator const WCHAR*(), -1, 0);
			MIN_LIMIT(PosX, TextExt);

			if (It->nFlags & 4) //MENU_DELIM
				++PosY;
			else	PosY += m_SubMenuOpenHeight;
			++It;
		}
		int Border = (m_BorderSize + m_SubMenuBorderSize)*2;
		Resize(PosX + Border + m_MenuOffset + m_SubMenuOffset, Border + PosY, true);
		RecalcLayout();
	}

	void CWispMenu::SetOwner(CWispBaseWnd *pWnd)
	{
		CWispBaseWnd::SetOwner(pWnd);
		TListIter<MenuItem> It = m_MenuItemList.Begin();
		while (It != m_MenuItemList.End())
		{
			if (It->nFlags & 1) //MENU_SUBMENU
				((CWispMenu*)It->dwData)->SetOwner(pWnd);
			++It;
		}
	}

	CWispMenu::MenuItem *CWispMenu::GetItemIndex(int Pos)
	{
		if (Pos < 0 || Pos >= m_MenuItemList.Size())
			return nullptr;
		return &*m_MenuItemList[Pos];
	}

	CWispMenu::MenuItem *CWispMenu::GetItem(const WCHAR *Text)
	{
		TListIter<MenuItem> It = m_MenuItemList.Begin();
		while (It != m_MenuItemList.End())
		{
			if (It->string.operator const WCHAR*())
			{
				if (!TStrCmp(Text, It->string.operator const WCHAR*()))
					return &*It;
			}

			if (It->nFlags & 1) //MENU_SUBMENU
			{
				CWispMenu::MenuItem *Item = ((CWispMenu*)It->dwData)->GetItem(Text);
				if (Item) return Item;
			}
			++It;
		}
		return nullptr;
	}
	CWispMenu::MenuItem *CWispMenu::GetItem(int nID)
	{
		TListIter<MenuItem> It = m_MenuItemList.Begin();
		while (It != m_MenuItemList.End())
		{
			if (It->nFlags & 1) //MENU_SUBMENU
			{
				CWispMenu::MenuItem *Item = ((CWispMenu*)It->dwData)->GetItem(nID);
				if (Item) return Item;
			} else
			if (It->nID == nID)
			{
				return &*It;
			}
			++It;
		}
		return nullptr;
	}

	bool CWispMenu::SetItemText(CWispMenu::MenuItem *Item, const WCHAR *Text)
	{
		int PosX = m_MenuOffset + m_SubMenuOffset + m_ClientDC.GetTextExtent(Text, -1, 0) + (m_BorderSize + m_SubMenuBorderSize)*2;
		if (PosX > m_WindowRect.cx)
			Resize(PosX, m_WindowRect.cy, true);
		Item->string = Text;
		return true;
	}

	bool CWispMenu::CheckedItem(int nID)
	{
		CWispMenu::MenuItem *Item = GetItem(nID);
		if (!Item) return false;
		CheckedItem(Item);
		return true;
	}
	bool CWispMenu::UnCheckedItem(int nID)
	{
		CWispMenu::MenuItem *Item = GetItem(nID);
		if (!Item) return false;
		UnCheckedItem(Item);
		return true;
	}
	bool CWispMenu::Checked(unsigned int nID, bool bChecked)
	{
		if (bChecked)
			return CheckedItem(nID);
		return UnCheckedItem(nID);
	}

	bool CWispMenu::EnableItem(int nID)
	{
		CWispMenu::MenuItem *Item = GetItem(nID);
		if (!Item) return false;
		EnableItem(Item);
		return true;
	}
	bool CWispMenu::DisableItem(int nID)
	{
		CWispMenu::MenuItem *Item = GetItem(nID);
		if (!Item) return false;
		DisableItem(Item);
		return true;
	}
	bool CWispMenu::Enable(unsigned int nID, bool bEnable)
	{
		if (bEnable)
			return EnableItem(nID);
		return DisableItem(nID);
	}

	bool CWispMenu::Point(WISP_POINT *pPoint, bool bCurent) //Open
	{
		if (!pPoint)
			pPoint = &m_pWispBase->m_MousePT;

		int PosX = pPoint->x;
		int PosY = pPoint->y;

		if (PosX > m_ParentWnd->m_WindowRect.cx - m_WindowRect.cx)
			PosX -= m_WindowRect.cx;

		if (PosY > m_ParentWnd->m_WindowRect.cy - m_WindowRect.cy)
			PosY -= m_WindowRect.cy;

		if (bCurent)
			m_CurMenuItemIter = m_MenuItemList.Begin();

		MoveToScreen(PosX, PosY, true);
		Show(WISP_SH_MINSIZE);
		Focus();
		Top(true);

		return true;
	}
	void CWispMenu::Open(CWispMenu::MenuItem *Item, bool bCurrent)
	{
		CWispMenu *SubMenu = ((CWispMenu*)Item->dwData);
		SubMenu->m_CurMenuItemIter = SubMenu->m_MenuItemList.End();
		int PosX,PosY;
		if (m_Style & 0x10000) //POPUP MENU
		{
			PosX = Item->Rect.x + Item->Rect.cx + m_ScrClientRect.x;
			PosY = Item->Rect.y + m_ScrClientRect.y;
		} else
		{
			PosY = Item->Rect.y + Item->Rect.cy + m_ScrClientRect.y;
			int PosXtmp = Item->Rect.x + m_ScrClientRect.x;
			int TextExt = m_ClientDC.GetTextExtent(Item->string.operator const WCHAR*(), -1, 0) + m_SubMenuBorderSize*2;
			if (Item->pDIB)
				TextExt += Item->pDIB->m_PaintRect.cx;
			if (TextExt > SubMenu->m_WindowRect.cx)
				SubMenu->Resize(TextExt, SubMenu->m_WindowRect.cy, true);
			PosX = PosXtmp;
		}

		if (PosX > m_ParentWnd->m_WindowRect.cx - SubMenu->m_WindowRect.cx)
			PosX -= m_WindowRect.cx + SubMenu->m_ClientRect.cx;

		if (PosY > m_ParentWnd->m_WindowRect.cy - SubMenu->m_WindowRect.cy)
			PosY += Item->Rect.cy - SubMenu->m_WindowRect.cy;

		if (bCurrent)
			SubMenu->m_CurMenuItemIter = SubMenu->m_MenuItemList.Begin();
		SubMenu->MoveToScreen(PosX, PosY, true);
		SubMenu->Show(WISP_SH_MINSIZE);
		SubMenu->Focus();
	}
	void CWispMenu::HideMenu()
	{
		CWispMenu *pMenu = this;
		while (!(pMenu->m_Style & 0x10000)) //POPUP MENU
		{
			pMenu->m_bShowed = 0;
			TListIter<MenuItem> It = pMenu->m_CurMenuItemIter;

			if (It == pMenu->m_MenuItemList.End())
				return;

			pMenu->m_CurMenuItemIter = pMenu->m_MenuItemList.End();
			if (!(It->nFlags & 1)) //MENU_SUBMENU
				return;
			pMenu = ((CWispMenu*)It->dwData);
		}

		pMenu->Show(WISP_SH_HIDDEN);

		CWispMenu *NextMenu = pMenu;
		while (1)
		{
			TListIter<MenuItem> It = NextMenu->m_CurMenuItemIter;

			if (It == NextMenu->m_MenuItemList.End())
				break;

			if (!(It->nFlags & 1)) //MENU_SUBMENU
				break;

			NextMenu->m_CurMenuItemIter = NextMenu->m_MenuItemList.End();

			NextMenu = ((CWispMenu*)It->dwData);

			((CWispMenu*)It->dwData)->Show(WISP_SH_HIDDEN);
			pMenu->m_pWispBase->m_pRootWnd->Update(&NextMenu->m_ScrWindowRect);
			if (NextMenu == 0) break; //???
		}

		pMenu->Update((WISP_RECT*)0);
	}

	bool CWispMenu::OnActiveMenu(CWispMenu::MenuItem *Item, int nID)
	{
		return true;
	}

	void CWispMenu::SwitchItem(CWispMenu::MenuItem *Item)
	{
		if (OnActiveMenu(Item, Item->nID))
		{
			if (Item->nFlags & 2) //MENU_CHECKBOX
				Checked(Item);

			WISP_MSG Msg;
			Msg.hWnd = m_OwnerWnd;
			Msg.Msg = WISP_WM_COMMAND;
			Msg.Command.CtrlType = WISP_CTRL_MENU;
			Msg.Command.CmdMsg = 0x80000203; //WISP_ID_LBUTTON
			Msg.Command.CmdID = Item->nID;
			Msg.Command.Param1 = 0;
			Msg.Command.Param2 = Item;
			if (m_pWispBase->SendMessage(&Msg))
			{
				Msg.hWnd = m_OwnerWnd;
				Msg.Msg = WISP_WM_EVENT;
				m_pWispBase->PostMessage(&Msg);
				if (m_OwnerWnd == this)
				{
					GetRootMenu()->HideMenu();
				} else
				{
					m_OwnerWnd->Focus();
				}
			}
		}
	}
	void CWispMenu::UpdateIter(TListIter<MenuItem> It)
	{
		if (m_CurMenuItemIter != m_MenuItemList.End() && (m_CurMenuItemIter->nFlags & 1)) //MENU_SUBMENU
			((CWispMenu*)m_CurMenuItemIter->dwData)->HideMenu();
		m_CurMenuItemIter = It;
		if (m_CurMenuItemIter != m_MenuItemList.End() && (m_Style & 0x10000 || m_bShowed)) //POPUP MENU
		{
			if (m_CurMenuItemIter->nFlags & 1) //MENU_SUBMENU
				Open(&*m_CurMenuItemIter, false);
		}
		Update((WISP_RECT*)0);
	}
	bool CWispMenu::UpdateItem(CWispMenu::MenuItem *Item)
	{
		TListIter<CWispMenu::MenuItem> It = Item; //???
		if (m_CurMenuItemIter != It)
			UpdateIter(It);
		return true;
	}

	int CWispMenu::Clear()
	{
		int Count = m_MenuItemList.Size();
		m_MenuItemList.Clear();
		return Count;
	}

	CWispMenu::CWispMenu()
	{
		//m_CurMenuItemIter = 0;

		m_OpenMenuHeight = 20;

		m_MenuOffset = 20;
		m_MenuBorderSize = 20;

		m_SubMenuOffset = 20;
		m_SubMenuBorderSize = 5;

		m_MenuHeight = 5;
		m_SubMenuHeight = 5;

		m_CtrlType = WISP_CTRL_MENU;

		m_SubMenuOpenHeight = 18;
		m_FullMenuHeight = 22;
	}

	CWispMenu::~CWispMenu()
	{
	}
	
	bool CWispMenu::RemoveMenu(int nID)
	{
		TListIter<MenuItem> It = m_MenuItemList.Begin();
		while (It != m_MenuItemList.End())
		{
			if ((It->nFlags & 1) && It->nID == nID) //== 1 ???
			{
				m_MenuItemList.Remove(It);
				return true;
			}
			++It;
		}
		return false;
	}

	bool CWispMenu::RemoveMenu(CWispMenu::MenuItem *Item)
	{
		TListIter<MenuItem> It = Item;
		if (Item->nFlags & 1) return false; //== 1 ???
		m_MenuItemList.Remove(It);
		return true;
	}

	CWispMenu::MenuItem * CWispMenu::AppendMenu(int Pos, const WCHAR *Text, int nFlags, int nID, CWispDIB *pDIB, unsigned long long dwData)
	{
		TListIter<MenuItem> It;
		if (Pos >= 0)
		{
			if (Pos >= m_MenuItemList.Size())
				return nullptr;

			It = m_MenuItemList.InsertBefore(m_MenuItemList[Pos]);
		} else
			It = m_MenuItemList.Append();

		It->string = Text;
		It->pDIB = pDIB;
		It->nFlags = nFlags;
		It->nID = nID;
		It->Color = 0;
		It->dwData = dwData;
		//It->Rect ???;
		return &*It;
	}
/////////

	bool CWispMenu::LoadPopupMenuGroup(WISP_MENU_GROUP_ITEM *pMenuGroup, CWispBaseWnd *pParentWnd, CWispDIBList *pDIBList)
	{
		if (!CreateWnd(0, 0,0,0,0, 0, 0, 0x10006, WISP_SH_HIDDEN)) //??? 0x10000 POPUP MENU
			return false;
		
		if (pMenuGroup)
		{
			CWispMenu *pMenu = nullptr;
			SetOwner(pParentWnd);
			int PosX = 0;
			int PosY = 0;
			while (pMenuGroup->pszItemText || pMenuGroup->nItemID || pMenuGroup->nFlags)
			{
				if ((pMenuGroup->nFlags & 1) && pMenuGroup->pSubMenu) //MENU_SUBMENU
				{
					pMenu = new CWispMenu;
					pMenu->LoadPopupMenuGroup(pMenuGroup->pSubMenu, pParentWnd, pDIBList);
					pMenu->pParentMenu = this;
					pMenu->SetOwner(pParentWnd);
					pMenu->m_AdvStyle |= 1;
				}

				CWispDIB *pDIB = nullptr;
				if (pDIBList && pMenuGroup->nDIBIndex >= 0)
					pDIB = &pDIBList->m_DIBList[pMenuGroup->nDIBIndex]; //GetDIB
				//else	pDIB = 0;

				AppendMenu(-1, pMenuGroup->pszItemText, pMenuGroup->nFlags, pMenuGroup->nItemID, pDIB, (unsigned long long)pMenu); //???
				int TextExt = m_ClientDC.GetTextExtent(pMenuGroup->pszItemText, -1, 0);
				MIN_LIMIT(PosX, TextExt);

				if (pMenuGroup->nFlags & 4) //MENU_DELIM
					++PosY;
				else	PosY += m_SubMenuOpenHeight;
				++pMenuGroup;
				pMenu = nullptr;
			}
			int Border = (m_BorderSize + m_SubMenuBorderSize)*2;
			Resize(PosX + Border + m_MenuOffset + m_SubMenuOffset, Border + PosY, true);
		}
		return true;
	}
	bool CWispMenu::LoadMenuGroup(CWispWnd *pParentWnd, int CmdID, WISP_MENU_GROUP_ITEM *pMenuGroup, CWispDIBList *pDIBList)
	{
		if (!CreateWnd(0, 0,0,pParentWnd->m_ClientRect.cx,m_FullMenuHeight, pParentWnd, CmdID, 0, WISP_SH_MINSIZE))
			return false;
		
		if (pMenuGroup)
		{
			CWispMenu *pMenu = nullptr;
			while (pMenuGroup->pszItemText || pMenuGroup->nItemID || pMenuGroup->nFlags)
			{
				if ((pMenuGroup->nFlags & 1) && pMenuGroup->pSubMenu) //MENU_SUBMENU
				{
					pMenu = new CWispMenu;
					pMenu->LoadPopupMenuGroup(pMenuGroup->pSubMenu, m_OwnerWnd, pDIBList);
					pMenu->pParentMenu = this;
					pMenu->m_AdvStyle |= 1;
					pMenu->m_OwnerWnd = m_OwnerWnd;
				}

				CWispDIB *pDIB = nullptr;
				if (pDIBList && pMenuGroup->nDIBIndex >= 0)
					pDIB = &pDIBList->m_DIBList[pMenuGroup->nDIBIndex]; //GetDIB
				//else	pDIB = 0;
				AppendMenu(-1, pMenuGroup->pszItemText, pMenuGroup->nFlags, pMenuGroup->nItemID, pDIB, (unsigned long long)pMenu); //???
				++pMenuGroup;
				pMenu = nullptr;
			}
			RecalcLayout();
		}
		return true;
	}

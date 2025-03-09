
#include "../Code/alt/altarray.hpp"
#include "wisplist.hpp"
#include "wispbase.hpp"

//
//
#define ID_EDIT		0x80000000
#define ID_TIP		0x80000001
#define ID_TIP_TIMER	0x80000002

	int CListStringItem::GetItemLevel()
	{
		int Level = 0;
		CListStringItem *Item = pParentItem;
		while (Item)
		{
			Item = Item->pParentItem;
			++Level;
		}
		return Level-1;
	}
	int CListStringItem::GetChildItemExpandCount()
	{
		int nCount = Size();
		TListIter<CListStringItem> It = Begin();
		while (It != End())
		{
			if (It->uStatus & 1) //LIST_EXPAND
				nCount += It->GetChildItemExpandCount();
			++It;
		}
		return nCount;
	}

	bool CListStringItem::ItemHasChildren(CListStringItem *Item)
	{
		do {
			if (!Item->pParentItem)
				return false;
			Item = Item->pParentItem;
		} while (Item != this);
		return true;
	}

//
	CWispList::CWispList()
	{
		m_RootItem.pParentItem = 0;
		m_RootItem.Checked = 0;

		m_nLayoutHeight = 0;
		m_nMaxLayoutHeight = 0;

		m_nSpaceWidth = 3;
		m_nCtrlBoxWidth = 12;
		m_nLineHeight = 16;

		m_RootItem.uStatus = 1; //LIST_EXPAND
		m_CtrlType = WISP_CTRL_LIST;
		m_nTipPeriod = 700;
	}

	CWispList::~CWispList()
	{
	}

	bool CWispList::Destroy()
	{
		m_TipWnd.Destroy();
		m_FocusedIter = m_RootItem.End();
		m_RootItem.Clear();
		m_RootItem.Checked = 0;
		m_RootItem.SemiChecked = 0;
		m_RootItem.uStatus = 1; //LIST_EXPAND
		m_Columns.Clear();
		return CWispWnd::Destroy();
	}

	bool CWispList::Recalc(bool bMin)
	{
		if (!(m_Style & 0x1'0000))
			return false;

		if (bMin)
		{
			if (m_nLayoutHeight <= 0)
			{
				m_nMaxLayoutHeight = m_nLayoutHeight;
				m_nLayoutHeight = 0;
				RecalcLayout();
			}
		} else
		{
			if (m_nLayoutHeight > 0)
			{
				m_nLayoutHeight = m_nMaxLayoutHeight;
				RecalcLayout();
			}
			
		}
		return true;
	}
	void CWispList::RecalcLayout()
	{
		CWispWnd::RecalcLayout();

		m_LayoutRect.x = 0;
		m_LayoutRect.y = 0;
		m_LayoutRect.cx = m_ClientRect.cx;
		if (m_Style & 0x1'0000)
			m_LayoutRect.cy = m_nLayoutHeight;
		else	m_LayoutRect.cy = 0;

		m_ColumnRect.x = 0;
		m_ColumnRect.y = m_LayoutRect.cy;
		
		m_ColumnRect.cy = m_ClientRect.cy - m_LayoutRect.cy;
		
		m_nVSBPage = m_ClientRect.cy;
		if (m_Style & 0x1'0000)
			m_nVSBPage = m_ColumnRect.cy; //m_ClientRect.cy - m_LayoutRect.cy;

		m_nVSBPage = (m_nVSBPage + m_nLineHeight/3)/m_nLineHeight;
		MIN_LIMIT(m_nVSBPage, 0);

		if (m_pScrollBar[0] && m_nVSBPage != m_pScrollBar[0]->Page)
			SetScrollBarPage(0, m_nVSBPage);

		m_nNonTextWidth = 0;
		if (m_Style & 0x2'0000)
			m_nNonTextWidth += m_nCtrlBoxWidth;
		if (m_Style & 0x40'0000)
			m_nNonTextWidth += m_nCtrlBoxWidth;

		if (m_pScrollBar[1] && m_ClientRect.cx > 0)
		{
			int nHSBPage = m_ClientRect.cx / m_CharMiddleWidth;
			if (m_pScrollBar[1]->Page != nHSBPage)
				SetScrollBarPage(1, nHSBPage);
		}
		
		m_nColumnWidth = 0;
		int PosX = 0;
		TListIter<stWispHeaderItem> It = m_Columns.Begin();
		while (It != m_Columns.End())
		{
			It->RCDIB.x = PosX;
			It->RCDIB.y = m_LayoutRect.y;
			if (It == m_Columns.End()-1)
			{
				MIN_LIMIT(It->nRealWidth, m_ClientRect.cx - m_nColumnWidth);
				m_nColumnWidth += It->nRealWidth;
				if (m_pScrollBar[1] &&
					m_nColumnWidth != m_CharMiddleWidth * m_pScrollBar[1]->MaxPos)
				{
					if (m_pScrollBar[1]->bHide)
					{
						if (It->nRealWidth >= It->nInitWidth + m_pWispBase->m_Metrics[WISP_SM_SCROLL_BAR_SIZE])
							m_nColumnWidth -= m_pWispBase->m_Metrics[WISP_SM_SCROLL_BAR_SIZE];
					}
					SetScrollBarRange(1, m_nColumnWidth/m_CharMiddleWidth, 0);
				}
			} else
			{
				m_nColumnWidth += It->nRealWidth;
			}
			It->RCDIB.cx = It->nRealWidth;
			It->RCDIB.cy = m_LayoutRect.cy;
			                              
			It->RCRECT.x = It->nRealWidth + PosX - 2;;
			It->RCRECT.y = m_LayoutRect.y + 2;
			It->RCRECT.cx = 2;
			It->RCRECT.cy = m_LayoutRect.cy - 4;

			It->RCDRAG.x = It->nRealWidth + PosX - 2;
			It->RCDRAG.y = m_LayoutRect.y;
			It->RCDRAG.cx = 6;
			It->RCDRAG.cy = m_LayoutRect.cy;
			PosX += It->nRealWidth;
			++It;
		}
		m_ColumnRect.cx = m_nColumnWidth;

		CWispList::ScrollPage(0);
	}
	bool CWispList::InitWnd()
	{
		m_pDragColumn = 0;
		m_nColumnWidth = 0;
		m_nVSBPage = 0;
		m_PrevMousePos.y = -1;
		m_PrevMousePos.x = -1;
		m_CharMiddleWidth = m_ClientDC.m_pFont->GetCharWidth(0x30)/2;
		m_RootIter = m_RootItem.End();
		m_bEnableEdit = true;
		m_bTipShow = false;
		m_FocusedIter = m_RootItem.End();
		m_bEditDateUpdate = false;
		m_nLineHeight = m_ClientDC.GetTextHeight(0) + 4;
		return CWispWnd::InitWnd();
	}
	void CWispList::Hide()
	{
		if (m_TipWnd.IsWindowShow())
			HideTip();
	}

	WISP_MSG_MAP_BEGIN(CWispList)
		WISP_MSG_MAP(WISP_WM_UPDATE, OnUpdate)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP(WISP_WM_SCROLL_EVENT, OnScrollEvent)
		WISP_MSG_MAP(WISP_WM_TIMER_EVENT, OnTimer)
		WISP_MSG_MAP(WISP_WM_LOST_FOCUS, OnLostFocus)
		WISP_MSG_MAP(WISP_WM_DRAG_BEGIN, OnDragBegin)
		WISP_MSG_MAP(WISP_WM_DRAG_MOVE, OnDragMove)
		WISP_MSG_MAP(WISP_WM_DRAG_END, OnDragEnd)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
		WISP_MSG_MAP(WISP_WM_MOUSE_LEAVE, OnMouseLeave)
		WISP_MSG_MAP(WISP_WM_MOUSE_DBLCLICK, OnMouseDblClick)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP(WISP_WM_MOUSE_MOVE, OnMouseMove)
		WISP_MSG_MAP(WISP_WM_CREATING, OnCreating)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
	WISP_MSG_MAP_END(CWispWnd)

	bool CWispList::OnUpdate(WISP_MSG *pMsg)
	{
//		UpdateItemColor(0);
		return true;
	}

	bool CWispList::OnSize(WISP_MSG *pMsg)
	{
		if (pMsg->Sizing.ScrSize.cx <= 0 && m_Columns.Size())
		{
			if ((m_Columns.End()-1)->nRealWidth > (m_Columns.End()-1)->nInitWidth)
			{
				(m_Columns.End()-1)->nRealWidth += pMsg->Sizing.ScrSize.cx;
				m_nColumnWidth += pMsg->Sizing.ScrSize.cx;
			}
		}
		return true;
	}
	bool CWispList::OnScrollEvent(WISP_MSG *pMsg)
	{
		if (CWispWnd::OnScrollEvent(pMsg) && pMsg->ScrollEvent.Type == 0) //WISP_SCROLL_BAR_VERT
		{
			ScrollPage(pMsg->ScrollEvent.Delta);

			if (pMsg->ScrollEvent.Event == 3 && m_FocusedIter != m_RootItem.End()) //WISP_SB_PAGE_DOWN
			{
				TListIter<CListStringItem> It = m_RootIter;
				if (pMsg->ScrollEvent.Delta == 0)
				{
					for (int i = 1; i < m_nVSBPage; ++i) //???
						if (!NextOpen(It)) break;
				}

				if (It != m_RootItem.End())
					Select(&*It);

			}

			if (pMsg->ScrollEvent.Event == 2 && m_FocusedIter != m_RootItem.End()) //WISP_SB_PAGE_UP
			{
				TListIter<CListStringItem> It = m_RootIter;
				if (pMsg->ScrollEvent.Delta < 0)
				{
					for (int i = 1; i < m_nVSBPage; ++i) //???
						if (!NextOpen(It)) break;
				}

				if (It != m_RootItem.End())
					Select(&*It);
			}
		}
		return false;
	}

	bool CWispList::OnTimer(WISP_MSG *pMsg)
	{
		if (pMsg->Timer.ID == ID_TIP_TIMER &&
			m_PrevMousePos.x == pMsg->m_LogicMousePosWnd.x &&
			m_PrevMousePos.y == pMsg->m_LogicMousePosWnd.y)
		{
			TListIter<CListStringItem> Iter;
			int nCol = Point2Iter(&m_PrevMousePos, Iter);
			if (nCol >= 0 && nCol < 0x1000)
			{
				CListStringItem::CONTENT *Content = GetItemContent(&*Iter, nCol);
				if (!ShowTip(&*Iter, nCol, Content->String.operator const WCHAR*()))
					HideTip();
			}
		}
		return true;
	}
	bool CWispList::OnLostFocus(WISP_MSG *pMsg)
	{
		HideTip();
		return true;
	}
	bool CWispList::OnDragBegin(WISP_MSG *pMsg)
	{
		m_pWispBase->ChangeCursor(WISP_CT_RESIZE_HORZ);
		return true;
	}
	bool CWispList::OnDragMove(WISP_MSG *pMsg)
	{
		int PosX = pMsg->m_LogicMousePosWnd.x + m_CharMiddleWidth * m_pScrollBar[1]->CurPos;
		int DeltaX = pMsg->DragEvent.DeltaX;
		TListIter<stWispHeaderItem> It = m_pDragColumn; //???
		if (It != m_Columns.End()-1)
		{
			++It;
			int Middle = m_pDragColumn->RCRECT.x + m_pDragColumn->RCRECT.cx/2;
			if (DeltaX <= 0)
			{
				if (PosX >= Middle) return true;
				if (PosX < m_pDragColumn->RCDIB.x + m_pDragColumn->RCDRAG.cx)
				{
					if (m_pDragColumn->nRealWidth <= m_pDragColumn->RCDRAG.cx) return true;
					if (PosX <= 0) return true;
				}
			} else
			{
				if (PosX < Middle) return true;
				if (PosX >= It->nRealWidth + It->RCDIB.x - It->RCDRAG.cx)
				{
					if (It->nRealWidth <= It->RCDRAG.cx) return true;
					DeltaX = It->nRealWidth - It->RCDRAG.cx;
				}
			}
			m_pDragColumn->nRealWidth += DeltaX;
			It->nRealWidth -= DeltaX;
			RecalcLayout();
			Update((WISP_RECT*)0);
		} else
		{
			if (DeltaX < 0)
			{
				if (PosX < It->RCDIB.x + It->RCDRAG.cx)
				{
					if (It->nRealWidth <= It->RCDRAG.cx) return true;
					DeltaX = It->nRealWidth - It->RCDRAG.cx;
				}
			}

			It->nRealWidth += DeltaX;
			m_nColumnWidth += DeltaX;
			SetScrollBarRange(1, m_nColumnWidth / m_CharMiddleWidth, 0);// HORZ
			RecalcLayout();
			Update((WISP_RECT*)0);
		}
		return true;
	}
	bool CWispList::OnDragEnd(WISP_MSG *pMsg)
	{
		m_pWispBase->ChangeCursor(WISP_CT_ARROW);
		m_pDragColumn = 0;
		return true;
	}
	bool CWispList::OnUpdateClient(WISP_MSG *pMsg)
	{
		if ( !(m_Style & 0x40) )
		{
		        if (m_pBKDIB)
		                m_ClientDC.DrawDIBTiled(m_pBKDIB, 0);
		        else
		        if (m_bBGColor)
		                m_WindowDC.DrawFullRect(&m_ClientRect, m_crBGColor);
		        else
		                m_pWispBase->m_pWispDrawObj->DrawCtrlClient(&m_WindowDC, &m_ClientRect);
		}
		
		TListIter<CListStringItem> It = m_RootIter;
		if (It != m_RootItem.End())
		{
			int nLine = 0;
			while (nLine < m_nVSBPage)
			{
				DrawItem(nLine, It);
				if (!NextOpen(It))
				{
					if (m_Style & 0x400'0000)
						m_ClientDC.DrawHLine(0, m_ClientRect.cx - 1, m_ColumnRect.y + (nLine + 1) * m_nLineHeight, m_SpaceColor);
					break;
				}
				++nLine;
			}
		}

		if (m_Style & 0x1'0000)
		{
			m_ClientDC.SetTextColor(m_BKColor);
			m_pWispBase->m_pWispDrawObj->DrawListBK(&m_ClientDC, &m_LayoutRect);
			
			WISP_RECT LayoutRect = m_LayoutRect;
			LayoutRect.x = m_nSpaceWidth - m_CharMiddleWidth * m_pScrollBar[1]->CurPos;
			TListIter<stWispHeaderItem> It = m_Columns.Begin();
			while (It != m_Columns.End())
			{
				WISP_RECT RCDIB = It->RCDIB;
				RCDIB.x -= m_CharMiddleWidth * m_pScrollBar[1]->CurPos;

				m_ClientDC.m_pClipRect = &RCDIB;
				RCDIB.x += m_ClientDC.m_pRect->x;
				RCDIB.y += m_ClientDC.m_pRect->y;
				LayoutRect.cx = It->nRealWidth - m_nSpaceWidth*2;

				if (It->pDIB)
				{
					m_ClientDC.DrawDIB(LayoutRect.x, LayoutRect.y + (LayoutRect.cy - It->pDIB->m_PaintRect.cy)/2, It->pDIB);
					LayoutRect.x += m_nSpaceWidth + It->pDIB->m_PaintRect.cx;
					LayoutRect.cx -= m_nSpaceWidth + It->pDIB->m_PaintRect.cx;
				}
				DrawHeader(&*It, &LayoutRect);

				LayoutRect.x += LayoutRect.cx + m_nSpaceWidth*2;

				WISP_RECT RCRECT = It->RCRECT;
				RCRECT.x -= m_CharMiddleWidth * m_pScrollBar[1]->CurPos;

				m_ClientDC.DrawFrameRect(&RCRECT, 0, 1);
				m_ClientDC.m_pClipRect = 0;

				if (m_Style & 0x200'0000)
				{
					RCRECT.y = m_ColumnRect.y + 2;
					RCRECT.cx = 2;
					RCRECT.cy = m_ColumnRect.cy - 4;
					m_ClientDC.DrawFrameRect(&RCRECT, 0, 1);
				}
				
				++It;
			}
		}
		return false;
	}
	bool CWispList::OnMouseLeave(WISP_MSG *pMsg)
	{
		if (m_pDragColumn)
		{
			if (m_pWispBase->m_CursorType != WISP_CT_ARROW)
			{
				m_pWispBase->ChangeCursor(WISP_CT_ARROW);
				m_pDragColumn = 0;
			}
		}
		if (pMsg->MouseEvent.hWndMouseAt == &m_TipWnd)
			HideTip();
		return true;
	}
	bool CWispList::OnCmdEdit(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000701) //WISP_ID_EDIT_LOST_FOCUS
		{
			if (!m_bEditDateUpdate)
				EditUpdate();
			return false;
		}

		if (pMsg->Command.CmdMsg == 0x80000703) //WISP_ID_EDIT_KEYDOWN
		{
			if (pMsg->Command.Param1 == VK_RETURN)
				EditChange(true);
			else
			if (pMsg->Command.Param1 == VK_ESCAPE)
				EditChange(false);
		}
		return true;
	}

	WISP_CMD_MAP_BEGIN(CWispList)
		WISP_CMD_MAP(ID_EDIT, OnCmdEdit)
	WISP_CMD_MAP_END

	bool CWispList::OnMouseMove(WISP_MSG *pMsg)
	{
		WISP_POINT point = pMsg->m_LogicMousePosWnd;
		if (m_pDragColumn == 0 || PtInRect(&m_ClientRect, &pMsg->MsgMouseWndPT))
		{
			point.x += m_CharMiddleWidth * m_pScrollBar[1]->CurPos;
			if (m_Style & 0x1'0000)
			{
				m_pDragColumn = 0;
				if (m_Style & 0x200'0000)
				{
					TListIter<stWispHeaderItem> It = m_Columns.Begin();
					while (It != m_Columns.End())
					{
						WISP_RECT RECT;
						RECT.x = It->RCDRAG.x;
						RECT.y = 0;
						RECT.cx = It->RCDRAG.cx;
						RECT.cy = m_ClientRect.cy;

						if (point.x < RECT.x) break;
						if (PtInRect(&RECT, &point) && !(It->nFormat & 2)) //WHI_FIXED
						{
							m_pDragColumn = &*It;
							break;
						}
						++It;
					}
				} else
				if (point.y >= m_LayoutRect.y && point.y < m_LayoutRect.y + m_LayoutRect.cy)
				{
					TListIter<stWispHeaderItem> It = m_Columns.Begin();
					while (It != m_Columns.End())
					{
						if (point.x < It->RCDRAG.x) break;
						if (PtInRect(&It->RCDRAG, &point) && !(It->nFormat & 2)) //WHI_FIXED
						{
							m_pDragColumn = &*It;
							break;
						}
						++It;
					}
				}

				if (PtInRect(&m_ClientRect, &pMsg->MsgMouseWndPT))
				{
					if (m_pDragColumn)
					{
						if (m_pWispBase->m_CursorType != WISP_CT_RESIZE_HORZ)
							m_pWispBase->ChangeCursor(WISP_CT_RESIZE_HORZ);
					} else
					{
						if (m_pWispBase->m_CursorType != WISP_CT_ARROW)
							m_pWispBase->ChangeCursor(WISP_CT_ARROW);
					}
					m_PrevMousePos = pMsg->m_LogicMousePosWnd;

					if (m_TipWnd.IsWindowShow() &&
						!PtInRect(&m_TipWnd.m_ScrWindowRect, &pMsg->MsgMouseScrPT))
						HideTip();

					if (!m_bTipShow)
					{
						InsertTimer(ID_TIP_TIMER, m_nTipPeriod, 0, 0);
						m_bTipShow = true;
					}
				}
			}

			if (m_Style & 0x1000'0000)
			{
				CListStringItem *Item = Point2Index(&point, 0);
				if (Item)
					Select(Item);
			}
		} else
		{
			m_pWispBase->ChangeCursor(WISP_CT_ARROW);
			m_pDragColumn = 0;
		}
		return true;
	}
	bool CWispList::OnCreating(WISP_MSG *pMsg)
	{
		m_nCtrlCol = 0;
		m_ItemColor = gpCurWisp->m_pWispDrawObj->m_crSystem[0];
		_____________m_Color_field_CD4 = gpCurWisp->m_pWispDrawObj->m_crSystem[1];
		m_ColorFocus = m_pWispBase->m_pWispDrawObj->m_crSystem[107];
		m_ColorUnFocus = m_pWispBase->m_pWispDrawObj->m_crSystem[107];
		m_SpaceColor = gpCurWisp->m_pWispDrawObj->m_crSystem[0];
		_____________m_Color_field_CE0 = gpCurWisp->m_pWispDrawObj->m_crSystem[7];
		m_BKColor = gpCurWisp->m_pWispDrawObj->m_crSystem[1];
		m_nLayoutHeight = m_ClientDC.m_pFont->m_Height + 6;
		EnableScrollBar(0, 1, 1);
		EnableScrollBar(1, 1, 1);
		return true;
	}
	bool CWispList::OnCreate(WISP_MSG *pMsg)
	{
		if (!(m_Style & 0x1'0000))
		{
			m_LayoutRect.x = 0;
			m_LayoutRect.y = 0;
			m_LayoutRect.cx = 0;
			m_LayoutRect.cy = 0;
		}
		m_EditWnd.CreateWnd(0, 0,0,0,0, this, ID_EDIT, 0x9000002, WISP_SH_HIDDEN); //CMD
		m_TipWnd.CreateWnd(0, 0,0,0,0,     0, ID_TIP, 0, WISP_SH_HIDDEN); //CMD
		return true;
	}
	bool CWispList::OnKeyEvent(WISP_MSG *pMsg)
	{
		switch (pMsg->KeyEvent.KeyType)
		{
		case VK_LBUTTON:{
				if (pMsg->m_LogicMousePosWnd.x > m_ClientRect.cx ||
				    pMsg->m_LogicMousePosWnd.y > m_ClientRect.cy)
					return true;

				if ((m_Style & 0x1'0000) && m_pDragColumn)
				{
					if (pMsg->KeyEvent.bKeyDown)
						BeginDrag();
					else	EndDrag();
					return true;
				}

				if (pMsg->KeyEvent.bKeyDown)
				{
					TListIter<CListStringItem> It;
					int nCol = Point2Iter(&pMsg->m_LogicMousePosWnd, It);
					switch (nCol)
					{
					//case -1:
					//	if (m_FocusedIter != m_RootItem && !(m_Style & 0x4000'0000))
					//		Unselect(It);
					//	break;
					case COL_EXPAND: //0x1001:
						Toggle(&*It);
						break;
					case COL_CHECK: //0x1002:
						ToggleChecked(&*It, true);
						break;
					case COL_LAYOUT: { //0x1003: {
						WISP_POINT point = pMsg->m_LogicMousePosWnd;
						point.y = m_LayoutRect.y + m_LayoutRect.cy + 1;
						nCol = Point2Iter(&point, It);
						OnLButtonLayout(nCol);
						SendEvent(0x80000121, nCol, 0); //WISP_ID_LBUTTON_LAYOUT
						}
					case -1: {
						if (m_FocusedIter != m_RootItem.End() && !(m_Style & 0x4000'0000))
							Unselect(&*m_FocusedIter);
						} break;
					//case COL_EMPTY: //0x1004:
					//	break;
					default:
						{
							if (It->uStatus & 8) //LIST_SELECT
							{
								if (nCol >= 0 && nCol < m_Columns.Size() &&
									m_EditWnd.m_ShowMode == WISP_SH_HIDDEN)
								{
									if (m_Columns[nCol]->nFormat & 1) //WHI_EDIT
									{
										EditItem(&*It, nCol);
									} else
									{
										OnLButton(&*It, nCol);
										m_nLButtonCol = nCol;
										SendEvent(0x80000203, nCol, &*It); //WISP_ID_LBUTTON
									}
								}

								if ((m_Style & 0x80'0000) && m_pWispBase->m_KeyMap[VK_CONTROL])
								{
									Unselect(&*It);
								}
							} else
							{
								if (!(m_Style & 0x80'0000) || !m_pWispBase->m_KeyMap[VK_CONTROL])
								{
									if (m_FocusedIter != m_RootItem.End())
										Unselect(&*m_FocusedIter);
								}
								Select(&*It);
							}
						}
						break;
					}
				}
				Update((WISP_RECT*)0);
			//	HideTip();
				break;}
		case VK_RBUTTON:{
				if (pMsg->m_LogicMousePosWnd.x > m_ClientRect.cx ||
				    pMsg->m_LogicMousePosWnd.y > m_ClientRect.cy || !pMsg->KeyEvent.bKeyDown)
					return true;

				TListIter<CListStringItem> It;
				int nCol = Point2Iter(&pMsg->m_LogicMousePosWnd, It);
				switch (nCol)
				{
				case -1:
					break;
				case COL_EXPAND: //0x1001:
					break;
				case COL_CHECK: //0x1002:
					break;
				case COL_LAYOUT: { //0x1003: {
					WISP_POINT point = pMsg->m_LogicMousePosWnd;
					point.y = m_LayoutRect.y + m_LayoutRect.cy + 1;
					nCol = Point2Iter(&point, It);
					OnRButtonLayout(nCol);
					SendEvent(0x80000122, nCol, 0); //WISP_ID_RBUTTON_LAYOUT
					} break;
				//case COL_EMPTY: //0x1004:
				//	break;
				default:
					if ((It->uStatus & 8) && //LIST_SELECT
						(nCol >= 0 && nCol < m_Columns.Size()) &&
						m_EditWnd.m_ShowMode == WISP_SH_HIDDEN)
					{
						OnRButton(&*It, nCol);
						SendEvent(0x80000204, nCol, &*It); //WISP_ID_RBUTTON
					}
					break;
				}
			//	HideTip();
				break;}
		case VK_RETURN:{
				if (pMsg->KeyEvent.bKeyDown && m_FocusedIter != m_RootItem.End())
					Toggle(&*m_FocusedIter);
			//	HideTip();
				break;}
		case VK_SPACE:{
				if ((m_Style & 0x40'0000) && pMsg->KeyEvent.bKeyDown)
				{
					if (m_FocusedIter != m_RootItem.End())
						ToggleChecked(&*m_FocusedIter, true);
				}
			//	HideTip();
				break;}
		case VK_LEFT:{
				if (pMsg->KeyEvent.bKeyDown && m_FocusedIter != m_RootItem.End() &&
					m_FocusedIter->GetItemLevel()>0)
				{
					Scroll(m_FocusedIter->pParentItem, -1);
					Select(m_FocusedIter->pParentItem);
				}
			//	HideTip();
				break;}
		case VK_UP:
		case VK_DOWN:{
				if (pMsg->KeyEvent.bKeyDown && !(m_Style & 0x80'0000))
				{
					if (m_FocusedIter != m_RootItem.End())
					{
						if (!CheckItemPage(&*m_FocusedIter))
						{
							Scroll(&*m_FocusedIter, -1);
						}

						TListIter<CListStringItem> It = m_FocusedIter;
						if (pMsg->KeyEvent.KeyType == VK_UP)
						{
							bool bSelect = PrevOpen(It);
							if (m_FocusedIter == m_RootIter)
								SendScrollEvent(0, 1); //VertScrollLineUp();
							if (bSelect)
							{
								Select(&*It);
								Update((WISP_RECT*)0);
							}
						} else
						if (pMsg->KeyEvent.KeyType == VK_DOWN)
						{
							bool bSelect = NextOpen(It);
							if (GetItemIndex(&*m_FocusedIter) == m_nVSBPage-1)
								SendScrollEvent(0, 0); //VertScrollLineDown();
							if (bSelect)
							{
								Select(&*It);
								Update((WISP_RECT*)0);
							}
						}
					} else
					if (m_RootItem.Size())
					{
						Select(&*m_RootItem.Begin());
						Scroll(&*m_FocusedIter, -1);
						Update((WISP_RECT*)0);
					}
				}
			//	HideTip();
				break;}
		case VK_RIGHT:{
				if (pMsg->KeyEvent.bKeyDown && m_FocusedIter != m_RootItem.End())
				{
					Expand(&*m_FocusedIter);
				}
			//	HideTip();
				break;}
		default:;
		}
		HideTip();
		return true;
	}

	//0x80000203
	void CWispList::OnLButton(CListStringItem *Item, int nCol) //0x38
	{
	}
	//0x80000204
	void CWispList::OnRButton(CListStringItem *Item, int nCol) //+3C
	{
	}

	//0x80000122
	void CWispList::OnRButtonLayout(int nCol) //+40
	{
	}
	//0x80000121
	void CWispList::OnLButtonLayout(int nCol) //+44
	{
	}

//DBLCLICK

	void CWispList::OnMouseDblClick(CListStringItem *Item, int nCol) //+48
	{
		//EMPTY
	}

	bool CWispList::OnMouseDblClick(WISP_MSG *pMsg)
	{
		if (m_FocusedIter != m_RootItem.End())
		{
			OnMouseDblClick(&*m_FocusedIter, m_nLButtonCol);

			if (pMsg->KeyEvent.KeyType == VK_LBUTTON)
				SendCommand(0x80000118, m_nLButtonCol, &*m_FocusedIter); //WISP_ID_LBUTTON_DBLCLICK
			else
			if (pMsg->KeyEvent.KeyType == VK_RBUTTON)
				SendCommand(0x80000117, m_nLButtonCol, &*m_FocusedIter); //WISP_ID_RBUTTON_DBLCLICK
		}
		return true;
	}

//TIP
	bool CWispList::HideTip()
	{
		if (m_bTipShow)
		{
			RemoveTimer(ID_TIP_TIMER);
			m_bTipShow = false;
		}
		if (!m_TipWnd.IsWindowShow())
			return false;
		if (!OnHideTip(m_pTipItem, m_nTipCol, m_TipWnd.m_WndText.operator const WCHAR*()))
			return false;
		m_TipWnd.m_WndText.Empty();
		m_TipWnd.Show(WISP_SH_HIDDEN);
		m_pTipItem = 0;
		m_nTipCol = 0;
		Update((WISP_RECT*)0);
		return true;
	}
	bool CWispList::ShowTip(CListStringItem *Item, int nCol, const WCHAR *Text)
	{
		int nIndex = GetItemIndex(Item);
		if (nIndex < 0)
			return false;

		m_pTipItem = Item;
		m_nTipCol = nCol;
		int PosX = m_ScrWindowRect.x + m_Columns[nCol]->RCDIB.x - m_CharMiddleWidth * m_pScrollBar[1]->CurPos;
		int PosY = m_ScrWindowRect.y + m_ColumnRect.y + nIndex * m_nLineHeight;
		unsigned long szText[256];
		if (IsColorString(Text))
			TStrCpy((DWORD*)szText, (const DWORD*)Text);
		else	TStrCpy((WCHAR*)szText, (const WCHAR*)Text);

		if (!GetTipText(Item, nCol, &PosX, &PosY, (WCHAR*)szText))
			return false;

		if (m_bTipShow)
		{
			RemoveTimer(ID_TIP_TIMER);
			m_bTipShow = false;
		}

		if (IsColorString((const WCHAR*)szText))
		{
			int Len = TStrLen((const DWORD*)&szText[1]);
			m_TipWnd.m_WndText.Preallocate(Len+1, 0);
			m_TipWnd.m_WndText.m_nLength = Len;
			//TStrCpy(m_TipWnd.m_WndText.operator const WCHAR*(), (const DWORD*)&szText[1]);
			TStrCpy(m_TipWnd.m_WndText.data(), (const DWORD*)&szText[1]);
		} else
		{
			m_TipWnd.SetWindowText((const WCHAR*)szText);
		}

		m_TipWnd.AutoAdjustPostion(this);

		MAX_LIMIT(PosX, m_pWispBase->m_pRootWnd->m_WindowRect.cx - m_TipWnd.m_WindowRect.cx);
		MIN_LIMIT(PosX, 0);

		MAX_LIMIT(PosY, m_pWispBase->m_pRootWnd->m_WindowRect.cy - m_TipWnd.m_WindowRect.cy);
		MIN_LIMIT(PosY, 0);

		m_TipWnd.MoveToScreen(PosX, PosY, true);
		m_TipWnd.Show(WISP_SH_MINSIZE);
		Update((WISP_RECT*)0);
		return true;
	}
	bool CWispList::CheckShowTip(CListStringItem *Item, int nCol)
	{
		CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
		int TextExt = m_ClientDC.GetTextExtent(Content->String.operator const WCHAR*(), -1, 0) + m_nSpaceWidth*2;
		if (nCol == 0) //m_nCtrlCol ???
		{
			if (m_Style & 0x2'0000)
				TextExt += m_nCtrlBoxWidth;
			if (m_Style & 0x40'0000)
				TextExt += m_nCtrlBoxWidth;
		}
		if (Content->pDIB)
			TextExt += Content->pDIB->m_PaintRect.cx;
		return TextExt <= m_Columns[nCol]->RCDIB.cx;
	}
	bool CWispList::GetTipText(CListStringItem *Item, int nCol, int *pPosX, int *pPosY, WCHAR *pOutText) //+4C
	{
		if (CheckShowTip(Item, nCol))
			return false;
		CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
		TStrCpy(pOutText, Content->String.operator const WCHAR*());
		return true;
	}
	bool CWispList::OnHideTip(CListStringItem *Item, int nCol, const WCHAR *Text) //+50
	{
		return true;
	}

//EDIT
	void CWispList::EditChange(bool bEnable)
	{
		if (m_pWispBase->m_pFocusWnd == &m_EditWnd)
		{
			m_bEnableEdit = bEnable;
			Focus();
			Update(&m_ClientRect);
		}
	}
	bool CWispList::Edit(CListStringItem *Item, int nCol, TWideString& String) //Content? 0x80000111 //+54
	{
		//EMPTY
		return true;
	}
	void CWispList::EditItem(CListStringItem *Item, int nCol)
	{
		if (nCol < m_Columns.Size())
		{
			int nIndex = GetItemIndex(Item);
			if (nIndex == -1)
				Scroll(Item, -1);
			WISP_RECT Rect = m_Columns[nCol]->RCDIB; //+28
			Rect.x -= m_CharMiddleWidth * m_pScrollBar[1]->CurPos;
			Rect.y = m_ColumnRect.y + nIndex * m_nLineHeight;

			Rect.cy = m_nLineHeight;

			m_nEditCol = nCol;
			m_pEditItem = Item;

			CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
			m_EditWnd.m_WndText.Empty();
			WISP_MSG Msg;
			Msg.hWnd = m_OwnerWnd;
			Msg.Msg = WISP_WM_COMMAND;
			Msg.Command.CmdMsg = 0x80000111; //WISP_ID_ITEM_EDITITING
			Msg.Command.CmdID = m_CmdID;
			Msg.Command.Param1 = m_nEditCol;
			Msg.Command.Param2 = m_pEditItem;
			if (m_pWispBase->SendMessage(&Msg) && Edit(m_pEditItem, m_nEditCol, Content->String))
			{
				if (nCol == m_nCtrlCol)
				{
					if (m_Style & 0x2'0000)
					{
						Rect.x += m_nCtrlBoxWidth;
						Rect.cx -= m_nCtrlBoxWidth;
					} else //???
					if (m_Style & 0x40'0000)
					{
						Rect.x += m_nCtrlBoxWidth;
						Rect.cx -= m_nCtrlBoxWidth;
					}
				}

				if (Content->pDIB)
				{
					Rect.x  += Content->pDIB->m_PaintRect.cx;
					Rect.cx -= Content->pDIB->m_PaintRect.cx;
				}
				m_EditWnd.MoveWindowToClient(Rect, true);
				if (m_EditWnd.m_WndText.m_nLength == 0)
					m_EditWnd.SetWindowText(Content->String.operator const WCHAR*());
				m_EditWnd.Show(WISP_SH_MINSIZE);
				m_EditWnd.Focus();
			}
		}
	}
	bool CWispList::OnEditing(CListStringItem *Item, int nCol, TWideString& String) //80000113,80000112 //+58
	{
		//EMPTY
		return true;
	}
	void CWispList::OnEdit(CListStringItem *Item, int nCol, TWideString& String) //0x8000010E //+5C
	{
		//EMPTY
	}
	void CWispList::EditUpdate()
	{
		m_bEditDateUpdate = true;
		m_EditWnd.Show(WISP_SH_HIDDEN);
		WISP_MSG Msg;
		Msg.hWnd = m_OwnerWnd;
		Msg.Msg = WISP_WM_COMMAND;
		Msg.Command.CmdMsg = (m_bEnableEdit != 0) ? 0x80000112 : 0x80000113; // WISP_ID_ITEM_EDIT_LBUTTON : WISP_ID_ITEM_EDIT_RBUTTON
		Msg.Command.CmdID = m_CmdID;
		Msg.Command.Param1 = m_nEditCol;
		Msg.Command.Param2 = m_pEditItem;
		if (m_pWispBase->SendMessage(&Msg) && OnEditing(m_pEditItem, m_nEditCol, m_EditWnd.m_WndText))
		{
			if (m_bEnableEdit)
			{
				CListStringItem::CONTENT *Content = GetItemContent(m_pEditItem, m_nEditCol);
				if (Content->String != m_EditWnd.m_WndText)
				{
					Content->String = m_EditWnd.m_WndText;
					Msg.Command.CmdMsg = 0x8000010E; //WISP_ID_ITEM_EDIT_UPDATE
					m_pWispBase->SendMessage(&Msg);
					OnEdit(m_pEditItem, m_nEditCol, Content->String);
				}
			}
		} else
		{
			m_EditWnd.m_WndText.Empty();
		}
		m_bEditDateUpdate = false;
		m_bEnableEdit = true;
	}


	bool CWispList::OnExpanding(CListStringItem *Item) //+60
	{
		//EMPTY
		return true;
	}
	void CWispList::OnExpand(CListStringItem *Item) //+64
	{
		//EMPTY
	}
	bool CWispList::Expand(CListStringItem *Item)
	{
		if (!(Item->uStatus & 1)) //LIST_EXPAND
		{
			WISP_MSG Msg;
			Msg.hWnd = m_OwnerWnd;
			Msg.Msg = WISP_WM_COMMAND;
			Msg.Command.CmdMsg = 0x80000100; //WISP_ID_EXPANDING
			Msg.Command.CmdID = m_CmdID;
			Msg.Command.Param1 = 0;
			Msg.Command.Param2 = Item;
			if (!m_pWispBase->SendMessage(&Msg) || !OnExpanding(Item))
				return false;

			Item->uStatus |= 1; //LIST_EXPAND
			int Count = Item->GetChildItemExpandCount();
			if (Count)
			{
				AdjustScrollBarMaxPos(0, Count);
				RecalcLayout();
				Msg.Command.CmdMsg = 0x80000101; //WISP_ID_EXPAND
				Msg.Command.Param1 = 1;
				m_pWispBase->SendMessage(&Msg);
				OnExpand(Item);
				Update((WISP_RECT*)0);
			}
		}
		return true;
	}

	bool CWispList::OnCollapsing(CListStringItem *Item) //+68
	{
		//EMPTY
		return true;
	}
	void CWispList::OnCollapse(CListStringItem *Item) //+6C
	{
		//EMPTY
	}
	bool CWispList::Collapse(CListStringItem *Item)
	{
		if (!Item)
			Item = &m_RootItem;
		if (Item->uStatus & 1) //LIST_EXPAND
		{
			WISP_MSG Msg;
			Msg.hWnd = m_OwnerWnd;
			Msg.Msg = WISP_WM_COMMAND;
			Msg.Command.CmdMsg = 0x80000102; //WISP_ID_COLLAPSING
			Msg.Command.CmdID = m_CmdID;
			Msg.Command.Param1 = 1;
			Msg.Command.Param2 = Item;
			if (!m_pWispBase->SendMessage(&Msg) || !OnCollapsing(Item))
				return false;

			Item->uStatus &= ~1; //LIST_EXPAND
			int Count = Item->GetChildItemExpandCount();
			if (Count)
			{
				if (m_FocusedIter != m_RootItem.End())
				{	
					if (Item->ItemHasChildren(&*m_FocusedIter))
						Select(Item);
				}
				AdjustScrollBarMaxPos(0, -Count);
				RecalcLayout();
				Msg.Command.CmdMsg = 0x80000103; //WISP_ID_COLLAPSE
				Msg.Command.Param1 = 0;
				m_pWispBase->SendMessage(&Msg);
				OnCollapse(Item);
				Update((WISP_RECT*)0);
			}
		}
		return true;
	}

	bool CWispList::Toggle(CListStringItem *Item)
	{
		if (Item->uStatus & 1) //LIST_EXPAND
		{
			Collapse(Item);
			return true;
		}
		Expand(Item);
		return false;
	}

	bool CWispList::OnSelecting(CListStringItem *Item) //+70
	{
		//EMPTY
		return true;
	}
	void CWispList::OnSelect(CListStringItem *Item) //+74
	{
		//EMPTY
	}
	bool CWispList::Select(CListStringItem *Item)
	{
		if (!(Item->uStatus & 8)) //LIST_SELECT
		{
			if (!(m_Style & 0x80'0000) || !m_pWispBase->m_KeyMap[VK_CONTROL])
			{
				if (m_FocusedIter != m_RootItem.End())
				{
					if (&*m_FocusedIter != Item)
						Unselect(&*m_FocusedIter);//Item);
				}
			}
			WISP_MSG Msg;
			Msg.hWnd = m_OwnerWnd;
			Msg.Msg = WISP_WM_COMMAND;
			Msg.Command.CmdMsg = 0x80000104; //WISP_ID_SELECTING
			Msg.Command.CmdID = m_CmdID;
			Msg.Command.Param1 = 0;
			Msg.Command.Param2 = Item;
			if (!m_pWispBase->SendMessage(&Msg) || !OnSelecting(Item))
				return false;
			Item->uStatus |= 8; //LIST_SELECT
			Msg.Command.CmdMsg = 0x80000105; //WISP_ID_SELECT
			Msg.Command.Param1 = 1;
			m_pWispBase->SendMessage(&Msg);
			m_FocusedIter = Item; //???
			OnSelect(Item);
			Update((WISP_RECT*)0);
		}
		return true;
	}

	bool CWispList::OnUnselecting(CListStringItem *Item) //+78
	{
		//EMPTY
		return true;
	}
	void CWispList::OnUnselect(CListStringItem *Item) //+7C
	{
		//EMPTY
	}
	bool CWispList::Unselect(CListStringItem *Item)
	{
		if (Item->uStatus & 8) //LIST_SELECT
		{
			WISP_MSG Msg;
			Msg.hWnd = m_OwnerWnd;
			Msg.Msg = WISP_WM_COMMAND;
			Msg.Command.CmdMsg = 0x80000106; //WISP_ID_UNSELECTING
			Msg.Command.CmdID = m_CmdID;
			Msg.Command.Param1 = 1;
			Msg.Command.Param2 = Item;
			if (!m_pWispBase->SendMessage(&Msg) || !OnUnselecting(Item))
				return false;
			Item->uStatus &= ~8; //LIST_SELECT
			Msg.Command.CmdMsg = 0x80000107; //WISP_ID_UNSELECT
			Msg.Command.Param1 = 0;
			m_pWispBase->SendMessage(&Msg);
			if (&*m_FocusedIter == Item)
				m_FocusedIter = m_RootItem.End();
			OnUnselect(Item);
			Update((WISP_RECT*)0);
		}
		return true;
	}


	bool CWispList::OnChecking(CListStringItem *Item) //0x80
	{
		//EMPTY
		return true;
	}
	void CWispList::OnChecked(CListStringItem *Item) //0x84
	{
		//EMPTY
	}
	bool CWispList::Checked(CListStringItem *Item, bool bWalk)
	{
		if (!(Item->uStatus & 2)) //LIST_CHECKED
		{
			WISP_MSG Msg;
			Msg.hWnd = m_OwnerWnd;
			Msg.Msg = WISP_WM_COMMAND;
			Msg.Command.CmdMsg = 0x80000108; //WISP_ID_CHECKING
			Msg.Command.CmdID = m_CmdID;
			Msg.Command.Param1 = 0;
			Msg.Command.Param2 = Item;
			if (!m_pWispBase->SendMessage(&Msg) || !OnChecking(Item))
				return false;

			if (Item->uStatus & 4) //LIST_SEMICHECKED
			{
				Item->uStatus &= ~4; //LIST_SEMICHECKED
				--Item->pParentItem->SemiChecked;
			}

			Item->uStatus |= 2; //LIST_CHECKED
			++Item->pParentItem->Checked;

			TListIter<CListStringItem> It = Item->Begin();
			while (It != Item->End())
			{
				Checked(&*It, 0);
				++It;
			}

			if (bWalk)
			{
				WalkChecked(Item);
			} else
			{
				Item->Checked = Item->Size();
				Item->SemiChecked = 0;
			}
			Msg.Command.CmdMsg = 0x80000109; //WISP_ID_CHECKED
			Msg.Command.Param1 = 1;
			m_pWispBase->SendMessage(&Msg);
			OnChecked(Item);
			Update((WISP_RECT*)0);
		}
		return true;
	}

	bool CWispList::OnUnchecking(CListStringItem *Item) //0x88
	{
		//EMPTY
		return true;
	}
	void CWispList::OnUnchecked(CListStringItem *Item) //0x8C
	{
		//EMPTY
	}
	bool CWispList::Unchecked(CListStringItem *Item, bool bWalk)
	{
		if (Item->uStatus & (2|4)) //LIST_CHECKED LIST_SEMICHECKED
		{
			WISP_MSG Msg;
			Msg.hWnd = m_OwnerWnd;
			Msg.Msg = WISP_WM_COMMAND;
			Msg.Command.CmdMsg = 0x8000010A; //WISP_ID_UNCHECKING
			Msg.Command.CmdID = m_CmdID;
			Msg.Command.Param1 = 1;
			Msg.Command.Param2 = Item;
			if (!m_pWispBase->SendMessage(&Msg) || !OnUnchecking(Item))
				return false;

			if (Item->uStatus & 2) //LIST_CHECKED
			{
				Item->uStatus &= ~2; //LIST_CHECKED
				--Item->pParentItem->Checked;
			} else
			if (Item->uStatus & 4) //LIST_SEMICHECKED
			{
				Item->uStatus &= ~4; //LIST_SEMICHECKED
				--Item->pParentItem->SemiChecked;
			}

			TListIter<CListStringItem> It = Item->Begin();
			while (It != Item->End())
			{
				Unchecked(&*It, 0);
				++It;
			}
			if (bWalk)
			{
				WalkChecked(Item);
			} else
			{
				Item->Checked = 0;
				Item->SemiChecked = 0;
			}
			Msg.Command.CmdMsg = 0x8000010B; //WISP_ID_UNCHECKED
			Msg.Command.Param1 = 0;
			m_pWispBase->SendMessage(&Msg);
			OnUnchecked(Item);
			Update((WISP_RECT*)0);
		}
		return true;
	}

	bool CWispList::ToggleChecked(CListStringItem *Item, char bWalk)
	{
		if (Item->uStatus & 2) //LIST_CHECKED
		{
			Unchecked(Item, bWalk);
			return true;
		}
		Checked(Item, bWalk);
		return false;
	}

	unsigned int CWispList::GetContent(CListStringItem *Item, int nCol, WCHAR *pOutStr, unsigned int OutLen, const WCHAR *pStr) //+90
	{
		TArray<const WCHAR*,0> Array;
		Array.Resize(GetParentLevel(Item)-1);
		unsigned int nRet = 0;
		if (pOutStr) pOutStr[0] = 0;
		while (Item != &m_RootItem)
		{
			CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
			Array.Append(Content->String.operator const WCHAR*());
			Item = Item->pParentItem;
		}
		for (int i = Array.Count - 1; i >= 0; --i)
		{
			if (nRet < OutLen)
			{
				if (pOutStr)
					nRet += TStrCpy(&pOutStr[nRet], Array[i]);
				else	nRet += TStrLen(Array[i]);
			}

			if (i > 0 && nRet < OutLen)
			{
				if (pOutStr)
					nRet += TStrCpy(&pOutStr[nRet], pStr);
				else	nRet += TStrLen(pStr);
			}
		}
		return nRet;
	}

	void CWispList::DrawContent(int nCol, CListStringItem::CONTENT *Content, WISP_RECT *pRect) //+94
	{
		if (IsColorString(Content->String.operator const WCHAR*()))
			m_ClientDC.DrawColorString(pRect, ((const unsigned long*)Content->String.operator const WCHAR*())+1, -1, 0);
		else	m_ClientDC.DrawString(pRect, Content->String.operator const WCHAR*(), -1, 0);
	}
	void CWispList::DrawHeader(stWispHeaderItem *Item, WISP_RECT *pRect) //+98
	{
		m_ClientDC.DrawShadowText(pRect, Item->strHeading.operator const WCHAR*(), 1, -1, 0);
	}

	void CWispList::DrawItem(int nCol, TListIter<CListStringItem> & It)
	{
		int Level = It->GetItemLevel();
		WISP_RECT LineRC;
		LineRC.cy = m_nLineHeight;
		LineRC.y = m_ColumnRect.y + nCol * m_nLineHeight;
		if ((m_Style & 0x6'0000) == 0x6'0000)
		{
			if (It->Size())
			{
				LineRC.cx = m_ColumnRect.cx;
				LineRC.x = m_ColumnRect.x + Level * m_nNonTextWidth - m_CharMiddleWidth * m_pScrollBar[1]->CurPos;
				m_ClientDC.DrawFullRect(&LineRC, m_pWispBase->m_pWispDrawObj->m_crSystem[12]);
			}

			for (int i = 0; i < Level; ++i)
			{
				if (!(i%2))
				{
					LineRC.cx = m_nCtrlBoxWidth;
					LineRC.x = m_ColumnRect.x + i * m_nCtrlBoxWidth - m_CharMiddleWidth * m_pScrollBar[1]->CurPos;
					m_ClientDC.DrawFullRect(&LineRC, m_pWispBase->m_pWispDrawObj->m_crSystem[12]);
				}
			}
		}

		int Line = 0;
		LineRC.x = m_ColumnRect.x + Level * m_nNonTextWidth - m_CharMiddleWidth * m_pScrollBar[1]->CurPos;
		TListIter<CListStringItem::CONTENT> Icon = It->Contents.Begin();
		TListIter<stWispHeaderItem> Icol = m_Columns.Begin();
		while (Icon != It->Contents.End() && Icol != m_Columns.End())
		{
			WISP_RECT MainRC;
			MainRC.x = Icol->RCDIB.x - m_CharMiddleWidth * m_pScrollBar[1]->CurPos;
			MainRC.y = LineRC.y;
			MainRC.cx = Icol->RCDIB.cx;
			MainRC.cy = m_nLineHeight;

			if (Line != 0)
			{
				m_ClientDC.m_pClipRect = &MainRC;
				MainRC.x += m_ClientDC.m_pRect->x;
				MainRC.y += m_ClientDC.m_pRect->y;
				LineRC.x = Icol->RCDIB.x - m_CharMiddleWidth * m_pScrollBar[1]->CurPos;
			} else
			{
				if (It->uStatus & 0x10) //LIST_COLOR
				{
					WISP_RECT ColorRC;
					ColorRC.x = 0;
					ColorRC.y = LineRC.y;
					ColorRC.cx = m_ClientRect.cx;
					ColorRC.cy = LineRC.cy;
					m_ClientDC.DrawFullRect(&ColorRC, It->Color);
				}

				if ((m_Style & 0x4000'0000 || m_pWispBase->m_pFocusWnd == this) &&
					It->uStatus & 8 && !(m_Style & 0x8000'0000))
				{
					//LIST_SELECT
					WISP_RECT RCx08;
					RCx08.x = 0;
					RCx08.y = LineRC.y;
					RCx08.cx = m_ClientRect.cx;
					RCx08.cy = LineRC.cy;
					if (m_Style & 0x100'0000)
					{
						m_ClientDC.DrawRect(&RCx08, m_pWispBase->m_pFocusWnd == this ? m_ColorFocus : m_ColorUnFocus);
					} else
					{
						m_pWispBase->m_pWispDrawObj->DrawListFocusRect(&m_ClientDC, &RCx08);
					}
					m_ClientDC.m_pClipRect = &MainRC;
					MainRC.x += m_ClientDC.m_pRect->x;
					MainRC.y += m_ClientDC.m_pRect->y;
				}
			}

			if (m_nCtrlCol == Line)
			{
				if (m_Style & 0x2'0000)
				{
					LineRC.cx = m_nCtrlBoxWidth;
					if (It->Size())
					{
						//LIST_EXPAND
						int State = (It->uStatus & 1) ? 9 : 10;
						//WISP_SSI_EXPANDBUTTON_EXPAND
						//WISP_SSI_EXPANDBUTTON_COLLAPSE
						m_ClientDC.DrawSystemStandardIcon(&LineRC, State, 0x21A221);
					}
					LineRC.x += m_nCtrlBoxWidth;
				}

				if (m_Style & 0x40'0000)
				{
					LineRC.cx = m_nCtrlBoxWidth;
					int State;
					//LIST_CHECKED
					if (It->uStatus & 2) State = 12; //WISP_SSI_CHECKBOX_CHECKED
					else
					//LIST_SEMICHECKED
					if (It->uStatus & 4) State = 13; //WISP_SSI_CHECKBOX_SEMICHECKED
					else State = 11; //WISP_SSI_CHECKBOX_UNCHECKED
					m_ClientDC.DrawSystemStandardIcon(&LineRC, State, 0x21A221);
					LineRC.x += m_nCtrlBoxWidth;
				}
			}

			if (Icon->pDIB)
			{
				m_ClientDC.DrawDIB(LineRC.x, LineRC.y, Icon->pDIB);
				LineRC.x += Icon->pDIB->m_PaintRect.cx;
			}

			m_ClientDC.m_pClipRect = 0;

			LineRC.x += m_nSpaceWidth;
			LineRC.cx = (Icol->RCDIB.x + Icol->RCDIB.cx) - m_CharMiddleWidth * m_pScrollBar[1]->CurPos - m_nSpaceWidth - LineRC.x;

			m_ClientDC.SetTextColor(Icon->Color);
			DrawContent(Line, &*Icon, &LineRC);

			++Icon;
			++Icol;
			++Line;
		}

		if ((m_Style & 0x400'0000) && nCol > 0)
		{
			int PosX = m_ColumnRect.x - m_CharMiddleWidth * m_pScrollBar[1]->CurPos;
			if ((m_Style & 0x6'0000) == 0x6'0000)
				PosX += Level * m_nNonTextWidth;
			m_ClientDC.DrawHLine(PosX, m_ColumnRect.cx-1, m_ColumnRect.y + nCol * m_nLineHeight, m_SpaceColor);
		}
	}

	bool CWispList::PrevOpen(TListIter<CListStringItem> & It)
	{
		if (It->pParentItem == 0)
			return false;

		if (It == It->pParentItem->Begin())
		{
			if (It->pParentItem == &m_RootItem)
				return false;
			It = It->pParentItem; //???
			return true;
		}
		--It;
		while (It->uStatus & 1) //LIST_EXPAND
		{
			if (It->Size()==0) break;
			//--It;
			It = It->End()-1;
		}
		return true;
	}
	bool CWispList::NextOpen(TListIter<CListStringItem> & It)
	{
		if (m_RootItem.Size()==0)
			return false;
		if (It == m_RootItem.End())
			return false;
		if ((It->uStatus & 1) && It->Size()) //LIST_EXPAND
		{
			It = It->Begin();
			return true;
		}
		if (It->pParentItem == 0)
			return false;

		CListStringItem *Item = It->pParentItem;
		++It;
		if (It == Item->End())
		{
			do {
				if (Item == &m_RootItem) break;
				It = Item; //???
				++It;
				Item = Item->pParentItem;
			} while (It == Item->End());
		}
		return It != Item->End();
	}
	int CWispList::ScrollPage(int Delta)
	{
		if (m_RootIter == m_RootItem.End())
			return 0;
		int Index = 0;
		if (Delta > 0)
		{
			while (Delta > 0)
			{
				if (!NextOpen(m_RootIter)) break;
				--Delta;
				++Index;
			}
		} else
		if (Delta < 0)
		{
			while (Delta < 0)
			{
				if (!PrevOpen(m_RootIter)) break;
				++Delta;
				++Index;
			}
		}
		return Index;
	}

	int CWispList::Point2Iter(WISP_POINT *pPoint, TListIter<CListStringItem> & It)
	{
		if (pPoint->y >= m_LayoutRect.y &&
		    pPoint->y <  m_LayoutRect.y + m_LayoutRect.cy)
			return COL_LAYOUT; //0x1003;

		if (!PtInRect(&m_ColumnRect, pPoint))
			return -1;

		int nLine = (pPoint->y - m_ColumnRect.y) / m_nLineHeight;
		if (nLine > m_pScrollBar[0]->MaxPos || !m_RootItem.Size())
			return -1;

		It = m_RootIter;
		while (nLine > 0)
		{
			if (!NextOpen(It))
				return -1;
			--nLine;
		}

		int Pos = pPoint->x + m_CharMiddleWidth * m_pScrollBar[1]->CurPos;
		int MaxPos = m_ColumnRect.x + m_nNonTextWidth * It->GetItemLevel();
		if (Pos < MaxPos)
			return COL_EMPTY; //0x1004;

		int Index = 0;
		if (!m_Columns.Size())
			return -1;
		TListIter<stWispHeaderItem> Itc = m_Columns.Begin();
		while (Itc != m_Columns.End())
		{
			if (Index == m_nCtrlCol)
			{
				if (m_Style & 0x2'0000)
				{
					MaxPos += m_nCtrlBoxWidth;
					if (Pos < MaxPos)
						return It->Size() != 0 ? COL_EXPAND : COL_EMPTY; //0x1001 : 0x1004
				}
				if ((m_Style & 0x40'0000) && Pos < MaxPos + m_nCtrlBoxWidth)
					return COL_CHECK; //0x1002;
			}
			MaxPos = Itc->RCDIB.x + Itc->RCDIB.cx;
			if (Pos < MaxPos)
				return Index;
			++Index;
			++Itc;
		}
		return Index-1;
	}
	CListStringItem *CWispList::Point2Index(WISP_POINT *pPoint, int *pIndex)
	{
		TListIter<CListStringItem> Iter;
		int Index = Point2Iter(pPoint, Iter);
		if (Index < 0)
		{
			if (pIndex) *pIndex = -1;
			return nullptr;
		}
		if (pIndex) *pIndex = Index;
		return &*Iter;
	}

	CListStringItem *CWispList::FindItem(CListStringItem *Item, unsigned int uStatus)
	{
		if (!Item)
			Item = &m_RootItem;
		TListIter<CListStringItem> It = Item->Begin();
		while (It != Item->End())
		{
			if ((uStatus & It->uStatus) == uStatus)
				return &*It;
			++It;
		}
		return nullptr;
	}
	int CWispList::GetItemCount(CListStringItem *Item, bool Sub)
	{
		if (!Item)
			Item = &m_RootItem;
		int Size = Item->Size();
		if (Sub)
		{
			TListIter<CListStringItem> It = Item->Begin();
			while (It != Item->End())
			{
				Size += GetItemCount(&*It, true);
				++It;
			}
		}
		return Size;
	}
	CListStringItem *CWispList::GetItem(int Pos, CListStringItem *Item)
	{
		if (!Item)
			Item = &m_RootItem;
		if (Item->Size() == 0)
			return nullptr;
		if (Pos < 0 || Pos >= Item->Size())
			return &*(Item->End()-1);
		return &*Item->operator[](Pos);
	}
	CListStringItem *CWispList::GetRootItem()
	{
		return m_RootIter == m_RootItem.End() ? nullptr : &*m_RootIter;
	}
	bool CWispList::Next(TListIter<CListStringItem>& It)
	{
		if (It->pParentItem == 0)
			return false;
		return ++It != It->pParentItem->End();
	}
	bool CWispList::Prev(TListIter<CListStringItem>& It)
	{
		if (It->pParentItem == 0)
			return false;
		return --It != It->pParentItem->End();
	}
	bool CWispList::NextSibling(TListIter<CListStringItem>& It)
	{
		if (It->Size())
		{
			It = It->Begin();
			return true;
		}

		if (It->pParentItem == 0)
			return false;

		CListStringItem *Item = It->pParentItem;
		++It;
		if (It == Item->End())
		{
			do {
				if (Item == &m_RootItem) break;
				It = Item; //???
				++It;
				Item = Item->pParentItem;
			} while (It == Item->End());
		}
		return It != Item->End();
	}
	CListStringItem *CWispList::FullFindItem(CListStringItem *Item, unsigned int uStatus)
	{
		TListIter<CListStringItem> It = Item ? Item : m_RootItem.Begin();
		if (It == m_RootItem.End())
			return nullptr;
		if (Item == 0)
		{
			if ((uStatus & It->uStatus) == uStatus)
				return &*It;
		}
		while (NextSibling(It))
		{
			if ((uStatus & It->uStatus) == uStatus)
				return &*It;
		}
		return nullptr;
	}
	//GetNextSiblingItem
	CListStringItem *CWispList::GetNext(CListStringItem *Item, unsigned int uStatus)
	{
		if (!Item)
			return FindItem(0, uStatus);
		TListIter<CListStringItem> It = Item; //???
		while (Next(It))
		{
			if ((uStatus & It->uStatus) == uStatus)
				return &*It;
		}
		return nullptr;
	}
	//GetPrevSiblingItem
	CListStringItem *CWispList::GetPrev(CListStringItem *Item, unsigned int uStatus)
	{
		TListIter<CListStringItem> It = Item; //???
		while (Prev(It))
		{
			if ((uStatus & It->uStatus) == uStatus)
				return &*It;
		}
		return nullptr;
	}
	int CWispList::GetPosition(CListStringItem *Item)
	{
		if (Item->pParentItem == 0)
			return 0;
		int Index = 0;
		CListStringItem *Cur = FindItem(Item->pParentItem, 0);
		while (Cur)
		{
			if (Cur == Item)
				return Index;
			Cur = GetNext(Cur, 0);
			++Index;
		}
		return -1;
	}
	CListStringItem *CWispList::GetItemParent(CListStringItem *Item)
	{
		return Item->pParentItem;
	}
	int CWispList::GetItemLevel(CListStringItem *Item)
	{
		return Item->GetItemLevel();
	}
	int CWispList::GetParentLevel(CListStringItem *Item)
	{
		int Level = 0;
		while (Item)
		{
			Item = Item->pParentItem;
			++Level;
		}
		return Level;
	}

	CWispList::stWispHeaderItem *CWispList::InsertColumn(const WCHAR *lpszHeading, int nWidth, int nFormat, CWispDIB *pDIB, int nCol)
	{
		TListIter<stWispHeaderItem> It;
		if (nCol>=0)
		{
			It = m_Columns[nCol];
		} else
		{
			if (m_Columns.Size()>0)
				(m_Columns.End()-1)->nRealWidth = (m_Columns.End()-1)->nInitWidth;
			It = m_Columns.End();
		}
		TListIter<stWispHeaderItem> p = m_Columns.InsertBefore(It);
		p->strHeading = lpszHeading;
		p->nFormat = nFormat;
		p->pDIB = pDIB;
		if (nWidth < 0)
			nWidth = m_ClientDC.GetTextExtent(lpszHeading, -1, 0) + m_nSpaceWidth*2 + 2;
		p->nRealWidth = nWidth;
		p->nInitWidth = nWidth;
		RecalcLayout();
		return &*p;
	}
	CListStringItem::CONTENT *CWispList::GetItemContent(CListStringItem *Item, int nCol)
	{
		if (nCol >= m_Columns.Size()) //> Size-1
			return nullptr;
		
		if (Item->Contents.Size() <= nCol)
	{
		int i = nCol - Item->Contents.Size() + 1;
		//for (int i = nCol; i <= (Item->Contents.Size()+1); ++i)
		do {
			TListIter<CListStringItem::CONTENT> Itc = Item->Contents.Append();
			Itc->Number = 0;
			Itc->v1 = 0;
			Itc->v2 = 0;
			Itc->pDIB = 0;
			Itc->Color = m_ItemColor;
			--i;
		} while (i);
	}
		return &*Item->Contents[nCol];
	}
	
	void CWispList::WalkChecked(CListStringItem *Item)
	{
		while ((Item = Item->pParentItem) != &m_RootItem)
		{
			if (((Item->Checked <= 0) || (Item->Checked >= Item->Size())) && //WARN TODO
			     Item->SemiChecked <= 0 || (Item->uStatus & 4)) //LIST_SEMICHECKED
			{
				if (Item->Checked == Item->Size() && !(Item->uStatus & 2)) //LIST_CHECKED
				{
					if (Item->uStatus & 4) //LIST_SEMICHECKED
					{
						Item->uStatus &= ~4; //LIST_SEMICHECKED
						--Item->pParentItem->SemiChecked;
					}
					Item->uStatus |= 2; //LIST_CHECKED
					++Item->pParentItem->Checked;
				} else
				if (!Item->Checked && !Item->SemiChecked)
				{
					if (Item->uStatus & 2) //LIST_CHECKED
					{
						Item->uStatus &= ~2; //LIST_CHECKED
						--Item->pParentItem->Checked;
					} else
					if (Item->uStatus & 4) //LIST_SEMICHECKED
					{
						Item->uStatus &= ~4; //LIST_SEMICHECKED
						--Item->pParentItem->SemiChecked;
					}
				}
				
			} else
			{
				if (Item->uStatus & 2) //LIST_CHECKED
				{
					Item->uStatus &= ~2; //LIST_CHECKED
					--Item->pParentItem->Checked;
				}
				Item->uStatus |= 4; //LIST_SEMICHECKED
				++Item->pParentItem->SemiChecked;
			}			
		}
	}

	void CWispList::UpdateItemContentColor(CListStringItem *Item)
	{
		TListIter<CListStringItem::CONTENT> It = Item->Contents.Begin();
		while (It != Item->Contents.End())
		{
		//	It->String.Empty();
		//	It->Number = 0;
			It->Color = m_ItemColor;
			++It;
		}
	}

	void CWispList::ClearItemContent(CListStringItem *Item)
	{
		TListIter<CListStringItem::CONTENT> It = Item->Contents.Begin();
		while (It != Item->Contents.End())
		{
			It->String.Empty();
			It->Number = 0;
			It->v1 = 0;
			It->v2 = 0;
			It->Color = m_ItemColor;
			++It;
		}
	}
	int CWispList::UpdateItemColor(CListStringItem *Item)
	{
		if (!Item)
			Item = &m_RootItem;

		TListIter<CListStringItem> It = Item->Begin();
		while (It != Item->End())
		{
			UpdateItemColor(&*It);
			UpdateItemContentColor(&*It);
			++It;
		}
		return Item->Size();
	}
	int CWispList::ClearItem(CListStringItem *Item)
	{
		if (!Item)
			Item = &m_RootItem;

		TListIter<CListStringItem> It = Item->Begin();
		while (It != Item->End())
		{
			ClearItemContent(&*It);
			++It;
		}
		return Item->Size();
	}
	void CWispList::Scroll(CListStringItem *Item, int Count)
	{
		CListStringItem *Next = Item->pParentItem; //???
		while (Next != &m_RootItem)
		{
			if (!(Next->uStatus & 1)) //LIST_EXPAND
				Expand(Next);
			Next = Next->pParentItem;
		}
	{
		TListIter<CListStringItem> It = Item;
		if (Count<0) Count = m_nVSBPage/2;
		while (Count>0)
		{
			if (!PrevOpen(It))
				break;
			--Count;
			Item = &*It;
		}
	}
	{
		int Index = 0;
		TListIter<CListStringItem> It = m_RootItem.Begin();
		while (It != m_RootItem.End())
		{
			if (&*It == Item)
			{
				SetScrollBarCurPos(0, Index, 1);
				break;
			}
			if (!NextOpen(It))
				break;
			++Index;
		}
	}
	}
	bool CWispList::CheckItemPage(CListStringItem *Item)
	{
		int nLine = 0;
		TListIter<CListStringItem> Iter = m_RootIter;
		while (nLine < m_nVSBPage)
		{
			if (&*Iter == Item) return true;
			if (!NextOpen(Iter)) break;
			++nLine;
		}
		return false;
	}
	int CWispList::GetItemIndex(CListStringItem *Item)
	{
		int nIndex = 0;
		TListIter<CListStringItem> Iter = m_RootIter;
		while (nIndex < m_nVSBPage)
		{
			if (&*Iter == Item) return nIndex;
			if (!NextOpen(Iter)) break;
			++nIndex;
		}
		if (&*Iter == Item) return nIndex;
		return -1;
	}
	bool CWispList::GetItemRect(CListStringItem *Item, int nCol, WISP_RECT *pRect)
	{
		int nIndex = GetItemIndex(Item);
		if (nIndex < 0)
			return false;
		pRect->y = m_ColumnRect.y + nIndex * m_nLineHeight;
		pRect->cy = m_nLineHeight;
		pRect->x = 0;
		pRect->cx = 0;
		TListIter<stWispHeaderItem> Itc = m_Columns.Begin();
		while (Itc != m_Columns.End())
		{
			if (nCol == 0)
			{
				pRect->x = Itc->RCDIB.x;
				pRect->cx = Itc->RCDIB.cx;
				break;
			}
			--nCol;
			++Itc;
		}
		//+++
		//if (nCol == m_nCtrlCol) //TODO
		if (m_Style & 0x2'0000)
			pRect->x += m_nCtrlBoxWidth;
		if (m_Style & 0x40'0000)
			pRect->x += m_nCtrlBoxWidth;
		return true;
	}
	int CWispList::GetItemText(CListStringItem *Item, int nCol, WCHAR *Des, int Len)
	{
		CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
		if (!Content)
			return 0;
		if (Content->String.m_nLength == 0)
		{
			Des[0] = 0;
			return 0;
		}
		if (IsColorString(Content->String.m_pData))
			return TStrCpyLimit(Des, ((const unsigned long*)Content->String.m_pData)+1, Len); //???
		return TStrCpyLimit(Des, Content->String.m_pData, Len);
	}
	unsigned long long CWispList::GetItemValue(CListStringItem *Item, int nCol)
	{
		CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
		//if (!Content)
		//	return false;
		return Content->Number;
	}

	bool CWispList::SetItemText(CListStringItem *Item, int nCol, const WCHAR *lpszString)
	{
		CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
		if (!Content)
			return false;
		Content->String = lpszString;
		return true;
	}
	bool CWispList::SetItemTextColor(CListStringItem *Item, int nCol, const unsigned long *lpColorString)
	{
		CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
		if (!Content)
			return false;
		int Len = TStrLen((const unsigned long*)lpColorString)*2+2;
		Content->String.Preallocate(Len+2, 0);
	//	TMemCpy(Content->String.operator WCHAR*()+2, (const WCHAR*)lpColorString, Len);
		TMemCpy(Content->String.data()+2, (const WCHAR*)lpColorString, Len);
		Content->String.m_pData[0] = -1;
		Content->String.m_pData[1] = -1;
		Content->String.m_nLength = Len/2-1;
		return true;
	}
	bool CWispList::SetItemValue(CListStringItem *Item, int nCol, unsigned long long Value)
	{
		CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
		if (!Content)
			return false;
		Content->Number = Value;
		return true;
	}
	bool CWispList::SetItemDIB(CListStringItem *Item, int nCol, CWispDIB *pDIB)
	{
		CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
		if (!Content)
			return false;
		Content->pDIB = pDIB;
		return true;
	}
	bool CWispList::SetItemColor(CListStringItem *Item, int nCol, unsigned long Color)
	{
		if (nCol < 0)
		{
			TListIter<CListStringItem::CONTENT> Itc = Item->Contents.Begin();
			while (Itc != Item->Contents.End())
			{
				Itc->Color = Color;
				++Itc;
			}
		} else
		{
			CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
			if (!Content)
				return false;
			Content->Color = Color;
		}
		return true;
	}
	bool CWispList::SetItemText(CListStringItem *Item, int nCol, const char *Text)
	{
		WCHAR Buffer[512];
		TStrCpyLimit(Buffer, Text, lenof(Buffer));
		return SetItemText(Item, nCol, Buffer);
	}
	bool CWispList::SetItemFormat(CListStringItem *Item, int nCol, unsigned long long Value, const WCHAR *Format)
	{
		CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
		if (!Content)
			return false;
		Content->String.Format(Format, Value);
		return true;
	}
	
	bool CWispList::DeleteItem(CListStringItem *Item)
	{
		if (Item->uStatus & 2) //LIST_CHECKED
			--Item->pParentItem->Checked;
		if (Item->uStatus & 4) //LIST_SEMICHECKED
			--Item->pParentItem->SemiChecked;
		WalkChecked(Item);
		TListIter<CListStringItem> SaveIter = m_RootIter;
		if ((&*m_RootIter == Item || Item->ItemHasChildren(&*m_RootIter)) && !Prev(m_RootIter))
		{
			m_RootIter = SaveIter; //???
			if (!Next(m_RootIter))
				m_RootIter = m_RootItem.End();
		}
		if (&*m_FocusedIter == m_pTipItem)
			HideTip();
		if (&*m_FocusedIter == Item || (m_FocusedIter != m_RootItem.End() && Item->ItemHasChildren(&*m_FocusedIter)))
			m_FocusedIter = m_RootItem.End();
		AdjustScrollBarMaxPos(0, -Item->GetChildItemExpandCount());
		CListStringItem *pParentItem = Item->pParentItem;
		TListIter<CListStringItem> It = Item;
		pParentItem->Remove(It);
		if (pParentItem != &m_RootItem && !pParentItem->End())
		{
			if (pParentItem->uStatus & 1) //LIST_EXPAND
				pParentItem->uStatus &= ~1; //LIST_EXPAND
		}
		return true;
	}
	bool CWispList::DeleteAllChildrenItems(CListStringItem *Item)
	{
		if (Item)
		{
			if (Item->uStatus & 1) //LIST_EXPAND
				AdjustScrollBarMaxPos(0, -Item->GetChildItemExpandCount());
			if (m_FocusedIter != m_RootItem.End() && Item->ItemHasChildren(&*m_FocusedIter))
				m_FocusedIter = m_RootItem.End();
			if (m_RootIter != m_RootItem.End() && Item->ItemHasChildren(&*m_RootIter))
				m_RootIter = Item; //???
			Item->Clear();
			Item->Checked = 0;
			Item->SemiChecked = 0;
			Item->uStatus &= ~1; //LIST_EXPAND
		} else
		{
			if (m_FocusedIter != m_RootItem.End() && m_RootItem.ItemHasChildren(&*m_FocusedIter))
				m_FocusedIter = m_RootItem.End();
			m_RootItem.Clear();
			m_RootIter = m_RootItem.End();
			m_RootItem.Checked = 0;
			m_RootItem.SemiChecked = 0;
			SetScrollBarInfo(0, 0, 0, m_pScrollBar[0]->Page, 0, 1);
		}
		return true;
	}

	CListStringItem *CWispList::InsertItem(const WCHAR *pString, CListStringItem *pParentItem, unsigned int uStatus, CWispDIB *pDIB, int Pos)
	{
		if (!pParentItem)
			pParentItem = &m_RootItem;
		if (pParentItem->uStatus & 2) //LIST_CHECKED
			uStatus |= 2; //LIST_CHECKED
		TListIter<CListStringItem> It = pParentItem->InsertBefore(pParentItem->operator[](Pos));
		
		TListIter<CListStringItem::CONTENT> Itc = It->Contents.Append();
		Itc->String = pString;
		Itc->pDIB = pDIB;
		Itc->Color = m_ItemColor;

		It->pParentItem = pParentItem;
		It->uStatus = uStatus & ~2; //LIST_CHECKED
		It->Checked = 0;
		It->SemiChecked = 0;
	//	It->STRING = pString; //DEBUG
		if (uStatus & 2) //LIST_CHECKED
			Checked(&*It, true);

		if (pParentItem->uStatus & 1) //LIST_EXPAND
			AdjustScrollBarMaxPos(0, 1);

		if (m_RootIter == m_RootItem.End())
			m_RootIter = It;
		return &*It;
	}

	CListStringItem *CWispList::InsertItemStr(const char *pString, CListStringItem *pParentItem, unsigned int uStatus, CWispDIB *pDIB, int Pos)
	{
		WCHAR Buffer[512];
		if (pString)
			TStrCpyLimit(Buffer, pString, lenof(Buffer));
		else	Buffer[0] = 0;
		return InsertItem(Buffer, pParentItem, uStatus, pDIB, Pos);
	}
	CListStringItem *CWispList::InsertItemStr(const WCHAR *pString, CListStringItem *pParentItem, unsigned int uStatus, CWispDIB *pDIB, int Pos)
	{
		WCHAR Buffer[512];
		TStrCpyLimit(Buffer, pString, lenof(Buffer));
		return InsertItem(Buffer, pParentItem, uStatus, pDIB, Pos);
	}
	CListStringItem *CWispList::InsertItemValue(unsigned long long Value, CListStringItem *pParentItem, const WCHAR *Format, unsigned int uStatus, CWispDIB *pDIB, int Pos)
	{
		CListStringItem *Item = InsertItem(nullptr, pParentItem, uStatus, pDIB, Pos);
		CListStringItem::CONTENT *Content = GetItemContent(Item, 0);
		Content->String.Format(Format, Value);
		return Item;
	}

	int CWispList::Adjust(CListStringItem *Item, int Pos)
	{
		if (Pos < 0) return 0;
		if (!Item)
			Item = &m_RootItem;
		int Size = Pos - GetItemCount(Item, 0);
		if (Size<0)
		{
			CListStringItem *PosItem = GetItem(Pos, Item);
			Size = -Size;
			for (int i = 0; i < Size; ++i)
			{
				CListStringItem *NextItem = GetNext(PosItem, 0);
				DeleteItem(PosItem);
				PosItem = NextItem;
			}
		} else
		if (Size>0)
		{
			for (int i = 0; i < Size; ++i)
			{
				InsertItem((WCHAR*)0, Item, 0, 0, -1);
			}
		}
		return 0;
	}

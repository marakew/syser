
#include "wispedit.hpp"
#include "wispbase.hpp"

	CWispEdit::CWispEdit()
	{
		m_CurrentCaretHot = 0;
		m_DebugValue = 0;
		m_CtrlType = WISP_CTRL_EDIT;
	}
	CWispEdit::~CWispEdit()
	{
	}
	
	bool CWispEdit::CreateWnd(const WCHAR *Name, int x, int y, int cx, int cy, CWispBaseWnd *pParentWnd, unsigned int CmdID, unsigned int uStyle, unsigned int ShowMode)
	{
		if ( !(uStyle & 0x20000000) )
		{
			uStyle &= ~0x2200000;
			if (uStyle & 0x4010000)
				;
			else	uStyle |= 0x8000000;
		} else
		if (uStyle & 0x4010000)
		{
			uStyle &= ~0x1100000;
			if (uStyle & 0x4010000)
				;
			else	uStyle |= 0x8000000;
		} else
		{
			uStyle |= 0x8000000;
		}
		if (!CWispWnd::CreateWnd(Name, x, y, cx, cy, pParentWnd, CmdID, uStyle & ~1, ShowMode))
			return false;

		m_StringMaxLen = (uStyle & 0x3200000) != 0 ? 30001 : 0;
		m_BorderSize = uStyle & 2;
		if (uStyle & 0x200000)
			EnableScrollBar(0, 1, 1);
		if (uStyle & 0x100000)
			EnableScrollBar(1, 1, 1);
		CreateCaret(WISP_DI_CARET);
		m_LineMaxLen = (uStyle & 0x20000000) != 0 ? 1024 : 0;

		if (uStyle & 0x10000)
		{
			m_Caret.x = m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width;
		} else
		if (uStyle & 0x4000000)
		{
			m_Caret.x = m_ClientRect.cx / 2;
		} else
		{
			m_Caret.x = 0;
		}

		if (uStyle & 0x20000000)
			m_Caret.y = 0;
		else	m_Caret.y = (m_ClientRect.cy - m_Caret.pDIB->m_PixelBuf.Height)/2;

		if ( !(m_Style & 0x40000))
			ShowCaret(true);
		m_bSelectFlag = 0;
		m_SelectBegin = 0;
		m_SelectEnd = 0;
		bMouseLeftButtonDown = 0;
		m_SelectColor = m_pWispBase->m_pWispDrawObj->m_crSystem[49];
		return true;
	}

	bool CWispEdit::Create(WISP_RECT& RectAtWnd, unsigned int uStyle, CWispWnd *pParentWnd, unsigned int CmdID)
	{
		return CreateWnd(0, RectAtWnd.x, RectAtWnd.y, RectAtWnd.cx, RectAtWnd.cy, pParentWnd, CmdID, uStyle, WISP_SH_MINSIZE);
	}

	WISP_MSG_MAP_BEGIN(CWispEdit)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_MOUSE_MOVE, OnMouseMove)
		WISP_MSG_MAP(WISP_WM_MOUSE_LEAVE, OnMouseLeave)
		WISP_MSG_MAP(WISP_WM_LOST_FOCUS, OnLostFocus)
		WISP_MSG_MAP(WISP_WM_GET_FOCUS, OnGetFocus)
		WISP_MSG_MAP_ON_EVENT
		WISP_MSG_MAP(WISP_WM_CHAR, OnChar)
		WISP_MSG_MAP(WISP_WM_UPDATE_BORDER, OnUpdateBorder)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
	WISP_MSG_MAP_END(CWispWnd)

	bool CWispEdit::OnEventCopy(WISP_MSG *pMsg)
	{
		if (!m_bSelectFlag)
			return false;

		int Begin = MIN(m_SelectBegin, m_SelectEnd);
		int End = MAX(m_SelectBegin, m_SelectEnd);

		TWideString String;
		String.Empty();
		String += &m_WndText.operator const WCHAR*()[Begin];
		String.Delete(End - Begin, Begin + String.m_nLength - End);
		
		m_pWispBase->SetClipboard(String.operator const WCHAR*());
		return true;
	}
	bool CWispEdit::OnEventPaste(WISP_MSG *pMsg)
	{
		if (!(m_Style & 0x40000))
		{
			m_pWispBase->GetClipboard(); //TODO
			if (!m_pWispBase->m_ClipString.IsEmpty())
			{
				InsertChar(m_pWispBase->m_ClipString.operator const WCHAR*(), -1, m_pWispBase->m_ClipString.m_nLength);
				WISP_POINT CaretPos;
				GetCaretPos(&CaretPos);
				int TextExt = m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), m_pWispBase->m_ClipString.m_nLength, m_CurrentCaretHot);
				if (m_Style & 0x8000000)
				{
					if (TextExt + CaretPos.x <= m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width)
					{
						CaretPos.x += TextExt;
					} else
					{
						CaretPos.x = m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width;
						if ((m_Style & 0x1000000) && m_CurrentCaretHot != m_WndText.m_nLength)
							CaretPos.x = (CaretPos.x/4)*3;
					}
					m_CurrentCaretHot += m_pWispBase->m_ClipString.m_nLength;
					SetCaretPos(CaretPos.x, CaretPos.y);
				}
				Update((WISP_RECT*)0);
			}
		}
		return true;
	}

	WISP_EVENT_MAP_BEGIN(CWispEdit)
		WISP_EVENT_MAP(WISP_ID_COPY, OnEventCopy)
		WISP_EVENT_MAP(WISP_ID_PASTE, OnEventPaste)
	WISP_EVENT_MAP_END

	bool CWispEdit::OnGetFocus(WISP_MSG *pMsg)
	{
		SendCommand(0x80000702, 0, (void*)m_WndText.operator const WCHAR*());//WISP_ID_EDIT_GET_FOCUS
		return true;
	}
	bool CWispEdit::OnLostFocus(WISP_MSG *pMsg)
	{
		SendCommand(0x80000701, 0, (void*)m_WndText.operator const WCHAR*()); //WISP_ID_EDIT_LOST_FOCUS
		if (bMouseLeftButtonDown == true)
		{
			bMouseLeftButtonDown = false;
			m_bSelectFlag = false;
		}
		return true;
	}
	bool CWispEdit::OnChar(WISP_MSG *pMsg)
	{
		if (m_Style & 0x40000)
			return true;

		switch (pMsg->Char.Char)
		{
		case 8: //BackKey()
			Update((WISP_RECT*)0);
			break;
		case 10: //NewLine
			break;
		case 13: //EnterKey
			Update((WISP_RECT*)0);
			break;
		default:
			WCHAR Char = pMsg->Char.Char;
			m_ClientDC.GetTextExtent(&Char, 1, 0);
			if (!IsInsertChar(Char))
			{
				Update((WISP_RECT*)0);
				break;
			}
			InsertChar(&Char, -1, 1);
			int TextExt = m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), m_CurrentCaretHot + 1, 0);
			TextExt -= m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), m_CurrentCaretHot, 0);
			WISP_POINT CaretPos;
			GetCaretPos(&CaretPos);
			if (m_Style & 0x8000000)
			{
				int PosX = m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width;
				if (TextExt + CaretPos.x > PosX)
				{
					CaretPos.x = PosX;
					if (m_Style & 0x1000000 && m_CurrentCaretHot != m_WndText.m_nLength)
						CaretPos.x = (CaretPos.x/4)*3;
				} else
				{
					CaretPos.x += TextExt;
				}
			}
			++m_CurrentCaretHot;
			const WCHAR *pStr = &m_WndText.operator const WCHAR*()[m_CurrentCaretHot - GetCurrentLinePosition(-1)];
			TextExt = m_ClientDC.GetTextExtent(pStr, GetLineWidth(pStr), 0);
			int PosX = m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width;
			if (TextExt < PosX)
			{
				if (m_Style & 0x4000000)
					CaretPos.x = (PosX - TextExt)/2 + m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), m_CurrentCaretHot, 0);
				else
				if (m_Style & 0x10000)
					CaretPos.x = PosX;
			}
			SetCaretPos(CaretPos.x, CaretPos.y);
			Update(&m_ClientRect);
		}
		return false;
	}
	bool CWispEdit::OnUpdateClient(WISP_MSG *pMsg)
	{
		if ( !(m_Style & 0x40) && (m_ParentWnd->m_CtrlType != WISP_CTRL_FORM || !(m_Style & 0x40000)) )
			m_pWispBase->m_pWispDrawObj->DrawCtrlClient(&m_WindowDC, &m_ClientRect);

		if (!m_WndText.IsEmpty())
		{
			WISP_POINT CaretPos;
			GetCaretPos(&CaretPos);

			m_CurrentShowRect.x = CaretPos.x - m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), m_CurrentCaretHot, 0);
			m_CurrentShowRect.cx = (m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width) - m_CurrentShowRect.x;
			m_CurrentShowRect.y = (m_Style & 0x20000000) ? CaretPos.y : 0;
			m_CurrentShowRect.cy = m_ClientRect.cy;

			WISP_RECT Rect = m_CurrentShowRect;

			int TextExt = 0;
			int Begin = 0;
			int End = 0;
			if (m_bSelectFlag && m_SelectBegin != m_SelectEnd)
			{
				Begin = MIN(m_SelectBegin, m_SelectEnd);
				End = MAX(m_SelectBegin, m_SelectEnd);
				if (Begin)
				{
					m_ClientDC.m_TextStyle = 0;
					m_ClientDC.DrawString(m_WndText.operator const WCHAR*(), &m_CurrentShowRect, 0x24, Begin, -1);
					TextExt = m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), Begin, 0);
	                        }
			}

			if (End != Begin)
			{
				Rect.x += TextExt;
				Rect.cx -= TextExt;
				m_ClientDC.m_TextStyle = 1;
				m_ClientDC.SetTextBKColor(m_SelectColor);
				m_ClientDC.DrawString(&m_WndText.operator const WCHAR*()[Begin], &Rect, 0x24, End - Begin, -1);
				TextExt = m_ClientDC.GetTextExtent(&m_WndText.operator const WCHAR*()[Begin], End - Begin, 0);
			}

			if (End != m_WndText.m_nLength)
			{
				Rect.x += TextExt;
				Rect.cx -= TextExt;
				m_ClientDC.m_TextStyle = 0;
				m_ClientDC.DrawString(&m_WndText.operator const WCHAR*()[End], &Rect, 0x24, 0, -1);
			}
		}
		return false;
	}
	bool CWispEdit::OnUpdateBorder(WISP_MSG *pMsg)
	{
		if (m_Style & 2)
			m_pWispBase->m_pWispDrawObj->DrawCtrlBorder(&m_WindowDC, &m_WindowRect, m_BorderSize);
		return false;
	}
	bool CWispEdit::OnCreate(WISP_MSG *pMsg)
	{
		m_CaretColor = m_pWispBase->m_pWispDrawObj->m_crSystem[0];
		return true;
	}
	bool CWispEdit::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (m_Style & 0x40000)
			return true;

		if (pMsg->KeyEvent.KeyType == VK_LBUTTON ||
		    pMsg->KeyEvent.KeyType == VK_RBUTTON)
			return OnMouseEvent(pMsg);

		if (!pMsg->KeyEvent.bKeyDown)
		{
			if (!SendCommand(0x80000704, pMsg->KeyEvent.KeyType, 0)) //WISP_ID_EDIT_KEYUP
				return false;

			if (pMsg->KeyEvent.KeyType == VK_SHIFT)
			{
				bMouseLeftButtonDown = 0;
				if (m_bSelectFlag)
					m_SelectEnd = m_CurrentCaretHot;
				Update((WISP_RECT*)0);
			}
			return true;
		}

		if (!SendCommand(0x80000703, pMsg->KeyEvent.KeyType, 0)) //WISP_ID_EDIT_KEYDOWN
			return false;

		WISP_MSG Msg;

		switch (pMsg->KeyEvent.KeyType)
		{
		case 0x10000010://WISP_SK_SHIFT VK_SHIFT
			if (bMouseLeftButtonDown != 1)
			{
				bMouseLeftButtonDown = 1;
				m_SelectBegin = m_CurrentCaretHot;
				m_SelectEnd = m_CurrentCaretHot;
			}
			Update((WISP_RECT*)0);
			return false;
		case VK_END://0x23
		case 0x10000023://WISP_SK_SHIFT VK_END
			OnEndKey(pMsg);
			Update((WISP_RECT*)0);
			return true;
		case VK_HOME:
		case 0x10000024://WISP_SK_SHIFT VK_HOME
			OnHomeKey(pMsg);
			Update((WISP_RECT*)0);
			return true;
		case VK_LEFT://0x25
		case 0x10000025://WISP_SK_SHIFT VK_LEFT
		case 0x40000025://WISP_SK_CTRL VK_LEFT
			if (!m_pWispBase->m_KeyMap[VK_CONTROL])
				OnLeftKey(pMsg);
			else	OnHomeKey(pMsg);
			Update((WISP_RECT*)0);
			return true;
		case VK_RIGHT: //0x27
		case 0x10000027://WISP_SK_SHIFT VK_RIGHT
		case 0x40000027://WISP_SK_CTRL VK_RIGHT
			if (!m_pWispBase->m_KeyMap[VK_CONTROL])
				OnRightKey(pMsg);
			else	OnEndKey(pMsg);
			Update((WISP_RECT*)0);
			return true;
		case 0x4000002D://WISP_SK_CTRL VK_INSERT
		case 0x40000043://WISP_SK_CTRL C
			Msg.hWnd = this;
			Msg.Msg = WISP_WM_EVENT;
			Msg.Command.CmdID = WISP_ID_COPY;
			m_pWispBase->PostMessage(&Msg);
			return false;
		case 0x1000002D://WISP_SK_SHIFT VK_INSERT
		case 0x40000056://WISP_SK_CTRL V
			Msg.hWnd = this;
			Msg.Msg = WISP_WM_EVENT;
			Msg.Command.CmdID = WISP_ID_PASTE;
			m_pWispBase->PostMessage(&Msg);
			return false;
		case VK_DELETE://0x2E
			OnDeleteKey(pMsg);
			Update((WISP_RECT*)0);
			return true;
		case VK_BACK://0x
			OnBackKey(pMsg);
			Update((WISP_RECT*)0);
			return true;
		case 0x40000041://WISP_SK_CTRL A
			if (m_WndText.m_nLength)
				Select(0, m_WndText.m_nLength);
			Update((WISP_RECT*)0);
			return false;
		default:
			Update((WISP_RECT*)0);
			return true;
		}
		return true;
#if 0
//////////////////////////////
        CHAR_v5 = pMsg->u.field_8;
        if ( CHAR_v5 > 0x10000023 )
        {
                if ( CHAR_v5 > 0x40000027 )
                {
                        switch ( CHAR_v5 )
                        {
                        case 0x4000002D:// VK_INSERT
                        case 0x40000056:// V
                                Msg.hWnd = this;
                                Msg.Msg = WISP_WM_EVENT;
                                Msg.u.field_8 = WISP_ID_PASTE;
                                m_pWispBase->PostMessage(&Msg);
                                result = 0;
                                break;
                        case 0x40000041:// A
                                if (m_WndText.m_nLength)
                                        Select(0, m_WndText.m_nLength);
				Update((WISP_RECT*)0);
			        return 0;
                        case 0x40000043:// C
				Msg.hWnd = this;
				Msg.Msg = WISP_WM_EVENT;
				Msg.u.field_8 = WISP_ID_COPY;
				m_pWispBase->PostMessage(&Msg);
				return 0;
                        default:
			        Update((WISP_RECT*)0);
			        return 1;

                        }
                        return result;
                }
                if ( CHAR_v5 == 0x40000027 )    // VK_RIGHT
		{
                        if (!m_pWispBase->m_KeyMap[VK_CONTROL])
                                OnRightKey(pMsg);
	                else	OnEndKey(pMsg);
	                Update((WISP_RECT*)0);
	                return 1;
		}

                if ( CHAR_v5 <= 0x10000027 )
                {
                        if (CHAR_v5 != 0x10000027)// VK_RIGHT
                        {
                                v7 = CHAR_v5 - 0x10000024;
                                v6 = v7 == 0;
                                goto LABEL_16;
                        }
//LABEL_36:
			//CHAR_v5 == 0x10000027
                        if (!m_pWispBase->m_KeyMap[VK_CONTROL])
                                OnRightKey(pMsg);
	                else	OnEndKey(pMsg);
	                Update((WISP_RECT*)0);
	                return 1;
                }

                if ( CHAR_v5 != 0x1000002D )    // VK_INSERT
                {
                        if (CHAR_v5 != 0x40000025)// VK_LEFT
			{
		                Update((WISP_RECT*)0);
	        	        return 1;
			}
			//CHAR_v5 == 0x40000025
                        //goto LABEL_34;
                        if (!m_pWispBase->m_KeyMap[VK_CONTROL])
                                OnLeftKey(pMsg);
                        else	OnHomeKey(pMsg);
	                Update((WISP_RECT*)0);
	                return 1;
                }
		//CHAR_v5 == 0x1000002D
                Msg.hWnd = this;
                Msg.Msg = WISP_WM_EVENT;
                Msg.u.field_8 = WISP_ID_COPY;
                m_pWispBase->PostMessage(&Msg);
                return 0;
        }

        if (CHAR_v5 == 0x10000023)            // VK_END
        {
//END_UP_R_1_LABEL_25:
                OnEndKey(pMsg);
                Update((WISP_RECT*)0);
                return 1;
        }

        if ( CHAR_v5 <= VK_END )
        {
                if ( CHAR_v5 != VK_END )
                {
                        if (CHAR_v5 == VK_BACK)
                            OnBackKey(pMsg);
	                Update((WISP_RECT*)0);
	                return 1;
                }
		//CHAR_v5 == VK_END
                OnEndKey(pMsg);
                Update((WISP_RECT*)0);
                return 1;
        }

        if ( CHAR_v5 <= VK_RIGHT )
        {
                if ( CHAR_v5 != VK_RIGHT )
                {
                        v7 = CHAR_v5 - VK_HOME;
                        v6 = v7 == 0;
LABEL_16:
			//CHAR_v5 == 0x10000024
                        if (v6)
                        {
//HOME_U_R_1_LABEL_31:
				//CHAR_v5 == VK_HOME
                                OnHomeKey(pMsg);
		                Update((WISP_RECT*)0);
		                return 1;

                        }
                        if (v7 != 1)
			{
		                Update((WISP_RECT*)0);
		                return 1;
			}

//LABEL_34:
			////CHAR_v5 == VK_LEFT
                        if (!m_pWispBase->m_KeyMap[VK_CONTROL])
                                OnLeftKey(pMsg);
                        else	OnHomeKey(pMsg);
	                Update((WISP_RECT*)0);
	                return 1;

                }

		//CHAR_v5 == VK_RIGHT
                //goto LABEL_36;
                if (!m_pWispBase->m_KeyMap[VK_CONTROL])
                        OnRightKey(pMsg);
	        else	OnEndKey(pMsg);
	        Update((WISP_RECT*)0);
	        return 1;

        }

		if (CHAR_v5 == VK_DELETE)
		{
			OnDeleteKey(pMsg);
			Update((WISP_RECT*)0);
			return 1;
		}
		if (CHAR_v5 == 0x10000010)            // VK_SHIFT
		{
			if (bMouseLeftButtonDown != 1)
			{
				bMouseLeftButtonDown = 1;
				m_SelectBegin = m_CurrentCaretHot;
				m_SelectEnd = m_CurrentCaretHot;
			}

			Update((WISP_RECT*)0);
			return 0;
		}
		Update((WISP_RECT*)0);
		return 1;
#endif
	}
	bool CWispEdit::OnMouseEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.KeyType == VK_RBUTTON)
			return true;
		if (pMsg->KeyEvent.bKeyDown)
			return OnMouseLeftButtonDown(pMsg);
		return OnMouseLeftButtonUp(pMsg);
	}
	bool CWispEdit::OnMouseLeave(WISP_MSG *pMsg)
	{
		if (bMouseLeftButtonDown == true)
		{
			if (pMsg->bMsgLBTDown) return false;
			bMouseLeftButtonDown = false;
			m_bSelectFlag = false;
		}
		return true;
	}
	bool CWispEdit::OnMouseLeftButtonUp(WISP_MSG *pMsg)
	{
		if (m_bSelectFlag)
		{
			WISP_POINT CaretPos;
			GetCaretPos(&CaretPos);
			WISP_POINT Point;
			int Index = GetCharIndexByPoint(pMsg->MsgMouseWndPT, Point);
			if (Index != m_CurrentCaretHot)
				SetCaretPos(Point.x, CaretPos.y);
			m_CurrentCaretHot = Index;
			m_SelectEnd = Index;
			Update((WISP_RECT*)0);
		}
		bMouseLeftButtonDown = false;
		return true;
	}
	bool CWispEdit::OnMouseLeftButtonDown(WISP_MSG *pMsg)
	{
		if (m_bSelectFlag)
			m_bSelectFlag = 0;

		WISP_POINT CaretPos;
		GetCaretPos(&CaretPos);
		WISP_POINT Point;
		int Index = GetCharIndexByPoint(pMsg->MsgMouseWndPT, Point);
		if (Index != m_CurrentCaretHot)
			SetCaretPos(Point.x, CaretPos.y);
		m_CurrentCaretHot = Index;
		m_SelectBegin = Index;
		m_SelectEnd = Index;
		bMouseLeftButtonDown = true;
		Update((WISP_RECT*)0);
		return true;
	}
	bool CWispEdit::OnMouseMove(WISP_MSG *pMsg)
	{
		if (pMsg->bMsgLBTDown)
		{
			WISP_POINT CaretPos;
			GetCaretPos(&CaretPos);
			WISP_POINT Point;
			int Index = GetCharIndexByPoint(pMsg->MsgMouseWndPT, Point);
			if (Point.x <= CaretPos.x)
			{
				m_SelectRect.x = Point.x;
				m_SelectRect.cx = CaretPos.x - Point.x;
			} else
			{
				m_SelectRect.x = CaretPos.x;
				m_SelectRect.cx = Point.x - CaretPos.x;
			}
			m_SelectRect.y = CaretPos.y;
			m_SelectRect.cy = 12;
			m_SelectEnd = Index;
			if (m_SelectBegin == m_SelectEnd)
				m_bSelectFlag = 0;
			else
			if ( !(m_Style & 0x40000) )
				m_bSelectFlag = 1;
			Update((WISP_RECT*)0);
		}
		return true;
	}
//	void OnKeyBack();
	void CWispEdit::OnLeftKey(WISP_MSG *pMsg)
	{
		WISP_POINT CaretPosPT;
		GetCaretPos(&CaretPosPT);
		if (m_CurrentCaretHot)
		{
			if (CaretPosPT.x - m_ClientDC.GetTextExtent(&m_WndText.operator const WCHAR*()[m_CurrentCaretHot-1], 1, 0) >= 0)
			{
				CaretPosPT.x -= m_ClientDC.GetTextExtent(&m_WndText.operator const WCHAR*()[m_CurrentCaretHot-1], 1, 0);
			} else
			{
				int X_v6 = m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), m_CurrentCaretHot-1, 0);
				if (X_v6 >= m_ClientRect.cx/4)
					X_v6 = m_ClientRect.cx/4;
				CaretPosPT.x = X_v6;
			}
			SetCaretPos(CaretPosPT.x, CaretPosPT.y);
			if (m_pWispBase->m_KeyMap[VK_SHIFT])
			{
				if (!m_bSelectFlag)
				{
					m_SelectBegin = m_CurrentCaretHot;
					m_SelectEnd = m_CurrentCaretHot-1;
					--m_CurrentCaretHot;
					m_bSelectFlag = 1;
				} else
				if (m_SelectBegin != m_CurrentCaretHot-1)
				{
					m_SelectEnd = m_CurrentCaretHot-1;
					--m_CurrentCaretHot;
					m_bSelectFlag = 1;
				} else
				{
					--m_CurrentCaretHot;
					m_bSelectFlag = 0;
				}
			} else
			{
				--m_CurrentCaretHot;
				m_bSelectFlag = 0;
			}
			return;
		}

		if (m_bSelectFlag)
		{
			if (!m_pWispBase->m_KeyMap[VK_SHIFT])
				m_bSelectFlag = 0;
		}
	}
	void CWispEdit::OnRightKey(WISP_MSG *pMsg)
	{
		if (m_CurrentCaretHot != m_WndText.m_nLength)
		{
			m_ClientDC.GetTextExtent(&m_WndText.operator const WCHAR*()[m_CurrentCaretHot], 1, 0);

			WISP_POINT CaretPosPT;
			GetCaretPos(&CaretPosPT);

			if (m_CurrentCaretHot >= m_WndText.m_nLength)
				return;
			if (CaretPosPT.x + m_ClientDC.GetTextExtent(&m_WndText.operator const WCHAR*()[m_CurrentCaretHot], 1, 0) <= m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width)
			{
				CaretPosPT.x += m_ClientDC.GetTextExtent(&m_WndText.operator const WCHAR*()[m_CurrentCaretHot], 1, 0);
			} else
			{
				CaretPosPT.x = (m_ClientRect.cx/4)*3;
			}
			SetCaretPos(CaretPosPT.x, CaretPosPT.y);
			if (m_pWispBase->m_KeyMap[VK_SHIFT])
			{
				if (!m_bSelectFlag)
				{
					m_SelectBegin = m_CurrentCaretHot;
					m_SelectEnd = m_CurrentCaretHot+1;
					m_CurrentCaretHot = m_CurrentCaretHot+1;
					m_bSelectFlag = 1;
				} else
				if (m_SelectBegin != m_CurrentCaretHot+1)
				{
					m_SelectEnd = m_CurrentCaretHot+1;
					m_CurrentCaretHot = m_CurrentCaretHot+1;
					m_bSelectFlag = 1;
				} else
				{
					++m_CurrentCaretHot;
					m_bSelectFlag = 0;
				}
			} else
			{
				++m_CurrentCaretHot;
				m_bSelectFlag = 0;
			}
			return;
		}

		if (m_bSelectFlag)
		{
			if (!m_pWispBase->m_KeyMap[VK_SHIFT])
				m_bSelectFlag = 0;
		}
	}
//	void OnUpKey(WISP_MSG *pMsg);
//	void OnDownKey(WISP_MSG *pMsg);
//	void OnReturnKey(WISP_MSG *pMsg);
	void CWispEdit::OnBackKey(WISP_MSG *pMsg)
	{
		if (m_CurrentCaretHot && m_CurrentCaretHot <= m_WndText.m_nLength)
			DeleteChar(m_CurrentCaretHot, 1, pMsg);
	}
	void CWispEdit::OnHomeKey(WISP_MSG *pMsg)
	{
		if (m_CurrentCaretHot)
		{
			int TextExt = m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), -1, 0);
			WISP_POINT CaretPosPT;
			GetCaretPos(&CaretPosPT);

			CaretPosPT.x = 0;
			int v8 = m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width;
			if (TextExt < v8)
			{
				if (m_Style & 0x4000000)
					CaretPosPT.x = (v8 - TextExt)/2;
				else
				if (m_Style & 0x10000)
					CaretPosPT.x = (v8 - TextExt);
			}

			SetCaretPos(CaretPosPT.x, CaretPosPT.y);

			if (m_pWispBase->m_KeyMap[VK_SHIFT])
			{
				if (!m_bSelectFlag)
				{
					m_SelectBegin = m_CurrentCaretHot;
					m_SelectEnd = 0;
					m_CurrentCaretHot = 0;
					m_bSelectFlag = 1;
				} else
				if (m_SelectBegin)
				{
					m_SelectEnd = 0;
					m_CurrentCaretHot = 0;
					m_bSelectFlag = 1;
				} else
				{
					m_bSelectFlag = 0;
					m_CurrentCaretHot = 0;
				}
			} else
			{
				m_bSelectFlag = 0;
				m_CurrentCaretHot = 0;
			}
			return;
		}

		if (m_bSelectFlag)
		{
			if (!m_pWispBase->m_KeyMap[VK_SHIFT])
				m_bSelectFlag = 0;
		}
	}
	void CWispEdit::OnEndKey(WISP_MSG *pMsg)
	{
		WISP_POINT CaretPosPT;
		GetCaretPos(&CaretPosPT);
		int TextExt = m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), -1, 0);
		int v7 = m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width;
		if (TextExt <= v7)
		{
			if (m_Style & 0x4000000)
			{
				CaretPosPT.x = (v7 - TextExt)/2 + m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), -1, 0);
			} else
			{
                                CaretPosPT.x = TextExt;
	                        if (m_Style & 0x10000)
		                        CaretPosPT.x = v7;
			}
		} else
		{
			CaretPosPT.x = v7;
		}
		SetCaretPos(CaretPosPT.x, CaretPosPT.y);

		if (m_pWispBase->m_KeyMap[VK_SHIFT])
		{
			if (m_bSelectFlag)
			{
				if (m_SelectBegin == m_WndText.m_nLength)
				{
					m_CurrentCaretHot = m_WndText.m_nLength;
					m_bSelectFlag = 0;
					return;
				}
			} else
			{
				m_SelectBegin = m_CurrentCaretHot;
			}
			m_SelectEnd = m_WndText.m_nLength;
			m_bSelectFlag = 1;
			m_CurrentCaretHot = m_WndText.m_nLength;
		} else
		{
			m_CurrentCaretHot = m_WndText.m_nLength;
			m_bSelectFlag = 0;
		}
	}
	void CWispEdit::OnDeleteKey(WISP_MSG *pMsg)
	{
		if (m_bSelectFlag && m_SelectBegin != m_SelectEnd)
		{
			WISP_POINT CaretPos;
			GetCaretPos(&CaretPos);
			int Begin = MIN(m_SelectBegin, m_SelectEnd);
			int End = MAX(m_SelectBegin, m_SelectEnd);
			WISP_POINT Point;
			GetCurrentPosition(&Point, Begin);
			m_WndText.Delete(Begin, End - Begin);
			m_CurrentCaretHot = Begin;
			CaretPos.x = MAX(Point.x, 0);
			SetCaretPos(CaretPos.x, CaretPos.y);
			m_bSelectFlag = 0;
			m_SelectBegin = 0;
			m_SelectEnd = 0;
		} else
		if (m_CurrentCaretHot != m_WndText.m_nLength)
		{
			WISP_POINT CaretPos;
			GetCaretPos(&CaretPos);
			int TextExt = m_ClientDC.GetTextExtent(&m_WndText.operator const WCHAR*()[m_CurrentCaretHot+1], -1, 0);
			int w = m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width;
			if (TextExt <= w - CaretPos.x &&
				CaretPos.x < m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), m_CurrentCaretHot, 0))
			{
				int w = m_ClientDC.GetTextExtent(&m_WndText.operator const WCHAR*()[m_CurrentCaretHot+1], 1, 0);
				m_WndText.Delete(m_CurrentCaretHot, 1);
				CaretPos.x += w;
			} else
			{
				m_WndText.Delete(m_CurrentCaretHot, 1);
			}
			TextExt = m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), -1, 0);
			w = m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width;
			if (TextExt < w)
			{
				if (m_Style & 0x4000000)
					CaretPos.x = (w - TextExt)/2 + m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), m_CurrentCaretHot, 0);
				else
				if (m_Style & 0x10000)
					CaretPos.x = w;
			}
			SetCaretPos(CaretPos.x, CaretPos.y);
		}
	}
	//void OnPageUpKey(WISP_MSG *pMsg);
	//void OnPageDownKey(WISP_MSG *pMsg);
	//bool OnShiftKey(WISP_MSG *pMsg);
	//int GetMaxShowCharNum(WCHAR*, int, int*, char);
	bool CWispEdit::IsInsertChar(WCHAR wch)
	{
		int TextWidth = m_ClientDC.GetTextExtent(&wch, 1, 0);
		if (m_WndText.IsEmpty() || m_Style & 0x1100000)
			return true;

		int TextExt = 0;
		if (m_bSelectFlag && m_SelectBegin != m_SelectEnd)
		{
			int Begin = MIN(m_SelectBegin, m_SelectEnd);
			int End = MAX(m_SelectBegin, m_SelectEnd);
			TextExt = m_ClientDC.GetTextExtent(&m_WndText.operator const WCHAR*()[Begin], End - Begin, 0);
		}
		return TextWidth + m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), -1, 0) - TextExt <=
						m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width;
	}
	int CWispEdit::InsertChar(const WCHAR *pChar, int nPosition, int nCount)
	{
		int Len = 0;
		if (!SendCommand(0x80000700, 0, 0)) //WISP_ID_EDIT_CHANGING
			return Len;

		if (m_bSelectFlag && m_SelectBegin != m_SelectEnd)
		{
			WISP_POINT CaretPos;
			GetCaretPos(&CaretPos);

			int Begin = MIN(m_SelectBegin, m_SelectEnd);
			int End = MAX(m_SelectBegin, m_SelectEnd);

			WISP_POINT Point;
			GetCurrentPosition(&Point, Begin);
			m_WndText.Delete(Begin, End - Begin);
			m_CurrentCaretHot = Begin;
			CaretPos.x = Point.x;
			SetCaretPos(CaretPos.x, CaretPos.y);
			m_bSelectFlag = 0;
			m_SelectBegin = 0;
			m_SelectEnd = 0;
		}

		if (nPosition == -1)
			nPosition = m_CurrentCaretHot;

		if (nCount == 1)
			Len = m_WndText.Insert(nPosition, *pChar);
		else	m_WndText.Insert(nPosition, pChar);
		SendCommand(0x80000705, 0, 0); //WISP_ID_EDIT_CHANGED
		return Len;		
	}
	int CWispEdit::DeleteChar(int nPosition, int nCount, WISP_MSG *pMsg)
	{
		if (nPosition == -1)
			nPosition = m_CurrentCaretHot;
		if (!SendCommand(0x80000700, 0, 0)) //WISP_ID_EDIT_CHANGING
			return 0;
		//??? fix if (nPosition == m_WndText.m_nLength)
			--nPosition;
		OnLeftKey(pMsg);
		m_WndText.Delete(nPosition, nCount);
		SendCommand(0x80000705, 0, 0); //WISP_ID_EDIT_CHANGED
		return nCount;
	}
	int CWispEdit::GetCurrentLinePosition(int nIndex)
	{
		if ( (m_Style & 0x20000000) == 0)
			return m_CurrentCaretHot;

		WISP_POINT CaretPos;
		GetCaretPos(&CaretPos);

		int Len = 0;
		for (int Hot = m_CurrentCaretHot - 1; Hot >= 0; --Hot)
		{
			if (m_WndText.operator const WCHAR*()[Hot] == 10) break;
			if (m_WndText.operator const WCHAR*()[Hot] == 13) break;
			++Len;
		}

		if (m_Style & 0x1100000)
		{
			int Pos = Len % 20;
			if (!Pos && Len)
				Pos = 20;
			return Pos;
		}

		int Pos = 0;
		int i = 0;
		for (int n = 0; n < Len; ++n)
		{
			int TextExt = m_ClientDC.GetTextExtent(&m_WndText.operator const WCHAR*()[n + m_CurrentCaretHot - Len], 1, 0);
			if (TextExt + i >= m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width)
			{
				Pos = -1;
				i = TextExt;
			} else
			{
				i += TextExt;
			}
			++Pos;
		}
		return Pos;
	}
	//int GetPrevLineBeginPosition(int, int*);
	//int GetCaretLineCharWidth();
	int CWispEdit::GetLineWidth(const WCHAR *pSrc)
	{
		int Len = 0;
		while (pSrc[Len++]);
		return Len;
	}
	//int CalcLineNum(const WCHAR* pSrc, int nCount, int nPerLineMaxCharNum);
	//int CalcLineNumForWidth(const WCHAR* pSrc, int nCount, int nLineWidth);
	//int CaretLeftMove();
	//int CaretRightMove();
	void CWispEdit::GetCurrentPosition(WISP_POINT *pPoint, int Len)
	{
		pPoint->x = 0;
		pPoint->y = 0;
		if (Len <= m_WndText.m_nLength)
		{
			pPoint->x = m_CurrentShowRect.x + m_ClientDC.GetTextExtent(m_WndText.operator const WCHAR*(), Len,0);
			pPoint->y = m_CurrentShowRect.y;
		}
	}
	bool CWispEdit::SetWindowText(const WCHAR *pString)
	{
		if (!pString)
			return false;
		m_WndText = pString;
		int PosX = 0;
		int PosY = m_Style & 0x20000000 ? 0 : (m_ClientRect.cy - m_Caret.pDIB->m_PixelBuf.Height)/2;
		m_CurrentStringLen = TStrLen(pString);
		if (m_Style & 0x4000000)
		{
			m_CurrentLineCharIndex = m_CurrentStringLen/2;
			m_CurrentCaretHot = m_CurrentStringLen/2;
			PosX = m_ClientRect.cx - m_Caret.pDIB->m_PixelBuf.Width;
			if (m_CurrentStringLen % 2)
				PosX -= m_ClientDC.GetTextExtent(&pString[m_CurrentStringLen/2], 1, 0);
			PosX /= 2;
		} else
		{
			m_CurrentLineCharIndex = 0;
			m_CurrentCaretHot = 0;
		}
		SetCaretPos(PosX, PosY);
		Update(&m_ClientRect);
		return true;
	}

	int CWispEdit::GetCharIndexByPoint(WISP_POINT point, WISP_POINT& retpoint)
	{
		int PosX = m_CurrentShowRect.x;
		int Index = 0;
		int TextExt = 0;
		for (Index = 0; Index < m_WndText.m_nLength; ++Index)
		{
				TextExt = m_ClientDC.GetTextExtent(&m_WndText.operator const WCHAR*()[Index], 1, 0);
				PosX += TextExt;
				if (PosX > point.x) break;
		}
		if (PosX > point.x)
			PosX -= TextExt;
		retpoint.x = PosX;
		retpoint.y = point.y;
		return Index;		
	}

	int CWispEdit::Select(int Pos, int Len)
	{
		if (m_WndText.m_nLength < MAX(Pos, Len))
			return 0;

		m_SelectBegin = MIN(Pos, Len);
		m_SelectEnd = MAX(Pos, Len);
		m_CurrentCaretHot = Len;
		m_bSelectFlag = 1;
		Update((WISP_RECT*)0);
		return m_SelectEnd - m_SelectBegin + 1;
	}

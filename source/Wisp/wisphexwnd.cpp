
#include "wisphexwnd.hpp"
#include "wispbase.hpp"

	const WCHAR *CWispHexWnd::m_UnReadable = WSTR("????????????????");
	const WCHAR *CWispHexWnd::m_UnReadableASCII = WSTR("...................");
	const WCHAR *CWispHexWnd::m_Spaces = WSTR("                ");

	CWispHexWnd::CWispHexWnd()
	{
		m_ImageHighBase = -1;
		m_ImageSize = -1;
		m_nByte_Line = 16;
		m_ShowLength = 1;
		m_CtrlType = WISP_CTRL_HEX_WND;
		m_ImageBase = 0;
		m_AlignScroll = 4;
		m_bShowOffset = true;
		m_ShowOffset = 0;
		m_bFloatAddress = false;
		m_Offset2 = 0;
		m_Size2 = 0;
		m_Size1 = 0;
		m_Offset1 = 0;
		m_HistoryIter = m_History.Begin();
	}
	CWispHexWnd::~CWispHexWnd()
	{
		m_History.Clear();
	}

	WISP_MSG_MAP_BEGIN(CWispHexWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_MOUSE_DBLCLICK, OnMouseDblClick)
		WISP_MSG_MAP(WISP_WM_MOUSE_MOVE, OnMouseMove)
		WISP_MSG_MAP(WISP_WM_UPDATE_CLIENT, OnUpdateClient)
		WISP_MSG_MAP(WISP_WM_SIZE, OnSize)
		WISP_MSG_MAP(WISP_WM_RECALC_LAYOUT, OnRecalcLayout)
		WISP_MSG_MAP(WISP_WM_CHAR, OnChar)
		WISP_MSG_MAP(WISP_WM_SCROLL_EVENT, OnScrollEvent)
	WISP_MSG_MAP_END(CWispWnd)

	bool CWispHexWnd::OnChar(WISP_MSG *pMsg)
	{
		char Buffer[4];
		char NumBuffer[16+1];

		int nStartLine = GetStartLine();
		WCHAR nChar = pMsg->Char.Char;
		int HitPoint = CorrectHitPoint(&m_CaretPos);

		if (HitPoint == HEX_ASCII_INDEX)
		{
			ULONG_PTR Addr;
			if (CalcPointPos(&m_CaretPos, &Addr, 0) && m_CaretIndex > m_nByte_Line)
			{
				if (ReadMemory(Addr, Buffer, 1))
				{
					Buffer[0] = nChar;
					if (WriteMemory(Addr, Buffer, 1))
					{
						WISP_MSG Msg;
						Msg.KeyEvent.bKeyDown = true;
						Msg.KeyEvent.KeyType = VK_RIGHT;
						OnKeyEvent(&Msg);
					}
				}
			}
			return true;
		}

		if (IS_HEX_DIGIT(nChar))
		{
			if (HitPoint == HEX_POS_INDEX && m_CaretIndex < ADDR_SIZE)
			{
				ULONG_PTR HexAddr = m_CurAddr + m_nByte_Line * (m_CaretnLine - nStartLine);
				TSPrintf(NumBuffer, F0ADDR "X", HexAddr);
				NumBuffer[m_CaretIndex] = nChar;
				ULONG_PTR Addr;
				if (USHexStrToNum(NumBuffer, &Addr))
				{
					unsigned long CaretnLine = (Addr - m_ImageBase) / m_nByte_Line;
					if (CaretnLine >= m_CaretnLine)
					{
						unsigned long CaretnLineSize = CaretnLine - m_CaretnLine;
						if (CaretnLineSize <= m_ImageHighBase/m_nByte_Line - m_nLines_Page)
						{
							ViewAddressMsg(Addr + m_CurAddr - HexAddr, 0);
							m_CaretPos.x += m_CharWidth;
							SetCaretPos(m_CaretPos.x, m_CaretPos.y);
							Update((WISP_RECT*)0);
							return true;
						}
						if (CaretnLineSize >= m_ImageHighBase / m_nByte_Line)
							return true;
						m_CaretPos.x += m_CharWidth;
						m_CaretPos.y = m_LineHeight * ((Addr-m_CurAddr)/m_nByte_Line);
					} else
					{
						m_CaretPos.y = CaretnLine * m_LineHeight;
						ViewAddressMsg(m_ImageBase, 0);
						m_CaretPos.x += m_CharWidth;
					}
					SetCaretPos(m_CaretPos.x, m_CaretPos.y);
					Update((WISP_RECT*)0);
				}
				return true;
			} else
			if (HitPoint == HEX_HEX_INDEX && m_CaretIndex%(m_ShowLength*3) < m_ShowLength*2)
			{
				ULONG_PTR Addr;
				if (CalcPointPos(&m_CaretPos, &Addr, 0))
				{
					ULONG_PTR NewAddr = Addr + m_ShowLength - m_CaretIndex%(m_ShowLength*3)/2 - 1;
					if (ReadMemory(NewAddr, Buffer, 1))
					{
						unsigned char byte = nChar > 0x39 ? (nChar|0x20)-0x57 : nChar-0x30;
						Buffer[0] = m_CaretIndex%(m_ShowLength*3)%2 ? (byte|(Buffer[0] & 0xF0)) : (byte<<4)|(Buffer[0] & 0xF);
						if (WriteMemory(NewAddr, Buffer, 1))
						{
							WISP_MSG Msg;
							Msg.KeyEvent.bKeyDown = true;
							Msg.KeyEvent.KeyType = VK_RIGHT;
							OnKeyEvent(&Msg);
							OnDataUpdated();
						}
					}
				}
				return true;
			}
		}
		return true;
	}
	bool CWispHexWnd::OnCreate(WISP_MSG *pMsg)
	{
		m_ItemColor[HEX_START_INDEX] = m_pWispBase->m_pWispDrawObj->m_crSystem[0];
		m_ItemColor[HEX_POS_INDEX] = m_pWispBase->m_pWispDrawObj->m_crSystem[11];
		m_ItemColor[HEX_HEX_INDEX] = m_pWispBase->m_pWispDrawObj->m_crSystem[5];
		m_ItemColor[HEX_ASCII_INDEX] = m_pWispBase->m_pWispDrawObj->m_crSystem[104];
		m_BSelColor = m_pWispBase->m_pWispDrawObj->m_crSystem[105];
		m_TSelColor = m_pWispBase->m_pWispDrawObj->m_crSystem[1];
		m_BBPXColor = m_pWispBase->m_pWispDrawObj->m_crSystem[106];
		EnableScrollBar(0, 1, 1);
		EnableScrollBar(1, 1, 1);
		CreateCaret(WISP_DI_CARET);
		ShowCaret(true);
		SetCaretPos(0, 0);
		m_SelEnd = -1;
		m_SelStart = -1;
		m_bSelText = false;
		m_bSelect = false;
		m_CurAddr = 0;
		m_CharXPos = 0;
		SetBGColor(m_pWispBase->m_pWispDrawObj->m_crSystem[0]);
		m_CaretPos.y = 0;
		m_CaretPos.x = 0;
		m_AddressMode = 0;
		m_AddressModeOffs = 0;
		memset(m_Array2, 0, sizeof(m_Array2));
		m_Offset2 = 0;
		m_Size2 = 0;
		m_Size1 = 0;
 		
		return true;
	}

	WISP_CMD_MAP_BEGIN(CWispHexWnd)
		WISP_CMD_MAP(WISP_ID_VSPIN_SCROLL, OnCmdVSpinScroll)
	WISP_CMD_MAP_END

	bool CWispHexWnd::OnCmdVSpinScroll(WISP_MSG *pMsg)
	{
		if (m_Style & 0x10000 && pMsg->Command.CmdMsg == 0x80000002) //WISP_ID_KEYUP
			SetScrollBarCurPos(0, m_AlignScroll * m_nLines_Page, 0);
		return true;
	}
	bool CWispHexWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
	        if (!pMsg->KeyEvent.bKeyDown)
			return true;
		
	        int LineHeight = m_bShowOffset ? m_LineHeight : 0;

	        switch (pMsg->KeyEvent.KeyType)
	        {
                case VK_LBUTTON: {
			WISP_POINT Point = pMsg->MsgMouseWndPT;
                        Point.y -= m_ClientRect.y;
                        ShowCaret(true);
                        m_CurArea = CorrectHitPoint(&Point);
                        m_CaretPos.x = ROUNDPREV(Point.x, m_CharWidth);
                        m_CaretPos.y = ROUNDPREV(Point.y, m_LineHeight);
                        if (m_bSelect)
                        {
                                m_bSelect = false;
                                m_SelEnd = -1;
                                m_SelStart = -1;
                        }
                        SetCaretPos(m_CaretPos.x, m_CaretPos.y);
                        Update(&m_ClientRect);
                        } break;
                case VK_TAB: {
                        m_ShowLength *= 2;
		#ifdef _X64_
                        if (m_ShowLength >= 16)
		#else
                        if (m_ShowLength >= 8)
		#endif
                                m_ShowLength = 1;
                        Update(&m_ClientRect);
                        } break;
                case VK_PRIOR: {
                        m_CurAddr -= m_nLines_Page * m_nByte_Line;
                        MIN_LIMIT(m_CurAddr, m_ImageBase);
                        Update(&m_ClientRect);
                        } break;
                case VK_NEXT: {
                        m_CurAddr += m_nLines_Page * m_nByte_Line;
                        MAX_LIMIT(m_CurAddr, m_ImageHighBase - m_nLines_Page * m_nByte_Line);
                        ViewAddressMsg(m_CurAddr, 0);
                        Update(&m_ClientRect);
                        } break;
                case VK_END: {
                        m_CurAddr = m_ImageHighBase - m_nLines_Page * m_nByte_Line;
                        ViewAddressMsg(m_CurAddr, 0);
                        Update(&m_ClientRect);
                        } break;
                case VK_HOME: {
                        m_CurAddr = m_ImageBase;
                        ViewAddressMsg(m_CurAddr, 0);
                        Update(&m_ClientRect);
                        } break;
                case VK_LEFT: {
                        if (m_CaretPos.x >= m_CharWidth)
                        {
                                m_CaretPos.x -= m_CharWidth;
                                if (CorrectHitPoint(&m_CaretPos) == HEX_HEX_INDEX &&
					m_CaretIndex%(m_ShowLength*3) >= 2 * m_ShowLength && 
					m_CaretPos.x >= m_CharWidth)
				{
					m_CaretPos.x -= m_CharWidth * m_ShowLength;
					m_CaretIndex -= m_ShowLength;
                                }
                                SetCaretPos(m_CaretPos.x, m_CaretPos.y);
                        } else
			if (m_CaretPos.x == 0 && m_CharXPos > 0)
			{
				--m_CharXPos;
				CorrectHitPoint(&m_CaretPos);
			}
			Update(&m_ClientRect);
			} break;
                case VK_RIGHT: {
                        if (m_CaretPos.x < m_CharWidth * m_nChars_Line)
                        {
                                m_CaretPos.x += m_CharWidth;
                                if (CorrectHitPoint(&m_CaretPos) == HEX_HEX_INDEX &&
					m_CaretIndex%(m_ShowLength*3) >= 2 * m_ShowLength &&
					m_CaretPos.x < m_CharWidth * m_nChars_Line)
				{
					m_CaretPos.x += m_CharWidth * m_ShowLength;
					m_CaretIndex += m_ShowLength;
                                }
                                SetCaretPos(m_CaretPos.x, m_CaretPos.y);
                        } else
                        if (m_CaretPos.x == m_CharWidth * m_nChars_Line && m_CharXPos < m_CharMax)
                        {
				++m_CharXPos;
				CorrectHitPoint(&m_CaretPos);
                        }
			Update(&m_ClientRect);
			} break;
		case VK_UP: {
			if (m_CaretPos.y >= m_LineHeight + LineHeight)
			{
				m_CaretPos.y -= m_LineHeight;
				CorrectHitPoint(&m_CaretPos);
				SetCaretPos(m_CaretPos.x, m_CaretPos.y);
			} else
			if (m_CaretPos.y == LineHeight && m_CurAddr > m_ImageBase)
			{
				m_CurAddr -= m_nByte_Line;
				if (m_CurAddr < m_ImageBase)
				{
					m_CurAddr = m_ImageBase;
					ViewAddressMsg(m_CurAddr, 0);
				}
				CorrectHitPoint(&m_CaretPos);
			}
			Update(&m_ClientRect);
			} break;
		case VK_DOWN: {
			if (m_CaretPos.y < m_LineHeight * (m_nLines_Page - 1))
			{
				m_CaretPos.y += m_LineHeight;
				CorrectHitPoint(&m_CaretPos);
				SetCaretPos(m_CaretPos.x, m_CaretPos.y);
			} else
			if (m_CaretPos.y == m_LineHeight * (m_nLines_Page - 1) &&
				m_CurAddr < m_ImageHighBase - m_nByte_Line * m_nLines_Page)
			{
				m_CurAddr += m_nByte_Line;
				ViewAddressMsg(m_CurAddr, 0);
				CorrectHitPoint(&m_CaretPos);
			}
			Update(&m_ClientRect);
			} break;
		default:
			break;
		}
		return true;
	}
	bool CWispHexWnd::OnMouseMove(WISP_MSG *pMsg)
	{
		WISP_POINT MousePT = pMsg->MsgMouseWndPT;
		PointToRect(&MousePT, &m_ClientRect);
		if (pMsg->bMsgLBTDown)
		{
			ULONG_PTR Pos;
			if (m_bSelect)
			{
				if (CalcPointPos(&MousePT, &Pos, 0))
				{
					m_SelEnd = m_bSelText ? Pos : m_ShowLength + Pos - 1;
					Update((WISP_RECT*)0);
				}
			} else
			if (CalcPointPos(&MousePT, &Pos, 0))
			{
				ShowCaret(false);
				m_SelStart = Pos;
				m_bSelect = true;
				m_bSelText = m_CurArea == HEX_ASCII_INDEX;
				Update((WISP_RECT*)0);
			}
		}
		return true;
	}
	bool CWispHexWnd::OnMouseDblClick(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.KeyType == VK_LBUTTON)
		{
			if (m_bShowOffset && pMsg->m_LogicMousePosWnd.y < m_LineHeight)
			{
				m_ShowOffset = 1 - m_ShowOffset;
				Update((WISP_RECT*)0);
			} else
			{
				int Width = 2 * m_ShowLength;
			#ifdef _X64_
				if (Width >= 16)
			#else
				if (Width >= 8)
			#endif
					Width = 1;
				if (m_CaretIndex >= 0 && m_CaretIndex < ADDR_SIZE && m_CurArea == HEX_POS_INDEX)
					SetDataWidth(Width);
			}
		}
		return true;
	}
	bool CWispHexWnd::OnUpdateClient(WISP_MSG *pMsg)
	{
		WCHAR Buffer[0x200];

		CWispWnd::OnUpdateClient(pMsg);

		int nStartLine = GetStartLine();
		m_Size1 = 0;

		if (m_bShowOffset != 0)
		{
			unsigned int v6 = m_nByte_Line/m_ShowLength;

			m_ClientDC.m_TextStyle = 0;
			int XStart = m_ItemSpace[HEX_START_INDEX] + m_ItemSpace[HEX_POS_INDEX];
			int GroupLen = m_ShowLength*3;

			m_ClientDC.SetTextColor(m_ItemColor[HEX_POS_INDEX]);
			if (m_ShowOffset)
			{
				//TODO
				m_ClientDC.SetTextColor(m_ItemColor[HEX_POS_INDEX]);
			}
			unsigned int HexLen = 0;
			int HexVal = 1;
			while (v6-- > 0)
			{
				if (m_ShowOffset)
				{
					for (int n = 0,i = 0; n < m_ShowLength; ++n,i+=2)
					{
						//TSPrintf(&Buffer[i], WSTR("+%X"), (HexVal*m_ShowLength-n-1));
						TSPrintf(&Buffer[i], WSTR("%2X"), (HexVal*m_ShowLength-n-1));
						//break; //FIX
					}
				} else
				{
					for (int n = 0,i = 0; n < m_ShowLength; ++n,i+=2)
						TSPrintf(&Buffer[i], WSTR("%X "), (HexVal*m_ShowLength-n-1+m_CurAddr) % m_nByte_Line);
				}
                                m_ClientDC.DrawString(m_CharWidth * (XStart + HexLen - m_CharXPos),
					(nStartLine-1)*m_LineHeight, Buffer, 2 * m_ShowLength, 0, 0);

				HexVal++;
				HexLen += GroupLen;
			}
		}

		if (m_bFloatAddress)
		{
			ULONG_PTR Addr = GetFloatAddress(m_FloatAddressStr);
			if (Addr != m_CurAddr)
				ViewAddressMsg(Addr, 0);
		}

		memset(m_LenArray, 0, sizeof(m_LenArray));

		unsigned char DataBuffer[32];
		memset(DataBuffer, 0, sizeof(DataBuffer));

		WCHAR szSpace[32+1];

		for (int nLine = 0; nLine < m_nLines_Page; ++nLine)
		{
			int XStart = m_ItemSpace[HEX_START_INDEX];
			int YLine = (nStartLine + nLine) * m_LineHeight;

			ULONG_PTR Address = m_CurAddr + nLine * m_nByte_Line;
			if (m_AddressMode == 0)
			{
				TSPrintf(Buffer, WSTR("" F0ADDR "X:"), Address);
			} else
			{
				LONG_PTR AddressOffset = Address - m_AddressModeOffs;
				if (AddressOffset > 0)
					TSPrintf(Buffer, WSTR("$+%X"), AddressOffset);
				else
				if (AddressOffset < 0)
					TSPrintf(Buffer, WSTR("$-%X"), -AddressOffset);
				else
					TStrCpy(Buffer, "$ ==>");
			}

			int Len = TStrLen(Buffer);

			m_LenArray[nLine][HEX_POS_INDEX] = Len;

			m_ClientDC.m_TextStyle = 0;
			m_ClientDC.SetTextColor(m_ItemColor[HEX_POS_INDEX]);

			m_ClientDC.DrawString( (XStart - m_CharXPos) * m_CharWidth,
							YLine, Buffer, Len, 0, 0);

			XStart += m_ItemSpace[HEX_POS_INDEX];

			unsigned long AsciiLen = m_ImageBase + m_ImageSize - (nLine * m_nByte_Line) - m_CurAddr;
			MAX_LIMIT(AsciiLen, m_nByte_Line);

			TStrCpy(szSpace, CWispHexWnd::m_Spaces);

			MAX_LIMIT(m_ShowLength, 32); //???
			szSpace[m_ShowLength] = 0;

			int HexLen = PAGE_COUNT(AsciiLen, m_ShowLength);

			int GroupLen = m_ShowLength*3;

			m_LenArray[nLine][HEX_HEX_INDEX] = HexLen * GroupLen;

			int GroupX = 0;
			
			for (int HexIt = 0; HexIt < HexLen; ++HexIt)
			{
				bool bSame = false;
				if (!ReadMemory(Address + HexIt * m_ShowLength, DataBuffer, m_ShowLength))
				{
					TStrCpy(Buffer, CWispHexWnd::m_UnReadable);
				} else
				{
					THexBytesToStr(DataBuffer, Buffer, AsciiLen, m_ShowLength, szSpace);
					
					for (unsigned int i = 0; i < m_ShowLength; ++i)
						m_Array1[i+ m_Size1] = DataBuffer[i];

					m_Size1 += m_ShowLength;
					bSame = DataWidthCompare(Address + HexIt * m_ShowLength, DataBuffer);
				}
				unsigned int HexAttr = GetHexByteAttribute(m_CurAddr + nLine * m_nByte_Line + HexIt * m_ShowLength);
				if (HexAttr == HEX_BYTE_NORMAL)
				{
					m_ClientDC.m_TextStyle = 0;
					if (!bSame)
						m_ClientDC.SetTextColor(m_ItemColor[HEX_HEX_INDEX]);
					else	m_ClientDC.SetTextColor(0xFFFF00);
				} else
				if (HexAttr == HEX_BYTE_SEL)
				{
					m_ClientDC.m_TextStyle = 1;
					m_ClientDC.SetTextBKColor(m_BSelColor);
					if (!bSame)
						m_ClientDC.SetTextColor(m_TSelColor);
					else	m_ClientDC.SetTextColor(0xFFFF00);
				} else
				if (HexAttr == HEX_BYTE_BPX)
				{
					m_ClientDC.m_TextStyle = 1;
					m_ClientDC.SetTextBKColor(m_BBPXColor);
					m_ClientDC.SetTextColor(m_ItemColor[HEX_HEX_INDEX]);
				}
				m_ClientDC.DrawString(m_CharWidth * (XStart + GroupX - m_CharXPos),
						YLine, Buffer, m_ShowLength*2, 0, 0);
				GroupX += GroupLen;
			}

			XStart += m_ItemSpace[HEX_HEX_INDEX];

			m_LenArray[nLine][HEX_ASCII_INDEX] = m_nByte_Line;

			for (int AsciiIt = 0; AsciiIt < AsciiLen; ++AsciiIt)
			{
				if (!ReadMemory(Address + AsciiIt, DataBuffer, 1))
					TStrCpy(Buffer, CWispHexWnd::m_UnReadableASCII);
				else	THexBytesToPrint(DataBuffer, Buffer, 1);

				unsigned int HexAttr = GetHexByteAttribute(AsciiIt + m_CurAddr + nLine * m_nByte_Line);
				if (HexAttr == HEX_BYTE_NORMAL)
				{
					m_ClientDC.m_TextStyle = 0;
					m_ClientDC.SetTextBKColor(m_pWispBase->m_pWispDrawObj->m_crSystem[20]);
					m_ClientDC.SetTextColor(m_ItemColor[HEX_ASCII_INDEX]);
				} else
				if (HexAttr == HEX_BYTE_SEL)
				{
					m_ClientDC.m_TextStyle = 1;
					m_ClientDC.SetTextBKColor(m_BSelColor);
					m_ClientDC.SetTextColor(m_TSelColor);
				}
				m_ClientDC.DrawString(m_CharWidth * (XStart + AsciiIt - m_CharXPos),
							YLine, Buffer, 1, 0, 0);
			}
		}

		m_Offset1 = m_CurAddr;
		if (!m_Size2)
			DataWidthUpdate();
		return false;
	}
	bool CWispHexWnd::OnRecalcLayout(WISP_MSG *pMsg)
	{
		m_nChars_Line = m_ClientRect.cx / m_ClientDC.m_pFont->m_Width;
		m_nLines_Page = m_ClientRect.cy / m_ClientDC.m_pFont->m_Height;

		m_CharWidth = m_ClientDC.m_pFont->m_Width;
		m_LineHeight = m_ClientDC.m_pFont->m_Height;

		if (m_nLines_Page % m_LineHeight > m_LineHeight/2)
			++m_nLines_Page;
		if (m_ClientRect.cx < 0)
			m_nChars_Line = 0;

		MIN_LIMIT(m_nLines_Page, 0);
		
		MAX_LIMIT(m_CurAddr, m_ImageHighBase - m_nLines_Page*m_nByte_Line);
		MIN_LIMIT(m_CurAddr, m_ImageBase);

		m_ItemSpace[HEX_START_INDEX] = 1;
		m_ItemSpace[HEX_ASCII_INDEX] = m_nByte_Line + 1; //used below
		m_ItemSpace[HEX_POS_INDEX] = ADDR_SIZE+2;
		m_ItemSpace[HEX_HEX_INDEX] = m_nByte_Line*3;

		if (m_nChars_Line > (int)(m_nByte_Line*4+ADDR_SIZE+3))
		{
			m_CharXPos = 0;
			int Width = (m_nChars_Line - m_nByte_Line*3 - m_ItemSpace[HEX_ASCII_INDEX] - (ADDR_SIZE+3))/2;
			m_ItemSpace[HEX_POS_INDEX] += Width;
			m_ItemSpace[HEX_HEX_INDEX] += Width;
		}

		MAX_LIMIT(m_nLines_Page, m_ImageSize/m_nByte_Line);

		m_CharMax = m_ItemSpace[HEX_POS_INDEX] +
			    m_ItemSpace[HEX_HEX_INDEX] +
			    m_ItemSpace[HEX_ASCII_INDEX] + 1;

		MIN_LIMIT(m_CharMax, m_nChars_Line-1);
		return true;
	}
	bool CWispHexWnd::OnScrollEvent(WISP_MSG *pMsg)
	{
		int nLines_Page = m_nLines_Page;
		CWispWnd::OnScrollEvent(pMsg);
		if (m_bShowOffset)
			--nLines_Page;
		if (pMsg->ScrollEvent.Type == 1) //HORZ ???
		{
			m_CharXPos = m_pScrollBar[1]->CurPos;
			return false;
		}
		
		if (m_Style & 0x10000)
		{
			switch (pMsg->ScrollEvent.Event)
			{
			case WISP_SB_INC:
				ViewAddressMsg(m_CurAddr + m_nByte_Line, 0);
				SetScrollBarCurPos(0, m_nLines_Page * m_AlignScroll, 0);
				break;
			case WISP_SB_DEC:
				ViewAddressMsg(m_CurAddr - m_nByte_Line, 0);
				SetScrollBarCurPos(0, m_nLines_Page * m_AlignScroll, 0);
				break;
			case WISP_SB_PAGE_UP:
				ViewAddressMsg(m_CurAddr - nLines_Page*m_nByte_Line, 0);
				SetScrollBarCurPos(0, m_nLines_Page * m_AlignScroll, 0);
				break;
			case WISP_SB_PAGE_DOWN:
				ViewAddressMsg(m_CurAddr + nLines_Page*m_nByte_Line, 0);
				SetScrollBarCurPos(0, m_nLines_Page * m_AlignScroll, 0);
				break;
			case WISP_SB_POS_CHANGE:
				ViewAddressMsg(m_CurAddr + pMsg->ScrollEvent.Delta * m_nByte_Line, 0);
				break;
			default:
				SetScrollBarCurPos(0, m_nLines_Page * m_AlignScroll, 0);
			}
			
			return false;
		} else
		{
			ULONG_PTR CurAddr = m_nByte_Line * m_pScrollBar[0]->CurPos;
			if (CurAddr >= m_ImageBase && CurAddr < m_ImageHighBase)
			{
				m_CurAddr = CurAddr;
				DataWidthUpdate();	
				OnChangeView();
			}
		}
		return true;
	}
	bool CWispHexWnd::OnSize(WISP_MSG *pMsg)
	{
		if (m_Style & 0x10000)
		{
			int nLines_Page = m_nLines_Page;
			if (m_bShowOffset)
				--nLines_Page;

			SetScrollBarInfo(0, m_nLines_Page * m_AlignScroll, m_nLines_Page * (2 * m_AlignScroll + 1), nLines_Page, 0, 0);
		} else
		{
			SetScrollBarRange(0, m_ImageHighBase / m_nByte_Line, m_ImageBase / m_nByte_Line);
		}
		SetScrollBarInfo(1, m_CharXPos, m_CharMax, m_nChars_Line, 0, 1);
		return true;
	}

	unsigned long CWispHexWnd::WriteMemory(ULONG_PTR Address, void *Buffer, unsigned long Size)
	{
		return 0;
	}
	unsigned long CWispHexWnd::ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size)
	{
		return 0;
	}

	unsigned int CWispHexWnd::GetHexByteAttribute(ULONG_PTR Address)
	{
		unsigned int HexByte = HEX_BYTE_NORMAL;
		if (m_bSelect)
		{
			ULONG_PTR Start = MIN(m_SelStart, m_SelEnd);
			ULONG_PTR End = MAX(m_SelStart, m_SelEnd);

			if (Start >= m_ImageBase && End < m_ImageBase + m_ImageSize &&
				Address >= Start && Address <= End)
				HexByte = HEX_BYTE_SEL;
		}
		return HexByte;
	}

	unsigned int CWispHexWnd::CorrectHitPoint(WISP_POINT *pPoint)
	{
		int x = pPoint->x + m_CharXPos * m_CharWidth;
		int nLine = pPoint->y/m_LineHeight;
		if (nLine >= m_nLines_Page)
		{
			nLine = m_nLines_Page - 1;
			pPoint->y = nLine * m_LineHeight;
		}
		int MaxX = 0;
		m_CaretnLine = pPoint->y/m_LineHeight;
		for (int SpaceIndex = 0; SpaceIndex < 4; ++SpaceIndex)
		{
			int MinX = MaxX;
			MaxX += m_CharWidth * m_ItemSpace[SpaceIndex];	
			if (x >= MinX && x < MaxX)
			{
				int WW = MinX + m_CharWidth * m_LenArray[nLine][SpaceIndex];
				if (x <= WW)
				{
					//WW = pPoint->x;
					m_CaretIndex = (pPoint->x - MinX) / m_CharWidth;
					return SpaceIndex;
				} else
				if (x - WW >= MaxX - x)
				{
					pPoint->x = MaxX - m_CharWidth * m_CharXPos;
					m_CaretIndex = 0;
					return SpaceIndex+1;
				} else
				{
					pPoint->x = WW - m_CharWidth * m_CharXPos;
					m_CaretIndex = (WW - MinX) / m_CharWidth;
					return SpaceIndex;
				}
				//return SpaceIndex;
			}
		}
		
		pPoint->x = MaxX;
		m_CaretIndex = m_ItemSpace[3]; //HEX_ASCII_INDEX
		return 3;
	}
	bool CWispHexWnd::CalcPointPos(WISP_POINT *pPoint, ULONG_PTR *pPos, unsigned long *pLength)
	{
		int Offset = GetStartLine();
		int x = pPoint->x + m_CharWidth * m_CharXPos;
		int MinX = m_CharWidth * (m_ItemSpace[HEX_START_INDEX] + m_ItemSpace[HEX_POS_INDEX]);
		int nLine = pPoint->y / m_LineHeight;
		if (nLine < Offset)
			return false;

		nLine -= Offset;
		if (x >= MinX && x < MinX + m_CharWidth*(m_nByte_Line*3 - m_ShowLength))
		{
			m_CurArea = HEX_HEX_INDEX;
			*pPos = m_CurAddr + (nLine*m_nByte_Line) + (((x - MinX)/m_CharWidth)/(m_ShowLength*3))*m_ShowLength;
			if (pLength)
				*pLength = (x - MinX)/m_CharWidth % (m_ShowLength*3);
			return true;
		}
		MinX += m_CharWidth * m_ItemSpace[HEX_HEX_INDEX];
		if (x >= MinX && x < MinX + m_CharWidth * m_nByte_Line)
		{
			m_CurArea = HEX_ASCII_INDEX;
			*pPos = m_CurAddr + (nLine*m_nByte_Line) + ((x-MinX)/m_CharWidth);
			return true;
		}
		return false;
	}
	bool CWispHexWnd::ViewAddress(ULONG_PTR Address, bool bHistory)
	{
		ULONG_PTR PrevAddr = m_CurAddr;
		if (Address < m_ImageBase || Address >= m_ImageHighBase)
				return false;
		if (m_Style & 0x10000)
			m_CurAddr = Address;
		else
		if (m_nLines_Page > 0)
			SetScrollBarCurPos(0, Address/m_nByte_Line, 0);
		if (bHistory && m_CurAddr)
			AddPos(m_CurAddr, PrevAddr);
		OnChangeView();
		DataWidthUpdate();
		Update((WISP_RECT*)0);
		return true;
			
	}
	bool CWispHexWnd::ViewAddressMsg(ULONG_PTR Address, bool bUpdate) //private
	{
		if (Address < m_ImageBase || Address >= m_ImageHighBase)
				return false;
		m_CurAddr = Address;
		DataWidthUpdate();
		OnChangeView();
		return true;
	}

	int CWispHexWnd::GetDataWidth()
	{
		return m_ShowLength;
	}

	bool CWispHexWnd::SetDataWidth(int width)
	{
		if (16 % width) return false;
		m_ShowLength = width;
		Update((WISP_RECT*)0);
		return true;
	}

	void CWispHexWnd::SetFloatAddress(const WCHAR *StringAddr)
	{
		m_FloatAddressStr = StringAddr;
	}

	ULONG_PTR CWispHexWnd::GetFloatAddress(const TWideString & FixedAddress)
	{
		return m_CurAddr;
	}

	void CWispHexWnd::OnChangeView()
	{
		//???
	}

	void CWispHexWnd::OnDataUpdated()
	{
		//???
	}

	int CWispHexWnd::GetStartLine()
	{
		int nStartLine = 0;
		if (m_bShowOffset)
			nStartLine += 1;
		return nStartLine;
	}

	bool CWispHexWnd::DataWidthCompare(ULONG_PTR Address, unsigned char *Buffer)
	{
		if (Address >= m_Offset2 && Address < m_Offset2+m_Size2)
		{
			unsigned long Start = Address - m_Offset2;
			for (int n = 0; n < m_ShowLength; ++n)
			{
				if (Buffer[n] != m_Array2[Start+n])
					return true;
			}
		}
		return false;
	}
	void CWispHexWnd::DataWidthUpdate()
	{
		if (m_Size1)
		{
			for (int n = 0; n < m_Size2; ++n)
			{
				if (m_Offset2+n >= m_Offset1 && m_Offset2+n < m_Offset1+m_Size1)
					m_Array1[m_Offset2+n - m_Offset1] = m_Array2[n];
			}
			memcpy(m_Array2, m_Array1, m_Size1);
			m_Size2 = m_Size1;
			m_Offset2 = m_Offset1;
		}
	}

	int CWispHexWnd::GetAddressMode()
	{
		return m_AddressMode;
	}

	void CWispHexWnd::SetAddressMode(int Mode, ULONG_PTR Addr)
	{
		if (Mode <= 0 || Mode >= 2) Mode = 0;
		m_AddressMode = Mode;
		m_AddressModeOffs = Addr;
		OnUpdateClient(0);
	}

	bool CWispHexWnd::GetFloatAddressMode()
	{
		return m_bFloatAddress;
	}

	bool CWispHexWnd::ToggleFloatAddressMode()
	{
		m_bFloatAddress = !m_bFloatAddress;
		Update((WISP_RECT*)0);
		return m_bFloatAddress==0;
	}

	void CWispHexWnd::ToggleHexOffset()
	{
		m_bShowOffset = !m_bShowOffset;
		Update((WISP_RECT*)0);
		//???
	}

	void CWispHexWnd::AddPos(ULONG_PTR Addr, ULONG_PTR PrevAddr)
	{
		if (m_History.Size()>100)
		{
			if (m_HistoryIter == m_History.Begin())
				m_HistoryIter++;
			m_History.Remove(m_History.Begin());
		}

		if (m_History.Size()==0 && Addr != PrevAddr)
			m_HistoryIter = m_History.Append(&Addr);

		if (Addr != *(m_History.End()-1)) //???
			m_HistoryIter = m_History.Append(&Addr);
	}

	void CWispHexWnd::ResetPos()
	{
	        m_History.Clear();
	        m_HistoryIter = m_History.Append(&m_CurAddr);
	}
	void CWispHexWnd::SavePos()
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
	}

	void CWispHexWnd::PrevPos() //WISP_MSG*
	{
		if (m_HistoryIter == m_History.End() || *m_HistoryIter == m_CurAddr)
		{
			if (m_HistoryIter != m_History.Begin())
			{
				m_HistoryIter--;
				ViewAddress(*m_HistoryIter, 0);
			}
		} else
		{
			ViewAddress(*m_HistoryIter, 0);
		}
	}

	void CWispHexWnd::NextPos() //WISP_MSG*
	{
		if (m_HistoryIter == m_History.End() || *m_HistoryIter == m_CurAddr)
		{
			if (m_HistoryIter != m_History.End()-1)
			{
				m_HistoryIter++;
				ViewAddress(*m_HistoryIter, 0);
			}
		} else
		{
			ViewAddress(*m_HistoryIter, 0);
		}
	}


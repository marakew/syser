
#include "../Code/txtfile.hpp"
#include "wispconsolewnd.hpp"
#include "wispbase.hpp"

	CWispTerminalWnd::CWispTerminalWnd()
	{
		m_CtrlType = WISP_CTRL_TERMINAL_WND;
		m_pConsoleWnd = 0;
		m_Prefix = WSTR(">");
		m_MaxChars = m_Prefix.m_nLength;
		m_OutPutStr.Preallocate(2561, 0);
		m_pDelegate = 0;
		m_uFlags = 0;
	}
	CWispTerminalWnd::~CWispTerminalWnd()
	{
	}

	void CWispTerminalWnd::SetHistoryLines(int nLine)
	{
		if (nLine > 0 && nLine < 10000)
			m_LineList.Resize(nLine);
	}
	void CWispTerminalWnd::ClearScreen()
	{
		m_LineList.Clear();
		m_ScrPos.x = 0;
		m_ScrPos.y = 0;
		m_CaretPos.x = m_Prefix.m_nLength;
		m_CaretPos.y = 0;
		m_MaxChars = m_Prefix.m_nLength + 1;
		SetMaxLines(m_LineList.Count + 1);
		m_InputStr = m_Prefix;
		RecalcLayout();
		Update((WISP_RECT*)0);
	}
	
	void CWispTerminalWnd::OutPut(WCHAR ch)
	{
		if (ch)
		{
			m_OutPutStr += ch;
			if (m_OutPutStr.m_nLength < WISP_TERMINAL_LINE_SIZE) return;
		} else
		if (m_OutPutStr.m_nLength == 0)
		{
			if (m_OutPutStr.m_nLength < WISP_TERMINAL_LINE_SIZE) return;
		}
		OutPut(m_OutPutStr.operator const WCHAR*());
		m_OutPutStr.SetAt(0, 0);
	}
	void CWispTerminalWnd::OutPut(const WCHAR *szLine)
	{
		if (m_CtrlType == WISP_CTRL_CONSOLE_WND && static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd)
		{
			static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd->OutPut(szLine);
			return;
		}

		TColorString *pLine;
		int TotalLength;
		if (m_bNewLine)
		{
			TotalLength = WISP_TERMINAL_LINE_SIZE;
			pLine = m_LineList.Append(0);
			pLine->Empty();
			SetMaxLines(m_LineList.Count + 1);
			m_bNewLine = 0;
		} else
		{
			TotalLength = WISP_TERMINAL_LINE_SIZE - m_LineList[m_LineList.Count-1].m_nLength;
			pLine = &m_LineList[m_LineList.Count-1];
		}

		unsigned long szLineBuffer[WISP_TERMINAL_LINE_SIZE+1];
		const WCHAR *pNextLine;
		const WCHAR *pThisLine = szLine;
		while (pThisLine && pThisLine[0])
		{
			if (TotalLength==0)
			{
				TotalLength = WISP_TERMINAL_LINE_SIZE;
				pLine = m_LineList.Append(0);
				pLine->Empty();
				SetMaxLines(m_LineList.Count + 1);
			}

			WCHAR *pStr = TStrChr(pThisLine, 10);
			if (pStr)
			{
				int Length = pStr - pThisLine;
				MAX_LIMIT(Length, TotalLength);

				pNextLine = pStr + 1;
				TotalLength = 0;
				ColorStrNCpy(szLineBuffer, pThisLine, Length, m_TextColor, (char)0);
				if (!pNextLine[0])
				{
					m_bNewLine = 1;
					if (m_pDelegate)
					{
						if (m_uFlags & 1)
							szLineBuffer[Length++] = 13;
						szLineBuffer[Length++] = 10;
						szLineBuffer[Length] = 0;
					}
				}
				if (m_pDelegate)
					m_pDelegate->Write(szLineBuffer, 4 * Length);
				(*pLine) += szLineBuffer; //ADD
				MIN_LIMIT(m_MaxChars, pLine->m_nLength);
			} else
			{
				int Length = TStrLen(pThisLine);
				MAX_LIMIT(Length, TotalLength);

				pNextLine = &pThisLine[Length];
				ColorStrNCpy(szLineBuffer, pThisLine, Length, m_TextColor, (char)0);
				*pLine += szLineBuffer; //ADD
				if (m_pDelegate)
					m_pDelegate->Write(szLineBuffer, 4 * Length);
				MIN_LIMIT(m_MaxChars, pLine->m_nLength);
				TotalLength = WISP_TERMINAL_LINE_SIZE - pLine->m_nLength;
			}
			pThisLine = pNextLine;
		}

		m_CaretPos.y = m_LineList.Count;
		if (m_nLinesPerPage > 0)
			EnsureLineVisible();
		RecalcLayout();
	}

	void CWispTerminalWnd::OutPut(const unsigned long *szLine)
	{
		if (m_CtrlType == WISP_CTRL_CONSOLE_WND && static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd)
		{
			static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd->OutPut(szLine);
			return;
		}

		TColorString *pLine;
		int TotalLength;
		if (m_bNewLine)
		{
			TotalLength = WISP_TERMINAL_LINE_SIZE;
			pLine = m_LineList.Append(0);
			pLine->Empty();
			SetMaxLines(m_LineList.Count + 1);
			m_bNewLine = 0;
		} else
		{
			TotalLength = WISP_TERMINAL_LINE_SIZE - m_LineList[m_LineList.Count-1].m_nLength;
			pLine = &m_LineList[m_LineList.Count-1];
		}

		unsigned long szLineBuffer[WISP_TERMINAL_LINE_SIZE+1];
		const unsigned long *pNextLine;
		const unsigned long *pThisLine = szLine;
		while (pThisLine && pThisLine[0])
		{
			if (TotalLength==0)
			{
				TotalLength = WISP_TERMINAL_LINE_SIZE;
				pLine = m_LineList.Append(0);
				pLine->Empty();
				SetMaxLines(m_LineList.Count + 1);
			}

			const unsigned long *pStr = TStrChr(pThisLine, 10);
			if (pStr)
			{
				int Length = pStr - pThisLine;
				MAX_LIMIT(Length, TotalLength);

				pNextLine = pStr + 1;
				TotalLength = 0;
				TStrCpyLimit(szLineBuffer, pThisLine, Length);
				if (!pNextLine[0])
				{
					m_bNewLine = 1;
					if (m_pDelegate)
					{
						if (m_uFlags & 1)
							szLineBuffer[Length++] = 13;
						szLineBuffer[Length++] = 10;
						szLineBuffer[Length] = 0;
					}
				}
				if (m_pDelegate)
					m_pDelegate->Write(szLineBuffer, 4 * Length);
				(*pLine) += szLineBuffer; //ADD
				MIN_LIMIT(m_MaxChars, pLine->m_nLength);
			} else
			{
				int Length = TStrLen(pThisLine);
				MAX_LIMIT(Length, TotalLength);

				pNextLine = &pThisLine[Length];
				TStrCpyLimit(szLineBuffer, pThisLine, Length);
				*pLine += szLineBuffer; //ADD
				if (m_pDelegate)
					m_pDelegate->Write(szLineBuffer, 4 * Length);
				MIN_LIMIT(m_MaxChars, pLine->m_nLength);
				TotalLength = WISP_TERMINAL_LINE_SIZE - pLine->m_nLength;				
			}
			pThisLine = pNextLine;
		}
	}

	int CWispTerminalWnd::VPrintf(const WCHAR *format, va_list ParamList)
	{
		//EmitNumber
		//EmitString
		//Done

		const WCHAR *str;
		bool bIsShort;
		static const WCHAR *strSub = WSTR("-");//[2];
		static const WCHAR *strSpace = WSTR(" ");//[2];
		static const WCHAR *strZero = WSTR("0");//[2];
		int Precision;
		bool bLeftJustify;
		bool bIsLong;
		bool bMore;
		bool bPlusSign;
		WCHAR * savestr;
		WCHAR strbuf[36]; //??? increase
		bool bZeroPrefix;
		va_list Next;
		static const WCHAR *str0X = WSTR("0X");//[3];
		unsigned long val;
		static const WCHAR *strPlus = WSTR("+");//[2];
		int Flags;
		WCHAR cPad;
		static const WCHAR *strNULL = WSTR("");//[1];
		int len;
		__int64 val64;
		bool bBlankSign;
		bool bIsLonglong;
		int Length;
		int Width;
		int nPad;
		static const WCHAR *strError = WSTR("<Error>");//[8];
		int nLeadingZeros;
		const WCHAR * sPrefix;
		static const WCHAR *str0x = WSTR("0x"); //[3];

		Next = ParamList;

		SaveFontColor();

		Length = 0;
		while (*format)
		{
			if (*format != '%')
			{
				OutPut(*format);
				++Length;
				++format;
				continue;
			}

			Width = 0;
			Precision = -1;
			bLeftJustify = false;
			bPlusSign = false;
			bBlankSign = false;
			bZeroPrefix = false;
			bIsLonglong = false;
			Flags = 0;
			cPad = 0x20;
			sPrefix = strNULL;

			format++;

			bool bMore = true;
			while (bMore)
			{
				switch (*format)
				{
				case ' ': /*Width = 0;*/ bBlankSign = true; format++; Flags |= SPACE; break;
				case '#': bZeroPrefix = true; format++; Flags |= HEX_PREP; break;
				case '+': bPlusSign = true; format++; Flags |= PLUS; break;
				case '-': /*Width = 0;*/ bLeftJustify = true; format++; Flags |= LEFT; break;
				case '0': cPad = '0'; format++; Flags |= ZEROPAD; break;
				default: bMore = false; break;
				}
			}

			if (*format == '*')
			{
				Width = va_arg(Next, int);
				format++;
				if (Width < 0)
				{
					Width = 0;
					bLeftJustify = true;
					Flags |= LEFT;
				}
			} else
			if (IS_DIGIT(*format))
			{
				while (IS_DIGIT(*format))
					Width = 10 * Width + *format++ - '0';
			}

			//switch TODO
			if (*format == '.')
			{
				format++;
				Precision = 0;
				if (*format == '*')
				{
					Precision = va_arg(Next, int);
					format++;
				} else
				{
					while (IS_DIGIT(*format))
						Precision = 10 * Precision + *format++ - '0';
				}
				if (Precision < 0) Precision = 0;
			}

			if (format[0] == 'I')
			{
				if (format[1] == '6' && format[2] == '4')
				{
					format += 3;
					bIsLonglong = true;
					
				}
			} else
			if (*format == 'h') ++format;
			else
			if (*format == 'l') ++format;

			switch (*format++)
			{
			case '%':
				strbuf[0] = '%';
				str = strbuf;
				len = 1;
				goto EmitString;
		#ifdef HAS_FLOAT
			case 'E':
			case 'G':
			case 'e':
			case 'f':
			case 'g': {
				double valDouble = va_arg(Next, double);

//unsigned long NaN[2] = {0xffffffff, 0x7fffffff}
//A quiet NaN is represented by any bit pattern between X'7FF80000 00000000' and X'7FFFFFFF FFFFFFFF' 
//or between X'FFF80000 00000000' and X'FFFFFFFF FFFFFFFF'.
//https://stackoverflow.com/questions/2249110/how-do-i-make-a-portable-isnan-isinf-function/2249173

				if (_fpclass(valDouble) == _FPCLASS_QNAN || _fpclass(valDouble) == _FPCLASS_SNAN)
				{
					TStrCpy(strbuf, ("Nan"));
					str = strbuf;
					len = TStrLen(strbuf);
					goto EmitString;
				} else
				if (_fpclass(valDouble) == _FPCLASS_NINF)
				{
					TStrCpy(strbuf, ("-Inf"));
					str = strbuf;
					len = TStrLen(strbuf);
					goto EmitString;
				} else
				if (_fpclass(valDouble) == _FPCLASS_PINF)
				{
					TStrCpy(strbuf, ("+Inf"));
					str = strbuf;
					len = TStrLen(strbuf);
					goto EmitString;
				}
				//_FPCLASS_NN
				//_FPCLASS_ND
				//_FPCLASS_NZ
				//_FPCLASS_PZ
				//_FPCLASS_PD
				//_FPCLASS_PN
				else {

				WCHAR *fstr = flt(strbuf, valDouble, Width, Precision, *format, Flags | SIGN);
				*fstr = (WCHAR)0;
				str = strbuf;
				len = TStrLen(strbuf);
				goto EmitNumber; }}
		#endif
			case 'X':
				if (bZeroPrefix) sPrefix = str0X;
				if (bIsLonglong)
				{
					val64 = va_arg(Next, __int64);
					NumToStr(val64, strbuf, 16);
					if (!val64) sPrefix = strNULL;
				} else
				{
					val = va_arg(Next, unsigned long);
					NumToStr(val, strbuf, 16);
					if (!val) sPrefix = strNULL;
				}
				TStrUpr(strbuf);
				goto EmitNumber;
			case 'c':
				strbuf[0] = va_arg(Next, int); //WCHAR
				str = strbuf;
				len = 1;
				goto EmitString;
			case 'd':
			case 'i':
				Flags |= SIGN;
				if (bIsLonglong)
				{
					long long val64 = va_arg(Next, long long);
					NumToStr(val64, strbuf, 10);
				} else
				{
					long val = va_arg(Next, long);
					NumToStr(val, strbuf, 10);
				}
				if (strbuf[0] == '-') sPrefix = strSub;
				else
				if (bPlusSign) sPrefix = strPlus;
				else
				if (bBlankSign) sPrefix = strSpace;
				goto EmitNumber;
			case 'n':
				goto EmitNumber;
			case 'o':
				if (bIsLonglong)
				{
					//goto EmitLongLongNumber;
					unsigned long long val64 = va_arg(Next, unsigned long long);
					NumToStr(val64, strbuf, 10); //??? 8
				} else
				{
					if (bZeroPrefix) sPrefix = strZero;
					unsigned long val = va_arg(Next, unsigned long);
					NumToStr(val, strbuf, 8);
					if (!val) sPrefix = strNULL;
				}
				goto EmitNumber;
			case 'p':
			case 'x':
				if (bZeroPrefix) sPrefix = str0x;
				if (bIsLonglong)
				{
					unsigned long long val64 = va_arg(Next, unsigned long long);
					NumToStr(val64, strbuf, 16);
					if (!val64) sPrefix = strNULL;
				} else
				{
					unsigned long val = va_arg(Next, unsigned long);
					NumToStr(val, strbuf, 16);
					if (!val) sPrefix = strNULL;
				}
				TStrLwr(strbuf);
				goto EmitNumber;
			case 's':
				str = va_arg(Next, WCHAR*);
				len = TStrLen(str);
				if (Precision != -1 && Precision < len)
					len = Precision;
				goto EmitString;
			case 'u':
				if (bIsLonglong)
				{
//EmitLongLongNumber:
					unsigned long long val64 = va_arg(Next, unsigned long long);
					NumToStr(val64, strbuf, 10);
				} else
				{	
					unsigned long val = va_arg(Next, unsigned long);
					NumToStr(val, strbuf, 10);		
				}
EmitNumber:
				if (Precision == -1) Precision = 1;
				savestr = strbuf;
				if (strbuf[0] == '-')
					savestr = &strbuf[1];
				len = TStrLen(savestr);
				nLeadingZeros = Precision - len;
				if (nLeadingZeros < 0)
					nLeadingZeros = 0;
				nPad = Width - (TStrLen(sPrefix) + nLeadingZeros + len);
				if (nPad < 0) nPad = 0;
				if (cPad == '0')
				{
					while (*sPrefix)
					{
						OutPut((WCHAR)*sPrefix++);
						++Length;
					}
				}
				if (nPad && !bLeftJustify)
				{
					while (nPad--)
					{
						OutPut((WCHAR)cPad);
						++Length;
					}
					nPad = 0;
				}
				while (*sPrefix)
				{
					OutPut((WCHAR)*sPrefix++);
					++Length;
				}
				while (nLeadingZeros--)
				{
					OutPut((WCHAR)0x30);
					++Length;
				}
				while (len--)
				{
					OutPut(*savestr++);
					++Length;
				}
				if (nPad)
				{
					while (nPad--)
					{
						OutPut((WCHAR)0x20);
						++Length;
					}
				}
				break;
			case '<':{
				WCHAR *pStr = TStrChr(format, '>');
				if (pStr)
				{
					int Length = pStr-format;
					if (Length == 1 || Length == 2)
					{
						WCHAR ColorNum[4];
						unsigned char ColorIndex = 0;
						TStrCpyLimit(ColorNum, format, Length+1);
						format += Length+1;
						if (USHexStrToNum(ColorNum, &ColorIndex))
						{
							OutPut((WCHAR)0);
							SetFontColor(ColorIndex);
							strbuf[0] = 0;
							Precision = 0;
							goto EmitNumber;
						}
					}
				}}
			default:
				str = strError;
				len = TStrLen(str);
			EmitString:
				if (Width <= len)
					nPad = 0;
				else	nPad = Width - len; //abs()
				if (nPad && !bLeftJustify)
				{
					while (nPad--)
					{
						OutPut((WCHAR)cPad);
						++Length;
					}
				} 
				while (len--)
				{
					OutPut(*str++);
					++Length;
				}
				if (nPad && bLeftJustify)
				{
					while (nPad--)
					{
						OutPut((WCHAR)0x20);
						++Length;
					}
				}
				break;
			}
		}

		OutPut((WCHAR)0);
		RestoreFontColor();
		return Length;
	}

	void CWispTerminalWnd::Printf(const WCHAR *szLine, ...)
	{
		va_list va;
		va_start(va, szLine);
		int Len = VPrintf(szLine, va);
		va_end(va);

		CWispTerminalWnd *pTermWnd = this;
		if (m_CtrlType == WISP_CTRL_CONSOLE_WND && static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd)
			pTermWnd = static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd;

		pTermWnd->m_CaretPos.y = pTermWnd->m_LineList.Count;
		if (pTermWnd->m_nLinesPerPage > 0)
			pTermWnd->EnsureLineVisible();
		m_bSelect = false; //???
		pTermWnd->UpdateCaretState();
		pTermWnd->RecalcLayout();
		pTermWnd->Update((WISP_RECT*)0);
	}
	void CWispTerminalWnd::AttachConsole(CWispConsoleWnd *pConsoleWnd)
	{
		m_pConsoleWnd = pConsoleWnd;
	}
	void CWispTerminalWnd::SetFontColor(unsigned char ColorIndex)
	{
		if (m_CtrlType == WISP_CTRL_CONSOLE_WND && static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd)
			static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd->m_TextColor = ColorIndex;
		else	m_TextColor = ColorIndex;
	}
	void CWispTerminalWnd::SaveFontColor()
	{
		if (m_CtrlType == WISP_CTRL_CONSOLE_WND && static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd)
			static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd->m_OldTextColor = static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd->m_TextColor;
		else	m_OldTextColor = m_TextColor;
	}
	void CWispTerminalWnd::RestoreFontColor()
	{
		if (m_CtrlType == WISP_CTRL_CONSOLE_WND && static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd)
			static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd->m_TextColor = static_cast<CWispConsoleWnd*>(this)->m_pTerminalWnd->m_OldTextColor;
		else	m_TextColor = m_OldTextColor;
	}
	void CWispTerminalWnd::EnsureLineVisible()
	{
		m_ScrPos.y = m_LineList.Count - m_nLinesPerPage + 1;
		MIN_LIMIT(m_ScrPos.y, 0);
		SetScrollBarCurPos(0, m_ScrPos.y, 1);
	}
	void CWispTerminalWnd::EnsureInputVisible()
	{
		EnsureLineVisible();
		UpdateCaretState();
	}
	void CWispTerminalWnd::EnsureInputVisible2()
	{
		MAX_LIMIT(m_ScrPos.x, m_CaretPos.x);
		MIN_LIMIT(m_ScrPos.x, m_CaretPos.x - m_nCharsPerLine + 1); //???
		MIN_LIMIT(m_CaretPos.x, 0);

		UpdateCaretState();
		Update((WISP_RECT*)0);
	
	}

	WISP_MSG_MAP_BEGIN(CWispTerminalWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP(WISP_WM_CHAR, OnChar)
		WISP_MSG_MAP_ON_EVENT
		WISP_MSG_MAP_ON_COMMAND
	WISP_MSG_MAP_END(CWispColorStrWnd)

	bool CWispTerminalWnd::OnChar(WISP_MSG *pMsg)
	{
		if (m_CaretPos.x < WISP_TERMINAL_LINE_SIZE-1 ||
			pMsg->Char.Char == 8 ||
			pMsg->Char.Char == 10)
		{
			if (m_LineList.Count > m_nLinesPerPage &&
			    m_ScrPos.y < m_LineList.Count - m_nLinesPerPage + 1)
				EnsureInputVisible();

			if (pMsg->Char.Char == 8) //BACKSPACE
			{
				if (m_bSelect &&
					(m_pSelStartPos->y == m_LineList.Count) &&
					(m_pSelStartPos->x >= m_Prefix.m_nLength) &&
					(m_pSelEndPos->x > m_pSelStartPos->x) )
				{
					m_InputStr.Delete(m_pSelStartPos->x, m_pSelEndPos->x - m_pSelStartPos->x);
					m_bSelect = false;
					m_CaretPos.x = m_pSelStartPos->x;
					RecalcLayout();
					Update(&m_ClientRect);
				} else
				{
					if (m_CaretPos.x > m_Prefix.m_nLength)
					{
						m_CaretPos.x--;
						if (m_CaretPos.x < m_ScrPos.x)
						{
							m_ScrPos.x -= m_nCharsPerLine/2;
							MIN_LIMIT(m_ScrPos.x, 0);
						}

						if (m_CaretPos.x > m_InputStr.m_nLength)
						{
							m_InputStr.SetAt(m_CaretPos.x, 0);
						} else
						if (m_CaretPos.x >= m_Prefix.m_nLength)
						{
							m_InputStr.Delete(m_CaretPos.x, 1);
						}
						RecalcLayout();
						Update((WISP_RECT*)0);
					}
				}
				return true;
			}
			
			if (pMsg->Char.Char != 10) //\n
			{
				if (pMsg->Char.Char == 9) //TAB \v
				{
					for (map<TWideNoCaseString, COMMAND_ENTRY>::IT it = m_pConsoleWnd->CCmdParser::m_CmdMap.begin();
						it != m_pConsoleWnd->CCmdParser::m_CmdMap.end(); ++it)
					{
						if (!TStrNICmp(&m_InputStr.m_pData[m_Prefix.m_nLength],
								it->first.operator const WCHAR*(),
								m_InputStr.m_nLength - m_Prefix.m_nLength))
						{
							m_InputStr = m_Prefix + it->first;
							m_CaretPos.x = m_InputStr.m_nLength;
							break;
						}
					}
				} else
				{
					if (m_CaretPos.x >= m_InputStr.m_nLength)
						m_InputStr += pMsg->Char.Char;
					else	m_InputStr.Insert(m_CaretPos.x, pMsg->Char.Char);
	
					MIN_LIMIT(m_MaxChars, m_InputStr.m_nLength + 1);
					++m_CaretPos.x;
					if (m_CaretPos.x >= m_ScrPos.x + m_nCharsPerLine)
						++m_ScrPos.x;
				}
			}

			if (pMsg->Char.Char == 10) //\n
			{
				if (m_InputStr.m_nLength > m_Prefix.m_nLength)
				{
					m_CmdHistroy.Append(&m_InputStr);
					m_CurHisIndex = -1;
				}
				WISP_MSG Msg;
				Msg.hWnd = this;
				Msg.Msg = WISP_WM_COMMAND;

				Msg.Command.CmdMsg = 0x80000010; //WISP_ID_CMD
				Msg.Command.CmdID = -1;

				Msg.Command.Param2 = (void*)m_InputStr.operator const WCHAR*(); //???

				m_CommandStr = &m_InputStr.m_pData[m_Prefix.m_nLength];
				m_InputStr.SetAt(m_Prefix.m_nLength, 0);

				m_CaretPos.x = m_Prefix.m_nLength;
				m_pWispBase->SendMessage(&Msg);
				if (m_nLinesPerPage > 0)
					EnsureLineVisible();

				m_CaretPos.y = m_LineList.Count;
				m_ScrPos.x = 0;
			}
			RecalcLayout();
			Update((WISP_RECT*)0);
			
		}
		return true;
	}
	bool CWispTerminalWnd::OnCreate(WISP_MSG *pMsg)
	{
		EnableScrollBar(0, 1, 1);
		EnableScrollBar(1, 1, 1);
		m_InputStr.Empty();
		m_InputStr = m_Prefix;
		m_CurHisIndex = -1;
		m_CaretPos.x = m_ScrPos.x + m_InputStr.m_nLength;
		m_CaretPos.y = m_LineList.Count;
		RecalcLayout();
		UpdateCaretState();
		CreateCaret(WISP_DI_CARET);
		ShowCaret(true);
		BlinkCaret(300);
		m_Style |= 0x20000;
		SetBGColor(m_pWispBase->m_pWispDrawObj->m_crSystem[0]);
		m_TextColor = 1;
		m_bNewLine = true;
		return true;
	}
	bool CWispTerminalWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (!pMsg->KeyEvent.bKeyDown)
			return true;

		switch (pMsg->KeyEvent.KeyType)
		{
		case VK_END:
			m_CaretPos.x = m_InputStr.m_nLength;
			EnsureInputVisible2();
			RecalcLayout();
			Update(&m_ClientRect);
			break;
		case VK_HOME:
			m_CaretPos.x = m_Prefix.m_nLength;
			EnsureInputVisible2();
			RecalcLayout();
			Update(&m_ClientRect);
			break;
		case VK_LEFT:
			ResetSelect();
			if (m_CaretPos.x > m_Prefix.m_nLength)
			{
				m_CaretPos.x--;
				EnsureInputVisible2();
				RecalcLayout();
			}
			Update(&m_ClientRect);
			break;
		case VK_RIGHT:
			ResetSelect();
			if (m_CaretPos.x < m_InputStr.m_nLength)
			{
				m_CaretPos.x++;
				EnsureInputVisible2();
				RecalcLayout();
			}
			Update(&m_ClientRect);
			break;
		case VK_UP:
			if (m_CmdHistroy.Count && m_CurHisIndex < m_CmdHistroy.Count-1)
			{
				if (m_CurHisIndex >= 0)
					m_CurHisIndex++;
				else	m_CurHisIndex = 0;
				m_InputStr = m_CmdHistroy[m_CurHisIndex];
				m_CaretPos.x = m_InputStr.m_nLength;
				RecalcLayout();
				Update((WISP_RECT*)0);
			}
			break;
		case VK_DOWN:
			if (m_CmdHistroy.Count && m_CurHisIndex > 0)
			{
				m_CurHisIndex--;
				m_InputStr = m_CmdHistroy[m_CurHisIndex];
				m_CaretPos.x = m_InputStr.m_nLength;
				RecalcLayout();
				Update((WISP_RECT*)0);
			}
			break;
		case 0x40000058: //WISP_SK_CTRL + X
		case 0x2E: //??? .
			if (m_bSelect
				&& (m_pSelStartPos->y == m_LineList.Count)
				&& (m_pSelStartPos->x >= m_Prefix.m_nLength)
				&& (m_pSelEndPos->x >= m_pSelStartPos->x))
			{
				int selectlen = m_pSelEndPos->x - m_pSelStartPos->x + 1;
				if (pMsg->KeyEvent.KeyType == 0x40000058) //WISP_SK_CTRL + X
				{
					TWideString S; //Set ???
					S.Preallocate(selectlen + 1, 0);
					S.m_nLength = TStrCpyLimit(S.m_pData, &m_InputStr.m_pData[m_pSelStartPos->x], selectlen + 1);
					m_pWispBase->SetClipboard(S.operator const WCHAR*());
				}
				m_InputStr.Delete(m_pSelStartPos->x, selectlen);
				m_bSelect = false;
				m_CaretPos.x = m_pSelStartPos->x;
				RecalcLayout();
				Update(&m_ClientRect);
			} else
			{
				if (m_CaretPos.x < m_InputStr.m_nLength)
				{
					m_InputStr.Delete(m_CaretPos.x, 1);
					RecalcLayout();
					Update(&m_ClientRect);
				}
			}
			break;
		case 0x10000025: //WISP_SK_SHIFT + VK_LEFT
			if (m_bSelect)
			{
				if (m_CaretPos.x > m_Prefix.m_nLength && m_RealSelEndPos.y == m_LineList.Count)
				{
					m_RealSelEndPos.x = --m_CaretPos.x;
					UpdateCaretState();
					m_RealSelEndPos.y = m_RealSelStartPos.y;
				}
			} else
			if (m_CaretPos.x > m_Prefix.m_nLength)
			{
				m_RealSelStartPos.x = m_CaretPos.x;
				m_RealSelStartPos.y = m_CaretPos.y;

				m_RealSelEndPos.x = --m_RealSelStartPos.x;
				m_RealSelEndPos.y = m_RealSelStartPos.y;

				m_CaretPos.x = m_RealSelEndPos.x;
				m_bSelect = true;
				UpdateCaretState();
			}
			//UpdateSelect();
			if (m_RealSelEndPos.y >= m_RealSelStartPos.y &&
				(m_RealSelEndPos.y != m_RealSelStartPos.y ||
				m_RealSelEndPos.x >= m_RealSelStartPos.x))
			{
				m_pSelStartPos = &m_RealSelStartPos;
				m_pSelEndPos = &m_RealSelEndPos;
			} else
			{
				m_pSelStartPos = &m_RealSelEndPos;
				m_pSelEndPos = &m_RealSelStartPos;
			}
			Update((WISP_RECT*)0);
			break;
		case 0x10000027: //WISP_SK_SHIFT + VK_RIGHT
			if (m_bSelect)
			{
				if (m_CaretPos.x < m_InputStr.m_nLength && m_RealSelEndPos.y == m_LineList.Count)
				{
					m_RealSelEndPos.x = m_CaretPos.x++;
					UpdateCaretState();
					m_RealSelEndPos.y = m_RealSelStartPos.y;
				}
			} else
			if (m_CaretPos.x < m_InputStr.m_nLength)
			{
				m_RealSelStartPos.x = m_CaretPos.x;
				m_RealSelStartPos.y = m_CaretPos.y;

				m_RealSelEndPos.x = m_CaretPos.x++;
				m_RealSelEndPos.y = m_CaretPos.y;
				m_bSelect = true;
				UpdateCaretState();
			}
			//UpdateSelect();
			if (m_RealSelEndPos.y >= m_RealSelStartPos.y &&
				(m_RealSelEndPos.y != m_RealSelStartPos.y ||
				m_RealSelEndPos.x >= m_RealSelStartPos.x))
			{
				m_pSelStartPos = &m_RealSelStartPos;
				m_pSelEndPos = &m_RealSelEndPos;
			} else
			{
				m_pSelStartPos = &m_RealSelEndPos;
				m_pSelEndPos = &m_RealSelStartPos;
			}
			Update((WISP_RECT*)0);
			break;
		case VK_LBUTTON:
			if ((pMsg->m_LogicMousePosWnd.y - m_Margin)/m_ClientDC.m_pFont->m_Height == m_LineList.Count - m_ScrPos.y)
			{
				int PosX = m_ScrPos.x + (pMsg->m_LogicMousePosWnd.x - m_Margin)/m_ClientDC.m_pFont->m_Width;
				if (PosX >= m_Prefix.m_nLength && PosX <= m_InputStr.m_nLength)
				{
					m_CaretPos.x = PosX;
					RecalcLayout();
					Update(&m_ClientRect);
				}
			}
			break;
		}
		return true;
	}
	bool CWispTerminalWnd::OnDestroy(WISP_MSG *pMsg)
	{
		return true;//???
	}

	WISP_CMD_MAP_BEGIN(CWispTerminalWnd)
		WISP_CMD_MAP_DEFAULT(OnCmdGetCmd)
	WISP_CMD_MAP_END

	bool CWispTerminalWnd::OnCmdGetCmd(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000010) //WISP_ID_CMD
		{
			++m_LockUpdateRef;
			Printf(WSTR("%s%s\n"), m_InputStr.operator const WCHAR*(), m_CommandStr.operator const WCHAR*());
			if (!m_pConsoleWnd)
			{
				Printf(WSTR("Error : inside error!\n"));
				--m_LockUpdateRef;
				return false;
			}

			if (m_CommandStr.m_nLength)
			{
				if (m_CtrlType == WISP_CTRL_TERMINAL_WND)
					m_pConsoleWnd->m_pTerminalWnd = this;

				if (m_pConsoleWnd->OnAcceptingCmd(m_CommandStr.operator const WCHAR*()))
				{
					if (!m_pConsoleWnd->CCmdParser::ParseCmd(m_CommandStr.operator const WCHAR*(), 0))
						Printf(WSTR("Error : unknown command !\n"));
					OnAcceptCmd(m_CommandStr.operator const WCHAR*());
				}
				if (m_CtrlType == WISP_CTRL_TERMINAL_WND)
					m_pConsoleWnd->m_pTerminalWnd = 0;
			}
			--m_LockUpdateRef;
		}
		return true;
	}

	WISP_EVENT_MAP_BEGIN(CWispTerminalWnd)
		WISP_EVENT_MAP(WISP_ID_PASTE, OnEventPaste)
	WISP_EVENT_MAP_END

	bool CWispTerminalWnd::OnEventPaste(WISP_MSG *pMsg)
	{
		m_pWispBase->GetClipboard(); //TODO
		if (!m_pWispBase->m_ClipString.IsEmpty())
		{
			m_InputStr.Insert(m_CaretPos.x, m_pWispBase->m_ClipString.operator const WCHAR*());

			if (m_InputStr.m_nLength >= WISP_TERMINAL_LINE_SIZE)
				m_InputStr.SetAt(WISP_TERMINAL_LINE_SIZE, 0);

			m_CaretPos.x += m_pWispBase->m_ClipString.m_nLength;

			MAX_LIMIT(m_CaretPos.x, m_InputStr.m_nLength);

			UpdateCaretState();
			Update((WISP_RECT*)0);
		}
		return true;
	}
	unsigned char CWispTerminalWnd::GetLineStr(int LineIndex, unsigned long *pColorStr)
	{
		if (LineIndex <= m_LineList.Count) //??? < +1
		{
			if (LineIndex < m_LineList.Count)
				TStrCpy(pColorStr, m_LineList[LineIndex].operator const unsigned long*());
			else	ColorStrCpy(pColorStr, m_InputStr.operator const WCHAR*(), m_TextColor, (unsigned char)0);
		}
		return 0;
	}

#include "../Code/fileio.hpp"

	bool CWispTerminalWnd::SaveHistory(const char *FileName)
	{
		CImageFileStream File;
		gpFileIO->DeleteFile(FileName);

		if (!File.Create(FileName, 0))
			return false;

		for (int Line = 0; Line < m_LineList.Count; ++Line)
		{
			WCHAR UniString[258];
			TStrCpyLimit(UniString, m_LineList[Line].operator const unsigned long*(), 258);
			char AnsiString[260];
			int Len = UnicodeToAnsi(UniString, AnsiString, 258); //??? Rtl
			TStrCpy(&AnsiString[Len], "\r\n");
			File.Write(AnsiString, Len + 2);
		}
		File.Close();
		return true;
	}
///
	CWispConsoleWnd::CWispConsoleWnd()
	{
		m_pTerminalWnd = 0;
		m_CtrlType = WISP_CTRL_CONSOLE_WND;
		m_pConsoleWnd = this;
	}
	CWispConsoleWnd::~CWispConsoleWnd()
	{
	}

	void CWispConsoleWnd::OnBeginCmd(const WCHAR *szCmd)
	{
		//EMPTY
	}
	void CWispConsoleWnd::OnCmd(const WCHAR *szCmd)
	{
		//EMPTY
	}
	void CWispConsoleWnd::OnEndCmd(const WCHAR *szCmd)
	{
		//EMPTY
	}

	void CWispConsoleWnd::PrintString(const WCHAR *pString)
	{
		Printf(pString);
	}

	int CWispConsoleWnd::Cls(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd)
	{
		CWispTerminalWnd *pTermWnd = (CWispTerminalWnd *)pConWnd;
		if (pConWnd->m_pTerminalWnd)
			pTermWnd = (CWispTerminalWnd *)pConWnd->m_pTerminalWnd;

		pTermWnd->ClearScreen();
		return 0;
	}
	int CWispConsoleWnd::WispVer(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd)
	{
		CWispTerminalWnd *pTermWnd = (CWispTerminalWnd *)pConWnd;
		if (pConWnd->m_pTerminalWnd)
			pTermWnd = (CWispTerminalWnd *)pConWnd->m_pTerminalWnd;

		pTermWnd->Printf(WSTR("Wisp GUI System\n"));
		WCHAR szDate[64];
		WCHAR szTime[64];
		TStrCpy(szDate, "" __DATE__ "");
		TStrCpy(szTime, "" __TIME__ "");
		pTermWnd->Printf(WSTR("Build Time : %s  %s\n"), szDate, szTime);
		return 0;
	}
	int CWispConsoleWnd::HL(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd)
	{
		CWispTerminalWnd *pTermWnd = (CWispTerminalWnd *)pConWnd;
		if (pConWnd->m_pTerminalWnd)
			pTermWnd = (CWispTerminalWnd *)pConWnd->m_pTerminalWnd;

		if (argc != 2)
		{
			pTermWnd->Printf(WSTR("Usage : hl <histroy lines count>\n"));
			pTermWnd->Printf(WSTR("Current count of histroy lines = %<E>%d (dec)\n"), pTermWnd->m_LineList.Size);
			return -1;
		}

		unsigned int line;
		if (!USDecStrToNum(argv[1], &line))
		{
			pTermWnd->Printf(WSTR("Error : %s is Invalid !\n"), argv[1]);
			return -1;
		}

		if (line < 1 || line > 10000)
		{
			pTermWnd->Printf(WSTR("Error : please input from 1 to 1000\n"));
			return -1;
		}
		pTermWnd->SetHistoryLines(line);
		return 0;
	}
	int CWispConsoleWnd::HCC(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd)
	{
		CWispTerminalWnd *pTermWnd = (CWispTerminalWnd *)pConWnd;
		if (pConWnd->m_pTerminalWnd)
			pTermWnd = (CWispTerminalWnd *)pConWnd->m_pTerminalWnd;

		if (argc == 1)
		{
			pTermWnd->Printf(WSTR("command histroy count = %d\n"), pTermWnd->m_CmdHistroy.Count);
			return 0;
		}
		unsigned int count;
		if (argc > 2 || !USDecStrToNum(argv[1], &count))
		{
			pTermWnd->Printf(WSTR("Usage : hcc command_histroy_count\n"));
			return 1;		
		}
		pTermWnd->m_CmdHistroy.Resize(count);
		return 0;
	}
	int CWispConsoleWnd::HC(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd)
	{
		//???++
		CWispTerminalWnd *pTermWnd = (CWispTerminalWnd *)pConWnd;
		if (pConWnd->m_pTerminalWnd)
			pTermWnd = (CWispTerminalWnd *)pConWnd->m_pTerminalWnd;

		if (argc == 1)
		{
			pConWnd->Printf(WSTR("Usage : hc sz_command_line\n"));
			return 1;
		}
		const TWideString str = pConWnd->m_Prefix + &szCmd[argv[1] - argv[0]];
		pTermWnd->m_CmdHistroy.Append(&str);
		return 0;
	}
	int CWispConsoleWnd::Color(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd)
	{
		CWispTerminalWnd *pTermWnd = (CWispTerminalWnd *)pConWnd;
		if (pConWnd->m_pTerminalWnd)
			pTermWnd = (CWispTerminalWnd *)pConWnd->m_pTerminalWnd;

		unsigned char Color;
		if (argc != 2 || !USHexStrToNum(argv[1], &Color))
		{
			pTermWnd->Printf(WSTR("Usage : Color Color_Index[0 - FF]\n"));
			pConWnd->DisplayColor(pTermWnd); //???
		} else
		{
			pTermWnd->SetFontColor(Color);
		}
		return 0;
	}
	int CWispConsoleWnd::BKColor(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd)
	{
		CWispTerminalWnd *pTermWnd = (CWispTerminalWnd *)pConWnd;
		if (pConWnd->m_pTerminalWnd)
			pTermWnd = (CWispTerminalWnd *)pConWnd->m_pTerminalWnd;

		unsigned char Color;
		if (argc != 2 || !USHexStrToNum(argv[1], &Color))
		{
			pTermWnd->Printf(WSTR("Usage : BKColor Color_Index[0 - FF]\n"));
			pConWnd->DisplayColor(pTermWnd); //???
		} else
		{
			pTermWnd->SetBGColor(pTermWnd->m_pWispBase->m_pWispDrawObj->m_crSystem[Color]);
		}
		return 0;
	}
	int CWispConsoleWnd::Help(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd)
	{
		WCHAR szMaskCmd[128]; //???
		szMaskCmd[0] = 0;

		CWispTerminalWnd *pTermWnd = (CWispTerminalWnd *)pConWnd;
		if (pConWnd->m_pTerminalWnd)
			pTermWnd = (CWispTerminalWnd *)pConWnd->m_pTerminalWnd;

		if (argc < 2)
		{
			TStrCpy(szMaskCmd, "*");
		} else
		{
			TStrCpy(szMaskCmd, argv[1]);
			TStrLwr(szMaskCmd);
			if (!TStrChr(szMaskCmd, '*') && !TStrChr(szMaskCmd, '?'))
			{
				map<TWideNoCaseString,COMMAND_ENTRY>::IT It = pConWnd->CCmdParser::m_CmdMap.find(szMaskCmd);
				//if (It == pConWnd->CCmdParser::m_CmdMap.end())
				if (!It)
				{
					pTermWnd->Printf(WSTR("Invalid Command\n"));
				} else
				{
					if (It->second.pUsage)
						pTermWnd->Printf(It->second.pUsage);
					else
					if (It->second.pComment)
						pTermWnd->Printf(WSTR("%s %s\n"), It->first.operator const WCHAR*(), It->second.pComment);
				}
				return 0;
			}
		}

		for (map<TWideNoCaseString,COMMAND_ENTRY>::IT it = pConWnd->CCmdParser::m_CmdMap.begin();
			it != pConWnd->CCmdParser::m_CmdMap.end(); ++it)
		{
			if (TIMultiMatchWithPattern(szMaskCmd, it->first.operator const WCHAR*()))
			{
				if (it->second.pComment)
					pTermWnd->Printf(WSTR("%<B>%-12s    %<F>%s\n"), it->first.operator const WCHAR*(), it->second.pComment);
			}
		}
		return 0;
	}
	int CWispConsoleWnd::Bat(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd)
	{
		CWispTerminalWnd *pTermWnd = (CWispTerminalWnd *)pConWnd;
		if (pConWnd->m_pTerminalWnd)
			pTermWnd = (CWispTerminalWnd *)pConWnd->m_pTerminalWnd;

		CTXTFile BatFile;
		if (argc < 2)
		{
			pTermWnd->Printf(WSTR("Usage:%<B>bat [-s|b] %<1>bat_file_name \n"));
			pTermWnd->Printf(WSTR("s : silence mode\n"));
			pTermWnd->Printf(WSTR("b : break if command fail\n"));
			return 1;
		}
		char FileName[MAX_FN_LEN];
		bool silence = false;
		bool fail = false;
		if (argv[1][0] == (WCHAR)'-')
		{
			silence = TStrIChr(argv[1], (WCHAR)'s') != 0;
			fail = TStrIChr(argv[1], (WCHAR)'b') != 0;
			if (argc < 3)
			{
				pTermWnd->Printf(WSTR("Usage:%<B>bat [-s|b] %<1>bat_file_name \n"));
				pTermWnd->Printf(WSTR("s : silence mode\n"));
				pTermWnd->Printf(WSTR("b : break if command fail\n"));
				return 1;
			}
			UnicodeToAnsi(&szCmd[argv[2] - argv[0]], FileName, MAX_FN_LEN); //??? Rtl
		} else
		{
			UnicodeToAnsi(&szCmd[argv[1] - argv[0]], FileName, MAX_FN_LEN); //??? Rtl
		}

		if (!BatFile.Open(FileName, 0))
		{
			if (!silence)
				pTermWnd->Printf(WSTR("%<3>Error : %<B>Fail to open file %<F>%s %<1>!\n"), argv[1]);
			return 1;
		}
		WCHAR szCmds[258];
		pConWnd->OnBeginCmd(szCmd);
		unsigned long Result = 0;
		TListIter<char*> It = BatFile.m_StrList.Begin();
		while (It != BatFile.m_StrList.End())
		{
			AnsiToUnicode(*It, szCmds, 257); //MAX_SIZE //??? Rtl
			pConWnd->OnCmd(szCmds);
			if (!pConWnd->CCmdParser::ParseCmd(szCmds, &Result) && fail)
				break;
			if (fail && Result)
				break;
			++It;
		}
		BatFile.Close();
		pConWnd->OnEndCmd(szCmd);
		return Result;
	}

	void SetHelp(const WCHAR *Name, int Type, void *Value, CWispTerminalWnd *pTermWnd)
	{
		switch (Type)
		{
		case SETOPTION_INVALID: pTermWnd->Printf(WSTR("Error : Invalid value name !\n")); break;
		case SETOPTION_STR:  pTermWnd->Printf(WSTR("%<B>%-12s %<1>= %<F>\"%s\"%<1>\n"), Name, (WCHAR*)Value); break;
		case SETOPTION_INT:  pTermWnd->Printf(WSTR("%<B>%-12s %<1>= %<4>%d %<1>(dec)\n"), Name, *(int*)Value); break;
		case SETOPTION_UINT: pTermWnd->Printf(WSTR("%<B>%-12s %<1>= %<4>%u %<1>(dec)\n"), Name, *(unsigned int*)Value); break;
		default:;
		}
	}

	int CWispConsoleWnd::Set(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd)
	{
		CWispTerminalWnd *pTermWnd = (CWispTerminalWnd *)pConWnd;
		if (pConWnd->m_pTerminalWnd)
			pTermWnd = (CWispTerminalWnd *)pConWnd->m_pTerminalWnd;

		if (argc == 1)
		{
			for (map<TWideNoCaseString, SETOPTION>::IT it = pConWnd->m_Options.begin();
				it != pConWnd->m_Options.end(); ++it)
			{
				SetHelp(it->first.operator const WCHAR*(), it->second.Type, (void*)it->second.Value, pTermWnd);
			}
			return 1;
		} else
		if (argc == 2)
		{
			void *Value;
			int Size;
			int Type = pConWnd->m_Options.Get(argv[1], &Value, &Size);
			SetHelp(argv[1], Type, Value, pTermWnd);
			return 1;
		}

		void *Value;
		int Size;
		int Type = pConWnd->m_Options.Get(argv[1], &Value, &Size);
		switch (Type)
		{
		case SETOPTION_INVALID: {
			pTermWnd->Printf(WSTR("Error : Invalid value name!\n"));
			return 1;
			} break;
		case SETOPTION_STR:
			TStrCpyLimit((WCHAR*)Value, argv[2], Size);
			break;
		case SETOPTION_INT:
			if (!SDecStrToNum(argv[2], (int*)Value))
			{
				pTermWnd->Printf(WSTR("Error : Invalid value!\n"));
				return 1;
			}
			break;
		case SETOPTION_UINT:
			if (!USDecStrToNum(argv[2], (unsigned int*)Value))
			{
				pTermWnd->Printf(WSTR("Error : Invalid value!\n"));
				return 1;
			}
			break;
		default:
			if (Value == 0)
			{
				pTermWnd->Printf(WSTR("Error : Invalid value!\n"));
				return 1;
			}
			break;
		}
		return 0;
	}
	int CWispConsoleWnd::Rem(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd)
	{
		return 0; //!
	}
	int CWispConsoleWnd::Echo(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd)
	{
		CWispTerminalWnd *pTermWnd = (CWispTerminalWnd *)pConWnd;
		if (pConWnd->m_pTerminalWnd)
			pTermWnd = (CWispTerminalWnd *)pConWnd->m_pTerminalWnd;

		pTermWnd->Printf(&szCmd[TStrLen(argv[0])+1]);
		pTermWnd->OutPut(WSTR("\n"));
		return 0;
	}

	int CWispConsoleWnd::HeapInfo(int argc, const WCHAR **argv, const WCHAR *szCmd, CWispConsoleWnd *pConWnd)
	{
		//TODO
		return 0;
	}

	void CWispConsoleWnd::DisplayColor(CWispTerminalWnd *pTermWnd)
	{
		pTermWnd->Printf(WSTR("%<0>0 ------- Black\n"));
		pTermWnd->Printf(WSTR("%<1>1 ------- White\n"));
		pTermWnd->Printf(WSTR("%<2>2 ------- Red\n"));
		pTermWnd->Printf(WSTR("%<3>3 ------- Light Red\n"));
		pTermWnd->Printf(WSTR("%<4>4 ------- Green\n"));
		pTermWnd->Printf(WSTR("%<5>5 ------- Light Green\n"));
		pTermWnd->Printf(WSTR("%<6>6 ------- Blue\n"));
		pTermWnd->Printf(WSTR("%<7>7 ------- Light Blue\n"));
		pTermWnd->Printf(WSTR("%<8>8 ------- Brown\n"));
		pTermWnd->Printf(WSTR("%<9>9 ------- Light Brown\n"));
		pTermWnd->Printf(WSTR("%<A>A ------- Cyan\n"));
		pTermWnd->Printf(WSTR("%<B>B ------- Light Cyan\n"));
		pTermWnd->Printf(WSTR("%<C>C ------- Gray\n"));
		pTermWnd->Printf(WSTR("%<D>D ------- Light Gray\n"));
		pTermWnd->Printf(WSTR("%<E>E ------- Yellow\n"));
		pTermWnd->Printf(WSTR("%<F>F ------- Light Yellow\n"));
	}

	WISP_MSG_MAP_BEGIN(CWispConsoleWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
	WISP_MSG_MAP_END(CWispTerminalWnd)

	bool CWispConsoleWnd::OnCreate(WISP_MSG *pMsg)
	{                                   
		CCmdParser::InsertCmd(WSTR("cls"), (CMDPROC)CWispConsoleWnd::Cls, this, WSTR("Clear screen"), 0);
		CCmdParser::InsertCmd(WSTR("wispver"), (CMDPROC)CWispConsoleWnd::WispVer, this, WSTR("Display WISP version"), 0);
		CCmdParser::InsertCmd(WSTR("hl"), (CMDPROC)CWispConsoleWnd::HL, this, WSTR("Set count of histroy lines"), 0);
		CCmdParser::InsertCmd(WSTR("hcc"), (CMDPROC)CWispConsoleWnd::HCC, this, WSTR("Set count of command histroy"), 0);
		CCmdParser::InsertCmd(WSTR("hc"), (CMDPROC)CWispConsoleWnd::HC, this, WSTR("add a command string into histroy"), 0);
		CCmdParser::InsertCmd(WSTR("Color"), (CMDPROC)CWispConsoleWnd::Color, this, WSTR("Set default font color"), 0);
		CCmdParser::InsertCmd(WSTR("BKColor"), (CMDPROC)CWispConsoleWnd::BKColor, this, WSTR("Set default font color"), 0);
		CCmdParser::InsertCmd(WSTR("help"), (CMDPROC)CWispConsoleWnd::Help, this, WSTR("Display this help information"), 0);
		CCmdParser::InsertCmd(WSTR("h"), (CMDPROC)CWispConsoleWnd::Help, this, WSTR("Display this help information"), 0);
		CCmdParser::InsertCmd(WSTR("bat"), (CMDPROC)CWispConsoleWnd::Bat, this, WSTR("Execute bat command file"), 0);
		CCmdParser::InsertCmd(WSTR("set"), (CMDPROC)CWispConsoleWnd::Set, this, WSTR("Set environment value"), 0);
		CCmdParser::InsertCmd(WSTR("#"), (CMDPROC)CWispConsoleWnd::Rem, this, WSTR("Do nothing"), 0);
		CCmdParser::InsertCmd(WSTR("@"), (CMDPROC)CWispConsoleWnd::Echo, this, WSTR("Print string"), 0);

		CCmdParser::InsertCmd(WSTR("wispheapinfo"), (CMDPROC)CWispConsoleWnd::HeapInfo, this, WSTR("Show heap debug information"), 0);
		return true;
	}
	bool CWispConsoleWnd::OnDestroy(WISP_MSG *pMsg)
	{
		CCmdParser::Reset();
		return true;
	}


#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "syserdefine.hpp"

int expr_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc < 2)
	{
		WCHAR szCmd[64];
		TSPrintf(szCmd, WSTR("help %s"), argv[0]);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(szCmd, 0);
		return -1;
	}

	ULONG_PTR Result;
	char szSym[256];
	if (USHexStrToNum(argv[1], &Result) && gpSyser->m_SyserUI.GetSymbol(Result, szSym, 64))
	{
		WCHAR Buffer[256];
		TStrCpy(Buffer, szSym);
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<F>%s\n"), Buffer);
		return 0;
	}

	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Result))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : %s is Invalid !\n"), argv[1]);
		return -1;
	}

	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Hex : " HEXFMT "\n"), Result);
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Dec : " DECFMT "\n"), Result);
	return 0;
}

int cv_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
	if (!pWnd) return -1;
	CListStringItem *Item = pWnd->GetRootItem();
	while (Item)
	{
		WCHAR Buffer[256];

		pWnd->GetItemText(Item, CV_ADDR, Buffer, lenof(Buffer));
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%s "), Buffer);

		pWnd->GetItemText(Item, CV_CODE, Buffer, lenof(Buffer));
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%-20s "), Buffer);

		pWnd->GetItemText(Item, CV_INST, Buffer, lenof(Buffer));
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%-30s "), Buffer);

		pWnd->GetItemText(Item, CV_CMT, Buffer, lenof(Buffer));
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%s\n"), Buffer);

		Item = pWnd->GetNext(Item, 0);
	}
	return 0;
}

int dv_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	ULONG_PTR Addr = -1;
	CDataView *pWnd = (CDataView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveWnd();
	if (!pWnd) return -1;
	int nLines_Page = pWnd->m_nLines_Page;
	if (argc > 1)
	{
		ULONG_PTR Address;
                if (gpSyser->m_SyserUI.CalcExp(argv[1], &Address))
                {
                        Addr = pWnd->m_CurAddr;
                        pWnd->m_CurAddr = Address;
                }

		ULONG_PTR Result;
                if (argc > 2 && gpSyser->m_SyserUI.CalcExp(argv[2], &Result))
                        nLines_Page = Result;
	}

	int nLine = 0;
	while (nLine < nLines_Page)
	{
		ULONG_PTR Address = pWnd->m_CurAddr + nLine * pWnd->m_nByte_Line;
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("" F0ADDR "X   "), Address);
		unsigned int Len = pWnd->m_ImageBase + pWnd->m_ImageSize - nLine * pWnd->m_nByte_Line - pWnd->m_CurAddr;
		MAX_LIMIT(Len, pWnd->m_nByte_Line);

		WCHAR szSpaces[32];
		TStrCpy(szSpaces, CWispHexWnd::m_Spaces);
		szSpaces[pWnd->m_ShowLength] = 0;
	
		WCHAR String[512];
		unsigned char Buffer[32];

		int LenAlign = (int)Len/pWnd->m_ShowLength;
		if (LenAlign % pWnd->m_ShowLength)
			++LenAlign;

		pWnd->m_LenArray[nLine][HEX_HEX_INDEX] = pWnd->m_ShowLength * LenAlign * 3;

		for (int i = 0; i < LenAlign; ++i)
		{
			if (pWnd->ReadMemory(Address + i * pWnd->m_ShowLength, Buffer, pWnd->m_ShowLength))
				THexBytesToStr(Buffer, String, Len, pWnd->m_ShowLength, szSpaces);
			else	TStrCpy(String, CWispHexWnd::m_UnReadable);
			String[pWnd->m_ShowLength*2] = 0;
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%s "), String);
		}

		for (int i = 0; i < Len; ++i)
		{
			if (pWnd->ReadMemory(Address+i, Buffer, 1))
				THexBytesToPrint(Buffer, String, 1);
			else	TStrCpy(String, CWispHexWnd::m_UnReadableASCII);
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%c"), String[0]);
		}
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("\n"));
		++nLine;
	}

	if (Addr != -1)
		pWnd->m_CurAddr = Addr;
	return 0;
}

int sw_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
	{
		if (argc == 1)
		{
			if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP)
				gpSyser->m_MainFrame.m_SystemExplorer.m_StackWatchList.UpdateContext(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP, false);
			return 0;
		}

		ULONG_PTR Address;
		if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Address))
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Input error!\n"));
			return -1;
		}
		gpSyser->m_MainFrame.m_SystemExplorer.m_StackWatchList.UpdateContext(Address, false);
	}
	return 0;
}

int d_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc  > 3)
		return 1;
	bool widthsame = false;

	switch ((*argv)[1])
	{
	case 'B':
	case 'b':
		if (pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveDataWidth() == 1) widthsame = 1;
		else	pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.SetActiveDataWidth(1);
		break;
	case 'W':
	case 'w':
		if (pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveDataWidth() == 2) widthsame = 1;
		else	pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.SetActiveDataWidth(2);
		break;
	case 'D':
	case 'd':
		if (pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveDataWidth() == 4) widthsame = 1;
		else	pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.SetActiveDataWidth(4);
		break;
#ifdef _X64_
	case 'Q':
	case 'q':
		if (pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveDataWidth() == 8) widthsame = 1;
		else	pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.SetActiveDataWidth(8);
		break;
#endif
	}

	if (argc == 1)
	{
		if (!widthsame) return 0;
		CDataView *pWnd = (CDataView *)pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveWnd();
		if (!pWnd) return 1;
		ULONG_PTR Addr = pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveAddress();
		Addr += pWnd->m_nLines_Page * 16;
		if (pWnd->m_bShowOffset)
			Addr -= 16;
		pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.SetActiveAddress(Addr, 0);
		return 0;
	} else
	if (argc == 2)
	{
		CDataView *pWnd = (CDataView *)pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveWnd();
		if (!pWnd) return 1;
		
		if (!TStrICmp(argv[1], WSTR("-")))
		{
			pWnd->OnEventPrevHistory(0);
			return 0;
		}
		if (!TStrICmp(argv[1], WSTR("+")))
		{
			pWnd->OnEventNextHistory(0);
			return 0;
		}
		ULONG_PTR Address;
		if (gpSyser->m_SyserUI.CalcExp(argv[1], &Address))
		{
			pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.SetActiveAddress(Address, argv[1]);
			return 0;
		}
		return 1;
	} else
	if (argc == 3)
	{
		if (!TStrICmp(argv[1], WSTR("-")))
		{
			ULONG_PTR Address;
			if (!gpSyser->m_SyserUI.CalcExp(argv[2], &Address))
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : %<3>Input error !\n"));
				return -1;
			}
			if (!pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveWnd())
				return 1;
			pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.SetActiveAddress(
			pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveAddress()-Address, 0);
			return 0;
		}

		if (!TStrICmp(argv[1], WSTR("+")))
		{
			ULONG_PTR Address;
			if (!gpSyser->m_SyserUI.CalcExp(argv[2], &Address))
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : %<3>Input error !\n"));
				return -1;
			}
			if (!pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveWnd())
				return 1;
			pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.SetActiveAddress(
			pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveAddress()+Address, 0);
			return 0;
		}
#if 0
		if (!TStrICmp(argv[1], WSTR("-P")))
		{
			ULONG_PTR Address;
			if (!gpSyser->m_SyserUI.CalcExp(argv[2], &Address))
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : %<3>Input error !\n"));
				return -1;
			}
			if (!MapToVAddr(Address, &Address, 1, 0, 0))
				return 1;
			pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.SetActiveAddress(Address, 0);
			return 0;
		}
#endif
	} else
	if (argc == 0)
	{
		//???
		//pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.SetActiveAddress(?, 0);
	}
	return 1;
}

int wc_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;

	pSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(CV_TAB); //CV_TAB(0)
	if (argc == 1)
	{
		CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
		pWnd->Focus();
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Press ESC to return console\n"));
		return 0;
	}

	ULONG_PTR Addr = 0;
	CCodeView *pWnd = 0;
	if (*argv[1] == '-')
	{
		if (pSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetWndCount()>1)
		{
			int Index = pSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWndIndex();
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.RemoveWnd(Index, true);
			gpSyser->m_MainFrame.FocusActive();
		}
		return 0;
	} else
	if (*argv[1] == '+')
	{
		Addr = static_cast<CCodeView*>(gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd())->m_CurAddr;
		pWnd = gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.InsertView(0);
		if (!pWnd) return 1;
	} else
	{
		ULONG_PTR Index;
		if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Index))
			Index = 0;

		pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetWnd(Index);
		if (pWnd)
		{
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.ChangeActiveWnd(pWnd);

			Addr = static_cast<CCodeView*>(gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd())->m_CurAddr;
		} else
		{
			Addr = static_cast<CCodeView*>(gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd())->m_CurAddr;
			pWnd = gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.InsertView(0);
			if (!pWnd) return 1;
		}
	}
	ULONG_PTR Address;
	if (argc > 2 && gpSyser->m_SyserUI.CalcExp(argv[2], &Address))
	{
		pWnd->GotoAddr(Address, true);
	} else
	if (Addr)
	{
		pWnd->GotoAddr(Addr, true);
	}
	pWnd->Focus();
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Press ESC to return console\n"));
	return 0;
}

int wd_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;

	pSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(CV_TAB); //CV_TAB(0)
	if (argc == 1)
	{
		CDataView *pWnd = (CDataView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveWnd();
		pWnd->Focus();
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Press ESC to return console\n"));
		return 0;
	}

	ULONG_PTR Addr = 0;
	CDataView *pWnd = 0;
	if (*argv[1] == '-')
	{
		if (pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetWndCount()>1)
		{
			int Index = pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveWndIndex();
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.RemoveWnd(Index, true);
			gpSyser->m_MainFrame.FocusActive();
		}
		return 0;
	} else
	if (*argv[1] == '+')
	{
		Addr = static_cast<CDataView*>(gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveWnd())->m_CurAddr;
		pWnd = gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.InsertView(0);
		if (!pWnd) return 1;
	} else
	{
		ULONG_PTR Index;
		if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Index))
			Index = 0;

		pWnd = (CDataView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetWnd(Index);
		if (pWnd)
		{
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.ChangeActiveWnd(pWnd);

			Addr = static_cast<CDataView*>(gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd())->m_CurAddr;
		} else
		{
			Addr = static_cast<CDataView*>(gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetActiveWnd())->m_CurAddr;
			pWnd = gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.InsertView(0);
			if (!pWnd) return 1;
		}
	}
	ULONG_PTR Address;
	if (argc > 2 && gpSyser->m_SyserUI.CalcExp(argv[2], &Address))
	{
		pWnd->ViewAddress(Address, true);
	} else
	if (Addr)
	{
		pWnd->ViewAddress(Addr, true);
	}
	pWnd->Focus();
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Press ESC to return console\n"));
	return 0;
}

int ws_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	pSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(CV_TAB); //CV_TAB(0)
	pSyser->m_MainFrame.m_SystemExplorer.m_ContextTabWnd.SetActiveWnd(CV_S_TAB); //CV_S_TAB(0)
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Press ESC to return console\n"));
	return 0;
}

int ww_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	pSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(CV_TAB); //CV_TAB(0)
	pSyser->m_MainFrame.m_SystemExplorer.m_ContextTabWnd.SetActiveWnd(CV_W_TAB); //CV_W_TAB(1)
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Press ESC to return console\n"));
	return 0;
}

int wr_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	pSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(CV_TAB); //CV_TAB(0)
	CWispTabWnd *pWnd = (CWispTabWnd *)pSyser->m_MainFrame.m_SystemExplorer.m_RegTabWnd.GetActiveWnd();
	if (pWnd) pWnd->SetActiveWnd(CV_REG_GENE_TAB); //CV_REG_GENE_TAB(0)
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Press ESC to return console\n"));
	return 0;
}

int wf_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	pSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(CV_TAB); //CV_TAB(0)
	CWispTabWnd *pWnd = (CWispTabWnd *)pSyser->m_MainFrame.m_SystemExplorer.m_RegTabWnd.GetActiveWnd();
	if (pWnd) pWnd->SetActiveWnd(CV_REG_FPU_TAB); //CV_REG_FPU_TAB(1)
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Press ESC to return console\n"));
	return 0;
}

int wx_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	pSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(CV_TAB); //CV_TAB(0)
	CWispTabWnd *pWnd = (CWispTabWnd *)pSyser->m_MainFrame.m_SystemExplorer.m_RegTabWnd.GetActiveWnd();
	if (pWnd) pWnd->SetActiveWnd(CV_REG_SSE_TAB); //CV_REG_SSE_TAB(2)
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Press ESC to return console\n"));
	return 0;
}

int dex_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc == 1)
	{
		unsigned long Index = 0;
		TListIter<WISP_TAB_WND_ITEM> It = gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.m_WndList.Begin();
		while (It != gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.m_WndList.End())
		{
			CDataView *pWnd = static_cast<CDataView*>(It->pWnd);
			bool Float = pWnd->GetFloatAddressMode() == true;
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("DataView[%d] Float Address %<f>%s %<1>Address " F0ADDR "X %s\n"),
					Index, Float?WSTR("On "):WSTR("Off"),
					pWnd->GetFloatAddress(pWnd->m_FloatAddressStr.operator const WCHAR*()),
					pWnd->m_FloatAddressStr.operator const WCHAR*());
			++It;
			++Index;
		}
	} else
	{
		ULONG_PTR Index;
		if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Index))
			Index = 0;
		CDataView *pWnd = (CDataView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.GetWnd(Index);
		if (!pWnd)
			pWnd = gpSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.InsertView(0);
		if (pWnd)
		{
			bool Float = pWnd->GetFloatAddressMode() == true;
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("DataView[%d] Float Address %<f>%s %<1>Address " F0ADDR "X %s\n"),
					Index, Float?WSTR("On "):WSTR("Off"),
					pWnd->GetFloatAddress(pWnd->m_FloatAddressStr.operator const WCHAR*()),
					pWnd->m_FloatAddressStr.operator const WCHAR*());
		}
	}
	return 0;
}

int isf_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc < 2)
	{
		WCHAR szCmd[64];
		TSPrintf(szCmd, WSTR("help %s"), argv[0]);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(szCmd, 0);
		return -1;
	}

	ULONG_PTR Result;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Result))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Address!\n")); //WSTR("Error : %s is Invalid !\n"), argv[1]);
		return -1;
	}

	CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(Result);
	if (!pDbgMod)
		return 1;
	{
		//TODO gpSyser->m_SyserUI.m_SymAnalyzer.???(pDbgMod, Result); //pDbgMod->+30);
		//TODO gpSyser->m_MainFrame.m_SystemExplorer.m_SymAnalyzer.???(pDbgMod, Result); //pDbgMod->+30);
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
	}
	return 0;
}

SYSER_CMD_ENTRY ShareCmdTable[] =
{
	{ WSTR("?"), WSTR("Calculate expression"), expr_command, 
	0},

	{ WSTR("cv"), WSTR("Print Code View string to console"), cv_command,
		WSTR("cv: Dump CodeView context to console (then you can save console histroy by loader)\n"
		"Format: cv\n"
		"Example1: cv\n") },
	{ WSTR("dv"), WSTR("Print Data View string to console"), dv_command,
		WSTR("dv: Dump DataView context to console (then you can save console histroy by loader)\n"
		"Format: dv\n"
		"Example1: dv\n") },

	{ WSTR("sw"), WSTR("Show data at stack watch"), sw_command, 
	0},

	{ WSTR("d"), WSTR("Show memory data"), d_command, 
		WSTR("d,db,dw,dd,dq: Show memory data\n"
		"Format: d[b,w,d,q] address\n"
		"Example1: d[b,w,d,q] 401000\n"
		"Example2: d[b,w,d,q] ESI+8\n"
		"Example3: d[b,w,d,q] +\n"
		"Example4: d[b,w,d,q] -\n"
		"Example5: d[b,w,d,q] + offset\n"
		"Example5: d[b,w,d,q] - offset\n")},
	{ WSTR("db"), WSTR("Show memory data (BYTE)"), d_command, 
		WSTR("d,db,dw,dd,dq: Show memory data\n"
		"Format: d[b,w,d,q] address\n"
		"Example1: d[b,w,d,q] 401000\n"
		"Example2: d[b,w,d,q] ESI+8\n"
		"Example3: d[b,w,d,q] +\n"
		"Example4: d[b,w,d,q] -\n"
		"Example5: d[b,w,d,q] + offset\n"
		"Example5: d[b,w,d,q] - offset\n")},
	{ WSTR("dw"), WSTR("Show memory data (WORD)"), d_command, 
		WSTR("d,db,dw,dd,dq: Show memory data\n"
		"Format: d[b,w,d,q] address\n"
		"Example1: d[b,w,d,q] 401000\n"
		"Example2: d[b,w,d,q] ESI+8\n"
		"Example3: d[b,w,d,q] +\n"
		"Example4: d[b,w,d,q] -\n"
		"Example5: d[b,w,d,q] + offset\n"
		"Example5: d[b,w,d,q] - offset\n")},
	{ WSTR("dd"), WSTR("Show memory data (DWORD)"), d_command, 
		WSTR("d,db,dw,dd,dq: Show memory data\n"
		"Format: d[b,w,d,q] address\n"
		"Example1: d[b,w,d,q] 401000\n"
		"Example2: d[b,w,d,q] ESI+8\n"
		"Example3: d[b,w,d,q] +\n"
		"Example4: d[b,w,d,q] -\n"
		"Example5: d[b,w,d,q] + offset\n"
		"Example5: d[b,w,d,q] - offset\n")},
	{ WSTR("dq"), WSTR("Show memory data (QWORD)"), d_command, 
		WSTR("d,db,dw,dd,dq: Show memory data\n"
		"Format: d[b,w,d,q] address\n"
		"Example1: d[b,w,d,q] 401000\n"
		"Example2: d[b,w,d,q] ESI+8\n"
		"Example3: d[b,w,d,q] +\n"
		"Example4: d[b,w,d,q] -\n"
		"Example5: d[b,w,d,q] + offset\n"
		"Example5: d[b,w,d,q] - offset\n")},

	{ WSTR("wc"), WSTR("focus code view (wc + to add new view)"), wc_command,
		WSTR("wc,wd: Code,Data Window command\n"
		"Format: wc[d] [+][-] [address][index_of_windows]\n"
		"Example1: wc[d] + eip+5\n"
		"Example2: wc[d] 1                   (focus code[data] window 1)\n"
		"Example3: wc[d] +                   (add new code[data] window)\n"
		"Example4: wc[d] -                   (remove current code[data] window)\n" )},
	{ WSTR("code"), WSTR("focus code view (code + to add new view)"), wc_command,
		WSTR("code: Code Window command\n"
		"Format: code [+][-] [address][index_of_windows]\n"
		"Example1: code + eip+5\n"
		"Example2: code 1                   (focus code window 1)\n"
		"Example3: code +                   (add new code window)\n"
		"Example4: code -                   (remove current code window)\n")},

	{ WSTR("wd"), WSTR("focus data view (wc + to add new view)"), wd_command,
		WSTR("wd,wc: Data,Code Window command\n"
		"Format: wd[c] [+][-] [address][index_of_windows]\n"
		"Example1: wd[c] + eip+5\n"
		"Example2: wd[c] 1                   (focus data[code] window 1)\n"
		"Example3: wd[c] +                   (add new data[code] window)\n"
		"Example4: wd[c] -                   (remove current data[code] window)\n" )},
	{ WSTR("data"), WSTR("focus data view (data + to add new view)"), wd_command,
		WSTR("data: Data Window command\n"
		"Format: data [+][-] [address][index_of_windows]\n"
		"Example1: data + eip+5\n"
		"Example2: data 1                   (focus data window 1)\n"
		"Example3: data +                   (add new data window)\n"
		"Example4: data -                   (remove current data window)\n")},

	{ WSTR("ws"), WSTR("focus stack watch list (HotKey ALT+S)"), ws_command, 
	0},
	{ WSTR("ww"), WSTR("focus watch list (HotKey ALT+S)"), ww_command, 
	0},
	{ WSTR("wr"), WSTR("focus register list (HotKey ALT+R)"), wr_command, 
	0},
	{ WSTR("wf"), WSTR("focus FPU register list (HotKey ALT+F)"), wf_command, 
	0},
	{ WSTR("wx"), WSTR("focus XMM register list (HotKey ALT+X)"), wx_command, 
	0},
	{ WSTR("dex"), WSTR("switch data view float address mode"), dex_command, 
	0},
	{ WSTR("isf"), WSTR("Identify standard C(++) library function"), isf_command, 
		WSTR("isf: Identify standard C(++) library function\n"
		"Format: isf address    (if address is a standard C(++) library function and symbols will be inserted automaticly)\n"
		"Example1: isf 401000\n")},
	{0},
};

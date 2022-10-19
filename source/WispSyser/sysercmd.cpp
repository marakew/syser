
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "syserdefine.hpp"
#include "../Code/time.hpp"

int bc_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;

	if (argc < 2)
	{
		WCHAR Buffer[64];
		TSPrintf(Buffer, WSTR("help %s"), *argv);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
		return -1;
	}

	if (argv[1][0] == '*')
	{
		gpSyser->m_pDebugger->DelAllCodeBP(0, 0x100);
		gpSyser->m_pDebugger->DelAllDataBP(0, 0x100);
		gpSyser->m_SyserUI.m_ModuleBPMap.clear();
		gpSyser->m_SyserUI.m_ModuleLoadBPMap.clear();
#if 1
		for (map<unsigned long, TList<CSyser::BPR>>::IT it = gpSyser->m_ProcsBPR.begin();
			it != gpSyser->m_ProcsBPR.end(); ++it)
		{
			it->second.Clear();
		}
		gpSyser->m_ProcsBPR.clear();
#endif
		gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
		gpSyser->m_MainFrame.Update((WISP_RECT*)0);
		return 0;
	}

	ULONG_PTR Result;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Result))
	{
		char szBuffer[MAX_FN_LEN];
		UnicodeToAnsi(argv[1], szBuffer, MAX_FN_LEN);
		if (gpSyser->m_SyserUI.m_ModuleLoadBPMap.erase(szBuffer)>0)
		return 0;
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Input error!\n"));
		return 1;
	}

	if (Result > 0x1000)
	{
		int Count = 0;
		Count += gpSyser->m_SyserUI.RemoveBP(Result);
		Count += gpSyser->m_pDebugger->DelDataBP(Result, 0, 0x300);
		if (Count == 0)
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: breakpoint isn't exist !\n"));
			return 0;
		}
		gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
		gpSyser->m_MainFrame.Update((WISP_RECT*)0);
		return 0;
	}
	int Index = 0;
{
	map<unsigned long long, BREAK_POINT>::IT it = gpSyser->m_pDebugger->m_pCodeBPMap->begin();
	//while (it != gpSyser->m_pDebugger->m_pCodeBPMap->end())
	while (it)
	{
		if (!(it->second.Type & 0x300) && !it->second.pModule)
		{
			if (Index == Result)
			{
				gpSyser->m_pDebugger->DelCodeBP(it->second);
				gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
				gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
				gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
				gpSyser->m_MainFrame.Update((WISP_RECT*)0);
				return 0;
			}
			++Index;
		}
		++it;
	}
}
{
	map<ULONG_PTR, BREAK_POINT::MODULE>::IT it = gpSyser->m_SyserUI.m_ModuleBPMap.begin();
	//while (it != gpSyser->m_SyserUI.m_ModuleBPMap.end())
	while (it)
	{
		if (Index == Result)
		{
			gpSyser->m_SyserUI.RemoveBP(it->first);
			gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
			gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
			gpSyser->m_MainFrame.Update((WISP_RECT*)0);
			return 0;
		}
		++Index;
		++it;
	}
}
{
	TListIter<BREAK_POINT> It = gpSyser->m_pDebugger->m_pDataBPList->Begin();
	while (It != gpSyser->m_pDebugger->m_pDataBPList->End())
	{
		if (!(It->Type & 0x300))
		{
			if (Index == Result)
			{
				gpSyser->m_pDebugger->DelDataBP(*It);
				gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
				gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
				gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
				gpSyser->m_MainFrame.Update((WISP_RECT*)0);
				return 0;
			}
			++Index;			
		}
		++It;
	}
}
{
	map<TAnsiNoCaseString,MODULE_LOAD>::IT it = gpSyser->m_SyserUI.m_ModuleLoadBPMap.begin();
	//while (it != gpSyser->m_SyserUI.m_ModuleLoadBPMap.end())
	while (it)
	{
		if (Index == Result)
		{
			gpSyser->m_SyserUI.m_ModuleLoadBPMap.erase(it);
			return 0;
		}
		++it;
	}
}
#if 0
{
	for (map<unsigned long,TList<CSyser::BPR>>::IT it = gpSyser->m_ProcsBPR.begin();
		it != gpSyser->m_ProcsBPR.end(); ++it)
	{
		if (gpSyser->m_SysInfo.GetProcess(it->first))
		{
			TListIter<CSyser::BPR> It = it->second.Begin();
			while (It != it->second.End())
			{
				if (Index == Result)
				{
					it->second.Remove(It);
					if (!it->second.Size())
						gpSyser->m_ProcsBPR.erase(it);
					return 0;
				}
				++It;
				++Index;
			}
		}
	}
}
#endif
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: breakpoint isn't exist !\n"));
	return 0;
}

void ShowCodeBP(int *Index, BREAK_POINT *BreakPoint, bool ALL)
{
	if (ALL || (!(BreakPoint->Type & 0x300) && !BreakPoint->pModule))
	{
		if (!*Index)
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Index OwnerProcess   Type     Address    Name\n"));
		WCHAR Buffer[128];
		TStrCpy(Buffer, "Code");

		WCHAR Owner[128];
		AnsiToUnicode(BreakPoint->Owner.operator const char*(), Owner, lenof(Owner));
		WCHAR Name[128];
		AnsiToUnicode(BreakPoint->Name.operator const char*(), Name, lenof(Name));

		if (ALL)
		{
			typedef _Tmap_pair<unsigned long long, BREAK_POINT> PBP;
			PBP *p = CONTAINING_RECORD_MY1(BreakPoint, PBP, second);
			//unsigned int it; //BreakPoint-8 //TODO
			if (BreakPoint->State)
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%-5X %<B>%-14s %<7>%-8s %<4>" F0ADDR "X   %<1>%s " F0ADDR "X\n"), *Index, Owner, Buffer, BreakPoint->Address, Name, p->first);
			else	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<C>%-5X %-14s %-8s " F0ADDR "X   %s  " F0ADDR "X\n"), *Index, Owner, Buffer, BreakPoint->Address, Name, p->first);
		} else
		{
			if (BreakPoint->State)
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%-5X %<B>%-14s %<7>%-8s %<4>" F0ADDR "X   %<1>%s\n"), *Index, Owner, Buffer, BreakPoint->Address, Name);
			else	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<C>%-5X %-14s %-8s " F0ADDR "X   %s\n"), *Index, Owner, Buffer, BreakPoint->Address, Name);
		}
		++*Index;
	}
}

void ShowModuleBP(int *Index, BREAK_POINT::MODULE *BreakPoint, bool ALL)
{
	if (!*Index)
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Index OwnerProcess   Type     Address    Name\n"));
	WCHAR Buffer[MAX_FN_LEN];
	AnsiToUnicode(BreakPoint->Name.operator const char*(), Buffer, lenof(Buffer));
	typedef _Tmap_pair<ULONG_PTR, BREAK_POINT::MODULE> PBP;
	PBP *p = CONTAINING_RECORD_MY1(BreakPoint, PBP, second);
	//unsigned int it; //BreakPoint-4 //TODO
	if (BreakPoint->State)
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%-5X %<B><All Process>  %<7>Code     %<4>" F0ADDR "X   %<1>%s\n"), *Index, p->first, Buffer);
	else	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<C>%-5X <All Process>  Code     " F0ADDR "X   %s\n"), *Index, p->first, Buffer);
	++*Index;
}

void ShowDataBP(int *Index, BREAK_POINT *BreakPoint, bool ALL)
{
	if (ALL || !(BreakPoint->Type & 0x300))
	{
		if (!*Index)
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Index OwnerProcess   Type     Address    Name\n"));
		WCHAR Buffer[128];
		TStrCpy(Buffer, "Data");
		TStrCat(Buffer, BreakPoint->TypeName.operator const char*());
		WCHAR Owner[128];
		AnsiToUnicode(BreakPoint->Owner.operator const char*(), Owner, lenof(Owner));
		WCHAR Name[128];
		AnsiToUnicode(BreakPoint->Name.operator const char*(), Name, lenof(Name));

		if (BreakPoint->State)
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%-5X %<B>%-14s %<7>%-8s %<4>" F0ADDR "X   %<1>%s\n"), *Index, Owner, Buffer, BreakPoint->Address, Name);
		else	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<C>%-5X %-14s %-8s " F0ADDR "X   %s\n"), *Index, Owner, Buffer, BreakPoint->Address, Name);
		++*Index;
	}
}

void ShowModuleLoadBP(int *Index, MODULE_LOAD *BreakPoint, const char *Name, bool ALL)
{
	if (ALL || !(BreakPoint->Type & 0x300))
	{
		if (!*Index) 
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Index OwnerProcess   Type     Address    Name\n"));
		WCHAR Buffer[MAX_FN_LEN];
		AnsiToUnicode(Name, Buffer, lenof(Buffer));
		if (BreakPoint->State)
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%-5X %<B><All Process>  %<7>BPLoad   %<4>EntryPoint %<1>%s\n"), *Index, Buffer);
		else	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<C>%-5X <All Process>  BPLoad   EntryPoint %s\n"), *Index, Buffer);
	}
}

void ShowProcBP(int *Index, CSyser::BPR *BreakPoint, const char *Name, bool ALL)
{
	if (!*Index)
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Index OwnerProcess   Type     Address    Name\n"));
	if (BreakPoint->State)
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%-5X %<B>%-14s %<7>%-8s %<4>" F0ADDR "X-" F0ADDR "X\n"), *Index, Name, WSTR("BMR"), BreakPoint->Address, BreakPoint->Address + BreakPoint->Size);
	else	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<C>-5X %<B>%-14s %-8s " F0ADDR "X-" F0ADDR "X\n"), *Index, Name, WSTR("BMR"), BreakPoint->Address, BreakPoint->Address + BreakPoint->Size);
}

int bl_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	bool ALL = argc > 1 && argv[1][0] == '*';
	int Count = 0;
{
	for (map<unsigned long long, BREAK_POINT>::IT it = gpSyser->m_pDebugger->m_pCodeBPMap->begin();
		it != gpSyser->m_pDebugger->m_pCodeBPMap->end(); ++it)
	{
		ShowCodeBP(&Count, &it->second, ALL);
	}
}
{
	map<ULONG_PTR, BREAK_POINT::MODULE>::IT it = gpSyser->m_SyserUI.m_ModuleBPMap.begin();
	//while (it != gpSyser->m_SyserUI.m_ModuleBPMap.end())
	while (it)
	{
		ShowModuleBP(&Count, &it->second, ALL);
		++it;
	}
}
{
	for (TListIter<BREAK_POINT> It = gpSyser->m_pDebugger->m_pDataBPList->Begin();
		It != gpSyser->m_pDebugger->m_pDataBPList->End(); ++It)
	{
		ShowDataBP(&Count, &*It, ALL);
	}
}
{
	for (map<TAnsiNoCaseString,MODULE_LOAD>::IT it = gpSyser->m_SyserUI.m_ModuleLoadBPMap.begin();
		it != gpSyser->m_SyserUI.m_ModuleLoadBPMap.end(); ++it)
	{
		ShowModuleLoadBP(&Count, &it->second, it->first.operator const char*(), ALL);
	}
}
#if 0
{
	for (map<unsigned long,TList<CSyser::BPR>>::IT it = gpSyser->m_ProcsBPR.begin();
		it != gpSyser->m_ProcsBPR.end(); ++it)
	{
		CDbgProcess *pDbgProc = gpSyser->m_SysInfo.GetProcess(it->first);
		if (pDbgProc)
		{
			for (TListIter<CSyser::BPR> It = It->second.Begin();
				It != It->second.End(); ++It)
			{
				ShowProcBP(&Count, &*It, pDbgProc->m_ProcessName.operator const char*(), ALL);
			}
		}
	}
}
#endif
	if (Count == 0)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Breakpoint doesn't exist!\n"));
	}
	return 0;
}

int bd_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;

	if (argc < 2)
	{
		WCHAR Buffer[64];
		TSPrintf(Buffer, WSTR("help %s"), *argv);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
		return -1;
	}

	if (argv[1][0] == '*')
	{
		gpSyser->m_pDebugger->DisableAllCodeBP(0, 0x300);
		gpSyser->m_pDebugger->DisableAllDataBP(0, 0x300);
		gpSyser->m_SyserUI.EnableModuleOnloadBP(false);
		gpSyser->m_SyserUI.EnableAllModuleBP(false);

		gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
		gpSyser->m_MainFrame.Update((WISP_RECT*)0);
		return 0;
	}

	ULONG_PTR Result;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Result))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Input error!\n"));
		return 1;
	}

	if (Result > 0x1000)
	{
		int Count = 0;
		Count += gpSyser->m_pDebugger->DisableCodeBP(Result, 0, 0x300);
		Count += gpSyser->m_pDebugger->DisableDataBP(Result, 0, 0x300);
		Count += gpSyser->m_SyserUI.EnableModuleBP(Result, false);
		if (Count == 0)
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: breakpoint isn't exist !\n"));
			return 0;
		}
		gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
		gpSyser->m_MainFrame.Update((WISP_RECT*)0);
		return 0;
	}
	int Index = 0;
{
	map<unsigned long long, BREAK_POINT>::IT it = gpSyser->m_pDebugger->m_pCodeBPMap->begin();
	//while (it != gpSyser->m_pDebugger->m_pCodeBPMap->end())
	while (it)
	{
		if (!(it->second.Type & 0x300) && !it->second.pModule)
		{
			if (Index == Result)
			{
				gpSyser->m_pDebugger->DisableCodeBP(it->second);
				gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
				gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
				gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
				gpSyser->m_MainFrame.Update((WISP_RECT*)0);
				return 0;
			}
			++Index;
		}
		++it;
	}
}
{
	map<ULONG_PTR, BREAK_POINT::MODULE>::IT it = gpSyser->m_SyserUI.m_ModuleBPMap.begin();
	//while (it != gpSyser->m_SyserUI.m_ModuleBPMap.end())
	while (it)
	{
		if (Index == Result)
		{
			gpSyser->m_SyserUI.EnableModuleBP(it->first, false);
			gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
			gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
			gpSyser->m_MainFrame.Update((WISP_RECT*)0);
			return 0;
		}
		++Index;
		++it;
	}
}
{
	TListIter<BREAK_POINT> It = gpSyser->m_pDebugger->m_pDataBPList->Begin();
	while (It != gpSyser->m_pDebugger->m_pDataBPList->End())
	{
		if (!(It->Type & 0x300))
		{
			if (Index == Result)
			{
				gpSyser->m_pDebugger->DisableDataBP(*It);
				gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
				gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
				gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
				gpSyser->m_MainFrame.Update((WISP_RECT*)0);
				return 0;
			}
			++Index;			
		}
		++It;
	}
}
{
	map<TAnsiNoCaseString,MODULE_LOAD>::IT it = gpSyser->m_SyserUI.m_ModuleLoadBPMap.begin();
	//while (it != gpSyser->m_SyserUI.m_ModuleLoadBPMap.end())
	while (it)
	{
		if (!(it->second.Type & 0x300))
		{
			if (Index == Result)
			{
				it->second.State = 0; //??? BP_STATE_DISABLE
				return 0;
			}
			++Index;
		}
		++it;
	}
}
#if 0
{
	for (map<unsigned long,TList<CSyser::BPR>>::IT it = gpSyser->m_ProcsBPR.begin();
		it != gpSyser->m_ProcsBPR.end(); ++it)
	{
		if (gpSyser->m_SysInfo.GetProcess(it->first))
		{
			for (TListIter<CSyser::BPR> It = it->second.Begin();
				It != it->second.End(); ++It)
			{
				if (Index == Result)
				{
					it->second.State = 0; //??? BP_STATE_DISABLE
					return 0;
				}
				++Index;
			}
		}
	}
}
#endif
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: breakpoint isn't exist !\n"));
	return 0;
}

int be_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;

	if (argc < 2)
	{
		WCHAR Buffer[64];
		TSPrintf(Buffer, WSTR("help %s"), *argv);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
		return -1;
	}

	if (argv[1][0] == '*')
	{
		gpSyser->m_pDebugger->EnableAllCodeBP(0, 0x300);
		gpSyser->m_pDebugger->EnableAllDataBP(0, 0x300);
		gpSyser->m_SyserUI.EnableModuleOnloadBP(true);
		gpSyser->m_SyserUI.EnableAllModuleBP(true);

		gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
		gpSyser->m_MainFrame.Update((WISP_RECT*)0);
		return 0;
	}

	ULONG_PTR Result;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Result))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Input error!\n"));
		return 1;
	}

	if (Result > 0x1000)
	{
		int Count = 0;
		Count += gpSyser->m_pDebugger->EnableCodeBP(Result, 0, 0x300);
		Count += gpSyser->m_pDebugger->EnableDataBP(Result, 0, 0x300);
		Count += gpSyser->m_SyserUI.EnableModuleBP(Result, true);
		if (Count == 0)
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: breakpoint isn't exist !\n"));
			return 0;
		}
		gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
		gpSyser->m_MainFrame.Update((WISP_RECT*)0);
		return 0;
	}
	int Index = 0;
{
	map<unsigned long long, BREAK_POINT>::IT it = gpSyser->m_pDebugger->m_pCodeBPMap->begin();
	//while (it != gpSyser->m_pDebugger->m_pCodeBPMap->end())
	while (it)
	{
		if (!(it->second.Type & 0x300) && !it->second.pModule)
		{
			if (Index == Result)
			{
				gpSyser->m_pDebugger->EnableCodeBP(it->second);
				gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
				gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
				gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
				gpSyser->m_MainFrame.Update((WISP_RECT*)0);
				return 0;
			}
			++Index;
		}
		++it;
	}
}
{
	map<ULONG_PTR, BREAK_POINT::MODULE>::IT it = gpSyser->m_SyserUI.m_ModuleBPMap.begin();
	//while (it != gpSyser->m_SyserUI.m_ModuleBPMap.end())
	while (it)
	{
		if (Index == Result)
		{
			gpSyser->m_SyserUI.EnableModuleBP(it->first, true);
			gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
			gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
			gpSyser->m_MainFrame.Update((WISP_RECT*)0);
			return 0;
		}
		++Index;
		++it;
	}
}
{
	TListIter<BREAK_POINT> It = gpSyser->m_pDebugger->m_pDataBPList->Begin();
	while (It != gpSyser->m_pDebugger->m_pDataBPList->End())
	{
		if (!(It->Type & 0x300))
		{
			if (Index == Result)
			{
				gpSyser->m_pDebugger->EnableDataBP(*It);
				gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
				gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
				gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
				gpSyser->m_MainFrame.Update((WISP_RECT*)0);
				return 0;
			}
			++Index;			
		}
		++It;
	}
}
{
	map<TAnsiNoCaseString,MODULE_LOAD>::IT it = gpSyser->m_SyserUI.m_ModuleLoadBPMap.begin();
	//while (it != gpSyser->m_SyserUI.m_ModuleLoadBPMap.end())
	while (it)
	{
		if (!(it->second.Type & 0x300))
		{
			if (Index == Result)
			{
				it->second.State = 1; //??? BP_STATE_ENABLE
				return 0;
			}
			++Index;
		}
		++it;
	}
}
#if 0
{
	for (map<unsigned long,TList<CSyser::BPR>>::IT it = gpSyser->m_ProcsBPR.begin();
		it != gpSyser->m_ProcsBPR.end(); ++it)
	{
		if (gpSyser->m_SysInfo.GetProcess(it->first))
		{
			for (TListIter<CSyser::BPR> It = it->second.Begin();
				It != it->second.End(); ++It)
			{
				if (Index == Result)
				{
					it->second.State = 1; //??? BP_STATE_ENABLE
					return 0;
				}
				++Index;
			}
		}
	}
}
#endif
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: breakpoint isn't exist !\n"));
	return 0;
}

int GetCondition(int argc, const WCHAR **argv, const WCHAR *szCmd, char *Condition, int MaxLen)
{
	int n = 2;
	while (n<argc)
	{
		if (!TStrICmp(argv[n++], "if"))
			break;
	}

	if (n<argc)
	{
		const WCHAR *Start = &szCmd[argv[n] - argv[0]];

		while (n<argc)
		{
			if (!TStrICmp(argv[n], "do"))
				break;
			++n;
		}

		if (n<argc)
		{
			const WCHAR *End = &szCmd[argv[n] - argv[0]];
			while (End[-1] == 0x20 && End>Start)
				--End;
			int Len = End - Start + 1;
			if (Len >= MaxLen)
				Len = MaxLen;
			MaxLen = Len;
		}
		return TStrCpyLimit(Condition, Start, MaxLen);
	}
	*Condition = 0;
	return 0;
}

int GetCommand(int argc, const WCHAR **argv, const WCHAR *szCmd, char *Command, int MaxLen)
{
	int n = 2;
	while (n<argc)
	{
		if (!TStrICmp(argv[n++], "do"))
			break;
	}
	if (n<argc)
	{
		const WCHAR *Start = &szCmd[argv[n] - argv[0]];
		return TStrCpyLimit(Command, Start, MaxLen);
	}
	*Command = 0;
	return 0;
}

int bpx_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;

	if (argc < 2)
	{
		WCHAR Buffer[64];
		TSPrintf(Buffer, WSTR("help %s"), *argv);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
		return 1;
	}

	char Name[256];
	UnicodeToAnsi(argv[1], Name, lenof(Name));

	char Condition[256];
	if (GetCondition(argc, argv, szCmd, Condition, lenof(Condition))>0 &&
		!gpSyser->m_SyserUI.CalcExp(Condition, 0))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Invalid condition string !\n"));
		return -1;
	}

	char Command[256];
	GetCommand(argc, argv, szCmd, Command, lenof(Command));

	ULONG_PTR Result;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Result))
	{
//		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Counldn't find symbol %<F>%s%<1> in [%<B>%s%<1>] process space\n"), argv[1], gpSyser->m_pDebugger->m_ProcessName.operator const char*());
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Counldn't find symbol %<F>%s\n"), argv[1]);
		return -1; //FIX
	} else
	if (!gpSyser->m_SyserUI.InsertBP(Result, Name, Condition, Command))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Fail to set code breakpoint at " F0ADDR "X!\n"), Result);
		return -1; //FIX
	} else
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Set code break point at %<B>" F0ADDR "X%<1>!\n"), Result);
	}
	gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
	gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
	gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
	gpSyser->m_MainFrame.Update((WISP_RECT*)0);
	return 0;
}

int bpload_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc == 1)
	{
		for (map<TAnsiNoCaseString,MODULE_LOAD>::IT it = gpSyser->m_SyserUI.m_ModuleLoadBPMap.begin();
			it != gpSyser->m_SyserUI.m_ModuleLoadBPMap.end(); ++it)
		{
			WCHAR szModule[MAX_FN_LEN];
			AnsiToUnicode(it->first.operator const char*(), szModule, lenof(szModule));
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%s\n"), szModule);
		}
	} else
	if (argc == 2)
	{
		char szModule[MAX_FN_LEN];
		UnicodeToAnsi(argv[1], szModule, MAX_FN_LEN);
		gpSyser->m_SyserUI.InsertModuleOnloadBP(szModule, 0, 1); //Type, BP_STATE_ENABLE
	}
	return 0;
}

int bpm_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
//	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
//		return 1;

	if (argc < 2)
	{
		WCHAR Buffer[64];
		TSPrintf(Buffer, WSTR("help %s"), *argv);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
		return 1;
	}

	int Size = 0;

	switch (argv[0][3])
	{
	case 'B':
	case 'b':
		Size = 1;
		break;
	case 'D':
	case 'd':
		Size = 4;
		break;
	case 'W':
	case 'w':
		Size = 2;
		break;
	default:
		Size = 0;
		break;
	}

	ULONG_PTR Result;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Result))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Address!\n"));
		return -1;
	}

	int Type = 0xC;
	if (argc > 2)
	{
		if (!TStrICmp(argv[2], "if"));
		else
		if (!TStrICmp(argv[2], "do"));
		else
		if (!TStrICmp(argv[2], "rw"));
		else
		if (!TStrICmp(argv[2], "w"))
			Type = 4;
		else
		if (!TStrICmp(argv[2], "x"))
			Type = 2;
		else
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Invalid access type !\n"));
			return -1;
		}
	}

	char Condition[256];
	if (GetCondition(argc, argv, szCmd, Condition, lenof(Condition))>0 &&
		!gpSyser->m_SyserUI.CalcExp(Condition, 0))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Invalid condition string !\n"));
		return -1;
	}

	char Command[256];
	GetCommand(argc, argv, szCmd, Command, lenof(Command));

	BREAK_POINT *BreakPoint = gpSyser->m_pDebugger->SetDataBP(Result, Type, BP_STATE_ENABLE, Size);
	if (!BreakPoint)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Fail to set data breakpoint at " F0ADDR "X!\n"), Result);
		return -1; //FIX
	} else
	{
		if (Condition[0])
			BreakPoint->Condition = Condition;
		if (Command[0])
			BreakPoint->Command = Command;

		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Set data break point at %<B>" F0ADDR "X%<1>!\n"), Result);
	}

	gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
	gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
	gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
	gpSyser->m_MainFrame.Update((WISP_RECT*)0);
	return 0;
}

int bpio_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;

	if (argc < 2)
	{
		WCHAR Buffer[64];
		TSPrintf(Buffer, WSTR("help %s"), *argv);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
		return 1;
	}

	ULONG_PTR Result;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Result) || Result > 0xFFFF)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Address!\n"));
		return -1;
	}

	char Condition[256];
	if (GetCondition(argc, argv, szCmd, Condition, lenof(Condition))>0 &&
		!gpSyser->m_SyserUI.CalcExp(Condition, 0))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Invalid condition string !\n"));
		return -1;
	}

	char Command[256];
	GetCommand(argc, argv, szCmd, Command, lenof(Command));

	BREAK_POINT *BreakPoint = gpSyser->m_pDebugger->SetDataBP(Result, 0x30, BP_STATE_ENABLE, 0);
	if (!BreakPoint)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Fail to set IO breakpoint at " F0ADDR "X!\n"), Result);
		return -1; //FIX
	} else
	{
		if (Condition[0])
			BreakPoint->Condition = Condition;
		if (Command[0])
			BreakPoint->Command = Command;

		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Set IO break point at %<B>" F0ADDR "X%<1>!\n"), Result);
	}

	gpSyser->m_MainFrame.m_BreakPointForm.CreateShow();
	gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.UpdateContext();
	gpSyser->m_MainFrame.m_SourceDebugFrameWnd.UpdateContext(false);
	gpSyser->m_MainFrame.Update((WISP_RECT*)0);
	return 0;
}

int x_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (pSyser->m_pDebugger)
		pSyser->m_pDebugger->ContinueDebug(true);
	return 0;
}

int g_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc != 2)
	{
		WCHAR Buffer[64];
		TSPrintf(Buffer, WSTR("help %s"), *argv);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
		return -1;
	}

	ULONG_PTR Address;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Address))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Address!\n"));
		return -1;
	}

	if (!gpSyser->m_pDebugger->SetCodeBP(Address, 0x200, BP_STATE_ENABLE))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Fail to set breakpoint at " F0ADDR "X\n"), Address);
		return -1;
	}

	gpSyser->m_pDebugger->ContinueDebug(true);
	return 0;
}

bool IsCode(unsigned char *CodeBuf, ULONG_PTR CIP, ULONG_PTR *pLen)
{
#if 1
	char pasm[200];
	DIS_CPU DisCPU;
	_INSTRUCTION_INFORMATION DasmInstr;

	DasmInstr.pasm = pasm;
	DasmInstr.U_x1 = 6;
	DasmInstr.U_x2 = 12;

	unsigned long CodeLen = Disassembly(&DasmInstr, CodeBuf, CIP, gpSyser->m_pDebugger->m_CodeMode, &DisCPU);
	if (CodeLen)
	{
		if (DasmInstr.OpCode == C_PUSHF ||
		    DasmInstr.OpCode == C_POPF)
		{
			*pLen = CodeLen;
			return true;
		}

		if ((!TStrICmp(DasmInstr.Name, "mov") &&
			DasmInstr.op[0].mode == OP_Segment &&
			!TStrICmp(DasmInstr.op[0].string, "ss")) ||

		    (!TStrICmp(DasmInstr.Name, "pop") &&
			DasmInstr.op[0].mode == OP_Segment &&
			!TStrICmp(DasmInstr.op[0].string, "ss")))
		{
			if (gpSyser->m_pDebugger->ReadMemory(CodeLen + *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP, CodeBuf, 16) != 16)
				return false;

			memset(&DasmInstr, 0, sizeof(DasmInstr));
			unsigned long NextCodeLen = Disassembly(&DasmInstr, CodeBuf, CodeLen + CIP, gpSyser->m_pDebugger->m_CodeMode, &DisCPU);
			if (NextCodeLen)
			{
				if (DasmInstr.OpCode == C_PUSHF ||
				    DasmInstr.OpCode == C_POPF)
				{
					*pLen = CodeLen + NextCodeLen;
					return true;
				}
			}
		}
		return false;
	}
#endif
	if (CodeBuf[0] == 0x66)
	{
		if (CodeBuf[1] == 0x9C || CodeBuf[1] == 0x9D)
		{
			*pLen = 2;
			return true;
		}
	} else
	if (CodeBuf[0] == 0x9C || CodeBuf[0] == 0x9D)
	{
		*pLen = 1;
		return true;
	}
	return false;
}

int t_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;

	if (argc > 1)
	{
		unsigned long Step;
		if (USHexStrToNum(argv[1], &Step))
		{
			gpSyser->m_SyserUI.m_DebugInterface.Trace(Step);
		} else
		{
			ULONG_PTR Test;
			if (!gpSyser->m_SyserUI.CalcExp(&szCmd[argv[1] - argv[0]], &Test))
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Input error!\n"));
				return -1;
			}
			char Condition[256];
			UnicodeToAnsi(&szCmd[argv[1] - argv[0]], Condition, 256);
			gpSyser->m_SyserUI.m_DebugInterface.Trace(Condition);
		}
	} else
	if (gpSyser->m_SyserUI.m_DebugInterface.m_State == 0) //CONTINUE
	{
		gpSyser->m_SyserUI.m_DebugInterface.Trace(1);
	}
	ULONG_PTR Addr = 0;
	unsigned char CodeBuf[MAX_INSTR_LEN];
	if (!gpSyser->m_pDebugger->ReadMemory(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP, CodeBuf, sizeof(CodeBuf)) ||
		!IsCode(CodeBuf, *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP, &Addr))
	{
		gpSyser->m_pDebugger->SetSingleStep();
		gpSyser->m_pDebugger->ContinueDebug(false);
		return 0;
	}

	Addr += *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP;
	if (!gpSyser->m_pDebugger->SetCodeBP(Addr, 0x200, BP_STATE_ENABLE))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Fail to set Debug BP at " F0ADDR "X\n"), Addr);
		return 0; //???
	}
	gpSyser->m_pDebugger->ContinueDebug(false);
	return 0;
}

int st_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;

	CSDSModule *pSDSMod = gpSyser->m_SyserUI.FindSDSModule(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP);
	if (!pSDSMod)
		return -1;
	//unsigned long
	ULONG_PTR Addr = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP - pSDSMod->m_pDbgModule->m_ImageBase;
	ULONG_PTR ImageBase = pSDSMod->m_pDbgModule->m_ImageBase;
	TList<FilesLN*> ListFilesLN;
	if (!pSDSMod->GetFilesLN(Addr, &ListFilesLN))
		return -1;
	FilesLN *pFilesLN = *ListFilesLN[0];
	CSourceCodeWnd *pSrcWnd = gpSyser->m_MainFrame.m_SourceDebugFrameWnd.OpenSrcView(pSDSMod, pFilesLN->FileID, pFilesLN->lineNumber);
	if (!pSrcWnd)
		return -1;

	CListStringItem *Item = pSrcWnd->GetItem(pFilesLN->lineNumber-1, 0);
	if (!Item)
		return -1;

	if (Item->uStatus & 1)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("T"), 0);
		return 0;
	}
	unsigned long EndAddr = pFilesLN->address + pFilesLN->length;
	gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("T"), 0);
	gpSyser->m_SyserUI.m_DebugInterface.Trace(ImageBase + pFilesLN->address, ImageBase + EndAddr, 0);
	return 0;
}

int tb_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	t_command(1,argv,szCmd,pSyser);
	return 0;
}

int p_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;

	bool bRestoreScr = false;
	if (argc > 1)
	{
		unsigned long Step;
		if (USHexStrToNum(argv[1], &Step))
		{
			gpSyser->m_SyserUI.m_DebugInterface.Step(Step);
			bRestoreScr = true;
		} else
		if (!TStrICmp(argv[1], "ret"))
		{
			gpSyser->m_SyserUI.m_DebugInterface.Return(gpSyser->m_pDebugger);
			bRestoreScr = true;
		} else
		{
			ULONG_PTR Test;
			if (!gpSyser->m_SyserUI.CalcExp(&szCmd[argv[1] - argv[0]], &Test))
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Input error!\n"));
				return -1;
			}
			char Condition[256];
			UnicodeToAnsi(&szCmd[argv[1] - argv[0]], Condition, lenof(Condition));
			gpSyser->m_SyserUI.m_DebugInterface.Step(Condition);
		}
	} else
	if (gpSyser->m_SyserUI.m_DebugInterface.m_State == 0) //CONTINUE
	{
		gpSyser->m_SyserUI.m_DebugInterface.Trace(1);
	}

	if (gpSyser->m_SyserUI.m_SyserCodeDoc.IsProcessCode(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP))
	{
		ULONG_PTR Offset = gpSyser->m_SyserUI.m_SyserCodeDoc.InstrLen(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP, MAX_INSTR_LEN);
		Offset += *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP;
		gpSyser->m_pDebugger->SetCodeBP(Offset, 0x200, BP_STATE_ENABLE);
		gpSyser->m_pDebugger->ContinueDebug(bRestoreScr);
		return 0;
	}

	ULONG_PTR Addr = 0;
	unsigned char CodeBuf[MAX_INSTR_LEN];
	if (gpSyser->m_pDebugger->ReadMemory(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP, CodeBuf, sizeof(CodeBuf)) != sizeof(CodeBuf) ||
		!IsCode(CodeBuf, *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP, &Addr))
	{
		gpSyser->m_pDebugger->SetSingleStep();
		gpSyser->m_pDebugger->ContinueDebug(bRestoreScr);
		return 0;
	}

	Addr += *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP;
	if (!gpSyser->m_pDebugger->SetCodeBP(Addr, 0x200, BP_STATE_ENABLE))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Fail to set Debug BP at " F0ADDR "X\n"), Addr);
		return 0; //???
	}
	gpSyser->m_pDebugger->ContinueDebug(bRestoreScr);
	return 0;
}

int sp_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;

	CSDSModule *pSDSMod = gpSyser->m_SyserUI.FindSDSModule(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP);
	if (!pSDSMod)
		return -1;
	//unsigned long
	ULONG_PTR Addr = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP - pSDSMod->m_pDbgModule->m_ImageBase;
	ULONG_PTR ImageBase = pSDSMod->m_pDbgModule->m_ImageBase;
	TList<FilesLN*> ListFilesLN;
	if (!pSDSMod->GetFilesLN(Addr, &ListFilesLN))
		return -1;
	FilesLN *pFilesLN = *ListFilesLN[0];
	CSourceCodeWnd *pSrcWnd = gpSyser->m_MainFrame.m_SourceDebugFrameWnd.OpenSrcView(pSDSMod, pFilesLN->FileID, pFilesLN->lineNumber);
	if (!pSrcWnd)
		return -1;

	CListStringItem *Item = pSrcWnd->GetItem(pFilesLN->lineNumber-1, 0);
	if (!Item)
		return -1;

	if (Item->uStatus & 1)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("P"), 0);
		return 0;
	}
	unsigned long EndAddr = pFilesLN->address + pFilesLN->length;
	gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("P"), 0);
	gpSyser->m_SyserUI.m_DebugInterface.Step(ImageBase + pFilesLN->address, ImageBase + EndAddr, 0);
	return 0;
}

int skip_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;
	X86_CPU_REG_PTR CpuREGPtr;
	pSyser->m_pDebugger->GetX86RegPtr(&CpuREGPtr);
	*CpuREGPtr.pCIP += pSyser->m_SyserUI.m_SyserCodeDoc.InstrLen(*CpuREGPtr.pCIP, MAX_INSTR_LEN);
	pSyser->m_pDebugger->UpdateRegisters();
	gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
	return 0;
}

int stack_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;
#if 0
			gpSyser->m_pDebugger->SaveRegisters();
			for(ULONG nFrame = 0; nFrame < 256; nFrame++)
			{
				STACK_FRAME StackFrame;

				if (!gpSyser->m_pDebugger->GetStack(&StackFrame)) break;

		#if 1
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%2d from %016I64X   STACK %016I64X    FRAME %016I64X    RET %016I64X %s\n"),
					nFrame,
					StackFrame.AddrPC,//.Offset,
					StackFrame.AddrStack,//.Offset,
					StackFrame.AddrFrame,//.Offset,
					StackFrame.AddrReturn,//.Offset,
					WSTR(""));//name);
		#endif
				if (StackFrame.AddrPC == StackFrame.AddrReturn) break;
				if (StackFrame.AddrPC == 0) break;
				if (StackFrame.AddrReturn == 0) break;

				char szFuncName[64];
				ULONG_PTR Addr;
				if (gpSyser->m_SyserUI.m_SyserCodeDoc.GetCall(StackFrame.AddrReturn, szFuncName, &Addr))
				{
					if (!szFuncName[0])
					{
						CDbgModule *pDbgMod = gpSyser->m_pDebugger->m_DbgModuleList.FindModule(Addr);
						if (!pDbgMod)
						{
							if (gpSyser->m_pSysDebugger != gpSyser->m_pDebugger)
								pDbgMod = gpSyser->m_pSysDebugger->m_DbgModuleList.FindModule(Addr);
						}

						if (pDbgMod)
						{
							unsigned long Offset = Addr - pDbgMod->m_ImageBase;
							TSPrintf(szFuncName, "%s+0x%X", pDbgMod->m_FileTitle.operator const char*(), Offset);
						} else
						{
							TSPrintf(szFuncName, "unknown_function");
						}
					}
				#if 0
					TListIter<Frame> It = m_Frames.Append();
					It->Address = StackFrame.AddrFrame;//Address; //Frame
					It->Return = StackFrame.AddrReturn;
					It->FuncAddr = Addr;
					It->FuncName = szFuncName;
				#endif
				}
			}

	return 0;
#endif
	ULONG_PTR Addr;
	if (argc == 1)
	{
		Addr = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBP;
	} else
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Addr))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Parameter!\n"));
		return -1;
	}
	gpSyser->m_SyserUI.m_CallStack.UpdateContext(Addr);
	if (gpSyser->m_SyserUI.m_CallStack.m_Frames.Size())
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("CallReturn Frame    Function Name\n"));
	else	gpSyser->m_SyserUI.m_CallStack.UpdateContext(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP);

	if (gpSyser->m_SyserUI.m_CallStack.m_Frames.Size())
	{
		TListIter<CCallStack::Frame> It = gpSyser->m_SyserUI.m_CallStack.m_Frames.Begin();
		while (It != gpSyser->m_SyserUI.m_CallStack.m_Frames.End())
		{
			WCHAR szFunc[256];
			AnsiToUnicode(It->FuncName.operator const char*(), szFunc, lenof(szFunc));
			if (gpSyser->m_SyserUI.m_CallStack.m_HeadIt == It)
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<7>" F0ADDR "X   " F0ADDR "X %s\n"), It->Return, It->Address, szFunc);
			else	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("" F0ADDR "X   " F0ADDR "X %s\n"), It->Return, It->Address, szFunc);
			++It;
		}
	} else
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("CallReturn Frame    Function Name  count=0\n"));
	}
	return 0;
}

int u_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;

	if (argc == 1)
	{
		ULONG_PTR Addr = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP;
		if (argv[0][0] != '.')
		{
			CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
			int n = pWnd->GetItemCount(0, true);
			CListStringItem *Item = pWnd->GetItem(n, 0);
			Addr = pWnd->GetItemValue(Item, 1);
		}
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.SetActiveAddress(Addr, true);
		return 0;
	} else
	if (argc == 2)
	{
		if (!TStrICmp(argv[1], WSTR("-")))
		{
			CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
			pWnd->OnEventPrevPos(0);
			return 0;
		}

		if (!TStrICmp(argv[1], WSTR("+")))
		{
			CCodeView *pWnd = (CCodeView *)gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd();
			pWnd->OnEventNextPos(0);
			return 0;
		}
		ULONG_PTR Address;
		if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Address))
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : %<3>Input error !\n"));
			return 1;
		}
		gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.SetActiveAddress(Address, true);
		return 0;
	} else
	if (argc == 3)
	{
		if (!TStrICmp(argv[1], WSTR("-")))
		{
			ULONG_PTR Address;
			if (!gpSyser->m_SyserUI.CalcExp(argv[2], &Address))
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : %<3>Input error !\n"));
				return 1;
			}
			if (!gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd())
				return 1;
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.SetActiveAddress(
			pSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveAddress()-Address, true);
			return 0;
		}

		if (!TStrICmp(argv[1], WSTR("+")))
		{
			ULONG_PTR Address;
			if (!gpSyser->m_SyserUI.CalcExp(argv[2], &Address))
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : %<3>Input error !\n"));
				return 1;
			}
			if (!gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveWnd())
				return 1;
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.SetActiveAddress(
			pSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.GetActiveAddress()+Address, true);
			return 0;
		}

		if (!TStrICmp(argv[1], WSTR("-a")))
		{
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.InsertView(0);
			ULONG_PTR Address;
			if (!gpSyser->m_SyserUI.CalcExp(argv[2], &Address))
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : %<3>Input error !\n"));
				return 1;
			}
			gpSyser->m_MainFrame.m_SystemExplorer.m_MultiCodeView.SetActiveAddress(Address, true);
			return 0;
		}
	}
	return 1;
}

int src_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	ULONG_PTR Addr;
	if (argc == 1)
	{
		Addr = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP;
	} else
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Addr))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Address!\n"));
		return 1;
	}
	CSDSModule *pSDSMod = gpSyser->m_SyserUI.FindSDSModule(Addr);
	if (!pSDSMod)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Couldn't find source code in SDS Module list !\n"));
		return -1;
	}
	gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_SDSModuleList.ExpandSDSModule(pSDSMod);
	TList<FilesLN*> ListFilesLN;
	if (pSDSMod->GetFilesLN(Addr, &ListFilesLN))
	{
		gpSyser->m_MainFrame.m_MainTabWnd.SetActiveWnd(SRC_TAB); //SRC_TAB(2)
		FilesLN *pFilesLN = *ListFilesLN[0];
		CSourceCodeWnd *pSrcWnd = gpSyser->m_MainFrame.m_SourceDebugFrameWnd.OpenSrcView(pSDSMod, pFilesLN->FileID, pFilesLN->lineNumber);
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_MultiSourceCodeView.UpdateContext();
		gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_TypeViewerWnd.UpdateContext();
	}
	return 0;
}

int sym_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	int Count = 0;

	WCHAR Buffer[MAX_FN_LEN];
	char szMod[MAX_FN_LEN];

	WCHAR szStr[64];
	char szSym[64];

	if (argc == 4)
	{
		int ADDOK = 0;
		ULONG_PTR Result;
		if (!TStrICmp(argv[1], "-a") || !TStrICmp(argv[1], "/a"))
		{
			//char szSym[64];
			UnicodeToAnsi(argv[2], szSym, 64);

			if (!gpSyser->m_SyserUI.CalcExp(argv[3], &Result))
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : %s input error!\n"), argv[3]);
			} else
			{
				CDbgModule *pDbgMod = pSyser->m_pDebugger->m_DbgModuleList.FindModule(Result);
				if (!pDbgMod)
					pDbgMod = pSyser->m_pSysDebugger->m_DbgModuleList.FindModule(Result);
				if (pDbgMod)
				{
					if (!pDbgMod->m_pSymbolModule)
					{
						CSymbolModule *pSymMod = gpSyser->m_SyserUI.m_SymbolContainer.InsertModule(pDbgMod->m_TimeStamp, pDbgMod->m_CheckSum);
						if (pSymMod)
							pSymMod->m_ModuleName = pDbgMod->m_pName;
						pDbgMod->SetSymbolModule(pSymMod);
					}
					ADDOK = pDbgMod->m_pSymbolModule->Set(Result - pDbgMod->m_ImageBase, szSym);
				} else
				{
					ADDOK = gpSyser->m_SyserUI.m_SyserCodeDoc.TExpCalc<char,ULONG_PTR,1>::InsertSym(szSym, Result);
				}
			}
		}
		if (ADDOK)
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Add '%s' ==> " F0ADDR "X succeed\n"), argv[2], Result);
		else	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Add symbols error!\n"));
		return 0;
	} else
	if (argc < 2)
	{
		//WCHAR Buffer[MAX_FN_LEN];
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("symbols ModuleName\n"));
		map<unsigned long long, CSymbolModule>::IT it = gpSyser->m_SyserUI.m_SymbolContainer.m_SymbolModulesMap.begin();
		//while (it != gpSyser->m_SyserUI.m_SymbolContainer.m_SymbolModulesMap.end())
		while (it)
		{
			if (it->second.m_ModuleName.IsEmpty())
			{
				TStrCpy(Buffer, "<No Name>");
			} else
			{
				AnsiToUnicode(it->second.m_ModuleName.operator const char*(), Buffer, lenof(Buffer));
			}
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%-5d   %s\n"), it->second.m_SymbolMap.size(), TGetFileName(Buffer));
			++it;
		}
		return 0;
	}

	//char szMod[MAX_FN_LEN];
	//char szSym[64];

	if (argc == 2)
	{
		UnicodeToAnsi(argv[1], szSym, 64);
		szMod[0] = 0;
	} else
	{
		UnicodeToAnsi(argv[1], szMod, MAX_FN_LEN);
		UnicodeToAnsi(argv[2], szSym, 64);
	}
{
	map<unsigned long long, CSymbolModule>::IT it = gpSyser->m_SyserUI.m_SymbolContainer.m_SymbolModulesMap.begin();
	//while (it != gpSyser->m_SyserUI.m_SymbolContainer.m_SymbolModulesMap.end())
	while (it)
	{
		if (it->second.m_ImageBase &&
			(TCmpModuleName(it->second.m_ModuleName.operator const char*(), szMod) || !szMod[0]))
		{
			map<ULONG_PTR, TAnsiNoCaseString>::IT ix = it->second.m_SymbolMap.begin();
			//while (ix != it->second.m_SymbolMap.end())
			while (ix)
			{
				if (TIMatchWithPattern(szSym, ix->second.operator const char*()))
				{
					//WCHAR szStr[64];
					AnsiToUnicode(ix->second.operator const char*(), szStr, lenof(szStr));
					if (it->second.m_ModuleName.IsEmpty())
					{
						TStrCpy(Buffer, "<No Name>");
					} else
					{
						AnsiToUnicode(it->second.m_ModuleName.operator const char*(), Buffer, lenof(Buffer));
					}
					gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<B>" F0ADDR "X %<1>: %<F>%s  %<1><%s>\n"),
						it->second.m_ImageBase + ix->first, szStr, TGetFileName(Buffer));
					++Count;
				}
				++ix;
			}
		}
		++it;
	}
}
{
	map<CMemoryImage, CDbgModule>::IT it = gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
	//while (it != gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.end())
	while (it)
	{
		if (it->second.m_pSymbolModule && !it->second.m_pSymbolModule->m_ImageBase &&
			(TCmpModuleName(it->second.m_pName, szMod) || !szMod[0]))
		{
			map<ULONG_PTR, TAnsiNoCaseString>::IT ix = it->second.m_pSymbolModule->m_SymbolMap.begin();
			//while (ix != it->second.m_pSymbolModule->m_SymbolMap.end())
			while (ix)
			{
				if (TIMatchWithPattern(szSym, ix->second.operator const char*()))
				{
					//WCHAR szStr[64];
					AnsiToUnicode(ix->second.operator const char*(), szStr, lenof(szStr));
					if (it->second.m_pSymbolModule->m_ModuleName.IsEmpty())
					{
						TStrCpy(Buffer, "<No Name>");
					} else
					{
						AnsiToUnicode(it->second.m_pSymbolModule->m_ModuleName.operator const char*(), Buffer, lenof(Buffer));
					}
					gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<B>" F0ADDR "X %<1>: %<F>%s  %<1><%s>\n"),
						it->second.m_ImageBase + ix->first, szStr, TGetFileName(Buffer));
					++Count;
				}
				++ix;
			}
		}
		++it;
	}
}
	if (gpSyser->m_pDebugger != gpSyser->m_pSysDebugger)
{
	map<CMemoryImage, CDbgModule>::IT it = gpSyser->m_pSysDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
	//while (it != gpSyser->m_pSysDebugger->m_DbgModuleList.m_DbgModuleMap.end())
	while (it)
	{
		if (it->second.m_pSymbolModule && !it->second.m_pSymbolModule->m_ImageBase &&
			(TCmpModuleName(it->second.m_pName, szMod) || !szMod[0]))
		{
			map<ULONG_PTR, TAnsiNoCaseString>::IT ix = it->second.m_pSymbolModule->m_SymbolMap.begin();
			//while (ix != it->second.m_pSymbolModule->m_SymbolMap.end())
			while (ix)
			{
				if (TIMatchWithPattern(szSym, ix->second.operator const char*()))
				{
					//WCHAR szStr[64];
					AnsiToUnicode(ix->second.operator const char*(), szStr, lenof(szStr));
					if (it->second.m_pSymbolModule->m_ModuleName.IsEmpty())
					{
						TStrCpy(Buffer, "<No Name>");
					} else
					{
						AnsiToUnicode(it->second.m_pSymbolModule->m_ModuleName.operator const char*(), Buffer, lenof(Buffer));
					}
					gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<B>" F0ADDR "X %<1>: %<F>%s  %<1><%s>\n"),
						it->second.m_ImageBase + ix->first, szStr, TGetFileName(Buffer));
					++Count;
				}
				++ix;
			}
		}
		++it;
	}
}
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%d Symbol(s)\n"), Count);
	return 0;
}

int symmod_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	int Count = 0;
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Ref  SymbolCount FullFileName\n"));
	map<unsigned long long, CSymbolModule>::IT it = gpSyser->m_SyserUI.m_SymbolContainer.m_SymbolModulesMap.begin();
	//while (it != gpSyser->m_SyserUI.m_SymbolContainer.m_SymbolModulesMap.end())
	while (it)
	{
		WCHAR szModuleName[MAX_FN_LEN];
		AnsiToUnicode(it->second.m_ModuleName.operator const char*(), szModuleName, lenof(szModuleName));
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%-4d %-8d    %s\n"),
			it->second.m_RefCount,
			it->second.m_SymbolMap.size(),
			szModuleName);
		++Count;
		++it;
	}
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%d Module(s)\n"), Count);
	return 0;
}

int sdsmod_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	unsigned int Count = 0;
	for (map<FILESIG, CSDSModule>::IT it = gpSyser->m_SyserUI.m_SDSModulesMap.begin();
		it != gpSyser->m_SyserUI.m_SDSModulesMap.end(); ++it)
	{
		if (Count == 0)
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("File BP(s) Size(k) CheckSum TimeStamp ModuleName\n"));
		WCHAR szModuleName[MAX_FN_LEN];
		AnsiToUnicode(it->second.m_ExeFileName.operator const char*(), szModuleName, lenof(szModuleName));
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%-4d %7dk %08X %08X  %s\n"),
				it->second.m_BreakPoints.Size(),
				it->second.m_MemSize / 1024,
				it->second.GetFILESIG().CheckSum,
				it->second.GetFILESIG().TimeStamp,
				szModuleName);
		++Count;
	}
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%d Module(s)\n"), Count);
	return 0;
}

int cmt_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;
	for (map<unsigned long long, CCommentModule>::IT It = gpSyser->m_SyserUI.m_CommentContainer.m_CommentModulesMap.begin();
		It; ++It)
	{
		WCHAR szModuleName[MAX_FN_LEN];
		AnsiToUnicode(It->second.m_ModuleName.operator const char*(), szModuleName, lenof(szModuleName));
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Comment Module : %<F>%s\n"), szModuleName);
		for (map<ULONG_PTR, TAnsiString>::IT it2 = It->second.m_CommentMap.begin();
			it2 != It->second.m_CommentMap.end(); ++it2)
		{
			WCHAR szComment[260];
			AnsiToUnicode(it2->second.operator const char*(), szComment, lenof(szComment));
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<5>" F0ADDR "X %<F>%s\n"), it2->first, szComment);
		}
	}
	return 0;
}

int seg_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;
	if (argc != 2)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Usage : seg segment_name\n"));
		return 1;
	}
	for (int i = 0; i < SEG_IDX_MAX; ++i) //lenof(CInstrSym::m_SegStr)
	{
		if (!TStrICmp(argv[1], CInstrSym::m_SegStr[i]))
		{
			unsigned long Seg = 0;
			switch (i)
			{
			case ES_IDX: Seg = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pES; break;
			case CS_IDX: Seg = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCS; break;
			case SS_IDX: Seg = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pSS; break;
			case DS_IDX: Seg = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pDS; break;
			case FS_IDX: Seg = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pFS; break;
			case GS_IDX: Seg = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pGS; break;
			default:Seg = 0; break;
			}
			ULONG_PTR Base = 0;
			if (pSyser->m_pDebugger->GetSegRegBase(Seg, &Base, 0))
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Seg %s : Base %08X\n"), CInstrSym::m_SegStr[i], Base);
			else	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Seg %s isn't present !\n"), CInstrSym::m_SegStr[i]);
			return 0;
		}
	}
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : %<3>Input error !\n"));
	return 1;
}

int r_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 1;
	if (argc < 2)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Usage : seg segment_name\n"));
		return 1;
	}
	if (argc < 3)
	{
		ULONG_PTR Result;
		if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Result))
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Parameter!\n"));
			return -1;
		}
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%s = %X\n"), argv[1], Result);
		return 0;
	}

	ULONG_PTR Result;
	if (!gpSyser->m_SyserUI.CalcExp(argv[2], &Result))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Parameter!\n"));
		return -1;
	}
	char Buffer[64];
	UnicodeToAnsi(argv[1], Buffer, 64);
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.TExpCalc<char,ULONG_PTR,1>::UpdateSym(Buffer, Result))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Parameter!\n"));
		return -1;
	}
	gpSyser->m_pDebugger->UpdateRegisters();
	gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
	return 0;
}

int m_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc != 4)
	{
		WCHAR Buffer[64];
		TSPrintf(Buffer, WSTR("help %s"), *argv);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
		return -1;
	}

	ULONG_PTR Src;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Src))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Address!\n"));
		return -1;
	}

	ULONG_PTR Len;
	if (!gpSyser->m_SyserUI.CalcExp(argv[2], &Len))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Invalid Length!\n"));
		return -1;
	}

	ULONG_PTR Dst;
	if (!gpSyser->m_SyserUI.CalcExp(argv[3], &Dst))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Address!\n"));
		return -1;
	}

	if (Len > 0x10000)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Length too large!\n"));
		return -1;
	}

	unsigned long Res = gpSyser->m_pDebugger->MoveMemory(Dst, Src, Len);
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%X bytes moved!\n"), Res);
	gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
	return 0;
}

int f_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc < 4)
	{
		WCHAR Buffer[64];
		TSPrintf(Buffer, WSTR("help %s"), argv[0]);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
		return -1;
	}

	ULONG_PTR Address;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Address))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Address!\n"));
		return -1;
	}

	ULONG_PTR Length;
	if (!gpSyser->m_SyserUI.CalcExp(argv[2], &Length))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Parameter!\n"));
		return -1;
	}

	unsigned char Buffer[256];
	memset(Buffer, 0, sizeof(Buffer));

	unsigned long Len = 0;
	const WCHAR *Str = &szCmd[argv[3] - argv[0]];

	if (*Str == '"' || *Str == '\'')
	{
		int n = 0;
		++Str;
		while (Str && (*Str != '"' && *Str != '\'') && n < 255)
		{
			Buffer[n++] = *Str++;
		}
		Buffer[n]=0;
		Len = n+1; //???
	} else
	{
		Len = THexBytesToASCII(Str, Buffer, 256);
		if (Len == 0)
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Parameter!\n"));
			return -1;
		}
	}

	unsigned long Count = 0;
	while (Length>0)
	{
		unsigned long WLen = MIN(Length, Len);
		if (pSyser->m_pDebugger->WriteMemory(Address+Count, Buffer, WLen) != WLen)
			break;
		Length -= WLen;
		Count += WLen;
	}
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%d Byte(s) filled !\n"), Count);
	gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
	return 0;
}

int s_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	static unsigned char gBuffer[256];
	static unsigned long gFoundCodeView = 0;
	static unsigned long gStartCodeView = 0;
	static unsigned long gPatternLen = 0;
	static ULONG_PTR gAddress = 0;
	static ULONG_PTR gLength = 0;

	unsigned long PatternLen;
	ULONG_PTR Address;
	ULONG_PTR Length;

	if (argc < 4)
	{
		if (argc != 1 || gLength == 0)
		{
			WCHAR Buffer[64];
			TSPrintf(Buffer, WSTR("help %s"), argv[0]);
			gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
			return -1;
		}
		Length = gLength;
		PatternLen = gPatternLen;
		Address = gAddress;
	} else
	{
		switch (argv[0][1])
		{
		case 0:
		case 'D':
		case 'd':
			gStartCodeView = 0;
			break;
		case 'C':
		case 'c':
			gStartCodeView = 1;
			break;
		}

		gFoundCodeView = gStartCodeView;

		if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Address))
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Address!\n"));
			return -1;
		}

		if (!gpSyser->m_SyserUI.CalcExp(argv[2], &Length))
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Parameter!\n"));
			return -1;
		}

		//static unsigned char gBuffer[256];
		memset(gBuffer, 0, sizeof(gBuffer));

		//unsigned long gPatternLen = 0;
		const WCHAR *Str = &szCmd[argv[3] - argv[0]];

		if (*Str == '"' || *Str == '\'')
		{
			int n = 0;
			++Str;
			gPatternLen = 0;
			while (Str && (*Str != '"' && *Str != '\'') && n < 255)
			{
				gBuffer[n++] = *Str++;
			}
			gBuffer[n] = 0;
			PatternLen = n;
			gPatternLen = PatternLen;
		} else
		{
			PatternLen = THexBytesToASCII(Str, gBuffer, 256);
			gPatternLen = PatternLen;
			if (PatternLen == 0)
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Parameter!\n"));
				return -1;
			}
		}
	}

	ULONG_PTR Res = gpSyser->m_pDebugger->SearchMemory(Address, Length, gBuffer, PatternLen, true); //bCaseCmp
	if (Res == -1)
	{
		gLength = 0;
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Couldn't found in range!\n"));
		return 0;
	} else
	{
		switch (argv[0][1])
		{
		case 0:
			gFoundCodeView = gStartCodeView;
			break;
		case 'D':
		case 'd':
			gStartCodeView = 1;
			break;
		case 'C':
		case 'c':
			gStartCodeView = 0;
			break;
		}
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Found at " F0ADDR "X\n"), Res);
		gLength = Address + Length - (Res + 1);
		gAddress = Res + 1;
		if (gFoundCodeView)
			gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(Res, true);
		else	gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(Res, gPatternLen);
	}
	return 0;
}

int c_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc != 4)
	{
		WCHAR Buffer[64];
		TSPrintf(Buffer, WSTR("help %s"), *argv);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
		return -1;
	}

	ULONG_PTR Src;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Src))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Address!\n"));
		return -1;
	}

	ULONG_PTR Len;
	if (!gpSyser->m_SyserUI.CalcExp(argv[2], &Len))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Invalid Length!\n"));
		return -1;
	}

	ULONG_PTR Dst;
	if (!gpSyser->m_SyserUI.CalcExp(argv[3], &Dst))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Address!\n"));
		return -1;
	}

	if (Len > 0x10000)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Length too large!\n"));
		return -1;
	}

	unsigned long Res = gpSyser->m_pDebugger->CompareMemory(Dst, Src, Len);
	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%X bytes is same!\n"), Res);
	gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
	return 0;
}

int a_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		return 0;
	
	if (argc < 3)
	{
		WCHAR Buffer[64];
		TSPrintf(Buffer, WSTR("help %s"), argv[0]);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
		return -1;
	}

	ULONG_PTR Address;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Address))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Address!\n"));
		return -1;
	}

	char szAsmCmd[128];
	TStrCpyLimit(szAsmCmd, &szCmd[argv[2] - argv[0]], 128);
	unsigned char CodeBuf[64];
	unsigned long CodeLen = gpSyser->m_SyserUI.m_SyserCodeDoc.Asm(szAsmCmd, CodeBuf, Address);
	if (!CodeLen)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid asm code string!\n"));
		return -1;
	}

	if (pSyser->m_pDebugger->WriteMemory(Address, CodeBuf, CodeLen) != CodeLen)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Fail to write code to " F0ADDR "X!\n"), Address);//argv[1]); //??? Address
		return -1;
	}
	gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
	return 0;
}

int e_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc < 2)
	{
		WCHAR Buffer[64];
		TSPrintf(Buffer, WSTR("help %s"), argv[0]);
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(Buffer, 0);
		return -1;
	}

	ULONG_PTR Address;
	if (!gpSyser->m_SyserUI.CalcExp(argv[1], &Address))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Address!\n"));
		return -1;
	}

	if (argc == 2)
	{
		pSyser->m_MainFrame.m_SystemExplorer.m_MultiDataView.SetActiveAddress(Address, argv[1]);
		return 0;
	}

	unsigned char Buffer[256];
	memset(Buffer, 0, sizeof(Buffer));

	unsigned long Len = 0;
	const WCHAR *Str = &szCmd[argv[2] - argv[0]];
	
	switch (argv[1][0])
	{
	case 'D':
	case 'd':
		if (!gpSyser->m_SyserUI.CalcExp(Str, (ULONG_PTR*)Buffer))
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Parameter!\n"));
			return -1;
		}
		Len = 4;
		break;
	case 'W':
	case 'w':
		if (!gpSyser->m_SyserUI.CalcExp(Str, (ULONG_PTR*)Buffer))
		{
			gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Parameter!\n"));
			return -1;
		}
		Len = 2;
		break;
	default:
		if (*Str == '"' || *Str == '\'')
		{
			int n = 0;
			++Str;
			while (Str && (*Str != '"' && *Str != '\'') && n < 255)
			{
				Buffer[n++] = *Str++;
			}
			Buffer[n]=0;
			Len = n+1;
		} else
		{
			Len = THexBytesToASCII(Str, Buffer, 256);
			if (Len == 0)
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Invalid Parameter!\n"));
				return -1;
			}
		}
	}
	if (pSyser->m_pDebugger->WriteMemory(Address, Buffer, Len) == Len)
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%<3>Error %<1>: Fail to write memory!\n"));
	gpSyser->m_MainFrame.m_SystemExplorer.UpdateContext();
	return 0;
}

void ShowDbgModule(CDbgModule *pDbgMod)
{
	char szTimeStamp[32];
	Time2Str(pDbgMod->m_TimeStamp - gpSyser->m_TimeOffset, szTimeStamp);

	WCHAR TimeStamp[30];
	TStrCpy(TimeStamp, szTimeStamp);

	WCHAR FileTitle[MAX_FN_LEN];
	AnsiToUnicode(pDbgMod->m_FileTitle.operator const char*(), FileTitle, lenof(FileTitle));

	WCHAR FullFileName[MAX_FN_LEN];
	AnsiToUnicode(pDbgMod->m_FullFileName.operator const char*(), FullFileName, lenof(FullFileName));

	gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("%-20s " F0ADDR "X  %08X  %08X %-20s%s\n"),
		FileTitle, pDbgMod->m_ImageBase, pDbgMod->m_ImageSize, pDbgMod->m_CheckSum, TimeStamp, FullFileName);
}

int mod_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (argc == 1)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("ModuleName           ImageBase ImageSize CheckSum TimeStamp           FullFileName\n"));
		for (map<CMemoryImage, CDbgModule>::IT it = pSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
			it != pSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.end(); ++it)
		{
			ShowDbgModule(&it->second);
		}
		return 0;
	}

	ULONG_PTR Address = -1;

	int cmd = 0;
	int start = 1;
	if (!TStrCmp(argv[1], "-u"))
	{
		cmd = 1;
		start = 2;
	} else
	if (!TStrCmp(argv[1], "-s"))
	{
		cmd = 2;
		start = 2;
	} 

	if (start < argc && gpSyser->m_SyserUI.CalcExp(argv[start], &Address))
	{
		CDbgModule *pDbgMod = pSyser->m_pSysDebugger->m_DbgModuleList.FindModule(Address);
		if (!pDbgMod)
		{
			pDbgMod = pSyser->m_pDebugger->m_DbgModuleList.FindModule(Address);
			if (!pDbgMod)
			{
				gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Can't find this module locate on " F0ADDR "X\n"), Address); //???
				return 1;
			}
		}
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("ModuleName           ImageBase ImageSize FullFileName\n"));
		ShowDbgModule(pDbgMod);
	} else
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("ModuleName           ImageBase ImageSize FullFileName\n"));
		if (pSyser->m_pSysDebugger != pSyser->m_pDebugger && cmd != 2) //-u
		{
			for (map<CMemoryImage, CDbgModule>::IT it = pSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
				it != pSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.end(); ++it)
			{
				if (start < argc)
				{
					WCHAR Buffer[MAX_FN_LEN];
					AnsiToUnicode(it->second.m_FullFileName.operator const char*(), Buffer, lenof(Buffer));
					for (int n = start; n < argc; ++n)
					{
						if (!TCmpModuleName(Buffer, argv[n]) &&
						    !TIMatchWithPattern(argv[n], TGetFileName(Buffer)))
							continue;
						ShowDbgModule(&it->second);
					}
				} else
				{
					ShowDbgModule(&it->second);
				}
			}
		}

		if (cmd != 1) //-s
		{
			for (map<CMemoryImage, CDbgModule>::IT it = pSyser->m_pSysDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
				it != pSyser->m_pSysDebugger->m_DbgModuleList.m_DbgModuleMap.end(); ++it)
			{
				if (start < argc)
				{
					WCHAR Buffer[MAX_FN_LEN];
					AnsiToUnicode(it->second.m_FullFileName.operator const char*(), Buffer, lenof(Buffer));
					for (int n = start; n < argc; ++n)
					{
						if (!TCmpModuleName(Buffer, argv[n]) &&
						    !TIMatchWithPattern(argv[n], TGetFileName(Buffer)))
							continue;
						ShowDbgModule(&it->second);
					}
				} else
				{
					ShowDbgModule(&it->second);
				}
			}
		}
	}
	return 0;
}

int open_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!pSyser->m_pDebugger)
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : please load debugger first !\n"));
		return 1;
	}

	if (pSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		pSyser->m_SyserUI.m_SyserCodeDoc.Close();

	char szFileName[MAX_FN_LEN];
	if (argc != 1)
	{
		UnicodeToAnsi(&szCmd[argv[1] - argv[0]], szFileName, MAX_FN_LEN);
	} else
	{
	#if 0
		OPENFILENAMEA ofn;
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hInstance = gpSyser->m_hInstance;
		ofn.hwndOwner = gpSyser->m_hWnd;

		char szFilter[MAX_FN_LEN];
		UnicodeToAnsi(gpSyser->m_szDbgFileNameFilter.operator const WCHAR*(), szFilter, MAX_FN_LEN);
		
		int Len = TStrLen(szFilter);
		MAX_LIMIT(Len, MAX_FN_LEN);

		for (int i = Len; i >= 0; --i)
		{
			if (szFilter[i] == '|')
				szFilter[i] = 0;
		}

		ofn.lpstrFile = szFileName;
		ofn.lpstrFilter = szFilter;
		ofn.nMaxFile = MAX_FN_LEN;
		szFileName[0] = 0;
		gpSyser->LockTimer();
		if (!::GetOpenFileNameA(&ofn))
		{
			gpSyser->UnlockTimer();
			return 1;
		}
		gpSyser->UnlockTimer();

		TStrCpy(szFileName, ofn.lpstrFile);
	#else
		if (!gpSyser->OpenFileDialog(szFileName))
			return 1;
	#endif
	}

	if (!pSyser->m_SyserUI.m_SyserCodeDoc.Open(szFileName))
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : Fail to open !\n"));
		return 1;
	}	
	return 0;
}

int close_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!pSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen && gpSyser->m_SyserUI.m_SyserCodeDoc.m_ExeFilePath.IsEmpty())
	{
		gpSyser->m_MainFrame.m_ConsoleWnd.Printf(WSTR("Error : please open file first !\n"));
		return 1;
	}
	pSyser->m_SyserUI.m_SyserCodeDoc.Close();
	return 0;
}

int reset_command(int argc, const WCHAR **argv, const WCHAR *szCmd, CSyser *pSyser)
{
	if (!gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen || gpSyser->m_SyserUI.m_SyserCodeDoc.m_ExeFilePath.IsEmpty())
		return 1;
	WCHAR szFileName[MAX_FN_LEN];
	AnsiToUnicode(gpSyser->m_SyserUI.m_SyserCodeDoc.m_ExeFilePath.operator const char*(), szFileName, lenof(szFileName));
	if (gpSyser->m_SyserUI.m_SyserCodeDoc.m_bOpen)
		gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(WSTR("close"), 0);
	WCHAR szOpenCmd[MAX_FN_LEN];
	TSPrintf(szOpenCmd, WSTR("open %s"), szFileName);
	gpSyser->m_MainFrame.m_ConsoleWnd.CCmdParser::ParseCmd(szOpenCmd, 0);
	return 0;
}

SYSER_CMD_ENTRY SysCmdTable[] =
{
	{ WSTR("bc"), WSTR("Clear breakpoint"), bc_command, 
		WSTR("bc: Clear breakpoint\n"
		"Format: bc address_or_index[*]\n"
		"Example1: bc *\n"
		"Example2: bc 1\n"
		"Example3: bc 401000\n")},
	{ WSTR("bl"), WSTR("List breakpoints"), bl_command, 
		WSTR("bl: List breakpoint\n"
		"Format: bl\n"
		"Example1: bl\n")},
	{ WSTR("bd"), WSTR("Disable breakpoints"), bd_command, 
		WSTR("bd: Disable breakpoint\n"
		"Format: bd address_or_index[*]\n"
		"Example1: bd *\n"
		"Example2: bd 1\n"
		"Example3: bd 401000\n")},
	{ WSTR("be"), WSTR("Enable breakpoints"), be_command, 
		WSTR("be: Enable breakpoint\n"
		"Format: be address_or_index[*]\n"
		"Example1: be *\n"
		"Example2: be 1\n"
		"Example3: be 401000\n")},
	{ WSTR("bpx"), WSTR("Set breakpoint"), bpx_command, 
		WSTR("bpx: Set breakpoint\n"
		"Format: bpx address [if condition_string] [do command_string]\n"
		"Example1: bpx 401000 EAX==0302\n"
		"Example2: bpx MessageBoxA\n"
		"Example3: bpx 402000 rw if (*esi)&FF==0 do d eip\n")},
	{ WSTR("bpload"), WSTR("Set module loading breakpoint"), bpload_command, 
		WSTR("bpload: Set module loading breakpoint\n"
		"Format: bpload module_name\n"
		"Example1: bpload kernel32\n"
		"Example2: bpload user32.dll\n")},
	{ WSTR("bpm"), WSTR("Breakpoint on memory access"), bpm_command, 
		WSTR("bpm,bpmb,bpmw,bpmd: Set data access breakpoint\n"
		"Format: bpm address [rw,w,x] [if condition_string] [do command_string]\n"
		"Example1: bpm 401000 w\n"
		"Example2: bpmw 401000 rw       (set word access breakpoint)\n"
		"Example3: bpm esi w if *eax==00 do d esi;bc *\n")},
	{ WSTR("bpmb"), WSTR("Breakpoint on memory access (BYTE)"), bpm_command, 
		WSTR("bpm,bpmb,bpmw,bpmd: Set data access breakpoint\n"
		"Format: bpm address [rw,w,x] [if condition_string] [do command_string]\n"
		"Example1: bpm 401000 w\n"
		"Example2: bpmw 401000 rw       (set word access breakpoint)\n"
		"Example3: bpm esi w if *eax==00 do d esi;bc *\n")},
	{ WSTR("bpmw"), WSTR("Breakpoint on memory access (WORD)"), bpm_command, 
		WSTR("bpm,bpmb,bpmw,bpmd: Set data access breakpoint\n"
		"Format: bpm address [rw,w,x] [if condition_string] [do command_string]\n"
		"Example1: bpm 401000 w\n"
		"Example2: bpmw 401000 rw       (set word access breakpoint)\n"
		"Example3: bpm esi w if *eax==00 do d esi;bc *\n")},
	{ WSTR("bpmd"), WSTR("Breakpoint on memory access (DWORD)"), bpm_command, 
		WSTR("bpm,bpmb,bpmw,bpmd: Set data access breakpoint\n"
		"Format: bpm address [rw,w,x] [if condition_string] [do command_string]\n"
		"Example1: bpm 401000 w\n"
		"Example2: bpmw 401000 rw       (set word access breakpoint)\n"
		"Example3: bpm esi w if *eax==00 do d esi;bc *\n")},
	{ WSTR("bpio"), WSTR("Breakpoint on I/O port access"), bpio_command, 
		WSTR("bpio: Set a breakpoint on an I/O Port access\n"
		"Format: bpio I/O_port [r,w,rw] [if condition_string] [do command_string]\n"
		"Example1: bpio 3E8 rw\n"
		"Example2: bpio 2E4 rw if eax==0 do dd esi\n")},
	{ WSTR("x"), WSTR("Return to host and continue running"), x_command, 
		WSTR("x: exit debugger (continue)\n"
		"format: x\n")},
	{ WSTR("exit"), WSTR("Return to host and continue running"), x_command, 
		WSTR("x: exit debugger (continue)\n"
		"format: x\n")},
	{ WSTR("g"), WSTR("Go to address"), g_command, 
		WSTR("go : set one-time breakpoint at specify address\n"
		"Format: go address\n"
		"Example1: go " CIPSTR "+20\n"
		"Example2: go 402000\n")},
	{ WSTR("t"), WSTR("Trace one instruction"), t_command, 
		WSTR("t: Trace one instruction\n"
		"Format: t [step count] [condition]\n"
		"Example1: t 100                       (run 100 t steps)\n"
		"Example2: t " CIPSTR "<401000 && " CIPSTR ">402000  (break " CIPSTR " jump out of 401000 - 402000)\n")},
	{ WSTR("st"), WSTR("Trace one source code line"), st_command, 
		WSTR("st: Trace one source code line\n"
		"Format: st\n"
		"Example1: st\n")},
	{ WSTR("tb"), WSTR("Trace and break at next branch"), tb_command, 
		0},
	{ WSTR("p"), WSTR("Execute one program step"), p_command, 
		WSTR("p: Execute one program step (skip CALL,REP )\n"
		"Format: p [step count] [condition] [ret]\n"
		"Example1: p 100                       (run 100 p steps)\n"
		"Example2: p " CIPSTR "<401000 && " CIPSTR ">402000  (break " CIPSTR " jump out of 401000 - 402000)\n"
		"Example3: p ret                       (stop at return)\n")},
	{ WSTR("sp"), WSTR("Execute one source code step"), sp_command, 
		WSTR("sp: Execute one source code step\n"
		"Format: sp\n"
		"Example1: sp\n)")},
	{ WSTR("skip"), WSTR("Skip current instruction"), skip_command, 
		0},
	{ WSTR("stack"), WSTR("Display call stack context"), stack_command, 
		WSTR("stack: Display call stack\n"
		"Format: stack [call_frame_address]\n"
		"Example1: stack             (analyse from ebp)\n"
		"Example2: stack  124000     (analyse from 124000)\n")},
	{ WSTR("u"), WSTR("Unassemble instructions"), u_command, 
		WSTR("u: Unassemble instructions\n"
		"Format: u address\n"
		"Exapmle1: u 401000\n"
		"Exapmle2: u eip+1000\n"
		"Example3: u +\n"
		"Example4: u -\n"
		"Example5: u + offset\n"
		"Example6: u - offset\n"
		"Example6: u -a address\n")},
	{ WSTR("."), WSTR("Locate " CIPSTR " instruction in Code View"), u_command, 
		WSTR(".: Locate " CIPSTR " instruction in Code View (== u eip)\n"
		"Format: .\n")},
	{ WSTR("src"), WSTR("Locate address in Source Code File"), src_command, 
		WSTR("src: Locate address in Source Code File\n"
		"Format: src [address]\n"
		"Example1: src (==src EIP)\n"
		"Example2: src 401000\n")},
	{ WSTR("sym"), WSTR("List symbols"), sym_command, 
		WSTR("sym: List symbols\n"
		"Format: sym [module_name] [symbol_wildcard] | -a symbol value\n"
		"Example1: sym kernel32 *         (List all symbols of kernel32.dll)\n"
		"Example2: sym CreateFile?        (List CreateFileA and CreateFileW)\n"
		"Example3: sym -a mysymbol 4013af (Add owner labels)\n")},
	{ WSTR("symmod"), WSTR("Display all symbol modules"), symmod_command,
		0},
	{ WSTR("sdsmod"), WSTR("Display all sds symbol modules"), sdsmod_command,
		WSTR("sdsmod: List all syser symbol modules loaded\n"
		"Format: sdsmod\n")},
	{ WSTR("cmt"), WSTR("List module comment in current process"), cmt_command,
		WSTR("cmt: List module comment in current process\n"
		"Format: cmt\n")},
	{ WSTR("seg"), WSTR("Display segment information"), seg_command,
		WSTR("seg: Display segment information\n"
		"Format: seg segment_register_name\n"
		"Example1: seg cs\n"
		"Example2: seg 08")},
	{ WSTR("r"), WSTR("Display/Change Register Value"), r_command,
		WSTR("r: Display/Change  Register Value\n"
		"Format: r reg_name [value]\n"
		"Example1: r eip 402000\n"
		"Example2: r eax 80001000\n"
		"Example3: r ah 10\n"
		"Example4: r ebx")},
	{ WSTR("m"), WSTR("Move data"), m_command, 
		WSTR("m: Move data\n"
		"Format: m source_address length dest_address (length is HEX bytes)\n"
		"Example1: m esi 100 edi\n"
		"Example2: m 402000 E00 403000\n")},
	{ WSTR("f"), WSTR("Fill memory with data"), f_command, 
		WSTR("f: Fill memory\n"
		"Format: f address length data_string\n"
		"Example1: f esi 4 80001000\n"
		"Example2: f 401000 5 E9,00,00,00,10\n"
		"Example3: f 401000 1000 \"hello\"\n")},
	{ WSTR("s"), WSTR("Search binary data or string"), s_command, 
		WSTR("s: Search memory\n"
		"Format: s address length data_string\n"
		"Example1: s 401000 1000 \"CreateFileA\"\n"
		"Example2: s " CIPSTR " 2000 FF,15,00,00,10,40\n"
		"Example3: s           (continue last search operation)\n")},
	{ WSTR("sc"), WSTR("Search binary data or string,show search resoult at code view"), s_command, 
		WSTR("s: Search memory\n"
		"Format: s address length data_string\n"
		"Example1: s 401000 1000 \"CreateFileA\"\n"
		"Example2: s " CIPSTR " 2000 FF,15,00,00,10,40\n"
		"Example3: s           (continue last search operation)\n")},
	{ WSTR("sd"), WSTR("Search binary data or string,show search resoult at data view"), s_command, 
		WSTR("s: Search memory\n"
		"Format: s address length data_string\n"
		"Example1: s 401000 1000 \"CreateFileA\"\n"
		"Example2: s " CIPSTR " 2000 FF,15,00,00,10,40\n"
		"Example3: s           (continue last search operation)\n")},
	{ WSTR("c"), WSTR("Compare two data blocks"), c_command, 
		WSTR("c: Compare two data blocks\n"
		"Format: c source_address length dest_address (length is HEX bytes)\n"
		"Example1: c esi 100 edi\n"
		"Example2: c 402000 E00 403000\n")},
	{ WSTR("a"), WSTR("modify memory by assemble code"), a_command, 
		WSTR("a: Modify memory by assemble code\n"
		"Format: a address asm_string\n"
		"Example1: a 401000 inc eax\n"
		"Example2: a EIP mov eax,ebx\n")},
	{ WSTR("e"), WSTR("Edit memory"), e_command, 
		WSTR("e,eb,ew,ed:  Edit memory\n"
		"Format: e address data_value\n"
		"Example1: e 403000 00,01,02,04\n"
		"Example2: e ESI \"Hello\"\n"
		"Example3: eb 401000 10\n"
		"Example4: ew 401000 2030\n"
		"Example5: ed 401000 40506070\n")},
	{ WSTR("eb"), WSTR("Edit memory (BYTE)"), e_command, 
		WSTR("e,eb,ew,ed:  Edit memory\n"
		"Format: e address data_value\n"
		"Example1: e 403000 00,01,02,04\n"
		"Example2: e ESI \"Hello\"\n"
		"Example3: eb 401000 10\n"
		"Example4: ew 401000 2030\n"
		"Example5: ed 401000 40506070\n")},
	{ WSTR("ew"), WSTR("Edit memory (WORD)"), e_command, 
		WSTR("e,eb,ew,ed:  Edit memory\n"
		"Format: e address data_value\n"
		"Example1: e 403000 00,01,02,04\n"
		"Example2: e ESI \"Hello\"\n"
		"Example3: eb 401000 10\n"
		"Example4: ew 401000 2030\n"
		"Example5: ed 401000 40506070\n")},
	{ WSTR("ed"), WSTR("Edit memory (DWORD)"), e_command, 
		WSTR("e,eb,ew,ed:  Edit memory\n"
		"Format: e address data_value\n"
		"Example1: e 403000 00,01,02,04\n"
		"Example2: e ESI \"Hello\"\n"
		"Example3: eb 401000 10\n"
		"Example4: ew 401000 2030\n"
		"Example5: ed 401000 40506070\n")},
	{ WSTR("mod"), WSTR("Display Modules"), mod_command, 
		WSTR("mod: List modules of current process\n"
		"Format: mod [address][mod_name]\n"
		"Example1: mod eip           (display module's name on current eip)\n"
		"Example2: mod nt*           (display module's name fit to \"nt*\")\n")},

	{ WSTR("open"), WSTR("open file"), open_command, 
		0},
	{ WSTR("close"), WSTR("close file"), close_command, 
		0},
	{ WSTR("reset"), WSTR("reset debug state"), reset_command, 
		0},

	{0},
};

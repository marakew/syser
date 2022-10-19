
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "peexplorerform.hpp"
#include "../Code/time.hpp"

	WISP_MSG_MAP_BEGIN(CPEHeaderWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispWnd)

        WISP_EVENT_MAP_BEGIN(CPEHeaderWnd)
//		WISP_EVENT_MAP(, OnEvent)
        WISP_EVENT_MAP_END

	bool CPEHeaderWnd::OnCreate(WISP_MSG *pMsg)
	{
		m_COFF.CreateWnd(0, 5,5,372,294, this, 0x101, 0x46010006, WISP_SH_MINSIZE);
		m_WIN.CreateWnd(0, 385,5,372,294, this, 0x102, 0x46010006, WISP_SH_MINSIZE);
		InitContext();
		return true;
	}

	bool CPEHeaderWnd::InitContext()
	{
		m_COFF.InsertColumn(WSTR("Field Name"), 165, 0, 0, -1);
		m_COFF.InsertColumn(WSTR("Date Value"), 80, 0, 0, -1);
		m_COFF.InsertColumn(WSTR("Description"), 120, 0, 0, -1);
		m_pItems[0] = m_COFF.InsertItem(WSTR("Machine"), 0, 0, 0, -1);
		m_pItems[1] = m_COFF.InsertItem(WSTR("Number of Sections"), 0, 0, 0, -1);
		m_pItems[2] = m_COFF.InsertItem(WSTR("Time Date Stamp"), 0, 0, 0, -1);
		m_pItems[3] = m_COFF.InsertItem(WSTR("Pointer to Symbol Table"), 0, 0, 0, -1);
		m_pItems[4] = m_COFF.InsertItem(WSTR("Number of Symbols"), 0, 0, 0, -1);
		m_pItems[5] = m_COFF.InsertItem(WSTR("Size of Optional Header"), 0, 0, 0, -1);
		m_pItems[6] = m_COFF.InsertItem(WSTR("Characteristics"), 0, 0, 0, -1);
		m_pItems[7] = m_COFF.InsertItem(WSTR("Magic"), 0, 0, 0, -1);
		m_pItems[8] = m_COFF.InsertItem(WSTR("Linker Version"), 0, 0, 0, -1);
		m_pItems[9] = m_COFF.InsertItem(WSTR("Size of code"), 0, 0, 0, -1);
		m_pItems[10] = m_COFF.InsertItem(WSTR("Size of Initialized Data"), 0, 0, 0, -1);
		m_pItems[11] = m_COFF.InsertItem(WSTR("Size of Uninitialized Data"), 0, 0, 0, -1);
		m_pItems[12] = m_COFF.InsertItem(WSTR("Address of entry Point"), 0, 0, 0, -1);
		m_pItems[13] = m_COFF.InsertItem(WSTR("Base of code"), 0, 0, 0, -1);
	#ifndef _X64_
		m_pItems[14] = m_COFF.InsertItem(WSTR("Base of data"), 0, 0, 0, -1);
	#endif
		m_pItems[15] = m_COFF.InsertItem(WSTR("Image Base"), 0, 0, 0, -1);

		m_WIN.InsertColumn(WSTR("Field Name"), 165, 0, 0, -1);
		m_WIN.InsertColumn(WSTR("Date Value"), 90, 0, 0, -1);
		m_WIN.InsertColumn(WSTR("Description"), 110, 0, 0, -1);
		m_pItems[16] = m_WIN.InsertItem(WSTR("Section Alignment"), 0, 0, 0, -1);
		m_pItems[17] = m_WIN.InsertItem(WSTR("File Alignment"), 0, 0, 0, -1);
		m_pItems[18] = m_WIN.InsertItem(WSTR("Operating System Version"), 0, 0, 0, -1);
		m_pItems[19] = m_WIN.InsertItem(WSTR("Image Version"), 0, 0, 0, -1);
		m_pItems[20] = m_WIN.InsertItem(WSTR("Subsystem Version"), 0, 0, 0, -1);
		m_pItems[21] = m_WIN.InsertItem(WSTR("Win32 Version Value"), 0, 0, 0, -1);
		m_pItems[22] = m_WIN.InsertItem(WSTR("Size of Image"), 0, 0, 0, -1);
		m_pItems[23] = m_WIN.InsertItem(WSTR("Size of Headers"), 0, 0, 0, -1);
		m_pItems[24] = m_WIN.InsertItem(WSTR("Checksum"), 0, 0, 0, -1);
		m_pItems[25] = m_WIN.InsertItem(WSTR("Subsystem"), 0, 0, 0, -1);
		m_pItems[26] = m_WIN.InsertItem(WSTR("Dll Characteristics"), 0, 0, 0, -1);
		m_pItems[27] = m_WIN.InsertItem(WSTR("Size of Stack Reserve"), 0, 0, 0, -1);
		m_pItems[28] = m_WIN.InsertItem(WSTR("Size of Stack Commit"), 0, 0, 0, -1);
		m_pItems[29] = m_WIN.InsertItem(WSTR("Size of Heap Reserve"), 0, 0, 0, -1);
		m_pItems[30] = m_WIN.InsertItem(WSTR("Size of Heap Commit"), 0, 0, 0, -1);
		m_pItems[31] = m_WIN.InsertItem(WSTR("Loader Flags"), 0, 0, 0, -1);
		m_pItems[32] = m_WIN.InsertItem(WSTR("Number of Data Directories"), 0, 0, 0, -1);
 		
		return true;
	}

	bool CPEHeaderWnd::Open(CPEFile *pPEFile)
	{
		char Buffer[52];

		Time2Str(pPEFile->m_PEHead.TimeDateStamp - gpSyser->m_TimeOffset, Buffer);

		m_COFF.SetItemFormat(m_pItems[0], 1, pPEFile->m_PEHead.Machine, WSTR("0x%04x"));
		m_COFF.SetItemFormat(m_pItems[1], 1, pPEFile->m_PEHead.NumberOfSections, WSTR("0x%04x"));
		m_COFF.SetItemFormat(m_pItems[2], 1, pPEFile->m_PEHead.TimeDateStamp, WSTR("0x%08x"));
		m_COFF.SetItemText(m_pItems[2], 2, Buffer);
		m_COFF.SetItemFormat(m_pItems[3], 1, pPEFile->m_PEHead.PointerToSymbolTable, WSTR("0x%08x"));
		m_COFF.SetItemFormat(m_pItems[4], 1, pPEFile->m_PEHead.NumberOfSymbols, WSTR("0x%08x"));
		m_COFF.SetItemFormat(m_pItems[5], 1, pPEFile->m_PEHead.SizeOfOptionalHeader, WSTR("0x%04x"));
		m_COFF.SetItemFormat(m_pItems[6], 1, pPEFile->m_PEHead.Characteristics, WSTR("0x%08x"));
		m_COFF.SetItemFormat(m_pItems[7], 1, pPEFile->m_PEHead.Magic, WSTR("0x%08x"));
		m_COFF.SetItemFormat(m_pItems[8], 1, MAKE_WORD(pPEFile->m_PEHead.MinorLinkerVersion, pPEFile->m_PEHead.MajorLinkerVersion), WSTR("0x%04x"));
		TSPrintf(Buffer, "%d.%d", pPEFile->m_PEHead.MajorLinkerVersion, pPEFile->m_PEHead.MinorLinkerVersion);
		m_COFF.SetItemText(m_pItems[8], 2, Buffer);
		m_COFF.SetItemFormat(m_pItems[9], 1, pPEFile->m_PEHead.SizeOfCode, WSTR("0x%08x"));
		m_COFF.SetItemFormat(m_pItems[10], 1, pPEFile->m_PEHead.SizeOfInitializedData, WSTR("0x%08x"));
		m_COFF.SetItemFormat(m_pItems[11], 1, pPEFile->m_PEHead.SizeOfUninitializedData, WSTR("0x%08x"));
		m_COFF.SetItemFormat(m_pItems[12], 1, pPEFile->m_PEHead.AddressOfEntryPoint, WSTR("0x%08x"));
		m_COFF.SetItemFormat(m_pItems[13], 1, pPEFile->m_PEHead.BaseOfCode, WSTR("0x%08x"));
	#ifndef _X64_
		m_COFF.SetItemFormat(m_pItems[14], 1, pPEFile->m_PEHead.BaseOfData, WSTR("0x%08x"));
	#endif
		m_COFF.SetItemFormat(m_pItems[15], 1, pPEFile->m_PEHead.ImageBase, WSTR("0x" F0ADDR "X")); //"0x%016I64X"

		m_WIN.SetItemFormat(m_pItems[16], 1, pPEFile->m_PEHead.SectionAlignment, WSTR("0x%08x"));
		m_WIN.SetItemFormat(m_pItems[17], 1, pPEFile->m_PEHead.FileAlignment, WSTR("0x%08x"));
		m_WIN.SetItemFormat(m_pItems[18], 1, MAKE_DWORD(pPEFile->m_PEHead.MinorOperatingSystemVersion, pPEFile->m_PEHead.MajorOperatingSystemVersion), WSTR("0x%08x"));
		TSPrintf(Buffer, "%d.%d", pPEFile->m_PEHead.MajorOperatingSystemVersion, pPEFile->m_PEHead.MinorOperatingSystemVersion);
		m_COFF.SetItemText(m_pItems[18], 2, Buffer);
		m_WIN.SetItemFormat(m_pItems[19], 1, MAKE_DWORD(pPEFile->m_PEHead.MinorImageVersion, pPEFile->m_PEHead.MajorImageVersion), WSTR("0x%08x"));
		TSPrintf(Buffer, "%d.%d", pPEFile->m_PEHead.MajorImageVersion, pPEFile->m_PEHead.MinorImageVersion);
		m_COFF.SetItemText(m_pItems[19], 2, Buffer);
		m_WIN.SetItemFormat(m_pItems[20], 1, MAKE_DWORD(pPEFile->m_PEHead.MinorSubsystemVersion, pPEFile->m_PEHead.MajorSubsystemVersion), WSTR("0x%08x"));
		TSPrintf(Buffer, "%d.%d", pPEFile->m_PEHead.MajorSubsystemVersion, pPEFile->m_PEHead.MinorSubsystemVersion);
		m_COFF.SetItemText(m_pItems[20], 2, Buffer);
		m_WIN.SetItemFormat(m_pItems[21], 1, pPEFile->m_PEHead.Win32VersionValue, WSTR("0x%08x"));
		m_WIN.SetItemFormat(m_pItems[22], 1, pPEFile->m_PEHead.SizeOfImage, WSTR("0x%08x"));
		TSPrintf(Buffer, "%d bytes", pPEFile->m_PEHead.SizeOfImage);
		m_WIN.SetItemText(m_pItems[22], 2, Buffer);
		m_WIN.SetItemFormat(m_pItems[23], 1, pPEFile->m_PEHead.SizeOfHeaders, WSTR("0x%08x"));
		m_WIN.SetItemFormat(m_pItems[24], 1, pPEFile->m_PEHead.CheckSum, WSTR("0x%08x"));
		m_WIN.SetItemFormat(m_pItems[25], 1, pPEFile->m_PEHead.Subsystem, WSTR("0x%08x"));
		m_WIN.SetItemFormat(m_pItems[26], 1, pPEFile->m_PEHead.DllCharacteristics, WSTR("0x%08x"));
		m_WIN.SetItemFormat(m_pItems[27], 1, pPEFile->m_PEHead.SizeOfStackReserve, WSTR("0x" F0ADDR "X")); //"0x%016I64X"
		m_WIN.SetItemFormat(m_pItems[28], 1, pPEFile->m_PEHead.SizeOfStackCommit, WSTR("0x" F0ADDR "X")); //"0x%016I64X"
		m_WIN.SetItemFormat(m_pItems[29], 1, pPEFile->m_PEHead.SizeOfHeapReserve, WSTR("0x" F0ADDR "X")); //"0x%016I64X"
		m_WIN.SetItemFormat(m_pItems[30], 1, pPEFile->m_PEHead.SizeOfHeapCommit, WSTR("0x" F0ADDR "X")); //"0x%016I64X"
		m_WIN.SetItemFormat(m_pItems[31], 1, pPEFile->m_PEHead.LoaderFlags, WSTR("0x%08x"));
		m_WIN.SetItemFormat(m_pItems[32], 1, pPEFile->m_PEHead.NumberOfRvaAndSizes, WSTR("0x%08x"));
 
		return true;
	}
//
	WISP_MSG_MAP_BEGIN(CPERelcationWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispWnd)

        WISP_EVENT_MAP_BEGIN(CPERelcationWnd)
//		WISP_EVENT_MAP(, OnEvent)
        WISP_EVENT_MAP_END

	bool CPERelcationWnd::OnCreate(WISP_MSG *pMsg)
	{
		m_ModuleList.CreateWnd(0, 5,5,372,294, this, 0x101, 0x46010006, WISP_SH_MINSIZE);
		m_EntryList.CreateWnd(0, 385,5,372,294, this, 0x102, 0x46010006, WISP_SH_MINSIZE);
		InitContext();
		return true;
	}

	bool CPERelcationWnd::InitContext()
	{
		m_ModuleList.InsertColumn(WSTR("RVA"), 165, 0, 0, -1);
		m_ModuleList.InsertColumn(WSTR("Items"), 90, 0, 0, -1);
		m_ModuleList.InsertColumn(WSTR("Owner"), 110, 0, 0, -1);

		m_EntryList.InsertColumn(WSTR("Offset"), 165, 0, 0, -1);
		m_EntryList.InsertColumn(WSTR("Types"), 90, 0, 0, -1);
		return true;
	}
//
	WISP_MSG_MAP_BEGIN(CPEImportModuleList)
		WISP_MSG_MAP(WISP_WM_MOUSE_DBLCLICK, OnMouseDblClick)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

	bool CPEImportModuleList::OnMouseDblClick(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetNext(0, 8);
		if (pMsg->KeyEvent.KeyType == VK_LBUTTON)
		{
			WISP_RECT Rect;
			GetItemRect(Item, 0, &Rect);
			if (pMsg->MsgMouseWndPT.x >= Rect.x && pMsg->MsgMouseWndPT.x <= Rect.x+Rect.cx)
			{
				ULONG_PTR Addr = GetItemValue(Item, 2);
				gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(Addr, 0);
			} else
			{
				ULONG_PTR Addr = GetItemValue(Item, 1);
				gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(Addr, 0);
			}
		}
		return true;
	}

	WISP_MSG_MAP_BEGIN(CPEImportEntryList)
		WISP_MSG_MAP(WISP_WM_MOUSE_DBLCLICK, OnMouseDblClick)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

	bool CPEImportEntryList::OnMouseDblClick(WISP_MSG *pMsg)	
	{
		CListStringItem *Item = GetNext(0, 8);
		if (pMsg->KeyEvent.KeyType == VK_LBUTTON)
		{
			WISP_RECT Rect;
			GetItemRect(Item, 0, &Rect);
			if (pMsg->MsgMouseWndPT.x >= Rect.x && pMsg->MsgMouseWndPT.x <= Rect.x+Rect.cx)
			{
				ULONG_PTR Addr = GetItemValue(Item, 0);
				gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(Addr, 0);
			} else
			{
				ULONG_PTR Addr = GetItemValue(Item, 2);
				gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(Addr, true);
			}
		}
		return true;
	}

	WISP_MSG_MAP_BEGIN(CPEImportWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispWnd)

        WISP_CMD_MAP_BEGIN(CPEImportWnd)
		WISP_CMD_MAP(0x103, OnCmdSelectEntry)
        WISP_CMD_MAP_END

        WISP_EVENT_MAP_BEGIN(CPEImportWnd)
//		WISP_EVENT_MAP(, OnEvent)
        WISP_EVENT_MAP_END

	bool CPEImportWnd::OnCreate(WISP_MSG *pMsg)
	{
		m_ModuleList.CreateWnd(0, 5,5,225,294, this, 0x103, 0x46010006, WISP_SH_MINSIZE);
		m_EntryList.CreateWnd(0, 240,5,510,294, this, 0x104, 0x46010006, WISP_SH_MINSIZE);
		InitContext();
		return true;
	}

	bool CPEImportWnd::OnCmdSelectEntry(WISP_MSG *pMsg)
	{
		CPEFile PEFile; //CMemPEFile
		if (pMsg->Command.CmdMsg == 0x80000105 && gpSyser->m_pDebugger)
		{
			bool bOpenOK = false;
			m_EntryList.DeleteAllChildrenItems(0);
			CListStringItem *Item = m_ModuleList.GetNext(0, 8);
			int Count = m_ModuleList.GetItemValue(Item, 0);
			if (Count < m_pPEFile->m_ImportModuleCount)
			{
				ULONG_PTR ImageBase = GetImageBase(m_pPEFile->m_ImportModule[Count].FileName);
				if (ImageBase)
					bOpenOK = PEFile.Open(ImageBase, 0x17, &CDebugger::Read, gpSyser->m_pDebugger);
				
				for (int i = 0; i < m_pPEFile->m_ImportModule[Count].FuncCount; ++i)
				{
					CListStringItem *Item = m_EntryList.InsertItemValue(m_pPEFile->m_ImportModule[Count].FuncList[i].ThunkAddr, 0, WSTR("0x" F0ADDR "X"), 0, nullptr, -1);
					m_EntryList.SetItemValue(Item, 0, m_pPEFile->m_ImportModule[Count].FuncList[i].ThunkAddr);
					if (bOpenOK)
					{
						unsigned long Index;
						ULONG_PTR Func = PEFile.GetExportFunc(m_pPEFile->m_ImportModule[Count].FuncList[i].FuncName, &Index);
						m_EntryList.SetItemFormat(Item, 1, Index, WSTR("0x%04X"));
						m_EntryList.SetItemFormat(Item, 2, Func, WSTR("0x" F0ADDR "X"));
						m_EntryList.SetItemValue(Item, 2, Func);
					}
					m_EntryList.SetItemText(Item, 3, m_pPEFile->m_ImportModule[Count].FuncList[i].FuncName);
				}
				if (bOpenOK)
					PEFile.Close();
			}
		}	
		return true;
	}

	bool CPEImportWnd::InitContext()
	{
		m_ModuleList.InsertColumn(WSTR("RVA"), 70, 0, 0, -1); //TODO
		m_ModuleList.InsertColumn(WSTR("Name"), 100, 0, 0, -1);
		m_ModuleList.InsertColumn(WSTR("Count"), 40, 0, 0, -1);

		m_EntryList.InsertColumn(WSTR("RVA"), 70, 0, 0, -1); //TODO
		m_EntryList.InsertColumn(WSTR("Hint"), 50, 0, 0, -1);
		m_EntryList.InsertColumn(WSTR("Entry Point"), 70, 0, 0, -1); //TODO
		m_EntryList.InsertColumn(WSTR("Name"), 270, 0, 0, -1);
		return true;
	}

	bool CPEImportWnd::Open(CPEFile *pPEFile)
	{
		m_pPEFile = pPEFile;
		m_ModuleList.DeleteAllChildrenItems(0);
		for (int i = 0; i < pPEFile->m_ImportModuleCount; ++i)
		{
			CListStringItem *Item = m_ModuleList.InsertItemValue(pPEFile->m_ImportModule[i].RVA, nullptr, WSTR("0x" F0ADDR "X"), 0, nullptr, -1);
			m_ModuleList.SetItemValue(Item, 2, pPEFile->m_ImportModule[i].RVA);
			m_ModuleList.SetItemText(Item, 1, pPEFile->m_ImportModule[i].FileName);
			m_ModuleList.SetItemValue(Item, 0, i);

			ULONG_PTR ImageBase = GetImageBase(pPEFile->m_ImportModule[i].FileName);
			m_ModuleList.SetItemValue(Item, 1, ImageBase);
			m_ModuleList.SetItemFormat(Item, 2, pPEFile->m_ImportModule[i].FuncCount, WSTR("%d"));
		}
		return true;
	}

	ULONG_PTR CPEImportWnd::GetImageBase(const char *szModuleName)
	{
		map<CMemoryImage, CDbgModule>::IT it = gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
		//while (it != gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.end())
		while (it)
		{
			if (!TStrICmp(it->second.m_pName, szModuleName))
				return it->second.m_ImageBase;
			++it;
		}
		return 0;
	}
//
	WISP_MSG_MAP_BEGIN(CPEExportList)
		WISP_MSG_MAP(WISP_WM_MOUSE_DBLCLICK, OnMouseDblClick)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

	bool CPEExportList::OnMouseDblClick(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetNext(0, 8);
		if (pMsg->KeyEvent.KeyType == VK_LBUTTON)
		{
			WISP_RECT Rect;
			GetItemRect(Item, 2, &Rect);
			if (pMsg->MsgMouseWndPT.x >= Rect.x && pMsg->MsgMouseWndPT.x <= Rect.x+Rect.cx)
			{
				ULONG_PTR Addr = GetItemValue(Item, 2);
				gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(Addr, 0);
			} else
			{
				ULONG_PTR Addr = GetItemValue(Item, 0);
				gpSyser->m_MainFrame.m_SystemExplorer.CodeGotoAddr(Addr, true);
			}
		}
		return true;
	}

	WISP_MSG_MAP_BEGIN(CPEExportWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispWnd)

        WISP_EVENT_MAP_BEGIN(CPEExportWnd)
//		WISP_EVENT_MAP(, OnEvent)
        WISP_EVENT_MAP_END

	bool CPEExportWnd::OnCreate(WISP_MSG *pMsg)
	{
		m_ExportList.CreateWnd(0, 5,5,736,294, this, 0x101, 0x46010006, WISP_SH_MINSIZE);
		InitContext();
		return true;
	}

	bool CPEExportWnd::InitContext()
	{
		m_ExportList.InsertColumn(WSTR("Entry Point"), 80, 0, 0, -1); //TODO
		m_ExportList.InsertColumn(WSTR("Ord"), 50, 0, 0, -1);
		m_ExportList.InsertColumn(WSTR("Thunk"), 80, 0, 0, -1); //TODO
		m_ExportList.InsertColumn(WSTR("Name"), 490, 0, 0, -1);
		return true;
	}

	bool CPEExportWnd::Open(CPEFile *pPEFile)
	{
		m_ExportList.DeleteAllChildrenItems(0);
		for (int i = 0; i < pPEFile->m_ExportFuncCount; ++i)
		{
			CListStringItem *Item = m_ExportList.InsertItemValue(pPEFile->m_ExportFunc[i].Address, nullptr, WSTR("0x" F0ADDR "X"), 0, nullptr, -1);
			m_ExportList.SetItemValue(Item, 0, pPEFile->m_ExportFunc[i].Address);
			m_ExportList.SetItemFormat(Item, 1, pPEFile->m_ExportFunc[i].FuncOrd, WSTR("%d"));
			m_ExportList.SetItemFormat(Item, 2, pPEFile->m_ExportFunc[i].Thunk, WSTR("0x" F0ADDR "X"));
			m_ExportList.SetItemValue(Item, 2, pPEFile->m_ExportFunc[i].Thunk);
			m_ExportList.SetItemText(Item, 3, pPEFile->m_ExportFunc[i].FuncName);
		}
		return true;
	}
//
	WISP_MSG_MAP_BEGIN(CPEDataDirectoryList)
		WISP_MSG_MAP(WISP_WM_MOUSE_DBLCLICK, OnMouseDblClick)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

	bool CPEDataDirectoryList::OnMouseDblClick(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetNext(0, 8);
		if (pMsg->KeyEvent.KeyType == VK_LBUTTON)
		{
			ULONG_PTR Addr = GetItemValue(Item, 1);
			gpSyser->m_MainFrame.m_SystemExplorer.DataGotoAddr(Addr, 0);
		}
		return true;
	}

	WISP_MSG_MAP_BEGIN(CPEDataDirectoryWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispWnd)

        WISP_EVENT_MAP_BEGIN(CPEDataDirectoryWnd)
//		WISP_EVENT_MAP(, OnEvent)
        WISP_EVENT_MAP_END

	bool CPEDataDirectoryWnd::OnCreate(WISP_MSG *pMsg)
	{
		m_DirectoryList.CreateWnd(0, 5,5,450,278, this, 0x105, 0x46010006, WISP_SH_MINSIZE);
		InitContext();
		return true;
	}

	bool CPEDataDirectoryWnd::InitContext()
	{
		m_DirectoryList.InsertColumn(WSTR("Directory Name"), 205, 0, 0, -1);
		m_DirectoryList.InsertColumn(WSTR("Virtual Address"), 90, 0, 0, -1); //TODO
		m_DirectoryList.InsertColumn(WSTR("Size"), 90, 0, 0, -1);

		m_pItems[0] = m_DirectoryList.InsertItem(WSTR("Export Table"), 0, 0, 0, -1);
		m_pItems[1] = m_DirectoryList.InsertItem(WSTR("Import Table"), 0, 0, 0, -1);
		m_pItems[2] = m_DirectoryList.InsertItem(WSTR("Resource Table"), 0, 0, 0, -1);
		m_pItems[3] = m_DirectoryList.InsertItem(WSTR("Exception Table"), 0, 0, 0, -1);
		m_pItems[4] = m_DirectoryList.InsertItem(WSTR("Certificate Table"), 0, 0, 0, -1);
		m_pItems[5] = m_DirectoryList.InsertItem(WSTR("Relocation Table"), 0, 0, 0, -1);
		m_pItems[6] = m_DirectoryList.InsertItem(WSTR("Debug Data"), 0, 0, 0, -1);
		m_pItems[7] = m_DirectoryList.InsertItem(WSTR("Architecture-specific data"), 0, 0, 0, -1);
		m_pItems[8] = m_DirectoryList.InsertItem(WSTR("Machine Value (MIPS GP)"), 0, 0, 0, -1);
		m_pItems[9] = m_DirectoryList.InsertItem(WSTR("TLS Table"), 0, 0, 0, -1);
		m_pItems[10] = m_DirectoryList.InsertItem(WSTR("Load Configuration Table"), 0, 0, 0, -1);
		m_pItems[11] = m_DirectoryList.InsertItem(WSTR("Bound Import Table"), 0, 0, 0, -1);
		m_pItems[12] = m_DirectoryList.InsertItem(WSTR("Import Address Table"), 0, 0, 0, -1);
		m_pItems[13] = m_DirectoryList.InsertItem(WSTR("Delay Import Descriptor"), 0, 0, 0, -1);
		m_pItems[14] = m_DirectoryList.InsertItem(WSTR("COM+ Runtime Header"), 0, 0, 0, -1);
		m_pItems[15] = m_DirectoryList.InsertItem(WSTR("(15) Reserved"), 0, 0, 0, -1);
		return true;
	}

	bool CPEDataDirectoryWnd::Open(CPEFile *pPEFile)
	{
		for (int i = 0; i < pPEFile->m_PEHead.NumberOfRvaAndSizes; ++i)
		{
			ULONG_PTR Addr = pPEFile->m_PEHead.DataDirectory[i].VirtualAddress;
			if (Addr) Addr += pPEFile->m_ImageBase;
			m_DirectoryList.SetItemFormat(m_pItems[i], 1, Addr, WSTR("0x" F0ADDR "X"));
			m_DirectoryList.SetItemValue(m_pItems[i], 1, Addr);
			m_DirectoryList.SetItemFormat(m_pItems[i], 2, pPEFile->m_PEHead.DataDirectory[i].Size, WSTR("0x%08X"));
		}
		return true;
	}

WISP_FORM_RES_ITEM PEExplorerForm[] =
{
	{WISP_CTRL_FORM, {0, 0, 780, 390}, 0, 0xB, WSTR("PE Explorer"), (void*)"\\SyserIcon\\Syser.ico", 0},
	{WISP_CTRL_COMBO_BOX, {5, 5, 135, 20}, 3, 0x9040006, 0, 0, 0},
	{WISP_CTRL_TAB, {5, 30, 765, 330}, 2, 6, 0, 0, 0},
	{0},
};

	WISP_MSG_MAP_BEGIN(CPEExplorerForm)
		WISP_MSG_MAP(WISP_WM_CREATE_FORM, OnCreateForm)
		WISP_MSG_MAP_ON_COMMAND
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispForm)

        WISP_CMD_MAP_BEGIN(CPEExplorerForm)
		WISP_CMD_MAP(3, OnCmdModuleSelect)
        WISP_CMD_MAP_END

        WISP_EVENT_MAP_BEGIN(CPEExplorerForm)
//		WISP_EVENT_MAP(, OnEvent)
        WISP_EVENT_MAP_END

	CPEExplorerForm::CPEExplorerForm()
	{
		m_Resource = PEExplorerForm;
		m_pTabWnd = 0;
		m_pPEHeaderWnd = 0;
		m_pPERelcationWnd = 0;
		m_pPEImportWnd = 0;
		m_pPEExportWnd = 0;
	}
	CPEExplorerForm::~CPEExplorerForm()
	{
	}

	bool CPEExplorerForm::OnCreateForm(WISP_MSG *pMsg)
	{
		m_pTabWnd = (CWispTabWnd *)GetFormItem(2);

		m_pPEHeaderWnd = new CPEHeaderWnd;
		m_pPEHeaderWnd->CreateWnd(0, 0,0,0,0, m_pTabWnd, 4, 0, WISP_SH_MINSIZE);
		m_pPEHeaderWnd->SetOwner(this);
		m_pTabWnd->InsertWnd(WSTR("PE Header"), m_pPEHeaderWnd, 0);
		m_pPEHeaderWnd->m_AdvStyle |= 1;

		m_pModuleList = (CWispComboBox *)GetFormItem(3);

		if (gpSyser->m_pDebugger)
		{
		map<CMemoryImage, CDbgModule>::IT it = gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.begin();
		//while (it != gpSyser->m_pDebugger->m_DbgModuleList.m_DbgModuleMap.end())
		while (it)
		{
			CListStringItem *Item = m_pModuleList->m_ComboList.InsertItemStr(it->second.m_pName, 0, 0, 0, -1);
			m_pModuleList->m_ComboList.SetItemValue(Item, 0, it->second.m_ImageBase);
			++it;
		}
		}
		m_pModuleList->SetCurSel(0);

		m_pPEDataDirectoryWnd = new CPEDataDirectoryWnd;
		m_pPEDataDirectoryWnd->CreateWnd(0, 0,0,0,0, m_pTabWnd, 0x105, 0, WISP_SH_MINSIZE);
		m_pPEDataDirectoryWnd->SetOwner(this);
		m_pTabWnd->InsertWnd(WSTR("Data Directories"), m_pPEDataDirectoryWnd, 0);
		m_pPEDataDirectoryWnd->m_AdvStyle |= 1;

		m_pPERelcationWnd = new CPERelcationWnd;
		m_pPERelcationWnd->CreateWnd(0, 0,0,0,0, m_pTabWnd, 5, 0, WISP_SH_MINSIZE);
		m_pPERelcationWnd->SetOwner(this);
		m_pTabWnd->InsertWnd(WSTR("Relocations"), m_pPERelcationWnd, 0);
		m_pPERelcationWnd->m_AdvStyle |= 1;

		m_pPEImportWnd = new CPEImportWnd;
		m_pPEImportWnd->CreateWnd(0, 0,0,0,0, m_pTabWnd, 6, 0, WISP_SH_MINSIZE);
		m_pPEImportWnd->SetOwner(this);
		m_pTabWnd->InsertWnd(WSTR("Imports"), m_pPEImportWnd, 0);
		m_pPEImportWnd->m_AdvStyle |= 1;

		m_pPEExportWnd = new CPEExportWnd;
		m_pPEExportWnd->CreateWnd(0, 0,0,0,0, m_pTabWnd, 7, 0, WISP_SH_MINSIZE);
		m_pPEExportWnd->SetOwner(this);
		m_pTabWnd->InsertWnd(WSTR("Exports"), m_pPEExportWnd, 0);
		m_pPEExportWnd->m_AdvStyle |= 1;

		return true;
	}

	bool CPEExplorerForm::OnCmdModuleSelect(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CmdMsg == 0x80000400)
		{
			ULONG_PTR Addr = m_pModuleList->m_ComboList.GetItemValue((CListStringItem *)pMsg->Command.Param2, 0);
			Open(Addr);
		}
		return true;
	}
	bool CPEExplorerForm::SelectModule(ULONG_PTR Address)
	{
		unsigned int Index = 0;
		unsigned int Count = m_pModuleList->m_ComboList.GetItemCount(0, 1);
		for (Index = 0; Index < Count; ++Index)
		{
			CListStringItem *Item = m_pModuleList->m_ComboList.GetItem(Index, 0);
			if (!Item) break;
			if (Address == m_pModuleList->m_ComboList.GetItemValue(Item, 0))
			{
				m_pModuleList->SetCurSel(Index);
				break;
			}
		}
		return true;
	}

	bool CPEExplorerForm::OpenModule(ULONG_PTR Address)
	{
		if (m_MemPEFile.m_hHandle || m_MemPEFile.m_Buffer)
			m_MemPEFile.Close();
			//PE_OPEN_SYS | PE_OPEN_DLL | 1
		return m_MemPEFile.Open(Address, 7, &CDebugger::Read, gpSyser->m_pDebugger);
	}

	void CPEExplorerForm::Open(ULONG_PTR Address)
	{
		if (IsWindow() && OpenModule(Address))
		{
			SelectModule(Address);
			m_pPEHeaderWnd->Open(&m_MemPEFile);
			m_pPEImportWnd->Open(&m_MemPEFile);
			m_pPEExportWnd->Open(&m_MemPEFile);
			m_pPEDataDirectoryWnd->Open(&m_MemPEFile);
		}
	}

	void CPEExplorerForm::Create()
	{
		if (IsWindow())
			Show(WISP_SH_MINSIZE);
		else	CWispForm::Create(0, WISP_SH_MINSIZE);
	}

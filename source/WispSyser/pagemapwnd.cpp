
#include "../Code/fileiont.hpp"
#include "pagemapwnd.hpp"

        WISP_MSG_MAP_BEGIN(CPageMapWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_CLOSE, OnClose)
		WISP_MSG_MAP_ON_COMMAND
        WISP_MSG_MAP_END(CWispSplitWnd)


//      WISP_COMMAND_MAP_BEGIN(CPageMapWnd)
//              case default: return CWispWnd::OnCommand(pMsg);
//      WISP_COMMAND_MAP_END

	CPageMapWnd::CPageMapWnd()
	{
		_field_260 = 0;
	#if 0
		m_PageTableBaseAddress = 0xC0000000;// PAGE_TABLE
		if (GETX86CR4(-1) & 0x20)
			m_PageDirectBaseAddress = 0xC0600000;// PAGE_DIR_PAE
		else	m_PageDirectBaseAddress = 0xC0300000;// PAGE_DIR
	#else
		m_PageTableBaseAddress = 0;
		m_PageDirectBaseAddress = 0;
	#endif
	}
	CPageMapWnd::~CPageMapWnd()
	{
	}

	bool CPageMapWnd::OnCreate(WISP_MSG *pMsg)
	{
		WISP_RECT Rect;

		Rect.x = 0;
		Rect.y = 0;
		Rect.cx = m_ClientRect.cx / 2;
		Rect.cy = m_ClientRect.cy;

		m_PageDirectList.CreateWnd(0, Rect, this, 1, 0x10000, WISP_SH_MINSIZE);
		m_PageDirectList.InsertColumn(WSTR("Page Table Physical Address"), 68, 0, 0, -1);
		m_PageDirectList.InsertColumn(WSTR("Lines Address"), 120, 0, 0, -1);
		m_PageDirectList.InsertColumn(WSTR("Attribute"), 120, 0, 0, -1);
		m_PageDirectList.InsertColumn(WSTR(""), 80, 0, 0, -1);

		Rect.x = m_ClientRect.cx / 2;
		Rect.y = 0;
		Rect.cx = Rect.x;
		Rect.cy = m_ClientRect.cy;

		m_PageTableList.CreateWnd(0, Rect, this, 2, 0x10000, WISP_SH_MINSIZE);
		m_PageTableList.InsertColumn(WSTR("Lines Address"), 120, 0, 0, -1);
		m_PageTableList.InsertColumn(WSTR("Physical"), 80, 0, 0, -1);
		m_PageTableList.InsertColumn(WSTR("Attribute"), 120, 0, 0, -1);
		m_PageTableList.InsertColumn(WSTR(""), 80, 0, 0, -1);

		InsertWnd(&m_PageDirectList, 0, 100, -1);
		InsertWnd(&m_PageTableList, 0, 100, -1);

		AdjustWndPos();

	#ifndef _RING0_
		CLocalFileIO File;
		HANDLE Handle;
		if (!File.OpenFile("PageTable.bin", &Handle, 0x10000))
		{
			m_PageTableBaseAddress = 0;
			m_PageDirectBaseAddress = 0;
			return true;
		}
		unsigned long len = File.GetFileLength(Handle);
		char *ptr = new char[len];
		m_PageTableBaseAddress = (unsigned long*)ptr;
		File.ReadFile(Handle, ptr, len);
		m_PageDirectBaseAddress = (unsigned long*)(ptr + 0x300000);
		File.CloseHandle(Handle);
	#endif

		InitContext();
		return true;
	}

	bool CPageMapWnd::OnDestroy(WISP_MSG *pMsg)
	{
	#ifndef _RING0_
		delete []m_PageTableBaseAddress;
	#endif
		return true;
	}

	bool CPageMapWnd::OnCommand(WISP_MSG *pMsg)
	{
		if (pMsg->Command.CtrlType == 11 && //WISP_CTRL_LIST
		    pMsg->Command.CmdMsg == 0x80000105 && //WISP_WM_UPDATE_HORZ_SCROLL_BAR
		    pMsg->Command.CmdID == 1)
		{
			m_PageTableList.DeleteAllChildrenItems(nullptr);
			if (m_PageTableBaseAddress)
				InsertPageTableList(pMsg);
		}
		return true;
	}

	bool CPageMapWnd::InsertPageTableList(WISP_MSG *pMsg)
	{
		//TODO
		return true;
	}

	void CPageMapWnd::Create()
	{
		if (IsWindow())
		{
			Destroy();
		} else
		{
			WISP_RECT Rect;
			Rect.x = 0;
			Rect.y = 0;
			Rect.cx = 400;
			Rect.cy = 300;
			CreateWnd(WSTR("Page Map"), Rect, 0, 0, 0x200BB, WISP_SH_MINSIZE);
			Center(nullptr);
		}
	}

	void CPageMapWnd::InitContext()
	{
		//TODO
	}


#include <stdarg.h>
#include "../Code/sysdep.hpp"
#include "winwisp.hpp"
#include "../wisp/wispbasewnd.hpp"

//#include <winuser.h>
//#include <windowsx.h>
//#include <winuser.h>

	CWinWisp *pWinWisp = NULL;


	CWinWisp::CWinWisp()
	{
		pWinWisp = this;

		m_hRResize = 0;
		m_hLResize = 0;
		m_hHResize = 0;
		m_hVResize = 0;
		m_hDrag = 0;
		m_hArrow = 0;

		m_dwStyle = WS_VISIBLE|WS_CAPTION|WS_SYSMENU|WS_SIZEBOX|WS_GROUP|WS_MAXIMIZEBOX; //0x10CF0000;
		m_dwExStyle = 0;

		m_pTaskBarIconName = 0;
		m_hTaskBarIcon = 0;
	}

	CWinWisp::~CWinWisp()
	{
	}

	bool CWinWisp::UnregisterTaskBar(int)
	{
		NOTIFYICONDATAA data;

		data.cbSize = sizeof(NOTIFYICONDATAA);
		data.hWnd = m_hWnd;
		data.uID = 0;

		if (m_hTaskBarIcon)
		{
			::DestroyIcon(m_hTaskBarIcon);
			m_hTaskBarIcon = 0;
		}
		return ::Shell_NotifyIconA(NIM_DELETE, &data);
	}

	bool CWinWisp::RegisterTaskBar(const char *HintName, const char *IconName)
	{
		NOTIFYICONDATAA data;

		m_pTaskBarIconName = IconName;
		m_TaskBarHintName = HintName;

		data.cbSize = sizeof(NOTIFYICONDATAA);
		data.hWnd = m_hWnd;
		data.uID = 0;
		data.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
		data.uCallbackMessage = 0x500; //WM_USER+
		data.hIcon = ::LoadIconA(m_hInstance, IconName);

		m_hTaskBarIcon = data.hIcon;
		TStrCpy(data.szTip, HintName);

		return ::Shell_NotifyIconA(NIM_ADD, &data);
	}

	bool CWinWisp::UpdateTaskBar(const char *IconName)
	{
		NOTIFYICONDATAA data;

		data.cbSize = sizeof(NOTIFYICONDATAA);
		data.hWnd = m_hWnd;
		data.uID = 0;
		data.uFlags = NIF_ICON;

		if (m_hTaskBarIcon)
			::DestroyIcon(m_hTaskBarIcon);

		data.hIcon = ::LoadIconA(m_hInstance, IconName);
		m_hTaskBarIcon = data.hIcon;

		return ::Shell_NotifyIconA(NIM_MODIFY, &data);
	}

	bool CWinWisp::CreateFrame(const WCHAR *WndName, int Width, int Height, int Bpp)
	{
		RECT Rect;
		::GetWindowRect(GetDesktopWindow(), &Rect);
		DesktopSize.cx = Rect.right - Rect.left;
		DesktopSize.cy = Rect.bottom - Rect.top;

		if (!m_hArrow)
			m_hArrow = ::LoadCursorA(0, IDC_ARROW);

		if (!m_hDrag)
			m_hDrag = ::LoadCursorA(0, IDC_ARROW); //IDC_HANG

		if (!m_hVResize)
			m_hVResize = ::LoadCursorA(0, IDC_SIZENS);
		if (!m_hHResize)
			m_hHResize = ::LoadCursorA(0, IDC_SIZEWE);
		if (!m_hLResize)
			m_hLResize = ::LoadCursorA(0, IDC_SIZENWSE);
		if (!m_hRResize)
			m_hRResize = ::LoadCursorA(0, IDC_SIZENESW);

		WNDCLASSEXA ExClass;

		ExClass.cbSize = sizeof(WNDCLASSEXA);
		ExClass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW; 
		ExClass.lpfnWndProc = CWinWisp::DefaultWindowProc;
		ExClass.cbClsExtra = 0;
		ExClass.cbWndExtra = 0;
		ExClass.hInstance = m_hInstance;
		ExClass.hIcon = m_hWindowClassIcon;
		ExClass.hCursor = m_hArrow;
		ExClass.hbrBackground = 0;
		ExClass.lpszMenuName = 0;
		ExClass.lpszClassName = "WispWindowClass";
		ExClass.hIconSm = 0;
		::RegisterClassExA(&ExClass);

		char WindowName[256];
		UnicodeToAnsi(WndName, WindowName, lenof(WindowName));

		m_hWnd = ::CreateWindowExA(m_dwExStyle, "WispWindowClass", WindowName, m_dwStyle,
				CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, 0, 0, m_hInstance, 0);

		RECT ClientRect;
		RECT WindowRect;

		::GetClientRect(m_hWnd, &ClientRect);
		::GetWindowRect(m_hWnd, &WindowRect);

		m_PaintRect.x = WindowRect.right - WindowRect.left - (ClientRect.right - ClientRect.left);
		m_PaintRect.y = WindowRect.bottom - WindowRect.top - (ClientRect.bottom - ClientRect.top);

		if (m_nShowCmd == 3) //SW_MAXIMIZE
		{
			Width = ::GetSystemMetrics(SM_CXFULLSCREEN);
			Height = ::GetSystemMetrics(SM_CYFULLSCREEN);
		}

		::SetWindowPos(m_hWnd, 0, (GetSystemMetrics(SM_CXFULLSCREEN) - m_PaintRect.x - Width) / 2,
					(GetSystemMetrics(SM_CYFULLSCREEN) - m_PaintRect.y - Height) / 2,
					m_PaintRect.x + Width, m_PaintRect.y + Height, 
					SWP_NOACTIVATE | SWP_NOZORDER);

		if (m_dwStyle & WS_VISIBLE)
		{
			::ShowWindow(m_hWnd, m_nShowCmd);
			::UpdateWindow(m_hWnd);
		}

		m_PaintRect.cy = 0;
		m_PaintRect.cx = 0;
		m_hWndDC = ::GetDC(m_hWnd);
		m_hPaintDC = ::CreateCompatibleDC(m_hWndDC);

		CreateFrameBuffer(Width, Height);
		return true;
	}

	bool CWinWisp::BeginResize(int Width, int Height)
	{
		CreateFrameBuffer(Width, Height);
		return true;
	}

	bool CWinWisp::EndResize(int Width, int Height)
	{
		::SetWindowPos(m_hWnd, 0,  0, 0, m_PaintRect.x + Width, m_PaintRect.y + Height, 
				SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
		return true;
	}

	void CWinWisp::DestroyFrame()
	{
		if (m_hArrow)
			::DestroyCursor(m_hArrow);

		if (m_hDrag)
			::DestroyCursor(m_hDrag);

		if (m_hVResize)
			::DestroyCursor(m_hVResize);
		if (m_hHResize)
			::DestroyCursor(m_hHResize);
		if (m_hLResize)
			::DestroyCursor(m_hLResize);
		if (m_hRResize)
			::DestroyCursor(m_hRResize);
		::DestroyWindow(m_hWnd);
	}

	void CWinWisp::BeginDrag()
	{
		::SetCapture(m_hWnd);
	}

	void CWinWisp::EndDrag()
	{
		::ReleaseCapture();
	}

	void CWinWisp::BeginDraw(WISP_RECT *pRect)
	{
		::GdiFlush();
	}

	void CWinWisp::EndDraw(WISP_RECT *pRect)
	{
		if (!pRect) pRect = &gpCurWisp->m_pRootWnd->m_WindowRect;

		::BitBlt(m_hWndDC, pRect->x, pRect->y, pRect->cx, pRect->cy,
			m_hPaintDC, pRect->x, pRect->y, SRCCOPY);

		//StretchBlt(m_hWndDC, pRect->x, pRect->y, pRect->cx, pRect->cy,
		//	m_hPaintDC, pRect->x, pRect->y, src_cx, src_cy, SRCCOPY);

		//AlphaBlt(
		//TransparentBlt(
	}

	const WCHAR *CWinWisp::GetClipboard()
	{
		if (!::IsClipboardFormatAvailable(CF_TEXT))
			return m_ClipString; //cstr

		::OpenClipboard(m_hWnd);

		HANDLE hClipboard = ::GetClipboardData(CF_TEXT);
		const char *buffer = (const char *)::GlobalLock(hClipboard);
		if (buffer)
		{
			int bufferLen = TStrLen(buffer) + 1;
			m_ClipString.Preallocate(bufferLen, 0);
			m_ClipString.Truncate( AnsiToUnicode(buffer, m_ClipString.data(), bufferLen) );
			::GlobalUnlock(hClipboard);
			::CloseClipboard();
			return CWispBase::GetClipboard();
		}

		::CloseClipboard();
		return m_ClipString; //cstr
	}

	bool CWinWisp::SetClipboard(const WCHAR *Text)
	{
		CWispBase::SetClipboard(Text);
		if (::OpenClipboard(m_hWnd))
		{
			::EmptyClipboard();
			int TextLen = TStrLen(Text)+1;
			HGLOBAL clipbuffer = ::GlobalAlloc(0x2042, TextLen*2); //GMEM_DDESHARE | GHND //GMEM_MOVEABLE | GMEM_ZEROINIT
			char *buffer = (CHAR *)::GlobalLock(clipbuffer);
			UnicodeToAnsi(Text, buffer, TextLen*2);
			::GlobalUnlock(clipbuffer);
			::SetClipboardData(CF_TEXT, clipbuffer);
			::CloseClipboard();
		}
		return true;
	}
		#define MYBPP 32
	void CWinWisp::CreateFrameBuffer(int Width, int Height)
	{
		if (Width > 0 && Height > 0)
		{
			m_FrameBufferInfo.Bpp = MYBPP;
			m_FrameBufferInfo.Width = Width;
			m_FrameBufferInfo.Height = Height;

			if (m_PaintRect.cx >= Width &&
			    m_PaintRect.cy >= Height
				&& m_PaintRect.cx < Width  + MYBPP &&
				   m_PaintRect.cy < Height + MYBPP)
			{
				return;
			}

			if (m_PaintRect.cx < Width + MYBPP) //CLAMP
				m_PaintRect.cx = Width + MYBPP;
			else	m_PaintRect.cx = Width;

			if (m_PaintRect.cy < Height + MYBPP) //CLAMP
				m_PaintRect.cy = Height + MYBPP;
			else	m_PaintRect.cy = Height;

			BITMAPINFO bitmap;

			bitmap.bmiHeader.biBitCount = MYBPP;
			bitmap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bitmap.bmiHeader.biCompression = BI_RGB;
			bitmap.bmiHeader.biPlanes = 1;
			bitmap.bmiHeader.biSizeImage = 0;
			bitmap.bmiHeader.biClrUsed = 0;
			bitmap.bmiHeader.biClrImportant = 0;
			bitmap.bmiHeader.biHeight = -m_PaintRect.cy;
			bitmap.bmiHeader.biWidth = m_PaintRect.cx;

			m_hPaintBitmap = ::CreateDIBSection(NULL, &bitmap, DIB_RGB_COLORS,
					(void **)&m_FrameBufferInfo.VideoBuffer, NULL, 0);

			::DeleteObject(SelectObject(m_hPaintDC, m_hPaintBitmap));

			m_FrameBufferInfo.LineDistance = m_PaintRect.cx * (m_FrameBufferInfo.Bpp / 8);
			m_FrameBufferInfo.VideoBufferSize = m_PaintRect.cy * m_FrameBufferInfo.LineDistance;

			//DebugPrintf("%dx%d, %d, %d", Width, Height, MYBPP, m_FrameBufferInfo.LineDistance);
		}
	}

	void CWinWisp::SetCursor(unsigned int CursorType, WISP_POINT *pHotPT, WISP_SIZE *pSize, CWispDIB *pDIB)
	{
		if (m_bHideMouse)
		{
			switch (CursorType)
			{
			case WISP_CT_ARROW:
				::SetClassLongPtrA(m_hWnd, GCLP_HCURSOR, (LONG_PTR)m_hArrow);
				break;
			case WISP_CT_RESIZE_VERT:
				::SetClassLongPtrA(m_hWnd, GCLP_HCURSOR, (LONG_PTR)m_hVResize);
				break;
			case WISP_CT_RESIZE_HORZ:
				::SetClassLongPtrA(m_hWnd, GCLP_HCURSOR, (LONG_PTR)m_hHResize);
				break;
			case WISP_CT_DRAG:
				::SetClassLongPtrA(m_hWnd, GCLP_HCURSOR, (LONG_PTR)m_hDrag);
				break;
			case WISP_CT_RESIZE_LEFT_LEAN:
				::SetClassLongPtrA(m_hWnd, GCLP_HCURSOR, (LONG_PTR)m_hLResize);
				break;	
			case WISP_CT_RESIZE_RIGHT_LEAN:
				::SetClassLongPtrA(m_hWnd, GCLP_HCURSOR, (LONG_PTR)m_hRResize);
				break;
			default:
				return;
			}
		} else
		{
			CWispBase::SetCursor(CursorType, pHotPT, pSize, pDIB);
		}
	}

	void CWinWisp::ChangeCursor(LONG_PTR Style, unsigned int CursorType, WISP_POINT *pHotPT, WISP_SIZE *pSize)
	{
		ICONINFO Info;

		if (::GetIconInfo((HICON)Style, &Info))
		{
			pHotPT->x = Info.xHotspot;
			pHotPT->y = Info.yHotspot;
			pSize->cy = 0;
			pSize->cx = 0;
			::SetClassLongPtrA(m_hWnd, GCLP_HCURSOR, Style);
		}
	}

	int CWinWisp::WinMainStart(HINSTANCE hInstance, const char* lpCmdLine, int nShowCmd)
	{
		m_hInstance = hInstance;
		m_nShowCmd = nShowCmd;
		m_lpCmdLine = lpCmdLine;

		::InitializeCriticalSection(&m_hCriticalSection);
		int Result = 0;

		if (Start())
		{
			Pump();
			Result = Stop();
		}

		::DeleteCriticalSection(&m_hCriticalSection);
		return Result;
	}

	bool CWinWisp::WakeUpMsgThread()
	{
		if (m_bInCriticalSection)
			::PostMessageA(m_hWnd, 0x410, 0, 0);
		return true;
	}

	bool CWinWisp::SleepMsgThread()
	{
		MSG Msg;

		if (m_bActive && ::GetMessageA(&Msg, 0, 0, 0))
		{
			::TranslateMessage(&Msg);
			::DispatchMessageA(&Msg);
			return true;
		}
		m_bActive = false;
		return true;
	}


	bool CWinWisp::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CREATE:
			pWinWisp->m_TaskbarWM = ::RegisterWindowMessageA("TaskbarCreated");
			::ShowCursor(pWinWisp->m_bHideMouse);
			::SetTimer(hWnd, 0, 30, 0);
			break;

		case WM_DESTROY:
			pWinWisp->m_bActive = false;
			break;

		case WM_ACTIVATE:
			if (pWinWisp->m_pRootWnd)
			{
				pWinWisp->ResetInput();
				pWinWisp->EndDraw(0);
			}
			break;

		case WM_ERASEBKGND:
			if (pWinWisp->m_pRootWnd)
				pWinWisp->EndDraw(0);
			break;

		case WM_NCCALCSIZE:
			if (pWinWisp->m_pRootWnd)
			{
				if (wParam)
				{
					RECT *rect = (RECT*)lParam;

					int x = rect->right - rect->left - pWinWisp->m_PaintRect.x;
					int y = rect->bottom - rect->top - pWinWisp->m_PaintRect.y;

					if (x > 0 && y > 0)
					{
						pWinWisp->EnterMsgCriticalSection();
						pWinWisp->m_pRootWnd->m_State = 2; //WISP_WSTATE_RESIZE
						pWinWisp->m_pRootWnd->Resize(x, y, true);
						pWinWisp->m_pRootWnd->m_State = WISP_WSTATE_RESET;
						pWinWisp->Update();
						pWinWisp->LeaveMsgCriticalSection();
					}
				}
			}
			return false;	//continue pass to windows proc

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (pWinWisp->m_pRootWnd)
			{
				for (int n = 0; n < LOWORD(lParam); ++n)
					pWinWisp->SendKeyEvent(wParam, true, HIWORD(lParam));
			}
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (pWinWisp->m_pRootWnd)
			{
				for (int n = 0; n < LOWORD(lParam); ++n)
					pWinWisp->SendKeyEvent(wParam, false, HIWORD(lParam));
			}
			break;

		case WM_TIMER:
			pWinWisp->SendTimer(20);
			break;

		case WM_MOUSEMOVE:
			if (pWinWisp->m_pRootWnd)
				pWinWisp->SendMousePosChg(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

		case WM_LBUTTONDOWN:
			if (pWinWisp->m_pRootWnd)
				pWinWisp->SendKeyEvent(1, true, 0); //VK_LBUTTON, true, 0
			break;

		case WM_LBUTTONUP:
			if (pWinWisp->m_pRootWnd)
				pWinWisp->SendKeyEvent(1, false, 0); //VK_LBUTTON, false, 0
			break;

		case WM_LBUTTONDBLCLK:
			if (pWinWisp->m_pRootWnd)
				pWinWisp->SendMouseDblClick(1); //VK_LBUTTON
			break;

		case WM_RBUTTONDOWN:
			if (pWinWisp->m_pRootWnd)
				pWinWisp->SendKeyEvent(2, true, 0); //VK_RBUTTON, true, 0
			break;

		case WM_RBUTTONUP:
			if (pWinWisp->m_pRootWnd)
				pWinWisp->SendKeyEvent(2, false, 0); //VK_RBUTTON, false, 0
			break;

		case WM_RBUTTONDBLCLK:
			if (pWinWisp->m_pRootWnd)
				pWinWisp->SendMouseDblClick(2); //VK_RBUTTON
			break;

		case WM_MOUSEWHEEL:
			if (pWinWisp->m_pRootWnd)
				pWinWisp->SendMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			break;

		case 0x410: //WakeUpMsgThread
			break;

		//case 0x500: //???

		default:
			if (uMsg != pWinWisp->m_TaskbarWM)
				return false; //contiue pass to windows proc

			if (pWinWisp->m_pTaskBarIconName)
				pWinWisp->RegisterTaskBar(pWinWisp->m_TaskBarHintName, pWinWisp->m_pTaskBarIconName);
			break;
		}
		return true; //stop pass to windows proc
	}

	LRESULT CALLBACK CWinWisp::DefaultWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (pWinWisp->WindowProc(hWnd, uMsg, wParam, lParam))
			return 0;

		return ::DefWindowProcA(hWnd, uMsg, wParam, lParam);
	}

	void CWinWisp::EnterMsgCriticalSection()
	{
		::EnterCriticalSection(&m_hCriticalSection);
	}

	void CWinWisp::LeaveMsgCriticalSection()
	{
		::LeaveCriticalSection(&m_hCriticalSection);
	}

#include <Shlobj.h>

	bool CWinWisp::GetOpenFolderName(WCHAR *Path, const WCHAR *Title)
	{
		IMalloc *Iface;

		if (FAILED(::SHGetMalloc(&Iface)))
			return false;

		BROWSEINFOW bi;

		bi.hwndOwner = m_hWnd;
		bi.pidlRoot = 0;
		bi.pszDisplayName = 0;
		bi.lpszTitle = (LPCWSTR)Title;
		bi.ulFlags = 0x49;	//??? BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS
		bi.lpfn = 0;
		bi.lParam = 0;
		bool Result = false;
		ITEMIDLIST *pidl = ::SHBrowseForFolderW(&bi);
		if (pidl)
		{
			Result = ::SHGetPathFromIDListW(pidl, (LPWSTR)Path) != 0;
			Iface->Free(pidl);
		}
		Iface->Release();
		return Result;
	}

	bool CWinWisp::GetOpenFileName(const WCHAR *lpstrInitialDir, const WCHAR *lpstrFilter, int, WCHAR *lpstrFile, int nMaxFile)
	{
		WCHAR Filter[MAX_FN_LEN];
		OPENFILENAMEW ofn;

		memset(&ofn, 0, sizeof(ofn)); //0x58
		ofn.lStructSize = sizeof(ofn); //0x58
		ofn.hInstance = m_hInstance;
		ofn.hwndOwner = m_hWnd;
		TStrCpy(Filter, lpstrFilter);
		int Len = TStrLen(Filter);
		MAX_LIMIT(Len, MAX_FN_LEN);
		for (int i = 0; i < Len; ++i)
		{
			if (Filter[Len-i] == '|') //-1 ???
				Filter[Len-i] = 0; //-1 ???
		}
		ofn.lpstrFilter = (LPCWSTR)Filter;
		ofn.nMaxFile = nMaxFile;
		ofn.lpstrFile = (LPWSTR)lpstrFile;
		ofn.lpstrInitialDir = (LPCWSTR)lpstrInitialDir;
		*lpstrFile = 0;
		return ::GetOpenFileNameW(&ofn) != 0;
	}


	void CWinWisp::ReportMsg(const WCHAR *Format, ...)
	{
		WCHAR Buffer[512];
		va_list va;
		va_start(va, Format);
		TSPrintf(Buffer, Format, va);
		va_end(va);

		if (!m_pRootWnd)
		{
			wprintf(WSTR("%s"), Buffer);
			return;
		}
		::OutputDebugStringW((LPCWSTR)Buffer);
	}



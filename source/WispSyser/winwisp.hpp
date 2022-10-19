
#ifndef _WINWISP_HPP_
#define _WINWISP_HPP_

//#include "../Code/osdefine.hpp"
#include "../Code/define.h"

#include "../Wisp/wispapp.hpp"

//#include <stdio.h>


//#define _WIN32_WINDOWS 0x0410
//#define _WIN32_WINNT 0x0500

//#define WIN32_LEAN_AND_MEAN

//#include <winresrc.h>
//#include <winuser.h>

//#include <windows.h>
//#include <windowsx.h>

	struct CWinWisp : public CWispApp
	{
		long m_dwStyle;
		long m_dwExStyle;
		HWND m_hWnd;
		HINSTANCE m_hInstance;
		HICON m_hWindowClassIcon;
		HICON m_hTaskBarIcon;

		int m_nShowCmd;
		const char *m_lpCmdLine;

		HDC m_hWndDC;
		HDC m_hPaintDC;
		HBITMAP m_hPaintBitmap;

		HCURSOR m_hArrow;
		HCURSOR m_hDrag;
		HCURSOR m_hVResize;
		HCURSOR m_hHResize;
		HCURSOR m_hLResize;
		HCURSOR m_hRResize;

		WISP_RECT m_PaintRect;
		CRITICAL_SECTION m_hCriticalSection;
		TAnsiString m_TaskBarHintName;
		const char *m_pTaskBarIconName;
		long m_TaskbarWM;
		WISP_SIZE DesktopSize;


	public:
		CWinWisp();
		virtual  ~CWinWisp();

		virtual bool CreateFrame(const WCHAR *WndName, int Width, int Height, int Bpp) override;
		virtual void DestroyFrame() override;

		virtual bool EndResize(int Width, int Height) override;
		virtual bool BeginResize(int Width, int Height) override;

		virtual void BeginDrag() override;
		virtual void EndDrag() override;

		virtual bool WakeUpMsgThread() override;
		virtual bool SleepMsgThread() override;

		virtual void EnterMsgCriticalSection() override;
		virtual void LeaveMsgCriticalSection() override;

		virtual void BeginDraw(WISP_RECT *pRect) override;
		virtual void EndDraw(WISP_RECT *pRect) override;

		virtual void SetCursor(unsigned int CursorType, WISP_POINT *pHotPT, WISP_SIZE *pSize, CWispDIB *pDIB) override;
		virtual void ChangeCursor(LONG_PTR Style, unsigned int CursorType, WISP_POINT *pHotPT, WISP_SIZE *pSize) override;

		virtual const WCHAR *GetClipboard() override;
		virtual bool SetClipboard(const WCHAR *Text) override;


		virtual bool GetOpenFolderName(WCHAR *Path, const WCHAR *Title) override;

		virtual bool GetOpenFileName(const WCHAR *lpstrInitialDir, const WCHAR *lpstrFilter, int, WCHAR *lpstrFile, int nMaxFile) override;

		virtual void ReportMsg(const WCHAR *Format, ...) override;

		virtual bool WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		
		bool UnregisterTaskBar(int);
		bool RegisterTaskBar(const char *HintName, const char *IconName);
		bool UpdateTaskBar(const char *IconName);

		void CreateFrameBuffer(int Width, int Height);

		static LRESULT CALLBACK DefaultWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		int WinMainStart(HINSTANCE hInstance, const char *lpCmdLine, int nShowCmd);
	};

#endif

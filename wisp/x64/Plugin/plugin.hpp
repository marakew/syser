
#ifndef _PLUGIN_HPP_
#define _PLUGIN_HPP_

	#include <windows.h>

	#define WSTR(x)	L##x

	typedef void (*FPMenuProc)();
	typedef int (*FPCmd)(int argc, const wchar_t *argv[], const wchar_t *szCommandLine, void *pUserData);

	typedef void (*PVFV)();

	struct SYSER_PLUGIN_MODULE
	{
		wchar_t PluginInfo[32];
		PVFV fpOnDebuggerOpen;
		PVFV fpOnDebuggerClose;
		PVFV fpOnDebuggerPlunge;
	};

	struct CSyserUI
	{
		virtual bool RegisterPluginModule(const wchar_t *ModuleName, SYSER_PLUGIN_MODULE *pPluginModule) = 0;
		virtual bool UnregisterPluginModule(const wchar_t *ModuleName) = 0;

		virtual int GetInstrLen(ULONG_PTR Address) = 0;

		virtual bool CalcExp(const wchar_t *szExp, ULONG_PTR *pResult) = 0;

		virtual bool InsertCmd(const wchar_t *szCmd, FPCmd pCmdProc, void *pUserData, const wchar_t *pComment, const wchar_t *pUsage) = 0;
		virtual void RemoveCmd(const wchar_t *szCmd) = 0;
		virtual int RunCmd(const wchar_t *szCmd) = 0;

		virtual void Outputf(const wchar_t *szMsg, ...) = 0;
		virtual void Output(const unsigned long *szMsg) = 0;

		virtual void *InsertMenu(void *hParentMenu, const wchar_t *szMenuName, FPMenuProc fpMenuProc) = 0;
		virtual bool RemoveMenu(void *hMenu) = 0;
		virtual bool EnableMenu(void *hMenu, bool bEnable) = 0;

		virtual void *GetMainTabWnd() = 0;
		virtual void *GetMainMenu() = 0;
		virtual void *GetWisp() = 0;

		virtual unsigned long WriteMemory(ULONG_PTR Address, void *Buffer, unsigned long Size) = 0;
		virtual unsigned long ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size) = 0;
	};

	extern "C" __declspec(dllimport) CSyserUI *gpSyserPluginUI;
#endif

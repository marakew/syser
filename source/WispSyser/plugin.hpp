
#ifndef _PLUGIN_HPP_
#define _PLUGIN_HPP_

#include "../Code/define.h"

	typedef void (*FPMenuProc)();
	typedef int (*FPCmd)(int argc, const WCHAR *argv[], const WCHAR *szCommandLine, void *pUserData);

	typedef void (*PVFV)();

	struct SYSER_PLUGIN_MODULE
	{
		WCHAR PluginInfo[32];
		PVFV fpOnDebuggerOpen;
		PVFV fpOnDebuggerClose;
		PVFV fpOnDebuggerPlunge;
	};

	struct CSyserPluginUI
	{
		virtual bool RegisterPluginModule(const WCHAR *ModuleName, SYSER_PLUGIN_MODULE *pPluginModule) = 0;
		virtual bool UnregisterPluginModule(const WCHAR *ModuleName) = 0;

		virtual int GetInstrLen(ULONG_PTR Address) = 0;

		virtual bool CalcExp(const WCHAR *szExp, ULONG_PTR *pResult) = 0;

		virtual bool InsertCmd(const WCHAR *szCmd, FPCmd pCmdProc, void *pUserData, const WCHAR *pComment, const WCHAR *pUsage) = 0;
		virtual void RemoveCmd(const WCHAR *szCmd) = 0;
		virtual int RunCmd(const WCHAR *szCmd) = 0;

		virtual void Outputf(const WCHAR *szFormat, ...) = 0;
		virtual void Output(const unsigned long *szMsg) = 0;

		virtual void *InsertMenu(void *hParentMenu, const WCHAR *szMenuName, FPMenuProc fpMenuProc) = 0;
		virtual bool RemoveMenu(void *hMenu) = 0;
		virtual bool EnableMenu(void *hMenu, bool bEnable) = 0;

		virtual void *GetMainTabWnd() = 0;
		virtual void *GetMainMenu() = 0;
		virtual void *GetWisp() = 0;

		virtual unsigned long WriteMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size) = 0;
		virtual unsigned long ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size) = 0;
	};

	extern "C" __declspec(dllimport) CSyserPluginUI *gpSyserPluginUI;

#endif
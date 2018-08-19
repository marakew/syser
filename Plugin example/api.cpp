
#ifndef _PLUGIN_HPP_
#define _PLUGIN_HPP_

	#include <windows.h>

	#define WSTR(x)	L##x

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
		virtual bool RegisterPluginModule(const wchar_t *ModuleName, SYSER_PLUGIN_MODULE *pPluginModule);
		virtual bool UnregisterPluginModule(const wchar_t *ModuleName);
		virtual int GetInstrLen(unsigned long Address);
		virtual bool CalcExp(const wchar_t *szExp, unsigned long *pResult);
		virtual bool InsertCmd(const wchar_t *szCmd, FPCmd pCmdProc, void *pUserData, const wchar_t *pComment, const wchar_t *pUsage);
		virtual void RemoveCmd(const wchar_t *szCmd);
		virtual int RunCmd(const wchar_t *szCmd);
		virtual void Outputf(const wchar_t *szMsg, ...);
		virtual void Output(const unsigned long *szMsg);
		virtual void *InsertMenu(void *hParentMenu, const wchar_t *szMenuName, void *fpMenuProc);
		virtual bool RemoveMenu(void *hMenu);
		virtual bool EnableMenu(void *hMenu, bool bEnable);
		virtual void *GetMainTabWnd();
		virtual void *GetMainMenu();
		virtual void *GetWisp();
		virtual unsigned long WriteMemory(unsigned long Address, void *Buffer, unsigned long Size);
		virtual unsigned long ReadMemory(unsigned long Address, void *Buffer, unsigned long Size);
	};

	extern "C" __declspec(dllimport) CSyserUI *gpSyserPluginUI;
#endif
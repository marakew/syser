
	#include "plugin.hpp"

	static void *hMenu = 0;

	void OnMenuAction()
	{
		gpSyserPluginUI->Outputf(WSTR("Menu Action!\n"));
	}

	void OnDebuggerOpen()
	{
		hMenu = gpSyserPluginUI->InsertMenu(0, WSTR("SPCommand"), OnMenuAction);
	        //::DbgPrint("SPCommand : OnDebuggerOpen\n");
	}

	void OnDebuggerClose()
	{
		gpSyserPluginUI->RemoveMenu(hMenu);
	        //::DbgPrint("SPCommand : OnDebuggerClose\n");
	}

	void OnDebuggerPlunge()
	{
	        //::DbgPrint("SPCommand : OnDebuggerPlunge\n");

		//avail all CPU context, see README

	        ULONG_PTR Value;

       	        if(gpSyserPluginUI->CalcExp(L"PID", &Value))
		{
       	                gpSyserPluginUI->Outputf(WSTR("PID = %08x\n"), (unsigned long)Value);
                }

       	        if(gpSyserPluginUI->CalcExp(L"TID", &Value))
		{
       	                gpSyserPluginUI->Outputf(WSTR("TID = %08x\n"), (unsigned long)Value);
                }

	}

	SYSER_PLUGIN_MODULE PluginModule =
	{
		L"Syser Command Plugin Module",
		OnDebuggerOpen,
		OnDebuggerClose,
		OnDebuggerPlunge,
	};

	int syser_calc(int argc, const wchar_t *argv[], const wchar_t *szCommandLine, void *pUserData)
	{
	        ULONG_PTR Value;
	        if(argc >= 2)
	        {
        	        if(gpSyserPluginUI->CalcExp(argv[1], &Value))
	                {
        	                gpSyserPluginUI->Outputf(WSTR("%s = %08x\n"), argv[1], Value);
	                }
        	}
	        gpSyserPluginUI->Outputf(WSTR("hello world!\n"));
        	return 0;
	}

	BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
	{
		switch (dwReason)
		{
		case DLL_PROCESS_ATTACH:
			if(!gpSyserPluginUI->RegisterPluginModule(WSTR("SPCommand"), &PluginModule))
				return FALSE;
			gpSyserPluginUI->InsertCmd(WSTR("calc"), syser_calc, NULL, WSTR("calc expression value."), WSTR("calc [parameter]"));
			//::DbgPrint("SPCommand : DllAttach\n");
			break;
		case DLL_PROCESS_DETACH:
			gpSyserPluginUI->RemoveCmd(WSTR("calc"));
			gpSyserPluginUI->UnregisterPluginModule(WSTR("SPCommand"));
			//::DbgPrint("SPCommand : DllDetach\n");
			break;    

		case DLL_THREAD_ATTACH:
			break;        

		case DLL_THREAD_DETACH:
			break;
		}    
		return TRUE;  
	}


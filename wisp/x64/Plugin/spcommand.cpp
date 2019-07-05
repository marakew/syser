
	#include "plugin.hpp"

	void OnDebuggerOpen()
	{
        	//Syser Loaded Notify
	        //::DbgPrint("SPCommand : OnDebuggerOpen\n");
	}

	void OnDebuggerClose()
	{
	        //Syser Unload Notify
	        //::DbgPrint("SPCommand : OnDebuggerClose\n");
	}

	void OnDebuggerPlunge()
	{
	        //Syser
	        //::DbgPrint("SPCommand : OnDebuggerPlunge\n");
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
	        if(argc>=2)
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


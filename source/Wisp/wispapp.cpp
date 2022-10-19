
#include "wispapp.hpp"

	bool CWispApp::Start()
	{
		return Init(); 
	}

	bool CWispApp::Stop()
	{
		Release();
		return false;
	}

	bool CWispApp::Run()
	{
		Pump();
		return false;
	}


#ifndef _WISPAPP_HPP_
#define _WISPAPP_HPP_

#include "wispbase.hpp"

	struct CWispApp : public CWispBase
	{

	public:
		virtual bool Start(); // { return Init(); }
		virtual bool Stop(); // { Release(); return false; }
		virtual bool Run(); // { Pump(); return false; }
	};

#endif
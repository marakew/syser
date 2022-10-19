
#ifndef _WISPTIPWND_HPP_
#define _WISPTIPWND_HPP_

#include "../Code/define.h"
#include "wispwnd.hpp"

	struct CWispTipWnd : public CWispWnd
	{
		CWispWnd *m_pTipOwnerWnd;

		CWispTipWnd();

		bool OnUpdate(WISP_MSG *pMsg);
		void AutoAdjustPostion(CWispWnd *pWnd);
		virtual bool MsgProc(WISP_MSG *pMsg) override;
	};
#endif
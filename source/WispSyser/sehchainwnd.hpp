
#ifndef _SEHCHAINWND_HPP_
#define _SEHCHAINWND_HPP_

#include "../Wisp/wisplist.hpp"

struct CSEHChainWnd : public CWispList
{
	CSEHChainWnd();
	virtual ~CSEHChainWnd();

        virtual bool MsgProc(WISP_MSG *pMsg) override;
        bool OnCreate(WISP_MSG *pMsg);
        bool OnClose(WISP_MSG *pMsg);
        bool OnEvent(WISP_MSG *pMsg);

        void UpdateContext();

        void Create();
};

#endif

#ifndef _MULTISOURCECODEVIEW_HPP_
#define _MULTISOURCECODEVIEW_HPP_

#include "../Code/alt/altmap.hpp"
#include "../Wisp/wispmultitabview.hpp"
#include "../EXEAnalyzer/sdsmodule.hpp"
#include "sourcecodewnd.hpp"

struct SCWKEY
{
	unsigned long FileID;
	CSDSModule *pSDSMod;
	//CSourceCodeWnd *pSrcWnd;
	
//	SCWKEY(unsigned long FileID, CSDSModule *pSDSMod):FileID(FileID), pSDSMod(pSDSMod) {}
	bool operator==(const SCWKEY & other) const
	{
		return FileID == other.FileID && pSDSMod == other.pSDSMod;
	}
	bool operator<(const SCWKEY & other) const
	{
	        if (FileID < other.FileID) return true;
	        if (FileID > other.FileID) return false;
	        if (pSDSMod < other.pSDSMod) return true;
	        return false;
	}
};

struct CMultiSourceCodeView : public CWispMultiTabView
{
	map<SCWKEY, CSourceCodeWnd*> m_SourceCodeWndMap;

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreate(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	virtual bool OnRemoveTab(CWispWnd *pWnd) override;
	virtual void OnRemoved() override;
	virtual void OnActiveWnd(CWispWnd *pWnd) override;

	void UpdateContext();
};

#endif
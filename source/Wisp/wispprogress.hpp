
#ifndef _WISPPROGRESS_HPP_
#define _WISPPROGRESS_HPP_

#include "../Code/define.h"
#include "wispbasewnd.hpp"

struct CWispProgress : public CWispBaseWnd
{
	int m_nStep;
	int m_nPos;
	int m_nLower;
	int m_nUpper;

	CWispProgress();
	virtual ~CWispProgress();
	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnUpdate(WISP_MSG *pMsg);
	bool Create(unsigned int Style, const WISP_RECT & rect, CWispBaseWnd *pParentWnd, unsigned int nID);
	
	int GetPos();
	void SetPos(int nPos);
	void GetRange(int & nLower, int & nUpper);
	void SetRange(int nLower, int nUpper);
	void SetStep(int nStep);
	void StepIt();
	void OffsetPos(int nPos);
};

#endif
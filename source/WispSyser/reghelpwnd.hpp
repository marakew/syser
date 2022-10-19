
#ifndef _REGHELPWND_HPP_
#define _REGHELPWND_HPP_

#include "../Wisp/wispwnd.hpp"
#include "../Wisp/wisptabwnd.hpp"

struct stRegisterBitName
{
	const WCHAR* RegName;
	int BeginBitIndex;
	int BitLen;
	const WCHAR* Descriptor;
};

struct CRegHelpWnd: public CWispWnd
{
	int m_RegisterGroup;

	CRegHelpWnd();
	virtual ~CRegHelpWnd();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnUpdateClient(WISP_MSG *pMsg);

	bool DrawRegister(int x, int y, unsigned int NumberMask, unsigned int ReservedMask, unsigned int LineMask, unsigned int OtherMask, CWispDC DrawDC, stRegisterBitName *RegBitName, const WCHAR *RegisterName);
	int SetRegisterGroup(int index);
};

struct CX86RegHelpTabWnd: public CWispTabWnd
{
	CRegHelpWnd m_RegisterGroup[6];

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreate(WISP_MSG *pMsg);
	void Create();
};

#endif
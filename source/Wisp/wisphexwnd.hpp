
#ifndef _WISPHEXWND_HPP_
#define _WISPHEXWND_HPP_

#include "../Code/define.h"
#include "../Code/alt/altlist.hpp"
#include "wispwnd.hpp"

enum 
{
	HEX_START_INDEX = 0,
	HEX_POS_INDEX = 1,
	HEX_HEX_INDEX = 2,
	HEX_ASCII_INDEX = 3,
	HEX_ITEM_COUNT = 4,
};

enum 
{
	HEX_BYTE_NORMAL = 0,
	HEX_BYTE_SEL = 1,
	HEX_BYTE_BPX = 2,
};

struct CWispHexWnd : public CWispWnd
{
	CWispHexWnd();
	virtual ~CWispHexWnd();
	virtual bool MsgProc(WISP_MSG *pMsg) override;
	bool OnChar(WISP_MSG *pMsg);
	bool OnCreate(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnCmdVSpinScroll(WISP_MSG *pMsg);
	bool OnKeyEvent(WISP_MSG *pMsg);
	bool OnMouseMove(WISP_MSG *pMsg);
	bool OnMouseDblClick(WISP_MSG *pMsg);
	bool OnUpdateClient(WISP_MSG *pMsg);
	bool OnRecalcLayout(WISP_MSG *pMsg);
	bool OnScrollEvent(WISP_MSG *pMsg);
	bool OnSize(WISP_MSG *pMsg);

	virtual unsigned long WriteMemory(ULONG_PTR Address, void *Buffer, unsigned long Size);
	virtual unsigned long ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size);

	virtual unsigned int GetHexByteAttribute(ULONG_PTR Address);

	unsigned int CorrectHitPoint(WISP_POINT *pPoint);
	bool CalcPointPos(WISP_POINT *pPoint, ULONG_PTR *pPos, unsigned long *pLength);
	bool ViewAddress(ULONG_PTR Address, bool bHistory);
	bool ViewAddressMsg(ULONG_PTR Address, bool bUpdate); //private

	int GetDataWidth();
	bool SetDataWidth(int width);

	void SetFloatAddress(const WCHAR *StringAddr);
	virtual ULONG_PTR GetFloatAddress(const TWideString & FloatAddress);

	virtual void OnChangeView();
	virtual void OnDataUpdated();
	virtual int GetStartLine();

	bool DataWidthCompare(ULONG_PTR Address, unsigned char *Buffer);
	void DataWidthUpdate();

	int GetAddressMode();
	void SetAddressMode(int Mode, ULONG_PTR Addr);

	bool GetFloatAddressMode();

	bool ToggleFloatAddressMode();
	void ToggleHexOffset();

	void AddPos(ULONG_PTR Addr, ULONG_PTR PrevAddr);
	void ResetPos();
	void SavePos();
	void PrevPos();
	void NextPos();

	unsigned long m_ShowOffset;
	bool m_bShowOffset;

	ULONG_PTR m_ImageBase;
	ULONG_PTR m_ImageSize;
	ULONG_PTR m_ImageHighBase;
	ULONG_PTR m_CurAddr;

	unsigned long m_BSelColor;
	unsigned long m_TextColor;
	unsigned long m_TSelColor;
	unsigned long m_BBPXColor;

	unsigned long m_AlignScroll;

	int m_ShowLength;
	int m_nLines_Page;
	int m_nChars_Line;
	int m_LineHeight;
	int m_CharWidth;
	int m_nByte_Line;
	int m_CharXPos;
	int m_CharMax;
	int m_CaretIndex;
	int m_CaretnLine;
	WISP_POINT m_CaretPos;
	ULONG_PTR m_SelStart;
	ULONG_PTR m_SelEnd;
	bool m_bSelect;
	bool m_bSelText;
	unsigned long m_CurArea;

	unsigned long m_SrhAddr;
	unsigned char m_SrhData[256];
	unsigned long m_SrhSize;
	unsigned long m_SrhLeftSize;
	char m_SrhbCase;

	int m_LenArray[256][HEX_ITEM_COUNT];
	unsigned long m_ItemColor[HEX_ITEM_COUNT];
	int m_ItemSpace[HEX_ITEM_COUNT];

	unsigned long m_AddressMode;//Address=1,Offset=0
	ULONG_PTR m_AddressModeOffs;
	TWideString m_FloatAddressStr;
	bool m_bFloatAddress;//Fixed=1,Float=0
	unsigned char m_Array2[4096];
	ULONG_PTR m_Offset2;
	ULONG_PTR m_Offset1;
	unsigned long m_Size2;

	unsigned char m_Array1[4096];
	unsigned long m_Size1;

	TList<ULONG_PTR> m_History;
	TListIter<ULONG_PTR> m_HistoryIter;

	static const WCHAR *m_UnReadable;      //L"????????????????"
	static const WCHAR *m_UnReadableASCII; //L"..................."
	static const WCHAR *m_Spaces;          //L"                "
};

#endif

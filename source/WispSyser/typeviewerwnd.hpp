
#ifndef _TYPEVIEWERWND_HPP_
#define _TYPEVIEWERWND_HPP_

#include "../Wisp/wisplist.hpp"
#include "../Wisp/wispmenu.hpp"
#include "../EXEAnalyzer/sdsmodule.hpp"

struct TVName
{
	unsigned long ID;
	CSDSModule *pSDSMod;
};

#define TV_STATIC_MASK	   0x8'0000 //+++
#define TV_VTBL_MASK	  0x80'0000
#define TV_NEW_MASK	 0x100'0000
// 0x200'0000
#define TV_HEX_MASK	 0x400'0000
// 0x800'0000
//0x1000'0000
#define TV_DATA_MASK	0x2000'0000
#define TV_FUNC_MASK	0x4000'0000
#define TV_CLOSE_MASK	0x8000'0000

struct TVValue
{
	unsigned long long Address;
	unsigned long long Options; //TODO bitfield
};

struct TVType
{
	unsigned long long X; //Address/Value
	unsigned long long ID;
};

struct TVOffset
{
	unsigned long long X; //Address
	unsigned long long ID;
};

struct TVInfo
{
	TVName Name;
	TVValue Value;
	TVType Type;
	TVOffset Offset;
};

#define SETPAIR(x,y) ((ULONGLONG)x), ((ULONGLONG)y)
#define GETPAIR(x,y) ((ULONGLONG*)x), ((ULONGLONG*)y)

struct CTypeViewerWnd : public CWispList
{
	CWispMenu m_PopupMenu;
	CWispMenu::MenuItem *m_pMenuItemRemove;
	CListStringItem *m_pFirstItem;

	CTypeViewerWnd();
	virtual ~CTypeViewerWnd();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreate(WISP_MSG *pMsg);
	bool OnDestroy(WISP_MSG *pMsg);

	bool OnKeyEvent(WISP_MSG *pMsg);

	bool OnEvent(WISP_MSG *pMsg);

	bool OnEventDelete(WISP_MSG *pMsg);
	bool OnEventHex(WISP_MSG *pMsg);

	virtual bool Edit(CListStringItem *Item, int nCol, TWideString& String) override; //Content? 0x80000111 //+54

	bool AddWatch(CListStringItem *pItem);
	void AddWatch(const WCHAR *String, CListStringItem *pItem);
	void AddWatch(const char *szString, CListStringItem *pItem);
	virtual bool OnEditing(CListStringItem *Item, int nCol, TWideString& String) override; //80000113,80000112 //+58

	bool Write(ULONG_PTR Address, CSDSModule *pSDSMod, unsigned int ID, unsigned long long Value);
	bool EDIT(CListStringItem *Item);
	virtual void OnEdit(CListStringItem *Item, int nCol, TWideString& String) override; //0x8000010E //+5C

	BASICTYPE *GetPointerRefType(CSDSModule *pSDSMod, unsigned long ID);
	void ExpandVTable(CListStringItem *Item, TVInfo *info);

	void ExpandPointer(CListStringItem *Item);
	void ExpandPointerRef(CListStringItem *Item);
	void ExpandArray(CListStringItem *Item);
	void ExpandStruct(CListStringItem *Item, TVInfo *info);

	void EXPAND(CListStringItem *Item);
	virtual void OnExpand(CListStringItem *Item) override; //+64

	void ResetItems();

	void ResetContext();

	bool UpdateArray(CListStringItem *Item, WCHAR *Str, unsigned int StrLen, ULONG_PTR Address);
	bool UpdateBaseClassPointer(CListStringItem *Item, ULONG_PTR Address);
	bool UpdateStructPointer(CListStringItem *Item, WCHAR *Str, unsigned int StrLen, ULONG_PTR Address);
	bool UpdateStructPointerRef(CListStringItem *Item, WCHAR *Str, unsigned int StrLen, ULONG_PTR Address);
	bool UpdateBaseClass(CListStringItem *Item, ULONG_PTR Address);
	bool UpdateStruct(CListStringItem *Item, WCHAR *Str, unsigned int StrLen, ULONG_PTR Address);
	bool UpdateWatch(CListStringItem *Item, WCHAR *Str, unsigned int StrLen, ULONG_PTR Address);

	bool ReadX(const char *Name, ULONG_PTR *Address, FUNCTION **pFunc, CSDSModule **SDSMod);
	bool ReadX(unsigned long regid, ULONG_PTR *Value);
	bool ReadX(CSDSModule *pSDSMod, FUNCTION *pFunc, ULONG_PTR *Value);
	bool ReadX(const char *Name, ULONG_PTR *Address, DATATYPE **Data, CSDSModule **SDSMod);
	void UpdateContext();

	void RemoveSDSModule(CSDSModule *pSDSMod);

	void ShowBaseType(CSDSModule *pSDSMod, unsigned long ID, ULONG_PTR Address, unsigned long Options, CListStringItem *Item, const char *VarName, bool bLocal);
	void ShowPointer(CSDSModule *pSDSMod, unsigned long ID, ULONG_PTR Address, unsigned long Options, CListStringItem *Item, const char *VarName, bool bLocal);
	void ShowArray(CSDSModule *pSDSMod, unsigned long ID, ULONG_PTR Address, unsigned long Options, CListStringItem *Item, const char *VarName, bool bLocal);
	void ShowEnum(CSDSModule *pSDSMod, unsigned long ID, ULONG_PTR Address, unsigned long Options, CListStringItem *Item, const char *VarName, bool bLocal);
	void ShowStruct(CSDSModule *pSDSMod, unsigned long ID, ULONG_PTR Address, unsigned long Options, CListStringItem *Item, const char *VarName, bool bLocal);
	void Show(CSDSModule *pSDSMod, unsigned long ID, ULONG_PTR Address, unsigned long Options, CListStringItem *Item, const char *VarName);

	bool GetOutputValue(CSDSModule *pSDSMod, BASICTYPE *Type, ULONG_PTR Address, unsigned long Options, WCHAR *DisplayOut, int *DisplayOutLen, unsigned long long *Value);

	bool GetItems(CListStringItem *Item, TVName *tvname, TVValue *tvvalue, TVType *tvtype, TVOffset *tvoffset);
	bool GetItems(CListStringItem *Item, TVInfo *info);

	bool SetItemComplexValue(CListStringItem *Item, int nCol, ULONGLONG v1, ULONGLONG v2);
	bool GetItemComplexValue(CListStringItem *Item, int nCol, ULONGLONG *v1, ULONGLONG *v2);

	bool SetAddress(CListStringItem *Item, unsigned long ID, ULONG_PTR Address);
};

#endif
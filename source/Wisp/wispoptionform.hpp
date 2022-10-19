
#ifndef _WISPOPTIONFORM_HPP_
#define _WISPOPTIONFORM_HPP_

#include "../Code/define.h"
#include "wispform.hpp"
#include "wisplist.hpp"
#include "wispbutton.hpp"

enum
{
	OPTION_SUB	= 0,
	OPTION_STRNUM	= 1,
	OPTION_INT	= 2,
	OPTION_STR	= 3,
	OPTION_DIR	= 4,
	OPTION_COLOR	= 5,
};

struct OPTION_FORM
{
	const WCHAR *Name;
	int Type;
	//union
	//{
		//unsigned char *DefaultValue;
		void *DefaultValue;
		//OPTION_FORM *Sub;//0
		//unsigned int *IntValue;//1,2
		//const WCHAR *StrValue;//3
		//const WCHAR *DirValue;//4
		//unsigned long *ColorValue; //5
	//};
	int Size;
	const WCHAR **SelectValue;
	bool (*CheckValue)(unsigned int *IntValue);
	unsigned int *Update;
	//unsigned char *NewValue;
	void *NewValue;
};

struct CWispOptionList : public CWispList
{
	virtual void DrawContent(int nCol, CListStringItem::CONTENT *Content, WISP_RECT *pRect) override; //+94
};

struct CWispOptionForm : public CWispForm
{
	OPTION_FORM *m_OptionForm;
	int m_nChanges;
	CWispOptionList m_OptionList;
	CWispButton *m_pApply;
	CWispButton *m_pRestore;
	CWispButton *m_pDefault;

	CWispOptionForm();
	virtual ~CWispOptionForm();

	virtual bool MsgProc(WISP_MSG *pMsg) override;

	bool OnCreateForm(WISP_MSG *pMsg);

	void SetText(OPTION_FORM *Option, CListStringItem *Item);
	void LoadTexts(CListStringItem *ParentItem);
	void LoadItems(OPTION_FORM *Option, CListStringItem *ParentItem);

	bool OnDestroy(WISP_MSG *pMsg);
	bool OnSize(WISP_MSG *pMsg);
	bool OnCmdOptionList(WISP_MSG *pMsg);
	bool OnCommand(WISP_MSG *pMsg);
	bool OnEvent(WISP_MSG *pMsg);

	bool Process(CListStringItem *Item, OPTION_FORM *Option);

	bool OnEventApply(WISP_MSG *pMsg);
	bool OnEventRestore(WISP_MSG *pMsg);
	bool OnEventDefault(WISP_MSG *pMsg);

	void EventApply(CListStringItem *ParentItem);
	void EventRestore(CListStringItem *ParentItem);
	void EventDefault(CListStringItem *ParentItem);

	void Destroy(CListStringItem *Item);

	bool GetFormInput(int PosX, int PoxY, CListStringItem *Item, OPTION_FORM *pOptionGroup);

	virtual bool OnAppling(); //{ return true; }
	virtual void OnApply(); //{}

	virtual bool OnRestoring(); //{ return true; }
	virtual void OnRestore(); //{}

	virtual bool OnDefaulting(); //{ return true; }
	virtual void OnDefault(); //{}


	virtual void OnNew(CListStringItem *Item);
	virtual void OnOld(CListStringItem *Item);
};


#endif
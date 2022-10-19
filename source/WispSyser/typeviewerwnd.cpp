
#include "../Wisp/wispbase.hpp"
#include "SyserRing3.hpp"
#include "typeviewerwnd.hpp"
#include "syserconfig.hpp"

enum
{
  TV_NAME = 0,
  TV_VALUE = 1,
  TV_TYPE = 2,
  TV_OFFSET = 3,
};

	WISP_MSG_MAP_BEGIN(CTypeViewerWnd)
		WISP_MSG_MAP(WISP_WM_CREATE, OnCreate)
		WISP_MSG_MAP(WISP_WM_DESTROY, OnDestroy)
		WISP_MSG_MAP(WISP_WM_KEY_EVENT, OnKeyEvent)
		WISP_MSG_MAP_ON_EVENT
	WISP_MSG_MAP_END(CWispList)

	WISP_EVENT_MAP_BEGIN(CTypeViewerWnd)
		WISP_EVENT_MAP(0xF0, OnEventDelete) //EVENT_ID_DEL_WATCH
		WISP_EVENT_MAP(0xEF, OnEventHex) //EVENT_ID_HEX_WATCH
	WISP_EVENT_MAP_END

WISP_MENU_GROUP_ITEM TypeViewerPopupMenu[] =
{
	{WSTR("Delete Watch"), 0xF0, 0xCA, 0, 0}, //EVENT_ID_DEL_WATCH
	{WSTR("Hexadecimal Display"), 0xEF, 0xD, 0, 0}, //EVENT_ID_HEX_WATCH
	{0},
};

//#define TXTDEBUG

#ifdef TXTDEBUG
FILE *f = NULL;
#endif

	CTypeViewerWnd::CTypeViewerWnd()
	{
		m_pFirstItem = nullptr;
#ifdef TXTDEBUG
	f = fopen("LOGWATCH", "w");
fprintf(f,"Start logging\n");
fflush(f);
#endif
	}
	CTypeViewerWnd::~CTypeViewerWnd()
	{
#ifdef TXTDEBUG
fclose(f);
#endif
	}

	bool CTypeViewerWnd::OnCreate(WISP_MSG *pMsg)
	{
		InsertColumn(WSTR("Name F2"), 100, 1, 0, -1);
		InsertColumn(WSTR("Value"), 400, 1, 0, -1);
		InsertColumn(WSTR("Type"), 100, 0, 0, -1);
		InsertColumn(WSTR("Offset"), 90, 0, 0, -1);

		m_ColorFocus = gSyserColors.Color[2];
		m_ItemColor = gSyserColors.Color[1];
		SetBGColor(gSyserColors.Color[0]);

		m_PopupMenu.LoadPopupMenuGroup(TypeViewerPopupMenu, this, gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF));
		m_pMenuItemRemove = m_PopupMenu.GetItem(0xF0); //EVENT_ID_DEL_WATCH
		ResetContext();
		return true;
	}
	bool CTypeViewerWnd::OnDestroy(WISP_MSG *pMsg)
	{
		m_PopupMenu.Destroy();
		return true;
	}

	bool CTypeViewerWnd::OnKeyEvent(WISP_MSG *pMsg)
	{
		if (pMsg->KeyEvent.bKeyDown)
		{
			if (pMsg->KeyEvent.KeyType == VK_F2)
			{
				CListStringItem *Item = GetNext(0, 8);
				if (Item) EditItem(Item, TV_NAME);
			}
		} else
		{
			if (pMsg->KeyEvent.KeyType == 0x40000048) //H
				m_PopupMenu.Point(0, 1);
			else
			if (pMsg->KeyEvent.KeyType == VK_RBUTTON)
			{
				CListStringItem *Item = GetNext(0, 8);
				m_PopupMenu.Enable(m_pMenuItemRemove, Item && Item != m_pFirstItem);
				m_PopupMenu.Point(0, 1);
			}
		}
		return true;
	}

	bool CTypeViewerWnd::OnEventDelete(WISP_MSG *pMsg)
	{
		CListStringItem *Item = GetNext(0, 8);
		if (Item) //+++
		{
			if (GetItemParent(Item) != GetItemParent(GetItem(0, 0)))
				return false;
			if (Item && Item != m_pFirstItem)
				DeleteItem(Item);
		}
		return true;
	}

	bool CTypeViewerWnd::OnEventHex(WISP_MSG *pMsg)
	{
		CListStringItem *FirstItem = FullFindItem(0, 8);
		if (FirstItem)
		{
			CListStringItem *LastItem = GetNext(FirstItem, 0);
			CListStringItem *Item = FirstItem;
			do {
				TVValue Value;
				GetItemComplexValue(Item, TV_VALUE, GETPAIR(&Value.Address, &Value.Options));
				Value.Options ^= TV_HEX_MASK;
				SetItemComplexValue(Item, TV_VALUE, SETPAIR(Value.Address, Value.Options));

				GetItemContent(Item, TV_NAME);
				Item = FullFindItem(Item, 0);
			} while (Item != LastItem && Item);
			WCHAR Str[512];
			int Len = 512;
			UpdateWatch(FirstItem, Str, Len, 0);
		}
		return true;
	}

	bool CTypeViewerWnd::Edit(CListStringItem *Item, int nCol, TWideString& String)
	{
		if (!Item) return false;
		TVName Name;
		TVValue Value;
		TVType Type;
		TVOffset Offset;
		if (!GetItems(Item, &Name, &Value, &Type, &Offset)) return false;
		if (nCol == TV_NAME)
		{
			return GetItemParent(Item) == GetItemParent(GetItem(0, 0));
		}
		if (nCol != TV_VALUE) return false;

		//if (Value.Options < 0) return false; // & 0x8000'0000
		if (Value.Options & TV_CLOSE_MASK) return false;

		if (!Name.pSDSMod) return false;

		if ((Value.Options & TV_FUNC_MASK) &&
		      (Value.Options & TV_VTBL_MASK))
			return false;

		unsigned long NewID = Name.ID;

		if (Value.Options & TV_DATA_MASK)
		{
			DATATYPE *data = Name.pSDSMod->GetLocal(Name.ID);
			if (!data) return false;
			NewID = data->TypeID;
		}

		POINTERREFTYPE *pr = (POINTERREFTYPE *)Name.pSDSMod->GetGlobal(NewID);
		if (!pr) return false;
		if (pr->Elem.Type == ELEM_POINTERREF)
		{
			pr = (POINTERREFTYPE *)Name.pSDSMod->GetGlobal(pr->ID);
			if (!pr) return false;
		}
		return pr->Elem.Type == ELEM_BASE_TYPE || pr->Elem.Type == ELEM_POINTER;
	}


	bool CTypeViewerWnd::AddWatch(CListStringItem *Item)
	{
		CWispDIB *pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11D);
#ifdef TXTDEBUG
fprintf(f,"%s DIB %d %d\n", __func__, pDIB->m_PaintRect.cx,pDIB->m_PaintRect.cy);
fflush(f);
#endif

		if (!Item) return false;
		TVName Name;
		TVValue Value;
		TVType Type;
		TVOffset Offset;
		if (!GetItems(Item, &Name, &Value, &Type, &Offset)) return false;
		unsigned long NewID = Name.ID;
		if (!Name.pSDSMod) return false;
		//if (Value.Options < 0) return false; //0x8000'0000
		if (Value.Options & TV_CLOSE_MASK) return false;

		if (Value.Options & TV_DATA_MASK)
		{
			DATATYPE *data = Name.pSDSMod->GetLocal(Name.ID);
			if (!data) return false;
			NewID = data->TypeID;
			switch (data->access)
			{
			case CV_private:
				pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11F);
				break;
			case CV_protected:
				pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11E);
				break;
			case CV_public:
				pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11D);
				break;
			}
		}
	//	pDIB = 0; //BUG TODO ????
		BASICTYPE *type = Name.pSDSMod->GetGlobal(NewID);
		if (!type) return false; //+++
		if (type->Elem.Type == ELEM_FUNC_TYPE)
			pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x120);
		SetItemDIB(Item, TV_NAME, pDIB);
		WCHAR Buffer[512];
		char szBuffer[512];
		int Len = 512;
		switch (type->Elem.Type)
		{
		case ELEM_BASE_TYPE:{
#ifdef TXTDEBUG
fprintf(f,"%s ELEM_BASE_TYPE\n", __func__);
fflush(f);
#endif
				unsigned long long Address = 0;
				GetOutputValue(Name.pSDSMod, type, Value.Address, Value.Options, Buffer, nullptr, &Address);
				SetItemText(Item, TV_VALUE, Buffer);
				SetItemComplexValue(Item, TV_TYPE, SETPAIR(Address, 0)); //???????????????
				const char *Str = Name.pSDSMod->GetName(type);
				SetItemText(Item, TV_TYPE, Str);
			} break;
		case ELEM_POINTER:{
#ifdef TXTDEBUG
fprintf(f,"%s POINTER\n", __func__);
fflush(f);
#endif
				unsigned long long Address = 0;
				if (type->Elem.Length && type->Elem.Length <= 8)
					gpSyser->m_pDebugger->ReadMemory(Value.Address, &Address, type->Elem.Length);
				TSPrintf(Buffer, WSTR("0x" F0ADDR "x"), Address); //TODO X64
				SetItemComplexValue(Item, TV_TYPE, SETPAIR(Address, 0));
				SetItemText(Item, TV_VALUE, Buffer);
				if (((POINTERTYPE*)type)->Num == 1)
				{
					BASICTYPE *bt = Name.pSDSMod->GetGlobal( ((POINTERTYPE*)type)->ID);
					if (bt && bt->Elem.Type == ELEM_BASE_TYPE && bt->Elem.Length == 1)
					{
						memset(szBuffer, 0, sizeof(szBuffer));
						TSPrintf(szBuffer, "0x" F0ADDR "x \"", Address); //TODO X64
						unsigned long Result = gpSyser->m_pDebugger->ReadMemory(Address, &szBuffer[ADDR_SIZE+4], 256);
						for (unsigned long n = 0; n < Result; ++n)
						{
							if (!Buffer[n])
							{
								TStrCpy(&szBuffer[n+ADDR_SIZE+4], "\"");
								SetItemText(Item, TV_VALUE, szBuffer);
							}
						}
					}
				}
				CListStringItem *NewItem = InsertItem(WSTR(".."), Item, 0, nullptr, -1);
				Value.Options |= TV_CLOSE_MASK;
				SetItemComplexValue(NewItem, TV_VALUE, SETPAIR(0, Value.Options));
				Name.pSDSMod->GetOutput(type, szBuffer, &Len);
				SetItemText(Item, TV_TYPE, szBuffer);
			} break;
		case ELEM_POINTERREF:{
#ifdef TXTDEBUG
fprintf(f,"%s POITERREF\n", __func__);
fflush(f);
#endif
				Name.pSDSMod->GetOutput(type, szBuffer, &Len);
				SetItemText(Item, TV_TYPE, szBuffer);
				BASICTYPE *bt = (BASICTYPE *)Name.pSDSMod->GetGlobal( ((POINTERREFTYPE*)type)->ID);
				if (!bt) return false;
				ULONG_PTR Address = 0;
				gpSyser->m_pDebugger->ReadMemory(Value.Address, &Address, sizeof(Address));
				TSPrintf(Buffer, WSTR("0x" F0ADDR "x"), Address); //TODO X64
				SetItemText(Item, TV_VALUE, Buffer);
				if (bt->Elem.Type == ELEM_CLASS ||
				    bt->Elem.Type == ELEM_STRUCT ||
				    bt->Elem.Type == ELEM_UNION ||
				    bt->Elem.Type == ELEM_POINTER)
				{
					CListStringItem *NewItem = InsertItem(WSTR(".."), Item, 0, nullptr, -1);
					Value.Options |= TV_CLOSE_MASK;
					SetItemComplexValue(NewItem, TV_VALUE, SETPAIR(0, Value.Options));
				}
				if (bt->Elem.Type == ELEM_BASE_TYPE)
				{
					unsigned long long Result = 0;
					GetOutputValue(Name.pSDSMod, bt, Address, Value.Options, Buffer, nullptr, &Result);
					SetItemText(Item, TV_VALUE, Buffer);
				}
			} break;
		case ELEM_ARRAY:{
#ifdef TXTDEBUG
fprintf(f,"%s ELEM_ARRAY\n", __func__);
fflush(f);
#endif
				TSPrintf(Buffer, WSTR("0x" F0ADDR "x"), Value.Address); //TODO X64
				SetItemText(Item, TV_VALUE, Buffer);
				CListStringItem *NewItem = InsertItem(WSTR(".."), Item, 0, nullptr, -1);
				Value.Options |= TV_CLOSE_MASK;
				SetItemComplexValue(NewItem, TV_VALUE, SETPAIR(0, Value.Options));
				Name.pSDSMod->GetOutput(type, szBuffer, &Len);
				SetItemText(Item, TV_TYPE, szBuffer);
			} break;
		case ELEM_STRUCT:
		case ELEM_CLASS:
		case ELEM_UNION:{
#ifdef TXTDEBUG
fprintf(f,"%s ELEM_UDT\n", __func__);
fflush(f);
#endif
				CListStringItem *NewItem = InsertItem(WSTR(".."), Item, 0, nullptr, -1);
				Value.Options |= TV_CLOSE_MASK;
				SetItemComplexValue(NewItem, TV_VALUE, SETPAIR(0, Value.Options));
				Name.pSDSMod->GetOutput(type, szBuffer, &Len);
				SetItemText(Item, TV_TYPE, szBuffer);
				SetItemText(Item, TV_VALUE, "{...}");
			} break;
		case ELEM_FUNC_TYPE:{
#ifdef TXTDEBUG
fprintf(f,"%s ELEM_FUNC\n", __func__);
fflush(f);
#endif
				//TSPrintf(Buffer, WSTR("0x" F0ADDR "x  "), *(ULONG_PTR*)Value.Address); //TODO X64
				TSPrintf(Buffer, WSTR("0x" F0ADDR "x  "), Value.Address); //TODO X64
				SetItemText(Item, TV_VALUE, Buffer);
				FUNCTION *pFunc = Name.pSDSMod->GetFunctionByID(Type.ID);
				if (pFunc) //func
				{
					TStrCpy(szBuffer, Buffer);
					Len = 512/2;
					Name.pSDSMod->GetOutputFunc(pFunc, &szBuffer[ADDR_SIZE+4], &Len); //sizeof("0x%08x  ")
					SetItemText(Item, TV_VALUE, szBuffer);
					Len = 512/2;
					Name.pSDSMod->GetOutput(type, szBuffer, &Len);
					SetItemText(Item, TV_TYPE, szBuffer);
				}
			} break;
		}
		return true;
	}

	void CTypeViewerWnd::AddWatch(const WCHAR *String, CListStringItem *Item)
	{
		if (!Item)
		{
			Item = m_pFirstItem;
			ResetItems();
		}

		CSDSModule *pSDSMod = gpSyser->m_MainFrame.m_SourceDebugFrameWnd.m_SDSModuleList.m_pSDSMod;

		SetItemComplexValue(Item, TV_NAME, SETPAIR(pSDSMod, 0));
		SetItemText(Item, TV_NAME, String);
		SetItemComplexValue(Item, TV_VALUE, SETPAIR(0, TV_DATA_MASK | TV_CLOSE_MASK | TV_NEW_MASK));
									 // 2000'0000 | 8000'0000 | 100'0000
		SetItemText(Item, TV_VALUE, WSTR(""));
		SetItemFormat(Item, TV_OFFSET, 0, WSTR("%X"));
		if (pSDSMod)
		{
		#if 1
			TStaticAnsiString Str = String;
		#else
			char Buffer[256];
			char *pBuffer = Buffer;
			int Len = TStrLen(String)+1;
			if (Len >= 256)
				pBuffer = new char[Len];
			TStrCpy(pBuffer, String);
		#endif
#ifdef TXTDEBUG
fprintf(f,"add varname %s %s\n",pBuffer, __func__);
fflush(f);
#endif

			ULONG_PTR Address = 0;
			DATATYPE *Data;
			FUNCTION *pFunc;

			if (ReadX(Str/*.operator char*()pBuffer*/, &Address, &Data, &pSDSMod))
			{
				SetItemFormat(Item, TV_OFFSET, Address, WSTR("0x" F0ADDR "x")); //TODO X64

				SetItemComplexValue(Item, TV_NAME, SETPAIR(pSDSMod, Data->ID));
				SetItemComplexValue(Item, TV_TYPE, SETPAIR(0, 0));

				SetItemComplexValue(Item, TV_VALUE, SETPAIR(Address, TV_DATA_MASK | TV_NEW_MASK));
				AddWatch(Item);
#ifdef TXTDEBUG
fprintf(f,"%s attach data 0x%08\n", __func__,Address);
fflush(f);
#endif
			} else
			if (ReadX(Str/*.operator char*()pBuffer*/, &Address, &pFunc, &pSDSMod))
			{
				pSDSMod->GetGlobal(pFunc->ReturnCallID); //???
				SetItemComplexValue(Item, TV_NAME, SETPAIR(pSDSMod, pFunc->ReturnCallID));
				CListStringItem::CONTENT *Content = GetItemContent(Item, TV_TYPE);
				SetItemComplexValue(Item, TV_TYPE, SETPAIR(Address, pFunc->ID));
				SetItemComplexValue(Item, TV_VALUE, SETPAIR(Content->v1, TV_FUNC_MASK | TV_NEW_MASK));
				AddWatch(Item);
#ifdef TXTDEBUG
fprintf(f,"%s attach fun 0x%08\n", __func__,Address);
fflush(f);
#endif
			}
		#if 0
			if (pBuffer != Buffer)
				delete []pBuffer;
		#endif
		}
	}

	void CTypeViewerWnd::AddWatch(const char *szString, CListStringItem *Item)
	{
	#if 1
		TStaticWideString Str = szString;
		AddWatch(Str/*.operator WCHAR*()*/, Item);
	#else
		WCHAR Buffer[256];
		WCHAR *pBuffer = Buffer;
		unsigned int Len = TStrLen(szString)+1;
		if (Len >= 256)
			pBuffer = new WCHAR[Len];

		TStrCpy(pBuffer, szString);

		AddWatch(pBuffer, Item);

		if (pBuffer != Buffer)
			delete []pBuffer;
	#endif
	}

	bool CTypeViewerWnd::OnEditing(CListStringItem *Item, int nCol, TWideString& String)
	{
		//Item &&
		if (Item != m_pFirstItem)
		{
			if (String.m_nLength != 0) return true;
			if (GetItemParent(Item) == GetItemParent(GetItem(0, 0)))
				DeleteItem(Item);
			return false;
		}

		if (String.m_nLength == 0)
			return false;
		AddWatch(String.operator const WCHAR*(), nullptr);
		return true;
	}

	bool CTypeViewerWnd::Write(ULONG_PTR Address, CSDSModule *pSDSMod, unsigned int ID, unsigned long long Value)
	{
		unsigned long Result = 0;
		if (!pSDSMod) return false;
		if (!ID) return false;
		BASICTYPE *type = pSDSMod->GetGlobal(ID);
		if (!type) return false;
		if (type->Elem.Type == ELEM_BASE_TYPE ||
		    type->Elem.Type == ELEM_POINTER)
		{
			if (type->Elem.Length && type->Elem.Length <= 8)
			{
				Result = gpSyser->m_pDebugger->WriteMemory(Address, &Value, type->Elem.Length);
			}
		}
		return Result == type->Elem.Length;
	}

struct BASETYPEMAP
{
	unsigned long Type;
	const char *Name;
};

	BASETYPEMAP gBaseTypeMap[] =
	{
		{0, "NoType"},
		{1, "void"},
		{0x80, "char"},
		{0x100, "wchar"},
		{0x80, "char"},
		{0x100, "short"},
		{0x200, "int"},
		{0x400, "__int64"},
		{0x1000, "unsigned short int"},
		{0x800, "unsigned char"},
		{0x1000, "unsigned short"},
		{0x2000, "unsigned int"},
		{0x4000, "unsigned __int64"},
		{0x200, "float"},
		{0x400, "double"},
		{9, "BCD"},
		{0x800, "bool"},
		{0x200, "long"},
		{0x2000, "unsigned long"},
		{0x19, "Currency"},
		{0x1a, "date"},
		{0x1b, "variant"},
		{0x1c, "complex"},
		{0x1d, "bit"},
		{0x1e, "BSTR"},
		{0x1f, "HRESULT"},
		{0, 0},
	};

	unsigned long GetBasicType(const char *Str)
	{
		for (unsigned long n = 0; n < lenof(gBaseTypeMap); ++n) //27
		{
			if (!TStrCmp(gBaseTypeMap[n].Name, Str))
				return gBaseTypeMap[n].Type;
		}
		return 0;
	}

	bool CTypeViewerWnd::EDIT(CListStringItem *Item)
	{
		TVInfo info;
		memset(&info, 0, sizeof(info));
		if (!GetItems(Item, &info)) return false;

		unsigned long ID = info.Name.ID;
		ULONG_PTR Address = info.Value.Address;

		if (!info.Name.pSDSMod)	return false;

		//if (info.Value.Options < 0) //??? TODO 0x8000'0000
		if (info.Value.Options & TV_CLOSE_MASK) return false;

		if (info.Value.Options & TV_DATA_MASK)
		{
			DATATYPE *data = info.Name.pSDSMod->GetLocal(info.Name.ID);
			if (!data) return false;
			ID = data->TypeID;
		}

		CListStringItem::CONTENT *Content = GetItemContent(Item, TV_VALUE);
		const WCHAR *Str = Content->String.operator const WCHAR*();
		BASICTYPE *type = info.Name.pSDSMod->GetGlobal(ID);
		if (type)
		{
			bool OK = false;
			if (type->Elem.Type != ELEM_BASE_TYPE)
			{
				if (type->Elem.Type == ELEM_POINTER)
				{
					int n = 0;
					while (Str[n])
					{
						if (Str[n] == L'x' || Str[n] == L'X')
						{
							++n;
							break;
						}
						++n;
					}
					ULONG_PTR Result = 0;
					OK = USHexStrToNum(&Str[n], &Result);
					if (OK)
						Write(info.Value.Address, info.Name.pSDSMod, ID, Result);
					return OK;
				} else
				if (type->Elem.Type == ELEM_POINTERREF)
				{
					POINTERREFTYPE *pt = (POINTERREFTYPE *)type;
					//BASICTYPE *
					type = (BASICTYPE *)info.Name.pSDSMod->GetGlobal(pt->ID);
					if (!type) return false;
					if (type->Elem.Type != ELEM_BASE_TYPE) return false;
					ULONG_PTR Value = 0;
					if (gpSyser->m_pDebugger->ReadMemory(info.Value.Address, &Value, sizeof(Value)) != sizeof(Value))
						return false;
					Address = Value;
					ID = pt->ID;
				} else
				{
					//
					return false;
				}
			}
			//ELEM_BASIC_TYPE ???
			unsigned long basictype = GetBasicType(type->Elem.Name);
			if (basictype == 0xA || //???	//bool
			    basictype == 0xD || //???	//long
			    basictype == 0x80 || //char
			    basictype == 0x100 || //short
			    basictype == 0x200 || //int
			    basictype == 0x400) //__int64
			{
				LONG_PTR Result = 0;
				if (info.Value.Options & TV_HEX_MASK)
				{
					int n = 0;
					while (Str[n])
					{
						if (Str[n] == L'x' || Str[n] == L'X')
						{
							++n;
							break;
						}
						++n;
					}
					OK = USHexStrToNum(&Str[n], &Result);
				} else	OK = USDecStrToNum(Str, &Result);

				if (OK)
					Write(Address, info.Name.pSDSMod, ID, Result);
				return OK;
			} else
			if (basictype == 0xE || //???			//unsigned long
			    basictype == 0x800 || //bool
			    basictype == 0x1000 || //unsigned short int
			    basictype == 0x2000 || //unsigned long		unsigned int
			    basictype == 0x4000) //unsigned __int64
			{
				ULONG_PTR Result = 0;
				if (info.Value.Options & TV_HEX_MASK)
				{
					int n = 0;
					while (Str[n])
					{
						if (Str[n] == L'x' || Str[n] == L'X')
						{
							++n;
							break;
						}
						++n;
					}
					OK = USHexStrToNum(&Str[n], &Result);
				} else	OK = USDecStrToNum(Str, &Result);

				if (OK)
					Write(Address, info.Name.pSDSMod, ID, Result);
				return OK;
			}
		}
		return false;
	}

	void CTypeViewerWnd::OnEdit(CListStringItem *Item, int nCol, TWideString& String)
	{
		if (Item != m_pFirstItem)
		{
			EDIT(Item);
			UpdateContext();
		}
	}

	BASICTYPE *CTypeViewerWnd::GetPointerRefType(CSDSModule *pSDSMod, unsigned long ID)
	{
		DATATYPE *data = pSDSMod->GetLocal(ID);
		if (!data) return nullptr;
		POINTERREFTYPE *ptr = (POINTERREFTYPE *)pSDSMod->GetGlobal(data->TypeID);
		if (!ptr) return nullptr;
		if (ptr->Elem.Type == ELEM_POINTERREF)
			return pSDSMod->GetGlobal(ptr->ID);
		return nullptr;
	}

	void CTypeViewerWnd::ExpandVTable(CListStringItem *Item, TVInfo *info)
	{
		CWispDIB *pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11D);
		if (info->Name.pSDSMod && info->Name.ID)
		{
			ULONG_PTR Address = info->Value.Address;
			unsigned long NewID = info->Name.ID;
			if (info->Value.Options & TV_DATA_MASK)
			{
				DATATYPE *data = info->Name.pSDSMod->GetLocal(info->Name.ID);
				if (!data) return;
				NewID = data->TypeID;
			}
			BASICTYPE *basetype = (BASICTYPE*)info->Name.pSDSMod->GetGlobal("unsigned int", 1);
			if (!basetype) return;
			
			UDTYPE *udt = (UDTYPE*)info->Name.pSDSMod->GetGlobal(NewID);
			if (!udt) return;
			if (!udt->PubSymbols) return;

			for (int n = 0; n < udt->PubSymbolsLen ; ++n)
			{
				if (udt->PubSymbols[n].ID == NewID)
				{
					CLASSPUBSYM *pubsym = &udt->PubSymbols[n];

					ULONG_PTR Addr = info->Name.pSDSMod->m_pDbgModule->m_ImageBase + pubsym->rva;

					//unsigned long long Address = 0;
					//if (type->Elem.Length && type->Elem.Length <= 8)
					unsigned long Result = gpSyser->m_pDebugger->ReadMemory(Address, &Addr, sizeof(Addr)); //TODO X64 type->Elem.Length);

					CListStringItem *NewItem = InsertItemStr("__vfptr", Item, 0, pDIB, -1);
					CListStringItem::CONTENT *Content = GetItemContent(NewItem, TV_TYPE);
					const char *pStrSym = info->Name.pSDSMod->m_pDbgModule->GetSymbol(Addr);
					if (!pStrSym)
						pStrSym = "";
					char szString[256];
					TSPrintf(szString, "0x" F0ADDR "x  %s", Addr, pStrSym); //TODO X64
					SetItemText(NewItem, TV_VALUE, szString);
					SetItemComplexValue(NewItem, TV_NAME, SETPAIR(info->Name.pSDSMod, basetype->Elem.ID));
					SetItemComplexValue(NewItem, TV_VALUE, SETPAIR(Address, TV_FUNC_MASK | TV_VTBL_MASK)); //??? &Content->v1
					SetItemComplexValue(NewItem, TV_TYPE, SETPAIR(Addr, 0));
					SetItemText(NewItem, TV_TYPE, "*");

					if (Result == sizeof(Addr) && Addr)

					if (pubsym->length & ~3) //-4
					for (int i = 0; i < pubsym->length/4; ++i)
					{
						ULONG_PTR PAddr = 0;
						if (gpSyser->m_pDebugger->ReadMemory(Addr, &PAddr, sizeof(PAddr)) != sizeof(PAddr) || !PAddr)
							break;
						TSPrintf(szString, "[0x%x]", i);
						CWispDIB *pVDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x120);
						CListStringItem *SubItem = InsertItemStr(szString, NewItem, 0, pVDIB, -1);
						CListStringItem::CONTENT *Content = GetItemContent(SubItem, TV_TYPE);

						char FuncName[256];
						int FuncNameLen = 256;

						const char *pStrSym = info->Name.pSDSMod->m_pDbgModule->GetSymbol(PAddr);
						if (!pStrSym)
						{
							unsigned char CodeBuf[16];
							if (gpSyser->m_pDebugger->ReadMemory(PAddr, CodeBuf, 5) == 5 && CodeBuf[0] == 0xE9)
							{
								FUNCTION *pFunc = info->Name.pSDSMod->GetFunction(*(long*)&CodeBuf[1] - info->Name.pSDSMod->m_pDbgModule->m_ImageBase + PAddr + 5); //TODO X64
								szString[0] = 0; //???
								FuncName[0] = 0; //+++
								FuncNameLen = 256;
								if (pFunc)
									pStrSym = info->Name.pSDSMod->GetOutputFunc(pFunc, FuncName, &FuncNameLen);
							}
						}
						
						if (!pStrSym)
							pStrSym = "";
						TSPrintf(szString, "0x" F0ADDR "x  %s", PAddr, pStrSym); //TODO X64
						SetItemText(SubItem, TV_VALUE, szString);
						SetItemComplexValue(SubItem, TV_NAME, SETPAIR(info->Name.pSDSMod, basetype->Elem.ID));
						SetItemComplexValue(SubItem, TV_VALUE, SETPAIR(Content->v1, TV_FUNC_MASK | TV_VTBL_MASK)); //??? &Content->v1
						SetItemComplexValue(SubItem, TV_TYPE, SETPAIR(PAddr, 0));
						SetItemText(NewItem, TV_TYPE, "*");
						Addr += sizeof(ULONG_PTR);
					}
				}
			}
		}
	}

	void CTypeViewerWnd::ExpandPointer(CListStringItem *Item)
	{
		if (!Item) return;
		TVInfo info;
		info.Name.ID = 0;
		info.Name.pSDSMod = 0;
		if (!GetItems(Item, &info)) return;
		unsigned long NewID = info.Name.ID;
		if (!info.Name.pSDSMod) return;
		if (info.Name.ID == 0) return;
		unsigned long Options = info.Value.Options;
		if (info.Value.Options & TV_DATA_MASK)
		{
			DATATYPE *data = info.Name.pSDSMod->GetLocal(info.Name.ID);
			if (!data) return;
			NewID = data->TypeID;
		}
		POINTERTYPE *pt = (POINTERTYPE *)info.Name.pSDSMod->GetGlobal(NewID);
		if (!pt) return; //+++
		unsigned long long value = 0;
		gpSyser->m_pDebugger->ReadMemory(info.Value.Address, &value, pt->Elem.Length);

		if (pt->Num <= 1)
		{
			Options = (Options & ~TV_DATA_MASK) | TV_FUNC_MASK;
			info.Value.Address = value;
			info.Name.ID = pt->ID;
			info.Value.Options = Options;
			BASICTYPE *bt = (BASICTYPE*)info.Name.pSDSMod->GetGlobal(pt->ID);
			if (!bt) return;
			switch (bt->Elem.Type)
			{
			case ELEM_BASE_TYPE:
				ShowBaseType(info.Name.pSDSMod, info.Name.ID, value, Options, Item, "", false);
				break;
			case ELEM_STRUCT:
			case ELEM_CLASS:
			case ELEM_UNION:
				ExpandStruct(Item, &info);
				break;
			}
		} else
		{
			Show(info.Name.pSDSMod, pt->PrevID, value, Options, Item, "");
		}
	}

	void CTypeViewerWnd::ExpandPointerRef(CListStringItem *Item)
	{
		if (!Item) return;
		TVInfo info;
		info.Name.ID = 0;
		info.Name.pSDSMod = 0;
		if (!GetItems(Item, &info)) return;
		//unsigned long NewID = info.Name.ID;
		if (!info.Name.pSDSMod) return;
		if (info.Name.ID == 0) return;
		unsigned long Options = info.Value.Options;
		POINTERREFTYPE *ptr = nullptr;
		if (info.Value.Options & TV_DATA_MASK)
		{
			ptr = (POINTERREFTYPE *)GetPointerRefType(info.Name.pSDSMod, info.Name.ID);
		} else
		{
			ptr = (POINTERREFTYPE *)info.Name.pSDSMod->GetGlobal(info.Name.ID);
		}
		if (!ptr) return; //+++
		unsigned long long value = 0;
		gpSyser->m_pDebugger->ReadMemory(info.Value.Address, &value, sizeof(ULONG_PTR));

		Options = (Options & ~TV_DATA_MASK) | TV_FUNC_MASK;
		info.Value.Address = value;
		info.Name.ID = ptr->ID;
		info.Value.Options = Options;
		BASICTYPE *bt = (BASICTYPE*)info.Name.pSDSMod->GetGlobal(ptr->ID);
		if (!bt) return;
		switch (bt->Elem.Type)
		{
		case ELEM_BASE_TYPE:
			ShowBaseType(info.Name.pSDSMod, info.Name.ID, value, Options, Item, "", false);
			break;
		case ELEM_STRUCT:
		case ELEM_CLASS:
		case ELEM_UNION:
			ExpandStruct(Item, &info);
			break;
		}
	}

	void CTypeViewerWnd::ExpandArray(CListStringItem *Item)
	{
		if (!Item) return;
		TVName Name;
		TVValue Value;
		TVType Type;
		TVOffset Offset;
		if (!GetItems(Item, &Name, &Value, &Type, &Offset)) return;
		unsigned long NewID = Name.ID;
		ULONG_PTR Address = Value.Address;
		if (!Name.pSDSMod) return;
		if (Name.ID == 0) return;
		if (Value.Options & TV_CLOSE_MASK) return;

		if (Value.Options & TV_DATA_MASK)
		{
			DATATYPE *data = Name.pSDSMod->GetLocal(Name.ID);
			if (!data) return;
			NewID = data->TypeID;
		}
		ARRAYTYPE *at = (ARRAYTYPE *)Name.pSDSMod->GetGlobal(NewID);
		if (at && at->Elem.Type == ELEM_ARRAY)
		{
			if (at->Size <= 1)
			{
				BASICTYPE *bt = (BASICTYPE *)Name.pSDSMod->GetGlobal(at->ID);
				if (!bt) return; //+++
				Value.Options = (Value.Options & ~TV_DATA_MASK) | TV_FUNC_MASK;
				char VarName[20];
				for (int n = 0; n < at->Values[0].Size; ++n)
				{
					TSPrintf(VarName, "[%d]", n);
					Show(Name.pSDSMod, bt->Elem.ID, Address, Value.Options, Item, VarName);
					Address += bt->Elem.Length;
				}
			} else
			{
				Name.pSDSMod->GetGlobal(at->ParentID); //???
			}
		}
	}

	void CTypeViewerWnd::ExpandStruct(CListStringItem *Item, TVInfo *info)
	{
		if (!Item) return;

		unsigned long Options = info->Value.Options;
		unsigned long NewID = info->Name.ID;
		ULONG_PTR Address = info->Value.Address;
		if (!info->Name.pSDSMod) return;
		if (info->Value.Options & TV_DATA_MASK)
		{
			DATATYPE *data = info->Name.pSDSMod->GetLocal(info->Name.ID);
			if (!data) return;
			NewID = data->TypeID;
		}
		UDTYPE *udt = (UDTYPE *)info->Name.pSDSMod->GetGlobal(NewID);
		if (udt->BaseClassLen == 0 && udt->vtable) //0x100
		{
			ExpandVTable(Item, info);
#ifdef TXTDEBUG
fprintf(f,"%s vtbl\n", __func__);
fflush(f);
#endif
		}
		for (int n = 0; n < udt->BaseClassLen; ++n)
		{
			BASECLASSTYPE *bc = (BASECLASSTYPE *)info->Name.pSDSMod->GetGlobal(udt->BaseClass[n]);
			if (!bc) return; //+++
#ifdef TXTDEBUG
fprintf(f,"%s bc\n", __func__);
fflush(f);
#endif
			ShowStruct(info->Name.pSDSMod, bc->ID, Address + bc->Offset, Options, Item, nullptr, false);
		}

		bool bitfield = false;
		ULONG_PTR HighAddr = Address;
		unsigned long BTID = 0;
		unsigned long NextPos = 0;
		ULONG_PTR Addr = 0; //???

		for (int n = 0; n < udt->DataLen; ++n)
		{
			DATATYPE *data = info->Name.pSDSMod->GetLocal(udt->Data[n]);
			if (!data) return; //+++
			BASICTYPE *bt = (BASICTYPE *)info->Name.pSDSMod->GetGlobal(data->TypeID);
			if (!bt) return;

			switch (data->locationType)
			{
			case LocIsStatic: {
				Addr = data->Static.rva + info->Name.pSDSMod->m_pDbgModule->m_ImageBase;
				bitfield = false;
				} break;
			case LocIsThisRel: {
				Addr = Address + data->ThisRel.offset;
				bitfield = false;
				HighAddr = Addr + bt->Elem.Length;
				} break;
			case LocIsBitField: {
				if (bitfield)
				{
					if (BTID == bt->Elem.ID &&  data->BitField.bitPosition >= NextPos)
					{
						NextPos = data->BitField.length + NextPos;
					} else
					{
						Addr = HighAddr + data->BitField.offset;
						BTID = bt->Elem.ID;
						HighAddr += bt->Elem.Length;
						NextPos = data->BitField.bitPosition + data->BitField.length;
					}
				} else
				{
					BTID = bt->Elem.ID;
					Addr = HighAddr + data->BitField.offset;
					bitfield = true;
					NextPos = data->BitField.bitPosition + data->BitField.length;
					HighAddr += bt->Elem.Length;
				}
				} break;
			default:
				bitfield = false;
				break;
			}
			switch (bt->Elem.Type)
			{
			case ELEM_BASE_TYPE:{
				Options = (Options & ~TV_FUNC_MASK) | TV_DATA_MASK;
				ShowBaseType(info->Name.pSDSMod, data->ID, Addr, Options, Item, data->name, true);
				} break;
			case ELEM_POINTER:
			case ELEM_POINTERREF:{
				ShowPointer(info->Name.pSDSMod, data->ID, Addr, Options, Item, data->name, true);
				} break;
			case ELEM_ARRAY:{
				ShowArray(info->Name.pSDSMod, data->ID, Addr, Options, Item, data->name, true);
				} break;
			case ELEM_ENUM:{
				ShowEnum(info->Name.pSDSMod, data->ID, Addr, Options, Item, data->name, true);
				} break;
			case ELEM_STRUCT:
			case ELEM_CLASS:
			case ELEM_UNION:{
				ShowStruct(info->Name.pSDSMod, data->ID, Addr, Options, Item, data->name, true);
				} break;
			}
		}
	}

	void CTypeViewerWnd::EXPAND(CListStringItem *Item)
	{
		if (!Item) return;
		TVInfo info;
		if (!GetItems(Item, &info)) return;
		if (info.Value.Options & TV_CLOSE_MASK) return;
		if (!info.Name.pSDSMod) return;
#ifdef TXTDEBUG
fprintf(f,"%s\n", __func__);
fflush(f);
#endif
		//if (info.Value.Options >= 0 && info.Name.pSDSMod) //>= 0 0x8000'0000 ???
		{
			unsigned long NewID = info.Name.ID;
			if (info.Value.Options & TV_DATA_MASK)
			{
				DATATYPE *data = info.Name.pSDSMod->GetLocal(info.Name.ID);
				if (!data) return;
				NewID = data->TypeID;
			}
			BASICTYPE *type = info.Name.pSDSMod->GetGlobal(NewID);
			if (!type) return;
			switch (type->Elem.Type)
			{
			case ELEM_POINTER:
				ExpandPointer(Item);
				break;
			case ELEM_POINTERREF:
				ExpandPointerRef(Item);
				break;
			case ELEM_ARRAY:
				ExpandArray(Item);
				break;
			case ELEM_STRUCT:
			case ELEM_CLASS:
			case ELEM_UNION:
				ExpandStruct(Item, &info);
				break;
			}
		}
	}

	void CTypeViewerWnd::OnExpand(CListStringItem *Item)
	{
		GetItemValue(Item, TV_NAME); //???
		GetItemValue(Item, TV_VALUE); //???
		CListStringItem *SubItem = GetItem(0, Item); //0 ??? TV_NAME
		if (!TStrCmp(GetItemContent(SubItem, TV_NAME)->String.operator const WCHAR*(), ".."))
		{
			DeleteAllChildrenItems(Item);
			EXPAND(Item);
			Expand(Item);
		}
	}

	void CTypeViewerWnd::ResetItems()
	{
		m_pFirstItem = InsertItem(nullptr, nullptr, 0, nullptr, -1);
		SetItemComplexValue(m_pFirstItem, TV_NAME, SETPAIR(0, 0));
		SetItemComplexValue(m_pFirstItem, TV_VALUE, SETPAIR(0, 0));
		SetItemComplexValue(m_pFirstItem, TV_TYPE, SETPAIR(0, 0));
		SetItemComplexValue(m_pFirstItem, TV_OFFSET, SETPAIR(0, 0));
	}

	void CTypeViewerWnd::ResetContext()
	{
		DeleteAllChildrenItems(0);
		ResetItems();
		Select(m_pFirstItem);
	}

	bool CTypeViewerWnd::UpdateArray(CListStringItem *Item, WCHAR *Str, unsigned int StrLen, ULONG_PTR Address)
	{
#ifdef TXTDEBUG
fprintf(f,"%s\n", __func__);
fflush(f);
#endif

		if (!Item) return false;
		TVInfo info;
		if (!GetItems(Item, &info)) return false;
		unsigned long NewID  = info.Name.ID;
		if (!info.Name.pSDSMod) return false;
		if (info.Name.ID == 0) return false;
		if (info.Value.Options & TV_DATA_MASK)
		{
			DATATYPE *data = info.Name.pSDSMod->GetLocal(info.Name.ID);
			if (!data) return false;
			NewID = data->TypeID;
		}
		ARRAYTYPE *at = (ARRAYTYPE *)info.Name.pSDSMod->GetGlobal(NewID);
		if (!at) return false;
		if (at->Size == 1)
		{
			Item = FullFindItem(Item, 0);
			BASICTYPE *e = info.Name.pSDSMod->GetGlobal(at->ID);
			for (int n = 0; n < at->Values[0].Size && Item; ++n)
			{
				UpdateWatch(Item, Str, StrLen, Address);
				Address += e->Elem.Length;
				Item = GetNext(Item, 0);
			}
		}
		return true;
	}

	bool CTypeViewerWnd::UpdateBaseClassPointer(CListStringItem *Item, ULONG_PTR Address)
	{
#ifdef TXTDEBUG
fprintf(f,"%s\n", __func__);
fflush(f);
#endif

		TVInfo info;
		if (!GetItems(Item, &info)) return false;
		unsigned long NewID = info.Name.ID;
		if (!info.Name.pSDSMod) return false;
		if (info.Name.ID == 0) return false;
		if (info.Value.Options & TV_DATA_MASK)
		{
			DATATYPE *data = info.Name.pSDSMod->GetLocal(info.Name.ID);
			if (!data) return false;
			NewID = data->TypeID;
		}
		POINTERTYPE *pt = (POINTERTYPE *)info.Name.pSDSMod->GetGlobal(NewID);
		if (!pt) return false;
		if (pt->Elem.Type != ELEM_POINTER) return false;
		if (pt->Num != 1) return false;
		
		UDTYPE *udt = (UDTYPE*)info.Name.pSDSMod->GetGlobal(pt->ID);
		if (!udt) return false;
		if (udt->Elem.Type == ELEM_CLASS ||
		    udt->Elem.Type == ELEM_STRUCT ||
		    udt->Elem.Type == ELEM_UNION)
		{
			for (int n = 0; n < udt->BaseClassLen; ++n)
			{
				BASECLASSTYPE *bt = (BASECLASSTYPE *)info.Name.pSDSMod->GetGlobal(udt->BaseClass[n]);
				if (!bt) return false; //+++ ???
				SetAddress(Item, bt->ID, Address + bt->Offset);
			}
			return true;
		}
		return false;
	}

	bool CTypeViewerWnd::UpdateStructPointer(CListStringItem *Item, WCHAR *Str, unsigned int StrLen, ULONG_PTR Address)
	{
		if (!Item) return false;
#ifdef TXTDEBUG
fprintf(f,"%s\n", __func__);
fflush(f);
#endif

		UpdateBaseClassPointer(Item, Address);
		Item = FullFindItem(Item, 0);
		while (Item)
		{
			ULONG_PTR NewAddress = 0;
			GetItemText(Item, TV_NAME, Str, StrLen);
			TVInfo info;
			if (GetItems(Item, &info))
			{
				unsigned long NewID = info.Name.ID;
				if ((info.Value.Options & TV_CLOSE_MASK)==0 &&
				    info.Name.pSDSMod &&
				    info.Name.ID != 0)
				{
					if (info.Value.Options & TV_DATA_MASK)
					{
						DATATYPE *data = info.Name.pSDSMod->GetLocal(info.Name.ID);
						if (data)
						{
							NewID = data->TypeID;
							if (data->locationType == LocIsThisRel)
								NewAddress = Address + data->ThisRel.offset;
						}
					}
					if (info.Name.pSDSMod->GetGlobal(NewID))
						UpdateWatch(Item, Str, StrLen, NewAddress);
				}
			}
			Item = GetNext(Item, 0);
		}
		return true;
	}

	bool CTypeViewerWnd::UpdateStructPointerRef(CListStringItem *Item, WCHAR *Str, unsigned int StrLen, ULONG_PTR Address)
	{
		if (!Item) return false;
#ifdef TXTDEBUG
fprintf(f,"%s\n", __func__);
fflush(f);
#endif

		Item = FullFindItem(Item, 0);
		while (Item)
		{
			ULONG_PTR NewAddress = 0;
			GetItemText(Item, TV_NAME, Str, StrLen);
			TVInfo info;
			if (GetItems(Item, &info))
			{
				unsigned long NewID = info.Name.ID;
				if ((info.Value.Options & TV_CLOSE_MASK)==0 &&
				    info.Name.pSDSMod &&
				    info.Name.ID != 0)
				{
					if (info.Value.Options & TV_DATA_MASK)
					{
						DATATYPE *data = info.Name.pSDSMod->GetLocal(info.Name.ID);
						if (data)
						{
							NewID = data->TypeID;
							if (data->locationType == LocIsThisRel)
								NewAddress = Address + data->ThisRel.offset;
						}
					}
					if (info.Name.pSDSMod->GetGlobal(NewID))
						UpdateWatch(Item, Str, StrLen, NewAddress);
				}
			}
			Item = GetNext(Item, 0);
		}
		return true;
	}

	bool CTypeViewerWnd::UpdateBaseClass(CListStringItem *Item, ULONG_PTR Address)
	{
#ifdef TXTDEBUG
fprintf(f,"%s\n", __func__);
fflush(f);
#endif

		TVInfo info;
		if (!GetItems(Item, &info)) return false;
		unsigned long NewID = info.Name.ID;
		if (!info.Name.pSDSMod) return false;
		if (info.Name.ID == 0) return false;
		if (info.Value.Options & TV_DATA_MASK)
		{
			DATATYPE *data = info.Name.pSDSMod->GetLocal(info.Name.ID);
			if (!data) return false;
			NewID = data->TypeID;
		}
		UDTYPE *udt = (UDTYPE*)info.Name.pSDSMod->GetGlobal(NewID);
		if (!udt) return false;
		if (udt->Elem.Type == ELEM_CLASS ||
		    udt->Elem.Type == ELEM_STRUCT ||
		    udt->Elem.Type == ELEM_UNION)
		{
			for (int n = 0; n < udt->BaseClassLen; ++n)
			{
				BASECLASSTYPE *bt = (BASECLASSTYPE *)info.Name.pSDSMod->GetGlobal(udt->BaseClass[n]);
				if (!bt) return false; //+++ ???
				SetAddress(Item, bt->ID, Address + bt->Offset);
			}
			return true;
		}
		return false;
	}

	bool CTypeViewerWnd::UpdateStruct(CListStringItem *Item, WCHAR *Str, unsigned int StrLen, ULONG_PTR Address)
	{
		if (!Item) return false;
#ifdef TXTDEBUG
fprintf(f,"%s\n", __func__);
fflush(f);
#endif
		UpdateBaseClass(Item, Address);
		CListStringItem *NextItem = FullFindItem(Item, 0);
		while (NextItem)
		{
			GetItemText(NextItem, TV_NAME, Str, StrLen);
			TVInfo info;
			if (GetItems(NextItem, &info))
			{
				ULONG_PTR NewAddress = info.Value.Address;
				unsigned long NewID = info.Name.ID;
				if ((info.Value.Options & TV_CLOSE_MASK)==0 &&
				    info.Name.pSDSMod &&
				    info.Name.ID != 0)
				{
					if (info.Value.Options & TV_DATA_MASK)
					{
						DATATYPE *data = info.Name.pSDSMod->GetLocal(info.Name.ID);
						if (data)
						{
							NewID = data->TypeID;
							if (data->locationType == LocIsThisRel)
								NewAddress = Address + data->ThisRel.offset;
						}
					}
					if (info.Name.pSDSMod->GetGlobal(NewID))
						UpdateWatch(NextItem, Str, StrLen, NewAddress);
				}
			}
			NextItem = GetNext(NextItem, 0);
		}
		return true;
	}

	bool CTypeViewerWnd::UpdateWatch(CListStringItem *Item, WCHAR *Str, unsigned int StrLen, ULONG_PTR Address)
	{
		TVInfo info;
		memset(&info, 0, sizeof(info));
		if (!GetItems(Item, &info)) return false;

		if (!Address)
			Address = info.Value.Address;
		SetItemComplexValue(Item, TV_VALUE, SETPAIR(Address, info.Value.Options));
		if (info.Value.Options & TV_CLOSE_MASK) return true;
		unsigned long NewID = info.Name.ID;
		if (!info.Name.pSDSMod) return false;
		if (info.Value.Options & TV_DATA_MASK)
		{
			DATATYPE *data = info.Name.pSDSMod->GetLocal(info.Name.ID);
			if (!data) return false;
			NewID = data->TypeID;
		}
		BASICTYPE *type = info.Name.pSDSMod->GetGlobal(NewID);
		if (!type) return false;
		switch (type->Elem.Type)
		{
		case ELEM_BASE_TYPE:{
#ifdef TXTDEBUG
fprintf(f,"ELEM_BASE_TYPE %s\n", __func__);
fflush(f);
#endif
			DATATYPE *data = info.Name.pSDSMod->GetLocal(info.Name.ID);
			unsigned long long Value = 0;
			int Len = 0;
			GetOutputValue(info.Name.pSDSMod, type, Address, info.Value.Options, Str, &Len, &Value);
			bool Changed = false;
			if (data && data->locationType == LocIsBitField) //+++ TEST IT
			{
				unsigned long long BitValue = Value >> data->BitField.bitPosition;
				BitValue &= ~(~0LL << data->BitField.length); //TODO ???

				unsigned long long OldBitValue = info.Type.X >> data->BitField.bitPosition;
				OldBitValue &= ~(~0LL << data->BitField.length); //TODO ???

				Changed = OldBitValue != BitValue;
			} else
			{
				Changed = info.Type.X != Value;
			}

			SetItemColor(Item, TV_VALUE, 0xFFFFFF);
			if (Changed)
				SetItemColor(Item, TV_VALUE, 0xFF0000);

			if (data && data->locationType == LocIsBitField) //+++ TEST IT
			{
				unsigned long long BitValue = Value >> data->BitField.bitPosition;
				BitValue &= ~(~0LL << data->BitField.length); //TODO ???
				if (info.Value.Options & TV_HEX_MASK)
					TSPrintf(&Str[Len], WSTR(" (%I64x)"), BitValue);
				else	TSPrintf(&Str[Len], WSTR(" (%I64d)"), BitValue);
			}

			SetItemText(Item, TV_VALUE, Str);
			SetItemComplexValue(Item, TV_TYPE, SETPAIR(Value, 0));
			} break;
		case ELEM_POINTER:{
#ifdef TXTDEBUG
fprintf(f,"ELEM_POINTER %s\n", __func__);
fflush(f);
#endif
			POINTERTYPE *pt = (POINTERTYPE*)type;
			unsigned long long Value = 0;
			if (gpSyser->m_pDebugger->ReadMemory(Address, &Value, pt->Elem.Length) != pt->Elem.Length) //???
				break;
			TSPrintf(Str, WSTR("0x" F0ADDR "x"), Value); //TODO X64
			SetItemColor(Item, TV_VALUE, 0xFFFFFF);
			if (info.Type.X != Value)
				SetItemColor(Item, TV_VALUE, 0xFF0000);
			SetItemText(Item, TV_VALUE, Str);
			if (pt->Num == 1)
			{
				BASICTYPE *bt = info.Name.pSDSMod->GetGlobal( ((POINTERTYPE*)type)->ID);
				if (!bt) break;
				switch (bt->Elem.Type)
				{
				case ELEM_BASE_TYPE:{
						if (bt->Elem.Length == 1)
						{
							char *str = (char*)Str; //??? jut reuse it, latter will fill WCHAR*
							TSPrintf(str, "0x" F0ADDR "x \"", Value); //TODO X64
							unsigned long r = gpSyser->m_pDebugger->ReadMemory(Value, str + ADDR_SIZE+4, StrLen*2 - (ADDR_SIZE+4+3));
							for (int n = 0; n < r; ++n)
							{
								if (!str[n+ADDR_SIZE+4])
								{
									TStrCpy(&str[n+ADDR_SIZE+4], "\"");
									SetItemText(Item, TV_VALUE, str);
									break;
								}
							}
							Item = FullFindItem(Item, 0);

							ULONGLONG Addr = 0;
							ULONGLONG Options = 0;
							GetItemComplexValue(Item, TV_VALUE, GETPAIR(&Addr, &Options));
							SetItemComplexValue(Item, TV_VALUE, SETPAIR(Value, Options));

							GetOutputValue(info.Name.pSDSMod, bt, Value, info.Value.Options, Str, nullptr, &Value);
							SetItemText(Item, TV_VALUE, Str);
						}
					} break;
				case ELEM_CLASS:
				case ELEM_STRUCT:
				case ELEM_UNION:{
					UpdateStructPointer(Item, Str, StrLen, Value);
					} break;
				}
			}
			} break;
		case ELEM_POINTERREF:{
#ifdef TXTDEBUG
fprintf(f,"ELEM_POINTERREF %s\n", __func__);
fflush(f);
#endif
			ULONG_PTR Value = 0;
			if (gpSyser->m_pDebugger->ReadMemory(Address, &Value, sizeof(Value)) != sizeof(Value)) //???
				break;
			TSPrintf(Str, WSTR("0x" F0ADDR "x"), Value); //TODO X64
			SetItemText(Item, TV_VALUE, Str);
			BASICTYPE *bt = info.Name.pSDSMod->GetGlobal( ((POINTERREFTYPE*)type)->ID);
			if (!bt) break;
			switch (bt->Elem.Type)
			{
			case ELEM_BASE_TYPE:{
				ULONGLONG Addr = 0;
				ULONGLONG Options = 0;
				GetItemComplexValue(Item, TV_VALUE, GETPAIR(&Addr, &Options));
				SetItemComplexValue(Item, TV_VALUE, SETPAIR(Value, Options));
				unsigned long long v = 0;
				GetOutputValue(info.Name.pSDSMod, bt, Value, info.Value.Options, Str, nullptr, &v);
				SetItemText(Item, TV_VALUE, Str);
				} break;
			case ELEM_STRUCT:
			case ELEM_CLASS:
			case ELEM_UNION:{
				UpdateStructPointerRef(Item, Str, StrLen, Value);
				} break;
			}
			} break;
		case ELEM_ARRAY:{
#ifdef TXTDEBUG
fprintf(f,"ELEM_ARRAY %s\n", __func__);
fflush(f);
#endif
			TSPrintf(Str, WSTR("0x" F0ADDR "x"), Address); //TODO X64
			SetItemText(Item, TV_VALUE, Str);
			UpdateArray(Item, Str, StrLen, Address);
			} break;
		case ELEM_ENUM:{
#ifdef TXTDEBUG
fprintf(f,"ELEM_ENUM %s\n", __func__);
fflush(f);
#endif
			unsigned long long Value = 0;
			BASICTYPE *bt = info.Name.pSDSMod->GetGlobal( ((ENUMTYPE*)type)->ID);
			if (GetOutputValue(info.Name.pSDSMod, bt, Address, info.Value.Options, Str, nullptr, &Value))
			{
				const char *name = info.Name.pSDSMod->GetEnumName(NewID, Value);
				if (name)
				{
					SetItemText(Item, TV_VALUE, name);
					break;
				}
			}
			SetItemText(Item, TV_VALUE, Str);
			} break;
		case ELEM_STRUCT:
		case ELEM_CLASS:
		case ELEM_UNION:{
#ifdef TXTDEBUG
fprintf(f,"ELEM_UDT %s\n", __func__);
fflush(f);
#endif
			UpdateStruct(Item, Str, StrLen, Address);
			} break;
		}
		return true;
	}

	bool CTypeViewerWnd::ReadX(const char *Name, ULONG_PTR *Address, FUNCTION **Func, CSDSModule **SDSMod)
	{
		CSDSModule *pSDSMod = gpSyser->m_SyserUI.FindSDSModule(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP);
		if (!pSDSMod) return false;
		ULONG_PTR ImageBase = pSDSMod->m_pDbgModule->m_ImageBase;

		FUNCTION *pFunc = pSDSMod->GetFunction(Name, true);
		if (!pFunc)
		{
			pFunc = pSDSMod->GetFunction(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP - ImageBase);
			if (!pFunc) return false;
			if (pFunc->ClassID == 0) return false;
			pFunc = pSDSMod->GetFunction(Name, (UDTYPE*)pSDSMod->GetGlobal(pFunc->ClassID), false);
			if (!pFunc) return false;
		}
		*Address = ImageBase + pFunc->rva;
		*Func = pFunc;
		*SDSMod = pSDSMod;
		return true;
	}

	bool CTypeViewerWnd::ReadX(unsigned long regid, ULONG_PTR *Value)
	{
		switch (regid)
		{
	#ifdef _X64_
                case CV_AMD64_RAX:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCAX;
			return true;
                case CV_AMD64_RCX:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCCX;
			return true;
                case CV_AMD64_RDX:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCDX;
			return true;
                case CV_AMD64_RBX:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBX;
			return true;
                case CV_AMD64_RSP:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP;
			return true;
                case CV_AMD64_RBP:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBP;
			return true;
                case CV_AMD64_RSI:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSI;
			return true;
                case CV_AMD64_RDI:
                       	*Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCDI;
			return true;
		default:;
	#else
                case CV_REG_EAX:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCAX;
			return true;
                case CV_REG_ECX:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCCX;
			return true;
                case CV_REG_EDX:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCDX;
			return true;
                case CV_REG_EBX:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBX;
			return true;
                case CV_REG_ESP:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSP;
			return true;
                case CV_REG_EBP:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCBP;
			return true;
                case CV_REG_ESI:
                        *Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCSI;
			return true;
                case CV_REG_EDI:
                       	*Value = *gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCDI;
			return true;
		default:;
	#endif
		}
		return false;
	}

	bool CTypeViewerWnd::ReadX(CSDSModule *pSDSMod, FUNCTION *pFunc, ULONG_PTR *Value)
	{
		DATATYPE *data = pSDSMod->GetClassLocal("this", pFunc);
		if (!data) return false;
		ULONG_PTR Address = 0;
		ULONG_PTR MemValue = 0;
		switch (data->locationType)
		{
		case LocIsStatic:
			Address = pSDSMod->m_pDbgModule->m_ImageBase + data->Static.rva;
			break;
		case LocIsRegRel: {
			if (!ReadX(data->RegRel.registerId, &MemValue)) return false;
			Address = MemValue + data->RegRel.offset;
			} break;
		case LocIsEnregistered: {
			if (!ReadX(data->Enregistered.registerId, &MemValue)) return false;
			Address = MemValue;
			} break;
		default:
			return false;
		}
		if (gpSyser->m_pDebugger->ReadMemory(Address, &MemValue, sizeof(MemValue)) == sizeof(MemValue))
		{
			if (Value) *Value = MemValue;
			return true;
		}
		return false;
	}

	bool CTypeViewerWnd::ReadX(const char *Name, ULONG_PTR *Address, DATATYPE **Data, CSDSModule **SDSMod)
	{
		CSDSModule *pSDSMod = gpSyser->m_SyserUI.FindSDSModule(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP);
		if (!pSDSMod) return false;
		ULONG_PTR ImageBase = pSDSMod->m_pDbgModule->m_ImageBase;
		DATATYPE *data = nullptr;
		FUNCTION *pFunc = pSDSMod->GetFunction(*gpSyser->m_SyserUI.m_SyserCodeDoc.m_CPUREGPTR.pCIP - ImageBase);
		if (!pFunc)
		{
			data = pSDSMod->GetLocal(Name, DataIsGlobal);
			if (!data) return false;
		} else
		{
			data = pSDSMod->GetClassLocal(Name, pFunc);
			if (!data)
			{
				data = pSDSMod->GetLocal(Name, DataIsGlobal);
				if (!data) return false;
			}
		}
		switch (data->locationType)
		{
		case LocIsStatic: {
			*Address = ImageBase + data->Static.rva;
			*Data = data;
			*SDSMod = pSDSMod;
			return true;
			} break;
		case LocIsRegRel: {
			ULONG_PTR Value = 0;
			if (!ReadX(data->RegRel.registerId, &Value)) return false;
			*Address = Value + data->RegRel.offset;
			*Data = data;
			*SDSMod = pSDSMod;
			return true;
			} break;
		case LocIsThisRel: {
			ULONG_PTR Value = 0;
			if (!ReadX(pSDSMod, pFunc, &Value)) return false;
			*Address = Value + data->ThisRel.offset;
			*Data = data;
			*SDSMod = pSDSMod;
			return true;
			} break;
		default:;
		}
		return false;
	}


	void CTypeViewerWnd::UpdateContext()
	{
		WCHAR Name[512];

#ifdef TXTDEBUG
fprintf(f,"%s >\n", __func__);
fflush(f);
#endif
		int n = 0;
		CListStringItem *Item = GetItem(0,0);
		while (Item != m_pFirstItem)
		{
			if (GetItemText(Item, TV_NAME, Name, lenof(Name)))
			{
				char szName[lenof(Name)];
				TStrCpy(szName, Name);
#ifdef TXTDEBUG
fprintf(f,"N %d, VarName %s %s\n", n, szName,__func__);
fflush(f);
#endif
				ULONGLONG Options = 0;
				ULONGLONG Addr = 0;
				GetItemComplexValue(Item, TV_VALUE, GETPAIR(&Addr, &Options));
				if (!(Options & TV_STATIC_MASK))
			  { //+++
				ULONG_PTR Address = 0;
				DATATYPE *Data = nullptr;
				CSDSModule *pSDSMod = nullptr;
				TVInfo info;
				if (!ReadX(szName, &Address, &Data, &pSDSMod))
				{
					DeleteAllChildrenItems(Item);

					ULONGLONG Options = 0;
					ULONGLONG Addr = 0;
					GetItemComplexValue(Item, TV_VALUE, GETPAIR(&Addr, &Options));
					Options |= TV_CLOSE_MASK;
					SetItemComplexValue(Item, TV_VALUE, SETPAIR(Addr, Options));

					TStrCpy(szName, "Error: symbol \"");
					TStrCat(szName, Name);
					TStrCat(szName, "\" not found");
					SetItemText(Item, TV_VALUE, szName);
					SetItemText(Item, TV_TYPE, "");
				} else
				if (GetItems(Item, &info))
				{
					if (info.Value.Options & TV_CLOSE_MASK)
					{
						AddWatch(Name, Item);
					} else
					if (info.Value.Options & TV_DATA_MASK)
					{
						if (Data->ID != info.Name.ID)
						{
							DeleteAllChildrenItems(Item);
							AddWatch(Name, Item);
						} else
						if (info.Value.Address != Address)
						{
							info.Value.Address = Address;
							SetItemComplexValue(Item, TV_VALUE, SETPAIR(info.Value.Address, info.Value.Options));
						}
					}
				}
			  } //+++
			}
#ifdef TXTDEBUG
fprintf(f,"N %d %s\n", n, __func__);
fflush(f);
#endif
			++n;
			Item = GetNext(Item, 0);
		}

		{
		CListStringItem *Item = GetItem(0, 0);
		while (Item != m_pFirstItem)
		{
			UpdateWatch(Item, Name, lenof(Name), 0);
#ifdef TXTDEBUG
			char szName[512];
			TStrCpy(szName, Name);
fprintf(f,"update VarName %s %s\n", szName,__func__);
fflush(f);
#endif
			Item = GetNext(Item, 0);
		}
		}
#ifdef TXTDEBUG
fprintf(f,"%s <\n", __func__);
fflush(f);
#endif
	}

	void CTypeViewerWnd::RemoveSDSModule(CSDSModule *pSDSMod)
	{
		TVInfo info;
		TList<CListStringItem *> List;
		CListStringItem *Item = GetNext(0, 0);
		while (Item != m_pFirstItem)
		{
			if (GetItems(Item, &info) &&
				//info.Value.Options >= 0 && //??? !( & 0x8000'0000)
				!(info.Value.Options & TV_CLOSE_MASK) &&
				pSDSMod == info.Name.pSDSMod)
			{
				List.Append(&Item);
			}
			Item = GetNext(Item, 0);
		}
		TList<CListStringItem *>::IT It = List.Begin();
		while (It != List.End())
		{
			DeleteItem(*It);
			++It;
		}
	}

	void CTypeViewerWnd::ShowBaseType(CSDSModule *pSDSMod, unsigned long ID, ULONG_PTR Address, unsigned long Options, CListStringItem *Item, const char *VarName, bool bLocal)
	{
		WCHAR DisplayOut[60];
		TStrCpy(DisplayOut, WSTR("" UNREADSTR ""));

#ifdef TXTDEBUG
fprintf(f,"%s\n", __func__);
fflush(f);
#endif
		CWispDIB *pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11D);
		if (pSDSMod)
		{
			unsigned long NewID = ID;
			if (bLocal)
			{
				DATATYPE *data = pSDSMod->GetLocal(ID);
				if (!data) return;
				NewID = data->TypeID;
				if (!VarName)
					VarName = data->name;
				switch (data->access)
				{
				case CV_private:
					pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11F);
					break;
				case CV_protected:
					pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11E);
					break;
				case CV_public:
					pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11D);
					break;
				}
			}

			BASICTYPE *type = pSDSMod->GetGlobal(NewID);
			if (type)
			{
				Options = (Options & ~(TV_DATA_MASK | TV_FUNC_MASK)) | (bLocal ? TV_DATA_MASK : TV_FUNC_MASK);
				CListStringItem *SubItem = InsertItemStr(VarName, Item, 0, pDIB, -1);
				SetItemComplexValue(SubItem, TV_NAME, SETPAIR(pSDSMod, (!bLocal ? NewID : ID)));
				SetItemComplexValue(SubItem, TV_VALUE, SETPAIR(Address, Options));
			#if 1 //TODO data unavail
				DATATYPE *data = pSDSMod->GetLocal(ID);
				if (data && data->locationType == LocIsBitField) //+++ TEST IT
				{
					WCHAR Str[30];
					int Len = TStrCpy(Str, type->Elem.Name);
					if (data->BitField.length == 1)
						TSPrintf(&Str[Len], WSTR(" (Bit %d)"), data->BitField.bitPosition);
					else	TSPrintf(&Str[Len], WSTR(" (Bit %d-%d)"), data->BitField.bitPosition, data->BitField.bitPosition+data->BitField.length-1);
					
					SetItemText(SubItem, TV_TYPE, Str);
#ifdef TXTDEBUG
fprintf(f,"%s %s %d, %d\n", __func__, data->name, data->BitField.bitPosition, data->BitField.length);
fflush(f);
#endif
				} else
			#endif
				{
					SetItemText(SubItem, TV_TYPE, type->Elem.Name);
				}
				SetItemFormat(SubItem, TV_OFFSET, Address, WSTR("0x" F0ADDR "x")); //TODO X64
				unsigned long long Value = 0;
				int Len = 0;
				GetOutputValue(pSDSMod, type, Address, Options, DisplayOut, &Len, &Value);
				SetItemComplexValue(SubItem, TV_TYPE, SETPAIR(Value, 0)); //???----------------
				if (data && data->locationType == LocIsBitField) //+++ TEST IT
				{
					unsigned long long BitValue = Value >> data->BitField.bitPosition;
					BitValue &= ~(~0LL << data->BitField.length); //TODO ???
					if (Options & TV_HEX_MASK)
						TSPrintf(&DisplayOut[Len], WSTR(" (%I64x)"), BitValue);
					else	TSPrintf(&DisplayOut[Len], WSTR(" (%I64d)"), BitValue);
				}
				SetItemText(SubItem, TV_VALUE, DisplayOut);
			}
		}
	}

	void CTypeViewerWnd::ShowPointer(CSDSModule *pSDSMod, unsigned long ID, ULONG_PTR Address, unsigned long Options, CListStringItem *Item, const char *VarName, bool bLocal)
	{
		WCHAR szUnread[sizeof(UNREADSTR)+1];
		TStrCpy(szUnread, WSTR("" UNREADSTR ""));

		char OutputType[512];
		int OutputTypeLen = 512;

#ifdef TXTDEBUG
fprintf(f,"%s\n", __func__);
fflush(f);
#endif

		CWispDIB *pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11D);
		if (pSDSMod)
		{
			unsigned long NewID = ID;
			if (bLocal)
			{
				DATATYPE *data = pSDSMod->GetLocal(ID);
				if (!data) return;
				NewID = data->TypeID;
				//if (!VarName)
				//	VarName = data->name;
				switch (data->access)
				{
				case CV_private:
					pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11F);
					break;
				case CV_protected:
					pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11E);
					break;
				case CV_public:
					pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11D);
					break;
				}
			}

			POINTERTYPE *pt = (POINTERTYPE*)pSDSMod->GetGlobal(NewID);
			if (pt && pt->Elem.Type == ELEM_POINTER)
			{
				unsigned long long Value = 0;
				unsigned long Result = 0;
				if (pt->Elem.Length && pt->Elem.Length <= 8)
					Result = gpSyser->m_pDebugger->ReadMemory(Address, &Value, pt->Elem.Length);
				WCHAR szValue[30];
				if (Result == pt->Elem.Length && Result)
					TSPrintf(szValue, WSTR("0x" F0ADDR "x"), Value); //TODO X64 ???
				else	TStrCpy(szValue, szUnread);
				BASICTYPE *bt = pSDSMod->GetGlobal(pt->ID);
				pSDSMod->GetOutput(pt, OutputType, &OutputTypeLen);
				bool bvoidptr = !TStrCmp(OutputType, "void*");
				if (bt->Elem.Type == ELEM_FUNC_TYPE)
					bvoidptr = true;
				OutputType[0] = 0;
				pSDSMod->GetOutput(pt, OutputType, &OutputTypeLen);
				CListStringItem *NewItem = nullptr;
				if (VarName)
					NewItem = InsertItemStr(VarName, Item, 0, pDIB, -1);
				else	NewItem = InsertItemStr(OutputType, Item, 0, pDIB, -1);
				SetItemText(NewItem, TV_TYPE, OutputType);
				SetItemComplexValue(NewItem, TV_TYPE, SETPAIR(Value, 0)); //???
				SetItemComplexValue(NewItem, TV_NAME, SETPAIR(pSDSMod, (!bLocal ? NewID : ID)));
				SetItemText(NewItem, TV_VALUE, szValue);
				unsigned long PointerOptions = (bLocal ? TV_DATA_MASK : TV_FUNC_MASK) | (Options & ~(TV_FUNC_MASK|TV_DATA_MASK));
				SetItemComplexValue(NewItem, TV_VALUE, SETPAIR(Address, PointerOptions));
				SetItemFormat(NewItem, TV_OFFSET, Address, WSTR("0x" F0ADDR "x ")); //TODO X64
				SetItemComplexValue(NewItem, TV_TYPE, SETPAIR(0, 0));
				if (bt->Elem.Type == ELEM_BASE_TYPE && bt->Elem.Length == 1)
				{
					TSPrintf(OutputType, "0x" F0ADDR "x \"", Value); //TODO X64
					unsigned long Result = gpSyser->m_pDebugger->ReadMemory(Value, &OutputType[ADDR_SIZE+4], 512-(ADDR_SIZE+4+3)); //15
					for (int n = 0; n < Result; ++n)
					{
						if (!OutputType[n+ADDR_SIZE+4]) //TODO X64 +12
						{
							OutputType[n+ADDR_SIZE+5] = 0;
							OutputType[n+ADDR_SIZE+4] = '"';
							SetItemText(NewItem, TV_VALUE, OutputType);
							break;
						}
					}
				}

				if (!bvoidptr)
				{
					NewItem = InsertItem(WSTR(".."), NewItem, 0, nullptr, -1);
					SetItemComplexValue(NewItem, TV_VALUE, SETPAIR(Address, (PointerOptions | TV_CLOSE_MASK)));
				}
				SetItemText(NewItem, TV_TYPE, OutputType);
				SetItemComplexValue(NewItem, TV_TYPE, SETPAIR(0, ID)); //???
			}
		}
	}

	void CTypeViewerWnd::ShowArray(CSDSModule *pSDSMod, unsigned long ID, ULONG_PTR Address, unsigned long Options, CListStringItem *Item, const char *VarName, bool bLocal)
	{
		char DisplayOut[512];
		int DisplayOutLen = 512;

#ifdef TXTDEBUG
fprintf(f,"%s\n", __func__);
fflush(f);
#endif

		CWispDIB *pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11D);
		if (pSDSMod)
		{
			unsigned long NewID = ID;
			if (bLocal)
			{
				DATATYPE *data = pSDSMod->GetLocal(ID);
				if (!data) return;
				NewID = data->TypeID;
				if (!VarName)
					VarName = data->name;
				switch (data->access)
				{
				case CV_private:
					pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11F);
					break;
				case CV_protected:
					pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11E);
					break;
				case CV_public:
					pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11D);
					break;
				}
			}

			BASICTYPE *type = pSDSMod->GetGlobal(NewID);
			if (type)
			{
				WCHAR szAddress[30];
				TSPrintf(szAddress, WSTR("0x" F0ADDR "x"), Address); //TODO X64
				pSDSMod->GetOutput(type, DisplayOut, &DisplayOutLen);
				CListStringItem *Item1;
				if (VarName)
					Item1 = InsertItemStr(VarName, Item, 0, pDIB, -1);
				else	Item1 = InsertItemStr(DisplayOut, Item, 0, pDIB, -1);
				SetItemText(Item1, TV_TYPE, DisplayOut);
				SetItemComplexValue(Item1, TV_NAME, SETPAIR(pSDSMod, (!bLocal ? NewID : ID)));
				SetItemText(Item1, TV_VALUE, szAddress);
				Options = (Options & ~(TV_DATA_MASK | TV_FUNC_MASK)) | (bLocal ? TV_DATA_MASK : TV_FUNC_MASK);
				SetItemComplexValue(Item1, TV_VALUE, SETPAIR(Address, Options));
				SetItemFormat(Item1, TV_OFFSET, Address, WSTR("0x" F0ADDR "x")); //TODO X64
				SetItemComplexValue(Item1, TV_TYPE, SETPAIR(0, ID));

				CListStringItem *SubItem = InsertItem(WSTR(".."), Item1, 0, nullptr, -1);
				SetItemComplexValue(SubItem, TV_VALUE, SETPAIR(ID, TV_CLOSE_MASK)); //??? ID
			}
		}
	}

	void CTypeViewerWnd::ShowEnum(CSDSModule *pSDSMod, unsigned long ID, ULONG_PTR Address, unsigned long Options, CListStringItem *Item, const char *VarName, bool bLocal)
	{
		if (pSDSMod)
		{
			unsigned long NewID = ID;
			if (bLocal)
			{
				DATATYPE *data = pSDSMod->GetLocal(ID);
				if (!data) return;
				NewID = data->TypeID;
			}

			BASICTYPE *type = pSDSMod->GetGlobal(NewID);
			if (type && type->Elem.Type == ELEM_ENUM)
			{
				WCHAR szAddress[30];
				TSPrintf(szAddress, WSTR("0x" F0ADDR "x"), Address); //TODO X64
				char DisplayOut[512];
				int DisplayOutLen = 512;
				pSDSMod->GetOutput(type, DisplayOut, &DisplayOutLen);
				if (VarName)
					Item = InsertItemStr(VarName, Item, 0, nullptr, -1);
				else	Item = InsertItemStr(DisplayOut, Item, 0, nullptr, -1);

				SetItemComplexValue(Item, TV_NAME, SETPAIR(pSDSMod, (!bLocal ? NewID : ID)));
				Options = (Options & ~(TV_DATA_MASK | TV_FUNC_MASK)) | (bLocal ? TV_DATA_MASK : TV_FUNC_MASK);
				SetItemComplexValue(Item, TV_VALUE, SETPAIR(Address, Options));
				SetItemComplexValue(Item, TV_TYPE, SETPAIR(0, ID));
				SetItemText(Item, TV_TYPE, DisplayOut);

				unsigned long long Value = 0;
				if (GetOutputValue(pSDSMod, pSDSMod->GetGlobal( ((ENUMTYPE*)type)->ID), Address, Options, szAddress, nullptr, &Value) )
				{
					const char *Name = pSDSMod->GetEnumName(ID, Value);
					if (Name)
						SetItemText(Item, TV_VALUE, Name);
					else	SetItemText(Item, TV_VALUE, szAddress);
				} else
				{
					SetItemText(Item, TV_VALUE, szAddress);
				}
				SetItemFormat(Item, TV_OFFSET, Address, WSTR("%X"));
			}
		}
	}

	void CTypeViewerWnd::ShowStruct(CSDSModule *pSDSMod, unsigned long ID, ULONG_PTR Address, unsigned long Options, CListStringItem *Item, const char *VarName, bool bLocal)
	{
		char DisplayOut[512];
		int DisplayOutLen = 512;

#ifdef TXTDEBUG
fprintf(f,"%s\n", __func__);
fflush(f);
#endif
		CWispDIB *pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11D);
		if (pSDSMod)
		{
			unsigned long NewID = ID;
			if (bLocal)
			{
				DATATYPE *data = pSDSMod->GetLocal(ID);
				if (!data) return;
				NewID = data->TypeID;
				if (!VarName)
					VarName = data->name;
				switch (data->access)
				{
				case CV_private:
					pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11F);
					break;
				case CV_protected:
					pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11E);
					break;
				case CV_public:
					pDIB = gpCurWisp->m_DIBLib.LoadDIBList("\\Toolbar.bmp", 16, 16, 0, 0xFF00FF)->GetDIB(0x11D);
					break;
				}
			}

			BASICTYPE *type = pSDSMod->GetGlobal(NewID);
			if (type && (type->Elem.Type == ELEM_CLASS || type->Elem.Type == ELEM_STRUCT))
			{
			#if 1
				TStaticAnsiString Str(TStrLen(type->Elem.Name)+1+8);
			#else
				char Buffer[256];
				char *pBuffer = Buffer;
				unsigned int Len = TStrLen(type->Elem.Name) + 9; //??? 
				if (Len >= 256)
					pBuffer = new char[Len];
				*pBuffer = 0;
				//TStrCpy(pBuffer, String);
			#endif
				switch (type->Elem.Type)
				{
				case ELEM_STRUCT:
					TSPrintf((char*)Str/*pBuffer*/, "struct %s", type->Elem.Name);
					break;
				case ELEM_CLASS:
					TSPrintf((char*)Str/*pBuffer*/, "class %s", type->Elem.Name);
					break;
				case ELEM_UNION:
					TSPrintf((char*)Str/*pBuffer*/, "union %s", type->Elem.Name);
					break;
				}

				const char *Name = type->Elem.Name; //Str ???
				CListStringItem *NewItem = InsertItemStr(Name, Item, 0, nullptr, -1); //??? pDIB
				SetItemText(NewItem, TV_TYPE, Name);

				if (VarName)
					SetItemText(NewItem, TV_NAME, VarName);
				SetItemComplexValue(NewItem, TV_NAME, SETPAIR(pSDSMod, (!bLocal ? NewID : ID)));
				SetItemText(NewItem, TV_VALUE, WSTR("{...}"));
				SetItemComplexValue(NewItem, TV_TYPE, SETPAIR(0, 0));

				Options = (Options & ~(TV_DATA_MASK | TV_FUNC_MASK)) | (bLocal ? TV_DATA_MASK : TV_FUNC_MASK);
				SetItemComplexValue(NewItem, TV_VALUE, SETPAIR(Address, Options));

				SetItemFormat(NewItem, TV_OFFSET, Address, WSTR("0x" F0ADDR "x")); //TODO X64

				CListStringItem *SubItem = InsertItem(WSTR(".."), NewItem, 0, nullptr, -1);
				Options |= TV_CLOSE_MASK;
				SetItemComplexValue(SubItem, TV_VALUE, SETPAIR(0, Options));
			#if 0
				if (pBuffer != Buffer)
					delete []pBuffer;
			#endif
			}
		}
	}

	void CTypeViewerWnd::Show(CSDSModule *pSDSMod, unsigned long ID, ULONG_PTR Address, unsigned long Options, CListStringItem *Item, const char *VarName)
	{
#ifdef TXTDEBUG
fprintf(f,"%s\n", __func__);
fflush(f);
#endif
		if (pSDSMod)
		{
			BASICTYPE *type = pSDSMod->GetGlobal(ID);
			if (type)
			{
				if (!Item)
				{
					DeleteItem(m_pFirstItem);
					m_pFirstItem = nullptr;
				}

				switch (type->Elem.Type)
				{
				case ELEM_BASE_TYPE:
					ShowBaseType(pSDSMod, ID, Address, Options, Item, VarName, false);
					break;
				case ELEM_POINTER:
					ShowPointer(pSDSMod, ID, Address, Options, Item, VarName, false);
					break;
				case ELEM_ARRAY:
					ShowArray(pSDSMod, ID, Address, Options, Item, VarName, false);
					break;
				case ELEM_ENUM:
					ShowEnum(pSDSMod, ID, Address, Options, Item, VarName, false);
					break;
				case ELEM_STRUCT:
				case ELEM_CLASS:
				case ELEM_UNION:
					ShowStruct(pSDSMod, ID, Address, Options, Item, VarName, false);
					break;
				}

				if (!m_pFirstItem) //+++
				{
					ResetItems();
				}
			}
		}
	}

	bool CTypeViewerWnd::GetOutputValue(CSDSModule *pSDSMod, BASICTYPE *Type, ULONG_PTR Address, unsigned long Options, WCHAR *DisplayOut, int *DisplayOutLen, unsigned long long *Value)
	{
		WCHAR szUnread[sizeof(UNREADSTR)+1];
		TStrCpy(szUnread, WSTR("" UNREADSTR ""));
		int Len = 0;
		*DisplayOut = 0;
		if (DisplayOutLen) *DisplayOutLen = 0;
#ifdef TXTDEBUG
fprintf(f,"%s\n", __func__);
fflush(f);
#endif
		if (pSDSMod && Type && Type->Elem.Type == ELEM_BASE_TYPE)
		{
			*Value = 0;
			unsigned long basetype = GetBasicType(Type->Elem.Name);
			unsigned long Result = 0;
			if (Type->Elem.Length && Type->Elem.Length <= 8)
			{
				Result = gpSyser->m_pDebugger->ReadMemory(Address, Value, Type->Elem.Length);
			}
		#if 0
			if (data && data->locationType == LocIsBitField) //+++ TEST IT
			{
				unsigned long long BitValue = Value >> data->BitField.bitPosition;
				BitValue &= ~(~0LL << data->BitField.length); //TODO ???
				Value = BitValue;
			}
		#endif
			if (Result == Type->Elem.Length && Result)
			{
				switch (basetype)
				{
				case 0x2: { //0x80 char
					char v = *Value;
					if (Options & TV_HEX_MASK)
					{
						if (v <= 0)
							Len = TSPrintf(DisplayOut, WSTR("0x%02x"), v);
						else	Len = TSPrintf(DisplayOut, WSTR("0x%02x '%c'"), v, v);
					} else
					{
						if (v <= 0)
							Len = TSPrintf(DisplayOut, WSTR("%d"), v);
						else	Len = TSPrintf(DisplayOut, WSTR("%d '%c'"), v, v);
					}
					if (DisplayOutLen) *DisplayOutLen = Len;
					return true;
					} break;
				case 0xA: { // bool
					if (*(bool*)Value)
						Len = TStrCpy(DisplayOut, "true");
					else	Len = TStrCpy(DisplayOut, "false");
					if (DisplayOutLen) *DisplayOutLen = Len;
					return true;
					} break;
				case 0xD: { // long
					long v = *Value;
					if (Options & TV_HEX_MASK)
						Len = TSPrintf(DisplayOut, WSTR("0x%x"), v);
					else	Len = TSPrintf(DisplayOut, WSTR("%d"), v);
					if (DisplayOutLen) *DisplayOutLen = Len;
					return true;
					} break;
				case 0xE: { // unsigned long
					unsigned long v = *Value;
					if (Options & TV_HEX_MASK)
						Len = TSPrintf(DisplayOut, WSTR("0x%08x"), v);
					else	Len = TSPrintf(DisplayOut, WSTR("%u"), v);
					if (DisplayOutLen) *DisplayOutLen = Len;
					return true;
					} break;
				case 0x80: { //0x2 char
					char v = *Value;
					if (Options & TV_HEX_MASK)
					{
						if (v <= 0)
							Len = TSPrintf(DisplayOut, WSTR("0x%02x"), v);
						else	Len = TSPrintf(DisplayOut, WSTR("0x%02x '%c'"), v, v);
					} else
					{
						if (v <= 0)
							Len = TSPrintf(DisplayOut, WSTR("%d"), v);
						else	Len = TSPrintf(DisplayOut, WSTR("%d '%c'"), v, v);
					}
					if (DisplayOutLen) *DisplayOutLen = Len;
					return true;
					} break;
				case 0x100: { // short
					short v = *Value;
					if (Options & TV_HEX_MASK)
						Len = TSPrintf(DisplayOut, WSTR("0x%x"), v);
					else	Len = TSPrintf(DisplayOut, WSTR("%d"), v);
					if (DisplayOutLen) *DisplayOutLen = Len;
					return true;
					} break;
				case 0x200: { // int
					int v = *Value;
					if (Options & TV_HEX_MASK)
						Len = TSPrintf(DisplayOut, WSTR("0x%x"), v);
					else	Len = TSPrintf(DisplayOut, WSTR("%d"), v);
					if (DisplayOutLen) *DisplayOutLen = Len;
					return true;
					} break;
				case 0x400: { // __int64
					__int64 v = *Value;
					if (Options & TV_HEX_MASK)
						Len = TSPrintf(DisplayOut, WSTR("0x%I64x"), v);
					else	Len = TSPrintf(DisplayOut, WSTR("%I64d"), v);
					if (DisplayOutLen) *DisplayOutLen = Len;
					return true;
					} break;
				case 0x800: { // unsigned char
					unsigned char v = *Value;
					if (Options & TV_HEX_MASK)
					{
						if (v > 0x80 || v == 0)
							Len = TSPrintf(DisplayOut, WSTR("0x%02x"), v);
						else	Len = TSPrintf(DisplayOut, WSTR("0x%0x '%c'"), v, v);
					} else
					{
						if (v > 0x80 || v == 0)
							Len = TSPrintf(DisplayOut, WSTR("%d"), v);
						else	Len = TSPrintf(DisplayOut, WSTR("%d '%c'"), v, v);
					}
					if (DisplayOutLen) *DisplayOutLen = Len;
					return true;
					} break;
				case 0x1000: { // unsigned short
					unsigned short v = *Value;
					if (Options & TV_HEX_MASK)
						Len = TSPrintf(DisplayOut, WSTR("0x%x"), v);
					else	Len = TSPrintf(DisplayOut, WSTR("%u"), v);
					if (DisplayOutLen) *DisplayOutLen = Len;
					return true;
					} break;
				case 0x2000: { // unsigned int
					unsigned int v = *Value;
					if (Options & TV_HEX_MASK)
						Len = TSPrintf(DisplayOut, WSTR("0x%08x"), v);
					else	Len = TSPrintf(DisplayOut, WSTR("%u"), v);
					if (DisplayOutLen) *DisplayOutLen = Len;
					return true;
					} break;
				case 0x4000: { // unsigned __int64
					unsigned __int64 v = *Value;
					if (Options & TV_HEX_MASK)
						Len = TSPrintf(DisplayOut, WSTR("0x%I64x"), v);
					else	Len = TSPrintf(DisplayOut, WSTR("%I64u"), v);
					if (DisplayOutLen) *DisplayOutLen = Len;
					return true;
					} break;
				}
			}
			Len = TStrCpy(DisplayOut, szUnread);
			if (DisplayOutLen) *DisplayOutLen = Len;
		}
		return false;
	}

	bool CTypeViewerWnd::GetItems(CListStringItem *Item, TVName *Name, TVValue *Value, TVType *Type, TVOffset *Offset)
	{
		if (!Item) return false;
		ULONGLONG v1,v2;
		GetItemComplexValue(Item, TV_NAME, GETPAIR(&v1, &v2));
		Name->pSDSMod = (CSDSModule*)v1;
		Name->ID = v2;
		GetItemComplexValue(Item, TV_VALUE, GETPAIR(&v1, &v2));
		Value->Address = v1;
		Value->Options = v2;
		GetItemComplexValue(Item, TV_TYPE, GETPAIR(&v1, &v2));
		Type->X = v1;
		Type->ID = v2;
		GetItemComplexValue(Item, TV_OFFSET, GETPAIR(&v1, &v2));
		Offset->X = v1;
		Offset->ID = v2;
		return true;
	}

	bool CTypeViewerWnd::GetItems(CListStringItem *Item, TVInfo *info)
	{
		if (!Item) return false;
		ULONGLONG v1,v2;
		GetItemComplexValue(Item, TV_NAME, GETPAIR(&v1, &v2));
		info->Name.pSDSMod = (CSDSModule*)v1;
		info->Name.ID = v2;
		GetItemComplexValue(Item, TV_VALUE, GETPAIR(&v1, &v2));
		info->Value.Address = v1;
		info->Value.Options = v2;
		GetItemComplexValue(Item, TV_TYPE, GETPAIR(&v1, &v2));
		info->Type.X = v1;
		info->Type.ID = v2;
		GetItemComplexValue(Item, TV_OFFSET, GETPAIR(&v1, &v2));
		info->Offset.X = v1;
		info->Offset.ID = v2;
		return true;
	}

	bool CTypeViewerWnd::SetItemComplexValue(CListStringItem *Item, int nCol, ULONGLONG v1, ULONGLONG v2)
	{
		CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
		if (!Content) return false;
		Content->v1 = v1;
		Content->v2 = v2;
		return true;
	}

	bool CTypeViewerWnd::GetItemComplexValue(CListStringItem *Item, int nCol, ULONGLONG *v1, ULONGLONG *v2)
	{
		CListStringItem::CONTENT *Content = GetItemContent(Item, nCol);
		if (!Content) return false;
		*v1 = Content->v1;
		*v2 = Content->v2;
		return true;
	}

	bool CTypeViewerWnd::SetAddress(CListStringItem *Item, unsigned long ID, ULONG_PTR Address)
	{
		Item = FullFindItem(Item, 0);
		while (Item)
		{
			TVInfo info;
			if (GetItems(Item, &info) && info.Name.ID == ID && (info.Value.Options & TV_FUNC_MASK)) //FUNC
			{
				WCHAR Buffer[256];
				GetItemText(Item, TV_NAME, Buffer, lenof(Buffer));
				SetItemComplexValue(Item, TV_VALUE, SETPAIR(Address, info.Value.Options));
				return true;
			}
			Item = GetNext(Item, 0);
		}
		return false;
	}

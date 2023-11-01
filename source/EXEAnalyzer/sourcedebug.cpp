
//#ifndef _RING0_
#include "pefile.hpp"
#include "sourcedebug.hpp"
#if 1
	class CDiaBSTR 
	{
		WCHAR *m_bstr;
	public:
		CDiaBSTR() { m_bstr = NULL; }
		~CDiaBSTR() { release(); }
		void release() { if (m_bstr != NULL) ::SysFreeString(m_bstr); m_bstr = NULL; } //LocalFree(m_bstr);
		WCHAR **operator &() { /*assert(m_bstr == NULL);*/ return &m_bstr; }
		operator WCHAR*() { /*assert(m_bstr != NULL);*/ return m_bstr; }
	};

	int TStrLen(const WCHAR *s) { return ::TStrLen<WCHAR>(s); }
	int TStrCpy(char *d, const WCHAR *s) { return ::TStrCpy<char,WCHAR>(d, s); }
#else
	typedef WCHAR* BSTR;
	class CDiaBSTR 
	{
		BSTR m_bstr;
	public:
		CDiaBSTR() { m_bstr = NULL; }
		~CDiaBSTR() { release(); }
		void release() { if (m_bstr != NULL) ::SysFreeString(m_bstr); m_bstr = NULL; }
		BSTR *operator &() { /*assert(m_bstr == NULL);*/ return &m_bstr; }
		operator BSTR()	{ /*assert(m_bstr != NULL);*/ return m_bstr; }
	};
#endif

//#define TXTDEBUG

#ifdef TXTDEBUG
FILE *f = NULL;
#endif
	struct Trace
	{
		const char *m_func;
		int level;
		Trace(unsigned long n, const char *func)
		{
			m_func = func;
			level = n;
#ifdef TXTDEBUG
{
	for (int l=0;l<level;++l)
	{
fprintf(f," ");
fflush(f);
	}
}
fprintf(f,"{ %s %d\n", m_func, level);
fflush(f);
#endif
		}
		~Trace()
		{
#ifdef TXTDEBUG
{
	for (int l=0;l<level;++l)
	{
fprintf(f," %d", level);
fflush(f);
	}
}
fprintf(f,"} %s\n", m_func);
fflush(f);
#endif
		}
	};

	CSourceDebug::CSourceDebug()
	{
#ifdef TXTDEBUG
        f = fopen("LOGWATCH", "w");
fprintf(f,"Start logging\n");
fflush(f);
#endif
		m_CompilandFileID = 0;
		m_CompilandUnitID = 0;
	#ifndef _RING0_		
		m_DllGCO = 0;
		m_hLibrary = 0;
	#endif
		m_ModuleName = nullptr;
		m_PDBFullFileName = nullptr;

		m_StringPublicSymbol = nullptr;
		m_HasStringPublicSymbol = false;

	#ifndef _RING0_
		m_Segments = nullptr;
		m_EnumFragmentData = nullptr;

		m_GlobalSymbol = nullptr;
		m_Session = nullptr;
		m_DataSource = nullptr;
	#endif
		m_CreateProgress = nullptr;
		m_UpdateProgress = nullptr;
		m_UpdateCaption = nullptr;
		m_DestroyProgress = nullptr;

		m_DialogAppendSrcFilename = nullptr;

		m_FuncID = 1;

		m_ElemID = 1;

		m_DataID = 1;
		m_Locals.push_back(0);
		
		m_nSrcFilesCount = 0; //TODO remove
	}

	CSourceDebug::~CSourceDebug()
	{
#ifdef TXTDEBUG
fclose(f);
#endif		//Release() from outside
		//???
	}

	unsigned long CSourceDebug::PackString(const char *Str)
	{
		if (!Str || !*Str) return 0;
		int StrLen = TStrLen(Str);

		if ((StrLen + m_StringsCur + 1) >= m_StringsTail)
		{
			stStringChunk Chunk;
			Chunk.StringsBuf = m_StringsBuf;
			Chunk.StringsHead = m_StringsHead;
			Chunk.StringsCur = m_StringsCur;
			m_StringsList.Append(&Chunk);
			m_StringsBuf = new char[(STRINGS_CHUNK_SIZE-(m_StringsCur % STRINGS_CHUNK_SIZE))+STRINGS_CHUNK_SIZE];
			m_StringsTail += STRINGS_CHUNK_SIZE;
			m_StringsHead = m_StringsCur;
		}
		map<const char *, unsigned long, MY_char_compare>::IT it = m_StringIDMap.find(Str);
		if (it != m_StringIDMap.end())
			return it->second;

		char *Dest = (m_StringsBuf + m_StringsCur) - m_StringsHead;
		TStrCpy(Dest, Str);
		m_StringIDMap.insert(_Tmap_pair<const char*,unsigned long>(Dest, m_StringsID));
		m_StringIndexes.push_back(m_StringsCur);
		m_StringsCur += StrLen+1;
		++m_StringsID;
		return m_StringsID-1;
	}

	unsigned long CSourceDebug::PackString(const DATATYPE *data)
	{
		return PackString(data->name);
	}

	unsigned long CSourceDebug::PackString(const DIABASICTYPE *Type)
	{
		if (Type->Elem.Name && *Type->Elem.Name)
			return PackString(Type->Elem.Name);
		return 0;
	}

	void CSourceDebug::Release()
	{

	{
		for (map<unsigned long, DIABASICTYPE*>::IT it = m_Globals.begin();
			it != m_Globals.end(); ++it)
		{
			//TODO second can be NULL if was used operator[]
			DIABASICTYPE *data = it->second;
			if (data->Elem.Type == ELEM_ENUM)
			{
				DIAENUMTYPE *et = (DIAENUMTYPE*)it->second;
				for (unsigned long n = 0; n < et->v.Length; ++n)
				{	
					if (et->v.Values[n].Name) delete []et->v.Values[n].Name;
				}
			} else
			if (data->Elem.Type == ELEM_STRUCT ||
			    data->Elem.Type == ELEM_CLASS ||
			    data->Elem.Type == ELEM_UNION)
			{
				DIAUDTYPE *udt = (DIAUDTYPE*)it->second;
				if (udt->v.BaseClass) delete []udt->v.BaseClass;
				if (udt->v.Data) delete []udt->v.Data;
				if (udt->v.Function) delete []udt->v.Function;
				if (udt->v.Friend) delete []udt->v.Friend;
				if (udt->v.PubSymbols) delete []udt->v.PubSymbols;
			}

			if (data->Elem.Name) delete []data->Elem.Name;
			delete data;
		}
		m_Globals.clear();
	}
	//
	{
		for (map<FUNCTIONKEY, unsigned long>::IT it = m_FunctionsIDMap.begin();
			it != m_FunctionsIDMap.end(); ++it)
		{
			FUNCTION *func = it->first.func;
			if (func->blocks) delete []func->blocks;
			if (func->stack) delete []func->stack;
			delete []func->name;
			delete func;
		}
		m_FunctionsIDMap.clear();
	}
		m_FunctionsRVA.Clear();
		m_FuncID = 1;
	//
	{
		for (set<PUBSYM>::IT it = m_PublicSymbols.begin();
			it != m_PublicSymbols.end(); ++it)
		{
			if (it->name)
				delete []it->name;
		//will delete by m_ImportModules
		//	if (it->moduleName)
		//		delete []it->moduleName;
		}
		m_PublicSymbols.clear();
	}

		m_CompilandFilesLN.clear();
	{
		for (map<char*, COMPUNIT*, MY_char_compare>::IT it = m_CompilandUnitMap.begin();
			it != m_CompilandUnitMap.end(); ++it)
		{
			delete []it->first;
		//	delete []it->second->UnitName; //???
			delete it->second;
		}
	}

	{
		for (map<char*, SrcFileNameID, MY_char_compare>::IT it = m_CompilandSrcMap.begin();
			it != m_CompilandSrcMap.end(); ++it)
		{
			if (it->first)
				delete []it->first;
		}
	}
		if (m_ModuleName) delete []m_ModuleName;
		if (m_PDBFullFileName) delete []m_PDBFullFileName;
	{
		for (TListIter<char*> It = m_ImportModules.Begin();
			It != m_ImportModules.End(); ++It)
		{
			delete []*It;
		}
		m_ImportModules.Clear();
	}

		m_CompilandSrcMap.clear();
		m_CompilandUnitMap.clear();

		//m_Globals.clear(); //???

		m_CompilandFileID = 0;
		m_CompilandUnitID = 0;

		m_CompilandFilesLN.clear(); //dup ???

		m_ElemID = 1;

		m_StaticDatas.clear();
		m_Locals.clear();
		m_Registered.Clear();
		m_DataID = 1;
		m_Locals.push_back(0);

		m_BaseDataTypeIDMap.clear();
		m_StringIDMap.clear();
		m_UDTStrIDMap.clear();

		m_ClassPublicSymbols.clear();
	}
#ifndef _RING0_
	bool CSourceDebug::LoadDIA()
	{
		if (!m_hLibrary)
		{
			m_hLibrary = LoadLibraryA("msdia80.dll");
			if (!m_hLibrary)
		#ifdef _X64_
				 m_hLibrary = LoadLibraryA("..\\..\\Addition\\DiaSDK\\bin\\amd64\\msdia80.dll");
		#else
				 m_hLibrary = LoadLibraryA("..\\..\\Addition\\DiaSDK\\bin\\msdia80.dll");
		#endif
			if (!m_hLibrary)
				return false;
		}
		if (!m_DllGCO)
			m_DllGCO = (DLLGCO)GetProcAddress(m_hLibrary, "DllGetClassObject");
		return true;
	}

	bool CSourceDebug::UnloadDIA()
	{
		if (m_hLibrary)
			FreeLibrary(m_hLibrary);
		m_hLibrary = 0;
		m_DllGCO = 0;
		return true;
	}

	HRESULT CSourceDebug::OpenDIA(void *, REFIID /*const IID *const*/ DiaSource, REFIID/*const IID *const*/ DataSource, void **DiaDataSource)
	{
		HRESULT Result = -1;
		if (!m_hLibrary)
			LoadDIA();
		if (m_DllGCO)
		{
			IClassFactory *ClassFactory;
			Result = m_DllGCO(DiaSource, __uuidof(IClassFactory), &ClassFactory);
			if (FAILED(Result)) return Result;
			ClassFactory->CreateInstance(0, DataSource, DiaDataSource);
			ClassFactory->Release();
		}
		return Result;
	}

	bool CSourceDebug::OpenDIA(const char *FileName, IDiaDataSource **DataSource, IDiaSession **Session, IDiaSymbol **Symbol, bool *LoadFromPDBOK)
	{
		if (FAILED(OpenDIA(0, __uuidof(DiaSource), __uuidof(IDiaDataSource), (void **)DataSource)))
		{
			if (FAILED(OpenDIA(0, __uuidof(DiaSource), __uuidof(IDiaDataSource), (void **)DataSource)))
				return false;
		}

		WCHAR FileNameW[1042];
		AnsiToUnicode(FileName, FileNameW, lenof(FileNameW)); //1024*2); //???

		if (LoadFromPDBOK) *LoadFromPDBOK = true;
		if (FAILED((*DataSource)->loadDataFromPdb(FileNameW)))
		{
			if (LoadFromPDBOK) *LoadFromPDBOK = false;
			if (FAILED((*DataSource)->loadDataForExe(FileNameW, 0, 0)))
				return false;
		}
		if (FAILED((*DataSource)->openSession(Session)))
			return false;
		if (FAILED((*Session)->get_globalScope(Symbol)))
			return false;
		return true;
	}

	void CSourceDebug::CloseDIA()
	{
		SafeRelease(m_Segments);
		SafeRelease(m_EnumFragmentData);
		SafeRelease(m_GlobalSymbol);
		SafeRelease(m_Session);
		SafeRelease(m_DataSource);
		UnloadDIA();
	}

	bool CSourceDebug::GetInfo(IDiaSymbol *Symbol)
	{
	#if 1
		CDiaBSTR ModuleName;
		Symbol->get_name(&ModuleName);
		m_ModuleName = new char[TStrLen(ModuleName)+1];
		TStrCpy(m_ModuleName, ModuleName); ModuleName.release();
	#else
		WCHAR *ModuleName = nullptr;
		Symbol->get_name(&ModuleName);
		m_ModuleName = new char[TStrLen(ModuleName)+1];
		TStrCpy(m_ModuleName, ModuleName);
		::LocalFree(ModuleName);
	#endif
		DWORD TimeStamp = 0;
		Symbol->get_timeStamp(&TimeStamp);

		//CDiaBSTR SymbolsFileName;
		WCHAR *SymbolsFileName = nullptr;
		Symbol->get_symbolsFileName(&SymbolsFileName);
		m_PDBFullFileName = new char[TStrLen(SymbolsFileName)+1];
		TStrCpy(m_PDBFullFileName, SymbolsFileName);
		::LocalFree(SymbolsFileName);

		return false;
	}

#define PROGRESS_SCALE 10
#define TOTAL_PER_STAGE

	bool CSourceDebug::FindHelper()
	{
		IDiaTable *Table = nullptr;
		IDiaEnumTables *EnumTables = nullptr;
		IDiaEnumFrameData *EnumFragmentData = nullptr;
		IDiaEnumSegments *Segments = nullptr;

		m_Segments = nullptr;
		m_EnumFragmentData = nullptr;
		ULONG Count = 0;
		LONG Total = 0;

		if (FAILED(m_Session->getEnumTables(&EnumTables)) || !EnumTables)
			return false;

		Total = 0;
		EnumTables->get_Count(&Total);

	#ifdef TOTAL_PER_STAGE
		if (m_CreateProgress)
		{
			m_CreateProgress(Total);
			m_nTotal = Total / PROGRESS_SCALE;
		}

		m_nProgress = 0;
	#endif
		if (m_UpdateCaption)
			m_UpdateCaption(WSTR("Loading PDB Segment"));

		while (SUCCEEDED(EnumTables->Next(1, &Table, &Count)) && Count == 1)
		{
			if (SUCCEEDED(Table->QueryInterface(__uuidof(IDiaSegment), (void**)&Segments)) && Segments)
			{
				m_Segments = Segments;
				Table->Release();
				Table = nullptr;
				m_nProgress += Total - Count;
				if (m_UpdateProgress && m_nTotal && !(m_nProgress % m_nTotal))
					m_UpdateProgress(m_nProgress);
				break;
			}
			Table->Release();
			Table = nullptr;
			++m_nProgress;
			if (m_UpdateProgress && m_nTotal && !(m_nProgress % m_nTotal))
				m_UpdateProgress(m_nProgress);
		}
		EnumTables->Release();

	#ifdef TOTAL_PER_STAGE
		if (m_DestroyProgress)
			m_DestroyProgress();
	#endif

		if (FAILED(m_Session->getEnumTables(&EnumTables)) || !EnumTables)
			return false;

		Total = 0;
		EnumTables->get_Count(&Total);

	#ifdef TOTAL_PER_STAGE
		if (m_CreateProgress)
		{
			m_CreateProgress(Total);
			m_nTotal = Total / PROGRESS_SCALE;
		}

		m_nProgress = 0;
	#endif
		if (m_UpdateCaption)
			m_UpdateCaption(WSTR("Loading PDB FrameData"));

		while (SUCCEEDED(EnumTables->Next(1, &Table, &Count)) && Count == 1)
		{
			if (SUCCEEDED(Table->QueryInterface(__uuidof(IDiaEnumFrameData), (void**)&EnumFragmentData)) && EnumFragmentData)
			{
				m_EnumFragmentData = EnumFragmentData;
				Table->Release();
				Table = nullptr;
				m_nProgress += Total - Count;
				if (m_UpdateProgress && m_nTotal && !(m_nProgress % m_nTotal))
					m_UpdateProgress(m_nProgress);
				break;
			}
			Table->Release();
			Table = nullptr;
			++m_nProgress;
			if (m_UpdateProgress && m_nTotal && !(m_nProgress % m_nTotal))
				m_UpdateProgress(m_nProgress);
		}
		EnumTables->Release();

	#ifdef TOTAL_PER_STAGE
		if (m_DestroyProgress)
			m_DestroyProgress();
	#endif
 		return true;
	}
#if 0
 //TODO REMOVE
	void CSourceDebug::MapNameID()
	{
		//m_GlobalsID.push_back(0); //??? is first skiped?
		for (map<unsigned long, DIABASICTYPE*>::IT it = m_Globals.begin();
			it != m_Globals.end(); ++it)
		{
			//if (!TStrCmp(it->second->Elem.Name, Name))
			//	return it->second;
			m_GlobalsName.insert(_Tmap_pair<const char*,unsigned long>(it->second->Elem.Name, it->second->Elem.ID));
			m_GlobalsID.push_back(it->second);
		}
	}
#endif
	unsigned long CSourceDebug::GetTotal()
	{
		long Total = 0;
	#ifndef TOTAL_PER_STAGE
		IDiaEnumSymbols *EnumSymbols = nullptr;
		IDiaEnumTables *EnumTables = nullptr;

		//SEGMENT
		if (SUCCEEDED(m_Session->getEnumTables(&EnumTables)) && EnumTables)
		{
			long Count = 0;
			EnumTables->get_Count(&Count);
			Total += Count;
		}

		//FRAGMENTDATA
		if (SUCCEEDED(m_Session->getEnumTables(&EnumTables)) && EnumTables)
		{
			long Count = 0;
			EnumTables->get_Count(&Count);
			Total += Count;
		}

		//PUBLIC
		if (SUCCEEDED(m_GlobalSymbol->findChildren(SymTagPublicSymbol, 0, nsNone, &EnumSymbols))
			&& EnumSymbols)
		{
			long Count = 0;
			EnumSymbols->get_Count(&Count);
			Total += Count;
		}

		//COMPILAND
		if (SUCCEEDED(m_GlobalSymbol->findChildren(SymTagCompiland, 0, nsNone, &EnumSymbols))
			&& EnumSymbols)
		{
			long Count = 0;
			EnumSymbols->get_Count(&Count);
			Total += Count;
		}

		//BASE
		if (SUCCEEDED(m_GlobalSymbol->findChildren(SymTagBaseType, 0, nsNone, &EnumSymbols))
			&& EnumSymbols)
		{
			long Count = 0;
			EnumSymbols->get_Count(&Count);
			Total += Count;
		}

		//NULL
		if (SUCCEEDED(m_GlobalSymbol->findChildren(SymTagNull, 0, nsNone, &EnumSymbols))
			&& EnumSymbols)
		{
			long Count = 0;
			EnumSymbols->get_Count(&Count);
			Total += Count;
		}
	#endif
		return Total;
	}

	bool CSourceDebug::Parse(bool LoadFromPDBOK)
	{
		if (!m_GlobalSymbol)
			return false;

		IDiaEnumSymbols *EnumSymbols = nullptr;
		IDiaSymbol *Symbol = nullptr;
		ULONG Count = 0;
		ULONG level = 0;

		m_nProgress = 0;
		LONG Total = GetTotal();
	#ifndef TOTAL_PER_STAGE
		if (m_CreateProgress)
		{
			m_CreateProgress(Total);
			m_nTotal = Total / PROGRESS_SCALE;
		}

		m_nProgress = 0;
		if (m_UpdateCaption)
			m_UpdateCaption(WSTR("Loading PDB"));
	#endif

		GetInfo(m_GlobalSymbol);

		FindHelper();

		//PUBLIC
		if (FAILED(m_GlobalSymbol->findChildren(SymTagPublicSymbol, 0, nsNone, &EnumSymbols))
			|| !EnumSymbols)
		{
	#ifndef TOTAL_PER_STAGE
			if (m_DestroyProgress)
				m_DestroyProgress();
	#endif
			return false;
		}
	#ifdef TOTAL_PER_STAGE
		if (m_CreateProgress)
		{
			Total = 0;
			EnumSymbols->get_Count(&Total);
			m_CreateProgress(Total);
			m_nTotal = Total / PROGRESS_SCALE;
		}

		m_nProgress = 0;
	#endif
		if (m_UpdateCaption)
			m_UpdateCaption(WSTR("Loading PDB PublicSymbol"));

		while (SUCCEEDED(EnumSymbols->Next(1, &Symbol, &Count)) && Count == 1 && Symbol)
		{
			bool release = true;
			GetPublicSymbol(Symbol, &release, nullptr);
			if (release)
			{
				Symbol->Release();
				Symbol = nullptr;
			}
			++m_nProgress;
			if (m_UpdateProgress && m_nTotal && !(m_nProgress % m_nTotal))
				m_UpdateProgress(m_nProgress);
		}
		EnumSymbols->Release();
		EnumSymbols = nullptr;
	#ifdef TOTAL_PER_STAGE
		if (m_DestroyProgress)
			m_DestroyProgress();
	#endif
		//COMPILAND
		if (FAILED(m_GlobalSymbol->findChildren(SymTagCompiland, 0, nsNone, &EnumSymbols))
			|| !EnumSymbols)
		{
	#ifndef TOTAL_PER_STAGE
			if (m_DestroyProgress)
				m_DestroyProgress();
	#endif
			return false;
		}
	#ifdef TOTAL_PER_STAGE
		if (m_CreateProgress)
		{
			Total = 0;
			EnumSymbols->get_Count(&Total);
			m_CreateProgress(Total);
			m_nTotal = Total / PROGRESS_SCALE;
		}

		m_nProgress = 0;
	#endif
		if (m_UpdateCaption)
			m_UpdateCaption(WSTR("Loading PDB TagCompiland"));

		while (SUCCEEDED(EnumSymbols->Next(1, &Symbol, &Count)) && Count == 1 && Symbol)
		{
			GetCompiland(Symbol);
			Symbol->Release();
			Symbol = nullptr;
			++m_nProgress;
			if (m_UpdateProgress && m_nTotal && !(m_nProgress % m_nTotal))
				m_UpdateProgress(m_nProgress);
		}
		EnumSymbols->Release();
		EnumSymbols = nullptr;
	#ifdef TOTAL_PER_STAGE
		if (m_DestroyProgress)
			m_DestroyProgress();
	#endif
		//INIT BASIC
		InitBasicType();

		//BASE
		if (FAILED(m_GlobalSymbol->findChildren(SymTagBaseType, 0, nsNone, &EnumSymbols)) || !EnumSymbols)
		{
	#ifndef TOTAL_PER_STAGE
			if (m_DestroyProgress)
				m_DestroyProgress();
	#endif
			return true; //???
		}

		//LONG TotalBaseType = 0;
		//EnumSymbols->get_Count(&TotalBaseType);
	#ifdef TOTAL_PER_STAGE
		if (m_CreateProgress)
		{
			Total = 0;
			EnumSymbols->get_Count(&Total);
			m_CreateProgress(Total);
			m_nTotal = Total / PROGRESS_SCALE;
		}

		m_nProgress = 0;
	#endif
		if (m_UpdateCaption)
			m_UpdateCaption(WSTR("Loading PDB TagBase"));
		level = 0;
		while (SUCCEEDED(EnumSymbols->Next(1, &Symbol, &Count)) && Count == 1 && Symbol)
		{
			bool release = true;
			DIABASICTYPE *Type;
			ParseDIASymbol(Symbol, &release, (NULLTYPE**)&Type, level);
			if (release)
			{
				Symbol->Release();
				Symbol = nullptr;
			}
			++level;
			++m_nProgress;
			if (m_UpdateProgress && m_nTotal && !(m_nProgress % m_nTotal))
				m_UpdateProgress(m_nProgress);
		}
		EnumSymbols->Release();
	#ifdef TOTAL_PER_STAGE
		if (m_DestroyProgress)
			m_DestroyProgress();
	#endif
		//NULL
		if (FAILED(m_GlobalSymbol->findChildren(SymTagNull, 0, nsNone, &EnumSymbols)) || !EnumSymbols)
		{
	#ifndef TOTAL_PER_STAGE
			if (m_DestroyProgress)
				m_DestroyProgress();
	#endif
			return true; //???
		}
	#ifdef TOTAL_PER_STAGE
		if (m_CreateProgress)
		{
			Total = 0;
			EnumSymbols->get_Count(&Total);
			m_CreateProgress(Total);
			m_nTotal = Total / PROGRESS_SCALE;
		}

		m_nProgress = 0;
	#endif
		if (m_UpdateCaption)
			m_UpdateCaption(WSTR("Loading PDB"));
		level = 0;
		while (SUCCEEDED(EnumSymbols->Next(1, &Symbol, &Count)) && Count == 1 && Symbol)
		{
			bool release = true;
			DIABASICTYPE *Type;
			ParseDIASymbol(Symbol, &release, (NULLTYPE**)&Type, level);
			if (release)
			{
				Symbol->Release();
				Symbol = nullptr;
			}
			++level;
			++m_nProgress;
			if (m_UpdateProgress && m_nTotal && !(m_nProgress % m_nTotal))
				m_UpdateProgress(m_nProgress);
		}
		EnumSymbols->Release();

		if (m_DestroyProgress)
			m_DestroyProgress();

		ParserRelease();
		//MapNameID(); //++
		return true;
	}

	void CSourceDebug::ParserRelease()
	{
		//m_GlobalTypes_IDType_ID
		for (map<unsigned long, DIABASICTYPE*>::IT it = m_Globals.begin();
			it != m_Globals.end(); ++it)
		{
			DIABASICTYPE *data = it->second;
			if (data->Symbol)
			{
				data->Symbol->Release();
				data->Symbol = nullptr;
			}
		}
	}

	bool CSourceDebug::StripClassName(char *String)
	{
		bool ret = false;

		char *strStruct = TStrStr(String, "struct ");
		char *strClass = TStrStr(String, "class ");
		if (!strStruct && !strClass)
			return true;

		char *pNewString = new char[TStrLen(String)+1];
		*pNewString = 0;

		char *read = String;
		char *write = pNewString;
		
		while (strClass)
		{
			TStrNCpy(write, read, strClass - read);
			write += strClass - read;
			read = strClass + (sizeof("class ")-1);
			strClass = TStrStr(read, "class ");
			ret = true;
		}

		if (ret)
		{
			TStrCpy(write, read);
			TStrCpy(String, pNewString);
			read = String;
			write = pNewString;
			ret = false;
		}

		while ((strStruct = TStrStr(read, "struct ")))
		{
			TStrNCpy(write, read, strStruct - read);
			write += strStruct - read;
			read = strStruct + (sizeof("struct ")-1);
			ret = true;
		}
		
		if (ret)
		{
			TStrCpy(write, read);
			TStrCpy(String, pNewString);
			ret = false;
		}
		delete []pNewString;
		return ret;
	}

	bool CSourceDebug::GetPublicSymbol(IDiaSymbol *Symbol, bool *bRelease, void *Type)
	{
		PUBSYM symbol;
		memset(&symbol, 0, sizeof(symbol));

		//CDiaBSTR Name;
		WCHAR *Name = nullptr;
		if (FAILED(Symbol->get_undecoratedName(&Name)) || !Name)
			return false;

		symbol.name = new char[TStrLen(Name)+1];
		TStrCpy(symbol.name, Name);
		::LocalFree(Name);

		if (TStrStr(symbol.name, "::`vftable'"))
		{
			symbol.options |= 0x100'0000; //<<24
			StripClassName(symbol.name);
		} else
		if (!TStrCmp(symbol.name, "`string'"))
		{
			symbol.options |= 0x1'0000; //<<16
		}

		BOOL Value = 0;

		Value = 0;
		if (SUCCEEDED(Symbol->get_function(&Value)))
			symbol.options = ((Value != 0) << 22) | (symbol.options & ~0x40'0000);

		Value = 0;
		if (SUCCEEDED(Symbol->get_managed(&Value)))
			symbol.options = ((Value != 0) << 20) | (symbol.options & ~0x10'0000);

		Value = 0;
		if (SUCCEEDED(Symbol->get_code(&Value)))
			symbol.options = ((Value != 0) << 23) | (symbol.options & ~0x80'0000);

		Value = 0;
		if (SUCCEEDED(Symbol->get_msil(&Value)))
			symbol.options = ((Value != 0) << 21) | (symbol.options & ~0x20'0000);

		unsigned long rva;
		if (SUCCEEDED(Symbol->get_relativeVirtualAddress(&rva)))
			symbol.rva = rva;

		ULONGLONG Length;
		if (SUCCEEDED(Symbol->get_length(&Length)))
			symbol.length = Length;

		m_PublicSymbols.insert(symbol);
		if (symbol.options & 0x100'0000) //<<24
		{	//+++???
		//	char *name = new char[TStrLen(symbol.name)+1];
		//	TStrCpy(name, symbol.name);
		//	symbol.name = name;
			m_ClassPublicSymbols.insert(symbol);
		}
		return true;
	}

	bool CSourceDebug::GetCompiland(IDiaSymbol *Symbol)
	{
		if (!Symbol)
			return false;

		//CDiaBSTR UnitName;
		WCHAR *UnitName = nullptr;
		if (FAILED(Symbol->get_name(&UnitName)) || !UnitName)
			return false;

		IDiaEnumSourceFiles *EnumSource = nullptr;
		if (FAILED(m_Session->findFile(Symbol, 0, 0, &EnumSource)) && !EnumSource) //??? ||
		{
			::LocalFree(UnitName);
			return false;
		}

		LONG nFiles = 0;
		EnumSource->get_Count(&nFiles);

		unsigned long Size = sizeof(COMPUNIT) + (nFiles ? sizeof(COMPUNIT::FILES)*(nFiles-1) : 0);
		COMPUNIT *CompUnit = (COMPUNIT *)new char[Size];
		memset(CompUnit, 0, Size);
		CompUnit->nFiles = nFiles;
		CompUnit->UnitName = new char[TStrLen(UnitName)+1];
		TStrCpy(CompUnit->UnitName, UnitName);
		::LocalFree(UnitName);
		COMPUNIT::FILES *Files = &CompUnit->Files;

		//CUNKEY Key(CompUnit->UnitName, m_CompilandUnitID);
		map<char*,COMPUNIT*,MY_char_compare>::IT it = m_CompilandUnitMap.find(CompUnit->UnitName);
		if (it != m_CompilandUnitMap.end())
		{
			EnumSource->Release();
			delete []CompUnit->UnitName;
			delete CompUnit;
			return false;
		}

		CompUnit->UnitID = m_CompilandUnitID;
		m_CompilandUnitMap.insert(_Tmap_pair<char*,COMPUNIT*>(CompUnit->UnitName, CompUnit));
		++m_CompilandUnitID;

		if (nFiles == 0)
		{
			EnumSource->Release();
			EnumSource = nullptr;
			return false;
		}

		int Index = 0;

		unsigned long max_rva = -1;
		unsigned long min_rva = 0;

		IDiaSourceFile *SourceFile = nullptr;
		ULONG SourceFileCount = 0;
		while (SUCCEEDED(EnumSource->Next(1, &SourceFile, &SourceFileCount)) && SourceFileCount == 1)
		{
			unsigned long FileID = -1;

			//CDiaBSTR SrcFileName;
			WCHAR *SrcFileName = nullptr;
			if (FAILED(SourceFile->get_fileName(&SrcFileName)) || !SrcFileName) continue;
			
			char *szSrcFileName = new char[TStrLen(SrcFileName)+1];
			TStrCpy(szSrcFileName, SrcFileName);
			::LocalFree(SrcFileName);

			int NewFile;

			//Key(szSrcFileName, m_CompilandFileID);
			map<char*,SrcFileNameID,MY_char_compare>::IT it = m_CompilandSrcMap.find(szSrcFileName);//Key);
			if (it == m_CompilandSrcMap.end())
			{
				FileID = m_CompilandFileID;
				NewFile = 1;
			} else
			{
				FileID = it->second.FileID;
				delete []szSrcFileName;
				NewFile = 0;
			}

			IDiaEnumLineNumbers *EnumLineNumbers = nullptr;
			if (SUCCEEDED(m_Session->findLines(Symbol, SourceFile, &EnumLineNumbers)))
			{
				LONG nLines = 0;
				if (SUCCEEDED(EnumLineNumbers->get_Count(&nLines)) && nLines)
				{
					IDiaLineNumber *LineNumber = nullptr;
					ULONG LineNumberCount = 0;
					while (SUCCEEDED(EnumLineNumbers->Next(1, &LineNumber, &LineNumberCount)) && LineNumberCount == 1)
					{
						DWORD address = 0;
						if (FAILED(LineNumber->get_relativeVirtualAddress(&address))) continue;
						
						int statement = 0;
						if (FAILED(LineNumber->get_statement(&statement))) continue;
						
						MAX_LIMIT(max_rva, address);
						MIN_LIMIT(min_rva, address);

						DWORD lineNumber = 0;
						if (FAILED(LineNumber->get_lineNumber(&lineNumber))) continue;

						DWORD length = 0;
						if (FAILED(LineNumber->get_length(&length))) continue;
						
						FilesLN FLN;
						FLN.UnitID = CompUnit->UnitID;
						FLN.length = length;
						FLN.address = address;
						FLN.statement = statement != 0;
						FLN.lineNumber = lineNumber;	
						FLN.FileID = FileID;
						//m_CompilandFilesLN.Append(&FLN);
						//m_CompilandFilesLN.AppendSort(FLN);
						m_CompilandFilesLN.push_back(FLN);
						LineNumber->Release();
						LineNumber = nullptr;
					}
					
				}
				EnumLineNumbers->Release();
				EnumLineNumbers = nullptr;
				Files[Index].nLines = nLines;
			}

			Files[Index].FileID = FileID;
			SourceFile->Release();
			SourceFile = nullptr;

			if (Files[Index].nLines)
			{
				if (NewFile)
				{
					SrcFileNameID second;
					second.FileID = m_CompilandFileID;
					second.FileName = szSrcFileName;
					second.Text = 0;
					second.FileSize = 0;

					//+++
				#if 0
					if (gpFileIO->IsFileExist(szSrcFileName))
					{
						second.FileSize = gpFileIO->GetFileSize(szSrcFileName);
			//printf("src %s, size %d, %d\n", szSrcFileName, second.FileSize, GetLastError());
					//TODO
					//	second.Text = new char[second.FileSize];
					//	gpFileIO->ReadFromFile(szSrcFileName, second.Text, second.FileSize);
						++m_nSrcFilesCount; //TODO REMOVE
					}
				#endif
					m_CompilandSrcMap.insert(_Tmap_pair<char*,SrcFileNameID>(szSrcFileName, &second));
					++m_CompilandFileID;
				}
				++Index;
			} else
			if (NewFile == 1)
			{
				delete []szSrcFileName;
			}
		}

		if (CompUnit->nFiles != Index)
			CompUnit->nFiles = Index;
		CompUnit->MinRVA = min_rva;
		CompUnit->MaxRVA = max_rva;
		EnumSource->Release();
		EnumSource = nullptr;
		return true;
	}


	static BASEDATATYPENAMED gBasicDataTypeMap[] =
		{
			{{0x80,1},"char"},
			{{0x100,2},"short"},
			{{0x200,4},"int"},
			{{0x400,8},"__int64"},

			{{0x800,1},"unsigned char"},
			{{0x1000,2},"unsigned short"},
			{{0x2000,4},"unsigned int"},
			{{0x4000,8},"unsigned __int64"},

			{{0xD,4},"long"},
			{{0xE,4},"unsigned long"},
		};

	void CSourceDebug::InitBasicType()
	{
		for (unsigned long n = 0; n < lenof(gBasicDataTypeMap); ++n)
		{
			BASEDATATYPE basedata = gBasicDataTypeMap[n].type;

			DIABASICTYPE *Type = new DIABASICTYPE;
			memset(Type, 0, sizeof(*Type));
			Type->Elem.Name = new char[TStrLen(gBasicDataTypeMap[n].name)+1];
			TStrCpy(Type->Elem.Name, gBasicDataTypeMap[n].name);
			Type->Elem.Size = sizeof(*Type);
			Type->Value = 0;
			Type->Elem.Type = ELEM_BASE_TYPE;
			Type->Elem.Length = basedata.length;
			Type->Elem.ID = m_ElemID++;

			m_BaseDataTypeIDMap.insert(std::pair<BASEDATATYPE,unsigned long>(basedata, Type->Elem.ID));

			m_Globals.Insert(Type->Elem.ID, (DIABASICTYPE*)Type);
		}
	}

	bool CSourceDebug::SetDataLocation(DATATYPE *data, IDiaSymbol *Symbol)
	{
		switch (data->locationType)
		{
		case LocIsStatic:{
			Symbol->get_relativeVirtualAddress(&data->Static.rva);
			} break;
		case LocIsTLS:{
			Symbol->get_addressSection(&data->TLS.section);
			Symbol->get_addressOffset(&data->TLS.offset);
			} break;
		case LocIsRegRel:{
			Symbol->get_registerId(&data->RegRel.registerId);
			Symbol->get_offset(&data->RegRel.offset);
			} break;
		case LocIsThisRel:
		case LocIsIlRel:{
			Symbol->get_offset(&data->ThisRel.offset);
			} break;
		case LocIsEnregistered:{
			Symbol->get_registerId(&data->Enregistered.registerId);
			--data->Enregistered.registerId;
			} break;
		case LocIsBitField:{
			Symbol->get_bitPosition(&data->BitField.bitPosition);
			unsigned long long length = 0;
			Symbol->get_length(&length);
			data->BitField.length = length;
			Symbol->get_offset(&data->BitField.offset);
			} break;
		case LocIsSlot:{
			Symbol->get_slot(&data->Slot.slot);
			} break;
		case LocInMetaData:{
			Symbol->get_token(&data->MetaData.token);
			} break;
		case LocIsConstant:{
			unsigned long long Value = 0;
			VARIANT Variant;
			memset(&Variant, 0, sizeof(Variant));
			Symbol->get_value(&Variant);
			switch (Variant.vt)
			{
			case VT_ERROR:
				Value = 0;
				break;
			case VT_I1:
				Value = Variant.cVal;
				break;
			case VT_UI1:
				Value = Variant.bVal;
				break;
			case VT_I2:
				Value = Variant.iVal;
				break;
			case VT_UI2:
				Value = Variant.uiVal;
				break;
			case VT_I4:
				Value = Variant.lVal;
				break;
			case VT_UI4:
				Value = Variant.ulVal;
				break;
			case VT_I8:
				Value = Variant.llVal;
				break;
			case VT_UI8:
				Value = Variant.ullVal;
				break;
			case VT_INT:
				Value = Variant.intVal;
				break;
			case VT_UINT:
				Value = Variant.uintVal;
				break;
			default:
				break;
			}
			data->Constant.value = Value;
			} break;
		}
		return true;
	}

	unsigned long CSourceDebug::ParseData(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		unsigned long ID = 0;
		if (Type) *Type = 0;
		DWORD dataKind = 0;
		if (FAILED(Symbol->get_dataKind(&dataKind)))
			return 0;

		//CDiaBSTR Name;
		WCHAR *Name = nullptr;
		if (FAILED(Symbol->get_name(&Name)))
			return 0;
		if (dataKind == DataIsStaticLocal && !TStrCmp(Name, ""))
		{
			::LocalFree(Name);
			return 0;
		}
		DWORD locationType = 0;
		if (FAILED(Symbol->get_locationType(&locationType)))
		{
			::LocalFree(Name); //+++
			return 0;
		}

		REGISTERED_ Registered;
		bool RegisterType = false;
		if (locationType == LocIsEnregistered && !TStrCmp(Name, "this"))
		{
			DWORD registerId = 0;
			if (FAILED(Symbol->get_registerId(&registerId)))
			{
				::LocalFree(Name); //+++
				return 0;
			}
			--registerId;

			Registered.dataKind = dataKind;
			Registered.registerId = registerId;
			TListIter<REGISTERED_> It = m_Registered.Find(Registered);
			if (It != m_Registered.End())
			{
				::LocalFree(Name); //+++
				if (Type) *Type = It->data;
				return It->ID;
			}
			RegisterType = true;
		}
		BOOL constType = 0;
		Symbol->get_constType(&constType);
		BOOL volatileType = 0;
		Symbol->get_volatileType(&volatileType);

		unsigned long Size = sizeof(DATATYPE)+TStrLen(Name)+1;
		DATATYPE *data = (DATATYPE *)new char[Size];
		memset(data, 0, Size);
		data->ID = m_DataID++;
		data->name = (char *)&data[1]; //name store at end of struct
		data->dataKind = dataKind;
		TStrCpy(data->name, Name);
		::LocalFree(Name);

		if (locationType == LocIsStatic)
		{
			unsigned long rva = 0;
			Symbol->get_relativeVirtualAddress(&rva); //???

			PUBSYM symbol;
			symbol.rva = rva;
			{
			set<PUBSYM>::IT it = m_PublicSymbols.find(symbol);
			if (it != m_PublicSymbols.end())
			{
				//PUBSYM *p = &*it;
				//delete []p->name;
//printf("ERASEDdata %s\n", it->name);
				delete []it->name; //???
				m_PublicSymbols.erase(it);
				//set<PUBSYM>::IT it2 = m_ClassPublicSymbols.find(symbol);
				//m_ClassPublicSymbols.erase(it2);
			}
			}
			DATAKEY key;
			key.name = data->name;
			key.rva = symbol.rva;
			map<DATAKEY,unsigned long>::IT it = m_StaticDatas.find(key);
			if (it != m_StaticDatas.end())
			{
				--m_DataID;
				delete [](char*)data;
				if (Type) //TODO check range m_Locals
					*Type = m_Locals[it->second];
				return it->second;
			}
			m_StaticDatas.insert(_Tmap_pair<DATAKEY,unsigned long>(key, data->ID));
		}
		DWORD access = 0;
		Symbol->get_access(&access); //???
		data->access = access;
		m_Locals.push_back(data);
		data->constType = constType;
		data->volatileType = volatileType;
		if (RegisterType)
		{
			Registered.ID = data->ID;
			Registered.data = data;
			m_Registered.Append(&Registered);
		}

		IDiaSymbol *SymbolType = nullptr;
		if (SUCCEEDED(Symbol->get_type(&SymbolType)) && SymbolType)
		{
			NULLTYPE *type = nullptr;
			bool release = true;
			unsigned long ID = ParseDIASymbol(SymbolType, &release, &type, level);
			if (type)
				data->TypeLength = ((DIABASICTYPE*)type)->Elem.Length;
			data->TypeID = ID;
			if (release)
				SymbolType->Release();
		} else
		{
			if (dataKind == DataIsConstant)
				data->TypeLength = 4;
			else	data->TypeLength = 0;
			data->TypeID = 0;
		}

		data->locationType = locationType;

		SetDataLocation(data, Symbol);

		IDiaSymbol *ClassSymbol = nullptr;
		if (SUCCEEDED(Symbol->get_classParent(&ClassSymbol)) && ClassSymbol)
		{
			bool release = true;
			data->ClassID = ParseDIASymbol(ClassSymbol, &release, nullptr, level);
			if (release)
				ClassSymbol->Release();
		}
		if (Type) *Type = data;
		return data->ID;
	}

	bool CSourceDebug::SameTag(IDiaSymbol *Symbol, unsigned long Tag)
	{
		if (!Symbol)
			return false;
		DWORD symTag = 0;
		if (FAILED(Symbol->get_symTag(&symTag)))
			return false;
		return symTag == Tag;
	}

	unsigned long CSourceDebug::MakeSDBlock(IDiaSymbol *Symbol, TList<unsigned long> *ListsID, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		if (!Symbol)
			return 0;
		if (!SameTag(Symbol, SymTagBlock))
			return 0;
		IDiaEnumSymbols *EnumSymbol = 0;
		Symbol->findChildren(SymTagData, 0, 0, &EnumSymbol); //???
		long Count = 0;
		EnumSymbol->get_Count(&Count); //???
		unsigned long Size = sizeof(DIABLOCKTYPE);
		if (Count>=1) Size += (Count-1)*4; // Count-1 BUG!!!
		DIABLOCKTYPE *block = (DIABLOCKTYPE *)new char[Size];
		memset(block, 0, Size);
		block->Elem.ID = m_ElemID++;
		block->Elem.Size = Size;
		block->Value = &block->v;

		block->v.count = Count;
		unsigned long rva = -1; //???
		Symbol->get_relativeVirtualAddress(&rva); //???
		block->v.rva = rva;
		ULONGLONG length = 0;
		Symbol->get_length(&length); //???
		block->v.length = length;
		
		int n = 0;

		IDiaSymbol *DataSymbol = nullptr;
		ULONG Celt = 0;
		while (SUCCEEDED(EnumSymbol->Next(1, &DataSymbol, &Celt)) && Celt == 1)
		{
			DATATYPE *data;
			block->v.IDs[n] = ParseData(DataSymbol, nullptr, (NULLTYPE**)&data, level);
			data->ClassID = block->Elem.ID;
			DataSymbol->Release();
			++n;
		}
		EnumSymbol->Release();
		EnumSymbol = nullptr;
		

		ListsID->Append(&block->Elem.ID);
		block->Elem.Type = ELEM_BLOCK;

		//m_Globals.insert(_Tmap_pair<unsigned long, DIABASICTYPE*>(block->Elem.ID, block));
		m_Globals.Insert(block->Elem.ID, (DIABASICTYPE*)block);

		//m_GlobalTypes_IDType_Type insert block
		
		//parse sub blocks
		if (SUCCEEDED(Symbol->findChildren(SymTagBlock, 0, 0, &EnumSymbol)) && EnumSymbol)
		{
			IDiaSymbol *BlockSymbol = nullptr;
			ULONG Celt = 0;
			while (SUCCEEDED(EnumSymbol->Next(1, &BlockSymbol, &Celt)) && Celt == 1)
			{
				MakeSDBlock(BlockSymbol, ListsID, level);
				BlockSymbol->Release();
			}
			EnumSymbol->Release();
			EnumSymbol = nullptr;
		}
		return block->Elem.ID;
	}

	unsigned long CSourceDebug::ParseFunction(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		if (Type) *Type = 0;

		DWORD rva = 0;
		if (FAILED(Symbol->get_relativeVirtualAddress(&rva)) || !rva)
			return 0;

		//CDiaBSTR Name;
		WCHAR *Name = nullptr;
		if (FAILED(Symbol->get_name(&Name)) || !Name)
			return 0;

		ULONGLONG length = 0;
		if (FAILED(Symbol->get_length(&length)))
		{
			::LocalFree(Name);
			return 0;
		}

		char *pName = new char[TStrLen(Name)+1];
		TStrCpy(pName, Name);
		::LocalFree(Name);

		FUNCTIONKEY funckey;
		funckey.name = pName;
		funckey.rva = rva;
		map<FUNCTIONKEY, unsigned long>::IT it = m_FunctionsIDMap.find(funckey);
		if (it != m_FunctionsIDMap.end())
		{
			FUNCTION *func = it->first.func;
			if (Type) *Type = func;
			delete []pName;
			return func->ID;
		} else
		{
			PUBSYM symbol;
			symbol.rva = rva;
			set<PUBSYM>::IT it = m_PublicSymbols.find(symbol);
			if (it != m_PublicSymbols.end())
			{
				//PUBSYM *p = &*it;
				//delete []p->name;
//printf("ERASEDfunc %s\n", it->name);
				delete []it->name;
				m_PublicSymbols.erase(it);
				//set<PUBSYM>::IT it2 = m_ClassPublicSymbols.find(symbol);
				//m_ClassPublicSymbols.erase(it2);
			}
		}
		
		FUNCTION *func = new FUNCTION;
		memset(func, 0, sizeof(FUNCTION));
		func->rva = rva;
		if (rva) m_FunctionsRVA.Append(&rva);
		func->name = pName;
		func->length = length; //??? IS REALY NEED ULONGLONG ???
		func->ID = m_FuncID++;
		funckey.func = func;
		m_FunctionsIDMap.insert(_Tmap_pair<FUNCTIONKEY, unsigned long>(funckey, &func->ID));

		IDiaFrameData *FrameData = nullptr;
		if (m_EnumFragmentData && SUCCEEDED(m_EnumFragmentData->frameByRVA(rva, &FrameData)) && FrameData)
		{
			BOOL Flag = 0;
			if (SUCCEEDED(FrameData->get_systemExceptionHandling(&Flag)))
				func->SystemExceptionHandling = Flag;
			Flag = 0;
			if (SUCCEEDED(FrameData->get_cplusplusExceptionHandling(&Flag)))
				func->CplusplusExceptionHandling = Flag;
			FrameData->Release();
			FrameData = nullptr;
		}

		if (Type) *Type = func;

		DWORD access = 0;
		if (FAILED(Symbol->get_access(&access)))
			return 0;
		func->Access = access;

		DWORD Location = 0;
		if (FAILED(Symbol->get_locationType(&Location)))
			return 0;
		func->LocationType = Location;

		//func->length = Length;                     // ?????????

		func->XX = 0;

		BOOL Virtual = 0;
		if (FAILED(Symbol->get_virtual(&Virtual)))
			return 0;

		if (Virtual)
		{
			DWORD VirtualOffset = -1;
			if (FAILED(Symbol->get_virtualBaseOffset(&VirtualOffset)))
				return 0;
			func->Virtual = 1;
			func->virtualOffset = VirtualOffset;
		}

		BOOL pure = 0;
		Symbol->get_pure(&pure);
		func->Pure = pure;

		BOOL intro = 0;
		Symbol->get_intro(&intro);
		func->Intro = intro;

		IDiaEnumSymbols *EnumSym = nullptr;

		if (SUCCEEDED(Symbol->findChildren(SymTagFuncDebugEnd, 0, 0, &EnumSym)) && EnumSym)
		{
			IDiaSymbol *SymbolDbgEnd = nullptr;
			DWORD DebugEndCelt = 0;
			LONG count = 0;
			if (SUCCEEDED(EnumSym->get_Count(&count))
				&& count == 1
				&& SUCCEEDED(EnumSym->Next(1, &SymbolDbgEnd, &DebugEndCelt))
				&& DebugEndCelt == 1)
			{
				SymbolDbgEnd->get_relativeVirtualAddress(&func->debugEnd);
				SymbolDbgEnd->Release();
				SymbolDbgEnd = nullptr;
			}
			EnumSym->Release();
			EnumSym = nullptr;
		}

		if (SUCCEEDED(Symbol->findChildren(SymTagFuncDebugStart, 0, 0, &EnumSym)) && EnumSym)
		{
			IDiaSymbol *SymbolDbgStart = nullptr;
			DWORD DebugStartCelt = 0;
			LONG count = 0;
			if (SUCCEEDED(EnumSym->get_Count(&count))
				&& count == 1
				&& SUCCEEDED(EnumSym->Next(1, &SymbolDbgStart, &DebugStartCelt))
				&& DebugStartCelt == 1)
			{
				SymbolDbgStart->get_relativeVirtualAddress(&func->debugStart);
				SymbolDbgStart->Release();
				SymbolDbgStart = nullptr;
			}
			EnumSym->Release();
			EnumSym = nullptr;
		}

		if (SUCCEEDED(Symbol->findChildren(SymTagBlock, 0, 0, &EnumSym)) && EnumSym)
		{
			TList<unsigned long> ListsID;
			
			IDiaSymbol *BlockSymbol = nullptr;
			int n = 0;
			ULONG Celt = 0;
			while (SUCCEEDED(EnumSym->Next(1, &BlockSymbol, &Celt)) && Celt == 1)
			{
				MakeSDBlock(BlockSymbol, &ListsID, level);
				BlockSymbol->Release();
			}
			EnumSym->Release();
			EnumSym = nullptr;

			if (ListsID.Size()>=1)
			{
				func->blocks = new unsigned long[ListsID.Size()];
				memset(func->blocks, 0, sizeof(unsigned long)*ListsID.Size()); //sizeof ???
				func->nblocks = ListsID.Size();

				int n = 0;
				TListIter<unsigned long> It = ListsID.Begin();
				while (It != ListsID.End())
				{
					func->blocks[n++] = *It;
					++It;
				}
			}
			ListsID.Clear();
		}

		if (SUCCEEDED(Symbol->findChildren(SymTagData, 0, 0, &EnumSym)) && EnumSym)
		{
			LONG Count = 0;
			if (SUCCEEDED(EnumSym->get_Count(&Count)) && Count)
			{
				func->nstack = Count;
				func->stack = new unsigned long[Count];
				memset(func->stack, 0, sizeof(*(func->stack))*Count);
			}

			IDiaSymbol *DataSymbol = nullptr;
			int n = 0;
			ULONG Celt = 0;
			while (SUCCEEDED(EnumSym->Next(1, &DataSymbol, &Celt)) && Celt == 1)
			{
				DATATYPE *data;
				func->stack[n] = ParseData(DataSymbol, 0, (NULLTYPE**)&data, level);
				if (func->stack[n])
				{	
				//	if (data) data->ClassID = func->ID; //??? ISSUE
					++n;
				}
				DataSymbol->Release();
			}
			EnumSym->Release();
			EnumSym = nullptr;
			func->nstack = n;
		}

		if (SUCCEEDED(Symbol->findChildren(SymTagLabel, 0, 0, &EnumSym)) && EnumSym)
		{
			LONG Count = 0;
			if (SUCCEEDED(EnumSym->get_Count(&Count)) && Count)
			{
				EnumSym->get_Count(&Count);
				func->nlabels = Count;
				//FUNCTIONLABEL *label = new FUNCTIONLABEL[Count];
				func->labels = new FUNCTIONLABEL[Count];
				memset(func->labels, 0, sizeof(*func->labels)*Count);
			}
			IDiaSymbol *LabelSymbol = nullptr;
			unsigned long n = 0;
			DWORD LabelCelt = 0;
			while (SUCCEEDED(EnumSym->Next(1, &LabelSymbol, &LabelCelt)) && LabelCelt == 1 && LabelSymbol)
			{
				//CDiaBSTR LabelName
				WCHAR *LabelName = nullptr;
				LabelSymbol->get_name(&LabelName);
				func->labels[n].Name = new char[TStrLen(LabelName)+1];
				TStrCpy(func->labels[n].Name, LabelName);
				::LocalFree(LabelName);

				LabelSymbol->get_relativeVirtualAddress(&func->labels[n].Offset);
				LabelSymbol->Release();
				LabelSymbol = nullptr;
				++n;
				LabelCelt = 0;
			}
			//func->labels = label;
			EnumSym->Release();
			EnumSym = nullptr;
		}

		IDiaSymbol *ReturnSymbol = nullptr;
		if (SUCCEEDED(Symbol->get_type(&ReturnSymbol)) && ReturnSymbol)
		{
			bool release = true;
			func->ReturnCallID = ParseDIASymbol(ReturnSymbol, &release, nullptr, level);
			if (release)
			ReturnSymbol->Release();
			map<unsigned long, DIABASICTYPE*>::IT it = m_Globals.find(func->ReturnCallID);
			if (it != m_Globals.end())
			{
				if (it->second->Elem.Type == ELEM_FUNC_TYPE)
				{
					func->ReturnTypeID = ((DIAFUNCTYPE *)it->second)->v.FuncTypeID;
				} else
				{
					func->ReturnTypeID = it->second->Elem.Type;
				}
			}
		}

		IDiaSymbol *ClassSymbol = nullptr;
		if (SUCCEEDED(Symbol->get_classParent(&ClassSymbol)) && ClassSymbol)
		{
			bool release = true;
			func->ClassID = ParseDIASymbol(ClassSymbol, &release, nullptr, level);
			if (release)
				ClassSymbol->Release();
		}

		return func->ID; //??? TODO POSSIBLE ISSUE
	}

	bool IsUnnamedTag(const char *Str)
	{
		char UNMANAGEDTAG[16];
		TStrCpy(UNMANAGEDTAG, "<unnamed-tag>");
		if (!Str) return false;
		int StrLen = TStrLen(Str);
		int Len = TStrLen(UNMANAGEDTAG);
		if (StrLen == 0 || StrLen < Len)
			return false;
		--StrLen;
		--Len;
		while (Len >= 0)
		{
			if (Str[StrLen] != UNMANAGEDTAG[Len])
				return false;
			--StrLen;
			--Len;
		}
		return StrLen <= 0 || Str[StrLen] == ':';
	}

	DIABASICTYPE *CSourceDebug::GetGlobal(unsigned long ID)
	{
		//m_Globals_IDType2ID_SET.begin();
		map<unsigned long, DIABASICTYPE*>::IT it = m_Globals.find(ID);
		if (it != m_Globals.end())
			return it->second;
#ifdef TXTDEBUG
fprintf(f,"%s NULL, ID %d\n", __func__, ID);
fflush(f);
#endif
		return nullptr;
	}

	unsigned long CSourceDebug::ParseUDType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		if (bRelease) *bRelease = true;

		if (Type) *Type = 0;

		//CDiaBSTR Name;
		WCHAR *Name = nullptr;
		if (FAILED(Symbol->get_name(&Name)) || !Name)
			return 0;

		char *pName = new char[TStrLen(Name)+1];
		TStrCpy(pName, Name);

		bool bUnnamed = false;

		if (IsUnnamedTag(pName))
		{
			char *pUnName = new char[TStrLen(Name)+(sizeof("_00000000")-1)+1];
			TStrCpy(pUnName, pName);
			TSPrintf(&pUnName[TStrLen(pName)], "_%08x", m_ElemID);

			std::list<IDTYPE>::iterator it = m_UDTList.begin();
			while (it != m_UDTList.end())
			{
				if (SUCCEEDED(m_Session->symsAreEquiv(Symbol, it->Type->Symbol)))
				{
					if (Type)
						*Type = it->Type;
					delete []pUnName;
					delete []pName;
			
					return it->Type->Elem.ID;
				}
				++it;
			}
			delete []pName;
			pName = pUnName;
			bUnnamed = true;
		} else
		{
			//( (pName,diatype), diatype->Elem.ID) //pName KEY
			m_StrIDMAPIter = m_UDTStrIDMap.find(pName); //char*,Type*,ID std::set?
			if (m_StrIDMAPIter != m_UDTStrIDMap.end())
			{
				if (Type)
					*Type = GetGlobal(m_StrIDMAPIter->second);
				delete []pName;
				return m_StrIDMAPIter->second;
			}
		}
		
		DWORD udtKind = 0;
		if (FAILED(Symbol->get_udtKind(&udtKind))) return 0;

		ULONGLONG length = 0;
		if (FAILED(Symbol->get_length(&length))) return 0;

		DWORD tag = 0;
		if (FAILED(Symbol->get_symTag(&tag))) return 0;

		unsigned long Size = sizeof(DIAUDTYPE);
		DIAUDTYPE *diatype = (DIAUDTYPE *)new char[Size];
		memset(diatype, 0, Size); //sizeof(DIAUDTYPE));//DIABASICTYPE));
		diatype->Elem.Name = pName;

		::LocalFree(Name);

		if (udtKind == UdtStruct)
			diatype->Elem.Type = ELEM_STRUCT;
		else
		if (udtKind == UdtClass)
			diatype->Elem.Type = ELEM_CLASS;
		else
		if (udtKind == UdtUnion)
			diatype->Elem.Type = ELEM_UNION;

		diatype->Elem.Length = length;
		elemUDT *v = &diatype->v;
		//memset(&diatype->v, 0, sizeof(diatype->v));
		diatype->Elem.ID = m_ElemID++;
		diatype->Value = v;
		diatype->Elem.Size = Size;

		if (bUnnamed)
		{
			IDTYPE idtype;
			idtype.ID = diatype->Elem.ID;
			idtype.Type = diatype;
			m_UDTList.push_back(idtype);
			v->unnamed = 1;
			diatype->Symbol = Symbol;
			if (bRelease) *bRelease = false;
		}

		m_Globals.Insert(diatype->Elem.ID, (DIABASICTYPE*)diatype);

//		m_NameIDMap.insert(pName, diatype->Elem.ID);

		//m_Globals_IDType2ID_SET ( (diatype->Elem.ID,diatype), diatype->Elem.ID) //ID KEY
		m_UDTStrIDMap.Insert(pName, diatype->Elem.ID);
		//m_UDTMap_STRType2ID_X_MAP ( (pName,diatype), diatype->Elem.ID) //pName KEY
		//m_Globals_ID2Type_MAP (diatype->Elem.ID,diatype) //ID KEY

		BOOL value;

		value = FALSE;
		if (SUCCEEDED(Symbol->get_scoped(&value)))
			v->scoped = value!=0;

		value = FALSE;
		if (SUCCEEDED(Symbol->get_nested(&value)))
			v->nested = value!=0;

		value = FALSE;
		if (SUCCEEDED(Symbol->get_packed(&value)))
			v->packed = value!=0;

		value = FALSE;
		if (SUCCEEDED(Symbol->get_overloadedOperator(&value)))
			v->overloadedOperator = value!=0;

		value = FALSE;
		if (SUCCEEDED(Symbol->get_hasAssignmentOperator(&value)))
			v->hasAssignmentOperator = value!=0;

		value = FALSE;
		if (SUCCEEDED(Symbol->get_hasCastOperator(&value)))
			v->hasCastOperator = value!=0;

		value = FALSE;
		if (SUCCEEDED(Symbol->get_hasNestedTypes(&value)))
			v->hasNestedTypes = value!=0;

		value = FALSE;
		if (SUCCEEDED(Symbol->get_constructor(&value)))
			v->constructor = value!=0;


            // BASECLASS
		IDiaEnumSymbols *EnumSymbols = nullptr;
		if (FAILED(Symbol->findChildren(SymTagBaseClass, 0, 0, &EnumSymbols)) || !EnumSymbols)
			return 0;

		LONG Count = 0;
		if (SUCCEEDED(EnumSymbols->get_Count(&Count)) && Count)
		{
			unsigned long *BaseClassIDs = new unsigned long[Count];
			unsigned long n = 0;
			IDiaSymbol *pDiaSymbol = nullptr;
			ULONG nCel = 0;
			while (SUCCEEDED(EnumSymbols->Next(1, &pDiaSymbol, &nCel)) && nCel == 1 && pDiaSymbol)
			{
				bool release = true;
				unsigned long ID = ParseDIASymbol(pDiaSymbol, &release, nullptr, level);
				if (ID)
				{
					BaseClassIDs[n] = ID;
					DIABASICTYPE *bt = GetGlobal(ID);
					elemBASECLASS *bc = (elemBASECLASS *)bt->Value;
					bc->X = diatype->Elem.ID;
				} else
				{
					BaseClassIDs[n] = 0;
				}

				if (release)
				{
					pDiaSymbol->Release();
					pDiaSymbol = nullptr;
				}
				++n;
			}
			v->BaseClass = BaseClassIDs;
			v->BaseClassLen = Count;
		} else
		{
			v->BaseClassLen = 0;
			v->BaseClass = nullptr;
		}
		EnumSymbols->Release();


		// VTBL
		CLASSPUBSYM *pubsym = new CLASSPUBSYM[(v->BaseClassLen <= 1) ? 1 : v->BaseClassLen];
		memset(pubsym, 0, ((v->BaseClassLen <= 1) ? 1 : v->BaseClassLen)*sizeof(CLASSPUBSYM)); //+++

		unsigned long pubCount = 0;

		set<PUBSYM>::iterator pit = m_ClassPublicSymbols.begin();
#if 1
		while (pit != m_ClassPublicSymbols.end())
		{
			char *name = pit->name;
		//	if (!TStrCmp(diatype->Elem.Name, "pairNode")) { ++pit; printf("SKIP pairNode\n"); continue;}

			char *StrName = TStrStr(name, diatype->Elem.Name);
			if (StrName)
			{
				if (!TStrNCmp(&StrName[TStrLen(diatype->Elem.Name)], "::`vftable'", (sizeof("::`vftable'")-1)))
				{
					v->vtable = 1;

					pubsym[pubCount].length = pit->length;
					pubsym[pubCount].rva = pit->rva;

					if (v->BaseClassLen <= 1)
					{
						pubsym[pubCount].ID = diatype->Elem.ID;
					} else
					{
						char *solvtbl = TStrStr(StrName, "::`vftable'{for `");
						if (solvtbl)
						{
							solvtbl += (sizeof("::`vftable'{for `")-1);
							char *eolvtbl = TStrChr(solvtbl, '\'');
							for (int i = 0; i < v->BaseClassLen; ++i)
							{
								//DIABASECLASSTYPE *bc = (DIABASECLASSTYPE *)GetGlobal(v->BaseClass[i]);
								DIABASICTYPE *bc = (DIABASICTYPE*)GetGlobal(v->BaseClass[i]);
								elemBASECLASS *baseclass = (elemBASECLASS*)bc->Value;

								DIAUDTYPE *udt = (DIAUDTYPE *)GetGlobal(baseclass->ID);
								if (!TStrNCmp(udt->Elem.Name, solvtbl, eolvtbl - solvtbl))
									pubsym[pubCount].ID = baseclass->ID;
							}
						}
					}
					++pubCount;
					break; //???
				}
			}
			++pit;
		}
#endif
		v->PubSymbolsLen = pubCount; //???
		if (pubCount)
		{
			v->PubSymbolsLen = pubCount;
			v->PubSymbols = pubsym;
		} else
		{
			v->PubSymbolsLen = 0;
			v->PubSymbols = nullptr;
			delete []pubsym;
		}


		// FUNCTION
		EnumSymbols = nullptr;
		if (FAILED(Symbol->findChildren(SymTagFunction, 0, 0, &EnumSymbols)) || !EnumSymbols)
			return 0;

		Count = 0;
		if (SUCCEEDED(EnumSymbols->get_Count(&Count)) && Count)
		{
			unsigned long *FuncIDs = new unsigned long[Count];
			memset(FuncIDs, 0, Count*(sizeof(*FuncIDs)));
			unsigned long n = 0;
			IDiaSymbol *pDiaSymbol = nullptr;
			ULONG nCel = 0;
			while (SUCCEEDED(EnumSymbols->Next(1, &pDiaSymbol, &nCel)) && nCel == 1 && pDiaSymbol)
			{
				FUNCTION *func = nullptr;
				bool release = true;
				FuncIDs[n] = ParseFunction(pDiaSymbol, &release, (NULLTYPE**)&func, level);
				if (release)
				{
					pDiaSymbol->Release();
					pDiaSymbol = nullptr;
				}

				if (FuncIDs[n])
				{
					func->ClassID = diatype->Elem.ID;
					++n;
				}
			}

			if (n > 1)
			{
				std::sort(FuncIDs, &FuncIDs[n]);
				unsigned long N = FuncIDs - std::unique(FuncIDs, &FuncIDs[n]);
			#if 0
				//n = sorted(FuncIDs, n);
			#else
				Count = n;
				for (n = 0; n < Count - 1 && FuncIDs[n] < FuncIDs[n + 1]; ++n);
				++n;
			#endif
			}
			v->FunctionLen = n;
			v->Function = FuncIDs;
		} else
		{
			v->FunctionLen = 0;
			v->Function = nullptr;
		}
		EnumSymbols->Release();
		EnumSymbols = nullptr;


		if (FAILED(Symbol->findChildren(SymTagData, 0, 0, &EnumSymbols)) || !EnumSymbols)
			return 0;

		Count = 0;
		if (SUCCEEDED(EnumSymbols->get_Count(&Count)) && Count)
		{
			v->DataLen = Count;
			v->Data = new unsigned long[Count];
			memset(v->Data, 0, Count*(sizeof(*(v->Data))));
			unsigned long n = 0;
			IDiaSymbol *pDiaSymbol = nullptr;
			ULONG nCel = 0;
			while (SUCCEEDED(EnumSymbols->Next(1, &pDiaSymbol, &nCel)) && nCel == 1 && pDiaSymbol)
			{
				DATATYPE *data = nullptr;
				v->Data[n] = ParseData(pDiaSymbol, nullptr, (NULLTYPE**)&data, level);
				if (data)
				{
					data->ClassID = diatype->Elem.ID;
					DIAUDTYPE *diaudt = (DIAUDTYPE *)GetGlobal(data->TypeID);
					if (diaudt) //+++???
					if (diaudt->Elem.Type == ELEM_CLASS ||
					    diaudt->Elem.Type == ELEM_STRUCT ||
					    diaudt->Elem.Type == ELEM_UNION)
					{
						elemUDT *udt = &diaudt->v;
						if (//_v60 != (DIAUDTYPE *)-0x1C && //???
							udt->unnamed)
							udt->X1 = 1;
					}
				}
				pDiaSymbol->Release();
				pDiaSymbol = nullptr;
				++n;
			}
		} else
		{
			v->DataLen = 0;
			v->Data = nullptr;
		}
		EnumSymbols->Release();
		EnumSymbols = nullptr;

                  // SKIP
		if (FAILED(Symbol->findChildren(SymTagFriend, 0, 0, &EnumSymbols)) || !EnumSymbols)
			return 0;

		Count = 0;
		if (SUCCEEDED(EnumSymbols->get_Count(&Count)) && Count)
		{
			unsigned long *FriendIDs = new unsigned long[Count];
			memset(FriendIDs, 0, Count*(sizeof(*FriendIDs)));
			unsigned long n = 0;
			IDiaSymbol *pDiaSymbol = nullptr;
			ULONG nCel = 0;
			while (SUCCEEDED(EnumSymbols->Next(1, &pDiaSymbol, &nCel)) && nCel == 1 && pDiaSymbol)
			{
				FUNCTION *func = nullptr;
				bool release = true;
				v->Friend[n] = ParseFunction(pDiaSymbol, &release, (NULLTYPE**)&func, level);
				if (release)
				{
					pDiaSymbol->Release();
					pDiaSymbol = nullptr;
				}

				if (v->Friend[n])
					++n;
			}

			if (n > 1)
			{
				std::sort(FriendIDs, &FriendIDs[n]);
				unsigned long N = FriendIDs - std::unique(FriendIDs, &FriendIDs[n]);
			#if 0
				//n = sorted(FriendIDs, n);
			#else
				Count = n;
				for (n = 0; n < Count - 1 && FriendIDs[n] < FriendIDs[n + 1]; ++n);
				++n;
			#endif
			}
			v->FriendLen = Count; //???
			v->Friend = FriendIDs;
		} else
		{
			v->FriendLen = 0;
			v->Friend = nullptr;
		}
		EnumSymbols->Release();
		EnumSymbols = nullptr;

		if (Type)
			*Type = diatype;

		return diatype->Elem.ID;
	}

	unsigned long CSourceDebug::ParseEnumType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		if (Type) *Type = 0;

		if (bRelease) *bRelease = true;

		std::list<IDTYPE>::iterator it = m_EnumList.begin();
		while (it != m_EnumList.end())
		{
			if (SUCCEEDED(m_Session->symsAreEquiv(Symbol, it->Type->Symbol)))
			{
				if (Type)
					*Type = it->Type;
				return it->Type->Elem.ID; //+C
			}
			++it;
		}

		IDiaEnumSymbols *EnumSymbols = nullptr;
		if (FAILED(Symbol->findChildren(SymTagData, 0, 0, &EnumSymbols))) return 0;

		LONG nEnums = 0;
		if (FAILED(EnumSymbols->get_Count(&nEnums))) return 0;

		unsigned long Size = sizeof(DIAENUMTYPE);//0x34;
		if (nEnums)
			Size += (nEnums - 1)*sizeof(ENUMTYPEVALUE);

		DIAENUMTYPE *diatype = (DIAENUMTYPE *)new char[Size];
		memset(diatype, 0, Size);
		elemENUM *v = &diatype->v;
		diatype->v.Length = nEnums;
		diatype->Symbol = Symbol;
		diatype->Elem.ID = m_ElemID++;
		diatype->Elem.Type = ELEM_ENUM;
		diatype->Elem.Size = Size;

		IDTYPE idtype;
		idtype.ID = diatype->Elem.ID;
		idtype.Type = diatype;
		m_EnumList.push_back(idtype);

		//v10 = PAIR_sub_431550((int)v21, (int)&idtype, (int)&idtype);
		//std_map_insert_IDType2ID_sub_430F10(&this->m_Globals_IDType2ID_SET, v20, (int)v10);

		BOOL value = FALSE;
		Symbol->get_nested(&value);
		v->Nested = value != 0;

		value = FALSE;
		Symbol->get_scoped(&value);
		v->Scoped = value != 0;

		//CDiaBSTR Name;
		WCHAR *Name = nullptr;
		if (FAILED(Symbol->get_name(&Name))) return 0;

		ULONGLONG length = 0;
		if (FAILED(Symbol->get_length(&length)))
		{
			::LocalFree(Name); //+++
			return 0;
		}

		diatype->Elem.Length = length;
		diatype->Elem.Name = new char[TStrLen(Name)+1];
		TStrCpy(diatype->Elem.Name, Name);
		::LocalFree(Name); //+++
		diatype->Value = v;
	{
		IDiaSymbol *SymbolType = nullptr;
		if (FAILED(Symbol->get_type(&SymbolType)) && !SymbolType) return 0; //??? ||

		bool release = true;
		v->ID = ParseDIASymbol(SymbolType, &release, nullptr, level);
		if (release)
		{
			SymbolType->Release();
			SymbolType = nullptr;
		}
	}
		IDiaSymbol *SymbolElement = nullptr;
		unsigned long n = 0;
		ULONG Count = 0;
		unsigned long long Val = 0; //???
		while (SUCCEEDED(EnumSymbols->Next(1, &SymbolElement, &Count)) && Count == 1 && SymbolElement)
		{
			DWORD locationType = 0;
			if (FAILED(SymbolElement->get_locationType(&locationType))) return 0;

			if (locationType != LocIsConstant) return 0;

			VARIANT Value = { VT_EMPTY };
			//Value.lVal = 16; //VT_UI1
			if (FAILED(SymbolElement->get_value(&Value))) return 0;

			Val = 0;
			switch (Value.vt)
			{
			case VT_ERROR:
				//???
				break;
			case VT_I1:
				Val = Value.cVal;
				break;
			case VT_UI1:
				Val = Value.bVal;
				break;
			case VT_I2:
				Val = Value.iVal;
				break;
			case VT_UI2:
				Val = Value.uiVal;
				break;
			case VT_I4:
				Val = Value.lVal;
				break;
			case VT_UI4:
				Val = Value.ulVal;
				break;
			case VT_I8:
				Val = Value.llVal;
				break;
			case VT_UI8:
				Val = Value.ullVal;
				break;
			case VT_INT:
				Val = Value.intVal;
				break;
			case VT_UINT:
				Val = Value.uintVal;
				break;
			default:
				Val = 0;
				break;
			}
			v->Values[n].Value = Val;

			VariantClear((VARIANTARG *) &Value); //+++

			//CDiaBSTR ElemName;
			WCHAR *ElemName = nullptr;
			if (FAILED(SymbolElement->get_name(&ElemName)) || !ElemName) return 0;
			v->Values[n].Name = new char[TStrLen(ElemName)+1];
			TStrCpy(v->Values[n].Name, ElemName);
			::LocalFree(ElemName);
			ElemName = nullptr;

			SymbolElement->Release();
			SymbolElement = nullptr;
			Count = 0;
			++n;
		}
		EnumSymbols->Release();

		//v14 = PAIR_sub_4314C0((int)&v17, (int)&diatype->Elem.ID, (int)&diatype);
		//std_map_insert_ID2Type_sub_431180(&this->m_GlobalsIDTypeMap, (int)v16, (int)v14);
		m_Globals.Insert(diatype->Elem.ID, diatype);

		if (bRelease) *bRelease = false;
		if (Type) *Type = diatype;
		return diatype->Elem.ID;
	}

	unsigned long CSourceDebug::ParseFunctionType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		if (bRelease) *bRelease = false;

		IDiaEnumSymbols *EnumSymbols = nullptr;
		if (FAILED(Symbol->findChildren(SymTagFunctionArgType, 0, 0, &EnumSymbols))) return 0;

		LONG ArgsCount = 0;
		if (FAILED(EnumSymbols->get_Count(&ArgsCount))) return 0;

		unsigned long Size = sizeof(DIAFUNCTYPE);//0x30;
		if (ArgsCount)
			Size += (ArgsCount-1)*4; //sizeof ???

		DIAFUNCTYPE *diatype = (DIAFUNCTYPE *)new char[Size];
		memset(diatype, 0, Size);
		elemFUNC *v = &diatype->v;
		diatype->Value = &diatype->v;
		diatype->Elem.Size = Size;
		diatype->Elem.ID = m_ElemID++;
		diatype->Symbol = Symbol;

		IDTYPE idtype;
		idtype.ID = diatype->Elem.ID;
		idtype.Type = diatype;
		m_FuncTypeList.push_back(idtype);

		//v5 = PAIR_sub_431550((int)v14, (int)&idtype, (int)&idtype);
		//std_map_insert_IDType2ID_sub_430F10(&m_Globals_IDType2ID_SET, v13, (int)v5);


		m_Globals.Insert(diatype->Elem.ID, (DIABASICTYPE*)diatype);
		//v6 = PAIR_sub_4314C0((int)&v12, (int)&diatype->Elem.ID, (int)&diatype);
		//std_map_insert_ID2Type_sub_431180(&m_Globals_ID2Type_MAP, (int)v11, (int)v6);
	{
		IDiaSymbol *SymbolType = nullptr;
		if (FAILED(Symbol->get_type(&SymbolType)) || !SymbolType) return 0;

		bool release = true;
		v->FuncTypeID = ParseDIASymbol(SymbolType, &release, nullptr, level);
		if (release)
		{
			SymbolType->Release();
			SymbolType = nullptr;
		}
	}
	{
		IDiaSymbol *SymbolType = nullptr;
		if (FAILED(Symbol->get_objectPointerType(&SymbolType))) return 0;

		if (SymbolType)
		{
			bool release = true;
			v->ObjPtrID = ParseDIASymbol(SymbolType, &release, nullptr, level);
			if (release)
			{
				SymbolType->Release();
				SymbolType = nullptr;
			}
			LONG Adjust = 0;
			Symbol->get_thisAdjust(&Adjust);
			v->ThisAdjust = Adjust;
		}
	}
		DWORD callingConvention = 0;
		if (FAILED(Symbol->get_callingConvention(&callingConvention))) return 0;

		v->CallingConvention = callingConvention;
		v->nArgs = ArgsCount;

		ULONG Count = ArgsCount; //???
		unsigned long n = 0;
		IDiaSymbol *SymbolArg = nullptr;
		while (SUCCEEDED(EnumSymbols->Next(1, &SymbolArg, &Count)) && Count == 1)
		{
			IDiaSymbol *SymbolArgType = nullptr;
			if (FAILED(SymbolArg->get_type(&SymbolArgType))) return 0;
			bool release = true;
			v->ArgsIDs[n] = ParseDIASymbol(SymbolArgType, &release, nullptr, level);
			if (release)
			{
				SymbolArgType->Release();
				SymbolArgType = nullptr;
			}
			SymbolArg->Release();
			SymbolArg = nullptr;
			++n;
		}
		EnumSymbols->Release();
		EnumSymbols = nullptr;

		diatype->Elem.Type = ELEM_FUNC_TYPE;
		diatype->Elem.Name = nullptr;
		diatype->Symbol = Symbol;

		return diatype->Elem.ID;
	}

	unsigned long CSourceDebug::ParsePointerBase(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		BOOL reference = FALSE;
		if (FAILED(Symbol->get_reference(&reference))) return 0;
		if (reference)
			return ParsePointerRefType(Symbol, bRelease, Type, level);
		return ParsePointerType(Symbol, bRelease, Type, level);
	}

	IDiaSymbol *CSourceDebug::GetSymbolPointer(IDiaSymbol *Symbol, int, unsigned long *Count)
	{
		int Number = 0;
		if (!Symbol) return nullptr;
		if (!SameTag(Symbol, SymTagPointerType)) return nullptr;
		BOOL reference = FALSE;
		if (FAILED(Symbol->get_reference(&reference))) return nullptr;
		if (reference) return nullptr;
		IDiaSymbol *SymbolType = nullptr;
		unsigned long n = 0;
		for (n = 0; ; n = 1)
		{
			if (FAILED(Symbol->get_type(&SymbolType)) || !SymbolType)  return nullptr;
			++Number;
			DWORD tag = 0;
			if (FAILED(SymbolType->get_symTag(&tag)))
			{
				SymbolType->Release();
				return nullptr;
			}
			if (tag != SymTagPointerType) break;

			if (FAILED(SymbolType->get_reference(&reference)))
			{
				SymbolType->Release();
				return nullptr;
			}

			if (reference)
			{
				SymbolType->Release();
				return nullptr;
			}
			if (n)
				Symbol->Release();
			Symbol = SymbolType;
		}
		if (Count) *Count = Number;
		if (n)
			Symbol->Release();
		return SymbolType;
	}

	char * CSourceDebug::GetName(unsigned long ID)
	{
		char *NewStr = nullptr;
		int Len = 0x2000;
		WCHAR *Str = new WCHAR[Len];
		GetName(ID, Str, &Len);
		int StrLen = TStrLen(Str);
		if (StrLen != 0)
		{
			NewStr = new char[StrLen+1];
			TStrCpy(NewStr, Str);
		}
		delete []Str;
		return NewStr;
	}
	WCHAR * CSourceDebug::GetName(unsigned long ID, WCHAR *pStr, int *pLen)
	{
		*pStr = 0;
		DIABASICTYPE *diabase = (DIABASICTYPE *)GetGlobal(ID);
		if (!diabase) return nullptr;
		return GetName(diabase, pStr, pLen, 0);
	}
	WCHAR * CSourceDebug::GetName(DIABASICTYPE *Type, WCHAR *pStr, int *pLen, int)
	{
		if (pStr) *pStr = 0;
		if (!Type) return nullptr;
		switch (Type->Elem.Type)
		{
		case ELEM_POINTER:{
			elemPOINTER *pointer = (elemPOINTER *)Type->Value;
			map<unsigned long, DIABASICTYPE*>::IT it = m_Globals.find(pointer->ID);
			if (it == m_Globals.end())
				return nullptr;
			DIABASICTYPE *bc = it->second;
			GetName(pointer->ID, pStr, pLen);
			//StaticBuffer<10> pBuf(pointer->Num);
			WCHAR Buf[11];
			WCHAR *pBuf = Buf;
			if (pointer->Num > 9)
			{
				pBuf = new WCHAR[pointer->Num + 1];
			}
			int n = 0;
			for (n = 0; n < pointer->Num; ++n)
				pBuf[n] = L'*';
			pBuf[n] = 0;
			if (bc->Elem.Type == ELEM_FUNC_TYPE)
			{
				WCHAR *StrArgs = new WCHAR[*pLen];
				TStrCpy(StrArgs, pStr);
				WCHAR *StrFunc = TStrChr(pStr, L'(');
				if (!StrFunc)
				{
					if (pBuf != Buf) delete pBuf;
					delete []StrArgs;
					return pStr;
				}
				int Pos = StrFunc - pStr;
				pStr[Pos] = '(';
				TStrCpy(&pStr[Pos + 1], pBuf);
				TStrCat(pStr, L")");
				TStrCat(pStr, &StrArgs[Pos]);
			} else
			{
				TStrCat(pStr, pBuf);
			}
			if (pBuf != Buf) delete []pBuf;
			return pStr;
			}break;
		case ELEM_POINTERREF:{
			elemPOINTERREF *pointerref = (elemPOINTERREF *)Type->Value;
			map<unsigned long, DIABASICTYPE*>::IT it = m_Globals.find(pointerref->ID);
			if (it == m_Globals.end())
				return nullptr;
			GetName(it->second, pStr, pLen, 0);
			TStrCat(pStr, WSTR("&"));
			return pStr;
			}break;
		case ELEM_ARRAY:{
			elemARRAY *array = (elemARRAY *)Type->Value;
			//str_find_ID2Type_sub_431350(&this->m_GlobalsIDTypeMap, v19, (int)&array->ID);
			//v8 = std_map_end_sub_401890(&this->m_GlobalsIDTypeMap, (int)v10);
			//if ( IT_eq_sub_4316C0((int)v19, v8) )
			map<unsigned long, DIABASICTYPE*>::IT it = m_Globals.find(array->ID);
			if (it == m_Globals.end())
				return nullptr;
			//v36 = *(DIABASETYPE **)(deref_sub_431A30((int)v19) + 4);
			GetName(it->second, pStr, pLen, 0);
			for (int n = 0; n < array->Size; ++n)
			{
				WCHAR Buffer[256];
				TSPrintf(Buffer, WSTR("%d"), array->Values[n].Size);
				TStrCat(pStr, WSTR("["));
				TStrCat(pStr, Buffer);
				TStrCat(pStr, WSTR("]"));
			}
			return pStr;
			}break;
		case ELEM_FUNC_TYPE:{
			elemFUNC *func = (elemFUNC *)Type->Value;
			GetName(func->FuncTypeID, pStr, pLen);
			switch (func->CallingConvention)
			{
			case CV_CALL_NEAR_C://0:
				TStrCat(pStr, WSTR(" __cdecl "));
				break;
			case CV_CALL_NEAR_FAST://4:
				TStrCat(pStr, WSTR(" __fastcall "));
				break;
			case CV_CALL_NEAR_STD://7:
				TStrCat(pStr, WSTR(" __stdcall "));
				break;
			case CV_CALL_NEAR_SYS://9:
				TStrCat(pStr, WSTR(" __syscall "));
				break;
			case CV_CALL_THISCALL://11:
				TStrCat(pStr, WSTR(" __thiscall "));
				break;
			default:
				TStrCat(pStr, WSTR(" Unknown calling convention in function "));
				break;
			}
			int nArgPos = TStrLen(pStr);
			TStrCat(&pStr[nArgPos++], WSTR("("));
			for (int n = 0; n < func->nArgs; ++n)
			{
				GetName(func->ArgsIDs[n], &pStr[nArgPos], pLen);
				nArgPos = TStrLen(pStr);
				TStrCat(&pStr[nArgPos++], WSTR(","));
			}
			if (func->nArgs)
				pStr[nArgPos - 1] = L')';
			else	TStrCat(&pStr[nArgPos++], WSTR(")"));
			return pStr;
			}break;
		case ELEM_BASE_CLASS:{
			DIABASECLASSTYPE *diabc = (DIABASECLASSTYPE *)Type;
			return GetName(diabc->v.ID, pStr, pLen);
			}break;
		default:
			if (Type->Elem.Name)
				TStrCpy(pStr, Type->Elem.Name);
			else	TStrCpy(pStr, WSTR("no name"));
		}
		return pStr;
	}

	unsigned long CSourceDebug::ParsePointerType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		if (Type) *Type = 0;
		if (bRelease) *bRelease = true; //??? false
		unsigned long Count = 1;
		IDiaSymbol *SymbolPointer = GetSymbolPointer(Symbol, 0, &Count);
		bool release = true;
		unsigned long ID = ParseDIASymbol(SymbolPointer, &release, nullptr, level);
		if (release)
		{
			SymbolPointer->Release();
			SymbolPointer = nullptr;
		}

		char *Name = GetName(ID);
		unsigned long long length = 0;
		if (FAILED(Symbol->get_length(&length)))
		{
			delete []Name;
			return 0;
		}

		DIAPOINTERTYPE *diatype = nullptr;
		unsigned long PrevID = 0;
		for (unsigned long n = 1; n <= Count; ++n)
		{
			char *pName = nullptr;
			if (Name)
			{
				pName = new char[TStrLen(Name)+(sizeof("_00000000")-1)+1]; //??? sizeof("_00000000")
				//*pName = 0;
				TSPrintf(pName, "%s_%08x", Name, n);
			} else
			{
				pName = new char[(sizeof("00000000_00000000")-1)+1]; //??? sizeof("00000000_00000000")
				//*pName = 0;
				TSPrintf(pName, "%08x_%08x", ID, n);
			}

			m_StrIDMAPIter = m_PointerStrIDMap.find(pName);
			if (m_StrIDMAPIter != m_PointerStrIDMap.end())
			{
				PrevID = m_StrIDMAPIter->second;
				if (n == Count)
				{
					if (Type) *Type = GetGlobal(m_StrIDMAPIter->second);
					delete []pName;
					delete []Name;
					return m_StrIDMAPIter->second;
				}
				delete []pName;
				pName = nullptr;
			} else
			{
				m_PointerStrIDMap.Insert(pName, m_ElemID);

				unsigned long Size = sizeof(DIAPOINTERTYPE);
				diatype = (DIAPOINTERTYPE *)new char[Size];
				memset(diatype, 0, Size);
				elemPOINTER *v = &diatype->v;
				diatype->v.ID = ID;
				v->Num = n;
				v->PrevID = PrevID;
				diatype->Elem.Type = ELEM_POINTER;
				diatype->Value = v;
				diatype->Elem.Size = Size;
				diatype->Elem.Length = length;
				diatype->Elem.Name = pName;
				diatype->Elem.ID = m_ElemID++;

				PrevID = diatype->Elem.ID;

				//v11 = PAIR_sub_4314C0((int)&v17, (int)&diatype->Elem.ID, (int)&diatype);
				//std_map_insert_ID2Type_sub_431180(&this->m_GlobalsIDTypeMap, (int)v16, (int)v11);
				m_Globals.Insert(diatype->Elem.ID, diatype);

				IDTYPE idtype;
				idtype.ID = diatype->Elem.ID;
				idtype.Type = diatype;
				//v12 = PAIR_sub_431550((int)v15, (int)&idtype, (int)&idtype);
				//std_map_insert_IDType2ID_sub_430F10(&this->m_Globals_IDType2ID_SET, v14, (int)v12);

				if (Type) *Type = diatype;
				if (bRelease) *bRelease = true;
			}
		}

		if (Name) delete []Name;
		return diatype ? diatype->Elem.ID : 0;
	}
	unsigned long CSourceDebug::ParsePointerRefType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		if (Type) *Type = 0;
		if (bRelease) *bRelease = true; //??? false
		IDiaSymbol *SymbolType = nullptr;
		if (FAILED(Symbol->get_type(&SymbolType)) || !SymbolType) return 0;
		bool release = true;
		unsigned long ID = ParseDIASymbol(SymbolType, &release, nullptr, level);
		if (release)
		{
			SymbolType->Release();
			SymbolType = nullptr;
		}
		char *pName = nullptr;
		char *Name = GetName(ID);
		if (Name)
		{
			pName = new char[TStrLen(Name)+1+1];
			TStrCpy(pName, Name);
			TStrCat(pName, "&");
			delete []Name;
		} else
		{
			pName = new char[2];
			pName[0] = '&';
			pName[1] = 0;
		}

		m_StrIDMAPIter = m_PointerRefStrIDMap.find(pName);
		if (m_StrIDMAPIter != m_PointerRefStrIDMap.end())
		{
			if (Type) *Type = GetGlobal(m_StrIDMAPIter->second);
			delete []pName;
			return m_StrIDMAPIter->second;
		}

		m_PointerRefStrIDMap.Insert(pName, m_ElemID);

		unsigned long Size = sizeof(DIAPOINTERREFTYPE);
		DIAPOINTERREFTYPE *diatype = (DIAPOINTERREFTYPE *)new char[Size];
		memset(diatype, 0, Size);
		diatype->Elem.Size = Size;
		elemPOINTERREF *v = &diatype->v;
		diatype->v.ID = ID;
		diatype->Elem.ID = m_ElemID++;
		diatype->Elem.Type = ELEM_POINTERREF;
		diatype->Elem.Name = pName;
		diatype->Value = v;

		//v11 = PAIR_sub_4314C0((int)&v17, (int)&diatype->Elem.ID, (int)&diatype);
		//std_map_insert_ID2Type_sub_431180(&this->m_GlobalsIDTypeMap, (int)v16, (int)v11);
		m_Globals.Insert(diatype->Elem.ID, diatype);

		IDTYPE idtype;
		idtype.ID = diatype->Elem.ID;
		idtype.Type = diatype;
		//v12 = PAIR_sub_431550((int)v15, (int)&idtype, (int)&idtype);
		//std_map_insert_IDType2ID_sub_430F10(&this->m_Globals_IDType2ID_SET, v14, (int)v12);

		DIABASICTYPE*diabase = (DIABASICTYPE*)GetGlobal(v->ID);
		if (diabase)
			diatype->Elem.Length = diabase->Elem.Length;
		if (bRelease) *bRelease = true;
		if (Type) *Type = diatype;
		return diatype->Elem.ID;
	}
	bool CSourceDebug::GetArray(IDiaSymbol *Symbol, std::list<IDiaSymbol *> &Symbols)
	{
		IDiaSymbol *NextSymbol = 0;
		Symbols.clear();
		DWORD tag = 0;
		if (FAILED(Symbol->get_symTag(&tag))) return false;
		while (tag == SymTagArrayType)
		{
			Symbols.push_front(Symbol);
			if (FAILED(Symbol->get_type(&NextSymbol))) return false;
			if (FAILED(NextSymbol->get_symTag(&tag))) return false;
			Symbol = NextSymbol;
		}
		if (NextSymbol)
			NextSymbol->Release();
		return true;
	}

	struct IDSIZE
	{
		unsigned long ID;
		unsigned long Size;
	};

	unsigned long CSourceDebug::ParseArray(IDiaSymbol *Symbol, unsigned long ID, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		if (Type) *Type = 0;
		if (bRelease) *bRelease = true;
		
		std::list<IDTYPE>::iterator it = m_ArrayList.begin();
		while (it != m_ArrayList.end())
		{
			if (SUCCEEDED(m_Session->symsAreEquiv(Symbol, it->Type->Symbol)))
			{
				if (Type)
					*Type = it->Type;
				return it->Type->Elem.ID; //+C
			}
			++it;
		}
		
		ULONGLONG length = 0;
		if (FAILED(Symbol->get_length(&length))) return 0;

		IDiaSymbol *SymbolIndexType = nullptr;
		if (FAILED(Symbol->get_arrayIndexType(&SymbolIndexType))) return 0;
		IDiaSymbol *SymbolType = nullptr;
		if (FAILED(Symbol->get_type(&SymbolType))) return 0;
		ULONGLONG SymbolTypeLength = 0;
		if (FAILED(SymbolType->get_length(&SymbolTypeLength))) return 0;

		IDiaSymbol *f_Symbol = Symbol;
		ULONGLONG f_length = length;

		std::list<IDSIZE> IDSizeList;
		bool first = true;
		DWORD tag = SymTagArrayType;
		while (tag == SymTagArrayType)
		{
			IDSIZE idsize;
			if (SymbolTypeLength)
			{
				idsize.Size = f_length / SymbolTypeLength;
			} else
			{
				idsize.Size = 0;
			}
			bool release = true;
			idsize.ID = ParseDIASymbol(SymbolIndexType, &release, nullptr, level);
			if (release)
			{
				SymbolIndexType->Release();
				SymbolIndexType = nullptr;
			}
			IDSizeList.push_back(idsize);
			if (!first)
			{
				f_Symbol->Release();
				f_Symbol = nullptr;
			}
			first = false;
			f_Symbol = SymbolType;
			f_length = SymbolTypeLength;

			if (FAILED(SymbolType->get_symTag(&tag))) return 0;

			if (tag != SymTagArrayType) break;
			if (FAILED(f_Symbol->get_arrayIndexType(&SymbolIndexType))) return 0;
			if (FAILED(f_Symbol->get_type(&SymbolType))) return 0;
			if (FAILED(SymbolType->get_length(&SymbolTypeLength))) return 0;
		}

		unsigned long Size = sizeof(DIAARRAYTYPE);
		if (IDSizeList.size()>1)
			Size += (IDSizeList.size()-1)*sizeof(ARRAYVALUE);

		DIAARRAYTYPE *diatype = (DIAARRAYTYPE *)new char[Size];
		diatype->Elem.Size = Size;
		diatype->Symbol = Symbol;
		diatype->Elem.ID = m_ElemID++;

		IDTYPE idtype;
		idtype.ID = diatype->Elem.ID;
		idtype.Type = diatype;
		m_ArrayList.push_back(idtype);

		//v12 = PAIR_sub_431550((int)v33, (int)&idtype, (int)&idtype);
		//std_map_insert_IDType2ID_sub_430F10(&this->m_Globals_IDType2ID_SET, v32, (int)v12);


		//v13 = PAIR_sub_4314C0((int)&v31, (int)&diatype->Elem.ID, (int)&diatype);
		//std_map_insert_ID2Type_sub_431180(&this->m_GlobalsIDTypeMap, (int)v30, (int)v13);
		m_Globals.Insert(diatype->Elem.ID, diatype);

		elemARRAY *v = &diatype->v;
		diatype->Value = &diatype->v;
		bool release = true;
		v->ID = ParseDIASymbol(SymbolType, &release, nullptr, level);
		if (release)
			SymbolType->Release();

		{
		unsigned long n = 0;
		std::list<IDSIZE>::iterator it = IDSizeList.begin();
		while (it != IDSizeList.end())
		{
			v->Values[n].ID = it->ID;
			v->Values[n].Size = it->Size;
			++it;
			++n;
		}
		}

		v->Size = IDSizeList.size();
		v->ParentID = ID;
		IDSizeList.clear();

		diatype->Elem.Type = ELEM_ARRAY;
		diatype->Elem.Length = length;
		diatype->Value = v;
		diatype->Elem.Name = GetName(diatype->Elem.ID);
		if (Type) *Type = diatype;
		if (bRelease) *bRelease = false;
		return diatype->Elem.ID;
	}
	unsigned long CSourceDebug::ParseArrayType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		unsigned long n = 0;
		unsigned long ID = 0;
		std::list<IDiaSymbol *> Symbols;
		if (Type) *Type = 0;
		if (bRelease) *bRelease = true;
		if (!GetArray(Symbol, Symbols)) return 0;
		std::list<IDiaSymbol *>::iterator it = Symbols.begin();
		while (it != Symbols.end())
		{
			if (n) ++n;
			IDiaSymbol *symbol = *it;
			ID = ParseArray(symbol, ID, bRelease, Type, level);
			if (*bRelease) //??? bRelease
			{
				symbol->Release();
				symbol = nullptr;
				*bRelease = false;
			}
			++n;
			++it;
		}
		Symbols.clear();
		return ID;
	}

struct BASETYPEMAP
{
	unsigned long Type;
	const char *Name;
};
	static const BASETYPEMAP gBaseTypeMap[] =
		{
			{ btNoType, "NoType" },
			{ btVoid, "void" },
			{ btChar, "char" },
			{ btWChar, "wchar" },
			{ btInt, "short" },
			{ 0x80, "char" },
			{ 0x100, "short" },
			{ 0x200, "int" },
			{ 0x400, "__int64" },
			{ btUInt, "unsigned short int" },
			{ 0x800, "unsigned char" },
			{ 0x1000, "unsigned short" },
			{ 0x2000, "unsigned int" },
			{ 0x4000, "unsigned __int64" },
			{ btFloat, "float" },
			{ 0x8000, "double" },
			{ btBCD, "BCD" },
			{ btBool, "bool" },
			{ btLong, "long" },
			{ btULong, "unsigned long" },
			{ btCurrency, "Currency" },
			{ btDate, "date" },
			{ btVariant, "variant" },
			{ btComplex, "complex" },
			{ btBit, "bit" },
			{ btBSTR, "BSTR" },
			{ btHresult, "HRESULT" },
		};

	const char *CSourceDebug::GetBaseTypeName(unsigned long type, unsigned long length)
	{
		if (type == btInt)
			type = (length << 7);
		if (type == btUInt)
			type = (length << 11);
		if (type == btFloat && length == 8)
			type = 0x8000;
		for (int i = 0; i < lenof(gBaseTypeMap); ++i)
		{
			if (type == gBaseTypeMap[i].Type)
				return gBaseTypeMap[i].Name;
		}
		return nullptr;
	}

	unsigned long CSourceDebug::ParseBaseType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		if (bRelease) *bRelease = true;
		if (Type) *Type = 0;

		BASEDATATYPE baseType;

		if (FAILED(Symbol->get_baseType(&baseType.basetype))) return 0;
		if (FAILED(Symbol->get_length(&baseType.length))) return 0;

		const char *Name = GetBaseTypeName(baseType.basetype, baseType.length);

		std::map<BASEDATATYPE, unsigned long>::iterator it = m_BaseDataTypeIDMap.find(baseType);
		if (it != m_BaseDataTypeIDMap.end())
		{
			if (Type)
				*Type = GetGlobal(it->second);
			return it->second;
		}

		unsigned long Size = sizeof(DIABASICTYPE);
		DIABASICTYPE *diatype = (DIABASICTYPE*)new char[Size];
		memset(diatype, 0, sizeof(DIABASICTYPE));
		diatype->Elem.Name = new char[TStrLen(Name)+1];
		TStrCpy(diatype->Elem.Name, Name);
		diatype->Elem.Size = Size;
		diatype->Value = 0;
		diatype->Elem.Type = ELEM_BASE_TYPE;
		diatype->Elem.Length = baseType.length;
		diatype->Elem.ID = m_ElemID++;
		diatype->Symbol = 0;

		m_BaseDataTypeIDMap.insert(std::pair<BASEDATATYPE,unsigned long>(baseType, diatype->Elem.ID));

		//m_Globals.insert(std::pair<unsigned long, DIABASICTYPE*>(diatype->Elem.ID, (DIABASICTYPE*)diatype));
		m_Globals.Insert(diatype->Elem.ID, (DIABASICTYPE*)diatype);
		//v11 = PAIR_sub_4314C0((int)&v17, (int)&diatype->Elem.ID, (int)&diatype);
		//std_map_insert_ID2Type_sub_431180(&this->m_Globals_ID2Type_MAP, (int)v16, (int)v11);

		//a3a[0] = diatype->Elem.ID;
		//a3a[1] = (int)diatype;
		//v12 = PAIR_sub_431550((int)v15, (int)a3a, (int)a3a);
		//std_map_insert_IDType2ID_sub_430F10(&this->m_Globals_IDType2ID_SET, v14, (int)v12);

		if (Type) *Type = diatype;
		if (bRelease) *bRelease = false; //+++
 		return diatype->Elem.ID;
	}
	unsigned long CSourceDebug::ParseTypedefType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		if (Type) *Type = 0;
		if (bRelease) *bRelease = true;

		//CDiaBSTR Name;
		WCHAR *Name = nullptr;
		if (FAILED(Symbol->get_name(&Name))) return 0;

		ULONGLONG length = 0;
		if (FAILED(Symbol->get_length(&length)))
		{
			::LocalFree(Name); //+++
			return 0;
		}

		unsigned long Size = sizeof(DIATYPEDEFTYPE);
		DIATYPEDEFTYPE *diatype = (DIATYPEDEFTYPE*)new char[Size];
		elemTYPEDEF *v = &diatype->v;
		char *pName = new char[TStrLen(Name)+1];
		TStrCpy(pName, Name);
		::LocalFree(Name);

		IDiaSymbol *SymbolType = nullptr;
		if (FAILED(Symbol->get_type(&SymbolType)) || !SymbolType)
		{
			delete []pName; //+++???
			return 0;
		}

		diatype->Elem.ID = m_ElemID++;
		diatype->Elem.Size = Size;
		diatype->Value = v;
		diatype->Symbol = Symbol;
		diatype->Elem.Type = ELEM_TYPEDEF;
		diatype->Elem.Name = pName;
		diatype->Elem.Length = length;

		//v6 = PAIR_sub_4314C0((int)&v13, (int)&diatype->Elem.ID, (int)&diatype);
		//std_map_insert_ID2Type_sub_431180(&this->m_GlobalsIDTypeMap, (int)v12, (int)v6);
		m_Globals.Insert(diatype->Elem.ID, diatype);

		IDTYPE idtype;
		idtype.ID = diatype->Elem.ID;
		idtype.Type = diatype;
		m_TypedefList.push_back(idtype);

		//v7 = PAIR_sub_431550((int)v11, (int)&idtype, (int)&idtype);
		//std_map_insert_IDType2ID_sub_430F10(&this->m_Globals_IDType2ID_SET, v10, (int)v7);

		if (bRelease) *bRelease = false;

		bool release = true;
		v->ID = ParseDIASymbol(SymbolType, &release, nullptr, level);
		DIABASICTYPE *diabasetype = (DIABASICTYPE *)GetGlobal(v->ID);
		if (diabasetype)
			diatype->Elem.Length = diabasetype->Elem.Length;
		if (release)
		{
			SymbolType->Release();
			SymbolType = nullptr;
		}
		if (Type) *Type = diatype;
		return diatype->Elem.ID;
	}
	unsigned long CSourceDebug::ParseBaseClassType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);

		if (bRelease) *bRelease = true;

		std::list<IDTYPE>::iterator it = m_BaseClassList.begin();
		while (it != m_BaseClassList.end())
		{
			if (SUCCEEDED(m_Session->symsAreEquiv(Symbol, it->Type->Symbol)))
			{
				if (Type)
					*Type = it->Type;
				return it->Type->Elem.ID; //+C
			}
			++it;
		}

		//CDiaBSTR Name;
		WCHAR *Name = nullptr;
		if (FAILED(Symbol->get_name(&Name)) || !Name)
			return 0;

		unsigned long Size = sizeof(DIABASECLASSTYPE);
		DIABASECLASSTYPE *diatype = (DIABASECLASSTYPE *)new char[Size];
		elemBASECLASS *v = &diatype->v;
		memset(diatype, 0, Size);
		diatype->Elem.Size = Size;
		diatype->Elem.Type = ELEM_BASE_CLASS;
		diatype->Symbol = Symbol;
		diatype->Elem.Name = new char[TStrLen(Name)+1];
		TStrCpy(diatype->Elem.Name, Name);
		::LocalFree(Name); //+++
		diatype->Elem.ID = m_ElemID++;
		diatype->Value = v;

		IDTYPE idtype;
		idtype.ID = diatype->Elem.ID;
		idtype.Type = diatype;

		m_BaseClassList.push_back(idtype);

		//v9 = PAIR_sub_431550((int)v16, (int)&idtype, (int)&idtype);

		//std_map_insert_IDType2ID_sub_430F10(&this->m_Globals_IDType2ID_SET, v15, (int)v9);

		//v10 = PAIR_sub_4314C0((int)&v14, (int)&diatype->Elem.ID, (int)&diatype);
		//std_map_insert_ID2Type_sub_431180(&this->m_GlobalsIDTypeMap, (int)v13, (int)v10);
		m_Globals.Insert(diatype->Elem.ID, diatype);

		IDiaSymbol *SymbolType = nullptr;
		if (FAILED(Symbol->get_type(&SymbolType)) || !SymbolType) return 0;

		DIABASICTYPE *R = nullptr;
		bool release = true;
		v->ID = ParseDIASymbol(SymbolType, &release, (NULLTYPE**)&R, level);
		if (R)
			diatype->Elem.Length = R->Elem.Length;
		if (release)
		{
			SymbolType->Release();
			SymbolType = nullptr;
		}

		BOOL virtualBaseClass = 0;
		if (FAILED(Symbol->get_virtualBaseClass(&virtualBaseClass))) return 0;
		v->VirtualBaseClass = virtualBaseClass != 0;

		BOOL indirectVirtualBaseClass = 0;
		if (FAILED(Symbol->get_indirectVirtualBaseClass(&indirectVirtualBaseClass))) return 0;
		v->IndirectVirtualBaseClass = indirectVirtualBaseClass != 0;

		LONG Offset = -1;
		if (FAILED(Symbol->get_offset(&Offset))) return 0;
		v->Offset = Offset;

		DWORD virtualBaseDispIndex = -1;
		if (FAILED(Symbol->get_virtualBaseDispIndex(&virtualBaseDispIndex))) return 0;
		v->VirtualBaseDispIndex = virtualBaseDispIndex;


		LONG virtualBasePointerOffset = 0; //???
		if (FAILED(Symbol->get_virtualBasePointerOffset(&virtualBasePointerOffset))) return 0;

		DWORD access = -1;
		if (FAILED(Symbol->get_access(&access))) return 0;
		v->Access = access;

		v->VirtualBasePointerOffset = virtualBasePointerOffset;

		if (bRelease) *bRelease = false;
		return diatype->Elem.ID;
	}

	unsigned long CSourceDebug::ParseFriend(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		Trace trace(level, __func__);
//printf("%s -------------------------------!!!!!!!!!!!!!!!!!!!",__func__);
		//TODO
		return 0;
	}
	unsigned long CSourceDebug::ParseThunk(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		Trace trace(level, __func__);

		if (Type) *Type = 0;
		if (bRelease) *bRelease = true;
		DWORD rva = 0;
		if (FAILED(Symbol->get_relativeVirtualAddress(&rva))) return 0;
		map<unsigned long, DIABASICTYPE*>::IT it = m_ThunkRVATypeMap.find(rva);
		if (it != m_ThunkRVATypeMap.end())
		{
			if (Type) *Type = it->second;
			return it->second->Elem.ID;
		}

		//CDiaBSTR Name;
		WCHAR *Name = nullptr;
		if (FAILED(Symbol->get_name(&Name)) || !Name) return 0;

		unsigned long Size = sizeof(DIATHUNK);
		DIATHUNK *diatype = (DIATHUNK *)new char[Size];
		memset(diatype, 0, Size);
		elemTHUNK *v = &diatype->v;
		diatype->Elem.Name = new char[TStrLen(Name)+1];
		TStrCpy(diatype->Elem.Name, Name);
		::LocalFree(Name);
		diatype->Elem.Size = Size;
		ULONGLONG length = 0;
		Symbol->get_length(&length);
		Symbol->get_thunkOrdinal(&v->thunkOrdinal);
		diatype->Elem.ID = m_ElemID++;

		//v7 = PAIR_sub_4314C0((int)&v16, (int)&diatype->Elem.ID, (int)&diatype);
		//std_map_insert_ID2Type_sub_431180(&this->m_GlobalsIDTypeMap, (int)v15, (int)v7);
		m_Globals.Insert(diatype->Elem.ID, diatype);

		IDTYPE idtype;
		idtype.ID = diatype->Elem.ID;
		idtype.Type = diatype;
		//v8 = PAIR_sub_431550((int)v14, (int)&idtype, (int)&idtype);
		//std_map_insert_IDType2ID_sub_430F10(&this->m_Globals_IDType2ID_SET, v13, (int)v8);

		//v9 = PAIR_sub_4314C0((int)&v12, (int)&rva, (int)&diatype);
		//std_map_insert_ID2Type_sub_431180(&this->m_ThunkRVATypeMap, (int)v11, (int)v9);
		m_ThunkRVATypeMap.Insert(rva, diatype);

		v->rva = rva;
		diatype->Elem.Length = length;
		diatype->Elem.Type = ELEM_THUNK;
		if (bRelease) *bRelease = false; //+++
		return diatype->Elem.ID;
	}
	unsigned long CSourceDebug::ParseCustomType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		Trace trace(level, __func__);

		if (Type) *Type = 0;
		if (bRelease) *bRelease = true;
//printf("%s -------------------------------!!!!!!!!!!!!!!!!!!!",__func__);
		//TODO
		return 0;
	}
	unsigned long CSourceDebug::ParseManagedType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		Trace trace(level, __func__);

		if (Type) *Type = 0;
		if (bRelease) *bRelease = true;
//printf("%s -------------------------------!!!!!!!!!!!!!!!!!!!",__func__);
		//TODO
		return 0;
	}

	unsigned long CSourceDebug::ParseDIASymbol(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long level)
	{
		++level;
		//Trace trace(level, __func__);
#if 0
#ifdef TXTDEBUG
{
	for (int l=0;l<n;++l)
	{
fprintf(f," ");
fflush(f);
	}
}
fprintf(f,"%s\n", __func__);
fflush(f);
#endif
#endif
		bool release = true;
		unsigned long ID = 0;

		if (Type) *Type = 0;

		if (bRelease) *bRelease = true;

		if (!Symbol)
			return 0;

		DWORD symTag = 0;
		if (FAILED(Symbol->get_symTag(&symTag)))
			return 0;

		DWORD dataKind = 0;

		switch (symTag)
		{
		case SymTagFunction:
			ID = ParseFunction(Symbol, &release, Type, level);
			break;
		case SymTagData:
			if (SUCCEEDED(Symbol->get_dataKind(&dataKind)) &&
				(dataKind == DataIsFileStatic ||
				 dataKind == DataIsGlobal))
			{
				//ID = ???
				ParseData(Symbol, &release, Type, level);
			}
			break;
		case SymTagUDT:
			ID = ParseUDType(Symbol, &release, Type, level);
			break;
		case SymTagEnum:
			ID = ParseEnumType(Symbol, &release, Type, level);
			break;
		case SymTagFunctionType:
			ID = ParseFunctionType(Symbol, &release, Type, level);
			break;
		case SymTagPointerType:
			ID = ParsePointerBase(Symbol, &release, Type, level);
			break;
		case SymTagArrayType:
			ID = ParseArrayType(Symbol, &release, Type, level);
			break;
		case SymTagBaseType:
			ID = ParseBaseType(Symbol, &release, Type, level);
			break;
		case SymTagTypedef:
			ID = ParseTypedefType(Symbol, &release, Type, level);
			break;
		case SymTagBaseClass:
			ID = ParseBaseClassType(Symbol, &release, Type, level);
			break;
		case SymTagFriend:
			ID = ParseFriend(Symbol, &release, Type, level);
			break;
		case SymTagThunk:
			ID = ParseThunk(Symbol, &release, Type, level);
			break;
		case SymTagCustomType:
			ID = ParseCustomType(Symbol, &release, Type, level);
			break;
		case SymTagManagedType:
			ID = ParseManagedType(Symbol, &release, Type, level);
			break;
		default:
			break;
		}
		if (bRelease) *bRelease = release;
		return ID;
	}
	
	bool CSourceDebug::ReadFILESIG(const char *FullFileName, unsigned long *TimeStamp, unsigned long *CheckSum)
	{
		CPEFile PEFile;
		if (TimeStamp) *TimeStamp = 0;
		if (CheckSum) *CheckSum = 0;

		PEFile.m_OpenMode |= 0x30; //???
		if (!PEFile.Open(FullFileName, 0))
			return false;
		if (TimeStamp) *TimeStamp = PEFile.m_PEHead.TimeDateStamp;
		if (CheckSum) *CheckSum = PEFile.m_PEHead.CheckSum;
		PEFile.Close();
		return true;
	}

	bool CSourceDebug::LoadPESym(const char *FullFileName)
	{
		CPEFile PEFile;
		if (!PEFile.Open(FullFileName, 0))
		{
			return false;
		}
		PUBSYM symbol;
		memset(&symbol, 0, sizeof(symbol));
		symbol.options |= 0x8'0000; //<<19
		for (int n = 0; n < PEFile.m_ImportModuleCount; ++n)
		{
			char *Name = nullptr;
			IMPORT_MODULE *pImportModule = &PEFile.m_ImportModule[n];
			TListIter<char*> It = m_ImportModules.Find(PEFile.m_ImportModule[n].ModuleName); //char_compare
			if (It == m_ImportModules.End())
			{
				Name = new char[TStrLen(pImportModule->ModuleName)+1];
				TStrCpy(Name, pImportModule->ModuleName);
				m_ImportModules.Append(&Name);
			} else
			{
				Name = *It;
			}

			for (int m = 0; m < pImportModule->FuncCount; ++m)
			{
				symbol.rva = pImportModule->FuncList[m].ThunkAddr - PEFile.m_PEHead.ImageBase;
				set<PUBSYM>::IT it = m_PublicSymbols.find(symbol);
				if (it == m_PublicSymbols.end())
				{
					char Buffer[1024];
					TSPrintf(Buffer, "Import_%s_%s", pImportModule->ModuleName, pImportModule->FuncList[m].FuncName);
					symbol.name = new char[TStrLen(Buffer)+1];
					TStrCpy(symbol.name, Buffer);
					symbol.length = 0;
					symbol.moduleName = Name;
					m_PublicSymbols.insert(symbol);
				} else
				{
					it->options |= 0x8'0000; //??? //<<19
					it->moduleName = Name; //???
				}
			}
		}

		symbol.options &= ~0x8'0000; //<<19
		symbol.options |= 0x4'0000; //<<18
		symbol.moduleName = nullptr;

		for (int n = 0; n < PEFile.m_ExportFuncCount; ++n)
		{
			symbol.rva = PEFile.m_ExportFunc[n].Address - PEFile.m_PEHead.ImageBase;
			set<PUBSYM>::IT it = m_PublicSymbols.find(symbol);
			if (it == m_PublicSymbols.end())
			{
				char Buffer[1024];
				TStrCpy(Buffer, PEFile.m_ExportFunc[n].FuncName);
				symbol.name = new char[TStrLen(Buffer)+1];
				TStrCpy(symbol.name, Buffer);
				symbol.length = 0;
				m_PublicSymbols.insert(symbol);
			} else
			{
				it->options |= 0x4'0000; //??? //<<18
			}
		}
		PEFile.Close();
		return true;
	}

	bool CSourceDebug::AddSources(const char *SDSFullFileName, UseSourcesEnum UseSources, int (CbProgress)(char *, char *))
	{
		char szNewFileName[1028];
		bool SkipAll = false;
		unsigned long nFiles = 0;
		//CLocalFileIO LocalIO; //NOT USES
		void *Buffer = nullptr;
		
		if (UseSources == eUseSourcesNone)
			return false;

		HANDLE Handle;
		if (!gpFileIO->OpenFile(SDSFullFileName, &Handle, 1))
			return false;

		unsigned long SDSFileSize = gpFileIO->GetFileLength(Handle);
		SDSFILE *SDSFile = (SDSFILE *)new char[SDSFileSize];
		gpFileIO->ReadFile(Handle, SDSFile, SDSFileSize, 0);
		if (SDSFile->Sources)
		{
			delete [](char*)SDSFile;
			gpFileIO->CloseHandle(Handle);
			return false;
		}

		ULONG_PTR *StringIndexes = (ULONG_PTR*)((char *)SDSFile + SDSFile->StringIndexes);
		char *StringsEntry = (char*)((char *)SDSFile + SDSFile->StringsEntry);
		gpFileIO->SetFilePointer(Handle, SDSFile->SrcFileNames, FILE_BEGIN);
		SrcFileNameID *SrcFileName = (SrcFileNameID *)((char *)SDSFile + SDSFile->SrcFileNames);
		for (unsigned long n = 0; n < SDSFile->SrcFileNamesLen; ++n)
		{
			char *pModuleFileName = (char *)StringsEntry + StringIndexes[(unsigned long)SrcFileName[n].FileName];
			bool FileExist = gpFileIO->IsFileExist(pModuleFileName);
			do {
				if (FileExist)
				{
					++nFiles;
					unsigned long FileSize = gpFileIO->GetFileSize(pModuleFileName);
					unsigned long SrcPos = 0;
					if (UseSources != eUseSourcesExtern)
					{
						SrcPos = gpFileIO->SetFilePointer(Handle, 0, FILE_END);
						char *Buffer = new char[FileSize];
						gpFileIO->ReadFromFile(pModuleFileName, Buffer, FileSize);
						gpFileIO->WriteFile(Handle, Buffer, FileSize);
						delete []Buffer;
					}
					SrcFileName[n].Text = (char*)SrcPos;
					SrcFileName[n].FileSize = FileSize;
					break;
				}
				SrcFileName[n].Text = 0;
				SrcFileName[n].FileSize = 0;

				if (!CbProgress || SkipAll) break;
				
				int ProgressResult = CbProgress(pModuleFileName, szNewFileName);
				if (ProgressResult)
				{
					if (ProgressResult == 2)
						SkipAll = true;
					break;
				}
				pModuleFileName = szNewFileName;
				FileExist = gpFileIO->IsFileExist(pModuleFileName);
			} while (FileExist);
		}

		
		gpFileIO->WriteFile(Handle, SrcFileName, SDSFile->SrcFileNamesLen*sizeof(*SrcFileName), SDSFile->SrcFileNames);

		unsigned long End = gpFileIO->SetFilePointer(Handle, 0, FILE_END);
		unsigned long Size = End & 0xFFF;
		if (Size)
		{
			Size = 4096 - Size; // PAGE_SIZE
			char *p = new char[Size];
			if (p)
			{
				memset(p, 0, Size);
				gpFileIO->WriteFile(Handle, p, Size);
				End = gpFileIO->SetFilePointer(Handle, 0, FILE_END);
				delete []p;
			} else
			{
				SDSFile->CRCSUM = 0;
			}
		}

		SDSFile->Sources = SDSFile->FileSize;
		SDSFile->FileSize = End;
		SDSFile->SourcesCount = nFiles;
		gpFileIO->WriteFile(Handle, SDSFile, SDSFILE_SIZE, 0);
		delete [](char*)SDSFile;
		gpFileIO->CloseHandle(Handle);

		WriteSDSCheckSum(SDSFullFileName);		
		return true;
	}
#endif
	bool CSourceDebug::IsExist(const char *FullFileName, bool *LoadFromPDBOK)
	{
		bool result = OpenDIA(FullFileName, &m_DataSource, &m_Session, &m_GlobalSymbol, LoadFromPDBOK);
		CloseDIA();
		return result;
	}

	bool CSourceDebug::CheckSDS(const char *FullFileName, char *NewFullFileName)
	{
		char SDSFullFileName[268]; //MAX_FN_LEN+8
		TStrCpy(SDSFullFileName, FullFileName);
		TStrCat(SDSFullFileName, ".sds");
		if (NewFullFileName)
			TStrCpy(NewFullFileName, SDSFullFileName);

		if (!gpFileIO->IsFileExist(SDSFullFileName))
			return false;

		unsigned long TimeStamp;
		unsigned long CheckSum;

		SDSFILE SDSFile;

		ReadFILESIG(FullFileName, &TimeStamp, &CheckSum);

		if (gpFileIO->ReadFromFile(SDSFullFileName, &SDSFile, SDSFILE_SIZE) != SDSFILE_SIZE)
			return false;

		if (gpFileIO->GetFileSize(SDSFullFileName) != SDSFile.FileSize)
			return false;

		if (SDSFile.TimeStamp != TimeStamp)
			return false;

		if (SDSFile.Version != SDSFILE_VERSION)
			return false;

		if (SDSFile.CheckSum != CheckSum)
			return false;

		return true;
	}

	bool CSourceDebug::Load(const char *FullFileName, char *NewFullFileName, UseSourcesEnum UseSources)
	{
#ifndef _RING0_
		unsigned long TimeDateStamp = 0;
		unsigned long CheckSum = 0;

		if (!gpFileIO->IsFileExist(FullFileName))
			return false;

		char SDSFullFileName[268]; //MAX_FN_LEN+8
		TStrCpy(SDSFullFileName, FullFileName);
		TStrCat(SDSFullFileName, ".sds");

		if (NewFullFileName)
			TStrCpy(NewFullFileName, SDSFullFileName);

		bool LoadFromPDBOK;
		if (!OpenDIA(FullFileName, &m_DataSource, &m_Session, &m_GlobalSymbol, &LoadFromPDBOK))
		{
			CloseDIA();
			return false;
		}

		bool WriteOK = false;

		//LoadFromPDBOK = false; //HACK ???

		if (Parse(LoadFromPDBOK))
		{
			if (LoadFromPDBOK)
			{
				TStrCpy(SDSFullFileName, TGetFileName(SDSFullFileName));
				if (NewFullFileName) TStrCpy(NewFullFileName, SDSFullFileName);//FullFileName = nullptr;
			} else	LoadPESym(FullFileName);

			//WriteDump("c:\\ntddkvista.h");
			if (LoadFromPDBOK)
			{
				TimeDateStamp = 1;
				CheckSum = 1;
			} else
			{
				ReadFILESIG(FullFileName, &TimeDateStamp, &CheckSum);
			}
			m_TimeStamp = TimeDateStamp;
			m_CheckSum = CheckSum;
			//printf("File %s, TimeStamp %d, CheckSum %d\n", FullFileName, m_TimeStamp, m_CheckSum);
			WriteOK = WriteSDS(SDSFullFileName, TimeDateStamp, CheckSum, FullFileName);
		}
		Release();
		CloseDIA();
		if (WriteOK && UseSources != eUseSourcesNone)
			AddSources(SDSFullFileName, UseSources, m_DialogAppendSrcFilename);
		return WriteOK;
#else
		return false;
#endif
	}
//#endif

	bool CSourceDebug::WriteSDSCheckSum(const char *SDSFullFileName)
	{
		HANDLE Handle;
		if (!gpFileIO->OpenFile(SDSFullFileName, &Handle, 1))
			return false;
		unsigned long Len = gpFileIO->GetFileLength(Handle);
		if (Len & 0xFFF)
		{
			gpFileIO->CloseHandle(Handle);
			return false;
		}
		SDSFILE *SDSFile = (SDSFILE *)new char[Len];
		if (!SDSFile)
		{
			gpFileIO->CloseHandle(Handle);
			return false;
		}
		gpFileIO->SetFilePointer(Handle, 0, FILE_CURRENT);
		if (gpFileIO->ReadFile(Handle, SDSFile, Len, 0) != Len)
		{
			delete [](char*)SDSFile;
			gpFileIO->CloseHandle(Handle);
			return false;
		}
		SDSFile->CRCSUM = 0;
		unsigned long long CheckSum = 0;
		for (unsigned long n = 0; n < Len/4; ++n)
			CheckSum += ((unsigned long*)SDSFile)[n];
		SDSFile->CRCSUM = CheckSum;
		gpFileIO->WriteFile(Handle, SDSFile, SDSFILE_SIZE, 0);
		gpFileIO->FlushFile(Handle);
		gpFileIO->CloseHandle(Handle);
		delete [](char*)SDSFile;
		return true;
	}

	void CSourceDebug::WriteAlign(void *Buffer, unsigned long Size)
	{
		unsigned long ModSize = m_nCurPos % Size;
		if (ModSize)
		{
			ModSize = Size - ModSize;
			gpFileIO->WriteFile(m_Handle, Buffer, ModSize);
		}
		m_nCurPos += ModSize;
	}

	unsigned long CSourceDebug::WriteLocals(DATATYPE *data)
	{
		char *name = data->name;
		data->name = (char *)PackString(data);
		gpFileIO->WriteFile(m_Handle, data, sizeof(*data));
		data->name = name;
		return sizeof(*data);
	}

	unsigned long CSourceDebug::WriteFunction(FUNCTION *FUNC)
	{
		FUNCTION *Func = new FUNCTION;
		*Func = *FUNC;
		unsigned long Size = sizeof(*Func);
		Func->stack = (unsigned long*)(Func->nstack!=0 ? Size : 0);  //TODO X64
		Size += Func->nstack*sizeof(*Func->stack);
		Func->blocks = (unsigned long*)(Func->nblocks!=0 ? Size : 0);  //TODO X64
		Size += Func->nblocks*sizeof(*Func->blocks);

		Func->name = (char *)PackString(Func->name);
		gpFileIO->WriteFile(m_Handle, Func, sizeof(*Func));
		if (Func->nstack)
			gpFileIO->WriteFile(m_Handle, FUNC->stack, Func->nstack*sizeof(*Func->stack));
		if (Func->nblocks)
			gpFileIO->WriteFile(m_Handle, FUNC->blocks, Func->nblocks*sizeof(*Func->blocks));
		if (Func->nlabels)
		{
			FUNCTIONLABEL *Labels = new FUNCTIONLABEL[Func->nlabels];
			for (unsigned long n = 0; n < Func->nlabels; ++n)
			{
				Labels[n].Name = (char*)PackString(Func->labels[n].Name);
//printf("label %d(%x) %s\n", Labels[n].Name, Labels[n].Name, Func->labels[n].Name);
				Labels[n].Offset = Func->labels[n].Offset;
			}
			Size += Func->nlabels*sizeof(*Labels);
			gpFileIO->WriteFile(m_Handle, Labels, Func->nlabels*sizeof(*Labels));
			delete []Labels;
		}
		delete Func;
		return Size;
	}

	unsigned long CSourceDebug::WriteFilesLNs(unsigned long *Count)
	{
		FilesLN *Files = new FilesLN[m_CompilandFilesLN.size()];
		memset(Files, 0, m_CompilandFilesLN.size()*sizeof(*Files));
		m_CompilandFilesLN.sort(); //sort by address
		unsigned long n = 0;
		std::list<FilesLN>::iterator it = m_CompilandFilesLN.begin();
		while (it != m_CompilandFilesLN.end())
		{
			Files[n] = *it;
			++n;
			++it;
		}
		gpFileIO->WriteFile(m_Handle, Files, n*sizeof(*Files));
		if (Count)
			*Count = n;
		delete []Files;
		return n*sizeof(*Files);
	}

	unsigned long CSourceDebug::WriteSrcFileNames(unsigned long *Count)
	{
		SrcFileNameID *SrcFileNames = new SrcFileNameID[m_CompilandSrcMap.size()];
		memset(SrcFileNames, 0, m_CompilandSrcMap.size()*sizeof(*SrcFileNames));
		unsigned long n = 0;
		for (map<char*,SrcFileNameID, MY_char_compare>::IT it = m_CompilandSrcMap.begin();
			it != m_CompilandSrcMap.end(); ++it)
		{
			SrcFileNames[n].FileName = (char*)PackString(it->first);
			SrcFileNames[n].FileID = it->second.FileID;
		#if 0 //+++
			SrcFileNames[n].FileSize = it->second.FileSize; //TODO update from add sources
		#endif
			++n;
		}
		gpFileIO->WriteFile(m_Handle, SrcFileNames, n*sizeof(*SrcFileNames));
		if (Count)
			*Count = n;
		delete []SrcFileNames;
		return n*sizeof(*SrcFileNames);
	}

	unsigned long CSourceDebug::WritePublicSymbols(unsigned long *Count)
	{
		PUBSYM *PublicSymbols = new PUBSYM[m_PublicSymbols.size()];
		memset(PublicSymbols, 0, m_PublicSymbols.size()*sizeof(*PublicSymbols));
		unsigned long nstr = 0;
		unsigned long n = 0;
		set<PUBSYM>::IT it = m_PublicSymbols.begin();
		while (it != m_PublicSymbols.end())
		{
			PublicSymbols[n] = *it;
			if (!TStrCmp("`string'", PublicSymbols[n].name))
			{
				if (m_HasStringPublicSymbol)
				{
					++nstr;
					PublicSymbols[n].name = m_StringPublicSymbol;
				} else
				{
					PublicSymbols[n].name = (char *)PackString(PublicSymbols[n].name);
					m_HasStringPublicSymbol = true;
					m_StringPublicSymbol = PublicSymbols[n].name;
				}
			} else
			{
				PublicSymbols[n].name = (char *)PackString(PublicSymbols[n].name);
			}
			PublicSymbols[n].moduleName = (char*)PackString(PublicSymbols[n].moduleName);
			PUBSYM *pubsym = PublicSymbols;
			++n;
			++it;
		}
		gpFileIO->WriteFile(m_Handle, PublicSymbols, n*sizeof(*PublicSymbols));
		if (Count)
			*Count = n;
		delete []PublicSymbols;
		return n*sizeof(*PublicSymbols);
	}

	unsigned long CSourceDebug::WriteCompilandUnits(unsigned long *Count)
	{
		COMPUNIT *CompUnits = new COMPUNIT[m_CompilandUnitMap.size()];
		memset(CompUnits, 0, m_CompilandUnitMap.size()*sizeof(*CompUnits));
		unsigned long n = 0;
		for (map<char*,COMPUNIT*, MY_char_compare>::IT it = m_CompilandUnitMap.begin();
			it != m_CompilandUnitMap.end(); ++it)
		{
			CompUnits[n] = *it->second;
			CompUnits[n].UnitName = (char*)PackString(CompUnits[n].UnitName);
			++n;
		}
		gpFileIO->WriteFile(m_Handle, CompUnits, n*sizeof(*CompUnits));
		if (Count)
			*Count = n;
		delete []CompUnits;
		return n*sizeof(*CompUnits);
	}

	unsigned long CSourceDebug::WriteBaseType(DIABASICTYPE *Type)
	{
		char *Name = Type->Elem.Name;
		Type->Elem.Name = (char *)PackString(Type);
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		gpFileIO->WriteFile(m_Handle, &Type->Elem, Size);
		Type->Elem.Name = Name;
		return Size;
	}

	unsigned long CSourceDebug::WritePointer(DIAPOINTERTYPE *Type)
	{
		char *Name = Type->Elem.Name;
		int HidePos = TStrLen(Name) - 9; //???
		int NameLen = TStrLen(Name);
		char *Buffer = new char[NameLen + Type->v.Num + 1];
		TStrCpy(Buffer, Name);
		unsigned long n = 0;
		for (n = 0; n < Type->v.Num; ++n)
			Buffer[n+HidePos] = '*';
		Buffer[n+HidePos] = 0;
		Type->Elem.Name = (char *)PackString(Buffer);
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		gpFileIO->WriteFile(m_Handle, &Type->Elem, Size);
		delete []Buffer;
		Type->Elem.Name = Name;
		return Size;
	}

	unsigned long CSourceDebug::WritePointerRef(DIABASICTYPE *Type)
	{
		char *Name = Type->Elem.Name;
		Type->Elem.Name = (char *)PackString(Type);
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		gpFileIO->WriteFile(m_Handle, &Type->Elem, Size);
		Type->Elem.Name = Name;
		return Size;
	}

	unsigned long CSourceDebug::WriteArray(DIABASICTYPE *Type)
	{
		char *Name = Type->Elem.Name;
		Type->Elem.Name = (char *)PackString(Type);
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		gpFileIO->WriteFile(m_Handle, &Type->Elem, Size);
		Type->Elem.Name = Name;
		return Size;
	}

	unsigned long CSourceDebug::WriteManaged(DIABASICTYPE *Type)
	{
		char *Name = Type->Elem.Name;
		Type->Elem.Name = (char *)PackString(Type);
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		gpFileIO->WriteFile(m_Handle, &Type->Elem, Size);
		Type->Elem.Name = Name;
		return Size;
	}

	unsigned long CSourceDebug::WriteCustom(DIABASICTYPE *Type)	
	{
		char *Name = Type->Elem.Name;
		Type->Elem.Name = (char *)PackString(Type);
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		gpFileIO->WriteFile(m_Handle, &Type->Elem, Size);
		Type->Elem.Name = Name;
		return Size;
	}

	unsigned long CSourceDebug::WriteEnum(DIABASICTYPE *Type)
	{
		char *Name = Type->Elem.Name;
		Type->Elem.Name = (char *)PackString(Type);
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);

		ENUMTYPE *enumtype = (ENUMTYPE*)new char[Size];
		memcpy(enumtype, &Type->Elem, Size);
		for (unsigned long n = 0; n < enumtype->Length; ++n)
			enumtype->Values[n].Name = (char*)PackString(enumtype->Values[n].Name);

		gpFileIO->WriteFile(m_Handle, enumtype, Size);
		delete [](char*)enumtype;

		Type->Elem.Name = Name;
		return Size;
	}

	unsigned long CSourceDebug::WriteUDT(DIAUDTYPE *Type)
	{
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		DIAUDTYPE *udtype = (DIAUDTYPE *)new char [Type->Elem.Size];
		memcpy(udtype, Type, Type->Elem.Size);

		udtype->Elem.Name = (char *)PackString(Type);
		udtype->v.Data = (unsigned long*)Size; //TODO X64
		Size += udtype->v.DataLen*sizeof(*udtype->v.Data);
		udtype->v.Function = (unsigned long*)Size; //TODO X64
		Size += udtype->v.FunctionLen*sizeof(*udtype->v.Function);
		udtype->v.Friend = (unsigned long*)Size; //TODO X64
		Size += udtype->v.FriendLen*sizeof(*udtype->v.Friend);
		udtype->v.BaseClass = (unsigned long*)Size; //TODO X64
		Size += udtype->v.BaseClassLen*sizeof(*udtype->v.BaseClass);
		udtype->v.PubSymbols = (CLASSPUBSYM*)Size; //TODO X64
		Size += udtype->v.PubSymbolsLen*sizeof(*udtype->v.PubSymbols);

		gpFileIO->WriteFile(m_Handle, &udtype->Elem, Type->Elem.Size - sizeof(DIASYMBOL));
		if (udtype->v.DataLen)
			gpFileIO->WriteFile(m_Handle, Type->v.Data, udtype->v.DataLen*sizeof(*udtype->v.Data));
		if (udtype->v.FunctionLen)
			gpFileIO->WriteFile(m_Handle, Type->v.Function, udtype->v.FunctionLen*sizeof(*udtype->v.Function));
		if (udtype->v.FriendLen)
			gpFileIO->WriteFile(m_Handle, Type->v.Friend, udtype->v.FriendLen*sizeof(*udtype->v.Friend));
		if (udtype->v.BaseClassLen)
			gpFileIO->WriteFile(m_Handle, Type->v.BaseClass, udtype->v.BaseClassLen*sizeof(*udtype->v.BaseClass));
		if (udtype->v.PubSymbolsLen)
			gpFileIO->WriteFile(m_Handle, Type->v.PubSymbols, udtype->v.PubSymbolsLen*sizeof(*udtype->v.PubSymbols));

		delete [](char*)udtype;
		return Size;
	}

	unsigned long CSourceDebug::WriteTypeDef(DIABASICTYPE *Type)
	{
		char *Name = Type->Elem.Name;
		Type->Elem.Name = (char *)PackString(Type);
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		gpFileIO->WriteFile(m_Handle, &Type->Elem, Size);
		Type->Elem.Name = Name;
		return Size;
	}

	unsigned long CSourceDebug::WriteFunction(DIABASICTYPE *Type)
	{
		char *Name = Type->Elem.Name;
		Type->Elem.Name = (char *)PackString(Type);
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		gpFileIO->WriteFile(m_Handle, &Type->Elem, Size);
		Type->Elem.Name = Name;
		return Size;
	}

	unsigned long CSourceDebug::WriteBaseClass(DIABASICTYPE *Type)
	{
		char *Name = Type->Elem.Name;
		Type->Elem.Name = (char *)PackString(Type);
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		gpFileIO->WriteFile(m_Handle, &Type->Elem, Size);
		Type->Elem.Name = Name;
		return Size;
	}

	unsigned long CSourceDebug::WriteFriend(DIABASICTYPE *Type)
	{
		char *Name = Type->Elem.Name;
		Type->Elem.Name = (char *)PackString(Type);
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		gpFileIO->WriteFile(m_Handle, &Type->Elem, Size);
		Type->Elem.Name = Name;
		return Size;
	}

	unsigned long CSourceDebug::WriteVTable(DIABASICTYPE *Type)
	{
		char *Name = Type->Elem.Name;
		Type->Elem.Name = (char *)PackString(Type);
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		gpFileIO->WriteFile(m_Handle, &Type->Elem, Size);
		Type->Elem.Name = Name;
		return Size;
	}

	unsigned long CSourceDebug::WriteThunk(DIABASICTYPE *Type)
	{
		char *Name = Type->Elem.Name;
		Type->Elem.Name = (char *)PackString(Type);
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		gpFileIO->WriteFile(m_Handle, &Type->Elem, Size);
		Type->Elem.Name = Name;
		return Size;
	}

	unsigned long CSourceDebug::WriteBlock(DIABASICTYPE *Type)
	{
		unsigned long Size = Type->Elem.Size - sizeof(DIASYMBOL);
		gpFileIO->WriteFile(m_Handle, &Type->Elem, Size);
		return Size;
	}

	bool CSourceDebug::WriteSDS(const char *SDSFullFileName, unsigned long TimeStamp, unsigned long CheckSum, const char *FullFileName)
	{
		char *EmptyPage = new char[PAGE_SIZE];
		memset(EmptyPage, 0, PAGE_SIZE);

		m_StringsHead = 0;
		m_StringsTail = STRINGS_CHUNK_SIZE;
		m_StringsBuf = new char[m_StringsTail];
		memset(m_StringsBuf, 0, m_StringsTail);
		m_StringsCur = 1;
		m_StringsID = 1;

		SDSFILE *SDSFile = (SDSFILE*)new char[SDSFILE_SIZE];
		memset(SDSFile, 0, sizeof(SDSFILE));
		SDSFile->Magic = SDSFILE_MAGIC;
		SDSFile->SUB = 20;
		SDSFile->Version = SDSFILE_VERSION;
		SDSFile->HeaderSize = SDSFILE_SIZE;
		SDSFile->TimeStamp = TimeStamp;
		SDSFile->CheckSum = CheckSum;

		if (!gpFileIO->CreateFile(SDSFullFileName, &m_Handle, 0x10001))
		{
			delete []EmptyPage;
			return false;
		}

		gpFileIO->WriteFile(m_Handle, SDSFile, SDSFILE_SIZE);
		m_nCurPos = SDSFILE_SIZE;

		SDSFile->GlobalsLen = m_Globals.size();
		++SDSFile->GlobalsLen;

		ULONG_PTR *Globals = new ULONG_PTR[SDSFile->GlobalsLen];
		memset(Globals, 0, SDSFile->GlobalsLen*sizeof(*Globals));

		Globals[0] = m_nCurPos;
		//COMMONTYPE
		DIABASICTYPE NullBaseType; //????? TODO MAX SIZE OF ELEM_TYPE
		memset(&NullBaseType, 0, sizeof(NullBaseType));
		//NullBaseType._basic.Elem.Name = ""; //+++
		//NullBaseType._basic.Elem.Name = (char*)PackString(NullBaseType._basic.Elem.Name); //+++
		gpFileIO->WriteFile(m_Handle, &NullBaseType, sizeof(NullBaseType));
		m_nCurPos += sizeof(NullBaseType);

		m_StringIndexes.push_back(0);

		//{ GLOBALS
		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
	{
		unsigned long n = 1;
		for (map<unsigned long, DIABASICTYPE*>::IT it = m_Globals.begin();
			it != m_Globals.end(); ++it)
		{
			unsigned long Size = 0;
			Globals[n] = m_nCurPos;
			if (n != it->first)
			{
				//MessageBoxA(0, "Sort error", "error", 0);
#ifdef TXTDEBUG
fprintf(f,"%s n %d != Type %d\n", __func__, n, it->second->Elem.Type);
fflush(f);
#endif
		printf("OutOfOrder! %d m_nCurPos %d(%x) Size %d, Type %d, Name %s\n",
			n, m_nCurPos, m_nCurPos, Size, it->second->Elem.Type, it->second->Elem.Name?it->second->Elem.Name:"");

			}

			switch (it->second->Elem.Type)
			{
			case ELEM_BASE_TYPE:{
				Size = WriteBaseType(it->second);
				} break;
			case ELEM_POINTER:{
				Size = WritePointer((DIAPOINTERTYPE *)it->second);
				} break;
			case ELEM_POINTERREF:{
				Size = WritePointerRef(it->second);
				} break;
			case ELEM_ARRAY:{
				Size = WriteArray(it->second);
				} break;
			case ELEM_MANAGED_TYPE:{
				Size = WriteManaged(it->second);
				} break;
			case ELEM_CUSTOM_TYPE:{
				Size = WriteCustom(it->second);
				} break;
			case ELEM_ENUM:{
				Size = WriteEnum(it->second);
				} break;
			case ELEM_STRUCT:
			case ELEM_CLASS:
			case ELEM_UNION:{
				Size = WriteUDT((DIAUDTYPE *)it->second);
				} break;
			case ELEM_TYPEDEF:{
				Size = WriteTypeDef(it->second);
				} break;
			case ELEM_FUNC_TYPE:{
				Size = WriteFunction(it->second);
				} break;
			case ELEM_BASE_CLASS:{
				Size = WriteBaseClass(it->second);
				} break;
			case ELEM_FRIEND:{
				Size = WriteFriend(it->second);
				} break;
			case ELEM_VTABLE:{
				Size = WriteVTable(it->second);
				} break;
			case ELEM_THUNK:{
				Size = WriteThunk(it->second);
				} break;
			case ELEM_BLOCK:{
				Size = WriteBlock(it->second);
				} break;
			default:;
				//Globals[n] = Globals[0]; //+++ SDSFILE_SIZE;
			}
	#if 0
		printf("%d m_nCurPos %d(%x) Size %d, Type %d, Name %s\n",
			n, m_nCurPos, m_nCurPos, Size, it->second->Elem.Type, it->second->Elem.Name?it->second->Elem.Name:"");
	#endif
			if (Size == 0)
				Globals[n] = Globals[0]; //+++ SDSFILE_SIZE;
			m_nCurPos += Size;
			gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
			++n;
		}
	}
		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
		// Globals size
		WriteAlign(EmptyPage, 16);
		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
		SDSFile->Globals = m_nCurPos;
		gpFileIO->WriteFile(m_Handle, Globals, SDSFile->GlobalsLen*sizeof(*Globals));
		delete []Globals;
		m_nCurPos += SDSFile->GlobalsLen*sizeof(*Globals);
		WriteAlign(EmptyPage, 16);
		//}

		//{ LOCALS
		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
		SDSFile->Locals = m_nCurPos;
		m_nCurPos += sizeof(DATATYPE);
		DATATYPE NullLocal;
		memset(&NullLocal, 0, sizeof(NullLocal));
		//NullLocal.name = "";
		//NullLocal.name = (char*)PackString(NullLocal.name);
		gpFileIO->WriteFile(m_Handle, &NullLocal, sizeof(NullLocal));
		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);

		for (unsigned long n = 1; n < m_Locals.size(); ++n)
		{
	#if 0
		printf("%d local m_nCurPos %d(%x) Size %d, Type %d, Name %s\n",
			n, m_nCurPos, m_nCurPos, sizeof(DATATYPE), m_Locals[n]->ID, m_Locals[n]->name);
	#endif
			m_nCurPos += WriteLocals(m_Locals[n]);
			gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
		}
		SDSFile->LocalsLen = m_Locals.size();

		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
		WriteAlign(EmptyPage, 16);
		//}

		//{ FUNCTION
		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
		ULONG_PTR *Functions = new ULONG_PTR[m_FunctionsIDMap.size()+1];
		memset(Functions, 0, (m_FunctionsIDMap.size()+1)*sizeof(*Functions));
		SDSFile->FunctionsEntry = m_nCurPos;
		Functions[0] = m_nCurPos;
		FUNCTION NullFunction;
		m_nCurPos += sizeof(NullFunction);
		memset(&NullFunction, 0, sizeof(NullFunction));
		//NullFunction.name = "";
		//NullFunction.name = (char*)PackString(NullFunction.name);
		gpFileIO->WriteFile(m_Handle, &NullFunction, sizeof(NullFunction));

		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
	{
		for (map<FUNCTIONKEY, unsigned long>::IT it = m_FunctionsIDMap.begin();
			it != m_FunctionsIDMap.end(); ++it)
		{
			map<unsigned long, FUNCTIONKEY*>::IT resit = m_IDFunctionsMap.insert(_Tmap_pair<unsigned long, FUNCTIONKEY*>(it->second, (FUNCTIONKEY*)&(it->first)));
		#if 0
			if (resit == m_IDFunctionsMap.end())
			{
				printf("FAILADD rva %08x id %d %s\n", it->first.rva, it->first.func->ID, it->first.name);

				map<unsigned long, FUNCTIONKEY*>::IT rr = m_IDFunctionsMap.find(it->first.rva);
				if (rr != m_IDFunctionsMap.end())
					printf("EXIST rva %08x id %d %s\n", rr->second->rva, rr->second->func->ID, rr->second->name);
			}
		#endif
		}
	}
	{	long n = 0;
		for (map<unsigned long, FUNCTIONKEY*>::IT it = m_IDFunctionsMap.begin();
			it != m_IDFunctionsMap.end(); ++it)
		{
			Functions[it->second->func->ID] = m_nCurPos;
			unsigned long Size = WriteFunction(it->second->func);
			if (Size == 0)
				Functions[it->second->func->ID] = Functions[0];
	#if 0
		printf("%d func m_nCurPos %d(%x) Size %d, Type %d, Name %s\n",
			n, m_nCurPos, m_nCurPos, sizeof(FUNCTION), it->second->func->ID, it->second->func->name);
	#endif
			m_nCurPos += Size;
			++n;
		}
	}
		m_IDFunctionsMap.clear();
		SDSFile->FunctionsLen = m_FunctionsIDMap.size()+1;
		WriteAlign(EmptyPage, 16);

		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
		SDSFile->Functions = m_nCurPos;
	#if 0
		for (unsigned long n = 0; n < (m_FunctionsIDMap.size()+1)*sizeof(*Functions); ++n)
		{
			if (!Functions[n])
				printf("func: empty slot %d\n", n);
		}
	#endif
		gpFileIO->WriteFile(m_Handle, Functions, (m_FunctionsIDMap.size()+1)*sizeof(*Functions));
		delete []Functions;
		m_nCurPos += (m_FunctionsIDMap.size()+1)*sizeof(*Functions);
		WriteAlign(EmptyPage, 16);		
		//}

		//{ COMPILAND FILES
		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
		SDSFile->FilesLN = m_nCurPos;
		m_nCurPos += WriteFilesLNs(&SDSFile->FilesLNLen);
		WriteAlign(EmptyPage, 16);
		//}

		//{ SRC FILE NAMES
		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
		SDSFile->SrcFileNames = m_nCurPos;
		m_nCurPos += WriteSrcFileNames(&SDSFile->SrcFileNamesLen);
		WriteAlign(EmptyPage, 16);
		SDSFile->SourcesCount = m_CompilandSrcMap.size(); //+++
		//}


		//{ PUBLIC SYMBOLS
		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
		SDSFile->PublicSymbols = m_nCurPos;
		m_nCurPos += WritePublicSymbols(&SDSFile->PublicSymbolsLen);
		WriteAlign(EmptyPage, 16);
		//}

		//{ COMPILAND UNITS, NOT USES
		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
		SDSFile->CompilandUnits = m_nCurPos;
		m_nCurPos += WriteCompilandUnits(&SDSFile->CompilandUnitsLen);
		WriteAlign(EmptyPage, 16);
		//}

		//{ STRINGS
		SDSFile->StringIndexesLen = m_StringIndexes.size();
		SDSFile->StringIndexes = m_nCurPos;
		//if (SDSFile->StringIndexsLen > SDSFile->GlobalsLen)
		//{
		ULONG_PTR *StringIndexes = new ULONG_PTR[SDSFile->StringIndexesLen];
		//}
		for (unsigned n = 0; n < SDSFile->StringIndexesLen; ++n)
		{
			StringIndexes[n] = m_StringIndexes[n];
		}
		gpFileIO->WriteFile(m_Handle, StringIndexes, SDSFile->StringIndexesLen*sizeof(*StringIndexes));
		delete []StringIndexes;
		m_nCurPos += SDSFile->StringIndexesLen*sizeof(*StringIndexes);

		WriteAlign(EmptyPage, 16);
		//
		gpFileIO->SetFilePointer(m_Handle, 0, FILE_CURRENT);
		SDSFile->StringsEntry = m_nCurPos;
	{
		TList<stStringChunk>::IT it = m_StringsList.Begin();
		while (it != m_StringsList.End())
		{
			const char *StringsBuf = it->StringsBuf;
			unsigned long Size = it->StringsCur - it->StringsHead;
			gpFileIO->WriteFile(m_Handle, StringsBuf, Size);
			m_nCurPos += Size;
			delete []StringsBuf;
			++it;
		}
	}
		const char *StringsBuf = m_StringsBuf;
		unsigned long Size = m_StringsCur - m_StringsHead;
		gpFileIO->WriteFile(m_Handle, StringsBuf, Size);
		m_nCurPos += Size;
		delete []StringsBuf;
		//} STRINGS
		WriteAlign(EmptyPage, 16);

		unsigned long CurPos = 0;
		//MODULE NAME
	{
		unsigned long Size = TStrLen(m_ModuleName) + 1;
		if (CurPos >= Size)
		{
			SDSFile->ModuleName = SDSFILE_SIZE- CurPos;
			TStrCpy((char *)SDSFile + SDSFILE_SIZE - CurPos, m_ModuleName);
			CurPos -= Size;
		} else
		{
			SDSFile->ModuleName = m_nCurPos;
			gpFileIO->WriteFile(m_Handle, m_ModuleName, Size);
			m_nCurPos += Size;
		}
		SDSFile->ModuleNameLen = Size;
	}
		//PDB FULL FILE NAME
	{
		unsigned long Size = TStrLen(m_PDBFullFileName) + 1;
		if (CurPos >= Size)
		{
			SDSFile->PDBFullFileName = SDSFILE_SIZE - CurPos;
			TStrCpy((char *)SDSFile + SDSFILE_SIZE - CurPos, m_PDBFullFileName);
			CurPos -= Size;
		} else
		{
			SDSFile->PDBFullFileName = m_nCurPos;
			gpFileIO->WriteFile(m_Handle, m_PDBFullFileName, Size);
			m_nCurPos += Size;
		}
		SDSFile->PDBFullFileNameLen = Size;
	}
		//PDB FULL FILE NAME
		if (FullFileName)
	{
		unsigned long Size = TStrLen(FullFileName) + 1;
		if (CurPos >= Size)
		{
			SDSFile->FullFileName = SDSFILE_SIZE - CurPos;
			TStrCpy((char *)SDSFile + SDSFILE_SIZE - CurPos, FullFileName);
			CurPos -= Size;
		} else
		{
			SDSFile->FullFileName = m_nCurPos;
			gpFileIO->WriteFile(m_Handle, FullFileName, Size);
			m_nCurPos += Size;
		}
		SDSFile->FullFileNameLen = Size;
	}

		WriteAlign(EmptyPage, PAGE_SIZE);

		SDSFile->FileSize = m_nCurPos;

		gpFileIO->WriteFile(m_Handle, SDSFile, SDSFILE_SIZE, 0);
		gpFileIO->CloseHandle(m_Handle);
		WriteSDSCheckSum(SDSFullFileName);
		delete []EmptyPage;
		delete []SDSFile;
		return true;
	}

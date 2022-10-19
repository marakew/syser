
#ifndef _SOURCEDEBUG_HPP_
#define _SOURCEDEBUG_HPP_
#include "../Code/define.h"
#include "../Code/tstrmem.hpp"
#include "../Code/alt/altlist.hpp"
#include "../Code/alt/altmap.hpp"

#ifndef _RING0_
#include <cvconst.h>
#include <dia2.h>
#endif
#include <map>
//#include <set>
#include <list>
#include <vector>
#include <algorithm>

//#define DIASOURCEDEBUG
#include "sdsfile.hpp"

	struct DIASYMBOL : public NULLTYPE
	{
        	void *Value; //
		IDiaSymbol *Symbol;
	};

	struct DIABASICTYPE : public DIASYMBOL { ELEM Elem; };
	struct DIAPOINTERTYPE : public DIABASICTYPE { elemPOINTER v; };
	struct DIAPOINTERREFTYPE : public DIABASICTYPE { elemPOINTERREF v; };
	struct DIAARRAYTYPE : public DIABASICTYPE { elemARRAY v; };
	struct DIAENUMTYPE : public DIABASICTYPE { elemENUM v; };
	struct DIAUDTYPE : public DIABASICTYPE { elemUDT v; };
	struct DIATYPEDEFTYPE : public DIABASICTYPE { elemTYPEDEF v; };
	struct DIAFUNCTYPE : public DIABASICTYPE { elemFUNC v; };
	struct DIABASECLASSTYPE : public DIABASICTYPE { elemBASECLASS v; };
	struct DIABLOCKTYPE  : public DIABASICTYPE { elemBLOCK v; };
	struct DIATHUNK  : public DIABASICTYPE { elemTHUNK v; };

	union COMMONTYPE
	{
		BASICTYPE _basic;
		POINTERTYPE _pointer;
		POINTERREFTYPE _pointerref;
		ARRAYTYPE _array;
		ENUMTYPE _enum;
		UDTYPE _ud;
		TYPEDEFTYPE _typedef;
		FUNCTYPE _func;
		BASECLASSTYPE _baseclass;
		BLOCKTYPE _block;
	};

	struct char_compare
	{
		bool operator()(const char *s1, const char *s2) const
		{
			return TStrICmp(s1, s2) != 0;
		}
	};

//	struct IMPORTMODULE
//	{
//		char *Name;
//		IMPORTMODULE()
//		{
//			Name = 0;
//		}
//		~IMPORTMODULE()
//		{
//			if (Name) delete Name;
//		}
//		bool operator<(const IMPORTMODULE &other) const { return TStrICmp(Name, other.Name)==0; }
//	};

	struct DATAKEY
	{
		char *name;
		unsigned long rva;

		bool operator<(const DATAKEY &other) const
		{
			if (rva < other.rva) return true;
			if (rva != other.rva) return false;
			return TStrCmp(name, other.name)<0;
		}
	};

	struct REGISTERED_
	{
		unsigned long dataKind;
		unsigned long registerId;
		unsigned long ID;
		DATATYPE *data;

		bool operator!=(const REGISTERED_ &other) const
		{
			if (dataKind != other.dataKind) return true;
			if (registerId != other.registerId) return true;
			return false;
		}
	};

	struct FUNCTIONKEY
	{
		char *name;
		unsigned long rva;
		FUNCTION *func;

		bool operator<(const FUNCTIONKEY &other) const
		{
			if (rva < other.rva) return true;
			if (rva > other.rva) return false;
			return TStrCmp(name, other.name)<0;
		}
	};

	struct BASEDATATYPE
	{
		unsigned long basetype;
		unsigned long long length;

		bool operator<(const BASEDATATYPE & other) const
		{
			if (basetype < other.basetype) return true;
			return (basetype == other.basetype) && length < other.length;
		}
	};

	struct BASEDATATYPENAMED
	{
		BASEDATATYPE type;
		char name[32];
	};

	struct IDTYPE
	{
		unsigned long ID;
		DIABASICTYPE *Type;
	};

	enum UseSourcesEnum
	{
		eUseSourcesNone = 0,
		eUseSourcesExtern = 1,
		eUseSourcesText = 2,
		eUseSourcesPacked = 3,
	};

struct CSourceDebug
{
#ifndef _RING0_
	IDiaSymbol *m_GlobalSymbol;
	IDiaSession *m_Session;
	IDiaDataSource *m_DataSource;

	bool IsOpen() const { return m_DataSource && m_Session && m_GlobalSymbol; }

	IDiaEnumFrameData *m_EnumFragmentData;
	IDiaEnumSegments *m_Segments;

	typedef HRESULT(WINAPI*DLLGCO)(REFCLSID, REFIID, LPVOID);
	DLLGCO m_DllGCO;
	HMODULE m_hLibrary;
#endif
	void (*m_CreateProgress)(unsigned long MaxPos);
	void (*m_UpdateProgress)(unsigned long Pos);
	void (*m_UpdateCaption)(const WCHAR *Text);
	void (*m_DestroyProgress)();
	unsigned long m_nTotal;
	unsigned long m_nProgress;

	int (*m_DialogAppendSrcFilename)(char *FullFileName, char *NewFullFileName); //ret 2 - OK

	map<char*, SrcFileNameID, MY_char_compare> m_CompilandSrcMap; //set //char_compare
	map<char*, COMPUNIT*, MY_char_compare> m_CompilandUnitMap; //char_compare

	unsigned long m_CompilandUnitID;
	unsigned long m_CompilandFileID;
	std::list<FilesLN> m_CompilandFilesLN;

	//{ FUNCTION
	unsigned long m_FuncID;
	map<FUNCTIONKEY, unsigned long> m_FunctionsIDMap;
	map<unsigned long, FUNCTIONKEY*> m_IDFunctionsMap;
	TList<unsigned long> m_FunctionsRVA;
	//}

	//{ DATATYPE
	unsigned long m_DataID;
	TList<REGISTERED_> m_Registered;
	map<DATAKEY, unsigned long> m_StaticDatas;
	std::vector<DATATYPE*> m_Locals;
	//}

	//{ PUBSYM
	bool m_HasStringPublicSymbol;
	char *m_StringPublicSymbol;
	set<PUBSYM> m_PublicSymbols;
	set<PUBSYM> m_ClassPublicSymbols;
	//}

	//{ GLOBAL
	unsigned long m_ElemID;
	map<unsigned long, DIABASICTYPE*> m_Globals; //improve speed O(1) vector ???
//	map<const char*, unsigned long, MY_char_compare> m_GlobalsName; //TODO REMOVE
//	std::vector<DIABASICTYPE*> m_GlobalsID; //TODO REMOVE
	//}

	//{ BASECLASS
	std::list<IDTYPE> m_BaseClassList;
	//}

	//{ FUNCTYPE
	std::list<IDTYPE> m_FuncTypeList;
	//}

	//{ TYPEDEF
	std::list<IDTYPE> m_TypedefList;
	//}

	//{ ENUM
	std::list<IDTYPE> m_EnumList;
	//}

	//{ BASE TYPE
	std::map<BASEDATATYPE, unsigned long> m_BaseDataTypeIDMap;
	//}

	//{ POINTER TYPE
	map<const char*,unsigned long, MY_char_compare> m_PointerStrIDMap;
	//}

	//{ POINTERREF TYPE
	map<const char*,unsigned long, MY_char_compare> m_PointerRefStrIDMap;
	//}

	//{ ARRAY TYPE
	std::list<IDTYPE> m_ArrayList;
	//}

	//{ UDTYPE
	std::list<IDTYPE> m_UDTList;
	map<const char*,unsigned long, MY_char_compare> m_UDTStrIDMap;
	map<const char*,unsigned long, MY_char_compare>::IT m_StrIDMAPIter;
	//}

	//{ THUNK
	map<unsigned long, DIABASICTYPE*> m_ThunkRVATypeMap;
	//}

	//{ FILE
	HANDLE m_Handle;
	unsigned long m_nCurPos;
	//} FILE

	//{ STRINGS
	#define STRINGS_CHUNK_SIZE 0x100000
	struct stStringChunk
	{
		const char *StringsBuf;
		unsigned long StringsHead;
		unsigned long StringsCur;
	};

	unsigned long m_StringsID;
	unsigned long m_StringsCur;
	unsigned long m_StringsHead;
	unsigned long m_StringsTail;
	char *m_StringsBuf;
	TList<stStringChunk> m_StringsList;
	std::vector<unsigned long> m_StringIndexes;
	map<const char *, unsigned long, MY_char_compare> m_StringIDMap;

	unsigned long PackString(const char *Str);
	unsigned long PackString(const DATATYPE *data);
	unsigned long PackString(const DIABASICTYPE *Type);
	//} STRINGS

	TList<char*, char_compare> m_ImportModules;

	char *m_ModuleName;
	char *m_PDBFullFileName;

	unsigned long m_nSrcFilesCount; //TODO REMOVE

	//{ FILESIG
	unsigned long m_TimeStamp;
	unsigned long m_CheckSum;

	FILESIG GetFILESIG() const { return FILESIG(m_TimeStamp, m_CheckSum); }
	//}

	CSourceDebug();
	~CSourceDebug();

	void Release();
#ifndef _RING0_
	bool LoadDIA();
	bool UnloadDIA();

	HRESULT OpenDIA(void *, REFIID DiaSource, REFIID DataSource, void **DiaDataSource);
	bool OpenDIA(const char *FileName, IDiaDataSource **DataSource, IDiaSession **Session, IDiaSymbol **Symbol, bool *LoadFromPDBOK);
	void CloseDIA();

	bool GetInfo(IDiaSymbol *Symbol);

	bool FindHelper();

 //TODO REMOVE
//	void MapNameID();

	unsigned long GetTotal();

	bool Parse(bool LoadFromPDBOK);

	void ParserRelease();

	bool StripClassName(char *String);

	bool GetPublicSymbol(IDiaSymbol *Symbol, bool *bRelease, void *Type);

	bool GetCompiland(IDiaSymbol *Symbol);

	void InitBasicType();
	bool SetDataLocation(DATATYPE *data, IDiaSymbol *Symbol);
	unsigned long ParseData(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	bool SameTag(IDiaSymbol *Symbol, unsigned long Tag);
	unsigned long MakeSDBlock(IDiaSymbol *Symbol, TList<unsigned long> *ListsID, unsigned long n);
	unsigned long ParseFunction(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	DIABASICTYPE *GetGlobal(unsigned long ID);
	unsigned long ParseUDType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	unsigned long ParseEnumType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	unsigned long ParseFunctionType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	unsigned long ParsePointerBase(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	IDiaSymbol *GetSymbolPointer(IDiaSymbol *Symbol, int, unsigned long *Count);
	char * GetName(unsigned long ID);
	WCHAR * GetName(unsigned long ID, WCHAR *pStr, int *pLen);
	WCHAR * GetName(DIABASICTYPE *Type, WCHAR *pStr, int *pLen, int);
	unsigned long ParsePointerType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	unsigned long ParsePointerRefType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	bool GetArray(IDiaSymbol *Symbol, std::list<IDiaSymbol *> &Symbols);
	unsigned long ParseArray(IDiaSymbol *Symbol, unsigned long ID, bool *bRelease, NULLTYPE **Type, unsigned long n);
	unsigned long ParseArrayType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	const char *GetBaseTypeName(unsigned long type, unsigned long length);
	unsigned long ParseBaseType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	unsigned long ParseTypedefType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	unsigned long ParseBaseClassType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	unsigned long ParseFriend(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	unsigned long ParseThunk(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	unsigned long ParseCustomType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	unsigned long ParseManagedType(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	unsigned long ParseDIASymbol(IDiaSymbol *Symbol, bool *bRelease, NULLTYPE **Type, unsigned long n);
	bool ReadFILESIG(const char *FullFileName, unsigned long *TimeStamp, unsigned long *CheckSum);
	bool LoadPESym(const char *FullFileName);
	bool AddSources(const char *SDSFullFileName, UseSourcesEnum UseSources, int (CbProgress)(char *, char *));
#endif
	bool IsExist(const char *FileName, bool *LoadFromPDBOK);
	bool CheckSDS(const char *FullFileName, char *NewFullFileName);
	bool Load(const char *FullFileName, char *NewFullFileName, UseSourcesEnum UseSources);
	bool WriteSDSCheckSum(const char *SDSFullFileName);
	void WriteAlign(void *Buffer, unsigned long Size);
	unsigned long WriteLocals(DATATYPE *data);
	unsigned long WriteFunction(FUNCTION *FUNC);
	unsigned long WriteFilesLNs(unsigned long *Count);
	unsigned long WriteSrcFileNames(unsigned long *Count);
	unsigned long WritePublicSymbols(unsigned long *Count);
	unsigned long WriteCompilandUnits(unsigned long *Count);

	unsigned long WriteBaseType(DIABASICTYPE *Type);
	unsigned long WritePointer(DIAPOINTERTYPE *Type);
	unsigned long WritePointerRef(DIABASICTYPE *Type);
	unsigned long WriteArray(DIABASICTYPE *Type);
	unsigned long WriteManaged(DIABASICTYPE *Type);
	unsigned long WriteCustom(DIABASICTYPE *Type);
	unsigned long WriteEnum(DIABASICTYPE *Type);
	unsigned long WriteUDT(DIAUDTYPE *Type);
	unsigned long WriteTypeDef(DIABASICTYPE *Type);
	unsigned long WriteFunction(DIABASICTYPE *Type);
	unsigned long WriteBaseClass(DIABASICTYPE *Type);
	unsigned long WriteFriend(DIABASICTYPE *Type);
	unsigned long WriteThunk(DIABASICTYPE *Type);
	unsigned long WriteBlock(DIABASICTYPE *Type);

	bool WriteSDS(const char *SDSFullFileName, unsigned long TimeStamp, unsigned long CheckSum, const char *FullFileName);
};
#endif

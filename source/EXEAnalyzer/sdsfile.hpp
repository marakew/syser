
#ifndef _SDSFILE_HPP_
#define _SDSFILE_HPP_

#include <cvconst.h>

	#pragma pack(push, 1)
	struct SDSFILE
	{
		unsigned long Magic;
		unsigned long Version;
		unsigned long FileSize;
		unsigned long GlobalsLen;
		unsigned long HeaderSize;
		ULONG_PTR Globals; //BASICTYPE** //???
		unsigned long FilesLNLen;
		ULONG_PTR FilesLN; //FilesLN*
		unsigned long StringIndexesLen;
		ULONG_PTR StringIndexes; //unsigned long*	//char**
		ULONG_PTR StringsEntry; //char*
		unsigned long SrcFileNamesLen;
		ULONG_PTR SrcFileNames; //SrcFileNameID*
		unsigned long PublicSymbolsLen;
		ULONG_PTR PublicSymbols; //PUBSYM*
		ULONG_PTR CompilandUnits; // NOT USES
		unsigned long CompilandUnitsLen; // NOT USES
		unsigned long LocalsLen;
		ULONG_PTR Locals; //DATATYPE*
		unsigned long X1;
		unsigned long FunctionsLen;
		ULONG_PTR FunctionsEntry; //FUNCTION*
		ULONG_PTR Functions; //FUNCTION**
		unsigned long Sources; //??? ULONG_PTR
		unsigned long TimeStamp;
		unsigned long CheckSum;
		unsigned long SUB;
		ULONG_PTR ModuleName; //char*
		unsigned long ModuleNameLen;
		ULONG_PTR PDBFullFileName; //char*
		unsigned long PDBFullFileNameLen;
		ULONG_PTR FullFileName; //char*
		unsigned long FullFileNameLen;
		unsigned long CRCSUM;
		unsigned long SourcesCount;
		unsigned long X[40];
	};
	#pragma pack(pop)
	#define SDSFILE_MAGIC 0x53445346 //'SDSF'
	#define SDSFILE_SIZE sizeof(SDSFILE) //300
	#define SDSFILE_VERSION 266

	// GLOBALS
	#pragma pack(push, 1)
	struct NULLTYPE
	{
	};
	#pragma pack(pop)

	enum ELEM_TYPE
	{
		ELEM_BASE_TYPE	= 0,
		ELEM_POINTER	= 1,
		ELEM_POINTERREF	= 2,
		ELEM_ARRAY	= 3,
		ELEM_MANAGED_TYPE = 4,
		ELEM_CUSTOM_TYPE = 5,
		ELEM_ENUM	= 6,
	//{ UD
		ELEM_STRUCT	= 7,
		ELEM_CLASS	= 8,
		ELEM_UNION	= 9,
	//}
		ELEM_TYPEDEF	= 10,
		ELEM_FUNC_TYPE	= 11,

		ELEM_BASE_CLASS	= 13,
		ELEM_FRIEND	= 14,
		ELEM_VTABLE	= 15,
		ELEM_THUNK	= 16,

		ELEM_BLOCK	= 19,
	};

	#pragma pack(push, 1)
	struct ELEM
	{
		unsigned long Size;
		unsigned long ID;
		unsigned long Length;
		char *Name;
		unsigned long Type;
	};
	#pragma pack(pop)

	// ELEM_BASE_TYPE
	#pragma pack(push, 1)
	struct BASICTYPE : public NULLTYPE
	{
		ELEM Elem;
	};
	#pragma pack(pop)

	// ELEM_POINTER
	#pragma pack(push, 1)
	struct elemPOINTER
	{
		unsigned long Num;
		unsigned long PrevID;
		unsigned long ID;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct POINTERTYPE : public BASICTYPE, elemPOINTER
	{
	};
	#pragma pack(pop)

	// ELEM_POINTERREF
	#pragma pack(push, 1)
	struct elemPOINTERREF
	{
		unsigned long ID;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct POINTERREFTYPE : public BASICTYPE, elemPOINTERREF
	{
	};
	#pragma pack(pop)

	// ELEM_ARRAY
	#pragma pack(push, 1)
	struct ARRAYVALUE
	{
		unsigned long ID;
		unsigned long Size;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct elemARRAY
	{
		unsigned long Size;
		unsigned long ParentID;
		unsigned long ID;
		ARRAYVALUE Values[1];
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct ARRAYTYPE : public BASICTYPE, elemARRAY
	{
	};
	#pragma pack(pop)

	// ...

	// ELEM_ENUM
	#pragma pack(push, 1)
	struct ENUMTYPEVALUE
	{
		char *Name;
		unsigned long X; //???
		unsigned long long Value; //???
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct elemENUM
	{
		unsigned long Length : 30;
		unsigned long Nested : 1;
		unsigned long Scoped : 1;		//CHECKED
		unsigned long ID;
		ENUMTYPEVALUE Values[1];
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct ENUMTYPE : public BASICTYPE, elemENUM
	{
	};
	#pragma pack(pop)

	// ELEM_STRUCT
	// ELEM_CLASS
	// ELEM_UNION
	#pragma pack(push, 1)
	struct CLASSPUBSYM
	{
		unsigned long length;
		unsigned long rva;
		unsigned long ID;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct elemUDT
	{
		unsigned long BaseClassLen;
		unsigned long FriendLen;
		unsigned long DataLen;
		unsigned long ThisRel; //???
		//unsigned long Flags : 19; //>>20 (0x10'0000) PubSymbolsLen 0xFFF F'FFFF
		unsigned long constructor:1; //1
		unsigned long scoped:1; //2
		unsigned long packed:1; //4
		unsigned long nested:1; //8

		unsigned long hasNestedTypes:1; //10
		unsigned long hasCastOperator:1; //20
		unsigned long hasAssignmentOperator:1; //40
		unsigned long overloadedOperator:1; //80

		unsigned long vtable:1; //100
		unsigned long X1:1; //200
		unsigned long unnamed:1; //400
		unsigned long X3:1; //800

		unsigned long X4:1; //'1000
		unsigned long X5:1; //'2000
		unsigned long X6:1; //'4000
		unsigned long X7:1; //'8000

		unsigned long X8:1; //1'0000
		unsigned long X9:1; //2'0000
		unsigned long X10:1; //4'0000
		unsigned long X11:1; //8'0000 //???

		unsigned long PubSymbolsLen : 12; //TODO BT
		CLASSPUBSYM *PubSymbols;
		unsigned long *BaseClass;
		unsigned long *Data;
		unsigned long FunctionLen;
		unsigned long *Function;
		unsigned long *Friend;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct UDTYPE : public BASICTYPE, elemUDT
	{
	};
	#pragma pack(pop)

	// ELEM_TYPEDEF
	#pragma pack(push, 1)
	struct elemTYPEDEF
	{
		unsigned long ID;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct TYPEDEFTYPE : public BASICTYPE, elemTYPEDEF
	{
	};
	#pragma pack(pop)

	// ELEM_FUNC_TYPE
	#pragma pack(push, 1)
	struct elemFUNC
	{
		unsigned long nArgs : 16;
		unsigned long CallingConvention : 16; //CHECKED
		unsigned long ObjPtrID;
		unsigned long FuncTypeID;
		unsigned long ThisAdjust;
		unsigned long ArgsIDs[1];
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct FUNCTYPE : public BASICTYPE, elemFUNC
	{
	};
	#pragma pack(pop)

	// ELEM_BASECLASS
	#pragma pack(push, 1)
	struct elemBASECLASS
	{
		unsigned long ID;
		unsigned long X; //???
		unsigned long Access : 16;
		unsigned long IndirectVirtualBaseClass : 1;
		unsigned long VirtualBaseClass : 1;
		unsigned long XX : 14; //???
		unsigned long Offset;
		unsigned long VirtualBasePointerOffset;
		unsigned long VirtualBaseDispIndex;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct BASECLASSTYPE : public BASICTYPE, elemBASECLASS
	{
	};
	#pragma pack(pop)

	// ELEM_BLOCK
	#pragma pack(push, 1)
	struct elemBLOCK
	{
		unsigned long rva;
		unsigned long length; //??? ULONGULONG
		unsigned long count; //nIDs
		unsigned long IDs[1];
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct BLOCKTYPE : public BASICTYPE, elemBLOCK
	{
	};
	#pragma pack(pop)

	// ELEM_THUNK
	#pragma pack(push, 1)
	struct elemTHUNK
	{
		unsigned long X; //???
		unsigned long thunkOrdinal;
		unsigned long rva;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct THUNKTYPE : public BASICTYPE, elemTHUNK
	{
	};
	#pragma pack(pop)


	// FILELN
	#pragma pack(push, 1)
	struct FilesLN
	{
		unsigned long address; //rva always DWORD
		unsigned long statement : 1 ;
		unsigned long UnitID : 15 ;
		unsigned long FileID : 16 ;
		unsigned long lineNumber : 16;
		unsigned long length : 16;
	};
	#pragma pack(pop)

	inline bool operator<(const FilesLN &l, const FilesLN &r) { return l.address < r.address; }

	// SRC FILE NAMES
	#pragma pack(push, 1)
	struct SrcFileNameID
	{
		unsigned long FileID;
		char *FileName;
		char *Text;
		unsigned long FileSize;
	};
	#pragma pack(pop)


	// PUBLIC SYMBOL
	#pragma pack(push, 1)
	struct PUBSYM
	{
		char *name;
		char *moduleName;
		unsigned long rva;
		unsigned long length; //???
		unsigned long options;


		//str	   0x1'0000
		//???
		//export   0x4'0000 //<<
		//import   0x8'0000 //<<19

		//managed 0x10'0000 //<<20
		//msil    0x20'0000 //<<21
		//func	  0x40'0000 //<<22
		//code    0x80'0000 //<<23

		//vtbl	 0x100'0000 //<<24

//		PUBSYM()
//		{
//			name = 0;
//			moduleName = 0;
//			address = 0;
//			length = 0;
//			options = 0;
//		}

//		~PUBSYM()
//		{
//			if (name) delete name;
//			if (moduleName) delete moduleName;
//		}
		bool operator<(const PUBSYM &other) const { return rva < other.rva; }
	};
	#pragma pack(pop)

	//inline bool operator<(const PUBSYM &l, const PUBSYM &r) { return l.rva < r.rva; }

	// COMPILAND UNIT
	#pragma pack(push, 1)
	struct COMPUNIT
	{
		struct FILES
		{
			unsigned long FIRST;
			unsigned long FileID;
			unsigned long nLines;
			unsigned long LAST;
		};

		char *UnitName;
		unsigned long UnitID;
		unsigned long nFiles;
		unsigned long MinRVA;
		unsigned long MaxRVA;
		FILES Files;

//		COMPUNIT()
//		{
//			UnitName = 0;
//		}
//		~COMPUNIT()
//		{
//			if (UnitName) delete UnitName;
//		}
		bool operator<(const COMPUNIT &other) const { return TStrCmp(UnitName, other.UnitName)<0; }
	};
	#pragma pack(pop)

	//inline bool operator<(const COMPUNIT &l, const COMPUNIT &r) { return TStrCmp(l.UnitName, r.UnitName)<0; }

	// DATATYPE
	#pragma pack(push, 1)
	struct DATATYPE : public NULLTYPE
	{
		unsigned long ID;
		unsigned long TypeLength;
		char *name; //VALUE *
		unsigned long ClassID;
		unsigned long TypeID;
	//VALUE
		unsigned long constType : 1;
		unsigned long volatileType : 1;
		unsigned long access : 14;
		unsigned long locationType : 8;
		unsigned long dataKind : 8;

		union 
		{
			struct {
				unsigned long rva;
			} Static;
			struct {
				unsigned long section;
				unsigned long offset;
			} TLS;
			struct {
				long offset;
			} ThisRel;
			struct {
				long offset;
				unsigned long registerId;

			} RegRel;
			struct {
				unsigned long registerId;
			} Enregistered;
			struct {
				unsigned long bitPosition;
				unsigned long length;
				long offset;
			} BitField;

			struct {
				unsigned long slot;
			} Slot;

			struct {
				unsigned long token;
			} MetaData;

			struct {
				unsigned long long value;
			} Constant;

			struct {
			unsigned long a0;
			unsigned long a1;
			unsigned long a2;
			unsigned long a3;
			};
		};
	};
	#pragma pack(pop)

	// FUNCTION
	#pragma pack(push, 1)
	struct FUNCTIONLABEL
	{
		char *Name;
		unsigned long Offset;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct FUNCTION : public NULLTYPE
	{
		char *name;

		unsigned long LocationType : 16; //0
		unsigned long Access : 8; //16
		unsigned long Virtual : 1; //24
		unsigned long Pure : 1; //25
		unsigned long Intro : 1; //26
		unsigned long XX : 1; //27
		unsigned long SystemExceptionHandling : 1; //28
		unsigned long CplusplusExceptionHandling : 1; //29

		unsigned long virtualOffset;
		unsigned long rva;
		unsigned long debugStart;
		unsigned long debugEnd;
		//ULONGLONG length; //???
		unsigned long length; //??? X64
		unsigned long ID;

		unsigned long ReturnCallID;
		unsigned long ReturnTypeID;

		unsigned long ClassID;

		unsigned long nstack;
		unsigned long *stack;

		unsigned long nblocks;
		unsigned long *blocks;

		unsigned long nlabels;
		FUNCTIONLABEL *labels;
	};
	#pragma pack(pop)
	//

#endif
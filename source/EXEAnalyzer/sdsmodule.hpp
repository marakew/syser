
#ifndef _SDSMODULE_HPP_
#define _SDSMODULE_HPP_

#include "../Code/tstrmem.hpp"
#include "../Code/alt/altlist.hpp"
#include "symbol.hpp"
#include "sdsfile.hpp"

inline int TSTRADD(char *d, const char *s, int *l)
{
	int Len = TStrLen(s);
	if (*l > Len)
		TStrCat(d, s);
	*l -= Len;
	return Len;
}

	struct MAPFilesLN
	{
		FilesLN *fileln;
		unsigned long count;
	};

	//0 - FUNC
	//1 - LOCAL
	//2 - PUBLIC

	struct SDSELEM
	{
		unsigned long hash:30; //0x80000000 pub, 0x40000000 - local, hash
		unsigned long type:2;
		unsigned long rva;
		char *name;
	};

	struct CSDSModule
	{
		CSDSModule();
		~CSDSModule();

		bool m_LoadOK;
		unsigned long long m_Size;
		HANDLE m_Handle;
		char **m_Strings;
		unsigned char *m_Buffer;
		map<const char*, unsigned long, MY_char_compare> m_GlobalsNameID;
		TList<SDSELEM> m_Elems;
		SDSFILE *m_pSDSFile;

		static FILESIG GetFILESIG(const char *FullFileName);
		FILESIG GetFILESIG();
		void Dump(SDSFILE *SDSFile);
		bool Load(const char *FullFileName);
		void Close();
		bool CheckCRC();
		void Relocate(SDSFILE *SDSFile);
		void InitGlobalsNameID();

		TList<unsigned long> m_BreakPoints; //lineNumbers
		TAnsiNoCaseString m_ExeFileName;
		TAnsiNoCaseString m_SDSFileName;
		CDbgModule *m_pDbgModule; //uses in original syser
		unsigned long m_MemSize;
		unsigned long m_dwProcessId;

		BASICTYPE **GetGlobal(unsigned long *Count) const
		{
			if (Count) *Count = m_pSDSFile->GlobalsLen;
			return (BASICTYPE**)m_pSDSFile->Globals;
		}

		//!
		DATATYPE *GetLocal(unsigned long *Count) const
		{
			if (Count) *Count = m_pSDSFile->LocalsLen;
			return (DATATYPE*)m_pSDSFile->Locals;
		}

		FilesLN *GetFilesLN(unsigned long *Count) const
		{
			if (Count) *Count = m_pSDSFile->FilesLNLen;
			return (FilesLN*)m_pSDSFile->FilesLN;
		}

		FUNCTION **GetFunctions(unsigned long *Count) const
		{
			if (Count) *Count = m_pSDSFile->FunctionsLen;
			return (FUNCTION **)m_pSDSFile->Functions;
		}
		//!
		PUBSYM *GetPublicSymbols(unsigned long *Count) const
		{
			if (Count) *Count = m_pSDSFile->PublicSymbolsLen;
			return (PUBSYM *)m_pSDSFile->PublicSymbols;
		}
		//!
		SrcFileNameID *GetSrcFiles(unsigned long *Count) const
		{
			if (Count) *Count = m_pSDSFile->SrcFileNamesLen;
			return (SrcFileNameID *)m_pSDSFile->SrcFileNames;
		}

		const char *GetExeFileName() const { return (const char *)m_pSDSFile->FullFileName; }
		const char *GetModuleName() const { return (const char *)m_pSDSFile->ModuleName; }

		SrcFileNameID *GetSrcFileName(const char *FileName) const;
		SrcFileNameID *GetSrcFileNameID(unsigned long FileID) const;
		unsigned long GetSrcFilesCount() const { return m_pSDSFile->SourcesCount; }

		int GetFilesLN(unsigned long FileID, map<unsigned long, MAPFilesLN> *FilesLNMap);
		//unsigned long
		int GetFilesLN(ULONG_PTR Address, TList<FilesLN*> *ListFilesLN);
		int GetFilesLN(unsigned long FileID, unsigned long lineNumber, TList<FilesLN*> *ListFilesLN);

		FilesLN *GetFilesLN(unsigned long FileID, unsigned long lineNumber);

		FUNCTION *GetFunction(const char *FuncName, UDTYPE *Type, bool bSkipClassName);
		FUNCTION *GetFunction(const char *FuncName, bool bCase);
		FUNCTION *GetFunction(unsigned long Address);
		FUNCTION *GetFunction(unsigned long FileID, unsigned long LineNumber);
		FUNCTION *GetFunctionByID(unsigned long ID);

		DATATYPE *GetLocal(unsigned long ID);
		DATATYPE *GetLocal(const char *Name, unsigned long DT);

		BASICTYPE *GetGlobal(unsigned long ID);

		char *GetOutput(BASICTYPE *Type, char *pStr, int *pLen);

		char *GetOutput(unsigned long ID, char *pStr, int *pLen);

		bool IsPointerFunction(BASICTYPE *Type);

		//for src tip
		bool GetOutputTypedef(TYPEDEFTYPE *Type, TAnsiString & String);

		//for src tip
		bool GetOutputEnum(ENUMTYPE *Type, TAnsiString & String);

		char *GetOutputFunc(FUNCTION *pFunc, char *pStr, int *Len);

		DATATYPE *GetClassLocal(const char *Name, FUNCTION *pFunc);

		DATATYPE *GetClassMember(const char *Name, UDTYPE *Type, unsigned long *offset);

		void *GetGlobal(char *Name, FUNCTION *pFunc, bool *bFunc, unsigned long *offset);

		void *GetGlobal(char *Name, unsigned long FileID, unsigned long LineNumber, bool *bFunc, unsigned long *offset);

		//TODO improve binary search or remap
		BASICTYPE *GetGlobal(const char *Name, bool X);

		const char *GetEnumName(unsigned long ID, unsigned long long Value);
		const char *GetName(BASICTYPE *Type);
	};
#endif

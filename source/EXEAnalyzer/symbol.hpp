
#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include "../Code/alt/altmap.hpp"
#include "../Code/alt/altfile.hpp"
#include "../Code/tstrmem.hpp"
#include "../Code/pageimage.hpp"

	struct CSymbolModule
	{
		int field_4;
		ULONG_PTR m_ImageBase;
		unsigned long m_ImageSize;
		map<ULONG_PTR, TAnsiNoCaseString> m_SymbolMap;
		TAnsiNoCaseString m_ModuleName;
		int m_RefCount;
		int _field_44;
		unsigned long m_TimeStamp;
		unsigned long m_CheckSum;

		CSymbolModule();// {}

		virtual bool Add(ULONG_PTR Address, const char *Name); //00
		virtual bool Set(ULONG_PTR Address, const char *Name); //04
		virtual int Del(const char *Name); //08
		virtual bool Del(ULONG_PTR Address); //0C

		virtual bool GetAddr(const char *Name, ULONG_PTR *pAddress); //10
	};

	struct CCommentModule
	{
		int field_4;
		map<ULONG_PTR, TAnsiString> m_CommentMap;
		TAnsiString m_ModuleName;
		int m_RefCount;
		int _field_3C;
		unsigned long m_TimeStamp;
		unsigned long m_CheckSum;

		CCommentModule();// {}

		virtual bool Add(ULONG_PTR Address, const char *Name); //00
		virtual bool Set(ULONG_PTR Address, const char *Name); //04
		virtual bool Del(ULONG_PTR Address); //08

		unsigned long Write(ALT::ALTFileStream &File);
		unsigned long Read(ALT::ALTFileStream &File);
	};


	struct CSymbolContainer
	{
		map<unsigned long long, CSymbolModule> m_SymbolModulesMap;
	public:
		CSymbolContainer() {}

		virtual CSymbolModule *FindModule(unsigned int TimeStamp, unsigned int CheckSum); //00
		virtual CSymbolModule *InsertModule(unsigned int TimeStamp, unsigned int CheckSum); //04
		virtual bool RemoveModule(unsigned int TimeStamp, unsigned int CheckSum); //08
		virtual void Reset(); //0C

		virtual bool GetFullSymbol(ULONG_PTR Address, char *SymbolName, int Limit); //10
		virtual const char *GetSymbol(ULONG_PTR Address); //14
	};

	struct CCommentContainer
	{
		map<unsigned long long, CCommentModule> m_CommentModulesMap;
	public:
		CCommentContainer() {}

		virtual CCommentModule *FindModule(unsigned int TimeStamp, unsigned int CheckSum); //00 TimeStamp,CheckSum
		virtual CCommentModule *InsertModule(unsigned int TimeStamp, unsigned int CheckSum); //04
		virtual bool RemoveModule(unsigned int TimeStamp, unsigned int CheckSum); //08
		virtual void Reset(); //0C

		unsigned long Write(ALT::ALTFileStream &File);
		unsigned long Read(ALT::ALTFileStream &File);
	};

	struct CDbgModule
	{
		int field_4;
		CSymbolModule *m_pSymbolModule;
		CCommentModule *m_pCommentModule;
		TAnsiNoCaseString m_FullFileName; //m_ModuleFullName
		TAnsiNoCaseString m_FileTitle;
		char *m_pName;
		ULONG_PTR m_ImageBase;
		unsigned long m_ImageSize;
		int XXXXXX__field_3C;
		unsigned long m_TimeStamp;
		unsigned long m_CheckSum;
		bool m_LoadPDB;
	public:
		CDbgModule();// {}

		virtual bool GetSymbolAddress(const char *szName, ULONG_PTR *Address); //00
		virtual bool GetSymboBaseAddr(const char *szName, ULONG_PTR *Address); //04
		virtual bool GetFullSymbol(ULONG_PTR Address, char *szName, unsigned int Len); //08

		virtual const char *GetSymbol(ULONG_PTR Address);
		virtual const char *GetComment(ULONG_PTR Address);

		virtual bool GetSymbolBase(ULONG_PTR *Address); //14

		virtual void SetSymbolModule(CSymbolModule *pSymbolModule); //18
		virtual void RemoveSymbolModule(CSymbolContainer *pSymbolContainer); //1C

		virtual void SetCommentModule(CCommentModule *pCommentModule); //20
		virtual void RemoveCommentModule(CCommentContainer *pCommentContainer); //24
	};

	struct CDbgModuleList
	{
		map<CMemoryImage, CDbgModule> m_DbgModuleMap;
	public:
		CDbgModuleList() {}

		virtual CDbgModule *FindModule(ULONG_PTR Address); //00 GetModule
		virtual CDbgModule *FindModule(const char *szName); //04 GetModule
		virtual CDbgModule *FindModule(unsigned long TimeStamp, unsigned long CheckSum); //08 GetModule
		virtual CDbgModule *InsertModule(const char *szModule, ULONG_PTR Address, unsigned long Size, unsigned long TimeStamp, unsigned long CheckSum);
		virtual bool RemoveModule(ULONG_PTR Address); //10
		virtual bool GetFullSymbol(ULONG_PTR Address, char *szName, int Len); //14
		virtual const char *GetSymbol(ULONG_PTR Address); //18
		virtual bool GetSymbolBase(ULONG_PTR *Address); //1C
		virtual const char *GetComment(ULONG_PTR Address); //20
	};


#endif

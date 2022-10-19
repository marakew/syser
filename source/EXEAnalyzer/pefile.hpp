
#ifndef _PEFILE_HPP_
#define _PEFILE_HPP_

#include "../Code/imagefile.hpp"
#include "pehead.hpp"
#include "mzhead.hpp"
	
	struct PE_SECTION
	{
		char Name[8];
		unsigned long VirtualSize; //PhysicalAddress
		unsigned long VirtualAddress;
		unsigned long SizeOfRawData;
		unsigned long PointerToRawData;
		unsigned long PointerToRelocations;
		unsigned long PointerToLinenumbers;
		unsigned short NumberOfRelocations;
		unsigned short NumberOfLinenumbers;
		unsigned long Characteristics;
	};


	struct IMPORT_FUNC
	{
		char FuncName[64];
		ULONG_PTR NameAddr;
		ULONG_PTR ThunkAddr; //RVA
		unsigned long Address;
		unsigned long FuncOrd;
		bool bUseOrd;
	};

	struct IMPORT_MODULE
	{
		char ModuleName[64];
		char FileName[64];
		IMPORT_FUNC *FuncList;
		int FuncCount;
		ULONG_PTR RVA;
	};

	struct EXPORT_FUNC
	{
		char FuncName[64];
		ULONG_PTR Address;
		unsigned long FuncOrd;
		ULONG_PTR Thunk;
	};


	struct RES_DATA
	{
		unsigned long CodePage;
		unsigned long ID;
		unsigned short IDName[64];
		unsigned long BufferAddr;
		unsigned long Size;
	};

	struct RES_DIR
	{
		unsigned long Type;
		unsigned short TypeName[64];
		int ResCount;
		RES_DATA *ResDataDir;
	};

	struct BASE_RELOC
	{
		unsigned long VirtualAddr;
		unsigned long Count;
		unsigned short *TypeOffset;
	};

struct CPEFile : public CImageFile
{
	CPEFile();

	bool MapToFile(ULONG_PTR MemAddr, unsigned long *pPhysAddr, bool bImageBase);
	bool MapToMemory(unsigned long MemAddr, ULONG_PTR *pPhysAddr, bool bImageBase);

	virtual bool Open(const char *FileName, HANDLE HandleOpen) override;
	virtual void Close() override;
	virtual unsigned long ReadImageMemory(ULONG_PTR Address, void *Buffer, unsigned long Size) override;
	virtual unsigned long WriteImageMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size) override;
	virtual bool LoadPage(ULONG_PTR Address, void *Buffer) override;
	virtual unsigned long GetPageAttribute(ULONG_PTR Address) override;
	bool Check();
	virtual void RelocLoad(ULONG_PTR BaseAddr) override;
  bool IsRelocArea(unsigned long HiAddr, unsigned long LowAddr);
  bool IsImportThunkArea(unsigned long LowAddr, unsigned long HiAddr);
  bool GetImportThunkArea(unsigned long *LowAddr, unsigned long *HiAddr);
	void GetBaseReloc();
	void GetImportModule();
  ULONG_PTR GetExportFunc(unsigned long Ord) const;
  ULONG_PTR GetExportFunc(const char *Name, unsigned long *Index) const;
	void GetExportFunc();
	void GetTLS();
  void GetResource();
  int GetLastPhyPosSection() const;
  int GetLastMemAddrSection() const;
  int GetFirstPhyPosSection() const;
  int GetFirstMemAddrSection() const;
  bool GetSectionIndex(ULONG_PTR Address, int *Index) const;
  int LoadStringW(unsigned long ID, WCHAR *Name, int Size);
  int LoadStringA(unsigned long ID, char *Name, int Size);
  RES_DATA *FindResource(const unsigned short *IDName, unsigned long ID, const unsigned short *Name, unsigned long Type);

	IMPORT_MODULE *GetImportModule(const char *FileName) const;
	IMPORT_FUNC *GetImportFunc(IMPORT_MODULE *pImportModule, const char *Name) const;
	IMPORT_FUNC *GetImport(const char *FileName, char *FuncName) const;
	bool GetDataDir(unsigned long Index, PE_DATA_DIR *DataDir) const;
	bool Open(ULONG_PTR ImageBase, int OpenMode, unsigned long (*RMFunc)(ULONG_PTR, void*, unsigned long, void*), void *RMObj);

	//0x01
	#define PE_OPEN_DLL	  0x02
	#define PE_OPEN_SYS	  0x04
	//0x08
	#define PE_OPEN_NO_IMPORT 0x10
	#define PE_OPEN_NO_EXPORT 0x20
	//0x40
	//0x80

	unsigned long m_OpenMode;

	enum
	{
		PE_ERROR_SUCCESS = 0,
		PE_ERROR_SIGN = 1,
		PE_ERROR_SECTION = 2,
		PE_ERROR_MODULE_TYPE = 3,
		PE_ERROR_HEAD = 4,
	};

	unsigned long m_ErrorCode;
	unsigned long m_PEOff;

	PE_SECTION *m_Section;
	int m_SectCount;

	PE_HEAD m_PEHead;

	IMPORT_MODULE *m_ImportModule;
	int m_ImportModuleCount;

	EXPORT_FUNC *m_ExportFunc;
	int m_ExportFuncCount;

	RES_DIR *m_ResDir;
	int m_ResDirCount;

	BASE_RELOC *m_BaseReloc;
	int m_BaseRelocCount;

	unsigned char *m_TLSData;
	int m_TLSDataSize;

	unsigned long m_AddressOfTLSIndex;
	ULONG_PTR *m_TLSCallBackFuncs;
	int m_TLSCallBackCount;
};

#endif
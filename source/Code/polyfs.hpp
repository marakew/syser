
#ifndef _POLYFS_HPP_
#define _POLYFS_HPP_

#include "define.h"
//#include "osdefine.hpp"
#include "imagefile.hpp"
#include "pagemem.hpp"
#include "fileio.hpp"
#include "handle.hpp"
#include "alt/altlist.hpp"

#define PFS_DIRECTORY	0x01
//0x02
#define PFS_READONLY	0x04
#define PFS_HIDDEN	0x08
#define PFS_SYSTEM	0x10
#define PFS_ARCHIVE	0x20


#pragma pack(push,1)
struct PFS_FILE_ENTRY //0x30
{
	unsigned long BeginClus;
	unsigned long FileSize;
	unsigned long Attribute;
	unsigned long NameCode;
	char FileName[32];
};
#pragma pack(pop)

#pragma pack(push,1)
struct PFS_FILE_DATA //0x68
{
	unsigned long FilePoint;
	unsigned long FileEntryFP;
	PFS_FILE_ENTRY ThisFileEntry;
	PFS_FILE_ENTRY ParentEntry;
};
#pragma pack(pop)

#pragma pack(push,1)
struct PFS_FIND_DATA //0x264
{
	char FileName[256];
	char PattenName[256];
	PFS_FILE_ENTRY ParentEntry;
	PFS_FILE_ENTRY ThisFileEntry;
	unsigned long FileEntryFP;
};
#pragma pack(pop)

#pragma pack(push,1)
struct PFS_HEAD //0x70
{
	char Sign[8];
	unsigned long ClusSize;
	unsigned long ClusCount;
	PFS_FILE_ENTRY FatEntry;
	PFS_FILE_ENTRY RootEntry;
};
#pragma pack(pop)

struct CPolyFS
{
	CPolyFS();
	virtual ~CPolyFS();

	CImageFile m_DiskImage;
	CPageMemory m_PageMemory;
	CFileIO *m_pFileIO;
	unsigned long m_MaxCachePageCount;
	unsigned long m_FatOff;
	unsigned long m_ClusSize;
	unsigned long m_ClusCount;
	unsigned int m_Style;
	PFS_FILE_ENTRY m_RootEntry;
	unsigned char *m_ZeroBuffer;

	bool ReadDiskFile(unsigned long Pos, void *Buffer, unsigned long Size);
	bool WriteDiskFile(unsigned long Pos, const void *Buffer, unsigned long Size);
	bool ReadClus(unsigned long Index, void *Buffer);
	bool WriteClus(unsigned long Index, const void *Buffer);
	bool ReadFatCell(unsigned long Index, void *Buffer);
	bool WriteFatCell(unsigned long Index, const void *Buffer);
	unsigned long FindFreeFileEntryFP(PFS_FILE_ENTRY *pParentEntry);
	unsigned long FindNextFreeClus(unsigned long ClusIdx);
	bool CreateFile(const char *FullFileName, unsigned long Attr, unsigned long Size, PFS_FILE_DATA *pHandleData);
	bool CreateFile(const char *FileName, unsigned long Attr, unsigned long Size, PFS_FILE_ENTRY *pParentDirEntry, unsigned long ParentFileEntryFP);
	unsigned long ReadFile(struct PFS_FILE_DATA *pHandleData, unsigned long Offset, void *Buffer, unsigned long Size);
	unsigned long ReadFile(struct PFS_FILE_ENTRY *pFileEntry, unsigned long Offset, void *Buffer, unsigned long Size);
	unsigned long WriteFile(struct PFS_FILE_DATA *pHandleData, unsigned long Offset, const void *Buffer, unsigned long Size);
	unsigned long WriteFile(struct PFS_FILE_ENTRY *pFileEntry, unsigned long Offset, const void *Buffer, unsigned long Size);
	bool DeleteFileClus(unsigned long BeginClus);
	unsigned long FileOffToDP(unsigned long ClusIndex, unsigned long Offset);
	unsigned long FileOffToClus(unsigned long ClusIndex, unsigned long Offset);
	bool CmpFileName(const char *FilePatternName, PFS_FILE_ENTRY *pFileEntry);
	void ChangeFileIO(CFileIO *pFileIO);

	virtual bool OpenDisk(const char *FileName, unsigned int Style);
	virtual void CloseDisk();
	virtual void InitParam();

	bool Format(const char*, unsigned long, unsigned long);

	bool DeleteFile(const char *FullFileName);

	bool FindFirstFile(const char *FullFileName, PFS_FIND_DATA *pFindData);
	bool FindNextFile(PFS_FIND_DATA *pFindData);

	bool OpenFile(const char *FullFileName, PFS_FILE_DATA *pHandleData);

	bool SetFileLength(const char *FileName, unsigned long Length);
	bool SetFileLength(PFS_FILE_DATA *pHandleData, unsigned long Length);
	unsigned long GetFileLength(const char *FileName);

	bool SetFileAttribute(const char *FileName, unsigned long Attr);
	unsigned long GetFileAttribute(const char *FileName);
};


struct MY_PFS_FILE_DATA
{
	CPolyFS *m_pPolyFS;
	PFS_FILE_DATA m_FileData;
};

struct MY_PFS_FIND_DATA
{
	CPolyFS *m_pPolyFS;
	PFS_FIND_DATA m_FindData;
};

struct CPFSFileIO : public CFileIO
{
	CHandle m_Handle;
	TList<CPolyFS> m_PolyFSDisk;
	CFileIO *m_pFileIO;

	CPFSFileIO();

	virtual bool OpenFile(const char *FileName, HANDLE *phFile, unsigned long Mode) override;
	virtual bool CreateFile(const char *FileName, HANDLE *phFile, unsigned long Mode) override;
	virtual bool CreateDir(const char *FileName) override;
	virtual bool CloseHandle(HANDLE hFile) override;
	virtual unsigned long ReadFile(HANDLE hFile, void *Buffer, unsigned long Size) override; //
	virtual unsigned long ReadFile(HANDLE hFile, void *Buffer, unsigned long Size, unsigned __int64 Offset) override;
	virtual unsigned long WriteFile(HANDLE hFile, const void *Buffer, unsigned long Size) override; //
	virtual unsigned long WriteFile(HANDLE hFile, const void *Buffer, unsigned long Size, unsigned __int64 Offset) override;
	virtual unsigned __int64 SetFilePointer(HANDLE hFile, __int64 Offset, unsigned long Pos) override; //
	virtual unsigned __int64 GetFileLength(HANDLE hFile) override;
	virtual bool SetFileLength(HANDLE hFile, unsigned __int64 Length) override;
	virtual bool DeleteFile(const char *FileName) override;
	virtual bool RenameFile(const char *FileName, const char *NewFileName) override; //

	virtual bool SetFileAttr(const char *FileName, unsigned long Attr) override;
	virtual unsigned long GetFileAttr(const char *FileName) override;

	virtual bool IsDir(const char *FileName) override;
	virtual bool DeleteDir(const char *FileName) override;

	virtual bool FindFirstFile(const char *FileName, FILE_FIND_DATA *pFindData) override;
	virtual bool FindNextFile(FILE_FIND_DATA *pFindData) override;
	virtual bool FindClose(FILE_FIND_DATA *pFindData) override;

	void ChangeFileIO(CFileIO *pFileIO);

	virtual bool OpenDisk(const char *FileName, unsigned int Style);
	virtual void CloseDisk();
};

#endif


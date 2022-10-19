
#ifndef _LOCALFILEIO_HPP_
#define _LOCALFILEIO_HPP_

#include "define.h"
#include "fileio.hpp"

struct CLocalFileIO : public CFileIO
{
	static WCHAR m_CurDirName[MAX_PATH];

	void SetCurDir(const WCHAR *DirName);

	virtual bool OpenFile(const char *FileName, HANDLE *phFile, unsigned long Mode) override;
	virtual bool CreateFile(const char *FileName, HANDLE *phFile, unsigned long Mode) override;
	virtual bool CreateDir(const char *FileName) override;
	virtual bool CloseHandle(HANDLE hFile) override;

	virtual void FlushFile(HANDLE hFile) override;

	virtual unsigned long ReadFile(HANDLE hFile, void *Buffer, unsigned long Size) override;
	virtual unsigned long ReadFile(HANDLE hFile, void *Buffer, unsigned long Size, unsigned __int64 Offset) override;

	virtual unsigned long WriteFile(HANDLE hFile, const void *Buffer, unsigned long Size) override;
	virtual unsigned long WriteFile(HANDLE hFile, const void *Buffer, unsigned long Size, unsigned __int64 Offset) override;

	virtual unsigned __int64 SetFilePointer(HANDLE hFile, __int64 Offset, unsigned long Pos) override;

	virtual unsigned __int64 GetFileLength(HANDLE hFile) override;
	virtual bool SetFileLength(HANDLE hFile, unsigned __int64 Length) override;

	virtual bool DeleteFile(const char *FileName) override;
	virtual bool RenameFile(const char *FileName, const char *NewFileName) override;

	virtual bool GetFileTime(const char *FileName, void *lpLastWriteTime, void *lpCreationTime, void *lpLastAccessTime) override; //new+

	virtual bool IsDir(const char *FileName) override;
	virtual bool DeleteDir(const char *FileName) override;
	
	virtual bool FindFirstFile(const char *DirName, FILE_FIND_DATA *pFindData) override;
	virtual bool FindNextFile(FILE_FIND_DATA *pFindData) override;
	virtual bool FindClose(FILE_FIND_DATA *pFindData) override;
};

#endif

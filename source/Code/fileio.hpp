
#ifndef _FILEIO_HPP_
#define _FILEIO_HPP_

#include "define.h"
//#include "osdefine.hpp"
//#include <windows.h>

struct CFileIO;

#define XCOPY_FOUND 0
#define XCOPY_NEXT  1
#define XCOPY_END 2

typedef int (*FUNC_XCOPY_CB)(CFileIO *pSrcIO, CFileIO *pDstIO, const char *pSrcPath, const char *pDstPath, unsigned long Length, void *pData);

#define XFIND_END 0
#define XFIND_NEXT 1

typedef int (*FUNC_XFIND_CB)(CFileIO *This, const char *FullFileName, int FileLength, void *pData);

struct FILE_FIND_DATA
{
	char FileName[MAX_FN_LEN];
	unsigned long FileAttribute; //??? as win
	HANDLE hFindFile;
	void *FindDataBuffer;
	unsigned __int64 FileLength;
};

#define FILE_MODE_RO    0x00000
#define FILE_MODE_RW    0x00001

#define FILE_MODE_SHR	0x10000
#define FILE_MODE_SHW	0x20000

struct CFileIO
{
	const char *m_pszSepar;

	CFileIO();
//	{
//		m_pszSepar = "\\";
//	}


	virtual bool OpenFile(const char *FileName, HANDLE *phFile, unsigned long Mode); //{ return false; }
	virtual bool CreateFile(const char *FileName, HANDLE *phFile, unsigned long Mode); //{ return false; }
	virtual bool CreateDir(const char *FileName); //{ return false; }
	virtual bool CloseHandle(HANDLE hFile); //{ return false; }
	virtual unsigned long ReadFile(HANDLE hFile, void *Buffer, unsigned long Size); //{ return 0; }
	virtual unsigned long ReadFile(HANDLE hFile, void *Buffer, unsigned long Size, unsigned __int64 Offset); //{ return 0; }
	virtual unsigned long WriteFile(HANDLE hFile, const void *Buffer, unsigned long Size); //{ return 0; }
	virtual unsigned long WriteFile(HANDLE hFile, const void *Buffer, unsigned long Size, unsigned __int64 Offset); //{ return 0; }
	virtual unsigned __int64 SetFilePointer(HANDLE hFile, __int64 Offset, unsigned long Pos); //{ return 0; }
	virtual unsigned __int64 GetFileLength(HANDLE hFile); //{ return 0; }
	virtual bool SetFileLength(HANDLE hFile, unsigned __int64 Length); //{ return false; }
	virtual bool DeleteFile(const char *FileName); //{ return false; }
	virtual bool RenameFile(const char *FileName, const char *NewFileName); //{ return false; }
	virtual bool GetFileTime(const char *FileName, void *pWriteTime, void *pCreationTime, void *pAccessTime); //new+ { return false; }
	virtual bool IsDir(const char *FileName); //{ return false; }
	virtual bool DeleteDir(const char *FileName); //{ return false; }
	virtual bool SetFileAttr(const char *FileName, unsigned long Attr); //{ return false; }
	virtual unsigned long GetFileAttr(const char *FileName); //{ return -1; }

	virtual bool FindFirstFile(const char *DirName, FILE_FIND_DATA *pFindData); //{ *pFindData = 0; return false; }
	virtual bool FindNextFile(FILE_FIND_DATA *pFindData); //{ *pFindData = 0; return false; }
	virtual bool FindClose(FILE_FIND_DATA *pFindData); //{ *pFindData = 0; return false; }

	virtual void FlushFile(HANDLE hFile); //new+ {}

	virtual bool WriteToFile(const char *FileName, const void *Buffer, unsigned long BufSize); 
	virtual unsigned long ReadFromFile(const char *FileName, void *Buffer, unsigned long BufSize);

	virtual unsigned __int64 GetFileSize(const char *FileName);

	virtual bool IsFileExist(const char *FileName);

	//virtual bool XXXXX(const char *FileName) { return false; } // new+

	virtual bool DeleteDirTree(const char *FileName);
	virtual bool XDelete(const char *FileName);
	virtual int XFind(const char *FullFileName, FUNC_XFIND_CB XFindCB, void *pData, bool bRoot, int *ReturnFlag);
	
	static bool XCopy(CFileIO *pSrcFileIO, CFileIO *pDstFileIO, const char *pSrcPath, const char *pDstPath, FUNC_XCOPY_CB XCopyCB, void *pXCopyData);
};

	extern CFileIO *gpFileIO;

#endif

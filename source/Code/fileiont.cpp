
#include "define.h"
//#include "../Code/osdefine.hpp"

#include "tstrmem.hpp"
#include "fileiont.hpp"

//#include <windows.h>
// //#include <stdio.h>
// //#include <winbase.h>
//#include <windowsx.h>
//#include <winnt.h>

	CLocalFileIO gFileIO;
	CFileIO *gpFileIO = (CFileIO*)&gFileIO;

	WCHAR CLocalFileIO::m_CurDirName[MAX_PATH]; // = "";


	void CLocalFileIO::SetCurDir(const WCHAR *DirName)
	{
		int Length = TStrCpy(m_CurDirName, DirName);
		MAX_LIMIT(Length, MAX_PATH-1);
		if (Length)
		{
			if (m_CurDirName[Length - 1] != '\\' &&
			    m_CurDirName[Length - 1] != '/' )
			{
				m_CurDirName[Length] = '\\';
				m_CurDirName[Length + 1] = 0;
			}
		}
	}

	bool CLocalFileIO::OpenFile(const char *FileName, HANDLE *phFile, unsigned long Mode)
	{
		DWORD OpenMode = GENERIC_READ; //0x80000000;
		if (Mode & 1)
			OpenMode = GENERIC_READ | GENERIC_WRITE; //0xC0000000;
		DWORD ShareMode = 0;
		if (Mode & 0x10000)
			ShareMode = FILE_SHARE_READ; //1;
		if (Mode & 0x20000)
			ShareMode |= FILE_SHARE_WRITE; //2;
		*phFile = ::CreateFileA(FileName, OpenMode, ShareMode, 0, OPEN_EXISTING, 0, 0); //3
		return *phFile != INVALID_HANDLE_VALUE;
	}

	bool CLocalFileIO::CreateFile(const char *FileName, HANDLE *phFile, unsigned long Mode)
	{
		DWORD CreatedMode = CREATE_NEW; //1;
		if (Mode & 1)
			CreatedMode = CREATE_ALWAYS; //2;
		DWORD ShareMode = 0;
		if (Mode & 0x10000)
			ShareMode = FILE_SHARE_READ; //1;
		if (Mode & 0x20000)
			ShareMode |= FILE_SHARE_WRITE; //2;
		*phFile = ::CreateFileA(FileName, GENERIC_READ | GENERIC_WRITE, ShareMode, 0, CreatedMode, 0, 0); //0xC0000000
		return *phFile != INVALID_HANDLE_VALUE;
	}

	bool CLocalFileIO::CreateDir(const char *FileName) //
	{
		return ::CreateDirectoryA(FileName, 0) != 0;
	}

	bool CLocalFileIO::CloseHandle(HANDLE hFile)
	{
		return ::CloseHandle(hFile) != 0;
	}

	void CLocalFileIO::FlushFile(HANDLE hFile) //new+
	{
		::FlushFileBuffers(hFile);
	}

	unsigned long CLocalFileIO::ReadFile(HANDLE hFile, void *Buffer, unsigned long Size, unsigned __int64 Offset)
	{
		unsigned long RDSize = 0;

		if (this->SetFilePointer(hFile, Offset, 0) == -1)
			return 0;
		::ReadFile(hFile, Buffer, Size, (LPDWORD)&RDSize, 0);
		return RDSize;
	}

	unsigned long CLocalFileIO::WriteFile(HANDLE hFile, const void *Buffer, unsigned long Size, unsigned __int64 Offset)
	{
		unsigned long WRSize = 0;
		if (this->SetFilePointer(hFile, Offset, 0) == -1)
			return 0;
		::WriteFile(hFile, Buffer, Size, (LPDWORD)&WRSize, 0);
		return WRSize;
	}

	unsigned long CLocalFileIO::ReadFile(HANDLE hFile, void *Buffer, unsigned long Size)
	{
		unsigned long RDSize = 0;
		::ReadFile(hFile, Buffer, Size, (LPDWORD)&RDSize, 0);
		return RDSize;
	}

	unsigned long CLocalFileIO::WriteFile(HANDLE hFile, const void *Buffer, unsigned long Size)
	{
		unsigned long WRSize = 0;
		::WriteFile(hFile, Buffer, Size, (LPDWORD)&WRSize, 0);
		return WRSize;
	}

	unsigned __int64 CLocalFileIO::SetFilePointer(HANDLE hFile, __int64 Offset, unsigned long Pos)
	{
		unsigned long HiOffset = Offset>>32;
		int result = ::SetFilePointer(hFile, Offset, (PLONG)&HiOffset, Pos);
		if (result == -1)
			return -1;
		return ((unsigned __int64)HiOffset)<<32|result;
	}

	unsigned __int64 CLocalFileIO::GetFileLength(HANDLE hFile)
	{
		unsigned __int64 Size = ::GetFileSize(hFile, 0);
		if (Size == -1LL)
		{
			::GetLastError(); //TODO
			Size = 0;
		}
		return Size;
	}

	bool CLocalFileIO::SetFileLength(HANDLE hFile, unsigned __int64 Length)
	{
		unsigned long HiLength = Length>>32;
		if (::SetFilePointer(hFile, Length, (PLONG)&HiLength, 0) == -1)
			return false;

		return ::SetEndOfFile(hFile) != 0;
	}

	bool CLocalFileIO::DeleteFile(const char *FileName)
	{
		return ::DeleteFileA(FileName) != 0;
	}

	bool CLocalFileIO::RenameFile(const char *FileName, const char *NewFileName)
	{
		return ::MoveFileA(FileName, NewFileName) != 0;
	}

	bool CLocalFileIO::GetFileTime(const char *FileName, void *pWriteTime, void *pCreationTime, void *pAccessTime) //new+
	{
		HANDLE hFile = ::CreateFileA(FileName, 0x80000000, 3, 0, 3, 0, 0);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;
		bool Result = ::GetFileTime(hFile, (LPFILETIME)pCreationTime, (LPFILETIME)pAccessTime, (LPFILETIME)pWriteTime) == 1;
		this->CloseHandle(hFile);
		return Result;
	}
	bool CLocalFileIO::IsDir(const char *FileName)
	{
		DWORD Attributes = ::GetFileAttributesA(FileName);
		if (Attributes == -1)
			return ::GetDriveTypeA(FileName) != 1;
		return (Attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	}

	bool CLocalFileIO::DeleteDir(const char *FileName)
	{
		return ::RemoveDirectoryA(FileName) != 0;
	}
	
	bool CLocalFileIO::FindFirstFile(const char *DirName, FILE_FIND_DATA *pFindData)
	{
		LPWIN32_FIND_DATA pWin32FindData = new WIN32_FIND_DATA; //sizeof 0x140
		pFindData->FindDataBuffer = pWin32FindData;
		char szPath[268];
		TStrCpy(szPath, DirName);
		if (!TStrChr(DirName, '*') && !TStrChr(DirName, '?'))
		{
			TStrCat(szPath, m_pszSepar);
			TStrCat(szPath, "*");
		}
		pFindData->hFindFile = ::FindFirstFileA(szPath, pWin32FindData);
		if (pFindData->hFindFile == INVALID_HANDLE_VALUE)
		{
			delete pWin32FindData;
			pFindData->FindDataBuffer = 0;
			return false;
		}
		
		TStrCpy(pFindData->FileName, pWin32FindData->cFileName);
		pFindData->FileAttribute = 0;
		if (pWin32FindData->dwFileAttributes & FILE_ATTRIBUTE_READONLY)
			pFindData->FileAttribute |= FILE_ATTRIBUTE_READONLY;
		if (pWin32FindData->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			pFindData->FileAttribute |= FILE_ATTRIBUTE_HIDDEN;
		if (pWin32FindData->dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
			pFindData->FileAttribute |= FILE_ATTRIBUTE_SYSTEM;
		if (pWin32FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			pFindData->FileAttribute |= FILE_ATTRIBUTE_DIRECTORY;
		pFindData->FileLength = pWin32FindData->nFileSizeLow;
		
		return true;
	}

	bool CLocalFileIO::FindNextFile(FILE_FIND_DATA *pFindData)
	{
		LPWIN32_FIND_DATA pWin32FindData = (LPWIN32_FIND_DATAA)pFindData->FindDataBuffer;

		bool Result = ::FindNextFileA(pFindData->hFindFile, pWin32FindData) != 0;
		strcpy_s(pFindData->FileName, MAX_FN_LEN, pWin32FindData->cFileName); //??? TODO
		//pFindData->FileAttribute = pWin32FindData->dwFileAttributes;
		pFindData->FileAttribute = 0;

		if (pWin32FindData->dwFileAttributes & FILE_ATTRIBUTE_READONLY)
			pFindData->FileAttribute |= FILE_ATTRIBUTE_READONLY;
		if (pWin32FindData->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			pFindData->FileAttribute |= FILE_ATTRIBUTE_HIDDEN;
		if (pWin32FindData->dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
			pFindData->FileAttribute |= FILE_ATTRIBUTE_SYSTEM;
		if (pWin32FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			pFindData->FileAttribute |= FILE_ATTRIBUTE_DIRECTORY;
		pFindData->FileLength = pWin32FindData->nFileSizeLow;
		return Result;
	}

	bool CLocalFileIO::FindClose(FILE_FIND_DATA *pFindData)
	{
		LPWIN32_FIND_DATA pWin32FindData = (LPWIN32_FIND_DATAA)pFindData->FindDataBuffer;
		if (pWin32FindData)
			delete pWin32FindData;
		return ::FindClose(pFindData->hFindFile) != 0;
	}


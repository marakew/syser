
#include "fileio.hpp"
#include "tstrmem.hpp"
//++
//#include "osdefine.hpp"

//	CFileIO *gpFileIO = nullptr;

	CFileIO::CFileIO()
	{
		m_pszSepar = "\\";
	}

	bool CFileIO::OpenFile(const char *FileName, HANDLE *phFile, unsigned long Mode) { return false; }

	bool CFileIO::CreateFile(const char *FileName, HANDLE *phFile, unsigned long Mode) { return false; }

	bool CFileIO::CreateDir(const char *FileName) { return false; }

	bool CFileIO::CloseHandle(HANDLE hFile) { return false; }

	unsigned long CFileIO::ReadFile(HANDLE hFile, void *Buffer, unsigned long Size) { return 0; }

	unsigned long CFileIO::ReadFile(HANDLE hFile, void *Buffer, unsigned long Size, unsigned __int64 Offset) { return 0; }

	unsigned long CFileIO::WriteFile(HANDLE hFile, const void *Buffer, unsigned long Size) { return 0; }

	unsigned long CFileIO::WriteFile(HANDLE hFile, const void *Buffer, unsigned long Size, unsigned __int64 Offset) { return 0; }

	unsigned __int64 CFileIO::SetFilePointer(HANDLE hFile, __int64 Offset, unsigned long Pos) { return 0; }

	unsigned __int64 CFileIO::GetFileLength(HANDLE hFile) { return 0; }

	bool CFileIO::SetFileLength(HANDLE hFile, unsigned __int64 Length) { return false; }

	bool CFileIO::DeleteFile(const char *FileName) { return false; }

	bool CFileIO::RenameFile(const char *FileName, const char *NewFileName) { return false; }

	bool CFileIO::GetFileTime(const char *FileName, void *lpLastWriteTime, void *lpCreationTime, void *lpLastAccessTime) { return false; } //new+

	bool CFileIO::IsDir(const char *FileName) { return false; }

	bool CFileIO::DeleteDir(const char *FileName) { return false; }

	bool CFileIO::SetFileAttr(const char *FileName, unsigned long Attr) { return false; }

	unsigned long CFileIO::GetFileAttr(const char *FileName) { return -1; }

	bool CFileIO::FindFirstFile(const char *DirName, FILE_FIND_DATA *pFindData) { if (pFindData) pFindData->FileName[0] = 0; return false; }

	bool CFileIO::FindNextFile(FILE_FIND_DATA *pFindData) { if (pFindData) pFindData->FileName[0] = 0; return false; }

	bool CFileIO::FindClose(FILE_FIND_DATA *pFindData) { if (pFindData) pFindData->FileName[0] = 0; return false; }

	void CFileIO::FlushFile(HANDLE hFile) { } //new+

	bool CFileIO::WriteToFile(const char *FileName, const void *Buffer, unsigned long BufSize)
	{
		HANDLE hFile;
		if (!CreateFile(FileName, &hFile, 0x10001)) //FILE_MODE_RW|FILE_MODE_SHR
			return false;

		WriteFile(hFile, Buffer, BufSize, 0);
		CloseHandle(hFile);
		return true;
	}

	unsigned long CFileIO::ReadFromFile(const char *FileName, void *Buffer, unsigned long BufSize)
	{
		HANDLE hFile;
		if (!OpenFile(FileName, &hFile, 0x10000)) //FILE_MODE_RO|FILE_MODE_SHR
			return 0;

		unsigned long Size = BufSize;
		if (BufSize==0)
			Size = GetFileLength(hFile);
		unsigned long Result = ReadFile(hFile, Buffer, Size, 0);
		CloseHandle(hFile);
		return Result;
	}

	unsigned __int64 CFileIO::GetFileSize(const char *FileName)
	{
		unsigned int Len = TStrLen(FileName);
		if ((Len == 2 || Len == 3) && FileName[1] == ':')
		{
			ULARGE_INTEGER freeBytesAvailable;
			ULARGE_INTEGER totalNumberOfBytes;
			ULARGE_INTEGER totalNumberOfFreeBytes;

			freeBytesAvailable.QuadPart = 0;
			totalNumberOfBytes.QuadPart = 0;
			totalNumberOfFreeBytes.QuadPart = 0;

			GetDiskFreeSpaceExA(FileName, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes);
			return totalNumberOfBytes.QuadPart;
		}

		HANDLE hFile;
		if (!OpenFile(FileName, &hFile, 0x10000)) //FILE_MODE_RO|FILE_MODE_SHR
			return -1;
			
		unsigned __int64 Length = GetFileLength(hFile);
		CloseHandle(hFile);
		return Length;
	}

	bool CFileIO::IsFileExist(const char *FileName)
	{
		HANDLE hFile;
		if (!OpenFile(FileName, &hFile, 0x10000)) //FILE_MODE_RO|FILE_MODE_SHR
			return false;

		return CloseHandle(hFile);
	}

	//bool CFileIO::XXXXX(const char *FileName) { return false; } // new+

	bool CFileIO::DeleteDirTree(const char *FileName)
	{
		FILE_FIND_DATA FileData;
		if (!FindFirstFile(FileName, &FileData))
		{
			FindClose(&FileData);
			return true;
		}
		
		do {
			if (FileData.FileAttribute & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (!DeleteDirTree(FileName))
				{
					FindClose(&FileData);
					return false;
				}
			} else
			{
				DeleteFile(FileName);
			}
		} while (FindNextFile(&FileData));
		FindClose(&FileData);
		return true;
	}

	bool CFileIO::XDelete(const char *FileName)
	{
		if (!IsDir(FileName))
			return DeleteFile(FileName);
		return DeleteDirTree(FileName);
	}

	int CFileIO::XFind(const char *FullFileName, FUNC_XFIND_CB XFindCB, void *pData, bool bRoot, int *bReturnFlag)
	{
		int ret = 0;
		int FLAG;
		if (!bReturnFlag)
		{
			bReturnFlag = &FLAG;
			FLAG = 0;
		}
		char FileName[MAX_FN_LEN];
		char FilePath[MAX_FN_LEN];
		char *pStr = TGetFileName(FullFileName);
		if (TStrChr(pStr,'*') || TStrChr(pStr,'?'))
		{
			pStr = TGetFileName(FullFileName);
			TStrCpy(FileName, pStr);
			TGetFilePath(FullFileName, FilePath);
			FullFileName = FilePath;
		} else
		{
			FileName[0] = 0;
			if (!IsDir(FullFileName) && IsFileExist(FullFileName))
			{
				if (!XFindCB(this, FullFileName, GetFileSize(FullFileName), pData))
					++ret;
				return ret;
			}
		}
		
		FILE_FIND_DATA FindData;
		if (!FindFirstFile(FullFileName, &FindData))
		{
			FindClose(&FindData);
			return ret;
		}
		
		char CurFilePath[MAX_FN_LEN];
		do {
			if ((FindData.FileName[0] == '.' && FindData.FileName[1] == 0) ||
			    (FindData.FileName[0] == '.' && FindData.FileName[1] == '.' && FindData.FileName[2] == 0))
				continue; //skip "." ".."
			{
				int Len = TStrCpy(CurFilePath, FilePath);
				if (Len && CurFilePath[Len-1] != m_pszSepar[0])
					TStrCat(CurFilePath, m_pszSepar);
				TStrCat(CurFilePath, FindData.FileName);
				if (!(FindData.FileAttribute & FILE_ATTRIBUTE_DIRECTORY) ||
				      FindData.FileAttribute & FILE_ATTRIBUTE_DEVICE)
				{
					if ((!(FindData.FileAttribute & 0x80000000) && FileName[0] == 0) ||
						TIMultiMatchWithPattern(FileName, CurFilePath))
					{
						if (!XFindCB(this, CurFilePath, FindData.FileLength, pData))
						{
							++ret;
						} else
						{
							*bReturnFlag = 1;
							break;
						}
					}
				} else
				if (bRoot)
				{
					if (FileName[0])
					{
						TStrCat(CurFilePath, "\\");
						TStrCat(CurFilePath, FileName);
					}
					ret += XFind(CurFilePath, XFindCB, pData, bRoot, bReturnFlag);
					if (*bReturnFlag) break;
				}
			}
		} while (FindNextFile(&FindData));
		FindClose(&FindData);
		return ret;
	}

	bool CFileIO::XCopy(CFileIO *pSrcIO, CFileIO *pDstIO, const char *pSrcPath, const char *pDstPath, FUNC_XCOPY_CB XCOPY, void *pData)
	{
		if (!pSrcIO->IsDir(pSrcPath))
		{
			unsigned int Length = pSrcIO->GetFileSize(pSrcPath);
			if (XCOPY)
			{
				int res = XCOPY(pSrcIO, pDstIO, pSrcPath, pDstPath, Length, pData);
				if (res == 1)
					return true; //continue recursive copy
				if (res == 2)
					return false; //finish copy
			}
			unsigned char *pFileData = new unsigned char[Length];
			pSrcIO->ReadFromFile(pSrcPath, pFileData, 0);
			pDstIO->WriteToFile(pDstPath, pFileData, Length);
			delete []pFileData;
			return true;
		}
		if (!pDstIO->IsDir(pDstPath) && !pDstIO->CreateDir(pDstPath))
			return false;

		FILE_FIND_DATA FileData;
		if (!pSrcIO->FindFirstFile(pSrcPath, &FileData))
		{
			pSrcIO->FindClose(&FileData);
			return true;
		}
		do {
			if ((FileData.FileName[0] == '.' && FileData.FileName[1] == 0) ||
			    (FileData.FileName[0] == '.' && FileData.FileName[1] == '.' && FileData.FileName[2] == 0))
				continue;
			{
				char DstFullPath[MAX_PATH];
				char SrcFullPath[MAX_PATH];
				int Pos;

				Pos = TStrCpy(SrcFullPath, pSrcPath);
				if (Pos && SrcFullPath[Pos-1] != pSrcIO->m_pszSepar[0])
					TStrCat(SrcFullPath, pSrcIO->m_pszSepar);
				TStrCat(SrcFullPath, FileData.FileName);

				Pos = TStrCpy(DstFullPath, pDstPath);
				if (Pos && DstFullPath[Pos-1] != pDstIO->m_pszSepar[0])
					TStrCat(DstFullPath, pDstIO->m_pszSepar);
				TStrCat(DstFullPath, FileData.FileName);

				if (!XCopy(pSrcIO, pDstIO, SrcFullPath, DstFullPath, XCOPY, pData))
					break;
			}
		} while (pSrcIO->FindNextFile(&FileData));
		pSrcIO->FindClose(&FileData);
		return true;
	}

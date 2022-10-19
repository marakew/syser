
#include "polyfs.hpp"
#include "compress.hpp"

	CPolyFS::CPolyFS()
		: m_pFileIO(gpFileIO)
		, m_MaxCachePageCount(256)
		, m_FatOff(0)
		, m_ClusSize(0)
		, m_ClusCount(0)
		, m_Style(0)
		, m_ZeroBuffer(nullptr)
	{
	}

	CPolyFS::~CPolyFS()
	{
		CloseDisk();
	}

	void CPolyFS::ChangeFileIO(CFileIO *pFileIO)
	{
		m_pFileIO = pFileIO;
	}

	bool CPolyFS::Format(const char *FileName, unsigned long ClusCount, unsigned long ClusSize)
	{
		unsigned long j;
		unsigned long i;
		unsigned long v8;
		PFS_FILE_ENTRY FileEntry;
		unsigned long v10;
		PFS_HEAD PFSHead;
		//int ClusCounta;

		if (ClusCount == 0)
			return false;

		if (ClusSize == 0)
			ClusSize = 4096;

		m_DiskImage.ChangeFileIO(m_pFileIO);
		if (!m_DiskImage.Create(FileName, 0))
			return false;

		m_ClusSize = ALIGN_SIZE(ClusSize, 256);

		memset(&PFSHead, 0, sizeof(PFS_HEAD));
		TStrCpy(PFSHead.Sign, "PFS");
		PFSHead.ClusCount = ALIGN_SIZE((ClusCount/m_ClusSize), (m_ClusSize/4));

		m_ClusCount = PFSHead.ClusCount;
		//ClusCounta = m_ClusSize * m_ClusCount;
		PFSHead.ClusSize = m_ClusSize;
		TStrCpy(PFSHead.FatEntry.FileName, "*FAT*");
		PFSHead.FatEntry.Attribute = 0x10; //PFS_SYSTEM
		PFSHead.FatEntry.BeginClus = 1;
		PFSHead.FatEntry.FileSize = 4 * PFSHead.ClusCount;
		PFSHead.FatEntry.NameCode = TGetNameCode(PFSHead.FatEntry.FileName);

		m_DiskImage.m_FileSize = m_ClusCount * m_ClusSize;
		m_Style = 0;
		InitParam();

		m_FatOff = m_ClusSize * PFSHead.FatEntry.BeginClus;

		if (!WriteDiskFile(0, &PFSHead, sizeof(PFS_HEAD)))
		{
			CloseDisk();
			return false;
		}

		v10 = (unsigned int)PFSHead.FatEntry.FileSize / m_ClusSize;
		for (i = 0; i < v10; ++i)
		{
			if (!WriteDiskFile(m_FatOff + m_ClusSize * i, m_ZeroBuffer, m_ClusSize))
			{
				CloseDisk();
				return false;
			}
		}

		v8 = -1;
		if (!WriteFatCell(0, &v8))
		{
			CloseDisk();
			return false;
		}
		
		for (j = 1; j < v10; ++j)
		{
			v8 = PFSHead.FatEntry.BeginClus + j;
			if (!WriteFatCell(j, &v8))
			{
				CloseDisk();
				return false;
			}
		}

		v8 = -1;
		if (!WriteFatCell(v10, &v8))
		{
			CloseDisk();
			return false;
		}

		
		memset(&FileEntry, 0, sizeof(PFS_FILE_ENTRY));
		FileEntry.BeginClus = 0;
		FileEntry.FileSize = m_ClusSize;
		CreateFile("*ROOT*", 0x11, m_ClusSize, &FileEntry, 0x40); //PFS_SYSTEM PFS_DIRECTORY
		ReadDiskFile(0x40, &m_RootEntry, sizeof(PFS_FILE_ENTRY));
		CloseDisk();
		return true;
	}

	bool CPolyFS::ReadDiskFile(unsigned long Pos, void *Buffer, unsigned long Size)
	{
		return m_PageMemory.ReadMemX(Pos, Buffer, Size) == Size;
	}

	bool CPolyFS::WriteDiskFile(unsigned long Pos, const void *Buffer, unsigned long Size)
	{
		return m_PageMemory.WriteMemX(Pos, Buffer, Size) == Size;
	}

	bool CPolyFS::ReadFatCell(unsigned long Index, void *Buffer)
	{
		return ReadDiskFile(m_FatOff + 4 * Index, Buffer, 4);
	}

	bool CPolyFS::WriteFatCell(unsigned long Index, const void *Buffer)
	{
		return WriteDiskFile(m_FatOff + 4 * Index, Buffer, 4);
	}

	bool CPolyFS::ReadClus(unsigned long Index, void *Buffer)
	{
		if (Index < m_ClusCount)
			return ReadDiskFile(m_ClusSize * Index, Buffer, m_ClusSize);
		return false;
	}

	bool CPolyFS::WriteClus(unsigned long Index, const void *Buffer)
	{
		if (Index < m_ClusCount)
			return WriteDiskFile(m_ClusSize * Index, Buffer, m_ClusSize);
		return false;
	}

	unsigned long CPolyFS::FileOffToDP(unsigned long ClusIndex, unsigned long Offset)
	{
		for (; Offset >= m_ClusSize; Offset -= m_ClusSize)
		{
			if (!ReadFatCell(ClusIndex, &ClusIndex))
				return 0;
			if (ClusIndex == -1)
				return 0;
		}
		return Offset + m_ClusSize * ClusIndex;
	}

	unsigned long CPolyFS::FileOffToClus(unsigned long ClusIndex, unsigned long Offset)
	{
		for (; Offset >= m_ClusSize; Offset -= m_ClusSize)
		{
			if (!ReadFatCell(ClusIndex, &ClusIndex))
				return -1;
			if (ClusIndex == -1)
				return -1;
		}
		return ClusIndex;
	}

	unsigned long CPolyFS::ReadFile(struct PFS_FILE_DATA *pHandleData, unsigned long Offset, void *Buffer, unsigned long Size)
	{
		if ( !(pHandleData->ThisFileEntry.Attribute & 0x20) ) //PFS_ARCHIVE
			return ReadFile(&pHandleData->ThisFileEntry, Offset, Buffer, Size);

		unsigned long FileSize;
		if (ReadFile(&pHandleData->ThisFileEntry, 0, &FileSize, 4) != 4)
			return 0;

		if (Offset || Size != FileSize)
			return 0;

		unsigned long nSize = pHandleData->ThisFileEntry.FileSize;
		unsigned char *pBuffer = new unsigned char[nSize];
		if (!pBuffer)
			return 0;

		if (ReadFile(&pHandleData->ThisFileEntry, 0, pBuffer, nSize) != nSize)
		{
			delete []pBuffer;
			return 0;
		}

		int sz = lzo_decompress(pBuffer, nSize, Buffer);

		delete []pBuffer;
		return Size;
	}

	unsigned long CPolyFS::WriteFile(struct PFS_FILE_DATA *pHandleData, unsigned long Offset, const void *Buffer, unsigned long Size)
	{
		if (Size + Offset > pHandleData->ThisFileEntry.FileSize)
		{
			if (!SetFileLength(pHandleData, Size + Offset))
				return 0;
		}

		//TODO lzo_compress may be in another funcs

		return WriteFile(&pHandleData->ThisFileEntry, Offset, Buffer, Size);
	}

	unsigned long CPolyFS::ReadFile(struct PFS_FILE_ENTRY *pFileEntry, unsigned long Offset, void *Buffer, unsigned long Size)
	{
		unsigned long v6;
		unsigned long n;
		unsigned long ClusIndex;
		unsigned long PageSize;
		unsigned long nBytes;
		unsigned long FileDP_v12;
		unsigned long FileDP_v12a;

		if (Offset >= pFileEntry->FileSize || !Size)
			return 0;
		nBytes = 0;
		PageSize = Offset % m_ClusSize;
		if (PageSize)
		{
			FileDP_v12 = FileOffToDP(pFileEntry->BeginClus, Offset);
			if (!FileDP_v12)
				return 0;
			v6 = MIN(m_ClusSize - PageSize, Size);
			if (!ReadDiskFile(FileDP_v12, Buffer, v6))
				return 0;
			nBytes = v6;
		}

		ClusIndex = FileOffToClus(pFileEntry->BeginClus, nBytes + Offset);
		for (n = (Size-nBytes)/m_ClusSize; n && nBytes + Offset < pFileEntry->FileSize; --n)
		{
			if (ClusIndex == -1 || !ReadClus(ClusIndex, nBytes + (char*)Buffer))
				return 0;
			nBytes += m_ClusSize;
			ClusIndex = FileOffToClus(ClusIndex, m_ClusSize);
		}

		if (Size != nBytes && nBytes + Offset < pFileEntry->FileSize)
		{
			FileDP_v12a = FileOffToDP(ClusIndex, (nBytes + Offset) % m_ClusSize);
			if (!FileDP_v12a)
				return nBytes;
			if (!ReadDiskFile(FileDP_v12a, nBytes + (char*)Buffer, Size - nBytes))
				return nBytes;
			nBytes = Size;
		}
		return nBytes;
	}

	unsigned long CPolyFS::WriteFile(struct PFS_FILE_ENTRY *pFileEntry, unsigned long Offset, const void *Buffer, unsigned long Size)
	{
		unsigned long v6;
		unsigned long n;
		unsigned long ClusIndex;
		unsigned long PageSize;
		unsigned long nBytes;
		unsigned long FileDP_v12;
		unsigned long FileDP_v13;

		if (Offset >= pFileEntry->FileSize || !Size)
			return 0;
		nBytes = 0;
		PageSize = Offset % m_ClusSize;
		if (PageSize)
		{
			FileDP_v12 = FileOffToDP(pFileEntry->BeginClus, Offset);
			if (!FileDP_v12)
				return 0;
			v6 = MIN(m_ClusSize - PageSize, Size);
			if (!WriteDiskFile(FileDP_v12, (void*)Buffer, v6))
				return 0;
			nBytes = v6;
		}

		ClusIndex = FileOffToClus(pFileEntry->BeginClus, nBytes + Offset);
		for (n = (Size - nBytes) / m_ClusSize; n && nBytes + Offset < pFileEntry->FileSize; --n)
		{
			if (ClusIndex == -1 || !WriteClus(ClusIndex, nBytes + (char*)Buffer))
				return 0;
			nBytes += m_ClusSize;
			ClusIndex = FileOffToClus(ClusIndex, m_ClusSize);
		}

		if (Size != nBytes && nBytes + Offset < pFileEntry->FileSize)
		{
			FileDP_v13 = FileOffToDP(ClusIndex, (nBytes + Offset) % m_ClusSize);
			if (!FileDP_v13)
				return nBytes;
			if (!WriteDiskFile(FileDP_v13, nBytes + (char*)Buffer, Size - nBytes) )
				return nBytes;
			nBytes = Size;
		}
		return nBytes;
	}

	unsigned long CPolyFS::FindFreeFileEntryFP(PFS_FILE_ENTRY *pParentEntry)
	{
		for (unsigned long Off = 0;
			Off < pParentEntry->FileSize;
			Off += sizeof(PFS_FILE_ENTRY))
		{
			PFS_FILE_ENTRY FileEntry;
			if (ReadFile(pParentEntry, Off, &FileEntry, sizeof(PFS_FILE_ENTRY)) != sizeof(PFS_FILE_ENTRY))
				return -1;
			if (!FileEntry.NameCode || FileEntry.NameCode == 0xFFFFFF)
				return Off;
		}
		return -1;
	}

	bool CPolyFS::CreateFile(const char *FileName, unsigned long Attr, unsigned long Size, PFS_FILE_ENTRY *pParentDirEntry, unsigned long ParentFileEntryFP)
	{
		unsigned long i;
		PFS_FILE_ENTRY FileEntry;
		unsigned long PrevClus;
		unsigned long n;
		unsigned long BeginClus;
		unsigned long nClus;
		unsigned long Off;

		nClus = PAGE_COUNT(Size, m_ClusSize);
		if (nClus)
		{
			Off = FindNextFreeClus(0);
			BeginClus = Off;
			for (i = 0; i < nClus - 1; ++i)
			{
				PrevClus = Off;
				Off = FindNextFreeClus(Off + 1);
				if (Off == -1)
					return false;
				if (!WriteFatCell(PrevClus, &Off))
					return false;
			}
			PrevClus = Off;
			Off = -1;
			if (!WriteFatCell(PrevClus, &Off))
				return false;
		} else
		{
			BeginClus = -1;
		}
		memset(&FileEntry, 0, sizeof(PFS_FILE_ENTRY));
		FileEntry.Attribute = Attr;
		FileEntry.FileSize = Size;
		FileEntry.BeginClus = BeginClus;
		TStrCpyLimit(FileEntry.FileName, FileName, 32);
		FileEntry.NameCode = TGetNameCode(FileEntry.FileName);
		if (WriteFile(pParentDirEntry, ParentFileEntryFP, &FileEntry, sizeof(PFS_FILE_ENTRY)) != sizeof(PFS_FILE_ENTRY))
			return false;

		if (Size == 0)
			return true;

		for (n = 0; n < Size && WriteFile(&FileEntry, n, m_ZeroBuffer, m_ClusSize); n += m_ClusSize)
		{
			;
		}
		return true;
	}

	bool CPolyFS::CreateFile(const char *FullFileName, unsigned long Attr, unsigned long Size, PFS_FILE_DATA *pHandleData)
	{
		PFS_FIND_DATA FindData;
		char FileName[256];
		char PathName[256];
		unsigned long FreeFileEntryFP;

		if (FullFileName[0] != '\\' )
			return false;

		if (FullFileName[1] == 0)
			return false;

		if (FindFirstFile(FullFileName, &FindData))
		{
			FreeFileEntryFP = FindData.FileEntryFP;
			if (!SetFileLength(FullFileName, Size))
				return false;
			
			pHandleData->FilePoint = 0;
			memcpy(&pHandleData->ParentEntry, &FindData.ParentEntry, sizeof(PFS_FILE_ENTRY));
			pHandleData->FileEntryFP = FreeFileEntryFP;
			return ReadFile(&pHandleData->ParentEntry, FreeFileEntryFP,
				&pHandleData->ThisFileEntry, sizeof(PFS_FILE_ENTRY)) == sizeof(PFS_FILE_ENTRY);
		}
		
		TStrCpy(FileName, TGetFileName(FullFileName));
		TGetFilePath(FullFileName, PathName);
		if (!PathName[0])
			TStrCpy(PathName, "\\");

		if (!FindFirstFile(PathName, &FindData))
			return false;

		FreeFileEntryFP = FindFreeFileEntryFP(&FindData.ThisFileEntry);
		if (FreeFileEntryFP == -1)
		{
			if (!SetFileLength(PathName, m_ClusSize + FindData.ThisFileEntry.FileSize))
			return false;
			FindData.ThisFileEntry.FileSize += m_ClusSize;
			if (PathName[0] == '\\' && !PathName[1])
				memcpy(&m_RootEntry, &FindData.ThisFileEntry, sizeof(PFS_FILE_ENTRY));
			FreeFileEntryFP = FindFreeFileEntryFP(&FindData.ThisFileEntry);
			if (FreeFileEntryFP == -1)
				return false;
		}

		pHandleData->FilePoint = 0;
		if (!CreateFile(FileName, Attr, Size, &FindData.ThisFileEntry, FreeFileEntryFP))
			return false;
		
		memcpy(&pHandleData->ParentEntry, &FindData.ThisFileEntry, sizeof(PFS_FILE_ENTRY));
		pHandleData->FileEntryFP = FreeFileEntryFP;
		return ReadFile(&pHandleData->ParentEntry, FreeFileEntryFP,
			&pHandleData->ThisFileEntry, sizeof(PFS_FILE_ENTRY)) == sizeof(PFS_FILE_ENTRY);
	}

	bool CPolyFS::OpenFile(const char *FullFileName, PFS_FILE_DATA *pHandleData)
	{
		PFS_FIND_DATA FindData;
		if (!FindFirstFile(FullFileName, &FindData))
			return false;

		pHandleData->FilePoint = 0;
		memcpy(&pHandleData->ThisFileEntry, &FindData.ThisFileEntry, sizeof(PFS_FILE_ENTRY));
		memcpy(&pHandleData->ParentEntry, &FindData.ParentEntry, sizeof(PFS_FILE_ENTRY));
		pHandleData->FileEntryFP = FindData.FileEntryFP;
		return true;
	}

	unsigned long CPolyFS::FindNextFreeClus(unsigned long ClusIdx)
	{
		for (unsigned long Index = ClusIdx; Index < m_ClusCount; ++Index)
		{
			unsigned long Data; //??? = 0;
			if (!ReadFatCell(Index, &Data))
				return -1;
			if (!Data)
				return Index;
		}
		return -1;
	}

	bool CPolyFS::SetFileLength(const char *FileName, unsigned long Length)
	{
		PFS_FILE_DATA FileData;
		if (!OpenFile(FileName, &FileData))
			return false;
		return SetFileLength(&FileData, Length);
	}

	bool CPolyFS::SetFileLength(PFS_FILE_DATA *pHandleData, unsigned long Length)
	{
		unsigned long WTSize;
		unsigned long ni;
		unsigned long Off;
		unsigned long Data;
		unsigned long PrevFatDP;
		unsigned long ClusIdx;
		unsigned long PrevIdx;
		unsigned long nNewClus;
		unsigned long nOldClus;

		nOldClus = PAGE_COUNT(Length, m_ClusSize);
		nNewClus = PAGE_COUNT(pHandleData->ThisFileEntry.FileSize, m_ClusSize);

		if (nOldClus <= nNewClus)
		{
			if (nOldClus >= nNewClus)
			{
				pHandleData->ThisFileEntry.FileSize = Length;
			} else
			{
				ClusIdx = FileOffToClus(pHandleData->ThisFileEntry.BeginClus, m_ClusSize * nOldClus);
				if (!DeleteFileClus(ClusIdx))
					return false;
				Data = -1;
				if (!WriteFatCell(ClusIdx, &Data))
					return false;
				pHandleData->ThisFileEntry.FileSize = Length;
			}
		} else
		{
			if (nNewClus)
			{
				ClusIdx = FileOffToClus(pHandleData->ThisFileEntry.BeginClus, m_ClusSize * (nNewClus - 1));
				if (ClusIdx == -1)
					return false;
			}

			for (ni = nNewClus; ni < nOldClus; ++ni)
			{
				if (ni)
				{
					PrevIdx = ClusIdx;
					ClusIdx = FindNextFreeClus(ClusIdx + 1);
					if (ClusIdx == -1)
						return false;
					if (!WriteFatCell(PrevIdx, &ClusIdx))
						return false;
				} else
				{
					PrevFatDP = FileOffToDP(pHandleData->ParentEntry.BeginClus,
								pHandleData->FileEntryFP);
					if (!PrevFatDP)
						return false;

					ClusIdx = FindNextFreeClus(1);
					pHandleData->ThisFileEntry.BeginClus = ClusIdx;
					if (!WriteDiskFile(PrevFatDP, &ClusIdx, 4))
						return false;
				}
			}
			Data = -1;
			if (!WriteFatCell(ClusIdx, &Data))
				return false;

			Off = pHandleData->ThisFileEntry.FileSize;
			pHandleData->ThisFileEntry.FileSize = Length;
			while (Off < pHandleData->ThisFileEntry.FileSize)
			{
				WTSize = MIN(m_ClusSize, pHandleData->ThisFileEntry.FileSize - Off);
				WriteFile(&pHandleData->ThisFileEntry, Off, m_ZeroBuffer, WTSize);
				Off += WTSize;
			}
		}
		return WriteFile(&pHandleData->ParentEntry, pHandleData->FileEntryFP + 4, &Length, 4) == 4;
	}

	unsigned long CPolyFS::GetFileLength(const char *FileName)
	{
		PFS_FIND_DATA FindData;
		if (!FindFirstFile(FileName, &FindData))
			return 0;
		if ( !(FindData.ThisFileEntry.Attribute & 0x20) ) //PFS_ARCHIVE
			return FindData.ThisFileEntry.FileSize;
		unsigned long Length;
		if (ReadFile(&FindData.ThisFileEntry, 0, &Length, 4) != 4)
			return 0;
		return Length;
	}

	bool CPolyFS::DeleteFile(const char *FullFileName)
	{
		if (FullFileName[0] != '\\' || FullFileName[1] == 0)
			return false;

		PFS_FIND_DATA FindData;
		if (!FindFirstFile(FullFileName, &FindData))
			return false;
		if (!DeleteFileClus(FindData.ThisFileEntry.BeginClus))
			return false;

		PFS_FILE_ENTRY FileEntry;
		memset(&FileEntry, 0, sizeof(PFS_FILE_ENTRY));
		FileEntry.NameCode = 0xFFFFFF;
		return WriteFile(&FindData.ParentEntry, FindData.FileEntryFP, &FileEntry, sizeof(PFS_FILE_ENTRY)) == sizeof(PFS_FILE_ENTRY);
	}

	bool CPolyFS::DeleteFileClus(unsigned long BeginClus)
	{
		unsigned long nThisClus = BeginClus;
		unsigned long Data = 0;
		while (nThisClus != -1)
		{
			unsigned long nNextClus = nThisClus;
			if (!ReadFatCell(nThisClus, &nThisClus))
				return false;
			if (!WriteFatCell(nNextClus, &Data))
				return false;
		}
		return true;
	}

	bool CPolyFS::FindFirstFile(const char *FullFileName, PFS_FIND_DATA *pFindData)
	{
		//char FileName[MAX_FN_LEN];
		//const char *pStr;
		//PFS_FIND_DATA FindData;

		if (FullFileName[0] == '\\')
		{
			PFS_FIND_DATA FindData;
			memset(&FindData, 0, sizeof(PFS_FIND_DATA));
			memcpy(&FindData.ThisFileEntry, &m_RootEntry, sizeof(PFS_FILE_ENTRY));
			FindData.ParentEntry.BeginClus = 0;
			FindData.ParentEntry.FileSize = 0x70;
			FindData.ParentEntry.Attribute = m_RootEntry.Attribute;
			FindData.FileEntryFP = 0x40;
			const char *pStr = FullFileName;
			while (*pStr)
			{
				char FileName[MAX_FN_LEN];
				int Length = 0;
				while (*pStr && *pStr != '\\' && Length < MAX_FN_LEN-1)
					FileName[Length++] = *pStr++;
				FileName[Length] = 0;
				if (Length)
				{
					memcpy(&FindData.ParentEntry, &FindData.ThisFileEntry, sizeof(PFS_FILE_ENTRY));
					if (!FindFirstFile(FileName, &FindData))
						return false;
				}
				while (*pStr == '\\') ++pStr;
			}
			memcpy(pFindData, &FindData, sizeof(PFS_FIND_DATA));
			return true;
		} 

		for (unsigned long Off = 0;
			Off < pFindData->ParentEntry.FileSize;
			Off += sizeof(PFS_FILE_ENTRY))
		{
			if (ReadFile(&pFindData->ParentEntry, Off, &pFindData->ThisFileEntry, sizeof(PFS_FILE_ENTRY)) != sizeof(PFS_FILE_ENTRY))
				return false;
			if (CmpFileName(FullFileName, &pFindData->ThisFileEntry))
			{
				TStrCpyLimit(pFindData->FileName, pFindData->ThisFileEntry.FileName, 256);
				pFindData->FileEntryFP = Off;
				TStrCpyLimit(pFindData->PattenName, FullFileName, 256);
				return true;
			}
		}
		return false;
	}

	bool CPolyFS::FindNextFile(PFS_FIND_DATA *pFindData)
	{
		for (unsigned long Off = pFindData->FileEntryFP + sizeof(PFS_FILE_ENTRY);
			Off < pFindData->ParentEntry.FileSize;
			Off += sizeof(PFS_FILE_ENTRY))
		{
			if (ReadFile(&pFindData->ParentEntry, Off, &pFindData->ThisFileEntry, sizeof(PFS_FILE_ENTRY)) != sizeof(PFS_FILE_ENTRY))
					return false;
			if (!pFindData->ThisFileEntry.NameCode)
				return false;
			if (CmpFileName(pFindData->PattenName, &pFindData->ThisFileEntry))
			{
				TStrCpyLimit(pFindData->FileName, pFindData->ThisFileEntry.FileName, 256);
				pFindData->FileEntryFP = Off;
				return true;
			}
		}
		return false;
	}

	bool CPolyFS::CmpFileName(const char *FilePatternName, PFS_FILE_ENTRY *pFileEntry)
	{
		if (!pFileEntry->FileName[0])
			return false;
		if (FilePatternName[0] == '*' && FilePatternName[1] == '0')
			return true;
		if (TStrChr(FilePatternName, '*') || TStrChr(FilePatternName, '?'))
			return TIMultiMatchWithPattern(FilePatternName, pFileEntry->FileName);
		return TStrICmp(FilePatternName, pFileEntry->FileName) == 0;
	}

	bool CPolyFS::SetFileAttribute(const char *FileName, unsigned long Attr)
	{
		PFS_FIND_DATA FindData;
		if (!FindFirstFile(FileName, &FindData))
			return false;

		return WriteFile(&FindData.ParentEntry, FindData.FileEntryFP + 8, &Attr, 4) == 4;
	}

	unsigned long CPolyFS::GetFileAttribute(const char *FileName)
	{
		PFS_FIND_DATA FindData;
		if (!FindFirstFile(FileName, &FindData))
			return 0;
		return FindData.ThisFileEntry.Attribute;
	}

	void CPolyFS::InitParam()
	{
		unsigned long PageCount;
		if (m_Style & 2)
			PageCount = PAGE_COUNT(m_DiskImage.m_FileSize, 4096);
		else	PageCount = MIN(PAGE_COUNT(m_DiskImage.m_FileSize, 4096), m_MaxCachePageCount);

		m_PageMemory.Init(PageCount, (m_Style & 1) == 0);
		m_DiskImage.m_ImageBase = 0;
		m_DiskImage.m_ImageSize = m_ClusCount * m_ClusSize;
		m_DiskImage.m_ImageHighBase = m_DiskImage.m_ImageBase + m_DiskImage.m_ImageSize;
		m_PageMemory.MapImage(&m_DiskImage);
		m_ZeroBuffer = new unsigned char[m_ClusSize];
		if (m_Style & 2)
		{
			for (unsigned long Address = m_DiskImage.m_ImageBase; Address < PageCount<<12; Address += m_ClusSize)
				ReadDiskFile(Address, m_ZeroBuffer, m_ClusSize);
		}
		memset(m_ZeroBuffer, 0, m_ClusSize);
	}

	bool CPolyFS::OpenDisk(const char *FileName, unsigned int Style)
	{
		PFS_HEAD PFSHead;

		m_Style = Style;
		m_DiskImage.ChangeFileIO(m_pFileIO);

		//hack++
		//m_DiskImage.m_RMObj = &m_DiskImage;
		//m_DiskImage.m_WRObj = &m_DiskImage;

		if (!m_DiskImage.Open(FileName, 0))
			return false;
		if (!m_DiskImage.ReadFile(&PFSHead, sizeof(PFSHead)))
			return false;
		if (TStrNCmp(PFSHead.Sign, "PFS", 3))
		{
			m_DiskImage.Close();
			return false;
		}
		m_ClusSize = PFSHead.ClusSize;
		m_ClusCount = PFSHead.ClusCount;
		m_FatOff = m_ClusSize * PFSHead.FatEntry.BeginClus;
		memcpy(&m_RootEntry, &PFSHead.RootEntry, sizeof(m_RootEntry));
		InitParam();
		return true;
	}

	void CPolyFS::CloseDisk()
	{
		if (m_DiskImage.m_hHandle || m_DiskImage.m_Buffer)
		{
			if ( !(m_Style & 1) )
				m_PageMemory.FlushAllPage();
			m_PageMemory.Release();
			SafeDeleteAry(m_ZeroBuffer);
			m_DiskImage.Close();
		}
	}


///
	CPFSFileIO::CPFSFileIO()
	{
		m_pszSepar = "\\";
		m_pFileIO = 0;
	}


	void CPFSFileIO::ChangeFileIO(CFileIO *pFileIO)
	{
		m_pFileIO = pFileIO;
	}

	bool CPFSFileIO::OpenDisk(const char *FileName, unsigned int Style)
	{
		m_Handle.Reset();
		m_Handle.Register(0, 0);

		TListIter<CPolyFS> It = m_PolyFSDisk.Append();
		if (m_pFileIO) It->ChangeFileIO(m_pFileIO);
		if (!It->OpenDisk(FileName, Style))
		{
			m_PolyFSDisk.Remove(It);
			return false;
		}
		return true;
	}

	void CPFSFileIO::CloseDisk()
	{
		m_Handle.CloseAllHandle();
		m_PolyFSDisk.Clear();
	}


	bool CPFSFileIO::OpenFile(const char *FileName, HANDLE *phFile, unsigned long Mode)
	{
		TListIter<CPolyFS> It = m_PolyFSDisk.Begin();
		for (; It != m_PolyFSDisk.End(); ++It)
		{
			PFS_FILE_DATA FileData;
			if (It->OpenFile(FileName, &FileData))
			{
				MY_PFS_FILE_DATA *pPFSFileData = new MY_PFS_FILE_DATA;
				memcpy(&pPFSFileData->m_FileData, &FileData, sizeof(PFS_FILE_DATA));
				pPFSFileData->m_pPolyFS = &*It;
				*phFile = (HANDLE)m_Handle.CreateHandle(pPFSFileData, 1);
				return true;
			}
		}
		return false;
	}

	bool CPFSFileIO::CreateFile(const char *FileName, HANDLE *phFile, unsigned long Mode)
	{
		if (m_PolyFSDisk.Size() == 0)
			return false;

		TListIter<CPolyFS> It = m_PolyFSDisk.Begin();

		PFS_FILE_DATA FileData;
		if (!It->CreateFile(FileName, 2, 0, &FileData)) //PFS_???
			return false;

		MY_PFS_FILE_DATA *pPFSFileData = new MY_PFS_FILE_DATA;
		memcpy(&pPFSFileData->m_FileData, &FileData, sizeof(PFS_FILE_DATA));
		pPFSFileData->m_pPolyFS = &*It;
		*phFile = (HANDLE)m_Handle.CreateHandle(pPFSFileData, 1);
		return true;
	}

	bool CPFSFileIO::CreateDir(const char *FileName)
	{
		if (m_PolyFSDisk.Size() == 0)
			return false;

		PFS_FILE_DATA FileData;
		return m_PolyFSDisk.Begin()->CreateFile(FileName, 1, //PFS_DIRECTORY
					m_PolyFSDisk.Begin()->m_ClusSize, &FileData);
	}

	bool CPFSFileIO::CloseHandle(HANDLE hFile)
	{
		return m_Handle.CloseHandle((ULONG_PTR)hFile);
	}

	unsigned long CPFSFileIO::ReadFile(HANDLE hFile, void *Buffer, unsigned long Size, unsigned __int64 Offset)
	{
		MY_PFS_FILE_DATA *pInfo = (MY_PFS_FILE_DATA *)m_Handle.GetHandleInfo((ULONG_PTR)hFile);
		if (!pInfo) return 0;
		return pInfo->m_pPolyFS->ReadFile(&pInfo->m_FileData, Offset, Buffer, Size);
	}
	unsigned long CPFSFileIO::WriteFile(HANDLE hFile, const void *Buffer, unsigned long Size, unsigned __int64 Offset)
	{
		MY_PFS_FILE_DATA *pInfo = (MY_PFS_FILE_DATA *)m_Handle.GetHandleInfo((ULONG_PTR)hFile);
		if (!pInfo) return 0;
		return pInfo->m_pPolyFS->WriteFile(&pInfo->m_FileData, Offset, Buffer, Size);
	}

	unsigned long CPFSFileIO::ReadFile(HANDLE hFile, void *Buffer, unsigned long Size)
	{
		return 0;
	}

	unsigned long CPFSFileIO::WriteFile(HANDLE hFile, const void *Buffer, unsigned long Size)
	{
		return 0;
	}

	unsigned __int64 CPFSFileIO::SetFilePointer(HANDLE hFile, __int64 Offset, unsigned long Pos)
	{
		return 0;
	}

	unsigned __int64 CPFSFileIO::GetFileLength(HANDLE hFile)
	{
		MY_PFS_FILE_DATA *pInfo = (MY_PFS_FILE_DATA *)m_Handle.GetHandleInfo((ULONG_PTR)hFile);
		if (!pInfo) return -1;
		if ( !(pInfo->m_FileData.ThisFileEntry.Attribute & 0x20) ) //PFS_ARCHIVE
			return pInfo->m_FileData.ThisFileEntry.FileSize;
		unsigned long Length;
		if (pInfo->m_pPolyFS->ReadFile(&pInfo->m_FileData.ThisFileEntry, 0, &Length, 4) == 4)
			return Length;
		return 0;
	}

	bool CPFSFileIO::SetFileLength(HANDLE hFile, unsigned __int64 Length)
	{
		MY_PFS_FILE_DATA *pInfo = (MY_PFS_FILE_DATA *)m_Handle.GetHandleInfo((ULONG_PTR)hFile);
		if (!pInfo) return false;
		return pInfo->m_pPolyFS->SetFileLength(&pInfo->m_FileData, Length);
	}

	bool CPFSFileIO::DeleteFile(const char *FileName)
	{
		int Count = 0;
		TListIter<CPolyFS> It = m_PolyFSDisk.Begin();
		for (; It != m_PolyFSDisk.End(); ++It)
			if (It->DeleteFile(FileName))
				Count++;
		return Count>0;
	}

	bool CPFSFileIO::RenameFile(const char *FileName, const char *NewFileName)
	{
		return false;
	}

	bool CPFSFileIO::SetFileAttr(const char *FileName, unsigned long Attr)
	{
		unsigned long PFSAttr = 0;
		if (Attr & 0x01) PFSAttr  = 0x04; //FILE_ATTRIBUTE_READONLY	PFS_READONLY	0x04
		if (Attr & 0x04) PFSAttr |= 0x10; //FILE_ATTRIBUTE_SYSTEM	PFS_SYSTEM	0x10
		if (Attr & 0x10) PFSAttr |= 0x01; //FILE_ATTRIBUTE_DIRECTORY	PFS_DIRECTORY	0x01
		if (Attr & 0x02) PFSAttr |= 0x08; //FILE_ATTRIBUTE_HIDDEN	PFS_HIDDEN	0x08
		if (Attr & 0x20) PFSAttr |= 0x20; //FILE_ATTRIBUTE_ARCHIVE	PFS_ARCHIVE	0x20
		int Count = 0;
		TListIter<CPolyFS> It = m_PolyFSDisk.Begin();
		for (; It != m_PolyFSDisk.End(); ++It)
			if (It->SetFileAttribute(FileName, PFSAttr))
				Count++;
		return Count>0;
	}

	unsigned long CPFSFileIO::GetFileAttr(const char *FileName)
	{
		TListIter<CPolyFS> It = m_PolyFSDisk.Begin();
		for (; It != m_PolyFSDisk.End(); ++It)
		{
			unsigned long PFSAttr = It->GetFileAttribute(FileName);
			if (PFSAttr != 0)
			{
				unsigned long FIOAttr = 0;
				if (PFSAttr & 0x10) FIOAttr  = 0x04; //FILE_ATTRIBUTE_SYSTEM
				if (PFSAttr & 0x04) FIOAttr |= 0x01; //FILE_ATTRIBUTE_READONLY
				if (PFSAttr & 0x08) FIOAttr |= 0x02; //FILE_ATTRIBUTE_HIDDEN
				if (PFSAttr & 0x01) FIOAttr |= 0x10; //FILE_ATTRIBUTE_DIRECTORY
				if (PFSAttr & 0x20) FIOAttr |= 0x20; //FILE_ATTRIBUTE_ARCHIVE
				return FIOAttr;
			}
		}

		return -1;
	}

	bool CPFSFileIO::IsDir(const char *FileName)
	{
		PFS_FIND_DATA FindData;
		TListIter<CPolyFS> It = m_PolyFSDisk.Begin();
		for (; It != m_PolyFSDisk.End(); ++It)
		{
			if (It->FindFirstFile(FileName, &FindData))
				return (FindData.ThisFileEntry.Attribute & 1) != 0; //PFS_DIRECTORY
		}
		return false;
	}

	bool CPFSFileIO::DeleteDir(const char *FileName)
	{
		char szBuffer[MAX_FN_LEN];
		TStrCpy(szBuffer, FileName); //??? Limit 260
		TStrCat(szBuffer, "\\*");

		TListIter<CPolyFS> It = m_PolyFSDisk.Begin();
		for (; It != m_PolyFSDisk.End(); ++It)
		{
			PFS_FIND_DATA FindData;
			if (It->FindFirstFile(FileName, &FindData))
				return It->DeleteFile(szBuffer);
		}
		return false;
	}

	bool CPFSFileIO::FindFirstFile(const char *FullFileName, FILE_FIND_DATA *pFindData)
	{
		char FileName[MAX_FN_LEN];
		PFS_FIND_DATA FindData;
		MY_PFS_FIND_DATA *pPFSFindData;

		memset(pFindData, 0, 0x118); //sizeof
		int Len = TStrCpy(FileName, FullFileName); //??? Limit 260
		MAX_LIMIT(Len, MAX_FN_LEN-2);
		if (Len && FileName[Len-1] == '\\')
			TStrCat(FileName, "*");
		else	TStrCat(FileName, "\\*");

		TListIter<CPolyFS> It = m_PolyFSDisk.Begin();
		for (; It != m_PolyFSDisk.End(); ++It)
		{
			if (It->FindFirstFile(FileName, &FindData))
				break;
		}
		if (It == m_PolyFSDisk.End())
			return false;

		pPFSFindData = new MY_PFS_FIND_DATA;
		memcpy(&pPFSFindData->m_FindData, &FindData, sizeof(PFS_FIND_DATA));
		pPFSFindData->m_pPolyFS = &*It;
		pFindData->FindDataBuffer = pPFSFindData;
		TStrCpy(pFindData->FileName, pPFSFindData->m_FindData.FileName);
		pFindData->FileAttribute = 0;

		if (FindData.ThisFileEntry.Attribute & 0x04)
				pFindData->FileAttribute |= 0x01; //FILE_ATTRIBUTE_READONLY
		if (FindData.ThisFileEntry.Attribute & 0x08)
				pFindData->FileAttribute |= 0x02; //FILE_ATTRIBUTE_HIDDEN
		if (FindData.ThisFileEntry.Attribute & 0x10)
				pFindData->FileAttribute |= 0x04; //FILE_ATTRIBUTE_SYSTEM
		if (FindData.ThisFileEntry.Attribute & 0x01)
				pFindData->FileAttribute |= 0x10; //FILE_ATTRIBUTE_DIRECTORY
		if (FindData.ThisFileEntry.Attribute & 0x20)
				pFindData->FileAttribute |= 0x20; //FILE_ATTRIBUTE_ARCHIVE

		pFindData->FileLength = pPFSFindData->m_FindData.ThisFileEntry.FileSize;
		pFindData->hFindFile = 0;
		return true;
	}

	bool CPFSFileIO::FindNextFile(FILE_FIND_DATA *pFindData)
	{
		MY_PFS_FIND_DATA *pPFSFindData = (MY_PFS_FIND_DATA *)pFindData->FindDataBuffer;
		if (!pPFSFindData->m_pPolyFS->FindNextFile(&pPFSFindData->m_FindData))
			return false;

		TStrCpy(pFindData->FileName, pPFSFindData->m_FindData.FileName);
		pFindData->FileAttribute = 0;

		if (pPFSFindData->m_FindData.ThisFileEntry.Attribute & 0x04)
				pFindData->FileAttribute |= 0x01; //FILE_ATTRIBUTE_READONLY
		if (pPFSFindData->m_FindData.ThisFileEntry.Attribute & 0x08)
				pFindData->FileAttribute |= 0x02; //FILE_ATTRIBUTE_HIDDEN
		if (pPFSFindData->m_FindData.ThisFileEntry.Attribute & 0x10)
				pFindData->FileAttribute |= 0x04; //FILE_ATTRIBUTE_SYSTEM
		if (pPFSFindData->m_FindData.ThisFileEntry.Attribute & 0x01)
				pFindData->FileAttribute |= 0x10; //FILE_ATTRIBUTE_DIRECTORY
		if (pPFSFindData->m_FindData.ThisFileEntry.Attribute & 0x20)
				pFindData->FileAttribute |= 0x20; //FILE_ATTRIBUTE_ARCHIVE

		pFindData->FileLength = pPFSFindData->m_FindData.ThisFileEntry.FileSize;
		pFindData->hFindFile = 0;
		return true;
	}

	bool CPFSFileIO::FindClose(FILE_FIND_DATA *pFindData)
	{
		MY_PFS_FIND_DATA *pPFSFindData = (MY_PFS_FIND_DATA *)pFindData->FindDataBuffer;
		delete pPFSFindData;
		return true;
	}


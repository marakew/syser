
#include "pefile.hpp"
#include <string.h>

	CPEFile::CPEFile()
		: m_OpenMode(7) //??? OPEN_DLL OPEN_SYS
		, m_ErrorCode(0)
		, m_PEOff(0)
		, m_Section(nullptr)
		, m_SectCount(0)
		, m_ImportModule(nullptr)
		, m_ImportModuleCount(0)
		, m_ExportFunc(nullptr)
		, m_ExportFuncCount(0)
		, m_ResDir(nullptr)
		, m_ResDirCount(0)
		, m_BaseReloc(nullptr)
		, m_BaseRelocCount(0)
		, m_TLSData(nullptr)
		, m_TLSDataSize(0)
		, m_AddressOfTLSIndex(0)
		, m_TLSCallBackFuncs(nullptr)
		, m_TLSCallBackCount(0)
	{
	}

	bool CPEFile::MapToMemory(unsigned long MemAddr, ULONG_PTR *pPhysAddr, bool bImageBase)
	{
		for (int i = 0; i <= m_SectCount; ++i) //??? =
		{
			if (MemAddr >= m_Section[i].PointerToRawData &&
				MemAddr < m_Section[i].PointerToRawData + m_Section[i].SizeOfRawData)
			{
				if (pPhysAddr)
				{
					*pPhysAddr = MemAddr + m_Section[i].VirtualAddress - m_Section[i].PointerToRawData;
					if (bImageBase)
						*pPhysAddr += m_PEHead.ImageBase;
				}
				return true;
			}
		}
		return false;
	}

	bool CPEFile::MapToFile(ULONG_PTR MemAddr, unsigned long *pPhysAddr, bool bImageBase)
	{
		if (bImageBase)
			MemAddr -= m_PEHead.ImageBase;
		for (int i = 0; i <= m_SectCount; ++i) //??? =
		{
			if (MemAddr >= m_Section[i].VirtualAddress
				&& MemAddr < m_Section[i].VirtualAddress + m_Section[i].VirtualSize
				&& MemAddr - m_Section[i].VirtualAddress <= m_Section[i].SizeOfRawData)
			{
				if (pPhysAddr)
					*pPhysAddr = MemAddr + m_Section[i].PointerToRawData - m_Section[i].VirtualAddress;
				return true;
			}
		}
		return false;
	}

	unsigned long CPEFile::GetPageAttribute(ULONG_PTR Address)
	{
		if (Address < m_ImageBase || Address >= m_ImageHighBase)
			return 0;

		unsigned long VirtualAddress = Address - m_ImageBase;
		for (int i = 0; i <= m_SectCount; ++i) //??? =
		{
			if (VirtualAddress >= m_Section[i].VirtualAddress &&
				VirtualAddress < m_Section[i].VirtualAddress + m_Section[i].VirtualSize)
			{
				unsigned long Attr = 0;
				if (m_Section[i].Characteristics & 0x20000000) //IMAGE_SCN_MEM_EXECUTE Executable section
					Attr = 2;
				if (m_Section[i].Characteristics & 0x40000000) //IMAGE_SCN_MEM_READ Readable section
					Attr |= 2;
				if (m_Section[i].Characteristics & 0x80000000) //IMAGE_SCN_MEM_WRITE Writable section
					Attr |= 6;
				if (m_Section[i].Characteristics == 0)
					Attr = 6;
				return Attr;
			}
		}
		return 6;
	}

	unsigned long CPEFile::ReadImageMemory(ULONG_PTR Address, void *Buffer, unsigned long Size)
	{
		unsigned long Len;

		unsigned long ReadSize = 0;
		if (Address < m_ImageBase || Address >= m_ImageHighBase)
			return 0;

		if (Address < m_ImageHighBase && Address+Size > m_ImageHighBase)
			Size = m_ImageHighBase - Address;

		unsigned long Pos = Address - m_ImageBase;

		if (!m_SectCount)
			return CImageFile::ReadFile(Pos, Buffer, Size);

		unsigned long dwSize = Size;
		memset(Buffer, 0, Size);
		for (int n = 0; n <= m_SectCount && dwSize; ++n)
		{
			PE_SECTION *pCurSec = &m_Section[n];
			unsigned long RangeStart = pCurSec->VirtualAddress;
			unsigned long RangeEnd = pCurSec->VirtualSize + pCurSec->VirtualAddress;
			if (Pos >= RangeStart && Pos < RangeEnd)
			{
				if (Pos >= RangeStart + pCurSec->SizeOfRawData)
				{
					if (Pos < RangeEnd)
					{
						if (dwSize <= RangeEnd - Pos)
						{
							Len = dwSize;
							dwSize = 0;
						} else
						{
							dwSize -= RangeEnd - Pos;
							Len = RangeEnd - Pos;
						}
						ReadSize += Len;
						Pos += Len;
					}
				} else
				{
					unsigned long LeftSize = RangeStart + pCurSec->SizeOfRawData - Pos;
					if (ReadSize <= LeftSize)
					{
						Len = dwSize;
						ReadSize = 0;
						CImageFile::ReadFile(
							Pos + pCurSec->PointerToRawData - RangeStart,
							&((char*)Buffer)[ReadSize], Len);
					} else
					{
						dwSize -= LeftSize;
						Len = pCurSec->VirtualAddress + pCurSec->SizeOfRawData - Pos;
						CImageFile::ReadFile(
							Pos + pCurSec->PointerToRawData - RangeStart,
							 &((char*)Buffer)[ReadSize], LeftSize);
					}
					ReadSize += Len;
					Pos += Len;
				}
			}
		}
		return Size;
	}

	unsigned long CPEFile::WriteImageMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size)
	{
		//???
		return 0;
	}

	bool CPEFile::LoadPage(ULONG_PTR Address, void *Buffer)
	{
		unsigned short TypeOffset_v4;
		int v5;
		unsigned short TypeOffset_v6;
		unsigned int k;
		int j;
		//int i;
		int Delta;
		//unsigned long VirtualAddress;
		signed int Size;
		unsigned short Type_v14;
		unsigned short Type_v15;

		if (!m_SectCount || !m_Section)
			return false;

		memset(Buffer, 0, 0x1000);
	#ifdef _X64_
		unsigned long VirtualAddress = (Address & 0xFFFFFFFFFFFFF000ULL) - m_ImageBase;
	#else
		unsigned long VirtualAddress = (Address & 0xFFFFF000) - m_ImageBase;
	#endif

		int i;
		for (i = 0; ; ++i)
		{
			if (i > m_SectCount)
				return false;

			if (VirtualAddress >= m_Section[i].VirtualAddress
				&& VirtualAddress < m_Section[i].VirtualSize + m_Section[i].VirtualAddress)
			{
				break;
			}
		}

		Size = m_Section[i].SizeOfRawData + m_Section[i].VirtualAddress - VirtualAddress;
		if (Size <= 0)
			return true;

		
		MAX_LIMIT(Size, 4096);

		CImageFile::ReadFile(
			m_Section[i].PointerToRawData + VirtualAddress - m_Section[i].VirtualAddress,
			Buffer,  Size);

		if (m_ImageBase == m_PEHead.ImageBase)
			return true;

		for (j = 0; j < m_BaseRelocCount; ++j)
		{
			if (VirtualAddress == m_BaseReloc[j].VirtualAddr + 4096)
			{
				if (m_BaseReloc[j].Count)
				{
					TypeOffset_v4 = m_BaseReloc[j].TypeOffset[m_BaseReloc[j].Count - 1];
					Type_v14 = TypeOffset_v4 & 0xFFF;
					if ( (unsigned __int16)((signed int)TypeOffset_v4 >> 12) == 3 && (signed int)Type_v14 > 4092)
					{
						Delta = m_ImageBase - m_PEHead.ImageBase;
						switch (Type_v14)
						{
						case 0xFFD:
							v5 = Delta + ((((char*)Buffer)[Type_v14 + 2] << 16) | ((char*)Buffer)[Type_v14]);
							*(short*)&((char*)Buffer)[Type_v14] = Delta + ((char*)Buffer)[Type_v14];
						//	((char*)Buffer)[Type_v14 + 2] = (Delta + ((((char*)Buffer)[Type_v14 + 2] << 16) | (unsigned int)((char*)Buffer)[Type_v14])) >> 16;
							((char*)Buffer)[Type_v14 + 2] = v5 >> 16;
							break;
						case 0xFFE:
							*(short *)&((char*)Buffer)[Type_v14] += Delta;
							break;
						case 0xFFF:
							((char*)Buffer)[Type_v14] += Delta;
							break;
						}
					}
				}
			}

			if (VirtualAddress == m_BaseReloc[j].VirtualAddr)
			{
				for (k = 0; k < m_BaseReloc[j].Count; ++k)
				{
					TypeOffset_v6 = m_BaseReloc[j].TypeOffset[k];
					Type_v15 = TypeOffset_v6 & 0xFFF;
					if ( (unsigned __int16)((signed int)TypeOffset_v6 >> 12) == 3 && (signed int)Type_v15 <= 4092)
					{
						Delta = m_ImageBase - m_PEHead.ImageBase;
						*(long*)&((char*)Buffer)[Type_v15] += Delta;//m_ImageBase
					//	+ *(long*)&((char*)Buffer)[Type_v15]
					//	- m_PEHead.ImageBase;
					}
				}
			}
		}
		
		return true;
	}

	bool CPEFile::Open(const char *FileName, HANDLE HandleOpen)
	{
		signed int i;
		int MinLowSectAddrFilePos;
		int FixAddress;

		if (!CImageFile::Open(FileName, HandleOpen))
			return false;

		MZHEAD HeadBuffer;
		if (!ReadMemory(m_ImageBase, &HeadBuffer, sizeof(MZHEAD)))
		{
			CImageFile::Close();
			return false;
		}

		if (HeadBuffer.Signature != 0x4D5A &&
		    HeadBuffer.Signature != 0x5A4D)
		{
			CImageFile::Close();
			return false;
		}

		m_PEOff = HeadBuffer.pfNewHead;
		if (m_PEOff >= m_FileSize - sizeof(PE_HEAD))
		{
			CImageFile::Close();
			return false;
		}

		if (!ReadMemory(m_PEOff + m_ImageBase, &m_PEHead, sizeof(PE_HEAD)))
		{
			CImageFile::Close();
			return false;
		}

		m_FileType = 0x1313;

		for (int i = 0; i < 16; ++i)
		{
			//TODO
		}

		if (m_PEHead.Signature != 0x4550)
		{
			CImageFile::Close();
			m_ErrorCode = PE_ERROR_SIGN;
			return false;
		}

		if (m_PEHead.Characteristics & 0x2000) //DLL
		{
			if (!(m_OpenMode & PE_OPEN_DLL))
			{
				CImageFile::Close();
				m_ErrorCode = PE_ERROR_MODULE_TYPE;
				return false;
			}
			m_FileType = 0x2313;
		}

		if (m_PEHead.Subsystem == 1) //SYS
		{
			if (!(m_OpenMode & PE_OPEN_SYS))
			{
				CImageFile::Close();
				m_ErrorCode = PE_ERROR_MODULE_TYPE;
				return false;
			}
			m_FileType = 0x4313;
		}

	#ifdef _X64_
		if (m_PEHead.Machine != 0x8664)	//IMAGE_FILE_MACHINE_AMD64
	#else
		if (m_PEHead.Machine != 0x14C)	//IMAGE_FILE_MACHINE_I386
	#endif
		{
			if (m_OpenMode & 8) //PE_???
			{
				CImageFile::Close();
				m_ErrorCode = PE_ERROR_MODULE_TYPE;
				return false;
			}
			m_FileType |= 4; //???
		}

		unsigned int SectionAlignment = m_PEHead.SectionAlignment;
		unsigned int FileAlignment = m_PEHead.FileAlignment;

		if (SectionAlignment == 0 || SectionAlignment > 65536)
				SectionAlignment = 4096;

		if (FileAlignment == 0 || FileAlignment > 8192)
				FileAlignment = 512;

		if (m_PEHead.NumberOfSections < 1 || m_PEHead.NumberOfSections > 128)
		{
			CImageFile::Close();
			m_ErrorCode = PE_ERROR_SECTION;
			return false;
		}
		
		m_Section = new PE_SECTION[m_PEHead.NumberOfSections + 1];
		if (!m_Section)
		{
			CImageFile::Close();
			m_ErrorCode = PE_ERROR_SECTION;
			return false;
		}


		if (!ReadMemory(m_PEHead.SizeOfOptionalHeader + m_ImageBase + m_PEOff + 0x18, //???
			&m_Section[1], sizeof(PE_SECTION) * m_PEHead.NumberOfSections) )
		{
			if (m_Section)
			{
				delete []m_Section;
				m_Section = nullptr;
			}
			CImageFile::Close();
			m_ErrorCode = PE_ERROR_SECTION;
			return false;
		}

		m_SectCount = m_PEHead.NumberOfSections;
		for (int secj = 1; secj <= m_SectCount; ++secj)
		{
			//TODO
		}

		memset(m_Section, 0, sizeof(PE_SECTION)); //???
		m_Section->VirtualAddress = 0;
		m_Section->PointerToRawData = 0;
		m_Section->VirtualSize = m_PEHead.SizeOfHeaders;
		m_Section->SizeOfRawData = m_Section->VirtualSize;
		m_Section->Characteristics = 0xC0000000; //IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE

		unsigned long MaxHighSectAddr = 0;
		unsigned long MinLowSectAddr = -1;

		for (int nSec = 1; nSec <= m_SectCount; ++nSec)
		{
			if (!m_Section[nSec].VirtualSize)
				m_Section[nSec].VirtualSize = m_Section[nSec].SizeOfRawData;

			if (m_Section[nSec].SizeOfRawData % FileAlignment)
				m_Section[nSec].SizeOfRawData = ALIGN_SIZE(m_Section[nSec].SizeOfRawData, FileAlignment);

			if (m_Section[nSec].VirtualSize % SectionAlignment)
				m_Section[nSec].VirtualSize = ALIGN_SIZE(m_Section[nSec].VirtualSize, SectionAlignment);

			if (nSec == m_SectCount && m_Section[nSec].VirtualSize < m_Section[nSec].SizeOfRawData)
			{
				FixAddress = ALIGN_SIZE(m_Section[nSec].SizeOfRawData, SectionAlignment);

				if (FixAddress + m_Section[nSec].VirtualAddress <= ALIGN_SIZE(m_PEHead.SizeOfImage, 4096))
					m_Section[nSec].VirtualSize = FixAddress;
			}

			if (m_Section[nSec].VirtualAddress < m_Section[nSec-1].VirtualSize
					+ m_Section[nSec-1].VirtualAddress
				&& m_Section[nSec-1].VirtualAddress < m_Section[nSec].VirtualAddress)
			{
				m_Section[nSec-1].VirtualSize = m_Section[nSec].VirtualAddress
					- m_Section[nSec-1].VirtualAddress;
			}

			if (nSec < m_SectCount
				&& m_Section[nSec].VirtualAddress == m_Section[nSec + 1].VirtualAddress)
			{
				m_Section[nSec].VirtualSize = 0;
			}

			if (m_Section[nSec].PointerToRawData > m_FileSize)
				m_Section[nSec].SizeOfRawData = 0;

			if (m_Section[nSec].SizeOfRawData > 0x1000000)
				m_Section[nSec].SizeOfRawData = m_Section[nSec].VirtualSize;

			MAX_LIMIT(m_Section[nSec].SizeOfRawData, m_FileSize - m_Section[nSec].PointerToRawData);

			if (m_Section[nSec].VirtualSize > 0x1000000)
				m_Section[nSec].VirtualSize = m_Section[nSec].SizeOfRawData;

			MIN_LIMIT(MaxHighSectAddr, m_Section[nSec].VirtualSize + m_Section[nSec].VirtualAddress);

			if (m_Section[nSec].VirtualAddress < MinLowSectAddr)
			{
				MinLowSectAddr = m_Section[nSec].VirtualAddress;
				MinLowSectAddrFilePos = m_Section[nSec].PointerToRawData;
			}
		}

		if (m_Section->SizeOfRawData < MinLowSectAddr && MinLowSectAddr != -1)
		{
			m_Section->VirtualSize = MinLowSectAddr;
			if (MinLowSectAddrFilePos)
				m_Section->SizeOfRawData = MinLowSectAddrFilePos;
		}

		if (m_SectCount > 0
			&& m_PEHead.AddressOfEntryPoint >= m_Section[m_SectCount].VirtualSize
							+ m_Section[m_SectCount].VirtualAddress)
		{
			if (m_PEHead.AddressOfEntryPoint >= m_PEHead.SizeOfImage)
			{
				if (m_FileSize >= m_Section[m_SectCount].PointerToRawData)
					m_Section[m_SectCount].SizeOfRawData = m_FileSize
								- m_Section[m_SectCount].PointerToRawData;
			} else
			{
				m_Section[m_SectCount].VirtualSize = m_PEHead.SizeOfImage
								- m_Section[m_SectCount].VirtualAddress;
			}
		}
		m_FileType = 0x313;

		if ( !(m_FileObjType & 0x4000000)) //FILE_OBJ_MEMORY
			m_ImageBase = m_PEHead.ImageBase;

		m_ImageSize = ALIGN_SIZE(m_PEHead.SizeOfImage, 4096);

		if (m_SectCount > 0
			&& m_Section[m_SectCount].SizeOfRawData + m_Section[m_SectCount].VirtualAddress > m_ImageSize)
		{
			m_ImageSize = m_Section[m_SectCount].SizeOfRawData + m_Section[m_SectCount].VirtualAddress;
			MIN_LIMIT(m_Section[m_SectCount].VirtualSize, m_Section[m_SectCount].SizeOfRawData);
		}

		m_EntryPoint = m_ImageBase + m_PEHead.AddressOfEntryPoint;
		if (m_ImageSize < MaxHighSectAddr && MaxHighSectAddr < 0x1000000)
			m_ImageSize = MaxHighSectAddr;
		if (m_ImageSize > 0x1000000 && MaxHighSectAddr < 0x1000000)
			m_ImageSize = MaxHighSectAddr;
		m_ImageHighBase = m_ImageBase + m_ImageSize;

		if (!Check())
		{
			if (m_Section)
			{
				delete []m_Section;
				m_Section = nullptr;
			}
			m_SectCount = 0;
			CImageFile::Close();
			return false;
		}

		GetTLS();

		m_ImportModuleCount = 0;
		m_ImportModule = 0;

		if (!(m_OpenMode & PE_OPEN_NO_IMPORT))
			GetImportModule();

		m_ExportFuncCount = 0;
		m_ExportFunc = 0;

		if (!(m_OpenMode & PE_OPEN_NO_EXPORT))
			GetExportFunc();

		m_ErrorCode = PE_ERROR_SUCCESS;
		return true;
	}

	void CPEFile::Close()
	{
		for (int n = 0; n < m_ImportModuleCount; ++n)
		{
			if (m_ImportModule[n].FuncList)
			{
				delete []m_ImportModule[n].FuncList;
				m_ImportModule[n].FuncList = nullptr;
			}
		}

		if (m_ImportModule)
		{
			delete []m_ImportModule;
			m_ImportModule = nullptr;
		}

		for (int n = 0; n < m_ResDirCount; ++n)
		{
			if (m_ResDir[n].ResDataDir)
			{
				delete []m_ResDir[n].ResDataDir;
				m_ResDir[n].ResDataDir = nullptr;
			}
		}

		if (m_ResDir)
		{
			delete []m_ResDir;
			m_ResDir = nullptr;
		}

		for (int n = 0; n < m_BaseRelocCount; ++n)
		{
			if (m_BaseReloc[n].TypeOffset)
			{
				delete []m_BaseReloc[n].TypeOffset;
				m_BaseReloc[n].TypeOffset = nullptr;
			}
		}

		if (m_BaseReloc)
		{
			delete []m_BaseReloc;
			m_BaseReloc = nullptr;
		}

		if (m_TLSCallBackFuncs)
		{
			delete []m_TLSCallBackFuncs;
			m_TLSCallBackFuncs = nullptr;
		}

		if (m_TLSData)
		{
			delete []m_TLSData;
			m_TLSData = nullptr;
		}

		if (m_ExportFunc)
		{
			delete []m_ExportFunc;
			m_ExportFunc = nullptr;
		}

		if (m_Section)
		{
			delete []m_Section;
			m_Section = nullptr;
		}

		m_ExportFuncCount = 0;
		m_TLSDataSize = 0;
		m_TLSCallBackCount = 0;
		m_AddressOfTLSIndex = 0;
		m_BaseRelocCount = 0;
		m_ResDirCount = 0;
		m_ImportModuleCount = 0;
		m_SectCount = 0;
		CImageFile::Close();
	}

	void CPEFile::RelocLoad(ULONG_PTR BaseAddr)
	{
		ULONG_PTR Delta = BaseAddr - m_ImageBase;
		if (BaseAddr != m_ImageBase && BaseAddr)
		{
			CImageFile::RelocLoad(BaseAddr);

			if (m_TLSCallBackFuncs)
			{
				for (int n = 0; n < m_TLSCallBackCount; ++n)
					m_TLSCallBackFuncs[n] += Delta;
			}

			GetBaseReloc();

			for (int n = 0; n < m_ImportModuleCount; ++n)
			{
				IMPORT_FUNC *pImportFunc = m_ImportModule[n].FuncList;
				for (int m = 0; m < m_ImportModule[n].FuncCount; ++m)
				{
					pImportFunc[m].NameAddr += Delta;
					pImportFunc[m].ThunkAddr += Delta;
				}
			}

			for (int n = 0; n < m_ExportFuncCount; ++n)
			{
				if (m_ExportFunc[n].Address)
					m_ExportFunc[n].Address += Delta;
				if (m_ExportFunc[n].Thunk)
					m_ExportFunc[n].Thunk += Delta;
			}
		}

	}

  bool CPEFile::IsRelocArea(unsigned long HiAddr, unsigned long LowAddr)
	{
		for (int n = 0; n < m_BaseRelocCount; ++n)
		{
			unsigned long low = m_BaseReloc[n].VirtualAddr;
			unsigned long hi = m_BaseReloc[n].VirtualAddr + 4096;

			if (HiAddr >= low && HiAddr < hi)
				return true;
			if (LowAddr > low && LowAddr <= hi)
				return true;
			if (HiAddr <= low && LowAddr >= hi)
				return true;
		}
		return false;
	}

  bool CPEFile::IsImportThunkArea(unsigned long LowAddr, unsigned long HiAddr)
	{
		unsigned long low;
		unsigned long hi;

		if (!GetImportThunkArea(&low, &hi))
			return false;
		if (LowAddr >= low && LowAddr < hi)
			return true;
		if (HiAddr > low && HiAddr <= hi)
			return true;
		if (LowAddr <= low && HiAddr >= hi)
			return true;
		return false;
	}

  bool CPEFile::GetImportThunkArea(unsigned long *LowAddr, unsigned long *HiAddr)
	{
		if (!m_ImportModuleCount)
			return false;
		ULONG_PTR low = -1;
		ULONG_PTR hi = 0;
		IMPORT_MODULE *Import = m_ImportModule;
		for (int i = 0; i < m_ImportModuleCount; ++i)
		{
			IMPORT_FUNC *Func = Import->FuncList;
			for (int j = 0; j < Import->FuncCount; ++j)
			{
				MAX_LIMIT(low, Func->ThunkAddr);
				MIN_LIMIT(hi, Func->ThunkAddr);
				++Func;
			}
			++Import;
		}
		if (low >= hi)
			return false;
		*LowAddr = low - m_ImageBase;
		*HiAddr = hi - m_ImageBase;
		return true;
	}
#ifdef _X64_
	#pragma pack(push,1)
	struct TLS_DIRECTORY64
	{
	  /* 0x0000 */ ULONG_PTR StartAddressOfRawData;
	  /* 0x0004 */ ULONG_PTR EndAddressOfRawData;
	  /* 0x0008 */ ULONG_PTR AddressOfIndex;
	  /* 0x000c */ ULONG_PTR AddressOfCallBacks;
	  /* 0x0010 */ unsigned long SizeOfZeroFill;
	  /* 0x0014 */ unsigned long Characteristics;
	}; /* size: 0x0018 */
	#pragma pack(pop)
#else
	#pragma pack(push,1)
	struct TLS_DIRECTORY32
	{
	  /* 0x0000 */ unsigned long StartAddressOfRawData;
	  /* 0x0004 */ unsigned long EndAddressOfRawData;
	  /* 0x0008 */ unsigned long AddressOfIndex;
	  /* 0x000c */ unsigned long AddressOfCallBacks;
	  /* 0x0010 */ unsigned long SizeOfZeroFill;
	  /* 0x0014 */ unsigned long Characteristics;
	}; /* size: 0x0018 */
	#pragma pack(pop)
#endif
	void CPEFile::GetTLS()
	{
		if (!m_PEHead.DataDirectory[9].VirtualAddress)
			return;

		unsigned long Address; //???
		if (!MapToFile((ULONG_PTR)m_PEHead.DataDirectory[9].VirtualAddress, &Address, false))
			return;
#ifdef _X64_
		TLS_DIRECTORY64 TLSDir;
#else
		TLS_DIRECTORY32 TLSDir;
#endif
		CImageFile::ReadFile(Address, &TLSDir, sizeof(TLSDir));

		if (!TLSDir.AddressOfCallBacks)
			return;

		if (!MapToFile(TLSDir.AddressOfCallBacks, &Address, true))
			return;

		m_TLSCallBackCount = 0;
		m_TLSCallBackFuncs = 0;

		ULONG_PTR Pos;
		while (CImageFile::ReadFile(Address + m_TLSCallBackCount*sizeof(ULONG_PTR), &Pos, sizeof(ULONG_PTR))
				&& m_TLSCallBackCount < 512 && Pos)
			++m_TLSCallBackCount;

		if (m_TLSCallBackCount > 0)
		{
			if (m_TLSCallBackCount >= 512)
			{
				m_TLSCallBackCount = 0;
				return;
			}

			m_TLSCallBackFuncs = new ULONG_PTR[m_TLSCallBackCount];
			if (!m_TLSCallBackFuncs)
				return;
		
			CImageFile::ReadFile(Address, m_TLSCallBackFuncs, sizeof(ULONG_PTR) * m_TLSCallBackCount);
		}

		m_TLSDataSize = 0;
		if (!TLSDir.AddressOfIndex)
			return;
		
		m_TLSDataSize = TLSDir.EndAddressOfRawData - TLSDir.StartAddressOfRawData;
		if (m_TLSDataSize <= 0 || m_TLSDataSize >= 0x10000)
		{
			m_AddressOfTLSIndex = 0;
			m_TLSDataSize = 0;
			return;
		}
		
		m_TLSData = new unsigned char[m_TLSDataSize]; //???
		if (!m_TLSData)
		{
			if (m_TLSCallBackFuncs)
			{
				delete []m_TLSCallBackFuncs;
				m_TLSCallBackFuncs = 0;
			}
		}
		
		ReadMemory(TLSDir.StartAddressOfRawData, m_TLSData, m_TLSDataSize);
		m_AddressOfTLSIndex = TLSDir.AddressOfIndex;
	}

	#pragma pack(push,1)
	struct IMAGE_BASE_RELOC
	{
		unsigned long VirtualAddress;
		unsigned long BlockSize;
	};
	#pragma pack(pop)

	void CPEFile::GetBaseReloc()
	{
		if (!m_PEHead.DataDirectory[5].VirtualAddress)
			return;

		unsigned long Address;
		if (!MapToFile((ULONG_PTR)m_PEHead.DataDirectory[5].VirtualAddress, &Address, false))
			return;
		
		IMAGE_BASE_RELOC BaseReloc;

		m_BaseRelocCount = 0;

		unsigned long ImageRelocBasePos;

		for (ImageRelocBasePos = Address;
			CImageFile::ReadFile(ImageRelocBasePos, &BaseReloc, sizeof(IMAGE_BASE_RELOC))
			&& BaseReloc.BlockSize
			&& BaseReloc.BlockSize > 8 && BaseReloc.BlockSize <= 0x1008;
			ImageRelocBasePos += BaseReloc.BlockSize )
		{
			++m_BaseRelocCount;
		}

		if (m_BaseRelocCount < 1 || m_BaseRelocCount > 4096)
		{
			m_BaseRelocCount = 0;
			return;
		}
		
		m_BaseReloc = new BASE_RELOC[m_BaseRelocCount];
		if (!m_BaseReloc)
			return;
		
		memset(m_BaseReloc, 0, sizeof(BASE_RELOC) * m_BaseRelocCount);
		unsigned long ImageRelocPos = Address;
		for (unsigned long n = 0; n < m_BaseRelocCount; ++n)
		{
			CImageFile::ReadFile(ImageRelocPos, &BaseReloc, sizeof(IMAGE_BASE_RELOC));
			m_BaseReloc[n].VirtualAddr = BaseReloc.VirtualAddress;
			m_BaseReloc[n].Count = (BaseReloc.BlockSize - sizeof(BASE_RELOC))/2;
			if (m_BaseReloc[n].Count <= 1024)
			{
				if (m_BaseReloc[n].Count)
					m_BaseReloc[n].TypeOffset = new unsigned short[m_BaseReloc[n].Count]; //*2

				if (!m_BaseReloc[n].TypeOffset)
				{
					m_BaseRelocCount = n;
					return;
				}

				if (!CImageFile::ReadFile(ImageRelocPos + sizeof(BASE_RELOC), m_BaseReloc[n].TypeOffset,
								m_BaseReloc[n].Count*2))
				{
					delete []m_BaseReloc[n].TypeOffset;
					m_BaseRelocCount = n;
					return;
				}
				ImageRelocPos += BaseReloc.BlockSize;
			} else
			{
				m_BaseReloc[n].Count = 0;
			}
		}
	}

	#pragma pack(push,1)
	struct IMPORT_DESCR
	{
	  union
	  {
	    /* 0x0000 */ unsigned long Characteristics;
	    /* 0x0000 */ unsigned long OriginalFirstThunk;
	  }; /* size: 0x0004 */
	  /* 0x0004 */ unsigned long TimeDateStamp;
	  /* 0x0008 */ unsigned long ForwarderChain;
	  /* 0x000c */ unsigned long Name;
	  /* 0x0010 */ unsigned long FirstThunk;
	}; /* size: 0x0014 */
	#pragma pack(pop)

	void CPEFile::GetImportModule()
	{
		if (!m_PEHead.DataDirectory[1].VirtualAddress)
			return;
		
		m_ImportModuleCount = 0;
		ULONG_PTR ImportDescPos = m_ImageBase + m_PEHead.DataDirectory[1].VirtualAddress;
		IMPORT_DESCR *pImportDesc = new IMPORT_DESCR[256];
		if (!pImportDesc)
			return;
		
		int ModuleCount = 0;
		while (ModuleCount < 256
			&& ReadMemory(ImportDescPos, &pImportDesc[ModuleCount], sizeof(IMPORT_DESCR)) == sizeof(IMPORT_DESCR)
			&& pImportDesc[ModuleCount].FirstThunk)
		{
			++ModuleCount;
			ImportDescPos += sizeof(IMPORT_DESCR);
		}

		if (ModuleCount <= 0 || ModuleCount >= 256)
		{
			delete []pImportDesc;
			return;
		}

		ULONG_PTR *pFuncThunk = new ULONG_PTR[0x1000];
		if (!pFuncThunk)
		{
			delete []pImportDesc;
			return;
		}

		
		m_ImportModuleCount = ModuleCount;
		m_ImportModule = new IMPORT_MODULE[ModuleCount];
		if (!m_ImportModule)
		{
			delete []pFuncThunk;
			delete []pImportDesc;
			return;
		}

		
		memset(m_ImportModule, 0, sizeof(IMPORT_MODULE) * ModuleCount);
		ModuleCount = 0;
		IMPORT_MODULE *pImportModule = m_ImportModule;
		while (ModuleCount < m_ImportModuleCount
			&& pImportDesc[ModuleCount].FirstThunk
			&& CImageFile::ReadString(
				m_ImageBase + pImportDesc[ModuleCount].Name,
				pImportModule->FileName,
				0x40,
				0x18) )
		{
			pImportModule->RVA = m_ImageBase + pImportDesc[ModuleCount].Name;
			TGetFileTitle(pImportModule->FileName, pImportModule->ModuleName);
			int FuncCount = 0;

			if (pImportDesc[ModuleCount].OriginalFirstThunk)
				FuncCount = CImageFile::ReadString(
					m_ImageBase + pImportDesc[ModuleCount].OriginalFirstThunk,
					(ULONG_PTR*)pFuncThunk,
					0x1000,
					0x18);
			if (!FuncCount)
				FuncCount = CImageFile::ReadString(
					m_ImageBase + pImportDesc[ModuleCount].FirstThunk,
					(ULONG_PTR*)pFuncThunk,
					0x1000,
					0x18);
			if (!FuncCount)
				break;

			if (FuncCount < 4096)
			{
				pImportModule->FuncCount = FuncCount;
				IMPORT_FUNC *pImportFunc = new IMPORT_FUNC[FuncCount];
				pImportModule->FuncList = pImportFunc;
				if (!pImportFunc && m_ImportModule)
				{
					delete []m_ImportModule;
					m_ImportModule = 0;
					delete []pFuncThunk;
					delete []pImportDesc;
					return;
				}

				memset(pImportFunc, 0, sizeof(IMPORT_FUNC) * FuncCount);
				int FuncThunkPos = 0;
				unsigned long FuncThunkAddrPos = pImportDesc[ModuleCount].FirstThunk;
				while (FuncThunkPos < pImportModule->FuncCount)
				{
					ULONG_PTR FuncCallAddr = pFuncThunk[FuncThunkPos];
				#ifdef _X64_
					bool bUseOrd = (FuncCallAddr & 0x8000000000000000ULL)!=0
				#else
					bool bUseOrd = (FuncCallAddr & 0x80000000)!=0
				#endif
						&& (FuncCallAddr < m_ImageBase || FuncCallAddr >= m_ImageHighBase); //!InBaseRange(FunCallAddr)
					pImportFunc->bUseOrd = bUseOrd;
					if (pImportFunc->bUseOrd)
					{
						pImportFunc->FuncOrd = (pFuncThunk[FuncThunkPos]) & 0xffff; //???
						TSPrintf(pImportFunc->FuncName, "Function_ORD_%04X", pImportFunc->FuncOrd);
					} else
					{
						pImportFunc->NameAddr = FuncCallAddr;
						if (m_FileObjType & 0x4000000) //FILE_OBJ_MEMORY
						{
							if (FuncCallAddr < m_ImageBase)
								pImportFunc->NameAddr += m_ImageBase;
						} else
						{
							pImportFunc->NameAddr += m_ImageBase;
						}

						if (!CImageFile::ReadString(
							pImportFunc->NameAddr + 2,
							pImportFunc->FuncName,
							0x40,
							0x18) )
						break;
					}
					pImportFunc->ThunkAddr = m_ImageBase + FuncThunkAddrPos;
					++FuncThunkPos;
					FuncThunkAddrPos += sizeof(ULONG_PTR);
					++pImportFunc;
				}
				pImportModule->FuncCount = FuncThunkPos;
			}
			++ModuleCount;
			++pImportModule;
		}
		m_ImportModuleCount = ModuleCount;
		delete []pFuncThunk;
		delete []pImportDesc;
	}

	#pragma pack(push,1)
	struct EXPORT_DESCR
	{
	  /* 0x0000 */ unsigned long Characteristics;
	  /* 0x0004 */ unsigned long TimeDateStamp;
	  /* 0x0008 */ unsigned short MajorVersion;
	  /* 0x000a */ unsigned short MinorVersion;
	  /* 0x000c */ unsigned long Name;
	  /* 0x0010 */ unsigned long Base;
	  /* 0x0014 */ unsigned long NumberOfFunctions;
	  /* 0x0018 */ unsigned long NumberOfNames;
	  /* 0x001c */ unsigned long AddressOfFunctions;
	  /* 0x0020 */ unsigned long AddressOfNames;
	  /* 0x0024 */ unsigned long AddressOfNameOrdinals;
	}; /* size: 0x0028 */
	#pragma pack(pop)

	void CPEFile::GetExportFunc()
	{
		m_ExportFuncCount = 0;

		if (!m_PEHead.DataDirectory[0].VirtualAddress)
			return;
		
		EXPORT_DESCR Export;
		if (ReadMemory(m_ImageBase + m_PEHead.DataDirectory[0].VirtualAddress, &Export, sizeof(EXPORT_DESCR)) != sizeof(EXPORT_DESCR))
			return;
		
		unsigned long FuncSize = Export.NumberOfFunctions;
		unsigned long NameSize = Export.NumberOfNames;

		if (FuncSize < 1 || FuncSize > 65535)
			return;

		if (NameSize > 4096)
			return;

		m_ExportFunc = new EXPORT_FUNC[FuncSize];
		if (!m_ExportFunc)
			return;
		
		memset(m_ExportFunc, 0, sizeof(EXPORT_FUNC) * FuncSize);
		unsigned long FuncNameAddrPos = Export.AddressOfNames;
		unsigned long FuncOrdPos = Export.AddressOfNameOrdinals;
		unsigned long FuncAddrPos = Export.AddressOfFunctions;

		unsigned long n;
		for (n = 0; n < FuncSize; ++n)
		{
			unsigned long FuncName;
			if (!ReadMemory(m_ImageBase + FuncNameAddrPos, &FuncName, 4))
				break;

			unsigned short FuncOrd;
			if (n < NameSize)
			{
				CImageFile::ReadString(m_ImageBase + FuncName, m_ExportFunc[n].FuncName,
							0x40, 0x18);

				ReadMemory(m_ImageBase + FuncOrdPos + n*2, &FuncOrd, 2);
			} else
			{
				FuncOrd = (Export.Base & 0xffff) + n; //???
				TStrCpy(m_ExportFunc[n].FuncName, "Export_by_Ord_");
				uNumToStr(FuncOrd, &m_ExportFunc[n].FuncName[sizeof("Export_by_Ord_")-1], 16);
			}

			if (FuncOrd >= FuncSize)
			{
				m_ExportFunc[n].Address = 0;
				m_ExportFunc[n].Thunk = 0;
			} else
			{
				unsigned long FuncAddr;
				if (!ReadMemory(m_ImageBase + FuncAddrPos + FuncOrd*4, &FuncAddr, 4))
					break;

				if (FuncAddr)
				{
					m_ExportFunc[n].Address = m_ImageBase + FuncAddr;
				}

				m_ExportFunc[n].Thunk = m_ImageBase + FuncAddrPos + FuncOrd*4;
				m_ExportFunc[n].FuncOrd = Export.Base + FuncOrd;
			}
			FuncNameAddrPos += 4;
		}
		m_ExportFuncCount = n;
	}

	#pragma pack(push,1)
	struct RESOURCE_DIRECTORY
	{
	  /* 0x0000 */ unsigned long Characteristics;
	  /* 0x0004 */ unsigned long TimeDateStamp;
	  /* 0x0008 */ unsigned short MajorVersion;
	  /* 0x000a */ unsigned short MinorVersion;
	  /* 0x000c */ unsigned short NumberOfNamedEntries;
	  /* 0x000e */ unsigned short NumberOfIdEntries;
	}; /* size: 0x0010 */

	struct RESOURCE_DATA_ENTRY
	{
	  /* 0x0000 */ unsigned long OffsetToData;
	  /* 0x0004 */ unsigned long Size;
	  /* 0x0008 */ unsigned long CodePage;
	  /* 0x000c */ unsigned long Reserved;
	}; /* size: 0x0010 */

	struct RESOURCE_DIRECTORY_ENTRY
	{
	  /* 0x0000 */ unsigned long Name;
	  /* 0x0004 */ unsigned long OffsetToData;
	}; /* size: 0x0008 */
	
	struct RESOURCE_DIRECTORY_STRING
	{
	  /* 0x0000 */ unsigned short Length;
	  /* 0x0002 */ char NameString[1];
	}; /* size: 0x0004 */
	#pragma pack(pop)


  void CPEFile::GetResource()
	{
		int DataPos;
		int ResPos;
		unsigned int NameSize2;
		unsigned int NameSize2_v5;
		int NameAddress;
		int NameAddr;
		int MixAddress;
		unsigned short NameSize;
		int ResourceAddress;
		int DataAddress;
		RESOURCE_DATA_ENTRY ResourceDataEntry; //???

		if (m_ResDir)
			return;
		
		if (!m_PEHead.DataDirectory[2].VirtualAddress)
			return;
		
		ResourceAddress = m_ImageBase + m_PEHead.DataDirectory[2].VirtualAddress;

		RESOURCE_DIRECTORY ResourceDirectory;
		if (ReadMemory(ResourceAddress, &ResourceDirectory, sizeof(RESOURCE_DIRECTORY)) != sizeof(RESOURCE_DIRECTORY))
			return;
		
		m_ResDirCount = (unsigned __int16)ResourceDirectory.NumberOfNamedEntries
			+ (unsigned __int16)ResourceDirectory.NumberOfIdEntries;

		if (m_ResDirCount < 1 || m_ResDirCount > 1024)
		{
			m_ResDirCount = 0;
			return;
		}

		m_ResDir = new RES_DIR[m_ResDirCount];
		if (!m_ResDir)
			return;

		memset(m_ResDir, 0, sizeof(RES_DIR) * m_ResDirCount);

		for (ResPos = 0; ResPos < m_ResDirCount; ++ResPos)
		{
			//if (ResPos >= m_ResDirCount)
			//	return;

			RESOURCE_DIRECTORY_ENTRY DirEntry;
			if (ReadMemory(ResourceAddress + 8 * ResPos + 0x10, &DirEntry, 8) != 8)
				continue;

			if (!(DirEntry.OffsetToData & 0x80000000))
				continue;

			if (DirEntry.Name & 0x80000000)
			{
				NameAddress = (DirEntry.Name + ResourceAddress) & ~0x10000000;
				if (ReadMemory(NameAddress, &NameSize, 2) != 2)
					continue;

				NameSize2 = 2 * NameSize;

				if (NameSize2 >= 64)
					continue;

				if (ReadMemory(NameAddress + 2, m_ResDir[ResPos].TypeName, NameSize2) != NameSize2)
					continue;

				m_ResDir[ResPos].Type = -1;
			} else
			{
				m_ResDir[ResPos].Type = DirEntry.Name & 0xffff; //???
			}

			DataAddress = (DirEntry.OffsetToData + ResourceAddress) & ~0x80000000;

			if (ReadMemory(DataAddress, &ResourceDirectory, sizeof(RESOURCE_DIRECTORY)) != sizeof(RESOURCE_DIRECTORY))
				continue;

			m_ResDir[ResPos].ResCount = (unsigned __int16)ResourceDirectory.NumberOfNamedEntries
				+ (unsigned __int16)ResourceDirectory.NumberOfIdEntries;

			if (m_ResDir[ResPos].ResCount < 1 || m_ResDir[ResPos].ResCount > 4096)
			{
				m_ResDir[ResPos].ResCount = 0;
				continue;
			}

			if (m_ResDir[ResPos].ResCount)
				m_ResDir[ResPos].ResDataDir = new RES_DATA[m_ResDir[ResPos].ResCount];

			if (!m_ResDir[ResPos].ResDataDir)
				continue;
			
			memset(m_ResDir[ResPos].ResDataDir, 0, sizeof(RES_DATA) * m_ResDir[ResPos].ResCount);

			if (!m_ResDir[ResPos].ResDataDir) //???
				continue;
			
			for (DataPos = 0; DataPos < m_ResDir[ResPos].ResCount; ++DataPos)
			{
				//if (DataPos >= m_ResDir[ResPos].ResCount)
				//	break;

				if (ReadMemory(DataAddress + 8 * DataPos + 0x10, &DirEntry, 8) != 8)
					continue;

				if (!(DirEntry.OffsetToData & 0x80000000))
					continue;
				
				if (DirEntry.Name & 0x80000000)
				{
					NameAddr = (DirEntry.Name + ResourceAddress) & ~0x80000000;
					if (ReadMemory(NameAddr, &NameSize, 2) != 2)
						continue;
				
					MAX_LIMIT(NameSize, 64);

					NameSize2_v5 = 2 * NameSize;
					if (ReadMemory(NameAddr + 2, m_ResDir[ResPos].ResDataDir[DataPos].IDName,
						NameSize2_v5) != NameSize2_v5)
							continue;
				
					m_ResDir[ResPos].ResDataDir[DataPos].IDName[NameSize2_v5 >> 1] = 0;
				} else
				{

					m_ResDir[ResPos].ResDataDir[DataPos].ID = DirEntry.Name & 0xffff; //???
				}

				MixAddress = (DirEntry.OffsetToData + ResourceAddress) & ~0x80000000;

				if (ReadMemory(MixAddress, &ResourceDirectory, sizeof(RESOURCE_DIRECTORY)) != sizeof(RESOURCE_DIRECTORY))
					continue;

				if (ResourceDirectory.NumberOfIdEntries != 1)
					continue;

				if (ReadMemory(MixAddress + 0x10, &DirEntry, 8) != 8)
					continue;

				if ((DirEntry.OffsetToData & 0x80000000))
					continue;

				if (ReadMemory(MixAddress, &ResourceDataEntry, sizeof(RESOURCE_DIRECTORY)) != sizeof(RESOURCE_DIRECTORY))
					continue;
				
				m_ResDir[ResPos].ResDataDir[DataPos].CodePage = ResourceDataEntry.CodePage;
				m_ResDir[ResPos].ResDataDir[DataPos].Size = ResourceDataEntry.Size;
				m_ResDir[ResPos].ResDataDir[DataPos].BufferAddr = m_ImageBase + ResourceDataEntry.OffsetToData;
			}
		}
	}

  int CPEFile::GetLastPhyPosSection() const
	{
		unsigned long Last = 0;
		int Pos = 0;
		for (int i = 1; i < m_SectCount; ++i) //??? <=
		{
			if (Last < m_Section[i].PointerToRawData)
			{
				Last = m_Section[i].PointerToRawData;
				Pos = i;
			}
		}
		return Pos;
	}

  int CPEFile::GetLastMemAddrSection() const
	{
		unsigned long Last = 0;
		int Pos = 0;
		for (int n = 1; n <= m_SectCount; ++n)
		{
			if (Last < m_Section[n].VirtualAddress)
			{
				Last = m_Section[n].VirtualAddress;
				Pos = n;
			}
		}
		return Pos;
	}

  int CPEFile::GetFirstPhyPosSection() const
	{
		int Pos = 0;
		unsigned long First = 0;
		for (int n = 1; n <= m_SectCount; ++n)
		{
			if (First >= m_Section[n].PointerToRawData)
			{
				First = m_Section[n].PointerToRawData;
				Pos = n;
			}
		}
		return Pos;
	}

  int CPEFile::GetFirstMemAddrSection() const
	{
		int Pos = 0;
		unsigned long First = 0;
		for (int n = 1; n <= m_SectCount; ++n)
		{
			if (First >= m_Section[n].VirtualAddress)
			{
				First = m_Section[n].VirtualAddress;
				Pos = n;
			}
		}
		return Pos;
	}

  ULONG_PTR CPEFile::GetExportFunc(const char *Name, unsigned long *Index) const
	{
		for (int n = 0; n < m_ExportFuncCount; ++n)
		{
			if (!TStrCmp(m_ExportFunc[n].FuncName, Name))
			{
				if (Index)
					*Index = n;
				return m_ExportFunc[n].Address;
			}
		}
		return 0;
	}

  ULONG_PTR CPEFile::GetExportFunc(unsigned long Ord) const
	{
		for (int n = 0; n < m_ExportFuncCount; ++n)
		{
			if (m_ExportFunc[n].FuncOrd == Ord)
				return m_ExportFunc[n].Address;
		}
		return 0;
	}

  int CPEFile::LoadStringW(unsigned long ID, WCHAR *Name, int Size)
	{
		int result;
		int nDir;
		int ResDataAddress;
		unsigned int nID;
		int i;
		unsigned int ResDataSize;
		int v12;
		int count_v13;
		char *p_v14;
		unsigned int nv15;
		int nv15a;

		if (Size < 2)
		{
			*Name = 0;
			return 0;
		}
		
		GetResource();
		RES_DIR *pDir = 0;
		for (nDir = 0; nDir < m_ResDirCount; ++nDir)
		{
			if (m_ResDir[nDir].Type == 6) //???
			{
				pDir = &m_ResDir[nDir];
				break;
			}
		}

		if (!pDir)
		{
			return 0;
		}
		
		for (i = 0; i < pDir->ResCount; ++i)
		{
			if ( (ID >> 4) + 1 == pDir->ResDataDir[i].ID)
			{
				ResDataAddress = pDir->ResDataDir[i].BufferAddr;
				ResDataSize = pDir->ResDataDir[i].Size;
				break;
			}
		}

		if (i >= pDir->ResCount || !ResDataAddress)
		{
			*Name = 0;
			return 0;
		}
		
		nID = ID % 16;
		if (ResDataSize >= 0xFFFF)
		{
			*Name = 0;
			return 0;
		}
		
		p_v14 = new char[ResDataSize];
		if (!p_v14)
		{
			*Name = 0;
			return 0;
		}

		
		if (!ReadMemory(ResDataAddress, p_v14, ResDataSize))
		{
			delete []p_v14;
			*Name = 0;
			return 0;
		}

		
		nv15 = 0;
		while (nv15 < ResDataSize && nID)
		{
			v12 = *(unsigned __int16 *)&p_v14[nv15];
			nv15 += 2;
			if (v12)
				nv15 += 2 * v12;
			--nID;
		}

		if (nID || nv15 >= ResDataSize)
		{
			delete []p_v14;
			*Name = 0;
			return 0;
		}
		
		count_v13 = *(unsigned __int16 *)&p_v14[nv15];
		MAX_LIMIT(count_v13, (Size-2)/2);

		nv15a = nv15 + 2;
		if (nv15a + 2 * count_v13 > ResDataSize)
			count_v13 = 0;
		else
			memcpy((void *)Name, &p_v14[nv15a], count_v13*2);
		Name[count_v13] = 0;
		delete p_v14;
		return count_v13;
	}

  int CPEFile::LoadStringA(unsigned long ID, char *Name, int Size)
	{
		WCHAR *pName = new WCHAR[Size]; //2
		if (!pName)
			return 0;
		*pName = 0;
		LoadStringW(ID, pName, Size*2);
		//int i = TStrCpy(Name, pName);
		int n = 0;
		for (; pName[n]; ++n)
			Name[n] = pName[n];
		Name[n] = 0;
		delete []pName;
		return n;
	}

  RES_DATA *CPEFile::FindResource(const unsigned short *IDName, unsigned long ID, const unsigned short *Name, unsigned long Type)
	{
		GetResource();

		RES_DIR *ResDir = 0;
		for (int n = 0; n < m_ResDirCount; ++n)
		{
			if ((Name && !TStrICmp(m_ResDir[n].TypeName, Name)) ||
				m_ResDir[n].Type == Type)
			{
				ResDir = &m_ResDir[n];
				break;
			}
		}
		if (!ResDir)
			return 0;

		for (int n = 0; n < ResDir->ResCount; ++n)
		{
			if (IDName)
			{
				if (!TStrICmp(ResDir->ResDataDir[n].IDName, IDName))
				{
				//	v6 = ResDir->ResDataDir[n].BufferAddr;
				//	v7 = ResDir->ResDataDir[n].Size;
					return &ResDir->ResDataDir[n];
				}
			} else
			if (ID == ResDir->ResDataDir[n].ID)
			{
			//	v8 = ResDir->ResDataDir[n].BufferAddr;
			//	v9 = ResDir->ResDataDir[n].Size;
				return &ResDir->ResDataDir[n];
			}
		}
		return 0;
	}

  bool CPEFile::GetSectionIndex(ULONG_PTR Address, int *Index) const
	{
		unsigned long VirtualAddress = Address - m_ImageBase;
		for (int n = 0; n < m_SectCount; ++n)
		{
			if (VirtualAddress >= m_Section[n].VirtualAddress &&
				VirtualAddress < m_Section[n].VirtualAddress + m_Section[n].VirtualSize)
			{
				if (Index)
					*Index = n;
				return true;
			}
		}
		return false;
	}

	bool CPEFile::Check()
	{
		if (m_ImageBase && m_ImageSize)
			return true;
		m_ErrorCode = PE_ERROR_HEAD;
		return false;
	}

	IMPORT_MODULE *CPEFile::GetImportModule(const char *FileName) const
	{
		for (int n = 0; n < m_ImportModuleCount; ++n)
		{
			if (!TStrICmp(m_ImportModule[n].FileName, FileName) ||
			    !TStrICmp(m_ImportModule[n].ModuleName, FileName))
			{
				return &m_ImportModule[n];
			}
		}
		return nullptr;
	}

	IMPORT_FUNC *CPEFile::GetImportFunc(IMPORT_MODULE *pImportModule, const char *Name) const
	{
		for (int n = 0; n < pImportModule->FuncCount; ++n)
		{
			if (!TStrICmp(pImportModule->FuncList[n].FuncName, Name))
				return &pImportModule->FuncList[n];
		}
		return nullptr;
	}

	IMPORT_FUNC *CPEFile::GetImport(const char *FileName, char *FuncName) const
	{
		IMPORT_MODULE *ImportModule = GetImportModule(FileName);
		if (ImportModule)
			return GetImportFunc(ImportModule, FuncName);
		return nullptr;
	}

	bool CPEFile::GetDataDir(unsigned long Index, PE_DATA_DIR *DataDir) const
	{
		if (Index >= 16)
			return false;

		if (DataDir)
		{
			DataDir->VirtualAddress = m_PEHead.DataDirectory[Index].VirtualAddress;
			DataDir->Size = m_PEHead.DataDirectory[Index].Size;
		}
		return true;

	}

	bool CPEFile::Open(ULONG_PTR ImageBase, int OpenMode, unsigned long (*RMFunc)(ULONG_PTR, void*, unsigned long, void*), void *RMObj)
	{
		if (OpenMode)
			m_OpenMode |= OpenMode;
		SetRelocRead(RMFunc, RMObj);
		m_ImageBase = ImageBase;
		m_FileObjType = 0x4000000; //FILE_OBJ_MEMORY
		return Open(0, 0);
	}


//???
struct DATASTRUCT
{
  /* 0x0000 */ unsigned long Length;
  /* 0x0004 */ unsigned long Size;
}; /* size: 0x0008 */


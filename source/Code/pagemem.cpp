
#include "pagemem.hpp"

	CPageMemory::CPageMemory()
		: m_PageUsedCount(0)
		, m_PageCount(0)
		, m_PhysMemory(nullptr)
		, m_PageExistMap(nullptr)
		, m_WriteAccessPageCount(0)
		, m_FreePhysIndex(0)
		, m_Style(0)
	{
		memset(m_PageDir, 0, sizeof(m_PageDir));
	}

	CPageMemory::~CPageMemory()
	{
	}

	bool CPageMemory::Init(int PageCount, unsigned long Style)
	{
		m_Style = Style;

		if (m_Style & 2)
		{
			memset(m_PageDir, 0, sizeof(m_PageDir));
			memset(&m_PageDataCache1, 0, sizeof(MEMORY_PAGE_CACHE));
			memset(&m_PageDataCache2, 0, sizeof(MEMORY_PAGE_CACHE));
			m_PageCount = PageCount;
			m_PageUsedCount = 0;
			m_FreePhysIndex = 0;
			m_WriteAccessPageCount = 0;
			return true;
		}

		m_PhysMemory = new unsigned char[PageCount*PAGE_SIZE]; //MEMORY_PAGE new[PageCount] //???
		if (!m_PhysMemory)
		{
			return false;
		}

		m_PageExistMap = new unsigned char[PageCount];
		if (!m_PageExistMap)
		{
			SafeDeleteAry(m_PhysMemory);
			return false;
		}
		
		memset(m_PageExistMap, 0, PageCount);
	//???
		memset(m_PageDir, 0, sizeof(m_PageDir));
		memset(&m_PageDataCache1, 0, sizeof(MEMORY_PAGE_CACHE));
		memset(&m_PageDataCache2, 0, sizeof(MEMORY_PAGE_CACHE));
		m_PageCount = PageCount;
		m_PageUsedCount = 0;
		m_FreePhysIndex = 0;
		m_WriteAccessPageCount = 0;
		return true;
	}

	void CPageMemory::Release()
	{
		if (m_Style & 2)
		{
			for (int n = 0; n < 1024; ++n)
			{
				if (m_PageDir[n])
				{
					for (int m = 0; m < 1024; ++m)
					{
						SafeDeleteAry(m_PageDir[n][m].PhysAddr);
					}
				}
			}
		} else
		{
			SafeDeleteAry(m_PhysMemory);
			SafeDeleteAry(m_PageExistMap);
		}

		for (int i = 0; i < 1024; ++i)
		{
			SafeDeleteAry(m_PageDir[i]);
		}
		m_ImageList.clear();
	}

	void CPageMemory::RefreshCache(MEMORY_PAGE *pPage)
	{
		if (pPage == m_PageDataCache1.MemoryPage)
			memset(&m_PageDataCache1, 0, sizeof(MEMORY_PAGE_CACHE));
		if (pPage == m_PageDataCache2.MemoryPage)
			memset(&m_PageDataCache2, 0, sizeof(MEMORY_PAGE_CACHE));
	}

	bool CPageMemory::ExchangePage()
	{
		if (ReleaseClearPage())
			return true;

		if (m_Style & 1)
			return FlushAllPage() != 0;

		return false;
	}

	int CPageMemory::FlushAllPage()
	{
		int count = 0;
		for (unsigned long n = 0; n < 1024; ++n)
		{
			if (m_PageDir[n])
			{
				for (unsigned long m = 0; m < 1024; ++m)
				{
					MEMORY_PAGE *pPage = &m_PageDir[n][m];
					if (pPage->Attribute & 1 && !(pPage->Attribute & 0x8000) && pPage->PhysAddr)
					{
						unsigned long Address = (m << 12) + (n << 22);
						if (SavePage(Address, pPage))
							++count;
					}
				}
			}
		}
		return count;
	}


	CPageImage *CPageMemory::GetImage(const char *FileName)
	{
		for (map<CMemoryImage, CPageImage*>::IT it = m_ImageList.begin();
			it != m_ImageList.end(); ++it)
		{
			if (TCmpModuleName(it->second->m_FileName.operator const char *(), FileName))
				return it->second;
		}
		return NULL;
	}

	CPageImage *CPageMemory::GetImage(unsigned long Address)
	{
		map<CMemoryImage, CPageImage*>::IT it = m_ImageList.find(CMemoryImage(Address));
		if (it == m_ImageList.end())
			return NULL;

		return it->second;
	}


	bool CPageMemory::MapImage(CPageImage *pImageFile)
	{
		map<CMemoryImage, CPageImage*>::IT it = m_ImageList.find(CMemoryImage(pImageFile->m_ImageBase));
		if (it != m_ImageList.end())
			return false;

		unsigned long Address = pImageFile->m_ImageBase;
		unsigned long Size = pImageFile->m_ImageSize;
		unsigned long PageIndex = (Address >> 12) & 1023;
		unsigned long DirIndex = Address >> 22;
		unsigned long PageCount = PAGE_COUNT(Size,PAGE_SIZE);
		while (DirIndex < 1024 && PageCount)
		{
			if (!m_PageDir[DirIndex])
			{
				m_PageDir[DirIndex] = new MEMORY_PAGE[1024];
				memset(m_PageDir[DirIndex], 0, sizeof(MEMORY_PAGE)*1024);
			}

			while (PageIndex < 1024 && PageCount)
			{
				MEMORY_PAGE *pPage = &m_PageDir[DirIndex][PageIndex];
				if (pPage->Attribute)
					return false;

				pPage->Attribute = pImageFile->GetPageAttribute(Address) | 0x10;
				pPage->PhysAddr = NULL;
				pPage->AccessReadCount = 0;
				pPage->AccessWriteCount = 0;
				--PageCount;
				Address += PAGE_SIZE;
				++PageIndex;
			}
			++DirIndex;
			PageIndex = 0;
		}
		m_ImageList.insert(_Tmap_pair<CMemoryImage, CPageImage*>(CMemoryImage(pImageFile->m_ImageBase, pImageFile->m_ImageHighBase-1), pImageFile));
		return true;
	}

	bool CPageMemory::UnmapImage(CPageImage *pImageFile)
	{
		map<CMemoryImage, CPageImage*>::IT it = m_ImageList.find(CMemoryImage(pImageFile->m_ImageBase));
		if (it == m_ImageList.end())
			return false;

		unsigned long Address = pImageFile->m_ImageBase;
		unsigned long PageIndex = (Address >> 12) & 1023;
		unsigned long DirIndex = Address >> 22;
		unsigned long PageCount = pImageFile->m_ImageSize >> 12; //PAGE_COUNT(pImageFile->m_ImageSize,PAGE_SIZE) //???
		while (DirIndex < 1024 && PageCount)
		{
			if (!m_PageDir[DirIndex])
				return false;

			while (PageIndex < 1024 && PageCount)
			{
				MEMORY_PAGE *pPage = &m_PageDir[DirIndex][PageIndex];
				DeletePage(pPage);
				memset(pPage, 0, sizeof(MEMORY_PAGE));
				--PageCount;
				Address += PAGE_SIZE;
						//WTF ++PageIndex; ???
			}
			++DirIndex;
			PageIndex = 0;
		}
		m_ImageList.erase(it);
		return true;
	}

	bool CPageMemory::AllocatePage(unsigned long Address, unsigned long Attr, void *Buffer, unsigned long Size)
	{
		if (m_PageUsedCount >= m_PageCount && !ExchangePage())
			return false;

		unsigned long DirIndex = Address >> 22;
		unsigned long PageIndex = (Address >> 12) & 1023;

		if (!m_PageDir[DirIndex])
		{
			m_PageDir[DirIndex] = new MEMORY_PAGE[1024];
			memset(m_PageDir[DirIndex], 0, sizeof(MEMORY_PAGE)*1024);
		}

		MEMORY_PAGE *pPage = &m_PageDir[DirIndex][PageIndex];

		if (pPage->Attribute)
			return false;

		pPage->Attribute = Attr;
		if ( !(pPage->Attribute & 0x4000) )
		{
			if (!NewPage(pPage))
			{
				pPage->Attribute = 0;
				return false;
			}
			pPage->Attribute |= 1;
		}
		pPage->AccessReadCount = 0;
		pPage->AccessWriteCount = 0;
		MAX_LIMIT(Size, PAGE_SIZE);

		if ( !(pPage->Attribute & 0x4000) )
		{
			if (Buffer)
			{
				MAX_LIMIT(Size, PAGE_SIZE);

				memcpy(pPage->PhysAddr, Buffer, Size);
				if (Size < PAGE_SIZE)
					memset(&pPage->PhysAddr[Size], 0, PAGE_SIZE - Size);
			} else
			{
				memset(pPage->PhysAddr, 0, PAGE_SIZE);
			}
		}
		return true;
	}

	bool CPageMemory::AllocatePage(unsigned long Address, unsigned long Attribute, unsigned char PageData)
	{
		unsigned long DirIndex = (Address >> 22);
		unsigned long PageIndex = (Address >> 12) & 1023;

		if (!m_PageDir[DirIndex])
		{
			m_PageDir[DirIndex] = new MEMORY_PAGE[1024];
			memset(m_PageDir[DirIndex], 0, sizeof(MEMORY_PAGE)*1024);
		}

		MEMORY_PAGE *pPage = &m_PageDir[DirIndex][PageIndex];

		if (pPage->Attribute)
			return false;

		memset(pPage, 0, sizeof(MEMORY_PAGE));
		pPage->Attribute = Attribute | 0x20;
		pPage->PageData = PageData;
		return true;
	}


	bool CPageMemory::ReleasePage(unsigned long Address)
	{
		unsigned long DirIndex = (Address >> 22);
		unsigned long PageIndex = (Address >> 12) & 1023;

		MEMORY_PAGE *pPage = &m_PageDir[DirIndex][PageIndex];

		if (!m_PageDir[DirIndex] || !pPage->PhysAddr)
			return false;

		DeletePage(pPage);
		memset(pPage, 0, sizeof(MEMORY_PAGE));
		return true;
	}

	unsigned long CPageMemory::ReleaseClearPage()
	{
		int Count = 0;
		for (int n = 0; n < 1024; ++n)
		{
			if (m_PageDir[n])
			{
				for (int m = 0; m < 1024; ++m)
				{
					MEMORY_PAGE *pPage = &m_PageDir[n][m];
					if ( !(pPage->Attribute & 1) && !(pPage->Attribute & 0x8000) && pPage->PhysAddr)
					{
						DeletePage(pPage);
						if (pPage->Attribute & 0x10)
							pPage->PhysAddr = 0;
						else	memset(pPage, 0, sizeof(MEMORY_PAGE));
						++Count;
						if (m_PageUsedCount <= 0)
							return Count;
					}
				}
			}
		}
		return Count;
	}

	bool CPageMemory::IsPageExist(unsigned long Address)
	{
		unsigned long DirIndex = (Address >> 22);
		unsigned long PageIndex = (Address >> 12) & 1023;

		MEMORY_PAGE *pPage = m_PageDir[DirIndex];
		return pPage && pPage[PageIndex].Attribute;
	}

	bool CPageMemory::IsPageDirExist(unsigned long Address)
	{
		unsigned long DirIndex = Address >> 22;
		return m_PageDir[DirIndex] != 0;
	}

	MEMORY_PAGE *CPageMemory::GetDataPage(unsigned long Address, bool bWrite)
	{
		Address &= 0xFFFFF000;

		if (m_PageDataCache1.bExist && Address == m_PageDataCache1.BaseAddr)
		{
			++m_PageDataCache1.AccessCount;
			return m_PageDataCache1.MemoryPage;
		}
		if (m_PageDataCache2.bExist && Address == m_PageDataCache2.BaseAddr)
		{
			++m_PageDataCache2.AccessCount;
			return m_PageDataCache2.MemoryPage;
		}

		unsigned long DirIndex = (Address >> 22);
		unsigned long PageIndex = (Address >> 12) & 1023;

		MEMORY_PAGE *pPage = &m_PageDir[DirIndex][PageIndex];

		if (!m_PageDir[DirIndex] || !pPage->Attribute)	//!IsPageExist(Address)
			return NULL;

		if (pPage->Attribute & 0x4000)
		{
			if (!NewPage(pPage))
				return NULL;
		} else
		if (pPage->Attribute & 0x10 && !pPage->PhysAddr && !LoadPage(Address, pPage))
		{
			return NULL;
		}

		if (m_PageDataCache1.AccessCount <= m_PageDataCache2.AccessCount)
		{
			m_PageDataCache1.bExist = true;
			m_PageDataCache1.BaseAddr = Address;
			m_PageDataCache1.AccessCount = 0;
			m_PageDataCache1.MemoryPage = pPage;
		} else
		{
			m_PageDataCache2.bExist = true;
			m_PageDataCache2.BaseAddr = Address;
			m_PageDataCache2.AccessCount = 0;
			m_PageDataCache2.MemoryPage = pPage;
		}
		return pPage;
	}

	bool CPageMemory::LoadPage(unsigned long Address, MEMORY_PAGE *pPage)
	{
		if (m_PageUsedCount >= m_PageCount && !ExchangePage())
			return false;

		map<CMemoryImage, CPageImage*>::IT it = m_ImageList.find(CMemoryImage(Address));
		if (it == m_ImageList.end())
			return false;

		if (!NewPage(pPage))
			return false;

		if (it->second->LoadPage(Address, pPage->PhysAddr))
			return true;

		DeletePage(pPage);
		memset(pPage, 0, sizeof(MEMORY_PAGE));
		return false;
	}

	bool CPageMemory::SavePage(unsigned long Address, MEMORY_PAGE *pPage)
	{
		if ( !(pPage->Attribute & 0x10) || !(m_Style & 1) || !pPage->PhysAddr)
			return false;

		map<CMemoryImage, CPageImage*>::IT it = m_ImageList.find(CMemoryImage(Address));
		if (it == m_ImageList.end())
			return false;

		if (!it->second->SavePage(Address, pPage->PhysAddr))
			return false;

		DeletePage(pPage);
		pPage->Attribute &= ~1;
		return true;
	}


	unsigned long CPageMemory::ReadMemX(unsigned long Address, void *Buffer, unsigned long Size)
	{
		unsigned long RAddr = Address;
		unsigned long RSize = MIN(Size,(PAGE_SIZE - Address % PAGE_SIZE));
		while (Size)
		{
			MEMORY_PAGE *pPage = GetDataPage(RAddr, 0);
			if (!pPage)
				return RAddr - Address;

			if ( !(pPage->Attribute & 0x20))
				memcpy((char *)Buffer + RAddr - Address, &pPage->PhysAddr[RAddr & 0xFFF], RSize);
			else	memset((char *)Buffer + RAddr - Address, pPage->PageData & 0xff, RSize);
			Size -= RSize;
			RAddr += RSize;
			RSize = MIN(Size,PAGE_SIZE);
		}
		return RAddr - Address;

	}

	unsigned long CPageMemory::WriteMemX(unsigned long Address, const void *Buffer, unsigned long Size)
	{
		unsigned long WAddr = Address;
		unsigned long WSize = MIN(Size,(PAGE_SIZE - Address % PAGE_SIZE));
		while (Size)
		{
			MEMORY_PAGE *pPage = GetDataPage(WAddr, 1);
			if (!pPage)
				return WAddr - Address;

			if ( !(pPage->Attribute & 0x20))
				memcpy(&pPage->PhysAddr[WAddr & 0xFFF], (char *)Buffer + WAddr - Address, WSize);
			pPage->Attribute |= 1;
			Size -= WSize;
			WAddr += WSize;
			WSize = MIN(Size,PAGE_SIZE);
		}
		return WAddr - Address;
	}

	unsigned long CPageMemory::ZeroMemX(unsigned long Address, unsigned long Size)
	{
		unsigned long WAddr = Address;
		unsigned long WSize = MIN(Size,(PAGE_SIZE - Address % PAGE_SIZE));
		while (Size)
		{
			MEMORY_PAGE *pPage = GetDataPage(WAddr, 1);
			if (!pPage)
				return WAddr - Address;

			if ( !(pPage->Attribute & 0x20))
				memset(&pPage->PhysAddr[WAddr & 0xFFF], 0, WSize);
			pPage->Attribute |= 1;
			Size -= WSize;
			WAddr += WSize;
			WSize = MIN(Size,PAGE_SIZE);
		}
		return WAddr - Address;
	}

	unsigned long CPageMemory::ReadMemory(unsigned long Address, void*Buffer, unsigned long Size, CPageMemory*pPageMemory)
	{
		return pPageMemory->ReadMemX(Address, Buffer, Size);
	}

	unsigned long CPageMemory::WriteMemory(unsigned long Address, const void*Buffer, unsigned long Size, CPageMemory*pPageMemory)
	{
		return pPageMemory->WriteMemX(Address, Buffer, Size);
	}

	unsigned long CPageMemory::ReadString(unsigned long Address, char *Buffer, unsigned long Size)
	{
		unsigned char Byte;
		unsigned int i;

		for (i = 0; i < Size-1 && ReadMemB(Address+i, &Byte); ++i)
		{
			if (Buffer)
				Buffer[i] = Byte;
			if (!Byte)
				return i;
		}
		if (Buffer)
			Buffer[i] = 0;
		return i;
	}
	unsigned long CPageMemory::WriteString(unsigned long Address, const char *Buffer, unsigned long Size)
	{
		unsigned int i;
		static const unsigned char Null = 0;

		for (i = 0; i < Size && WriteMemB(Address+i, &Buffer[i]); ++i)
		{
			if (!Buffer[i])
				return i-1;
		}
		if (i >= Size)
			WriteMemB(Address+i-1, &Null);
		return i - 1;
	}

	unsigned long CPageMemory::ReadString(unsigned long Address, unsigned short *Buffer, unsigned long Size)
	{
		unsigned short Word;
		unsigned int i;

		for (i = 0; i < Size-1 && ReadMemW(Address+2*i, &Word); ++i)
		{
			if (Buffer)
				Buffer[i] = Word;
			if (!Word)
				return i;
		}
		if (Buffer)
			Buffer[i] = 0;
		return i;
	}
	unsigned long CPageMemory::WriteString(unsigned long Address, const unsigned short *Buffer, unsigned long Size)
	{
		unsigned int i;
		static const unsigned short Null = 0;

		for (i = 0; i < Size && WriteMemW(Address+2*i, &Buffer[i]); ++i)
		{
			if (!Buffer[i])
				return i-1;
		}
		if (i >= Size)
			WriteMemW(Address+2*(i-1), &Null);
		return i - 1;
	}

	unsigned long CPageMemory::ReadString(unsigned long Address, unsigned long *Buffer, unsigned long Size)
	{
		unsigned long DWord;
		unsigned int i;

		for (i = 0; i < Size-1 && ReadMemD(Address+4*i, &DWord); ++i)
		{
			if (Buffer)
				Buffer[i] = DWord;
			if (!DWord)
				return i;
		}
		if (Buffer)
			Buffer[i] = 0;
		return i;

	}

	unsigned long CPageMemory::WriteString(unsigned long Address, const unsigned long *Buffer, unsigned long Size)
	{
		unsigned int i;
		static const unsigned long Null = 0;

		for (i = 0; i < Size && WriteMemD(Address+4*i, &Buffer[i]); ++i)
		{
			if (!Buffer[i])
				return i-1;
		}
		if (i >= Size)
			WriteMemD(Address+4*(i-1), &Null);
		return i-1;
	}

	bool CPageMemory::ReadMemB(unsigned long Address, void *Buffer)
	{
		MEMORY_PAGE *pPage = GetDataPage(Address, false);
		if (!pPage)
			return false;

		if (pPage->Attribute & 0x20)
			*(char*)Buffer = pPage->PageData;
		else
			*(char*)Buffer = pPage->PhysAddr[Address & 0xFFF];
		++pPage->AccessReadCount;
		return true;
	}

	bool CPageMemory::ReadMemW(unsigned long Address, void *Buffer)
	{
		MEMORY_PAGE *pPage = GetDataPage(Address, false);
		if ( (Address & 0xFFF) > 0xFFE)
			return ReadMemB(Address, Buffer) && ReadMemB(Address + 1, (char *)Buffer + 1);

		if (!pPage)
			return false;

		if (pPage->Attribute & 0x20)
		{
			((char*)Buffer)[0] = pPage->PageData;
			((char*)Buffer)[1] = pPage->PageData;
		} else
		{
			((short*)Buffer)[0] = *(short*)&pPage->PhysAddr[Address & 0xFFF];
		}
		++pPage->AccessReadCount;
		return true;
	}

	bool CPageMemory::ReadMemD(unsigned long Address, void *Buffer)
	{
		MEMORY_PAGE *pPage = GetDataPage(Address, false);
		if ( (Address & 0x1000) > 0xFFC)
			return ReadMemW(Address, Buffer) && ReadMemW(Address+2, (char *)Buffer+2);

		if (!pPage)
			return false;

		if (pPage->Attribute & 0x20)
		{
			((char*)Buffer)[0] = pPage->PageData;
			((char*)Buffer)[1] = pPage->PageData;
			((char*)Buffer)[2] = pPage->PageData;
			((char*)Buffer)[4] = pPage->PageData;
		} else
		{
			((long*)Buffer)[0] = *(long*)&pPage->PhysAddr[Address & 0xFFF];
		}
		++pPage->AccessReadCount;
		return true;
	}

	bool CPageMemory::WriteMemB(unsigned long Address, const void *Buffer)
	{
		MEMORY_PAGE *pPage = GetDataPage(Address, true);
		if (!pPage)
			return false;

		if ( !(pPage->Attribute & 0x20))
			pPage->PhysAddr[Address & 0xFFF] = *(char*)Buffer;

		if ( !(pPage->Attribute & 1))
		{
			++m_WriteAccessPageCount;
			pPage->Attribute |= 1;
		}
		++pPage->AccessWriteCount;
		return true;
	}

	bool CPageMemory::WriteMemW(unsigned long Address, const void *Buffer)
	{
		MEMORY_PAGE *pPage = GetDataPage(Address, true);
		if ( (Address & 0xFFF) > 0xFFE)
			return WriteMemB(Address, Buffer) && WriteMemB(Address+1, (char*)Buffer+1);

		if (!pPage)
			return false;

		if ( !(pPage->Attribute & 0x20))
			*(short*)&pPage->PhysAddr[Address & 0xFFF] = *(short*)Buffer;

		if ( !(pPage->Attribute & 1))
		{
			++m_WriteAccessPageCount;
			pPage->Attribute |= 1;
		}
		++pPage->AccessWriteCount;
		return true;
	}

	bool CPageMemory::WriteMemD(unsigned long Address, const void *Buffer)
	{
		MEMORY_PAGE *pPage = GetDataPage(Address, true);
		if ( (Address & 0xFFF) > 0xFFC)
			return WriteMemW(Address, Buffer) && WriteMemW(Address+2, (char*)Buffer+2);

		if (!pPage)
			return false;

		if ( !(pPage->Attribute & 0x20))
			*(long*)&pPage->PhysAddr[Address & 0xFFF] = *(long*)Buffer;

		if ( !(pPage->Attribute & 1))
		{
			++m_WriteAccessPageCount;
			pPage->Attribute |= 1;
		}
		++pPage->AccessWriteCount;
		return true;
	}


	bool CPageMemory::NewPage(MEMORY_PAGE *pPage)
	{
		if (m_Style & 2)
		{
			pPage->PhysAddr = new unsigned char[PAGE_SIZE];
			memset(pPage->PhysAddr, 0, PAGE_SIZE);
		} else
		{
			if (m_FreePhysIndex >= m_PageCount)
				return false;

			pPage->PhysAddr = &m_PhysMemory[PAGE_SIZE * m_FreePhysIndex];
			m_PageExistMap[m_FreePhysIndex] = 1;
			while (m_FreePhysIndex < m_PageCount && m_PageExistMap[m_FreePhysIndex])
				++m_FreePhysIndex;
		}
		pPage->Attribute &= ~0x4000;
		++m_PageUsedCount;
		return true;
	}

	bool CPageMemory::DeletePage(MEMORY_PAGE *pPage)
	{
		if ( !(pPage->Attribute & 0x4000) && pPage->PhysAddr)
		{
			if (m_Style & 2)
			{
				delete []pPage->PhysAddr;
				pPage->PhysAddr = NULL;
			} else
			{
				int PhysIndex = (pPage->PhysAddr - m_PhysMemory) / PAGE_SIZE; //???
				m_PageExistMap[PhysIndex] = 0;
				pPage->PhysAddr = NULL;
				MAX_LIMIT(m_FreePhysIndex, PhysIndex);
			}
			--m_PageUsedCount;
		}
		RefreshCache(pPage);
		return true;
	}


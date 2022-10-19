
#include <string.h>

#include "imagefile.hpp"
#include "compress.hpp"

	CImageFile::CImageFile()
		: m_hHandle(0)
		, m_EntryPoint(0)
		, m_FileType(1)
		, m_FileObjType(0)
		, m_PageAttribute(0)
		, m_FileSize(0)
		, m_BaseOffset(0)
		, m_FilePointer(0)
		, m_Buffer(nullptr)
		, m_pFileIO(gpFileIO)
	{
		m_ReadMemory = CImageFile::ReadMemory;
		m_RMObj = this;
		m_WriteMemory = CImageFile::WriteMemory;
		m_WRObj = this;
	}

	void CImageFile::InitParam()
	{
		m_PageAttribute = 0x16; //??? PAGE_MAPPED|PAGE_WRITE|PAGE_READ
		m_EntryPoint = 0;
		m_BaseOffset = 0;
		m_ImageBase = 0;
		m_ImageSize = m_FileSize;
		m_ImageHighBase = m_ImageBase + m_ImageSize;
		m_FileType = 1;
		m_FilePointer = 0;
	}

	bool CImageFile::Create(const char *FileName, unsigned long Size)
	{
		if (!m_pFileIO) m_pFileIO = gpFileIO;

		if (!m_pFileIO->CreateFile(FileName, &m_hHandle, 0x10001)) //FILE_MODE_RW|FILE_MODE_SHR
			return false;

		m_FileName = FileName;

		m_FileObjType = FILE_OBJ_FILE;
		SetFileLength(Size);
		GetFileLength();
		InitParam();
		return true;
	}

	bool CImageFile::Create(unsigned long Size, unsigned char *Buffer, bool bCompress)
	{
		if (Buffer && !bCompress)
		{
			m_Buffer = Buffer;
			m_FileObjType |= FILE_OBJ_BUFFER;
		} else
		{
			m_Buffer = new unsigned char[Size];
			m_FileObjType |= FILE_OBJ_COMPRESS;
		}

		if (bCompress)
		{
			if (!Buffer || !Size)
				return false;

			if (lzo_decompress(Buffer, Size, m_Buffer) != Size)
			{
				delete []m_Buffer;
				m_FileObjType &= ~FILE_OBJ_COMPRESS;
				return false;
			}
		}

		m_FileSize = Size;
		m_FilePointer = 0;
		InitParam();
		return true;
	}

	bool CImageFile::Open(const char *FileName, HANDLE HandleOpen)
	{
		if (!m_pFileIO) m_pFileIO = gpFileIO;

		if (FileName)
		{
			m_FileObjType = 0;
			if (!m_pFileIO->OpenFile(FileName, &m_hHandle, 0x10000)) //FILE_MODE_RO|FILE_MODE_SHR
				return false;

			m_FileName = FileName;
		} else
		{
			m_FileObjType |= FILE_OBJ_FILE;

			if (m_FileObjType & 0x6000000) //??? FILE_OBJ_BUFFER | FILE_OBJ_MEMORY
				return true;

			m_hHandle = HandleOpen;
			m_FileName.Empty();
			m_FileObjType |= FILE_OBJ_HANDLE;
		}

		m_FilePointer = 0;
		m_FileSize = m_pFileIO->GetFileLength(m_hHandle);
		InitParam();
		return true;
	}

	void CImageFile::Close()
	{
		m_FileName.Empty();

		if ((m_FileObjType & FILE_OBJ_COMPRESS) && m_Buffer)
		{
			delete []m_Buffer;
			m_Buffer = 0;
		}

		if ( !(m_FileObjType & FILE_OBJ_HANDLE) )
		{
			m_pFileIO->CloseHandle(m_hHandle);
			m_hHandle = 0;
		}

		m_ImageBase = 0;
		m_ImageHighBase = 0;
		m_ImageSize = 0;
		m_FilePointer = 0;

		if (m_FileObjType & FILE_OBJ_DESTROY)
		{
			delete this;
		} else
		{
			m_FileObjType = 0;
		}
	}

	void CImageFile::ChangeFileIO(CFileIO *pFileIO)
	{
		m_pFileIO = pFileIO;
	}

	bool CImageFile::ReadFile(unsigned long Pos, void *Buffer, unsigned long Size)
	{
		if (m_Buffer)
		{
			if (Pos < m_FileSize)
			{
				unsigned long RDSize_ = MIN(m_FileSize - Pos, Size);
				memcpy(Buffer, &m_Buffer[m_BaseOffset + Pos], RDSize_);
				m_FilePointer = RDSize_ + Pos;
				return true;
			}
			return false;
		}
		
		if (!m_FileSize)
			GetFileLength(); //???

		if (Pos > m_FileSize)
			return false;
		
		unsigned long RDSize = m_pFileIO->ReadFile(m_hHandle, Buffer, Size, m_BaseOffset + Pos);
		m_FilePointer = RDSize + m_BaseOffset + Pos;
		return RDSize == Size;
	}

	bool CImageFile::WriteFile(unsigned long Pos, const void *Buffer, unsigned long Size)
	{
		if (m_Buffer)
		{
			if (Pos < m_FileSize)
			{
				unsigned long WRSize_ = MIN(m_FileSize - Pos, Size);
				memcpy(&m_Buffer[m_BaseOffset + Pos], Buffer, WRSize_);
				m_FilePointer = WRSize_ + Pos;
				return true;
			}
			return false;
		}

		if ( !(m_FileObjType & FILE_OBJ_FILE) )
		{
			m_pFileIO->CloseHandle(m_hHandle);

			if (!m_pFileIO->OpenFile(m_FileName, &m_hHandle, 0x10001)) //FILE_MODE_RW|FILE_MODE_SHR
				return false;

			m_FileObjType |= FILE_OBJ_FILE;
		}

		unsigned long WRSize = m_pFileIO->WriteFile(m_hHandle, Buffer, Size, m_BaseOffset + Pos);
		m_FilePointer = WRSize + m_BaseOffset + Pos;
		MIN_LIMIT(m_FileSize, m_FilePointer);
		return WRSize == Size;
	}

	bool CImageFile::ReadFile(void *Buffer, unsigned long Size)
	{
		if (m_Buffer)
		{
			if (m_FilePointer < m_FileSize)
			{
				unsigned long RDSize_ = MIN(m_FileSize - m_FilePointer, Size);
				memcpy(Buffer, &m_Buffer[m_FilePointer], RDSize_);
				m_FilePointer += RDSize_;
				return true;
			}
			return false;
		}

		unsigned long RDSize = m_pFileIO->ReadFile(m_hHandle, Buffer, Size, m_FilePointer);
		m_FilePointer += RDSize;
		return RDSize == Size;
	}

	bool CImageFile::WriteFile(const void *Buffer, unsigned long Size)
	{
		if (m_Buffer)
		{
			if (m_FilePointer < m_FileSize)
			{
				unsigned long WRSize_ = MIN(m_FileSize - m_FilePointer, Size);
				memcpy(&m_Buffer[m_FilePointer], Buffer, WRSize_);
				m_FilePointer += WRSize_;
				return true;
			}
			return false;
		}
		
		unsigned long WRSize = m_pFileIO->WriteFile(m_hHandle, Buffer, Size, m_FilePointer);
		m_FilePointer += WRSize;
		MIN_LIMIT(m_FileSize, m_FilePointer);
		return WRSize == Size;
	}

	bool CImageFile::DeleteMemory(unsigned long Pos, unsigned long Size)
	{
		if (Size + Pos > m_FileSize)
			return false;

		unsigned long DeleteSize = m_FileSize - Pos - Size;

		if (DeleteSize > 0x100000)
		{
			char *pBuffer = new char[0x100000];
			if (!pBuffer)
				return false;

			unsigned long BlockSize = 0;
			unsigned long BlockCount = PAGE_COUNT(DeleteSize,0x100000);

			for (int i = 1; i < BlockCount; ++i)
			{
				ReadFile(Pos+Size+BlockSize, pBuffer, 0x100000);
				WriteFile(Pos+BlockSize, pBuffer, 0x100000);
				BlockSize += 0x100000;
			}

			ReadFile(Pos+Size+BlockSize, pBuffer, DeleteSize % 0x100000);
			WriteFile(Pos+BlockSize, pBuffer, DeleteSize % 0x100000);
			SetFileLength(Pos+DeleteSize);
			delete []pBuffer;
		} else
		{
			char *pBuffer = new char[DeleteSize];
			if (!pBuffer)
				return false;

			ReadFile(Pos+Size, pBuffer, DeleteSize);
			WriteFile(Pos, pBuffer, DeleteSize);
			SetFileLength(Pos+DeleteSize);
			delete []pBuffer;
		}
		return true;
	}

	bool CImageFile::DeleteFile()
	{
		return m_pFileIO->DeleteFile(m_FileName);
	}

	unsigned long CImageFile::ReadString(ULONG_PTR Address, char *Buffer, unsigned long MaxLen, unsigned long AlignLen)
	{
		if (!MaxLen)
			return 0;

		unsigned int Offset = 0;
		while (Offset < MaxLen)
		{
			unsigned int Len = MIN(MaxLen - Offset, AlignLen);

			ReadMemory(Address + 1 * Offset, &Buffer[Offset], 1 * Len);

			for (unsigned int n = 0; n < Len; ++n)
			{
				if (!Buffer[n + Offset])
					return n + Offset;
			}
			Offset += Len;
		}

		Buffer[MaxLen - 1] = 0;
		return MaxLen - 1;
	}

	unsigned long CImageFile::ReadString(ULONG_PTR Address, unsigned short *Buffer, unsigned long MaxLen, unsigned long AlignLen)
	{
		if (!MaxLen)
			return 0;

		unsigned int Offset = 0;
		while (Offset < MaxLen)
		{
			unsigned int Len = MIN(MaxLen - Offset, AlignLen);

			ReadMemory(Address + 2 * Offset, &Buffer[Offset], 2 * Len);

			for (unsigned int n = 0; n < Len; ++n)
			{
				if (!Buffer[n + Offset])
					return n + Offset;
			}
			Offset += Len;
		}

		Buffer[MaxLen - 1] = 0;
		return MaxLen - 1;
	}

	unsigned long CImageFile::ReadString(ULONG_PTR Address, unsigned long *Buffer, unsigned long MaxLen, unsigned long AlignLen)
	{
		if (!MaxLen)
			return 0;

		unsigned int Offset = 0;
		while (Offset < MaxLen)
		{
			unsigned int Len = MIN(MaxLen - Offset, AlignLen);

			ReadMemory(Address + 4 * Offset, &Buffer[Offset], 4 * Len);

			for (unsigned int n = 0; n < Len; ++n)
			{
				if (!Buffer[n + Offset])
					return n + Offset;
			}
			Offset += Len;
		}

		Buffer[MaxLen - 1] = 0;
		return MaxLen - 1;
	}

	unsigned long CImageFile::ReadString(ULONG_PTR Address, unsigned long long *Buffer, unsigned long MaxLen, unsigned long AlignLen)
	{
		if (!MaxLen)
			return 0;

		unsigned int Offset = 0;
		while (Offset < MaxLen)
		{
			unsigned int Len = MIN(MaxLen - Offset, AlignLen);

			ReadMemory(Address + 8 * Offset, &Buffer[Offset], 8 * Len);

			for (unsigned int n = 0; n < Len; ++n)
			{
				if (!Buffer[n + Offset])
					return n + Offset;
			}
			Offset += Len;
		}

		Buffer[MaxLen - 1] = 0;
		return MaxLen - 1;
	}

	unsigned long CImageFile::GetFileLength()
	{
		if (!m_Buffer)
			m_FileSize = m_pFileIO->GetFileLength(m_hHandle);
		return m_FileSize;
	}

	bool CImageFile::SetFileLength(unsigned long Length)
	{
		if (m_Buffer)
			return false;

		if ( !(m_FileObjType & FILE_OBJ_FILE) )
		{
			m_pFileIO->CloseHandle(m_hHandle);

			if (!m_pFileIO->OpenFile(m_FileName, &m_hHandle, 0x10001)) //FILE_MODE_RW|FILE_MODE_SHR
				return false;

			m_FileObjType |= FILE_OBJ_FILE;
		}

		if (!m_pFileIO->SetFileLength(m_hHandle, Length))
			return false;

		m_FileSize = m_pFileIO->GetFileLength(m_hHandle);
		return true;
	}

	unsigned long CImageFile::ReadImageMemory(ULONG_PTR Address, void *Buffer, unsigned long Size)
	{
	//	if (Address < m_ImageBase || Address >= m_ImageHighBase)
		if (OutOfBaseRange(Address))
			return 0;

		//if (Address < m_ImageHighBase && Size + Address > m_ImageHighBase)
		//	Size = m_ImageHighBase - Address;
		MAX_LIMIT(Size, m_ImageHighBase - Address);

		ReadFile(Address - m_ImageBase, Buffer, Size);
		return Size;
	}

	unsigned long CImageFile::WriteImageMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size)
	{
	//	if (Address < m_ImageBase || Address >= m_ImageHighBase)
		if (OutOfBaseRange(Address))
			return 0;

		//if (Address < m_ImageHighBase && Size + Address > m_ImageHighBase)
		//	Size = m_ImageHighBase - Address;
		MAX_LIMIT(Size, m_ImageHighBase - Address);

		WriteFile(Address - m_ImageBase, Buffer, Size);
		return Size;
	}

	bool CImageFile::LoadPage(ULONG_PTR Address, void *Buffer)
	{
		memset(Buffer, 0, PAGE_SIZE);
		ReadMemory(Address - m_ImageBase, Buffer, PAGE_SIZE);
		return true;

	}

	bool CImageFile::SavePage(ULONG_PTR Address, const void *Buffer)
	{
		return WriteFile(Address - m_ImageBase, Buffer, PAGE_SIZE);
	}

	unsigned long CImageFile::GetPageAttribute(ULONG_PTR Address)
	{
		return m_PageAttribute;
	}

	void CImageFile::RelocLoad(ULONG_PTR BaseAddr)
	{
		ULONG_PTR Delta = BaseAddr - m_ImageBase;
		m_ImageBase = BaseAddr;
		m_ImageHighBase += Delta;
		m_EntryPoint += Delta;
	}

	void CImageFile::SetRelocRead(unsigned long (*RMFunc)(ULONG_PTR, void*, unsigned long, void*), void *RMObj)
	{
		m_ReadMemory = RMFunc;
		m_RMObj = RMObj;
	}

	void CImageFile::SetRelocWrite(unsigned long (*WRFunc)(ULONG_PTR, const void*, unsigned long, void*), void *WRObj)
	{
		m_WriteMemory = WRFunc;
		m_WRObj = WRObj;
	}

	unsigned long CImageFile::ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size)
	{
		if (m_ReadMemory) return m_ReadMemory(Address, Buffer, Size, m_RMObj);
		return 0;
	}

	unsigned long CImageFile::WriteMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size)
	{
		if (m_WriteMemory) return m_WriteMemory(Address, Buffer, Size, m_WRObj);
		return 0;
	}

	unsigned long CImageFile::ReadMemory(ULONG_PTR Address, void *Buffer, unsigned long Size, void *RMObj)
	{
		CImageFile *pImageFile = (CImageFile *)RMObj;
		return pImageFile->ReadImageMemory(Address, Buffer, Size);
	}

	unsigned long CImageFile::WriteMemory(ULONG_PTR Address, const void *Buffer, unsigned long Size, void *WRObj)
	{
		CImageFile *pImageFile = (CImageFile *)WRObj;
		return pImageFile->WriteImageMemory(Address, Buffer, Size);
	}

	bool CImageFile::OutOfBaseRange(ULONG_PTR BaseAddr)
	{
		//return ???.!InBaseRange(BaseAddr);
		return BaseAddr < m_ImageBase || BaseAddr >= m_ImageHighBase;
	}

//
	unsigned long CImageFileStream::Write(const void *Buffer, unsigned long Size)
	{
		return CImageFile::WriteFile(Buffer, Size) != 0 ? Size:0;
	}

	unsigned long CImageFileStream::Read(void *Buffer, unsigned long Size)
	{
		return CImageFile::ReadFile(Buffer, Size) != 0 ? Size:0;
	}	

